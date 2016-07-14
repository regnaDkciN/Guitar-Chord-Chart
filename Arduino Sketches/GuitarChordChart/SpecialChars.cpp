/////////////////////////////////////////////////////////////////////////////////
// SpecialChars.cpp
//
// Contains data defined by the SpecialChars class.  These data
// form the special musical characters used by the Guitar Chord Chart application.
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

#include "SpecialChars.h"



 // The flat character.
const byte SpecialChars::FlatChar[8]  =
{
    B10000,     // *
    B10000,     // *
    B10000,     // *
    B10110,     // * **
    B11001,     // **  *
    B11110,     // ****
    B00000,     // 
    B00000
};

// The augment character.
const byte SpecialChars::AugmentChar[8]  =
{
    B00100,     //   *
    B00100,     //   *
    B11111,     // *****
    B00100,     //   *
    B00100,     //   *
    B00000,     //
    B00000,     // 
    B00000
};

// The diminish character.
const byte SpecialChars::DiminishChar[8]  =
{
    B01110,     //  ***
    B10001,     // *   *
    B10001,     // *   *
    B01110,     //  ***
    B00000,     // 
    B00000,     //
    B00000,     // 
    B00000
};

// The sharp character.
const byte SpecialChars::SharpChar[8]  =
{
    B01010,     //  * *
    B11111,     // *****
    B01010,     //  * *
    B11111,     // *****
    B01010,     //  * *
    B00000,     //
    B00000,     // 
    B00000
};

