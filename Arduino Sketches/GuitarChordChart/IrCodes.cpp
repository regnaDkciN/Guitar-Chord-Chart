/////////////////////////////////////////////////////////////////////////////////
// IrCodes.cpp
//
// Contains methods defined by the IrCodes class.  
// The method in this class simply returns an indication of whether or not a 
// received IR key value is know to the application.
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

#include "IrCodes.h"        // For IrCodes class.


// Return 'true' if the passed in code is known, false otherwise.
uint32_t IrCodes::Validate(uint32_t irCode)
{
    // Assume we see a valid IR code.
    uint32_t returnIrCode = irCode;
    
    switch (irCode)
    {
    case POWER:
    case MUTE:
    case REWIND:
    case PLAY:
    case FFWD:
    case STOP:
    case PAUSE:
    case MENU:
    case VOL_UP:
    case VOL_DOWN:
    case CH_UP:
    case CH_DOWN:
    case UP:
    case DOWN:
    case LEFT:
    case RIGHT:
    case OK:
    case INFO:
    case INPUT_CH:
    case BACK:
    case GUIDE:
    case NUM_1:
    case NUM_2:
    case NUM_3:
    case NUM_4:
    case NUM_5:
    case NUM_6:
    case NUM_7:
    case NUM_8:
    case NUM_9:
    case NUM_0:
    case DASH:
        break;
        
    default:
        // Not a valid IR code.  Return 0.
        returnIrCode = 0;
        break;
    }
    
    return returnIrCode;
}