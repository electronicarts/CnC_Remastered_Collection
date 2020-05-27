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

/* $Header:   F:\projects\c&c\vcs\code\ini.cpv   2.18   16 Oct 1995 16:48:50   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : INI.CPP                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : July 30, 1995 [BRR]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Assign_Houses -- Assigns multiplayer houses to various players                            *
 *   Clear_Flag_Spots -- Clears flag overlays off the map                                      *
 *   Clip_Move -- moves in given direction from given cell; clips to map                       *
 *   Clip_Scatter -- randomly scatters from given cell; won't fall off map                     *
 *   Create_Units -- Creates infantry & units, for non-base multiplayer                        *
 *   Furthest_Cell -- Finds cell furthest from a group of cells                                *
 *   Place_Flags -- Places flags for multiplayer games                                         *
 *   Read_Scenario_Ini -- Read specified scenario INI file.                                    *
 *   Remove_AI_Players -- Removes the computer AI houses & their units                         *
 *   Scan_Place_Object -- places an object >near< the given cell                               *
 *   Set_Scenario_Name -- Creates the INI scenario name string.                                *
 *   Sort_Cells -- sorts an array of cells by distance                                         *
 *   Write_Scenario_Ini -- Write the scenario INI file.                                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

/************************************* Prototypes *********************************************/
static void Assign_Houses(void);
static void Remove_AI_Players(void);
static void Create_Units(void);
static void Sort_Cells(CELL *cells, int numcells, CELL *outcells);
static int Furthest_Cell(CELL *cells, int numcells, CELL *tcells, int numtcells);
static CELL Clip_Scatter(CELL cell, int maxdist);
static CELL Clip_Move(CELL cell, FacingType facing, int dist);


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
void Set_Scenario_Name(char *buf, int scenario, ScenarioPlayerType player, ScenarioDirType dir, ScenarioVarType var)
{
	char c_player;			// character representing player type
	char c_dir;				// character representing direction type
	char c_var;				// character representing variation type
	ScenarioVarType i;
	char fname[_MAX_FNAME+_MAX_EXT];

	/*
	** Set the player-type value.
	*/
	switch (player) {
		case SCEN_PLAYER_GDI:
			c_player = HouseTypeClass::As_Reference(HOUSE_GOOD).Prefix;
//			c_player = 'G';
			break;

		case SCEN_PLAYER_NOD:
			c_player = HouseTypeClass::As_Reference(HOUSE_BAD).Prefix;
//			c_player = 'B';
			break;

		case SCEN_PLAYER_JP:
			c_player = HouseTypeClass::As_Reference(HOUSE_JP).Prefix;
//			c_player = 'J';
			break;

		/*
		**	Multi player scenario.
		*/
		default:
			c_player = HouseTypeClass::As_Reference(HOUSE_MULTI1).Prefix;
//			c_player = 'M';
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
			c_dir = (Random_Pick(0, 1) == 0) ? 'W' : 'E';
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
	sprintf(buf, "SC%c%02d%c%c", c_player, scenario, c_dir, c_var);
}


extern void GlyphX_Assign_Houses(void);	//ST - 6/25/2019 11:08AM


/***********************************************************************************************
 * Read_Scenario_Ini -- Read specified scenario INI file.                                      *
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
 *   10/07/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Read_Scenario_Ini(char *root, bool fresh)
{
	char *buffer;				// Scenario.ini staging buffer pointer.
	char fname[_MAX_FNAME+_MAX_EXT];			// full INI filename
	char buf[128];				// Working string staging buffer.
#ifndef USE_RA_AI
	int rndmax;
	int rndmin;
#endif //USE_RA_AI
	int len;
	unsigned char val;

	ScenarioInit++;

	/*
	**	Fetch working pointer to the INI staging buffer. Make sure that the buffer
	**	is cleared out before proceeding.  (Don't use the HidPage for this, since
	**	the HidPage may be needed for various uncompressions during the INI
	**	parsing.)
	*/
	buffer = (char *)_ShapeBuffer;
	memset(buffer, '\0', _ShapeBufferSize);

	if (fresh) {
		Clear_Scenario();
	}

	/*
	** If we are not dealing with scenario 1, or a multi player scenario
	** then make sure the correct disk is in the drive.
	*/
	if (RequiredCD != -2) {
		if (Scenario >= 20 && Scenario <60 && GameToPlay == GAME_NORMAL) {
			RequiredCD = 2;
		} else {
			if (Scenario != 1) {
				if (Scenario >=60){
					RequiredCD = -1;
				}else{
					switch (ScenPlayer) {
						case SCEN_PLAYER_GDI:
							RequiredCD = 0;
							break;
						case SCEN_PLAYER_NOD:
							RequiredCD = 1;
							break;
						default:
							RequiredCD = -1;
							break;
					}
				}
			} else {
				RequiredCD = -1;
			}
		}
	}
	if (!Force_CD_Available(RequiredCD)) {
		Prog_End("Read_Scenario_Ini - CD not found", true);
		if (!RunningAsDLL) {
			exit(EXIT_FAILURE);
		}
	}

	/*
	**	Create scenario filename and read the file.
	*/

	sprintf(fname,"%s.INI",root);
	CCFileClass file(fname);
	if (!file.Is_Available()) {
		GlyphX_Debug_Print("Failed to find scenario file");
		GlyphX_Debug_Print(fname);
		return(false);
	} else {
		
		GlyphX_Debug_Print("Opened scenario file");
		GlyphX_Debug_Print(fname);
		
		file.Read(buffer, _ShapeBufferSize-1);
	}

	/*
	** Init the Scenario CRC value
	*/
	ScenarioCRC = 0;
	len = strlen(buffer);
	for (int i = 0; i < len; i++) {
		val = (unsigned char)buffer[i];
#ifndef DEMO
		Add_CRC(&ScenarioCRC, (unsigned long)val);
#endif
	}

	/*
	**	Fetch the appropriate movie names from the INI file.
	*/
	WWGetPrivateProfileString("Basic", "Intro", "x", IntroMovie, sizeof(IntroMovie), buffer);
	WWGetPrivateProfileString("Basic", "Brief", "x", BriefMovie, sizeof(BriefMovie), buffer);
	WWGetPrivateProfileString("Basic", "Win", "x", WinMovie, sizeof(WinMovie), buffer);
	WWGetPrivateProfileString("Basic", "Win2", "x", WinMovie2, sizeof(WinMovie2), buffer);
	WWGetPrivateProfileString("Basic", "Win3", "x", WinMovie3, sizeof(WinMovie3), buffer);
	WWGetPrivateProfileString("Basic", "Win4", "x", WinMovie4, sizeof(WinMovie4), buffer);
	WWGetPrivateProfileString("Basic", "Lose", "x", LoseMovie, sizeof(LoseMovie), buffer);
	WWGetPrivateProfileString("Basic", "Action", "x", ActionMovie, sizeof(ActionMovie), buffer);

	/*
	**	For single-player scenarios, 'BuildLevel' is the scenario number.
	**	This must be set before any buildings are created (if a factory is created,
	**	it needs to know the BuildLevel for the sidebar.)
	*/
	if (GameToPlay == GAME_NORMAL) {
#ifdef NEWMENU
		if (Scenario <= 15) {
			BuildLevel = Scenario;
		} else {
			BuildLevel = WWGetPrivateProfileInt("Basic", "BuildLevel", Scenario, buffer);
		}
#else
		BuildLevel = Scenario;
#endif
	}

	/*
	**	Jurassic scenarios are allowed to build the full multiplayer set
	**	of objects.
	*/
	if (Special.IsJurassic && AreThingiesEnabled) {
		BuildLevel = 98;
	}

	/*
	**	Fetch the transition theme for this scenario.
	*/
	TransitTheme = THEME_NONE;
	WWGetPrivateProfileString("Basic", "Theme", "No Theme", buf, sizeof(buf), buffer);
	TransitTheme = Theme.From_Name(buf);

	/*
	**	Read in the team-type data. The team types must be created before any
	**	triggers can be created.
	*/
	TeamTypeClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in the specific information for each of the house types.  This creates
	**	the houses of different types.
	*/
	HouseClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in the trigger data. The triggers must be created before any other
	**	objects can be initialized.
	*/
	TriggerClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in the map control values. This includes dimensions
	**	as well as theater information.
	*/
	Map.Read_INI(buffer);
	Call_Back();

	/*
	**	Assign PlayerPtr by reading the player's house from the INI;
	**	Must be done before any TechnoClass objects are created.
	*/
//	if (GameToPlay == GAME_NORMAL && (ScenPlayer == SCEN_PLAYER_GDI || ScenPlayer == SCEN_PLAYER_NOD)) {
	if (GameToPlay == GAME_NORMAL) {
		WWGetPrivateProfileString("Basic", "Player", "GoodGuy", buf, 127, buffer);
		CarryOverPercent = WWGetPrivateProfileInt("Basic", "CarryOverMoney", 100, buffer);
		CarryOverPercent = Cardinal_To_Fixed(100, CarryOverPercent);
		CarryOverCap = WWGetPrivateProfileInt("Basic", "CarryOverCap", -1, buffer);

		PlayerPtr = HouseClass::As_Pointer(HouseTypeClass::From_Name(buf));
		PlayerPtr->IsHuman = true;
		int carryover;
		if (CarryOverCap != -1) {
			carryover = MIN((int)Fixed_To_Cardinal(CarryOverMoney, CarryOverPercent), CarryOverCap);
		} else {
			carryover = Fixed_To_Cardinal(CarryOverMoney, CarryOverPercent);
		}
		PlayerPtr->Credits += carryover;
		PlayerPtr->InitialCredits += carryover;

		if (Special.IsJurassic) {
			PlayerPtr->ActLike = Whom;
		}

		if (Special.IsEasy) {
			PlayerPtr->Assign_Handicap(DIFF_EASY);
		} else if (Special.IsDifficult) {
			PlayerPtr->Assign_Handicap(DIFF_HARD);
		}
	} else {

#ifdef OBSOLETE
		if (GameToPlay==GAME_NORMAL && ScenPlayer==SCEN_PLAYER_JP) {
			PlayerPtr = HouseClass::As_Pointer(HOUSE_MULTI4);
			PlayerPtr->IsHuman = true;
			PlayerPtr->Credits += CarryOverMoney;
			PlayerPtr->InitialCredits += CarryOverMoney;
			PlayerPtr->ActLike = Whom;
		} else {
			Assign_Houses();
		}
#endif
		//Call new Assign_Houses function. ST - 6/25/2019 11:07AM
		//Assign_Houses();
		GlyphX_Assign_Houses();
	}

	/*
	**	Attempt to read the map's binary image file; if fails, read the
	**	template data from the INI, for backward compatibility
	*/
	if (fresh) {
		if (!Map.Read_Binary(root, &ScenarioCRC)) {
			TemplateClass::Read_INI(buffer);
		}
	}
	Call_Back();

	/*
	**	Read in and place the 3D terrain objects.
	*/
	TerrainClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in and place the units (all sides).
	*/
	UnitClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in and place the infantry units (all sides).
	*/
	InfantryClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in and place all the buildings on the map.
	*/
	BuildingClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in the AI's base information.
	*/
	Base.Read_INI(buffer);
	Call_Back();

	/*
	**	Read in any normal overlay objects.
	*/
	OverlayClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in any smudge overlays.
	*/
	SmudgeClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in any briefing text.
	*/
	char * stage = &BriefingText[0];
	*stage = '\0';
	int index = 1;

	/*
	**	Build the full text of the mission objective.
	*/
	for (;;) {
		char buff[16];

		sprintf(buff, "%d", index++);
		*stage = '\0';
		WWGetPrivateProfileString("Briefing", buff, "", stage, (sizeof(BriefingText)-strlen(BriefingText))-1, buffer);
		if (strlen(stage) == 0) break;
		strcat(stage, " ");
		stage += strlen(stage);
	}

	/*
	**	If the briefing text could not be found in the INI file, then search
	**	the mission.ini file.
	*/
	if (BriefingText[0] == '\0') {
		memset(_ShapeBuffer, '\0', _ShapeBufferSize);
		CCFileClass("MISSION.INI").Read(_ShapeBuffer, _ShapeBufferSize);

		char * buffer = (char *)Add_Long_To_Pointer(_ShapeBuffer, strlen(_ShapeBuffer));
		char * work = &BriefingText[0];
		int index = 1;

		/*
		**	Build the full text of the mission objective.
		*/
		for (;;) {
			char buff[16];

			sprintf(buff, "%d", index++);
			*work = '\0';
			WWGetPrivateProfileString(root, buff, "", work, (sizeof(BriefingText)-strlen(BriefingText))-1, _ShapeBuffer);
			if (strlen(work) == 0) break;
			strcat(work, " ");
			work += strlen(work);
		}
	}

	/*
	**	Perform a final overpass of the map. This handles smoothing of certain
	**	types of terrain (tiberium).
	*/
	Map.Overpass();
	Call_Back();

	/*
	**	Special cases:
	**		NOD7A cell 2795 - LAND_ROCK
	**		NOD09A - delete airstrike trigger when radar destroyed
	**		NOD10B cell 2015 - LAND_ROCK
	*/
	if (_stricmp(ScenarioName, "scb07ea") == 0) {
		Map[(CELL)2795].Override_Land_Type(LAND_ROCK);
	}
	if (_stricmp(ScenarioName, "scb09ea") == 0) {
		for (int index = 0; index < Buildings.Count(); ++index) {
			BuildingClass* building = Buildings.Ptr(index);
			if (building != NULL && building->Owner() == HOUSE_GOOD && *building == STRUCT_RADAR) {
				building->Trigger = TriggerClass::As_Pointer("dely");
				if (building->Trigger) {
					building->Trigger->AttachCount++;
				}
				break;
			}
		}
	}
	if (_stricmp(ScenarioName, "scb10eb") == 0) {
		Map[(CELL)2015].Override_Land_Type(LAND_ROCK);
	}

	/*
	**	Multi-player last-minute fixups:
	**	- If computer players are disabled, remove all computer-owned houses
	** - Otherwise, set MPlayerBlitz to 0 or 1, randomly
	**	- If bases are disabled, create the scenario dynamically
	**	- Remove any flag spot overlays lying around
	**	- If capture-the-flag is enabled, assign flags to cells.
	*/
	if (GameToPlay != GAME_NORMAL || ScenPlayer == SCEN_PLAYER_2PLAYER ||
		ScenPlayer == SCEN_PLAYER_MPLAYER) {

		/*
		**	If Ghosts are disabled and we're not editing, remove computer players
		**	(Must be done after all objects are read in from the INI)
		*/
		if (!MPlayerGhosts && !Debug_Map) {
			//Remove_AI_Players();		    // Done elsewhere now. ST - 6/25/2019 12:33PM
		} else {

			/*
			** If Ghosts are on, set up their houses for blitzing the humans
			*/
#ifndef USE_RA_AI
			MPlayerBlitz = IRandom (0,1);					// 1 = computer will blitz
			
			if (MPlayerBlitz) {
				if (MPlayerBases) {
					rndmax = 14000;
					rndmin = 10000;
				} else {
					rndmax = 8000;
					rndmin = 4000;
				}

				for (int i = 0; i < MPlayerMax; i++) {
					HousesType house = (HousesType)(i + (int)HOUSE_MULTI1);
					HouseClass *housep = HouseClass::As_Pointer (house);
					if (housep) {	//Added. ST - 6/25/2019 11:37AM
						housep->BlitzTime = IRandom (rndmin,rndmax);
					}
				}

			}
#else // USE_RA_AI
			MPlayerBlitz = 0;
#endif // USE_RA_AI
		}

		/*
		**	Units must be created for each house.  If bases are ON, this routine
		**	will create an MCV along with the units; otherwise, it will just create
		**	a whole bunch of units.  MPlayerUnitCount is the total # of units
		**	to create.
		*/
		if (!Debug_Map) {
			int save_init = ScenarioInit;			// turn ScenarioInit off
			ScenarioInit = 0;
			Create_Units();
			ScenarioInit = save_init;				// turn ScenarioInit back on
		}

		/*
		**	Place crates if MPlayerGoodies is on.
		*/
		if (MPlayerGoodies) {
			for (int index = 0; index < MPlayerCount; index++) {
			//for (int index = 0; index < 200; index++) {  // Lots of crates for test		
				Map.Place_Random_Crate();
			}
		}
	}

	Call_Back();

	/*
	**	Return with flag saying that the scenario file was read.
	*/
	ScenarioInit--;
	return(true);
}


