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

/* $Header: /CounterStrike/CHEKLIST.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CHEKLIST.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/05/96                                                     *
 *                                                                                             *
 *                  Last Update : July 6, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CheckListClass::Action -- action function for this class                                  *
 *   CheckListClass::Add_Item -- Adds specifies text to check list box.                        *
 *   CheckListClass::CheckListClass -- constructor                                             *
 *   CheckListClass::Check_Item -- [un]checks an items                                         *
 *   CheckListClass::Draw_Entry -- draws a list box entry                                      *
 *   CheckListClass::Get_Item -- Fetches a pointer to the text associated with the index.      *
 *   CheckListClass::Remove_Item -- Remove the item that matches the text pointer specified.   *
 *   CheckListClass::Set_Selected_Index -- Set the selected index to match the text pointer spe*
 *   CheckListClass::~CheckListClass -- Destructor for check list object.                      *
 *   CheckListClass::~CheckListClass -- destructor                                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


/***************************************************************************
 * CheckListClass::CheckListClass -- constructor                           *
 *                                                                         *
 * INPUT:                                                                  *
 *      id         control ID for this list box                            *
 *      x         x-coord                                                  *
 *      y         y-coord                                                  *
 *      w         width                                                    *
 *      h         height                                                   *
 *      flags      mouse event flags                                       *
 *      up         ptr to Up-arrow shape                                   *
 *      down      ptr to Down-arrow shape                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   02/16/1995 BR : Created.                                              *
 *=========================================================================*/
CheckListClass::CheckListClass(int id, int x, int y, int w, int h, TextPrintType flags,
	void const * up, void const * down) :
	ListClass (id, x, y, w, h, flags, up, down),
	IsReadOnly(false)
{
}


/***********************************************************************************************
 * CheckListClass::~CheckListClass -- Destructor for check list object.                        *
 *                                                                                             *
 *    This destructor will delete all entries attached to it.                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
CheckListClass::~CheckListClass(void)
{
	while (CheckListClass::Count()) {
		CheckObject * obj = (CheckObject *)ListClass::Get_Item(0);

		ListClass::Remove_Item(0);
		delete obj;
	}
}


/***********************************************************************************************
 * CheckListClass::Add_Item -- Adds specifies text to check list box.                          *
 *                                                                                             *
 *    This routine will add the specified text string to the check list.                       *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text string to add to the list box.                        *
 *                                                                                             *
 * OUTPUT:  Returns the index number where the text object was added.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int CheckListClass::Add_Item(char const * text)
{
	CheckObject * obj = new CheckObject(text, false);
	return(ListClass::Add_Item((char const *)obj));
}


char const * CheckListClass::Current_Item(void) const
{
	CheckObject * obj = (CheckObject *)ListClass::Current_Item();
	if (obj) {
		return(obj->Text);
	}
	return(0);
}


/***********************************************************************************************
 * CheckListClass::Get_Item -- Fetches a pointer to the text associated with the index.        *
 *                                                                                             *
 *    This routine will find the text associated with the entry specified and return a pointer *
 *    to that text.                                                                            *
 *                                                                                             *
 * INPUT:   index -- The entry (index) to fetch a pointer to.                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the text pointer associated with the index specified.                 *
 *                                                                                             *
 * WARNINGS:   If the index is out of range, then NULL is returned.                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * CheckListClass::Get_Item(int index) const
{
	CheckObject * obj = (CheckObject *)ListClass::Get_Item(index);
	if (obj) {
		return(obj->Text);
	}
	return(0);
}


/***********************************************************************************************
 * CheckListClass::Remove_Item -- Remove the item that matches the text pointer specified.     *
 *                                                                                             *
 *    This routine will find the entry that matches the text pointer specified and then        *
 *    delete that entry.                                                                       *
 *                                                                                             *
 * INPUT:   text  -- The text pointer to use to find the exact match in the list.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void CheckListClass::Remove_Item(char const * text)
{
	for (int index = 0; index < Count(); index++) {
		CheckObject * obj = (CheckObject *)ListClass::Get_Item(index);
		if (obj && stricmp(obj->Text, text) == 0) {
			ListClass::Remove_Item(index);
			delete obj;
			break;
		}
	}
}


/***********************************************************************************************
 * CheckListClass::Set_Selected_Index -- Set the selected index to match the text pointer spec *
 *                                                                                             *
 *    This routine will find the entry that exactly matches the text pointer specified. If     *
 *    found, then that entry will be set as the currently selected index.                      *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text string to find the match for.                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   If an exact match to the specified text string could not be found, then the     *
 *             currently selected index is not changed.                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void CheckListClass::Set_Selected_Index(char const * text)
{
	for (int index = 0; index < Count(); index++) {
		CheckObject * obj = (CheckObject *)ListClass::Get_Item(index);
		if (obj && stricmp(obj->Text, text) == 0) {
			Set_Selected_Index(index);
			break;
		}
	}
}


/***************************************************************************
 * CheckListClass::Check_Item -- [un]checks an items                       *
 *                                                                         *
 * INPUT:                                                                  *
 *      index         index of item to check or uncheck                    *
 *      checked      0 = uncheck, non-zero = check                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   02/16/1995 BR : Created.                                              *
 *   02/14/1996 JLB : Revamped.                                            *
 *=========================================================================*/
