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

/* $Header:   F:\projects\c&c\vcs\code\credits.h_v   2.18   16 Oct 1995 16:47:26   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CREDIT.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 19, 1994                                               *
 *                                                                                             *
 *                  Last Update : April 19, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CREDITS_H
#define CREDITS_H

class HouseClass;
extern HouseClass *PlayerPtr;

/****************************************************************************
**	The animating credit counter display is controlled by this class.
*/
class CreditClass {
	public:
		long Credits;		// Value of credits trying to update display to.

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		CreditClass(void);

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		void Update(bool forced=false, bool redraw=false);

		void Graphic_Logic(bool forced=false);
		void AI(bool forced=false, HouseClass *player_ptr = PlayerPtr, bool logic_only = false);		// Added house and logic_only parameters. ST - 10/16/2019 2:26PM

		long	Current;		// Credit value currently displayed.

		unsigned IsToRedraw:1;
		unsigned IsUp:1;
		unsigned IsAudible:1;

	private:
		int	Countdown;	// Delay between ticks.
};

#endif
