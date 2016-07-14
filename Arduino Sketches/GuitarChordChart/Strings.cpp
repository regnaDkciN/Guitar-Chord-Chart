/////////////////////////////////////////////////////////////////////////////////
// Strings.cpp
//
// Contains strings that are stored in FLASH, and methods to access them.
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

#include "Strings.h"
#include "SpecialChars.h"


// All of the strings in this file are stored in FLASH.  
// This file also includes accessor methods for retrieving the strings.


// The key strings, starting with "C".
// Note that special characters are used in some of these strings:
// \001 = Flat character (b)
// \002 = Augment character (+)
// \003 = Diminish character (0)
// \004 = Sharp character (#)
static const char KeyString_00[] PROGMEM = "C";
static const char KeyString_01[] PROGMEM = "C" GCSC_SHARP ;     // C#
static const char KeyString_02[] PROGMEM = "D";                 // D
static const char KeyString_03[] PROGMEM = "E" GCSC_FLAT ;      // Eb
static const char KeyString_04[] PROGMEM = "E";                 // E
static const char KeyString_05[] PROGMEM = "F";                 // F
static const char KeyString_06[] PROGMEM = "F" GCSC_SHARP ;     // F#
static const char KeyString_07[] PROGMEM = "G";                 // G
static const char KeyString_08[] PROGMEM = "A" GCSC_FLAT ;      // Ab
static const char KeyString_09[] PROGMEM = "A";                 // A
static const char KeyString_10[] PROGMEM = "B" GCSC_FLAT;       // Bb
static const char KeyString_11[] PROGMEM = "B";

// An array of pointers to the key strings.
static const char* const KeyStrings[] PROGMEM = 
{
    KeyString_00, KeyString_01, KeyString_02, KeyString_03,
    KeyString_04, KeyString_05, KeyString_06, KeyString_07,
    KeyString_08, KeyString_09, KeyString_10, KeyString_11
};


// The short chord type strings.
// Note that special characters are used in some of these strings:
// \001 = Flat character (b)
// \002 = Augment character (+)
// \003 = Diminish character (0)
// \004 = Sharp character (#)
static const char ChordTypeShort_00[] PROGMEM = "";                                 // 
static const char ChordTypeShort_01[] PROGMEM = "m";                                // m
static const char ChordTypeShort_02[] PROGMEM = "6";                                // 6
static const char ChordTypeShort_03[] PROGMEM = "m6";                               // m6
static const char ChordTypeShort_04[] PROGMEM = "7";                                // 7
static const char ChordTypeShort_05[] PROGMEM = "M7";                               // M7
static const char ChordTypeShort_06[] PROGMEM = "m7";                               // m7
static const char ChordTypeShort_07[] PROGMEM = "9";                                // 9
static const char ChordTypeShort_08[] PROGMEM = GCSC_DIMINISH ;                     // o
static const char ChordTypeShort_09[] PROGMEM = GCSC_AUGMENT ;                      // +
static const char ChordTypeShort_10[] PROGMEM = "sus2";                             // sus2
static const char ChordTypeShort_11[] PROGMEM = "7sus2";                            // 7sus2
static const char ChordTypeShort_12[] PROGMEM = "sus4";                             // sus4
static const char ChordTypeShort_13[] PROGMEM = "7sus4";                            // 7sus4
static const char ChordTypeShort_14[] PROGMEM = "5";                                // 5
static const char ChordTypeShort_15[] PROGMEM = "-5";                               // -5
static const char ChordTypeShort_16[] PROGMEM = "7" GCSC_FLAT "5";                  // 7b5
static const char ChordTypeShort_17[] PROGMEM = "m7" GCSC_FLAT "5";                 // m7b5
static const char ChordTypeShort_18[] PROGMEM = "7" GCSC_SHARP "5";                 // 7#5
static const char ChordTypeShort_19[] PROGMEM = "7" GCSC_FLAT "9";                  // 7b9
static const char ChordTypeShort_20[] PROGMEM = "7" GCSC_SHARP "9";                 // 7#9
static const char ChordTypeShort_21[] PROGMEM = "7" GCSC_FLAT "9" GCSC_SHARP "5";   // 7b9#5
static const char ChordTypeShort_22[] PROGMEM = "7/6";                              // 7/6
static const char ChordTypeShort_23[] PROGMEM = "9" GCSC_FLAT "5";                  // 9b5
static const char ChordTypeShort_24[] PROGMEM = "9" GCSC_SHARP "5";                 // 9#5
static const char ChordTypeShort_25[] PROGMEM = "M9";                               // M9
static const char ChordTypeShort_26[] PROGMEM = "m9";                               // m9
static const char ChordTypeShort_27[] PROGMEM = "9/6";                              // 9/6
static const char ChordTypeShort_28[] PROGMEM = "m9/6";                             // m9/6
static const char ChordTypeShort_29[] PROGMEM = "add9";                             // add9
static const char ChordTypeShort_30[] PROGMEM = "11";                               // 11
static const char ChordTypeShort_31[] PROGMEM = "m11";                              // m11
static const char ChordTypeShort_32[] PROGMEM = "11" GCSC_AUGMENT ;                 // 11+
static const char ChordTypeShort_33[] PROGMEM = "13";                               // 13
static const char ChordTypeShort_34[] PROGMEM = "13" GCSC_FLAT "9";                 // 13b9
static const char ChordTypeShort_35[] PROGMEM = "13" GCSC_FLAT "9" GCSC_FLAT "5";   // 13b9b5

