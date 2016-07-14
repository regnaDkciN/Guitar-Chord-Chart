////////////////////////////////////////////////////////////////////////////////
// IrCodeDisplayMode.h
//
// Defines the class that handles  displaying raw IR codes.  This mode reports
// the raw key codes that are received.  This mode cannot be exited because
// it disables the decoding of IR keys.  To exit this mode, power must be cycled.
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

#if !defined IRCODEDISPLAYMODE_H
#define IRCODEDISPLAYMODE_H

#include "Mode.h"               // For Mode base class.



/////////////////////////////////////////////////////////////////////////////////
// IrCodeDisplayMode class
/////////////////////////////////////////////////////////////////////////////////
class IrCodeDisplayMode : public Mode
{
public:
    static IrCodeDisplayMode *Instance();
    void Initialize();
    
    virtual uint32_t Execute(uint32_t irKey) { };
    virtual void     Startup();
    virtual void     Shutdown() { }

protected:
    
private:
    // Unimplemented methods
    IrCodeDisplayMode(IrCodeDisplayMode &rMgr);
    IrCodeDisplayMode &operator=(IrCodeDisplayMode &rMgr);

    // Private constructor and destructor for singleton.
    IrCodeDisplayMode()  { }; 
    ~IrCodeDisplayMode() { }
};



#endif  // IRCODEDISPLAYMODE_H
