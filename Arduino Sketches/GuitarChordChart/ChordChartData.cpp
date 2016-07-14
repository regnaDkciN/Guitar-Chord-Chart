/////////////////////////////////////////////////////////////////////////////////
// ChordChartData.cpp
//
// Contains data and methods defined by the ChordChartData class.  
// The data in this class represents multiple variations of various guitar
// chords, and are stored in FLASH in order to save RAM.  The methods of the
// class manage access to the chord data saved in FLASH.
//
// This file contains a reduced number of chords.
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

#include "ChordChartData.h"



///////////////////////////////////////////////////////////////////////////////
// The data for the following chord table was taken from 
// "CompleteGuitarChordPoster_WayneChase_FreeEdition.pdf" found at
// www.HowMusicReallyWorks.com.
// 
// This version of the chord data normalizes all chords by making the first
// displayed fret always contain fingering positions.  For example, the first
// variant of C9 is displayed as starting at fret 2, rather than starting at
// fret 1 with fret 1 empty.  This might help in the future with reverse 
// chord lookup.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
static const uint8_t Keys[sizeof(Key) * NUM_KEYS] PROGMEM =
///////////////////////////////////////////////////////////////////////////////
{
///////////////////////////////////////////////////////////////////////////////
// C
///////////////////////////////////////////////////////////////////////////////

    // C Major
    1, 0x00, 0x02, 0x08, 0x30, 0x00, 0x00,     3, 0x00, 0x31, 0x00, 0x0e, 0x00, 0x00,

    // C Minor
    1, 0x30, 0x0a, 0x00, 0x01, 0x00, 0x00,     3, 0x00, 0x31, 0x02, 0x0c, 0x00, 0x00,
    
    // C Sixth
    1, 0x00, 0x02, 0x0c, 0x01, 0x00, 0x00,     5, 0x00, 0x0f, 0x00, 0x10, 0x20, 0x00,
    
    // C Minor Sixth
    1, 0x20, 0x0a, 0x00, 0x01, 0x00, 0x00,     4, 0x30, 0x02, 0x0d, 0x00, 0x00, 0x00,
    
    // C Seventh
    1, 0x00, 0x02, 0x08, 0x34, 0x00, 0x00,     3, 0x00, 0x35, 0x00, 0x0a, 0x00, 0x00,
    
    // C Major Seventh
    2, 0x00, 0x08, 0x30, 0x00, 0x00, 0x00,     3, 0x00, 0x31, 0x04, 0x0a, 0x00, 0x00,
    
    // C Minor Seventh
    1, 0x20, 0x0a, 0x00, 0x15, 0x00, 0x00,     3, 0x00, 0x35, 0x02, 0x08, 0x00, 0x00,
    
    // C Ninth
    2, 0x00, 0x08, 0x37, 0x00, 0x00, 0x00,     5, 0x20, 0x1e, 0x01, 0x00, 0x00, 0x00,
    
    

///////////////////////////////////////////////////////////////////////////////
// C# 
///////////////////////////////////////////////////////////////////////////////

    // C Sharp Major
    1, 0x00, 0x25, 0x02, 0x08, 0x10, 0x00,     4, 0x00, 0x31, 0x00, 0x0e, 0x00, 0x00,

    // C Sharp Minor
    1, 0x30, 0x04, 0x0a, 0x00, 0x00, 0x00,     4, 0x00, 0x31, 0x02, 0x0c, 0x00, 0x00,
    
    // C Sharp Sixth
    1, 0x00, 0x34, 0x02, 0x08, 0x01, 0x00,     6, 0x00, 0x0f, 0x00, 0x10, 0x20, 0x00,
    
    // C Sharp Minor Sixth
    1, 0x20, 0x14, 0x0a, 0x00, 0x00, 0x00,     2, 0x20, 0x0a, 0x04, 0x11, 0x00, 0x00,
    
    // C Sharp Seventh
    2, 0x30, 0x02, 0x08, 0x05, 0x00, 0x00,     4, 0x00, 0x35, 0x00, 0x0a, 0x00, 0x00,
    
    // C Sharp Major Seventh
    1, 0x00, 0x27, 0x00, 0x08, 0x10, 0x00,     4, 0x00, 0x31, 0x04, 0x0a, 0x00, 0x00,
    
    // C Sharp Minor Seventh
    1, 0x20, 0x04, 0x08, 0x00, 0x10, 0x00,     2, 0x20, 0x0a, 0x00, 0x15, 0x00, 0x00,
    
    // C Sharp Ninth
    3, 0x00, 0x08, 0x37, 0x00, 0x00, 0x00,     6, 0x20, 0x1e, 0x01, 0x00, 0x00, 0x00,
    
///////////////////////////////////////////////////////////////////////////////
// D 
///////////////////////////////////////////////////////////////////////////////

    // D Major
    2, 0x00, 0x25, 0x02, 0x00, 0x00, 0x00,     5, 0x00, 0x31, 0x00, 0x0e, 0x00, 0x00,

    // D Minor
    1, 0x00, 0x21, 0x04, 0x02, 0x00, 0x00,     5, 0x00, 0x31, 0x02, 0x0c, 0x00, 0x00,
    
    // D Sixth
    2, 0x00, 0x25, 0x00, 0x00, 0x00, 0x00,     2, 0x00, 0x34, 0x02, 0x08, 0x01, 0x00,
    
    // D Minor Sixth
    1, 0x00, 0x21, 0x04, 0x00, 0x00, 0x00,     3, 0x20, 0x0a, 0x04, 0x11, 0x00, 0x00,
    
    // D Seventh
    1, 0x00, 0x02, 0x25, 0x00, 0x00, 0x00,     3, 0x30, 0x02, 0x08, 0x05, 0x00, 0x00,
    
    // D Major Seventh
    2, 0x00, 0x27, 0x00, 0x00, 0x00, 0x00,     5, 0x00, 0x31, 0x04, 0x0a, 0x00, 0x00,
    
    // D Minor Seventh
    1, 0x20, 0x03, 0x04, 0x00, 0x00, 0x00,     3, 0x20, 0x0a, 0x00, 0x15, 0x00, 0x00,
    
    // D Ninth
    1, 0x00, 0x02, 0x24, 0x00, 0x00, 0x00,     4, 0x00, 0x08, 0x37, 0x00, 0x00, 0x00,
    
///////////////////////////////////////////////////////////////////////////////
// Eb 
///////////////////////////////////////////////////////////////////////////////

    // E Flat Major
    3, 0x00, 0x25, 0x02, 0x08, 0x10, 0x00,     6, 0x00, 0x31, 0x00, 0x0e, 0x00, 0x00,

    // E Flat Minor
    1, 0x20, 0x18, 0x01, 0x04, 0x02, 0x00,     2, 0x30, 0x01, 0x04, 0x0a, 0x00, 0x00,
    
    // E Flat Sixth
    1, 0x20, 0x1a, 0x00, 0x05, 0x00, 0x00,     3, 0x00, 0x34, 0x02, 0x08, 0x01, 0x00,
    
    // E Flat Minor Sixth
    1, 0x20, 0x1a, 0x01, 0x04, 0x00, 0x00,     4, 0x20, 0x0a, 0x04, 0x11, 0x00, 0x00,
    
    // E Flat Seventh
    1, 0x20, 0x18, 0x02, 0x05, 0x00, 0x00,     4, 0x30, 0x02, 0x08, 0x05, 0x00, 0x00,
    
    // E Flat Major Seventh
    3, 0x00, 0x27, 0x00, 0x08, 0x10, 0x00,     6, 0x00, 0x31, 0x04, 0x0a, 0x00, 0x00,
    
    // E Flat Minor Seventh
    1, 0x20, 0x18, 0x03, 0x04, 0x00, 0x00,     4, 0x20, 0x0a, 0x00, 0x15, 0x00, 0x00,
    
    // E Flat Ninth
    1, 0x20, 0x19, 0x02, 0x00, 0x00, 0x00,     1, 0x00, 0x19, 0x02, 0x24, 0x00, 0x00,
    
///////////////////////////////////////////////////////////////////////////////
// E 
///////////////////////////////////////////////////////////////////////////////
    
    // E Major
    1, 0x00, 0x04, 0x18, 0x00, 0x00, 0x00,     4, 0x00, 0x25, 0x02, 0x08, 0x10, 0x00,

    // E Minor
    2, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00,     2, 0x20, 0x18, 0x01, 0x04, 0x02, 0x00,
    
    // E Sixth
    1, 0x00, 0x04, 0x1a, 0x00, 0x00, 0x00,     2, 0x00, 0x1a, 0x00, 0x05, 0x00, 0x00,
    
    // E Minor Sixth
    2, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00,     2, 0x20, 0x1a, 0x01, 0x04, 0x00, 0x00,
    
    // E Seventh
    1, 0x00, 0x04, 0x18, 0x02, 0x00, 0x00,     2, 0x00, 0x18, 0x02, 0x05, 0x00, 0x00,
    
    // E Major Seventh
    1, 0x00, 0x0c, 0x10, 0x00, 0x00, 0x00,     4, 0x00, 0x27, 0x00, 0x08, 0x10, 0x00,
    
    // E Minor Seventh
    2, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,     2, 0x20, 0x18, 0x03, 0x04, 0x00, 0x00,
    
    // E Ninth
    1, 0x00, 0x04, 0x11, 0x00, 0x00, 0x00,     2, 0x00, 0x19, 0x02, 0x24, 0x00, 0x00,
    
///////////////////////////////////////////////////////////////////////////////
// F 
///////////////////////////////////////////////////////////////////////////////

    // F Major
    1, 0x00, 0x23, 0x04, 0x18, 0x00, 0x00,     5, 0x00, 0x25, 0x02, 0x08, 0x10, 0x00,

    // F Minor
    1, 0x00, 0x27, 0x00, 0x18, 0x00, 0x00,     3, 0x20, 0x18, 0x01, 0x04, 0x02, 0x00,
    
    // F Sixth
    1, 0x00, 0x23, 0x04, 0x00, 0x00, 0x00,     3, 0x20, 0x1a, 0x00, 0x05, 0x00, 0x00,
    
    // F Minor Sixth
    1, 0x30, 0x07, 0x00, 0x00, 0x00, 0x00,     1, 0x00, 0x25, 0x00, 0x1a, 0x00, 0x00,
    
    // F Seventh
    1, 0x00, 0x29, 0x04, 0x10, 0x02, 0x00,     3, 0x20, 0x18, 0x02, 0x05, 0x00, 0x00,
    
    // F Major Seventh
    1, 0x00, 0x22, 0x04, 0x18, 0x00, 0x00,     5, 0x00, 0x27, 0x00, 0x08, 0x10, 0x00,
    
    // F Minor Seventh
    1, 0x00, 0x2f, 0x00, 0x10, 0x00, 0x00,     3, 0x20, 0x18, 0x03, 0x04, 0x00, 0x00,
    
    // F Ninth
    1, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x00,     1, 0x00, 0x2a, 0x04, 0x11, 0x00, 0x00,
    
///////////////////////////////////////////////////////////////////////////////
// F# 
///////////////////////////////////////////////////////////////////////////////

    // F Sharp Major
    2, 0x00, 0x23, 0x04, 0x18, 0x00, 0x00,     6, 0x00, 0x25, 0x02, 0x08, 0x10, 0x00,

    // F Sharp Minor
    2, 0x00, 0x27, 0x00, 0x18, 0x00, 0x00,     4, 0x20, 0x18, 0x01, 0x04, 0x02, 0x00,
    
    // F Sharp Sixth
    1, 0x20, 0x18, 0x03, 0x04, 0x00, 0x00,     4, 0x20, 0x1a, 0x00, 0x05, 0x00, 0x00,
    
    // F Sharp Minor Sixth
    1, 0x00, 0x08, 0x27, 0x00, 0x00, 0x00,     2, 0x00, 0x25, 0x00, 0x1a, 0x00, 0x00,
    
    // F Sharp Seventh
    2, 0x00, 0x22, 0x04, 0x18, 0x00, 0x00,     2, 0x00, 0x29, 0x04, 0x10, 0x02, 0x00,
    
    // F Sharp Major Seventh
    1, 0x20, 0x11, 0x02, 0x04, 0x08, 0x00,     2, 0x00, 0x23, 0x0c, 0x10, 0x00, 0x00,
    
    // F Sharp Minor Seventh
    2, 0x00, 0x2f, 0x00, 0x10, 0x00, 0x00,     4, 0x20, 0x18, 0x03, 0x04, 0x00, 0x00,
    
    // F Sharp Ninth
    1, 0x20, 0x14, 0x0b, 0x00, 0x00, 0x00,     2, 0x00, 0x2a, 0x04, 0x11, 0x00, 0x00,
    
///////////////////////////////////////////////////////////////////////////////
// G 
///////////////////////////////////////////////////////////////////////////////
    
    // G Major
    2, 0x00, 0x10, 0x21, 0x00, 0x00, 0x00,     3, 0x00, 0x23, 0x04, 0x18, 0x00, 0x00,

    // G Minor
    3, 0x00, 0x27, 0x00, 0x18, 0x00, 0x00,     5, 0x20, 0x18, 0x01, 0x04, 0x02, 0x00,
    
    // G Sixth
    2, 0x00, 0x10, 0x20, 0x00, 0x00, 0x00,     3, 0x00, 0x22, 0x04, 0x18, 0x00, 0x00,
    
    // G Minor Sixth
    1, 0x20, 0x10, 0x08, 0x02, 0x00, 0x00,     2, 0x30, 0x08, 0x07, 0x00, 0x00, 0x00,
    
    // G Seventh
    1, 0x00, 0x01, 0x10, 0x20, 0x00, 0x00,     3, 0x00, 0x29, 0x04, 0x10, 0x02, 0x00,
    
    // G Major Seventh
    2, 0x00, 0x11, 0x20, 0x00, 0x00, 0x00,     2, 0x20, 0x11, 0x02, 0x04, 0x08, 0x00,
    
    // G Minor Seventh
    3, 0x00, 0x2f, 0x00, 0x10, 0x00, 0x00,     5, 0x20, 0x18, 0x03, 0x04, 0x00, 0x00,
    
    // G Ninth
    1, 0x00, 0x01, 0x14, 0x20, 0x00, 0x00,     2, 0x20, 0x14, 0x0b, 0x00, 0x00, 0x00,
    
///////////////////////////////////////////////////////////////////////////////
// Ab 
///////////////////////////////////////////////////////////////////////////////
   
    // A Flat Major
    1, 0x00, 0x0e, 0x00, 0x10, 0x21, 0x00,     4, 0x00, 0x23, 0x04, 0x18, 0x00, 0x00,

    // A Flat Minor
    4, 0x00, 0x27, 0x00, 0x18, 0x00, 0x00,     6, 0x20, 0x18, 0x01, 0x04, 0x02, 0x00,
    
    // A Flat Sixth
    1, 0x00, 0x0f, 0x00, 0x10, 0x20, 0x00,     3, 0x20, 0x18, 0x03, 0x04, 0x00, 0x00,
    
    // A Flat Minor Sixth
    1, 0x00, 0x2d, 0x10, 0x00, 0x00, 0x00,     3, 0x30, 0x08, 0x07, 0x00, 0x00, 0x00,
    
    // A Flat Seventh
    1, 0x30, 0x0e, 0x01, 0x00, 0x00, 0x00,     4, 0x00, 0x29, 0x04, 0x10, 0x02, 0x00,
    
    // A Flat Major Seventh
    1, 0x20, 0x0e, 0x00, 0x11, 0x00, 0x00,     3, 0x20, 0x11, 0x02, 0x04, 0x08, 0x00,
    
    // A Flat Minor Seventh
    1, 0x30, 0x0c, 0x01, 0x00, 0x00, 0x00,     4, 0x00, 0x2f, 0x00, 0x10, 0x00, 0x00,
    
    // A Flat Ninth
    1, 0x20, 0x1e, 0x01, 0x00, 0x00, 0x00,     3, 0x20, 0x14, 0x0b, 0x00, 0x00, 0x00,
    
///////////////////////////////////////////////////////////////////////////////
// A 
///////////////////////////////////////////////////////////////////////////////

    // A Major
    2, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00,     2, 0x00, 0x0e, 0x00, 0x10, 0x21, 0x00,

    // A Minor
    1, 0x00, 0x02, 0x0c, 0x00, 0x00, 0x00,     5, 0x00, 0x27, 0x00, 0x18, 0x00, 0x00,
    
    // A Sixth
    2, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00,     4, 0x20, 0x18, 0x03, 0x04, 0x00, 0x00,
    
    // A Minor Sixth
    1, 0x00, 0x02, 0x0d, 0x00, 0x00, 0x00,     4, 0x20, 0x08, 0x07, 0x00, 0x00, 0x00,
    
    // A Seventh
    2, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00,     5, 0x00, 0x29, 0x04, 0x10, 0x02, 0x00,
    
    // A Major Seventh
    1, 0x00, 0x04, 0x0a, 0x00, 0x00, 0x00,     4, 0x20, 0x11, 0x02, 0x04, 0x08, 0x00,
    
    // A Minor Seventh
    1, 0x00, 0x02, 0x08, 0x00, 0x00, 0x00,     5, 0x00, 0x2f, 0x00, 0x10, 0x00, 0x00,
    
    // A Ninth
    2, 0x00, 0x0a, 0x01, 0x04, 0x00, 0x00,     4, 0x20, 0x14, 0x0b, 0x00, 0x00, 0x00,
    
///////////////////////////////////////////////////////////////////////////////
// Bb 
///////////////////////////////////////////////////////////////////////////////
    
    // B Flat Major
    1, 0x00, 0x31, 0x00, 0x0e, 0x00, 0x00,     3, 0x00, 0x0e, 0x00, 0x10, 0x21, 0x00,

    // B Flat Minor
    1, 0x00, 0x31, 0x02, 0x0c, 0x00, 0x00,     6, 0x00, 0x27, 0x00, 0x18, 0x00, 0x00,
    
    // B Flat Sixth
    3, 0x00, 0x0f, 0x00, 0x10, 0x20, 0x00,     5, 0x20, 0x18, 0x03, 0x04, 0x00, 0x00,
    
    // B Flat Minor Sixth
    2, 0x30, 0x02, 0x0d, 0x00, 0x00, 0x00,     5, 0x30, 0x08, 0x07, 0x00, 0x00, 0x00,
    
    // B Flat Seventh
    1, 0x00, 0x35, 0x00, 0x0a, 0x00, 0x00,     3, 0x30, 0x0e, 0x01, 0x00, 0x00, 0x00,
    
    // B Flat Major Seventh
    1, 0x00, 0x31, 0x04, 0x0a, 0x00, 0x00,     3, 0x20, 0x0e, 0x00, 0x11, 0x00, 0x00,
    
    // B Flat Minor Seventh
    1, 0x00, 0x35, 0x02, 0x08, 0x00, 0x00,     6, 0x00, 0x2f, 0x00, 0x10, 0x00, 0x00,
    
    // B Flat Ninth
    1, 0x00, 0x37, 0x00, 0x00, 0x00, 0x00,     3, 0x20, 0x1e, 0x01, 0x00, 0x00, 0x00,
    
///////////////////////////////////////////////////////////////////////////////
// B 
///////////////////////////////////////////////////////////////////////////////

    // B Major
    2, 0x00, 0x31, 0x00, 0x0e, 0x00, 0x00,     4, 0x00, 0x0e, 0x00, 0x10, 0x21, 0x00,

    // B Minor
    2, 0x00, 0x31, 0x02, 0x0c, 0x00, 0x00,     7, 0x00, 0x27, 0x00, 0x18, 0x00, 0x00,
    
    // B Sixth
    1, 0x30, 0x0c, 0x01, 0x00, 0x00, 0x00,     4, 0x00, 0x0f, 0x00, 0x10, 0x20, 0x00,
    
    // B Minor Sixth
    1, 0x00, 0x04, 0x31, 0x00, 0x00, 0x00,     3, 0x30, 0x02, 0x0d, 0x00, 0x00, 0x00,
    
    // B Seventh
    1, 0x00, 0x08, 0x35, 0x00, 0x00, 0x00,     2, 0x00, 0x35, 0x00, 0x0a, 0x00, 0x00,
    
    // B Major Seventh
    2, 0x00, 0x31, 0x04, 0x0a, 0x00, 0x00,     4, 0x20, 0x0e, 0x00, 0x11, 0x00, 0x00,
    
    // B Minor Seventh
    2, 0x00, 0x35, 0x00, 0x00, 0x00, 0x00,     2, 0x00, 0x35, 0x02, 0x08, 0x00, 0x00,
    
    // B Ninth
    1, 0x00, 0x08, 0x37, 0x00, 0x00, 0x00,     4, 0x20, 0x1e, 0x01, 0x00, 0x00, 0x00    
};


