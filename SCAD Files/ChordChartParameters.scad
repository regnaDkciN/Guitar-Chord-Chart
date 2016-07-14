////////////////////////////////////////////////////////////////////////////////
//
// ChordChartParameters.scad
//
// Contains settings, constants, and shared modules for the guitar chord chart
// project.
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
////////////////////////////////////////////////////////////////////////////////

use<MCAD/utilities.scad>                        // For distance().


$fn = 100;

// S E L E C T A B L E   F E A T U R E S
Explode                 = 1;                    // 1 for explode, 0 for not.
UseLedHoles             = 1;                    // 1 for LED holes, 0 for not.
UseSideInlays           = 1;                    // 1 for side inlays, 0 for not.
UseSquareSideInlays     = 0;                    // 1 for square side inlays,
                                                // 0 for circular.
UseStraightNeck         = 0;                    // 1 for straight neck transition
                                                //   to head, 0 for spherical.
UseTopInlays            = 1;                    // 1 for top inlays, 0 for not.
UseTriangularTopInlays  = 1;                    // 1 for triangular top inlays
                                                // 0 for rectangular.
UseLcdPot               = 1;                    // 1 for LCD contrast potentiometer,
                                                // 0 for not
PowerSwitchSelection    = 1;                    // 0 for rocker switch,
                                                // 1 for pushbutton switch.
InputDeviceSelection    = 0;                    // 0 for IR sensor,
                                                // 1 for 5-way switch.

// C O N F I G U R A B L E   P A R A M E T E R S
ArduinoPadSize          = [56, 74, 10];         // Pad for mounting the Arduino.
BaseFootRadius          = 12.5;                 // Radius of a base foot.
FBWidth                 = 41.91;                // Width of the finger board - usually around 42mm.
FretHeight              = 0.7;                  // Height of a fret (radius of semicircle used
                                                // to represent a fret).
HeadstockYAngle         = 13;                   // Size of headstock rotation about Y.
HeadThickness           = 9;                    // Thickness of the headstock.
LedDiameter             = 3;                    // Diameter of an LED (3mm used here).
LedHeight               = 3.5;                  // Height of LED to bottom ring.
NeckBackMatingHoleDepth = 8;                    // Depth of a mating hole.
MatingPinDiameter       = 4;                    // Diameter of pin used for mating the finger..
                                                // board to the neck back.
MatingSupportWidth      = 6;                    // Size of a side of a mating support.
NeckBackThickness       = 2;                    // Thickness of back (round part) of neck.
NeckExtension           = 25;                   // Extension to neck past last fret.
NeckZScale              = 0.75;                 // Scale factor for neck back radius.
NumStrings              = 6;                    // Number of strings (6 for guitar).
NutNotchRadius          = 2.5;                  // Radius of string notches in the nut.
NutWidth                = 5;                    // Width of the nut.
TunerAngle              = -17.1;                // Angle that tuners makes with neck.
TunerBaseThickness      = 1;                    // Thickness of the tuner assembly base.
TunerBushingHeight      = 1;                    // Height of bushing (collar) on tuning post.
TunerHoleRadius         = 4;                    // Radius of holes for tuner posts.
TunerPostHeight         = 11;                   // Height of a tuning post.
TunerShaftRadius        = 2;                    // Radius of a shaft between a tuner box and knob.
TunerStringHoleRadius   = 1.1;                  // Radius of string hole in tuner post.

BaseHeight              = 60;                   // The height of the base.
BaseWidth               = 200;                  // The width of the base.
BaseDepth               = 120;                  // The depth of the base.
BaseScale               = 0.85;                 // The linear_extrude scale (slope) of the base.
BaseOffset              = 5;                    // Radius of corners of the base.
BaseSupportStudDiameter = 6;                    // Diameter of stud mating the neck support to the base.
BaseThickness           = 3.6;                  // Thickness of the base shell.
SwitchDiameter          = 19.8;                 // Diameter of power switch.
SwitchNotchHeight       = 2.4;                  // Switch orientation notch height.
SwitchNotchWidth        = 1.5;                  // Switch orientation notch width;
SwitchNotchOffset       = [-SwitchDiameter / 2 - SwitchNotchWidth / 2 + .3, - BaseThickness, 0];
                                                // Switch orientation notch offset from center.

