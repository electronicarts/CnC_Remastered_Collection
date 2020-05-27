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

/* $Header: /CounterStrike/ROTBMP.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : ROTBMP.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/05/96                                                     *
 *                                                                                             *
 *                  Last Update : July 5, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if (0)//PG
//#include	"function.h"
#include	"watcom.h"
#include "rotbmp.h"
#define FILE_H
#define WWMEM_H
#include	<wwlib32.h>


int Rotate_Bitmap(GraphicViewPortClass * srcvp, GraphicViewPortClass * destvp, int angle);

struct WPPOINT {
	int x;
	int y;
};

/***************************************************************************
 * Rotate_bitmap -- rotate a bitmap from srcvp to destvp                   *
 *                                                                         *
 * INPUT: note that angles are 0 - 255 (best if used in increments of 4!)  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *	   destvp should be a square. width and height should be equal to : 	 	*
 * 	MAX(srcvp->width,srcvp->height) * 1.7 (square root of 2!!)    		 	*
 *																								 	*
 *																								 	*
 * HISTORY:                                                                *
 *   01/02/1996  BP : Created.                                             *
 *=========================================================================*/
Rotate_Bitmap(GraphicViewPortClass * srcvp, GraphicViewPortClass * destvp, int a)
{
	int shift = 7;
	int fixpoint1 = 1 << shift; // this is a fixed point 1
	int Deltax;
	int Deltay;
	int sx,sy,dx,dy;
	int Error = 0;
	int Decimal = 0;
	// this is used if I walk in Y
	int buffwidth = srcvp->Get_Width() + srcvp->Get_XAdd();
	int buffwid2 = destvp->Get_Width() + destvp->Get_XAdd();
	char * dest;
	char * src;
	int sa,ca;
	int t;
	int x,y;
	int rx,ry;
	int w,h;
	int dw = destvp->Get_Width();
	int dh = destvp->Get_Height();
	WPPOINT sp[4];
	WPPOINT dp[4];
	sa = Sin256[a];
	ca = Cos256[a];
	// get rectangle size
	x = 0;
	y = 0;
	w = srcvp->Get_Width();
	h = srcvp->Get_Height();

	int halfws = w >> 1;
	int halfhs = h >> 1;
	int halfwd = dw >> 1;
	int halfhd = dh >> 1;


	// make the src rectangle
	sp[0].x = x;
	sp[0].y = y;

	sp[1].x = x + w;
	sp[1].y = y;

	// now calculate the rotated rectangle
	dp[0].x = ( ((sp[0].x - halfws) * ca) - ((sp[0].y - halfhs) * sa) ) >> shift;
	dp[0].x += halfwd;

	dp[0].y = ( ((sp[0].x - halfws) * sa) + ((sp[0].y - halfhs) * ca) ) >> shift;
	dp[0].y += halfhd;

	dp[1].x = ( ((sp[1].x - halfws) * ca) - ((sp[1].y - halfhs) * sa) ) >> shift;
	dp[1].x += halfwd;

	dp[1].y = ( ((sp[1].x - halfws) * sa) + ((sp[1].y - halfhs) * ca) ) >> shift;
	dp[1].y += halfhd;

	rx = dp[0].x;
	ry = dp[0].y;
	// now calculate slope

	// diff from new to old x

	Deltax = (dp[1].x - dp[0].x);

	// diff from new to old y

	Deltay = (dp[1].y - dp[0].y);

	// handle the easy cases

	// no change in x
	int r;
	if (!Deltax) { // must be 90 or 270 degree transpose!
		if (Deltay < 0) {
			// walk across source in the x + dir
			// walk across dest in the y - dir

			x = 0;
			dy = 0;
			dx = 0;
			for (t = 0; t< h; t++) {
				x = 0;
				src = MAKE_PTR(srcvp, x, y);
				dest = MAKE_PTR(destvp, rx + dx, ry - dy);
				for (r = 0; r< w; r++) {
					// transparency
					if (* src)
						*dest = *src;
					src++;
					dest -= buffwid2;
				}
				y++;
				dx++;
			}
		} else {
			// walk across source in the x + dir
			// walk across dest in the y + dir

			x = 0;
			dy = 0;
			dx = 0;
			for (t = 0; t< h; t++) {
				x = 0;
				src = MAKE_PTR(srcvp, x, y);
				dest = MAKE_PTR(destvp, rx - dx, ry + dy);
				for (r = 0; r< w; r++) {
					// transparency
					if (*src)
						*dest = *src;
					src++;
					dest += buffwid2;
				}
				y++;
				dx++;
			}
		}
		return 0;
	}
	// no change in y

	if (!Deltay) { // must be 0 or 180 degree transpose !
		if (Deltax < 0) {
			y = 0;
			for (y = 0; y< h; y++) {
				x = 0;
				src = MAKE_PTR(srcvp, x, y);
				dest = MAKE_PTR(destvp, rx - x , ry - y);
				for (x = 0 ; x< w; x++) {
					// transparency
					if (*src)
						*dest = *src;
					dest--;
					src++;
				}
			}
		} else {
			for (y = 0; y< h; y++) {
				x = 0;
				src = MAKE_PTR(srcvp, x, y);
				dest = MAKE_PTR(destvp, rx + x, ry + y);
				for (x = 0 ; x< w; x++) {
					// transparency
					if (*src)
						*dest = *src;
					dest++;
					src++;
				}
			}
		}
		return 0;
	}


	// ok now the hard part

	// make them 16.16
	if ( ABS(Deltax) < ABS(Deltay)) { // ok this means we want to walk in y

		// walk in  + x in the src and
		// walk in  + y in the dest
		Error = 0;
		// start at left top corner in src and dest

		sx = 0;
		sy = 0;

		dx = 0;
		dy = 0;

		dest = MAKE_PTR(destvp, rx + dx, ry + dy);

		src = MAKE_PTR(srcvp, x + sx, y + sy);

		// this gets added to error each inc of x
		// when error is > 1 then inc y!
		int xinc = 1;
		if (Deltax < 0) {
			Deltax = -Deltax;
			xinc = -1;
		}
		int yinc = 1;
		int yinc1 = 1;
		if (Deltay < 0) {
			Deltay = - Deltay;
			buffwid2 = - buffwid2;
		}
		Decimal = ( Deltax << shift) / Deltay ;
		// walk in X

		int Deltax2 = Deltax << shift;
		int Deltay2 = Deltay << shift;

		// this is the ratio between the source height and the dest height
		// as the rectangle rotates the height and width change
		int DeltaH = (w << shift) / Deltay;
		int Error2 = 0;
		sy = 0;
		for (int r = 0; r< h ;r++) {
			// now we walk across the top calculating each rotated point
			// along the side
			// the use delta formula to walk in x and y in destination space
			// always walking in the x in the source!
			// figure out rotated location to start in dest
			rx = ( ( (sx - halfws) * ca) - ( (sy - halfhs) * sa) ) >> shift;
			rx += halfwd;
			ry = ( ( (sx - halfws) * sa) + ( (sy - halfhs) * ca) ) >> shift;
			ry += halfhd;

			// this is the end point of the line

			int y2 = ( ( ((w) - halfws) * sa) + ( (sy - halfhs) * ca) ) >> shift;
			y2 += halfhd;

			// length of line

			int diff = ABS(y2 - ry);

			// if walking backwards reveres diff to reflect sign

			src = MAKE_PTR(srcvp, x, y + sy);

			dest = MAKE_PTR(destvp, rx, ry);

			Error = 0;
			Error2 = 0;
			char * baseptr = src;
			// while walking line
			while (diff--) {
				char c = *src;
				// transparency
				if (c)
					*dest = *src;
				Error2 += DeltaH;
				dest += buffwid2;
				Error += Decimal;
				src = baseptr + (Error2 >> shift) ;
				// this is time to inc x in src y in dest
				if (Error >= fixpoint1) {
					Error -= fixpoint1;
					if (*src)
						*dest = *src;
					dest += xinc;
				}
			}
			sy += yinc;
		}
		return 0;
	} else { // else we walk in X
		int lasterror = 0;
		Error = 0;
		// start at left top corner in src and dest

		sx = 0;
		sy = 0;

		dx = 0;
		dy = 0;

		dest = MAKE_PTR(destvp, rx + dx, ry + dy);

		src = MAKE_PTR(srcvp, x + sx, y + sy);

		// this gets added to error each inc of x
		// when error is > 1 then inc y!
		int xinc = 1;
		if (Deltax < 0) {
			Deltax = -Deltax;
			xinc = -1;
		}
		int yinc = 1;
		if (Deltay < 0) {
			Deltay = - Deltay;
			buffwid2 = - buffwid2;
			sy = sy + h - 1;
			yinc = -1;
		}


		Decimal = ( Deltay << shift) / Deltax ;
		// walk in X

		int Deltax2 = Deltax << shift;
		int Deltay2 = Deltay << shift;

		// this is the ratios between the source width and the dest width
		// as the rectangle rotates the actual size changes!

		int DeltaW = (w << shift) / Deltax;
		int Error2 = 0;
		for (int r = 0; r< h ;r++) {
			sx = 0;

			// now we walk across the side calculating each rotated point
			// along the side
			// the use delta formula to walk in x and y in destination space
			// always walking in the x in the source!
			// figure out rotated location to start

			rx = ( ( (sx - halfws) * ca) - ( (sy - halfhs) * sa) ) >> shift;
			rx += halfwd;
			ry = ( ( (sx - halfws) * sa) + ( (sy - halfhs) * ca) ) >> shift;
			ry += halfhd;
			// this is the other side of the box

			int x2 = ( ( ((sx + w) - halfws) * ca) - ( (sy - halfhs) * sa) ) >> shift;

			x2 += halfwd;

			int diff = x2 - rx;


			dx = 0;
			dy = 0;

			if (xinc == -1) {
				diff = -diff;
			}

			src = MAKE_PTR(srcvp, x + sx, y + sy);

			dest = MAKE_PTR(destvp, rx + dx, ry + dy);
			Error = 0;
			Error2 = 0;
			char * baseptr = src;

			while (diff--) {
				char c = *src;
				// transparency
				if (c)
					*dest = *src;
				Error2 += DeltaW;
 				rx++;
				dest += xinc;
				Error += Decimal;
				src = baseptr + (Error2 >> shift);
				if (Error >= fixpoint1) {
					Error -= fixpoint1;
					if (*src)
						*dest = *src;
					dest += buffwid2;
				}
			}
			sy += yinc;
		}
	}
	return 0;
}
#endif