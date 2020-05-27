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

/* $Header:   F:\projects\c&c\vcs\code\list.cpv   2.17   16 Oct 1995 16:51:36   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : LIST.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/15/95                                                     *
 *                                                                                             *
 *                  Last Update : June 25, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   ListClass::Add -- This object adds itself to the given list                               *
 *   ListClass::Add_Head -- This gadget makes itself the head of the given list.               *
 *   ListClass::Add_Item -- Adds a text item (as number) to the list box.                      *
 *   ListClass::Add_Item -- Adds an item to the list box.                                      *
 *   ListClass::Add_Scroll_Bar -- Adds a scroll bar to the list box.                           *
 *   ListClass::Add_Tail -- Add myself to the end of the given list.                           *
 *   ListClass::Bump -- Bumps the list box up/down one "page".                                 *
 *   ListClass::Current_Index -- Fetches the current selected index.                           *
 *   ListClass::Current_Item -- Fetches pointer to current item string.                        *
 *   ListClass::Draw_Entry -- Draws a list box text line as indicated.                         *
 *   ListClass::Draw_Me -- Draws the listbox.                                                  *
 *   ListClass::Get_Item -- Fetches an arbitrary item string.                                  *
 *   ListClass::Peer_To_Peer -- A peer gadget was touched -- make adjustments.                 *
 *   ListClass::Remove -- Removes the specified object from the list.                          *
 *   ListClass::Remove_Item -- Remove specified text from list box.                            *
 *   ListClass::Remove_Scroll_Bar -- Removes the scroll bar if present                         *
 *   ListClass::Set_Selected_Index -- Set the top of the listbox to index specified.           *
 *   ListClass::Set_Tabs -- Sets the tab stop list to be used for text printing.               *
 *   ListClass::Set_View_Index -- Sets the top line for the current list view.                 *
 *   ListClass::Step -- Moves the list view one line in direction specified.                   *
 *   ListClass::Step_Selected_Index -- Change the listbox top line in direction specified.     *
 *   ListClass::~ListClass -- Destructor for list class objects.                               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***************************************************************************
 * ListClass::ListClass -- class constructor                               *
 *                                                                         *
 * INPUT:            id               button ID                            *
 *                                                                         *
 *                     x,y            upper-left corner, in pixels         *
 *                                                                         *
 *                     w,h            width, height, in pixels             *
 *                                                                         *
 *                     list            ptr to array of char strings to list*
 *                                                                         *
 *                   flags, style   flags for mouse, style of listbox      *
 *                                                                         *
 * OUTPUT:           none.                                                 *
 *                                                                         *
 * WARNINGS:         none.                                                 *
 *                                                                         *
 * HISTORY:          01/05/1995 MML : Created.                             *
 *=========================================================================*/
ListClass::ListClass (int id, int x, int y, int w, int h, TextPrintType flags, void const * up, void const * down) :
	ControlClass(id, x, y, w, h, LEFTPRESS | LEFTRELEASE | KEYBOARD, false),
	UpGadget(0, up, x+w, y),
	DownGadget(0, down, x+w, y+h),
	ScrollGadget(0, x+w, y, 0, h, true)
{
	/*
	**	Set preliminary values for the slider related gadgets. They don't automatically
	**	appear at this time, but there are some values that can be pre-filled in.
	*/
	UpGadget.X -= UpGadget.Width;
	DownGadget.X -= DownGadget.Width;
	DownGadget.Y -= DownGadget.Height;
	ScrollGadget.X -= MAX(UpGadget.Width, DownGadget.Width);
	ScrollGadget.Y = Y+UpGadget.Height;
	ScrollGadget.Height -= UpGadget.Height + DownGadget.Height;
	ScrollGadget.Width = MAX(UpGadget.Width, DownGadget.Width);

	/*
	**	Set the list box to a default state.
	*/
	TextFlags = flags;
	IsScrollActive = false;
	Tabs = 0;
	SelectedIndex = 0;
	CurrentTopIndex = 0;
	Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, TextFlags);
	LineHeight = FontHeight+FontYSpacing-1;
	LineCount = (h-1) / LineHeight;
}


