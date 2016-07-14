/////////////////////////////////////////////////////////////////////////////////
// ReverseChordFinderMode.cpp
//
// Contains methods defined by the ReverseChordFinderMode class.  These methods
// manage all of the states of the Reverse Chord Finder mode.
//
// History:
// - jmcorbett 23-Dec-2015 Original creation.
//
// Copyright (C) 2015-2016 Joseph M. Corbett
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <http://www.gnu.org/licenses>.
//
/////////////////////////////////////////////////////////////////////////////////

#include "ReverseChordFinderMode.h" // For ReverseChordFinderMode class.
#include "Display.h"                // For Display class.
#include "LedDriver.h"              // For NUM_FRETS, NUM_STRINGS.
#include "ModeManager.h"            // For AGAIN.
#include "EepromConfig.h"           // For EepromConfig class.


// Return a pointer to our singleton instance.
ReverseChordFinderMode *ReverseChordFinderMode::Instance()
{
    static ReverseChordFinderMode instance;
    return &instance;
}

// Get the Reverse Chord Finer mode ready to run.
void ReverseChordFinderMode::Startup()
{
    // Cache a pointer to the display instance.
    Display *pDisplay = Display::Instance();
    
    // Setup for the first state and initialize our instance daata.
    m_CurrentState  = GETTING_STRING_MATCH_PATTERN_STATE;
    m_CurrentString = 0;
    m_CurrentFret   = 0;
    m_CurrentValue  = 0;
    m_MatchPattern = (1 << LedDriver::NUM_STRINGS) - 1;
    memset(&m_Chord, 0, sizeof(Chord));
    
    // Reset our chord data to the first chord.
    m_ChordData.GetChord(0, 0, 0);
    
    // Clear all LEDs.
    pDisplay->SetAllLeds(false);
    
    // Display the initial match pattern on the fingerboard.
    uint8_t *pPattern = m_Chord.GetPatternPtr();
    pPattern[0] = m_MatchPattern;
    pDisplay->DisplayLeds(&m_Chord);
    
    pDisplay->DispLcdProgmem(F("Strings to Match"), true, 0, 0);
}


// Execute the reverse chord finder based on our current state.
uint32_t ReverseChordFinderMode::Execute(uint32_t irKey)
{
    // We'll start over if our mode key is entered again while in any state.
    if (irKey == ModeManager::AGAIN)
    {
        Startup();
        m_CurrentState = GETTING_STRING_MATCH_PATTERN_STATE;
        irKey = 0;
    }

    // Run the current state.
    switch (m_CurrentState)
    {
    case GETTING_STRING_MATCH_PATTERN_STATE:
        irKey = HandleGettingStringsMatchPatternState(irKey);
        break;
            
    case GETTING_FRET_STATE:
        irKey = HandleGettingFretState(irKey);
        break;
        
    case GETTING_PATTERN_STATE:
        irKey = HandleGettingPatternState(irKey);
        break;
        
    case WAITING_DONE_STATE:
        irKey = HandleWaitingDoneState(irKey);
        break;
        
     default:
        // Somrthing went wrong somewhere.  Setup to start this mode over.
        Startup();
        m_CurrentState = GETTING_FRET_STATE;
        break;
    }
    
    // Return the passed in key if we haven't used it.
    return irKey;
}

// Set/clear the LED corresponding to the current selection.
void ReverseChordFinderMode::UpdateCurrentPattern()
{
    // Cache a pointer to the LED pattern array.
    uint8_t *pPattern = m_Chord.GetPatternPtr();
    
    // Setup the bit pattern of the current string.
    uint8_t stringBp = (1 << m_CurrentString);
    
    // Set or clear the currently selected LED based on its current value.
    if (m_CurrentValue)
    {
        // Current LED value is "on" so set the corresponding bit.
        pPattern[m_CurrentFret] |= stringBp;
        
        // Insure that only one fret per string is set.
        for (unsigned fret = 0; fret < LedDriver::NUM_FRETS; fret++)
        {
            if (fret != m_CurrentFret)
            {
                pPattern[fret] &= ~stringBp;
            }
        }
    }
    else
    {
        // Current LED value is "off" so clear the corresponding bit.
        pPattern[m_CurrentFret] &= ~(1 << m_CurrentString);
    }
}

