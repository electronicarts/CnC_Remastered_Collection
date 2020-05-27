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

/* $Header: /CounterStrike/GSCREEN.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : GSCREEN.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/15/94                                                     *
 *                                                                                             *
 *                  Last Update : January 19, 1995 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   GScreenClass::Add_A_Button -- Add a gadget to the game input system.                      *
 *   GScreenClass::Blit_Display -- Redraw the display from the hidpage to the seenpage.        *
 *   GScreenClass::Flag_To_Redraw -- Flags the display to be redrawn.                          *
 *   GScreenClass::GScreenClass -- Default constructor for GScreenClass.                       *
 *   GScreenClass::Init -- Init's the entire display hierarchy by calling all Init routines.   *
 *   GScreenClass::Init_Clear -- Sets the map to a known state.                                *
 *   GScreenClass::Init_IO -- Initializes the Button list ('Buttons').                         *
 *   GScreenClass::Init_Theater -- Performs theater-specific initializations.                  *
 *   GScreenClass::Input -- Fetches input and processes gadgets.                               *
 *   GScreenClass::One_Time -- Handles one time class setups.                                  *
 *   GScreenClass::Remove_A_Button -- Removes a gadget from the game input system.             *
 *   GScreenClass::Render -- General drawing dispatcher an display update function.            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


GadgetClass * GScreenClass::Buttons = 0;

GraphicBufferClass * GScreenClass::ShadowPage = 0;


/***********************************************************************************************
 * GScreenClass::GScreenClass -- Default constructor for GScreenClass.                         *
 *                                                                                             *
 *    This constructor merely sets the display system, so that it will redraw the first time   *
 *    the render function is called.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
GScreenClass::GScreenClass(void)
{
	IsToUpdate = true;
	IsToRedraw = true;
}


/***********************************************************************************************
 * GScreenClass::One_Time -- Handles one time class setups.                                    *
 *                                                                                             *
 * This routine (and all those that overload it) must perform truly one-time initialization.   *
 * Such init's would normally be done in the constructor, but other aspects of the game may    *
 * not have been initialized at the time the constructors are called (such as the file system, *
 * the display, or other WWLIB subsystems), so many initializations should be deferred to the  *
 * One_Time init's.                                                                            *
 *                                                                                             *
 * Any variables set in this routine should be declared as static, so they won't be modified   *
 * by the load/save process.  Non-static variables will be over-written by a loaded game.      *
 *                                                                                             *
 * This function allocates the shadow buffer that is used for quick screen updates. If         *
 * there were any data files to load, they would be loaded at this time as well.               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Call this routine only ONCE at the beginning of the game.                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void GScreenClass::One_Time(void)
{
	/*
	**	Allocate the screen shadow page. This page is used to reduce access to the
	**	actual screen memory. It contains a duplicate of what the SEENPAGE is.
	*/
	Buttons = 0;
	ShadowPage = new GraphicBufferClass(320, 200);
	if (ShadowPage) {
		ShadowPage->Clear();
		HidPage.Clear();
	}
}


/***********************************************************************************************
 * GScreenClass::Init -- Init's the entire display hierarchy by calling all Init routines.     *
 *                                                                                             *
 * This routine shouldn't be overloaded.  It's the main map initialization routine, and will   *
 * perform a complete map initialization, from mixfiles to clearing the buffers.  Calling this *
 * routine results in calling every initialization routine in the entire map hierarchy.        *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      theater      theater to initialize to                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void GScreenClass::Init(TheaterType theater)
{
	Init_Clear();
	Init_IO();
	Init_Theater(theater);
}


/***********************************************************************************************
 * GScreenClass::Init_Clear -- Sets the map to a known state.                                  *
 *                                                                                             *
 * This routine (and those that overload it) clears any buffers and variables to a known       *
 * state.  It assumes that all buffers are allocated & valid.  The map should be displayable   *
 * after calling this function, and should draw basically an empty display.                    *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void GScreenClass::Init_Clear(void)
{
	/*
	** Clear the ShadowPage & HidPage to force a complete shadow blit.
	*/
	if (ShadowPage) {
		ShadowPage->Clear();
		HidPage.Clear();
	}

	IsToRedraw = true;
}


