////////////////////////////////////////////////////////////////////////////////
//
// FingerBoard.scad
//
// Contains OpenScad code for the fingerboard/neck for the guitar chord chart
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

include <ChordChartParameters.scad>     // For part parameters.

// Local constants
HoleTolerance           = 0.2;                  // Amount to enlarge holes.
PrinterTolerance        = 0.2;                  // Amount to enlarge hole diameters
                                                //    due to printing tolerance.
MatingOffset            = [4, 0, 0];            // Offset from fret to mating blocks.

// A single fret.
module fret(tolerance = 0)
{
    base = 1.5 + tolerance;
    riserWidth = 0.75 + tolerance;
    riserHeight = 2 + tolerance;
    // The rounded top part of the fret
	cube([base, FBWidth + 2 * tolerance, base], center = true);
    // The squared off lower part of the fret.
    translate([0, 0, riserHeight / 2]) 
        cube([riserWidth, FBWidth + 2 * tolerance, riserHeight], center = true);
    translate([0, -(FBWidth + 2 * tolerance) / 2, riserHeight]) rotate (X * -90)
        cylinder(d = riserWidth, h = FBWidth + 2 * tolerance);


    // // The rounded top part of the fret
	// rotate(X * -90) cylinder(r = FretHeight, h = FBWidth + 2 * tolerance);
    // // The squared off lower part of the fret.
    // translate([-FretHeight, 0, -FretHeight]) 
        // cube([FretHeight * 2, FBWidth + 2 * tolerance, FretHeight]);
}

// All frets.
module frets(tolerance = 0)
{
	for (i = [1:len(FretOffsets) - 1])
	{
		translate([FretOffsets[i], (FBWidth + tolerance) / 2, FBThickness - 1.5]) fret(tolerance);
	}
}

// A single LED hole.
module LedHoles()
{
    if (UseLedHoles)
    {
        for (i = [1:len(FretOffsets) - 1], j = [1:NumStrings])
        {
            // Add a hole for the dome of the LED.
            translate([FretOffsets[i - 1] + (FretOffsets[i] - FretOffsets[i - 1]) / 2,
                      j * (FBWidth / (NumStrings + 1)),
                      -1])
                cylinder(d = LedDiameter + HoleTolerance, h = FBThickness + 2);
            // Add space for the ring at the base of the LED.
            translate([FretOffsets[i - 1] + (FretOffsets[i] - FretOffsets[i - 1]) / 2,
                       j * (FBWidth / (NumStrings + 1)),
                      0]) 
                cylinder(d = LedDiameter + 2 + HoleTolerance, 
                         h = FBThickness - LedHeight + 1);
        }
    }
}

// A mating post.
module MatingPost(tolerance = 0)
{
    side = MatingPinDiameter - tolerance;
    translate([-side / 2, -side / 2, 0])
    cube([side, side , FBMatingHoleDepth + NeckBackMatingHoleDepth - 6]);
}

// All the mating posts.
module MatingPosts()
{
    for(i = FretOffsets)
    {
        // Add a mating post to both sides of the neck at each of the frets.
        translate([i + MatingSupportWidth / 2, 
                   NeckBackThickness + MatingSupportWidth / 2, 
                   -NeckBackMatingHoleDepth - PrinterTolerance] + MatingOffset)
            MatingPost();
        translate([i + MatingSupportWidth / 2, 
                   FBWidth - NeckBackThickness - MatingSupportWidth / 2,
                   -NeckBackMatingHoleDepth - PrinterTolerance] + MatingOffset)
            MatingPost();
    }
}

// The rounded back of the neck.
module NeckBack()
{
	Depth           = FBWidth * NeckZScale / 2;
    TestCubeLength  = 100;
    SphereXScale    = 1.5;
	ExtensionLength = 68;
    