// An array of pointers to the short chord type strings.
static const char* const ChordShortStrings[] PROGMEM = 
{
    ChordTypeShort_00, ChordTypeShort_01, ChordTypeShort_02, ChordTypeShort_03,
    ChordTypeShort_04, ChordTypeShort_05, ChordTypeShort_06, ChordTypeShort_07,
    ChordTypeShort_08, ChordTypeShort_09, ChordTypeShort_10, ChordTypeShort_11,
    ChordTypeShort_12, ChordTypeShort_13, ChordTypeShort_14, ChordTypeShort_15,
    ChordTypeShort_16, ChordTypeShort_17, ChordTypeShort_18, ChordTypeShort_19,
    ChordTypeShort_20, ChordTypeShort_21, ChordTypeShort_22, ChordTypeShort_23,
    ChordTypeShort_24, ChordTypeShort_25, ChordTypeShort_26, ChordTypeShort_27,
    ChordTypeShort_28, ChordTypeShort_29, ChordTypeShort_30, ChordTypeShort_31,
    ChordTypeShort_32, ChordTypeShort_33, ChordTypeShort_34, ChordTypeShort_35
};


        
// The long chord type strings.
static const char ChordTypeLong_00[] PROGMEM = "Major";
static const char ChordTypeLong_01[] PROGMEM = "Minor";
static const char ChordTypeLong_02[] PROGMEM = "6th";
static const char ChordTypeLong_03[] PROGMEM = "Minor 6th";
static const char ChordTypeLong_04[] PROGMEM = "7th";
static const char ChordTypeLong_05[] PROGMEM = "Major 7th";
static const char ChordTypeLong_06[] PROGMEM = "Minor 7th";
static const char ChordTypeLong_07[] PROGMEM = "9th";
static const char ChordTypeLong_08[] PROGMEM = "Diminished";
static const char ChordTypeLong_09[] PROGMEM = "Augmented";
static const char ChordTypeLong_10[] PROGMEM = "Suspended 2nd";
static const char ChordTypeLong_11[] PROGMEM = "7th Susp. 2nd";
static const char ChordTypeLong_12[] PROGMEM = "Suspended 4th";
static const char ChordTypeLong_13[] PROGMEM = "7th Susp. 4th";
static const char ChordTypeLong_14[] PROGMEM = "5th";
static const char ChordTypeLong_15[] PROGMEM = "Diminished 5th";
static const char ChordTypeLong_16[] PROGMEM = "7th Dim. 5th";
static const char ChordTypeLong_17[] PROGMEM = "Mnr 7th Dim. 5th";
static const char ChordTypeLong_18[] PROGMEM = "7th Aug. 5th";
static const char ChordTypeLong_19[] PROGMEM = "7th Minor 9th";
static const char ChordTypeLong_20[] PROGMEM = "7th Aug. 9th";
static const char ChordTypeLong_21[] PROGMEM = "7 Minor 9 Aug. 5";
static const char ChordTypeLong_22[] PROGMEM = "Seven-Six";
static const char ChordTypeLong_23[] PROGMEM = "9th Dim. 5th";
static const char ChordTypeLong_24[] PROGMEM = "9th Aug. 5th";
static const char ChordTypeLong_25[] PROGMEM = "Major 9th";
static const char ChordTypeLong_26[] PROGMEM = "Minor 9th";
static const char ChordTypeLong_27[] PROGMEM = "Nine-Six";
static const char ChordTypeLong_28[] PROGMEM = "Minor Nine-Six";
static const char ChordTypeLong_29[] PROGMEM = "Added 9th";
static const char ChordTypeLong_30[] PROGMEM = "11th";
static const char ChordTypeLong_31[] PROGMEM = "Minor 11th";
static const char ChordTypeLong_32[] PROGMEM = "Augmented 11th";
static const char ChordTypeLong_33[] PROGMEM = "13th";
static const char ChordTypeLong_34[] PROGMEM = "13th Minor 9th";
static const char ChordTypeLong_35[] PROGMEM = "13 Minor 9 Dim 5";

