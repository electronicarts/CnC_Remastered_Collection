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

//Mono_Printf("%d %s\n",__LINE__,__FILE__);
/* $Header: /CounterStrike/SCENARIO.CPP 15    3/13/97 2:06p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SCENARIO.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : October 21, 1996 [JLB]                                       *
 *                                                                                             *
 * This module handles the scenario reading and writing. Scenario related                      *
 * code that is executed between scenario play can also be here.                               *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Assign_Houses -- Assigns multiplayer houses to various players                            *
 *   Clear_Flag_Spots -- Clears flag overlays off the map                                      *
 *   Clear_Scenario -- Clears all data in preparation for scenario load.                       *
 *   Clip_Move -- moves in given direction from given cell; clips to map                       *
 *   Clip_Scatter -- randomly scatters from given cell; won't fall off map                     *
 *   Create_Units -- Creates infantry & units, for non-base multiplayer                        *
 *   Do_Lose -- Display losing comments.                                                       *
 *   Do_Restart -- Handle the restart mission process.                                         *
 *   Do_Win -- Display winning congratulations.                                                *
 *   Fill_In_Data -- Recreate all data that is not loaded with scenario.                       *
 *   Post_Load_Game -- Fill in an inferred data from the game state.                           *
 *   Read_Scenario -- Reads a scenario from disk.                                              *
 *   Read_Scenario_INI -- Read specified scenario INI file.                                    *
 *   Remove_AI_Players -- Removes the computer AI houses & their units                         *
 *   Restate_Mission -- Handles restating the mission objective.                               *
 *   Scan_Place_Object -- places an object >near< the given cell                               *
 *   ScenarioClass::ScenarioClass -- Constructor for the scenario control object.              *
 *   ScenarioClass::Set_Global_To -- Set scenario global to value specified.                   *
 *   Set_Scenario_Name -- Creates the INI scenario name string.                                *
 *   Start_Scenario -- Starts the scenario.                                                    *
 *   Write_Scenario_INI -- Write the scenario INI file.                                        *
 *   ScenarioClass::Do_BW_Fade -- Cause the palette to temporarily shift to B/W.               *
 *   ScenarioClass::Do_Fade_AI -- Process the palette fading effect.                           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#ifdef WIN32
#include "tcpip.h"
#include "ccdde.h"

extern bool SpawnedFromWChat;
#endif
extern int PreserveVQAScreen;

void Display_Briefing_Text_GlyphX();

extern void GlyphX_Assign_Houses(void);	//ST - 8/8/2019 12:35PM
extern bool UseGlyphXStartLocations;		//ST - 3/31/2020 9:54AM

//#include "WolDebug.h"

#ifdef FIXIT_VERSION_3		//	Stalemate games.
#include "WolStrng.h"
#endif

static void Remove_AI_Players(void);
static void Create_Units(bool official);
static CELL Clip_Scatter(CELL cell, int maxdist);
static CELL Clip_Move(CELL cell, FacingType facing, int dist);

// Made this non-static so we can access it from the updated assign players function. ST - 8/9/2019 10:35AM
int _build_tech[11] = {
	2,2,			// Tech level 0 and 1 are the same (tech 0 is never used).
	4,
	5,
	7,
	8,
	9,
	10,
	11,
	12,
	13
};


#ifdef FRENCH
#define TXT_HACKHACK  "Accomplie"
#endif
#if defined(ENGLISH) || defined(GERMAN)
#define TXT_HACKHACK  Text_String(TXT_ACCOMPLISHED)
#endif

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
bool Is_Mission_Counterstrike (char *file_name);
bool Is_Mission_Aftermath (char *file_name);
#endif

/***********************************************************************************************
 * ScenarioClass::ScenarioClass -- Constructor for the scenario control object.                *
 *                                                                                             *
 *    This constructs the default scenario control object. Normally, all the default values    *
 *    are meaningless since the act of starting a scenario will fill in all of the values with *
 *    settings retrieved from the scenario control file.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
ScenarioClass::ScenarioClass(void) :
	Difficulty(DIFF_NORMAL),
	CDifficulty(DIFF_NORMAL),
	Timer(0),
	MissionTimer(0),
	ShroudTimer(TICKS_PER_MINUTE * Rule.ShroudRate),
	Scenario(1),
	Theater(THEATER_TEMPERATE),
	IntroMovie(VQ_NONE),
	BriefMovie(VQ_NONE),
	WinMovie(VQ_NONE),
	WinMovie2(VQ_NONE),
	WinMovie3(VQ_NONE),
	WinMovie4(VQ_NONE),
	LoseMovie(VQ_NONE),
	ActionMovie(VQ_NONE),
	TransitTheme(THEME_NONE),
	PlayerHouse(HOUSE_GREECE),
	CarryOverPercent(0),
	CarryOverMoney(0),
	CarryOverCap(0),
	Percent(0),
	BridgeCount(0),
	CarryOverTimer(0),
	IsBridgeChanged(false),
	IsGlobalChanged(false),
	IsToCarryOver(false),
	IsToInherit(false),
	IsTanyaEvac(false),
	IsFadingBW(false),
	IsFadingColor(false),
	IsEndOfGame(false),
	IsInheritTimer(false),
	IsNoSpyPlane(false),
	IsSkipScore(false),
	IsOneTimeOnly(false),
	IsNoMapSel(false),
	IsTruckCrate(false),
	IsMoneyTiberium(false),
#ifdef FIXIT_VERSION_3			//	For endgame auto-sonar pulse.
#define AUTOSONAR_PERIOD	TICKS_PER_SECOND * 40
	AutoSonarTimer( AUTOSONAR_PERIOD ),
#endif
	FadeTimer(0)
{
	for (int index = 0; index < ARRAY_SIZE(Waypoint); index++) {
		Waypoint[index] = -1;
	}
	strcpy(Description, "");
	strcpy(ScenarioName, "");
	strcpy(BriefingText, "");
	memset(GlobalFlags, '\0', sizeof(GlobalFlags));
	memset(Views, '\0', sizeof(Views));
}


/***********************************************************************************************
 * ScenarioClass::Do_BW_Fade -- Cause the palette to temporarily shift to B/W.                 *
 *                                                                                             *
 *    This routine will start the palette to fade to B/W for a brief moment.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void ScenarioClass::Do_BW_Fade(void)
{
	IsFadingBW = true;
	IsFadingColor = false;
	FadeTimer = GRAYFADETIME;
}


/***********************************************************************************************
 * ScenarioClass::Do_Fade_AI -- Process the palette fading effect.                             *
 *                                                                                             *
 *    This routine will handle the maintenance of the palette fading effect. It should be      *
 *    called once per game frame.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/21/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void ScenarioClass::Do_Fade_AI(void)
{
	if (IsFadingColor) {
		if (FadeTimer == 0) {
			IsFadingColor = false;
		}
		fixed newsat = Options.Get_Saturation() * fixed(GRAYFADETIME-FadeTimer, GRAYFADETIME);
		Options.Adjust_Palette(OriginalPalette, GamePalette, Options.Get_Brightness(), newsat, Options.Get_Tint(), Options.Get_Contrast());
		GamePalette.Set();
	}
	if (IsFadingBW) {
		if (FadeTimer == 0) {
			IsFadingBW = false;
		}
		fixed newsat = Options.Get_Saturation() * fixed(FadeTimer, GRAYFADETIME);
		Options.Adjust_Palette(OriginalPalette, GamePalette, Options.Get_Brightness(), newsat, Options.Get_Tint(), Options.Get_Contrast());
		GamePalette.Set();
		if (!IsFadingBW) {
			IsFadingColor = true;
			FadeTimer = GRAYFADETIME;
		}
	}
}


/***********************************************************************************************
 * ScenarioClass::Set_Global_To -- Set scenario global to value specified.                     *
 *                                                                                             *
 *    This routine will set the global flag to the falue (true/false) specified. It will       *
 *    also scan for and spring any triggers that are dependant upon that global.               *
 *                                                                                             *
 * INPUT:   global   -- The global flag to change.                                             *
 *                                                                                             *
 *          value    -- The value to change the global flag to.                                *
 *                                                                                             *
 * OUTPUT:  Returns with the previous value of the flag.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ScenarioClass::Set_Global_To(int global, bool value)
{
	if ((unsigned)global < ARRAY_SIZE(Scen.GlobalFlags)) {

		bool previous = GlobalFlags[global];
		if (previous != value) {
			GlobalFlags[global] = value;
			IsGlobalChanged = true;

			/*
			**	Special case to scan through all triggers and if any are found that depend on this
			**	global being set/cleared, then if there is an elapsed time event associated, it
			**	will be reset at this time.
			*/
			for (int index = 0; index < Triggers.Count(); index++) {
				TriggerClass * tp = Triggers.Ptr(index);
				if ((tp->Class->Event1.Event == TEVENT_GLOBAL_SET || tp->Class->Event1.Event == TEVENT_GLOBAL_CLEAR) && tp->Class->Event1.Data.Value == global) {
					tp->Class->Event2.Reset(tp->Event1);
				}
				if ((tp->Class->Event2.Event == TEVENT_GLOBAL_SET || tp->Class->Event2.Event == TEVENT_GLOBAL_CLEAR) && tp->Class->Event2.Data.Value == global) {
					tp->Class->Event1.Reset(tp->Event1);
				}
			}
		}
		return(previous);
	}
	return(false);
}


/***********************************************************************************************
 * Start_Scenario -- Starts the scenario.                                                      *
 *                                                                                             *
 *    This routine will start the scenario. In addition to loading the scenario data, it will  *
 *    play the briefing and action movies.                                                     *
 *                                                                                             *
 * INPUT:   root     -- Pointer to the filename root for this scenario (e.g., "SCG01EA").      *
 *                                                                                             *
 *          briefing -- Should the briefing be played? Normally this is true except when the   *
 *                      scenario is restarting.                                                *
 *                                                                                             *
 * OUTPUT:  Was the scenario started without error?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Start_Scenario(char * name, bool briefing)
{
//BG	Theme.Queue_Song(THEME_QUIET);
Theme.Stop();
	IsTanyaDead = SaveTanya;
	if (!Read_Scenario(name)) {
		return(false);
	}

	/*
	**	Play the winning movie and then start the next scenario.
	*/
	RequiredCD = -1;
//	if (RequiredCD != -2 && Session.Type == GAME_NORMAL) {
//		if (Scen.Scenario == 1)
//			RequiredCD = -1;
//		else {
//			 if((Scen.Scenario >= 20 && Scen.ScenarioName[2] == 'G' || Scen.ScenarioName[2] == 'U') || Scen.ScenarioName[2] == 'A'
//				|| (Scen.ScenarioName[2] == 'M' && Scen.Scenario >= 25))
//		       	    RequiredCD = 2;
//			 else if(Scen.ScenarioName[2] == 'U')
//			    RequiredCD = 1;
//			 else if(Scen.ScenarioName[2] == 'G')
//			    RequiredCD = 0;
//			}
//
//#ifdef FIXIT_FORCE_CD
// Forces the CD to be inserted according to the scenario being loaded.
//Hide_Mouse();
//VisiblePage.Clear();
//Show_Mouse();
//GamePalette.Set();
//if (!Force_CD_Available(RequiredCD)) {
//       	Prog_End();
//       	exit(EXIT_FAILURE);
//}
//#endif



//   	}
	Theme.Stop();

	if (briefing) {
		Hide_Mouse();
		VisiblePage.Clear();
		Show_Mouse();
		Play_Movie(Scen.IntroMovie);
		Play_Movie(Scen.BriefMovie);
	}

	/*
	** If there's no briefing movie, restate the mission at the beginning.
	*/
#if 1 // 12/04/2019 - LLL
	if (Session.Type == GAME_NORMAL && Scen.BriefMovie == VQ_NONE) {
		Display_Briefing_Text_GlyphX();
	}
#else
	char buffer[25];
	if (Scen.BriefMovie != VQ_NONE) {
		sprintf(buffer, "%s.VQA", VQName[Scen.BriefMovie]);
	}
	if (Session.Type == GAME_NORMAL && Scen.BriefMovie == VQ_NONE || !CCFileClass(buffer).Is_Available()) {
		/*
		** Make sure the mouse is visible before showing the restatement.
		*/
		while(Get_Mouse_State()) {
			Show_Mouse();
		}
		Restate_Mission(Scen.ScenarioName, TXT_OK, TXT_NONE);
	}
#endif

	if (briefing) {
		Hide_Mouse();
		VisiblePage.Clear();
		Show_Mouse();
		Play_Movie(Scen.ActionMovie, Scen.TransitTheme);
	}

	if (Scen.TransitTheme == THEME_NONE) {
		Theme.Queue_Song(THEME_FIRST);
	}

	/*
	** Set the options values, since the palette has been initialized by Read_Scenario
	*/
	Options.Set();

	return(true);
}


/***********************************************************************************************
 * Set_Scenario_Difficulty -- Sets the difficulty of the scenario.                             *
 *                                                                                             *
 *    Updates the player's difficulty in single-player mode.                                   *
 *                                                                                             *
 * INPUT:   difficulty     -- Scenario difficulty                                              *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   Only works in single-player.                                                    *
 *             Must call Start_Scenario first to initialize the player.                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/2019 SKY : Created.                                                                 *
 *=============================================================================================*/
void Set_Scenario_Difficulty(int difficulty)
{
	if (Session.Type == GAME_NORMAL) {
		switch (difficulty) {
			case 0:
				PlayerPtr->Assign_Handicap(DIFF_EASY);
				break;
			case 1:
				PlayerPtr->Assign_Handicap(DIFF_NORMAL);
				break;
			case 2:
				PlayerPtr->Assign_Handicap(DIFF_HARD);
				break;
			default:
				break;
		}
	}
}


/***********************************************************************************************
 * Read_Scenario -- Reads a scenario from disk.                                                *
 *                                                                                             *
 *    This will read a scenario from disk. Use this to begin a scenario.                       *
 *    It doesn't perform any rendering, it merely sets up the system                           *
 *    with the proper data. Setting of the right game state will start                         *
 *    the scenario running.                                                                    *
 *                                                                                             *
 * INPUT:   root     -- Scenario root filename                                                 *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   You must clear out the system variables before calling                          *
 *             this function. Use the Clear_Scenario() function.                               *
 *               It is assumed that Scenario is set to the current scenario number.            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/22/1991     : Created.                                                                 *
 *   02/03/1992 JLB : Uses house identification.                                               *
 *=============================================================================================*/