// This method fills our Chord structure with the specified chord data from
// FLASH memory.  It also updates the current position data.
// key       - zero based index into the key array (starting with C).
// type      - zero based index into the type array.
// variation - zero based variation number of the specified chord.
void ChordChartData::GetChord(unsigned key, unsigned type, unsigned variation)
{
    // Limit the key value to valid values.
    if (key >= NUM_KEYS)
    {
        key = NUM_KEYS - 1;
    }
    
    // Index to the start of the specified key.
    unsigned index = key * sizeof(Key);

    // Index to the chord type within the specified key.
    if (type < NUM_COMMON_CHORDS)
    {
        // It's a common chord.  Index to it.
        index += type * sizeof(CommonChord);
        // Limit the variation value to valid values.
        if (variation >= NUM_COMMON_CHORD_VARIATIONS)
        {
            variation = NUM_COMMON_CHORD_VARIATIONS - 1;
        }
        // Index to the specified variation.
        index += variation * sizeof(Chord);
    }
    else
    {
        // It's a supplamental chord.  Limit the type to valid values.
        if (type >= TOTAL_CHORDS_PER_KEY)
        {
            type = TOTAL_CHORDS_PER_KEY - 1;
        }
        // Index to the start of the specified type.
        index += (NUM_COMMON_CHORDS * sizeof(CommonChord)) +
            (type - NUM_COMMON_CHORDS) * sizeof(SuppChord);
        // Limit the variation to valid values.
        if (variation >= NUM_SUPP_CHORD_VARIATIONS)
        {
            variation = NUM_SUPP_CHORD_VARIATIONS - 1;
        }
        // Index to the specified variation.
        index += variation * sizeof(Chord);
    }
  
    // Save the new current position data.
    m_CurrentKey       = key;
    m_CurrentType      = type;
    m_CurrentVariation = variation;
    
    // Copy the specified chord's data from FLASH to our buffer.
    memcpy_P(&m_Chord, &Keys[index], sizeof(Chord));
}


