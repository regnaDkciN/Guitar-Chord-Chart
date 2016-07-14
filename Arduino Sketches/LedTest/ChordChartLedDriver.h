////////////////////////////////////////////////////////////////////////////////
//
// ChordChartLedDriver.h
// This file defines the ChordChartLedDriver class.  This class handles the
// multiplexing of the Guitar Chord Chart LEDs and provides a simplified
// interface to the driver.
//
// Copyright (C) 2015-2016 J. M. Corbett
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

#if !defined CHORDCHARTLEDDRIVER_H
#define CHORDCHARTLEDDRIVER_H

#include "TimerOne.h"   // For timer.
#include <inttypes.h>   // For uint8_t, ...
#include "Arduino.h"    // For Arduino specific definitions.


class ChordChartLedDriver
{
public:
    // Initialization - resets the driver's pointers.  Should only be needed at
    // setup time, but can be used any time.
    static void Initialize(
                const uint8_t *pFretMap,    // Array of DIO numbers mapped to
                                            // corresponding frets.
                const uint8_t *pStringMap,  // Array of DIO numbers mapped to
                                            // corresponding strings, first
                                            // member is E, then B, then ...
                uint8_t *pFingerBoard,      // Array in which the fingerboard
                                            // to be displayed resides.  
                                            // First entry is bit pattern of
                                            // strings of first fret (LSB =
                                            // high E, etc.
                uint16_t dutyCycle);        // Duty cycle value (1..10).                                

                              
    // Destructor - does nothing.                              
    ~ChordChartLedDriver() { }
    
    // Set the LED display duty cycle.  Valid values are 1 thru 10, corresponding
    // to 10% thru 100%.  Returns the previous duty cycle value.
    static uint16_t SetDutyCycle(uint16_t dutyCycle);
  
protected:

private:
    // Unimplemented methods.
    ChordChartLedDriver();
    
    // ISR to display the LEDs sequentially, one per iteration.
    // This could have been done much more simply, but since digitalWrite()
    // operations are very slow, this implementation minimizes their use.
    static void Isr();
    
    // Turn on a specified string output by setting it high.
    static void TurnStringOn(uint8_t string)
    {
        digitalWrite(m_pStringMap[string], LOW);
    }
    
    // Turn off a specified string output by setting it low.
    static void TurnStringOff(uint8_t string)
    {
        digitalWrite(m_pStringMap[string], HIGH);
    }
    
    // Turn on a specified fret output by setting it low.
    static void TurnFretOn(uint8_t fret)
    {
        digitalWrite(m_pFretMap[fret], HIGH);
    }
    
    // Turn off a specified fret output by setting it high.
    static void TurnFretOff(uint8_t fret)
    {
        digitalWrite(m_pFretMap[fret], LOW);
    }
    
    
    static const uint16_t DUTY_CYCLE_LIMIT  = 10;  // Limit of duty cycle count.
    static const uint16_t NUM_FRETS         = 5;   // Number of frets.
    static const uint16_t NUM_STRINGS       = 6;   // Number of strings.
    static const uint16_t UPDATE_RATE       = 60;  // Desired update rate (per second).
    static const uint16_t TICK_RATE         =      // Rate of timer tick in microseconds.
            (uint16_t)(1000000ul / (NUM_FRETS * NUM_STRINGS * UPDATE_RATE * DUTY_CYCLE_LIMIT));
    
    static       uint8_t  m_CurrentFret;     // Fret being manipulated this iteration.
    static       uint8_t  m_CurrentString;   // String being manipulated this iteration.
    static       bool     m_LastStringWasOn; // true if an LED was lit last iteration. 
    static const uint8_t *m_pFretMap;        // Pointer to the fret map.
    static const uint8_t *m_pStringMap;      // Pointer to the string map.
    static       uint8_t *m_pFingerBoard;    // Pointer to the fingerboard.
    static       uint16_t m_DutyCycle;       // Duty cycle value (1..10).
};

#endif // CHORDCHARTLEDDRIVER_H
