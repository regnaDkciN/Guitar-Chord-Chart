/////////////////////////////////////////////////////////////////////////////////
// Mode.h
//
// Defines the abstract base class that all mode classes derive from.
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

#if !defined MODE_H
#define MODE_H

#include "Arduino.h"                // For uintxx_t.


/////////////////////////////////////////////////////////////////////////////////
// Mode base class.  All modes derive from this abstract class.
/////////////////////////////////////////////////////////////////////////////////
class Mode
{
public:
    Mode() { }
    virtual ~Mode() { }

    // All derived classes must supply Execute(), Startup(), and Shutdown() methods.
    virtual uint32_t Execute(uint32_t irKey) = 0;
    virtual void     Startup()  = 0;
    virtual void     Shutdown() = 0;

protected:
    
private:
    
};



#endif // MODE_H