	difference()
	{
        // Create the outer part of the cylinder shell.
		translate([0, FBWidth/2, 0]) rotate(Y * 90) 
            scale([NeckZScale, 1, 1]) cylinder(d = FBWidth, h = NeckLength);
        // Remove the inner part of the cylinder shell.
		translate([-1, FBWidth / 2, 0]) rotate(Y * 90) scale([NeckZScale, 1, 1])
            cylinder(d = FBWidth - (NeckBackThickness * 2), h = NeckLength + 2);
        // Remove the top half of the cylinder.
		translate([-1, -1, 0]) 
            cube([NeckLength + 2, FBWidth + 2, FBWidth * NeckZScale / 2 + 1]);
        // Clip off the small piece that was left due to the headstock merge.
        if (UseStraightNeck == 0)
        {
            // Limit the sphere shell to where it meets the headstock.
            translate([-TestCubeLength * cos(HeadstockYAngle) + 1,
                       -0.1,
                       -TestCubeLength * sin(HeadstockYAngle)])
                rotate([0, -HeadstockYAngle + 0.1, 0])
                cube([TestCubeLength + 10, FBWidth + 0.2, HeadThickness * 2]);
        }
        // Hole for the retainer pin.
        translate([NeckLength - NeckSupportPinHeight, FBWidth / 2, -20]) 
            cylinder(r = NeckSupportPinRadius + 0.1, h = 10);
	}
	difference()
	{
        // Add the mating supports.
		for (i = [0:len(FretOffsets) - 1])
		{
			translate([FretOffsets[i], NeckBackThickness, -Depth] + MatingOffset) 
                cube([MatingSupportWidth, MatingSupportWidth, Depth]);
			translate([FretOffsets[i], FBWidth - NeckBackThickness - MatingSupportWidth,
                      -Depth] + MatingOffset)
                cube([MatingSupportWidth, MatingSupportWidth, Depth]);
		}
        // Lop off the mating supports outside the neck back shell.
		difference()
		{
			translate([-1, FBWidth / 2, 0]) rotate(Y * 90) 
                scale([NeckZScale, 1, 1]) 
                cylinder(d = FBWidth * 2, h = NeckLength + 2);
			translate([0, FBWidth / 2, 0]) rotate(Y * 90) 
                scale([NeckZScale, 1, 1])
                cylinder(d = FBWidth, h = NeckLength);
			translate([-1, -FBWidth / 2, 0]) 
                cube([NeckLength + 2, 
                      FBWidth * 2 + 2,
                      FBWidth * NeckZScale / 2 + 1]);
        }
        // Bore out the holes for the mating connectors.
        for (i = [0:len(FretOffsets) - 1])
        {
            translate([FretOffsets[i] + MatingSupportWidth / 2, 
                       NeckBackThickness + MatingSupportWidth / 2,
                      -NeckBackMatingHoleDepth / 2 - 2] + MatingOffset)
                      MatingPost();
            translate([FretOffsets[i] + MatingSupportWidth / 2,
                       FBWidth - NeckBackThickness-MatingSupportWidth / 2,
                       -NeckBackMatingHoleDepth / 2 - 2] + MatingOffset)
                      MatingPost();
        }
	}
    if(UseStraightNeck)
    {
        // Straight neck merge to the headstock is selected.
        difference()
        {
            // Extend the neck back shell toward the headstock.
            translate([-ExtensionLength, FBWidth / 2, 0]) rotate([0, 90, 0])
                scale([NeckZScale, 1, 1])
                cylinder(d = FBWidth, h = ExtensionLength + 1);
            translate([-ExtensionLength- 0.1, FBWidth / 2, 0])
                rotate([0, 90, 0]) scale([NeckZScale, 1, 1])
                cylinder(d = FBWidth - (NeckBackThickness * 2), h = ExtensionLength + 1.2);
            // Remove the top of the extension cylinders.
            translate([-ExtensionLength - 0.1, -1, 0])
                cube([100 + 2, FBWidth + 2, FBWidth * NeckZScale / 2 + 1]);  
            // Limit the shell to where it meets the headstock.
            translate([-TestCubeLength * cos(HeadstockYAngle),
                       -.01,
                       -TestCubeLength * sin(HeadstockYAngle)])
                rotate([0, -HeadstockYAngle, 0]) 
                cube([TestCubeLength, FBWidth + 0.2, HeadThickness * 2]);
        }
    }
    else
    {
        // Spherical neck merge to the headstock is selected.
        difference()
        {
            // Create a scaled sphere to extend the neck back toward the headstock.
            translate([1, FBWidth / 2, 0]) 
                scale([SphereXScale, 1, NeckZScale]) 
                sphere(d = FBWidth);
            // Truncate the sphere at the point of intersection with the neck back.
            translate([1, FBWidth / 2, 0]) rotate(Y * 90) 
                scale([NeckZScale, 1, 1]) 
                cylinder(d = FBWidth - (NeckBackThickness * 2), h = FBWidth + 1);
            // Limit the sphere to be a shell with the same thickness as the neck back.
            translate([1, FBWidth / 2, 0])
                scale([SphereXScale, 1, NeckZScale])
                sphere(d = FBWidth - (NeckBackThickness * 2));
            // Cut off the top of the sphere shell.
            translate([-ExtensionLength - 0.1, -1, 0])
                cube([100 + 2, FBWidth + 2, FBWidth * NeckZScale / 2 + 1]); 
            // Limit the sphere shell to where it meets the headstock.
            translate([-TestCubeLength * cos(HeadstockYAngle) + 1,
                       -0.1,
                       -TestCubeLength * sin(HeadstockYAngle)])
                rotate([0, -HeadstockYAngle + 0.1, 0])
                cube([TestCubeLength + 10, FBWidth + 0.2, HeadThickness * 2]);
        }
    }
}

