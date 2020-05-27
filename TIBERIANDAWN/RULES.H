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

/* $Header: /CounterStrike/RULES.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RULES.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/12/96                                                     *
 *                                                                                             *
 *                  Last Update : May 12, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef RULES_H
#define RULES_H

class DifficultyClass
{
	public:
		DifficultyClass(void);

		float FirepowerBias;
		float GroundspeedBias;
		float AirspeedBias;
		float ArmorBias;
		float ROFBias;
		float CostBias;
		float BuildSpeedBias;

		float RepairDelay;
		float BuildDelay;

		unsigned IsBuildSlowdown:1;
		unsigned IsWallDestroyer:1;
		unsigned IsContentScan:1;
};

class RulesClass {
	public:

		RulesClass(void);

		/*
		**	The computer is limited in the size of the base it can build. It is limited to the
		**	size of the largest human opponent base plus this surplus count.
		*/
		int BaseSizeAdd;

		/*
		**	If the power surplus is less than this amount, then the computer will
		**	build power plants.
		*/
		int PowerSurplus;

		/*
		**	This specifies the average number of minutes between each computer attack.
		*/
		int AttackInterval;

		/*
		**	This specifies the average minutes delay before the computer will begin
		**	its first attack upon the player. The duration is also modified by the
		**	difficulty level.
		*/
		int AttackDelay;

		/*
		**	If the power ratio falls below this percentage, then a power emergency is
		**	in effect. At such times, the computer might decide to sell off some
		**	power hungry buildings in order to alleviate the situation.
		*/
		unsigned short PowerEmergencyFraction;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of helipads.
		*/
		unsigned short HelipadRatio;

		/*
		**	Limit the number of helipads to this amount.
		*/
		int HelipadLimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of Tesla Coils.
		*/
		unsigned short TeslaRatio;

		/*
		**	Limit tesla coil production to this maximum.
		*/
		int TeslaLimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of anti-aircraft defense.
		*/
		unsigned short AARatio;

		/*
		**	Limit anti-aircraft building quantity to this amount.
		*/
		int AALimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of defensive structures.
		*/
		unsigned short DefenseRatio;

		/*
		**	This is the limit to the number of defensive building that can be built.
		*/
		int DefenseLimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of war factories.
		*/
		unsigned short WarRatio;

		/*
		**	War factories are limited to this quantity for the computer controlled player.
		*/
		int WarLimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of infantry producing structures.
		*/
		unsigned short BarracksRatio;

		/*
		**	No more than this many barracks can be built.
		*/
		int BarracksLimit;

		/*
		**	Refinery building is limited to this many refineries.
		*/
		int RefineryLimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of refineries.
		*/
		unsigned short RefineryRatio;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of airstrips.
		*/
		unsigned short AirstripRatio;

		/*
		**	Limit the number of airstrips to this amount.
		*/
		int AirstripLimit;

		/*
		**	This is the maximum number of IQ settings available. The human player is
		**	presumed to be at IQ level zero.
		*/
		int MaxIQ;

		/*
		**	The IQ level at which super weapons will be automatically fired by the computer.
		*/
		int IQSuperWeapons;

		/*
		**	The IQ level at which production is automatically controlled by the computer.
		*/
		int IQProduction;

		/*
		**	The IQ level at which newly produced units start out in guard area mode instead
		**	of normal guard mode.
		*/
		int IQGuardArea;

		/*
		**	The IQ level at which the computer will be able to decide what gets repaired
		**	or sold.
		*/
		int IQRepairSell;

		/*
		**	At this IQ level or higher, a unit is allowed to automatically try to crush
		**	an atagonist if possible.
		*/
		int IQCrush;

		/*
		**	The unit/infantry will try to scatter if an incoming threat
		**	is detected.
		*/
		int IQScatter;

		/*
		**	Tech level at which the computer will scan the contents of a transport
		**	in order to pick the best target to fire upon.
		*/
		int IQContentScan;

		/*
		**	Aircraft replacement production occurs at this IQ level or higher.
		*/
		int IQAircraft;

		/*
		**	Checks for and replaces lost harvesters.
		*/
		int IQHarvester;

		/*
		**	Is allowed to sell a structure being damaged.
		*/
		int IQSellBack;

		/*
		**	The computer will build infantry if their cash reserve is greater than this amount.
		*/
		int InfantryReserve;

		/*
		**	This factor is multiplied by the number of buildings in the computer's base and infantry
		**	are always built until it matches that number.
		*/
		int InfantryBaseMult;

		/*
		**	Is the computer paranoid? If so, then it will band together with other computer
		**	paranoid players when the situation looks rough.
		*/
		bool IsComputerParanoid;

		/*
		**	Are superweapons allowed?
		*/
		bool AllowSuperWeapons;

		/*
		**	This array controls the difficulty affects on the game. There is one
		**	difficulty class object for each difficulty level.
		*/
		DifficultyClass Diff[DIFF_COUNT];
};


#endif
