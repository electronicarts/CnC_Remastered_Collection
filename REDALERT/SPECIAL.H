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

/* $Header: /CounterStrike/SPECIAL.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SPECIAL.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 02/27/95                                                     *
 *                                                                                             *
 *                  Last Update : February 27, 1995 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef SPECIAL_H
#define SPECIAL_H

class SpecialClass
{
	public:

		/*
		**	This initializes all members just like a constructor. A constructor
		**	cannot be used for this class because it is part of a union.
		*/
		void Init(void);

		/*
		**	If the shroud should regenerated, then this flag will be true.
		*/
		unsigned IsShadowGrow:1;

		/*
		**	Controls the speedy build option -- used for testing.
		*/
		unsigned IsSpeedBuild:1;

		/*
		** If from install, then play the special installation movie and
		** skip asking them what type of game they want to play.
		*/
		unsigned IsFromInstall:1;

		/*
		**	If capture the flag mode is on, this flag will be true. With this
		**	flag enabled, then the flag is initially placed at the start of
		**	the scenario.
		*/
		unsigned IsCaptureTheFlag:1;

		/*
		**	This flags controls whether weapons are inert. An inert weapon doesn't do any
		**	damage. Effectively, if this is true, then the units never die.
		*/
		unsigned IsInert:1;

		/*
		**	If wheeled vehicles should do a 3-point turn when rotating in place, then
		**	this flag is true.
		*/
		unsigned IsThreePoint:1;

		/*
		**	If Tiberium is allowed to spread and grow, then these flags will be true.
		**	These are duplicated from the rules.ini file and also controlled by the
		**	multiplayer dialog box.
		*/
		unsigned IsTGrowth:1;
		unsigned IsTSpread:1;

		/*
		** Flag that we were spawned from WChat.
		*/
		unsigned IsFromWChat:1;

		/*
		**	If this flag is true, then the construction yard can undeploy back into an MCV.
		**	Used to override the rules setting.
		*/
		unsigned UseMCVDeploy:1;
		unsigned IsMCVDeploy:1;

		/*
		** New anti-griefing early win mode. ST - 1/31/2020 3:42PM
		*/
		unsigned IsEarlyWin:1;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[128];
};


#endif