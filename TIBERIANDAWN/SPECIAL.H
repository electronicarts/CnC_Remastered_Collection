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

/* $Header:   F:\projects\c&c\vcs\code\special.h_v   2.15   16 Oct 1995 16:47:36   JOE_BOSTIC  $ */
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
		void Init(void) {
			IsScrollMod = false;
			IsGross = false;
			IsEasy = false;
			IsDifficult = false;
			IsSpeedBuild = false;
			IsDefenderAdvantage = true;
			IsVisibleTarget = false;
			IsVariation = false;
			IsJurassic = false;
			IsJuvenile = false;
			IsSmartDefense = false;
			IsTreeTarget = false;
			IsMCVDeploy = false;
			IsVisceroids = false;
			IsMonoEnabled = false;
			IsInert = false;
			IsShowPath = false;
			IsThreePoint = false;
			IsTGrowth = true;
			IsTSpread = true;
			IsTFast = true;
			IsRoad = false;
			IsScatter = false;
			IsCaptureTheFlag = false;
			IsNamed = false;
			IsFromInstall = false;
			IsSeparate = false;
			IsFromWChat = false;
			IsEarlyWin = false;
			HealthBarDisplayMode = HB_SELECTED;
			ResourceBarDisplayMode = RB_SELECTED;
		}

		/*
		**	Is the game flagged for easy mode?
		*/
		unsigned IsEasy:1;

		/*
		**	Is the game flagged for difficult?
		*/
		unsigned IsDifficult:1;

		/*
		**	Controls the speedy build option -- used for testing.
		*/
		unsigned IsSpeedBuild:1;

		/*
		**	If the player can build the helipad separate from the helipad and
		**	helicopter combo, then this flag will be true.
		*/
		unsigned IsSeparate:1;

		/*
		**	If the defender has the advantage then this will be true. This flag
		**	allows the defender to have a better advantage in combat than the
		**	attacker. Moving units will not be able to dish out or take as much
		**	damage when this flag is true.
		*/
		unsigned IsDefenderAdvantage:1;

		/*
		**	If civilian structures are to have a name, then this flag will be
		**	set to true. The default case is to just use generic names for
		**	civilians.
		*/
		unsigned IsNamed:1;

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
		**	Is target selecting by other human opponents visible to the player?
		*/
		unsigned IsVisibleTarget:1;

		/*
		**	If human generated sound effects are to be used, then this
		**	flag will be true.
		*/
		unsigned IsJuvenile:1;

		/*
		**	If friendly units should return fire when fired upon, set this
		**	flag to true. The default is only for the enemy units to do this.
		*/
		unsigned IsSmartDefense:1;

		/*
		**	If targeting of trees is allowed, then this flag will be true.
		*/
		unsigned IsTreeTarget:1;

		/*
		**	If this flag is true, then the construction yard can undeploy back into an MCV.
		*/
		unsigned IsMCVDeploy:1;

		/*
		**	Controls whether or not visceroids spawn
		*/
		unsigned IsVisceroids:1;

		/*
		**	If the monochrome debugging output is enabled, then this flag will be true.
		*/
		unsigned IsMonoEnabled:1;

		/*
		**	This flags controls whether weapons are inert. An inert weapon doesn't do any
		**	damage. Effectively, if this is true, then the units never die.
		*/
		unsigned IsInert:1;

		/*
		**	When this flag is true, the computer findpath algorithm reveals the route being
		**	examined. This is used to trace findpath bugs.
		*/
		unsigned IsShowPath:1;

		/*
		**	If wheeled vehicles should do a 3-point turn when rotating in place, then
		**	this flag is true.
		*/
		unsigned IsThreePoint:1;

		/*
		**	If Tiberium is allowed to grow, then this flag will be true.
		*/
		unsigned IsTGrowth:1;

		/*
		**	If Tiberium is allowed to spread, then this flag will be true.
		*/
		unsigned IsTSpread:1;

		/*
		**	This controls whether Tiberium grows&spreads quickly or not.
		*/
		unsigned IsTFast:1;

		/*
		**	This flag controls whether the road additional pieces are added to
		**	the bottom of buildings. If true, then the roads are NOT added.
		*/
		unsigned IsRoad:1;

		/*
		**	Controls whether units (especially infantry) will scatter when there
		**	is an immediate threat. This gives infantry a "mind of their own" when
		**	it comes to self preservation. If set to false, then units will not
		**	scatter.
		*/
		unsigned IsScatter:1;

		/*
		**	Special bonus scenario enabled.
		*/
		unsigned IsJurassic:1;

		/*
		**	Are score variations allowed?
		*/
		unsigned IsVariation:1;

		/*
		**	If the gross human splatter marks should be present.
		*/
		unsigned IsGross:1;

		/*
		**	Disables scrolling over the "options" and "sidebar" tabs.
		*/
		unsigned IsScrollMod:1;

		/*
		** Flag that we were spawned from WChat.
		*/
		unsigned IsFromWChat:1;

		/*
		** New anti-griefing early win mode. ST - 1/31/2020 3:42PM
		*/
		unsigned IsEarlyWin:1;

		/*
		** Health bar display mode
		*/
		enum eHealthBarDisplayMode
		{
			HB_DAMAGED = 0,
			HB_ALWAYS,
			HB_SELECTED
		} HealthBarDisplayMode;

		/*
		** Resource bar display mode
		*/
		enum eResourceBarDisplayMode
		{
			RB_SELECTED = 0,
			RB_ALWAYS,
		} ResourceBarDisplayMode;


		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[128];
};


#endif