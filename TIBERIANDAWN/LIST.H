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

/* $Header:   F:\projects\c&c\vcs\code\list.h_v   2.17   16 Oct 1995 16:46:24   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : LIST.H                                                       * 
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

#ifndef LIST_H
#define LIST_H

#include "control.h"
#include "shapebtn.h"
#include "slider.h"


/***************************************************************************
 * ListClass -- Like a Windows ListBox structure									*
 *                                                                         *
 * INPUT:      int x -- x position of gadget											*
 *					int y -- y position of gadget											*
 *					int w -- width of gadget												*							
 *					int h -- height of gadget												*							
 *					UWORD flags -- see enumeration choices								*							
 *                                                                         *
 * OUTPUT:     none.																			*
 * WARNINGS:														   						*
 * HISTORY:    01/03/1995 MML : Created.                                   *
 *=========================================================================*/
class ListClass : public ControlClass
{
	public:
		ListClass(int id, int x, int y, int w, int h, TextPrintType flags, void const * up, void const * down);
		virtual ~ListClass(void);

//		static ListClass * Create_One_Of(int id, int x, int y, int w, int h, TextPrintType flags, void const * up, void const * down);
		virtual int  Add_Item(char const * text);
		virtual int  Add_Item(int text);
		virtual int  Add_Scroll_Bar(void);
		virtual void Bump(int up);
		virtual int  Count(void) {return List.Count();};
		virtual int  Current_Index(void);
		virtual char const * Current_Item(void);
		virtual int  Draw_Me(int forced);
		virtual char const * Get_Item(int index) const;
		virtual int Step_Selected_Index(int forward);

		virtual void Peer_To_Peer(unsigned flags, KeyNumType & key, ControlClass & whom);
		virtual void Remove_Item(char const * text);
		virtual int  Remove_Scroll_Bar(void);
		virtual void Set_Selected_Index(int index);  
		virtual void Set_Tabs(int const * tabs);
		virtual int  Set_View_Index(int index);
		virtual void Step(int up);

		/*
		** These overloaded list routines handle adding/removing the scroll bar
		** automatically when the list box is added or removed.
		*/
		virtual LinkClass & Add(LinkClass & object);
		virtual LinkClass & Add_Tail(LinkClass & object);
		virtual LinkClass & Add_Head(LinkClass & object);
		virtual GadgetClass * Remove(void);

	protected:
		virtual int  Action(unsigned flags, KeyNumType &key);
		virtual void Draw_Entry(int index, int x, int y, int width, int selected);

		/*
		**	This controls what the text looks like. It uses the basic TPF_ flags that
		**	are used to control Fancy_Text_Print().
		*/
		TextPrintType TextFlags;

		/*
		**	This is a series of tabstop pixel positions to use when processing any
		**	<TAB> characters found in a list box string. The tabs are a series of
		**	pixel offsets from the starting pixel position of the text.
		*/
		int const *Tabs;

		/*
		**	The actual list of text pointers is maintained by this list manager. The pointers
		**	are stored in EMS. The text that is pointed to may also be in EMS.
		*/
		DynamicVectorClass<char const *> List;
		//EMSListOf<char const *> List;

		/*
		**	This is the total pixel height of a standar line of text. This is greatly 
		**	influenced by the TextFlags value.
		*/
		int LineHeight;

		/*
		**	This is the number of text lines that can fit within the list box.
		*/
		int LineCount;

		/*
		**	If the slider bar has been created, these point to the respective gadgets
		**	that it is composed of.
		*/
		unsigned IsScrollActive:1;
		ShapeButtonClass UpGadget;
		ShapeButtonClass DownGadget;
		SliderClass ScrollGadget;

		/*
		**	This is the currently selected index. It is highlighted.
		*/
		int SelectedIndex;

		/*
		**	This specifies the line (index) that is at the top of the list box.
		*/
		int CurrentTopIndex;
};

#endif
