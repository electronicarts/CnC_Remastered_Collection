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

/* $Header: /CounterStrike/TOGGLE.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TOGGLE.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/15/95                                                     *
 *                                                                                             *
 *                  Last Update : February 2, 1995 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   ToggleClass::ToggleClass -- Normal constructor for toggle button gadgets.                 *
 *   ToggleClass::Turn_Off -- Turns the toggle button to the "OFF" state.                      *
 *   ToggleClass::Turn_On -- Turns the toggle button to the "ON" state.                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"toggle.h"


/***********************************************************************************************
 * ToggleClass::ToggleClass -- Normal constructor for toggle button gadgets.                   *
 *                                                                                             *
 *    This is the normal constructor for toggle buttons. A toggle button is one that most      *
 *    closely resembles the Windows style. It has an up and down state as well as an on        *
 *    and off state.                                                                           *
 *                                                                                             *
 * INPUT:   id    -- ID number for this button.                                                *
 *                                                                                             *
 *          x,y   -- Pixel coordinate of upper left corner of this button.                     *
 *                                                                                             *
 *          w,h   -- Width and height of the button.                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ToggleClass::ToggleClass(unsigned id, int x, int y, int w, int h) :
	ControlClass(id, x, y, w, h, LEFTPRESS|LEFTRELEASE, true),
	IsPressed(false),
	IsOn(false),
	IsToggleType(false)
{
}


/***********************************************************************************************
 * ToggleClass::Turn_On -- Turns the toggle button to the "ON" state.                          *
 *                                                                                             *
 *    This routine will turn the button on. The button will also be flagged to be redrawn      *
 *    at the next opportunity.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ToggleClass::Turn_On(void)
{
	IsOn = true;
	Flag_To_Redraw();
}


/***********************************************************************************************
 * ToggleClass::Turn_Off -- Turns the toggle button to the "OFF" state.                        *
 *                                                                                             *
 *    This routine will turn the toggle button "off". It will also be flagged to be redrawn    *
 *    at the next opportunity.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ToggleClass::Turn_Off(void)
{
	IsOn = false;
	Flag_To_Redraw();
}


/***********************************************************************************************
 * ToggleClass::Action -- Handles mouse clicks on a text button.                               *
 *                                                                                             *
 *    This routine will process any mouse or keyboard event that is associated with this       *
 *    button object. It detects and flags the text button so that it will properly be drawn    *
 *    in a pressed or raised state. It also handles any toggle state for the button.           *
 *                                                                                             *
 * INPUT:   flags -- The event flags that triggered this button.                               *
 *                                                                                             *
 *          key   -- The keyboard code associated with this event. Usually this is KN_LMOUSE   *
 *                   or similar, but it could be a regular key if this text button is given    *
 *                   a hotkey.                                                                 *
 *                                                                                             *
 * OUTPUT:  Returns whatever the lower level processing for buttons decides. This is usually   *
 *          true.                                                                              *
 *                                                                                             *
 * WARNINGS:   The button is flagged to be redrawn by this routine.                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/14/1995 JLB : Created.                                                                 *
 *   02/02/1995 JLB : Left press doesn't get passed to other buttons now                       *
 *=============================================================================================*/
int ToggleClass::Action(unsigned flags, KeyNumType &key)
{
	/*
	**	If there are no action flag bits set, then this must be a forced call. A forced call
	**	must never actually function like a real call, but rather only performs any necessary
	**	graphic updating.
	*/
	bool overbutton = ((Get_Mouse_X() - X) < Width && (Get_Mouse_Y() - Y) < Height );
	if (!flags) {
		if (overbutton) {
			if (!IsPressed) {
				IsPressed = true;
				Flag_To_Redraw();
			}
		} else {
			if (IsPressed) {
				IsPressed = false;
				Flag_To_Redraw();
			}
		}
	}

	/*
	**	Handle the sticky state for this gadget. It must be processed here
	**	because the event flags might be cleared before the action function
	**	is called.
	*/
	Sticky_Process(flags);

	/*
	**	Flag the button to show the pressed down imagery if this mouse button
	**	was pressed over this gadget.
	*/
	if (flags & LEFTPRESS) {
		IsPressed = true;
		Flag_To_Redraw();
		flags &= ~LEFTPRESS;
		ControlClass::Action(flags, key);
		key = KN_NONE;				// erase the event
		return(true);		// stop processing other buttons now
	}

	if (flags & LEFTRELEASE) {
		if (IsPressed) {
			if (IsToggleType && overbutton) {
				IsOn = (IsOn == false);
			}
			IsPressed = false;
			Flag_To_Redraw();
		} else {
			flags &= ~LEFTRELEASE;
		}
	}

	/*
	**	Do normal button processing. This ends up causing the button's ID number to
	**	be returned from the controlling Input() function.
	*/
	return(ControlClass::Action(flags, key));
}


