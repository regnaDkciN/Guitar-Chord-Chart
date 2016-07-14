/////////////////////////////////////////////////////////////////////////////////
// SpecialChars.h
//
// Defines the class that creates special LCD characters.
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

#if !defined SPECIALCHARS_H
#define SPECIALCHARS_H

#include <LiquidCrystal.h>          // For LCD support.
#include "Arduino.h"                // For Arduino specific definitions.


/////////////////////////////////////////////////////////////////////////////////
// SpecialChars class.
/////////////////////////////////////////////////////////////////////////////////
class SpecialChars
{
public:
    static void Initialize(LiquidCrystal &rLcd)
    {
        rLcd.createChar(FLAT_CHAR,     const_cast<uint8_t *>(FlatChar));
        rLcd.createChar(AUGMENT_CHAR,  const_cast<uint8_t *>(AugmentChar));
        rLcd.createChar(DIMINISH_CHAR, const_cast<uint8_t *>(DiminishChar));
        rLcd.createChar(SHARP_CHAR,    const_cast<uint8_t *>(SharpChar));
    }    

protected:
    
private:

    // The flat character.
#define GCSC_FLAT    "\001"
    static const unsigned FLAT_CHAR = 1;
    static const byte FlatChar[8];

    // The augment character.
#define GCSC_AUGMENT  "\002"    
    static const unsigned AUGMENT_CHAR = 2;
    static const byte AugmentChar[8];

    // The diminish character.
#define GCSC_DIMINISH  "\003"
    static const unsigned DIMINISH_CHAR = 3;
    static const byte DiminishChar[8];

    // The sharp character.
#define GCSC_SHARP    "\004"    
    static const unsigned SHARP_CHAR = 4;
    static const byte SharpChar[8];
};

#endif // SPECIALCHARS_H
