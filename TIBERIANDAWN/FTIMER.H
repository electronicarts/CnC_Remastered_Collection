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

/* $Header:   F:\projects\c&c\vcs\code\ftimer.h_v   2.14   16 Oct 1995 16:47:28   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : FTIMER.H                                                     * 
 *                                                                                             * 
 *                   Programmer : Joe L. Bostic                                                * 
 *                                                                                             * 
 *                   Start Date : 03/16/95                                                     * 
 *                                                                                             * 
 *                  Last Update : March 16, 1995 [JLB]                                         * 
 *                                                                                             * 
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef FTIMER_H
#define FTIMER_H

/*
**	This timer class is based around an external tick system. As such, it is inherently 
**	in sync with any connected system (through network or modem) that also keeps the external
**	tick system in sync. The game frame number is a good sync value.
*/
class TCountDownTimerClass {
	public:
		// Constructor.  Timers set before low level init has been done will not
		// be able to be 'Started' or 'on' until timer system is in place.	
		TCountDownTimerClass(long set=0) {
			Set(set);
		};

		// No destructor.
		~TCountDownTimerClass(void) {}

		operator long(void) const {return Time();};

		// Public functions
		void Set(long set) {
			Started = Frame;
			DelayTime = set;
		};		// Set count down value.

		void Clear(void) {
			Started   = -1;
			DelayTime = 0;
		};
		long Get_Start(void) const {
			return(Started);
		};
		long Get_Delay(void) const {
			return(DelayTime);
		};
		bool Active(void) const {
			return(Started != -1);
		};
		int Expired(void) const {return (Time() == 0);};
		long Time(void) const {
			long remain = DelayTime - (Frame-Started);
			if (remain < 0) remain = 0;
			return(remain);
		};			// Fetch current count down value.

	protected:
		long Started;				// Initial frame time start.
		long DelayTime;			// Ticks remaining before countdown timer expires.
};


#endif