/***********************************************************************************************
 * ListClass::~ListClass -- Destructor for list class objects.                                 *
 *                                                                                             *
 *    This is the destructor for list objects. It handles removing anything it might have      *
 *    allocated. This is typically the scroll bar.                                             *
 *                                                                                             *
 * INPUT:      none                                                                            *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/16/1995 JLB : Created.                                                       *
 *=============================================================================================*/
ListClass::~ListClass(void)
{
	Remove_Scroll_Bar();
}


/***********************************************************************************************
 * ListClass::Add_Item -- Adds an item to the list box.                                        *
 *                                                                                             *
 *    This will add the specified string to the list box. The string is added to the end       *
 *    of the list.                                                                             *
 *                                                                                             *
 * INPUT:      text  -- Pointer to the string to add to the list box.                          *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
int ListClass::Add_Item(char const * text)
{
	if (text) {
		List.Add(text);
		Flag_To_Redraw();

		/*
		**	Add scroll gadget if the list gets too large to display all of the items
		**	at the same time.
		*/
		if (List.Count() > LineCount) {
			Add_Scroll_Bar();
		}

		/*
		**	Tell the slider that there is one more entry in the list.
		*/
		if (IsScrollActive) {
			ScrollGadget.Set_Maximum(List.Count());
		}
	}
	return(List.Count() - 1);
}


/***********************************************************************************************
 * ListClass::Add_Item -- Adds a text item (as number) to the list box.                        *
 *                                                                                             *
 *    This will add the text as specified by the text number provided, to the list box.        *
 *    The string is added to the end of the list.                                              *
 *                                                                                             *
 * INPUT:      text  -- The text number for the string to add to the list box.                 *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   Once a string is added to the list box in this fashion, there is no method of   *
 *             retrieving the text number as it relates to any particular index in the list.   *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ListClass::Add_Item(int text)
{
	if (text != TXT_NONE) {
		Add_Item(Text_String(text));
	}
	return(List.Count() - 1);
}


/***********************************************************************************************
 * ListClass::Remove_Item -- Remove specified text from list box.                              *
 *                                                                                             *
 *    This routine will remove the specified text string from the list box.                    *
 *                                                                                             *
 * INPUT:      text  -- Pointer to the string to remove.                                       *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   The text pointer passed into this routine MUST be the same text pointer that    *
 *             was used to add the string to the list.                                         *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ListClass::Remove_Item(char const * text)
{
	if (text) {
		List.Delete(text);

		/*
		**	If the list is now small enough to display completely within the list box region,
		**	then delete the slider gadget (if they are present).
		*/
		if (List.Count() <= LineCount) {
			Remove_Scroll_Bar();
		}

		/*
		**	Tell the slider that there is one less entry in the list.
		*/
		if (IsScrollActive) {
			ScrollGadget.Set_Maximum(List.Count());
		}

		/*
		** If we just removed the selected entry, select the previous one
		*/
		if (SelectedIndex >= List.Count()) {
			SelectedIndex--;
			if (SelectedIndex < 0)
				SelectedIndex = 0;
		}

		/*
		** If we just removed the top-displayed entry, step up one item
		*/
		if (CurrentTopIndex >= List.Count()) {
			CurrentTopIndex--;
			if (CurrentTopIndex < 0)
				CurrentTopIndex = 0;
			if (IsScrollActive)
				ScrollGadget.Step(1);
		}
	}
}


/***************************************************************************
 * ListClass::Action -- If clicked on, do this!                            *
 *                                                                         *
 * INPUT:            int flags -- combination of mouse flags indicating    *
 *                                  what action to take.                   *
 *                                                                         *
 * OUTPUT:           bool result.                                          *
 *                                                                         *
 * WARNINGS:         none.                                                 *
 *                                                                         *
 * HISTORY:          01/05/1995 MML : Created.                             *
 *=========================================================================*/
