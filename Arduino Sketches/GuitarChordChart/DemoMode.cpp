/////////////////////////////////////////////////////////////////////////////////
// DemoMode.cpp
//
// Contains methods defined by the DemoMode class.  These methods
// manage all of the states of the Demo mode.
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

#include "DemoMode.h"       // For DemoMode class.
#include "Display.h"        // For Display class.
#include "Arduino.h"        // For Arduino specific definitions.
#include "Strings.h"        // For Strings class.
#include "LedDriver.h"      // For LedDriver class.
#include <avr/pgmspace.h>   // For PROGMEM.
#include <EEPROM.h>         // For EEPROM class



static const DemoData DemoDataArray[] PROGMEM = 
{
    {4000, DemoMode::StartupAllOn,  DemoMode::ExecuteAllOnOff,    0}, // ALL_ON
    {4000, DemoMode::StartupAllOff, DemoMode::ExecuteAllOnOff,    0}, // ALL_OFF
    {1000, DemoMode::StartupAllOff, DemoMode::ExecuteWalkLed,     0}, // WALK_LED_ON
    {1000, DemoMode::StartupAllOff, DemoMode::ExecuteWalkLed,     1}, // SWEEP_LED_ON
    {1000, DemoMode::StartupAllOn,  DemoMode::ExecuteWalkLed,     0}, // WALK_LED_OFF
    {1000, DemoMode::StartupAllOn,  DemoMode::ExecuteWalkLed,     1}, // SWEEP_LED_OFF
    {800,  DemoMode::StartupAllOff, DemoMode::ExecuteWalkFret,    0}, // WALK_FRET_ON
    {800,  DemoMode::StartupAllOff, DemoMode::ExecuteWalkFret,    1}, // SWEEP_FRET_ON
    {800,  DemoMode::StartupAllOn,  DemoMode::ExecuteWalkFret,    0}, // WALK_FRET_OFF
    {800,  DemoMode::StartupAllOn,  DemoMode::ExecuteWalkFret,    1}, // SWEEP_FRET_OFF
    {800,  DemoMode::StartupAllOff, DemoMode::ExecuteWalkString,  0}, // WALK_STRING_ON
    {800,  DemoMode::StartupAllOff, DemoMode::ExecuteWalkString,  1}, // SWEEP_STRING_ON
    {800,  DemoMode::StartupAllOn,  DemoMode::ExecuteWalkString,  0}, // WALK_STRING_OFF
    {800,  DemoMode::StartupAllOn,  DemoMode::ExecuteWalkString,  1}, // SWEEP_STRING_OFF
    {1   , DemoMode::StartupAllOff, DemoMode::ExecuteRandomChord, 0}  // RANDOM_CHORD
};

// Return our singleton instance.
DemoMode *DemoMode::Instance()
{
    static DemoMode instance;
    return &instance;
}

// Startup the Demo Mode.
void DemoMode::Startup()
{
    // Cache needed pointer.
    Display  *pDisplay  = Display::Instance();
    
    // Initialize the random number generator upon entry to the Demo mode.
    randomSeed(millis());
    if (m_DisplayOption == RANDOM)
    {
        // Random mode is selected, start with a random demo.
        m_State = random(NUMBER_DEMO_STATES);
    }
    else
    {
        // Not random.  Start with the first state.
        m_State = FIRST_DEMO_STATE;
    }
    // Display the headline.
    pDisplay->DispLcdProgmem(F("DEMO MODE"), true, 0, 3);
    // Initialize the first state.
    StartCurrentState();
}
    