/***********************************************************************************************
 * GScreenClass::Init_Theater -- Performs theater-specific initializations.                    *
 *                                                                                             *
 * This routine (and those that overload it) performs any theater-specific initializations     *
 * needed.  This will include setting the palette, setting up remap tables, etc.  This routine *
 * only needs to be called when the theater has changed.                                       *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void GScreenClass::Init_Theater(TheaterType )
{
}


/***********************************************************************************************
 * GScreenClass::Init_IO -- Initializes the Button list ('Buttons').                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void GScreenClass::Init_IO(void)
{
	/*
	** Reset the button list.  This means that any other elements of the map that need
	** buttons must attach them after this routine is called!
	*/
	Buttons = 0;
}


/***********************************************************************************************
 * GScreenClass::Flag_To_Redraw -- Flags the display to be redrawn.                            *
 *                                                                                             *
 *    This function is used to flag the display system whether any rendering is needed. The    *
 *    parameter tells the system either to redraw EVERYTHING, or just that something somewhere *
 *    has changed and the individual Draw_It functions must be called. When a sub system       *
 *    determines that it needs to render something local to itself, it would call this routine *
 *    with a false parameter. If the entire screen gets trashed or needs to be rebuilt, then   *
 *    this routine will be called with a true parameter.                                       *
 *                                                                                             *
 * INPUT:   complete -- bool; Should the ENTIRE screen be redrawn?                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This doesn't actually draw the screen, it merely sets flags so that when the    *
 *             Render() function is called, the appropriate drawing steps will be performed.   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void GScreenClass::Flag_To_Redraw(bool complete)
{
	IsToUpdate = true;
	if (complete) {
		IsToRedraw = true;
	}
}


/***********************************************************************************************
 * GScreenClass::Input -- Fetches input and processes gadgets.                                 *
 *                                                                                             *
 *    This routine will fetch the keyboard/mouse input and dispatch this through the gadget    *
 *    system.                                                                                  *
 *                                                                                             *
 * INPUT:   key      -- Reference to the key code (for future examination).                    *
 *                                                                                             *
 *          x,y      -- Reference to mouse coordinates (for future examination).               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void GScreenClass::Input(KeyNumType & key, int & x, int & y)
{
	key = Keyboard->Check();

	x = Keyboard->Mouse_X();
	y = Keyboard->Mouse_Y();

	if (Buttons != NULL) {

		/*
		** If any buttons need redrawing, they will do so in the Input routine, and
		** they should draw themselves to the HidPage.  So, flag ourselves for a Blit
		** to show the newly drawn buttons.
		*/
		if (Buttons->Is_List_To_Redraw()) {
			Flag_To_Redraw(false);
		}

#ifdef WIN32
		GraphicViewPortClass * oldpage= Set_Logic_Page(HidPage);
#else
		GraphicBufferClass * oldpage= Set_Logic_Page(HidPage);
#endif

		key = Buttons->Input();

		Set_Logic_Page(oldpage);

	} else {

		if (key != 0) {
			key = Keyboard->Get();
		}
	}

	AI(key, x, y);
}


/***********************************************************************************************
 * GScreenClass::Add_A_Button -- Add a gadget to the game input system.                        *
 *                                                                                             *
 *    This will add a gadget to the game input system. The gadget will be processed in         *
 *    subsequent calls to the GScreenClass::Input() function.                                  *
 *                                                                                             *
 * INPUT:   gadget   -- Reference to the gadget that will be added to the input system.        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void GScreenClass::Add_A_Button(GadgetClass & gadget)
{
	/*
	**	If this gadget is already in the list, remove it before adding it in:
	**	- If 1st gadget in list, use Remove_A_Button to remove it, to reset the
	**	  value of 'Buttons' appropriately
	**	- Otherwise, just call the Remove function for that gadget to remove it
	**	  from any list it may be in
	*/
	if (Buttons == &gadget) {
		Remove_A_Button(gadget);
	} else {
		gadget.Remove();
	}

	/*
	**	Now add the gadget to our list:
	**	- If there are not buttons, start the list with this one
	**	- Otherwise, add it to the tail of the existing list
	*/
	if (Buttons) {
		gadget.Add_Tail(*Buttons);
	} else {
		Buttons = &gadget;
	}
}