bool Read_Scenario(char * name)
{
	BStart(BENCH_SCENARIO);
	Clear_Scenario();
	ScenarioInit++;
	if (Read_Scenario_INI(name)) {
#ifdef FIXIT_CSII	//	ajw - Added runtime check for Aftermath to skirmish mode case.
		bool readini = false;
		switch(Session.Type) {
			case GAME_NORMAL:
				readini = false;
				break;
			case GAME_SKIRMISH:
#ifdef FIXIT_VERSION_3
				readini = bAftermathMultiplayer;
#endif
				break;
			case GAME_INTERNET:
#ifndef FIXIT_VERSION_3			//	Loading of Aftermath rules depends on bAftermathMultiplayer now.
				if (Is_Mission_Counterstrike(name)) {
					readini = false;	// Don't allow AM units on a CS map in WChat
					break;
				}
#endif		//	( Note lack of break; )
			default:
#ifdef FIXIT_VERSION_3
				readini = bAftermathMultiplayer;
#else
				if (PlayingAgainstVersion >= VERSION_AFTERMATH_CS) {
					readini = true;
				}
#endif
				break;
		}
		if(readini) {
			/*
			** Find out if the CD in the current drive is the Aftermath disc.
		  	*/
#ifdef FIXIT_VERSION_3
			int cd_index = Get_CD_Index(CCFileClass::Get_CD_Drive(), 1*60);
			if( !( Using_DVD() && cd_index == 5 ) && cd_index != 3 ) {
#else
			if(Get_CD_Index(CCFileClass::Get_CD_Drive(), 1*60) != 3) {
#endif
				GamePalette.Set(FADE_PALETTE_FAST, Call_Back);
				RequiredCD = 3;
				if (!Force_CD_Available(RequiredCD)) {	// force Aftermath CD in drive.
#ifndef WOLAPI_INTEGRATION
					#ifdef WIN32
					if(Special.IsFromWChat || SpawnedFromWChat) {
						char packet[10] = {"Hello"};
					Send_Data_To_DDE_Server (packet, strlen(packet), DDEServerClass::DDE_CONNECTION_FAILED);
						}
					#endif
#endif
					if (!RunningAsDLL) {	//PG
						Emergency_Exit(EXIT_FAILURE);
					}
		     	}
			}
			CCINIClass ini;
			if (ini.Load(CCFileClass("MPLAYER.INI"), false)) {
				Rule.General(ini);
				Rule.Recharge(ini);
				Rule.AI(ini);
				Rule.Powerups(ini);
				Rule.Land_Types(ini);
				Rule.Themes(ini);
				Rule.IQ(ini);
				Rule.Objects(ini);
				Rule.Difficulty(ini);
			}
		}
#endif
		Fill_In_Data();
		Map.Set_View_Dimensions(0, 0, Map.MapCellWidth, Map.MapCellHeight);
	} else {
		
#if (1)
		char message[200];
		if (name) {
			sprintf(message, "Failed to load scenario %s", name);
			GlyphX_Debug_Print(message);
		} else {
			GlyphX_Debug_Print("Failed to load scenario");
		}
#else		
		GamePalette.Set(FADE_PALETTE_FAST, Call_Back);
//		Fade_Palette_To(GamePalette, FADE_PALETTE_FAST, Call_Back);
		Show_Mouse();
		WWMessageBox().Process(TXT_UNABLE_READ_SCENARIO);
		Hide_Mouse();
#endif
		
		BEnd(BENCH_SCENARIO);
		return(false);
	}
	ScenarioInit--;
	BEnd(BENCH_SCENARIO);
	return(true);
}


/***********************************************************************************************
 * Fill_In_Data -- Recreate all data that is not loaded with scenario.                         *
 *                                                                                             *
 *    This routine is called after the INI file for the scenario has been processed. It will   *
 *    infer the game state from the scenario INI data.                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
void Fill_In_Data(void)
{
	/*
	**	The basic scenario data load does not contain the full set of
	**	game data. We now must fill in the missing pieces.
	*/
	ScenarioInit++;
	int index;
	for (index = 0; index < Buildings.Count(); index++) {
		Buildings.Ptr(index)->Update_Buildables();
	}

	Map.Flag_To_Redraw(true);

	/*
	**	Reset the movement zones according to the terrain passability.
	*/
	Map.Zone_Reset(MZONEF_ALL);


#ifdef WIN32
	/*
	**	Since the sidebar starts up activated, adjust the home start position so that
	**	the right edge of the map will still be visible.
	*/
	if (!Debug_Map) {
		Map.SidebarClass::Activate(1);
//		if (Session.Type == GAME_NORMAL) {
			Scen.Views[0] = Scen.Views[1] = Scen.Views[2] = Scen.Views[3] = Scen.Waypoint[WAYPT_HOME];
			Map.Set_Tactical_Position(Cell_Coord((Scen.Waypoint[WAYPT_HOME] - (MAP_CELL_W * 4 * RESFACTOR)) - (5*RESFACTOR)));
//		}
	}
#endif

	/*
	**	Handle any data resetting that can be safely inferred from the actual
	**	data that has been loaded.
	*/
	/*
	**	Distribute the trigger pointers to the appropriate working lists.
	*/
	for (index = 0; index < TriggerTypes.Count(); index++) {
		TriggerTypeClass * tp = TriggerTypes.Ptr(index);

		assert(tp != NULL);

		if (tp->Attaches_To() & ATTACH_MAP) {
			MapTriggers.Add(Find_Or_Make(tp));
		}
		if (tp->Attaches_To() & ATTACH_GENERAL) {
			LogicTriggers.Add(Find_Or_Make(tp));
		}
		if (tp->Attaches_To() & ATTACH_HOUSE) {
			HouseTriggers[tp->House].Add(Find_Or_Make(tp));
		}
	}

	ScenarioInit--;

	/*
	** Now go through and set all the cells ringing the map to be visible, so
	** we won't get the wall of shadow at the edge of the map.
	*/
	int x,y;
	for (x = Map.MapCellX-1; x < ((Map.MapCellX + Map.MapCellWidth + 1)); x++) {
		Map[XY_Cell(x, Map.MapCellY-1)].IsVisible =
			Map[XY_Cell(x, Map.MapCellY-1)].IsMapped = true;

		Map[XY_Cell(x, Map.MapCellY+Map.MapCellHeight)].IsVisible =
			Map[XY_Cell(x, Map.MapCellY+Map.MapCellHeight)].IsMapped = true;
	}
	for (y = Map.MapCellY; y < (Map.MapCellY + Map.MapCellHeight); y++) {
		Map[XY_Cell(Map.MapCellX-1, y)].IsVisible =
			Map[XY_Cell(Map.MapCellX-1, y)].IsMapped = true;
		Map[XY_Cell(Map.MapCellX+Map.MapCellWidth, y)].IsVisible =
			Map[XY_Cell(Map.MapCellX+Map.MapCellWidth, y)].IsMapped = true;
	}

	/*
	**	If inheriting from a previous scenario was indicated, then create the carry over
	**	objects at this time.
	*/
	if (Scen.IsToInherit) {
		CarryoverClass * cptr = Carryover;
		while (cptr != NULL) {
			cptr->Create();
			cptr = (CarryoverClass *)cptr->Get_Next();
		}
	}

	/*
	**	The "allow win" action is a special case that is handled here. The total number
	**	of triggers that have this action must be recorded.
	*/
	for (index = 0; index < TriggerTypes.Count(); index++) {
		TriggerTypeClass * tp = TriggerTypes.Ptr(index);
		if (tp->Action1.Action == TACTION_ALLOWWIN ||
			(tp->ActionControl != MULTI_ONLY && tp->Action2.Action == TACTION_ALLOWWIN)) {

			HouseClass::As_Pointer(tp->House)->Blockage++;
		}
	}

	/*
	**	Move available money to silos, if the scenario flag so indicates.
	*/
	if (Scen.IsMoneyTiberium) {
		for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
			HouseClass * hptr = HouseClass::As_Pointer(house);
			if (hptr != NULL) {
				int tomove = hptr->Capacity - hptr->Tiberium;
				hptr->Credits -= tomove;
				hptr->Tiberium += tomove;
			}
		}
	}

	/*
	**	Count all non-destroyed bridges on the map.
	*/
	Scen.BridgeCount = Map.Intact_Bridge_Count();

	Map.All_To_Look(PlayerPtr, true);
}


/***********************************************************************************************
 * Post_Load_Game -- Fill in an inferred data from the game state.                             *
 *                                                                                             *
 *    This routine is typically called after a game has been loaded. Some working data lists   *
 *    can be rebuild from the game state. This working data is rebuilt rather than being       *
 *    stored with the game data file.                                                          *
 *                                                                                             *
 * INPUT:   load_multi -- true if we're loading a multiplayer game                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Although it is safe to call this routine whenever, it is only needed after a    *
 *             game load.                                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/30/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Post_Load_Game(int load_multi)
{
	Map.Set_View_Dimensions(0, 0, Map.MapCellWidth, Map.MapCellHeight);
	//
	// Do NOT call Overpass if we're loading a multiplayer game; it calls the
	// random # generator, which throws the games out of sync if they were
	// saved on different frame #'s.
	//
	if (!load_multi) {
		Map.Overpass();
	}
	Scen.BridgeCount = Map.Intact_Bridge_Count();
	Map.Zone_Reset(MZONEF_ALL);
}


/***********************************************************************************************
 * Clear_Scenario -- Clears all data in preparation for scenario load.                         *
 *                                                                                             *
 *    This routine will clear out all data specific to a scenario in                           *
 *    preparation for a subsequent scenario data load. This will free                          *
 *    all units, animations, and icon maps.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/22/1991     : Created.                                                                 *
 *   03/21/1992 JLB : Changed buffer allocations, so changes memset code.                      *
 *   07/13/1995 JLB : End count down moved here.                                               *
 *=============================================================================================*/
void Clear_Scenario(void)
{
// TCTCTC -- possibly just use in-place new of scenario object?

	Scen.MissionTimer = 0;
	Scen.MissionTimer.Stop();
	Scen.Timer = 0;
	Scen.ShroudTimer = 0;
	Scen.IntroMovie = VQ_NONE;
	Scen.BriefMovie = VQ_NONE;
	Scen.WinMovie = VQ_NONE;
	Scen.WinMovie2 = VQ_NONE;
	Scen.WinMovie3 = VQ_NONE;
	Scen.WinMovie4 = VQ_NONE;
	Scen.LoseMovie = VQ_NONE;
	Scen.ActionMovie = VQ_NONE;
	Scen.IsNoSpyPlane = false;
	Scen.IsTanyaEvac = false;
	Scen.IsEndOfGame = false;
	Scen.IsInheritTimer = false;
	Scen.IsToCarryOver = false;
	Scen.IsSkipScore = false;
	Scen.IsOneTimeOnly = false;
	Scen.IsTruckCrate = false;
	Scen.IsMoneyTiberium = false;
	Scen.IsNoMapSel = false;
	Scen.CarryOverCap = 0;
	Scen.CarryOverPercent = 0;
	Scen.TransitTheme = THEME_NONE;
	Scen.Percent = 0;

	memset(Scen.GlobalFlags, 0, sizeof(Scen.GlobalFlags));

	MapTriggers.Clear();
	LogicTriggers.Clear();

	for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
		HouseTriggers[house].Clear();
	}

	/*
	** Call everyone's Init routine, except the Map's; for the Map, only call
	** MapClass::Init, which clears the Cell array.  The Display::Init requires
	** a Theater argument, and the theater is not known at this point; also, it
	** would reload MixFiles, which isn't desired.  Display::Read_INI calls its
	** own Init, which will Init the entire Map hierarchy.
	*/
	Map.Init_Clear();
	Score.Init();
	Logic.Init();

	HouseClass::Init();
	ObjectClass::Init();
	TeamTypeClass::Init();
	TeamClass::Init();
	TriggerClass::Init();
	TriggerTypeClass::Init();
	AircraftClass::Init();
	AnimClass::Init();
	BuildingClass::Init();
	BulletClass::Init();
	InfantryClass::Init();
	OverlayClass::Init();
	SmudgeClass::Init();
	TemplateClass::Init();
	TerrainClass::Init();
	UnitClass::Init();
	VesselClass::Init();

	FactoryClass::Init();

	Base.Init();

	CurrentObject.Clear_All();

	for (int index = 0; index < WAYPT_COUNT; index++) {
		Scen.Waypoint[index] = -1;
	}

#ifdef FIXIT_VERSION_3			//	For endgame auto-sonar pulse.
	bAutoSonarPulse = false;
#endif

#ifdef FIXIT_VERSION_3		//	Stalemate games.
	Scen.bLocalProposesDraw = false;
	Scen.bOtherProposesDraw = false;
#endif

}


/***********************************************************************************************
 * Do_Win -- Display winning congratulations.                                                  *
 *                                                                                             *
 *    Perform the win the mission process. This will display any winning movies and the score  *
 *    screen. Followed by the map selection screen and then the load of the new scenario.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1992 JLB : Created.                                                                 *
 *   01/01/1995 JLB : Carries money forward into next scenario.                                *
 *=============================================================================================*/
