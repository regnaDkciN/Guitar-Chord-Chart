////////////////////////////////////////////////////////////////////////////////
//
// ChordChartBase.scad
//
// Contains OpenScad code for the base for the guitar chord chart
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

include <ChordChartParameters.scad>             // For part parameters.
include <Arduino.scad>                          // For Arduino().
use<MCAD/boxes.scad>                            // For roundedBox().
use <pins.scad>                                 // For pintack().
use<MCAD/regular_shapes.scad>                   // For torus().

// Diagnostic - display the front panel angle for diagnostic purposes only.
echo("BaseXAngle = ", BaseXAngle);


// Retainer pin for holding the Arduino to the base and the LCD to the base.
// Four pins will be needed for each device (8 total).
module ArduinoAndLcdPin()
{
    baseRadius   = 2.6;
    baseHeight   = 4;
    pinHeight    = 7.7;
    pinRadius    = 1.6;
    lipHeight    = 3;
    lipThickness = 1;
    
    pintack(h = pinHeight, r = pinRadius, lh = lipHeight, lt = lipThickness,
            bh = baseHeight, br = baseRadius);
}

// Retainer pin for holding the neck to the neck support.
module NeckSupportPin()
{
    baseRadius   = 7;
    baseHeight   = 5;
    pinHeight    = 8;
    pinRadius    = NeckSupportPinRadius - 0.1;
    lipHeight    = 3;
    lipThickness = 0.2;
    
    difference()
    {
        // The pin itself.
        pintack(h = pinHeight, r = pinRadius, lh = lipHeight, lt = lipThickness,
                bh = baseHeight, br = baseRadius, $fn = 60);
        // Notch for easy removal.
        translate([0, 0, baseHeight * 0.3]) 
            torus(outerRadius = baseRadius + 0.75, innerRadius = baseRadius - 0.75,
                 $fn = 60);
    }
}

// Wire hanger for holding wires inside the sides of the base.  The 'h' argument
// specifies the height of the hanger hole.
module WireHanger(h = 0)
{
    thickness  = 3;
    holeHeight = 2 * 2;
    holeWidth  = 3;
    height     = holeHeight + thickness + h;
    width      = holeWidth  + 2 * thickness;
    
    translate([-width / 2, -width / 2, 0])
    difference()
    {
        // Use a rounded box so the edges won't cut into the wire.
        translate([width / 2, width / 2, height / 2])
            roundedBox(size = [width, width, height], radius = 1, sidesonly = false, $fn = 10);
        // The holes for the tie wraap.
        translate([(width - holeWidth) / 2, -1, h + 1])
            cube([holeWidth, width + 2, holeHeight]);
        translate([-1, 3, h + 1])
            cube([width + 2, holeWidth, holeHeight]);
        translate([(width - 2 * holeWidth) / 2, (width - 2 * holeWidth) / 2, h + 1])
            cube([2 * holeWidth, 2 * holeWidth, thickness + 3.1]);
    }
    
}

// Shape of the inside of the neck.  Used for making holes.
// length = the length or height of the hole.
module InsideNeck(length = 1)
{
    difference()
    {
        // Inside diameter of the neck (before scaling).
        diameter = FBWidth - (NeckBackThickness * 2);
        
        // Start with the inner part of the neck.
        rotate(Z * 90) scale([NeckZScale, 1, 1])
            cylinder(d = diameter, h = length);
            
        // Remove the top half of the cylinder.
        translate([-1 - diameter / 2, -diameter, -1]) 
            cube([diameter + 2, diameter, length + 2]);
    }
}

// Shape of the outside of the neck.  Used for making holes.
// length = the length or height of the hole.
module OutsideNeck(length = 1)
{
	difference()
	{
        // Create the outer part of the cylinder shell.
		rotate(Z * 90) scale([NeckZScale, 1, 1]) 
            cylinder(d = FBWidth + 0.2, h = length);
            
        // Remove the top half of the cylinder.
		translate([-1 - FBWidth / 2, -FBWidth, -1]) 
            cube([FBWidth + 2, FBWidth, length + 2]);
	}
    // Add the finger board.
    translate([-FBWidth / 2, -FBThickness, 0]) 
        cube([FBWidth + 0.2, FBThickness + 0.2, length]);
}