// Display the current base fret value.
void ReverseChordFinderMode::UpdateBaseFretDisplay()
{
    // Cache a pointer to the display instance,
    Display *pDisplay = Display::Instance();
    
    // A base fret of 0 indicates that any fret will match.
    if (m_Chord.GetFret() == 0)
    {
        pDisplay->DispLcdProgmem(F("ANY"), false, 0, 11);
    }
    else
    {
        // Base fret is not 0, display the current value.
        pDisplay->DispLcdProgmem(F("   "), false, 0, 11);
        pDisplay->DispLcdInt(m_Chord.GetFret(), false, 0, 11);
    }
}

// Get the string match pattern
uint32_t ReverseChordFinderMode::HandleGettingStringsMatchPatternState(uint32_t irKey)
{
    // Cache some useful data.
    Display *pDisplay = Display::Instance();
    uint8_t *pPattern = m_Chord.GetPatternPtr();
        
    // Assume that we're going to use the specified IR key.
    uint32_t returnKey = 0;
    
    switch (irKey)
    {
    case PATTERN_RIGHT: // Remove upper strings
        if (m_MatchPattern > 1)
        {
            m_MatchPattern >>= 1;
        }
        pPattern[0] = m_MatchPattern;
        
        // Display the match pattern on the fingerboard.
        pDisplay->DisplayLeds(&m_Chord);
        break;
        
    case PATTERN_LEFT: // Add upper strings.
        if (m_MatchPattern < ((1 << LedDriver::NUM_STRINGS) - 1))
        {
            m_MatchPattern = (m_MatchPattern <<= 1) + 1;
        }
        pPattern[0] = m_MatchPattern;

        // Display the match pattern on the fingerboard.
        pDisplay->DisplayLeds(&m_Chord);
        break;
        
    case SELECT: // Select the current pattern.  Advance to the next state.
        m_CurrentState = GETTING_FRET_STATE;

        // Reset the match pattern on the fingerboard.
        pPattern[0] = 0;
        pDisplay->DisplayLeds(&m_Chord);

        // Display our headline.
        m_Chord.SetFret(0);
        pDisplay->DispLcdProgmem(F("Base Fret: ANY"), true, 0, 0);
        break;
        
    default:
        // We didn't handle the passed in key, so return it unmodified.
        returnKey = irKey;
        break;
    }
    
    return returnKey;
}

// Maintain the current base fret selection based on IR key input.
uint32_t ReverseChordFinderMode::HandleGettingFretState(uint32_t irKey)
{
    // Cache some useful data.
    uint8_t baseFret   = m_Chord.GetFret();
    Display *pDisplay  = Display::Instance();
    uint8_t *pPattern  = m_Chord.GetPatternPtr();
    
    // Assume that we're going to use the specified IR key.
    uint32_t returnKey = 0;
    
    switch (irKey)
    {
    case PREV_FRET: // Advance to the next fret and wrap if needed.
        if (++baseFret > MAX_BASE_FRET)
        {
            baseFret = 0;
        }
        m_Chord.SetFret(baseFret);
        UpdateBaseFretDisplay();
        break;
        
    case NEXT_FRET: // Decrement to the previous fret and wrap if needed.
        if (baseFret-- == 0)
        {
            baseFret = MAX_BASE_FRET;
        }
        m_Chord.SetFret(baseFret);
        UpdateBaseFretDisplay();
        break;
        
    case SELECT: // Select the current fret.  Advance to the next state.
        m_CurrentState = GETTING_PATTERN_STATE;
        pDisplay->DispLcdProgmem(F("F5 => Don't Care"), false, 1, 0);
        m_Delay.Start(LONG_DELAY);
        break;
        
    default:
        // We didn't handle the passed in key, so return it unmodified.
        returnKey = irKey;
        break;
    }
    
    return returnKey;
}