void Do_Win(void)
{
	Map.Set_Default_Mouse(MOUSE_NORMAL);
	Hide_Mouse();
	Theme.Queue_Song(THEME_QUIET);

	/*
	** If this is a multiplayer game, clear the game's name so we won't respond
	** to game queries any more (in Call_Back)
	*/
	if (Session.Type != GAME_NORMAL) {
		Session.GameName[0] = 0;
	}

	/*
	**	Determine a cosmetic center point for the text.
	*/
	int x = Map.TacPixelX + (Lepton_To_Pixel(Map.TacLeptonWidth)/2);

	/*
	** Hack section.  If it's allied scenario 10, variation A, then skip the
	** score and map selection, don't increment scenario, and set it to
	** variation B.
	*/
#ifdef FIXIT_ANTS
	if (Session.Type != GAME_NORMAL || !Scen.IsSkipScore || AntsEnabled) {
#else
	if (Session.Type != GAME_NORMAL || !Scen.IsSkipScore ) {
#endif	//FIXIT_ANTS

		/*
		**	Announce win to player.
		*/
		Set_Logic_Page(SeenBuff);
		Map.Flag_To_Redraw (true);
		Map.Render();
#ifdef WIN32
		Fancy_Text_Print(TXT_SCENARIO_WON, x, 90*RESFACTOR, &ColorRemaps[PCOLOR_RED], TBLACK, TPF_CENTER|TPF_VCR|TPF_USE_GRAD_PAL|TPF_DROPSHADOW);
#else
		Fancy_Text_Print(TXT_MISSION, x, 90*RESFACTOR, &ColorRemaps[PCOLOR_RED], TBLACK, TPF_CENTER|TPF_VCR|TPF_USE_GRAD_PAL|TPF_DROPSHADOW);
		Fancy_Text_Print(TXT_HACKHACK, x, 110*RESFACTOR, &ColorRemaps[PCOLOR_RED], TBLACK, TPF_CENTER|TPF_VCR|TPF_USE_GRAD_PAL|TPF_DROPSHADOW);
#endif
		CountDownTimer = TIMER_SECOND * 3;
		while (Is_Speaking()) {};
		Speak(VOX_ACCOMPLISHED);
		while (CountDownTimer || Is_Speaking()) {
			Call_Back();
		}
	}

	/*
	** Stop here if this is a multiplayer game.
	*/
	if (Session.Type != GAME_NORMAL) {
		if (!Session.Play) {
			Session.GamesPlayed++;
			Multi_Score_Presentation();
			Session.CurGame++;
			if (Session.CurGame >= MAX_MULTI_GAMES) {
				Session.CurGame = MAX_MULTI_GAMES - 1;
			}
		}
		GameActive = 0;
		Show_Mouse();
		return;
	}

	Hide_Mouse();
	VisiblePage.Clear();
	Show_Mouse();
	Play_Movie(Scen.WinMovie);
	Play_Movie(Scen.WinMovie2);
	Play_Movie(Scen.WinMovie3);
	Play_Movie(Scen.WinMovie4);

	Keyboard->Clear();

	SaveTanya = IsTanyaDead;
	Scen.CarryOverTimer = Scen.MissionTimer;
//	int timer = Scen.MissionTimer;

	/*
	**	Do the ending screens only if not playing back a recorded game.
	*/
	if (!Session.Play) {
		/*
		**	If the score presentation should be performed, then do
		**	so now.
		*/
		Keyboard->Clear();
		if (!Scen.IsSkipScore) {
			Score.Presentation();
		}


		if (Scen.IsOneTimeOnly) {
			GameActive = false;
			Show_Mouse();
#ifdef FIXIT_ANTS
			AntsEnabled = false;
//			Mono_Printf("Scenario.cpp one time only antsenabled is false\n");
#endif
			return;
		}

		/*
		** If this scenario is flagged as ending the game then print the credits and exit.
		*/
#if (0)//PG
		if (Scen.IsEndOfGame) {
			if (PlayerPtr->ActLike == HOUSE_USSR) {
				Play_Movie(VQ_SOVFINAL);
			} else {
				Play_Movie(VQ_ALLYEND);
			}
			Show_Who_Was_Responsible();
			GameActive = false;
			Show_Mouse();
#ifdef FIXIT_ANTS
			AntsEnabled = false;
#endif
			return;
		}
#endif
		/*
		** Hack section.  If it's allied scenario 10, variation A, then skip the
		** score and map selection, don't increment scenario, and set it to
		** variation B.
		*/
		if (Scen.IsNoMapSel) {
			// force it to play the second half of scenario 10
#ifdef FIXIT_ANTS
			if (AntsEnabled) {
				char scenarioname[24];
				strcpy(scenarioname, Scen.ScenarioName);
				char buf[10];
				Scen.Scenario++;
				sprintf(buf, "%02d", Scen.Scenario);
				memcpy(&scenarioname[3], buf, 2);
				Scen.Set_Scenario_Name(scenarioname);
			} else {
				Scen.ScenarioName[6] = 'B';
			}

#else
			Scen.ScenarioName[6] = 'B';
#endif

		} else {
			Scen.Set_Scenario_Name(Map_Selection());
		}

		Keyboard->Clear();
	}

	Scen.CarryOverMoney = PlayerPtr->Credits;

	/*
	**	If requested, record the scenario's objects in the carry over list
	**	for possible use in a future scenario.
	*/
	if (Scen.IsToCarryOver) {

		/*
		**	First delete any existing carry over list. Any old list will be
		**	blasted over by the new list -- there is only one logic carryover
		**	list to be maintained.
		*/
		while (Carryover) {
			CarryoverClass * cptr = (CarryoverClass *)Carryover->Get_Next();
			Carryover->Remove();
			delete Carryover;
			Carryover = cptr;
		}

		/*
		**	Record all objects, that are to be part of the carry over set, into
		**	the carry over list.
		*/
		for (int building_index = 0; building_index < Buildings.Count(); building_index++) {
			BuildingClass * building = Buildings.Ptr(building_index);

			if (building && !building->IsInLimbo && building->Strength > 0) {
				CarryoverClass * cptr = new CarryoverClass(building);

				if (cptr) {
					if (Carryover) {
						cptr->Add_Tail(*Carryover);
					} else {
						Carryover = cptr;
					}
				}
			}
		}
		for (int unit_index = 0; unit_index < Units.Count(); unit_index++) {
			UnitClass * unit = Units.Ptr(unit_index);

			if (unit && !unit->IsInLimbo && unit->Strength > 0) {
				CarryoverClass * cptr = new CarryoverClass(unit);

				if (cptr) {
					if (Carryover) {
						cptr->Add_Tail(*Carryover);
					} else {
						Carryover = cptr;
					}
				}
			}
		}
		for (int infantry_index = 0; infantry_index < Infantry.Count(); infantry_index++) {
			InfantryClass * infantry = Infantry.Ptr(infantry_index);

			if (infantry && !infantry->IsInLimbo && infantry->Strength > 0) {
				CarryoverClass * cptr = new CarryoverClass(infantry);

				if (cptr) {
					if (Carryover) {
						cptr->Add_Tail(*Carryover);
					} else {
						Carryover = cptr;
					}
				}
			}
		}
		for (int vessel_index = 0; vessel_index < Vessels.Count(); vessel_index++) {
			VesselClass * vessel = Vessels.Ptr(vessel_index);

			if (vessel && !vessel->IsInLimbo && vessel->Strength > 0) {
				CarryoverClass * cptr = new CarryoverClass(vessel);

				if (cptr) {
					if (Carryover) {
						cptr->Add_Tail(*Carryover);
					} else {
						Carryover = cptr;
					}
				}
			}
		}
	}

	/*
	** Generate a new scenario filename
	*/
//	Scen.Set_Scenario_Name(Scen.Scenario, Scen.ScenPlayer, Scen.ScenDir, Scen.ScenVar);
	Start_Scenario(Scen.ScenarioName);

	/*
	**	If the mission timer is to be inheriteded from the previous scenario then do it now.
	*/
	if (Scen.IsInheritTimer) {
		Scen.MissionTimer = Scen.CarryOverTimer;
		Scen.MissionTimer.Start();
	}

//	PlayerPtr->NukePieces = nukes;

	Map.Render();
	GamePalette.Set(FADE_PALETTE_FAST, Call_Back);
//	Fade_Palette_To(GamePalette, FADE_PALETTE_FAST, Call_Back);
	Show_Mouse();
}


/***********************************************************************************************
 * Do_Lose -- Display losing comments.                                                         *
 *                                                                                             *
 *    Performs the lose mission processing. This will generally display a "would you like      *
 *    to replay" dialog and then either reload the scenario or set flags such that the main    *
 *    menu will appear.                                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
void Do_Lose(void)
{
	Map.Set_Default_Mouse(MOUSE_NORMAL);
	Hide_Mouse();

	Theme.Queue_Song(THEME_QUIET);

	/*
	** If this is a multiplayer game, clear the game's name so we won't respond
	** to game queries any more (in Call_Back)
	*/
	if (Session.Type != GAME_NORMAL) {
		Session.GameName[0] = 0;
	}

	/*
	**	Determine a cosmetic center point for the text.
	*/
	int x = Map.TacPixelX + (Lepton_To_Pixel(Map.TacLeptonWidth)/2);

	/*
	**	Announce win to player.
	*/
	Set_Logic_Page(SeenBuff);
	Fancy_Text_Print(TXT_SCENARIO_LOST, x, 90*RESFACTOR, &ColorRemaps[PCOLOR_RED], TBLACK, TPF_CENTER|TPF_VCR|TPF_USE_GRAD_PAL|TPF_DROPSHADOW);
	CountDownTimer = TIMER_SECOND * 3;
	while (Is_Speaking()) {};
	Speak(VOX_FAIL);
	while (CountDownTimer || Is_Speaking()) {
		Call_Back();
	}

	/*
	** Stop here if this is a multiplayer game.
	*/
	if (Session.Type != GAME_NORMAL) {
		if (!Session.Play) {
			Session.GamesPlayed++;
			Multi_Score_Presentation();
			Session.CurGame++;
			if (Session.CurGame >= MAX_MULTI_GAMES) {
				Session.CurGame = MAX_MULTI_GAMES - 1;
			}
		}
		GameActive = 0;
		Show_Mouse();
		return;
	}

	Hide_Mouse();
	VisiblePage.Clear();
	Show_Mouse();
#ifdef CHEAT_KEYS
//	Mono_Printf("Trying to play lose movie\n");
#endif //CHEAT_KEYS
	Play_Movie(Scen.LoseMovie);

	/*
	** Start same scenario again
	*/
	GamePalette.Set();
	Show_Mouse();
	if (!Session.Play && !WWMessageBox().Process(TXT_TO_REPLAY, TXT_YES, TXT_NO)) {
		Hide_Mouse();
		Keyboard->Clear();
		Start_Scenario(Scen.ScenarioName, false);

		/*
		**	Start the scenario timer with the carried over value if necessary.
		*/
		if (Scen.IsInheritTimer) {
			Scen.MissionTimer = Scen.CarryOverTimer;
			Scen.MissionTimer.Start();
		}

		Map.Render();
	} else {
		Hide_Mouse();
		GameActive = 0;
	}

	GamePalette.Set(FADE_PALETTE_FAST, Call_Back);
	Show_Mouse();
}

#ifdef FIXIT_VERSION_3		//	Stalemate games.
/***********************************************************************************************
 * Do_Draw -- Parallels Do_Win and Do_Lose, for multiplayer games that end in a draw.
 *=============================================================================================*/
void Do_Draw(void)
{
	Map.Set_Default_Mouse(MOUSE_NORMAL);
	Hide_Mouse();

	Theme.Queue_Song(THEME_QUIET);

	/*
	** If this is a multiplayer game, clear the game's name so we won't respond
	** to game queries any more (in Call_Back)
	*/
	if (Session.Type != GAME_NORMAL) {
		Session.GameName[0] = 0;
	}

	/*
	**	Determine a cosmetic center point for the text.
	*/
	int x = Map.TacPixelX + (Lepton_To_Pixel(Map.TacLeptonWidth)/2);

	/*
	**	Announce win to player.
	*/
	Set_Logic_Page(SeenBuff);
	Fancy_Text_Print(TXT_WOL_DRAW, x, 90*RESFACTOR, &ColorRemaps[PCOLOR_RED], TBLACK, TPF_CENTER|TPF_VCR|TPF_USE_GRAD_PAL|TPF_DROPSHADOW);
	CountDownTimer = TIMER_SECOND * 3;
	while (Is_Speaking()) {};
	Speak(VOX_CONTROL_EXIT);
	while (CountDownTimer || Is_Speaking()) {
		Call_Back();
	}

	/*
	** Stop here if this is a multiplayer game.
	*/
	if (!Session.Play) {
		Session.GamesPlayed++;
		Multi_Score_Presentation();
		Session.CurGame++;
		if (Session.CurGame >= MAX_MULTI_GAMES) {
			Session.CurGame = MAX_MULTI_GAMES - 1;
		}
	}
	GameActive = 0;
	Show_Mouse();
}
#endif

/***********************************************************************************************
 * Do_Restart -- Handle the restart mission process.                                           *
 *                                                                                             *
 *    This routine is called in the main game loop when the mission must be restarted. This    *
 *    routine will throw away the current game and reload the appropriate mission. The         *
 *    game will "resume" at the start of the mission.                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Do_Restart(void)
{
	/*
	** Start a timer going, before we restart the scenario
	*/
	CDTimerClass<SystemTimerClass> timer;
	timer = TICKS_PER_SECOND * 4;
	Theme.Queue_Song(THEME_QUIET);

	WWMessageBox().Process(TXT_RESTARTING, TXT_NONE);

	Map.Set_Default_Mouse(MOUSE_NORMAL);
	Keyboard->Clear();
	Start_Scenario(Scen.ScenarioName, false);

	/*
	**	Start the scenario timer with the carried over value if necessary.
	*/
	if (Scen.IsInheritTimer) {
		Scen.MissionTimer = Scen.CarryOverTimer;
		Scen.MissionTimer.Start();
	}

	/*
	** Make sure the message stays displayed for at least 1 second
	*/
	while (timer > 0) {
		Call_Back();
	}
	Keyboard->Clear();

	Map.Render();
}


/***********************************************************************************************
 * Restate_Mission -- Handles restating the mission objective.                                 *
 *                                                                                             *
 *    This routine will display the mission objective (as text). It will also give the         *
 *    option to redisplay the mission briefing video.                                          *
 *                                                                                             *
 * INPUT:   name  -- The scenario name. This is the unique identifier for the scenario         *
 *                   briefing text as it appears in the "MISSION.INI" file.                    *
 *                                                                                             *
 * OUTPUT:  Returns the response from the dialog. This will either be 1 if the video was       *
 *          requested, or 0 if the return to game options button was selected.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/23/1995 JLB : Created.                                                                 *
 *   08/06/1995 JLB : Uses preloaded briefing text.                                            *
 *=============================================================================================*/
bool Restate_Mission(char const * name, int button1, int button2)
{
	if (name) {

		bool brief = true;
		char buffer[25];
		if (Scen.BriefMovie != VQ_NONE) {
			sprintf(buffer, "%s.VQA", VQName[Scen.BriefMovie]);
		}

		if (Scen.BriefMovie == VQ_NONE || !CCFileClass(buffer).Is_Available()) {
			button2 = TXT_OK;
			button1 = TXT_NONE;
			brief = false;
		}

		/*
		**	If mission object text was found, then display it.
		*/
		if (strlen(Scen.BriefingText)) {
			strcpy(_ShapeBuffer, Scen.BriefingText);
			BlackPalette.Set(FADE_PALETTE_MEDIUM, Call_Back);
			if (BGMessageBox(_ShapeBuffer, button2, button1)) {
				return(true);
			}
			if (!brief) return(true);
			return(false);
		}
	}
	return(false);
}