// Stud that holds the neck support to the base.  The 't' argument is used to
// specify extra hole dimensions so studs will fit into corresponding holes.
module NeckSupportStud(t = 0)
{
    height = BaseThickness + 2 * t + 5;
    diameter = BaseSupportStudDiameter + t;
    
    // Make the stud itself.
    cylinder(d = diameter, h = height);
    // Round the end of the stud for easier insertion.
    sphere(d = diameter);
}

// This piece is used to attach the neck to the base.  The 'tol' argument is used
// to specify extra radius for holes so the mating stud will fit inside.
module NeckSupport(tol = 0)
{ 
    supportHeight = NeckSupportHeight;
    
    difference()
    {
        // The outside of the support.  Make sure we leave some room for the
        // last fret by making the support 5mm shorter than the neck extension.
        linear_extrude(height = supportHeight, scale = BaseScale)
        {
            offset(r = BaseOffset)
            {
                square([BaseWidth / 4, BaseDepth / 4], center = true);
            }
        }
        // Hole for the neck.
        translate([0, SupportHoleYOffset, -1]) 
            scale([1.01, 1.02, 1])
            OutsideNeck(length = NeckExtension + 2);
        // Hole for the inside of the neck (goes through top of the base).
        translate([0, SupportHoleYOffset, -BaseThickness - 1]) 
            InsideNeck(length = NeckExtension + BaseThickness + 2);

        // Add a hole for the neck support retention pin.
        translate([0, 22.2, NeckSupportPinHeight]) rotate([90, 0, 0]) 
            scale(1.025) NeckSupportPin(); 
        translate([0, 22.2, NeckSupportPinHeight]) rotate([90, 0, 0]) 
            cylinder(r = NeckSupportPinRadius + 0.1, h = 10);           
    }
    
    // Add the mating studs.
    translate([BaseWidth / 4 / 2, BaseDepth / 4 / 2, -BaseThickness - 2 * tol - 5]) 
        NeckSupportStud(t = tol);
    translate([BaseWidth / 4 / 2, -BaseDepth / 4 / 2, -BaseThickness - 2 * tol - 5]) 
        NeckSupportStud(t = tol);
    translate([-BaseWidth / 4 / 2, BaseDepth / 4 / 2, -BaseThickness - 2 * tol - 5]) 
        NeckSupportStud(t = tol);
    translate([-BaseWidth / 4 / 2, -BaseDepth / 4 / 2, -BaseThickness - 2 * tol - 5]) 
        NeckSupportStud(t = tol);
}        

// The envelope of the outside shell of the base.  Can be used with intersection()
// to limit add ons to be within the scope of the base.
module OutsideBase()
{
    difference()
    {
        // The outside of the box.
        linear_extrude(height = BaseHeight, scale = BaseScale)
        {
            offset(r = BaseOffset)
            {
                square([BaseWidth, BaseDepth], center = true);
            }
        }
        // Remove the bevelled box back.
        translate([0, BaseDepth / 2 * BaseScale + BaseOffset,
                      BaseHeight / 2 + 0.5])
            cube([BaseWidth + 2 * BaseOffset + 1, 20, BaseHeight + 2], center = true);
    }
}        

// The envelope of the inside of the base.  Can be differenced with OutsideBase()
// to form the shell of the base.
module InsideBase()
{
    // The actual dimensions of the base.
    w = BaseWidth + 2 * BaseOffset;
    d = BaseDepth / 2 + BaseOffset + BaseDepth * BaseScale / 2;
    h = BaseHeight;
    
    // Scale factors used to get the desired base thickness.
    XFactor = (w - 2 * BaseThickness) / w;
    YFactor = (d - 2 * BaseThickness) / d;
    ZFactor = (h - BaseThickness) / h;
    
    // The scaled opening for the neck into the neck support.
    translate([0, 0, -0.02]) 
        scale([XFactor, YFactor, ZFactor])
        OutsideBase();
}

// Corner feet support, can be used to apply sticky rubber feet.
module BaseFoot(xlation = [0, 0, 0])
{   
    intersection()
    { 
        translate(xlation)
            cylinder(r = BaseFootRadius, h = BaseHeight, $fn = 30);
        OutsideBase();
    }
}     
                
// Translate in the base front coordinate system.
module TranslateInFrontCoords(xlation = [0, 0, 0])
{
    // Offset to the local coordinate system.
    LocalXlation  = [-BaseWidth / 2,
                    -(BaseDepth + BaseOffset) / 2, 
                    -sin(BaseXAngle) * BaseDepth / 2];
    
