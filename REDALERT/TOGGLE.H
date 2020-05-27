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

/* $Header: /CounterStrike/TOGGLE.H 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TOGGLE.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/15/95                                                     *
 *                                                                                             *
 *                  Last Update : January 15, 1995 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef TOGGLE_H
#define TOGGLE_H

#include "control.h"

/*
**	This class handles gadgets that behave like the Windows buttons. That is, once the mouse
**	button is clicked over them, they capture the focus until the mouse button is released.
**	They have a different imagery for the pressed and released states. They only recognize
**	a valid selection when the mouse button is release while over the button.
*/
class ToggleClass : public ControlClass
{
	public:
		ToggleClass(unsigned id, int x, int y, int w, int h);
		void Turn_On(void);
		void Turn_Off(void);

		/*
		**	Is this button in a pressed down state? This occurs when the mouse is clicked on the
		**	button and the mouse is still being held down.
		*/
		unsigned IsPressed:1;

		/*
		**	This is the button on/off state. Sometimes a button that is "on" has a different
		**	imagery than one that is "off". If the on/off state is not necessary, then just
		**	ignore this flag.
		*/
		unsigned IsOn:1;

		/*
		**	If this button can be turned "on" or "off", then this flag should be set to
		**	true. Sometimes a button needs to display its on/off state. In the render routine,
		**	examine the IsOn flag and display accordingly. If this flag is false, then the
		**	IsOn flag will not be changed, regardless of button clicking.
		*/
		unsigned IsToggleType:1;

	protected:

		virtual int Action(unsigned flags, KeyNumType &key);
};

#endif
