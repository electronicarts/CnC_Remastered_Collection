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

/* $Header:   F:\projects\c&c\vcs\code\gauge.cpv   2.19   16 Oct 1995 16:50:56   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : GAUGE.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic, Maria del Mar McCready Legg                   *
 *                                                                                             *
 *                   Start Date : 01/15/95                                                     *
 *                                                                                             *
 *                  Last Update : January 16, 1995 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   GaugeClass::Action -- Handles input events for the gauge.                                 *
 *   GaugeClass::Draw_Me -- Draws the body of the gauge.                                       *
 *   GaugeClass::Value_To_Pixel -- Convert gauge value to pixel offset.                        *
 *   GaugeClass::Pixel_To_Value -- Convert a pixel offset into a gauge value.                  *
 *   GaugeClass::Set_Value -- Set the value of the gauge.                                      *
 *   GaugeClass::Set_Maximum -- Sets the maximum value for the gauge.                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***************************************************************************
 * GAUGECLASS::GAUGECLASS -- class constructor                             *
 *                                                                         *
 * INPUT:    id                     -- button ID                           *
 *                                                                         *
 *             x,y                     -- upper-left corner, in pixels     *
 *                                                                         *
 *             w,h                     -- width, height, in pixels         *
 *                                                                         *
 * OUTPUT:   none.                                                         *
 *                                                                         *
 * WARNINGS: none.                                                         *
 *                                                                         *
 * HISTORY:  01/05/1995 MML : Created.                                     *
 *=========================================================================*/
GaugeClass::GaugeClass(unsigned id, int x, int y, int w, int h)
	: ControlClass(id, x, y, w, h, LEFTHELD|LEFTPRESS|LEFTRELEASE, true)
{
	Set_Maximum(255);
	Set_Value(0);

	HasThumb = true;
	IsHorizontal = (w > h);
	IsColorized = true;

	ClickDiff = 0;
}


/***********************************************************************************************
 * GaugeClass::Set_Maximum -- Sets the maximum value for the gauge.                            *
 *                                                                                             *
 *    This routine will set the maximum value for the gauge. This is the largest value that    *
 *    the current setting may reach. The ability to change this allows the guage to use and    *
 *    return values that are convenient for the programmer's current needs.                    *
 *                                                                                             *
 * INPUT:   value -- The value to use as the gauge maximum.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the gauge maximum changed? A false indicates that the specified value    *
 *                already matches the current maximum.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int GaugeClass::Set_Maximum(int value)
{
	if (value != MaxValue) {
	   MaxValue = value;
		Flag_To_Redraw();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * GaugeClass::Set_Value -- Set the value of the gauge.                                        *
 *                                                                                             *
 *    This routine will set the current value for the gauge. This value is clipped to the      *
 *    limits of the gauge maximum.                                                             *
 *                                                                                             *
 * INPUT:   value -- The value to set at the new current value.                                *
 *                                                                                             *
 * OUTPUT:  bool; Was the current setting changed? A false indicates that the setting          *
 *                specified is the same as what was already there.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int GaugeClass::Set_Value(int value)
{
	value = Bound(value, 0, MaxValue);
//	value = MIN(value, MaxValue);
//	value = MAX(value, 0);
	if (value != CurValue) {
		CurValue = value;
		Flag_To_Redraw();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * GaugeClass::Pixel_To_Value -- Convert a pixel offset into a gauge value.                    *
 *                                                                                             *
 *    Use this routine to conver the specified pixel offset into a gauge value. This is used   *
 *    in translating mouse clicks into a cooresponding setting for the guage.                  *
 *                                                                                             *
 * INPUT:   pixel -- The pixel offset form the start of the gauge.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the setting value in guage coordinates.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int GaugeClass::Pixel_To_Value(int pixel)
{
	int maximum;

	if (IsHorizontal) {
		pixel -= X+1;
		maximum = Width;
	} else {
		pixel -= Y+1;
		maximum = Height;
	}
	maximum -= 2;
	pixel = Bound(pixel, 0, maximum);
//	pixel = MIN(pixel, maximum);
//	pixel = MAX(pixel, 0);
	return(Fixed_To_Cardinal(MaxValue, Cardinal_To_Fixed(maximum, pixel)));
}


/***********************************************************************************************
 * GaugeClass::Value_To_Pixel -- Convert gauge value to pixel offset.                          *
 *                                                                                             *
 *    Use this routine to convert the specified gauge value into a pixel offset from the       *
 *    star of the gauge. This is used for thumb positioning.                                   *
 *                                                                                             *
 * INPUT:   value -- The value to convert to a pixel offset.                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the pixel offset of the specified value from the start of the         *
 *          guage.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int GaugeClass::Value_To_Pixel(int value)
{
	int maximum;
	int start;
	if (IsHorizontal) {
		maximum = Width;
		start = X;
	} else {
		maximum = Height;
		start = Y;
	}
	maximum -= 2;
	return(start + Fixed_To_Cardinal(maximum, Cardinal_To_Fixed(MaxValue, value)));
}


/***********************************************************************************************
 * GaugeClass::Draw_Me -- Draws the body of the gauge.                                         *
 *                                                                                             *
 *    This routine will draw the body of the gauge if necessary.                               *
 *                                                                                             *
 * INPUT:      forced   -- Should the gauge be redrawn regardless of the current redraw flag?  *
 *                                                                                             *
 * OUTPUT:     bool; Was the gauge redrawn?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:    01/16/1995 JLB : Created.                                                       *
 *=============================================================================================*/
