////////////////////////////////////////////////////////////////////////////////
// IrTest.ino
// Simple Arduino sketch to test the IR remote sensing for the Guitar Chord
// Chart project.  This sketch receives IR input on pint 19, and displays
// information about any received IR key presses on the attached LCD.
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

#include <LiquidCrystal.h>
#include <IRLib.h>           // IR remote control library


const int    irReceivePin = 19;     // pin connected to IR detector output
IRdecodeHash gDecodeResults;        // IR data goes here
IRrecv       gIrRecv(irReceivePin); // create the IR library


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

void setup()
{
    // Set up the IR receiver
    gIrRecv.enableIRIn(); // Start the receiver

    // Set up the LCD's number of columns and rows.
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("Press a key.");
}

void loop()
{
    static uint16_t irCount = 0;

    // See if we got an IR key.
    if (gIrRecv.GetResults(&gDecodeResults))
    {
        gDecodeResults.decode();
        const uint32_t IrWaitMillis       = 500; // Minimum delay between IR key receipt.
        static uint32_t previousHashValue = 0;   // IR hash value from last IR key.
        static uint32_t previousMillis    = 0;   // System time in ms fron last IR key.
        uint32_t currentMillis = millis();       // Get the current system time in ms.
        
        // Only accept the key if it is not an immediate repeat.
        if ((gDecodeResults.hash != previousHashValue) ||
            ((currentMillis - previousMillis) > IrWaitMillis))
        {
            // Save the key and its receipt time.
            previousHashValue = gDecodeResults.hash;
            previousMillis = currentMillis;
            
            // Update the key count and display it and the key's value.
            irCount++;
            lcd.clear();
            lcd.print("Count: ");
            lcd.print(irCount);
            lcd.setCursor(0, 1);
            lcd.print("Val = 0x");
            lcd.print(gDecodeResults.hash, HEX);
        }
        // Set up to receive the next key.
        gIrRecv.resume();
    }
    delay(100);
}