#define	BUTTON_1		1
#define	BUTTON_2		2
#define	BUTTON_3		3
#define	BUTTON_FLAG	0x8000
int BGMessageBox(char const * msg, int btn1, int btn2)
{
#define BUFFSIZE 511
	char buffer[BUFFSIZE];
	int retval;
	bool process;								// loop while true
	KeyNumType input;							// user input
	int selection;
	bool pressed;
	int curbutton;
	TextButtonClass * buttons[3];
	BOOL display;									// display level
	int  realval[5];
	int  morebutton = 3;						// which button says "more": 2 or 3?

	const char * b1txt = Text_String(btn1);
	const char * b2txt = Text_String(btn2);
#ifdef FRENCH
	const char * b3txt = "SUITE";
#else
#ifdef GERMAN
	const char * b3txt = "MEHR";
#else
	const char * b3txt = "MORE";
#endif
#endif

	const void *briefsnd = MFCD::Retrieve("BRIEFING.AUD");

	GadgetClass::Set_Color_Scheme(&ColorRemaps[PCOLOR_TYPE]);

	/*
	** If the message won't be needing the 'more' button, get rid of it.
	*/
	if (strlen(msg) <= BUFFSIZE-1) {
		b3txt = "";
	}

#ifdef WIN32
	GraphicBufferClass seen_buff_save(VisiblePage.Get_Width(), VisiblePage.Get_Height(), (void*)NULL);
#endif

	/*
	** If there's no text for button one, zero it out.
	*/
	if (*b1txt == '\0') {
		b1txt = b2txt;
		b2txt = "";
		if(*b1txt == '\0') {
			b1txt=0;
		}
	}

	/*
	** If there's no text for button two, zero it out.  However, if there
	** is text for button three, move its text (always "MORE") to button two,
	** and set the morebutton flag to point to button two.  Then, clear out
	** button 3.
	*/
	if (*b2txt == '\0') {
		b2txt = 0;
		if (*b3txt != '\0') {
			b2txt = b3txt;
			b3txt = "";
			morebutton = 1;
		}
	}

	/*
	** If there's no text for button three, zero it out.
	*/
	if (*b3txt == '\0') b3txt = 0;

	Fancy_Text_Print(TXT_NONE, 0, 0, &ColorRemaps[PCOLOR_TYPE], TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL);
	/*
	**	Examine the optional button parameters. Fetch the width and starting
	**	characters for each.
	*/
	char b1char, b2char, b3char;	// 1st char of each string
	int bwidth, bheight;				// button width and height
	int numbuttons = 0;
	if (b1txt) {
		b1char = toupper(b1txt[0]);

		/*
		**	Build the button list.
		*/
		bheight = FontHeight + FontYSpacing + 2;
		bwidth = max((String_Pixel_Width(b1txt) + 8), 80);
		if (b2txt) {
			numbuttons = 2;
			b2char = toupper(b2txt[0]);
			bwidth = max(((int)String_Pixel_Width( b2txt ) + 8), bwidth);
//			b1x = x + 10;								// left side

			if (b3txt) {
				numbuttons = 3;
				b3char = toupper(b3txt[0]);
				bwidth = max(((int)String_Pixel_Width( b3txt ) + 8), bwidth);
			}

		} else {
			numbuttons = 1;
//			b1x = x + ((width - bwidth) >> 1);		// centered
		}
	}

	/*
	**	Determine the dimensions of the text to be used for the dialog box.
	**	These dimensions will control how the dialog box looks.
	*/
	buffer[BUFFSIZE-1] = 0;
	int buffend = BUFFSIZE-1;
	strncpy(buffer, msg, BUFFSIZE-1);
	/*
	** Scan through the string to see if it got clipped, and if so, we'll
	** trim it back to the last space so it'll clip on a word.
	*/
	if (strlen(buffer) != strlen(msg)) {
		while (buffer[buffend] != ' ') buffend--;
		buffer[buffend]=0;
	}
	Fancy_Text_Print(TXT_NONE, 0, 0, &ColorRemaps[PCOLOR_TYPE], TBLACK, TPF_6PT_GRAD|TPF_USE_GRAD_PAL);
	int width;
	int height;
	Format_Window_String(buffer, 300, width, height);
	height += (numbuttons == 0) ? 30 : 60;

	int x = (SeenBuff.Get_Width() - width) / 2;
	int y = (SeenBuff.Get_Height() - height) / 2;

	/*
	**	Other inits.
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Initialize the button structures. All are initialized, even though one (or none) may
	**	actually be added to the button list.
	*/
	TextButtonClass button1(BUTTON_1, b1txt, TPF_BUTTON,
		x + ((numbuttons == 1) ? ((width - bwidth) >> 1) : 10), y + height - (bheight + 5), bwidth);

	TextButtonClass button2(BUTTON_2, b2txt, TPF_BUTTON,
		x + width - (bwidth + 10), y + height - (bheight + 5), bwidth);

	TextButtonClass button3(BUTTON_3, b3txt, TPF_BUTTON,
		0, y + height - (bheight + 5));
	button3.X = x + ((width - button3.Width) >> 1);

	TextButtonClass * buttonlist = 0;
	curbutton = 0;

	/*
	**	Add and initialize the buttons to the button list.
	*/
	if (numbuttons) {
		buttonlist = &button1;
		buttons[0] = &button1;
		realval[0] = BUTTON_1;
		if (numbuttons > 2) {
			button3.Add(*buttonlist);
			buttons[1] = &button3;
			realval[1] = BUTTON_3;
			button2.Add(*buttonlist);
			buttons[2] = &button2;
			realval[2] = BUTTON_2;
			buttons[curbutton]->Turn_On();
		} else if (numbuttons == 2) {
			button2.Add(*buttonlist);
			buttons[1] = &button2;
			realval[1] = BUTTON_2;
			buttons[curbutton]->Turn_On();
		}
	}

	/*
	**	Draw the dialog.
	*/
	Hide_Mouse();

	PaletteClass temp;
#ifdef WIN32
	char *filename = "SOVPAPER.PCX";
	if (PlayerPtr->Class->House != HOUSE_USSR && PlayerPtr->Class->House != HOUSE_UKRAINE) {
		filename = "ALIPAPER.PCX";
	}
	Load_Title_Screen(filename, &HidPage, (unsigned char*)temp.Get_Data());
#else
	char *filename = "SOVPAPER.CPS";
	if (PlayerPtr->Class->House != HOUSE_USSR && PlayerPtr->Class->House != HOUSE_UKRAINE) {
		filename = "ALIPAPER.CPS";
	}
	Load_Uncompress(CCFileClass(filename), HidPage, HidPage, temp);
#endif
	HidPage.Blit(SeenPage);

	#ifdef WIN32
	VisiblePage.Blit(seen_buff_save);
	#endif

	static unsigned char _scorepal[]={0,1,12,13,4,5,6,7,8,9,10,255,252,253,14,248};
	Set_Font_Palette(_scorepal);
	temp.Set(FADE_PALETTE_MEDIUM, Call_Back);

	/*
	**	Main Processing Loop.
	*/

	int bufindex = 0;

	Keyboard->Clear();

	Set_Font_Palette(_scorepal);
	int xprint = x + 20;
	int yprint = y + 25;
	do {
		#ifdef WIN32
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored = false;
			Hide_Mouse();
			seen_buff_save.Blit(VisiblePage);
			display = true;
			Show_Mouse();
		}
		#endif
		char bufprint[2];
		bufprint[1]=0;
		bufprint[0] = buffer[bufindex];
		if (bufprint[0] == '\r' || bufprint[0] == '@') {
			xprint = x + 20;
			yprint += FontHeight + FontYSpacing;

		} else {
			if (bufprint[0] != 20) {
				SeenPage.Print(bufprint, xprint, yprint, TBLACK, TBLACK);
#ifdef WIN32
				seen_buff_save.Print(bufprint, xprint, yprint, TBLACK, TBLACK);
#endif
				xprint += Char_Pixel_Width(bufprint[0]);
			}
		}
		if (bufprint[0] == '\r' || bufprint[0] == '@') {
#ifdef WIN32
			Play_Sample(briefsnd, 255, Options.Normalize_Volume(135));
#else
			Play_Sample(briefsnd, 255, Options.Normalize_Volume(45));
#endif
			CDTimerClass<SystemTimerClass> cd;
			cd = 5;
			do {
				Call_Back();
			} while(!Keyboard->Check() && cd);
		}
	} while (buffer[++bufindex]);

	Show_Mouse();
	Keyboard->Clear();

	if (buttonlist) {
		process = true;
		pressed = false;
		while (process) {
			#ifdef WIN32
			/*
			** If we have just received input focus again after running in the background then
			** we need to redraw.
			*/
			if (AllSurfaces.SurfacesRestored) {
				AllSurfaces.SurfacesRestored = false;
				Hide_Mouse();
				seen_buff_save.Blit(VisiblePage);
				display = true;
				Show_Mouse();
			}
			#endif

			if (display) {
				display = false;

				Hide_Mouse();

				/*
				**	Redraw the buttons.
				*/
				if (buttonlist) {
					buttonlist->Draw_All();
				}
				Show_Mouse();
			}

			/*
			**	Invoke game callback.
			*/
			Call_Back();

			/*
			**	Fetch and process input.
			*/
			input = buttonlist->Input();
			switch (input) {
				case (BUTTON_1|BUTTON_FLAG):
					selection = realval[0];
					pressed = true;
					break;

				case (KN_ESC):
					if (numbuttons > 2) {
						selection = realval[1];
						pressed = true;
					} else {
						selection = realval[2];
						pressed = true;
					}
					break;

				case (BUTTON_2|BUTTON_FLAG):
					selection = BUTTON_2;
					pressed = true;
					break;

				case (BUTTON_3|BUTTON_FLAG):
					selection = realval[1];
					pressed = true;
					break;

				case (KN_LEFT):
					if (numbuttons > 1) {
						buttons[curbutton]->Turn_Off();
						buttons[curbutton]->Flag_To_Redraw();

						curbutton--;
						if (curbutton < 0) {
							curbutton = numbuttons - 1;
						}

						buttons[curbutton]->Turn_On();
						buttons[curbutton]->Flag_To_Redraw();
					}
					break;

				case (KN_RIGHT):
					if (numbuttons > 1) {
						buttons[curbutton]->Turn_Off();
						buttons[curbutton]->Flag_To_Redraw();

						curbutton++;
						if (curbutton > (numbuttons - 1) ) {
							curbutton = 0;
						}

						buttons[curbutton]->Turn_On();
						buttons[curbutton]->Flag_To_Redraw();
					}
					break;

				case (KN_RETURN):
					selection = curbutton + BUTTON_1;
					pressed = true;
					break;

				/*
				**	Check 'input' to see if it's the 1st char of button text
				*/
				default:
					if (b1char == toupper(Keyboard->To_ASCII((KeyNumType)(input & 0xFF)))) {
						selection = BUTTON_1;
						pressed = true;
					} else if (b2txt!=NULL &&
						b2char == toupper(Keyboard->To_ASCII((KeyNumType)(input & 0xFF)))) {
						selection = BUTTON_2;
						pressed = true;
					} else if (b3txt!=NULL &&
						b3char == toupper(Keyboard->To_ASCII((KeyNumType)(input & 0xFF)))) {
						selection = BUTTON_3;
						pressed = true;
					}
					break;
			}

			if (pressed) {
				switch (selection) {
					case (BUTTON_1):
						retval = 1;
						process = false;
						break;

					case (BUTTON_2):
						retval = 0;
						process = false;
						break;

					case BUTTON_3:
						retval = 2;
						process = false;
						break;
				}

				pressed = false;
			}
		}

	} else {

		Keyboard->Clear();
	}

	if (retval == (morebutton-1) && strlen(msg) > BUFFSIZE-1) {
		retval = BGMessageBox(msg + buffend + 1, btn1, btn2);
	}
	/*
	** Restore the screen.
	*/
	Hide_Mouse();
	/*
	** Now set the palette, depending on if we're going to show the video or
	** go back to the main menu.
	*/
	switch (retval) {
		case 0:
//			BlackPalette.Set(FADE_PALETTE_MEDIUM, Call_Back);
//			SeenPage.Clear();
////			CCPalette.Set();
//			break;
		case 1:
			BlackPalette.Set(FADE_PALETTE_MEDIUM, Call_Back);
			SeenPage.Clear();
			break;
		default:
			break;
	}
	Show_Mouse();

	GadgetClass::Set_Color_Scheme(&ColorRemaps[PCOLOR_DIALOG_BLUE]);

	return(retval);
}


/***********************************************************************************************
 * Set_Scenario_Name -- Creates the INI scenario name string.                                  *
 *                                                                                             *
 *    This routine is used by the scenario loading and saving code. It generates the scenario  *
 *    INI root file name for the specified scenario parameters.                                *
 *                                                                                             *
 * INPUT:                                                                                      *
 *         buf         buffer to store filename in; must be long enough for root.ext           *
 *       scenario      scenario number                                                         *
 *       player      player type for this game (GDI, NOD, multi-player, ...)                   *
 *       dir         directional parameter for this game (East/West)                           *
 *       var         variation of this game (Lose, A/B/C/D, etc)                               *
 *                                                                                             *
 * OUTPUT:  none.                                                                              *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *   05/01/1995 BRR : 2-player scenarios use same names as multiplayer                         *
 *=============================================================================================*/
void ScenarioClass::Set_Scenario_Name(int scenario, ScenarioPlayerType player, ScenarioDirType dir, ScenarioVarType var)
{
	Scenario = scenario;
//	ScenPlayer = player;
//	ScenDir = dir;
//	ScenVar = var;

	char c_player;			// character representing player type
	char c_dir;				// character representing direction type
	char c_var;				// character representing variation type
	ScenarioVarType i;
	char fname[_MAX_FNAME+_MAX_EXT];

	/*
	** Set the player-type value.
	*/
	switch (player) {
		case SCEN_PLAYER_SPAIN:
			c_player = HouseTypeClass::As_Reference(HOUSE_SPAIN).Prefix;
			break;

		case SCEN_PLAYER_GREECE:
			c_player = HouseTypeClass::As_Reference(HOUSE_GREECE).Prefix;
			break;

		case SCEN_PLAYER_USSR:
			c_player = HouseTypeClass::As_Reference(HOUSE_USSR).Prefix;
			break;

		case SCEN_PLAYER_JP:
			c_player = HouseTypeClass::As_Reference(HOUSE_JP).Prefix;
			break;

		/*
		**	Multi player scenario.
		*/
		default:
			c_player = HouseTypeClass::As_Reference(HOUSE_MULTI1).Prefix;
			break;
	}

	/*
	** Set the directional character value.
	** If SCEN_DIR_NONE is specified, randomly pick a direction; otherwise, use 'E' or 'W'
	*/
	switch (dir) {
		case SCEN_DIR_EAST:
			c_dir = 'E';
			break;

		case SCEN_DIR_WEST:
			c_dir = 'W';
			break;

		default:
		case SCEN_DIR_NONE:
			c_dir = Percent_Chance(50) ? 'W' : 'E';
			break;
	}

	/*
	** Set the variation value.
	*/
	if (var == SCEN_VAR_NONE) {

		/*
		** Find which variations are available for this scenario
		*/
		for (i = SCEN_VAR_FIRST; i < SCEN_VAR_COUNT; i++) {
			sprintf(fname, "SC%c%02d%c%c.INI", c_player, scenario, c_dir, 'A' + i);
			if (!CCFileClass(fname).Is_Available()) {
				break;
			}
		}

		if (i==SCEN_VAR_FIRST) {
			c_var = 'X';						// indicates an error
		} else {
			c_var = 'A' + Random_Pick(0, i-1);
//			ScenVar = (ScenarioVarType)i;
		}
	} else {
		switch (var) {
			case SCEN_VAR_A:
				c_var = 'A';
				break;

			case SCEN_VAR_B:
				c_var = 'B';
				break;

			case SCEN_VAR_C:
				c_var = 'C';
				break;

			case SCEN_VAR_D:
				c_var = 'D';
				break;

			default:
				c_var = 'L';
				break;

		}
	}

	/*
	** generate the filename
	*/
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
//Mono_Printf("In set_scenario_name, scenario # = %d\n",scenario);Keyboard->Get();Keyboard->Get();
	if (scenario < 100) {
		sprintf(ScenarioName, "SC%c%02d%c%c.INI", c_player, scenario, c_dir, c_var);
	} else {
		char first = (scenario / 36) + 'A';
		char second = scenario % 36;

		if (second < 10) {
			second += '0';
		} else {
			second = (second - 10) + 'A';
		}

		sprintf(ScenarioName, "SC%c%c%c%c%c.INI", c_player, first, second, c_dir, c_var);
	}
#else
	sprintf(ScenarioName, "SC%c%02d%c%c.INI", c_player, scenario, c_dir, c_var);
#endif
}


