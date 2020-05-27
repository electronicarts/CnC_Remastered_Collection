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

/* $Header: /CounterStrike/BAR.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BAR.CPP                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 08/16/96                                                     *
 *                                                                                             *
 *                  Last Update : August 16, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   ProgressBarClass::Is_Horizontal -- Determines if the bargraph is horizontal or not.       *
 *   ProgressBarClass::Outline -- Draw an outline around the bargraph if supposed to.          *
 *   ProgressBarClass::ProgressBarClass -- Constructor for the bargraph object.                *
 *   ProgressBarClass::Redraw -- Redraw the bargraph.                                          *
 *   ProgressBarClass::Set_Limit -- Set the logic tracking value.                              *
 *   ProgressBarClass::Update -- Update the value and redraw as necessary.                     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"bar.h"
#include	"fixed.h"


/***********************************************************************************************
 * ProgressBarClass::ProgressBarClass -- Constructor for the bargraph object.                  *
 *                                                                                             *
 *    This is the constructor for the bargraph object. It establishes the dimensions and       *
 *    coordinate of the bargraph as well as the colors it will use when drawn.                 *
 *                                                                                             *
 * INPUT:   w,y      -- Pixel coordinate of the upper left corner of the bargraph.             *
 *                                                                                             *
 *          width,height   -- Dimensions of the bargraph.                                      *
 *                                                                                             *
 *          forecolor   -- The color to use for the filled portion of the bargraph.            *
 *                                                                                             *
 *          backcolor   -- The color to use for the non-filled portion of the bargraph.        *
 *                                                                                             *
 *          bordercolor -- Optional border color. If not zero, then the bargraph will be       *
 *                         outlined with this color.                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
ProgressBarClass::ProgressBarClass(int x, int y, int width, int height, int forecolor, int backcolor, int bordercolor) :
	X(x),
	Y(y),
	Width(width),
	Height(height),
	BarColor(forecolor),
	BackColor(backcolor),
	BorderColor(bordercolor),
	CurrentValue(0),
	LastDisplayCurrent(0),
	IsDrawn(false)
{
}


/***********************************************************************************************
 * ProgressBarClass::Is_Horizontal -- Determines if the bargraph is horizontal or not.         *
 *                                                                                             *
 *    If the bargraph is oriented horizontally, then this function will return TRUE.           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this bargraph horizontal?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ProgressBarClass::Is_Horizontal(void) const
{
	if (Width > Height) return(true);
	return(false);
}


/***********************************************************************************************
 * ProgressBarClass::Update -- Update the value and redraw as necessary.                       *
 *                                                                                             *
 *    This will update the value of the bargraph to the fill ratio specified and then          *
 *    redraw it if required. Very small changes to the bargraph value might not result in a    *
 *    visual change.                                                                           *
 *                                                                                             *
 * INPUT:   value -- The new value to assign to this bargraph.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   bool; Did this update result in a redraw?                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ProgressBarClass::Update(fixed value)
{
	CurrentValue = value;

	if (!IsDrawn || value - LastDisplayCurrent >= fixed(1, 10)) {
		Redraw();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * ProgressBarClass::Outline -- Draw an outline around the bargraph if supposed to.            *
 *                                                                                             *
 *    This routine will draw a border around the bargraph if this bargraph has a color         *
 *    specified for the border.                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void ProgressBarClass::Outline(void) const
{
	if (Is_Outlined()) {
		LogicPage->Draw_Line(X, Y, X+Width, Y, BorderColor);
		LogicPage->Draw_Line(X, Y, X, Y+Height, BorderColor);
		LogicPage->Draw_Line(X, Y+Height, X, Y+Height, BorderColor);
		LogicPage->Draw_Line(X+Width, Y, X+Width, Y+Height, BorderColor);
	}
}


/***********************************************************************************************
 * ProgressBarClass::Redraw -- Redraw the bargraph.                                            *
 *                                                                                             *
 *    This will redraw the entire bargraph.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void ProgressBarClass::Redraw(void) const
{
	Hide_Mouse();

	Outline();

	/*
	**	Determine the inner dimensions of the bargraph. This will be
	**	somewhat smaller than indicated if it has a border.
	*/
	int x = X;
	int y = Y;
	int w = Width;
	int h = Height;
	if (Is_Outlined()) {
		x += 1;
		y += 1;
		w -= 2;
		h -= 2;
	}

	/*
	**	The working "length" of the bargraph is dependant on whether the
	**	bargraph is horizontal or vertical.
	*/
	int size = Is_Horizontal() ? w : h;

	/*
	**	Determine the number of pixels to fill in the bargraph depending on the
	**	size of the internal value. The larger the internal value the more
	**	filled the bargraph becomes.
	*/
	int fill = CurrentValue * size;

	/*
	**	Draw the filled portion of the bargraph if there is any pixels to draw.
	*/
	if (fill > 0) {
		if (Is_Horizontal()) {
			LogicPage->Fill_Rect(x, y, x+fill, y+h, BarColor);
		} else {
			LogicPage->Fill_Rect(x, y+fill, x+w, y+h, BarColor);
		}
	}

	/*
	**	Draw the unfilled portion of the bargraph if there are any pixels to
	**	draw of it.
	*/
	if (w-fill > 0) {
		if (Is_Horizontal()) {
			LogicPage->Fill_Rect(x+fill, y, x+w, y+h, BackColor);
		} else {
			LogicPage->Fill_Rect(x, y, x+w, y+fill-1, BackColor);
		}
	}

	Show_Mouse();

	ProgressBarClass * me = (ProgressBarClass *)this;
	me->LastDisplayCurrent = CurrentValue;
	me->IsDrawn = true;
}