void CheckListClass::Check_Item(int index, bool checked)
{
	CheckObject * obj = (CheckObject *)ListClass::Get_Item(index);
	if (obj && obj->IsChecked != checked) {
		obj->IsChecked = checked;
		Flag_To_Redraw();
	}
}


/***************************************************************************
 * CheckListClass::Is_Checked -- returns checked state of an item          *
 *                                                                         *
 * INPUT:                                                                  *
 *      index         index of item to query                               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = item is unchecked, 1 = item is checked                         *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   02/16/1995 BR : Created.                                              *
 *   02/14/1996 JLB : Revamped.                                            *
 *=========================================================================*/
bool CheckListClass::Is_Checked(int index) const
{
	CheckObject * obj = (CheckObject *)ListClass::Get_Item(index);
	if (obj) {
		return(obj->IsChecked);
	}
	return(false);
}


/***************************************************************************
 * CheckListClass::Action -- action function for this class                *
 *                                                                         *
 * INPUT:                                                                  *
 *      flags      the reason we're being called                           *
 *      key      the KN_number that was pressed                            *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = event was processed, false = event not processed            *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   02/16/1995 BR : Created.                                              *
 *=========================================================================*/
int CheckListClass::Action(unsigned flags, KeyNumType &key)
{
	int rc;

	/*
	** If this is a read-only list, it's a display-only device
	*/
	if (IsReadOnly) {
		return(false);
	}

	/*
	**	Invoke parents Action first, so it can set the SelectedIndex if needed.
	*/
	rc =  ListClass::Action(flags, key);

	/*
	**	Now, if this event was a left-press, toggle the checked state of the
	**	current item.
	*/
	if (flags & LEFTPRESS) {
		Check_Item(SelectedIndex, !Is_Checked(SelectedIndex));
	}

	return(rc);
}


/***************************************************************************
 * CheckListClass::Draw_Entry -- draws a list box entry                    *
 *                                                                         *
 * INPUT:                                                                  *
 *		index			index into List of item to draw                      		*
 *		x,y			x,y coords to draw at                                  	*
 *		width			maximum width allowed for text                       		*
 *		selected		true = this item is selected                         		*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   12/14/1995 BRR : Created.                                             *
 *=========================================================================*/
void CheckListClass::Draw_Entry(int index, int x, int y, int width, int selected)
{
	if (index >= Count()) return;

	CheckObject * obj = (CheckObject *)ListClass::Get_Item(index);

	if (obj) {
		char buffer[100] = "";

		if (obj->IsChecked) {
			buffer[0] = CHECK_CHAR;
		} else {
			buffer[0] = UNCHECK_CHAR;
		}
		buffer[1] = ' ';
		sprintf(&buffer[2], obj->Text);

		TextPrintType flags = TextFlags;
		RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

		if (selected) {
			flags = flags | TPF_BRIGHT_COLOR;
			LogicPage->Fill_Rect (x, y, x + width - 1, y + LineHeight - 1, scheme->Shadow);
		} else {
			if (!(flags & TPF_USE_GRAD_PAL)) {
				flags = flags | TPF_MEDIUM_COLOR;
			}
		}

		Conquer_Clip_Text_Print(buffer, x, y, scheme, TBLACK, flags, width, Tabs);
	}
}