/***********************************************************************************************
 * Read_Scenario_Ini_File -- Read specified scenario INI file.                                 *
 *                                                                                             *
 *    Read in the scenario INI file. This routine only sets the game                           *
 *    globals with that data that is explicitly defined in the INI file.                       *
 *    The remaining necessary interpolated data is generated elsewhere.                        *
 *                                                                                             *
 * INPUT:																												  *
 *				scenario_file_name	path to the ini for the scenario										  *
 *																															  *
 *				bin_file_name			path to the bin for the scenario										  *
 *											 																				  *
 *          root      root filename for scenario file to read                                  *
 *                                                                                             *
 *          fresh      true = should the current scenario be cleared?                          *
 *                                                                                             *
 * OUTPUT:  bool; Was the scenario read successful?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/28/2019 JAS : Created.                                                                 *
 *=============================================================================================*/
bool Read_Scenario_Ini_File(char *scenario_file_name, char* bin_file_name, const char* root, bool fresh)
{
	ScenarioInit++;

	char *buffer = (char *)_ShapeBuffer;
	memset(buffer, '\0', _ShapeBufferSize);
	char buf[128];
	int len;
	unsigned char val;

	CCFileClass file(scenario_file_name);
	if (!file.Is_Available()) {
		GlyphX_Debug_Print("Failed to find scenario file");
		GlyphX_Debug_Print(scenario_file_name);
		return(false);
	}
	else {

		GlyphX_Debug_Print("Opened scenario file");
		GlyphX_Debug_Print(scenario_file_name);

		file.Read(buffer, _ShapeBufferSize - 1);
	}

	/*
	** Init the Scenario CRC value
	*/
	ScenarioCRC = 0;
	len = strlen(buffer);
	for (int i = 0; i < len; i++) {
		val = (unsigned char)buffer[i];
#ifndef DEMO
		Add_CRC(&ScenarioCRC, (unsigned long)val);
#endif
	}

	/*
	**	Fetch the appropriate movie names from the INI file.
	*/
	WWGetPrivateProfileString("Basic", "Intro", "x", IntroMovie, sizeof(IntroMovie), buffer);
	WWGetPrivateProfileString("Basic", "Brief", "x", BriefMovie, sizeof(BriefMovie), buffer);
	WWGetPrivateProfileString("Basic", "Win", "x", WinMovie, sizeof(WinMovie), buffer);
	WWGetPrivateProfileString("Basic", "Win2", "x", WinMovie2, sizeof(WinMovie2), buffer);
	WWGetPrivateProfileString("Basic", "Win3", "x", WinMovie3, sizeof(WinMovie3), buffer);
	WWGetPrivateProfileString("Basic", "Win4", "x", WinMovie4, sizeof(WinMovie4), buffer);
	WWGetPrivateProfileString("Basic", "Lose", "x", LoseMovie, sizeof(LoseMovie), buffer);
	WWGetPrivateProfileString("Basic", "Action", "x", ActionMovie, sizeof(ActionMovie), buffer);

	/*
	**	For single-player scenarios, 'BuildLevel' is the scenario number.
	**	This must be set before any buildings are created (if a factory is created,
	**	it needs to know the BuildLevel for the sidebar.)
	*/
	if (GameToPlay == GAME_NORMAL) {
		/*
		** In this function we are only dealing with custom maps, so set based on the BuildLevel from the map, or 98 if none.
		** ST - 4/22/2020 5:14PM
		*/
		BuildLevel = WWGetPrivateProfileInt("Basic", "BuildLevel", 98, buffer);
	}

	/*
	**	Jurassic scenarios are allowed to build the full multiplayer set
	**	of objects.
	*/
	if (Special.IsJurassic && AreThingiesEnabled) {
		BuildLevel = 98;
	}

	/*
	**	Fetch the transition theme for this scenario.
	*/
	TransitTheme = THEME_NONE;
	WWGetPrivateProfileString("Basic", "Theme", "No Theme", buf, sizeof(buf), buffer);
	TransitTheme = Theme.From_Name(buf);

	/*
	**	Read in the team-type data. The team types must be created before any
	**	triggers can be created.
	*/
	TeamTypeClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in the specific information for each of the house types.  This creates
	**	the houses of different types.
	*/
	HouseClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in the trigger data. The triggers must be created before any other
	**	objects can be initialized.
	*/
	TriggerClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in the map control values. This includes dimensions
	**	as well as theater information.
	*/
	Map.Read_INI(buffer);
	Call_Back();

	/*
	**	Assign PlayerPtr by reading the player's house from the INI;
	**	Must be done before any TechnoClass objects are created.
	*/
	//	if (GameToPlay == GAME_NORMAL && (ScenPlayer == SCEN_PLAYER_GDI || ScenPlayer == SCEN_PLAYER_NOD)) {
	if (GameToPlay == GAME_NORMAL) {
		WWGetPrivateProfileString("Basic", "Player", "GoodGuy", buf, 127, buffer);
		CarryOverPercent = WWGetPrivateProfileInt("Basic", "CarryOverMoney", 100, buffer);
		CarryOverPercent = Cardinal_To_Fixed(100, CarryOverPercent);
		CarryOverCap = WWGetPrivateProfileInt("Basic", "CarryOverCap", -1, buffer);

		PlayerPtr = HouseClass::As_Pointer(HouseTypeClass::From_Name(buf));
		PlayerPtr->IsHuman = true;
		int carryover;
		if (CarryOverCap != -1) {
			carryover = MIN((int)Fixed_To_Cardinal(CarryOverMoney, CarryOverPercent), CarryOverCap);
		}
		else {
			carryover = Fixed_To_Cardinal(CarryOverMoney, CarryOverPercent);
		}
		PlayerPtr->Credits += carryover;
		PlayerPtr->InitialCredits += carryover;

		if (Special.IsJurassic) {
			PlayerPtr->ActLike = Whom;
		}

		if (Special.IsEasy) {
			PlayerPtr->Assign_Handicap(DIFF_EASY);
		}
		else if (Special.IsDifficult) {
			PlayerPtr->Assign_Handicap(DIFF_HARD);
		}
	}
	else {

#ifdef OBSOLETE
		if (GameToPlay == GAME_NORMAL && ScenPlayer == SCEN_PLAYER_JP) {
			PlayerPtr = HouseClass::As_Pointer(HOUSE_MULTI4);
			PlayerPtr->IsHuman = true;
			PlayerPtr->Credits += CarryOverMoney;
			PlayerPtr->InitialCredits += CarryOverMoney;
			PlayerPtr->ActLike = Whom;
		}
		else {
			Assign_Houses();
		}
#endif
		//Call new Assign_Houses function. ST - 6/25/2019 11:07AM
		//Assign_Houses();
		GlyphX_Assign_Houses();
	}

	/*
	**	Attempt to read the map's binary image file; if fails, read the
	**	template data from the INI, for backward compatibility
	*/
	if (fresh) {
		if (!Map.Read_Binary_File(bin_file_name, &ScenarioCRC)) {
			TemplateClass::Read_INI(buffer);
		}
	}
	Call_Back();

	/*
	**	Read in and place the 3D terrain objects.
	*/
	TerrainClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in and place the units (all sides).
	*/
	UnitClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in and place the infantry units (all sides).
	*/
	InfantryClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in and place all the buildings on the map.
	*/
	BuildingClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in the AI's base information.
	*/
	Base.Read_INI(buffer);
	Call_Back();

	/*
	**	Read in any normal overlay objects.
	*/
	OverlayClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in any smudge overlays.
	*/
	SmudgeClass::Read_INI(buffer);
	Call_Back();

	/*
	**	Read in any briefing text.
	*/
	char * stage = &BriefingText[0];
	*stage = '\0';
	int index = 1;

	/*
	**	Build the full text of the mission objective.
	*/
	for (;;) {
		char buff[16];

		sprintf(buff, "%d", index++);
		*stage = '\0';
		WWGetPrivateProfileString("Briefing", buff, "", stage, (sizeof(BriefingText) - strlen(BriefingText)) - 1, buffer);
		if (strlen(stage) == 0) break;
		strcat(stage, " ");
		stage += strlen(stage);
	}

	/*
	**	If the briefing text could not be found in the INI file, then search
	**	the mission.ini file.
	*/
	if (BriefingText[0] == '\0') {
		memset(_ShapeBuffer, '\0', _ShapeBufferSize);
		CCFileClass("MISSION.INI").Read(_ShapeBuffer, _ShapeBufferSize);

		char * buffer = (char *)Add_Long_To_Pointer(_ShapeBuffer, strlen(_ShapeBuffer));
		char * work = &BriefingText[0];
		int index = 1;

		/*
		**	Build the full text of the mission objective.
		*/
		for (;;) {
			char buff[16];

			sprintf(buff, "%d", index++);
			*work = '\0';
			WWGetPrivateProfileString(root, buff, "", work, (sizeof(BriefingText) - strlen(BriefingText)) - 1, _ShapeBuffer);
			if (strlen(work) == 0) break;
			strcat(work, " ");
			work += strlen(work);
		}
	}

	/*
	**	Perform a final overpass of the map. This handles smoothing of certain
	**	types of terrain (tiberium).
	*/
	Map.Overpass();
	Call_Back();

	/*
	**	Multi-player last-minute fixups:
	**	- If computer players are disabled, remove all computer-owned houses
	** - Otherwise, set MPlayerBlitz to 0 or 1, randomly
	**	- If bases are disabled, create the scenario dynamically
	**	- Remove any flag spot overlays lying around
	**	- If capture-the-flag is enabled, assign flags to cells.
	*/
	if (GameToPlay != GAME_NORMAL || ScenPlayer == SCEN_PLAYER_2PLAYER ||
		ScenPlayer == SCEN_PLAYER_MPLAYER) {

		/*
		**	If Ghosts are disabled and we're not editing, remove computer players
		**	(Must be done after all objects are read in from the INI)
		*/
		if (!MPlayerGhosts && !Debug_Map) {
			//Remove_AI_Players();		    // Done elsewhere now. ST - 6/25/2019 12:33PM
		}
		else {

			/*
			** If Ghosts are on, set up their houses for blitzing the humans
			*/
#ifndef USE_RA_AI
			MPlayerBlitz = IRandom(0, 1);					// 1 = computer will blitz

			if (MPlayerBlitz) {
				if (MPlayerBases) {
					rndmax = 14000;
					rndmin = 10000;
				}
				else {
					rndmax = 8000;
					rndmin = 4000;
				}

				for (int i = 0; i < MPlayerMax; i++) {
					HousesType house = (HousesType)(i + (int)HOUSE_MULTI1);
					HouseClass *housep = HouseClass::As_Pointer(house);
					if (housep) {	//Added. ST - 6/25/2019 11:37AM
						housep->BlitzTime = IRandom(rndmin, rndmax);
					}
				}

			}
#else // USE_RA_AI
			MPlayerBlitz = 0;
#endif // USE_RA_AI
		}

		/*
		**	Units must be created for each house.  If bases are ON, this routine
		**	will create an MCV along with the units; otherwise, it will just create
		**	a whole bunch of units.  MPlayerUnitCount is the total # of units
		**	to create.
		*/
		if (!Debug_Map) {
			int save_init = ScenarioInit;			// turn ScenarioInit off
			ScenarioInit = 0;
			Create_Units();
			ScenarioInit = save_init;				// turn ScenarioInit back on
		}

		/*
		**	Place crates if MPlayerGoodies is on.
		*/
		if (MPlayerGoodies) {
			for (int index = 0; index < MPlayerCount; index++) {
				//for (int index = 0; index < 200; index++) {  // Lots of crates for test		
				Map.Place_Random_Crate();
			}
		}
	}

	Call_Back();

	/*
	**	Return with flag saying that the scenario file was read.
	*/
	ScenarioInit--;
	return(true);
}



