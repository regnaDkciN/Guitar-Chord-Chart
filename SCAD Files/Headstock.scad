////////////////////////////////////////////////////////////////////////////////
//
// Headstock.scad
//
// Contains OpenScad code for the headstock for the guitar chord chart
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
/////////////////////////////////////////////////////////////////////////////////

include <ChordChartParameters.scad>             // For part parameters.
use<MCAD/regular_shapes.scad>                   // For torus().
use<MCAD/boxes.scad>                            // For roundedBox().


// Local constants
HoleTolerance           = 0.2;                  // Amount to enlarge holes.
PrinterTolerance        = 0.2;                  // Amount to enlarge hole diameters
                                                //    due to printing tolerance.


// Position children at tuner holes.
module TunerPositions()
{
    // For each tuner hole.
    for (xlation = TunerCoords)
    {
        // For each child.
        for (theChild = [0:$children - 1])
        {
            // Translate the child.
            translate(xlation) children(theChild);
        }
    }
}

// A single tuner shaft that connects the tuner box to the tuning knob.
module TunerShaft(tolerance = 0)
{
    // Shaft which will plug into a tuning knob.
    cylinder(r = TunerShaftRadius + tolerance / 2 + 0.1, h = TunerShaftLength);
}

// A single tuner box.  Part of the tuner assembly.
module TunerBox()
{
    difference()
    {
        // A tuner box consists of a box...
        translate([(TunerHoleSpacing - TunerBoxLength) / 2, 
                   (TunerBaseWidth-TunerBoxWidth) / 2, TunerBaseThickness])
                   cube([TunerBoxLength, TunerBoxWidth, TunerBoxHeight]);
        // ... and a hole for the shaft which will plug into a tuning knob.
        translate([2 * TunerHoleSpacing / 3,
                  TunerBaseWidth / 2, TunerBoxHeight / 2 + TunerBaseThickness])
          rotate(X * 90) 
          TunerShaft(PrinterTolerance);
    }
}

// A tuner post contains the concave post, a bushing, and a hole for the string.
module TuningPost()
{
        difference()
        {
            // The post itself.
            cylinder(r = TunerHoleRadius-PrinterTolerance / 2, 
                     h = HeadThickness + TunerBushingHeight + TunerPostHeight +
                       TunerBoxHeight / 2 + TunerBaseThickness - PrinterTolerance - 2);
            // Make the post concave by subtracting a torus from its outside.
            translate([0, 0, HeadThickness + TunerPostHeight / 2 + TunerBushingHeight +
                       TunerBoxHeight / 2 + TunerBaseThickness - 2])
                torus(outerRadius = 30, innerRadius = TunerHoleRadius - 0.7);
            // The hole for the string.
            translate([-TunerHoleRadius - 1, 0, 
                      HeadThickness+TunerPostHeight / 2 + TunerBushingHeight +
                      TunerBoxHeight / 2 + TunerBaseThickness - 2])
                      rotate(Y * 90) cylinder(r = TunerStringHoleRadius, 
                                              h = TunerHoleRadius * 2 + 2);
        }
        // The bushing around the post.
        translate([0, 0, HeadThickness + TunerBoxHeight / 2 + TunerBushingHeight - PrinterTolerance - 2]) 
            cylinder(r1 = TunerBushingRadius, r2 = TunerHoleRadius, h = TunerBushingHeight);
}


// A tuning knob that connects to the tuning shaft.
module TuningKnob()
{
    height = 2 * TunerShaftRadius + PrinterTolerance + 2.4;
    intersectDiameter = 4 * TunerHoleSpacing;

//    rotate(Z * (-90+TunerAngle)) 
    difference()
    {
        intersection()
        {
            // The basic knob.
            scale([0.75, 1, 1]) cylinder(d = TunerHoleSpacing * 0.75, h = height);
            // Round the top of the knob.
            translate([0, 0, -intersectDiameter / 2 + height])
                sphere(d = intersectDiameter);
            // Round the bottom of the knob.
            translate([0, 0, intersectDiameter / 2]) sphere(d = intersectDiameter);
        }
        // Bore out the hole for the tuning shaft.
        translate([2, 0, TunerShaftRadius + PrinterTolerance + 1]) rotate(Y * 90)
            cylinder(r = TunerShaftRadius + PrinterTolerance, h = 10);
    }
}

