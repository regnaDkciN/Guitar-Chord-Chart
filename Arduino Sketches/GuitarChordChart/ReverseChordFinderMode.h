/////////////////////////////////////////////////////////////////////////////////
// ReverseChordFinderMode.h
//
// Defines the class that handles the reverse chord finding mode.  This mode
// allows the user to specify a base fret and a fingering pattern.  It then
// finds the specified chord, if possible, and reports the information regarding
// the chord that was found.
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

#if !defined REVERSECHORDFINDERMODE_H
#define REVERSECHORDFINDERMODE_H

#include "Mode.h"               // For Mode base class.
#include "IrCodes.h"            // For raw IR codes.
#include "ChordChartData.h"     // For the chord data.
#include "PolledDelay.h"        // For PolledDelay class.
#include "LedDriver.h"          // For NUM_STRINGS.


/////////////////////////////////////////////////////////////////////////////////
// ReverseChordFinderMode class.
/////////////////////////////////////////////////////////////////////////////////
class ReverseChordFinderMode : public Mode
{
public:
    static ReverseChordFinderMode *Instance();
    void Initialize()
    {
    }

    virtual uint32_t Execute(uint32_t irKey);
    virtual void     Startup();
    virtual void     Shutdown() { }
    
protected:
    
private:
    // Map raw IR keys to our specific use.
    // Change mapping to suit your taste.
    static const uint32_t PREV_FRET           = IrCodes::UP;      // Up
    static const uint32_t NEXT_FRET           = IrCodes::DOWN;    // Down
    static const uint32_t NEXT_STRING         = IrCodes::LEFT;    // Left
    static const uint32_t PREV_STRING         = IrCodes::RIGHT;   // Right
    static const uint32_t TOGGLE              = IrCodes::OK;      // Enter
    static const uint32_t SELECT              = IrCodes::OK;      // Enter
    static const uint32_t SEARCH              = IrCodes::PLAY;    // Play
    static const uint32_t SEARCH_NEXT         = IrCodes::FFWD;    // Ffwd
    static const uint32_t TOGGLE_TYPE_DISPLAY = IrCodes::INFO;    // Info
    static const uint32_t NEXT_UNPLAYED_OPT   = IrCodes::GUIDE;   // Guide
    static const uint32_t PATTERN_RIGHT       = IrCodes::RIGHT;   // Right
    static const uint32_t PATTERN_LEFT        = IrCodes::LEFT;    // Left
    
    // Sub-mode (state) definitions.
    static const uint8_t GETTING_STRING_MATCH_PATTERN_STATE = 0;
    static const uint8_t GETTING_FRET_STATE                 = 1;
    static const uint8_t GETTING_PATTERN_STATE              = 2;
    static const uint8_t WAITING_DONE_STATE                 = 3;
    
    // Miscellaneous constants.
    static const uint8_t  MAX_BASE_FRET   = 11;
    static const uint32_t LONG_DELAY      = 500;
    static const uint32_t SHORT_DELAY     = 125;
    
    
    // Unimplemented methods
    ReverseChordFinderMode(ReverseChordFinderMode &rMgr);
    ReverseChordFinderMode &operator=(ReverseChordFinderMode &rMgr);

    // Private constructor and destructor for singleton.
    ReverseChordFinderMode() : m_ChordData(), m_CurrentState(GETTING_STRING_MATCH_PATTERN_STATE), m_CurrentString(0),
                               m_CurrentFret(0), m_CurrentValue(0),
                               m_MatchPattern((1 << LedDriver::NUM_STRINGS) - 1),
                               m_Delay(LONG_DELAY), m_Chord()
    { 
        memset(&m_Chord, 0, sizeof(Chord));
        m_ChordData.GetChord(0, 0, 0);
    }
    ~ReverseChordFinderMode() { }
    
    void     UpdateCurrentPattern();
    void     UpdateBaseFretDisplay();
    uint32_t HandleGettingFretState(uint32_t irKey);
    uint32_t HandleGettingStringsMatchPatternState(uint32_t irKey);
    uint32_t HandleGettingPatternState(uint32_t irKey);
    uint32_t HandleWaitingDoneState(uint32_t irKey);
    bool     Search();
      
    ChordChartData m_ChordData;
    uint8_t        m_CurrentState;
    uint8_t        m_CurrentString;
    uint8_t        m_CurrentFret;
    uint8_t        m_CurrentValue;
    uint8_t        m_MatchPattern;
    PolledDelay    m_Delay;
    Chord          m_Chord;
};



#endif // REVERSECHORDFINDERMODE_H
