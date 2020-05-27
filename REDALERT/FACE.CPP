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

/* $Header: /CounterStrike/FACE.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : FACE.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 03/08/96                                                     *
 *                                                                                             *
 *                  Last Update : March 8, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Desired_Facing8 -- Determines facing from one coordinate to another.                      *
 *   Desired_Facing256 -- Determines facing from one coordinate to another.                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


/***********************************************************************************************
 * Desired_Facing8 -- Determines facing from one coordinate to another.                        *
 *                                                                                             *
 *    This routine will find the facing (compass direction) from one location to another.      *
 *    Typical use of this is in find path and other 'monster' movement logic.                  *
 *                                                                                             *
 * INPUT:   x1,y1    -- X and Y coordinates for the source location. The coordinate 0,0 is     *
 *                      presumed to be the northwest corner of the map.                        *
 *                                                                                             *
 *          x2,y2    -- X and Y coordinates for the destination (target) location.             *
 *                                                                                             *
 * OUTPUT:  Returns with the facing from the first coordinate to the second coordinate. The    *
 *          value returned will range from 0 being North, increasing clockwise until reaching  *
 *          255 which is just shy of North in a Westerly direction.                            *
 *                                                                                             *
 * WARNINGS:   This routine is only accurate to the 8 primary compass directions. It is much   *
 *             faster than the Desired_Facing256() function so it should be used where speed   *
 *             is more important than accuracy.                                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType Desired_Facing8(int x1, int y1, int x2, int y2)
{
	int index = 0;				// Facing composite value.

	/*
	**	Figure the absolute X difference. This determines
	**	if the facing is leftward or not.
	*/
	int xdiff = x2-x1;
	if (xdiff < 0) {
		index |= 0x00C0;
		xdiff = -xdiff;
	}

	/*
	**	Figure the absolute Y difference. This determines
	**	if the facing is downward or not. This also clarifies
	**	exactly which quadrant the facing lies.
	*/
	int ydiff = y1-y2;
	if (ydiff < 0) {
		index ^= 0x0040;
		ydiff = -ydiff;
	}

	/*
	**	Determine which of the two direction offsets it bigger. The
	**	offset direction that is bigger (X or Y) will indicate which
	**	orthogonal direction the facing is closer to.
	*/
	unsigned bigger;
	unsigned smaller;
	if (xdiff < ydiff) {
		smaller = xdiff;
		bigger = ydiff;
	} else {
		smaller = ydiff;
		bigger = xdiff;
	}

	/*
	**	If on the diagonal, then incorporate this into the facing
	**	and then bail. The facing is known.
	*/
	if (((bigger+1)/2) <= smaller) {
		index += 0x0020;
		return(DirType(index));
	}

	/*
	**	Determine if the facing is closer to the Y axis or
	**	the X axis.
	*/
	int adder = (index & 0x0040);
	if (xdiff == bigger) {
		adder ^= 0x0040;
	}
	index += adder;

	return(DirType(index));
}


/***********************************************************************************************
 * Desired_Facing256 -- Determines facing from one coordinate to another.                      *
 *                                                                                             *
 *    This routine will figure the facing from the source coordinate toward the destination    *
 *    coordinate. Typically, this routine is used for movement and other 'monster' logic. It   *
 *    is more accurate than the corresponding Desired_Facing8() function, but is slower.       *
 *                                                                                             *
 * INPUT:   srcx, srcy  -- The source coordinate to determine the facing from.                 *
 *                                                                                             *
 *          dstx, dsty  -- The destination (or target) coordinate to determine the facing      *
 *                         toward.                                                             *
 *                                                                                             *
 * OUTPUT:  Returns with the facing from the source coordinate toward the destination          *
 *          coordinate with 0 being North increasing in a clockwise direction. 64 is East,     *
 *          128 is South, etc.                                                                 *
 *                                                                                             *
 * WARNINGS:   The coordinate 0,0 is presumed to be in the Northwest corner of the map.        *
 *             Although this routine is fast, it is not as fast as Desired_Facing8().          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType Desired_Facing256(int srcx, int srcy, int dstx, int dsty)
{
	int composite=0;		// Facing built from intermediate calculations.

	/*
	**	Fetch the absolute X difference. This also gives a clue as
	**	to which hemisphere the direction lies.
	*/
	int xdiff = dstx - srcx;
	if (xdiff < 0) {
		composite |= 0x00C0;
		xdiff = -xdiff;
	}

	/*
	**	Fetch the absolute Y difference. This clarifies the exact
	**	quadrant that the direction lies.
	*/
	int ydiff = srcy - dsty;
	if (ydiff < 0) {
		composite ^= 0x0040;
		ydiff = -ydiff;
	}

	/*
	**	Bail early if the coordinates are the same. This check also
	**	has the added bonus of ensuring that checking for division
	**	by zero is not needed in the following section.
	*/
	if (xdiff == 0 && ydiff == 0) return(DirType(0xFF));

	/*
	**	Determine which of the two direction offsets it bigger. The
	**	offset direction that is bigger (X or Y) will indicate which
	**	orthogonal direction the facing is closer to.
	*/
	unsigned bigger;
	unsigned smaller;
	if (xdiff < ydiff) {
		smaller = xdiff;
		bigger = ydiff;
	} else {
		smaller = ydiff;
		bigger = xdiff;
	}

	/*
	**	Now that the quadrant is known, we need to determine how far
	**	from the orthogonal directions, the facing lies. This value
	**	is calculated as a ratio from 0 (matches orthogonal) to 31
	**	(matches diagonal).
	*/
	//lint -e414		Division by zero cannot occur here.
	int frac = (smaller * 32U) / bigger;

	/*
	**	Given the quadrant and knowing whether the facing is closer
	**	to the X or Y axis, we must make an adjustment toward the
	**	subsequent quadrant if necessary.
	*/
	int adder = (composite & 0x0040);
	if (xdiff > ydiff) {
		adder ^= 0x0040;
	}
	if (adder) {
		frac = (adder - frac)-1;
	}

	/*
	**	Integrate the fraction value into the quadrant.
	*/
	composite += frac;

	/*
	**	Return with the final facing value.
	*/
	return(DirType(composite & 0x00FF));
}
