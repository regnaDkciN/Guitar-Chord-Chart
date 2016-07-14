/////////////////////////////////////////////////////////////////////////////////
// IrCodeDisplayMode.cpp
//
// Contains methods defined by the IrCodeDisplayMode class.  These methods
// manage all of the states of the IR Code Display mode.
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

#include "IrCodeDisplayMode.h"      // For IrCodeDisplayMode class.
#include "Display.h"                // For Display class.
#include "IrDriver.h"               // For IrDriver class.


// Return a pointer to our singleton instance.
IrCodeDisplayMode *IrCodeDisplayMode::Instance()
{
    static IrCodeDisplayMode instance;
    return &instance;
}

// IR display mode is special.  Instead of using a Startup() and Execute() method,
// we do all the work within the Startup() method and stub the Execute() method.
// This is because we never return from the IR code display mode (you need to
// cycle power to get out of this mode since we can't depend on decoding IR key
// values).
void IrCodeDisplayMode::Startup()
{
    // Cache some useful pointers.
    IrDriver *pIrDriver = IrDriver::Instance();
    Display  *pDisplay  = Display::Instance();
    
    // Keep a count of the number of IR keys seen so far.
    uint16_t irCount = 0;
        
    // Display a headline and clear all the LEDS.
    pDisplay->DispLcdProgmem(F("IR Key Code"), true, 0, 2);
    pDisplay->DispLcdProgmem(F("Values"), false, 1, 5);
    pDisplay->SetAllLeds(false);
    
    // Do this forever and never return.
    while (1)
    {
        // Get a key.
        uint32_t newIrKey = pIrDriver->GetKey();
        
        if (newIrKey)
        {
            // Got a new key, update the key count and display it and the key's value.
            irCount++;
            pDisplay->DispLcdProgmem(F("Count:"), true, 0, 0);
            pDisplay->DispLcdInt(irCount, false, 0, 7);

            pDisplay->DispLcdProgmem(F("Val = 0x"), false, 1, 0);
            pDisplay->DispLcdIntHex(newIrKey, false, 1, 8);
        }
    }
}
