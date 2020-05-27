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

/* $Header:   F:\projects\c&c\vcs\code\colrlist.cpv   1.9   16 Oct 1995 16:50:02   JOE_BOSTIC  $ */
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
 *                  Last Update : April 19, 1995 [BRR]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   ColorListClass::Add_Item -- Adds an item to the list                                      *
 *   ColorListClass::ColorListClass -- Class constructor                                       *
 *   ColorListClass::Draw_Entry -- Draws one text line                                         *
 *   ColorListClass::Remove_Item -- Removes an item from the list                              *
 *   ColorListClass::Set_Selected_Style -- tells how to draw selected item                     *
 *   ColorListClass::~ColorListClass -- Class destructor                                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***************************************************************************
 * ColorListClass::ColorListClass -- class constructor                     *
 *                                                                         *
 * INPUT:                                                                  *
 *      id               button ID                                         *
 *      x,y            upper-left corner, in pixels                        *
 *      w,h            width, height, in pixels                            *
 *      list            ptr to array of char strings to list               *
 *    flags          flags for mouse, style of listbox                     *
 *      up,down         pointers to shapes for up/down buttons             *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:          01/05/1995 MML : Created.                             *
 *=========================================================================*/
ColorListClass::ColorListClass (int id, int x, int y, int w, int h,
	TextPrintType flags, void const * up, void const * down) :
	ListClass (id, x, y, w, h, flags, up, down)
{
	Style = SELECT_HIGHLIGHT;
	SelectColor = -1;
}


/***************************************************************************
 * ColorListClass::~ColorListClass -- Class destructor                     *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/19/1995 BRR : Created.                                             *
 *=========================================================================*/
ColorListClass::~ColorListClass(void)
{
	Colors.Clear();
}


/***************************************************************************
 * ColorListClass::Add_Item -- Adds an item to the list                    *
 *                                                                         *
 * INPUT:                                                                  *
 *      text      text to add to list                                      *
 *      color      color for item                                          *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      position of item in the list                                       *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/19/1995 BRR : Created.                                             *
 *=========================================================================*/
int ColorListClass::Add_Item(char const * text, char color)
{
	Colors.Add(color);
	return(ListClass::Add_Item(text));
}


/***************************************************************************
 * ColorListClass::Add_Item -- Adds an item to the list                    *
 *                                                                         *
 * INPUT:                                                                  *
 *      text      text to add to list                                      *
 *      color      color for item                                          *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      position of item in the list                                       *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/19/1995 BRR : Created.                                             *
 *=========================================================================*/
int ColorListClass::Add_Item(int text, char color)
{
	Colors.Add(color);
	return(ListClass::Add_Item(text));
}


/***************************************************************************
 * ColorListClass::Remove_Item -- Removes an item from the list            *
 *                                                                         *
 * INPUT:                                                                  *
 *      text      ptr to item to remove                                    *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/19/1995 BRR : Created.                                             *
 *=========================================================================*/
void ColorListClass::Remove_Item(char const * text)
{
	int index = List.ID(text);
	if (index != -1) {
		Colors.Delete(index);
		ListClass::Remove_Item(text);
	}
}


/***************************************************************************
 * ColorListClass::Set_Selected_Style -- tells how to draw selected item   *
 *                                                                         *
 * INPUT:                                                                  *
 *      style      style to draw                                           *
 *      color      color to draw the special style in; -1 = use item's color*
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/19/1995 BRR : Created.                                             *
 *=========================================================================*/
void ColorListClass::Set_Selected_Style(SelectStyleType style, int color)
{
	Style = style;
	SelectColor = color;
}


/***************************************************************************
 * ColorListClass::Draw_Entry -- Draws one text line                       *
 *                                                                         *
 * INPUT:                                                                  *
 *      index         index into List of item to draw                      *
 *      x,y         x,y coords to draw at                                  *
 *      width         maximum width allowed for text                       *
 *      selected      true = this item is selected                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/19/1995 BRR : Created.                                             *
 *=========================================================================*/
void ColorListClass::Draw_Entry(int index, int x, int y, int width, int selected)
{
	int color;

	/*
	** Draw a non-selected item in its color
	*/
	if (!selected) {
		Conquer_Clip_Text_Print(List[index], x, y, Colors[index], TBLACK, TextFlags, width, Tabs);
		return;
	}

	/*
	** For selected items, choose the right color & style:
	*/
	if (SelectColor==-1) {
		color = Colors[index];
	} else {
		color = SelectColor;
	}

	switch (Style) {
		/*
		**	NONE: Just print the string in its native color
		*/
		case SELECT_NONE:
			Conquer_Clip_Text_Print(List[index], x, y, Colors[index], TBLACK, TextFlags, width, Tabs);
			break;

		/*
		**	HIGHLIGHT: Draw the string in the highlight color (SelectColor must
		**	be set)
		*/
		case SELECT_HIGHLIGHT:
			if (TextFlags & TPF_6PT_GRAD) {
				Conquer_Clip_Text_Print(List[index], x, y, color, TBLACK, TextFlags | TPF_BRIGHT_COLOR, width, Tabs);
			} else {
				Conquer_Clip_Text_Print(List[index], x, y, color, TBLACK, TextFlags, width, Tabs);
			}
			break;

		/*
		**	BOX: Draw a box around the item in the current select color
		*/
		case SELECT_BOX:
			LogicPage->Draw_Rect (x, y, x + width - 2, y + LineHeight - 2, color);
			Conquer_Clip_Text_Print(List[index], x, y, Colors[index], TBLACK, TextFlags, width, Tabs);
			break;

		/*
		**	BAR: draw a color bar under the text
		*/
		case SELECT_BAR:
			if (TextFlags & TPF_6PT_GRAD) {
				LogicPage->Fill_Rect (x, y, x + width - 1, y + LineHeight - 1, SelectColor);
				Conquer_Clip_Text_Print(List[index], x, y, Colors[index], TBLACK, TextFlags | TPF_BRIGHT_COLOR, width, Tabs);
			} else {
				LogicPage->Fill_Rect (x, y, x + width - 2, y + LineHeight - 2, SelectColor);
				Conquer_Clip_Text_Print(List[index], x, y, Colors[index], TBLACK, TextFlags, width, Tabs);
			}
			break;

		/*
		**	INVERT: Draw text as the background color on foreground color
		*/
		case SELECT_INVERT:
			if (TextFlags & TPF_6PT_GRAD) {
				LogicPage->Fill_Rect (x, y, x + width - 1, y + LineHeight - 1, Colors[index]);
				Conquer_Clip_Text_Print(List[index], x, y, BLACK, TBLACK, TextFlags, width, Tabs);
			} else {
				LogicPage->Fill_Rect (x, y, x + width - 2, y + LineHeight - 2, Colors[index]);
				Conquer_Clip_Text_Print(List[index], x, y, LTGREY, TBLACK, TextFlags, width, Tabs);
			}
			break;

	}
}