// A tuner assembly that includes the tuner base and tuner boxes.
module TunerAssembly()
{
    // Rotate the entire assembly.
    rotate([180, 0, TunerAngle])
    difference()
    {
        union()
        {
            // The tuner base that accommodates all 6 tuner boxes.
            translate([TunerBaseLength / 2, 
                       TunerBaseWidth / 2, 
                       TunerBaseThickness / 2])
            roundedBox(size = [TunerBaseLength, TunerBaseWidth, 
                       TunerBaseThickness], radius = 3, sidesonly = true);
            // The six tuner boxes.
            for (i = [0:len(TunerCoords) - 1])
            {
                translate([i * TunerHoleSpacing, 0, 0]) TunerBox();
            }
        }
        // The holes in the base and box to accommodate the tuner mounting post.
        for (i = [0:len(TunerCoords) - 1])
        {
            translate([i * TunerHoleSpacing + TunerHoleSpacing / 3,
                       TunerBaseWidth / 2, 
                      -(TunerBoxHeight + TunerBaseThickness)])
            cylinder(r = TunerHoleRadius, 
                     h = HeadThickness / 2 + TunerBushingHeight + TunerBoxHeight - 1 - PrinterTolerance);
        }
    }
}

// The headstock without holes.  All dimensions gathered from the internet.
// http://img.photobucket.com/albums/v208/jwells393/TeleHeadstock.jpg
module HeadstockBlank()
{
    // Ad hoc headstock shape.  Taken from internet plans.
    difference()
    {
        union()
        {
            difference()
            {
                translate([0, 35, 0])
                    rotate([0, 0, -90]) cube([70.1, 175, HeadThickness]);
                translate([18, 29, -1]) 
                    cube([7, 7, HeadThickness + 2]);
                translate([3.68, 36.95, -1]) 
                    cylinder(r = 16, h = HeadThickness + 2);
                translate([129.49, -46.6, -1]) 
                    cylinder(r = 16, h = HeadThickness + 2);
                translate([3.68, -173.36, -1])
                    cylinder(r=152.4, h = HeadThickness + 2);
                translate([0, 41.91/2, -1])
                    cube([3.68, 3.68, HeadThickness + 2]);
                translate([0, -41.91/2 - 3.68, -1])
                    cube([3.68, 3.68, HeadThickness + 2]);
            }

            translate([28.69, 22.87, 0]) cylinder(r = 12.7, h = HeadThickness);
            translate([168.66, -19.92, 0]) cylinder(r = 12.7, h = HeadThickness);
            difference()
            {
                translate([159.6, -23.25, 0]) cylinder(r = 22.35, h = HeadThickness);
                translate([130, -16, -1]) cube([70, 50, HeadThickness + 2]);
            }

            difference()
            {
                translate([93, 27.9, 0]) cylinder(r = 67.9, h = HeadThickness);
                translate([0, -5, -1]) cube([200, 110, HeadThickness + 2]);
            }
        }
        translate([29, 36, -1]) rotate(Z * TunerAngle) cube([200, 200, HeadThickness + 2]);
    }
}

// Translate in the tuner hole coordinate system (TunerAngle degrees about Z).
module TranslateInTunerCoords(xlation = [0, 0, 0])
{
    for (theChild = [0:$children - 1])
    {
        rotate(Z * TunerAngle) 
        translate(xlation)
        rotate(Z * -TunerAngle)
        children(theChild);
    }
}

// Text to emboss into the headstock (just silly stuff).
module HeadstockText()
{
    // Add some text at the top of the headstock.
    jmcLabel = "JMC";
    jmcBlockSize = 7;
    jmcBlockHeight = HeadThickness * 0.5;
    jmcFont = "Magneto:style=Bold";
//    jmcFont = "Castellar:style=Regular";
    TranslateInTunerCoords([167, 37, HeadThickness * 0.9]) rotate([0, 0, 170 + 90 - 5])
    linear_extrude(height = jmcBlockHeight)
    {
       text(text = jmcLabel, font = jmcFont, size = jmcBlockSize);
    }
    
    // Add some text along the side.
    sideLabel = "Chord-O-Matic";
//    sideLabel = "CHORD-O-MATIC";
    sideBlockSize = 5;
    sideBlockHeight = HeadThickness * 0.5;
    sideBlockFont = "Magneto:style=Bold";
//    sideBlockFont = "Brush Script Std:style=Medium";
    TranslateInTunerCoords([30, 0, HeadThickness * 0.9]) rotate([0, 0, TunerAngle])
    linear_extrude(height = sideBlockHeight)
    {
       text(text = sideLabel, font = sideBlockFont, size = sideBlockSize);
    }
}

