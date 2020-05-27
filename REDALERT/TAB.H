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

/* $Header: /CounterStrike/TAB.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TAB.H                                                        *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/15/94                                                     *
 *                                                                                             *
 *                  Last Update : December 15, 1994 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef TAB_H
#define TAB_H

#include	"sidebar.h"
#include	"credits.h"

class TabClass: public SidebarClass
{
	public:
		TabClass(void);
		TabClass(NoInitClass const & x) : SidebarClass(x), Credits(x), FlasherTimer(x), MoneyFlashTimer(x) {};

		virtual void AI(KeyNumType &input, int x, int y);
		virtual void Draw_It(bool complete=false);
		static void Draw_Credits_Tab(void);
		static void Hilite_Tab(int tab);
		void Flash_Money(void);

		virtual void One_Time(void);							// One-time inits
		void Redraw_Tab(void) {IsToRedraw = true;Flag_To_Redraw(false);};

		CreditClass Credits;

		CDTimerClass<FrameTimerClass> FlasherTimer;

	protected:

		/*
		**	If the tab graphic is to be redrawn, then this flag is true.
		*/
		unsigned IsToRedraw:1;

	private:
		void Set_Active(int select);

		CDTimerClass<FrameTimerClass> MoneyFlashTimer;

		static void const * TabShape;
};


#endif