// Holes in the bottom of the fingerboard used for mating the fingerboard to the back of the neck.
module MatingHoles(depth = 1)
{
	for (i = [0:len(FretOffsets) - 1])
	{
		translate([FretOffsets[i] + MatingSupportWidth / 2, 
                   NeckBackThickness + MatingSupportWidth / 2, 
                   -0.2] + MatingOffset)
                   MatingPost();
		translate([FretOffsets[i] + MatingSupportWidth / 2,
                   FBWidth - NeckBackThickness - MatingSupportWidth / 2,
                   -0.2] + MatingOffset)
                   MatingPost();
	}	
}

// A single decorative inlay in the top of the fingerboard.
module TopInlay(FretIndex, tolerance = 0)
{
    if (UseTopInlays)
    {
        margin = 3;
        base   = FretOffsets[FretIndex - 1];
        height = FretOffsets[FretIndex] - base - 2 * margin;
        width  = FBWidth - 2 * margin;

        difference()
        {
            // Start the inlay with a square centered within the fret box.
            translate([base + margin + 0.1, margin, FBThickness - 1.9])
                cube([height + tolerance, width + tolerance, 2]);
            if (UseTriangularTopInlays)
            {
                // Turn the inlay into a triangle by removing half of the square diagonally.
                translate([base + margin + 0.1, margin, FBThickness - 2])
                    rotate([0, 0, atan2(width, height)-10])
                    cube([height * 5, width * 5, 2.2]);
            }
            // Cut the LID holes into the inlays.
            if (UseLedHoles)
            {
                for (i = [1:NumStrings])
                {
                    translate([base + height / 2 + margin,
                               i * (FBWidth / (NumStrings + 1)),
                               FBThickness - 2])
                        cylinder(d = LedDiameter + HoleTolerance, h = 3);
                }
            }
        }
    }
}

// A single decorative inlay in the side of the fingerboard.
module SideInlay(FretIndex, tolerance = 0)
{
    if (UseSideInlays)
    {
        margin      = 2;
        inlayHeight = FBThickness - 2 * margin;
        inlayWidth  = inlayHeight;
        baseX       = (FretOffsets[FretIndex] + FretOffsets[FretIndex - 1] - inlayWidth) / 2;
        baseZ       = margin;

        if (UseSquareSideInlays)
        {
            // Side inlays should be square.
            translate([baseX, -0.1, baseZ]) 
                cube([inlayWidth + tolerance, 2, inlayHeight + tolerance]);
        }
        else
        {
            // Side inlays should be circular.
            translate([baseX + (inlayWidth + tolerance) / 2, 1.9, baseZ + (inlayWidth + tolerance) / 2]) 
                rotate([90, 0, 0])
                cylinder(d = inlayWidth + tolerance, h = 2);
        }
    }
}

// The fingerboard itself containing frets, LED holes, and inlays.
module FingerBoard()
{
    difference()
    {
        // Start with the basic fingerboard shape.
        cube([NeckLength, FBWidth, FBThickness]);
        // Add the fret slots.
        frets(.3);
        // Add the LED holes.
        LedHoles();
        // Add the mounting holes.
        translate([0, 0, -3])
            MatingHoles(FBMatingHoleDepth + HoleTolerance);
        // Add the inlay insets.
        for(i = InlayFrets)
        {
            TopInlay(i, HoleTolerance);
            SideInlay(i, HoleTolerance);
        }
        // Cut out space for the nut.
        translate([-PrinterTolerance, -1, 2]) 
            cube([NutWidth + PrinterTolerance, FBWidth + 2, NutHeight]);
        // Add the mounting holes for the nut.
        translate([-PrinterTolerance, 0, 2]) Nut(HoleTolerance);
    }
}

// Fingerboard inlays on specific frets only.
module TopInlays()
{
    for(i=InlayFrets)
    {
        TopInlay(i, 0);
    }
}

// Fingerboard side inlays on specific frets only.
module SideInlays()
{
    for(i = InlayFrets)
    {
        SideInlay(i, 0);
    }
}


// color(FBColor) FingerBoard();