// This function increments the class's chord indices to point to the
// next sequential chord.  Returns true if successful, or false if an
// attempt was made to go beyond the last chord.
bool ChordChartData::NextChord()
{
    // Increment the current variation index.
    m_CurrentVariation++;

    // Is the current type a common chord?
    if (m_CurrentType < NUM_COMMON_CHORDS)
    {
        // Yes, it's a common chord.  See if the variation needs to wrap.
        if (m_CurrentVariation >= NUM_COMMON_CHORD_VARIATIONS)
        {
            // Variation wrapped. Increment the type, and reset the
            // variation to zero.
            m_CurrentVariation = 0;
            m_CurrentType++;
            if ((NUM_SUPP_CHORD_VARIATIONS == 0) && (m_CurrentType >= NUM_COMMON_CHORDS))
            {
                return false;
            }
        }
    }
    else // Must be supp chord.
    {
        // Do we need to wrap the incremented variation? 
        if (m_CurrentVariation >= NUM_SUPP_CHORD_VARIATIONS)
        {
            // Yes, wrap the variation and increment the type.
            m_CurrentVariation = 0;
            m_CurrentType++;
            // Do we need to wrap the type?
            if (m_CurrentType >= TOTAL_CHORDS_PER_KEY)
            {
                // Yes, reset the type, and bump the key.
                m_CurrentType = 0;
                m_CurrentKey++;
                // If we've gone beyond the last chord, just return false.
                if (m_CurrentKey >= NUM_KEYS)
                {
                    return false;
                }
            }
        }
    }
    
    //We've adjusted all the indices.  Now copy the chord from FLASH to the
    // user's buffer and return success.
    GetChord(m_CurrentKey, m_CurrentType, m_CurrentVariation);
    return true;
}