LcdBezelWidth           = 71.3;                 // Width of 16x2 LCD opening.
LcdBezelHeight          = 24.3;                 // Height of 16x2 LCD opening.

LcdMountRadius          = 3.8;                  // The radius of an LCD mount.
LcdMountHoleRadius      = 1.5;                  // The radius of an LCD mount hole.
LcdMountDepth           = 3.5;                  // The depth an LCD mount.
LcdMountWidth           = 74.8;                 // LCD mount spacing (width).
LcdMountHeight          = 30.0;                 // LCD mount spacing (height).

IrBezelWidth            = 4.6;                  // Width  of bezel for IR sensor.
IrBezelHeight           = 7.0;                  // Height of bezel for IR sensor.
IrBezelDepth            = 0.4;                  // Depth  of bezel frame for IR sensor.
IrWindowWidth           = 6.8;                  // Width  of window for IR sensor
IrWindowHeight          = 15;                   // Height of window for IR sensor (includes leads).
StayDiameter            = 6;                    // Diameter of an input sensor stay.
StayHoleDiameter        = 3;                    // Diameter of the hole in an input sensor stay.
StayThickness           = 2;                    // Thickness of an input sensor stay.

NeckSupportPinRadius    = 4;                    // Radius of neck support pin.
NeckSupportHeight       = NeckExtension - 5;    // Height of the neck support;
NeckSupportPinHeight    = NeckSupportHeight / 2;// Height of the neck support pin.

// C O L O R   P A R A M E T E R S
BaseColor               = "DarkBlue";               // Color for base.
FBColor                 = "black";              // Fingerboard color.
FretColor               = "silver";             // Fret color.
HeadstockColor          = "BurlyWood";          // Color used to display the headstock.
HeadTextColor           = "white";              // Color of text on headstock.
InlayColor              = "white";              // Inlay color.
LcdColor                = "LightBlue";          // Color of LCD.
NeckBackColor           = "BurlyWood";          // Neck back color.
NeckSupportColor        = "black";              // Color of neck support.
NutColor                = "white";              // Nut color.
TunerAssemblyColor      = "silver";             // Color used to display the tuner assembly.
TuningKnobColor         = "white";              // Color used to display a tuning knob.
TuningPostColor         = "silver";             // Color used to display a tuning post.

// Coordinates of the tuner holes (taken from on-line drawings).
// http://img.photobucket.com/albums/v208/jwells393/TeleHeadstock.jpg
TunerCoords = 
[
    [41.91, 20.32, 0], [65.25, 13.18, 0],  [88.6, 6.05, 0],
    [111.94, -1.1, 0], [135.28, -8.23, 0], [158.62, -15.37, 0]
];

// F R E T   O F F S E T S
// These fret offsets were taken from the Fretboard Calculator by The Tundra Man
// Workshop (http://www.tundraman.com/Guitars/FretCalc/index.php).  The table is
// based on a 25.5" Fender Stratocaster/Telecaster with 22 frets.
NutExtension            = NutWidth + 0.2;
FretOffsets             = [
                             NutExtension + 0, 
                             NutExtension + 36.316,
                             NutExtension + 70.596,
                             NutExtension + 102.954,
                             NutExtension + 133.498,
                             NutExtension + 162.329
                 //          NutExtension + 189.543,
                 //          NutExtension + 215.232,
                 //          NutExtension + 239.48,
                 //          NutExtension + 262.369,
                 //          NutExtension + 283.974,
                 //          NutExtension + 304.368,
                 //          NutExtension + 323.619,
                 //          NutExtension + 341.79,
                 //          NutExtension + 358.942,
                 //          NutExtension + 375.133,
                 //          NutExtension + 390.415,
                 //          NutExtension + 404.841,
                 //          NutExtension + 418.458,
                 //          NutExtension + 431.312,
                 //          NutExtension + 443.444,
                 //          NutExtension + 454.897,
                 //          NutExtension + 465.707
                          ];