void ScenarioClass::Set_Scenario_Name(char const * name)
{
	if (name != NULL) {
		strncpy(ScenarioName, name, sizeof(ScenarioName));
		ScenarioName[ARRAY_SIZE(ScenarioName)-1] = '\0';

		char buf[3];
		memcpy(buf, &ScenarioName[3], 2);
		buf[2] = '\0';
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		if (buf[0] > '9' || buf[1] > '9') {
			char first = buf[0];
			char second = buf[1];
			if (first <= '9') {
				first -= '0';
			} else {
				first -= 'A';
			}

			if (second <= '9') {
				second -= '0';
			} else {
				second = (second - 'A') + 10;
			}

			Scenario = (36 * first) + second;
		} else {
			Scenario = atoi(buf);
		}
#else
		Scenario = atoi(buf);
#endif
	}
}



/***********************************************************************************************
 * Read_Scenario_INI -- Read specified scenario INI file.                                      *
 *                                                                                             *
 *    Read in the scenario INI file. This routine only sets the game                           *
 *    globals with that data that is explicitly defined in the INI file.                       *
 *    The remaining necessary interpolated data is generated elsewhere.                        *
 *                                                                                             *
 * INPUT:                                                                                      *
 *          root      root filename for scenario file to read                                  *
 *                                                                                             *
 *          fresh      true = should the current scenario be cleared?                          *
 *                                                                                             *
 * OUTPUT:  bool; Was the scenario read successful?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.  V.Grippi added CS check 2/5/97                                                               *
 *=============================================================================================*/