// Get the search chord pattern.
uint32_t ReverseChordFinderMode::HandleGettingPatternState(uint32_t irKey)
{
    // Cache some useful data.
    uint8_t *pPattern = m_Chord.GetPatternPtr();    
    Display *pDisplay = Display::Instance();
    
    // Setup to return 0 if we handle the new IR key, or the passed in value 
    // if we don't.  Assume we'll handle the passed in IR key.
    uint32_t returnedIrKey = 0;
    
    // Handle blinking the currently selected LED as a cursor for the user to use
    // as a reference.  This will blink longer LEDs that have been selected as on
    // then for ones that have been selected as off.  This will hopefully let the
    // user know the current value of the LED.
    if (m_Delay.IsExpired())
    {
        if (m_Delay.GetDelayValue() == LONG_DELAY)
        {
            m_Delay.Start(SHORT_DELAY);
        }
        else
        {
            m_Delay.Start(LONG_DELAY);
        }
        pPattern[m_CurrentFret] ^= (1 << m_CurrentString);
        pDisplay->DisplayLeds(&m_Chord);
    }
    
    // Handle IR key input if any.
    switch (irKey)
    {
    case NEXT_FRET: // The next fret has been selected.
        UpdateCurrentPattern();
        // Bump the fret, and wrap if needed.
        if (++m_CurrentFret >= LedDriver::NUM_FRETS)
        {
            m_CurrentFret = 0;
        }
        // Set the new LED as active and re-start the delay.
        m_CurrentValue = (pPattern[m_CurrentFret] & (1 << m_CurrentString)) ? 1 : 0;
        m_Delay.Start(LONG_DELAY);
        break;
        
    case PREV_FRET: // The previous fret has been selected.
        UpdateCurrentPattern();
        // Bump the fret, and wrap if needed.
        if (0 == m_CurrentFret--)
        {
            m_CurrentFret = LedDriver::NUM_FRETS - 1;
        }
        // Set the new LED as active and re-start the delay.
        m_CurrentValue = (pPattern[m_CurrentFret] & (1 << m_CurrentString)) ? 1 : 0;
        m_Delay.Start(LONG_DELAY);
        break;
        
    case NEXT_STRING: // The next string has been selected.
        UpdateCurrentPattern();
        // Bump the string, and wrap if needed.
        if (++m_CurrentString >= LedDriver::NUM_STRINGS)
        {
            m_CurrentString = 0;
        }
        // Set the new LED as active and re-start the delay.
        m_CurrentValue = (pPattern[m_CurrentFret] & (1 << m_CurrentString)) ? 1 : 0;
        m_Delay.Start(LONG_DELAY);
        break;
        
    case PREV_STRING: // The previous string has been selected.
        UpdateCurrentPattern();
        // Bump the string, and wrap if needed.
        if (0 == m_CurrentString--)
        {
            m_CurrentString = LedDriver::NUM_STRINGS - 1;
        }
        // Set the new LED as active and re-start the delay.
        m_CurrentValue = (pPattern[m_CurrentFret] & (1 << m_CurrentString)) ? 1 : 0;
        m_Delay.Start(LONG_DELAY);
        break;
        
    case TOGGLE: // Toggle the current LED's state.
        m_CurrentValue ^= 1;
        UpdateCurrentPattern();
        m_Delay.Start(LONG_DELAY);
        break;
        
    case SEARCH: // Start the search.
        UpdateCurrentPattern();
        pDisplay->DisplayLeds(&m_Chord);
        
        if (Search())
        {
            // A chord matching the specified pattern was found, display it.
            pDisplay->DisplayChord(&m_ChordData);
        }
        else
        {
            // The specified pattern was not found, let the user know.
            pDisplay->DispLcdProgmem(F("Chord Not Found"), true, 0, 0);
        }
        m_CurrentState = WAITING_DONE_STATE;
        break;
        
    default:
        returnedIrKey = irKey;
        break;
    }
    
    return returnedIrKey;
}

