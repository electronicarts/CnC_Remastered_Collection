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


/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Library profiler                                             *
 *                                                                                             *
 *                    File Name : PROFILE.H                                                    *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : 11/17/95                                                     *
 *                                                                                             *
 *                  Last Update : November 20th 1995 [ST]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Overview:                                                                                   *
 *                                                                                             *
 *  New System                                                                                 *
 * ~~~~~~~~~~~                                                                                 *
 *                                                                                             *
 *  The new profiler system creates a seperate thread and then starts a timer off there. The   *
 *  timer in the second thread uses GetThreadContext to sample the IP address of each user     *
 *  thread. This system has the advantage of being able to sample what is happening in all the *
 *  threads we own not just the main thread. Another advantage is that it doesnt require a     *
 *  major recompilation.                                                                       *
 *  The disadvantage is that we dont really know what is going on when the IP is outside the   *
 *  scope of our threads - We could be in direct draw, direct sound or even something like the *
 *  VMM and there is no way to tell.                                                           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 *  Old System                                                                                 *
 * ~~~~~~~~~~~                                                                                 *
 *                                                                                             *
 *  The profiler works by using the function prologue and epilogue hooks available in Watcom   *
 *  to register the current functions address in a global variable and then sampling the       *
 *  contents of the variable using a windows timer which runs at up to 1000 samples per second.*
 *                                                                                             *
 *  Compile the code to be sampled with the -ep and -ee flags to enable the prologue (__PRO)   *
 *  and epilogue (__EPI) calls to be generated.                                                *
 *  At the beginning of the section to be profiled (just before main loop normally) call the   *
 *  Start_Profiler function to start sampling. At the end of the section, call Stop_Profiler   *
 *  which will stop the timer and write the profile data to disk in the PROFILE.BIN file.      *
 *                                                                                             *
 *  Use PROFILE.EXE to view the results of the session.                                        *
 *                                                                                             *
 *  The addition of prologue and epilogue code will slow down the product and the profiler     *
 *  allocates a huge buffer for data so it should not be linked in unless it is going to be    *
 *  used.                                                                                      *
 *                                                                                             *
 *  The advantage of the prologue/epilogue approach is that all samples represent valid        *
 *  addresses within our code so we get valid results we can use even when the IP is in system *
 *  code.                                                                                      *
 *                                                                                             *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 * Functions:                                                                                  *
 *                                                                                             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#define	MAX_PROFILE_TIME	60*1		//1 minute(s) @ 14.4 Mb per hour
#define	PROFILE_RATE		1000	//samples per sec (max 1000)


/*
 * Defines for choosing between the old and new profiler system
 *
*/

#define	OLD_PROFILE_SYSTEM	1
#define	NEW_PROFILE_SYSTEM	2

//#define	PROFILE_SYSTEM			OLD_PROFILE_SYSTEM
#define	PROFILE_SYSTEM			NEW_PROFILE_SYSTEM



extern "C"{
	void __cdecl Profile_Init(void);
	void __cdecl Profile_End(void);
	void __cdecl Start_Profiler(void);
	void __cdecl Stop_Profiler(void);
}