// Execute DemoMode.
uint32_t DemoMode::Execute(uint32_t irKey)
{
    // Cache a useful pointer.
    Display  *pDisplay  = Display::Instance();
    
    // Assume we'll handle this key.  We'll need to return 0.
    uint32_t returnKey = 0;
        
    // Handle the IR key if we know how to.
    switch (irKey)
    {
    case NEXT:
        // Manually increment the state here rather than calling NextState() since
        // we always want to increment the state, even if m_DisplayOption say we
        // shouldn't.
        if (++m_State >= NUMBER_DEMO_STATES)
        {
            m_State = FIRST_DEMO_STATE;
        }    
        StartCurrentState();
        break;
        
    case PREVIOUS:
        // Decrement the current state.
        if (m_State-- == FIRST_DEMO_STATE)
        {
            m_State = NUMBER_DEMO_STATES - 1;
        }
        StartCurrentState();
        break;
        
    default:
        // It's a key we don't understand.  Just return it.
        returnKey = irKey;
        break;
    }

    // Execute the current state.
    (*m_pExecuteState)(m_Argument);
    
    // Return an indication of whether or not we handled this key.
    return returnKey;
}

// Start the selected state by initializing all the pertinent state data.
void DemoMode::StartCurrentState()
{
    // Cache a useful pointer.
    Display  *pDisplay = Display::Instance();
    // We'll use this buffer for displaying the mode string.
    char buf[17];
    // Copy the information about the selected demo from FLASH to ram so we
    // can use it locally.
    DemoData data;
    memcpy_P(&data, &DemoDataArray[m_State], sizeof(DemoData));
    
    // Initialize all state data.
    m_Delay.Start(data.m_StartupDelay);
    m_String = 0;
    m_Fret = 0;
    m_ChordData.GetChord(0, 0, 0);
    m_pExecuteState = data.m_pExecute;
    m_FirstPass = true;
    m_Argument = data.m_Argument;
    m_Count = 0;
    
    // Display the mode and state data.
    pDisplay->DispLcdProgmem(F("DEMO MODE"), true, 0, 3);
    pDisplay->DispLcd(Strings::GetDemoModeString(m_State, buf), false, 1, 0);
    
    // Call the startup method for the current state.
    (*data.m_StartupFunc)();
}


// Startup methods for all demo states.  All states require that the LEDs be 
// either all on or all off.
void DemoMode::StartupAllOn()
{
    Display::Instance()->SetAllLeds(true);
}

void DemoMode::StartupAllOff()
{
    Display::Instance()->SetAllLeds(false);
}


// Execution methods for each demo state.  All On and All Off states simply
// wait for a timeout before switching to the next state.
void DemoMode::ExecuteAllOnOff(unsigned)
{
    // 
    if (Instance()->m_Delay.IsExpired())
    {
        Instance()->NextState();
    }
}

// Setup to execute the next state based on the current state and the selected
// display option.
void DemoMode::NextState()
{
    // Continuous option causes the next sequential state to start.
    if (m_DisplayOption == CONTINUOUS)
    {
        if (++m_State >= NUMBER_DEMO_STATES)
        {
            m_State = FIRST_DEMO_STATE;
        }
    }
    // Random option causes a random state to be selected.
    else if (m_DisplayOption == RANDOM)
    {
        m_State = random(NUMBER_DEMO_STATES);
    }
    // If not Continuous or Random, then must be Repeat.  So don't do anything.
    
    // Staart the (possibly) new state.
    StartCurrentState();
}

// Walk/sweep single LEDs (on or off) depending on original LED state.
void DemoMode::ExecuteWalkLed(unsigned sweep)
{
    // Cache some useful pointers.
    DemoMode *pInstance = Instance();
    uint8_t  *pPattern  = Display::Instance()->GetLedArrayPtr();
    
    // Only do something if the current delay expires.
    if (pInstance->m_Delay.IsExpired())
    {
        // Restore the previous bit to its original state or change the first
        // bit if this is the first time through here.
        if (!sweep || pInstance->m_FirstPass)
        {
            pPattern[pInstance->m_Fret] ^= (1 << pInstance->m_String);
        }
        
        // Need to handle the first pass specially.
        if (pInstance->m_FirstPass)
        {
            pInstance->m_FirstPass = false;
        }
        else 
        {
            // Walk the bit.
            if (++pInstance->m_String >= LedDriver::NUM_STRINGS)
            {
                pInstance->m_String = 0;
                if (++pInstance->m_Fret >= LedDriver::NUM_FRETS)
                {
                    pInstance->m_Fret = 0;
                    pInstance->NextState();
                    return;
                }
            }
            pPattern[pInstance->m_Fret] ^= (1 << pInstance->m_String);        
        }
        // Re-start the delay for next time.
        pInstance->m_Delay.Start(SHORT_DELAY);    
    }
}