// color(NeckBackColor) translate(-1 * ExplodeZOffset * Explode) NeckBack();
// color(InlayColor)    translate(ExplodeZOffset*Explode) TopInlays();
// color(InlayColor)    translate(-1 * ExplodeYOffset * Explode) SideInlays();
// color(FretColor)     translate(ExplodeZOffset * Explode) frets(0);
// color(NutColor)      translate([0, 0, 2] + ExplodeZOffset * Explode) Nut(0);
// color(NeckBackColor) translate(-0.4 * ExplodeZOffset * Explode) MatingPosts();

//==============================================================================
// The following modules are used for making test prints of various portions
// of the finger board.
//==============================================================================

// Test the lower part of the fingerboard that goes into the base's neck mount.
module TestFbBase()
{
    length = FretOffsets[FretOffsetSize - 1] -
             FretOffsets[FretOffsetSize - 2] + NeckExtension + 4;
    translate([FBWidth / 2, -FretOffsets[FretOffsetSize - 1], 0]) rotate([0, 0, 90])
        intersection()
        {
            FingerBoard();
            translate([FretOffsets[FretOffsetSize - 2] - 2, 0, 0])
                cube([length, FBWidth + 2, FBThickness + 2]);
        }
}

// Test the lower part of the neck that goes into the base's neck mount.
module TestNeckBackBase()
{
    length = FretOffsets[FretOffsetSize - 1] -
             FretOffsets[FretOffsetSize - 2] + NeckExtension + 4;
    translate([-FBWidth / 2, -FretOffsets[FretOffsetSize - 1], 0]) rotate([180, 0, 90])
        intersection()
        {
            NeckBack();
            translate([FretOffsets[FretOffsetSize - 2] - 2, 0, -25])
                cube([length, FBWidth + 2, 30]);
        }
}

// Test the upper part of the fingerboard that connects to the head.
module TestFbTop()
{
    length = FretOffsets[1] + 15;
    translate([FBWidth / 2, -FretOffsets[1] / 2, 0]) rotate([0, 0, 90])
        intersection()
        {
            FingerBoard();
            translate([-1, 0, 0])
                cube([length, FBWidth + 2, FBThickness + 2]);
        }
}

// Test the upper part of the neck that connects to the head.
module TestNeckBackTop()
{
    length = FretOffsets[1] + 55;
    translate([-FBWidth / 2, -FretOffsets[1] / 2, 0]) rotate([180, 0, 90])
        intersection()
        {
            NeckBack();
            translate([-FretOffsets[1], 0, -25])
                cube([length, FBWidth + 2, 30]);
        }
}

// Print a specified top inlay oriented for printing.
module PrintTopInlay(fretIndex = 0, tol = 0)
{
    translate([-FretOffsets[InlayFrets[fretIndex]], FBWidth / 2, FBThickness + 0.1])
        rotate([180, 0, 0]) TopInlay(InlayFrets[fretIndex], tol);
}

// Print a side inlay oriented for printing.
module PrintSideInlay(fretIndex = 0, tol = 0)
{
    translate([-FretOffsets[InlayFrets[fretIndex]], 0, 2])
        rotate([-90, 0, 0]) SideInlay(InlayFrets[fretIndex], tol);
}

// Print the entire neck back oriented for printing.
module PrintNeckBack()
{
    rotate([0, 0, 90]) 
        translate([-NeckLength / 2, FBWidth / 2, 0]) rotate([180, 0, 0]) 
        NeckBack();
}

// Print the lower portion of the neck back that contains the retainer pin hole
// as a test.
module TestNeckBackHole()
{
    translate([-(NeckLength - NeckSupportHeight) / 2, 0, 0])
        intersection()
        {
            PrintNeckBack();
            translate([(NeckLength - NeckSupportHeight) / 2 + 1, 0, 0]) 
                cube([NeckSupportHeight + 4, FBWidth + 1, FBWidth + 1], center = true);
        }
}

module PrintNut()
{
    translate([0, 0, NutHeight]) rotate([180, 0, 0]) Nut(0);
}

module PrintMatingPost()
{
    translate([0, 0, (MatingPinDiameter - 0.2) / 2]) rotate([90, 0, 0]) MatingPost(0.4);
}


// TestNeckBackHole();
// TestFbBase();
// TestNeckBackBase();
// rotate([-90, 0, 0]) SideInlay(0, 0);
// TestFbTop();
// TestNeckBackTop(); // Export as .off file
// translate([0, 0, NutHeight]) rotate([180, 0, 0]) Nut(0);


// FingerBoard();
// PrintNeckBack(); // Export as .off file
// PrintNut();
// fret();
PrintMatingPost();
// PrintTopInlay(0, 0);
// PrintTopInlay(1, 0);
// PrintSideInlay(3, 0);
// PrintSideInlay(5, 0);
