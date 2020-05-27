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

/* $Header: /CounterStrike/BAR.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BAR.H                                                        *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 08/16/96                                                     *
 *                                                                                             *
 *                  Last Update : August 16, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef BAR_H
#define BAR_H

/*
**	The "bool" integral type was defined by the C++ committee in
**	November of '94. Until the compiler supports this, use the following
**	definition.
*/
#ifndef __BORLANDC__
#ifndef TRUE_FALSE_DEFINED
#define TRUE_FALSE_DEFINED
enum {false=0,true=1};
typedef int bool;
#endif
#endif

#include "fixed.h"


/*
**	This is a manager for a progress (or other) bargraph. Such a graph consists of a fill
**	and a background region. The fill percentage of the bargraph is controlled by an
**	update value. The bargraph can be optionally outlined.
*/
class ProgressBarClass
{
	public:
		ProgressBarClass(int x, int y, int width, int height, int forecolor, int backcolor, int bordercolor=0);

		bool Update(fixed value);
		void Redraw(void) const;

	private:

		void Outline(void) const;
		bool Is_Horizontal(void) const;
		bool Is_Outlined(void) const {return(BorderColor != 0);}

		/*
		**	This is the upper left coordinates of the bargraph.
		*/
		int X,Y;

		/*
		**	This is the dimensions of the bargraph.
		*/
		int Width, Height;

		/*
		**	These are the colors to use when drawing the progress bar.
		*/
		int BarColor;
		int BackColor;
		int BorderColor;

		/*
		**	This is the current value of the bargraph.
		*/
		fixed CurrentValue;

		/*
		**	This is the current value as of the last time the bargraph was rendered.
		*/
		fixed LastDisplayCurrent;

		/*
		**	If the bargraph has been drawn at least once, then this flag will
		**	be true.
		*/
		unsigned IsDrawn:1;
};


#endif