/***********************************************************************************************
 * Read_Movies_From_Scenario_Ini -- Reads just the movie files from the scenario.              *
 *                                                                                             *
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
 *   10/14/2019 JAS : Created.                                                                 *
 *=============================================================================================*/
bool Read_Movies_From_Scenario_Ini(char *root, bool fresh)
{
	char *buffer;				// Scenario.ini staging buffer pointer.
	char fname[_MAX_FNAME + _MAX_EXT];			// full INI filename
//	char buf[128];				// Working string staging buffer.
#ifndef USE_RA_AI
	int rndmax;
	int rndmin;
#endif //USE_RA_AI
	int len;
	unsigned char val;

	ScenarioInit++;

	/*
	**	Fetch working pointer to the INI staging buffer. Make sure that the buffer
	**	is cleared out before proceeding.  (Don't use the HidPage for this, since
	**	the HidPage may be needed for various uncompressions during the INI
	**	parsing.)
	*/
	buffer = (char *)_ShapeBuffer;
	memset(buffer, '\0', _ShapeBufferSize);

	if (fresh) {
		Clear_Scenario();
	}

	/*
	** If we are not dealing with scenario 1, or a multi player scenario
	** then make sure the correct disk is in the drive.
	*/
	if (RequiredCD != -2) {
		if (Scenario >= 20 && Scenario < 60 && GameToPlay == GAME_NORMAL) {
			RequiredCD = 2;
		}
		else {
			if (Scenario != 1) {
				if (Scenario >= 60) {
					RequiredCD = -1;
				}
				else {
					switch (ScenPlayer) {
					case SCEN_PLAYER_GDI:
						RequiredCD = 0;
						break;
					case SCEN_PLAYER_NOD:
						RequiredCD = 1;
						break;
					default:
						RequiredCD = -1;
						break;
					}
				}
			}
			else {
				RequiredCD = -1;
			}
		}
	}
	if (!Force_CD_Available(RequiredCD)) {
		Prog_End("Read_Scenario_Ini - CD not found", true);
		if (!RunningAsDLL) {
			exit(EXIT_FAILURE);
		}
	}

	/*
	**	Create scenario filename and read the file.
	*/

	sprintf(fname, "%s.INI", root);
	CCFileClass file(fname);
	if (!file.Is_Available()) {
		GlyphX_Debug_Print("Failed to find scenario file");
		GlyphX_Debug_Print(fname);
		return(false);
	}
	else {

		GlyphX_Debug_Print("Opened scenario file");
		GlyphX_Debug_Print(fname);

		file.Read(buffer, _ShapeBufferSize - 1);
	}

	/*
	** Init the Scenario CRC value
	*/
	ScenarioCRC = 0;
	len = strlen(buffer);
	for (int i = 0; i < len; i++) {
		val = (unsigned char)buffer[i];
#ifndef DEMO
		Add_CRC(&ScenarioCRC, (unsigned long)val);
#endif
	}

	/*
	**	Fetch the appropriate movie names from the INI file.
	*/
	WWGetPrivateProfileString("Basic", "Intro", "x", IntroMovie, sizeof(IntroMovie), buffer);
	WWGetPrivateProfileString("Basic", "Brief", "x", BriefMovie, sizeof(BriefMovie), buffer);
	WWGetPrivateProfileString("Basic", "Win", "x", WinMovie, sizeof(WinMovie), buffer);
	WWGetPrivateProfileString("Basic", "Win2", "x", WinMovie2, sizeof(WinMovie2), buffer);
	WWGetPrivateProfileString("Basic", "Win3", "x", WinMovie3, sizeof(WinMovie3), buffer);
	WWGetPrivateProfileString("Basic", "Win4", "x", WinMovie4, sizeof(WinMovie4), buffer);
	WWGetPrivateProfileString("Basic", "Lose", "x", LoseMovie, sizeof(LoseMovie), buffer);
	WWGetPrivateProfileString("Basic", "Action", "x", ActionMovie, sizeof(ActionMovie), buffer);

	/*
	**	Fetch the transition theme for this scenario.
	*/
	TransitTheme = THEME_NONE;
	WWGetPrivateProfileString("Basic", "Theme", "No Theme", MovieThemeName, sizeof(MovieThemeName), buffer);
	//TransitTheme = Theme.From_Name(buf);

	/*
	**	Return with flag saying that the scenario file was read.
	*/
	ScenarioInit--;
	return(true);
}