// An array of pointers to the long chord type strings.
static const char* const ChordLongStrings[] PROGMEM = 
{
    ChordTypeLong_00, ChordTypeLong_01, ChordTypeLong_02, ChordTypeLong_03,
    ChordTypeLong_04, ChordTypeLong_05, ChordTypeLong_06, ChordTypeLong_07,
    ChordTypeLong_08, ChordTypeLong_09, ChordTypeLong_10, ChordTypeLong_11,
    ChordTypeLong_12, ChordTypeLong_13, ChordTypeLong_14, ChordTypeLong_15,
    ChordTypeLong_16, ChordTypeLong_17, ChordTypeLong_18, ChordTypeLong_19,
    ChordTypeLong_20, ChordTypeLong_21, ChordTypeLong_22, ChordTypeLong_23,
    ChordTypeLong_24, ChordTypeLong_25, ChordTypeLong_26, ChordTypeLong_27,
    ChordTypeLong_28, ChordTypeLong_29, ChordTypeLong_30, ChordTypeLong_31,
    ChordTypeLong_32, ChordTypeLong_33, ChordTypeLong_34, ChordTypeLong_35
};

// Unused string display option strings.
static const char UnplayedOption_00[] PROGMEM = "LIGHT ALL FRETS";
static const char UnplayedOption_01[] PROGMEM = " LIGHT UNPLAYED";
static const char UnplayedOption_02[] PROGMEM = "  LIGHT PLAYED ";
static const char UnplayedOption_03[] PROGMEM = "     IGNORE    ";

// An array of pointers to the unused string option strings.
static const char* const UnplayedOptionStrings[] PROGMEM = 
{
    UnplayedOption_00, UnplayedOption_01, UnplayedOption_02, UnplayedOption_03
};
 

// Unused demo display option strings.
static const char DemoOption_00[] PROGMEM = "   CONTINUOUS  ";
static const char DemoOption_01[] PROGMEM = " REPEAT CURRENT";
static const char DemoOption_02[] PROGMEM = "     RANDOM    ";

// An array of pointers to the unused demo option strings.
static const char* const DemoOptionStrings[] PROGMEM = 
{
    DemoOption_00, DemoOption_01, DemoOption_02
};




// An array of pointers to the demo mode state strings.
//                                          "   DEMO MODE    "
static const char DemoMode_00[] PROGMEM = "     ALL ON     ";
static const char DemoMode_01[] PROGMEM = "    ALL OFF     ";
static const char DemoMode_02[] PROGMEM = "  WALK LED ON   ";
static const char DemoMode_03[] PROGMEM = "  SWEEP LED ON  ";
static const char DemoMode_04[] PROGMEM = "  WALK LED OFF  ";
static const char DemoMode_05[] PROGMEM = " SWEEP LED OFF  ";
static const char DemoMode_06[] PROGMEM = "  WALK FRET ON  ";
static const char DemoMode_07[] PROGMEM = " SWEEP FRET ON  ";
static const char DemoMode_08[] PROGMEM = " WALK FRET OFF  ";
static const char DemoMode_09[] PROGMEM = " SWEEP FRET OFF ";
static const char DemoMode_10[] PROGMEM = " WALK STRING ON ";
static const char DemoMode_11[] PROGMEM = "SWEEP STRING ON ";
static const char DemoMode_12[] PROGMEM = "WALK STRING OFF ";
static const char DemoMode_13[] PROGMEM = "SWEEP STRING OFF";
static const char DemoMode_14[] PROGMEM = "  RANDOM CHORD  ";

// An array of pointers to the unused string option strings.
static const char* const DemoModeStrings[] PROGMEM = 
{
    DemoMode_00, DemoMode_01, DemoMode_02, DemoMode_03, DemoMode_04,
    DemoMode_05, DemoMode_06, DemoMode_07, DemoMode_08, DemoMode_09,
    DemoMode_10, DemoMode_11, DemoMode_12, DemoMode_13, DemoMode_14
};