// The headstock with all holes added.
module HeadstockWithHoles()
{
    difference()
    {
        // The blank headstock.
        color(HeadstockColor) HeadstockBlank();
        // The tuning post holes.
        for(i = TunerCoords)
        {
            translate(i - HoleTolerance * Z) 
                cylinder(r = TunerHoleRadius, h = HeadThickness + 2 * HoleTolerance);
        }
        // The headstock text.
        color(HeadTextColor) HeadstockText();                   
    }
}

// The full headstock with holes and neck connector.
module HeadstockFull()
{
    ConnectorWidth = FBWidth - 2 * MatingSupportWidth - 0.5;
    ConnectorHeight = 5;
    
    difference()
    {
        // Start with the rotated headstock with holes and slide it over so
        // that the end of the rotated headstock aligns with the x origin.
        translate([-HeadThickness * sin(HeadstockYAngle), 0, 0])
            rotate(HeadstockAngle) HeadstockWithHoles();
        // Cut off the overhang that was created by translating the headstock above.
        translate([-FBWidth - 1, -(FBWidth + 1) / 2, -1]) cube(FBWidth + 1);
    }
    difference()
    {
        // Add a connector to mate with the guitar neck.
        color(HeadstockColor) //%%%%%%%%%%%%%%%
            translate([-1.5 * NutWidth, -ConnectorWidth / 2, -ConnectorHeight - 1]) 
            cube([5.5 * NutWidth + PrinterTolerance, ConnectorWidth, ConnectorHeight]);
        // Bore out holes for the nut studs.
        translate([-NutWidth, -FBWidth / 2, -1]) Nut(HoleTolerance);
    }
}

// Headstock assembly with holes, text, and all tuner parts.
module HeadstockAssembly()
{
    // Start with the complete headstock.
    HeadstockFull();
    // Rotate the entire tuner mechanism.
    rotate(HeadstockAngle)
    {
        // Add the tuner parts at the correct positions.
        TunerPositions()
        {
            // Add the tuning knobs.
            color(TuningKnobColor) 
                TranslateInTunerCoords([6.5, 
                           24, 
                           -TunerBaseThickness - TunerBoxHeight / 2 + 2 - 5.5] - 
                            Explode * ExplodeZOffset + 2 * Explode *ExplodeYOffset)
                rotate(Z * (-90 + TunerAngle)) 
                TuningKnob();
            // Add the tuning posts.
            color(TuningPostColor) translate([0,  0,  -4] + ExplodeZOffset * Explode)
                TuningPost();
            // Add the tuner shafts.
            color(TuningPostColor) TranslateInTunerCoords([6.5, 
                                              TunerBaseWidth + 12,
                                              -TunerBaseThickness - TunerBoxHeight / 2 + 2 - 1.8]
                                              - Explode*ExplodeZOffset+Explode*ExplodeYOffset)
                rotate([90, 0, TunerAngle]) 
                TunerShaft(0);            
        }
        // Add the tuner assembly.
        color(TunerAssemblyColor) translate([36,  30.5, 0] - Explode * ExplodeZOffset) TunerAssembly();
    }
}

// FOR TEST ONLY - print the lower part of the headstock.
module TestHeadNeckConnection()
{
    translate([0, 0, HeadThickness])
    rotate(HeadstockAngle + [180, 0, 0])
    intersection()
    {
        HeadstockFull();
        translate([20, 0, 0])
            cube([60, 75, 50], center = true);
    }
}

// Output the headstock in a printer friendly orientation.
module PrintHeadstock()
{
    translate([0, 100, HeadThickness])
        rotate(HeadstockAngle + [180, 0, -90])
        HeadstockFull($fn = 200);
}

// Output the tuning post in a printer friendly orientation.
module PrintTuningPost()
{
    translate([0, 0, 
               HeadThickness + TunerBushingHeight + TunerPostHeight +
               TunerBoxHeight / 2 + TunerBaseThickness - 2])
       rotate([180, 0, 0])
        TuningPost();
}

// Output the tuning knob in a printer friendly orientation.
module PrintTuningKnob()
{
    translate([0, 0, TunerHoleSpacing * 0.75 * 0.75 / 2]) rotate([0, 90, 0])
        TuningKnob();
}

// Output the printer assembly in a printer friendly orientation.
module PrintTunerAssembly()
{
    translate([-TunerBaseLength / 2, 0, 0])
        rotate([180, 0, TunerAngle]) TunerAssembly();
}


// TestHeadNeckConnection();
// HeadstockAssembly();



// PrintHeadstock();
// PrintTuningPost();
// PrintTunerAssembly();
// PrintTuningKnob();
TunerShaft(0.1);