// This function decrements the class's chord indices to point to the
// previous sequential chord.  Returns true if successful, or false if an
// attempt was made to go before the first chord.
bool ChordChartData::PreviousChord()
{
    // If we're already at the first chord, just return false.
    if (!m_CurrentKey && !m_CurrentType && !m_CurrentVariation)
    {
        return false;
    }
    
    // Decrement the variation and see if we need to wrap.
    if (m_CurrentVariation-- == 0)
    {
        // We need to wrap.  Decrement the type and check for wrap.
        if (m_CurrentType-- == 0)
        {
            // Need to wrap.  Decrement the key and wrap the type.
            m_CurrentKey--;
            m_CurrentType = TOTAL_CHORDS_PER_KEY - 1;
        }
        // Don't forget to update the variation.  Wrap it based on the new type.
        if (m_CurrentType < NUM_COMMON_CHORDS)
        {
            m_CurrentVariation = NUM_COMMON_CHORD_VARIATIONS - 1;
        }
        else
        {
            m_CurrentVariation = NUM_SUPP_CHORD_VARIATIONS - 1;
        }
    }

    //We've adjusted all the indices.  Now copy the chord from FLASH to the
    // user's buffer and return success.
    GetChord(m_CurrentKey, m_CurrentType, m_CurrentVariation);
    return true;
}


