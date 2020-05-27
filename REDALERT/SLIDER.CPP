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

/* $Header: /CounterStrike/SLIDER.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SLIDER.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/15/95                                                     *
 *                                                                                             *
 *                  Last Update : September 20, 1995 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   SliderClass::Action -- Handles input processing for the slider.                           *
 *   SliderClass::Bump -- Bumps the slider one "thumb size" up or down.                        *
 *   SliderClass::Recalc_Thumb -- Recalculates the thumb pixel size and starting offset.       *
 *   SliderClass::Set_Maximum -- Sets the maximum value for this slider.                       *
 *   SliderClass::Set_Thumb_Size -- Sets the size of the thumb in "slider units".              *
 *   SliderClass::Set_Value -- Sets the current thumb position for the slider.                 *
 *   SliderClass::SliderClass -- Normal constructor for a slider (with thumb) gadget.          *
 *   SliderClass::Step -- Steps the slider one value up or down.                               *
 *   SliderClass::Draw_Thumb -- Draws the "thumb" for this slider.                             *
 *   SliderClass::~SliderClass -- Destructor for slider object.                                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include "slider.h"


/***********************************************************************************************
 * SliderClass::SliderClass -- Normal constructor for a slider (with thumb) gadget.            *
 *                                                                                             *
 *    This is the normal constructor for the slider gadget.                                    *
 *                                                                                             *
 * INPUT:   id    -- The ID number to assign to this gadget.                                   *
 *          x,y   -- The pixel coordinate of the upper left corner for this gadget.            *
 *          w,h   -- The width and height of the slider gadget. The slider automatically       *
 *                   adapts for horizontal or vertical operation depending on which of these   *
 *                   dimensions is greater.                                                    *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
SliderClass::SliderClass(unsigned id, int x, int y, int w, int h, int belong_to_list)
	: GaugeClass(id, x, y, w, h)
{
	BelongToList = belong_to_list ? true : false;

	PlusGadget = 0;
	MinusGadget = 0;
	if (!BelongToList) {
		PlusGadget  = new ShapeButtonClass(id, MFCD::Retrieve("BTN-PLUS.SHP"), X+Width+2, Y);
		MinusGadget = new ShapeButtonClass(id, MFCD::Retrieve("BTN-MINS.SHP"), X-6, Y);

		if (PlusGadget) {
			PlusGadget->Make_Peer(*this);
			PlusGadget->Add(*this);
			PlusGadget->Flag_To_Redraw();
		}
		if (MinusGadget) {
			MinusGadget->Make_Peer(*this);
			MinusGadget->Add(*this);
			MinusGadget->Flag_To_Redraw();
		}
	}
	Set_Thumb_Size(1);
	Recalc_Thumb();

	/*
	** Gauges have at least 2 colors, but sliders should only have one.
	*/
	IsColorized = 0;
}


/***********************************************************************************************
 * SliderClass::~SliderClass -- Destructor for slider object.                                  *
 *                                                                                             *
 *    This cleans up the slider object in preparation for deletion.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
SliderClass::~SliderClass(void)
{
	if (PlusGadget) {
		delete PlusGadget;
		PlusGadget = 0;
	}
	if (MinusGadget) {
		delete MinusGadget;
		MinusGadget = 0;
	}
}


/***********************************************************************************************
 * SliderClass::Set_Maximum -- Sets the maximum value for this slider.                         *
 *                                                                                             *
 *    This sets the maximum value that the slider can be set at. The maximum value controls    *
 *    the size of the thumb and the resolution of the thumb's movement.                        *
 *                                                                                             *
 * INPUT:   value -- The value to set for the slider's maximum.                                *
 * OUTPUT:  bool; Was the maximum value changed? A false indicates a set to the value it       *
 *                is currently set to already.                                                 *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
int SliderClass::Set_Maximum(int value)
{
	if (GaugeClass::Set_Maximum(value)) {
		Recalc_Thumb();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * SliderClass::Set_Thumb_Size -- Sets the size of the thumb in "slider units".                *
 *                                                                                             *
 *    This routine will set the size of the thumb as it relates to the maximum value the       *
 *    slider can achieve. This serves to display a proportionally sized thumb as well as       *
 *    control how the slider "bumps" up or down.                                               *
 *                                                                                             *
 * INPUT:   value -- The new value of the thumb. It should never be larger than the slider     *
 *                   maximum.                                                                  *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
void SliderClass::Set_Thumb_Size(int value)
{
	Thumb = min(value, MaxValue);
	Thumb = max(Thumb, 1);
	Flag_To_Redraw();
	Recalc_Thumb();
}


/***********************************************************************************************
 * SliderClass::Set_Value -- Sets the current thumb position for the slider.                   *
 *                                                                                             *
 *    This routine will set the thumb position for the slider.                                 *
 *                                                                                             *
 * INPUT:   value -- The position to set the slider. This position is relative to the maximum  *
 *                   value for the slider.                                                     *
 *                                                                                             *
 * OUTPUT:  bool; Was the slider thumb position changed at all?                                *
 * WARNINGS:  none                                                                             *
 * HISTORY:   01/15/1995 JLB : Created.                                                        *
 *=============================================================================================*/
