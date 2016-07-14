/////////////////////////////////////////////////////////////////////////////////
// PolledDelay.h
//
// This class implements a polled timer.  Rather than waiting while the timer
// times, this class must be polled to determine if the specified timeout has
// yet expired.
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

#if !defined POLLEDDELAY_H
#define POLLEDDELAY_H

#include "Arduino.h"                // For uintxx_t.

/////////////////////////////////////////////////////////////////////////////////
// PolledDelay class.
/////////////////////////////////////////////////////////////////////////////////
class PolledDelay
{
public:
    // Default constructor - init to default unused values.
    PolledDelay() : m_StartTime(0), m_DelayValue(0), m_Status(INACTIVE) { }
    // Initializer constructor.  Construct the instance and start it timing.
    PolledDelay(uint32_t delayValue) : 
            m_StartTime(millis()), 
            m_DelayValue(delayValue),
            m_Status(TIMING)
    {
    }
    ~PolledDelay() { }
    
    // Start/re-start the timer.  The delay value is in milliseconds.
    void Start(uint32_t delayValue)
    {
        m_StartTime  = millis();
        m_DelayValue = delayValue;
        m_Status     = TIMING;
    }
    
    // Return 'true' if the specified time has expired, 'false' otherwise.
    bool IsExpired()
    {
        if (m_Status == TIMING)
        {
            if ((millis() - m_StartTime) >= m_DelayValue)
            {
                m_Status = EXPIRED;
            }
        }
        return (m_Status == EXPIRED);
    }
    
    // Return 'true' if the timer is still timing, 'false' otherwise.
    bool IsTiming() const
    {
        return (m_Status == TIMING);
    }
    
    // Return the last specified timeout (delay) value.
    uint32_t GetDelayValue() const
    {
        return m_DelayValue;
    }
    
    
protected:
    
private:
    static const unsigned INACTIVE = 0;
    static const unsigned TIMING   = 1;
    static const unsigned EXPIRED  = 2;

    uint32_t m_StartTime;
    uint32_t m_DelayValue;
    unsigned m_Status;
};


#endif // POLLEDDELAY_H