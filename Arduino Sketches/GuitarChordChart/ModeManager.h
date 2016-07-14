/////////////////////////////////////////////////////////////////////////////////
// ModeManager.h
//
// Defines the class that manages selection and execution of all modes.
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

#if !defined MODEMANAGER_H
#define MODEMANAGER_H

#include "IrCodes.h"            // For raw IR codes.
#include "ChordChartData.h"     // For the chord data.
#include "Display.h"            // For Display class.
#include "Mode.h"               // For Mode class.



/////////////////////////////////////////////////////////////////////////////////
// ModeManager class.
/////////////////////////////////////////////////////////////////////////////////
class ModeManager
{
public:
    static   ModeManager *Instance();
    void     Initialize();
    uint32_t Execute(uint32_t irKey);
    void     ActivatePreviousMode();
    bool     IsModeKey(uint32_t irKey) const;
    void     SetIrCodeDisplayPermission(bool val) { m_IrCodeDisplayPermission = val; }
    bool     GetIrCodeDisplayPermission() const   { return m_IrCodeDisplayPermission; }
    
    static const uint32_t REFRESH = 0x11111111;
    static const uint32_t AGAIN   = 0x22222222;

protected:
    
private:
    static const uint32_t FIND_CHORD_MODE         = IrCodes::NUM_1;      // 1
    static const uint32_t REVERSE_FIND_CHORD_MODE = IrCodes::NUM_2;      // 2
    static const uint32_t SETTINGS_MODE           = IrCodes::NUM_3;      // 3
    static const uint32_t DEMO_MODE               = IrCodes::NUM_4;      // 4
    static const uint32_t DISP_IR_CODE_MODE       = IrCodes::NUM_5;      // 5

    // Unimplemented methods
    ModeManager(ModeManager &rMgr);
    ModeManager &operator=(ModeManager &rMgr);

    // Private constructor and destructor for singleton.
    ModeManager();    
    ~ModeManager() { }
    
    
    uint32_t    m_CurrentMode;
    Mode *      m_pCurrentMode;
    uint32_t    m_PreviousMode;
    Mode        *m_pPreviousMode;
    bool        m_IrCodeDisplayPermission;
};


#endif // MODEMANAGER_H
