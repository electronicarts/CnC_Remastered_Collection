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

/* $Header: /CounterStrike/DROP.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DROP.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/24/96                                                     *
 *                                                                                             *
 *                  Last Update : January 24, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"drop.h"


DropListClass::DropListClass(int id, char * text, int max_len, TextPrintType flags, int x, int y, int w, int h, void const * up, void const * down) :
	EditClass(id, text, max_len, flags, x, y, w, 9*RESFACTOR, ALPHANUMERIC),
	IsDropped(false),
	ListHeight(h),
	DropButton(0, down, x+w, y),
	List(0, x, y+Get_Build_Frame_Height(down), w+Get_Build_Frame_Width(down), h, flags, up, down)
{
	Fancy_Text_Print("", 0, 0, 0, 0, flags);
	EditClass::Height = FontHeight+1;
	List.Make_Peer(*this);
	DropButton.Make_Peer(*this);
}


void DropListClass::Zap(void)
{
	Collapse();
	List.Zap();
	DropButton.Zap();
	EditClass::Zap();
}

DropListClass & DropListClass::Add(LinkClass & object)
{
	DropButton.Add(object);
	return((DropListClass &)EditClass::Add(object));
}

DropListClass & DropListClass::Add_Tail(LinkClass & object)
{
	DropButton.Add_Tail(object);
	return((DropListClass &)EditClass::Add_Tail(object));
}

DropListClass & DropListClass::Add_Head(LinkClass & object)
{
	DropButton.Add_Head(object);
	return((DropListClass &)EditClass::Add_Head(object));
}

DropListClass * DropListClass::Remove(void)
{
	if (IsDropped) {
		Collapse();
	}
	DropButton.Remove();
	return((DropListClass *)EditClass::Remove());
}

int DropListClass::Add_Item(char const * text)
{
	strncpy(String, text, MaxLength);
	Flag_To_Redraw();
	return(List.Add_Item(text));
}

char const * DropListClass::Current_Item(void)
{
	return(List.Current_Item());
}

int DropListClass::Current_Index(void)
{
	return(List.Current_Index());
}

void DropListClass::Set_Selected_Index(int index)
{
	if (index < List.Count()) {
		List.Set_Selected_Index(index);
		strcpy(String, List.Get_Item(Current_Index()));
	} else {
		String[0] = '\0';
	}
}


void DropListClass::Clear_Focus(void)
{
	Collapse();
}

void DropListClass::Peer_To_Peer(unsigned flags, KeyNumType & key, ControlClass & whom)
{
	if (&whom == &DropButton) {
		if (flags & LEFTRELEASE) {
			if (IsDropped) {
				Collapse();
				key = (KeyNumType)(ID | KN_BUTTON);
			} else {
				Expand();
			}
		}
	}

	if (&whom == &List) {
		strncpy(String, List.Current_Item(), MaxLength);
		Flag_To_Redraw();
		key = (KeyNumType)(ID | KN_BUTTON);
	}
}

void DropListClass::Expand(void)
{
	if (!IsDropped) {
		List.X = X;
		List.Y = Y+9*RESFACTOR;
		List.Width = Width;
		List.Height = ListHeight;
		List.Add(Head_Of_List());
		List.Flag_To_Redraw();
		IsDropped = true;
	}
}

void DropListClass::Collapse(void)
{
	if (IsDropped) {
		List.Remove();
		IsDropped = false;
	}
}


DropListClass & DropListClass::operator = (DropListClass const & list)
{
	if (this == &list) return(*this);
	EditClass::operator =(list);
	List = list.List;
	IsDropped = list.IsDropped;
	ListHeight = list.ListHeight;
	DropButton = list.DropButton;
	List.Make_Peer(*this);
	DropButton.Make_Peer(*this);
	return(*this);
}


DropListClass::DropListClass(DropListClass const & list) :
	EditClass(list),
	IsDropped(list.IsDropped),
	ListHeight(list.ListHeight),
	DropButton(list.DropButton),
	List(list.List)
{
	List.Make_Peer(*this);
	DropButton.Make_Peer(*this);
}


void DropListClass::Set_Position(int x, int y)
{
	EditClass::Set_Position(x, y);
	List.Set_Position(x, y + Get_Build_Frame_Height(DropButton.Get_Shape_Data()));
	DropButton.Set_Position(x + Width, y);
}


void DropListClass::Set_Selected_Index(char const * text)
{
	if (text) {
		for (int index = 0; index < Count(); index++) {
			if (stricmp(text, List.Get_Item(index)) == 0) {
				Set_Selected_Index(index);
				break;
			}
		}
	}
}

#ifdef WOLAPI_INTEGRATION
void DropListClass::Flag_To_Redraw(void)
{
	if( IsDropped )
		List.Flag_To_Redraw();
	EditClass::Flag_To_Redraw();
}
#endif
