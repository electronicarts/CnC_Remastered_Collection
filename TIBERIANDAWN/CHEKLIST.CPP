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

/* $Header:   F:\projects\c&c\vcs\code\cheklist.cpv   2.18   16 Oct 1995 16:48:36   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : CHEKLIST.CPP                             *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : February 16, 1995                        *
 *                                                                         *
 *                  Last Update : February 16, 1995   [BR]                 *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   CheckListClass::Action -- action function for this class              *
 *   CheckListClass::CheckListClass -- constructor                         *
 *   CheckListClass::Check_Item -- [un]checks an items                     *
 *   CheckListClass::~CheckListClass -- destructor                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

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
	ListClass (id, x, y, w, h, flags, up, down)
{
	IsReadOnly = false;
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
 *=========================================================================*/
void CheckListClass::Check_Item(int index, int checked)
{
	if (List[index]) {
		((char &)List[index][0]) = checked ? CHECK_CHAR : UNCHECK_CHAR;
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
 *=========================================================================*/
int CheckListClass::Is_Checked(int index) const
{
	if (List[index]) {
		return(List[index][0] == CHECK_CHAR);
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
		if (Is_Checked(SelectedIndex)) {
			Check_Item(SelectedIndex,0);
		} else {
			Check_Item(SelectedIndex,1);
		}
	}

	return(rc);
}