/***********************************************************************************************
 * GScreenClass::Remove_A_Button -- Removes a gadget from the game input system.               *
 *                                                                                             *
 * INPUT:   gadget   -- Reference to the gadget that will be removed from the input system.    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   'gadget' MUST be already a part of 'Buttons', or the new value of 'Buttons'     *
 *               will be invalid!                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void GScreenClass::Remove_A_Button(GadgetClass & gadget)
{
	Buttons = gadget.Remove();
}


/***********************************************************************************************
 * GScreenClass::Render -- General drawing dispatcher an display update function.              *
 *                                                                                             *
 *    This routine should be called in the main game loop (once every game frame). It will     *
 *    call the Draw_It() function if necessary. All rendering is performed to the LogicPage    *
 *    which is set to the HIDPAGE. After rendering has been performed, the HIDPAGE is          *
 *    copied to the visible page.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This actually updates the graphic display. As a result it can take quite a      *
 *             while to perform.                                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void GScreenClass::Render(void)
{
	//This is unnessasary surely?	ST - 10/16/96 2:30PM
	//if (Buttons && Buttons->Is_List_To_Redraw()) {
	//	IsToRedraw = true;
	//}


	if (IsToUpdate || IsToRedraw) {
		BStart(BENCH_GSCREEN_RENDER);

#ifdef WIN32
		GraphicViewPortClass * oldpage= Set_Logic_Page(HidPage);
#else
		GraphicBufferClass * oldpage= Set_Logic_Page(HidPage);

		if (IsToRedraw) {
			Hide_Mouse();
			SeenPage.To_Buffer(0, 0, 320, 200, ShadowPage);
			Show_Mouse();
		}
#endif

		Draw_It(IsToRedraw);

		if (Buttons) Buttons->Draw_All(false);

#ifdef SCENARIO_EDITOR
		/*
		** Draw the Editor's buttons
		*/
		if (Debug_Map) {
			if (Buttons) {
				Buttons->Draw_All();
			}
		}
#endif
		/*
		** Draw the multiplayer message system to the Hidpage at this point.
		** This way, they'll Blit along with the rest of the map.
		*/
		if (Session.Messages.Num_Messages() > 0) {
			Session.Messages.Set_Width(
				Lepton_To_Cell(Map.TacLeptonWidth) * ICON_PIXEL_W);
		}
		Session.Messages.Draw();

		//Blit_Display(); // 5/19/20 SKY - Skip copying to scene page, we can get the data directly from hidden page
		IsToUpdate = false;
		IsToRedraw = false;

		BEnd(BENCH_GSCREEN_RENDER);
		Set_Logic_Page(oldpage);
	}
}


/***********************************************************************************************
 * GScreenClass::Blit_Display -- Redraw the display from the hidpage to the seenpage.          *
 *                                                                                             *
 *    This routine is used to copy the correct display from the HIDPAGE                        *
 *    to the SEENPAGE.                                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1994 JLB : Created.                                                                 *
 *   05/01/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
extern "C" {
	void ModeX_Blit (GraphicBufferClass * source);
}

void GScreenClass::Blit_Display(void)
{
	BStart(BENCH_BLIT_DISPLAY);
	#ifdef WIN32
		if (SeenBuff.Get_Width()!=320) {
			WWMouse->Draw_Mouse(&HidPage);
			HidPage.Blit(SeenBuff , 0 , 0 , 0 , 0 , HidPage.Get_Width() , HidPage.Get_Height() , (BOOL) FALSE );
			WWMouse->Erase_Mouse(&HidPage, FALSE);
		} else {
			//PG ModeX_Blit(&HiddenPage);
		}
	#else
		Shadow_Blit(0, 0, 320, 200, HidPage, SeenPage, ShadowPage->Get_Buffer());
	#endif
	BEnd(BENCH_BLIT_DISPLAY);
}