    // Rotate/translate each child using the front of the base as reference.
    for (theChild = [0:$children - 1])
    {
        rotate([-BaseXAngle, 0, 0]) 
        translate(LocalXlation + xlation)
        children(theChild);
    }
}

// Translate in the base top coordinate system.
module TranslateInTopCoords(xlation = [0, 0, 0])
{
    // Offset to the local coordinate system.
    LocalXlation  = [0, 0, BaseHeight];
    
    // Rotate/translate each child using the front of the base as reference.
    for (theChild = [0:$children - 1])
    {
        translate(LocalXlation + xlation)
        children(theChild);
    }
}

// A hole in the LCD mount.
module LcdMountHole(offset = [0, 0, 0])
{
    // The screw hole in the mount.
    TranslateInFrontCoords([BaseWidthCenter - offset[0], 
                            0.6 + BaseThickness / 2 + LcdMountDepth + 1,
                            BaseHeightCenter - offset[2]])
        rotate([90, 0, 0])
        cylinder(r = LcdMountHoleRadius, h = LcdMountDepth + 4, $fn=25);
}

// A mount for the LCD inside the base.
module LcdMount(offset = [0, 0, 0])
{
    difference()
    {
        // The mount itself.
        TranslateInFrontCoords([BaseWidthCenter - offset[0], 
                                0.5 + BaseThickness / 2 + LcdMountDepth,
                                BaseHeightCenter - offset[2]])
            rotate([90, 0, 0])
            cylinder(r1 = LcdMountRadius, r2 = LcdMountDepth + LcdMountRadius, h = LcdMountDepth, $fn = 30);
            
        // The screw hole in the mount.
        LcdMountHole(offset);
    }
}

// The base itself without holes and mounting posts, etc.
module BaseShell()
{
    difference()
    {
        // Start with the outside of the box.
        OutsideBase();
        
        // Remove the inside of the base, forming a shell.
        InsideBase();
    }
}

// The hole for the power switch.  If PowerSwitchSelection == 0, a rocker switch
// is used.  If it is 1, a pushbutton switch is used (Adafruit product ID: 1477.
// https://www.adafruit.com/product/1477),
module PowerSwitchHole()
{
    // 0 == Rocker switch selection.
    if (PowerSwitchSelection == 0)
    {
        // Add the main circular part of the hole.
        rotate([90, 0, 0])
            cylinder(d = SwitchDiameter, h = BaseThickness + 2);
        // Add the side and top/bottom lugs.
        translate(SwitchNotchOffset)
            cube([SwitchNotchWidth, BaseThickness + 4, SwitchNotchHeight], center = true);
        translate([0, -2, 0])
            cube([4, BaseThickness + 4, SwitchDiameter + 2], center = true);
    }
    else    // The pushbutton switch is selected.
    {
        intersection()
        {
            // Add the main circular part of the hole.
            rotate([90, 0, 0])
                cylinder(d = 16, h = BaseThickness + 2);
            // Add the squared off sides.
            translate([0, -3, 0])
                cube([15, BaseThickness + 4, 18], center = true);
        }
    }
}

// The hole for the power jack (Adafruit #610, https://www.adafruit.com/products/610).
module PowerJackHole()
{
    intersection()
    {
        // Add the main circular hole for the power jack.
        rotate([90, 0, 0])
            cylinder(d = 12.9, h = BaseThickness + 2, center = true);
        // Add the squared off sides.
        cube([12, BaseThickness + 4, 15], center = true);
    }
}

// Hole for the input device.  
module InputDeviceHole()
{
    // If InputDeviceSelection == 0, an IR sensor is used.  
    // If 1, a 5-way switch is used (Adafruit #504, https://www.adafruit.com/products/504).
    if (InputDeviceSelection == 0)
    {
        // Add the window for the IR sensor.
        cube([IrBezelWidth, BaseThickness + 2, IrBezelHeight], center = true);
        translate([0, 1.4, 0])
            cube([IrWindowWidth, BaseThickness + 0.2, IrWindowHeight], center = true);        
    }
    else    // 5-way switch is selected.
    {
        translate([0, BaseThickness - .1, 0])
            rotate([90, 0, 0])
            cylinder(d = 7.5, h = BaseThickness + 2);
        translate([0, 1.4, 0])
            cube([10.1, BaseThickness + 0.2, 10.4], center = true);
    }
}

