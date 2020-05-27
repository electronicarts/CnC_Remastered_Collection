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

/* $Header:   F:\projects\c&c\vcs\code\mouse.h_v   2.16   16 Oct 1995 16:45:06   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MOUSE.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/15/94                                                     *
 *                                                                                             *
 *                  Last Update : December 15, 1994 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MOUSE_H
#define MOUSE_H

#include	"stage.h"
#include	"scroll.h"

class MouseClass: public ScrollClass
{
	public:
		MouseClass(void);

		/*
		** Initialization
		*/
		virtual void One_Time(void);							// One-time inits
		virtual void Init_Clear(void);						// Clears all to known state

		virtual void AI(KeyNumType &input, int x, int y);
		virtual bool Override_Mouse_Shape(MouseType mouse, bool wwsmall=false);
		virtual void Revert_Mouse_Shape(void);
		virtual MouseType Get_Mouse_Shape(void) const {return NormalMouseShape;};
		virtual void Mouse_Small(bool wwsmall);

		/*
		**	File I/O.
		*/
		virtual bool Load(FileClass & file);
		virtual bool Save(FileClass & file);
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		virtual void Set_Default_Mouse(MouseType mouse, bool wwsmall = false);

		/*
		**	This allows the tactical map input gadget access to change the
		**	mouse shapes.
		*/
		friend class TacticalClass;

	private:

		/*
		**	This type is used to control the frames and rates of the mouse
		**	pointer. Some mouse pointers are actually looping animations.
		*/
		typedef struct MouseStruct
		{
			int StartFrame;			// Starting frame number.
			int FrameCount;			// Number of animation frames.
			int FrameRate;				// Frame delay between changing frames.
			int SmallFrame;			// Start frame number for small version (if any).
			int X,Y;						// Hotspot X and Y offset.
		} MouseStruct;

		/*
		**	The control frames and rates for the various mouse pointers are stored
		**	in this static array.
		*/
		static MouseStruct MouseControl[MOUSE_COUNT];

		/*
		**	If the small representation of the mouse is active, then this flag is true.
		*/
		unsigned IsSmall:1;

		/*
		**	This points to the loaded mouse shapes.
		*/
		static void const * MouseShapes;

		/*
		**	The mouse shape is controlled by these variables. These
		**	hold the current mouse shape (so resetting won't be needlessly performed) and
		**	the normal default mouse shape (when arrow shapes are needed).
		*/
		MouseType CurrentMouseShape;
		MouseType NormalMouseShape;

		/*
		**	For animating mouse shapes, this controls the frame and animation rate.
		*/
		static CountDownTimerClass Timer;
		int Frame;
//		StageClass Control;

		/*
		** This contains the value of the Virtual Function Table Pointer
		*/
		static void * VTable;
};


#endif
