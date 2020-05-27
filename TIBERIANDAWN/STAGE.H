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

/* $Header:   F:\projects\c&c\vcs\code\stage.h_v   2.15   16 Oct 1995 16:45:16   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : STAGE.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : June 17, 1994                                                *
 *                                                                                             *
 *                  Last Update : June 17, 1994   [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef STAGE_H
#define STAGE_H

class StageClass {

		/*
		**	This handles the animation stage of the object. This includes smoke, walking,
		**	flapping, and rocket flames.
		*/
		unsigned short Stage;

		/*
		**	This is the countdown timer for stage animation. When this counts down
		**	to zero, then the stage increments by one and the time cycle starts 
		**	over again.
		*/
		unsigned char StageTimer;

		/*
		**	This is the value to assign the StageTimer whenever it needs to be reset. Thus,
		**	this value is the control of how fast the stage value increments.
		*/
		unsigned char Rate;

	public:
		StageClass(void) {
			StageTimer = 0;
			Stage = 0;
			Rate = 0;
		};

		int Fetch_Stage(void) const {return Stage;};
		int Fetch_Rate(void) const {return Rate;};
		void Set_Stage(int stage) {Stage = stage;};
		void Set_Rate(unsigned char rate) {Rate = StageTimer = rate;};
		void AI(void) {};
		bool Graphic_Logic(void) {
			if (Rate) {
				StageTimer--;
				if (!StageTimer) {
					Stage++;
					StageTimer = Rate;
					return true;
				}
			}
			return false;
		};
		#ifdef CHEAT_KEYS
		void Debug_Dump(MonoClass *mono) const {
			mono->Set_Cursor(56, 7);
			mono->Printf("%3d[%d]", Stage, Rate);
		};
		#endif

		/*
		**	File I/O.
		*/
		void Code_Pointers(void) { return; }
		void Decode_Pointers(void) { return; }
};


#endif
