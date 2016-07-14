/////////////////////////////////////////////////////////////////////////////////
// IrCodes.h
//
// Defines the class that specifies all the IR codes that are known.
//
// IR codes for Sony TV from Vivitar 6 Device Universal Remote (VIV-URC-721)
// Using TV code 5321.
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

#if !defined IRCODES_H
#define IRCODES_H

#include "Arduino.h"                // For uintxx_t.


/////////////////////////////////////////////////////////////////////////////////
// IrCodes class.
/////////////////////////////////////////////////////////////////////////////////
class IrCodes
{
public:
    static uint32_t Validate(uint32_t irCode);

    static const uint32_t POWER     = 0xf124b0b6;      //
    static const uint32_t MUTE      = 0x78f479a7;      //
    static const uint32_t REWIND    = 0x2bdc941f;      //
    static const uint32_t PLAY      = 0x633dd8c4;      //
    static const uint32_t FFWD      = 0xbd293f06;      //
    static const uint32_t STOP      = 0xc502105e;      //
    static const uint32_t PAUSE     = 0x36d87f5b;      //
    static const uint32_t MENU      = 0x3ac82a07;      //
    static const uint32_t VOL_UP    = 0xb44d8dfb;      //
    static const uint32_t VOL_DOWN  = 0x43898cc0;      //
    static const uint32_t CH_UP     = 0xbc392255;      //
    static const uint32_t CH_DOWN   = 0xffafb8dc;      //
    static const uint32_t UP        = 0xec27d43d;      //
    static const uint32_t DOWN      = 0x086bd99c;      //
    static const uint32_t LEFT      = 0x1a422e43;      //
    static const uint32_t RIGHT     = 0xa23bd824;      //
    static const uint32_t OK        = 0x7295a904;      //
    static const uint32_t INFO      = 0xdc68e43f;      //
    static const uint32_t INPUT_CH  = 0xba00cb82;      //
    static const uint32_t BACK      = 0x99c7ec8f;      //
    static const uint32_t GUIDE     = 0xd1c67b61;      //
    static const uint32_t NUM_1     = 0x207bef0f;      //
    static const uint32_t NUM_2     = 0xe8455d8e;      //
    static const uint32_t NUM_3     = 0xcbb7e949;      //
    static const uint32_t NUM_4     = 0x2c1f3172;      //
    static const uint32_t NUM_5     = 0x905ed4f5;      //
    static const uint32_t NUM_6     = 0x8ce1e3fc;      //
    static const uint32_t NUM_7     = 0x315b1905;      //
    static const uint32_t NUM_8     = 0xc67c01b6;      //
    static const uint32_t NUM_9     = 0xb418d969;      //
    static const uint32_t NUM_0     = 0x3b0b8330;      //
    static const uint32_t DASH      = 0xbe980a4b;      //
    
protected:
    
private:

};


#endif // IRCODES_H