int ListClass::Action(unsigned flags, KeyNumType & key)
{
	if (flags & LEFTRELEASE) {
		key = KN_NONE;
		flags &= (~LEFTRELEASE);
		ControlClass::Action(flags, key);
		return(true);
	} else {

		/*	--------------------------------------------------
		**			Handle keyboard events here.
		*/
		if (flags & KEYBOARD) {

			/*
			**	Process the keyboard character. If indicated, consume this keyboard event
			**	so that the edit gadget ID number is not returned.
			*/
			if (key == KN_UP) {
				Step_Selected_Index(-1);
				key = KN_NONE;
			} else if (key == KN_DOWN) {
				Step_Selected_Index(1);
				key = KN_NONE;
			} else {
				flags &= ~KEYBOARD;
			}

		} else {

			int index = Get_Mouse_Y() - (Y+1);
			index = index / LineHeight;
			SelectedIndex = CurrentTopIndex + index;
			SelectedIndex = MIN(SelectedIndex, List.Count()-1);
		}
	}
	return(ControlClass::Action(flags, key));
}


/***********************************************************************************************
 * ListClass::Draw_Me -- Draws the listbox.                                                    *
 *                                                                                             *
 *    This routine will render the listbox.                                                    *
 *                                                                                             *
 * INPUT:   forced   -- Should the listbox be redrawn even if it already thinks it doesn't     *
 *                      need to be? This is true when something outside of the gadget system   *
 *                      has trashed the screen.                                                *
 *                                                                                             *
 * OUTPUT:  Was the listbox redrawn?                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ListClass::Draw_Me(int forced)
{
	if (GadgetClass::Draw_Me(forced)) {

		/*
		**	Turn off the mouse.
		*/
		if (LogicPage == &SeenBuff) {
			Conditional_Hide_Mouse(X, Y, X+Width, Y+Height);
		}

		Draw_Box (X, Y, Width, Height, BOXSTYLE_GREEN_BOX, true);

		/*
		**	Draw List.
		*/
		if (List.Count()) {
			for (int index = 0; index < LineCount; index++)  {
				int line = CurrentTopIndex + index;

				if (List.Count() > line) {

					/*
					**	Prints the text and handles right edge clipping and tabs.
					*/
					Draw_Entry(line, X+1, Y+(LineHeight*index)+1, Width-2, (line == SelectedIndex));
				}
			}
		}

		/*
		**	Turn on the mouse.
		*/
		if (LogicPage == &SeenBuff) {
			Conditional_Show_Mouse();
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * ListClass::Bump -- Bumps the list box up/down one "page".                                   *
 *                                                                                             *
 *    Use this routine to adjust the "page" that is being viewed in the list box. The view     *
 *    will move up or down (as specified) one page (screen full) of text strings.              *
 *                                                                                             *
 * INPUT:   up -- Should the adjustment be up?                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ListClass::Bump(int up)
{
	if (IsScrollActive) {
		if (ScrollGadget.Step(up)) {
			CurrentTopIndex = ScrollGadget.Get_Value();
			Flag_To_Redraw();
		}
	}
}


/***********************************************************************************************
 * ListClass::Step -- Moves the list view one line in direction specified.                     *
 *                                                                                             *
 *    This routine will move the current view "page" one line in the direction specified.      *
 *                                                                                             *
 * INPUT:   up -- Should the view be moved upward?                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ListClass::Step(int up)
{
	if (IsScrollActive) {
		if (ScrollGadget.Step(up)) {
			CurrentTopIndex = ScrollGadget.Get_Value();
			Flag_To_Redraw();
		}
	}
}


/***********************************************************************************************
 * ListClass::Get_Item -- Fetches an arbitrary item string.                                    *
 *                                                                                             *
 *    This routine will fetch an item string from the list box. The item fetched can be any    *
 *    one of the ones in the list.                                                             *
 *                                                                                             *
 * INPUT:   index -- The index to examine and return the text pointer from.                    *
 *                                                                                             *
 * OUTPUT:  Returns with the text pointer to the string at the index position specified.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * ListClass::Get_Item(int index) const
{
	if (List.Count() == 0) return (NULL);

	index = MIN(index, List.Count()-1 );
	return(List[index]);
}


/***********************************************************************************************
 * ListClass::Current_Item -- Fetches pointer to current item string.                          *
 *                                                                                             *
 *    This routine will fetch a pointer to the currently selected item's text.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Return with pointer to currently selected text.                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * ListClass::Current_Item(void)
{
	return(List[SelectedIndex]);
}


/***********************************************************************************************
 * ListClass::Current_Index -- Fetches the current selected index.                             *
 *                                                                                             *
 *    This routine will fetch the index number for the currently selected line.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the index of the currently selected line. This ranges from zero to    *
 *          the number of items in the list minus one.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ListClass::Current_Index(void)
{
	return(SelectedIndex);
}


/***********************************************************************************************
 * ListClass::Peer_To_Peer -- A peer gadget was touched -- make adjustments.                   *
 *                                                                                             *
 *    This routine is called when one of the peer gadgets (the scroll arrows or the slider)    *
 *    was touched in some fashion. This routine will sort out whom and why and then make       *
 *    any necessary adjustments to the list box.                                               *
 *                                                                                             *
 * INPUT:   flags    -- The event flags that affected the peer gadget.                         *
 *                                                                                             *
 *          key      -- The key value at the time of the event.                                *
 *                                                                                             *
 *          whom     -- Which gadget is being touched.                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ListClass::Peer_To_Peer(unsigned flags, KeyNumType &, ControlClass & whom)
{
	if (flags & LEFTRELEASE) {
		if (&whom == &UpGadget) {
			Step(true);
		}
		if (&whom == &DownGadget) {
			Step(false);
		}
	}

	/*
	**	The slider has changed, so reflect the current list position
	**	according to the slider setting.
	*/
	if (&whom == &ScrollGadget) {
		Set_View_Index(ScrollGadget.Get_Value());
	}
}


