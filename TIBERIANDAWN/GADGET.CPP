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

/* $Header:   F:\projects\c&c\vcs\code\gadget.cpv   2.18   16 Oct 1995 16:49:40   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : GADGET.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Maria del Mar McCready Legg                                  *
 *                                Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/03/95                                                     *
 *                                                                                             *
 *                  Last Update : July 8, 1995 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   GadgetClass::Action -- Base action for gadget.                                            *
 *   GadgetClass::Clear_Focus -- Clears the focus if this gadget has it.                       *
 *   GadgetClass::Delete_List -- Deletes all gadget objects in list.                           *
 *   GadgetClass::Disable -- Disables the gaget from input processing.                         *
 *   GadgetClass::Draw_All -- Forces all gadgets in list to be redrawn.                        *
 *   GadgetClass::Draw_Me -- Gadget redraw action (flag control).                              *
 *   GadgetClass::Enable -- Enables the gadget.                                                *
 *   GadgetClass::Extract_Gadget -- Sweeps through the gadget chain to find gadget specified.  *
 *   GadgetClass::Flag_To_Redraw -- Flags this gadget to be redrawn.                           *
 *   GadgetClass::GadgetClass -- Constructor for gadget object.                                *
 *   GadgetClass::GadgetClass -- Default constructor for a gadget class object.                *
 *   GadgetClass::Get_Next -- Returns a pointer to the next gadget in the chain.               *
 *   GadgetClass::Get_Prev -- Fetches a pointer to the previous gadget.                        *
 *   GadgetClass::Has_Focus -- Checks if this object currently has the keyboard focus.         *
 *   GadgetClass::Remove -- Removes the specified gagdet from the list.                        *
 *   GadgetClass::Set_Focus -- Sets focus to this gadget.                                      *
 *   GadgetClass::Sticky_Process -- Handles the sticky flag processing.                        *
 *   GadgetClass::~GadgetClass -- Destructor for gadget object.                                *
 *   GadgetClass::Is_List_To_Redraw -- tells if any gadget in the list needs redrawing         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include <filepcx.h>
#include <io.h>

/*
**	This records the current gadget the the gadget system is "stuck on". Such a
**	gadget will be processed to the exclusion of all others until the mouse button
**	is no longer pressed.
*/
GadgetClass * GadgetClass::StuckOn = 0;

/*
**	This is a copy of a pointer to the last list used by the gadget input system.
**	If a change of list is detected, then all gadgets are forced to be redrawn.
*/
GadgetClass * GadgetClass::LastList = 0;


/*
**	This points to the gadget that is intercepting all keyboard events.
*/
GadgetClass * GadgetClass::Focused = 0;


/***********************************************************************************************
 * GadgetClass::GadgetClass -- Constructor for gadget object.                                  *
 *                                                                                             *
 *    This is the normal constructor for gadget objects. A gadget object is only concerned     *
 *    with the region on the screen to considered "its own" as well as the flags that tell     *
 *    what mouse action should be recognized when the mouse is over this screen area.          *
 *                                                                                             *
 * INPUT:   x,y   -- Coordinates (in pixels) of the upper left hand corner of the region that  *
 *                   will be "owned" by this gadget.                                           *
 *                                                                                             *
 *          w,h   -- Width and height (in pixels) of this gadget's region.                     *
 *                                                                                             *
 *          flags -- The flags (mouse conditions) that will cause this gadget's action         *
 *                   function to be called.                                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/03/1995 MML : Created.                                                                 *
 *=============================================================================================*/
GadgetClass::GadgetClass(int x, int y, int w, int h, unsigned flags, int sticky)
{
	X = x;
	Y = y;
	Width = w;
	Height = h;
	Flags = flags;
	IsToRepaint = false;
	IsSticky = sticky;
	IsDisabled = false;

	if (IsSticky) {
		Flags |= LEFTPRESS|LEFTRELEASE;
	}
}


