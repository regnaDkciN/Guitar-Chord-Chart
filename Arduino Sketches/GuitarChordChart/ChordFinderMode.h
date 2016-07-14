/////////////////////////////////////////////////////////////////////////////////
// ChordFinderMode.h
//
// Defines the class that handles the chord finding mode.  This mode allows the
// user to look up chords by cycling through the desired key, type, and variation.
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

#if !defined CHORDFINDERMODE_H
#define CHORDFINDERMODE_H

#include "IrCodes.h"            // For raw IR codes.
#include "ChordChartData.h"     // For the chord data.
#include "Mode.h"               // For Mode class.

/////////////////////////////////////////////////////////////////////////////////
// ChordFinderMode class.
/////////////////////////////////////////////////////////////////////////////////
class ChordFinderMode : public Mode
{
public:
    static ChordFinderMode *Instance();
    void Initialize()
    {
    }

    virtual uint32_t Execute(uint32_t irKey);
    virtual void     Startup()  { }
    virtual void     Shutdown() { }
    
protected:
    
private:
    // Map raw IR keys to our specific use.
    // Change mapping to suit your taste.
    static const uint32_t NEXT_KEY            = IrCodes::UP;       // Up
    static const uint32_t PREV_KEY            = IrCodes::DOWN;     // Down
    static const uint32_t PREV_TYPE           = IrCodes::LEFT;     // Left
    static const uint32_t NEXT_TYPE           = IrCodes::RIGHT;    // Right
    static const uint32_t FIRST_TYPE          = IrCodes::OK;       // Enter
    static const uint32_t PREV_VAR            = IrCodes::CH_DOWN;  // Channel Down
    static const uint32_t NEXT_VAR            = IrCodes::CH_UP;    // Channel Up
    static const uint32_t TOGGLE_TYPE_DISPLAY = IrCodes::INFO;     // Info
    static const uint32_t NEXT_UNPLAYED_OPT   = IrCodes::GUIDE;    // Guide
    
    // Unimplemented methods
    ChordFinderMode(ChordFinderMode &rMgr);
    ChordFinderMode &operator=(ChordFinderMode &rMgr);

    // Private constructor and destructor for singleton.
    ChordFinderMode() : m_ChordData(), m_VerboseDisplay(true)
    { 
        m_ChordData.GetChord(0, 0, 0);
    }
    ~ChordFinderMode() { }
    
    ChordChartData m_ChordData;
    bool           m_VerboseDisplay;
};



#endif // CHORDFINDERMODE_H