bool Read_Scenario_INI(char * fname, bool )
{
//	char fname[_MAX_FNAME+_MAX_EXT];			// full INI filename


	ScenarioInit++;

	Clear_Scenario();
#ifdef OBSOLETE
	/*
	** If we are not dealing with scenario 1, or a multi player scenario
	** then make sure the correct disk is in the drive.
	*/
	if (RequiredCD != -2) {
		RequiredCD = -1;
	}
#endif

	/*
	** Only force a CD check if this is a single player game or if its
	** a multiplayer game on an official scenario. If its non-official
	** (a user scenario) then we dont care which CD is in because the
	** scenario is stored locally on the hard drive. In this case, we
	** have already verified its existance. ST 3/1/97 4:52PM.
	*/
#ifdef FIXIT_VERSION_3		//	Avoid CD check if official scenario was downloaded.
	if( ( Session.Type == GAME_NORMAL || Session.ScenarioIsOfficial ) && _stricmp( Scen.ScenarioName, "download.tmp" ) ){
#else
	if (Session.Type == GAME_NORMAL || Session.ScenarioIsOfficial){
#endif

		/*
		** If this is scenario 1 then it should be on all CDs unless its an ant scenario
		*/
		if (Scen.Scenario == 1 && Scen.ScenarioName[2] != 'A') {
	   	RequiredCD = -1;
		} else {
//			Mono_Printf("Read_SCen_INI scenario is: %s\n", Scen.ScenarioName);
			/*
			** If this is a multiplayer scenario we need to find out if its a counterstrike
			** scenario. If so then we need CD 2. The original multiplayer scenarios are on
			** all CDs.
			*/
			if (Session.Type != GAME_NORMAL) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				RequiredCD = -1;	// default that any CD will do.
// If it's a counterstrike mission, require the counterstrike CD, unless the
// Aftermath CD is already in the drive, in which case, leave it there.
// Note, this works because this section only tests for multiplayer scenarios.
				if (Is_Mission_Counterstrike(Scen.ScenarioName)) {
					RequiredCD = 2;
					if( Is_Aftermath_Installed() || Get_CD_Index(CCFileClass::Get_CD_Drive(), 1*60) == 3 )
					{
						RequiredCD = 3;
					}
				}
				if(Is_Mission_Aftermath(Scen.ScenarioName)) {
					RequiredCD = 3;
				}
#else
				if (Scen.Scenario > 24) {
					RequiredCD = 2;
				} else {
					RequiredCD = -1;
				}
#endif
			} else {

				/*
				** This is a solo game. If the scenario number is >= 20 or its an ant mission
				** then we need the counterstrike CD (2)
				*/
     			if (Scen.Scenario >= 20 || Scen.ScenarioName[2] == 'A') {
					RequiredCD = 2;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
     				if (Scen.Scenario >= 36 && Scen.ScenarioName[2] != 'A') {
						RequiredCD = 3;
#ifdef BOGUSCD
	RequiredCD = -1;
#endif
					}
#endif
				} else {

					/*
					** This is a solo mission from the original Red Alert. Choose the Soviet or
					** allied CD depending on the scenario name.
					*/
					if (Scen.ScenarioName[2] == 'U') {
						RequiredCD = 1;
					} else {
						if (Scen.ScenarioName[2] == 'G') {
//							Mono_Printf("We are setting REquiredCD to 0");
							RequiredCD = 0;

						}
					}
     			}
			}
		}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
// If we're asking for a CD swap, check to see if we need to set the palette
// to avoid a black screen.  If this is a normal RA game, and the CD being
// requested is an RA CD, then don't set the palette, leave the map screen up.

#ifdef FIXIT_VERSION_3
		int cd_index = Get_CD_Index(CCFileClass::Get_CD_Drive(), 1*60);
		if( !( Using_DVD() && cd_index == 5 ) && cd_index != RequiredCD ) {
#else
		if (Get_CD_Index(CCFileClass::Get_CD_Drive(), 1*60) != RequiredCD) {
#endif
			if ((RequiredCD == 0 || RequiredCD == 1) && Session.Type == GAME_NORMAL) {
				SeenPage.Clear();
			}
			GamePalette.Set(FADE_PALETTE_FAST, Call_Back);
		}
#endif
		if (!Force_CD_Available(RequiredCD)) {
			Prog_End("Read_Scenario_INI Force_CD_Available failed", true);
			if (!RunningAsDLL) {	//PG
				Emergency_Exit(EXIT_FAILURE);
			}
     	}
	} else {
		/*
		** This is a user scenario so any old CD will do.
		*/
		RequiredCD = -1;
	}


	/*
	**	Create scenario filename and read the file.
	*/
//	sprintf(fname, "%s.INI", root);
	CCINIClass ini;
	CCFileClass file(fname);
//	file.Cache();

	int result = ini.Load(file, true);
	if (result == 0) {
//		Mono_Printf("ini.Load failed");
		GlyphX_Debug_Print("Failed to load scenario file");
		GlyphX_Debug_Print(fname);
		return(false);
	}

	GlyphX_Debug_Print("Loaded scenario file");
	GlyphX_Debug_Print(file);

	/*
	** If the scenario digest is wrong then the return code will be a 2.
	*/
	if (result == 2) {
//		if (Session.Type == GAME_NORMAL || Session.ScenarioIsOfficial) {
			/*
			**	Make a special exception so that multiplayer maps from 1 through
			**	24 will not care if the message digest is in error. All other
			**	maps will abort the scenario load.
			*/
			if (Scen.ScenarioName[2] != 'M' || Scen.Scenario >= 25) {
#if (1)				
				GlyphX_Debug_Print("Scenario digest is wrong");
#else				
				GamePalette.Set();
				WWMessageBox().Process(TXT_SCENARIO_ERROR, TXT_OK);
#endif
#ifdef RELEASE_VERSION
				return(false);
#endif
			}
//		}
	}

	/*
	**	Reset the rules values to their initial settings.
	*/
#ifdef FIXIT_NAME_OVERRIDE
	for (int index = 0; index < ARRAY_SIZE(NameOverride); index++) {
		if (NameOverride[index] != NULL) free((void*)NameOverride[index]);
		NameOverride[index] = NULL;
		NameIDOverride[index] = 0;
	}
	if (Session.Type == GAME_NORMAL)  {
		Special.IsShadowGrow = false;
	}
#endif

#ifdef FIXIT_ANTS
	Session.Messages.Reset();
//	Session.Messages.Add_Message(NULL, 0, NULL, PCOLOR_GREEN, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, 1);
//	Session.Messages.Add_Message(NULL, 0, NULL, PCOLOR_GREEN, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, 1);
//	Session.Messages.Add_Message(NULL, 0, NULL, PCOLOR_GREEN, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, 1);
//	Session.Messages.Add_Message(NULL, 0, NULL, PCOLOR_GREEN, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, 1);
//	Session.Messages.Add_Message(NULL, 0, NULL, PCOLOR_GREEN, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, 1);
//	Session.Messages.Add_Message(NULL, 0, NULL, PCOLOR_GREEN, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, 1);
	WeaponTypeClass::As_Pointer(WEAPON_FLAMER)->Sound = VOC_NONE;
	InfantryTypeClass::As_Reference(INFANTRY_THIEF).IsDoubleOwned = false;
	InfantryTypeClass::As_Reference(INFANTRY_E4).IsDoubleOwned = false;
	InfantryTypeClass::As_Reference(INFANTRY_SPY).PrimaryWeapon = NULL;
	InfantryTypeClass::As_Reference(INFANTRY_SPY).SecondaryWeapon = NULL;
	InfantryTypeClass::As_Reference(INFANTRY_GENERAL).IsBomber = false;
	UnitTypeClass::As_Reference(UNIT_HARVESTER).IsExploding = false;
	UnitTypeClass::As_Reference(UNIT_ANT1).Level = -1;
	UnitTypeClass::As_Reference(UNIT_ANT2).Level = -1;
	UnitTypeClass::As_Reference(UNIT_ANT3).Level = -1;
	BuildingTypeClass::As_Reference(STRUCT_QUEEN).Level = -1;
	BuildingTypeClass::As_Reference(STRUCT_LARVA1).Level = -1;
	BuildingTypeClass::As_Reference(STRUCT_LARVA2).Level = -1;
#endif


	Rule.General(RuleINI);
	Rule.Recharge(RuleINI);
	Rule.AI(RuleINI);
	Rule.Powerups(RuleINI);
	Rule.Land_Types(RuleINI);
	Rule.Themes(RuleINI);
	Rule.IQ(RuleINI);
	Rule.Objects(RuleINI);
	Rule.Difficulty(RuleINI);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98 - Except does this _change_ any rules, or just add to them? - Just adds.
	Rule.General(AftermathINI);
	Rule.Recharge(AftermathINI);
	Rule.AI(AftermathINI);
	Rule.Powerups(AftermathINI);
	Rule.Land_Types(AftermathINI);
	Rule.Themes(AftermathINI);
	Rule.IQ(AftermathINI);
	Rule.Objects(AftermathINI);
	Rule.Difficulty(AftermathINI);
#endif

	/*
	**	For civilians, remove the graphics name override from the base rules (can still be overridden in scenario-specific INI).
	*/
	for (int iindex = 0; iindex < InfantryTypes.Count(); iindex++) {
		InfantryTypeClass* itype = InfantryTypes.Ptr(iindex);
		if (itype->IsCivilian) {
			itype->GraphicName[0] = '\0';
		}
	}

	/*
	**	Override any rules values specified in this
	**	particular scenario file.
	*/
	Rule.General(ini);
	Rule.Recharge(ini);
	Rule.AI(ini);
	Rule.Powerups(ini);
	Rule.Land_Types(ini);
	Rule.Themes(ini);
	Rule.IQ(ini);
	Rule.Objects(ini);
	Rule.Difficulty(ini);

	/*
	**	Fix a legacy bug with England and France country bonuses
	*/
	if (Session.Type != GAME_NORMAL) {
		HouseTypeClass::As_Reference(HOUSE_ENGLAND).ArmorBias = fixed(9, 10);
		HouseTypeClass::As_Reference(HOUSE_FRANCE).ROFBias = fixed(9, 10);
	}

	/*
	** Init the Scenario CRC value
	*/
	ScenarioCRC = 0;
#ifdef TOFIX
	len = strlen(buffer);
	for (int i = 0; i < len; i++) {
		val = (unsigned char)buffer[i];
		Add_CRC(&ScenarioCRC, (unsigned long)val);
	}
#endif

	/*
	**	Fetch the appropriate movie names from the INI file.
	*/
	const char * const BASIC = "Basic";
	ini.Get_String(BASIC, "Name", "<none>", Scen.Description, sizeof(Scen.Description));
	Scen.IntroMovie = ini.Get_VQType(BASIC, "Intro", Scen.IntroMovie);
	Scen.BriefMovie = ini.Get_VQType(BASIC, "Brief", Scen.BriefMovie);
	Scen.WinMovie = ini.Get_VQType(BASIC, "Win", Scen.WinMovie);
	Scen.WinMovie2 = ini.Get_VQType(BASIC, "Win2", Scen.WinMovie2);
	Scen.WinMovie3 = ini.Get_VQType(BASIC, "Win3", Scen.WinMovie3);
	Scen.WinMovie4 = ini.Get_VQType(BASIC, "Win4", Scen.WinMovie4);
	Scen.LoseMovie = ini.Get_VQType(BASIC, "Lose", Scen.LoseMovie);
	Scen.ActionMovie = ini.Get_VQType(BASIC, "Action", Scen.ActionMovie);
	Scen.IsToCarryOver = ini.Get_Bool(BASIC, "ToCarryOver", Scen.IsToCarryOver);
	Scen.IsToInherit = ini.Get_Bool(BASIC, "ToInherit", Scen.IsToInherit);
	Scen.IsInheritTimer = ini.Get_Bool(BASIC, "TimerInherit", Scen.IsInheritTimer);
	Scen.IsEndOfGame = ini.Get_Bool(BASIC, "EndOfGame", Scen.IsEndOfGame);
	Scen.IsTanyaEvac = ini.Get_Bool(BASIC, "CivEvac", Scen.IsTanyaEvac);
	Scen.TransitTheme = ini.Get_ThemeType(BASIC, "Theme", THEME_NONE);
	NewINIFormat = ini.Get_Int(BASIC, "NewINIFormat", 0);
	Scen.CarryOverPercent = ini.Get_Fixed(BASIC, "CarryOverMoney", Scen.CarryOverPercent);
	Scen.CarryOverPercent = Saturate(Scen.CarryOverPercent, 1);
	Scen.CarryOverCap = ini.Get_Int(BASIC, "CarryOverCap", Scen.CarryOverCap);
	Scen.IsNoSpyPlane = ini.Get_Bool(BASIC, "NoSpyPlane", Scen.IsNoSpyPlane);
	Scen.IsSkipScore = ini.Get_Bool(BASIC, "SkipScore", Scen.IsSkipScore);
	Scen.IsOneTimeOnly = ini.Get_Bool(BASIC, "OneTimeOnly", Scen.IsOneTimeOnly);
	Scen.IsNoMapSel = ini.Get_Bool(BASIC, "SkipMapSelect", Scen.IsNoMapSel);
	Scen.IsTruckCrate = ini.Get_Bool(BASIC, "TruckCrate", Scen.IsTruckCrate);
	Scen.IsMoneyTiberium = ini.Get_Bool(BASIC, "FillSilos", Scen.IsMoneyTiberium);
	Scen.Percent = ini.Get_Int(BASIC, "Percent", Scen.Percent);

	/*
	**	Read in the specific information for each of the house types.  This creates
	**	the houses of different types.
	*/
	HouseClass::Read_INI(ini);
	Call_Back();

	/*
	**	Read in the team-type data. The team types must be created before any
	**	triggers can be created.
	*/
	TeamTypeClass::Read_INI(ini);
	Call_Back();

	/*
	**	Read in the trigger data. The triggers must be created before any other
	**	objects can be initialized.
	*/
	TriggerTypeClass::Read_INI(ini);
	Call_Back();


	/*
	**	Read in the map control values. This includes dimensions
	**	as well as theater information.
	*/
	Map.Read_INI(ini);
	Call_Back();

	/*
	**	Assign PlayerPtr by reading the player's house from the INI;
	**	Must be done before any TechnoClass objects are created.
	*/
	if (Session.Type == GAME_NORMAL) {
		PlayerPtr = HouseClass::As_Pointer(ini.Get_HousesType(BASIC, "Player", HOUSE_GREECE));
		PlayerPtr->Assign_Handicap(Scen.Difficulty);
		int carryover;
		if (Scen.CarryOverCap != -1) {
			carryover = min(Scen.CarryOverMoney * Scen.CarryOverPercent, Scen.CarryOverCap);
		} else {
			carryover = Scen.CarryOverMoney * Scen.CarryOverPercent;
		}
		PlayerPtr->Credits += carryover;
		PlayerPtr->Control.InitialCredits += carryover;
	} else {
		//Call the new Assign_Houses function. ST - 8/8/2019 12:35PM
		//Assign_Houses();
		GlyphX_Assign_Houses();
	}
	PlayerPtr->IsHuman = true;
	PlayerPtr->IsPlayerControl = true;

//	if (NewINIFormat < 2 || !ini.Is_Present("MapPack")) {
//		Map.Read_Binary(root, &ScenarioCRC);
//	}


	/*
	**	Read in and place the 3D terrain objects.
	*/
	TerrainClass::Read_INI(ini);
	Call_Back();
	/*
	**	Read in and place the units (all sides).
	*/
	UnitClass::Read_INI(ini);
	Call_Back();

    	VesselClass::Read_INI(ini);
	Call_Back();

	/*
	**	Read in and place the infantry units (all sides).
	*/
	InfantryClass::Read_INI(ini);
	Call_Back();



	/*
	**	Read in and place all the buildings on the map.
	*/
	BuildingClass::Read_INI(ini);
	Call_Back();

	/*
	**	Read in the AI's base information.
	*/
	Base.Read_INI(ini);
	Call_Back();

      	/*
	**	Read in any normal overlay objects.
	*/
	OverlayClass::Read_INI(ini);
	Call_Back();

	/*
	**	Read in any smudge overlays.
	*/
	SmudgeClass::Read_INI(ini);
	Call_Back();

	/*	Moved above ini.Get_TextBlock(...) so Xlat mission.ini could be loaded
	**	If the briefing text could not be found in the INI file, then search
	**	the mission.ini file.  VG 10/17/96
	*/
	INIClass mini;
	mini.Load(CCFileClass("MISSION.INI"));
	mini.Get_TextBlock(fname, Scen.BriefingText, sizeof(Scen.BriefingText));

	/*
	**	Read in any briefing text.
	*/
	if (Scen.BriefingText[0] == '\0') {
		ini.Get_TextBlock("Briefing", Scen.BriefingText, sizeof(Scen.BriefingText));
	}
	/*
	**	Perform a final overpass of the map. This handles smoothing of certain
	**	types of terrain (tiberium).
	*/
	Map.Overpass();
	Call_Back();

	/*
	**	Special cases:
	**		Gold Rush multiplayer map cell 9033 - LAND_ROCK
	**		USSR mission 13 - fixup trigger action
	**		Allied mission 9A - fail mission if tech center is destroyed before being spied
	**		Aftermath: Brother in Arms - have transports move to separate waypoints
	**		Aftermath: Let's Make a Steal - Make the pillboxes un-capturable
	**		Counterstrike: Soviet Soldier Volkov and Chitzkoi / Deus Ex Machina - Sniper burst fix
	*/
	if (_stricmp(Scen.ScenarioName, "scmh8ea.ini") == 0) {
		Map[(CELL)9033].Override_Land_Type(LAND_ROCK);
	}

	if (_stricmp(Scen.ScenarioName, "scu13ea.ini") == 0) {
		TriggerTypeClass* trigger = TriggerTypes.Ptr(11);
		trigger->Action1.Trigger.Set_Raw(39);
	}

	if (_stricmp(Scen.ScenarioName, "scg09ea.ini") == 0) {
		TriggerTypeClass* spyd_trigger = TriggerTypeClass::From_Name("Spyd");
		assert(spyd_trigger != NULL);

		TriggerTypeClass* kos_trigger = new TriggerTypeClass();
		kos_trigger->IsPersistant = spyd_trigger->IsPersistant;
		kos_trigger->House = spyd_trigger->House;
		kos_trigger->EventControl = MULTI_LINKED;
		kos_trigger->ActionControl = MULTI_ONLY;
		kos_trigger->Event1.Event = spyd_trigger->Event1.Event;
		kos_trigger->Event1.Data = spyd_trigger->Event1.Data;
		kos_trigger->Event2.Event = TEVENT_DESTROYED;
		kos_trigger->Event2.Data.Value = 0;
		kos_trigger->Action1.Action = TACTION_SET_GLOBAL;
		kos_trigger->Action1.Data.Value = 22;
		kos_trigger->Action2.Action = TACTION_SET_GLOBAL;
		kos_trigger->Action2.Data.Value = 23;

		spyd_trigger->Event1.Event = TEVENT_GLOBAL_SET;
		spyd_trigger->Event1.Data.Value = 22;

		for (int index = 0; index < Buildings.Count(); index++) {
			BuildingClass* building = Buildings.Ptr(index);
			if (building->Trigger.Is_Valid() && (building->Trigger->Class == spyd_trigger)) {
				building->Attach_Trigger(Find_Or_Make(kos_trigger));
			}
		}

		TriggerTypeClass* los3_trigger = new TriggerTypeClass();
		los3_trigger->IsPersistant = spyd_trigger->IsPersistant;
		los3_trigger->House = spyd_trigger->House;
		los3_trigger->EventControl = MULTI_AND;
		los3_trigger->ActionControl = MULTI_AND;
		los3_trigger->Event1.Event = TEVENT_GLOBAL_SET;
		los3_trigger->Event1.Data.Value = 23;
		los3_trigger->Event2.Event = TEVENT_GLOBAL_CLEAR;
		los3_trigger->Event2.Data.Value = 22;
		los3_trigger->Action1.Action = TACTION_LOSE;
		los3_trigger->Action1.Data.Value = -255;
		los3_trigger->Action2.Action = TACTION_TEXT_TRIGGER;
		los3_trigger->Action2.Data.Value = 54;
	}

	if (_stricmp(Scen.ScenarioName, "scu46ea.ini") == 0) {
		Scen.Waypoint[20] = 9915;
		Scen.Waypoint[21] = 9919;
		Map[Scen.Waypoint[20]].IsWaypoint = 1;
		Map[Scen.Waypoint[21]].IsWaypoint = 1;

		TeamTypeClass* rnf1_team = TeamTypeClass::From_Name("rnf1");
		assert(rnf1_team != NULL);
		rnf1_team->MissionList[0].Data.Value = 20;

		TeamTypeClass* rnf2_team = TeamTypeClass::From_Name("rnf2");
		assert(rnf2_team != NULL);
		rnf2_team->MissionList[0].Data.Value = 21;
	}

	if (_stricmp(Scen.ScenarioName, "scu42ea.ini") == 0) {
		BuildingTypeClass::As_Reference(STRUCT_PILLBOX).IsCaptureable = false;
	}

	if ((_stricmp(Scen.ScenarioName, "scu35ea.ini") == 0) || (_stricmp(Scen.ScenarioName, "scu47ea.ini") == 0)) {
		WeaponTypeClass::As_Pointer(Weapon_From_Name("Sniper"))->Burst = 2;
	}

	/*
	**	Multi-player last-minute fixups:
	**	- If computer players are disabled, remove all computer-owned houses
	**	- If bases are disabled, create the scenario dynamically
	**	- Remove any flag spot overlays lying around
	**	- If capture-the-flag is enabled, assign flags to cells.
	*/
	if (Session.Type != GAME_NORMAL /*|| Scen.ScenPlayer == SCEN_PLAYER_2PLAYER || Scen.ScenPlayer == SCEN_PLAYER_MPLAYER*/) {

		/*
		**	If Ghosts are disabled and we're not editing, remove computer players
		**	(Must be done after all objects are read in from the INI)
		*/
		if ( (Session.Options.AIPlayers + Session.Players.Count() < Rule.MaxPlayers) && !Debug_Map) {
			Remove_AI_Players();
		}

		/*
		**	Units must be created for each house.  If bases are ON, this routine
		**	will create an MCV along with the units; otherwise, it will just create
		**	a whole bunch of units.  Session.Options.UnitCount is the total # of units
		**	to create.
		*/
		if (!Debug_Map) {
			int save_init = ScenarioInit;			// turn ScenarioInit off
			ScenarioInit = 0;
			Create_Units(ini.Get_Bool("Basic", "Official", false));
			ScenarioInit = save_init;				// turn ScenarioInit back on
		}

		/*
		**	Place crates if random crates are enabled for
		**	this scenario.
		*/
		if (Session.Options.Goodies) {
			int count = max(Rule.CrateMinimum, Session.NumPlayers);
			count = min(count, Rule.CrateMaximum);
			for (int index = 0; index < count; index++) {
				Map.Place_Random_Crate();
			}
		}
	}

	Call_Back();

	/*
	**	Return with flag saying that the scenario file was read.
	*/
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98 - Added runtime check.
	if( Is_Aftermath_Installed() )
	{
		if (Session.Type == GAME_SKIRMISH || Session.Type == GAME_GLYPHX_MULTIPLAYER) {
			bAftermathMultiplayer = NewUnitsEnabled = OverrideNewUnitsEnabled;
		}
	}	
#endif
	ScenarioInit--;
	return(true);
}


/***********************************************************************************************
 * Write_Scenario_INI -- Write the scenario INI file.                                          *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      root      root filename for the scenario                                               *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   05/11/1995 JLB : Updates movie data.                                                      *
 *=============================================================================================*/
void Write_Scenario_INI(char * fname)
{
#ifndef CHEAT_KEYS
	fname = fname;
#else
//	CCFileClass file(fname);

	CCINIClass ini;

	/*
	**	Preload the old scenario if it is present because there may
	**	be some fields in the INI that are processed but not written
	**	out. Preloading the scenario will preserve these manually
	**	maintained entries.
	*/
	if (CCFileClass(fname).Is_Available()) {
		ini.Load(CCFileClass(fname), true);
	}

	static char const * const BASIC = "Basic";
	ini.Clear(BASIC);
	ini.Put_String(BASIC, "Name", Scen.Description);
	ini.Put_VQType(BASIC, "Intro", Scen.IntroMovie);
	ini.Put_VQType(BASIC, "Brief", Scen.BriefMovie);
	ini.Put_VQType(BASIC, "Win", Scen.WinMovie);
	ini.Put_VQType(BASIC, "Lose", Scen.LoseMovie);
	ini.Put_VQType(BASIC, "Action", Scen.ActionMovie);
	ini.Put_HousesType(BASIC, "Player", PlayerPtr->Class->House);
	ini.Put_ThemeType(BASIC, "Theme", Scen.TransitTheme);
	ini.Put_Fixed(BASIC, "CarryOverMoney", Scen.CarryOverPercent);
	ini.Put_Bool(BASIC, "ToCarryOver", Scen.IsToCarryOver);
	ini.Put_Bool(BASIC, "ToInherit", Scen.IsToInherit);
	ini.Put_Bool(BASIC, "TimerInherit", Scen.IsInheritTimer);
	ini.Put_Bool(BASIC, "CivEvac", Scen.IsTanyaEvac);
	ini.Put_Int(BASIC, "NewINIFormat", 3);
	ini.Put_Int(BASIC, "CarryOverCap", Scen.CarryOverCap/100);
	ini.Put_Bool(BASIC, "EndOfGame", Scen.IsEndOfGame);
	ini.Put_Bool(BASIC, "NoSpyPlane", Scen.IsNoSpyPlane);
	ini.Put_Bool(BASIC, "SkipScore", Scen.IsSkipScore);
	ini.Put_Bool(BASIC, "OneTimeOnly", Scen.IsOneTimeOnly);
	ini.Put_Bool(BASIC, "SkipMapSelect", Scen.IsNoMapSel);
	ini.Put_Bool(BASIC, "Official", true);
	ini.Put_Bool(BASIC, "FillSilos", Scen.IsMoneyTiberium);
	ini.Put_Bool(BASIC, "TruckCrate", Scen.IsTruckCrate);
	ini.Put_Int(BASIC, "Percent", Scen.Percent);

	HouseClass::Write_INI(ini);
	TeamTypeClass::Write_INI(ini);
	TriggerTypeClass::Write_INI(ini);
	Map.Write_INI(ini);
	TerrainClass::Write_INI(ini);
	UnitClass::Write_INI(ini);
	VesselClass::Write_INI(ini);
	InfantryClass::Write_INI(ini);
	BuildingClass::Write_INI(ini);
	Base.Write_INI(ini);
	OverlayClass::Write_INI(ini);
	SmudgeClass::Write_INI(ini);

	if (strlen(Scen.BriefingText)) {
		ini.Put_TextBlock("Briefing", Scen.BriefingText);
	}
//	sprintf(fname, "%s.INI", root);
	RawFileClass rawfile(fname);
	ini.Save(rawfile, true);
#endif
}


/***********************************************************************************************
 * Assign_Houses -- Assigns multiplayer houses to various players                              *
 *                                                                                             *
 * This routine assigns all players to a multiplayer house slot; it forms network connections  *
 * to each player.  The Connection ID used is the value for that player's HousesType.			  *
 *                                                                                             *
 * PlayerPtr is also set here.																					  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *		This routine assumes the 'Players' vector has been properly filled in with players'		  *
 *		names, addresses, color, etc.																				  *
 *		Also, it's assumed that the HouseClass's have all been created & initialized.				  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/09/1995 BRR : Created.                                                                 *
 *   07/14/1995 JLB : Records name of player in house structure.                               *
 *=============================================================================================*/
void Assign_Houses(void)
{
	int assigned[MAX_PLAYERS];
	int color_used[8];
	int i,j;
	HousesType house;
	HouseClass * housep;
	int lowest_color;
	int index;
	HousesType pref_house;
	int color;

	//------------------------------------------------------------------------
	// Initialize
	//------------------------------------------------------------------------
	for (i = 0; i < MAX_PLAYERS; i++) {
		assigned[i] = 0;
		color_used[i] = 0;
	}

//	debugprint( "Assign_Houses()\n" );
	//------------------------------------------------------------------------
	// Assign each player in 'Players' to a multiplayer house.  Players will
	// be sorted by their chosen color value (this value must be unique among
	// all the players).
	//------------------------------------------------------------------------
	for (i = 0; i < Session.Players.Count(); i++) {

		//.....................................................................
		// Find the player with the lowest color index
		//.....................................................................
		index = 0;
		lowest_color = 255;
		for (j = 0; j < Session.Players.Count(); j++) {
			//..................................................................
			// If we've already assigned this house, skip it.
			//..................................................................
			if (assigned[j]) {
				continue;
			}
			if (Session.Players[j]->Player.Color < lowest_color) {
				lowest_color = Session.Players[j]->Player.Color;
				index = j;
			}
		}

		//.....................................................................
		// Mark this player as having been assigned.
		//.....................................................................
		assigned[index] = 1;
		color_used[Session.Players[index]->Player.Color] = 1;

		//.....................................................................
		// Assign the lowest-color'd player to the next available slot in the
		// HouseClass array.
		//.....................................................................
		house = (HousesType)(i + HOUSE_MULTI1);
		housep = HouseClass::As_Pointer(house);
		memset((char *)housep->IniName, 0, MPLAYER_NAME_MAX);
		strncpy((char *)housep->IniName, Session.Players[index]->Name, MPLAYER_NAME_MAX - 1);
#ifdef WOLAPI_INTEGRATION
		//	Make another copy of name, permanent throughout entire game.
		strncpy((char *)housep->InitialName, Session.Players[index]->Name, MPLAYER_NAME_MAX - 1);
#endif
		housep->IsHuman = true;
		housep->Init_Data((PlayerColorType)(Session.Players[index]->Player.Color),
			Session.Players[index]->Player.House, Session.Options.Credits);
		if (index == 0) {
			PlayerPtr = housep;
		}
		/*
		**	Convert the build level into an actual tech level to assign to the house.
		**	There isn't a one-to-one correspondence.
		*/
		housep->Control.TechLevel = _build_tech[BuildLevel];

		housep->Assign_Handicap(Scen.Difficulty);

		//.....................................................................
		// Record where we placed this player
		//.....................................................................
		Session.Players[index]->Player.ID = house;

//		debugprint( "Assigned ID of %i to %s\n", house, Session.Players[index]->Name );
	}

	//------------------------------------------------------------------------
	// Now assign computer players to the remaining houses.
	//------------------------------------------------------------------------
	for (i = Session.Players.Count(); i < Session.Players.Count() + Session.Options.AIPlayers; i++) {
		house = (HousesType)(i + HOUSE_MULTI1);
		housep = HouseClass::As_Pointer(house);
		if (Percent_Chance(50)) {
			pref_house = HOUSE_GREECE;
		} else {
			pref_house = HOUSE_USSR;
		}

		//.....................................................................
		// Pick a color for this house; keep looping until we find one.
		//.....................................................................
		while (1) {
			color = Random_Pick(0, 7);
			if (color_used[color] == false) {
				break;
			}
		}
		color_used[color] = true;

		//.....................................................................
		// Set up the house
		//.....................................................................
//		housep->Control.MaxUnit = 80;
//		housep->Control.MaxInfantry = 60;
//		housep->Control.MaxBuilding = 60;
//		housep->Control.MaxVessel = 60;
		housep->IsHuman = false;
		housep->IsStarted = true;

		strcpy(housep->IniName, Text_String(TXT_COMPUTER));

		if (Session.Type != GAME_NORMAL) {
			housep->IQ = Rule.MaxIQ;
		}

		housep->Init_Data((PlayerColorType)color, pref_house, Session.Options.Credits);
		housep->Control.TechLevel = _build_tech[BuildLevel];
//		housep->Control.TechLevel = BuildLevel;

		DiffType difficulty = Scen.CDifficulty;

		if (Session.Players.Count() > 1 && Rule.IsCompEasyBonus && difficulty > DIFF_EASY) {
			difficulty = (DiffType)(difficulty - 1);
		}
		housep->Assign_Handicap(difficulty);
	}

	for (i = Session.Players.Count()+Session.Options.AIPlayers; i < Rule.MaxPlayers; i++) {
		house = (HousesType)(i + HOUSE_MULTI1);
		housep = HouseClass::As_Pointer(house);
		if (housep != NULL) {
			housep->IsDefeated = true;
		}
	}
}


/***********************************************************************************************
 * Remove_AI_Players -- Removes the computer AI houses & their units                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/09/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static void Remove_AI_Players(void)
{
	int i;
	int aicount = 0;
	HousesType house;
	HouseClass * housep;

#if (0)
	for (i = 0; i < MAX_PLAYERS; i++) {
		house = (HousesType)(i + (int)HOUSE_MULTI1);
		housep = HouseClass::As_Pointer (house);
		if (housep->IsHuman == false) {
			aicount++;
			if(aicount > Session.Options.AIPlayers) {
				housep->Clobber_All();
			}
		}
	}
#else
	/*
	** AI players are set up like human players now. ST - 8/13/2019 1:32PM
	*/
	for (i = 0; i < MAX_PLAYERS; i++) {
		if (i >= Session.Players.Count()) {
			house = (HousesType)(i + (int)HOUSE_MULTI1);
			housep = HouseClass::As_Pointer (house);
			if (housep->IsHuman == false) {
				housep->Clobber_All();
			}
		}
	}

#endif
}


