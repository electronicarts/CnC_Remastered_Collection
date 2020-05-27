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

/* $Header: /CounterStrike/FUSE.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : FUSE.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 24, 1994                                               *
 *                                                                                             *
 *                  Last Update : October 17, 1994   [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   FuseClass::Arm_Fuse -- Sets up fuse for detonation check.                                 *
 *   FuseClass::Fuse_Checkup -- Determines if the fuse triggers.                               *
 *   FuseClass::Fuse_Read -- Reads the fuse class data from the save game file.                *
 *   FuseClass::Fuse_Write -- Writes the fuse data to the save game file.                      *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***********************************************************************************************
 * FuseClass::FuseClass -- Constructor.                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/27/1995 BRR : Created.  Gosh, what a lotta work.                                       *
 *=============================================================================================*/
FuseClass::FuseClass(void)
{
	Timer = 0;
	Arming = 0;
	HeadTo = 0;
	Proximity = 0;
}


/***********************************************************************************************
 * FuseClass::Arm_Fuse -- Sets up fuse for detonation check.                                   *
 *                                                                                             *
 *    This starts a fuse. Fuses are proximity detonation variety but                           *
 *    can be modified to have a minimum time to elapse before detonation                       *
 *    and a maximum time to exist before detonation. Typically, the                            *
 *    timing values are used for missiles that have a minimum arming                           *
 *    distance and a limited amount of fuel.                                                   *
 *                                                                                             *
 * INPUT:   location -- The coordinate where the projectile start. This                        *
 *                      is needed for proper proximity tracking.                               *
 *                                                                                             *
 *          target   -- The actual impact point. Fuses are based on real                       *
 *                      word coordinates.                                                      *
 *                                                                                             *
 *          time     -- The maximum time that the fuse may work before                         *
 *                      explosion is forced.                                                   *
 *                                                                                             *
 *          arming   -- The minimum time that must elapse before the                           *
 *                      fuse may explode.                                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void FuseClass::Arm_Fuse(COORDINATE location, COORDINATE target, int timeto, int arming)
{
	timeto = max(timeto, arming);
	Timer = min(timeto, 0xFF);
	Arming = min(arming, 0xFF);
	HeadTo = target;
	Proximity = Distance(location, target);
}


/***********************************************************************************************
 * FuseClass::Fuse_Checkup -- Determines if the fuse triggers.                                 *
 *                                                                                             *
 *    This will process the fuse and update the internal clocks as well                        *
 *    as check to see if the fuse should trigger (explode) or not.                             *
 *                                                                                             *
 * INPUT:   newlocation -- The new location of the fuse. This is needed                        *
 *                         to determine proximity explosions.                                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the fuse triggered to explode now?                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FuseClass::Fuse_Checkup(COORDINATE newlocation)
{
	int	proximity;

	/*
	**	Always decrement the fuse timer.
	*/
	if (Timer) Timer--;

	/*
	**	If the arming countdown has not expired, then do nothing.
	*/
	if (Arming) {
		Arming--;
	} else {

		/*
		**	If the timer has run out, then the warhead explodes.
		*/
		if (!Timer) return(true);

		proximity = Distance(newlocation, HeadTo);
		if (proximity < 0x0010) return(true);
		if (proximity < ICON_LEPTON_W && proximity > Proximity) {
			return(true);
		}
		Proximity = proximity;
	}
	return(false);
}


/***********************************************************************************************
 * FuseClass::Fuse_Write -- Writes the fuse data to the save game file.                        *
 *                                                                                             *
 *    Use this routine to output the fuse class data to the save game file specified.          *
 *                                                                                             *
 * INPUT:   file  -- The file to output the data to.                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void FuseClass::Fuse_Write(FileClass & file)
{
	file.Write(&Timer, sizeof(Timer));
	file.Write(&Arming, sizeof(Arming));
	file.Write(&HeadTo, sizeof(HeadTo));
	file.Write(&Proximity, sizeof(Proximity));
}


/***********************************************************************************************
 * FuseClass::Fuse_Read -- Reads the fuse class data from the save game file.                  *
 *                                                                                             *
 *    Use this routine to input the fuse class data from the save game file specified.         *
 *                                                                                             *
 * INPUT:   file  -- The file to input the data from.                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void FuseClass::Fuse_Read(FileClass & file)
{
	file.Read(&Timer, sizeof(Timer));
	file.Read(&Arming, sizeof(Arming));
	file.Read(&HeadTo, sizeof(HeadTo));
	file.Read(&Proximity, sizeof(Proximity));
}


