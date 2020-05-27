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

/***************************************************************************
 **     C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S       **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Temp timer for 32bit lib                 *
 *                                                                         *
 *                    File Name : TIMER.CPP                                *
 *                                                                         *
 *                   Programmer : Scott K. Bowen                           *
 *                                                                         *
 *                   Start Date : July 6, 1994                             *
 *                                                                         *
 *                  Last Update : May 3, 1995   [SKB]                      *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   TC::Time -- Return the time on the timer.                             *
 *   TC::TimerClass -- Construct a timer class object.                     *
 *   TC::Stop -- Stop the timer.                                           *
 *   TC::Start -- Start a timer.                                           *
 *   TC::Set -- Set the time of a timer.                                   *
 *   TC::Reset -- Clear the timer.                                         *
 *   TimerClass::Time -- Get the current time of timer.                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <wwstd.h>
#include "timer.H"
#include <stdio.h>
#include <stdlib.h>


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Code ////////////////////////////////////////

/***************************************************************************
 * TC::TIMERCLASS -- Construct a timer class object.                       *
 *                                                                         *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/12/1994 SKB : Created.                                             *
 *=========================================================================*/
TimerClass::TimerClass(BaseTimerEnum timer, BOOL on)
{
	Accumulated = 0;
	Started = 0;

	TickType=timer;

	if (on && TimerSystemOn) Start();
}




/***********************************************************************************************
 * TC:Get_Ticks -- return the number of ticks on the system or user timers                     *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   tick count                                                                        *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *              10/5/95 4:17PM ST : Created                                                    *
 *=============================================================================================*/

long TimerClass::Get_Ticks ( void )

{
	if ( WindowsTimer ){
		switch ( TickType ){

		 	case BT_SYSTEM :
				return ( WindowsTimer->Get_System_Tick_Count() );

		 	case BT_USER :
				return ( WindowsTimer->Get_User_Tick_Count() );

		}
	}
	return 0;
}



/***************************************************************************
 * TIMERCLASS::TIME -- Get the current time of timer.                      *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/03/1995 SKB : Created.                                             *
 *=========================================================================*/
long TimerClass::Time(void)
{
	if (Started) {
		long ticks = Get_Ticks();
		Accumulated += ticks - (Started-1);
		Started = ticks+1;
	}
	return(Accumulated);
}


/***************************************************************************
 * TC::STOP -- Stop the timer.                                             *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/12/1994 SKB : Created.                                             *
 *=========================================================================*/
long TimerClass::Stop(void)
{
	long time = Time();
	Started = 0;
	return(time);
}


/***************************************************************************
 * TC::START -- Start a timer.                                             *
 *                                                                         *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/12/1994 SKB : Created.                                             *
 *=========================================================================*/
long TimerClass::Start(void)
{
	if (!Started) {
		Started = Get_Ticks()+1;
	}
	return(Time());
}


/***************************************************************************
 * TC::SET -- Set the time of a timer.                                     *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:  long value to set timer at.                                     *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/12/1994 SKB : Created.                                             *
 *   05/03/1995 SKB : If start return Start since it returns Time          *
 *=========================================================================*/
long TimerClass::Set(long value, BOOL start)
{
	Started = 0;
	Accumulated = value;
	if (start) return (Start());
	return(Time());
}