#define USE_GLYPHX_START_LOCATIONS 1


/***********************************************************************************************
 * Create_Units -- Creates infantry & units, for non-base multiplayer                          *
 *                                                                                             *
 * This routine uses data tables to determine which units to create for either                 *
 * a GDI or NOD house, and how many of each.                                                   *
 *                                                                                             *
 * It also sets each house's FlagHome & FlagLocation to the Waypoint selected                  *
 * as that house's "home" cell.                                                                *
 *                                                                                             *
 * INPUT:   official -- Directs the placement logic to use the full set of waypoints rather    *
 *                      than biasing toward the first four.                                    *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/09/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static int ReserveInfantryIndex = 0;
static void Reserve_Infantry()
{
	if (Infantry.Count() == Infantry.Length()) {
		delete Infantry.Ptr(ReserveInfantryIndex);
		ReserveInfantryIndex = (ReserveInfantryIndex + 1) % Infantry.Length();
	}
}

static int ReserveUnitIndex = 0;
static void Reserve_Unit()
{
	if (Units.Count() == Units.Length()) {
		delete Units.Ptr(ReserveUnitIndex);
		ReserveUnitIndex = (ReserveUnitIndex + 1) % Units.Length();
	}
}

static void Create_Units(bool official)
{
	static struct {
		int MinLevel;
		UnitType AllyType[2];
		UnitType SovietType[2];
	} utable[] = {
		{4,	{UNIT_MTANK2,   UNIT_LTANK}, 	{UNIT_MTANK,		 UNIT_NONE}},
		{5,	{UNIT_APC,      UNIT_NONE},  	{UNIT_V2_LAUNCHER, UNIT_NONE}},
		{8,	{UNIT_ARTY,     UNIT_JEEP},  	{UNIT_MTANK,		 UNIT_NONE}},
		{10,	{UNIT_MTANK2,   UNIT_MTANK2},	{UNIT_HTANK,		 UNIT_NONE}}
	};
	static int num_units[ARRAY_SIZE(utable)];		// # of each type of unit to create
	int tot_units;												// total # units to create

	static struct {
		int MinLevel;
		int AllyCount;
		InfantryType AllyType;
		int SovietCount;
		InfantryType SovietType;
	} itable[] = {
		{0,	1,INFANTRY_E1,				1,INFANTRY_E1},
		{2,	1,INFANTRY_E3,				1,INFANTRY_E2},
		{4,	1,INFANTRY_E3,				1,INFANTRY_E4},

// removed because of bug B478 (inappropriate infantry given in a bases off scenario).
//		{5,	1,INFANTRY_RENOVATOR,	1,INFANTRY_RENOVATOR},
//		{6,	1,INFANTRY_SPY,			1,INFANTRY_DOG},
//		{10,	1,INFANTRY_THIEF,			1,INFANTRY_DOG},
//		{12,	1,INFANTRY_MEDIC,			2,INFANTRY_DOG}
	};
	static int num_infantry[ARRAY_SIZE(itable)];// # of each type of infantry to create
	int tot_infantry;											// total # infantry to create


	CELL centroid;			// centroid of this house's stuff
	CELL centerpt;			// centroid for a category of objects, as a CELL

	int u_limit=0;			// last allowable index of units for this BuildLevel
	int i_limit=0;			// last allowable index of infantry for this BuildLevel
	TechnoClass * obj;		// newly-created object
	int i,j,k;				// loop counters
	int scaleval;			// value to scale # units or infantry

	ReserveInfantryIndex = ReserveUnitIndex = 0;

	/*
	**	For the current BuildLevel, find the max allowable index into the tables
	*/
	for (i = 0; i < ARRAY_SIZE(utable); i++) {
		if (PlayerPtr->Control.TechLevel >= utable[i].MinLevel) {
			u_limit = i+1;
		}
	}
	for (i = 0; i < ARRAY_SIZE(itable); i++) {
		if (PlayerPtr->Control.TechLevel >= itable[i].MinLevel) {
			i_limit = i+1;
		}
	}

	/*
	**	Compute how many of each buildable category to create
	*/
	/*
	**	Compute allowed # units
	*/
	tot_units = (Session.Options.UnitCount * 2) / 3;
	if (u_limit == 0) tot_units = 0;

	/*
	**	Init # of each category to 0
	*/
	for (i = 0; i < u_limit; i++) {
		num_units[i] = 0;
	}

	/*
	**	Increment # of each category, until we've used up all units
	*/
	j = 0;
	for (i = 0; i < tot_units; i++) {
		num_units[j]++;
		j++;
		if (j >= u_limit) {
			j = 0;
		}
	}

	/*
	**	Compute allowed # infantry
	*/
	tot_infantry = Session.Options.UnitCount - tot_units;

	/*
	**	Init # of each category to 0
	*/
	for (i = 0; i < i_limit; i++) {
		num_infantry[i] = 0;
	}

	/*
	**	Increment # of each category, until we've used up all infantry
	*/
	j = 0;
	for (i = 0; i < tot_infantry; i++) {
		num_infantry[j]++;
		j++;
		if (j >= i_limit) {
			j = 0;
		}
	}

	/*
	**	Build a list of the valid waypoints. This normally shouldn't be
	**	necessary because the scenario level designer should have assigned
	**	valid locations to the first N waypoints, but just in case, this
	**	loop verifies that.
	*/

	const unsigned int MAX_STORED_WAYPOINTS = 26;

	bool taken[MAX_STORED_WAYPOINTS];
	CELL waypts[MAX_STORED_WAYPOINTS];
	assert(Rule.MaxPlayers < ARRAY_SIZE(waypts));
	int num_waypts = 0;

	/*
	**	Calculate the number of waypoints (as a minimum) that will be lifted from the
	**	mission file. Bias this number so that only the first 4 waypoints are used
	**	if there are 4 or fewer players. Unofficial maps will pick from all the
	**	available waypoints.
	*/
#ifndef USE_GLYPHX_START_LOCATIONS
	int look_for = max(4, Session.Players.Count()+Session.Options.AIPlayers);
	if (!official) {
		look_for = 8;
	}
#else
	/*
	** We allow the users to choose from all available start positions, even on official maps. ST - 1/15/2020 9:19AM
	*/
	int look_for = Session.Players.Count();
#endif
		
	for (int waycount = 0; waycount < 26; waycount++) {
//	for (int waycount = 0; waycount < max(4, Session.Players.Count()+Session.Options.AIPlayers); waycount++) {
		if (Scen.Waypoint[waycount] != -1) {
			waypts[num_waypts] = Scen.Waypoint[waycount];
			taken[num_waypts] = false;
			num_waypts++;

			if (num_waypts >= MAX_STORED_WAYPOINTS)
			{
				break;
			}
		}
	}

	/*
	**	If there are insufficient waypoints to account for all players, then randomly assign
	**	starting points until there is enough.
	*/
	int deficiency = look_for - num_waypts;