/***********************************************************************************************
 * Write_Scenario_Ini -- Write the scenario INI file.                                          *
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
void Write_Scenario_Ini(char *root)
{
#ifndef CHEAT_KEYS
	root = root;
#else
	char * buffer;			// Scenario.ini staging buffer pointer.
	char fname[_MAX_FNAME+_MAX_EXT];		// full scenario name
	HousesType house;
	CCFileClass file;

	/*
	**	Get a working pointer to the INI staging buffer. Make sure that the buffer
	**	starts cleared out of any data.
	*/
	buffer = (char *)_ShapeBuffer;
	memset(buffer, '\0', _ShapeBufferSize);

	switch (ScenPlayer) {
		case SCEN_PLAYER_GDI:
			house = HOUSE_GOOD;
			break;

		case SCEN_PLAYER_NOD:
			house = HOUSE_BAD;
			break;

		case SCEN_PLAYER_JP:
			house = HOUSE_JP;
			break;

		default:
			house = HOUSE_MULTI1;
			break;
	}

	/*
	**	Create scenario filename and clear the buffer to empty.
	*/
	sprintf(fname,"%s.INI",root);
	file.Set_Name(fname);
	if (file.Is_Available()) {
//		file.Open(READ);
		file.Read(buffer, _ShapeBufferSize-1);
//		file.Close();
	} else {
		sprintf(buffer, "; Scenario %d control for house %s.\r\n", Scenario, HouseTypeClass::As_Reference(house).IniName);
	}

	WWWritePrivateProfileString("Basic", "Intro", IntroMovie, buffer);
	WWWritePrivateProfileString("Basic", "Brief", BriefMovie, buffer);
	WWWritePrivateProfileString("Basic", "Win", WinMovie, buffer);
	WWWritePrivateProfileString("Basic", "Win2", WinMovie, buffer);
	WWWritePrivateProfileString("Basic", "Win3", WinMovie, buffer);
	WWWritePrivateProfileString("Basic", "Win4", WinMovie, buffer);
	WWWritePrivateProfileString("Basic", "Lose", LoseMovie, buffer);
	WWWritePrivateProfileString("Basic", "Action", ActionMovie, buffer);
	WWWritePrivateProfileString("Basic", "Player", PlayerPtr->Class->IniName, buffer);
	WWWritePrivateProfileString("Basic", "Theme", Theme.Base_Name(TransitTheme), buffer);
	WWWritePrivateProfileInt("Basic", "BuildLevel", BuildLevel, buffer);
	WWWritePrivateProfileInt("Basic", "CarryOverMoney", Fixed_To_Cardinal(100, CarryOverPercent), buffer);
	WWWritePrivateProfileInt("Basic", "CarryOverCap", CarryOverCap, buffer);

	TeamTypeClass::Write_INI(buffer, true);
	TriggerClass::Write_INI(buffer, true);
	Map.Write_INI(buffer);
	Map.Write_Binary(root);
	HouseClass::Write_INI(buffer);
	UnitClass::Write_INI(buffer);
	InfantryClass::Write_INI(buffer);
	BuildingClass::Write_INI(buffer);
	TerrainClass::Write_INI(buffer);
	OverlayClass::Write_INI(buffer);
	SmudgeClass::Write_INI(buffer);

	Base.Write_INI(buffer);

	/*
	**	Write the scenario data out to a file.
	*/
