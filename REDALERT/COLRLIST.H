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

/* $Header: /CounterStrike/COLRLIST.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : COLRLIST.H                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/15/95                                                     *
 *                                                                                             *
 *                  Last Update : January 15, 1995 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef COLORLIST_H
#define COLORLIST_H

#include "list.h"


/***************************************************************************
** This class adds the ability for every list item to have a different color.
*/
class ColorListClass : public ListClass
{
	public:
		/*********************************************************************
		** These enums are the ways a selected item can be drawn
		*/
		//lint -esym(578,SELECT_NONE)
		typedef enum SelectEnum {
			SELECT_NORMAL,				// selected items aren't drawn differently
			SELECT_HIGHLIGHT,			// item is highlighted
			SELECT_BOX,					// draw a box around the item
			SELECT_BAR,					// draw a bar behind the item
			SELECT_INVERT				// draw the string inverted
		} SelectStyleType;

		ColorListClass(int id, int x, int y, int w, int h, TextPrintType flags, void const * up, void const * down);
		virtual ~ColorListClass(void);

		virtual int  Add_Item(char const * text, RemapControlType * color = NULL);
		virtual int  Add_Item(int text, RemapControlType * color = NULL);
		virtual void Remove_Item(char const * text);

		virtual void Set_Selected_Style(SelectStyleType style, RemapControlType * color = NULL);

		/*
		**	This is the list of colors for each item.
		*/
		DynamicVectorClass<RemapControlType *> Colors;

	protected:
		virtual void Draw_Entry(int index, int x, int y, int width, int selected);

		/*
		**	This tells how to draw the selected item.
		*/
		SelectStyleType Style;
		RemapControlType * SelectColor;

};

#endif