/***********************************************************************************************
 * GadgetClass::~GadgetClass -- Destructor for gadget object.                                  *
 *                                                                                             *
 *    This is the destructor for the gadget object. It will clear the focus from this gadget   *
 *    if this gadget currently has the focus.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
GadgetClass::~GadgetClass(void)
{
	if (Has_Focus()) {
		Clear_Focus();
	}
}


/***************************************************************************
 * GADGETCLASS::CLICKEDON -- If a mouse click is detected within gadget's  *
 *      area and the appropriate flag is set, then call Action().          *
 *                                                                         *
 * INPUT:      int key, int mousex, int mousey                             *
 *                                                                         *
 * OUTPUT:     true or false                                               *
 *                                                                         *
 * WARNINGS:   none.                                                       *
 *                                                                         *
 * HISTORY:    01/03/1995 MML : Created.                                   *
 *=========================================================================*/
int GadgetClass::Clicked_On(KeyNumType & key, unsigned flags, int mousex, int mousey)
{
	/*
	**	Set flags to match only those events that occur AND are being looked for. If
	**	the result is NULL, then we know that this button should be ignored.
	*/
	flags &= Flags;

	/*
	**	If keyboard input should be processed by this "gadget" and keyboard input is
	**	detected, then always call the action function. It is up to the action function
	**	in this case to either ignore the keyboard input or not.
	**
	**	For mouse actions, check to see if the mouse is in the region of the button
	**	before calling the associated action function. This is the typical action for
	**	buttons.
	*/
	if (this == StuckOn ||
		(flags & KEYBOARD) ||
		(flags && (mousex - X) < Width  && (mousey - Y) < Height)) {

		return(Action(flags, key));
	}
	return(false);
}


/***********************************************************************************************
 * GadgetClass::Enable -- Enables the gadget.                                                  *
 *                                                                                             *
 *    This function enables the gadget. An enabled gadget will be processed for input          *
 *    purposes.                                                                                *
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
void GadgetClass::Enable(void)
{
	IsDisabled = false;
	IsToRepaint = true;
	Clear_Focus();
}


/***********************************************************************************************
 * GadgetClass::Disable -- Disables the gaget from input processing.                           *
 *                                                                                             *
 *    This routine will disable the gadget. A disabled gadget might be rendered, but is        *
 *    ignored for input processing.                                                            *
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
void GadgetClass::Disable(void)
{
	IsDisabled = true;
	IsToRepaint = true;
	Clear_Focus();
}


/***********************************************************************************************
 * GadgetClass::Remove -- Removes the specified gagdet from the list.                          *
 *                                                                                             *
 *    Use this routine if an individual gadget needs to be removed from the list of gadgets.   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the specified gadget found and removed? A false indicates that the       *
 *                gadget wasn't in the list.                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
GadgetClass * GadgetClass::Remove(void)
{
	Clear_Focus();
	return(GadgetClass *)LinkClass::Remove();
}


/***********************************************************************************************
 * GadgetClass::Get_Next -- Returns a pointer to the next gadget in the chain.                 *
 *                                                                                             *
 *    This returns with the next gadget's pointer. It is identical to the base Get_Next()      *
 *    function, but returns a pointer to a GadgetClass object.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the next gadget in the list.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
GadgetClass * GadgetClass::Get_Next(void) const
{
	return(GadgetClass*)LinkClass::Get_Next();
}


/***********************************************************************************************
 * GadgetClass::Get_Prev -- Fetches a pointer to the previous gadget.                          *
 *                                                                                             *
 *    This routine will return the previous gadget in the list. It is identical to the base    *
 *    function Get_Prev, but returns a pointer to a GadgetClass object.                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the previous gadget in the list.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
GadgetClass * GadgetClass::Get_Prev(void) const
{
	return(GadgetClass*)LinkClass::Get_Prev();
}


/***********************************************************************************************
 * GadgetClass::Delete_List -- Deletes all gadget objects in list.                             *
 *                                                                                             *
 *    This function will delete all gadgets in the list. It is the counterpart to the          *
 *    Create_One_Of functions.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Any references to these gadget become invalidated by this routine.              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void GadgetClass::Delete_List(void)
{
	GadgetClass * g = this;

	/*
	**	Move to head of the list.
	*/
	while (g->Get_Prev()) {
		g = g->Get_Prev();
	}

	/*
	**	First delete all the gadgets following the first one. The reason the first one
	**	is kept around is that sometimes deleting one gadget will result in related gadgets
	**	in the same list also being deleted. The first gadget will always contain the
	**	correct gadget pointer.
	*/
	while (g) {
		g->Clear_Focus();

		GadgetClass * temp = g;
		g = g->Get_Next();
		delete temp;
	}
}


