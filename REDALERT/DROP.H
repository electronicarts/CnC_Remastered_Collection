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

/* $Header: /CounterStrike/DROP.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DROP.H                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/05/96                                                     *
 *                                                                                             *
 *                  Last Update : July 5, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef DROP_H
#define DROP_H

#include	"list.h"
#include	"edit.h"

class DropListClass : public EditClass {
	public:
		DropListClass(int id, char * text, int max_len, TextPrintType flags, int x, int y, int w, int h, void const * up, void const * down);
		virtual ~DropListClass(void) {};

		virtual DropListClass & Add(LinkClass & object);
		virtual DropListClass & Add_Tail(LinkClass & object);
		virtual DropListClass & Add_Head(LinkClass & object);
		virtual DropListClass * Remove(void);
		virtual void Zap(void);

		virtual int Add_Item(char const * text);
		virtual char const * Current_Item(void);
		virtual int Current_Index(void);
		virtual void Set_Selected_Index(int index);
		virtual void Set_Selected_Index(char const * text);
		virtual void Peer_To_Peer(unsigned flags, KeyNumType &, ControlClass & whom);
		virtual void Clear_Focus(void);
		virtual int Count(void) const {return(List.Count());};
		virtual char const * Get_Item(int index) const {return(List.Get_Item(index));};

#ifdef WOLAPI_INTEGRATION
		virtual void Flag_To_Redraw(void);
#endif

		void Expand(void);
		void Collapse(void);

		virtual void Set_Position(int x, int y);

		DropListClass & operator = (DropListClass const & list);
		DropListClass(DropListClass const & list);

		/*
		**	Indicates whether the list box has dropped down or not.
		*/
		unsigned IsDropped:1;

		/*
		**	Height of list box when it is expanded.
		*/
		int ListHeight;

		/*
		**	Drop down button.
		*/
		ShapeButtonClass DropButton;

		/*
		**	List object when it is expanded.
		*/
		ListClass List;
};




template<class T>
class TDropListClass : public EditClass {
	public:
		TDropListClass(int id, char * text, int max_len, TextPrintType flags, int x, int y, int w, int h, void const * up, void const * down);
		TDropListClass(TDropListClass<T> const & list);
		virtual ~TDropListClass(void) {};

		T operator [] (int index) const {return(List[index]);};
		T & operator [] (int index) {return(List[index]);};

		virtual TDropListClass<T> & Add(LinkClass & object);
		virtual TDropListClass<T> & Add_Tail(LinkClass & object);
		virtual TDropListClass<T> & Add_Head(LinkClass & object);
		virtual TDropListClass<T> * Remove(void);
		void Zap(void);

		virtual int Add_Item(T text);
		virtual T Current_Item(void);
		virtual int Current_Index(void);
		virtual void Set_Selected_Index(int index);
		virtual void Set_Selected_Index(T item);
		virtual void Peer_To_Peer(unsigned flags, KeyNumType &, ControlClass & whom);
		virtual void Clear_Focus(void);
		virtual int Count(void) const {return(List.Count());};
		virtual T Get_Item(int index) const {return(List.Get_Item(index));};


		void Expand(void);
		void Collapse(void);

		virtual void Set_Position(int x, int y);

		TDropListClass<T> & operator = (TDropListClass<T> const & list);

		/*
		**	Indicates whether the list box has dropped down or not.
		*/
		unsigned IsDropped:1;

		/*
		**	Height of list box when it is expanded.
		*/
		int ListHeight;

		/*
		**	Drop down button.
		*/
		ShapeButtonClass DropButton;

		/*
		**	List object when it is expanded.
		*/
		TListClass<T> List;
};


