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

/* $Header: /CounterStrike/SCENARIO.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SCENARIO.H                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 02/26/96                                                     *
 *                                                                                             *
 *                  Last Update : February 26, 1996 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef SCENARIO_H
#define SCENARIO_H


/*
**	This class holds the information about the current game being played. This information is
**	global to the scenario and is generally of a similar nature to the information that was held
**	in the controlling scenario INI file. It is safe to write this structure out as a whole since
**	it doesn't contain any embedded pointers.
*/
class ScenarioClass {
	public:

		// Constructor.
		ScenarioClass(void);
		void Set_Scenario_Name(int scenario, ScenarioPlayerType player, ScenarioDirType dir = SCEN_DIR_NONE, ScenarioVarType var = SCEN_VAR_NONE);
		void Set_Scenario_Name(char const * name);

		bool Set_Global_To(int global, bool value);
		void Do_BW_Fade(void);
		void Do_Fade_AI(void);

		/*
		**	This is the source of the random numbers used in the game. This controls
		**	the game logic and thus must be in sync with any networked machines.
		*/
		RandomClass RandomNumber;

		/*
		**	This is the difficulty setting of the game.
		*/
		DiffType Difficulty;			// For human player.
		DiffType CDifficulty;		// For computer players.

		/*
		**	This is the main mission timer. This is the timer that is reset at the
		**	start of the mission. It, effectively, holds the elapsed time of the
		**	mission.
		*/
		TTimerClass<FrameTimerClass> Timer;

		/*
		**	This is an array of waypoints; each waypoint corresponds to a letter of
		** the alphabet, and points to a cell number.  -1 means unassigned.
		** The CellClass has a bit that tells if that cell has a waypoint attached to
		** it; the only way to find which waypoint it is, is to scan this array.  This
		** shouldn't be needed often; usually, you know the waypoint & you want the CELL.
		*/
		CELL Waypoint[WAYPT_COUNT];

		/*
		**	This holds the system wide mission countdown timer. Time based missions
		**	are governed by this timer. Various trigger events can modify and examine
		**	this timer. The current value of this timer will display on the game
		**	screen.
		*/
		CDTimerClass<FrameTimerClass> MissionTimer;

		/*
		**	The shroud regrowth (if enabled) is regulated by this timer. When the
		**	timer expires, the shroud will regrow one step.
		*/
		CDTimerClass<FrameTimerClass> ShroudTimer;

		/*
		**	The scenario number.
		*/
		int Scenario;

		/*
		**	The theater of the current scenario.
		*/
		TheaterType Theater;

		/*
		**	The full name of the scenario (as it exists on disk).
		*/
		char ScenarioName[_MAX_FNAME+_MAX_EXT];

		/*
		**	Description of the scenario.
		*/
		char Description[DESCRIP_MAX];

		/*
		**	The filename of the introduction movie.
		*/
		VQType IntroMovie;

		/*
		**	The filename of the briefing movie.
		*/
		VQType BriefMovie;

		/*
		**	The filename of the movie to play if the scenario is won.
		*/
		VQType WinMovie;
		/*
		**	The filename of the movie to play if the scenario is won.
		*/
		VQType WinMovie2;
		/*
		**	The filename of the movie to play if the scenario is won.
		*/
		VQType WinMovie3;
		/*
		**	The filename of the movie to play if the scenario is won.
		*/
		VQType WinMovie4;

		/*
		**	The filename of the movie to play if the scenario is lost.
		*/
		VQType LoseMovie;

		/*
		**	The filename of the movie to play right after the briefing and
		**	just before the game.
		*/
		VQType ActionMovie;

		/*
		**	This is the full text of the briefing. This text will be
		**	displayed when the player commands the "restate mission
		**	objectives" operation.
		*/
		char BriefingText[1024];

		/*
		**	This is the theme to start playing at the beginning of the action
		**	movie. A score started in this fashion will continue to play as
		**	the game progresses.
		*/
		ThemeType TransitTheme;

		/*
		**	The house that the player is to be (obsolete).
		*/
		HousesType PlayerHouse;

		/*
		**	The percentage of money that is allowed to be carried over into the
		**	following scenario.
		*/
		fixed CarryOverPercent;

		/*
		**	This is the amount of money that was left over in the previous
		**	scenario.
		*/
		int CarryOverMoney;

