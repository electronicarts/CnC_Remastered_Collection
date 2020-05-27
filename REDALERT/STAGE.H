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

/* $Header: /CounterStrike/STAGE.H 1     3/03/97 10:25a Joe_bostic $ */
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
		unsigned Stage;

		/*
		**	This is the countdown timer for stage animation. When this counts down
		**	to zero, then the stage increments by one and the time cycle starts
		**	over again.
		*/
		CDTimerClass<FrameTimerClass> Timer;

		/*
		**	This is the value to assign the StageTimer whenever it needs to be reset. Thus,
		**	this value is the control of how fast the stage value increments.
		*/
		int Rate;

	public:
		StageClass(void) : Stage(0), Timer(0), Rate(0) {};
		StageClass(NoInitClass const & x) : Timer(x) {};

		int Fetch_Stage(void) const {return(Stage);};
		int Fetch_Rate(void) const {return(Rate);};
		void Set_Stage(int stage) {Stage = stage;};
		void Set_Rate(int rate) {Timer = rate; Rate = rate;};
		void AI(void) {};
		bool About_To_Change(void) const {return(Timer == 0 && Rate != 0);}
		bool Graphic_Logic(void) {
			if (About_To_Change()) {
				Stage++;
				Timer = Rate;
				return(true);
			}
			return(false);
		};
};


#endif
