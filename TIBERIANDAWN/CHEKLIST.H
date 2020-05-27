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

/* $Header:   F:\projects\c&c\vcs\code\cheklist.h_v   2.19   16 Oct 1995 16:46:20   JOE_BOSTIC  $ */
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


class CheckListClass : public ListClass
{
	public:
		/*---------------------------------------------------------------------
		Constructor/Destructor
		---------------------------------------------------------------------*/
		CheckListClass(int id, int x, int y, int w, int h, TextPrintType flags,
			void const * up, void const * down);
		~CheckListClass(void) {};

		/*---------------------------------------------------------------------
		Checkmark utility functions
		---------------------------------------------------------------------*/
		void Check_Item(int index, int checked);	// sets checked state of item
		int Is_Checked(int index) const;				// gets checked state of item

		/*---------------------------------------------------------------------
		This defines the ASCII value of the checkmark character & non-checkmark
		character.
		---------------------------------------------------------------------*/
		enum CheckListClassEnum {
			CHECK_CHAR = '\3',
			UNCHECK_CHAR = ' ',
		};

		void Set_Read_Only(int rdonly) {IsReadOnly = rdonly ? true : false;}

	protected:
		virtual int Action(unsigned flags, KeyNumType &key);

	private:
		bool IsReadOnly;

};



#endif
/************************** end of cheklist.h ******************************/