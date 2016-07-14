/////////////////////////////////////////////////////////////////////////////////
// SettingsMode.cpp
//
// Contains methods defined by the SettingsMode class.  These methods
// manage all of the states of the Settings mode.
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

#include "SettingsMode.h"      // For SettingsMode class.
#include "Display.h"           // For Display class.
#include "ModeManager.h"       // For AGAIN.
#include "Strings.h"           // For GetUnplayedOptionString().
#include "EepromConfig.h"      // For AppConfig struct.
#include "LedDriver.h"         // For LedDriver class.
#include "DemoMode.h"          // For DemoMode class.


// Return a pointer to our singleton instance.
SettingsMode *SettingsMode::Instance()
{
    static SettingsMode instance;
    return &instance;
}

// Get the previous config data fron EEPROM (if one exists) and distribute it to
// the corresponding classes.  If a valid EEPROM config does not yet exist, 
// update the config data and save it to EEPROM for the first time.
void SettingsMode::Initialize()
{
    // Get our saved configuration (if any) from EEPROM.
    EepromConfig *pConfig     = EepromConfig::Instance();
    Display  *pDisplay        = Display::Instance();
    AppConfig *pAppConfig     = &pConfig->m_AppConfig;
    DemoMode *pDemoMode       = DemoMode::Instance();
    ModeManager *pModeManager = ModeManager::Instance();

    if (pConfig->GetFromEE())
    {
        // A valid config exists.  Distribute its contents to our application classes.
        DistributeSettings();
    }
    else
    {
        // No config exists yet.  Update the EEPROM config data and save it.
        pAppConfig->m_Verbose              = pDisplay->GetVerbose();
        pAppConfig->m_UnplayedOption       = pDisplay->GetUnplayedOption();
        pAppConfig->m_DemoDisplayOption    = pDemoMode->GetDisplayOption();
        pAppConfig->m_IrCodeDispPermission = pModeManager->GetIrCodeDisplayPermission();
        pAppConfig->m_Brightness           = LedDriver::GetDutyCycle();
        
        // Store the config data locally.
        m_AppConfig = *pAppConfig;
        
        // Save the config to EEPROM.
        pConfig->SaveToEE();
    }
}

// Setup to execute the first state.
void SettingsMode::Startup()
{
    // Clear the fingerboard LEDs.
    Display::Instance()->SetAllLeds(false);
    
    // Save the current EEPROM settings for local use.
    m_AppConfig = EepromConfig::Instance()->m_AppConfig;
    EnterChordDisplayState();
}

// Execute the currently active state.
uint32_t SettingsMode::Execute(uint32_t irKey)
{
    // We'll start over if our mode key is entered again while in any state.
    if (irKey == ModeManager::AGAIN)
    {
        Startup();
        m_CurrentState = GETTING_CHORD_DISPLAY_STATE;
        irKey = 0;
    }
        
    switch (m_CurrentState)
    {
    case GETTING_CHORD_DISPLAY_STATE:
        irKey = HandleGettingChordDisplayDisplay(irKey);
        break;
            
    case GETTING_UNPLAYED_OPTION_STATE:
        irKey = HandleGettingUnplayedOption(irKey);
        break;
        
    case GETTING_DEMO_OPTION_STATE:
        irKey = HandleGettingDemoOption(irKey);
        break;
        
    case GETTING_BRIGHTNESS_STATE:
        irKey = HandleGettingBrightness(irKey);
        break;
        
    case GETTING_IR_PERMISSION_STATE:
        irKey = HandleGettingIrPermission(irKey);
        break;
        
    case SAVING_TO_EEPROM_STATE:
        irKey = HandleSavingToEeprom(irKey);
        break;
        
     default:
        // Somrthing went wrong somewhere.  Setup to start this mode over.
        Startup();
        m_CurrentState = GETTING_CHORD_DISPLAY_STATE;
        break;
    }
    return irKey;
}

