/////////////////////////////////////////////////////////////////////////////////
// ChordFinderMode.cpp
//
// Contains methods defined by the ChordFinderMode class.  These methods
// manage all of the states of the Chord Finder mode.
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

#include "Mode.h"               // For Mode class.
#include "ChordFinderMode.h"    // For ChordFinderMode class.
#include "Display.h"            // For Display class.
#include "ModeManager.h"        // For ModeManager class.
#include "EepromConfig.h"       // For EepromConfig class.


ChordFinderMode *ChordFinderMode::Instance()
{
    static ChordFinderMode instance;
    return &instance;
}

uint32_t ChordFinderMode::Execute(uint32_t irKey)
{
    // Cache a pointer to the display instance,
    Display *pDisplay       = Display::Instance();
    EepromConfig *pConfig   = EepromConfig::Instance();
    AppConfig *pAppConfig   = &pConfig->m_AppConfig;
    unsigned unplayedOption = pDisplay->GetUnplayedOption();
    
    // Setup to return 0 if we handle the new IR key, or the passed in value 
    // if we don't.  Assume we'll handle the passed in IR key.
    uint32_t returnedIrKey = 0;
    if (irKey)
    {
        switch(irKey)
        {
        case NEXT_KEY:    //Next key of same type, variation 1.
            m_ChordData.NextKey();
            pDisplay->DisplayChord(&m_ChordData);
            break;
            
        case PREV_KEY:    // Previous key of same type, variation 1.
            m_ChordData.PreviousKey();
            pDisplay->DisplayChord(&m_ChordData);
            break;
            
        case PREV_TYPE:   // Previous type of same key, variation 1.
            m_ChordData.PreviousType();
            pDisplay->DisplayChord(&m_ChordData);
            break;
            
        case NEXT_TYPE:   // Next type of same key, variation 1,
            m_ChordData.NextType();
            pDisplay->DisplayChord(&m_ChordData);
            break;
            
        case FIRST_TYPE:  // First type of current key, variation 1.
            m_ChordData.GetChord(m_ChordData.GetCurrentKey(), 0, 0);
            pDisplay->DisplayChord(&m_ChordData);
            break;
            
        case PREV_VAR:    // Previous variation of current key and type.
            m_ChordData.PreviousVariation();
            pDisplay->DisplayChord(&m_ChordData);
            break;
            
        case NEXT_VAR:    // Next variation of current key and type.
            m_ChordData.NextVariation();
            pDisplay->DisplayChord(&m_ChordData);
            break;
            
        case TOGGLE_TYPE_DISPLAY: // Toggle chord type display type.
            m_VerboseDisplay ^= true;
            pAppConfig->m_Verbose = m_VerboseDisplay;
            pDisplay->SetVerbose(pAppConfig->m_Verbose);
            pDisplay->DisplayChord(&m_ChordData);
            break;
            
        case NEXT_UNPLAYED_OPT: // Cycle through unplayed string options.
            if (++unplayedOption >= Display::NUM_UNPLAYED_OPTIONS)
            {
                unplayedOption = Display::FIRST_UNPLAYED_OPTION;
            }
            pAppConfig->m_UnplayedOption = unplayedOption;
            pDisplay->SetUnplayedOption(unplayedOption);
            pDisplay->DisplayChord(&m_ChordData);
            break;
            
        case ModeManager::REFRESH:  // Just re-display the current chord.
            pDisplay->DisplayChord(&m_ChordData);
            break;
            
        default:
            // We didn't handle the IR key, so return it to the caller.
            returnedIrKey = irKey;
            break;
        }
    }
    
    return returnedIrKey;
}


