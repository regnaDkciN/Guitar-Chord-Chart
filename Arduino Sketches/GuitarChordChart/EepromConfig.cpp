/////////////////////////////////////////////////////////////////////////////////
// EepromConfig.cpp
//
// Defines the methods for the EepromConfig class.
// See Configuration.h.
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

#include <stddef.h>
#include "Arduino.h"
#include <EEPROM.h>
#include "EepromConfig.h"


/////////////////////////////////////////////////////////////////////////////////
// Return a pointer to the singleton instance of this class.
/////////////////////////////////////////////////////////////////////////////////
EepromConfig *EepromConfig::Instance()
{
    static EepromConfig instance;
    return &instance;
}

/////////////////////////////////////////////////////////////////////////////////
// EepromConfig class constructor
/////////////////////////////////////////////////////////////////////////////////
EepromConfig::EepromConfig()
{
    memset(this, 0, sizeof(EepromConfig));
}

/////////////////////////////////////////////////////////////////////////////////
// EepromConfig::SaveToEE()
//
// Saves the configuration structure to EEPROM.  Adds checksum and version
// information as needed.  After writing the data to EEPROM, verifies that it
// got there correctly.
//
// Returns true if successful, false otherwise.
/////////////////////////////////////////////////////////////////////////////////
bool EepromConfig::SaveToEE()
{
    // Update needed fields.
    m_Id          = THIS_ID;
    m_Version     = THIS_VERSION;
    m_Checksum    = CalculateThisChecksum();
    m_NotChecksum = ~m_Checksum;
    
    // Save the data in EEPROM.
    uint8_t *pData = reinterpret_cast<uint8_t *>(this);
    for (unsigned i = 0; i < sizeof(EepromConfig); i++)
    {
        EEPROM.write(i, *pData++);
    }
    
    // Validate that the data was saved successully, and return status.
    return ValidateSave();
}

/////////////////////////////////////////////////////////////////////////////////
// EepromConfig::GetFromEE()
//
// Gets saved configuration information from EEPROM if it is valid.
//
// Returns true if data was successfully retrieved from EEPROM, false otherwise.
// If EEPROM is not valid, the data in this class is not modified.
/////////////////////////////////////////////////////////////////////////////////
bool EepromConfig::GetFromEE()
{
    // If the EEPROM is not valid, return false.
    if (!IsEEValid())
    {
        return false;
    }
    
    // Read the data from EEPROM into ourself.
    uint8_t *pData = reinterpret_cast<uint8_t *>(this);
    for (unsigned i = 0; i < sizeof(EepromConfig); i++)
    {
        *pData++ = EEPROM.read(i);
    }
    
    // Data was good return success.
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
// EepromConfig::ValidateSave()
//
// Verifies that the data saved in EEPROM matches the current contents if this
// instance.
//
// Returns true is the data matches, false otherwise.
/////////////////////////////////////////////////////////////////////////////////
bool EepromConfig::ValidateSave()
{
    // Validate the data in EEPROM.
    uint8_t *pData = reinterpret_cast<uint8_t *>(this);
    for (unsigned i = 0; i < sizeof(EepromConfig); i++)
    {
        if (EEPROM.read(i) != *pData++)
        {
            return false;
        }
    }
    return true;    
}

/////////////////////////////////////////////////////////////////////////////////
// EepromConfig::IsEEValid()
//
// Checks the contents of EEPROM and determines if it is valid or not.
//
// Returns true if EEPROM is valid, false otherwise.
/////////////////////////////////////////////////////////////////////////////////
bool EepromConfig::IsEEValid()
{
    // Use a temporary copy of the config.
    EepromConfig config;
    
    // Read the data from EEPROM into our local copy.
    uint8_t *pData = reinterpret_cast<uint8_t *>(&config);
    for (unsigned i = 0; i < sizeof(EepromConfig); i++)
    {
        *pData++ = EEPROM.read(i);        
    }
    
    // Verify that the data looks good.
    if ((config.m_Id != THIS_ID)                              ||
        (config.m_Version != THIS_VERSION)                    ||
        (config.CalculateThisChecksum() != config.m_Checksum) ||
        (config.m_NotChecksum != ~config.m_Checksum))
    {
        return false;
    }
    
    // Looks like the data is OK, return true.
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
// EepromConfig::CalculateThisChecksum()
//
// Returns the checksum value of the data within this class.
/////////////////////////////////////////////////////////////////////////////////
unsigned EepromConfig::CalculateThisChecksum()
{
    // The running checksum value.
    unsigned checksum = 0;
    
    // Read the data from EEPROM and update the checksum along the way.
    uint8_t *pData = reinterpret_cast<uint8_t *>(this);
    for (unsigned i = 0; i < sizeof(EepromConfig) - 2 * sizeof(unsigned); i++)
    {
        checksum += *pData++;
    }
    
    // Return the calculated checksum value.
    return checksum;
}
