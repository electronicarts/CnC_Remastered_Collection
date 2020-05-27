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

/* $Header: /CounterStrike/CHEKLIST.H 1     3/03/97 10:24a Joe_bostic $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : CHEKLIST.H                               *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : February 16, 1995                        *
 *                                                                         *
 *                  Last Update : February 16, 1995   [BR]                 *
 *                                                                         *
 *-------------------------------------------------------------------------*
 *	This class behaves just like the standard list box, except that if the	*
 * first character of a list entry is a space, clicking on it toggles the	*
 * space with a check-mark ('\3').  This makes each entry in the list box	*
 * "toggle-able".																				*
 *-------------------------------------------------------------------------*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CHEKLIST_H
#define CHEKLIST_H

#include "list.h"

class CheckObject
{
	public:
		CheckObject(char const * text = 0, bool checked=false) :
			Text(text),
			IsChecked(checked)
		{};

		char const * Text;
		bool IsChecked;
};


class CheckListClass : public ListClass
{
	public:
		/*
		**	Constructor/Destructor
		*/
		CheckListClass(int id, int x, int y, int w, int h, TextPrintType flags,
			void const * up, void const * down);
		~CheckListClass(void);

		virtual int Add_Item(int text) {return ListClass::Add_Item(text);}
		virtual int Add_Item(char const * text);
		virtual char const * Current_Item(void) const;
		virtual char const * Get_Item(int index) const;
		virtual void Remove_Item(char const * text);
		virtual void Remove_Item(int text) {ListClass::Remove_Item(text);}
		virtual void Set_Selected_Index(char const * text);
		virtual void Set_Selected_Index(int index) {ListClass::Set_Selected_Index(index);};

		/*
		**	Checkmark utility functions
		*/
		void Check_Item(int index, bool checked);	// sets checked state of item
		bool Is_Checked(int index) const;				// gets checked state of item

		void Set_Read_Only(int rdonly) {IsReadOnly = rdonly;}

		/*
		**	This defines the ASCII value of the checkmark character & non-checkmark
		**	character.
		*/
		typedef enum CheckListClassEnum {
			CHECK_CHAR = '\3',
			UNCHECK_CHAR = ' '
		} CheckListClassEnum;

	protected:
		virtual int Action(unsigned flags, KeyNumType &key);
		virtual void Draw_Entry(int index, int x, int y, int width, int selected);

	private:
		bool IsReadOnly;
};


#endif
