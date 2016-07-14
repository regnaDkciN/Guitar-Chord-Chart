/////////////////////////////////////////////////////////////////////////////////
// GuitarChordChart.ino
//
// Contains the setup() and loop() functions for the Guitar Chord Chart
// application.  Also defines all of the global peripheral info needed by
// the application.  Basically, this file contains the root of all the 
// executable code for the application.
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

#include <IRLib.h>                  // For IR support.
#include <LiquidCrystal.h>          // For LCD support.
#include <EEPROM.h>                 // For EEPPROM methods.
#include "LedDriver.h"              // For mux'd LED support.
#include "IrDriver.h"               // For IR support.
#include "Arduino.h"                // For Arduino specific definitions.
#include "SpecialChars.h"           // For special LCD chars.
#include "ModeManager.h"            // For ModeManager class.
#include "IrCodes.h"                // For raw IR codes (IrCodes).
#include "Display.h"                // For Display class.
#include "EepromConfig.h"           // For EepromConfig class.
#include "DemoMode.h"               // For DemoMode class.
#include "SettingsMode.h"           // For SettingsMode class.


// IR key decode values.
static const uint32_t IR_LED_BRIGHT = IrCodes::VOL_UP;     // Vol +
static const uint32_t IR_LED_DIM    = IrCodes::VOL_DOWN;   // Vol -

// Map the IR pin, and define some useful constants.
static const int IR_RX_PIN           = 19;                // Pin connected to IR detector output.
static const int IR_KEY_DELAY_MILLIS = 200;               // Minimum inter IR key delay.
static IrDriver  gIrRecv(IR_RX_PIN, IR_KEY_DELAY_MILLIS); // Create the IR library.

// The I/O pin assignments for the mux'd string and fret LEDs (columns and rows).
// For a left handed guitar, simply reverse the entries in the gStrings array:
// static const uint8_t gStrings[LedDriver::NUM_STRINGS] = {13, 12, 11, 10, 9, 8};
static const uint8_t gStrings[LedDriver::NUM_STRINGS] = { 8,  9, 10, 11, 12, 13};
static const uint8_t gFrets[LedDriver::NUM_FRETS]     = {14, 15, 16, 17, 18};

// The LED display array.  Each byte represents a fret, while the lower 6 bits
// of each fret represents the 6 guitar strings.
uint8_t gPattern[LedDriver::NUM_FRETS] = { 0,  0,  0,  0,  0};

// Initialize the LCD library with the numbers of the interface pins.
static LiquidCrystal gLcd(2, 3, 4, 5, 6, 7);


// Get the hardware ready to go...
void setup()
{
    // Cache pointers to class instances we'll use in this function.
    Display     *pDisplay     = Display::Instance();
    ModeManager *pModeManager = ModeManager::Instance();
    
    // Initialize the fingerboard (LED) driver.
    LedDriver::Initialize(gFrets, gStrings, gPattern, 1);

    // Set up the IR receiver
    gIrRecv.Enable(); // Start the receiver

    // Set up the LCD's number of columns and rows and clear its display.
    gLcd.begin(16, 2);
    SpecialChars::Initialize(gLcd);
    gLcd.clear();
    
    // Initialize the display.
    pDisplay->Initialize(gPattern, &gLcd);

    // Start with all LEDs off.
    pDisplay->SetAllLeds(false);    
    
    // Display a power-up message.
    pDisplay->PowerupDisplay();
    pDisplay->SetAllLeds(false);
    
    // Initialize the mode manager.
    pModeManager->Initialize();
    
    // Make sure DemoMode is initialized by calling its Instance() method.
    DemoMode::Instance();
    
    // Get our saved configuration (if any) from EEPROM.
    SettingsMode::Instance()->Initialize();
    
    // Clear out any old IR data.
    while (gIrRecv.GetKey())
    {
        delay(500);
    }
}


// This loop gets called repeatedly, and implements the actual application.
void loop()
{
    // Cache some pointers for local use.
    ModeManager *pModeManager = ModeManager::Instance();
    Display     *pDisplay     = Display::Instance();
    
    // See if we got an IR key.
    uint32_t newIrKey = gIrRecv.GetKey();
    
    // Only accept IR keys that we know about.  Will return 0 f unknown.
    newIrKey = IrCodes::Validate(newIrKey);
    
    // Handle global IR requests (i.e. keys that do the same thing
    // in all modes).
    if (newIrKey)
    {
        EepromConfig *pConfig = EepromConfig::Instance();
        AppConfig *pAppConfig = &pConfig->m_AppConfig;
        
        switch(newIrKey)
        {
        case IR_LED_BRIGHT:  // LEDs brighter.
            pAppConfig->m_Brightness = LedDriver::IncrementDutyCycle();
            // Nobody else uses this key, so set it to 0.
            newIrKey = 0;
            break;
            
        case IR_LED_DIM:     // LEDs dimmer,
            pAppConfig->m_Brightness = LedDriver::DecrementDutyCycle();
            // Nobody else uses this key, so set it to 0.
            newIrKey = 0;
            break;
            
        default:
            break;
        }
    }
    
    // Execute whatever mode is currently active.
    newIrKey = pModeManager->Execute(newIrKey);
}