int SliderClass::Set_Value(int value)
{
	value = min(value, MaxValue-Thumb);

	if (GaugeClass::Set_Value(value)) {
		Recalc_Thumb();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * SliderClass::Recalc_Thumb -- Recalculates the thumb pixel size and starting offset.         *
 *                                                                                             *
 *    This takes the current thumb logical size and starting value and calculates the pixel    *
 *    size and starting offset accordingly. This function should be called whenever one of     *
 *    these elements has changed.                                                              *
 *                                                                                             *
 * INPUT:      none                                                                            *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
void SliderClass::Recalc_Thumb(void)
{
	int length = IsHorizontal ? Width : Height;
	int size   = length * fixed(Thumb, MaxValue);
//	int size   = Fixed_To_Cardinal(length, Cardinal_To_Fixed(MaxValue, Thumb));
	ThumbSize  = max(size, 4);
	int start  = length * fixed(CurValue, MaxValue);
//	int start  = Fixed_To_Cardinal(length, Cardinal_To_Fixed(MaxValue, CurValue));
	ThumbStart = min(start, length-ThumbSize);
}


/***********************************************************************************************
 * SliderClass::Action -- Handles input processing for the slider.                             *
 *                                                                                             *
 *    This routine is called when a qualifying input event has occurred. This routine will     *
 *    process that event and make any adjustments to the slider as necessary.                  *
 *                                                                                             *
 * INPUT:   flags -- Flag bits that tell the input event that caused this function to          *
 *                   be called.                                                                *
 *          key   -- Reference to the key that caused the input event.                         *
 * OUTPUT:  bool; Was the event consumed and further processing of the gadget list should be   *
 *                aborted?                                                                     *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
int SliderClass::Action(unsigned flags, KeyNumType &key)
{
	/*
	**	Handle the mouse click in a special way. If the click was not on the thumb, then
	**	jump the thumb position one "step" in the appropriate direction. Otherwise, let normal
	**	processing take place -- the slider then "sticks" and the thumb moves according to
	**	mouse position.
	*/
	if (flags & LEFTPRESS) {
		int mouse;		// Mouse pixel position.
		int edge;		// Edge of slider.

		if (IsHorizontal) {
			mouse = Get_Mouse_X();
			edge = X;
		} else {
			mouse = Get_Mouse_Y();
			edge = Y;
		}
		edge += 1;

		/*
		** Clicking outside the thumb: invoke parent's Action to process flags etc,
		** but turn off the event & return true so processing stops at this button.
		*/
		if (mouse < edge+ThumbStart) {
			Bump(true);
			GaugeClass::Action(0, key);
			key = KN_NONE;
			return(true);
		} else {
			if (mouse > edge+ThumbStart+ThumbSize) {
				Bump(false);
				GaugeClass::Action(0, key);
				key = KN_NONE;
				return(true);
			} else {
				GaugeClass::Action(flags, key);
				key = KN_NONE;
				return(true);
			}
		}
	}

	/*
	**  CHANGE GAUGECLASS::ACTION -- REMOVE (LEFTRELEASE) FROM IF STMT
	*/
	return(GaugeClass::Action(flags, key));
}


/***********************************************************************************************
 * SliderClass::Bump -- Bumps the slider one "thumb size" up or down.                          *
 *                                                                                             *
 *    This support function will bump the slider one "step" or the size of the thumb up or     *
 *    down as specified. It is typically called when the slider is clicked outside of the      *
 *    thumb region but still inside of the slider.                                             *
 *                                                                                             *
 * INPUT:   up -- Should the bump be to increase the current position?                         *
 * OUTPUT:  bool; Was the slider changed at all? A false indicates that the slider is already  *
 *                at one end or the other.                                                     *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
int SliderClass::Bump(int up)
{
	if (up) {
		return(Set_Value(CurValue - Thumb));
	}
	return(Set_Value(CurValue + Thumb));
}


/***********************************************************************************************
 * SliderClass::Step -- Steps the slider one value up or down.                                 *
 *                                                                                             *
 *    This routine will move the slider thumb one step in the direction specified.             *
 *                                                                                             *
 * INPUT:   up -- Should the step be up (i.e., forward)?                                       *
 * OUTPUT:  bool; Was the slider changed at all? A false indicates that the slider is already  *
 *                at one end or the other.                                                     *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
int SliderClass::Step(int up)
{
	if (up) {
		return(Set_Value(CurValue - 1));
	}
	return(Set_Value(CurValue + 1));
}


/***********************************************************************************************
 * SliderClass::Draw_Thumb -- Draws the "thumb" for this slider.                               *
 *                                                                                             *
 *    This will draw the thumb graphic for this slider. Sometimes the thumb requires special   *
 *    drawing, thus the need for this function separate from the normal Draw_Me function.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 * OUTPUT:  none                                                                               *
 * WARNINGS:   The mouse is guaranteed to be hidden when this routine is called.               *
 * HISTORY:    01/16/1995 JLB : Created.                                                       *
 *=============================================================================================*/
void SliderClass::Draw_Thumb(void)
{
	if (IsHorizontal) {
		Draw_Box(X+ThumbStart, Y, ThumbSize, Height, BOXSTYLE_RAISED, true);
	} else {
		Draw_Box(X, Y+ThumbStart, Width, ThumbSize, BOXSTYLE_RAISED, true);
	}
}


/***********************************************************************************************
 * SliderClass::Draw_Me -- Draws the body of the gauge.                                        *
 *                                                                                             *
 *    This routine will draw the body of the gauge if necessary.                               *
 *                                                                                             *
 * INPUT:   forced   -- Should the gauge be redrawn regardless of the current redraw flag?     *
 * OUTPUT:  bool; Was the gauge redrawn?                                                       *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/16/1995 JLB : Created.                                                       *
 *=============================================================================================*/
int SliderClass::Draw_Me(int forced)
{
	if (BelongToList) {
		if (ControlClass::Draw_Me(forced)) {

			/*
			**	Hide the mouse.
			*/
			if (LogicPage == &SeenBuff) {
				Conditional_Hide_Mouse(X, Y, X+Width, Y+Height);
			}

			/*
			**	Draw the body & set text color.
			*/
			Draw_Box (X, Y, Width, Height, BOXSTYLE_DOWN, true);
			Draw_Thumb();

			/*
			**	Display the mouse.
			*/
			if (LogicPage == &SeenBuff) {
				Conditional_Show_Mouse();
			}
			return(true);
		}
	}

	/*
	**	If it does not belong to a listbox...
	*/
	return(GaugeClass::Draw_Me(forced));
}


/***********************************************************************************************
 * SliderClass::Peer_To_Peer -- A peer gadget was touched -- make adjustments.                 *
 *                                                                                             *
 *    This routine is called when one of the peer gadgets (the scroll arrows or the slider)    *
 *    was touched in some fashion. This routine will sort out whom and why and then make       *
 *    any necessary adjustments to the list box.                                               *
 *                                                                                             *
 * INPUT:   flags    -- The event flags that affected the peer gadget.                         *
 *          key      -- The key value at the time of the event.                                *
 *          whom     -- Which gadget is being touched.                                         *
 * OUTPUT:  none                                                                               *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/16/1995 JLB : Created.                                                       *
 *=============================================================================================*/
void SliderClass::Peer_To_Peer(unsigned flags, KeyNumType & , ControlClass & whom)
{
	if (flags & LEFTRELEASE) {
		if (&whom == PlusGadget) {
			Step(false);
		}
		if (&whom == MinusGadget) {
			Step(true);
		}
	}
}