/***********************************************************************************************
 * ListClass::Set_View_Index -- Sets the top line for the current list view.                   *
 *                                                                                             *
 *    This routine is used to set the line that will be at the top of the list view. This is   *
 *    how the view can be scrolled up and down. This does not affect the currently selected    *
 *    item.                                                                                    *
 *                                                                                             *
 * INPUT:   index -- The line (index) to move to the top of the list view.                     *
 *                                                                                             *
 * OUTPUT:  bool; Was the view actually changed?                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ListClass::Set_View_Index(int index)
{
	index = Bound(index, 0, List.Count() - LineCount);
	if (index != CurrentTopIndex) {
		CurrentTopIndex = index;
		Flag_To_Redraw();
		if (IsScrollActive) {
			ScrollGadget.Set_Value(CurrentTopIndex);
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * ListClass::Add_Scroll_Bar -- Adds a scroll bar to the list box.                             *
 *                                                                                             *
 *    This routine will add a scroll bar (with matching arrows) to the list box. They are      *
 *    added to the right edge and cause the interior of the list box to become narrower.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the scroll bar added?                                                    *
 *                                                                                             *
 * WARNINGS:   The list box becomes narrower when the scroll bar is added.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ListClass::Add_Scroll_Bar(void)
{
	if (!IsScrollActive) {
		IsScrollActive = true;

		/*
		**	Everything has been created successfully. Flag the list box to be
		**	redrawn because it now must be made narrower to accomodate the new
		**	slider gadgets.
		*/
		Flag_To_Redraw();
		Width -= ScrollGadget.Width;

		/*
		**	Tell the newly created gadgets that they should inform this list box
		**	whenever they get touched. In this way, the list box will automatically
		**	be updated under control of the slider buttons.
		*/
		UpGadget.Make_Peer(*this);
		DownGadget.Make_Peer(*this);
		ScrollGadget.Make_Peer(*this);

		/*
		**	Add these newly created gadgets to the same gadget list that the
		**	list box is part of.
		*/
		UpGadget.Add(*this);
		DownGadget.Add(*this);
		ScrollGadget.Add(*this);

		/*
		**	Make sure these added gadgets get redrawn at the next opportunity.
		*/
		UpGadget.Flag_To_Redraw();
		DownGadget.Flag_To_Redraw();
		ScrollGadget.Flag_To_Redraw();

		/*
		**	Inform the slider of the size of the window and the current view position.
		*/
		ScrollGadget.Set_Maximum(List.Count());
		ScrollGadget.Set_Thumb_Size(LineCount);
		ScrollGadget.Set_Value(CurrentTopIndex);

		/*
		**	Return with success flag.
		*/
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * ListClass::Remove_Scroll_Bar -- Removes the scroll bar if present                           *
 *                                                                                             *
 *    Use this routine to remove any attached scroll bar to this list box. If the scroll bar   *
 *    is not present, then no action occurs.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the scroll bar removed?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int ListClass::Remove_Scroll_Bar(void)
{
	if (IsScrollActive) {
		IsScrollActive = false;
		Width += ScrollGadget.Width;
		ScrollGadget.Remove();
		UpGadget.Remove();
		DownGadget.Remove();
		Flag_To_Redraw();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * ListClass::Set_Tabs -- Sets the tab stop list to be used for text printing.                 *
 *                                                                                             *
 *    This sets the tab stop list to be used for text printing. It specifies a series of       *
 *    pixel offsets for each tab stop. The offsets are from the starting pixel position that   *
 *    the text begins at.                                                                      *
 *                                                                                             *
 * INPUT:   tabs  -- Pointer to a list of tab pixel offsets.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only a pointer to the tabs is recorded by the ListClass object. Make sure that  *
 *             the list remains intact for the duration of the existence of this object.       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ListClass::Set_Tabs(int const * tabs)
{
	Tabs = tabs;
}


/***********************************************************************************************
 * ListClass::Draw_Entry -- Draws a list box text line as indicated.                           *
 *                                                                                             *
 *    This routine is called by the Draw_Me function when it desired to redraw a particular    *
 *    text line in the list box.                                                               *
 *                                                                                             *
 * INPUT:   index    -- The index of the list entry to draw. This index is based on the        *
 *                      total list and NOT the current visible view page.                      *
 *                                                                                             *
 *          x,y      -- Pixel coordinates for the upper left corner of the text entry.         *
 *                                                                                             *
 *          width    -- The maximum width that the text may draw over. It is expected that     *
 *                      this drawing routine entirely fills this length.                       *
 *                                                                                             *
 *          selected -- bool; Is this a selected (highlighted) listbox entry?                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 * WARNINGS:   none                                                                            *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void ListClass::Draw_Entry(int index, int x, int y, int width, int selected)
{
	if (TextFlags & TPF_6PT_GRAD) {
		TextPrintType flags = TextFlags;

		if (selected) {
			flags = flags | TPF_BRIGHT_COLOR;
			LogicPage->Fill_Rect (x, y, x + width - 1, y + LineHeight - 1, CC_GREEN_SHADOW);
		} else {
			if (!(flags & TPF_USE_GRAD_PAL)) {
				flags = flags | TPF_MEDIUM_COLOR;
			}
		}

		Conquer_Clip_Text_Print(List[index], x, y, CC_GREEN, TBLACK, flags, width, Tabs);

	} else {
		Conquer_Clip_Text_Print(List[index], x, y, (selected ? BLUE : WHITE), TBLACK, TextFlags, width, Tabs);
	}
}


/***********************************************************************************************
 * ListClass::Add -- Adds myself to list immediately after given object                        *
 *                                                                                             *
 * Adds the list box to the chain, immemdiately after the given object.  The order will be:    *
 * - Listbox                                                                                   *
 * - Up arrow (if active)                                                                      *
 * - Down arrow (if active)                                                                    *
 * - Scroll gadget (if active)                                                                 *
 *                                                                                             *                                                                                       *
 * INPUT:   object   -- Pointer to the object to be added right after this one.                *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the head of the list.                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass & ListClass::Add(LinkClass & list)
{
	/*
	**	Add the scroll bar gadgets if they're active.
	*/
	if (IsScrollActive) {
		ScrollGadget.Add(list);
		DownGadget.Add(list);
		UpGadget.Add(list);
	}

	/*
	**	Add myself to the list, then return.
	*/
	return(ControlClass::Add(list));
}	


/*********************************************************************************************** 
 * ListClass::Add_Head -- Adds myself to head of the given list                                *
 *                                                                                             * 
 * INPUT:   list -- list to add myself to                                                      *
 *                                                                                             * 
 * OUTPUT:  Returns with a reference to the object at the head of the list. This should be     * 
 *          the same object that is passed in.                                                 * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/19/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass & ListClass::Add_Head(LinkClass & list) 
{
	/*
	**	Add the scroll bar gadgets if they're active.
	*/
	if (IsScrollActive) {
		ScrollGadget.Add_Head(list);
		DownGadget.Add_Head(list);
		UpGadget.Add_Head(list);
	}

	/*
	**	Add myself to the list, then return.
	*/
	return(ControlClass::Add_Head(list));
}


