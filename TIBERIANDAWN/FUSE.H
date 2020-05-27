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

/* $Header:   F:\projects\c&c\vcs\code\fuse.h_v   2.17   16 Oct 1995 16:46:18   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : FUSE.H                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 24, 1994                                               *
 *                                                                                             *
 *                  Last Update : April 24, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef FUSE_H
#define FUSE_H

/****************************************************************************
**	The fuse is used by projectiles to determine whether detonation should
**	occur. This is usually determined by tracking the distance to the
**	designated target reaches zero or when the timer expires.
*/
class FuseClass {
	public:
		FuseClass(void);
		void Arm_Fuse(COORDINATE location, COORDINATE target, int time=0xFF, int arming=0);
		bool Fuse_Checkup(COORDINATE newlocation);
		void Fuse_Write(FileClass & file);
		void Fuse_Read(FileClass & file);
		COORDINATE Fuse_Target(void);

		/*
		**	File I/O.
		*/
		void Code_Pointers(void);
		void Decode_Pointers(void);

		/*
		**	Fuses can detonate if enough time has elapsed. This value counts
		**	down. When it reaches zero, detonation occurs.
		*/
		unsigned char Timer;

	private:

		/*
		**	Some fuses need a certain amount of time before detonation can
		**	occur. This counts down and when it reaches zero, normal fuse
		**	detonation checking can occur.
		*/
		unsigned char Arming;

		/*
		**	This is the designated impact point of the projectile. The fuse
		**	will trip when the closest point to this location has been reached.
		*/
		COORDINATE HeadTo;

		/*
		**	This is the running proximity value to the impact point. This value
		**	will progressively get smaller. Detonation occurs when it reaches
		**	zero or when it starts to grow larger.
		*/
		short Proximity;
};

inline COORDINATE FuseClass::Fuse_Target(void)
{
	return(HeadTo);
}

#endif