// Stays used to hold the input device in place on the front panel.  Used with
// both IR sensor and 5-way switch.
module InputDeviceHold()
{
    rotate([-90, 0, 0])
    difference()
    {
        // Start with a cube the size of stay.
        translate([StayThickness / 2, 0, 0]) cube([StayThickness, 3 * StayDiameter, StayDiameter], center = true);
        // Bevel both sides.
        translate([StayThickness / 2, 5 * StayThickness, 0]) rotate([45, 0, 0])
            cube([StayThickness + 2, StayDiameter, 3 * StayDiameter], center = true);
        translate([StayThickness / 2, -5 * StayThickness, 0]) rotate([-45, 0, 0])
            cube([StayThickness + 2, StayDiameter, 3 * StayDiameter], center = true);
        // Add a hole for the holding rod.
        rotate([90, 0, 90]) translate([0, 0, -1]) cylinder(d = StayHoleDiameter, h = StayThickness + 2);
    }
}

// Rod that fits into the InputDeviceHoldingRings and holds the input device in place.
module InputDeviceRetainingPin()
{
    cylinder(d = StayHoleDiameter - 0.5, h = 24);
    difference()
    {
        cylinder(d = 3 * StayHoleDiameter, h = 2.6);
        translate([StayHoleDiameter / 1.5, -1.5 * StayHoleDiameter, -0.2])
            #cube([4 * StayHoleDiameter, 3 * StayHoleDiameter, 3]);
        translate([-4 * StayHoleDiameter - StayHoleDiameter / 1.5, -1.5 * StayHoleDiameter, -0.2])
            #cube([4 * StayHoleDiameter, 3 * StayHoleDiameter, 3]);
    }
}

// Mount for LCD contrast potentiometer.
module PotMount()
{
    mountHeight    = 22;
    mountWidth     = 20;
    mountThickness = 3.5;
    tabOffset      = 7.75;
    tabDiameter    = 3;
    shaftDiameter  = 6.9;
    supportDepth   = 5;
    
    difference()
    {
        // The mount itself.
        union()
        {
            translate([0, 0, mountHeight / 2])
                cube([mountWidth, mountThickness, mountHeight], center = true);
            translate([0, mountThickness / 2, mountHeight]) rotate([90, 0, 0]) 
                cylinder(r = mountWidth / 2, h = mountThickness);
            
        }
        // The hole for the pot.
        translate([0, mountThickness / 2 + .1, mountHeight]) rotate([90, 0, 0])
            cylinder(r = shaftDiameter / 2, h = 2 * mountThickness);
        // The hole for the alignment tab.
        translate([0, mountThickness / 2 + .1, mountHeight + tabOffset]) rotate([90, 0, 0])
            cylinder(r = tabDiameter / 2, h = 2 * mountThickness);
    }
    // Support for the support.
    intersection()
    {
        translate([0, 0, supportDepth / 2]) 
            cube([mountWidth, supportDepth * 2 + mountThickness, supportDepth], center = true);
        translate([-mountWidth / 2, -supportDepth - mountThickness / 2, 0]) rotate([-45, 0, 0]) 
            cube([mountWidth, (supportDepth + mountThickness/2) * sqrt(2), (supportDepth + mountThickness/2) * sqrt(2)]);
    }
}


// The entire base with all mounting holes and posts.
module Base()
{   
    LcdHoleXBase = BaseWidth  / 2;
    LcdHoleZBase = BaseHeight / 2;
        