// Walk/sweep entire frets (on or off) depending on original LED state.
void DemoMode::ExecuteWalkFret(unsigned sweep)
{
    // Cache some useful pointers.
    DemoMode *pInstance = Instance();
    uint8_t  *pPattern  = Display::Instance()->GetLedArrayPtr();
    
    // Only do something if the current delay expires.
    if (pInstance->m_Delay.IsExpired())
    {
        // Restore the previous bits to their original state or change the first
        // fret if this is the first time through here.
        if (!sweep || pInstance->m_FirstPass)
        {
            pPattern[pInstance->m_Fret] ^= 0xff;
        }
        
        // Need to handle the first pass specially.
        if (pInstance->m_FirstPass)
        {
            pInstance->m_FirstPass = false;
        }
        else 
        {
            // Walk the frets.
            if (++pInstance->m_Fret >= LedDriver::NUM_FRETS)
            {
                pInstance->NextState();
                return;
            }
            pPattern[pInstance->m_Fret] ^= 0xff;        
        }
        // Re-start the delay for next time.
        pInstance->m_Delay.Start(SHORT_DELAY * 2);    
    }
}

// Walk/sweep entire strings (on or off) depending on original LED state.
void DemoMode::ExecuteWalkString(unsigned sweep)
{
    // Cache some useful pointers.
    DemoMode *pInstance = Instance();
    uint8_t  *pPattern  = Display::Instance()->GetLedArrayPtr();
    
    // Only do something if the current delay expires.
    if (pInstance->m_Delay.IsExpired())
    {
        // Restore the previous bits to their original state or change the first
        // string if this is the first time through here.
        if (!sweep || pInstance->m_FirstPass)
        {
            for (unsigned fret = 0; fret < LedDriver::NUM_FRETS; fret++)
            {
                pPattern[fret] ^= (1 << pInstance->m_String);
            }
        }
        
        // Need to handle the first pass specially.
        if (pInstance->m_FirstPass)
        {
            pInstance->m_FirstPass = false;
        }
        else 
        {
            // Walk the strings.
            if (++pInstance->m_String >= LedDriver::NUM_STRINGS)
            {
                pInstance->NextState();
                return;
            }
            for (unsigned fret = 0; fret < LedDriver::NUM_FRETS; fret++)
            {
                pPattern[fret] ^= (1 << pInstance->m_String);
            }
        }
        // Re-start the delay for next time.
        pInstance->m_Delay.Start(SHORT_DELAY * 2);    
    }
}

// Display a number of random chords.
void DemoMode::ExecuteRandomChord(unsigned)
{
    // Cache some useful pointers.
    DemoMode *pInstance = Instance();
    Display  *pDisplay  = Display::Instance();
    
    // Only do something if the current delay expires.
    if (pInstance->m_Delay.IsExpired())
    {
        // Select a random key, type, and variation.
        unsigned key           = random(NUM_KEYS);
        unsigned type          = random(TOTAL_CHORDS_PER_KEY);
        unsigned numVariations = NUM_COMMON_CHORD_VARIATIONS;
        if (type >= NUM_COMMON_CHORDS)
        {
            numVariations = NUM_SUPP_CHORD_VARIATIONS;
        }
        unsigned variation = random(numVariations);
        
        // Get the selected random chord.
        pInstance->m_ChordData.GetChord(key, type, variation);
        
        // Display the selected random chord.
        pDisplay->DisplayChord(&pInstance->m_ChordData);
        
        // Re-start our delay.
        pInstance->m_Delay.Start(LONG_DELAY * 4);
        
        // Limit the number of random chords displayed before we switch to the
        // next state.
        if (++pInstance->m_Count > MAX_RANDOM_CHORDS)
        {
            pInstance->NextState();
        }
    }
    
}