template<class T>
TDropListClass<T>::TDropListClass(int id, char * text, int max_len, TextPrintType flags, int x, int y, int w, int h, void const * up, void const * down) :
	EditClass(id, text, max_len, flags, x, y, w, 9, ALPHANUMERIC),
	IsDropped(false),
	ListHeight(h),
	DropButton(0, down, x+w, y),
	List(0, x, y+Get_Build_Frame_Height(down), w+Get_Build_Frame_Width(down), h, flags, up, down)
{
	List.Make_Peer(*this);
	DropButton.Make_Peer(*this);
}


template<class T>
void TDropListClass<T>::Zap(void)
{
	Collapse();
	List.Zap();
	DropButton.Zap();
	EditClass::Zap();
}


template<class T>
TDropListClass<T> & TDropListClass<T>::Add(LinkClass & object)
{
	DropButton.Add(object);
	return((TDropListClass &)EditClass::Add(object));
}


template<class T>
TDropListClass<T> & TDropListClass<T>::Add_Tail(LinkClass & object)
{
	DropButton.Add_Tail(object);
	return((TDropListClass &)EditClass::Add_Tail(object));
}


template<class T>
TDropListClass<T> & TDropListClass<T>::Add_Head(LinkClass & object)
{
	DropButton.Add_Head(object);
	return((TDropListClass &)EditClass::Add_Head(object));
}


template<class T>
TDropListClass<T> * TDropListClass<T>::Remove(void)
{
	if (IsDropped) {
		Collapse();
	}
	DropButton.Remove();
	return((TDropListClass *)EditClass::Remove());
}


template<class T>
int TDropListClass<T>::Add_Item(T item)
{
	strncpy(String, item->Description(), MaxLength);
	Flag_To_Redraw();
	return(List.Add_Item(item));
}


template<class T>
T TDropListClass<T>::Current_Item(void)
{
	return(List.Current_Item());
}


template<class T>
int TDropListClass<T>::Current_Index(void)
{
	return(List.Current_Index());
}


template<class T>
void TDropListClass<T>::Set_Selected_Index(int index)
{
	if ((unsigned)index < List.Count()) {
		List.Set_Selected_Index(index);
		strncpy(String, List.Get_Item(Current_Index())->Description(), MaxLength);
	} else {
		String[0] = '\0';
	}
}


template<class T>
void TDropListClass<T>::Clear_Focus(void)
{
	Collapse();
}


template<class T>
void TDropListClass<T>::Peer_To_Peer(unsigned flags, KeyNumType & key, ControlClass & whom)
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
		strncpy(String, List.Current_Item()->Description(), MaxLength);
		Flag_To_Redraw();
		key = (KeyNumType)(ID | KN_BUTTON);
	}
}


template<class T>
void TDropListClass<T>::Expand(void)
{
	if (!IsDropped) {
		List.X = X;
		List.Y = Y+9;
		List.Width = Width;
		List.Height = ListHeight;
		List.Add(Head_Of_List());
		List.Flag_To_Redraw();
		IsDropped = true;
	}
}


template<class T>
void TDropListClass<T>::Collapse(void)
{
	if (IsDropped) {
		List.Remove();
		IsDropped = false;
	}
}


template<class T>
TDropListClass<T> & TDropListClass<T>::operator = (TDropListClass<T> const & list)
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


template<class T>
TDropListClass<T>::TDropListClass(TDropListClass<T> const & list) :
	EditClass(list),
	IsDropped(list.IsDropped),
	ListHeight(list.ListHeight),
	DropButton(list.DropButton),
	List(list.List)
{
	List.Make_Peer(*this);
	DropButton.Make_Peer(*this);
}


template<class T>
void TDropListClass<T>::Set_Position(int x, int y)
{
	EditClass::Set_Position(x, y);
	List.Set_Position(x, y + Get_Build_Frame_Height(DropButton.Get_Shape_Data()));
	DropButton.Set_Position(x + Width, y);
}


template<class T>
void TDropListClass<T>::Set_Selected_Index(T text)
{
	for (int index = 0; index < Count(); index++) {
		if (text == List.Get_Item(index)) {
			Set_Selected_Index(index);
			break;
		}
	}
}


#endif