    difference()
    {
        union()
        {
            // Start with the blank base.
            BaseShell();
            
            // The LCD mounts within the case.
            LcdMount([-LcdMountWidthOffset, 0, -LcdMountHeightOffset - 0.6]);
            LcdMount([ LcdMountWidthOffset, 0, -LcdMountHeightOffset - 0.6]);
            LcdMount([-LcdMountWidthOffset, 0,  LcdMountHeightOffset]);
            LcdMount([ LcdMountWidthOffset, 0,  LcdMountHeightOffset]);
            
            // Add the Arduino pad.
            TranslateInTopCoords([-60, 7, -BaseThickness - ArduinoPadSize[2] / 2])
                difference()
                {
                        cube(ArduinoPadSize, center = true);
                        cube(ArduinoPadSize + [-20, -20, 0.2], center = true);
                }
                
            // Add the Arduino bumper to cradle the Arduino.
            rotate([180, 0, 0]) rotate([0, 0, 90])
                TranslateInTopCoords([-5.5, 
                                     60,
                                      -2 * BaseHeight+BaseThickness - 0.1 + ArduinoPadSize[2]])
                    difference()
                    {
                        import("Arduino_Bumper_0006.stl", convexity=3);
                        translate([-33, 0, 2])
                            cube([20, 57, 8], center = true);
                    }
                
            // Add supports for IR sensor stay.
            TranslateInFrontCoords([BaseWidth * 0.8 - StayThickness / 2 - 8, 
                                    BaseThickness + .5, 
                                    BaseHeight / 2])
                InputDeviceHold();
            TranslateInFrontCoords([BaseWidth * 0.8 - StayThickness / 2 + 8, 
                                    BaseThickness + .5, 
                                    BaseHeight / 2])
                InputDeviceHold();

            // Add a foot support at each corner of the base.           
            BaseFoot([-BaseWidth / 2,
                      BaseDepth / 2 * BaseScale - BaseOffset,
                      0]);
            BaseFoot([BaseWidth / 2,
                      BaseDepth / 2 * BaseScale - BaseOffset,
                      0]);
            BaseFoot([BaseWidth / 2,
                      -BaseDepth / 2,
                      0]);
            BaseFoot([-BaseWidth / 2,
                      -BaseDepth / 2,
                      0]);

            // Add wire hangers.
            // Near power jack.
            TranslateInTopCoords([0, BaseDepth / 8, -BaseThickness + 1]) 
                rotate([0, 180, 90]) WireHanger(9);
            // Near Arduino and neck hole.
            TranslateInTopCoords([-24, 0, -BaseThickness + 1])
                rotate([0, 180, 0]) WireHanger(9);
            // Near input sensor.
            TranslateInTopCoords([BaseWidth / 4, -BaseDepth / 5, -BaseThickness + 1]) 
                rotate([0, 180, 0]) WireHanger(9);
        } // end union().
        
        // Create the hole for the LCD display.
        TranslateInFrontCoords([LcdHoleXBase, BaseThickness/2 - 1, LcdHoleZBase])
            cube([LcdBezelWidth, BaseThickness + 20, LcdBezelHeight], center = true);
        TranslateInFrontCoords([LcdBezelWidth / 2 + LcdHoleXBase + 2, 2, LcdHoleZBase])
            cube([5, BaseThickness, 17], center = true);

        // Create the holes for the Arduino.
        TranslateInTopCoords([-84, 25, BaseThickness - 9 - ArduinoPadSize[2]]) 
            rotate([0, 180, 90]) Arduino(1, 0, 1);
        
        // The LCD mounts holes within the case.
        LcdMountHole([-LcdMountWidthOffset, 0, -LcdMountHeightOffset - 0.6]);
        LcdMountHole([ LcdMountWidthOffset, 0, -LcdMountHeightOffset - 0.6]);
        LcdMountHole([-LcdMountWidthOffset, 0,  LcdMountHeightOffset]);
        LcdMountHole([ LcdMountWidthOffset, 0,  LcdMountHeightOffset]);  
        
        // Power switch hole.
        TranslateInFrontCoords([(1 - BaseScale) * BaseWidth / 2 + 15,
                                 BaseThickness, BaseHeight / 2])
            PowerSwitchHole();
            
        // Add hole for power jack.
        translate([0, BaseDepth / 2 * BaseScale - BaseOffset - 1, 20])
            PowerJackHole();
            
        // Add the window for the input device.
        TranslateInFrontCoords([BaseWidth * 0.8, 
                                0, 
                                BaseHeight / 2])
            InputDeviceHole();

        // Add the neck support mounting holes.    
        translate([0, SupportHoleYOffset + NeckBackThickness, BaseHeight + .02])
            NeckSupport(tol = 0.2);
            
        // Add the hole for the neck's LED wires.    
        translate([0, SupportHoleYOffset - NeckBackThickness, BaseHeight - BaseThickness - 1]) 
            InsideNeck(length = NeckExtension + 2);            
    } // end difference().
    
    if(UseLcdPot)
    {
        // Mounting for the LCD contrast pot.
        TranslateInTopCoords([60, 10, -3.3]) rotate([180, 0, 90]) PotMount();
    }
}

// Show the Arduino module.  For display purposes only.
module ShowArduino()
{
    // Add the Arduino itself (for display purposes only).
    TranslateInTopCoords([-69, 25, BaseThickness - 9 - ArduinoPadSize[2]]) 
        rotate([0, 180, 90])
        Arduino(1, 0, 1);
}

