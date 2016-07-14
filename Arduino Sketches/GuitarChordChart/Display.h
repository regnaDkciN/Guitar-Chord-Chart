/////////////////////////////////////////////////////////////////////////////////
// Display.h
//
// This class implements the display class.  It mainly contains methods to 
// display data and strings on the LCD.
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

#if !defined DISPLAY_H
#define DISPLAY_H


#include <LiquidCrystal.h>          // For LCD support.
#include "Arduino.h"                // For Arduino specific definitions.
#include "ChordChartData.h"         // For the chord data.

/////////////////////////////////////////////////////////////////////////////////
// Display class.
/////////////////////////////////////////////////////////////////////////////////
class Display
{
public:
    static   Display *Instance();
    void     Initialize(uint8_t *pLedArray, LiquidCrystal *pLcd);
    
    void     SetAllLeds(bool state);
    void     DisplayChord(ChordChartData *pData);
    void     PowerupDisplay();
    void     DispLcdProgmem(
                  const __FlashStringHelper *pStr, bool clear = true, unsigned row = 0, unsigned col = 0);    
    void     DispLcd(
                  const char *pStr, bool clear = true, unsigned row = 0, unsigned col = 0);
    void     DispLcdInt(
                  uint32_t val, bool clear = true, unsigned row = 0, unsigned col = 0);
    void     DispLcdIntHex(uint32_t val, bool clear, unsigned row, unsigned col);
    void     DisplayLeds(Chord *pChord);
    bool     GetVerbose() const              { return m_Verbose; }
    void     SetVerbose(bool val)            { m_Verbose = val; }
    unsigned GetUnplayedOption() const       { return m_UnplayedOption; }
    void     SetUnplayedOption(unsigned val) { m_UnplayedOption = val; }
    uint8_t  *GetLedArrayPtr()               { return m_pLedArray; }
        
    // Unplayed option values.
    static const unsigned LIGHT_ALL_FRETS        = 0; // For unused strings, light all frets of that string.
    static const unsigned LIGHT_LAST_FRET_UNUSED = 1; // For unused strings, light UNUSED strings on last fret.
    static const unsigned LIGHT_LAST_FRET_USED   = 2; // For unused strings, light   USED strings on last fret.
    static const unsigned IGNORE                 = 3; // Ignore unused strings.  Do nothing to mark them.
    static const unsigned FIRST_UNPLAYED_OPTION  = LIGHT_ALL_FRETS;
    static const unsigned NUM_UNPLAYED_OPTIONS   = IGNORE + 1;                                         


protected:
    
private:
    Display() :
        m_pLedArray(NULL), m_pLcd(NULL),
        m_Verbose(true), m_UnplayedOption(LIGHT_LAST_FRET_UNUSED)
        { }
    ~Display() { }
    
    // Unimplemented methods
    Display(Display &rMgr);
    Display &operator=(Display &rMgr);

    
    uint8_t       *m_pLedArray;
    LiquidCrystal *m_pLcd;
    bool           m_Verbose;
    unsigned       m_UnplayedOption;
};



#endif // DISPLAY_H

