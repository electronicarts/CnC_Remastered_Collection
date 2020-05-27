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

/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Westwood 32 Bit Library						*
 *                                                                         *
 *                    File Name : DRAWRECT.C                               *
 *                                                                         *
 *                   Programmer : Christopher Yates                        *
 *                                                                         *
 *                  Last Update : August 20, 1993   [JLB]                  *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Draw_Rect -- Draws a rectangle to the LogicPage.                      *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef GBUFFER_H
#include "gbuffer.h"
#include "misc.h"
#endif

/***************************************************************************
 * Draw_Rect -- Draws a rectangle to the LogicPage.                        *
 *                                                                         *
 *    This routine will draw a rectangle to the LogicPage.  The rectangle  *
 *    doesn't have to be aligned on the vertical or horizontal axis.  In   *
 *    fact, it doesn't even have to be a rectangle.  The "square" can be   *
 *    skewed.                                                              *
 *                                                                         *
 * INPUT:   x1_pixel, y1_pixel   -- One corner.                            *
 *                                                                         *
 *          x2_pixel, y2_pixel   -- The other corner.                      *
 *                                                                         *
 *          color                -- The color to draw the lines.           *
 *                                                                         *
 * OUTPUT:  none                                                           *
 *                                                                         *
 * WARNINGS:   None, but the rectangle will be clipped to the current      *
 *             draw line clipping rectangle.                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/20/1993 JLB : Created.                                             *
 *=========================================================================*/
VOID GraphicViewPortClass::Draw_Rect(int x1_pixel, int y1_pixel, int x2_pixel, int y2_pixel, unsigned char color)
{
	Lock();
	Draw_Line(x1_pixel, y1_pixel, x2_pixel, y1_pixel, color);
	Draw_Line(x1_pixel, y2_pixel, x2_pixel, y2_pixel, color);
	Draw_Line(x1_pixel, y1_pixel, x1_pixel, y2_pixel, color);
	Draw_Line(x2_pixel, y1_pixel, x2_pixel, y2_pixel, color);
	Unlock();
}