//	int deficiency = (Session.Players.Count() + Session.Options.AIPlayers) - num_waypts;
	if (deficiency > 0) {
		for (int index = 0; index < deficiency; index++) {
			CELL trycell = XY_Cell(Map.MapCellX + Random_Pick(0, Map.MapCellWidth-1), Map.MapCellY + Random_Pick(0, Map.MapCellHeight-1));

			trycell = Map.Nearby_Location(trycell, SPEED_TRACK);
			waypts[num_waypts] = trycell;
			taken[num_waypts] = false;
			num_waypts++;
		}
	}

	/*
	**	Loop through all houses.  Computer-controlled houses, with Session.Options.Bases
	**	ON, are treated as though bases are OFF (since we have no base-building
	**	AI logic.)
	*/
	int numtaken = 0;
	for (HousesType house = HOUSE_MULTI1; house < (HOUSE_MULTI1 + Session.MaxPlayers); house++) {

		/*
		**	Get a pointer to this house; if there is none, go to the next house
		*/
		HouseClass * hptr = HouseClass::As_Pointer(house);
		if (hptr == NULL) {
			continue;
		}

		/*
		**	Pick the starting location for this house. The first house just picks
		**	one of the valid locations at random. The other houses pick the furthest
		**	wapoint from the existing houses.
		*/
		if (!UseGlyphXStartLocations) {
		
			if (numtaken == 0) {
				int pick = Random_Pick(0, num_waypts-1);
				centroid = waypts[pick];
				taken[pick] = true;
				numtaken++;
			} else {

				/*
				**	Set all waypoints to have a score of zero in preparation for giving
				**	a distance score to all waypoints.
				*/
				int score[26];
				memset(score, '\0', sizeof(score));

				/*
				**	Scan through all waypoints and give a score as a value of the sum
				**	of the distances from this waypoint to all taken waypoints.
				*/
				for (int index = 0; index < num_waypts; index++) {

					/*
					**	If this waypoint has not already been taken, then accumulate the
					**	sum of the distance between this waypoint and all other taken
					**	waypoints.
					*/
					if (!taken[index]) {
						for (int trypoint = 0; trypoint < num_waypts; trypoint++) {

							if (taken[trypoint]) {
								score[index] += Distance(Cell_Coord(waypts[index]), Cell_Coord(waypts[trypoint]));
							}
						}
					}
				}

				/*
				**	Now find the waypoint with the largest score. This waypoint is the one
				**	that is furthest from all other taken waypoints.
				*/
				int best = 0;
				int bestvalue = 0;
				for (int searchindex = 0; searchindex < num_waypts; searchindex++) {
					if (score[searchindex] > bestvalue || bestvalue == 0) {
						bestvalue = score[searchindex];
						best = searchindex;
					}
				}

				/*
				**	Assign this best position to the house.
				*/
				centroid = waypts[best];
				taken[best] = true;
				numtaken++;
			}
		} else {
			
			/*
			** New code that respects the start locations passed in from GlyphX.
			**
			** ST - 1/8/2020 3:39PM
			*/
			centroid = waypts[hptr->StartLocationOverride];
		}

		/*
		**	Assign the center of this house to the waypoint location.
		*/
		hptr->Center = Cell_Coord(centroid);

		/*
		**	If Bases are ON, human & computer houses are treated differently
		*/
		if (Session.Options.Bases) {

			/*
			**	- For a human-controlled house:
			**	  - Set 'scaleval' to 1
			**	  - Create an MCV
			**	  - Attach a flag to it for capture-the-flag mode
			*/
			scaleval = 1;
			Reserve_Unit();
			obj = new UnitClass (UNIT_MCV, house);
			if (!obj->Unlimbo(Cell_Coord(centroid), DIR_N)) {
				if (!Scan_Place_Object(obj, centroid)) {
					delete obj;
					obj = NULL;
				}
			}
			if (obj != NULL) {
				hptr->FlagHome = 0;
				hptr->FlagLocation = 0;
				if (Special.IsCaptureTheFlag) {
					hptr->Flag_Attach((UnitClass *)obj, true);
				}
			}
		} else {

			/*
			**	If bases are OFF, set 'scaleval' to 1 & create a Mobile HQ for
			**	capture-the-flag mode.
			*/
			scaleval = 1;
#ifdef TOFIX
			if (Special.IsCaptureTheFlag) {
				obj = new UnitClass (UNIT_TRUCK, house);
				obj->Unlimbo(Cell_Coord(centroid), DIR_N);
				hptr->FlagHome = 0;					// turn house's flag off
				hptr->FlagLocation = 0;
			}
#endif
		}

		/*
		**	Create units for this house
		*/
		for (i = 0; i < u_limit; i++) {

			/*
			**	Find the center point for this category.
			*/
			centerpt = Clip_Scatter(centroid, 4);

			/*
			**	Place objects; loop through all unit in this category
			*/
			for (j = 0; j < num_units[i] * scaleval; j++) {

				/*
				**	Create an Ally unit
				*/
				if (hptr->ActLike != HOUSE_USSR && hptr->ActLike != HOUSE_UKRAINE) {
					for (k = 0; k < 2; k++) if(utable[i].AllyType[k] != UNIT_NONE) {
						Reserve_Unit();
						obj = new UnitClass (utable[i].AllyType[k], house);
						if (!Scan_Place_Object(obj, centerpt)) {
							delete obj;
						} else {
							if (!hptr->IsHuman) {
								obj->Set_Mission(MISSION_GUARD_AREA);
							} else {
								obj->Set_Mission(MISSION_GUARD);
							}
						}
					}
				} else {

					/*
					**	Create a Soviet unit
					*/
					for (k = 0; k < 2; k++) if(utable[i].SovietType[k] != UNIT_NONE) {
						Reserve_Unit();
						obj = new UnitClass (utable[i].SovietType[k], house);
						if (!Scan_Place_Object(obj, centerpt)) {
							delete obj;
						} else {
							if (!hptr->IsHuman) {
								obj->Set_Mission(MISSION_GUARD_AREA);
							} else {
								obj->Set_Mission(MISSION_GUARD);
							}
						}
					}
				}
			}
		}

		/*
		**	Create infantry
		*/
		for (i = 0; i < i_limit; i++) {
			/*
			**	Find the center point for this category.
			*/
			centerpt = Clip_Scatter(centroid, 4);

			/*
			**	Place objects; loop through all unit in this category
			*/
			for (j = 0; j < num_infantry[i] * scaleval; j++) {

				/*
				**	Create Ally infantry (Note: Unlimbo calls Enter_Idle_Mode(), which
				**	assigns the infantry to HUNT; we must use Set_Mission() to override
				**	this state.)
				*/
				if (hptr->ActLike != HOUSE_USSR && hptr->ActLike != HOUSE_UKRAINE) {
					for (k = 0; k < itable[i].AllyCount; k++) {
						Reserve_Infantry();
						obj = new InfantryClass (itable[i].AllyType, house);
						if (!Scan_Place_Object(obj, centerpt)) {
							delete obj;
						} else {
							if (!hptr->IsHuman) {
								obj->Set_Mission(MISSION_GUARD_AREA);
							} else {
								obj->Set_Mission(MISSION_GUARD);
							}
						}
					}
				} else {

					/*
					**	Create Soviet infantry
					*/
					for (k = 0; k < itable[i].SovietCount; k++) {
						Reserve_Infantry();
						obj = new InfantryClass (itable[i].SovietType, house);
						if (!Scan_Place_Object(obj, centerpt)) {
							delete obj;
						} else {
							if (!hptr->IsHuman) {
								obj->Set_Mission(MISSION_GUARD_AREA);
							} else {
								obj->Set_Mission(MISSION_GUARD);
							}
						}
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * Scan_Place_Object -- places an object >near< the given cell                                 *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      obj      ptr to object to Unlimbo                                                      *
 *      cell      center of search area                                                        *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      true = object was placed; false = it wasn't                                            *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/09/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
int Scan_Place_Object(ObjectClass * obj, CELL cell)
{
	int dist;				// for object placement
	FacingType rot;		// for object placement
	FacingType fcounter;	// for object placement
	int tryval;
	CELL newcell;
	TechnoClass * techno;
	int skipit;

	/*
	**	First try to unlimbo the object in the given cell.
	*/
	if (Map.In_Radar(cell)) {
		techno = Map[cell].Cell_Techno();
		if (!techno || (techno->What_Am_I()==RTTI_INFANTRY &&
			obj->What_Am_I()==RTTI_INFANTRY)) {
			if (obj->Unlimbo(Cell_Coord(cell), DIR_N)) {
				return(true);
			}
		}
	}

	/*
	**	Loop through distances from the given center cell; skip the center cell.
	**	For each distance, try placing the object along each rotational direction;
	**	if none are available, try each direction with a random scatter value.
	**	If that fails, go to the next distance.
	**	This ensures that the closest coordinates are filled first.
	*/
	for (dist = 1; dist < 32; dist++) {

		/*
		**	Pick a random starting direction
		*/
		rot = Random_Pick(FACING_N, FACING_NW);

		/*
		**	Try all directions twice
		*/
		for (tryval = 0 ; tryval < 2; tryval++) {

			/*
			**	Loop through all directions, at this distance.
			*/
			for (fcounter = FACING_N; fcounter <= FACING_NW; fcounter++) {

				skipit = false;

				/*
				**	Pick a coordinate along this directional axis
				*/
				newcell = Clip_Move(cell, rot, dist);

				/*
				**	If this is our second try at this distance, add a random scatter
				**	to the desired cell, so our units aren't all aligned along spokes.
				*/
				if (tryval > 0) {
					newcell = Clip_Scatter (newcell, 1);
				}

				/*
				**	If, by randomly scattering, we've chosen the exact center, skip
				**	it & try another direction.
				*/
				if (newcell==cell) {
					skipit = true;
				}

				if (!skipit) {
					/*
					**	Only attempt to Unlimbo the object if:
					**	- there is no techno in the cell
					**	- the techno in the cell & the object are both infantry
					*/
					techno = Map[newcell].Cell_Techno();
					if (!techno || (techno->What_Am_I()==RTTI_INFANTRY &&
						obj->What_Am_I()==RTTI_INFANTRY)) {
						if (obj->Unlimbo(Cell_Coord(newcell), DIR_N)) {
							return(true);
						}
					}
				}

				rot++;
				if (rot > FACING_NW) {
					rot = FACING_N;
				}
			}
		}
	}

	return(false);
}


/***********************************************************************************************
 * Clip_Scatter -- randomly scatters from given cell; won't fall off map                       *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      cell      cell to scatter from                                                         *
 *      maxdist   max distance to scatter                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      new cell number                                                                        *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static CELL Clip_Scatter(CELL cell, int maxdist)
{
	int x,y;
	int xdist;
	int ydist;
	int xmin,xmax;
	int ymin,ymax;

	/*
	**	Get X & Y coords of given starting cell
	*/
	x = Cell_X(cell);
	y = Cell_Y(cell);

	/*
	**	Compute our x & y limits
	*/
	xmin = Map.MapCellX;
	xmax = xmin + Map.MapCellWidth - 1;
	ymin = Map.MapCellY;
	ymax = ymin + Map.MapCellHeight - 1;

	/*
	**	Adjust the x-coordinate
	*/
	xdist = Random_Pick(0, maxdist);
	if (Percent_Chance(50)) {
		x += xdist;
		if (x > xmax) {
			x = xmax;
		}
	} else {
		x -= xdist;
		if (x < xmin) {
			x = xmin;
		}
	}

	/*
	**	Adjust the y-coordinate
	*/
	ydist = Random_Pick(0, maxdist);
	if (Percent_Chance(50)) {
		y += ydist;
		if (y > ymax) {
			y = ymax;
		}
	} else {
		y -= ydist;
		if (y < ymin) {
			y = ymin;
		}
	}

	return (XY_Cell(x, y));
}


/***********************************************************************************************
 * Clip_Move -- moves in given direction from given cell; clips to map                         *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      cell      cell to start from                                                           *
 *      facing   direction to move                                                             *
 *      dist      distance to move                                                             *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      new cell number                                                                        *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static CELL Clip_Move(CELL cell, FacingType facing, int dist)
{
	int x,y;
	int xmin,xmax;
	int ymin,ymax;

	/*
	**	Get X & Y coords of given starting cell
	*/
	x = Cell_X(cell);
	y = Cell_Y(cell);

	/*
	**	Compute our x & y limits
	*/
	xmin = Map.MapCellX;
	xmax = xmin + Map.MapCellWidth - 1;
	ymin = Map.MapCellY;
	ymax = ymin + Map.MapCellHeight - 1;

	/*
	**	Adjust the x-coordinate
	*/
	switch (facing) {
		case FACING_N:
			y -= dist;
			break;

		case FACING_NE:
			x += dist;
			y -= dist;
			break;

		case FACING_E:
			x += dist;
			break;

		case FACING_SE:
			x += dist;
			y += dist;
			break;

		case FACING_S:
			y += dist;
			break;

		case FACING_SW:
			x -= dist;
			y += dist;
			break;

		case FACING_W:
			x -= dist;
			break;

		case FACING_NW:
			x -= dist;
			y -= dist;
			break;
	}

	/*
	**	Clip to the map
	*/
	if (x > xmax)
		x = xmax;
	if (x < xmin)
		x = xmin;

	if (y > ymax)
		y = ymax;
	if (y < ymin)
		y = ymin;

	return (XY_Cell(x, y));
}


void Disect_Scenario_Name(char const * name, int & scenario, ScenarioPlayerType & player, ScenarioDirType & dir, ScenarioVarType & var)
{
	if (name == NULL) return;

	/*
	**	Fetch the scenario number.
	*/
	char buf[3];
	memcpy(buf, &name[3], 2);
	buf[2] = '\0';
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	char first = buf[0];
	char second = buf[1];
	if (first <= '9' && second <= '9') {
		scenario = atoi(buf);
	} else {
		if (first <= '9') {
			first -= '0';
		} else {
			if (first >= 'a' && first <= 'z') {
				first -= 'a';
			} else {
				first -= 'A';
			}
		}
		if (second <= '9') {
			second -= '0';
		} else {
			if (second >= 'a' && second <= 'z') {
				second = (second - 'a') + 10;
			} else {
				second = (second - 'A') + 10;
			}
		}
		scenario = (36 * first) + second;
	}
#else
	scenario = atoi(buf);
#endif

	/*
	**	Fetch the scenario player (side).
	*/
	player = SCEN_PLAYER_GREECE;
	if (name[2] == HouseTypeClass::As_Reference(HOUSE_SPAIN).Prefix) {
		player = SCEN_PLAYER_SPAIN;
	}
	if (name[2] == HouseTypeClass::As_Reference(HOUSE_GREECE).Prefix) {
		player = SCEN_PLAYER_GREECE;
	}
	if (name[2] == HouseTypeClass::As_Reference(HOUSE_USSR).Prefix) {
		player = SCEN_PLAYER_USSR;
	}

	/*
	**	Fetch the direction.
	*/
	dir = SCEN_DIR_EAST;
	if (name[5] == 'E') {
		dir = SCEN_DIR_EAST;
	} else {
		dir = SCEN_DIR_WEST;
	}

	/*
	**	Fetch the variation.
	*/
	var = SCEN_VAR_A;
	var = ScenarioVarType((name[6] - 'A') + SCEN_VAR_A);
}