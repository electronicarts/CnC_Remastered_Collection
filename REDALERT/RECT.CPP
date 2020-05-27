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

/* $Header: /CounterStrike/RECT.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RECT.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/22/96                                                     *
 *                                                                                             *
 *                  Last Update : July 22, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Rect::Rect -- Constructs a rectangle object.                                              *
 *   Rect::Is_Valid -- Determines if the rectangle is valid.                                   *
 *   Rect::Intersect -- Find the intersection between two rectangles.                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"rect.h"


/***********************************************************************************************
 * Rect::Rect -- Constructs a rectangle object.                                                *
 *                                                                                             *
 *    This will construct a rectangle object according to the parameters specified.            *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y values of the upper left corner of the rectangle.             *
 *                                                                                             *
 *          w,h   -- The width and height values of the rectangle.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/22/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
Rect::Rect(int x, int y, int w, int h) :
	X(x),
	Y(y),
	Width(w),
	Height(h)
{
}


/***********************************************************************************************
 * Rect::Is_Valid -- Determines if the rectangle is valid.                                     *
 *                                                                                             *
 *    An invalid rectangle has values that do not make any sense. This is a useful state since *
 *    this can be used to determine if a rectangle has been initialized correctly or for       *
 *    detecting an error return condition for rectangle manipulation routines.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Does this rectangle appear valid?                                            *
 *                                                                                             *
 * WARNINGS:   An invalid rectangle is one that has a width or height of less than one.        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/22/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Rect::Is_Valid(void) const
{
	return(Width > 0 && Height > 0);
}


/***********************************************************************************************
 * Rect::Intersect -- Find the intersection between two rectangles.                            *
 *                                                                                             *
 *    This routine will take the specified rectangle and use it like a "cookie cutter" on this *
 *    rectangle. The intersection of these two rectangles is returned. An optional X and       *
 *    Y parameter is supplied so that an absolute coordinate can be maintained to the new      *
 *    rectangle.                                                                               *
 *                                                                                             *
 * INPUT:   rectangle   -- Reference to the rectangle to use as a cookie cutter.               *
 *                                                                                             *
 *          x,y         -- Optional pointer to a coordinate that will be adjusted to stay      *
 *                         in an absolute position in coordinates even though it is a          *
 *                         relative offset.                                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the rectangle that is the intersection of the one specified and       *
 *          this rectangle.                                                                    *
 *                                                                                             *
 * WARNINGS:   The rectangle returned may be invalid. This can occur if there is no legal      *
 *             intersection between the rectangles.                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/22/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
Rect const Rect::Intersect(Rect const & rectangle, int * x, int * y) const
{
	Rect rect(0, 0, 0, 0);			// Dummy (illegal) rectangle.
	Rect r = rectangle;				// Working rectangle.

	/*
	**	Both rectangles must be valid or else no intersection can occur. In such
	**	a case, return an illegal rectangle.
	*/
	if (!Is_Valid() || !rectangle.Is_Valid()) return(rect);

	/*
	**	The rectangle spills past the left edge.
	*/
	if (r.X < X) {
		r.Width -= X - r.X;
		r.X = X;
	}
	if (r.Width < 1) return(rect);

	/*
	**	The rectangle spills past top edge.
	*/
	if (r.Y < Y) {
		r.Height -= Y - r.Y;
		r.Y = Y;
	}
	if (r.Height < 1) return(rect);

	/*
	**	The rectangle spills past the right edge.
	*/
	if (r.X + r.Width > X + Width) {
		r.Width -= (r.X + r.Width) - (X + Width);
	}
	if (r.Width < 1) return(rect);

	/*
	**	The rectangle spills past the bottom edge.
	*/
	if (r.Y + r.Height > Y + Height) {
		r.Height -= (r.Y + r.Height) - (Y + Height);
	}
	if (r.Height < 1) return(rect);

	/*
	**	Adjust Height relative draw position according to Height new rectangle
	**	union.
	*/
	if (x != NULL) {
		*x -= (r.X-X);
	}
	if (y != NULL) {
		*y -= (r.Y-Y);
	}

	return(r);
}


Rect const Union(Rect const & rect1, Rect const & rect2)
{
	if (rect1.Is_Valid()) {
		if (rect2.Is_Valid()) {
			Rect result = rect1;

			if (result.X > rect2.X) {
				result.Width += result.X-rect2.X;
				result.X = rect2.X;
			}
			if (result.Y > rect2.Y) {
				result.Height += result.Y-rect2.Y;
				result.Y = rect2.Y;
			}
			if (result.X+result.Width < rect2.X+rect2.Width) {
				result.Width = ((rect2.X+rect2.Width)-result.X)+1;
			}
			if (result.Y+result.Height < rect2.Y+rect2.Height) {
				result.Height = ((rect2.Y+rect2.Height)-result.Y)+1;
			}
			return(result);
		}
		return(rect1);
	}
	return(rect2);
}
