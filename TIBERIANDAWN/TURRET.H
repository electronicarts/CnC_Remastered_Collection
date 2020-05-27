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

/* $Header:   F:\projects\c&c\vcs\code\turret.h_v   2.17   16 Oct 1995 16:48:00   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TURRET.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 25, 1994                                               *
 *                                                                                             *
 *                  Last Update : April 25, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef TURRET_H
#define TURRET_H

#include "drive.h"

class TurretClass : public DriveClass
{
	public:

		/*
		**	This is the timer that controls the reload rate. The MSAM rocket
		**	launcher is the primary user of this.
		*/
		TCountDownTimerClass Reload;

		/*
		**	This is the facing of the turret. It can be, and usually is,
		**	rotated independently of the body it is attached to.
		*/
		FacingClass SecondaryFacing;

		#ifdef CHEAT_KEYS
		virtual void Debug_Dump(MonoClass *mono) const;
		#endif
		virtual bool Unlimbo(COORDINATE , DirType facing=DIR_N);

		/*
		**	File I/O.
		*/
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

	protected:
		TurretClass(UnitType classid, HousesType house);
		TurretClass(void);
		virtual ~TurretClass(void);

		BulletClass * Fire_At(TARGET target, int which);

		virtual DirType Fire_Direction(void) const;
		virtual FireErrorType Can_Fire(TARGET target, int which) const;
		virtual bool Ok_To_Move(DirType facing);
		virtual void AI(void);
		virtual COORDINATE Fire_Coord(int which) const;
};


#endif
