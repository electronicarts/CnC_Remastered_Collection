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

/* $Header: /CounterStrike/DOOR.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DOOR.H                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 06/11/95                                                     *
 *                                                                                             *
 *                  Last Update : June 11, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef DOOR_H
#define DOOR_H

class DoorClass
{
	private:

		/*
		**	This is the animation control handler.
		*/
		StageClass Control;

		/*
		**	This is the recorded number of stages of the current
		**	door animation process.
		*/
		unsigned char Stages;

		/*
		**	This is the door state.
		*/
		enum {
			IS_CLOSED,		// Door is closed.
			IS_OPENING,		// Door is in the process of opening.
			IS_OPEN,			// Door is fully open.
			IS_CLOSING		//	Door is in the process of closing.
		} State;

		/*
		**	If the animation for this door indicates that the object it is
		**	attached to should be redrawn, then this flag will be true.
		*/
		unsigned IsToRedraw:1;

	public:
		DoorClass(void);
		DoorClass(NoInitClass const & x) : Control(x) {};

		bool Time_To_Redraw(void) {return(IsToRedraw);};
		void Clear_Redraw_Flag(void) {IsToRedraw = false;};
		void AI(void);
		int Door_Stage(void) const;
		bool Is_Door_Opening(void) const {return(State == IS_OPENING);};
		bool Is_Door_Closing(void) const {return(State == IS_CLOSING);};
		bool Open_Door(int rate, int stages);
		bool Close_Door(int rate, int stages);
		bool Is_Door_Open(void) const {return(State == IS_OPEN);};
		bool Is_Door_Closed(void) const {return(State == IS_CLOSED);};
		bool Is_Ready_To_Open(void) const;
};

#endif
