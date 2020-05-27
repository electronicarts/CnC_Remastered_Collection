//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection

/* $Header: /CounterStrike/RGB.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RGB.CPP                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/02/95                                                     *
 *                                                                                             *
 *                  Last Update : February 20, 1996 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   RGBClass::Adjust -- Adjust one RGB value toward another.                                  *
 *   RGBClass::Difference -- Determines the "distance" between two colors.                     *
 *   RGBClass::Set -- Set the color index to the RGB object specified.                         *
 *   RGBClass::operator HSVClass -- Conversion operator for RGB to HSV object.                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"watcom.h"
#include "rgb.h"
#include	"hsv.h"
#include	"palettec.h"

RGBClass const BlackColor(0, 0, 0);


/***********************************************************************************************
 * RGBClass::Adjust -- Adjust one RGB value toward another.                                    *
 *                                                                                             *
 *    This routine is used to modify an RGB value to proportionately match another RGB value   *
 *    according to the ratio parameter specified. Typical use of this routine is in palette    *
 *    fading from one palette to another or to black.                                          *
 *                                                                                             *
 * INPUT:   ratio    -- The ration of transformation. This value is in the form of 0 to 255,   *
 *                      with 0 being no change, and 255 being 100% transformed into the        *
 *                      destination color.                                                     *
 *                                                                                             *
 *          rgb      -- Reference to the destination RGB color to transform this color into.   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void RGBClass::Adjust(int ratio, RGBClass const & rgb)
{
	/*
	**	Ratio conversion is limited to 0 through 100%. This is
	**	the range of 0 to 255.
	*/
	ratio &= 0x00FF;

	/*
	**	Adjust the color guns by the ratio specified toward the
	**	destination color.
	*/
	int value = (int)rgb.Red - (int)Red;
	Red = (int)Red + (value * ratio) / 256;

	value = (int)rgb.Green - (int)Green;
	Green = (int)Green + (value * ratio) / 256;

	value = (int)rgb.Blue - (int)Blue;
	Blue = (int)Blue + (value * ratio) / 256;
}


/***********************************************************************************************
 * RGBClass::Difference -- Determines the "distance" between two colors.                       *
 *                                                                                             *
 *    This routine is used to calculate a relative distance between two colors. The value is   *
 *    relative only to itself and thus is useful only for determining the magnitude of         *
 *    color difference rather than the nature of the color difference. Palette remapping       *
 *    code uses this routine to find closest matches for colors.                               *
 *                                                                                             *
 * INPUT:   rgb   -- Reference to the color to be compared to this color.                      *
 *                                                                                             *
 * OUTPUT:  Returns the difference between the two colors. The value returned is zero if the   *
 *          colors exactly match. The greater the positive value the greater the difference    *
 *          between the colors.                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int RGBClass::Difference(RGBClass const & rgb) const
{
	int r = (int)Red - (int)rgb.Red;
	if (r < 0) r = -r;

	int g = (int)Green - (int)rgb.Green;
	if (g < 0) g = -g;

	int b = (int)Blue - (int)rgb.Blue;
	if (b < 0) b = -b;

	return(r*r + g*g + b*b);
}


/***********************************************************************************************
 * RGBClass::operator HSVClass -- Conversion operator for RGB to HSV object.                   *
 *                                                                                             *
 *    This conversion operator will convert an RGBClass object into an HSVClass object.        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a reference (implicit) to the HSVClass object that most closely       *
 *          represents the RGBClass object.                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
RGBClass::operator HSVClass (void) const
{
	int hue;
	int saturation;
	int value;

	/*
	**	Fetch working component values for the color guns.
	*/
	int red = Red_Component();
	int green = Green_Component();
	int blue = Blue_Component();

	/*
	**	The hue defaults to none. Only if there is a saturation value will the
	**	hue be calculated.
	*/
	hue = 0;

	/*
	**	Set the value (brightness) to match the brightest color gun.
	*/
	value = (red > green) ? red : green;
	if (blue > value) value = blue;

	/*
	**	Determine the amount of true white present in the color. This is the
	**	minimum color gun value. The white component is used to determine
	**	color saturation.
	*/
	int white = (red < green) ? red : green;
	if (blue < white) white = blue;

	/*
	**	Determine the saturation (intensity) of the color by comparing the
	**	ratio of true white as a component of the overall color. The more
	**	white component, the less saturation.
	*/
	saturation = 0;
	if (value) {
		saturation = ((value - white) * 255) / value;
	}

	/*
	** If there is any saturation at all, then the hue must be calculated. The
	**	hue is based on a six sided color wheel.
	*/
	if (saturation != 0) {
		unsigned int tmp = value - white;
	 	unsigned int r1 = ((value - red) * 255) / tmp;
	 	unsigned int g1 = ((value - green) * 255) / tmp;
	 	unsigned int b1 = ((value - blue) * 255) / tmp;

		// Find effect of second most predominant color.
		// In which section of the hexagon of colors does the color lie?
		if (value == red) {
		 	if (white == green) {
				tmp = 5 * 256 + b1;
			} else {
				tmp = 1 * 256 - g1;
			}
		} else {
			if (value == green) {
			 	if (white == blue) {
					tmp = 1 * 256 + r1;
				} else {
					tmp = 3 * 256 - b1;
				}
			} else {
			 	if (white == red) {
					tmp = 3 * 256 + g1;
				} else {
					tmp = 5 * 256 - r1;
				}
			}
		}

		// Divide by six and round.
		hue = tmp / 6;
	}

	return(HSVClass(hue, saturation, value));
}


/***********************************************************************************************
 * RGBClass::Set -- Set the color index to the RGB object specified.                           *
 *                                                                                             *
 *    This routine will set the specified color index with this RGBClass object. Use this      *
 *    routine to set one color.                                                                *
 *                                                                                             *
 * INPUT:   color -- The color index to set with this RGBClass object.                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void RGBClass::Set(int color) const
{
	Raw_Color_Prep(color);
	Raw_Set();

	((RGBClass &)PaletteClass::CurrentPalette[color]) = *this;
}

