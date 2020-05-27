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

/* $Header: /CounterStrike/LIST.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
		ListClass(ListClass const & list);
		virtual ~ListClass(void);

		virtual int Add_Item(char const * text);
		virtual int Add_Item(int text);
		virtual int Add_Scroll_Bar(void);
		virtual void Bump(int up);
		virtual int Count(void) const {return List.Count();};
		virtual int Current_Index(void) const;
		virtual char const * Current_Item(void) const;
		virtual int Draw_Me(int forced);
		virtual char const * Get_Item(int index) const;
		virtual int Step_Selected_Index(int forward);
		virtual void Flag_To_Redraw(void);

		virtual void Peer_To_Peer(unsigned flags, KeyNumType & key, ControlClass & whom);
		virtual void Remove_Item(char const * text);
		virtual void Remove_Item(int);
		virtual int  Remove_Scroll_Bar(void);
		virtual void Set_Selected_Index(int index);
		virtual void Set_Selected_Index(char const * text);
		virtual void Set_Tabs(int const * tabs);
		virtual int  Set_View_Index(int index);
		virtual void Step(int up);
		virtual void Set_Position(int x, int y);

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

		/*
		**	This is the total pixel height of a standard line of text. This is greatly
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


template<class T>
class TListClass : public ControlClass
{
	public:
		TListClass(int id, int x, int y, int w, int h, TextPrintType flags, void const * up, void const * down);
		TListClass(TListClass<T> const & list);
		virtual ~TListClass(void);
		T operator [] (int index) const {return(List[index]);};
		T & operator [] (int index) {return(List[index]);};

		virtual int Add_Item(T text);
		virtual int Add_Scroll_Bar(void);
		virtual void Insert_Item(T item);
		virtual void Bump(int up);
		virtual int Count(void) const {return List.Count();};
		virtual int Current_Index(void) const;
		virtual T Current_Item(void) const;
		virtual int Draw_Me(int forced);
		virtual int Step_Selected_Index(int forward);
		virtual void Flag_To_Redraw(void);
		virtual T Get_Item(int index) const {return(List[index]);};

		virtual void Peer_To_Peer(unsigned flags, KeyNumType & key, ControlClass & whom);
		virtual void Remove_Item(T);
		virtual void Remove_Index(int);
		virtual int  Remove_Scroll_Bar(void);
		virtual void Set_Selected_Index(int index);
		virtual void Set_Selected_Index(T item);
		virtual void Set_Tabs(int const * tabs);
		virtual int  Set_View_Index(int index);
		virtual void Step(int up);
		virtual void Set_Position(int x, int y);

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
		int const * Tabs;

		/*
		**	The actual list of text pointers is maintained by this list manager.
		*/
		DynamicVectorClass<T> List;

		/*
		**	This is the total pixel height of a standard line of text. This is greatly
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

template<class T>
TListClass<T>::TListClass(int id, int x, int y, int w, int h, TextPrintType flags, void const * up, void const * down) :
	ControlClass(id, x, y, w, h, LEFTPRESS | LEFTRELEASE | KEYBOARD, false),
	UpGadget(0, up, x+w, y),
	DownGadget(0, down, x+w, y+h),
	ScrollGadget(0, x+w, y, 0, h, true),
	TextFlags(flags),
	Tabs(0),
	IsScrollActive(false),
	SelectedIndex(0),
	CurrentTopIndex(0)
{
	/*
	**	Set preliminary values for the slider related gadgets. They don't automatically
	**	appear at this time, but there are some values that can be pre-filled in.
	*/
	UpGadget.X -= UpGadget.Width;
	DownGadget.X -= DownGadget.Width;
	DownGadget.Y -= DownGadget.Height;
	ScrollGadget.X -= max(UpGadget.Width, DownGadget.Width);
	ScrollGadget.Y = Y+UpGadget.Height;
	ScrollGadget.Height -= UpGadget.Height + DownGadget.Height;
	ScrollGadget.Width = max(UpGadget.Width, DownGadget.Width);

	/*
	**	Set the list box to a default state.
	*/
	Fancy_Text_Print(TXT_NONE, 0, 0, TBLACK, TBLACK, TextFlags);
	LineHeight = FontHeight+FontYSpacing-1;
	LineCount = (h-1) / LineHeight;
}

template<class T>
TListClass<T>::TListClass(TListClass<T> const & list) :
	ControlClass(list),
	TextFlags(list.TextFlags),
	Tabs(list.Tabs),
	List(list.List),
	LineHeight(list.LineHeight),
	LineCount(list.LineCount),
	IsScrollActive(list.IsScrollActive),
	UpGadget(list.UpGadget),
	DownGadget(list.DownGadget),
	ScrollGadget(list.ScrollGadget),
	SelectedIndex(list.SelectedIndex),
	CurrentTopIndex(list.CurrentTopIndex)
{
	UpGadget.Make_Peer(*this);
	DownGadget.Make_Peer(*this);
	ScrollGadget.Make_Peer(*this);
}

template<class T>
void TListClass<T>::Set_Position(int x, int y)
{
	UpGadget.X = x + Width - UpGadget.Width;
	UpGadget.Y = y;
	DownGadget.X = x + Width - DownGadget.Width;
	DownGadget.Y = y + Height - DownGadget.Height;
	ScrollGadget.X = x + Width - max(UpGadget.Width, DownGadget.Width);
	ScrollGadget.Y = y + UpGadget.Height;
	ScrollGadget.Height = Height - (UpGadget.Height + DownGadget.Height);
	ScrollGadget.Width = max(UpGadget.Width, DownGadget.Width);
}

