/////////////////////////////////////////////////////////////////////////////////
// IrDriver.cpp
//
// Contains methods defined by the IrDriver class.  These methods
// manage the decoding of IR input to the Guitar Chord Chart application.
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

#include "IrDriver.h"

// Our static instance pointer.
IrDriver *IrDriver::m_pInstance = NULL;

// Return a new key press hash value, if any (0 otherwise).
uint32_t IrDriver::GetKey()
{
    // See if we got a new key press.
    if (m_IrRecv.GetResults(this))
    {
        // Got a new key press so decode it.
        decode();
        
        // Save the received key value.
        uint32_t keyValue = hash;
    
        // Set up to receive the next key.
        m_IrRecv.resume();
        
        // Return the received character (may be 0).
        return keyValue;
    }
    
    // we didn't receive a new key.  Return 0.
    return 0;
    
}

// Decode the new key press and filter out immediate repeats.
bool IrDriver::decode()
{
    // Remember the current time.
    uint16_t currentMillis = millis();
    
    // Let the has decoder do all the work.
    IRdecodeHash::decode();
    
    // Only accept the key if it is not an immediate repeat.
    if ((currentMillis - m_LastKeyMillis) > m_DelayMillis)
    {
        // Save the key and its receipt time.
        m_LastHashValue = hash;
        m_LastKeyMillis = currentMillis;
    }
    else
    {
        // The key was a repeat of the last one and too soon.  Pretend that 
        // no new key was received by returning 0.
        hash = 0;
    }
    return true;
}
