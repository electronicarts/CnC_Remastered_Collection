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

/* $Header: /CounterStrike/SCROLL.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SCROLL.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 11/18/94                                                     *
 *                                                                                             *
 *                  Last Update : November 18, 1994 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef SCROLL_H
#define SCROLL_H

#include	"help.h"


class ScrollClass: public HelpClass
{
		/*
		**	If map scrolling is automatic, then this flag is true. Automatic scrolling will
		**	cause the map to scroll if the mouse is in the scroll region, regardless of
		**	whether or not the mouse button is held down.
		*/
		unsigned IsAutoScroll:1;

		/*
		**	Scroll speed is regulated by this count down timer. When this value reaches zero,
		**	scroll the map in the direction required and reset this timer.
		*/
		static CDTimerClass<SystemTimerClass> Counter;

		/*
		** Inertia control for scrolling
		*/
		int	Inertia;

	public:
		ScrollClass(void);
		ScrollClass(NoInitClass const & x) : HelpClass(x) {};

		bool Set_Autoscroll(int control);

		virtual void AI(KeyNumType &input, int x, int y);
		virtual void Init_IO(void) {Counter = 0;HelpClass::Init_IO();};
};

#endif