template<class T>
TListClass<T>::~TListClass(void)
{
	Remove_Scroll_Bar();
}

template<class T>
void TListClass<T>::Insert_Item(T item)
{
	if (Current_Index() >= Count()) {
		List.Add(item);
	} else {
		List.Add(item);

		/*
		**	Move all trailing items upward.
		*/
		for (int index = List.Count()-1; index >= Current_Index(); index--) {
			List[index+1] = List[index];
		}

		/*
		**	Insert the new item into the location at the current index.
		*/
		List[Current_Index()] = item;
	}
}


template<class T>
int TListClass<T>::Add_Item(T text)
{
//	if (text) {
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
//	}
	return(List.Count() - 1);
}

template<class T>
void TListClass<T>::Remove_Index(int index)
{
	if ((unsigned)index < List.Count()) {
		List.Delete(index);

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
			if (SelectedIndex < 0) {
				SelectedIndex = 0;
			}
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

template<class T>
void TListClass<T>::Remove_Item(T text)
{
	Remove_Index(List.ID(text));
}

template<class T>
int TListClass<T>::Action(unsigned flags, KeyNumType & key)
{
	if (flags & LEFTRELEASE) {
		key = KN_NONE;
		flags &= (~LEFTRELEASE);
		ControlClass::Action(flags, key);
		return(true);
	} else {

		/*
		** Handle keyboard events here.
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
			SelectedIndex = min(SelectedIndex, List.Count()-1);
		}
	}
	return(ControlClass::Action(flags, key));
}

template<class T>
int TListClass<T>::Draw_Me(int forced)
{
	if (GadgetClass::Draw_Me(forced)) {

		/*
		**	Turn off the mouse.
		*/
		if (LogicPage == &SeenBuff) {
			Conditional_Hide_Mouse(X, Y, X+Width, Y+Height);
		}

		Draw_Box(X, Y, Width, Height, BOXSTYLE_BOX, true);

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
					List[line]->Draw_It(line, X+1, Y+(LineHeight*index)+1, Width-2, LineHeight, (line == SelectedIndex), TextFlags);
//					List[index].Draw_It(line, X+1, Y+(LineHeight*index)+1, Width-2, LineHeight, (line == SelectedIndex), TextFlags);
//					Draw_Entry(line, X+1, Y+(LineHeight*index)+1, Width-2, (line == SelectedIndex));
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

template<class T>
void TListClass<T>::Bump(int up)
{
	if (IsScrollActive) {
		if (ScrollGadget.Step(up)) {
			CurrentTopIndex = ScrollGadget.Get_Value();
			Flag_To_Redraw();
		}
	}
}

template<class T>
void TListClass<T>::Step(int up)
{
	if (IsScrollActive) {
		if (ScrollGadget.Step(up)) {
			CurrentTopIndex = ScrollGadget.Get_Value();
			Flag_To_Redraw();
		}
	}
}

#ifdef NEVER
template<class T>
T TListClass<T>::Get_Item(int index) const
{
	index = min(index, List.Count());
	return(List[index]);
}
#endif

template<class T>
T TListClass<T>::Current_Item(void) const
{
	static T _temp;
	if (List.Count() <= SelectedIndex) {
		return(_temp);
	}
	return(List[SelectedIndex]);
}

template<class T>
int TListClass<T>::Current_Index(void) const
{
	return(SelectedIndex);
}

template<class T>
void TListClass<T>::Peer_To_Peer(unsigned flags, KeyNumType &, ControlClass & whom)
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

template<class T>
int TListClass<T>::Set_View_Index(int index)
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

template<class T>
int TListClass<T>::Add_Scroll_Bar(void)
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

template<class T>
int TListClass<T>::Remove_Scroll_Bar(void)
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

template<class T>
void TListClass<T>::Set_Tabs(int const * tabs)
{
	Tabs = tabs;
}

#ifdef NEVER
template<class T>
void TListClass<T>::Draw_Entry(int index, int x, int y, int width, int selected)
{
	if (TextFlags & TPF_6PT_GRAD) {
		TextPrintType flags = TextFlags;

		if (selected) {
			flags = flags | TPF_BRIGHT_COLOR;
			LogicPage->Fill_Rect(x, y, x + width - 1, y + LineHeight - 1, CC_GREEN_SHADOW);
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
#endif

template<class T>
LinkClass & TListClass<T>::Add(LinkClass & list)
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

template<class T>
LinkClass & TListClass<T>::Add_Head(LinkClass & list)
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

template<class T>
LinkClass & TListClass<T>::Add_Tail(LinkClass & list)
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

template<class T>
GadgetClass * TListClass<T>::Remove(void)
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

template<class T>
void TListClass<T>::Set_Selected_Index(int index)
{
   if ((unsigned)index < List.Count()) {
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

template<class T>
int TListClass<T>::Step_Selected_Index(int step)
{
	int old = SelectedIndex;

	Set_Selected_Index(old + step);
	return(old);
}

template<class T>
void TListClass<T>::Flag_To_Redraw(void)
{
	if (IsScrollActive) {
		UpGadget.Flag_To_Redraw();
		DownGadget.Flag_To_Redraw();
		ScrollGadget.Flag_To_Redraw();
	}
	ControlClass::Flag_To_Redraw();
}

template<class T>
void TListClass<T>::Set_Selected_Index(T text)
{
	for (int index = 0; index < Count(); index++) {
		if (text == Get_Item(index)) {
			Set_Selected_Index(index);
			break;
		}
	}
}


#endif
