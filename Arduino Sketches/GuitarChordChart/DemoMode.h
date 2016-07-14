/////////////////////////////////////////////////////////////////////////////////
// DemoMode.h
//
// This class implements the demo mode.  It mainly displays various patterns
// on the finger board LEDs.
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

#if !defined DEMOMODE_H
#define DEMOMODE_H


#include "IrCodes.h"            // For raw IR codes.
#include "ChordChartData.h"     // For the chord data.
#include "Display.h"            // For Display class.
#include "Mode.h"               // For Mode base class.
#include "PolledDelay.h"        // For PolledDelay class.



/////////////////////////////////////////////////////////////////////////////////
// DemoData structure.
//
// This structure contains information used to setup and execute the currently
// active demo state.
/////////////////////////////////////////////////////////////////////////////////
struct DemoData
{
    uint32_t m_StartupDelay;        // The state's startup delay.
    void     (*m_StartupFunc)();    // Pointer to the state's startup method.
    void     (*m_pExecute)(unsigned sweep); // Pointer to the state's execution method.
    unsigned m_Argument;            // Argument passed to the execution method on invocation.
};


/////////////////////////////////////////////////////////////////////////////////
// DemoMode class
/////////////////////////////////////////////////////////////////////////////////
class DemoMode : public Mode
{
public:
    static DemoMode *Instance();
    void Initialize();
    
    virtual uint32_t Execute(uint32_t irKey);
    virtual void     Startup();
    virtual void     Shutdown() { }
    unsigned         GetDisplayOption() const       { return m_DisplayOption; }
    void             SetDisplayOption(unsigned val) { m_DisplayOption = val; }
    void             NextState();
    
    static void StartupAllOn();
    static void StartupAllOff();
    static void ExecuteAllOnOff(unsigned);
    static void ExecuteWalkLed(unsigned sweep);
    static void ExecuteWalkFret(unsigned sweep);
    static void ExecuteWalkString(unsigned sweep);
    static void ExecuteRandomChord(unsigned);

    // Display option valaues.
    static const unsigned CONTINUOUS             = 0;
    static const unsigned REPEAT_CURRENT         = 1;
    static const unsigned RANDOM                 = 2;
    static const unsigned FIRST_DISPLAY_OPTION   = CONTINUOUS;
    static const unsigned NUMBER_DISPLAY_OPTIONS = RANDOM + 1;
    
protected:
    
private:
    // Unimplemented methods
    DemoMode(DemoMode &rMgr);
    DemoMode &operator=(DemoMode &rMgr);

    // Private constructor and destructor for singleton.
    DemoMode() : m_State(FIRST_DEMO_STATE), m_Delay(), m_String(0),
                 m_Fret(0), m_ChordData(), 
                 m_pExecuteState(ExecuteAllOnOff), m_FirstPass(true), m_Argument(0),
                 m_Count(0), m_DisplayOption(CONTINUOUS)
    { }
    ~DemoMode() { }
    
    void StartCurrentState();
    
    // Demo state definitions.
    static const unsigned ALL_ON             = 0;
    static const unsigned ALL_OFF            = 1;
    static const unsigned WALK_LED_ON        = 2;
    static const unsigned SWEEP_LED_ON       = 3;
    static const unsigned WALK_LED_OFF       = 4;
    static const unsigned SWEEP_LED_OFF      = 5;
    static const unsigned WALK_FRET_ON       = 6;
    static const unsigned SWEEP_FRET_ON      = 7;
    static const unsigned WALK_FRET_OFF      = 8;
    static const unsigned SWEEP_FRET_OFF     = 9;
    static const unsigned WALK_STRING_ON     = 10;
    static const unsigned SWEEP_STRING_ON    = 11;
    static const unsigned WALK_STRING_OFF    = 12;
    static const unsigned SWEEP_STRING_OFF   = 13;
    static const unsigned RANDOM_CHORD       = 14;
    static const unsigned FIRST_DEMO_STATE   = ALL_ON;
    static const unsigned NUMBER_DEMO_STATES = RANDOM_CHORD + 1;
    static const unsigned MAX_RANDOM_CHORDS  = 5;
    
    // IR key mappings.
    static const uint32_t   NEXT     = IrCodes::UP;
    static const uint32_t   PREVIOUS = IrCodes::DOWN;
    
    // Miscellaneous constants.
    static const uint32_t SHORT_DELAY = 300;
    static const uint32_t LONG_DELAY  = 1000;
    
    unsigned        m_State;
    PolledDelay     m_Delay;
    uint8_t         m_String;
    uint8_t         m_Fret;
    ChordChartData  m_ChordData;
    void            (*m_pExecuteState)(unsigned);
    bool            m_FirstPass;
    unsigned        m_Argument;
    unsigned        m_Count;
    unsigned        m_DisplayOption;
};



#endif // DEMOMODE_H