//	file.Open(WRITE);
	file.Write(buffer, strlen(buffer));
//	file.Close();

	/*
	**	Now update the Master INI file, containing the master list of triggers & teams
	*/
	memset(buffer, '\0', _ShapeBufferSize);

	file.Set_Name("MASTER.INI");
	if (file.Is_Available()) {
//		file.Open(READ);
		file.Read(buffer, _ShapeBufferSize-1);
//		file.Close();
	} else {
		sprintf(buffer, "; Master Trigger & Team List.\r\n");
	}

	TeamTypeClass::Write_INI(buffer, false);
	TriggerClass::Write_INI(buffer, false);

//	file.Open(WRITE);
	file.Write(buffer,strlen(buffer));
//	file.Close();
#endif
}


/***********************************************************************************************
 * Assign_Houses -- Assigns multiplayer houses to various players                              *
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
 *   07/14/1995 JLB : Records name of player in house structure.                               *
 *=============================================================================================*/
static void Assign_Houses(void)
{
	HousesType house;
	HousesType pref_house;
	HouseClass *housep;
	bool house_used[MAX_PLAYERS];	// true = this house is in use
	bool color_used[16];				// true = this color is in use. We have more than 6 color options now, so bumped this to 16. ST - 6/19/2019 5:18PM
	int i,j;
	PlayerColorType color;
	HousesType house2;
	HouseClass *housep2;

	/*
	**	Init the 'used' flag for all houses & colors to 0
	*/
	for (i = 0; i < MAX_PLAYERS; i++) {
		house_used[i] = false;
	}
	for (i = 0; i < 16; i++) {
		color_used[i] = false;
	}

	/*
	**	For each player, randomly pick a house
	*/
	for (i = 0; i < MPlayerCount; i++) {
		j = Random_Pick(0, MPlayerMax-1);

		/*
		**	If this house was already selected, decrement 'i' & keep looping.
		*/
		if (house_used[j]) {
			i--;
			continue;
		}

		/*
		**	Set the house, preferred house (GDI/NOD), color, and actual house;
		**	get a pointer to the house instance
		*/
		house = (HousesType)(j + (int)HOUSE_MULTI1);
		pref_house = MPlayerID_To_HousesType(MPlayerID[i]);
		color = MPlayerID_To_ColorIndex(MPlayerID[i]);
		housep = HouseClass::As_Pointer(house);
		MPlayerHouses[i] = house;

		/*
		**	Mark this house & color as used
		*/
		house_used[j] = true;
		color_used[color] = true;

		/*
		**	Set the house's IsHuman, Credits, ActLike, & RemapTable
		*/
		memset((char *)housep->Name, 0, MPLAYER_NAME_MAX);
		strncpy((char *)housep->Name, MPlayerNames[i], MPLAYER_NAME_MAX-1);
		housep->IsHuman = true;
		housep->Init_Data(color, pref_house, MPlayerCredits);

		/*
		**	If this ID is for myself, set up PlayerPtr
		*/
		if (MPlayerID[i] == MPlayerLocalID) {
			PlayerPtr = housep;
		}
	}

	/*
	**	For all houses not assigned to a player, set them up for computer use
	*/
	for (i = 0; i < MPlayerMax; i++) {
		if (house_used[i] == false) {

			/*
			**	Set the house, preferred house (GDI/NOD), and color; get a pointer
			**	to the house instance
			*/
			house = (HousesType)(i + (int)HOUSE_MULTI1);
			pref_house = (HousesType)(IRandom(0, 1) + (int)HOUSE_GOOD);
			for (;;) {
				color = Random_Pick(REMAP_FIRST, REMAP_LAST);
				if (color_used[color] == false) {
					break;
				}
			}
			housep = HouseClass::As_Pointer (house);

			/*
			**	Mark this house & color as used
			*/
			house_used[i] = true;
			color_used[color] = true;

			/*
			**	Set the house's IsHuman, Credits, ActLike, & RemapTable
			*/
			housep->IsHuman = false;
			housep->Init_Data(color, pref_house, MPlayerCredits);
		}
	}

	/*
	**	Now make all computer-owned houses allies of each other.
	*/
	for (house = HOUSE_MULTI1; house < (HOUSE_MULTI1 + MPlayerMax); house++) {
		housep = HouseClass::As_Pointer(house);
		if (housep->IsHuman)
			continue;

		for (house2 = HOUSE_MULTI1; house2 < (HOUSE_MULTI1 + MPlayerMax); house2++) {
			housep2 = HouseClass::As_Pointer (house2);
			if (housep2->IsHuman)
				continue;
			housep->Make_Ally(house2);
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
	HousesType house;
	HouseClass *housep;

	for (i = 0; i < MAX_PLAYERS; i++) {
		house = (HousesType)(i + (int)HOUSE_MULTI1);
		housep = HouseClass::As_Pointer (house);
		if (housep->IsHuman == false) {
			housep->Clobber_All();
		}
	}
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
 *   ------------------ Unit Summary: -------------------------------                          *
 *   UNIT_MTANK               Medium tank (M1).            GDI      7                          *
 *   UNIT_JEEP               4x4 jeep replacement.      GDI      5                             *
 *   UNIT_MLRS               MLRS rocket launcher.      GDI      99                            *
 *   UNIT_APC                  APC.                        GDI      10                         *
 *   UNIT_HTANK               Heavy tank (Mammoth).      GDI      13                           *
 *                                                                                             *
 *   UNIT_LTANK               Light tank ('Bradly').      NOD      5                           *
 *   UNIT_BUGGY               Rat patrol dune buggy type NOD      5                            *
 *   UNIT_ARTY               Artillery unit.            NOD      10                            *
 *   UNIT_FTANK               Flame thrower tank.         NOD      11                          *
 *   UNIT_STANK               Stealth tank (Romulan).      NOD      13                         *
 *   UNIT_BIKE               Nod recon motor-bike.      NOD      99                            *
 *                                                                                             *
 *   ~1/3 chance of getting: {UNIT_MHQ,               Mobile Head Quarters.                    *
 *                                                                                             *
 *   ------------------ Infantry Summary: -------------------------------                      *
 *   INFANTRY_E1,            Mini-gun armed.            GDI/NOD                                *
 *   INFANTRY_E2,            Grenade thrower.            GDI                                   *
 *   INFANTRY_E3,            Rocket launcher.            NOD                                   *
 *   INFANTRY_E6,            Rocket launcher             GDI                                   *
 *   INFANTRY_E4,            Flame thrower equipped.      NOD                                  *
 *   INFANTRY_RAMBO,         Commando.                  GDI/NOD                                *
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

static void Create_Units(void)
{
	enum {
		NUM_UNIT_CATEGORIES = 8,
		NUM_INFANTRY_CATEGORIES = 5,
	};

	static struct {
		int MinLevel;
		int GDICount;
		UnitType GDIType;
		int NODCount;
		UnitType NODType;
	} utable[] = {
		{0,	1,UNIT_MTANK,	2,UNIT_LTANK},
		{2,	1,UNIT_JEEP,	1,UNIT_BUGGY},
		{3,	1,UNIT_MLRS,	1,UNIT_ARTY},
		{4,	1,UNIT_APC,		2,UNIT_BUGGY},
		{5,	1,UNIT_JEEP,	1,UNIT_BIKE},
		{5,	2,UNIT_JEEP,	1,UNIT_FTANK},
		{6,	1,UNIT_MSAM,	1,UNIT_MSAM},
		{7,	1,UNIT_HTANK,	2,UNIT_STANK},
	};
	static int num_units[NUM_UNIT_CATEGORIES];		// # of each type of unit to create
	int tot_units;												// total # units to create

	static struct {
		int MinLevel;
		int GDICount;
		InfantryType GDIType;
		int NODCount;
		InfantryType NODType;
	} itable[] = {
		{0,	1,INFANTRY_E1,	1,INFANTRY_E1},
		{1,	1,INFANTRY_E2,	1,INFANTRY_E3},
		{3,	1,INFANTRY_E3,	1,INFANTRY_E3},
		{5,	1,INFANTRY_E3,	1,INFANTRY_E4},
		{7,	1,INFANTRY_RAMBO,	1,INFANTRY_RAMBO},
	};
	static int num_infantry[NUM_INFANTRY_CATEGORIES];// # of each type of infantry to create
	int tot_infantry;											// total # infantry to create

	CELL waypts[26];
//	CELL sorted_waypts[26];
	int num_waypts;

	HousesType h;			// house loop counter
	HouseClass *hptr;		// ptr to house being processed

	CELL centroid;			// centroid of this house's stuff
//	int try_count;			// # times we've tried to select a centroid
	CELL centerpt;			// centroid for a category of objects, as a CELL

	int u_limit;			// last allowable index of units for this BuildLevel
	int i_limit;			// last allowable index of infantry for this BuildLevel
	TechnoClass *obj;		// newly-created object
	int i,j,k;				// loop counters
	int scaleval;			// value to scale # units or infantry

	ReserveInfantryIndex = ReserveUnitIndex = 0;

	/*------------------------------------------------------------------------
	For the current BuildLevel, find the max allowable index into the tables
	------------------------------------------------------------------------*/
	for (i = 0; i < NUM_UNIT_CATEGORIES; i++) {
		if (BuildLevel >= (unsigned)utable[i].MinLevel)
			u_limit = i;
	}
	for (i = 0; i < NUM_INFANTRY_CATEGORIES; i++) {
		if (BuildLevel >= (unsigned)utable[i].MinLevel)
			i_limit = i;
	}

	/*------------------------------------------------------------------------
	Compute how many of each buildable category to create
	------------------------------------------------------------------------*/
	/*........................................................................
	Compute allowed # units
	........................................................................*/
	tot_units = (MPlayerUnitCount * 2) / 3;
//	tot_units = MAX(tot_units, 1);

	/*........................................................................
	Init # of each category to 0
	........................................................................*/
	for (i = 0; i <= u_limit; i++)
		num_units[i] = 0;

	/*........................................................................
	Increment # of each category, until we've used up all units
	........................................................................*/
	j = 0;
	for (i = 0; i < tot_units; i++) {
		num_units[j]++;
		j++;
		if (j > u_limit)
			j = 0;
	}

	/*........................................................................
	Compute allowed # infantry
	........................................................................*/
	tot_infantry = MPlayerUnitCount - tot_units;

	/*........................................................................
	Init # of each category to 0
	........................................................................*/
	for (i = 0; i <= i_limit; i++)
		num_infantry[i] = 0;

	/*........................................................................
	Increment # of each category, until we've used up all infantry
	........................................................................*/
	j = 0;
	for (i = 0; i < tot_infantry; i++) {
		num_infantry[j]++;
		j++;
		if (j > i_limit)
			j = 0;
	}

	/*------------------------------------------------------------------------
	Now sort all the Waypoints on the map by distance.
	------------------------------------------------------------------------*/
	num_waypts = 0;									// counts # waypoints

	/*........................................................................
	First, copy all valid waytpoints into my 'waypts' array
	........................................................................*/
	for (i = 0; i < 26; i++) {
		if (Waypoint[i] != -1) {
			waypts[num_waypts] = Waypoint[i];
			num_waypts++;
		}
	}

	/*........................................................................
	Now sort the 'waypts' array
	........................................................................*/
#ifndef USE_GLYPHX_START_LOCATIONS
	//Sort_Cells (waypts, num_waypts, sorted_waypts);
#endif

	/*------------------------------------------------------------------------
	Loop through all houses.  Computer-controlled houses, with MPlayerBases
	ON, are treated as though bases are OFF (since we have no base-building
	AI logic.)
	------------------------------------------------------------------------*/
	for (h = HOUSE_MULTI1; h < (HOUSE_MULTI1 + MPlayerMax); h++) {

		/*.....................................................................
		Get a pointer to this house; if there is none, go to the next house
		.....................................................................*/
		hptr = HouseClass::As_Pointer(h);
		if (!hptr)
			continue;

#ifdef USE_GLYPHX_START_LOCATIONS
		/*
		** New code that respects the start locations passed in from GlyphX.
		**
		** ST - 1/8/2020 3:39PM
		*/
		centroid = waypts[hptr->StartLocationOverride];

#else // USE_GLYPHX_START_LOCATIONS
		/*
		** Original start position logic.
		*/

		/*.....................................................................
		Pick a random waypoint; if the chosen waypoint isn't valid, try again.
		'centroid' will be the centroid of all this house's stuff.
		.....................................................................*/
		try_count = 0;
		while (1) {
			j = IRandom(0,MPlayerMax - 1);
			if (sorted_waypts[j] != -1) {
				centroid = sorted_waypts[j];
				sorted_waypts[j] = -1;
				break;
			}
			try_count++;

			/*..................................................................
			OK, we've tried enough; just pick any old cell at random, as long
			as it's mappable.
			..................................................................*/
			if (try_count > 200) {
				while (1) {
					centroid = IRandom(0,MAP_CELL_TOTAL - 1);
					if (Map.In_Radar(centroid))
						break;
				}
				break;
			}
		}
#endif // USE_GLYPHX_START_LOCATIONS

		/*---------------------------------------------------------------------
		If Bases are ON, human & computer houses are treated differently
		---------------------------------------------------------------------*/
		if (MPlayerBases) {
			/*..................................................................
			- For a human-controlled house:
			  - Set 'scaleval' to 1
			  - Create an MCV
			  - Attach a flag to it for capture-the-flag mode
			..................................................................*/
			if (hptr->IsHuman) {
				scaleval = 1;

#ifndef USE_RA_AI           // Moved to below. ST - 7/25/2019 11:21AM
				obj = new UnitClass (UNIT_MCV, h);
				if (!obj->Unlimbo(Cell_Coord(centroid),DIR_N)) {
					if (!Scan_Place_Object(obj, centroid)) {
						delete obj;
						obj = NULL;
					}
				}
				if (obj) {
					hptr->FlagHome = 0;
					hptr->FlagLocation = 0;
					if (Special.IsCaptureTheFlag) {
						hptr->Flag_Attach((UnitClass *)obj,true);
					}
				}
#endif //USE_RA_AI
			} else {

				/*..................................................................
				- For computer-controlled house:
				  - Set 'scaleval' to 3
				  - Create a Mobile HQ for capture-the-flag mode
				..................................................................*/
				// Added fix for divide by zero. ST - 6/26/2019 10:40AM
				int ai_player_count = MPlayerMax - MPlayerCount;
				//scaleval = 3 / (MPlayerMax - MPlayerCount);
				//scaleval = max(ai_player_count, 1);
				scaleval = 1;		//Set to 1 since EA QA can't beat skirmish with scaleval set higher.
				
				//if (scaleval==0) {
				//	scaleval = 1;
				//}

#ifndef USE_RA_AI           // Give the AI an MCV below. ST - 7/25/2019 11:22AM
				if (Special.IsCaptureTheFlag) {
					obj = new UnitClass (UNIT_MHQ, h);
					if (!obj->Unlimbo(Cell_Coord(centroid),DIR_N)) {
						if (!Scan_Place_Object(obj, centroid)) {
							delete obj;
							obj = NULL;
						}
					}
					hptr->FlagHome = 0;					// turn house's flag off
					hptr->FlagLocation = 0;
				}
#endif //USE_RA_AI
			}

#ifdef USE_RA_AI
			/*
			** Moved HQ code down here, so the AI player gets one too. ST - 7/25/2019 11:21AM
			*/
			Reserve_Unit();
			obj = new UnitClass (UNIT_MCV, h);
			if (!obj->Unlimbo(Cell_Coord(centroid),DIR_N)) {
				if (!Scan_Place_Object(obj, centroid)) {
					delete obj;
					obj = NULL;
				}
			}
			if (obj) {
				hptr->FlagHome = 0;
				hptr->FlagLocation = 0;
				if (Special.IsCaptureTheFlag) {
					hptr->Flag_Attach((UnitClass *)obj,true);
				}
			}
#endif //USE_RA_AI


		} else {

			/*---------------------------------------------------------------------
			If bases are OFF, set 'scaleval' to 1 & create a Mobile HQ for
			capture-the-flag mode.
			---------------------------------------------------------------------*/
			scaleval = 1;
			if (Special.IsCaptureTheFlag) {
				Reserve_Unit();
				obj = new UnitClass (UNIT_MHQ, h);
				obj->Unlimbo(Cell_Coord(centroid),DIR_N);
				hptr->FlagHome = 0;					// turn house's flag off
				hptr->FlagLocation = 0;
			}
		}

		/*---------------------------------------------------------------------
		Set the house's max # units (this is used in the Mission_Timed_Hunt())
		---------------------------------------------------------------------*/
		hptr->MaxUnit = MPlayerUnitCount * scaleval;

		/*---------------------------------------------------------------------
		Create units for this house
		---------------------------------------------------------------------*/
		for (i = 0; i <= u_limit; i++) {
			/*..................................................................
			Find the center point for this category.
			..................................................................*/
			centerpt = Clip_Scatter(centroid,4);

			/*..................................................................
			Place objects; loop through all unit in this category
			..................................................................*/
			for (j = 0; j < num_units[i] * scaleval; j++) {
				/*...............................................................
				Create a GDI unit
				...............................................................*/
				if (hptr->ActLike == HOUSE_GOOD) {
					for (k = 0; k < utable[i].GDICount; k++) {
						Reserve_Unit();
						obj = new UnitClass (utable[i].GDIType, h);
						if (!Scan_Place_Object(obj, centerpt)) {
							delete obj;
						} else {
							
							/*
							** Don't use MISSION_TIMED_HUNT since it can trigger blitz behavior. ST - 2/28/2020 10:51AM
							*/
							//if (!hptr->IsHuman) {
							//	obj->Set_Mission(MISSION_TIMED_HUNT);
							//}
							if (!hptr->IsHuman) {
								obj->Set_Mission(MISSION_GUARD_AREA);
							}
						}
					}
				} else {

					/*...............................................................
					Create a NOD unit
					...............................................................*/
					for (k = 0; k < utable[i].NODCount; k++) {
						Reserve_Unit();
						obj = new UnitClass (utable[i].NODType, h);
						if (!Scan_Place_Object(obj, centerpt)) {
							delete obj;
						} else {
							/*
							** Don't use MISSION_TIMED_HUNT since it can trigger blitz behavior. ST - 2/28/2020 10:51AM
							*/
							//if (!hptr->IsHuman) {
							//	obj->Set_Mission(MISSION_TIMED_HUNT);
							//}
							if (!hptr->IsHuman) {
								obj->Set_Mission(MISSION_GUARD_AREA);
							}
						}
					}
				}
			}
		}

		/*---------------------------------------------------------------------
		Create infantry
		---------------------------------------------------------------------*/
		for (i = 0; i <= i_limit; i++) {
			/*..................................................................
			Find the center point for this category.
			..................................................................*/
			centerpt = Clip_Scatter(centroid,4);

			/*..................................................................
			Place objects; loop through all unit in this category
			..................................................................*/
			for (j = 0; j < num_infantry[i] * scaleval; j++) {
				/*...............................................................
				Create GDI infantry (Note: Unlimbo calls Enter_Idle_Mode(), which
				assigns the infantry to HUNT; we must use Set_Mission() to override
				this state.)
				...............................................................*/
				if (hptr->ActLike == HOUSE_GOOD) {
					for (k = 0; k < itable[i].GDICount; k++) {
						Reserve_Infantry();
						obj = new InfantryClass (itable[i].GDIType, h);
						if (!Scan_Place_Object(obj, centerpt)) {
							delete obj;
						} else {
							/*
							** Don't use MISSION_TIMED_HUNT since it can trigger blitz behavior. ST - 2/28/2020 10:51AM
							*/
							//if (!hptr->IsHuman) {
							//	obj->Set_Mission(MISSION_TIMED_HUNT);
							//}
							if (!hptr->IsHuman) {
								obj->Set_Mission(MISSION_GUARD_AREA);
							}
						}
					}
				} else {

					/*...............................................................
					Create NOD infantry
					...............................................................*/
					for (k = 0; k < itable[i].NODCount; k++) {
						Reserve_Infantry();
						obj = new InfantryClass (itable[i].NODType, h);
						if (!Scan_Place_Object(obj, centerpt)) {
							delete obj;
						} else {
							/*
							** Don't use MISSION_TIMED_HUNT since it can trigger blitz behavior. ST - 2/28/2020 10:51AM
							*/
							//if (!hptr->IsHuman) {
							//	obj->Set_Mission(MISSION_TIMED_HUNT);
							//}
							if (!hptr->IsHuman) {
								obj->Set_Mission(MISSION_GUARD_AREA);
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
int Scan_Place_Object(ObjectClass *obj, CELL cell)
{
	int dist;				// for object placement
	FacingType rot;		// for object placement
	FacingType fcounter;	// for object placement
	int tryval;
	CELL newcell;
	TechnoClass *techno;
	int skipit;

	/*------------------------------------------------------------------------
	First try to unlimbo the object in the given cell.
	------------------------------------------------------------------------*/
	if (Map.In_Radar(cell)) {
		techno = Map[cell].Cell_Techno();
		if (!techno || (techno->What_Am_I()==RTTI_INFANTRY &&
			obj->What_Am_I()==RTTI_INFANTRY)) {
			if (obj->Unlimbo(Cell_Coord(cell),DIR_N)) {
				return(true);
			}
		}
	}

	/*------------------------------------------------------------------------
	Loop through distances from the given center cell; skip the center cell.
	For each distance, try placing the object along each rotational direction;
	if none are available, try each direction with a random scatter value.
	If that fails, go to the next distance.
	This ensures that the closest coordinates are filled first.
	------------------------------------------------------------------------*/
	for (dist = 1; dist < 32; dist++) {
		/*.....................................................................
		Pick a random starting direction
		.....................................................................*/
		rot = (FacingType)IRandom (FACING_N, FACING_NW);
		/*.....................................................................
		Try all directions twice
		.....................................................................*/
		for (tryval = 0 ; tryval < 2; tryval++) {
			/*..................................................................
			Loop through all directions, at this distance.
			..................................................................*/
			for (fcounter = FACING_N; fcounter <= FACING_NW; fcounter++) {

				skipit = false;

				/*...............................................................
				Pick a coordinate along this directional axis
				...............................................................*/
				newcell = Clip_Move(cell, rot, dist);

				/*...............................................................
				If this is our second try at this distance, add a random scatter
				to the desired cell, so our units aren't all aligned along spokes.
				...............................................................*/
				if (tryval > 0)
					newcell = Clip_Scatter (newcell, 1);

				/*...............................................................
				If, by randomly scattering, we've chosen the exact center, skip
				it & try another direction.
				...............................................................*/
				if (newcell==cell)
					skipit = true;

				if (!skipit) {
					/*............................................................
					Only attempt to Unlimbo the object if:
					- there is no techno in the cell
					- the techno in the cell & the object are both infantry
					............................................................*/
					techno = Map[newcell].Cell_Techno();
					if (!techno || (techno->What_Am_I()==RTTI_INFANTRY &&
						obj->What_Am_I()==RTTI_INFANTRY)) {
						if (obj->Unlimbo(Cell_Coord(newcell),DIR_N)) {
							return(true);
						}
					}
				}

				rot++;
				if (rot > FACING_NW)
					rot = FACING_N;
			}
		}
	}

	return(false);
}


/***********************************************************************************************
 * Sort_Cells -- sorts an array of cells by distance                                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      cells         array to sort                                                            *
 *      numcells      # entries in 'cells'                                                     *
 *      outcells      array to store sorted values in                                          *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static void Sort_Cells(CELL *cells, int numcells, CELL *outcells)
{
	int i,j,k;
	int num_sorted = 0;
	int num_unsorted = numcells;

	/*------------------------------------------------------------------------
	Pick the first cell at random
	------------------------------------------------------------------------*/
	j = Random_Pick(0,numcells - 1);
	outcells[0] = cells[j];
	num_sorted++;

	for (k = j; k < num_unsorted - 1; k++) {
		cells[k] = cells[k + 1];
	}
	num_unsorted--;

	/*------------------------------------------------------------------------
	After the first cell, assign the other cells based on who's furthest away
	from the chosen ones.
	------------------------------------------------------------------------*/
	for (i = 0; i < numcells; i++) {
		j = Furthest_Cell (outcells, num_sorted, cells, num_unsorted);
		outcells[num_sorted] = cells[j];
		num_sorted++;

		for (k = j; k < num_unsorted - 1; k++) {
			cells[k] = cells[k + 1];
		}
		num_unsorted--;
	}
}


/***********************************************************************************************
 * Furthest_Cell -- Finds cell furthest from a group of cells                                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      cells            array of cells to find furthest-cell-away-from                        *
 *      numcells         # entries in 'cells'                                                  *
 *      tcells         array of cells to test; one of these will be selected as being          *
 *                     "furthest" from all the cells in 'cells'                                *
 *      numtcells      # entries in 'tcells'                                                   *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      index of 'tcell' that's furthest away from 'cells'                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static int Furthest_Cell(CELL *cells, int numcells, CELL *tcells, int numtcells)
{
	int i;
	int j;
	int mindist;			// minimum distance a 'tcell' is from a 'cell'
	int maxmindist;		// the highest mindist value of all tcells
	int maxmin_idx;		// index of the tcell with largest mindist
	int dist;				// working distance measure

	/*------------------------------------------------------------------------
	Initialize
	------------------------------------------------------------------------*/
	maxmindist = 0;
	maxmin_idx = 0;

	/*------------------------------------------------------------------------
	Loop through all test cells, finding the furthest one from all entries in
	the 'cells' array
	------------------------------------------------------------------------*/
	for (i = 0; i < numtcells; i++) {

		/*.....................................................................
		Find the 'cell' closest to this 'tcell'
		.....................................................................*/
		mindist = 0xffff;
		for (j = 0; j < numcells; j++) {
			dist = Distance (tcells[i],cells[j]);
			if (dist <= mindist) {
				mindist = dist;
			}
		}

		/*.....................................................................
		If this tcell is further away than the others, save its distance &
		index value
		.....................................................................*/
		if (mindist >= maxmindist) {
			maxmindist = mindist;
			maxmin_idx = i;
		}
	}

	return (maxmin_idx);
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

	/*------------------------------------------------------------------------
	Get X & Y coords of given starting cell
	------------------------------------------------------------------------*/
	x = Cell_X(cell);
	y = Cell_Y(cell);

	/*------------------------------------------------------------------------
	Compute our x & y limits
	------------------------------------------------------------------------*/
	xmin = Map.MapCellX;
	xmax = xmin + Map.MapCellWidth - 1;
	ymin = Map.MapCellY;
	ymax = ymin + Map.MapCellHeight - 1;

	/*------------------------------------------------------------------------
	Adjust the x-coordinate
	------------------------------------------------------------------------*/
	xdist = IRandom(0,maxdist);
	if (IRandom(0,1)==0) {
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

	/*------------------------------------------------------------------------
	Adjust the y-coordinate
	------------------------------------------------------------------------*/
	ydist = IRandom(0,maxdist);
	if (IRandom(0,1)==0) {
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

	return (XY_Cell(x,y));
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

	/*------------------------------------------------------------------------
	Get X & Y coords of given starting cell
	------------------------------------------------------------------------*/
	x = Cell_X(cell);
	y = Cell_Y(cell);

	/*------------------------------------------------------------------------
	Compute our x & y limits
	------------------------------------------------------------------------*/
	xmin = Map.MapCellX;
	xmax = xmin + Map.MapCellWidth - 1;
	ymin = Map.MapCellY;
	ymax = ymin + Map.MapCellHeight - 1;

	/*------------------------------------------------------------------------
	Adjust the x-coordinate
	------------------------------------------------------------------------*/
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

	/*------------------------------------------------------------------------
	Clip to the map
	------------------------------------------------------------------------*/
	if (x > xmax)
		x = xmax;
	if (x < xmin)
		x = xmin;

	if (y > ymax)
		y = ymax;
	if (y < ymin)
		y = ymin;

	return (XY_Cell(x,y));
}