/*********************************************************************************************** 
 * ListClass::Add_Tail -- Adds myself to tail of given list                                    *
 *                                                                                             * 
 * Adds the list box to the tail of the give chain.  The order will be:                        *
 * - Listbox                                                                                   *
 * - Up arrow (if active)                                                                      *
 * - Down arrow (if active)                                                                    *
 * - Scroll gadget (if active)                                                                 *
 *                                                                                             * 
 * INPUT:   list -- list to add myself to                                                      *
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   The previous and next pointers for the added object MUST have been properly     * 
 *             initialized for this routine to work correctly.                                 * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/15/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
LinkClass & ListClass::Add_Tail(LinkClass & list) 
{
	/*
	**	Add myself to the list.
	*/
	ControlClass::Add_Tail(list);

	/*
	**	Add the scroll bar gadgets if they're active.
	*/
	if (IsScrollActive) {
		UpGadget.Add_Tail(list);
		DownGadget.Add_Tail(list);
		ScrollGadget.Add_Tail(list);
	}

	return(Head_Of_List());
}


/*********************************************************************************************** 
 * ListClass::Remove -- Removes the specified object from the list.                            * 
 *                                                                                             * 
 *    This routine will remove the specified object from the list of objects. Because of the   * 
 *    previous and next pointers, it is possible to remove an object from the list without     * 
 *    knowing the head of the list. To do this, just call Remove() with the parameter of       * 
 *    "this".                                                                                  * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Returns with the new head of list.                                                 * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/15/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
GadgetClass * ListClass::Remove(void) 
{
	/*
	**	Remove the scroll bar if it's active
	*/
	if (IsScrollActive) {
		ScrollGadget.Remove();
		DownGadget.Remove();
		UpGadget.Remove();
	}

	/*
	**	Remove myself & return
	*/
	return(ControlClass::Remove());
}


