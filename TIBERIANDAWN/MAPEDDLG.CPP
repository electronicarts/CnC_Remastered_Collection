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

/* $Header:   F:\projects\c&c\vcs\code\mapeddlg.cpv   2.18   16 Oct 1995 16:49:00   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : MAPEDDLG.CPP                             *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : November 18, 1994                        *
 *                                                                         *
 *                  Last Update : December 12, 1994   [BR]                 *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Map Editor dialogs & main menu options                                  *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   MapEditClass::New_Scenario -- creates a new scenario                  *
 *   MapEditClass::Load_Scenario -- loads a scenario INI file              *
 *   MapEditClass::Save_Scenario -- saves current scenario to an INI file  *
 *   MapEditClass::Pick_Scenario -- dialog for choosing scenario           *
 *   MapEditClass::Size_Map -- lets user set size & location of map        *
 *   MapEditClass::Scenario_Dialog -- scenario global parameters dialog    *
 *   MapEditClass::Handle_Triggers -- processes the trigger dialogs        *
 *   MapEditClass::Select_Trigger -- lets user select a trigger            *
 *   MapEditClass::Edit_Trigger -- lets user edit a [new] trigger          *
 *   MapEditClass::Import_Triggers -- lets user import triggers            *
 *   MapEditClass::Import_Teams -- lets user import teams                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

#ifdef SCENARIO_EDITOR


/***************************************************************************
 * MapEditClass::New_Scenario -- creates a new scenario                    *
 *                                                                         *
 * - Prompts user for scenario data (house, scenario #); sets globals      *
 *     PlayerPtr (for house) & Scenario (for scenario #)                   *
 *   - Prompts user for map size                                           *
 * - Initializes the scenario by calling Clear_Scenario(), which calls     *
 *     everybody's Init() routine                                          *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = new scenario created, -1 = not                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   10/21/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::New_Scenario(void)
{
	int scen_num = Scenario;
	ScenarioPlayerType player = ScenPlayer;
	ScenarioDirType dir = ScenDir;
	ScenarioVarType var = ScenVar;
	int rc;
	HousesType house;

	/*
	------------------------ Prompt for scenario info ------------------------
	*/
	rc = Pick_Scenario("New Scenario", &scen_num, &player, &dir, &var, 1);
	if (rc != 0) {
		return(-1);
	}

	/*
	-------------------------- Blow away everything --------------------------
	*/
	Clear_Scenario();

	/*
	----------------------------- Set parameters -----------------------------
	*/
	Scenario = scen_num;
	ScenPlayer = player;
	ScenDir = dir;
	ScenVar = var;
	Set_Scenario_Name(ScenarioName,scen_num,player,dir,var);

	/*
	----------------------------- Create houses ------------------------------
	*/
	for (house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
		new HouseClass(house);
	}

	if (ScenPlayer == SCEN_PLAYER_MPLAYER) {
		PlayerPtr = HouseClass::As_Pointer(HOUSE_MULTI1);
		PlayerPtr->IsHuman = true;
		LastHouse = HOUSE_MULTI1;
	} else {
		if (player == SCEN_PLAYER_GDI) {
			PlayerPtr = HouseClass::As_Pointer(HOUSE_GOOD);
			PlayerPtr->IsHuman = true;
			Base.House = HOUSE_BAD;
		} else {
			if (player == SCEN_PLAYER_NOD) {
				PlayerPtr = HouseClass::As_Pointer(HOUSE_BAD);
				PlayerPtr->IsHuman = true;
				Base.House = HOUSE_GOOD;
			} else {
				PlayerPtr = HouseClass::As_Pointer(HOUSE_MULTI4);
				PlayerPtr->IsHuman = true;
				Base.House = HOUSE_MULTI4;
			}
		}
		LastHouse = HOUSE_GOOD;
	}

	/*
	-------------------------- Init the entire map ---------------------------
	*/
	Init_Clear();
	Fill_In_Data();

	/*
	-------------------------- Prompt for map size ---------------------------
	*/
	Size_Map(-1,-1,20,20);

	/*
	------ Set the Home & Reinforcement Cells to the center of the map -------
	*/
	Waypoint[WAYPT_REINF] = XY_Cell(MapCellX + MapCellWidth / 2, MapCellY + MapCellHeight / 2);
	Waypoint[WAYPT_HOME] = XY_Cell(MapCellX + MapCellWidth / 2, MapCellY + MapCellHeight / 2);
	(*this)[Coord_Cell(TacticalCoord)].IsWaypoint = 1;
	Flag_Cell(Coord_Cell(TacticalCoord));

	ScenarioInit++;
	Set_Tactical_Position(Cell_Coord(Waypoint[WAYPT_HOME]));
	ScenarioInit--;

	return(0);
}


/***************************************************************************
 * MapEditClass::Load_Scenario -- loads a scenario INI file                *
 *                                                                         *
 * - Prompts user for scenario data (house, scenario #); sets globals      *
 *     PlayerPtr (for house) & Scenario (for scenario #)                   *
 * - Loads the INI file for that scenario                                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0.                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   10/21/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Load_Scenario(void)
{
	int scen_num = Scenario;
	ScenarioPlayerType player = ScenPlayer;
	ScenarioDirType dir = ScenDir;
	ScenarioVarType var = ScenVar;
	int rc;

	/*
	------------------------ Prompt for scenario info ------------------------
	*/
	rc = Pick_Scenario("Load Scenario", &scen_num, &player, &dir, &var, 1);
	if (rc != 0) {
		return(-1);
	}

	/*
	----------------------------- Set parameters -----------------------------
	*/
	Scenario = scen_num;
	ScenPlayer = player;
	ScenDir = dir;
	ScenVar = var;
	Set_Scenario_Name(ScenarioName,scen_num,player,dir,var);

	/*------------------------------------------------------------------------
	Read_Scenario_Ini() must be able to set PlayerPtr to the right house:
	- Reading the INI will create the house objects
	- PlayerPtr must be set before any Techno objects are created
	- For GDI or NOD scenarios, PlayerPtr is set by reading the INI;
	  but for multiplayer, it's set via the MPlayerLocalID; so, here we have
	  to set various multiplayer variables to fool the Assign_Houses() routine
	  into working properly.
	------------------------------------------------------------------------*/
	if (ScenPlayer == SCEN_PLAYER_MPLAYER) {
		MPlayerLocalID = Build_MPlayerID(2,HOUSE_GOOD);
		MPlayerCount = 1;
		LastHouse = HOUSE_MULTI1;
	}
	else if (ScenPlayer==SCEN_PLAYER_JP) {
		PlayerPtr = HouseClass::As_Pointer(HOUSE_MULTI4);
		PlayerPtr->IsHuman = true;
		Base.House = HOUSE_MULTI4;
	}
	else {
		LastHouse = HOUSE_GOOD;
	}

	/*
	-------------------------- Blow away everything --------------------------
	*/
	Clear_Scenario();

	/*
	------------------------------ Read the INI ------------------------------
	*/
	if (Read_Scenario_Ini(ScenarioName) == 0) {
		CCMessageBox().Process("Unable to read scenario!");
		HiddenPage.Clear();
		Flag_To_Redraw(true);
		Render();
	} else {
		Fill_In_Data();
		Set_Palette(GamePalette);
	}

	return(0);
}


/***************************************************************************
 * MapEditClass::Save_Scenario -- saves current scenario to an INI file    *
 *                                                                         *
 * - Prompts user for scenario data (house, scenario #); sets globals      *
 *     PlayerPtr (for house) & Scenario (for scenario #)                   *
 * - Saves the INI file for this scenario                                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, -1 = error/cancel                                          *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   10/21/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Save_Scenario(void)
{
	int scen_num = Scenario;
	ScenarioPlayerType player = ScenPlayer;
	ScenarioDirType dir = ScenDir;
	ScenarioVarType var = ScenVar;
	int rc;
	FILE *fp;
	char fname[13];

	/*
	------------------------ Prompt for scenario info ------------------------
	*/
	rc = Pick_Scenario("Save Scenario", &scen_num, &player, &dir, &var, 0);
	if (rc != 0) {
		return(-1);
	}

	/*
	------------------- Warning if scenario already exists -------------------
	*/
	Set_Scenario_Name(fname, scen_num, player, dir, var);
	fp = fopen(fname,"rb");
	if (fp) {
		fclose(fp);
		rc = CCMessageBox().Process("File exists. Replace?", TXT_YES, TXT_NO);
		HiddenPage.Clear();
		Flag_To_Redraw(true);
		Render();
		if (rc==1)
			return(-1);
	}

	/*
	----------------------------- Set parameters -----------------------------
	*/
	Scenario = scen_num;
	ScenPlayer = player;
	ScenDir = dir;
	ScenVar = var;
	Set_Scenario_Name(ScenarioName,scen_num,player,dir,var);

	/*------------------------------------------------------------------------
	Player may have changed from GDI to NOD, so change playerptr accordingly
	------------------------------------------------------------------------*/
	if (ScenPlayer == SCEN_PLAYER_GDI || ScenPlayer==SCEN_PLAYER_NOD) {
		if (ScenPlayer==SCEN_PLAYER_GDI) {
			PlayerPtr = HouseClass::As_Pointer(HOUSE_GOOD);
			PlayerPtr->IsHuman = true;
			Base.House = HOUSE_BAD;
		} else {
			if (ScenPlayer == SCEN_PLAYER_NOD) {
				PlayerPtr = HouseClass::As_Pointer(HOUSE_BAD);
				PlayerPtr->IsHuman = true;
				Base.House = HOUSE_GOOD;
			} else {
				PlayerPtr = HouseClass::As_Pointer(HOUSE_MULTI4);
				PlayerPtr->IsHuman = true;
				Base.House = HOUSE_MULTI4;
			}
		}
		LastHouse = HOUSE_GOOD;
	}

	/*
	----------------------------- Write the INI ------------------------------
	*/
	Write_Scenario_Ini(ScenarioName);

	return(0);
}