int GaugeClass::Draw_Me(int forced)
{
	if (ControlClass::Draw_Me(forced)) {

		/*
		===================== Hide the mouse =====================
		*/
		if (LogicPage == &SeenBuff) {
			Conditional_Hide_Mouse(X, Y, X+Width, Y+Height);
		}

		/*
		=========== Draw the body & set text color ===============
		*/
		Draw_Box (X, Y, Width, Height, BOXSTYLE_GREEN_DOWN, true);

		/*
		**	Colorize the inside of the gauge if indicated.
		*/
		if (IsColorized) {
			int middle = Value_To_Pixel(CurValue);
			int color = CC_BRIGHT_GREEN;
			if (IsHorizontal) {
				if (middle >= (X + 1))
					LogicPage->Fill_Rect(X+1, Y+1, middle, Y+Height-2, color);
			} else {
				if (middle >= (Y + 1))
					LogicPage->Fill_Rect(X+1, Y+1, X+Width-2, middle, color);
			}
		}

		if (HasThumb)
			Draw_Thumb();

		/*
		=================== Display the mouse ===================
		*/
		if (LogicPage == &SeenBuff) {
			Conditional_Show_Mouse();
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * GaugeClass::Action -- Handles input events for the gauge.                                   *
 *                                                                                             *
 *    This routine will handle input event processing for the gauge. It will adjust the        *
 *    current setting of the gauge according to the mouse position.                            *
 *                                                                                             *
 * INPUT:   flags -- The input event that is the reason for this function call.                *
 *          key   -- The key code that caused the event.                                       *
 *                                                                                             *
 * OUTPUT:  bool; Was the even recognized, processed, and no further gadget scanning is        *
 *                desird (for this pass).                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int GaugeClass::Action(unsigned flags, KeyNumType &key)
{
	/*
	** If there's no thumb on this gauge, it's a display-only device; ignore
	** any input.
	*/
	if (!HasThumb) {
		key = KN_NONE;
		return(true);
	}

	/*
	**	We might end up clearing the event bits. Make sure that the sticky
	**	process is properly updated anyway.
	*/
	Sticky_Process(flags);

	/*
	**	If the thumb is currently being "dragged around", then update the slider
	**	position according to the mouse position. In all other cases, ignore the
	**	button being held down.
	*/
	if ((flags & LEFTPRESS) || ((flags & LEFTHELD) && StuckOn == this)) {

		/*
		** Compute the difference between where we clicked, and the edge of
		** the thumb (only if we clicked on the thumb.)
		*/
		if (flags & LEFTPRESS) {
			int curpix = Value_To_Pixel(CurValue);
			int clickpix = (IsHorizontal ? Get_Mouse_X() : Get_Mouse_Y());

			if ( (clickpix > curpix) && (clickpix - curpix) < Thumb_Pixels()) {
				ClickDiff = (clickpix - curpix);
			} else {
				ClickDiff = 0;
			}

			int testval = Pixel_To_Value(IsHorizontal ?
				Get_Mouse_X() - ClickDiff : Get_Mouse_Y() - ClickDiff);

			/*
			** Correct for round-down errors in Pixel_To_Value() and
			** Value_To_Pixe(); make ClickDiff exactly right so that
			** at this point, Get_Mouse_n() - ClickDiff converts to
			** CurValue.
			*/
			while (testval < CurValue && ClickDiff > 0) {
				ClickDiff--;
				testval = Pixel_To_Value(IsHorizontal ?
				Get_Mouse_X() - ClickDiff : Get_Mouse_Y() - ClickDiff);
			}
		}

		/*
		**	If no change occurred in the gauge, just call Control's Action routine,
		** but turn off the flags so it won't fill in 'key' with the button ID.
		** Thus, no button ID will be returned by Input.
		*/
		if (!Set_Value(Pixel_To_Value(IsHorizontal ?
			Get_Mouse_X() - ClickDiff : Get_Mouse_Y() - ClickDiff))) {

			flags &= ~(LEFTHELD|LEFTRELEASE|LEFTPRESS);
			ControlClass::Action(0,key);
			key = KN_NONE;
			return(true);
		}

	} else {

		/*
		**	Ingore the left mouse button being held down if this gauge is not
		**	currently in "sticky" mode. This allows processing of the LEFTPRESS
		**	by any derived classes such that this guage can be more closely
		**	controlled.
		*/
		flags &= ~LEFTHELD;
	}
	return(ControlClass::Action(flags, key));
}


/***********************************************************************************************
 * GaugeClass::Draw_Thumb -- Draws the body of the gauge.                                      *
 *                                                                                             *
 *    This routine will draw the body of the gauge if necessary.                               *
 *                                                                                             *
 * INPUT:      none.                                                                           *
 *                                                                                             *
 * OUTPUT:     none.                                                                           *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:    01/16/1995 MML : Created.                                                       *
 *=============================================================================================*/
void GaugeClass::Draw_Thumb(void)
{
	int x = Value_To_Pixel(CurValue);

//	if ((x + 8) > Value_To_Pixel(MaxValue)) {
	if ((x + 4) > Value_To_Pixel(MaxValue)) {
		x = Value_To_Pixel(MaxValue) - 2;
	}

	if (IsHorizontal) {
		Draw_Box(x, Y, 4, Height, BOXSTYLE_GREEN_RAISED, true);
		//Draw_Box(x, Y, 8, Height, BOXSTYLE_GREEN_RAISED, true);
	} else {
		Draw_Box(X, x, Width, 4,  BOXSTYLE_GREEN_RAISED, true);
		//Draw_Box(X, x, Width, 8,  BOXSTYLE_GREEN_RAISED, true);
	}
}


/***********************************************************************************************
 * TriColorGaugeClass::TriColorGaugeClass -- Constructor for 3 color (red\yellow\green) gauge. *
 *                                                                                             *
 *    This routine will draw the body of the gauge if necessary.                               *
 *                                                                                             *
 * INPUT:      See below.                                                                      *
 *                                                                                             *
 * OUTPUT:     none.                                                                           *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:    01/16/1995 MML : Created.                                                       *
 *=============================================================================================*/
TriColorGaugeClass::TriColorGaugeClass(unsigned id, int x, int y, int w, int h)
	: GaugeClass(id, x, y, w, h)
{
	RedLimit = 0;				// maximum value for red
	YellowLimit = 0;			// maximum value for yellow
}


/***********************************************************************************************
 * TriColorGaugeClass::Set_Red_Limit -- Set the value for the red area of gauge.               *
 *                                                                                             *
 * INPUT:      int value.                                                                      *
 *                                                                                             *
 * OUTPUT:     bool true of false.                                                             *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:    01/16/1995 MML : Created.                                                       *
 *=============================================================================================*/
int TriColorGaugeClass::Set_Red_Limit(int value)
{
	if (value >= 0 && value < MaxValue) {

//		if (value > YellowLimit) {
//			RedLimit = YellowLimit;
//			YellowLimit = value;
//		} else {
			RedLimit = value;
//		}
		Flag_To_Redraw();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TriColorGaugeClass::Set_Yellow_Limit -- Set the value for the yellow area of gauge.         *
 *                                                                                             *
 * INPUT:      int value.                                                                      *
 *                                                                                             *
 * OUTPUT:     bool true of false.                                                             *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:    01/16/1995 MML : Created.                                                       *
 *=============================================================================================*/
int TriColorGaugeClass::Set_Yellow_Limit(int value)
{
	if (value >= 0 && value < MaxValue) {

//		if (value < RedLimit) {
//			YellowLimit = RedLimit;
//			RedLimit = value;
//		} else {
			YellowLimit = value;
//		}
		Flag_To_Redraw();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TriColorGaugeClass::Draw_Me -- Draw the tri color gauge.                                    *
 *                                                                                             *
 * INPUT:      int forced -- draw or not?                                                      *
 *                                                                                             *
 * OUTPUT:     bool true of false.                                                             *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:    01/16/1995 MML : Created.                                                       *
 *=============================================================================================*/
int TriColorGaugeClass::Draw_Me(int forced)
{
	if (ControlClass::Draw_Me(forced)) {

		/*
		===================== Hide the mouse =====================
		*/
		if (LogicPage == &SeenBuff) {
			Conditional_Hide_Mouse(X, Y, X+Width, Y+Height);
		}
		/*
		=========== Draw the body & set text color ===============
		*/
		Draw_Box (X, Y, Width, Height, (IsDisabled ? BOXSTYLE_GREEN_RAISED : BOXSTYLE_GREEN_DOWN), true);

		/*
		**	Colorize the inside of the gauge if indicated.
		*/
		int red    = Value_To_Pixel(RedLimit);
		int yellow = Value_To_Pixel(YellowLimit);
		int middle = Value_To_Pixel(CurValue);

		if (CurValue <= RedLimit) {
			if (IsHorizontal) {
				LogicPage->Fill_Rect(X+1, Y+1, middle, Y+Height-2, PINK);
			} else {
				LogicPage->Fill_Rect(X+1, Y+1, X+Width-2, middle, PINK);
			}
		} else if (CurValue > RedLimit && CurValue <= YellowLimit) {
			if (IsHorizontal) {
				LogicPage->Fill_Rect(X+1, Y+1, red,    Y+Height-2, PINK);
				LogicPage->Fill_Rect(red, Y+1, middle, Y+Height-2, YELLOW);
			} else {
				LogicPage->Fill_Rect(X+1, Y+1, X+Width-2, red,    PINK);
				LogicPage->Fill_Rect(X+1, red, X+Width-2, middle, YELLOW);
			}
		} else if (CurValue > YellowLimit && CurValue <= MaxValue) {

			if (IsHorizontal) {
				LogicPage->Fill_Rect(X+1,    Y+1, red,    Y+Height-2, PINK);
				LogicPage->Fill_Rect(red,    Y+1, yellow, Y+Height-2, YELLOW);
				LogicPage->Fill_Rect(yellow, Y+1, middle, Y+Height-2, GREEN);
			} else {
				LogicPage->Fill_Rect(X+1, Y+1,    X+Width-2, red,    PINK);
				LogicPage->Fill_Rect(X+1, red,    X+Width-2, yellow, YELLOW);
				LogicPage->Fill_Rect(X+1, yellow, X+Width-2, middle, GREEN);
			}
		}

		if (HasThumb)
			Draw_Thumb();

		/*
		=================== Display the mouse ===================
		*/
		if (LogicPage == &SeenBuff) {
			Conditional_Show_Mouse();
		}
		return(true);
	}
	return(false);
}