/***********************************************************************************************
 * GadgetClass::Action -- Base action for gadget.                                              *
 *                                                                                             *
 *    This handles the base level action that a gadget performs when a qualifying input event  *
 *    is detected. This sets the redraw flag and returns true (to stop further processing).    *
 *    If no qualifying input event was detected, but this routine was called anyway, then      *
 *    don't perform any action. The call to this routine, in that case, must have been forced  *
 *    for some other reason.                                                                   *
 *                                                                                             *
 * INPUT:   flag  -- The input event bits that qualify for this gadget. A NULL indicates that  *
 *                   no qualifying event occured.                                              *
 *                                                                                             *
 * OUTPUT:  bool; Should further gadget list processing be aborted?                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int GadgetClass::Action(unsigned flags, KeyNumType &)
{
	/*
	**	If any of the event flags are active, then this indicates that something probably
	**	has changed the gadget. Flag the gadget to be redrawn. Also, make sure that
	**	any sticky flags are cleared up.
	*/
	if (flags) {
		IsToRepaint = true;
		Sticky_Process(flags);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * GadgetClass::Draw_Me -- Gadget redraw action (flag control).                                *
 *                                                                                             *
 *    At this level, there is no actual rendering taking place with the call to Draw_Me, but   *
 *    the IsToRepaint flag must be cleared. Derived objects will call this routine and if it   *
 *    returns true, they will perform their custom rendering.                                  *
 *                                                                                             *
 * INPUT:   forced   -- Is this redraw forced by outside circumstances?                        *
 *                                                                                             *
 * OUTPUT:  bool; Should the gadget imagery be redrawn?                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/14/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int GadgetClass::Draw_Me(int forced)
{
	if (forced || IsToRepaint) {
		IsToRepaint = false;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * GadgetClass::Draw_All -- Forces all gadgets in list to be redrawn.                          *
 *                                                                                             *
 *    Use this function to cause all gadget in the list to be redrawn regardless of the state  *
 *    of the IsToRepaint flag.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/03/1995 MML : Created.                                                                 *
 *=============================================================================================*/
void GadgetClass::Draw_All(bool forced)
{
	GadgetClass *gadget = this;

	while (gadget != NULL) {
		gadget->Draw_Me(forced);
		gadget = gadget->Get_Next();
	}
}


/***************************************************************************
 * GADGETCLASS::PROCESSLIST -- Check list for a mouse click within a gadget*
 *                                                                         *
 * INPUT:      none.                                                       *
 *                                                                         *
 * OUTPUT:     key pressed.                                                *
 *                                                                         *
 * WARNINGS:   none.                                                       *
 *                                                                         *
 * HISTORY:    01/03/1995 MML : Created.                                   *
 *=========================================================================*/
KeyNumType GadgetClass::Input(void)
{
	int mousex, mousey;
	KeyNumType key;
	unsigned flags;
	int forced = false;

	/*
	**	Record this list so that a forced redraw only occurs the FIRST time the
	**	gadget list is passed to this routine.
	*/
	if (LastList != this) {
		LastList = this;
		forced = true;
		StuckOn = NULL;
		Focused = NULL;
	}

	/*
	**	Fetch any pending keyboard input.
	*/
	key = Keyboard::Check();
	if (key) {
		key = Keyboard::Get();
	}

#ifdef SCENARIO_EDITOR

	if ( key == KN_K ){
		/*
		** time to create a screen shot using the PCX code (if it works)
		*/
		GraphicBufferClass temp_page(	SeenBuff.Get_Width(),
												SeenBuff.Get_Height(),
												NULL,
												SeenBuff.Get_Width() * SeenBuff.Get_Height());
		char filename[30];

		SeenBuff.Blit(temp_page);
		for (int lp = 0; lp < 99; lp ++) {
			if (lp < 10) {
				sprintf(filename, "scrsht0%d.pcx", lp);
			} else {
				sprintf(filename, "scrsht%d.pcx", lp);
			}
			if (access(filename, F_OK) == -1)
				break;
		}

		Write_PCX_File(filename, temp_page, (unsigned char *)CurrentPalette);
		//Map.Place_Random_Crate();
	}

#endif	//SCENARIO_EDITOR

	/*
	**	For mouse button clicks, the mouse position is actually held in the MouseQ...
	**	globals rather than their normal Mouse... globals. This is because we need to
	**	know the position of the mouse at the exact instant when the click occured
	**	rather the the mouse position at the time we get around to this function.
	*/
	if (((key&0x10FF) == KN_LMOUSE) || ((key&0x10FF) == KN_RMOUSE)) {
	   mousex = _Kbd->MouseQX;
	   mousey = _Kbd->MouseQY;
	} else {
	   mousex = Get_Mouse_X();
	   mousey = Get_Mouse_Y();
	}

	/*
	**	Set the mouse button state flags. These will be passed to the individual
	**	buttons so that they can determine what action to perform (if any).
	*/
	flags = 0;
	if (key) {
		if (key == KN_LMOUSE) {
			flags |= LEFTPRESS;
		}
		if (key == KN_RMOUSE) {
			flags |= RIGHTPRESS;
		}
		if (key == (KN_LMOUSE | KN_RLSE_BIT)) {
			flags |= LEFTRELEASE;
		}
		if (key == (KN_RMOUSE | KN_RLSE_BIT)) {
			flags |= RIGHTRELEASE;
		}
	}

	/*
	**	If the mouse wasn't responsible for this key code, then it must be from
	**	the keyboard. Flag this fact.
	*/
	if (key && !flags) {
		flags |= KEYBOARD;
	}

	/*
	**	Mouse button up or down action is ignored if there is a keyboard event. This
	**	allows keyboard events to fall through normally even if the mouse is over a
	**	gadget that is flagged for LEFTUP or RIGHTUP.
	*/
	if (!key) {

		/*
		**	Check for the mouse being held down. We can't use the normal input system
		**	for this, so we must examine the actual current state of the mouse
		**	buttons. As a side note, if we determine that the mouse button isn't being
		**	held down, then we automatically know that it must be up -- set the flag
		**	accordingly.
		*/
		if (Keyboard::Down(KN_LMOUSE)) {
			flags |= LEFTHELD;
		} else {
			flags |= LEFTUP;
		}
		if (Keyboard::Down(KN_RMOUSE)) {
			flags |= RIGHTHELD;
		} else {
			flags |= RIGHTUP;
		}
	}

	/*
	**	If "sticky" processing is active, then only process the stuck gadget.
	*/
	if (StuckOn) {
		StuckOn->Draw_Me(false);
		StuckOn->Clicked_On(key, flags, mousex, mousey);
		if (StuckOn) {
			StuckOn->Draw_Me(false);
		}
	} else {

		/*
		**	If there is a gadget that has the keyboard focus, then route all keyboard
		**	events to it.
		*/
		if (Focused && (flags & KEYBOARD)) {
			Focused->Draw_Me(false);
			Focused->Clicked_On(key, flags, mousex, mousey);
			if (Focused) {
				Focused->Draw_Me(false);
			}
		} else {

			/*
			**	Sweep through all the buttons in the chain and pass the current button state
			**	and keyboard input data to them. These routines will detect whether they should
			**	perform some action and return a flag to this effect. They also have the option
			**	of changing the key value so that an appropriate return value is use for this
			**	processing routine.
			*/
			GadgetClass *next_button = this;
			while (next_button != NULL) {

				/*
				**	Maybe redraw the button if it needs to or is being forced to redraw.
				*/
				next_button->Draw_Me(forced);

				if (!next_button->IsDisabled) {

					/*
					**	Process this button. If the button was recognized and action was
					**	performed, then bail from further processing (speed reasons?).
					*/
					if (next_button->Clicked_On(key, flags, mousex, mousey)) {

						/*
						**	Some buttons will require repainting when they perform some action.
						**	Do so at this time.
						*/
						next_button->Draw_Me(false);
						break;
					}
				}

				next_button = next_button->Get_Next();
			}
		}
	}
	return(key);
}


/***********************************************************************************************
 * GadgetClass::Extract_Gadget -- Sweeps through the gadget chain to find gadget specified.    *
 *                                                                                             *
 *    This examines the gadget list looking for on that has the same ID as specified. If that  *
 *    gadget was found, then a pointer to it is returned. Since only ControlClass gadgets      *
 *    or ones derived from it can have an ID value, we know that the returned pointer is at    *
 *    least of the ControlClass type.                                                          *
 *                                                                                             *
 * INPUT:   id -- The ID number to scan for. Zero is not a legal ID number and if passed in,   *
 *                a NULL will always be returned.                                              *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the ControlClass gadget that has the matching ID number. *
 *          If no matching gadget was found, then NULL is returned.                            *
 *                                                                                             *
 * WARNINGS:   If there happens to be more than one gadget with a matching ID, this routine    *
 *             will return a pointer to the first one only.                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ControlClass * GadgetClass::Extract_Gadget(unsigned id)
{
	GadgetClass * g = this;

	if (id) {
		while (g) {
			if (g->Get_ID() == id) {
				return((ControlClass *)g);
			}
			g = g->Get_Next();
		}
	}
	return(0);
}


/***********************************************************************************************
 * GadgetClass::Flag_To_Redraw -- Flags this gadget to be redrawn.                             *
 *                                                                                             *
 *    Use this routine to flag the gadget to be redrawn. A gadget so flagged will have its     *
 *    Draw_Me function called at the next available opportunity. Usually, this is the next     *
 *    time the Input() function is called.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void GadgetClass::Flag_To_Redraw(void)
{
	IsToRepaint = true;
}


/***********************************************************************************************
 * GadgetClass::Sticky_Process -- Handles the sticky flag processing.                          *
 *                                                                                             *
 *    This function examines the event flags and handles any "sticky" processing required.     *
 *    Sticky processing is when the button is flagged with the "IsSticky" bit and it will      *
 *    be processed to the exclusion of all other gadgets while the mouse button is held        *
 *    down.                                                                                    *
 *                                                                                             *
 * INPUT:   flags -- The event flags that triggered the call to this routine.                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void GadgetClass::Sticky_Process(unsigned flags)
{
	if (IsSticky && (flags & LEFTPRESS)) {
		StuckOn = this;
	}
	if (StuckOn == this && (flags & LEFTRELEASE)) {
		StuckOn = 0;
	}
}


/***********************************************************************************************
 * GadgetClass::Set_Focus -- Sets focus to this gadget.                                        *
 *                                                                                             *
 *    This will set the focus to this gadget regardless of any current focus setting. If there *
 *    is another gadget that has focus, it will have its focus cleared before this gadget will *
 *    get the focus. A focused gadget is one that has all keyboard input routed to it.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void GadgetClass::Set_Focus(void)
{
	if (Focused) {
		Focused->Flag_To_Redraw();
		Focused->Clear_Focus();
	}
	Flags |= KEYBOARD;
	Focused = this;
}


/***********************************************************************************************
 * GadgetClass::Clear_Focus -- Clears the focus if this gadget has it.                         *
 *                                                                                             *
 *    Use this function to clear the focus for the gadget. If the gadget doesn't currently     *
 *    have focus, then this routine will do nothing. For added functionality, overload this    *
 *    virtual function so that gadget specific actions may be take when focus is lost.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void GadgetClass::Clear_Focus(void)
{
	if (Focused == this) {
		Flags &= ~KEYBOARD;
		Focused = 0;
	}
}


/***********************************************************************************************
 * GadgetClass::Has_Focus -- Checks if this object currently has the keyboard focus.           *
 *                                                                                             *
 *    If this object has the keyboard focus, then this routine will return true. When the      *
 *    gadget has keyboard focus, all keyboard events get routed to the gadget.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Does this gadget have the keyboard focus?                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool GadgetClass::Has_Focus(void)
{
	return(this == Focused);
}

/***********************************************************************************************
 * GadgetClass::Is_List_To_Redraw -- tells if any gadget in the list needs redrawing           *
 *                                                                                             *
 * This function is mostly for supporting HidPage drawing.  If it returns true, it means       *
 * the application needs to re-blit the HidPage forward, after calling the list's Input().     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  true = an item needs redrawing, false = no items need redrawing                    *
 *                                                                                             *
 * WARNINGS:   It is assumed 'this' is the head of the list.                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/03/1995 MML : Created.                                                                 *
 *=============================================================================================*/
int GadgetClass::Is_List_To_Redraw(void)
{
	GadgetClass *gadget = this;

	while (gadget != NULL) {
		if (gadget->IsToRepaint)
			return (true);
		gadget = gadget->Get_Next();
	}
	return (false);
}