/***************************************************************************
 * MapEditClass::Pick_Scenario -- dialog for choosing scenario             *
 *                                                                         *
 * Prompts user for:                                                       *
 *   - House (GDI, NOD)                                                    *
 *   - Scenario #                                                          *
 *                                                                         *
 *           ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                          *
 *           ³            Caption               ³                          *
 *           ³                                  ³                          *
 *           ³         Scenario ___             ³                          *
 *           ³          Version ___             ³                          *
 *           ³                                  ³                          *
 *           ³         [East]  [West]           ³                          *
 *           ³                                  ³                          *
 *           ³         [    GDI     ]           ³                          *
 *           ³         [    NOD     ]           ³                          *
 *           ³         [Multi-Player]           ³                          *
 *           ³                                  ³                          *
 *           ³         [OK]  [Cancel]           ³                          *
 *           ³                                  ³                          *
 *           ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                          *
 *                                                                         *
 * INPUT:                                                                  *
 *      caption      string to use as a title                              *
 *      scen_nump   output: ptr to scenario #                              *
 *      playerp      output: ptr to player type                            *
 *      dirp         output: ptr to direction                              *
 *      varp         output: ptr to variation                              *
 *      multi         1 = allow to change single/multiplayer; 0 = not      *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, -1 = cancel                                                *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   10/21/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Pick_Scenario(char const * caption, int *scen_nump,
	ScenarioPlayerType *playerp, ScenarioDirType *dirp, ScenarioVarType *varp,
	int multi)
{
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	enum {
		D_DIALOG_W = 400,											// dialog width
		D_DIALOG_H = 328,											// dialog height
		D_DIALOG_X = ((640 - D_DIALOG_W) / 2),				// centered x-coord
		D_DIALOG_Y = ((400 - D_DIALOG_H) / 2),				// centered y-coord
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),		// coord of x-center

		D_TXT8_H = 22,												// ht of 8-pt text
		D_MARGIN = 14,												// margin width/height

		D_SCEN_W = 90,												// Scenario # width
		D_SCEN_H = 18,												// Scenario # height
		D_SCEN_X = D_DIALOG_CX + 5,							// Scenario # x
		D_SCEN_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H + D_MARGIN,	// Scenario # y

		D_VARA_W = 26,												// Version A width
		D_VARA_H = 18,												// Version A height
		D_VARA_X = D_DIALOG_CX - (D_VARA_W * 5) / 2,		// Version A x
		D_VARA_Y = D_SCEN_Y + D_SCEN_H + D_MARGIN,		// Version A y

		D_VARB_W = 26,												// Version B width
		D_VARB_H = 18,												// Version B height
		D_VARB_X = D_VARA_X + D_VARA_W,						// Version B x
		D_VARB_Y = D_SCEN_Y + D_SCEN_H + D_MARGIN,		// Version B y

		D_VARC_W = 26,												// Version C width
		D_VARC_H = 18,												// Version C height
		D_VARC_X = D_VARB_X + D_VARB_W,						// Version C x
		D_VARC_Y = D_SCEN_Y + D_SCEN_H + D_MARGIN,		// Version C y

		D_VARD_W = 26,												// Version D width
		D_VARD_H = 18,												// Version D height
		D_VARD_X = D_VARC_X + D_VARC_W,						// Version D x
		D_VARD_Y = D_SCEN_Y + D_SCEN_H + D_MARGIN,		// Version D y

		D_VARLOSE_W = 26,											// Version Lose width
		D_VARLOSE_H = 18,											// Version Lose height
		D_VARLOSE_X = D_VARD_X + D_VARD_W,					// Version Lose x
		D_VARLOSE_Y = D_SCEN_Y + D_SCEN_H + D_MARGIN,	// Version Lose y

		D_EAST_W = 100,												// EAST width
		D_EAST_H = 18,												// EAST height
		D_EAST_X = D_DIALOG_CX - D_EAST_W - 5,				// EAST x
		D_EAST_Y = D_VARLOSE_Y + D_VARLOSE_H + D_MARGIN,// EAST y

		D_WEST_W = 100,												// WEST width
		D_WEST_H = 18,												// WEST height
		D_WEST_X = D_DIALOG_CX + 5,							// WEST x
		D_WEST_Y = D_VARLOSE_Y + D_VARLOSE_H + D_MARGIN,// EAST y

		D_GDI_W = 140,												// GDI width
		D_GDI_H = 18,												// GDI height
		D_GDI_X = D_DIALOG_CX - (D_GDI_W / 2),				// GDI x
		D_GDI_Y = D_EAST_Y + D_EAST_H + D_MARGIN,			// GDI y

		D_NOD_W = 140,												// NOD width
		D_NOD_H = 18,												// NOD height
		D_NOD_X = D_DIALOG_CX - (D_NOD_W / 2),				// NOD x
		D_NOD_Y = D_GDI_Y + D_GDI_H,							// NOD y

		D_NEU_W = 140,												// Neutral width
		D_NEU_H = 18,												// Neutral height
		D_NEU_X = D_DIALOG_CX - (D_NOD_W / 2),				// Neutral x
		D_NEU_Y = D_NOD_Y + D_NOD_H,							// Neutral y

		D_MPLAYER_W = 140,											// Multi-Player width
		D_MPLAYER_H = 18,											// Multi-Player height
		D_MPLAYER_X = D_DIALOG_CX - (D_MPLAYER_W / 2),	// Multi-Player x
		D_MPLAYER_Y = D_NEU_Y + D_NEU_H,						// Multi-Player y

		D_OK_W = 90,												// OK width
		D_OK_H = 18,													// OK height
		D_OK_X = D_DIALOG_CX - D_OK_W - 5,					// OK x
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_OK_H - D_MARGIN,	// OK y

		D_CANCEL_W = 90,											// Cancel width
		D_CANCEL_H = 18,											// Cancel height
		D_CANCEL_X = D_DIALOG_CX + 5,							// Cancel x
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - D_CANCEL_H - D_MARGIN, // Cancel y

	};
	/*........................................................................
	Button enumerations
	........................................................................*/
	enum {
		BUTTON_GDI=100,
		BUTTON_NOD,
		BUTTON_NEUTRAL,
		BUTTON_MPLAYER,
		BUTTON_EAST,
		BUTTON_WEST,
		BUTTON_OK,
		BUTTON_CANCEL,
		BUTTON_SCENARIO,
		BUTTON_VAR_A,
		BUTTON_VAR_B,
		BUTTON_VAR_C,
		BUTTON_VAR_D,
		BUTTON_VAR_L,
	};
	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;
	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display;							// requested redraw level
	bool process;									// loop while true
	KeyNumType input;
	bool cancel = false;							// true = user cancels
	/*........................................................................
	Other Variables
	........................................................................*/
	char scen_buf[10]={0};						// buffer for editing scenario #
	/*........................................................................
	Buttons
	........................................................................*/
	ControlClass *commands = NULL;		// the button list

	EditClass editbtn (BUTTON_SCENARIO,
		scen_buf, 5,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_SCEN_X, D_SCEN_Y, D_SCEN_W, D_SCEN_H, EditClass::NUMERIC);

	TextButtonClass varabtn (BUTTON_VAR_A, "A",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_VARA_X, D_VARA_Y, D_VARA_W, D_VARA_H);

	TextButtonClass varbbtn (BUTTON_VAR_B, "B",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_VARB_X, D_VARB_Y, D_VARB_W, D_VARB_H);

	TextButtonClass varcbtn (BUTTON_VAR_C, "C",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_VARC_X, D_VARC_Y, D_VARC_W, D_VARC_H);

	TextButtonClass vardbtn (BUTTON_VAR_D, "D",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_VARD_X, D_VARD_Y, D_VARD_W, D_VARD_H);

	TextButtonClass varlbtn (BUTTON_VAR_L, "L",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_VARLOSE_X, D_VARLOSE_Y, D_VARLOSE_W, D_VARLOSE_H);

	TextButtonClass gdibtn (BUTTON_GDI, "GDI",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_GDI_X, D_GDI_Y, D_GDI_W, D_GDI_H);

	TextButtonClass nodbtn (BUTTON_NOD, "NOD",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NOD_X, D_NOD_Y, D_NOD_W, D_NOD_H);

	TextButtonClass playermbtn (BUTTON_MPLAYER, "Multi Player",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MPLAYER_X, D_MPLAYER_Y, D_MPLAYER_W, D_MPLAYER_H);

	TextButtonClass eastbtn (BUTTON_EAST, "East",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_EAST_X, D_EAST_Y, D_EAST_W, D_EAST_H);

	TextButtonClass westbtn (BUTTON_WEST, "West",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_WEST_X, D_WEST_Y, D_WEST_W, D_WEST_H);

	TextButtonClass okbtn (BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	TextButtonClass cancelbtn (BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);

	sprintf(scen_buf,"%d",(*scen_nump));		// init edit buffer
	editbtn.Set_Text(scen_buf,5);

	varabtn.Turn_Off();
	varbbtn.Turn_Off();
	varcbtn.Turn_Off();
	vardbtn.Turn_Off();
	varlbtn.Turn_Off();
	switch (*varp) {
		case SCEN_VAR_A:
			varabtn.Turn_On();
			break;

		case SCEN_VAR_B:
			varbbtn.Turn_On();
			break;

		case SCEN_VAR_C:
			varcbtn.Turn_On();
			break;

		case SCEN_VAR_D:
			vardbtn.Turn_On();
			break;

		case SCEN_VAR_LOSE:
			varlbtn.Turn_On();
			break;
	}

	/*
	......................... Create the button list .........................
	*/
	commands = &editbtn;
	varabtn.Add_Tail(*commands);
	varbbtn.Add_Tail(*commands);
	varcbtn.Add_Tail(*commands);
	vardbtn.Add_Tail(*commands);
	varlbtn.Add_Tail(*commands);
	if (multi) {
		gdibtn.Add_Tail(*commands);
		nodbtn.Add_Tail(*commands);
		playermbtn.Add_Tail(*commands);
	} else {
		if ((*playerp) == SCEN_PLAYER_MPLAYER) {
			playermbtn.Add_Tail(*commands);
		} else {
			gdibtn.Add_Tail(*commands);
			nodbtn.Add_Tail(*commands);
		}
	}
	eastbtn.Add_Tail(*commands);
	westbtn.Add_Tail(*commands);
	okbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);
	/*
	......................... Init the button states .........................
	*/
	if ((*playerp) == SCEN_PLAYER_GDI) {
		gdibtn.Turn_On();
		nodbtn.Turn_Off();
		playermbtn.Turn_Off();
	} else {
		if ((*playerp) == SCEN_PLAYER_NOD) {
			gdibtn.Turn_Off();
			nodbtn.Turn_On();
			playermbtn.Turn_Off();
		} else {
			gdibtn.Turn_Off();
			nodbtn.Turn_Off();
			playermbtn.Turn_On();
		}
	}

	if ((*dirp)==SCEN_DIR_EAST) {
		eastbtn.Turn_On();
		westbtn.Turn_Off();
	} else {
		eastbtn.Turn_Off();
		westbtn.Turn_On();
	}

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	display = REDRAW_ALL;
	process = true;
	while (process) {
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {

			/*
			...................... Display the dialog box ......................
			*/
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {
				Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
				Draw_Caption(TXT_NONE, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);

				/*
				....................... Draw the captions .......................
				*/
				Fancy_Text_Print(caption, D_DIALOG_CX, D_DIALOG_Y + D_MARGIN,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print("Scenario", D_DIALOG_CX - 5,
					D_SCEN_Y, CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
			}

			/*
			........................ Redraw the buttons ........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Draw_All();
			}
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		............................ Process input ............................
		*/
		switch (input) {
			case (BUTTON_VAR_A | KN_BUTTON):
				(*varp) = SCEN_VAR_A;
				varabtn.Turn_On();
				varbbtn.Turn_Off();
				varcbtn.Turn_Off();
				vardbtn.Turn_Off();
				varlbtn.Turn_Off();
				break;

			case (BUTTON_VAR_B | KN_BUTTON):
				(*varp) = SCEN_VAR_B;
				varabtn.Turn_Off();
				varbbtn.Turn_On();
				varcbtn.Turn_Off();
				vardbtn.Turn_Off();
				varlbtn.Turn_Off();
				break;

			case (BUTTON_VAR_C | KN_BUTTON):
				(*varp) = SCEN_VAR_C;
				varabtn.Turn_Off();
				varbbtn.Turn_Off();
				varcbtn.Turn_On();
				vardbtn.Turn_Off();
				varlbtn.Turn_Off();
				break;

			case (BUTTON_VAR_D | KN_BUTTON):
				(*varp) = SCEN_VAR_D;
				varabtn.Turn_Off();
				varbbtn.Turn_Off();
				varcbtn.Turn_Off();
				vardbtn.Turn_On();
				varlbtn.Turn_Off();
				break;

			case (BUTTON_VAR_L | KN_BUTTON):
				(*varp) = SCEN_VAR_LOSE;
				varabtn.Turn_Off();
				varbbtn.Turn_Off();
				varcbtn.Turn_Off();
				vardbtn.Turn_Off();
				varlbtn.Turn_On();
				break;

			case (BUTTON_EAST | KN_BUTTON):
				(*dirp) = SCEN_DIR_EAST;
				eastbtn.Turn_On();
				westbtn.Turn_Off();
				break;

			case (BUTTON_WEST | KN_BUTTON):
				(*dirp) = SCEN_DIR_WEST;
				eastbtn.Turn_Off();
				westbtn.Turn_On();
				break;

			case (BUTTON_GDI | KN_BUTTON):
				(*playerp) = SCEN_PLAYER_GDI;
				gdibtn.Turn_On();
				nodbtn.Turn_Off();
				playermbtn.Turn_Off();
				break;

			case (BUTTON_NOD | KN_BUTTON):
				(*playerp) = SCEN_PLAYER_NOD;
				gdibtn.Turn_Off();
				nodbtn.Turn_On();
				playermbtn.Turn_Off();
				break;

			case (BUTTON_MPLAYER | KN_BUTTON):
				(*playerp) = SCEN_PLAYER_MPLAYER;
				gdibtn.Turn_Off();
				nodbtn.Turn_Off();
				playermbtn.Turn_On();
				break;

			case (KN_RETURN):
			case (BUTTON_OK | KN_BUTTON):
				cancel = false;
				process = false;
				break;

			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				cancel = true;
				process = false;
				break;

			case (BUTTON_SCENARIO | KN_BUTTON):
				break;

			default:
				break;
		}
	}

	/*
	--------------------------- Redraw the display ---------------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();

	/*
	------------------------- If cancel, just return -------------------------
	*/
	if (cancel)
		return(-1);

	/*
	------------------------ Save selections & return ------------------------
	*/
	(*scen_nump) = atoi(scen_buf);

	return(0);
}


/***************************************************************************
 * MapEditClass::Size_Map -- lets user set size & location of map          *
 *                                                                         *
 * Lets the user select a side of the map and expand/shrink it to the      *
 * desired size, or move the whole map around the available map area.      *
 *                                                                         *
 * The entire available map area is displayed, but the map is limited such *
 * that there's always one blank cell around the map; this lets objects    *
 * properly exit the screen, since they have a blank undisplayed cell to   *
 * exit onto.                                                              *
 *                                                                         *
 *   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿               *
 *   ³                                                     ³               *
 *   ³  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿   Clear Terrain            ³               *
 *   ³  ³                     ³   Water                    ³               *
 *   ³  ³                     ³   Tiberium                 ³               *
 *   ³  ³                     ³   Rock/Wall/Road           ³               *
 *   ³  ³     (Map Area)      ³   GDI Unit                 ³               *
 *   ³  ³                     ³   NOD Unit                 ³               *
 *   ³  ³                     ³   Neutral Unit             ³               *
 *   ³  ³                     ³   Terrain Object           ³               *
 *   ³  ³                     ³   Starting Cell            ³               *
 *   ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                            ³               *
 *   ³                                                     ³               *
 *   ³      X            Y            Width      Height    ³               *
 *   ³      ##           ##            ##          ##      ³               *
 *   ³                                                     ³               *
 *   ³                                                     ³               *
 *   ³               [OK]            [Cancel]              ³               *
 *   ³                                                     ³               *
 *   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ               *
 *                                                                         *
 * INPUT:                                                                  *
 *      x,y,w,h:      initial size parameters (-1 = center the thing)      *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, -1 = cancel                                                *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   10/21/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Size_Map(int x, int y, int w, int h)
{
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	enum {
		D_DIALOG_W = 480,											// dialog width
		D_DIALOG_H = 280,											// dialog height
		D_DIALOG_X = ((640 - D_DIALOG_W) / 2),				// centered x-coord
		D_DIALOG_Y = ((400 - D_DIALOG_H) / 2),				// centered y-coord
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),		// coord of x-center

		D_TXT8_H = 22,												// ht of 8-pt text
		D_MARGIN = 14,												// margin width/height

		D_BORD_X1 = D_DIALOG_X + (D_DIALOG_W / 2 - MAP_CELL_W) / 2,
		D_BORD_Y1 = D_DIALOG_Y + 10,
		D_BORD_X2 = D_BORD_X1 + MAP_CELL_W + 1,
		D_BORD_Y2 = D_BORD_Y1 + MAP_CELL_H + 1,

		D_OK_W = 90,												// OK width
		D_OK_H = 18,												// OK height
		D_OK_X = D_DIALOG_CX - D_OK_W - 5,					// OK x
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_OK_H - D_MARGIN,	// OK y

		D_CANCEL_W = 90,											// Cancel width
		D_CANCEL_H = 18,											// Cancel height
		D_CANCEL_X = D_DIALOG_CX + 5,							// Cancel x
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - D_CANCEL_H - D_MARGIN, // Cancel y

	};
	/*........................................................................
	Button enumerations:
	........................................................................*/
	enum {
		BUTTON_OK=100,
		BUTTON_CANCEL,
	};
	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_MAP,				// includes map interior & coord values
		REDRAW_BACKGROUND,	// includes box, map bord, key, coord labels, btns
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;
	/*........................................................................
	Dialog variables:
	........................................................................*/
	bool process;									// Loop while true
	RedrawType display;							// requested redraw level
	bool cancel = false;							// true = user cancels
	KeyNumType input;								// user input
	int grabbed = 0;								// 1=TLeft,2=TRight,3=BRight,4=BLeft
	int map_x1;										// map coords x1, pixel coords
	int map_x2;										// map coords x2, pixel coords
	int map_y1;										// map coords y1, pixel coords
	int map_y2;										// map coords y2, pixel coords
	int delta1, delta2;							// mouse-click proximity
	int mx,my;										// last-saved mouse coords
	char txt[40];
	int txt_x,txt_y;								// for displaying text
	unsigned index;								// for drawing map symbology
	CELL cell;										// for drawing map symbology
	int color;										// for drawing map symbology
	ObjectClass * occupier;						// cell's occupier
	/*........................................................................
	Buttons
	........................................................................*/
	ControlClass *commands = NULL;

	TextButtonClass okbtn (BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	TextButtonClass cancelbtn (BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);

	/*........................................................................
	Set up the actual map area relative to the map's border coords
	........................................................................*/
	if (x==-1) {
		map_x1 = D_BORD_X1 + (MAP_CELL_W - w) / 2 + 1;
	} else {
		map_x1 = D_BORD_X1 + x + 1;
	}

	if (y==-1) {
		map_y1 = D_BORD_Y1 + (MAP_CELL_H - h) / 2 + 1;
	} else {
		map_y1 = D_BORD_Y1 + y + 1;
	}

	map_x2 = map_x1 + w - 1;
	map_y2 = map_y1 + h - 1;

	/*
	------------------------- Build the button list --------------------------
	*/
	commands = &okbtn;
	cancelbtn.Add_Tail(*commands);

	/*------------------------------------------------------------------------
	Main processing loop
	------------------------------------------------------------------------*/
	display = REDRAW_ALL;
	process = true;
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		------------------------ Invoke game callback -------------------------
		*/
		Call_Back();

		/*
		---------------------- Refresh display if needed ----------------------
		*/
		if (display) {
			Hide_Mouse();
			/*------------------------------------------------------------------
			Redraw the background, map border, key, and coord labels
			------------------------------------------------------------------*/
			if (display >= REDRAW_BACKGROUND) {
				/*
				.......................... Background ...........................
				*/
				Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
				Draw_Caption(TXT_NONE, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);

				/*
				..................... Draw the map border .......................
				*/
				LogicPage->Lock();
				LogicPage->Draw_Rect(D_BORD_X1, D_BORD_Y1, D_BORD_X2, D_BORD_Y2, CC_GREEN_SHADOW);
				for (index = D_BORD_X1; index < D_BORD_X2;
					index += (320/ICON_PIXEL_W)) {
					LogicPage->Put_Pixel(index, D_BORD_Y1-1, CC_GREEN_SHADOW);
					LogicPage->Put_Pixel(index, D_BORD_Y2+1, CC_GREEN_SHADOW);
				}
				for (index = D_BORD_Y1; index < D_BORD_Y2-8;
					index += (200/ICON_PIXEL_H)) {
					LogicPage->Put_Pixel(D_BORD_X1-1, index, CC_GREEN_SHADOW);
					LogicPage->Put_Pixel(D_BORD_X2+1, index, CC_GREEN_SHADOW);
				}

				/*...............................................................
				Draw the map "key"
				...............................................................*/
				txt_x = D_DIALOG_CX;
				txt_y = D_DIALOG_Y + 8;
				Fancy_Text_Print("Clear Terrain", txt_x, txt_y, LTGREY, TBLACK, TPF_DROPSHADOW | TPF_6POINT);
				txt_y += 16;
				Fancy_Text_Print("Water", txt_x, txt_y, BLUE, TBLACK, TPF_DROPSHADOW | TPF_6POINT);
				txt_y += 16;
				Fancy_Text_Print("Tiberium", txt_x, txt_y, GREY, TBLACK, TPF_DROPSHADOW | TPF_6POINT);
				txt_y += 16;
				Fancy_Text_Print("Rock/Wall/Road", txt_x, txt_y, BROWN, TBLACK, TPF_DROPSHADOW | TPF_6POINT);
				txt_y += 16;
				Fancy_Text_Print("GDI Unit", txt_x, txt_y, YELLOW, TBLACK, TPF_DROPSHADOW | TPF_6POINT);
				txt_y += 16;
				Fancy_Text_Print("Nod Unit", txt_x, txt_y, RED, TBLACK, TPF_DROPSHADOW | TPF_6POINT);
				txt_y += 16;
				Fancy_Text_Print("Neutral Unit", txt_x, txt_y, PURPLE, TBLACK, TPF_DROPSHADOW | TPF_6POINT);
				txt_y += 16;
				Fancy_Text_Print("Terrain Object", txt_x, txt_y, DKGREEN, TBLACK, TPF_DROPSHADOW | TPF_6POINT);
				txt_y += 16;
				Fancy_Text_Print("Starting Cell", txt_x, txt_y, WHITE, TBLACK, TPF_DROPSHADOW | TPF_6POINT);
				/*
				.................. Draw the coordinate labels ...................
				*/
				txt_x = D_DIALOG_X + D_DIALOG_W / 8;
				txt_y = D_DIALOG_Y + D_DIALOG_H - D_OK_H - 10 - 33;
				Fancy_Text_Print("X", txt_x, txt_y, CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				txt_x += (D_DIALOG_W - 20) / 4;
				Fancy_Text_Print("Y", txt_x, txt_y, CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				txt_x += (D_DIALOG_W - 20) / 4;
				Fancy_Text_Print("Width", txt_x, txt_y, CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				txt_x += (D_DIALOG_W - 20) / 4;
				Fancy_Text_Print("Height", txt_x, txt_y, CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				LogicPage->Unlock();

				/*
				...................... Redraw the buttons .......................
				*/
				commands->Flag_List_To_Redraw();

			}

			/*------------------------------------------------------------------
			Redraw the map symbology & location
			------------------------------------------------------------------*/
			if (display >= REDRAW_MAP) {

				LogicPage->Lock();

				/*
				.................... Erase the map interior .....................
				*/
				LogicPage->Fill_Rect(D_BORD_X1 + 1, D_BORD_Y1 + 1, D_BORD_X2 - 1, D_BORD_Y2 - 1, BLACK);

				/*...............................................................
				Draw Land map symbols (use color according to Ground[] array).
				...............................................................*/
				for (cell=0; cell < MAP_CELL_TOTAL; cell++) {
					occupier = (*this)[cell].Cell_Occupier();
					if (occupier == NULL) {
						color = Ground[(*this)[cell].Land_Type()].Color;
						LogicPage->Put_Pixel(D_BORD_X1 + Cell_X(cell) + 1, D_BORD_Y1 + Cell_Y(cell) + 1, color);
					}
				}

				LogicPage->Unlock();

				/*
				................. Draw the actual map location ..................
				*/
				LogicPage->Draw_Rect(map_x1, map_y1, map_x2, map_y2, WHITE);
				switch (grabbed) {
					case 1:
						LogicPage->Draw_Line(map_x1, map_y1, map_x1 + 5, map_y1, BLUE);
						LogicPage->Draw_Line(map_x1, map_y1, map_x1, map_y1 + 5, BLUE);
						break;

					case 2:
						LogicPage->Draw_Line(map_x2, map_y1, map_x2 - 5, map_y1, BLUE);
						LogicPage->Draw_Line(map_x2, map_y1, map_x2, map_y1 + 5, BLUE);
						break;

					case 3:
						LogicPage->Draw_Line(map_x2, map_y2, map_x2 - 5, map_y2, BLUE);
						LogicPage->Draw_Line(map_x2, map_y2, map_x2, map_y2 - 5, BLUE);
						break;

					case 4:
						LogicPage->Draw_Line(map_x1, map_y2, map_x1 + 5, map_y2, BLUE);
						LogicPage->Draw_Line(map_x1, map_y2, map_x1, map_y2 - 5, BLUE);
						break;

					case 5:
						LogicPage->Draw_Rect(map_x1, map_y1, map_x2, map_y2, BLUE);
						break;

					default:
						break;
				}

				/*...............................................................
				Draw Unit map symbols (Use the radar map color according to
				that specified in the house type class object.
				DKGREEN = terrain object
				...............................................................*/
				for (cell=0; cell < MAP_CELL_TOTAL; cell++) {
					occupier = (*this)[cell].Cell_Occupier();
					if (occupier) {
						color = DKGREEN;
						if (occupier && occupier->Owner() != HOUSE_NONE) {
							color = HouseClass::As_Pointer(occupier->Owner())->Class->Color;
						}
						LogicPage->Put_Pixel(D_BORD_X1 + Cell_X(cell) + 1, D_BORD_Y1 + Cell_Y(cell) + 1, color);
					}
				}

				/*
				...................... Draw Home location .......................
				*/
				LogicPage->Put_Pixel(D_BORD_X1 + Cell_X(Waypoint[WAYPT_HOME]) + 1, D_BORD_Y1 + Cell_Y(Waypoint[WAYPT_HOME]) + 1, WHITE);

				/*
				..................... Erase old coordinates .....................
				*/
				LogicPage->Fill_Rect( D_DIALOG_X + 7,
					D_DIALOG_Y + D_DIALOG_H - D_OK_H - 10 - 22,
					D_DIALOG_X + D_DIALOG_W - 7,
					D_DIALOG_Y + D_DIALOG_H - D_OK_H - 10 - 22 + 10, BLACK);

				/*
				..................... Draw the coordinates ......................
				*/
				txt_x = D_DIALOG_X + D_DIALOG_W / 8;
				txt_y = D_DIALOG_Y + D_DIALOG_H - D_OK_H - 10 - 22;
				sprintf(txt,"%d",map_x1 - D_BORD_X1 - 1);
				Fancy_Text_Print(txt, txt_x, txt_y, CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				txt_x += (D_DIALOG_W - 20) / 4;
				sprintf(txt,"%d",map_y1 - D_BORD_Y1 - 1);
				Fancy_Text_Print(txt, txt_x, txt_y, CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				txt_x += (D_DIALOG_W - 20) / 4;
				sprintf(txt,"%d",map_x2 - map_x1 + 1);
				Fancy_Text_Print(txt, txt_x, txt_y, CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				txt_x += (D_DIALOG_W - 20) / 4;
				sprintf(txt,"%d",map_y2 - map_y1 + 1);
				Fancy_Text_Print(txt, txt_x, txt_y, CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
			}

			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		------------------------- Process user input --------------------------
		*/
		input = commands->Input();
		/*.....................................................................
		Normal button processing: This is done when the mouse button is NOT
		being held down ('grabbed' is 0).
		.....................................................................*/
		if (grabbed == 0) {
			switch (input) {
				case (KN_RETURN):
				case (BUTTON_OK | KN_BUTTON):
					cancel = false;
					process = false;
					break;

				case (KN_ESC):
				case (BUTTON_CANCEL | KN_BUTTON):
					cancel = true;
					process = false;
					break;

				case KN_LMOUSE:
					/*
					....................... Grab top left ........................
					*/
					delta1 =  abs(_Kbd->MouseQX - map_x1);
					delta2 =  abs(_Kbd->MouseQY - map_y1);
					if (delta1 < 3 && delta2 < 3) {
						grabbed = 1;
						mx = _Kbd->MouseQX;
						my = _Kbd->MouseQY;
						display = REDRAW_MAP;
						break;
					}
					/*
					...................... Grab top right ........................
					*/
					delta1 =  abs(_Kbd->MouseQX - map_x2);
					delta2 =  abs(_Kbd->MouseQY - map_y1);
					if (delta1 < 3 && delta2 < 3) {
						grabbed = 2;
						mx = _Kbd->MouseQX;
						my = _Kbd->MouseQY;
						display = REDRAW_MAP;
						break;
					}
					/*
					..................... Grab bottom right ......................
					*/
					delta1 =  abs(_Kbd->MouseQX - map_x2);
					delta2 =  abs(_Kbd->MouseQY - map_y2);
					if (delta1 < 3 && delta2 < 3) {
						grabbed = 3;
						mx = _Kbd->MouseQX;
						my = _Kbd->MouseQY;
						display = REDRAW_MAP;
						break;
					}
					/*
					..................... Grab bottom left .......................
					*/
					delta1 =  abs(_Kbd->MouseQX - map_x1);
					delta2 =  abs(_Kbd->MouseQY - map_y2);
					if (delta1 < 3 && delta2 < 3) {
						grabbed = 4;
						mx = _Kbd->MouseQX;
						my = _Kbd->MouseQY;
						display = REDRAW_MAP;
						break;
					}
					/*
					..................... Grab the whole map .....................
					*/
					delta1 = abs(_Kbd->MouseQX - ((map_x1 + map_x2) / 2));
					delta2 = abs(_Kbd->MouseQY - ((map_y1 + map_y2) / 2));
					if (delta1 < (map_x2 - map_x1) / 4 &&
						delta2 < (map_y2 - map_y1) / 4) {
						grabbed = 5;
						mx = _Kbd->MouseQX;
						my = _Kbd->MouseQY;
						display = REDRAW_MAP;
					}
					break;

				default:
					break;
			}
		} else {
			/*.....................................................................
			Mouse motion processing: This is done while the left mouse button IS
			being held down.
			- First, check for the button release; if detected, un-grab
			- Then, handle mouse motion. WWLIB doesn't pass through a KN_MOUSE_MOVE
			  value while the button is being held down, so this case must be
			  trapped as a default.
			.....................................................................*/
			switch (input) {
				case ((int)KN_LMOUSE | (int)KN_RLSE_BIT):
					grabbed = 0;
					display = REDRAW_MAP;
					break;

				default:
					delta1 = Get_Mouse_X() - mx;
					delta2 = Get_Mouse_Y() - my;
					if (delta1==0 && delta2==0) {
						break;
					}

					/*
					....................... Move top left ........................
					*/
					if (grabbed==1) {
						map_x1 += delta1;
						if (map_x1 > map_x2 - 2) {
							map_x1 = map_x2 - 2;
						} else {
							if (map_x1 < D_BORD_X1 + 2) {
								map_x1 = D_BORD_X1 + 2;
							}
						}

						map_y1 += delta2;
						if (map_y1 > map_y2 - 2) {
							map_y1 = map_y2 - 2;
						} else {
							if (map_y1 < D_BORD_Y1 + 2) {
								map_y1 = D_BORD_Y1 + 2;
							}
						}
						display = REDRAW_MAP;
						mx = Get_Mouse_X();
						my = Get_Mouse_Y();
					}

					/*
					....................... Move top right .......................
					*/
					if (grabbed==2) {
						map_x2 += delta1;
						if (map_x2 < map_x1 + 2) {
							map_x2 = map_x1 + 2;
						} else {
							if (map_x2 > D_BORD_X2 - 2) {
								map_x2 = D_BORD_X2 - 2;
							}
						}

						map_y1 += delta2;
						if (map_y1 > map_y2 - 2) {
							map_y1 = map_y2 - 2;
						} else {
							if (map_y1 < D_BORD_Y1 + 2) {
								map_y1 = D_BORD_Y1 + 2;
							}
						}
						display = REDRAW_MAP;
						mx = Get_Mouse_X();
						my = Get_Mouse_Y();
					}

					/*
					..................... Move bottom right ......................
					*/
					if (grabbed==3) {
						map_x2 += delta1;
						if (map_x2 < map_x1 + 2) {
							map_x2 = map_x1 + 2;
						} else {
							if (map_x2 > D_BORD_X2 - 2) {
								map_x2 = D_BORD_X2 - 2;
							}
						}

						map_y2 += delta2;
						if (map_y2 < map_y1 + 2) {
							map_y2 = map_y1 + 2;
						} else {
							if (map_y2 > D_BORD_Y2 - 2) {
								map_y2 = D_BORD_Y2 - 2;
							}
						}
						display = REDRAW_MAP;
						mx = Get_Mouse_X();
						my = Get_Mouse_Y();
					}

					/*
					...................... Move bottom left ......................
					*/
					if (grabbed==4) {
						map_x1 += delta1;
						if (map_x1 > map_x2 - 2) {
							map_x1 = map_x2 - 2;
						} else {
							if (map_x1 < D_BORD_X1 + 2) {
								map_x1 = D_BORD_X1 + 2;
							}
						}

						map_y2 += delta2;
						if (map_y2 < map_y1 + 2) {
							map_y2 = map_y1 + 2;
						} else {
							if (map_y2 > D_BORD_Y2 - 2) {
								map_y2 = D_BORD_Y2 - 2;
							}
						}
						display = REDRAW_MAP;
						mx = Get_Mouse_X();
						my = Get_Mouse_Y();
					}

					/*
					....................... Move whole map .......................
					*/
					if (grabbed==5) {
						if (map_x1 + delta1 > D_BORD_X1 + 1 && map_x2 + delta1 < D_BORD_X2 - 1) {
							map_x1 += delta1;
							map_x2 += delta1;
						}

						if (map_y1 + delta2 > D_BORD_Y1 + 1 && map_y2 + delta2 < D_BORD_Y2 - 1) {
							map_y1 += delta2;
							map_y2 += delta2;
						}
						display = REDRAW_MAP;
						mx = Get_Mouse_X();
						my = Get_Mouse_Y();
					}
					break;
			}
		}
	}

	/*
	--------------------------- Redraw the display ---------------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();

	/*
	------------------------- If cancel, just return -------------------------
	*/
	if (cancel) {
		return(-1);
	}

	/*
	---------------------------- Save selections -----------------------------
	*/
	MapCellX = map_x1 - D_BORD_X1 - 1;
	MapCellY = map_y1 - D_BORD_Y1 - 1;
	MapCellWidth = map_x2 - map_x1 + 1;
	MapCellHeight = map_y2 - map_y1 + 1;

	/*
	--------------------- Clip Home Cell to new map size ---------------------
	*/
	if (Cell_X(Waypoint[WAYPT_HOME]) < MapCellX) {
		Waypoint[WAYPT_HOME] = XY_Cell(MapCellX, Cell_Y(Waypoint[WAYPT_HOME]));
	}

	if (Cell_X(Waypoint[WAYPT_HOME]) > MapCellX + MapCellWidth - 1) {
		Waypoint[WAYPT_HOME] = XY_Cell(MapCellX + MapCellWidth - 1, Cell_Y(Waypoint[WAYPT_HOME]));
	}

	if (Cell_Y(Waypoint[WAYPT_HOME]) < MapCellY) {
		Waypoint[WAYPT_HOME] = XY_Cell(Cell_X(Waypoint[WAYPT_HOME]), MapCellY);
	}

	if (Cell_Y(Waypoint[WAYPT_HOME]) > MapCellY + MapCellHeight - 1) {
		Waypoint[WAYPT_HOME] = XY_Cell(Cell_X(Waypoint[WAYPT_HOME]), MapCellY + MapCellHeight - 1);
	}

	return(0);
}


/***************************************************************************
 * MapEditClass::Scenario_Dialog -- scenario global parameters dialog      *
 *                                                                         *
 * Lets the user edit the Theater, starting credits for houses, and the    *
 * Edge for HOUSE_GOOD & HOUSE_BAD.                                        *
 *                                                                         *
 * ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿ *
 * ³       Theater                                Credits / 1000         ³ *
 * ³  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                                                  ³ *
 * ³  ³ Temperate     ³                          GDI: _____              ³ *
 * ³  ³ Desert        ³                          NOD: _____              ³ *
 * ³  ³ Jungle        ³                      Neutral: _____              ³ *
 * ³  ³               ³                                                  ³ *
 * ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                                                  ³ *
 * ³   Build Level:___                                                   ³ *
 * ³                                                                     ³ *
 * ³                         Reinforcements                              ³ *
 * ³                                                                     ³ *
 * ³                    GDI                NOD                           ³ *
 * ³                                                                     ³ *
 * ³                                                                   ³ *
 * ³                  <-   ->            <-   ->                         ³ *
 * ³                                                                   ³ *
 * ³                                                                     ³ *
 * ³                                                                     ³ *
 * ³                       [OK]     [Cancel]                             ³ *
 * ³                                                                     ³ *
 * ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, -1 = cancel                                                *
 *                                                                         *
 * WARNINGS:                                                               *
 *      Uses HIDBUFF.                                                      *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/14/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Scenario_Dialog(void)
{
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	enum {
		D_DIALOG_W = 544,
		D_DIALOG_H = 320,
		D_DIALOG_X = ((640 - D_DIALOG_W) / 2),
		D_DIALOG_Y = ((400 - D_DIALOG_H) / 2),
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),

		D_TXT8_H = 22,
		D_MARGIN = 14,

		D_THEATER_W = 200,
		D_THEATER_H = 68,
		D_THEATER_X = D_DIALOG_X + D_MARGIN,
		D_THEATER_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H,

		D_LEVEL_W = 80,
		D_LEVEL_H = 18,
		D_LEVEL_X = D_THEATER_X + D_THEATER_W - D_LEVEL_W,
		D_LEVEL_Y = D_THEATER_Y + D_THEATER_H + D_MARGIN,

		D_GDICRED_W = 120,
		D_GDICRED_H = 18,
		D_GDICRED_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_GDICRED_W,
		D_GDICRED_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H,

		D_NODCRED_W = 120,
		D_NODCRED_H = 18,
		D_NODCRED_X = D_GDICRED_X,
		D_NODCRED_Y = D_GDICRED_Y + D_GDICRED_H,

		D_NEUTCRED_W = 120,
		D_NEUTCRED_H = 18,
		D_NEUTCRED_X = D_GDICRED_X,
		D_NEUTCRED_Y = D_NODCRED_Y + D_NODCRED_H,

		D_GDIN_W = 26,
		D_GDIN_H = 18,
		D_GDIN_X = D_DIALOG_CX - 5 - D_GDIN_W * 2,
		D_GDIN_Y = D_LEVEL_Y + D_LEVEL_H + D_MARGIN + D_TXT8_H + D_MARGIN + D_TXT8_H,

		D_GDIS_W = 26,
		D_GDIS_H = 18,
		D_GDIS_X = D_GDIN_X,
		D_GDIS_Y = D_GDIN_Y + D_GDIN_H * 2,

		D_GDIW_W = 26,
		D_GDIW_H = 18,
		D_GDIW_X = D_DIALOG_CX - 5 - D_GDIN_W * 3,
		D_GDIW_Y = D_GDIN_Y + D_GDIN_H,

		D_GDIE_W = 26,
		D_GDIE_H = 18,
		D_GDIE_X = D_DIALOG_CX - 5 - D_GDIN_W,
		D_GDIE_Y = D_GDIN_Y + D_GDIN_H,

		D_NODN_W = 26,
		D_NODN_H = 18,
		D_NODN_X = D_DIALOG_CX + 5 + D_NODN_W,
		D_NODN_Y = D_LEVEL_Y + D_LEVEL_H + D_MARGIN + D_TXT8_H + D_MARGIN + D_TXT8_H,

		D_NODS_W = 26,
		D_NODS_H = 18,
		D_NODS_X = D_NODN_X,
		D_NODS_Y = D_NODN_Y + D_NODN_H * 2,

		D_NODW_W = 26,
		D_NODW_H = 18,
		D_NODW_X = D_DIALOG_CX + 5,
		D_NODW_Y = D_NODN_Y + D_NODN_H,

		D_NODE_W = 26,
		D_NODE_H = 18,
		D_NODE_X = D_DIALOG_CX + 5 + D_NODN_W * 2,
		D_NODE_Y = D_NODN_Y + D_NODN_H,

		D_OK_W = 90,
		D_OK_H = 18,
		D_OK_X = D_DIALOG_CX - D_OK_W - 5,
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_OK_H - D_MARGIN,

		D_CANCEL_W = 90,
		D_CANCEL_H = 18,
		D_CANCEL_X = D_DIALOG_CX + 5,
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - D_CANCEL_H - D_MARGIN,
	};
	/*........................................................................
	Button enumerations:
	........................................................................*/
	enum {
		LIST_THEATER=100,
		TEDIT_LEVEL,
		TEDIT_GDICRED,
		TEDIT_NODCRED,
		TEDIT_NEUTCRED,
		BUTTON_GDI_N,
		BUTTON_GDI_E,
		BUTTON_GDI_S,
		BUTTON_GDI_W,
		BUTTON_NOD_N,
		BUTTON_NOD_E,
		BUTTON_NOD_S,
		BUTTON_NOD_W,
		BUTTON_OK,
		BUTTON_CANCEL,
	};
	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,		// includes map interior & coord values
		REDRAW_BACKGROUND,	// includes box, map bord, key, coord labels, btns
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;
	/*........................................................................
	Dialog variables:
	........................................................................*/
	KeyNumType input;								// input from user
	bool process;									// loop while true
	RedrawType display;							// true = re-draw everything
	bool cancel = false;							// true = user cancels
	/*
	.......................... Scenario parameters ...........................
	*/
	TheaterType theater;				// DisplayClass::Theater
	TheaterType orig_theater;		// original theater
	long gdi_credits;					// HouseClass::As_Pointer(HouseType)->Credits
	long nod_credits;					// HouseClass::As_Pointer(HouseType)->Credits
	long neut_credits;				// HouseClass::As_Pointer(HouseType)->Credits
	SourceType gdi_edge;				// HouseClass::As_Pointer(HouseType)->Edge
	SourceType nod_edge;				// HouseClass::As_Pointer(HouseType)->Edge
	char level_buf[10] = {0};
	char gdicred_buf[10] = {0};
	char nodcred_buf[10] = {0};
	char neutcred_buf[10] = {0};
	/*
	....................... Theater-changing variables .......................
	*/
	unsigned char theater_mask;	// template/terrain mask
	TerrainClass * terrain;	// cell's terrain pointer
	CELL i;								// loop counter
	/*........................................................................
	Buttons
	........................................................................*/
	ControlClass *commands = NULL;				// the button list
	ListClass theaterbtn (LIST_THEATER,
		D_THEATER_X, D_THEATER_Y, D_THEATER_W, D_THEATER_H,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		Hires_Retrieve("BTN-UP.SHP"),
		Hires_Retrieve("BTN-DN.SHP"));

	EditClass leveledt (TEDIT_GDICRED, level_buf, 4,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_LEVEL_X, D_LEVEL_Y, D_LEVEL_W, D_LEVEL_H, EditClass::NUMERIC);

	EditClass gdicred (TEDIT_GDICRED, gdicred_buf, 8,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_GDICRED_X, D_GDICRED_Y, D_GDICRED_W, D_GDICRED_H, EditClass::NUMERIC);

	EditClass nodcred (TEDIT_NODCRED, nodcred_buf, 8,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NODCRED_X, D_NODCRED_Y, D_NODCRED_W, D_NODCRED_H, EditClass::NUMERIC);

	EditClass neutcred (TEDIT_NEUTCRED, neutcred_buf, 8,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NEUTCRED_X, D_NEUTCRED_Y, D_NEUTCRED_W, D_NEUTCRED_H, EditClass::NUMERIC);

	TextButtonClass gdinbtn (BUTTON_GDI_N, TXT_UP,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_GDIN_X, D_GDIN_Y, D_GDIN_W, D_GDIN_H);

	TextButtonClass gdiebtn (BUTTON_GDI_E, TXT_RIGHT,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_GDIE_X, D_GDIE_Y, D_GDIE_W, D_GDIE_H);

	TextButtonClass gdisbtn (BUTTON_GDI_S, TXT_DOWN,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_GDIS_X, D_GDIS_Y, D_GDIS_W, D_GDIS_H);

	TextButtonClass gdiwbtn (BUTTON_GDI_W, TXT_LEFT,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_GDIW_X, D_GDIW_Y, D_GDIW_W, D_GDIW_H);

	TextButtonClass nodnbtn (BUTTON_NOD_N, TXT_UP,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NODN_X, D_NODN_Y, D_NODN_W, D_NODN_H);

	TextButtonClass nodebtn (BUTTON_NOD_E, TXT_RIGHT,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NODE_X, D_NODE_Y, D_NODE_W, D_NODE_H);

	TextButtonClass nodsbtn (BUTTON_NOD_S, TXT_DOWN,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NODS_X, D_NODS_Y, D_NODS_W, D_NODS_H);

	TextButtonClass nodwbtn (BUTTON_NOD_W, TXT_LEFT,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NODW_X, D_NODW_Y, D_NODW_W, D_NODW_H);

	TextButtonClass okbtn (BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	TextButtonClass cancelbtn (BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);


	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);

	/*
	.......................... Fill in theater items .........................
	*/
	theaterbtn.Add_Item("Desert");
	theaterbtn.Add_Item("Jungle");
	theaterbtn.Add_Item("Temperate");
	theaterbtn.Add_Item("Winter");

	/*
	............................ Init parameters .............................
	*/
	orig_theater = theater = Theater;
	if (ScenPlayer != SCEN_PLAYER_MPLAYER) {
		gdi_credits = HouseClass::As_Pointer(HOUSE_GOOD)->Credits / 1000L;
		nod_credits = HouseClass::As_Pointer(HOUSE_BAD)->Credits / 1000L;
		neut_credits = HouseClass::As_Pointer(HOUSE_NEUTRAL)->Credits / 1000L;
		gdi_edge = HouseClass::As_Pointer(HOUSE_GOOD)->Edge;
		nod_edge = HouseClass::As_Pointer(HOUSE_BAD)->Edge;
	} else {
		gdi_credits = 0;
		nod_credits = 0;
		neut_credits = 0;
		gdi_edge = SOURCE_NONE;
		nod_edge = SOURCE_NONE;
	}

	/*
	............................ Create the list .............................
	*/
	commands = &theaterbtn;
	leveledt.Add_Tail(*commands);
	gdicred.Add_Tail(*commands);
	nodcred.Add_Tail(*commands);
	neutcred.Add_Tail(*commands);
	gdinbtn.Add_Tail(*commands);
	gdiebtn.Add_Tail(*commands);
	gdisbtn.Add_Tail(*commands);
	gdiwbtn.Add_Tail(*commands);
	nodnbtn.Add_Tail(*commands);
	nodebtn.Add_Tail(*commands);
	nodsbtn.Add_Tail(*commands);
	nodwbtn.Add_Tail(*commands);
	okbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);

	/*
	...................... Init GDI Edge button states .......................
	*/
	if (gdi_edge==SOURCE_NORTH) gdinbtn.Turn_On();
	if (gdi_edge==SOURCE_EAST) gdiebtn.Turn_On();
	if (gdi_edge==SOURCE_SOUTH) gdisbtn.Turn_On();
	if (gdi_edge==SOURCE_WEST) gdiwbtn.Turn_On();

	/*
	...................... Init NOD Edge button states .......................
	*/
	if (nod_edge==SOURCE_NORTH) nodnbtn.Turn_On();
	if (nod_edge==SOURCE_EAST) nodebtn.Turn_On();
	if (nod_edge==SOURCE_SOUTH) nodsbtn.Turn_On();
	if (nod_edge==SOURCE_WEST) nodwbtn.Turn_On();

	/*
	.......................... Init credits buffers ..........................
	*/
	sprintf(level_buf,"%ld",BuildLevel);
	leveledt.Set_Text(level_buf,4);

	sprintf(gdicred_buf,"%ld",gdi_credits);
	gdicred.Set_Text(gdicred_buf,8);

	sprintf(nodcred_buf,"%ld",nod_credits);
	nodcred.Set_Text(nodcred_buf,8);

	sprintf(neutcred_buf,"%ld",neut_credits);
	neutcred.Set_Text(neutcred_buf,8);

	theaterbtn.Set_Selected_Index(orig_theater - THEATER_NONE - 1);

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	display = REDRAW_ALL;
	process = true;
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {

				/*
				..................... Draw the background .......................
				*/
				Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
				Draw_Caption(TXT_NONE, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);

				/*
				....................... Draw the labels .........................
				*/
				Fancy_Text_Print("Theater", D_THEATER_X + D_THEATER_W / 2,
					D_THEATER_Y - D_TXT8_H, CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print("Build Level", D_LEVEL_X, D_LEVEL_Y,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print("Credits/1000", D_GDICRED_X + D_GDICRED_W / 2,
					D_GDICRED_Y - D_TXT8_H, CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print ("GDI", D_GDICRED_X - 5, D_GDICRED_Y,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print ("NOD", D_NODCRED_X - 5, D_NODCRED_Y,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print ("Neutral", D_NEUTCRED_X - 5, D_NEUTCRED_Y,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print ("Reinforcements", D_DIALOG_CX,
					D_LEVEL_Y + D_LEVEL_H + D_MARGIN,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print ("GDI", D_GDIN_X + D_GDIN_W / 2,
					D_GDIN_Y - D_TXT8_H,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print ("NOD", D_NODN_X + D_NODN_W / 2,
					D_NODN_Y - D_TXT8_H,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

			}
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		............................ Process input ............................
		*/
		switch (input) {

			/*..................................................................
			Credit edit boxes: no need for any action
			..................................................................*/
			case (TEDIT_GDICRED | KN_BUTTON):
				break;

			case (TEDIT_NODCRED | KN_BUTTON):
				break;

			case (TEDIT_NEUTCRED | KN_BUTTON):
				break;

			/*..................................................................
			GDI Edge buttons: turn this one on, others off, save the edge value
			..................................................................*/
			case (BUTTON_GDI_N | KN_BUTTON):
				gdi_edge = SOURCE_NORTH;
				gdinbtn.Turn_On();
				gdiebtn.Turn_Off();
				gdisbtn.Turn_Off();
				gdiwbtn.Turn_Off();
				break;

			case (BUTTON_GDI_E | KN_BUTTON):
				gdi_edge = SOURCE_EAST;
				gdinbtn.Turn_Off();
				gdiebtn.Turn_On();
				gdisbtn.Turn_Off();
				gdiwbtn.Turn_Off();
				break;

			case (BUTTON_GDI_S | KN_BUTTON):
				gdi_edge = SOURCE_SOUTH;
				gdinbtn.Turn_Off();
				gdiebtn.Turn_Off();
				gdisbtn.Turn_On();
				gdiwbtn.Turn_Off();
				break;

			case (BUTTON_GDI_W | KN_BUTTON):
				gdi_edge = SOURCE_WEST;
				gdinbtn.Turn_Off();
				gdiebtn.Turn_Off();
				gdisbtn.Turn_Off();
				gdiwbtn.Turn_On();
				break;

			/*..................................................................
			NOD Edge buttons: turn this one on, others off, save the edge value
			..................................................................*/
			case (BUTTON_NOD_N | KN_BUTTON):
				nod_edge = SOURCE_NORTH;
				nodnbtn.Turn_On();
				nodebtn.Turn_Off();
				nodsbtn.Turn_Off();
				nodwbtn.Turn_Off();
				break;

			case (BUTTON_NOD_E | KN_BUTTON):
				nod_edge = SOURCE_EAST;
				nodnbtn.Turn_Off();
				nodebtn.Turn_On();
				nodsbtn.Turn_Off();
				nodwbtn.Turn_Off();
				break;

			case (BUTTON_NOD_S | KN_BUTTON):
				nod_edge = SOURCE_SOUTH;
				nodnbtn.Turn_Off();
				nodebtn.Turn_Off();
				nodsbtn.Turn_On();
				nodwbtn.Turn_Off();
				break;

			case (BUTTON_NOD_W | KN_BUTTON):
				nod_edge = SOURCE_WEST;
				nodnbtn.Turn_Off();
				nodebtn.Turn_Off();
				nodsbtn.Turn_Off();
				nodwbtn.Turn_On();
				break;

			case (KN_RETURN):
			case (BUTTON_OK | KN_BUTTON):
				cancel = false;
				process = false;
				break;

			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				cancel = true;
				process = false;
				break;

			default:
				break;
		}
	}

	/*
	----------------------------- Redraw the map -----------------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();

	/*
	------------------------- If cancel, just return -------------------------
	*/
	if (cancel) {
		return(-1);
	}

	/*
	------------------------ Save selections & return ------------------------
	*/
	if (ScenPlayer != SCEN_PLAYER_MPLAYER) {
		/*
		.............................. Credits ................................
		*/
		gdi_credits = atol(gdicred_buf);
		nod_credits = atol(nodcred_buf);
		neut_credits = atol(neutcred_buf);
		HouseClass::As_Pointer(HOUSE_GOOD)->Credits = gdi_credits * 1000L;
		HouseClass::As_Pointer(HOUSE_BAD)->Credits = nod_credits * 1000L;
		HouseClass::As_Pointer(HOUSE_NEUTRAL)->Credits = neut_credits * 1000L;
		/*
		............................... Edges .................................
		*/
		HouseClass::As_Pointer(HOUSE_GOOD)->Edge = gdi_edge;
		HouseClass::As_Pointer(HOUSE_BAD)->Edge = nod_edge;
	}

	/*
	........................... Sidebar build level ..........................
	*/
	BuildLevel = atoi(level_buf);

	/*........................................................................
	Change the theater:
	- 1st set the Theater global
	- scan all cells to check their TType for compatibility with the new
	  theater; if not compatible, set TType to TEMPLATE_NONE & TIcon to 0
	- Then, re-initialize the TypeClasses for the new Theater
	........................................................................*/
	theater = (TheaterType)(THEATER_NONE + 1 + theaterbtn.Current_Index());
	if (theater != orig_theater) {
		/*
		....................... Loop through all cells ........................
		*/
		for (i =0;i<MAP_CELL_TOTAL; i++) {
			/*..................................................................
			If this cell has a template icon & that template isn't compatible
			with this theater, set the icon to NONE
			..................................................................*/
			if ((*this)[i].TType != TEMPLATE_NONE) {
				theater_mask = TemplateTypeClass::As_Reference((*this)[i].TType).Theater;
				if ( (theater_mask & (1 << theater))==0) {
					(*this)[i].TType = TEMPLATE_NONE;
					(*this)[i].TIcon = 0;
				}
			}
			/*..................................................................
			If this cell has terrain in it, and that terrain isn't compatible
			with this theater, delete the terrain object.
			..................................................................*/
			terrain = (*this)[i].Cell_Terrain();
			if (terrain) {
				theater_mask = terrain->Class->Theater;
				if ( (theater_mask & (1<<theater))==0) {
					delete terrain;
				}
			}
		}

		/*.....................................................................
		Re-init the object Type Classes for this theater
		.....................................................................*/
		Init_Theater(theater);
		TerrainTypeClass::Init(theater);
		TemplateTypeClass::Init(theater);
		OverlayTypeClass::Init(theater);
		UnitTypeClass::Init(theater);
		InfantryTypeClass::Init(theater);
		BuildingTypeClass::Init(theater);
		BulletTypeClass::Init(theater);
		AnimTypeClass::Init(theater);
		AircraftTypeClass::Init(theater);
		SmudgeTypeClass::Init(theater);
	}

	return(0);
}


/***************************************************************************
 * Handle_Triggers -- processes the trigger dialogs                        *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/29/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Handle_Triggers(void)
{
	int rc;

	/*------------------------------------------------------------------------
	Trigger dialog processing loop:
	- Invoke the trigger selection dialog. If a trigger's selected, break
	  & return
	- If user wants to edit the current trigger, do so
	- If user wants to create new trigger, new a TriggerClass & edit it
	- If user wants to delete trigger, delete the current trigger
	- Keep looping until 'OK'
	------------------------------------------------------------------------*/
	while (1) {
		/*
		........................... Select trigger ............................
		*/
		rc = Select_Trigger();

		/*
		............................. 'OK'; break .............................
		*/
		if (rc==0) break;

		/*
		............................... 'Edit' ................................
		*/
		if (rc==1 && CurTrigger) {
			if (Edit_Trigger()==0) {
				Changed = 1;
			}
		}

		/*
		................................ 'New' ................................
		*/
		if (rc==2) {
			/*
			..................... Create a new trigger ......................
			*/
			CurTrigger = new TriggerClass();
			if (CurTrigger) {
				/*
				................... delete it if user cancels ...................
				*/
				if (Edit_Trigger()==-1) {
					delete CurTrigger;
					CurTrigger = NULL;
				} else {
					Changed = 1;
				}

			} else {

				/*
				................. Unable to create; issue warning ..................
				*/
				CCMessageBox().Process("No more triggers available.");
				HiddenPage.Clear();
				Flag_To_Redraw(true);
				Render();
			}
		}

		/*
		.............................. 'Delete' ...............................
		*/
		if (rc==3) {
			if (CurTrigger) {
				CurTrigger->Remove();
				CurTrigger = NULL;
				Changed = 1;
			}
		}
	}

	/*------------------------------------------------------------------------
	Don't allow trigger placement if the trigger is house-specific; such
	triggers cannot be "placed".
	------------------------------------------------------------------------*/
	if (CurTrigger) {
		if (!TriggerClass::Event_Need_Object(CurTrigger->Event)) {
			CurTrigger = NULL;
		}
	}
}


/***************************************************************************
 * MapEditClass::Select_Trigger -- lets user select a trigger              *
 *                                                                         *
 * CurTrigger can be NULL when this function is called.                    *
 *                                                                         *
 *    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿           *
 *    ³                       Triggers                         ³           *
 *    ³    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿    ³           *
 *    ³    ³ Name     Event     Action    House   Team  ³³    ³           *
 *    ³    ³ Name     Event     Action    House   Team  ÃÄ´    ³           *
 *    ³    ³ Name     Event     Action    House   Team  ³ ³    ³           *
 *    ³    ³ Name     Event     Action    House   Team  ³ ³    ³           *
 *    ³    ³                                            ³ ³    ³           *
 *    ³    ³                                            ³ ³    ³           *
 *    ³    ³                                            ÃÄ´    ³           *
 *    ³    ³                                            ³³    ³           *
 *    ³    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ    ³           *
 *    ³                                                        ³           *
 *    ³      [Edit]        [New]        [Delete]      [OK]     ³           *
 *    ³                                                        ³           *
 *    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ           *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, 1 = Edit, 2 = New, 3 = Delete                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      Uses HIDBUFF.                                                      *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/29/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Select_Trigger(void)
{
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	enum {
		D_DIALOG_W = 640,
		D_DIALOG_H = 290,
		D_DIALOG_X = ((640 - D_DIALOG_W) / 2),
		D_DIALOG_Y = ((400 - D_DIALOG_H) / 2),
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),

		D_TXT8_H = 22,
		D_MARGIN = 14,

		D_LIST_W = 612,
		D_LIST_H = 208,
		D_LIST_X = D_DIALOG_X + D_MARGIN,
		D_LIST_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H,

		D_EDIT_W = 90,
		D_EDIT_H = 18,
		D_EDIT_X = D_DIALOG_X + (D_DIALOG_W / 8) - (D_EDIT_W / 2),
		D_EDIT_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_EDIT_H,

		D_NEW_W = 90,
		D_NEW_H = 18,
		D_NEW_X = D_DIALOG_X + (D_DIALOG_W / 8) * 3 - (D_NEW_W / 2),
		D_NEW_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_NEW_H,

		D_DELETE_W = 90,
		D_DELETE_H = 18,
		D_DELETE_X = D_DIALOG_X + (D_DIALOG_W / 8) * 5 - (D_DELETE_W / 2),
		D_DELETE_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_DELETE_H,

		D_OK_W = 90,
		D_OK_H = 18,
		D_OK_X = D_DIALOG_X + (D_DIALOG_W / 8) * 7 - (D_OK_W / 2),
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_OK_H,

	};

	/*........................................................................
	Button enumerations:
	........................................................................*/
	enum {
		TRIGGER_LIST=100,
		BUTTON_EDIT,
		BUTTON_NEW,
		BUTTON_DELETE,
		BUTTON_OK,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*........................................................................
	Dialog variables:
	........................................................................*/
	RedrawType display;							// requested redraw level
	bool process;									// loop while true
	char *trigtext[TRIGGER_MAX + 1];			// text for defined triggers
	KeyNumType input;								// user input
	bool edit_trig = false;						// true = user wants to edit
	bool new_trig = false;						// true = user wants to new
	bool del_trig = false;						// true = user wants to new
	int i;											// loop counter
	int def_idx;									// default list index
	static int tabs[] = {70, 240, 390, 440}; 	// list box tab stops

	/*........................................................................
	Buttons
	........................................................................*/
	ControlClass *commands = NULL;				// the button list

	ListClass triggerlist (TRIGGER_LIST, D_LIST_X, D_LIST_Y, D_LIST_W, D_LIST_H,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		Hires_Retrieve("BTN-UP.SHP"),
		Hires_Retrieve("BTN-DN.SHP"));

	TextButtonClass editbtn (BUTTON_EDIT, "Edit", TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW,
		D_EDIT_X, D_EDIT_Y, D_EDIT_W, D_EDIT_H);

	TextButtonClass newbtn (BUTTON_NEW, "New", TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW,
		D_NEW_X, D_NEW_Y, D_NEW_W, D_NEW_H);

	TextButtonClass deletebtn (BUTTON_DELETE, "Delete", TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW,
		D_DELETE_X, D_DELETE_Y, D_DELETE_W, D_DELETE_H);

	TextButtonClass okbtn (BUTTON_OK, TXT_OK, TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW,
		D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);

	/*
	......................... Fill in trigger names ..........................
	*/
	def_idx = 0;
	for (i = 0; i < Triggers.Count(); i++) {
		/*.....................................................................
		Generate string for this trigger
		- Name can be up to 4 characters
		- Event can be up to 15 characters
		- Action can be up to 15 characters
		- House is 3 characters
		- Team name is up to 11 characters
		.....................................................................*/
		//trigtext[i] = (char *)HidPage.Get_Graphic_Buffer()->Get_Buffer() + 60 * i;
		trigtext[i] = new char[255];
		sprintf(trigtext[i],"%s\t%s\t%s\t",
			Triggers.Ptr(i)->Get_Name(),
			TriggerClass::Name_From_Event(Triggers.Ptr(i)->Event),
			TriggerClass::Name_From_Action(Triggers.Ptr(i)->Action));

		/*
		......................... Add on the house ID .........................
		*/
		if (TriggerClass::Event_Need_House(Triggers.Ptr(i)->Event)) {
			if (Triggers.Ptr(i)->House != HOUSE_NONE) {
				strcat(trigtext[i], HouseTypeClass::As_Reference(Triggers.Ptr(i)->House).Suffix);
			} else {
				strcat(trigtext[i], "!!!");
			}
		} else {
			strcat(trigtext[i],"   ");
		}

		/*
		.......................... Add the team name ..........................
		*/
		strcat(trigtext[i],"\t");
		if (TriggerClass::Action_Need_Team(Triggers.Ptr(i)->Action)) {
			if (Triggers.Ptr(i)->Team) {
				strcat(trigtext[i],Triggers.Ptr(i)->Team->IniName);
			} else {
				strcat(trigtext[i], "!!!");
			}
		}

		/*
		................. Set def_idx if this is CurTrigger ...................
		*/
		if (Triggers.Ptr(i) == CurTrigger) {
			def_idx = i;
		}
	}

	/*
	.......................... Fill in the list box ..........................
	*/
	for (i = 0; i < Triggers.Count(); i++) {
		triggerlist.Add_Item(trigtext[i]);
	}
	triggerlist.Set_Selected_Index(def_idx);

	/*
	....................... Set CurTrigger if it isn't .......................
	*/
	if (Triggers.Count()==0) {
		CurTrigger = NULL;
	} else {
		if (!CurTrigger) {
			CurTrigger = Triggers.Ptr(def_idx);
		}
	}

	/*
	............................ Create the list .............................
	*/
	commands = &triggerlist;
	editbtn.Add_Tail(*commands);
	newbtn.Add_Tail(*commands);
	deletebtn.Add_Tail(*commands);
	okbtn.Add_Tail(*commands);

	/*
	------------------------ Init tab stops for list -------------------------
	*/
	triggerlist.Set_Tabs(tabs);

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	display = REDRAW_ALL;
	process = true;
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {

			/*
			...................... Display the dialog box ......................
			*/
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {
				Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
				Draw_Caption(TXT_NONE, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);

				/*
				....................... Draw the captions .......................
				*/
				Fancy_Text_Print("Triggers", D_DIALOG_CX, D_DIALOG_Y + D_MARGIN, CC_GREEN, TBLACK,
					TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);
			}

			/*
			........................ Redraw the buttons ........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Draw_All();
			}
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		............................ Process input ............................
		*/
		switch (input) {
			case (TRIGGER_LIST | KN_BUTTON):
				def_idx = triggerlist.Current_Index();
				if (def_idx < Triggers.Count()) {
					CurTrigger = Triggers.Ptr(def_idx);
				}
				break;

			case (BUTTON_EDIT | KN_BUTTON):
				if (CurTrigger) {		// only allow if there's one selected
					process = false;
					edit_trig = true;
				}
				break;

			case (BUTTON_NEW | KN_BUTTON):
				process = false;
				new_trig = true;
				break;

			case (BUTTON_DELETE | KN_BUTTON):
				process = false;
				del_trig = true;
				break;

			case (KN_RETURN):
			case (BUTTON_OK | KN_BUTTON):
				process = false;
				break;
		}
	}

	/*
	--------------------------- Redraw the display ---------------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();

	for (i = 0; i < Triggers.Count(); i++) {
		delete [] trigtext[i];
	}

	if (edit_trig) return(1);
	if (new_trig) return(2);
	if (del_trig) return(3);
	return(0);
}


/***************************************************************************
 * MapEditClass::Edit_Trigger -- lets user edit a [new] trigger            *
 *                                                                         *
 * ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿ *
 * ³                           Trigger Editor                            ³ *
 * ³                                                                     ³ *
 * ³             Events                              Actions             ³ *
 * ³  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿        ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿  ³ *
 * ³  ³                        ³³        ³                         ³³  ³ *
 * ³  ³                        ÃÄ´        ³                         ÃÄ´  ³ *
 * ³  ³                        ³ ³        ³                         ³ ³  ³ *
 * ³  ³                        ³ ³        ³                         ³ ³  ³ *
 * ³  ³                        ÃÄ´        ³                         ÃÄ´  ³ *
 * ³  ³                        ³³        ³                         ³³  ³ *
 * ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ        ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ  ³ *
 * ³                                                                     ³ *
 * ³           Name: _______                   [  Volatile    ]          ³ *
 * ³                               [GDI]       [  Persistent  ]          ³ *
 * ³ Time / Credits: _______       [NOD]       [SemiPersistent]          ³ *
 * ³                                                                     ³ *
 * ³          [Team] Team_Name                                           ³ *
 * ³                                                                     ³ *
 * ³                         [OK]        [Cancel]                        ³ *
 * ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, -1 = cancel                                                *
 *                                                                         *
 * WARNINGS:                                                               *
 *      CurTrigger must NOT be NULL when this function is called.          *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/29/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Edit_Trigger(void)
{
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	enum {
		D_DIALOG_W = 528,
		D_DIALOG_H = 376,
		D_DIALOG_X = ((640 - D_DIALOG_W) / 2),
		D_DIALOG_Y = ((400 - D_DIALOG_H) / 2),
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),

		D_TXT8_H = 22,
		D_MARGIN = 14,

		D_EVENT_W = 240,
		D_EVENT_H = 88,
		D_EVENT_X = D_DIALOG_X + D_MARGIN,
		D_EVENT_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H + D_MARGIN + D_TXT8_H,

		D_ACTION_W = 240,
		D_ACTION_H = 88,
		D_ACTION_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_ACTION_W,
		D_ACTION_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H + D_MARGIN + D_TXT8_H,

		D_NAME_W = 80,
		D_NAME_H = 18,
		D_NAME_X = D_EVENT_X + (D_EVENT_W / 2) - 10,
		D_NAME_Y = D_EVENT_Y + D_EVENT_H + D_MARGIN,

		D_DATA_W = 80,
		D_DATA_H = 18,
		D_DATA_X = D_NAME_X,
		D_DATA_Y = D_NAME_Y + D_NAME_H + D_MARGIN,

		D_TEAM_W = 80,
		D_TEAM_H = 18,
		D_TEAM_X = D_NAME_X - D_TEAM_W - 5,
		D_TEAM_Y = D_DATA_Y + D_DATA_H + D_MARGIN,

		D_GDI_W = 90,
		D_GDI_H = 18,
		D_GDI_X = D_DIALOG_CX - (D_GDI_W / 2),
		D_GDI_Y = D_NAME_Y,

		D_NOD_W = 90,
		D_NOD_H = 18,
		D_NOD_X = D_GDI_X,
		D_NOD_Y = D_GDI_Y + D_GDI_H,

		D_NEU_W = 90,
		D_NEU_H = 18,
		D_NEU_X = D_NOD_X,
		D_NEU_Y = D_NOD_Y + D_NOD_H,

		D_MULTI1_W = 44,
		D_MULTI1_H = 18,
		D_MULTI1_X = D_GDI_X,
		D_MULTI1_Y = D_GDI_Y,

		D_MULTI2_W = 44,
		D_MULTI2_H = 18,
		D_MULTI2_X = D_GDI_X + D_MULTI1_W,
		D_MULTI2_Y = D_GDI_Y,

		D_MULTI3_W = 44,
		D_MULTI3_H = 18,
		D_MULTI3_X = D_NOD_X,
		D_MULTI3_Y = D_NOD_Y,

		D_MULTI4_W = 44,
		D_MULTI4_H = 18,
		D_MULTI4_X = D_NOD_X + D_MULTI1_W,
		D_MULTI4_Y = D_NOD_Y,

		D_VOLATILE_W = 100,
		D_VOLATILE_H = 18,
		D_VOLATILE_X = D_ACTION_X + (D_ACTION_W / 2) - (D_VOLATILE_W / 2) + 10,
		D_VOLATILE_Y = D_NAME_Y,

		D_PERSIST_W = 100,
		D_PERSIST_H = 18,
		D_PERSIST_X = D_ACTION_X + (D_ACTION_W / 2) - (D_PERSIST_W / 2) + 10,
		D_PERSIST_Y = D_VOLATILE_Y + D_VOLATILE_H,

		D_SEMIPERSIST_W = 100,
		D_SEMIPERSIST_H = 18,
		D_SEMIPERSIST_X = D_ACTION_X + (D_ACTION_W / 2) - (D_SEMIPERSIST_W / 2) + 10,
		D_SEMIPERSIST_Y = D_PERSIST_Y + D_PERSIST_H,

		D_OK_W = 90,
		D_OK_H = 18,
		D_OK_X = D_DIALOG_CX - 5 - D_OK_W,
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_OK_H,

		D_CANCEL_W = 90,
		D_CANCEL_H = 18,
		D_CANCEL_X = D_DIALOG_CX + 5,
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_CANCEL_H,

	};

	/*........................................................................
	Button enumerations:
	........................................................................*/
	enum {
		EVENT_LIST=100,
		ACTION_LIST,
		NAME_EDIT,
		DATA_EDIT,
		BUTTON_TEAM,
		BUTTON_GDI,
		BUTTON_NOD,
		BUTTON_NEUTRAL,
		BUTTON_JP,			// placeholder
		BUTTON_MULTI1,
		BUTTON_MULTI2,
		BUTTON_MULTI3,
		BUTTON_MULTI4,
		BUTTON_MULTI5,
		BUTTON_MULTI6,
		BUTTON_VOLATILE,
		BUTTON_PERSIST,
		BUTTON_SEMIPERSIST,
		BUTTON_OK,
		BUTTON_CANCEL,
	};

	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*........................................................................
	Dialog variables:
	........................................................................*/
	RedrawType display;								// requested redraw level
	bool process;										// loop while true
	KeyNumType input;									// user input
	bool cancel = false;								// true = user cancels
	int i;												// loop counter
	EventType event_idx;								// index for event list
	TriggerClass::ActionType action_idx;		// index for action list
	char namebuf[5];									// name of this trigger
	char databuf[10];									// for credit/time-based triggers
	HousesType house;									// house for this trigger
	const char *eventnames[EVENT_COUNT + 1];	// names of events
	const char *actionnames[TriggerClass::ACTION_COUNT + 1];	// names of actions
	TriggerClass::PersistantType persistant;									// trigger's persistence level

	/*........................................................................
	These flags enable various controls for each EventType.
	........................................................................*/
//	static char data_enabled[EVENT_COUNT] = {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0};
//	static char house_enabled[EVENT_COUNT] = {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1};
//	static char team_enabled[TriggerClass::ACTION_COUNT] = {0,0,0,1,1,0,1,0,0,0,0,0,0,0};

	/*........................................................................
	Buttons
	........................................................................*/
	ControlClass *commands = NULL;				// the button list

	ListClass eventlist(EVENT_LIST,
		D_EVENT_X, D_EVENT_Y, D_EVENT_W, D_EVENT_H,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		Hires_Retrieve("BTN-UP.SHP"),
		Hires_Retrieve("BTN-DN.SHP"));

	ListClass actionlist(ACTION_LIST,
		D_ACTION_X, D_ACTION_Y, D_ACTION_W, D_ACTION_H,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		Hires_Retrieve("BTN-UP.SHP"),
		Hires_Retrieve("BTN-DN.SHP"));

	EditClass name_edt(NAME_EDIT, namebuf, 5,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NAME_X, D_NAME_Y, D_NAME_W, D_NAME_H, EditClass::ALPHANUMERIC);

	EditClass data_edt(DATA_EDIT, databuf, 8,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_DATA_X, D_DATA_Y, D_DATA_W, D_DATA_H, EditClass::ALPHANUMERIC);

	TextButtonClass teambtn(BUTTON_TEAM, "Team",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_TEAM_X, D_TEAM_Y, D_TEAM_W, D_TEAM_H);

	TextButtonClass gdibtn(BUTTON_GDI, "GDI",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_GDI_X, D_GDI_Y, D_GDI_W, D_GDI_H);

	TextButtonClass nodbtn(BUTTON_NOD, "NOD",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NOD_X, D_NOD_Y, D_NOD_W, D_NOD_H);

	TextButtonClass neutralbtn(BUTTON_NEUTRAL, "Neutral",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NEU_X, D_NEU_Y, D_NEU_W, D_NEU_H);

	TextButtonClass multi1btn(BUTTON_MULTI1, "M1",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI1_X, D_MULTI1_Y, D_MULTI1_W, D_MULTI1_H);

	TextButtonClass multi2btn(BUTTON_MULTI2, "M2",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI2_X, D_MULTI2_Y, D_MULTI2_W, D_MULTI2_H);

	TextButtonClass multi3btn(BUTTON_MULTI3, "M3",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI3_X, D_MULTI3_Y, D_MULTI3_W, D_MULTI3_H);

	TextButtonClass multi4btn(BUTTON_MULTI4, "M4",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI4_X, D_MULTI4_Y, D_MULTI4_W, D_MULTI4_H);

	TextButtonClass volatilebtn(BUTTON_VOLATILE, "Volatile",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_VOLATILE_X, D_VOLATILE_Y, D_VOLATILE_W, D_VOLATILE_H);

	TextButtonClass persistbtn(BUTTON_PERSIST, "Persistant",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_PERSIST_X, D_PERSIST_Y, D_PERSIST_W, D_PERSIST_H);

	TextButtonClass semipersistbtn(BUTTON_SEMIPERSIST, "SemiPersistant",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_SEMIPERSIST_X, D_SEMIPERSIST_Y, D_SEMIPERSIST_W, D_SEMIPERSIST_H);

	TextButtonClass okbtn(BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);

	/*
	....................... Set default button states ........................
	*/
	event_idx = CurTrigger->Event;				// event list
	if (event_idx == EVENT_NONE) event_idx = EVENT_FIRST;

	action_idx = CurTrigger->Action;			// action list
	if (action_idx == TriggerClass::ACTION_NONE) action_idx = TriggerClass::ACTION_FIRST;

	strcpy(namebuf,CurTrigger->Get_Name());				// Name
	name_edt.Set_Text(namebuf,5);

	if (TriggerClass::Event_Need_Data(event_idx)) {
		sprintf(databuf,"%ld",CurTrigger->Data);	// Credits/Time
		data_edt.Set_Text(databuf,8);
	}

	house = CurTrigger->House;						// House

	persistant = CurTrigger->IsPersistant;

	volatilebtn.Turn_Off();
	persistbtn.Turn_Off();
	semipersistbtn.Turn_Off();
	switch (CurTrigger->IsPersistant) {
		case TriggerClass::VOLATILE:
			volatilebtn.Turn_On();
			break;

		case TriggerClass::SEMIPERSISTANT:
			semipersistbtn.Turn_On();
			break;

		case TriggerClass::PERSISTANT:
			persistbtn.Turn_On();
			break;
	}

	/*
	......................... Fill in the list boxes .........................
	*/
	for (i = 0; i < EVENT_COUNT; i++) {
		eventnames[i] = TriggerClass::Name_From_Event( (EventType)i);
		eventlist.Add_Item(eventnames[i]);
	}
	eventlist.Set_Selected_Index(event_idx);

	for (i = 0; i < TriggerClass::ACTION_COUNT; i++) {
		actionnames[i] = TriggerClass::Name_From_Action( (TriggerClass::ActionType)i);
		actionlist.Add_Item(actionnames[i]);
	}
	actionlist.Set_Selected_Index(action_idx);

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	display = REDRAW_ALL;
	process = true;
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();

		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {

			/*
			...................... Display the dialog box ......................
			*/
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {
				Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
				Draw_Caption(TXT_NONE, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);

				/*
				....................... Draw the captions .......................
				*/
				Fancy_Text_Print("Trigger Editor", D_DIALOG_CX, D_DIALOG_Y + D_MARGIN,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print("Events", D_EVENT_X + D_EVENT_W / 2,
					D_EVENT_Y - D_TXT8_H,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print("Actions", D_ACTION_X + D_ACTION_W / 2,
					D_ACTION_Y - D_TXT8_H,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				Fancy_Text_Print("Name", D_NAME_X - 5, D_NAME_Y,
					CC_GREEN, TBLACK,
					TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				if ((EventType)event_idx==EVENT_CREDITS) {	// use 'Data' for Credits
					Fancy_Text_Print("Credits", D_DATA_X - 5, D_DATA_Y,
						CC_GREEN, TBLACK,
						TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);

				} else {
					if ((EventType)event_idx==EVENT_TIME) {	// use 'Data' for Time
						Fancy_Text_Print("1/10 Min", D_DATA_X - 5, D_DATA_Y,
							CC_GREEN, TBLACK,
							TPF_RIGHT | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
					}
				}

				if (TriggerClass::Action_Need_Team(action_idx)) {
					if (CurTrigger->Team) {
						Fancy_Text_Print(CurTrigger->Team->IniName,
							D_TEAM_X + D_TEAM_W + 5, D_TEAM_Y, CC_GREEN, TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
					} else {
						Fancy_Text_Print( "!!!",
							D_TEAM_X + D_TEAM_W + 5, D_TEAM_Y, CC_GREEN, TBLACK,
							TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
					}
				}
			}

			/*
			..................... Rebuild the button list ......................
			*/
			eventlist.Zap();
			actionlist.Zap();
			name_edt.Zap();
			data_edt.Zap();
			teambtn.Zap();
			gdibtn.Zap();
			nodbtn.Zap();
			neutralbtn.Zap();
			volatilebtn.Zap();
			persistbtn.Zap();
			semipersistbtn.Zap();
			okbtn.Zap();
			cancelbtn.Zap();

			commands = &okbtn;
			cancelbtn.Add_Tail(*commands);
			eventlist.Add_Tail(*commands);
			actionlist.Add_Tail(*commands);
			name_edt.Add_Tail(*commands);
			volatilebtn.Add_Tail(*commands);
			persistbtn.Add_Tail(*commands);
			semipersistbtn.Add_Tail(*commands);
			if (TriggerClass::Event_Need_Data(event_idx)) {
				data_edt.Add_Tail(*commands);
				sprintf(databuf,"%ld",CurTrigger->Data);
				data_edt.Set_Text(databuf,8);
			}
			if (TriggerClass::Event_Need_House(event_idx)) {
				gdibtn.Add_Tail(*commands);
				nodbtn.Add_Tail(*commands);
				neutralbtn.Add_Tail(*commands);
				Set_House_Buttons(house, commands, BUTTON_GDI);
			}
			if (TriggerClass::Action_Need_Team(action_idx)) teambtn.Add_Tail(*commands);

			/*
			........................ Redraw the buttons ........................
			*/
			if (display >= REDRAW_BUTTONS) {
				commands->Flag_List_To_Redraw();
			}
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		............................ Process input ............................
		*/
		switch (input) {
			case (EVENT_LIST | KN_BUTTON):
				if (eventlist.Current_Index() != event_idx) {
					event_idx = EventType(eventlist.Current_Index());
					databuf[0] = 0;
					CurTrigger->Data = 0;
					if (!TriggerClass::Event_Need_House(event_idx)) {
						CurTrigger->House = HOUSE_NONE;
					}
					display = REDRAW_ALL;
				}
				break;

			case (ACTION_LIST | KN_BUTTON):
				if (actionlist.Current_Index() != action_idx) {
					action_idx = TriggerClass::ActionType(actionlist.Current_Index());
					display = REDRAW_ALL;
				}
				break;

			case (NAME_EDIT | KN_BUTTON):
				break;

			case (DATA_EDIT | KN_BUTTON):
				break;

			case (BUTTON_GDI | KN_BUTTON):
			case (BUTTON_NOD | KN_BUTTON):
			case (BUTTON_NEUTRAL | KN_BUTTON):
			case (BUTTON_MULTI1 | KN_BUTTON):
			case (BUTTON_MULTI2 | KN_BUTTON):
			case (BUTTON_MULTI3 | KN_BUTTON):
			case (BUTTON_MULTI4 | KN_BUTTON):
			case (BUTTON_MULTI5 | KN_BUTTON):
			case (BUTTON_MULTI6 | KN_BUTTON):
				house = (HousesType)( (input & (~KN_BUTTON)) - BUTTON_GDI);
				Set_House_Buttons(house, commands, BUTTON_GDI);
				break;

			case (BUTTON_TEAM | KN_BUTTON):
				Handle_Teams("Select a Team");
				if (CurTeam) {
					CurTrigger->Team = CurTeam;
				}
				HiddenPage.Clear();
				Flag_To_Redraw(true);
				Render();
				display = REDRAW_ALL;
				break;

			case (BUTTON_VOLATILE | KN_BUTTON):
				persistant = TriggerClass::VOLATILE;
				volatilebtn.Turn_On();
				persistbtn.Turn_Off();
				semipersistbtn.Turn_Off();
				break;

			case (BUTTON_PERSIST | KN_BUTTON):
				persistant = TriggerClass::PERSISTANT;
				volatilebtn.Turn_Off();
				persistbtn.Turn_On();
				semipersistbtn.Turn_Off();
				break;

			case (BUTTON_SEMIPERSIST | KN_BUTTON):
				persistant = TriggerClass::SEMIPERSISTANT;
				volatilebtn.Turn_Off();
				persistbtn.Turn_Off();
				semipersistbtn.Turn_On();
				break;

			case (KN_RETURN):
			case (BUTTON_OK | KN_BUTTON):
				process = false;
				break;

			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				cancel = true;
				process = false;
				break;

			default:
				break;
		}
	}

	/*
	------------------------------ Save values -------------------------------
	*/
	if (!cancel) {

		/*
		.......................... Get list indices ...........................
		*/
		event_idx = EventType(eventlist.Current_Index());
		action_idx = TriggerClass::ActionType(actionlist.Current_Index());

		/*
		......................... Set Event & Action ..........................
		*/
		CurTrigger->Event = EventType(event_idx);
		CurTrigger->Action = TriggerClass::ActionType(action_idx);

		/*
		.............................. Set name ...............................
		*/
		if (strlen(namebuf)==0) {
			CurTrigger->Set_Name("____");
		} else {
			CurTrigger->Set_Name(namebuf);
		}

		/*
		.............................. Set Data ...............................
		*/
		if (TriggerClass::Event_Need_Data(event_idx)) {
			CurTrigger->Data = atol(databuf);
		}

		/*
		.............................. Set House ..............................
		*/
		if (TriggerClass::Event_Need_House(event_idx)) {
			CurTrigger->House = house;
		} else {
			CurTrigger->House = HOUSE_NONE;
		}

		/*
		........................... Set Persistence  ..........................
		*/
		CurTrigger->IsPersistant = persistant;
	}

	/*
	--------------------------- Redraw the display ---------------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();

	if (cancel) {
		return(-1);
	} else {
		return(0);
	}
}


/***************************************************************************
 * MapEditClass::Import_Triggers -- lets user import triggers              *
 *                                                                         *
 *    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                  *
 *    ³                    Triggers                     ³                  *
 *    ³    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿   ³                  *
 *    ³    ³x Name     Event     Action    House  ³³   ³                  *
 *    ³    ³  Name     Event     Action    House  ÃÄ´   ³                  *
 *    ³    ³x Name     Event     Action    House  ³ ³   ³                  *
 *    ³    ³  Name     Event     Action    House  ³ ³   ³                  *
 *    ³    ³                                      ³ ³   ³                  *
 *    ³    ³                                      ³ ³   ³                  *
 *    ³    ³                                      ÃÄ´   ³                  *
 *    ³    ³                                      ³³   ³                  *
 *    ³    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ   ³                  *
 *    ³                                                 ³                  *
 *    ³                [OK]     [Cancel]                ³                  *
 *    ³                                                 ³                  *
 *    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, -1 = user cancelled                                        *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   03/29/1995 BRR : Created.                                             *
 *=========================================================================*/
int MapEditClass::Import_Triggers(void)
{
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	enum {
		D_DIALOG_W = 480,
		D_DIALOG_H = 290,
		D_DIALOG_X = ((640 - D_DIALOG_W) / 2),
		D_DIALOG_Y = ((400 - D_DIALOG_H) / 2),
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),

		D_TXT8_H = 22,
		D_MARGIN = 14,

		D_LIST_W = 452,
		D_LIST_H = 208,
		D_LIST_X = D_DIALOG_X + D_MARGIN,
		D_LIST_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H,

		D_OK_W = 90,
		D_OK_H = 18,
		D_OK_X = D_DIALOG_CX - D_OK_W - 5,
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_OK_H,

		D_CANCEL_W = 90,
		D_CANCEL_H = 18,
		D_CANCEL_X = D_DIALOG_CX + 5,
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_OK_H,

	};
	/*........................................................................
	Button enumerations:
	........................................................................*/
	enum {
		TRIGGER_LIST=100,
		BUTTON_OK,
		BUTTON_CANCEL,
	};
	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;
	/*........................................................................
	Dialog variables:
	........................................................................*/
	RedrawType display;							// requested redraw level
	bool process;									// loop while true
	KeyNumType input;								// user input
	bool cancel = false;
	static int tabs[] =
		{70, 220, 370, 420}; 					// list box tab stops
	DynamicVectorClass<char *> trignames;	// list of INI trigger names
	char *inibuf;									// working INI buffer
	CCFileClass file;								// file for reading the INI file
	char buf[128];									// for reading an INI entry
	char *tbuffer;									// Accumulation buffer of trigger IDs.
	int len;											// Length of data in buffer.
	TriggerClass *trigger;						// Working trigger pointer.
	char *item;										// for adding to list box
	char *eventptr;
	char *actionptr;
	char *houseptr;
	int i;
	/*........................................................................
	Buttons
	........................................................................*/
	ControlClass *commands = NULL;				// the button list

	CheckListClass triggerlist (TRIGGER_LIST,
		D_LIST_X, D_LIST_Y, D_LIST_W, D_LIST_H,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		Hires_Retrieve("BTN-UP.SHP"),
		Hires_Retrieve("BTN-DN.SHP"));

	TextButtonClass okbtn (BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	TextButtonClass cancelbtn (BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	Set_Logic_Page(SeenBuff);

	/*------------------------------------------------------------------------
	Read the MASTER.INI file
	------------------------------------------------------------------------*/
	/*........................................................................
	Read the file into the staging buffer
	........................................................................*/
	inibuf = new char [30000];
	memset(inibuf, '\0', 30000);
	file.Set_Name("MASTER.INI");
	if (!file.Is_Available()) {
		file.Close();
		delete [] inibuf;
		return(-1);
	} else {
		file.Read(inibuf, 30000 - 1);
	}
	file.Close();

	/*........................................................................
	Read all entry names in the Triggers section into a temp buffer
	........................................................................*/
	len = strlen(inibuf) + 2;
	tbuffer = inibuf + len;
	WWGetPrivateProfileString(TriggerClass::INI_Name(), NULL, NULL, tbuffer,
		30000 - len, inibuf);

	/*........................................................................
	For each entry in the INI section:
	- Get the entry
	- Generate a string describing the trigger
	- Add that string to the list box
	- Add a ptr to the INI entry name to our 'trignames' list
	........................................................................*/
	while (*tbuffer != '\0') {
		WWGetPrivateProfileString(TriggerClass::INI_Name(), tbuffer, NULL, buf, sizeof(buf)-1, inibuf);
		item = new char [60];

		/*
		** Parse the INI entry
		*/
		eventptr = strtok(buf,",");
		actionptr = strtok(NULL,",");
		strtok(NULL,",");
		houseptr = strtok(NULL,",");

		/*
		** Generate the descriptive string
		*/
		sprintf(item, " %s\t%s\t%s\t", tbuffer, eventptr, actionptr);

		/*
		** Add house name if needed
		*/
		if (TriggerClass::Event_Need_House(TriggerClass::Event_From_Name(eventptr))) {
			HousesType house = HouseTypeClass::From_Name(houseptr);
			if (house != HOUSE_NONE) {
				strcat(item, HouseTypeClass::As_Reference(house).Suffix);
			} else {
				strcat(item, "!!!");
			}
		} else {
			strcat(item,"   ");
		}

		/*
		** Add the item to the list box
		*/
		triggerlist.Add_Item(item);

		/*
		** Add the name to our internal name list
		*/
		trignames.Add(tbuffer);

		tbuffer += strlen(tbuffer)+1;
	}

	/*
	............................ Create the list .............................
	*/
	commands = &triggerlist;
	okbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);

	/*
	------------------------ Init tab stops for list -------------------------
	*/
	triggerlist.Set_Tabs(tabs);

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	display = REDRAW_ALL;
	process = true;
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();
		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			/*
			...................... Display the dialog box ......................
			*/
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {
				Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
				Draw_Caption(TXT_NONE, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);
				/*
				....................... Draw the captions .......................
				*/
				Fancy_Text_Print("Import Triggers", D_DIALOG_CX, D_DIALOG_Y + D_MARGIN,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
			}
			/*
			........................ Redraw the buttons ........................
			*/
			if (display >= REDRAW_BUTTONS)
				commands->Flag_List_To_Redraw();
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		............................ Process input ............................
		*/
		switch (input) {
			case (TRIGGER_LIST | KN_BUTTON):
				break;

			case (KN_RETURN):
			case (BUTTON_OK | KN_BUTTON):
				process = false;
				break;

			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				cancel = true;
				process = false;
				break;
		}
	}

	/*
	--------------------------- Redraw the display ---------------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();

	/*........................................................................
	Re-parse the INI section; if any item is checked in the list box, create
	that trigger for this scenario.
	........................................................................*/
	if (!cancel) {
		tbuffer = inibuf + len;
		i = 0;
		while (*tbuffer != '\0') {

			/*
			** If this item is checked on the list, create a new trigger
			** and fill it in.
			*/
			if (triggerlist.Is_Checked(i)) {
				WWGetPrivateProfileString(TriggerClass::INI_Name(), tbuffer, NULL,
					buf, sizeof(buf)-1, inibuf);

				trigger = new TriggerClass();
				trigger->Fill_In(tbuffer, buf);

				if (trigger->House != HOUSE_NONE)
					HouseTriggers[trigger->House].Add(trigger);
			}

			tbuffer += strlen(tbuffer)+1;
			i++;
		}
	}


	/*........................................................................
	Clean up memory
	........................................................................*/
	trignames.Clear();
	while (triggerlist.Count()) {
		item = (char *)triggerlist.Get_Item(0);
		triggerlist.Remove_Item(item);
		delete [] item;
	}
	delete [] inibuf;

	if (cancel) {
		return(-1);
	} else {
		return(0);
	}
}


/***************************************************************************
 * MapEditClass::Import_Teams -- lets the user import teams                *
 *                                                                         *
 *    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿           *
 *    ³                         Teams                          ³           *
 *    ³    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ¿    ³           *
 *    ³    ³ Name     House    Class:Count,Class:Count  ³³    ³           *
 *    ³    ³ Name     House    Class:Count,Class:Count  ÃÄ´    ³           *
 *    ³    ³ Name     House    Class:Count,Class:Count  ³ ³    ³           *
 *    ³    ³ Name     House    Class:Count,Class:Count  ³ ³    ³           *
 *    ³    ³                                            ³ ³    ³           *
 *    ³    ³                                            ³ ³    ³           *
 *    ³    ³                                            ÃÄ´    ³           *
 *    ³    ³                                            ³³    ³           *
 *    ³    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÙ    ³           *
 *    ³                                                        ³           *
 *    ³                    [OK]    [Cancel]                    ³           *
 *    ³                                                        ³           *
 *    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ           *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, -1 = user cancelled                                        *
 *                                                                         *
 * WARNINGS:                                                               *
 *      Uses HIDBUFF.                                                      *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/08/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Import_Teams(void)
{
	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	enum {
		D_DIALOG_W = 528,
		D_DIALOG_H = 290,
		D_DIALOG_X = ((640 - D_DIALOG_W) / 2),
		D_DIALOG_Y = ((400 - D_DIALOG_H) / 2),
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),

		D_TXT8_H = 22,
		D_MARGIN = 14,

		D_LIST_W = 500,
		D_LIST_H = 208,
		D_LIST_X = D_DIALOG_X + D_MARGIN,
		D_LIST_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H,

		D_OK_W = 90,
		D_OK_H = 18,
		D_OK_X = D_DIALOG_CX - D_OK_W - 5,
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_OK_H,

		D_CANCEL_W = 90,
		D_CANCEL_H = 18,
		D_CANCEL_X = D_DIALOG_CX + 5,
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - D_MARGIN - D_OK_H,

		TEAMTXT_LEN = 43,				// max length of a team entry
	};
	/*........................................................................
	Button enumerations:
	........................................................................*/
	enum {
		TEAM_LIST=100,
		BUTTON_OK,
		BUTTON_CANCEL,
	};
	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;
	/*........................................................................
	Dialog variables:
	........................................................................*/
	RedrawType display;							// requested redraw level
	bool process;									// loop while true
	KeyNumType input;								// user input
	bool cancel = false;
	static int tabs[] = {120, 180}; 			// list box tab stops
	DynamicVectorClass<char *> teamnames;	// list of INI team names
	char *inibuf;									// working INI buffer
	CCFileClass file;								// file for reading the INI file
	char buf[128];									// for reading an INI entry
	char *tbuffer;									// Accumulation buffer of team IDs.
	int len;											// Length of data in buffer.
	TeamTypeClass *team;							// Working team pointer.
	char *item;										// for adding to list box
	char *houseptr;
	char *classptr;
	int numclasses;
	int i;
	/*........................................................................
	Buttons
	........................................................................*/
	ControlClass *commands = NULL;				// the button list

	CheckListClass teamlist (TEAM_LIST,
		D_LIST_X, D_LIST_Y, D_LIST_W, D_LIST_H,
		TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		Hires_Retrieve("BTN-UP.SHP"),
		Hires_Retrieve("BTN-DN.SHP"));

	TextButtonClass okbtn (BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	TextButtonClass cancelbtn (BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	Set_Logic_Page(SeenBuff);

	/*------------------------------------------------------------------------
	Read the MASTER.INI file
	------------------------------------------------------------------------*/
	/*........................................................................
	Read the file into the staging buffer
	........................................................................*/
	inibuf = new char [30000];
	memset(inibuf, '\0', 30000);
	file.Set_Name("MASTER.INI");
	if (!file.Is_Available()) {
		file.Close();
		delete [] inibuf;
		return(-1);
	} else {
		file.Read(inibuf, 30000 - 1);
	}

	file.Close();
	/*........................................................................
	Read all entry names in the TeamTypes section into a temp buffer
	........................................................................*/
	len = strlen(inibuf) + 2;
	tbuffer = inibuf + len;
	WWGetPrivateProfileString(TeamTypeClass::INI_Name(), NULL, NULL, tbuffer,
		30000 - len, inibuf);

	/*........................................................................
	For each entry in the INI section:
	- Get the entry
	- Generate a string describing the team
	- Add that string to the list box
	- Add a ptr to the INI entry name to our 'teamnames' list
	........................................................................*/
	while (*tbuffer != '\0') {
		WWGetPrivateProfileString(TeamTypeClass::INI_Name(), tbuffer, NULL, buf, sizeof(buf)-1, inibuf);
		item = new char [60];

		/*
		** Parse the INI entry
		*/
		houseptr = strtok(buf,",");
		for (i = 0; i < 9; i++) {
			strtok(NULL,",");
		}
		numclasses = atoi(strtok(NULL,","));

		/*
		** Generate the descriptive string
		*/
		sprintf(item," %s\t",tbuffer);
		HousesType house = HouseTypeClass::From_Name(houseptr);
		if (house != HOUSE_NONE) {
			strcat(item, HouseTypeClass::As_Reference(house).Suffix);
		} else {
			strcat(item, "!!!");
		}
		strcat(item, "\t");

		classptr = strtok(NULL,",");
		for (i = 0; i < numclasses; i++) {
			if (strlen(item) + strlen(classptr) < 60) {
				strcat(item,classptr);
				classptr = strtok(NULL,",");
			} else {
				break;
			}
		}

		/*
		** Add the item to the list box
		*/
		teamlist.Add_Item(item);
		/*
		** Add the name to our internal name list
		*/
		teamnames.Add(tbuffer);

		tbuffer += strlen(tbuffer)+1;
	}

	/*
	............................ Create the list .............................
	*/
	commands = &teamlist;
	okbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);

	/*
	------------------------ Init tab stops for list -------------------------
	*/
	teamlist.Set_Tabs(tabs);

	/*
	-------------------------- Main Processing Loop --------------------------
	*/
	display = REDRAW_ALL;
	process = true;
	while (process) {

		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored){
			AllSurfaces.SurfacesRestored=FALSE;
			display=REDRAW_ALL;
		}

		/*
		........................ Invoke game callback .........................
		*/
		Call_Back();
		/*
		...................... Refresh display if needed ......................
		*/
		if (display) {
			/*
			...................... Display the dialog box ......................
			*/
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {
				Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
				Draw_Caption(TXT_NONE, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);
				/*
				....................... Draw the captions .......................
				*/
				Fancy_Text_Print("Import Teams", D_DIALOG_CX, D_DIALOG_Y + D_MARGIN,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
			}
			/*
			........................ Redraw the buttons ........................
			*/
			if (display >= REDRAW_BUTTONS)
				commands->Flag_List_To_Redraw();
			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		............................ Process input ............................
		*/
		switch (input) {
			case (TEAM_LIST | KN_BUTTON):
				break;

			case (KN_RETURN):
			case (BUTTON_OK | KN_BUTTON):
				process = false;
				break;

			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				cancel = true;
				process = false;
				break;
		}
	}

	/*
	--------------------------- Redraw the display ---------------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();

	/*........................................................................
	Re-parse the INI section; if any item is checked in the list box, create
	that team for this scenario.
	........................................................................*/
	if (!cancel) {
		tbuffer = inibuf + len;
		i = 0;
		while (*tbuffer != '\0') {
			/*
			** If this item is checked on the list, create a new team
			** and fill it in.
			*/
			if (teamlist.Is_Checked(i)) {
				WWGetPrivateProfileString(TeamTypeClass::INI_Name(), tbuffer, NULL,
					buf, sizeof(buf)-1, inibuf);

				team = new TeamTypeClass();
				team->Fill_In(tbuffer,buf);
			}

			tbuffer += strlen(tbuffer)+1;
			i++;
		}
	}

	/*........................................................................
	Clean up memory
	........................................................................*/
	teamnames.Clear();
	while (teamlist.Count()) {
		item = (char *)teamlist.Get_Item(0);
		teamlist.Remove_Item(item);
		delete [] item;
	}
	delete [] inibuf;

	if (cancel) {
		return(-1);
	} else {
		return(0);
	}
}

#endif