// State entry methods.  Each one sets the current state, displays a headline,
// and displays the current value of the specific option.
void SettingsMode::EnterChordDisplayState()
{
    Display  *pDisplay  = Display::Instance();
    
    m_CurrentState = GETTING_CHORD_DISPLAY_STATE;
    pDisplay->DispLcdProgmem(F("Chord Display:"), true, 0, 1);
    DisplayChordDisplaySelection();
}

void SettingsMode::EnterUnplayedState()
{
    Display  *pDisplay  = Display::Instance();
    unsigned option     = pDisplay->GetUnplayedOption();
        
    m_CurrentState = GETTING_UNPLAYED_OPTION_STATE;
    pDisplay->DispLcdProgmem(F("Unplayed Option:"), true, 0, 0);
    DisplayUnplayedSelection();
}

void SettingsMode::EnterDemoOptionState()
{
    Display  *pDisplay  = Display::Instance();
        
    m_CurrentState = GETTING_DEMO_OPTION_STATE;
    pDisplay->DispLcdProgmem(F("Demo Option:"), true, 0, 2);
    DisplayDemoSelection();
}

void SettingsMode::EnterBrightnessState()
{
    Display  *pDisplay = Display::Instance();
        
    m_CurrentState = GETTING_BRIGHTNESS_STATE;
    pDisplay->DispLcdProgmem(F("LED Brightness:"), true, 0, 0);
    DisplayBrightnessSelection();
    pDisplay->SetAllLeds(true);
}

void SettingsMode::EnterIrPermissionState()
{
    Display  *pDisplay  = Display::Instance();
    
    m_CurrentState = GETTING_IR_PERMISSION_STATE;
    pDisplay->DispLcdProgmem(F("Allow IR Test:"), true, 0, 1);
    DisplayIrPermissionSelection();
}

void SettingsMode::EnterSavingToEepromState()
{
    Display  *pDisplay  = Display::Instance();
    
    m_CurrentState = SAVING_TO_EEPROM_STATE;
    pDisplay->DispLcdProgmem(F("Save Options?"), true, 0, 1);
    DisplaySavingSelection();
}

// State handler methods.  Each one handles IR codes specific to the state.
uint32_t SettingsMode::HandleGettingChordDisplayDisplay(uint32_t irKey)
{
    // Cache some useful data.
    Display   *pDisplay   = Display::Instance();
    
    // Assume that we're going to use the specified IR key.
    uint32_t returnKey = 0;
    
    switch (irKey)
    {
    case PREVIOUS:
    case NEXT:      // Toggle the verbose option value.
        m_AppConfig.m_Verbose ^= true;
        DisplayChordDisplaySelection();
        break;
        
    case SELECT:  // Enter the next state.
        EnterUnplayedState();
        break;
        
    default:
        // We didn't handle the passed in key, so return it unmodified.
        returnKey = irKey;
        break;
    }
    
    return returnKey;
}

uint32_t SettingsMode::HandleGettingUnplayedOption(uint32_t irKey)
{
    // Cache some useful data.
    Display   *pDisplay = Display::Instance();
    unsigned  option    = m_AppConfig.m_UnplayedOption;
    
    // Assume that we're going to use the specified IR key.
    uint32_t returnKey = 0;
    
    switch (irKey)
    {
    case PREVIOUS: // Bump to the previous option and display the selection.
        if (option-- == Display::FIRST_UNPLAYED_OPTION)
        {
            option = Display::NUM_UNPLAYED_OPTIONS - 1;
        }
        m_AppConfig.m_UnplayedOption = option;
        DisplayUnplayedSelection();
        break;
        
    case NEXT: // Bump to the next option and display the selection.
        if (++option >= Display::NUM_UNPLAYED_OPTIONS)
        {
            option = Display::FIRST_UNPLAYED_OPTION;
        }
        m_AppConfig.m_UnplayedOption = option;
        DisplayUnplayedSelection();
        break;
        
    case SELECT: // Enter the next state.
        pDisplay->SetAllLeds(false);
        EnterDemoOptionState();
        break;
        
    default:
        // We didn't handle the passed in key, so return it unmodified.
        returnKey = irKey;
        break;
    }
    
    return returnKey;
}

