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

/* $Header: /CounterStrike/CHECKBOX.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CHECKBOX.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/26/95                                                     *
 *                                                                                             *
 *                  Last Update : July 6, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CheckBoxClass::Action -- Handles a button action on a checkbox object.                    *
 *   CheckBoxClass::Draw_Me -- Draws the checkbox imagery.                                     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"checkbox.h"


/***********************************************************************************************
 * CheckBoxClass::Draw_Me -- Draws the checkbox imagery.                                       *
 *                                                                                             *
 *    This routine will draw the checkbox either filled or empty as necessary.                 *
 *                                                                                             *
 * INPUT:   forced   -- Should the check box be drawn even if it doesn't think it needs to?    *
 *                                                                                             *
 * OUTPUT:  Was the check box rendered?                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int CheckBoxClass::Draw_Me(int forced)
{
	if (ToggleClass::Draw_Me(forced)) {

		Hide_Mouse();
		Draw_Box(X, Y, Width, Height, BOXSTYLE_DOWN, false);
		LogicPage->Fill_Rect(X+1, Y+1, X+Width-2, Y+Height-2, DKGREY);
		if (IsOn) {
			LogicPage->Fill_Rect(X+1, Y+1, X+Width-2, Y+Height-2, LTGREEN);
		}
		Show_Mouse();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * CheckBoxClass::Action -- Handles a button action on a checkbox object.                      *
 *                                                                                             *
 *    This routine will detect if the mouse has been clicked on the checkbox object. If so,    *
 *    the check box state will be toggled.                                                     *
 *                                                                                             *
 * INPUT:   flags -- The event flags that resulted in this routine being called.               *
 *                                                                                             *
 *          key   -- The key that resulted in this routine being called.                       *
 *                                                                                             *
 * OUTPUT:  bool; Should normal processing occur?                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int CheckBoxClass::Action(unsigned flags, KeyNumType & key)
{
	if (flags & LEFTRELEASE) {
		if (IsOn) {
			Turn_Off();
		} else {
			Turn_On();
		}
	}
	return(ToggleClass::Action(flags, key));
}