FretOffsetSize          = len(FretOffsets);     // # elements in the fret offset array.
                          
// F R E T S   C O N T A I N I N G   I N L A Y S                          
InlayFrets              = [3, 5];               // Frets containing inlays.

// Some useful definitions.                                                
X                       = [1, 0, 0];            // X unit vector.
Y                       = [0, 1, 0];            // Y unit vector.
Z                       = [0, 0, 1];            // Z unit vector.



// D E R I V E D   P A R A M E T E R S
BaseWidthCenter         = BaseWidth  / 2;       // Base center (width).
BaseHeightCenter        = BaseHeight / 2;       // Base center (height).
HeadstockAngle          = Y * HeadstockYAngle;  // Angle that headstock makes with neck.
LcdMountWidthOffset     = LcdMountWidth  / 2;   // LCD width mount offset from center.
LcdMountHeightOffset    = LcdMountHeight / 2;   // LCD height mount offset from center.
SupportHoleYOffset      = -BaseDepth / 24 + 3;  // Y-offset of neck hole in support.
TunerBoxWidth           = 3 * TunerHoleRadius;  // Width of an individual tuner box.
TunerBushingRadius      = TunerHoleRadius + 1.5;// Radius of tuner bushing.
TunerDispOffset         = Z * 20;               // Z offset for displaying tuner parts.
TunerHoleSpacing        = distance(TunerCoords[0], TunerCoords[1]);
                                                // Distance between adjacent tuner holes in headstock
TunerBaseLength         = NumStrings * TunerHoleSpacing; 
                                                // Length of the tuner assembly base.
TunerBaseWidth          = TunerBoxWidth + 2;    // Width of the tuner assembly base.
TunerBoxLength          = TunerHoleSpacing * 0.75; // Length of an individual tuner box.
TunerBoxHeight          = 3 * TunerShaftRadius; // Height of a tuner box.
TunerShaftLength        = TunerHoleSpacing * 0.8;// Length of a shaft between a tuner box and knob.
ExplodeYOffset          = Y * 15;               // Y-Offset for exploded part placement. 
ExplodeZOffset          = Z * 15;               // Z-Offset for exploded part placement. 
NeckLength              = FretOffsets[len(FretOffsets) - 1] + NeckExtension;
                                                // Length of the neck from the nut to the end.
FBThickness             = HeadThickness * cos(HeadstockYAngle);
                                                // Thickness of the fingerboard.
NutHeight               = FBThickness + 2;      // Height of nut.
FBMatingHoleDepth       = FBThickness - 2;      // Depth of finger board mating holes.

// The angle about the X axis that the front of the base makes due to BaseScale.
BaseXAngle    = atan2(0.5 * BaseDepth * (1 - BaseScale), BaseHeight);




// C O M M O N   M O D U L E   D E F I N I T I O N S


// The nut.
module Nut(tolerance = 0)
{
    StudHeight=6;
    RoundRadius = 2;
    
    difference()
    {
        intersection()
       {
            // The nut body.
            cube([NutWidth, FBWidth, NutHeight]);
             // The outside of the box.
             translate([-1, FBWidth / 2, -RoundRadius]) rotate([90, 0, 90])
            linear_extrude(height = NutHeight)
            {
                offset(r = RoundRadius)
                {
                    square([FBWidth - 2 * RoundRadius, NutHeight * 2], center = true);
                }
            }
        }
        // String notches.
        for (i = [1:NumStrings])
        {
            translate([-1, i * (FBWidth / (NumStrings + 1)), NutHeight]) 
                rotate(Y * 90) cylinder(d = NutNotchRadius / 2, h = NutWidth + 2);
        }
    }
    // Add the mounting studs.
    translate([NutWidth / 2, FBWidth / 4, -StudHeight + 0.1]) 
        cylinder(d = NutWidth - 1.5 + tolerance, h = StudHeight);
    translate([NutWidth / 2, 3 * FBWidth / 4, -StudHeight + 0.1]) 
        cylinder(d = NutWidth - 1.5 + tolerance, h = StudHeight);
}