uint32_t SettingsMode::HandleGettingDemoOption(uint32_t irKey)
{
    // Cache some useful data.
    Display   *pDisplay = Display::Instance();
    unsigned  option    = m_AppConfig.m_DemoDisplayOption;
    
    // Assume that we're going to use the specified IR key.
    uint32_t returnKey = 0;
    
    switch (irKey)
    {
    case PREVIOUS:  // Bump to the previous option and display the selection.
        if (option-- == DemoMode::FIRST_DISPLAY_OPTION)
        {
            option = DemoMode::NUMBER_DISPLAY_OPTIONS - 1;
        }
        m_AppConfig.m_DemoDisplayOption = option;
        DisplayDemoSelection();
        break;
        
    case NEXT: // Bump to the next option and display the selection.
        if (++option >= DemoMode::NUMBER_DISPLAY_OPTIONS)
        {
            option = DemoMode::FIRST_DISPLAY_OPTION;
        }
        m_AppConfig.m_DemoDisplayOption = option;
        DisplayDemoSelection();
        break;
        
    case SELECT: // Enter the next state.
        EnterBrightnessState();
        break;
        
    default:
        // We didn't handle the passed in key, so return it unmodified.
        returnKey = irKey;
        break;
    }
    
    return returnKey;
}

uint32_t SettingsMode::HandleGettingBrightness(uint32_t irKey)
{
    // Assume that we're going to use the specified IR key.
    uint32_t returnKey = 0;
    
    switch (irKey)
    {
    case PREVIOUS:  // Increment the brightness and display the selection.
        m_AppConfig.m_Brightness = LedDriver::IncrementDutyCycle();
        DisplayBrightnessSelection();
        break;
        
    case NEXT:  //  Increment the brightness and display the selection.
        m_AppConfig.m_Brightness = LedDriver::DecrementDutyCycle();
        DisplayBrightnessSelection();
        break;
        
    case SELECT: // Enter the next state.
        Display::Instance()->SetAllLeds(false);
        EnterIrPermissionState();
        break;
        
    default:
        // We didn't handle the passed in key, so return it unmodified.
        returnKey = irKey;
        break;
    }
    
    return returnKey;
}

uint32_t SettingsMode::HandleGettingIrPermission(uint32_t irKey)
{
    // Cache some useful data.
    Display     *pDisplay     = Display::Instance();
    
    // Assume that we're going to use the specified IR key.
    uint32_t returnKey = 0;
    
    switch (irKey)
    {
    case PREVIOUS:
    case NEXT:     // Toggle the value and display its value
        m_AppConfig.m_IrCodeDispPermission ^= true;
        DisplayIrPermissionSelection();
        break;
        
    case SELECT: // Enter the next state.
        EnterSavingToEepromState();
        break;
        
    default:
        // We didn't handle the passed in key, so return it unmodified.
        returnKey = irKey;
        break;
    }
    
    return returnKey;
}

uint32_t SettingsMode::HandleSavingToEeprom(uint32_t irKey)
{
    // Cache some useful data.
    Display      *pDisplay     = Display::Instance();
    EepromConfig *pConfig      = EepromConfig::Instance();
    AppConfig    *pAppConfig   = &pConfig->m_AppConfig;
    DemoMode     *pDemoMode    = DemoMode::Instance();
    ModeManager  *pModeManager = ModeManager::Instance();
    
    // Assume that we're going to use the specified IR key.
    uint32_t returnKey = 0;
    
    switch (irKey)
    {
    case PREVIOUS:
    case NEXT:    // Toggle the selection value. 
        m_Save ^= true;
        DisplaySavingSelection();
        break;
        
    case SELECT:  // Save the current options to EEPROM if the user desires so.
        if (m_Save)
        {
            // Copy our local settings to the EEPROM config data.
            pConfig->m_AppConfig = m_AppConfig;
            
            // Distribute the new settings to our application classes.
            DistributeSettings();
            
            // Copy our (possibly) new settings to EEPROM.
            pConfig->SaveToEE();
        }
        EnterChordDisplayState();
        break;
        
    default:
        // We didn't handle the passed in key, so return it unmodified.
        returnKey = irKey;
        break;
    }
    
    return returnKey;
}

