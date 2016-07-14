/////////////////////////////////////////////////////////////////////////////////
// EepromConfig.h
//
// Defines the EepromConfig class used with the GuitarChordChart application.
// This class is used to save and restore the configuration information
// to/from EEPROM.
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

#if !defined EEPROMCONFIG_H
#define EEPROMCONFIG_H

#include <Arduino.h>
#include <EEPROM.h>

// This structure defines the actual application data to be stored into EEPROM.
// !!!NOTE!!! EepromConfig::THIS_VERSION must be incremented any time this
//            structure is modified.
struct AppConfig
{
    bool     m_Verbose;                  // Verbose option (true/false).
    unsigned m_UnplayedOption;           // Unplayed string option.
    unsigned m_DemoDisplayOption;        // Demo display option.
    bool     m_IrCodeDispPermission;     // Allow IR code display mode or not.
    unsigned m_Brightness;               // Startup LED brightness.
};

/////////////////////////////////////////////////////////////////////////////////
// EepromConfig class
//
// Saves and restores configuration information to/from EEPROM.
/////////////////////////////////////////////////////////////////////////////////
class EepromConfig
{
public:
    // Return singleton instance address.
    static EepromConfig *Instance();
    
    // Save the this configuratiion to flash.  Return true if successful.
    bool SaveToEE();
    
    // Get saved configuration.
    bool GetFromEE();
    
    /////////////////////////////////////////////////////////////////////////////
    // The version of this configuration.  
    // !!!NOTE!!! Increment this value any time the AppConfig structure is 
    // !!!!!!!!!! changed (member added, size of member changed, etc).
    /////////////////////////////////////////////////////////////////////////////
    static const unsigned THIS_VERSION = 1;
    
    // This constant identifies the application.  It should never change.
    static const unsigned THIS_ID      = 0xC04D;
    
    // The data that goes to / comes from EEPROM.
    unsigned m_Id;                       // ID of this structure.
    unsigned m_Version;                  // Version of this configuration.
    
    AppConfig m_AppConfig;               // Application data.
    
    unsigned m_Checksum;                 // Checksum of this data.
    unsigned m_NotChecksum;              // Complemented checksum of this data.
    
private:
    // Constructor
    EepromConfig();
    
    // Unimplemented methods.
    EepromConfig(EepromConfig &);
    EepromConfig &operator=(EepromConfig&);
    
    // Return true if flash contents are valid.
    bool IsEEValid();
    
    // Return the value of the checksum of this configuration.
    unsigned CalculateThisChecksum();
    
    // Validate that a data in EEPROM was saved correctly.
    bool ValidateSave();
};

#endif  // EEPROMCONFIG_H
