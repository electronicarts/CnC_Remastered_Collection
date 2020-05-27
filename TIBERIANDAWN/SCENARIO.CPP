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

/* $Header:   F:\projects\c&c\vcs\code\scenario.cpv   2.17   16 Oct 1995 16:52:08   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : August 24, 1995 [JLB]                                        *
 *                                                                                             *
 * This module handles the scenario reading and writing. Scenario related                      *
 * code that is executed between scenario play can also be here.                               *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Clear_Scenario -- Clears all data in preparation for scenario load.                       *
 *   Do_Lose -- Display losing comments.                                                       *
 *   Do_Restart -- Handle the restart mission process.                                         *
 *   Do_Win -- Display winning congratulations.                                                *
 *   Fill_In_Data -- Recreate all data that is not loaded with scenario.                       *
 *   Read_Scenario -- Reads a scenario from disk.                                              *
 *   Restate_Mission -- Handles restating the mission objective.                               *
 *   Start_Scenario -- Starts the scenario.                                                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
extern int PreserveVQAScreen;


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
bool Start_Scenario(char *root, bool briefing)
{

	if (!Read_Scenario(root)) {
		CCDebugString ("C&C95 - Failed to read scenario.\n");
		return(false);
	}
	CCDebugString ("C&C95 - Scenario read OK.\n");

#ifdef DEMO

	if (briefing) {
		Play_Movie(BriefMovie);
		Play_Movie(ActionMovie, TransitTheme);
	}
	Theme.Queue_Song(THEME_AOI);

#else

	/*
	** Install some hacks around the movie playing to account for the choose-
	** sides introduction.  We don't want an intro movie on scenario 1, and
	** we don't want a briefing movie on GDI scenario 1.
	*/
	if (Scenario < 20 && (!Special.IsJurassic || !AreThingiesEnabled)) {
		if (Scenario != 1 || Whom == HOUSE_GOOD) {
			Play_Movie(IntroMovie);
		}

		if (briefing) {
			PreserveVQAScreen = (Scenario == 1);
			Play_Movie(BriefMovie);
		}
		Play_Movie(ActionMovie, TransitTheme);
		if (TransitTheme == THEME_NONE) {
			Theme.Queue_Song(THEME_AOI);
		}
	} else {
		Play_Movie(BriefMovie);
		Play_Movie(ActionMovie, TransitTheme);

#ifdef NEWMENU

		char buffer[25];
		sprintf(buffer, "%s.VQA", BriefMovie);
		CCFileClass file(buffer);

		if (GameToPlay == GAME_NORMAL && !file.Is_Available()) {
			VisiblePage.Clear();
			Set_Palette(GamePalette);
//			Show_Mouse();
			/*
			** Show the mission briefing. Pretend we are inside the main loop so the palette
			** will be correct on the textured buttons.
			*/
			bool oldinmain = InMainLoop;
			InMainLoop = true;
			
			// TO_FIX - Covert ops missions want to pop up a dialog box. ST - 9/6/2019 1:48PM
			//Restate_Mission(ScenarioName, TXT_OK, TXT_NONE);
			
			
			InMainLoop = oldinmain;
//			Hide_Mouse();
			if (TransitTheme == THEME_NONE) {
				Theme.Queue_Song(THEME_AOI);
			}
		}

#endif
	}