// This group of methods displays the current value of the option used in the
// current state.
void SettingsMode::DisplayChordDisplaySelection()
{
    Display  *pDisplay  = Display::Instance();
    
    pDisplay->DispLcdProgmem(F("Type Display:"), true, 0, 1);
    if (m_AppConfig.m_Verbose)
    {
        pDisplay->DispLcdProgmem(F("  VERBOSE  "), false, 1, 2);
    }
    else
    {
        pDisplay->DispLcdProgmem(F("ABBREVIATED"), false, 1, 2);
    }            
}

void SettingsMode::DisplayUnplayedSelection()
{
    Display  *pDisplay   = Display::Instance();
    unsigned option      = m_AppConfig.m_UnplayedOption;
    unsigned savedOption = pDisplay->GetUnplayedOption();
    ChordChartData       chordData;
    char                 dispBuf[17];
        
    // Update the LCD display with the currently selected option string.
    pDisplay->DispLcd(Strings::GetUnplayedOptionString(option, dispBuf), false, 1, 0);    
    
    // Temporarily set the current unplayed option so we can display it below.
    pDisplay->SetUnplayedOption(option);
    
    // Select a chord that has unplayed strings, and show how it will be displayed.
    chordData.GetChord(KeyTypeC, TypeMinorSixth, 0);
    pDisplay->DisplayLeds(chordData.GetChordPtr());
    
    // Restore the saved unplayed option value.
    pDisplay->SetUnplayedOption(savedOption);    
}

void SettingsMode::DisplayDemoSelection()
{
    Display  *pDisplay  = Display::Instance();
    unsigned option     = m_AppConfig.m_DemoDisplayOption;
    char     dispBuf[17];
        
    pDisplay->DispLcd(Strings::GetDemoOptionString(option, dispBuf), false, 1, 0);    
}

void SettingsMode::DisplayBrightnessSelection()
{
    Display  *pDisplay = Display::Instance();
    unsigned level     = LedDriver::GetDutyCycle() * 10;
    unsigned offset    = level < 100 ? 8 : 9;
    
    pDisplay->DispLcdInt(level, false, 1, 6); 
    pDisplay->DispLcd("% ", false, 1, offset);
}

void SettingsMode::DisplayIrPermissionSelection()
{
    Display  *pDisplay  = Display::Instance();
    
    if (m_AppConfig.m_IrCodeDispPermission)
    {
        pDisplay->DispLcdProgmem(F("PERMIT"), false, 1, 4);
    }
    else
    {
        pDisplay->DispLcdProgmem(F("FORBID"), false, 1, 4);
    }            
}

void SettingsMode::DisplaySavingSelection()
{
    Display  *pDisplay  = Display::Instance();
    
    if (m_Save)
    {
        pDisplay->DispLcdProgmem(F("SAVE  "), false, 1, 5);
    }
    else
    {
        pDisplay->DispLcdProgmem(F("CANCEL"), false, 1, 5);
    }            
    
}

// Distribute the current option settings to the owner classes.
void SettingsMode::DistributeSettings()
{
    // Get our saved configuration (if any) from EEPROM.
    EepromConfig *pConfig     = EepromConfig::Instance();
    Display  *pDisplay        = Display::Instance();
    AppConfig *pAppConfig     = &pConfig->m_AppConfig;
    DemoMode *pDemoMode       = DemoMode::Instance();
    ModeManager *pModeManager = ModeManager::Instance();

    // A valid config exists.  Distribute its contents to our application classes.
    pDisplay->SetVerbose(pAppConfig->m_Verbose);
    pDisplay->SetUnplayedOption(pAppConfig->m_UnplayedOption);
    pDemoMode->SetDisplayOption(pAppConfig->m_DemoDisplayOption);
    pModeManager->SetIrCodeDisplayPermission(pAppConfig->m_IrCodeDispPermission);
    LedDriver::SetDutyCycle(pAppConfig->m_Brightness);
}
