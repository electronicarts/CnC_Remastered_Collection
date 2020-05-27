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

/* $Header: /CounterStrike/GSCREEN.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : GSCREEN.H                                                    *
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

#ifndef GSCREEN_H
#define GSCREEN_H

#include	"function.h"
#include	"cell.h"

class GScreenClass
{
	public:

		GScreenClass(void);
		GScreenClass(NoInitClass const &) {};

		/*
		** Initialization
		*/
		virtual void One_Time(void);							// One-time initializations
		virtual void Init(TheaterType = THEATER_NONE);	// Inits everything
		virtual void Init_Clear(void);						// Clears all to known state
		virtual void Init_IO(void);							// Inits button list
		virtual void Init_Theater(TheaterType theater);	// Theater-specific inits

		/*
		**	Player I/O is routed through here. It is called every game tick.
		*/
		virtual void Input(KeyNumType & key, int & x, int & y);
		virtual void AI(KeyNumType &, int, int) {};
		virtual void Add_A_Button(GadgetClass & gadget);
		virtual void Remove_A_Button(GadgetClass & gadget);

		/*
		**	Called when map needs complete updating.
		*/
		virtual void Flag_To_Redraw(bool complete=false);

		/*
		**	Render maintenance routine (call every game tick). Probably no need
		**	to override this in derived classes.
		*/
		virtual void Render(void);

		/*
		**	Is called when actual drawing is required. This is the function to
		**	override in derived classes.
		*/
		virtual void Draw_It(bool =false) {};

		/*
		**	This moves the hidpage up to the seenpage.
		*/
		virtual void Blit_Display(void);

		/*
		**	Changes the mouse shape as indicated.
		*/
		virtual void Set_Default_Mouse(MouseType mouse, bool wsmall) = 0;
		virtual bool Override_Mouse_Shape(MouseType mouse, bool wsmall) = 0;
		virtual void Revert_Mouse_Shape(void) = 0;
		virtual void Mouse_Small(bool wsmall) = 0;

		/*
		**	Misc routines.
		*/
		virtual void * Shadow_Address(void) {return(ShadowPage);};

		/*
		**	This points to the buttons that are used for input. All of the derived classes will
		**	attached their specific buttons to this list.
		*/
		static GadgetClass * Buttons;

	private:

		/*
		**	If the entire map is required to redraw, then this flag is true. This flag
		**	is set by the Flag_To_Redraw function. Typically, this occurs when the screen
		**	has been trashed or is first created.
		*/
		unsigned IsToRedraw:1;

		/*
		**	If only a sub-system of the map must be redrawn, then this flag will be set.
		**	An example of something that would set this flag would be an animating icon
		**	in the sidebar. In such a case, complete redrawing of the entire display is not
		**	necessary, but the Draw_It function should still be called so that the appropriate
		**	class can perform it's rendering.
		*/
		unsigned IsToUpdate:1;

		/*
		**	Pointer to an exact copy of the visible graphic page. This copy is used to speed
		**	display rendering by using an only-update-changed-pixels algorithm.
		*/
		static GraphicBufferClass * ShadowPage;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[1024];
};

#endif