// This method increments the current key, and returns the corresponding
// chord data to our Chord buffer.
// wrap - Set to true if the incremented key should wrap to the first key
//        when going beyond the last key. Set to false if the incremented key
//        should be limited to the last key and not wrap when already at the
//        last key.
void ChordChartData::NextKey(bool wrap)
{
    // Bump the key and see if it needs to wrap.
    if (++m_CurrentKey >= NUM_KEYS)
    {
        // We've gone beyond the last key.
        if (wrap)
        {
            // It's OK to wrap, so do so.
            m_CurrentKey = 0;
        }
        else
        {
            // It's not OK to wrap so set key to the last possible value.
            m_CurrentKey = NUM_KEYS - 1;
        }
    }
    
    // Reset the variation.
    m_CurrentVariation = 0;

    // We've adjusted the key.  Now copy the chord from FLASH to our
    // buffer and return.
    GetChord(m_CurrentKey, m_CurrentType, m_CurrentVariation);
}


// This method decrements the current key, and returns the corresponding
// chord data to our Chord buffer.
// wrap - Set to true if the decremented key should wrap to the last key
//        when going beyond the first key. Set to false if the decremented key
//        should be limited to the first key and not wrap when already at the
//        first key.
void ChordChartData::PreviousKey(bool wrap)
{
    // Decrement the key and see if it needs to wrap.
    if (m_CurrentKey-- == 0)
    {
        // We've decremented before the first key.
        if (wrap)
        {
            // It's OK to wrap, so do so.
            m_CurrentKey = NUM_KEYS - 1;
        }
        else
        {
            // It's not OK to wrap so set the key to 0.
            m_CurrentKey = 0;
        }
    }

    // Reset the variation.
    m_CurrentVariation = 0;

    // We've adjusted the key.  Now copy the chord from FLASH to our
    // buffer and return.
    GetChord(m_CurrentKey, m_CurrentType, m_CurrentVariation);
}