// Show the LCD module.  For display purposes only.
module ShowLcd()
{
    // Add the 16 x 2 LCD (for display purposes only).
    color(LcdColor) TranslateInFrontCoords([42, 7, 54])
        rotate([-90, -90, 0])
        import("SUNLIKE-SC1602B-High.stl", convexity=3);
}
 
// Show the neck support.
module ShowNeckSupport()
{
     // Add the neck support (for display purposes only).
    color(NeckSupportColor)
        translate([0, 0, BaseHeight - 0.1])
        NeckSupport(tol = 0);
}

// Paddle for the 5-way switch.
module Paddle()
{
    // difference()
    // {
        // // The body of the paddle.
        // cylinder(r1 = 6.5 / 2, r2 = 10 / 2, h = 10);        
        // // Add a cutout for the switch nub.
        // translate([0, 0, 3-.1])
            // cube([3.6, 3.4, 6], center = true);
        // // Indent the end of the paddle for easy use.
        // translate([0, 0, 19])
            // sphere(d = 20);
    // }
    angle = -10;
    width = 16;
    depth = 10;
    thickness = 1;
    height = 4;
    radius1 = 6.5 / 2;
    radius2 = 10 / 2;
    cylinderHeight = 7;
    roundingRadius = 0.75;
    difference()
    {
        // The body of the paddle.
        cylinder(r1 = radius1, r2 = radius2, h = cylinderHeight);        
        // Add a cutout for the switch nub.
        translate([0, 0, 3-.1])
            cube([3.6, 3.4, 6], center = true);
        // Indent the end of the paddle for easy use.
        translate([0, 0, 16])
            sphere(d = 20);
    }
    translate([width - 1.75 * radius1, 0, height]) rotate([0, angle, 0])
        roundedBox(size = [width, depth, thickness], radius = roundingRadius, sidesonly = false, $fn = 12);
    translate([-width + 1.75 * radius1, 0, height]) rotate([0, angle, 180])
        roundedBox(size = [width, depth, thickness], radius = roundingRadius, sidesonly = false, $fn = 12);
    translate([0, -width + 1.75 * radius1, height]) rotate([-angle, 0, 180])
        roundedBox(size = [depth, width, thickness], radius = roundingRadius, sidesonly = false, $fn = 12);
    translate([0, width - 1.75 * radius1, height]) rotate([angle, 0, 180])
        roundedBox(size = [depth, width, thickness], radius = roundingRadius, sidesonly = false, $fn = 12);
}

// Print the neck support, oriented for printing.
module PrintNeckSupport()
{
   translate([0, 0, NeckExtension - BaseThickness]) rotate([180, 0, 0])
        NeckSupport();
}

// Print the base, oriented for printing.
module PrintBase()
{
    translate([0, 0, BaseHeight]) rotate([0, 180, 90]) Base();
}

//===============================================================================
// Modules below are for testing individual sections of the base by printing
// out only the portion of the base that corresponds to the piece that needs 
// be tested.  These can be used to help tailor the device openings to the 
// actual devices being used.
//===============================================================================

// Test the entire front of the base.  Rotated to be printable laying flat on
// the print bed.
module TestFrontRotated()
{
    translate([0, 0, BaseHeight + BaseThickness * (1 + sin(BaseXAngle))])
        rotate([BaseXAngle + 90.5, 0, 0]) 
        intersection()
        {
            Base();
            TranslateInFrontCoords([BaseWidth / 2, 1, BaseHeight / 2])
                cube([BaseWidth + 2 * BaseOffset, BaseThickness + 8, BaseHeight], center = true);
        }
}

// Test the entire front of the base.  Print will be upright, just as if it were
// being printed with the whole base.
module TestFrontUpright()
{
    translate([0, -BaseDepth / 2, BaseHeight]) rotate([180, 0, 0]) 
        intersection()
        {
            Base();
            TranslateInFrontCoords([BaseWidth / 2, 1, BaseHeight / 2])
                cube([BaseWidth + 2 * BaseOffset, BaseThickness + 8, BaseHeight + 2], center = true);
        }
}

