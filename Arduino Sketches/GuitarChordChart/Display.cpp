/////////////////////////////////////////////////////////////////////////////////
// Display.cpp
//
// Contains methods defined by the Display class.  
// The methods in this class manage dispalying data to the LCD, and a few that
// handle special access to the LEDs.
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

#include "Display.h"        // For Display class.
#include "PolledDelay.h"    // For PolledDelay class.
#include "ChordChartData.h" // For the chord data.
#include "LedDriver.h"      // For mux'd LED support.
#include "Strings.h"        // For strings displayed on the LCD.


// Return our singleton instance.
Display *Display::Instance()
{
    static Display instance;
    return &instance;
}

// Init our display data and get ready for use.
void Display::Initialize(uint8_t *pLedArray, LiquidCrystal *pLcd)
{
    m_pLedArray      = pLedArray;
    m_pLcd           = pLcd;
    m_Verbose        = true;
    m_UnplayedOption = LIGHT_LAST_FRET_UNUSED;
}

// Function to set all LEDs to the same state (off if state is 0, on otherwise).
void Display::SetAllLeds(bool state)
{
    uint8_t fretValue = state ? 0x3f : 0;
    for (uint8_t fret = 0; fret < LedDriver::NUM_FRETS; fret++)
    {
        m_pLedArray[fret] = fretValue;
    }
}

// Diaplay some information regarding the chord chart, and run a quick
// LED test.
void Display::PowerupDisplay()
{
    // Init the current string and fret such that the first increment will
    // make each of them zero.
    uint16_t string  = LedDriver::NUM_STRINGS;
    uint16_t fret    = LedDriver::NUM_FRETS;

    // Display the welcome screen.
    m_pLcd->clear();
    m_pLcd->print(F(" Chord-O-Matic"));
    m_pLcd->setCursor(0, 1);
    m_pLcd->print(F("V1.0      By JMC"));
    
    // Run a bit sweep on the LEds as a visual indication that the LEDs are OK.
    while (1)
    {
        // Step through each string of each fret sequentially, periodically.
        const  uint32_t ledUpdatePeriodMillis = 120;
        static PolledDelay updateTimer(ledUpdatePeriodMillis);
        static uint16_t loopCount = 0;
        if (updateTimer.IsExpired())
        {
            updateTimer.Start(ledUpdatePeriodMillis);
            
            if (++string >= LedDriver::NUM_STRINGS)
            {
                string = 0;
                if (++fret >= LedDriver::NUM_FRETS)
                {
                    fret = 0;
                    if (++loopCount >= 3)
                    {
                        return;
                    }
                }
            }
            m_pLedArray[fret] ^= (1 << string);
        }
    }
}

// Display a specified chord on the LCD and on the fingerboard.
// pData - A pointer to the ChordData to be displayed.
// A run time option is available to select how non-played strings
// strings will be handled.
void Display::DisplayChord(ChordChartData *pData)
{
    // Display the chord's data:
    // --------------------------            --------------------
    // | Key   Variation   Fret |  Example:  | Bb  V:4  Fret:10 |
    // | Type                   |            | Sixth            |
    // --------------------------            --------------------
    char buf[17];
    m_pLcd->clear();
    m_pLcd->print(Strings::GetKeyString(pData->GetCurrentKey(), buf));
    m_pLcd->setCursor(4, 0);
    m_pLcd->print(F("V:"));
    m_pLcd->print(pData->GetCurrentVariation() + 1);
    m_pLcd->print(F("  Fret:"));
    m_pLcd->print(pData->GetCurrentFret());
    m_pLcd->setCursor(0, 1);
    if (m_Verbose)
    {
        m_pLcd->print(Strings::GetLongTypeString(pData->GetCurrentType(), buf));
    }
    else
    {
        m_pLcd->print(Strings::GetShortTypeString(pData->GetCurrentType(), buf));
    }
    
    // Display the chord pattern on the fingerboard.
    DisplayLeds(pData->GetChordPtr());
}    

// Display a specified chord on the LCD and on the fingerboard.
// pData - A pointer to the ChordData to be displayed.
// A compile time option is available to select how non-played strings
// strings will be handled.
void Display::DisplayLeds(Chord *pChord)
{
    Chord chord;
    memcpy(&chord, pChord, sizeof(Chord));
    
    // Determine how to handle non-played strings.
    switch (m_UnplayedOption)
    {
    // Option: Set all string LEDs for non-played strings.
    case LIGHT_ALL_FRETS:  
        for (unsigned i = 0; i < LedDriver::NUM_FRETS; i++)
        {
            chord.GetPatternPtr()[i] |= chord.GetUnplayed();
        }
        break;
        
    // Option: Set the strings of the last (unused) fret for non-played strings.
    case LIGHT_LAST_FRET_UNUSED:
        chord.GetPatternPtr()[LedDriver::NUM_FRETS - 1] |= chord.GetUnplayed();;
        break;

    // Option: Set the strings of the last (unused) fret for played strings.
    case LIGHT_LAST_FRET_USED:
        chord.GetPatternPtr()[LedDriver::NUM_FRETS - 1] |= ~chord.GetUnplayed();
        break;
        
    // Option: Do nothing
    case IGNORE:
    default:
        break;
    }

    // Display the chord pattern on the fingerboard.
    memcpy(m_pLedArray, chord.GetPatternPtr(), LedDriver::NUM_FRETS);
}    

// Display a string from FLASH (PROGMEM).
void Display::DispLcdProgmem(const __FlashStringHelper *pStr, bool clear, unsigned row, unsigned col)
{
    // Clear the display if requested.
    if (clear)
    {
        m_pLcd->clear();
    }
    // Display the string.
    if (pStr != NULL)
    {
        m_pLcd->setCursor(col, row);
        m_pLcd->print(pStr);
    }
}

// Display a decimal integer value.
void Display::DispLcdInt(uint32_t val, bool clear, unsigned row, unsigned col)
{
    // Clear the display if requested.
    if (clear)
    {
        m_pLcd->clear();
    }
    // Display the int.
    m_pLcd->setCursor(col, row);
    m_pLcd->print(val);
}

// Display a hex integer value.
void Display::DispLcdIntHex(uint32_t val, bool clear, unsigned row, unsigned col)
{
    // Clear the display if requested.
    if (clear)
    {
        m_pLcd->clear();
    }
    // Display the int in hex.
    m_pLcd->setCursor(col, row);
    m_pLcd->print(val, HEX);
}

// Display a string.
void Display::DispLcd(const char *pStr, bool clear, unsigned row, unsigned col)
{
    // Clear the display if requested.
    if (clear)
    {
        m_pLcd->clear();
    }
    // Display the string.
    if (pStr != NULL)
    {
        m_pLcd->setCursor(col, row);
        m_pLcd->print(pStr);
    }
}