// This method returns the specified key string into the caller's specified buffer.
// The method's return value is the pointer to the caller's buffer that was
// passed in pBuf if successful.  If the requested index is out of bounds, a
// NULL is returned.  This can be useful when using the method in a print statement,
// for example.
char *Strings::GetKeyString(unsigned index, char *pBuf)
{
    // Make sure the key value is valid.  If not, return NULL.
    if (index >= sizeof(KeyStrings) / sizeof(KeyStrings[0]))
    {
        return (char *)0;
    }
    
    // Copy the specified string from FLASH into the caller's buffer.
    strcpy_P(pBuf, (char *)pgm_read_word(&KeyStrings[index]));
    
    // Return the caller's buffer pointer.
    return pBuf;
}
 
 
// This method returns the specified short type string into the caller's
// specified buffer.  The method's return value is the pointer to the caller's
// buffer that was passed in pBuf if successful.  If the requested index is out
// of bounds, a NULL is returned.  This can be useful when using the method in
// a print statement, for example.
char *Strings::GetShortTypeString(unsigned index, char *pBuf)
{
    // Make sure the short type value is valid.  If not, return NULL.
    if (index >= sizeof(ChordShortStrings) / sizeof(ChordShortStrings[0]))
    {
        return (char *)0;
    }
    
    // Copy the specified string from FLASH into the caller's buffer.    
    strcpy_P(pBuf, (char *)pgm_read_word(&ChordShortStrings[index]));

    // Return the caller's buffer pointer.
    return pBuf;
}
    
// This method returns the specified long type string into the caller's
// specified buffer.  The method's return value is the pointer to the caller's
// buffer that was passed in pBuf if successful.  If the requested index is out
// of bounds, a NULL is returned.  This can be useful when using the method in
// a print statement, for example.
char *Strings::GetLongTypeString(unsigned index, char *pBuf)
{
    // Make sure the short type value is valid.  If not, return NULL.
    if (index >= sizeof(ChordLongStrings) / sizeof(ChordLongStrings[0]))
    {
        return (char *)0;
    }

    // Copy the specified string from FLASH into the caller's buffer.    
    strcpy_P(pBuf, (char *)pgm_read_word(&ChordLongStrings[index]));

    // Return the caller's buffer pointer.
    return pBuf;
}

// This method returns the specified unplayed option string into the caller's 
// specified buffer.  The method's return value is the pointer to the caller's
// buffer that was passed in pBuf if successful.  If the requested index is out
// of bounds, a NULL is returned.  This can be useful when using the method in
// a print statement, for example.
char *Strings::GetUnplayedOptionString(unsigned index, char *pBuf)
{
    // Make sure the key value is valid.  If not, return NULL.
    if (index >= sizeof(UnplayedOptionStrings) / sizeof(UnplayedOptionStrings[0]))
    {
        return (char *)0;
    }
    
    // Copy the specified string from FLASH into the caller's buffer.
    strcpy_P(pBuf, (char *)pgm_read_word(&UnplayedOptionStrings[index]));
    
    // Return the caller's buffer pointer.
    return pBuf;
}
 
// This method returns the specified demo mode string into the caller's 
// specified buffer.  The method's return value is the pointer to the caller's
// buffer that was passed in pBuf if successful.  If the requested index is out
// of bounds, a NULL is returned.  This can be useful when using the method in
// a print statement, for example.
char *Strings::GetDemoModeString(unsigned index, char *pBuf)
{
    // Make sure the key value is valid.  If not, return NULL.
    if (index >= sizeof(DemoModeStrings) / sizeof(DemoModeStrings[0]))
    {
        return (char *)0;
    }
    
    // Copy the specified string from FLASH into the caller's buffer.
    strcpy_P(pBuf, (char *)pgm_read_word(&DemoModeStrings[index]));
    
    // Return the caller's buffer pointer.
    return pBuf;
}
 
// This method returns the specified demo OPTION string into the caller's 
// specified buffer.  The method's return value is the pointer to the caller's
// buffer that was passed in pBuf if successful.  If the requested index is out
// of bounds, a NULL is returned.  This can be useful when using the method in
// a print statement, for example.
char *Strings::GetDemoOptionString(unsigned index, char *pBuf)
{
    // Make sure the key value is valid.  If not, return NULL.
    if (index >= sizeof(DemoOptionStrings) / sizeof(DemoOptionStrings[0]))
    {
        return (char *)0;
    }
    
    // Copy the specified string from FLASH into the caller's buffer.
    strcpy_P(pBuf, (char *)pgm_read_word(&DemoOptionStrings[index]));
    
    // Return the caller's buffer pointer.
    return pBuf;
}
 