		/*
		**	This specifies the maximum amount of money that is allowed to be
		**	carried over from the previous scenario. This limits the amount
		**	regardless of what the carry over percentage is set to.
		*/
		int CarryOverCap;

		/*
		**	This is the percent that the computer controlled base is to be
		**	built up to at the scenario start.
		*/
		int Percent;

		/*
		**	Global flags that are used in the trigger system and are persistent
		**	over the course of the game.
		*/
		bool GlobalFlags[30];

		/*
		**	This records the bookmark view locations the player has recorded.
		*/
		CELL Views[4];

		/*
		**	This is the number of active passable bridges in the current game.
		*/
		int BridgeCount;

		/*
		**	This records the carry over timer value that is used when the mission
		**	starts (presuming the appropriate flag is set) and also used when the
		**	scenario restarts.
		*/
		int CarryOverTimer;

		/*
		**	If a bridge has been destroyed, then this flag will be set to true.
		**	If there is a trigger that depends on this, it might be triggered.
		*/
		unsigned IsBridgeChanged:1;

		/*
		**	If a global has changed and global change trigger events must be
		**	processed, then this flag will be set to true.
		*/
		unsigned IsGlobalChanged:1;

		/*
		**	Are the buildings and units in this scenario to carry over into
		**	some (unspecified) later scenario and thus have to be recorded
		**	at the end?
		*/
		unsigned IsToCarryOver:1;

		/*
		**	Is this scenario to begin by taking the previously recorded
		**	carryover objects and creating them onto the map?
		*/
		unsigned IsToInherit:1;

		/*
		**	If Tanya or a civilian is to be automatically evacuated when they enter
		**	a transport vehicle, then this flag will be true.
		*/
		unsigned IsTanyaEvac:1;

		/*
		** These variables are assigned to the chronosphere effect, and control
		** whether the palette should be fading towards b&w or towards color.
		*/
		unsigned IsFadingBW:1;
		unsigned IsFadingColor:1;

		/*
		**	If this scenario is to be the last mission of the game (for this side), then
		**	this flag will be true.
		*/
		unsigned IsEndOfGame:1;

		/*
		**	If the mission countdown timer is to be inherited from the previous
		**	scenario, then this flag will be set to true.
		*/
		unsigned IsInheritTimer:1;

		/*
		**	If the spy plane is to be disabled in this scenario even though circumstances
		**	might otherwise indicate that it should appear, then this flag will be true.
		*/
		unsigned IsNoSpyPlane:1;

		/*
		**	If the score screen (and "mission accomplished" voice) is to be skipped when
		**	this scenario is finished, then this flag will be true.
		*/
		unsigned IsSkipScore:1;

		/*
		**	If this is to be a one time only mission such that when it is completed, the game
		**	will return to the main menu, then this flag will be set to true.
		*/
		unsigned IsOneTimeOnly:1;

		/*
		**	If the map selection is to be skipped then this flag will be true. If this
		**	ins't a one time only scenario, then the next scenario will have the same
		**	name as the current one but will be for variation "B".
		*/
		unsigned IsNoMapSel:1;

		/*
		**	If trucks are supposed to drop wood crates when they explode, then this flag
		**	will be set to true.
		*/
		unsigned IsTruckCrate:1;

		/*
		**	If the initial money is to be assigned as ore in available silos, then
		**	this flag will be set to true.
		*/
		unsigned IsMoneyTiberium:1;

		/*
		**	This is the fading countdown timer.  As this timer counts down, the
		**	fading to b&w or color will progress.  This timer represents a
		** percentage of the Options.Get_Saturation() to fade towards.
		*/
		CDTimerClass<FrameTimerClass> FadeTimer;

#ifdef FIXIT_VERSION_3			//	For endgame auto-sonar pulse.
		//	Timer to set the period for checking if an auto-sonar pulse should be performed.
		//	This will take place if a player has nothing but subs left in the game.
		CDTimerClass<FrameTimerClass> AutoSonarTimer;
#endif

#ifdef FIXIT_VERSION_3		//	Stalemate games.
		bool	bLocalProposesDraw;				//	True if the local player in a 2-player game has a draw offer extended.
		bool	bOtherProposesDraw;				//	True if the other player in a 2-player game has a draw offer extended.
#endif

};


#endif
