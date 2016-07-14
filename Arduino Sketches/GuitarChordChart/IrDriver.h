/////////////////////////////////////////////////////////////////////////////////
// IrDriver.h
//
// Defines the class that interfaces to the IR receiver.
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

#if !defined IRDRIVER_H
#define IRDRIVER_H

#include <IRLib.h>      // IR remote control library.
#include "Arduino.h"    // For Arduino specific definitions.



/////////////////////////////////////////////////////////////////////////////////
// IrDriver class.
/////////////////////////////////////////////////////////////////////////////////
class IrDriver : public IRdecodeHash
{
public:
    // Constructor - init all instance data.  
    // Arg irPin       = input pin for IR signal
    // Arg delayMillis = minimum acceptable inter key delay in milliseconds
    //                   default is 500ms.
    IrDriver(uint8_t  irPin, uint16_t delayMillis = 500) : 
    m_IrRecv(irPin), m_DelayMillis(delayMillis), 
    m_LastKeyMillis(0), m_LastHashValue(0)
    {
        m_pInstance = this;
    }

    // Destructor - does nothing.                              
    ~IrDriver() { }
    
    // Pointer to our one and only instance.  This class is a singleton,
    // but it is not enforced.  It is a gentleman's agreement that only
    // one instance of the class be created.
    static IrDriver *Instance()
    {
        return m_pInstance;
    }
    
    // Enable the IR receiver.
    void Enable() { m_IrRecv.enableIRIn(); }
    
    // Return the last key press if any (0 if none).
    uint32_t GetKey();
    
    // Get/Set the inter key minumun delay value.
    uint16_t GetKeyDelayMillis() const
             { return m_DelayMillis; }
    void     SetKeyDelayMillis(uint16_t delayMillis) 
             { m_DelayMillis = delayMillis; }
        

protected:
    
private:
    // Unimplemented methods.
    IrDriver();

    virtual bool decode();        // Virtual so we can substitute our own
    
    IRrecv   m_IrRecv;            // IR library
    uint32_t m_DelayMillis;       // Minimum delay between IR key receipt.
    uint32_t m_LastKeyMillis;     // System time in ms fron last IR key.
    uint32_t m_LastHashValue;     // IR hash value from last IR key.
    static IrDriver *m_pInstance; // Pointer to ourselves.
};

#endif // IRDRIVER_H
