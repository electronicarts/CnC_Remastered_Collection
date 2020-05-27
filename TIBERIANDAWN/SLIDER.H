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

/* $Header:   F:\projects\c&c\vcs\code\slider.h_v   2.16   16 Oct 1995 16:45:38   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : SLIDER.H                                                     * 
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

#ifndef SLIDER_H
#define SLIDER_H

#include "gauge.h"
#include "shapebtn.h"

/***************************************************************************
 * SliderClass -- Like a Windows ListBox structure									*
 *                                                                         *
 * INPUT:      int id-- id of gadget													*
 *					int x -- x position of gadget											*
 *					int y -- y position of gadget											*
 *					int w -- width of gadget												*							
 *					int h -- height of gadget												*							
 *					int belong_to_list -- does this slider go with a listclass? *
 *                                                                         *
 * OUTPUT:     none.																			*
 * WARNINGS:														   						*
 * HISTORY:    01/03/1995 MML : Created.                                   *
 *=========================================================================*/
class SliderClass : public GaugeClass
{
	public:
		SliderClass(unsigned id, int x, int y, int w, int h, int belong_to_list=false);
		virtual ~SliderClass(void);
//		static SliderClass * Create_One_Of(unsigned id, int x, int y, int w, int h, int belong_to_list=false);

		virtual void Set_Thumb_Size(int value);
		virtual int  Set_Maximum(int value);
		virtual int  Set_Value(int);
		virtual int  Bump(int up);
		virtual int  Step(int up);
		virtual int  Draw_Me(int forced);
		virtual void Peer_To_Peer(unsigned flags, KeyNumType & key, ControlClass & whom);

		virtual int Thumb_Pixels(void) { return (ThumbSize);}

	protected:

		/*
		**	If the slider bar has been created, these point to the respective gadgets
		**	that it is composed of.
		*/
		ShapeButtonClass * PlusGadget;
		ShapeButtonClass * MinusGadget;

		/*
		**	If I belong to a listbox, I have to draw myself differently...
		**/
		unsigned BelongToList:1;

		/*
		**	This is the logical size of the thumb. This value is used when drawing 
		**	the thumb imagery. It is also the amount that is bumped when the 
		**	Bump() function is called.  (This value is in application units.)
		*/
		int Thumb;

		/*
		**	This is the current thumb pixel size and starting offset from beginning
		**	of slider region.  (These values are in pixels.)
		*/
		int ThumbSize;
		int ThumbStart;			// x or y position for the thumb

		virtual int  Action(unsigned flags, KeyNumType &key);
		virtual void Draw_Thumb(void);

	private:
		void Recalc_Thumb(void);
};

#endif