// Test the power switch mount.
module TestPowerSwitch()
{
    translate([70, SwitchDiameter, BaseHeight + BaseThickness * (1 + sin(BaseXAngle))])
        rotate([BaseXAngle + 90, 0, 0]) 
        intersection()
        {
            Base();
            TranslateInFrontCoords([(1 - BaseScale) * BaseWidth / 2 + 15, 1, BaseHeight / 2])
                cube([SwitchDiameter * 2, BaseThickness + 2, SwitchDiameter * 2], center = true);
        }    
}

// Test the power switch mount.
module TestPowerSwitchUpright()
{
    translate([70, -BaseDepth / 2, BaseHeight])
        rotate([180, 0, 0]) 
        intersection()
        {
            Base();
            TranslateInFrontCoords([(1 - BaseScale) * BaseWidth / 2 + 15, 1, BaseHeight / 2])
                cube([SwitchDiameter * 2.5, BaseThickness + 35, BaseHeight + 2], center = true);
        }    
}

// Test the IR sensor mounting.
module TestInputDeviceRotated()
{
    translate([-60, 20, BaseHeight + BaseThickness * (1 + sin(BaseXAngle))])
        rotate([BaseXAngle + 90, 0, 0]) 
        intersection()
        {
            Base();
            TranslateInFrontCoords([BaseWidth * 0.8, 1, BaseHeight / 2])
                cube([IrWindowWidth * 2, BaseThickness + 8, IrWindowHeight * 2], center = true);
        }    
}

// Test the IR sensor mounting.
module TestInputDeviceUpright()
{
    translate([-60, -60, BaseHeight])
        rotate([180, 0, 0]) 
        intersection()
        {
            Base();
            TranslateInFrontCoords([BaseWidth * 0.8, 1, BaseHeight / 2])
                cube([IrWindowWidth * 4.5, BaseThickness + 55, BaseHeight + 2], center = true);
        }    
}

// Test the LCD mount.
module TestLcd()
{
    translate([0, 20, BaseHeight + BaseThickness * (1 + sin(BaseXAngle))])
        rotate([BaseXAngle + 90, 0, 0]) 
        intersection()
        {
            Base();
            TranslateInFrontCoords([BaseWidth / 2, 1, BaseHeight / 2])
                cube([LcdBezelWidth + 20, BaseThickness + 8, LcdBezelHeight + 20], center = true);
        }
}

// Test the Arduino support.
module TestArduino()
{
    translate([-40, 0, 60])
        rotate([0, 180, 0])
            intersection()
            {
                Base();
                TranslateInTopCoords([-44, 7, -BaseThickness - ArduinoPadSize[2] / 2])
                    cube(ArduinoPadSize + [10, 20, 40], center = true);
            }
}

// Test the neck support holes in the top of the base.
module TestNeckSupportMount()
{
   translate([0, 0, BaseHeight])
        rotate([0, 180, 0])
            intersection()
            {
                Base();
                TranslateInTopCoords([0, 0, -BaseThickness - ArduinoPadSize[2] / 2])
                    cube([BaseWidth / 4 + 15, BaseDepth / 4 + 15, 40], center = true);
            }
}

// Test the power jack mount.
module TestPowerJackUpright()
{
    translate([0, BaseDepth / 2 * BaseScale, BaseHeight])
        rotate([180, 0, 0]) 
        intersection()
        {
            Base();
            translate([0, BaseDepth / 2 * BaseScale - BaseOffset - 1, 20])
                cube([20 * 2, 70, BaseHeight + 20], center = true);
        }    
}



// Test the LCD opening.
module TestLcdHole()
{
    boxDepth = 69;
    
    intersection()
    {
        translate([(BaseDepth - boxDepth) / 2, -LcdBezelWidth / 2 - 9, -1]) 
            cube([boxDepth, LcdBezelWidth + 18, BaseHeight + 2]);
        PrintBase();
    }
}

module TestNeckSupportPin()
{
    intersection()
    {
        PrintNeckSupport();
        translate([0, -10, 14]) cube([20, 30, 30], center = true);
    }
}


// ShowArduino();
// ShowLcd();
// color("yellow") ShowNeckSupport();
// TestPowerSwitchUpright();
// TestInputDeviceUpright();
// TestPowerJackUpright();
// Paddle();
// TestArduino();
// TestLcd();
// TestLcdHole();
// PotMount();
// ArduinoAndLcdPin();
// TestNeckSupportPin();


PrintBase();
// InputDeviceRetainingPin();
// PrintNeckSupport();
// NeckSupportPin();
