/////////////////////////////////////////////////////////////////////////////////
// ModeManager.cpp
//
// Contains methods defined by the ModeManager class.  These methods manage the
// modes supported by the Guitar Chord Chart application.
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

#include "ChordFinderMode.h"        // For ChordFinderMode class.
#include "ReverseChordFinderMode.h" // For ReverseChordFinderMode class.
#include "DemoMode.h"               // For DemoMode class.
#include "SettingsMode.h"           // For SettingsMode class.
#include "IrCodeDisplayMode.h"      // For IrCodeDisplayMode class.
#include "ModeManager.h"            // For ModeManager class.


// Return a pointer to our singleton instance.
ModeManager *ModeManager::Instance()
{
    static ModeManager instance;
    return &instance;
}

// Constructor - just initialize default values for our member data.
ModeManager::ModeManager() : 
        m_CurrentMode(FIND_CHORD_MODE), 
        m_pCurrentMode(ChordFinderMode::Instance()),
        m_PreviousMode(0), 
        m_pPreviousMode(NULL),
        m_IrCodeDisplayPermission(false)
{ 
}

// Initialize - refreshes the current mode.
void ModeManager::Initialize()
{
    m_pCurrentMode->Execute(REFRESH);
}
    
// Execute the mode manager - call the currently active mode to execute.    
uint32_t ModeManager::Execute(uint32_t irKey)
{
    // Assume the new IR key is not a mode key.
    uint32_t returnKey = irKey;
    
    if (IsModeKey(irKey))
    {
        // Remember that we already handled this key.
        returnKey = 0;
            
        if (irKey != m_CurrentMode)
        {
            // Tell the current mode that we're shutting down.
            m_pCurrentMode->Shutdown();
            
            // Save the old mode.
            m_PreviousMode  = m_CurrentMode;
            m_pPreviousMode = m_pCurrentMode;
            
            // Find the requested new mode.
            m_CurrentMode = irKey;
            switch(irKey)
            {
            case FIND_CHORD_MODE:
                m_pCurrentMode = ChordFinderMode::Instance();
                break;
            
            case REVERSE_FIND_CHORD_MODE:
                m_pCurrentMode = ReverseChordFinderMode::Instance();
                break;
                
            case SETTINGS_MODE:
                m_pCurrentMode = SettingsMode::Instance();
                break;
            
            case DEMO_MODE:
                m_pCurrentMode = DemoMode::Instance();
                break;
                
            case DISP_IR_CODE_MODE:
                if (m_IrCodeDisplayPermission)
                {
                    m_pCurrentMode = IrCodeDisplayMode::Instance();
                }
                break;
                
            default:
                break;
            }
            
            // Tell the new mode that we're starting up.
            m_pCurrentMode->Startup();
            m_pCurrentMode->Execute(REFRESH);
        }
        else
        {
            // Same mode key hit.  Let the current mode handle it.
             m_pCurrentMode->Execute(AGAIN);
        }
    }
    
    // Execute the current mode using the (possibly modified) IR key.
    m_pCurrentMode->Execute(returnKey);
    
    // Return the (possibly modified) IR key.
    return returnKey;
}

// Activate the mode that was active before the current one.
void ModeManager::ActivatePreviousMode()
{
    if (m_pPreviousMode)
    {
        // Tell the current mode that we're shutting down.
        m_pCurrentMode->Shutdown();
        
        // Save the previous mode as current and Null out the previous mode.
        m_CurrentMode   = m_PreviousMode;
        m_pCurrentMode  = m_pPreviousMode;
        m_PreviousMode  = 0;
        m_pPreviousMode = NULL;
        
        // Tell the new mode that we're starting up.
        m_pCurrentMode->Startup();
        
        // Tell the new mode to refresh itself.
        m_pCurrentMode->Execute(REFRESH);
    }
}

// Return 'true' if the argument specifies a mode key, 'false' otherwise.
bool ModeManager::IsModeKey(uint32_t irKey) const
{
    // Assume it is not a mode key.
    bool isMode = false;
    
    switch (irKey)
    {
    case FIND_CHORD_MODE:
    case REVERSE_FIND_CHORD_MODE:
    case SETTINGS_MODE:
    case DEMO_MODE:
    case DISP_IR_CODE_MODE:
        // It is a mode key, setup to return true status.
        isMode = true;
        break;
        
     default:
        // Not a mode key, use the false default.
        break;
    }
    
    return isMode;
}