#endif

	/*
	** Hack for laser-firing Orcas in the PATSUX secret mission
	*/
	if (GameToPlay == GAME_NORMAL && Scenario == 72) {
		((AircraftTypeClass&)AircraftTypeClass::As_Reference(AIRCRAFT_ORCA)).Primary = WEAPON_OBELISK_LASER;
	}
	else {
		((AircraftTypeClass&)AircraftTypeClass::As_Reference(AIRCRAFT_ORCA)).Primary = WEAPON_DRAGON;
	}

	/*
	** Set the options values, since the palette has been initialized by Read_Scenario
	*/
	CCDebugString ("C&C95 - About to call Options.Set.\n");
	Options.Set();
	CCDebugString ("C&C95 - About to return from Start_Scenario.\n");
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
	if (GameToPlay == GAME_NORMAL) {
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
bool Read_Scenario(char *root)
{
	CCDebugString ("C&C95 - In Read_Scenario.\n");
	Clear_Scenario();
	ScenarioInit++;
	if (Read_Scenario_Ini(root)) {

		Fill_In_Data();

		Map.Set_View_Dimensions(0, Map.Get_Tab_Height(), Map.MapCellWidth, Map.MapCellHeight);


		/*
		**	SPECIAL CASE:
		**	Clear out the tutor flags for scenarios one and two. This is designed
		**	so that tutorial message will reappear in scenario two.
		*/
		if (Scenario < 5) {
			TutorFlags[0] = 0L;
			TutorFlags[1] = 0L;
		}

	} else {

#if (1)
		char message[200];
		if (root) {
			sprintf(message, "Failed to load scenario %s", root);
			GlyphX_Debug_Print(message);
		} else {
			GlyphX_Debug_Print("Failed to load scenario");
		}
#else		  
		Fade_Palette_To(GamePalette, FADE_PALETTE_FAST, Call_Back);
		Show_Mouse();
		CCMessageBox().Process(TXT_UNABLE_READ_SCENARIO);
		Hide_Mouse();
#endif
		return(false);
	}
	ScenarioInit--;
	CCDebugString ("C&C95 - Leaving Read_Scenario.\n");
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

	for (int index = 0; index < Buildings.Count(); index++) {
		Buildings.Ptr(index)->Update_Buildables();
	}

	Map.Flag_To_Redraw(true);

	/*
	**	Bring up the score display on the radar map when starting a multiplayer
	**	game.
	*/
	if (GameToPlay != GAME_NORMAL) {
		Map.Player_Names(1);
	}

	ScenarioInit--;
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
	EndCountDown = TICKS_PER_SECOND * 30;
	CrateCount = 0;
	CrateTimer = 0;
	CrateMaker = false;

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

	FactoryClass::Init();

	Base.Init();

	CurrentObject.Clear_All();

	Invalidate_Cached_Icons();
}


/***********************************************************************************************
 * Do_Win -- Display winning congratulations.                                                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1992 JLB : Created.                                                                 *
 *   01/01/1995 JLB : Carries money forward into next scenario.                                *
 *=============================================================================================*/
void Do_Win(void)
{
	Map.Set_Default_Mouse(MOUSE_NORMAL);
	Hide_Mouse();

	/*
	** If this is a multiplayer game, clear the game's name so we won't respond
	** to game queries any more (in Call_Back)
	*/
	if (GameToPlay != GAME_NORMAL) {
		MPlayerGameName[0] = 0;
	}

	/*
	**	Determine a cosmetic center point for the text.
	*/
	int x = Map.TacPixelX + (Lepton_To_Pixel(Map.TacLeptonWidth)/2);
	int y = Map.TacPixelY + (Lepton_To_Pixel(Map.TacLeptonHeight)/2) -32;

	/*
	**	Announce win to player.
	*/
	Set_Logic_Page(SeenBuff);
#if !(GERMAN | FRENCH)
	Fancy_Text_Print(TXT_MISSION, x, y, WHITE, TBLACK, TPF_CENTER|TPF_VCR);
#endif
	Fancy_Text_Print(TXT_SCENARIO_WON, x, y+30, WHITE, TBLACK, TPF_CENTER|TPF_VCR);
	CountDownTimer.Set(TIMER_SECOND * 3);
	Stop_Speaking();
	Speak(VOX_ACCOMPLISHED);
	while (CountDownTimer.Time() || Is_Speaking()) {
		Call_Back();
	}

	/*
	** Stop here if this is a multiplayer game.
	*/
	if (GameToPlay != GAME_NORMAL) {
		if (!PlaybackGame) {
			MPlayerGamesPlayed++;
			Multi_Score_Presentation();
			MPlayerCurGame++;
			if (MPlayerCurGame >= MAX_MULTI_GAMES) {
				MPlayerCurGame = MAX_MULTI_GAMES - 1;
			}
		}
		GameActive = 0;
		Show_Mouse();
		return;
	}

	/*
	**	Play the winning movie and then start the next scenario.
	*/
	if (RequiredCD != -2) {
		if (Scenario >= 20 && Scenario <60 && GameToPlay == GAME_NORMAL) {
			RequiredCD = 2;
		} else {
			if (Scenario >=60){
				RequiredCD = -1;
			}else{
				if (PlayerPtr->Class->House == HOUSE_GOOD) {
					RequiredCD = 0;
				} else {
					RequiredCD = 1;
				}
			}
		}
	}

#ifndef DEMO
	Play_Movie(WinMovie);
#endif

	Keyboard::Clear();

	/*
	**	Do the ending screens only if not playing back a recorded game.
	*/
	if (!PlaybackGame) {

#ifdef DEMO

		switch (Scenario) {
			case 1:
				Score.Presentation();
				Scenario = 10;
				break;

			case 10:
				Score.Presentation();
				Scenario = 6;
				break;

			default:
				Score.Presentation();
				GDI_Ending();
				GameActive = false;
				Show_Mouse();
				return;
//				Prog_End();
//				exit(0);
//				break;
		}

#else

#ifdef NEWMENU
		if (Scenario >= 20) {
			Keyboard::Clear();
			Score.Presentation();
			GameActive = false;
			Show_Mouse();
			return;
		}
#endif

		if (PlayerPtr->Class->House == HOUSE_BAD && Scenario == 13) {
			Nod_Ending();
			//Prog_End();
			//exit(0);
			SeenBuff.Clear();
			Show_Mouse();
			GameActive = false;
			return;
		}
		if (PlayerPtr->Class->House == HOUSE_GOOD && Scenario == 15) {
			GDI_Ending();
			//Prog_End();
			//exit(0);
			SeenBuff.Clear();
			Show_Mouse();
			GameActive = false;
			return;
		}

		if ( (Special.IsJurassic && AreThingiesEnabled) && Scenario == 5) {
			Prog_End("Do_Win - Last Jurassic mission complete");
			if (!RunningAsDLL) {
				exit(0);
			}
			return;
		}

		if (!Special.IsJurassic || !AreThingiesEnabled) {
			Keyboard::Clear();
			InterpolationPaletteChanged = TRUE;
			InterpolationPalette = Palette;
			Score.Presentation();

			/*
			**	Skip scenario #7 if the airfield was blown up.
			*/
			if (Scenario == 6 && PlayerPtr->Class->House == HOUSE_GOOD && SabotagedType == STRUCT_AIRSTRIP) {
				Scenario++;
			}

			Map_Selection();
		}
		Scenario++;
#endif
		Keyboard::Clear();
	}

	CarryOverMoney = PlayerPtr->Credits;

	int pieces = PlayerPtr->NukePieces;

	/*
	** Generate a new scenario filename
	*/
	Set_Scenario_Name(ScenarioName, Scenario, ScenPlayer, ScenDir, ScenVar);
	Start_Scenario(ScenarioName);

	PlayerPtr->NukePieces = pieces;

	/*
	**	Destroy the building that was sabotaged in the previous scenario. This only
	**	applies to GDI mission #7.
	*/
	int index;
	if (SabotagedType != STRUCT_NONE && Scenario == 7 && PlayerPtr->Class->House == HOUSE_GOOD) {
		for (index = 0; index < Buildings.Count(); index++) {
			BuildingClass * building = Buildings.Ptr(index);

			if (building && !building->IsInLimbo && building->House != PlayerPtr && building->Class->Type == SabotagedType) {
				building->Limbo();
				delete building;
				break;
			}
		}

		/*
		**	Remove the building from the prebuild list.
		*/
		for (index = 0; index < Base.Nodes.Count(); index++) {
			BaseNodeClass * node = Base.Get_Node(index);

			if (node && node->Type == SabotagedType) {
				Base.Nodes.Delete(index);
				break;
			}
		}
	}
	SabotagedType = STRUCT_NONE;

	Map.Render();
	Fade_Palette_To(GamePalette, FADE_PALETTE_FAST, Call_Back);
	Show_Mouse();
}


/***********************************************************************************************
 * Do_Lose -- Display losing comments.                                                         *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
void Do_Lose(void)
{
	Map.Set_Default_Mouse(MOUSE_NORMAL);
	Hide_Mouse();

	/*
	** If this is a multiplayer game, clear the game's name so we won't respond
	** to game queries any more (in Call_Back)
	*/
	if (GameToPlay != GAME_NORMAL) {
		MPlayerGameName[0] = 0;
	}

	/*
	**	Determine a cosmetic center point for the text.
	*/
	int x = Map.TacPixelX + (Lepton_To_Pixel(Map.TacLeptonWidth)/2);
	int y = Map.TacPixelY + (Lepton_To_Pixel(Map.TacLeptonHeight)/2) -32;

	/*
	**	Announce win to player.
	*/
	Set_Logic_Page(SeenBuff);
	Fancy_Text_Print(TXT_MISSION, x, y, WHITE, TBLACK, TPF_CENTER|TPF_VCR);
	Fancy_Text_Print(TXT_SCENARIO_LOST, x, y+30, WHITE, TBLACK, TPF_CENTER|TPF_VCR);
	CountDownTimer.Set(TIMER_SECOND * 3);
	Stop_Speaking();
	Speak(VOX_FAIL);
	while (CountDownTimer.Time() || Is_Speaking()) {
		Call_Back();
	}

#ifdef OBSOLETE
	if (Debug_Play_Map) {
		Go_Editor(true);
		Show_Mouse();
		return;
	}
#endif

	/*
	** Stop here if this is a multiplayer game.
	*/
	if (GameToPlay != GAME_NORMAL) {
		if (!PlaybackGame) {
			MPlayerGamesPlayed++;
			Multi_Score_Presentation();
			MPlayerCurGame++;
			if (MPlayerCurGame >= MAX_MULTI_GAMES) {
				MPlayerCurGame = MAX_MULTI_GAMES - 1;
			}
		}
		GameActive = 0;
		Show_Mouse();
		return;
	}

	Play_Movie(LoseMovie);

	/*
	** Start same scenario again
	*/
	Set_Palette(GamePalette);
	Show_Mouse();
	if (!PlaybackGame && !CCMessageBox().Process(TXT_TO_REPLAY, TXT_YES, TXT_NO)) {
		Hide_Mouse();
		Keyboard::Clear();
		Start_Scenario(ScenarioName, false);
		Map.Render();
	} else {
		Hide_Mouse();
		GameActive = 0;
	}

	Fade_Palette_To(GamePalette, FADE_PALETTE_FAST, Call_Back);
	Show_Mouse();
}


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
	bool hidden = Get_Mouse_State();

	if (hidden) Show_Mouse();
	CCMessageBox().Process(TXT_RESTARTING, TXT_NONE);
	Map.Set_Default_Mouse(MOUSE_NORMAL);
	Keyboard::Clear();
	Start_Scenario(ScenarioName, false);
	if (hidden) Hide_Mouse();
	Keyboard::Clear();
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
#ifdef JAPANESE
		char fname[14];
		strcpy(fname, name);
		strcat(fname,".CPS");

		if(CCFileClass(fname).Is_Available()) {
			CCMessageBox box(TXT_NONE, true);
			return(box.Process(fname, button1, button2));
		}
#else
		/*
		**	Make sure that if there is no briefing movie, that the briefing text is
		**	the only option available.
		*/
		bool brief = true;
#ifdef NEWMENU
		char buffer[25];
		char buffer1[25];
		sprintf(buffer, "%s.VQA", BriefMovie);
		sprintf(buffer1, "%s.VQA", ActionMovie);
		CCFileClass file1(buffer);
		CCFileClass file2(buffer1);
		if (!file1.Is_Available() && !file2.Is_Available()) {
			button1 = TXT_OK;
			button2 = TXT_NONE;
			brief = false;
		}
#endif

		/*
		**	If mission object text was found, then display it.
		*/
		if (strlen(BriefingText)) {
			static char _buff[512];

			strcpy(_buff, BriefingText);
//			strcpy(_ShapeBuffer, BriefingText);

			bool hidden = Get_Mouse_State();
			if (hidden) Show_Mouse();

			if (CCMessageBox(TXT_OBJECTIVE).Process(_buff, button1, button2)) {
				if (hidden) Hide_Mouse();
				return(true);
			}
			if (hidden) Hide_Mouse();
			if (!brief) return(true);
			return(false);
		}
#endif
	}
	return(false);
}