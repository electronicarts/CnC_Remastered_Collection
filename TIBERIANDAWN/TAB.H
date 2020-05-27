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

/* $Header:   F:\projects\c&c\vcs\code\tab.h_v   2.18   16 Oct 1995 16:45:26   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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

		virtual void AI(KeyNumType &input, int x, int y);
		virtual void Draw_It(bool complete=false);

		virtual void One_Time(void);							// One-time inits
		static void Draw_Credits_Tab(void);
		static void Hilite_Tab(int tab);
		void  Redraw_Tab(void) {IsToRedraw = true;Flag_To_Redraw(false);};

		/*
		**	File I/O.
		*/
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);
		inline int Get_Tab_Height(void) { return(Tab_Height); };

		CreditClass Credits;

	protected:

		/*
		**	If the tab graphic is to be redrawn, then this flag is true.
		*/
		unsigned IsToRedraw:1;
		int Eva_Width;
		int Tab_Height;

	private:
		void  Set_Active(int select);

		static void const * TabShape;
};


#endif
