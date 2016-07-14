/////////////////////////////////////////////////////////////////////////////////
// SettingsMode.h
//
// Defines the class that handles the settings mode.  This mode allows the user
// to specify various configuration settings.  These settings get saved into
// EEPROM when directed by the user.  Upon power-up, the options are retrieved
// from EEPROM and distributed to the corredponding classes.
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

#if !defined SETTINGSMODE_H
#define SETTINGSMODE_H


#include "IrCodes.h"            // For raw IR codes.
#include "Mode.h"               // For Mode base class.
#include "EepromConfig.h"       // For AppConfig struct.



/////////////////////////////////////////////////////////////////////////////////
// SettingsMode class.
/////////////////////////////////////////////////////////////////////////////////
class SettingsMode : public Mode
{
public:
    static SettingsMode *Instance();
    
    virtual uint32_t Execute(uint32_t irKey);
    virtual void     Startup();
    virtual void     Shutdown() { }
    
    void Initialize();

protected:
    
private:
    // Unimplemented methods
    SettingsMode(SettingsMode &rMgr);
    SettingsMode &operator=(SettingsMode &rMgr);
    
    // Private constructor and destructor for singleton.
    SettingsMode() : m_CurrentState(GETTING_CHORD_DISPLAY_STATE), m_Save(false) { }; 
    ~SettingsMode() { }
      
    // State definitions.
    static const unsigned GETTING_CHORD_DISPLAY_STATE   = 0;
    static const unsigned GETTING_UNPLAYED_OPTION_STATE = 1;
    static const unsigned GETTING_DEMO_OPTION_STATE     = 2;
    static const unsigned GETTING_BRIGHTNESS_STATE      = 3;
    static const unsigned GETTING_IR_PERMISSION_STATE   = 4;
    static const unsigned SAVING_TO_EEPROM_STATE        = 5;
    static const unsigned FIRST_OPTION_STATE            = GETTING_CHORD_DISPLAY_STATE;
    static const unsigned NUMBER_OPTION_STATES          = SAVING_TO_EEPROM_STATE - 1;
    
    // State entry methods.
    void EnterChordDisplayState();
    void EnterUnplayedState();
    void EnterDemoOptionState();
    void EnterBrightnessState();
    void EnterIrPermissionState();
    void EnterSavingToEepromState();
    
    // State handling methods.
    uint32_t HandleGettingChordDisplayDisplay(uint32_t irKey);
    uint32_t HandleGettingUnplayedOption(uint32_t irKey);
    uint32_t HandleGettingDemoOption(uint32_t irKey);
    uint32_t HandleGettingBrightness(uint32_t irKey);
    uint32_t HandleGettingIrPermission(uint32_t irKey);
    uint32_t HandleSavingToEeprom(uint32_t irKey);
    
    // Option setting string display methods.
    void DisplayChordDisplaySelection();
    void DisplayUnplayedSelection();
    void DisplayDemoSelection();
    void DisplayBrightnessSelection();
    void DisplayIrPermissionSelection();
    void DisplaySavingSelection();
    
    void DistributeSettings();
    
    
    
    // Map raw IR keys to our specific use.
    // Change mapping to suit your taste.
    static const uint32_t PREVIOUS = IrCodes::UP;      // Up
    static const uint32_t NEXT     = IrCodes::DOWN;    // Down
    static const uint32_t SELECT   = IrCodes::OK;      // Enter

    unsigned  m_CurrentState;
    bool      m_Save;
    AppConfig m_AppConfig;
};



#endif // SETTINGSMODE_H