// Wait for the user to acknowledge that he is done, or start a new search.
uint32_t ReverseChordFinderMode::HandleWaitingDoneState(uint32_t irKey)
{
    // Cache some useful data.
    Display *pDisplay       = Display::Instance();
    EepromConfig *pConfig   = EepromConfig::Instance();
    AppConfig *pAppConfig   = &pConfig->m_AppConfig;
    unsigned unplayedOption = pDisplay->GetUnplayedOption();
    bool verboseDisplay     = pDisplay->GetVerbose();
    
    // Setup to return 0 if we handle the new IR key, or the passed in value 
    // if we don't.  Assume we'll handle the passed in IR key.
    uint32_t returnedIrKey = 0;
    
    switch (irKey)
    {
    case SEARCH:
        // Point past last found chord, then search.
        m_ChordData.NextChord();
        if (Search())
        {
            pDisplay->DisplayChord(&m_ChordData);
        }
        else
        {
            pDisplay->DispLcdProgmem(F("Chord Not Found"), true, 0, 0);
        }
        m_CurrentState = WAITING_DONE_STATE;
        break;
        
    case TOGGLE_TYPE_DISPLAY: // Toggle chord type display type.
        verboseDisplay ^= true;
        pAppConfig->m_Verbose = verboseDisplay;
        pDisplay->SetVerbose(pAppConfig->m_Verbose);
        pDisplay->DisplayChord(&m_ChordData);
        break;
            
    case NEXT_UNPLAYED_OPT:
        if (++unplayedOption >= Display::NUM_UNPLAYED_OPTIONS)
        {
            unplayedOption = Display::FIRST_UNPLAYED_OPTION;
        }
        pAppConfig->m_UnplayedOption = unplayedOption;
        pDisplay->SetUnplayedOption(unplayedOption);
        pDisplay->DisplayChord(&m_ChordData);
        break;
            
    default:
        returnedIrKey = irKey;
        break;
    }
    
    return returnedIrKey;
}

// Perform the actual search to try to match the pattern entered by the userl.
bool ReverseChordFinderMode::Search()
{
    // Cache some useful data.
    uint8_t *pPattern   = m_Chord.GetPatternPtr();    
    uint8_t patternMask = ~pPattern[LedDriver::NUM_FRETS - 1];
    uint8_t baseFret    = m_Chord.GetFret();
    uint8_t patternSize = LedDriver::NUM_FRETS - 1;
    
    // Normalize the pattern so that the pattern's first fret is always used.
    while ((*pPattern == 0) && (patternSize != 0))
    {
        pPattern++;
        patternSize--;
        baseFret++;
    }
    // Handle the degenerate case where all frets are open.
    if (patternSize == 0)
    {
        patternSize = 1;
        baseFret    = 1;
        pPattern = m_Chord.GetPatternPtr();
    }
        
    // We haven't found the chord yet.
    bool found = false;
    
    // Loop to try to find the specified pattern.
    do
    {
        // See if the base fret matches the request.
        if ((baseFret != 0) && (baseFret != m_ChordData.GetCurrentFret()))
        {
            // Doesn't match, look for another.
            continue;
        }
        
        // See if the unplayed strings match the request.
        if ((pPattern[LedDriver::NUM_FRETS - 1] & m_MatchPattern) !=
            (m_ChordData.GetChordPtr()->GetUnplayed() & m_MatchPattern))
        {
            // Doesn't match, look for another.
            continue;
        }
        
        // Base fret and unplayed strings match, see if the pattern matches.
        unsigned index = 0;
        for ( ; index < patternSize; index++)
        {
            if ((pPattern[index] & m_MatchPattern) != 
                (m_ChordData.GetChordPatternPtr()[index] & m_MatchPattern))
            {
                // Pattern doesn't match.
                break;
            }
        }
        
        // See if we found a match.
        if (index == patternSize)
        {
            found = true;
        }
    } while (!found && m_ChordData.NextChord());  // Loop till we find one or until
                                                  // we hit the end of the known chords.
                                                  
    // Return the found status.
    return found;
}