// This method increments the current type, and returns the corresponding
// chord data to our Chord buffer.
// wrap - Set to true if the incremented type should wrap to the first type
//        when going beyond the last key. Set to false if the incremented key
//        should be limited to the last type and not wrap when already at the
//        last type.
void ChordChartData::NextType(bool wrap)
{
    // Bump the type and see if it needs to wrap.
    if (++m_CurrentType >= TOTAL_CHORDS_PER_KEY)
    {
        // We've gone beyond the last type.
        if (wrap)
        {
            // It's OK to wrap, so do so.
            m_CurrentType = 0;
        }
        else
        {
            // It's not OK to wrap so set type to the last possible value.
            m_CurrentKey = TOTAL_CHORDS_PER_KEY - 1;
        }
    }

    // Reset the variation.
    m_CurrentVariation = 0;

    // We've adjusted the type.  Now copy the chord from FLASH to our
    // buffer and return.
    GetChord(m_CurrentKey, m_CurrentType, m_CurrentVariation);
}


// This method decrements the current type, and returns the corresponding
// chord data to our Chord buffer.
// wrap - Set to true if the decremented type should wrap to the last type
//        when going beyond the first type. Set to false if the decremented type
//        should be limited to the first type and not wrap when already at the
//        first type.
void ChordChartData::PreviousType(bool wrap)
{
    // Decrement the type and see if it needs to wrap.
    if (m_CurrentType-- == 0)
    {
        // We've decremented before the first type.
        if (wrap)
        {
            // It's OK to wrap, so do so.
            m_CurrentType = TOTAL_CHORDS_PER_KEY - 1;
        }
        else
        {
            // It's not OK to wrap so set the type to 0.
            m_CurrentType = 0;
        }
    }

    // Reset the variation.
    m_CurrentVariation = 0;

    // We've adjusted the type.  Now copy the chord from FLASH to our
    // buffer and return.
    GetChord(m_CurrentKey, m_CurrentType, m_CurrentVariation);
}