/*********************************************************************************************** 
 * ListClass::Set_Selected_Index -- Set the top of the listbox to index specified.             * 
 *                                                                                             * 
 *    This routine will set the top line of the listbox to the index value specified.          * 
 *                                                                                             * 
 * INPUT:   index -- The index to set the top of the listbox to.                               * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   The requested index may be adjusted to fit within legal parameters.             * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/25/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
void ListClass::Set_Selected_Index(int index)
{ 
   if (index < List.Count()) {
		SelectedIndex = index; 
		Flag_To_Redraw();
		if (SelectedIndex < CurrentTopIndex) {
			Set_View_Index(SelectedIndex);
		}
		if (SelectedIndex >= CurrentTopIndex+LineCount) {
			Set_View_Index(SelectedIndex-(LineCount-1));
		}
   }
}


/*********************************************************************************************** 
 * ListClass::Step_Selected_Index -- Change the listbox top line in direction specified.       * 
 *                                                                                             * 
 *    This routine will scroll the top line of the listbox in the direction specified.         * 
 *                                                                                             * 
 * INPUT:   step  -- The direction (and amount) to adjust the listbox. If negative value, then * 
 *                   the top line is scrolled upward.                                          * 
 *                                                                                             * 
 * OUTPUT:  Returns with the original top line index number.                                   * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/25/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
int ListClass::Step_Selected_Index(int step)
{
	int old = SelectedIndex;

	Set_Selected_Index(old + step);
	return(old);
}	
