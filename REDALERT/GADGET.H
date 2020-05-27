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

/* $Header: /CounterStrike/GADGET.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : GADGET.H                                                     *
 *                                                                                             *
 *                   Programmer : Maria del Mar McCready Legg                                  *
 *                                                                                             *
 *                   Start Date : January 3, 1995                                              *
 *                                                                                             *
 *                  Last Update : January 3, 1995   [MML]                                      *
 *                                                                                             *
 *                                                                                             *
 *        LinkClass [This is the linked list manager class. It keeps a record                  *
 *            ³      of the next and previous gadget in the list. It is possible               *
 *            ³      delete a gadget out of the middle of the list with this                   *
 *            ³      class.]                                                                   *
 *            ³                                                                                *
 *       GadgetClass [The is the basic gadget class. It handles processing of                  *
 *            ³       input events and dispatching the appropriate functions.                  *
 *            ³       All gadgets must be derived from this class.]                            *
 *            ÃÄÄÄÄ¿                                                                           *
 *            ³    ³                                                                           *
 *            ³  ListClass [This list class functions like a list box does in Windows. It      *
 *            ³             keeps track of a list of text strings. This list can be            *
 *            ³             scrolled and an item selected. If the list becomes larger than     *
 *            ³             can be completely displayed, it will automatically create a        *
 *            ³             slider (at the right edge) to manage the scrolling.]               *
 *            ³                                                                                *
 *      ControlClass [This class adds the concept of giving an ID number to the                *
 *            ³       gadget. This ID can then be returned from the Input()                    *
 *            ³       function as if it were a pseudo-keystroke. Additionally,                 *
 *            ³       the ability to inform another button that this button has                *
 *            ³       been actioned is allowed. This ability allows one button                 *
 *            ³       to watch what happens to another button. Example: a list                 *
 *            ³       box gadget can tell when an attached slider has been                     *
 *            ³       touched.]                                                                *
 *    ÚÄÄÄÄÄÄÄÅÄÄÄÄ¿                                                                           *
 *    ³       ³    ³                                                                           *
 *    ³       ³  GaugeClass [This class looks similar to Windows slider, but has               *
 *    ³       ³    ³         a different controlling logic. There is no thumb and              *
 *    ³       ³    ³         it serves as a simple variable control setting. This              *
 *    ³       ³    ³         is analogous to a volume slider.]                                 *
 *    ³       ³    ³                                                                           *
 *    ³       ³ SliderClass [The slider class is similar to the typical Windows slider. It     *
 *    ³       ³              has a current setting, a thumb, and a controllable scale. This    *
 *    ³       ³              is the object created to handle a scrolling list box.]            *
 *    ³       ³                                                                                *
 *    ³   EditClass                                                                            *
 *    ³                                                                                        *
 *    ³                                                                                        *
 * ToggleClass [The toggle class is used for buttons that have an image and behave just        *
 *    ³         like the buttons in Windows do. That is, they have a separate visual for       *
 *    ³         when they are pressed and raised. They are officially triggered (return        *
 *    ³         their ID number) when the mouse button is released while over the button.      *
 *    ³         This class doesn't perform any rendering itself. It merely provides the        *
 *    ³         logic so that the derived classes will function correctly.]                    *
 *  ÚÄÁÄÄÄÄ¿                                                                                   *
 *  ³      ³                                                                                   *
 *  ³   TextButtonClass [The text button functions like a normal Windows style button, but     *
 *  ³                    the imagery is based on text that is displayed on the button. A       *
 *  ³                    typical example would be the "OK" or "Cancel" buttons.]               *
 *  ³                                                                                          *
 * ShapeButtonClass [The shape buttons is similar to the TextButton but instead of text        *
 *                   being used to give the button its imagery, an actual shape is used        *
 *                   instead. This allows graphic buttons. These are similar to the up/down    *
 *                   arrows seen in a Windows slider.]                                         *
 *                                                                                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef GADGET_H
#define GADGET_H

#include "link.h"

class ControlClass;

class GadgetClass : public LinkClass
{
	public:
		friend class DLLExportClass;	// ST - 5/13/2019

		typedef enum FlagEnum {
			LEFTPRESS    = 0x0001,	// Left mouse button press.
			LEFTHELD     = 0x0002,	// Left mouse button is being held down.
			LEFTRELEASE  = 0x0004,	// Left mouse button released.
			LEFTUP       = 0x0008,	// Left mouse button is being held up.
			RIGHTPRESS   = 0x0010,	// Right mouse button press.
			RIGHTHELD    = 0x0020,	// Right mouse button is being held down.
			RIGHTRELEASE = 0x0040,	// Right mouse button released.
			RIGHTUP      = 0x0080,	// Right mouse button is being held up.
			KEYBOARD     = 0x0100	// Keyboard input processing (maybe).
		} FlagEnum;

		GadgetClass(int x, int y, int w, int h, unsigned flags, int sticky=false);
		GadgetClass(NoInitClass const & x) : LinkClass(x) {};
		GadgetClass(void) {};
		GadgetClass(GadgetClass const & gadget);
		virtual ~GadgetClass(void);

		/*
		**	Gadget list management functions.
		*/
		virtual KeyNumType Input(void);
		virtual void Draw_All(bool forced=true);
		virtual void Delete_List(void);
		virtual ControlClass * Extract_Gadget(unsigned id);
		virtual void Flag_List_To_Redraw(void) {LastList = 0;};
		virtual GadgetClass * Remove(void);
		virtual GadgetClass * Get_Next(void) const;
		virtual GadgetClass * Get_Prev(void) const;

		/*
		**	Manages individual gadget states and actions.
		*/
		virtual void Disable(void);
		virtual void Enable(void);
		virtual unsigned Get_ID(void) const {return 0;};
		virtual void Flag_To_Redraw(void);
		virtual void Peer_To_Peer(unsigned , KeyNumType & , ControlClass & ) {};
		virtual void Set_Focus(void);
		virtual void Clear_Focus(void);
		virtual bool Has_Focus(void);
		virtual int  Is_List_To_Redraw(void);
		virtual bool Is_To_Redraw(void) {return (IsToRepaint);}
		virtual void Set_Position(int x, int y);

		/*
		**	General render function.
		*/
		virtual int Draw_Me(int forced=false);

		/*
		** Sets the current color scheme
		*/
		static void Set_Color_Scheme(RemapControlType *scheme)
			{ ColorScheme = scheme; }

		static RemapControlType * Get_Color_Scheme(void)
			{ return (ColorScheme); }

		/*
		**	This is the coordinates and dimensions of the gadget region. These are in
		**	absolute screen pixel coordinates.
		*/
		int X;
		int Y;
		int Width;
		int Height;

	protected:

		/*
		**	Processes the event flags so that if this gadget needs to "stick" or
		**	"unstick", it will be properly flagged. Call this function if you are
		**	going to clear the button press flags before calling the base class
		**	Action() function. Otherwise, calling this function manually, is
		**	unnecessary since the base class Action() function already does so.
		*/
		virtual void Sticky_Process(unsigned flags);

		/*
		**	This is the action function that will be called whenever the flags and mouse
		**	input indicates. This is the main method by which this button performs a useful
		**	function.
		*/
		virtual int Action(unsigned flags, KeyNumType & key);

		/*
		**	This is a record of the last list passed to the Input() function. If a list
		**	different than the last recorded one is detected, then the draw function is
		**	called for every gadget in the list. This causes all buttons to be redrawn the
		**	fire time Input() is called without forced a manual call to Draw_All().
		*/
		static GadgetClass * LastList;

		/*
		**	This points to the gadget that has the keyboard focus. All keyboard only
		**	events are fed to this gadget to the exclusion of all others.
		*/
		static GadgetClass * Focused;

		/*
		**	This button should call the Draw_Me function because some graphic element needs
		**	to be redrawn. This flag is set by default if the Action function is called.
		*/
		unsigned IsToRepaint:1;

	public:		// HACK HACK HACK.. this is here because the sidebar buttons are static.
		/*
		**	A sticky button is one that is processed to the exclusion of all other buttons
		**	IF the mouse was pressed down while over this button and the mouse continues
		**	to remain pressed. This is the standard behavior for all normal Windows style
		**	buttons.
		*/
		unsigned IsSticky:1;

		//	ajw - Publicized StuckOn 7/30/98 (was protected)
		/*
		**	If there is a sticky button being processed, then this will point to it. A sticky
		**	button is one that will ONLY be processed while the mouse button is being
		**	held down.
		*/
		static GadgetClass * StuckOn;

	protected:

		/*
		**	If the button is disabled, then it won't be processed by the input function. It will
		**	have its Draw_Me function called as necessary. In order to not display the button
		**	at all, the appropriate draw function should perform no action -- just return. Or,
		**	just remove the button from the list.
		*/
		unsigned IsDisabled:1;

		/*
		**	These are the action flags that are used to determine when the action function
		**	should be called. Example: If this gadget only wants the action button called when
		**	the left mouse button is pressed over the its region, then the flag will be set
		**	to LEFTPRESS.
		*/
		unsigned Flags;

		/*
		** This is the current color scheme; it must be initialized by the app.
		*/
		static RemapControlType *ColorScheme;

	private:
public:		//ST - 5/14/2019
		virtual int Clicked_On(KeyNumType & key, unsigned flags, int x, int y);
};

//PG
//inline GadgetClass::FlagEnum operator |(GadgetClass::FlagEnum, GadgetClass::FlagEnum);
//inline GadgetClass::FlagEnum operator &(GadgetClass::FlagEnum, GadgetClass::FlagEnum);
//inline GadgetClass::FlagEnum operator ~(GadgetClass::FlagEnum);

inline GadgetClass::FlagEnum operator|(GadgetClass::FlagEnum a, GadgetClass::FlagEnum b)
{
	return static_cast<GadgetClass::FlagEnum>(static_cast<int>(a) | static_cast<int>(b));
}

inline GadgetClass::FlagEnum operator&(GadgetClass::FlagEnum a, GadgetClass::FlagEnum b)
{
	return static_cast<GadgetClass::FlagEnum>(static_cast<int>(a) & static_cast<int>(b));
}

inline GadgetClass::FlagEnum operator~(GadgetClass::FlagEnum a)
{
	return static_cast<GadgetClass::FlagEnum>(~static_cast<int>(a));
}


#endif