// This method increments the current variation, and returns the corresponding
// chord data to our chord buffer.
// wrap - Set to true if the incremented variation should wrap to the first variation
//        when going beyond the last variation.  Set to false if the incremented 
//        variation should be limited to the last variation and not wrap when 
//        already at the last variation.
void ChordChartData::NextVariation(bool wrap)
{
    // Bump the variation.
    m_CurrentVariation++;
    
    // We handle common and supplemental chords differently.
    if (m_CurrentType < NUM_COMMON_CHORDS)
    {
        // It's a common chord, check if we need to wrap.
        if (m_CurrentVariation >= NUM_COMMON_CHORD_VARIATIONS)
        {
            // We've gone beyond the last variation.
            if (wrap)
            {
                // It's OK to wrap, so do so.
                m_CurrentVariation = 0;
            }
            else
            {
                // It's not OK to wrap so set variation to the last possible value.
                m_CurrentVariation = NUM_COMMON_CHORD_VARIATIONS - 1;
            }
        }
    }
    else  // It's a supplemental chord.
    {
        // See if we need to wrap.
        if (m_CurrentVariation >= NUM_SUPP_CHORD_VARIATIONS)
        {
            // It's OK to wrap, so do so.
            if (wrap)
            {
                m_CurrentVariation = 0;
            }
            else
            {
                // It's not OK to wrap so set variation to the last possible value.
                m_CurrentVariation = NUM_SUPP_CHORD_VARIATIONS - 1;
            }
        }        
    }

    // We've adjusted the variation.  Now copy the chord from FLASH to our
    // buffer and return.
    GetChord(m_CurrentKey, m_CurrentType, m_CurrentVariation);
}


// This method decrements the current variation, and returns the corresponding
// chord data to our Chord buffer.
// wrap - Set to true if the decremented variation should wrap to the last variation
//        when going beyond the first variation.  Set to false if the decremented
//        variation should be limited to the first variation and not wrap when
//        already at the first variation.
void ChordChartData::PreviousVariation(bool wrap)
{
    // Decrement the variation and see if it needs to wrap.
    if (m_CurrentVariation-- == 0)
    {
        // We've decremented before the first variation.
        if (!wrap)
        {
            // It's not OK to wrap, so set the variation to 0.
            m_CurrentVariation = 0;
        }
        else if (m_CurrentType < NUM_COMMON_CHORDS)
        {
            // It's OK to wrap, and this is a common chord, so wrap.
            m_CurrentVariation = NUM_COMMON_CHORD_VARIATIONS - 1;
        }
        else
        {
            // It's OK to wrap, and this is a supplemental chord, so wrap.
            m_CurrentVariation = NUM_SUPP_CHORD_VARIATIONS - 1;
        }
    }

    // We've adjusted the variation.  Now copy the chord from FLASH to our
    // buffer and return.
    GetChord(m_CurrentKey, m_CurrentType, m_CurrentVariation);
}

