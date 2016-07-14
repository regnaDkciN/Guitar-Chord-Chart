////////////////////////////////////////////////////////////////////////////////
//
// LedTest.ino
// This Arduino sketch implements a simple test of the Guitar Chord Chart's
// LED display.  It simply displays the name of a chord on the LCD, and the
// corresponding chord fingering on the multiplexed LEDs.
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
////////////////////////////////////////////////////////////////////////////////

#include "ChordChartLedDriver.h"
#include <LiquidCrystal.h>

const uint16_t NUM_STRINGS = 6;
const uint16_t NUM_FRETS   = 5;

const uint8_t gStrings[NUM_STRINGS] = { 8,  9, 10, 11, 12, 13};
const uint8_t gFrets[NUM_FRETS]     = {14, 15, 16, 17, 18};
      uint8_t gPattern[NUM_FRETS]   = { 0,  0,  0,  0,  0};

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);


const uint8_t Chord_A[] = {0x00, 0x04, 0x00, 0x10, 0x02};
const uint8_t Chord_B[] = {0x00, 0x31, 0x00, 0x0e, 0x00};
const uint8_t Chord_C[] = {0x02, 0x08, 0x30, 0x00, 0x00};
const uint8_t Chord_D[] = {0x00, 0x25, 0x02, 0x00, 0x00};
const uint8_t Chord_E[] = {0x04, 0x02, 0x08, 0x10, 0x00};
const uint8_t Chord_F[] = {0x03, 0x04, 0x18, 0x00, 0x00};
const uint8_t Chord_G[] = {0x00, 0x00, 0x03, 0x04, 0x18};
const uint8_t *pMajorChords[] = {Chord_A, Chord_B, Chord_C, Chord_D,
                                 Chord_E, Chord_F, Chord_G};

void SetAllLeds(bool state)
{
    uint8_t fretValue = state ? 0x3f : 0;
    for (uint8_t fret = 0; fret < NUM_FRETS; fret++)
    {
        gPattern[fret] = fretValue;
    }
}    

void setup()
{
    // Initialize the fingerboard (LED) driver.
    ChordChartLedDriver::Initialize(gFrets, gStrings, gPattern, 1);
    
    // Set up the LCD's number of columns and rows.
    lcd.begin(16, 2);
}

void loop()
{
    static bool state = false;
    uint16_t    string, fret;
    
    const char *pState = state ? "ON" : "OFF";
    
    // Start with all LEDs in the same state.
    SetAllLeds(state);
    
    // Display each duty cycle if state is all on.
    if (state)
    {
        uint16_t savedDutyCycle = ChordChartLedDriver::SetDutyCycle(1);
        SetAllLeds(false);
        for (uint16_t dutyCycle = 1; dutyCycle <= 10; dutyCycle++)
        {
            ChordChartLedDriver::SetDutyCycle(dutyCycle);
            lcd.clear();
            lcd.print("Duty Cycle: ");
            lcd.print(dutyCycle * 10);
            lcd.print("%");
            SetAllLeds(true);
            delay(1500);
        }
        SetAllLeds(state);
        ChordChartLedDriver::SetDutyCycle(savedDutyCycle);
    }
        
    // Step through each string of each fret sequentially.
    lcd.clear();
    lcd.print("Walk all ");
    lcd.print(pState);
    for (fret = 0; fret < NUM_FRETS; fret++)
    {
        for (string = 0; string < NUM_STRINGS; string++)
        {
            lcd.setCursor(0, 1);
            lcd.print("Fret:"); lcd.print(fret + 1); lcd.print("  String:"); lcd.print(string + 1);
            gPattern[fret] ^= 1 << string;
            delay(1000);
            gPattern[fret] ^= 1 << string;
        }
    }
    
    // Toggle all LEDs of each fret in turn.
    lcd.clear();
    lcd.print("Walk frets ");
    lcd.print(pState);
    for (fret = 0; fret < NUM_FRETS; fret++)
    {
        lcd.setCursor(0, 1);
        lcd.print("Fret: "); lcd.print(fret + 1);
        gPattern[fret] ^= 0x3f;
        delay(1000);
        gPattern[fret] ^= 0x3f;
    }
    
    // Toggle all the LEDs of each string in turn.
    lcd.clear();
    lcd.print("Walk strings ");
    lcd.print(pState);
    for (string = 0; string < NUM_STRINGS; string++)
    {
        lcd.setCursor(0, 1);
        lcd.print("String: "); lcd.print(string + 1);
        uint8_t stringBit = 1 << string;
        for (fret = 0; fret < NUM_FRETS; fret++)
        {
            gPattern[fret] ^= stringBit;
        }
        delay(1000);
        for (fret = 0; fret < NUM_FRETS; fret++)
        {
            gPattern[fret] ^= stringBit;
        }
    }
    
    // Show the major chords.
    if (state)
    {
        const int16_t NUM_MAJOR_CHORDS = sizeof(pMajorChords) / sizeof(pMajorChords[0]);
        for (uint8_t index = 0; index < NUM_MAJOR_CHORDS; index++)
        {
            lcd.clear();
            lcd.print(char('A' + index)); 
            lcd.print(" Major Chord");
            uint8_t const *pChord = pMajorChords[index];
            memcpy(gPattern, pChord, NUM_FRETS);
            delay(1500);
        }
    }
    
    state ^= true;
}    
