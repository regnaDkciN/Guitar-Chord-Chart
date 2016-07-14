/////////////////////////////////////////////////////////////////////////////////
// ChordChartData.h
//
// Defines several classes and constants used to describe and save chord 
// information.  This file contains a reduced number of chords.
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

#if !defined CHORDCHARTDATA_H
#define CHORDCHARTDATA_H

#include <avr/pgmspace.h>       // For uintXX_t.


const unsigned NUM_CHORD_FRETS             = 5;
const unsigned NUM_COMMON_CHORDS           = 8;
const unsigned NUM_COMMON_CHORD_VARIATIONS = 2;
const unsigned NUM_SUPP_CHORDS             = 0; 
const unsigned NUM_SUPP_CHORD_VARIATIONS   = 0;
const unsigned NUM_KEYS                    = 12;
const unsigned TOTAL_CHORDS_PER_KEY        = NUM_COMMON_CHORDS + NUM_SUPP_CHORDS;

/////////////////////////////////////////////////////////////////////////////////
// Chord class.  Defines the data pertaining to a single chord instance.
/////////////////////////////////////////////////////////////////////////////////
class Chord
{
public:
    uint8_t GetFret()          const { return m_Fret; }
    uint8_t GetUnplayed()      const { return m_Unplayed; }
    uint8_t *GetPatternPtr()   const { return const_cast<uint8_t *>(m_Chord); }
    void    SetFret(uint8_t fret)    { m_Fret = fret; }
    void    SetUnplayed(uint8_t val) { m_Unplayed = val; }

protected:
    
private:
    uint8_t m_Fret;                     // Starting fret of the chord.
    uint8_t m_Unplayed;                 // Bit pattern of unplayed strings.
    uint8_t m_Chord[NUM_CHORD_FRETS];   // Chord layout.  Each element represents a fret.
                                        // starting at m_Fret.
};

/////////////////////////////////////////////////////////////////////////////////
// CommonChord class.  Defines all the variations for a common chord instance.
/////////////////////////////////////////////////////////////////////////////////
class CommonChord
{
public:
    Chord m_Variation[NUM_COMMON_CHORD_VARIATIONS];
protected:
    
private:
};

/////////////////////////////////////////////////////////////////////////////////
// SuppChord class.  Defines all the variations for a supplemental chord instance.
/////////////////////////////////////////////////////////////////////////////////
class SuppChord
{
public:
    Chord m_Variation[NUM_SUPP_CHORD_VARIATIONS];
protected:
    
private:
};


/////////////////////////////////////////////////////////////////////////////////
// Key class.  Defines all the chords for a single key (C, C#, ...).
/////////////////////////////////////////////////////////////////////////////////
class Key
{
public:
    CommonChord  m_CommonChord[NUM_COMMON_CHORDS];
    SuppChord    m_SuppChord[NUM_SUPP_CHORDS];
protected:
    
private:
};

/////////////////////////////////////////////////////////////////////////////////
// ChordUnion.  Union that allows a chord to be accessed as an array characters,
// or visa versa.
/////////////////////////////////////////////////////////////////////////////////
union ChordUnion
{
    Chord   uChord;
    uint8_t uUint8Array[sizeof(Chord)];
};



/////////////////////////////////////////////////////////////////////////////////
// ChordChartData class.  This class manages fetching chords from FLASH and
// traversing the chord array.
/////////////////////////////////////////////////////////////////////////////////
class ChordChartData
{
public:
    ChordChartData() : m_CurrentKey(0), m_CurrentType(0), m_CurrentVariation(0)
    {
        GetChord(m_CurrentKey, m_CurrentType, m_CurrentVariation);
    }
    
    ~ChordChartData() {}
    
    void GetChord(unsigned key, unsigned type, unsigned variation);    
    bool NextChord();
    bool PreviousChord();    
    void NextKey(bool wrap = true);
    void PreviousKey(bool wrap = true);
    void NextType(bool wrap = true);
    void PreviousType(bool wrap = true);
    void NextVariation(bool wrap = true);
    void PreviousVariation(bool wrap = true);

    unsigned GetCurrentKey()       const { return m_CurrentKey; }
    unsigned GetCurrentType()      const { return m_CurrentType; }
    unsigned GetCurrentVariation() const { return m_CurrentVariation; }
    uint8_t  GetCurrentFret()      const { return m_Chord.GetFret(); }
    uint8_t  GetCurrentUnplayed()  const { return m_Chord.GetUnplayed(); }
    uint8_t *GetChordPatternPtr()  const { return m_Chord.GetPatternPtr(); }
    Chord   *GetChordPtr()         const { return const_cast<Chord *>(&m_Chord); }
    
protected:
    
private:

    unsigned m_CurrentKey;
    unsigned m_CurrentType;
    unsigned m_CurrentVariation;
    Chord    m_Chord;
};


#endif // CHORDCHARTDATA_H
