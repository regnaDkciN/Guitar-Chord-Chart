////////////////////////////////////////////////////////////////////////////////
// Strings.h
//
// Defines the class that is used to fetch strings fronm FLASH memory.
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

#if !defined STRINGS_H
#define STRINGS_H

#include <avr/pgmspace.h>       // For PROGMEM.

/////////////////////////////////////////////////////////////////////////////////
// ChordType - enum of chord types (major, minor, ...).
/////////////////////////////////////////////////////////////////////////////////
enum ChordType
{
    TypeMajor = 0,                          // M
    TypeMinor,                              // m
    TypeSixth,                              // 6
    TypeMinorSixth,                         // m6
    TypeSeventh,                            // 7
    TypeMajorSeventh,                       // M7
    TypeMinorSeventh,                       // m7
    TypeNinth,                              // 9
    //===============================
    TypeDiminished,                         // o
    TypeAugmented,                          // +
    TypeSuspendedSecond,                    // sus2
    TypeSeventhSuspendedSecond,             // 7sus2
    TypeSuspendedFourth,                    // sus4
    TypeSeventhSuspendedFourth,             // 7sus4
    TypeFifth,                              // 5
    TypeDiminishedFifth,                    // -5
    TypeSeventhDiminishedFifth,             // 7b5
    TypeMinorSeventhDiminishedFifth,        // m7b5
    TypeSeventhAugmentedFifth,              // 7#5
    TypeSeventhMinorNinth,                  // 7b9
    TypeSeventhAugmentedNinth,              // 7#9
    TypeSeventhMinorNinthAugmentedFifth,    // 7b9#5
    TypeSevenSix,                           // 7/6
    TypeNinthDiminishedFifth,               // 9b5
    TypeNinthAugmentedFifth,                // 9#5
    TypeMajorNinth,                         // M9
    TypeMinorNinth,                         // m9
    TypeNineSix,                            // 9/6
    TypeMinorNineSix,                       // m9/6
    TypeAddedNinth,                         // add9
    TypeEleventh,                           // 11
    TypeMinorEleventh,                      // m11
    TypeAugmentedEleventh,                  // 11+
    TypeThirteenth,                         // 13
    TypeThirteenthMinorNinth,               // 13b9
    TypeThirteenthMinorNinthDiminishedFifth // 13b9b5
};

/////////////////////////////////////////////////////////////////////////////////
// ChordKey - enum of chord keys (C, C#, ...).
/////////////////////////////////////////////////////////////////////////////////
enum ChordKey
{
    KeyTypeC = 0,
    KeyTypeCSharp,
    KeyTypeD,
    KeyTypeEFlat,
    KeyTypeE,
    KeyTypeF,
    KeyTypeFSharp,
    KeyTypeG,
    KeyTypeAFlat,
    KeyTypeA,
    KeyTypeBFlat,
    KeyTypeB
};



/////////////////////////////////////////////////////////////////////////////////
// Strings class - provides accessor methods to fetch strings from FLASH.
/////////////////////////////////////////////////////////////////////////////////
class Strings
{
public:
    static char *GetKeyString(unsigned index, char *pBuf);
    static char *GetShortTypeString(unsigned index, char *pBuf);
    static char *GetLongTypeString(unsigned indes, char *pBuf);
    static char *GetUnplayedOptionString(unsigned index, char *pBuf);
    static char *GetDemoModeString(unsigned index, char *pBuf);
    static char *GetDemoOptionString(unsigned index, char *pBuf);

protected:
    
private:    
};


#endif  // STRINGS_H
