/////////////////////////////////////////////////////////////////////////////////
// LedDriver.cpp
//
// Contains methods defined by the LedDriver class.  These methods manage the
// interface to the LEDs.
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

#include "LedDriver.h"

// Uncomment to use for timing then specify a pin for timing output
// #define CHORD_CHART_LED_DRIVER_TEST_PIN 19

#if defined CHORD_CHART_LED_DRIVER_TEST_PIN
#define TIMING_PIN_INIT() pinMode(CHORD_CHART_LED_DRIVER_TEST_PIN, OUTPUT)
#define TIMING_PIN_ON()   digitalWrite(CHORD_CHART_LED_DRIVER_TEST_PIN, HIGH)
#define TIMING_PIN_OFF()  digitalWrite(CHORD_CHART_LED_DRIVER_TEST_PIN, LOW)
#else
#define TIMING_PIN_ON()
#define TIMING_PIN_OFF()
#define TIMING_PIN_INIT()
#endif




      uint8_t  LedDriver::m_CurrentFret     = 0;     // Fret being manipulated this iteration.
      uint8_t  LedDriver::m_CurrentString   = 0;     // String being manipulated this iteration.
      bool     LedDriver::m_LastStringWasOn = false; // true if an LED was lit last iteration. 
const uint8_t *LedDriver::m_pFretMap        = NULL;  // Pointer to the fret map.
const uint8_t *LedDriver::m_pStringMap      = NULL;  // Pointer to the string map.
      uint8_t *LedDriver::m_pFingerBoard    = NULL;  // Pointer to the fingerboard.
      uint16_t LedDriver::m_DutyCycle       = 10;    // LED display duty cycle.

// Initialization - resets the driver's pointers.  Should only be needed at
// setup time, but can be used any time.
void LedDriver::Initialize(
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
            uint16_t dutyCycle)         // Duty cycle value (1..10).                                
{
    // Initialize our static variables.
    m_CurrentFret     = 0;
    m_CurrentString   = 0;
    m_LastStringWasOn = false;
    m_pFretMap        = pFretMap;
    m_pStringMap      = pStringMap;
    m_pFingerBoard    = pFingerBoard;
    SetDutyCycle(dutyCycle);
    
    // Initialize fret outputs to digital outputs, off.
    for (int fret = 0; fret < NUM_FRETS; fret++)
    {
        pinMode(m_pFretMap[fret], OUTPUT);
        TurnFretOff(fret);
    }
    // Initialize string outputs to digital outputs, off.
    for (int string = 0; string < NUM_STRINGS; string++)
    {
        pinMode(m_pStringMap[string], OUTPUT);
        TurnStringOff(string);
    }
    TIMING_PIN_INIT();
    // Initialize the timer tick rate and attach our ISR to it.
    Timer1.initialize(TICK_RATE);
    Timer1.attachInterrupt(Isr, TICK_RATE);
}

// Set the LED display duty cycle.  Valid values are 1 thru 10, corresponding
// to 10% thru 100%.  Returns the previous duty cycle value.
uint16_t LedDriver::SetDutyCycle(uint16_t dutyCycle)
{
    // Force the value into the valid range (1..10) range.
    if (dutyCycle < 1)
    {
        dutyCycle = 1;
    }
    else if (dutyCycle > DUTY_CYCLE_LIMIT)
    {
        dutyCycle = DUTY_CYCLE_LIMIT;
    }
    
    // Save the old duty cycle value for return.
    uint16_t oldDutyCycle = m_DutyCycle;
    
    // Set the new duty cycle value.
    m_DutyCycle = dutyCycle;
    
    // Return the old duty cycle value.
    return oldDutyCycle;
}

// Increase the brightness of the LEDs and return the new value
uint16_t LedDriver::IncrementDutyCycle()
{
    SetDutyCycle(m_DutyCycle + 1);
    return m_DutyCycle;
}
  
// Decrease the brightness of the LEDs.
uint16_t LedDriver::DecrementDutyCycle()
{
    SetDutyCycle(m_DutyCycle - 1);
    return m_DutyCycle;
}

// ISR to display the LEDs sequentially, one per iteration.
// This could have been done much more simply, but since digitalWrite()
// operations are very slow, this implementation minimizes their use.
void LedDriver::Isr()
{
    // Toggle the timing pin for debug purposes.
    TIMING_PIN_ON();
    TIMING_PIN_OFF();
    
    // Used to keep track of the duty cycle.
    static uint16_t dutyCycleCount = 0;
    
    // Bump the duty cucle counter and wrap if needed.
    if (++dutyCycleCount >= DUTY_CYCLE_LIMIT)
    {
        dutyCycleCount = 0;
    }
    
    // If the last iteration turned an LED on, then turn it off now.  Just
    // turn off the string now because the next string on the same fret
    // may need to be turned on next.
    if (m_LastStringWasOn)
    {
        TurnStringOff(m_CurrentString);
    }
    // Bump the string index if the duty cycle  count just wrapped.
    if ((dutyCycleCount == 0) && (++m_CurrentString >= NUM_STRINGS))
    {
        // We've done all the strings for this fret, so reset the string
        // index, turn off the old fret output, and bump the fret index.
        m_CurrentString = 0;
        TurnFretOff(m_CurrentFret);
        if (++m_CurrentFret >= NUM_FRETS)
        {
            // We've done all of the frets, reset the fret index.
            m_CurrentFret = 0;
        }
        // Turn on the new fret output.
        TurnFretOn(m_CurrentFret);
    }
    
    // If within duty cycle, see if fret/string LED should be lit now.
    if((dutyCycleCount < m_DutyCycle) && (m_pFingerBoard[m_CurrentFret] & (1 << m_CurrentString)))
    {
        // The currently selected LED must be lit, so turn it on and remember
        // that we did so.
        TurnStringOn(m_CurrentString);
        m_LastStringWasOn = true;
    }
    else
    {
        // The currently selected LED must stay off.  Remember it.
        m_LastStringWasOn = false;
    }
    TIMING_PIN_ON();
    TIMING_PIN_OFF();
}
