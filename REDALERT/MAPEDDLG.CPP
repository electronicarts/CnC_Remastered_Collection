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

/* $Header: /CounterStrike/MAPEDDLG.CPP 1     3/03/97 10:25a Joe_bostic $ */
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
 *                  Last Update : September 4, 1996 [JLB]                  *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Map Editor dialogs & main menu options                                  *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   MapEditClass::Handle_Triggers -- processes the trigger dialogs        *
 *   MapEditClass::Load_Scenario -- loads a scenario INI file              *
 *   MapEditClass::New_Scenario -- creates a new scenario                  *
 *   MapEditClass::Pick_Scenario -- dialog for choosing scenario           *
 *   MapEditClass::Save_Scenario -- saves current scenario to an INI file  *
 *   MapEditClass::Scenario_Dialog -- scenario global parameters dialog    *
 *   MapEditClass::Select_Trigger -- lets user select a trigger            *
 *   MapEditClass::Size_Map -- lets user set size & location of map        *
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
	int scen_num;
	ScenarioPlayerType player;
	ScenarioDirType dir;
	ScenarioVarType var;
	Disect_Scenario_Name(Scen.ScenarioName, scen_num, player, dir, var);

	int rc;
	HousesType house;

	/*
	**	Force the house save value to match the player house.
	*/
	if (PlayerPtr) {
		switch (PlayerPtr->Class->House) {
			case HOUSE_SPAIN:
				player = SCEN_PLAYER_SPAIN;
				break;

			case HOUSE_GREECE:
				player = SCEN_PLAYER_GREECE;
				break;

			default:
			case HOUSE_USSR:
				player = SCEN_PLAYER_USSR;
				break;
		}
	}

	/*
	**	Prompt for scenario info
	*/
	rc = Pick_Scenario("New Scenario", scen_num, player, dir, var);
	if (rc != 0) {
		return(-1);
	}

	ScenarioInit++;

	/*
	**	Blow away everything
	*/
	Clear_Scenario();

	/*
	**	Set parameters
	*/
//	Scen.Scenario = scen_num;
//	Scen.ScenPlayer = player;
//	Scen.ScenDir = dir;
//	Scen.ScenVar = var;
	Scen.Set_Scenario_Name(scen_num, player, dir, var);

	/*
	**	Create houses
	*/
	for (house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
		new HouseClass(house);
	}

	switch (player) {
		case SCEN_PLAYER_MPLAYER:
			PlayerPtr = HouseClass::As_Pointer(HOUSE_MULTI1);
			PlayerPtr->IsHuman = true;
			LastHouse = HOUSE_MULTI1;
			break;

		case SCEN_PLAYER_USSR:
			PlayerPtr = HouseClass::As_Pointer(HOUSE_USSR);
			PlayerPtr->IsHuman = true;
			Base.House = HOUSE_SPAIN;
			LastHouse = HOUSE_GOOD;
			break;

		case SCEN_PLAYER_SPAIN:
			PlayerPtr = HouseClass::As_Pointer(HOUSE_SPAIN);
			PlayerPtr->IsHuman = true;
			Base.House = HOUSE_USSR;
			LastHouse = HOUSE_GOOD;
			break;

		case SCEN_PLAYER_GREECE:
			PlayerPtr = HouseClass::As_Pointer(HOUSE_GREECE);
			PlayerPtr->IsHuman = true;
			Base.House = HOUSE_USSR;
			LastHouse = HOUSE_GOOD;
			break;
	}

	/*
	**	Init the entire map
	*/
//	Init_Clear();
	Fill_In_Data();

	/*
	**	Prompt for map size
	*/
	Size_Map(-1, -1, 30, 30);

	/*
	**	Set the Home & Reinforcement Cells to the center of the map
	*/
	Scen.Waypoint[WAYPT_REINF] = XY_Cell(MapCellX + MapCellWidth / 2, MapCellY + MapCellHeight / 2);
	Scen.Waypoint[WAYPT_HOME] = XY_Cell(MapCellX + MapCellWidth / 2, MapCellY + MapCellHeight / 2);
	(*this)[TacticalCoord].IsWaypoint = 1;
	Flag_Cell(Coord_Cell(TacticalCoord));

	Set_Tactical_Position(Cell_Coord(Scen.Waypoint[WAYPT_HOME] - (MAP_CELL_W * 4 * RESFACTOR) - (5 * RESFACTOR)));
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
	int scen_num;
	ScenarioPlayerType player;
	ScenarioDirType dir;
	ScenarioVarType var;
	Disect_Scenario_Name(Scen.ScenarioName, scen_num, player, dir, var);

	int rc;
	NodeNameType * who;						// node to add to Players

	/*
	**	Prompt for scenario info
	*/
	rc = Pick_Scenario("Load Scenario", scen_num, player, dir, var);
	if (rc != 0) {
		return(-1);
	}

	/*
	**	Set parameters
	*/
//	Scen.Scenario = scen_num;
//	Scen.ScenPlayer = player;
//	Scen.ScenDir = dir;
//	Scen.ScenVar = var;
	Scen.Set_Scenario_Name(scen_num, player, dir, var);

	/*
	**	Read_Scenario_Ini() must be able to set PlayerPtr to the right house:
	**	- Reading the INI will create the house objects
	**	- PlayerPtr must be set before any Techno objects are created
	**	- For GDI or NOD scenarios, PlayerPtr is set by reading the INI;
	**	  but for multiplayer, it's set via the Players vector; so, here we have
	**	  to set various multiplayer variables to fool the Assign_Houses() routine
	**	  into working properly.
	*/
	if (player == SCEN_PLAYER_MPLAYER) {
		Clear_Vector(&Session.Players);

		who = new NodeNameType;
		strcpy(who->Name, Session.Handle);
		who->Player.House = Session.House;
		who->Player.Color = Session.ColorIdx;
		Session.Players.Add (who);

		Session.NumPlayers = 1;
		LastHouse = HOUSE_MULTI1;
	} else {
#ifdef NEVER
	if (ScenPlayer==SCEN_PLAYER_JP) {
		PlayerPtr = HouseClass::As_Pointer(HOUSE_MULTI4);
		PlayerPtr->IsHuman = true;
		Base.House = HOUSE_MULTI4;
	} else {
#endif
		LastHouse = HOUSE_GOOD;
	}

	/*
	**	Blow away everything
	*/
	Clear_Scenario();

	/*
	**	Read the INI
	*/
	if (Read_Scenario_INI(Scen.ScenarioName) == 0) {
		if(Scen.Scenario < 20 && Scen.ScenarioName[2] == 'G'){
	            WWMessageBox().Process("Please insert Red Alert CD1");
		}else if(Scen.Scenario < 20 && Scen.ScenarioName[2] == 'U')
            	    WWMessageBox().Process("Please insert Red Alert CD2");
		else
		    WWMessageBox().Process("Unable to read scenario!");
		HidPage.Clear();
		Flag_To_Redraw(true);
		Render();
	} else {
		Fill_In_Data();
		GamePalette.Set();
//		Set_Palette(GamePalette);
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
	int scen_num;
	ScenarioPlayerType player;
	ScenarioDirType dir;
	ScenarioVarType var;

	Disect_Scenario_Name(Scen.ScenarioName, scen_num, player, dir, var);

	int rc;
//	FILE * fp;
//	char fname[13];

	/*
	**	Prompt for scenario info
	*/
	rc = Pick_Scenario("Save Scenario", scen_num, player, dir, var);
	if (rc != 0) {
		return(-1);
	}

	/*
	**	Warning if scenario already exists
	*/
//	Scen.Set_Scenario_Name(scen_num, player, dir, var);
//	fp = fopen(fname, "rb");
//	if (fp) {
//		fclose(fp);
//		rc = WWMessageBox().Process("File exists. Replace?", TXT_YES, TXT_NO);
//		HidPage.Clear();
//		Flag_To_Redraw(true);
//		Render();
//		if (rc==1) {
//			return(-1);
//		}
//	}

	/*
	**	Set parameters
	*/
//	Scen.Scenario = scen_num;
//	Scen.ScenPlayer = player;
//	Scen.ScenDir = dir;
//	Scen.ScenVar = var;
	Scen.Set_Scenario_Name(scen_num, player, dir, var);

	/*
	**	Player may have changed from GDI to NOD, so change playerptr accordingly
	*/
	switch (player) {
		case SCEN_PLAYER_USSR:
			PlayerPtr = HouseClass::As_Pointer(HOUSE_USSR);
			PlayerPtr->IsHuman = true;
//			Base.House = HOUSE_SPAIN;
			LastHouse = HOUSE_GOOD;
			break;

		case SCEN_PLAYER_SPAIN:
			PlayerPtr = HouseClass::As_Pointer(HOUSE_SPAIN);
			PlayerPtr->IsHuman = true;
//			Base.House = HOUSE_USSR;
			LastHouse = HOUSE_GOOD;
			break;

		case SCEN_PLAYER_GREECE:
			PlayerPtr = HouseClass::As_Pointer(HOUSE_GREECE);
			PlayerPtr->IsHuman = true;
//			Base.House = HOUSE_USSR;
			LastHouse = HOUSE_GOOD;
			break;
	}

	/*
	**	Write the INI
	*/
	Write_Scenario_INI(Scen.ScenarioName);

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
 *   09/04/1996 JLB : Simplified                                           *
 *=========================================================================*/
int MapEditClass::Pick_Scenario(char const * caption, int & scen_nump, ScenarioPlayerType & playerp, ScenarioDirType & dirp, ScenarioVarType & varp)
{
	/*
	**	Dialog & button dimensions
	*/
	enum {
		D_DIALOG_W = 200,											// dialog width
		D_DIALOG_H = 164,											// dialog height
		D_DIALOG_X = ((320 - D_DIALOG_W) / 2),				// centered x-coord
		D_DIALOG_Y = ((200 - D_DIALOG_H) / 2),				// centered y-coord
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),		// coord of x-center

		D_TXT8_H = 11,												// ht of 8-pt text
		D_MARGIN = 7,												// margin width/height

		D_SCEN_W = 45,												// Scenario # width
		D_SCEN_H = 9,												// Scenario # height
		D_SCEN_X = D_DIALOG_CX + 5,							// Scenario # x
		D_SCEN_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H + D_MARGIN,	// Scenario # y

		D_VARA_W = 13,												// Version A width
		D_VARA_H = 9,												// Version A height
		D_VARA_X = D_DIALOG_CX - (D_VARA_W * 5) / 2,		// Version A x
		D_VARA_Y = D_SCEN_Y + D_SCEN_H + D_MARGIN,		// Version A y

		D_VARB_W = 13,												// Version B width
		D_VARB_H = 9,												// Version B height
		D_VARB_X = D_VARA_X + D_VARA_W,						// Version B x
		D_VARB_Y = D_SCEN_Y + D_SCEN_H + D_MARGIN,		// Version B y

		D_VARC_W = 13,												// Version C width
		D_VARC_H = 9,												// Version C height
		D_VARC_X = D_VARB_X + D_VARB_W,						// Version C x
		D_VARC_Y = D_SCEN_Y + D_SCEN_H + D_MARGIN,		// Version C y

		D_VARD_W = 13,												// Version D width
		D_VARD_H = 9,												// Version D height
		D_VARD_X = D_VARC_X + D_VARC_W,						// Version D x
		D_VARD_Y = D_SCEN_Y + D_SCEN_H + D_MARGIN,		// Version D y

		D_VARLOSE_W = 13,											// Version Lose width
		D_VARLOSE_H = 9,											// Version Lose height
		D_VARLOSE_X = D_VARD_X + D_VARD_W,					// Version Lose x
		D_VARLOSE_Y = D_SCEN_Y + D_SCEN_H + D_MARGIN,	// Version Lose y

		D_EAST_W = 50,												// EAST width
		D_EAST_H = 9,												// EAST height
		D_EAST_X = D_DIALOG_CX - D_EAST_W - 5,				// EAST x
		D_EAST_Y = D_VARLOSE_Y + D_VARLOSE_H + D_MARGIN,// EAST y

		D_WEST_W = 50,												// WEST width
		D_WEST_H = 9,												// WEST height
		D_WEST_X = D_DIALOG_CX + 5,							// WEST x
		D_WEST_Y = D_VARLOSE_Y + D_VARLOSE_H + D_MARGIN,// EAST y

		D_GDI_W = 90,												// GDI width
		D_GDI_H = 9,												// GDI height
		D_GDI_X = D_DIALOG_CX - (D_GDI_W / 2),				// GDI x
		D_GDI_Y = D_EAST_Y + D_EAST_H + D_MARGIN,			// GDI y

		D_NOD_W = 90,												// NOD width
		D_NOD_H = 9,												// NOD height
		D_NOD_X = D_DIALOG_CX - (D_NOD_W / 2),				// NOD x
		D_NOD_Y = D_GDI_Y + D_GDI_H,							// NOD y

		D_NEU_W = 90,												// Neutral width
		D_NEU_H = 9,												// Neutral height
		D_NEU_X = D_DIALOG_CX - (D_NOD_W / 2),				// Neutral x
		D_NEU_Y = D_NOD_Y + D_NOD_H,							// Neutral y

		D_MPLAYER_W = 90,											// Multi-Player width
		D_MPLAYER_H = 9,											// Multi-Player height
		D_MPLAYER_X = D_DIALOG_CX - (D_MPLAYER_W / 2),	// Multi-Player x
		D_MPLAYER_Y = D_NEU_Y + D_NEU_H,						// Multi-Player y

		D_OK_W = 45,												// OK width
		D_OK_H = 9,													// OK height
		D_OK_X = D_DIALOG_CX - D_OK_W - 5,					// OK x
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_OK_H - (D_MARGIN+15),	// OK y

		D_CANCEL_W = 45,											// Cancel width
		D_CANCEL_H = 9,											// Cancel height
		D_CANCEL_X = D_DIALOG_CX + 5,							// Cancel x
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - D_CANCEL_H - (D_MARGIN+15), // Cancel y

	};

	/*
	**	Button enumerations
	*/
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
	};

	/*
	**	Dialog variables
	*/
	bool cancel = false;							// true = user cancels

	/*
	**	Other Variables
	*/
	char scen_buf[10]={0};						// buffer for editing scenario #

	/*
	**	Buttons
	*/
	ControlClass * commands = NULL;		// the button list
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	EditClass editbtn (BUTTON_SCENARIO, scen_buf, 5, TPF_EFNT|TPF_NOSHADOW, D_SCEN_X, D_SCEN_Y, D_SCEN_W, D_SCEN_H, EditClass::ALPHANUMERIC);
#else
	EditClass editbtn (BUTTON_SCENARIO, scen_buf, 5, TPF_EFNT|TPF_NOSHADOW, D_SCEN_X, D_SCEN_Y, D_SCEN_W, D_SCEN_H, EditClass::NUMERIC);
#endif

	TextButtonClass varabtn(BUTTON_VAR_A, "A", TPF_EBUTTON, D_VARA_X, D_VARA_Y, D_VARA_W, D_VARA_H);
	TextButtonClass varbbtn(BUTTON_VAR_B, "B", TPF_EBUTTON, D_VARB_X, D_VARB_Y, D_VARB_W, D_VARB_H);
	TextButtonClass varcbtn(BUTTON_VAR_C, "C", TPF_EBUTTON, D_VARC_X, D_VARC_Y, D_VARC_W, D_VARC_H);
	TextButtonClass vardbtn(BUTTON_VAR_D, "D", TPF_EBUTTON, D_VARD_X, D_VARD_Y, D_VARD_W, D_VARD_H);
	TextButtonClass gdibtn(BUTTON_GDI, "North (Spain)", TPF_EBUTTON, D_GDI_X, D_GDI_Y, D_GDI_W, D_GDI_H);
	TextButtonClass nodbtn(BUTTON_NOD, "South (Greece)", TPF_EBUTTON, D_NOD_X, D_NOD_Y, D_NOD_W, D_NOD_H);
	TextButtonClass neubtn(BUTTON_NEUTRAL, HouseTypeClass::As_Reference(HOUSE_USSR).IniName, TPF_EBUTTON, D_NEU_X, D_NEU_Y, D_NEU_W, D_NEU_H);
	TextButtonClass playermbtn(BUTTON_MPLAYER, "Multiplayer", TPF_EBUTTON, D_MPLAYER_X, D_MPLAYER_Y, D_MPLAYER_W, D_MPLAYER_H);
	TextButtonClass eastbtn(BUTTON_EAST, "East", TPF_EBUTTON, D_EAST_X, D_EAST_Y, D_EAST_W, D_EAST_H);
	TextButtonClass westbtn(BUTTON_WEST, "West", TPF_EBUTTON, D_WEST_X, D_WEST_Y, D_WEST_W, D_WEST_H);
	TextButtonClass okbtn(BUTTON_OK, TXT_OK, TPF_EBUTTON, D_OK_X, D_OK_Y, D_OK_W, D_OK_H);
	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_EBUTTON, D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	/*
	**	Initialize
	*/
	Set_Logic_Page(SeenBuff);

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (scen_nump < 100) {
		sprintf(scen_buf, "%d", scen_nump);		// init edit buffer
	} else {
		char first = scen_nump / 36;
		char second = scen_nump % 36;
		scen_buf[0] = first + 'A';
//Mono_Printf("picking map, scen# = %d, first = %c, second = %d (numeric)\n",scen_nump, scen_buf[0],second);Keyboard->Get();Keyboard->Get();
		if (second < 10) {
			scen_buf[1] = second + '0';
		} else {
			scen_buf[1] = (second-10) + 'A';
		}
		scen_buf[2] = 0;
	}
#else
	sprintf(scen_buf, "%d", scen_nump);		// init edit buffer
#endif
	editbtn.Set_Text(scen_buf, 5);

	varabtn.Turn_Off();
	varbbtn.Turn_Off();
	varcbtn.Turn_Off();
	vardbtn.Turn_Off();
	switch (varp) {
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
	}

	/*
	**	Create the button list
	*/
	commands = &editbtn;
	varabtn.Add_Tail(*commands);
	varbbtn.Add_Tail(*commands);
	varcbtn.Add_Tail(*commands);
	vardbtn.Add_Tail(*commands);
	gdibtn.Add_Tail(*commands);
	nodbtn.Add_Tail(*commands);
	neubtn.Add_Tail(*commands);
	playermbtn.Add_Tail(*commands);
	eastbtn.Add_Tail(*commands);
	westbtn.Add_Tail(*commands);
	okbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);

	/*
	**	Init the button states
	*/
	gdibtn.Turn_Off();
	nodbtn.Turn_Off();
	neubtn.Turn_Off();
	playermbtn.Turn_Off();
	if (playerp == SCEN_PLAYER_MPLAYER) {
		playermbtn.Turn_On();
	} else {
		if (PlayerPtr) {
			switch (PlayerPtr->Class->House) {
				case HOUSE_SPAIN:
					gdibtn.Turn_On();
					break;

				case HOUSE_GREECE:
					nodbtn.Turn_On();
					break;

				case HOUSE_USSR:
					neubtn.Turn_On();
					break;
			}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		} else {
			switch (Scen.ScenarioName[2]) {
				case 'G':
					gdibtn.Turn_On();
					break;

				case 'U':
					nodbtn.Turn_On();
					break;

				case 'M':
					playermbtn.Turn_On();
					break;
			}
#endif
		}
	}

	eastbtn.Turn_Off();
	westbtn.Turn_Off();
	if (dirp == SCEN_DIR_EAST) {
		eastbtn.Turn_On();
	} else {
		westbtn.Turn_On();
	}

	/*
	**	Main Processing Loop
	*/
	bool display = true;
	bool process = true;
	while (process) {

		/*
		**	Invoke game callback
		*/
		Call_Back();

		/*
		**	Refresh display if needed
		*/
		if (display) {
			Hide_Mouse();
			Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
			Draw_Caption(caption, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);
			Fancy_Text_Print("Scenario", D_DIALOG_CX - 5, D_SCEN_Y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_RIGHT | TPF_EFNT | TPF_NOSHADOW);
			commands->Draw_All();
			Show_Mouse();

			display = false;
		}

		/*
		**	Get user input
		*/
		KeyNumType input = commands->Input();

		/*
		**	Process input
		*/
		switch (input) {

			/*
			**	Handle a click on one of the scenario variation group buttons.
			*/
			case (BUTTON_VAR_A | KN_BUTTON):
			case (BUTTON_VAR_B | KN_BUTTON):
			case (BUTTON_VAR_C | KN_BUTTON):
			case (BUTTON_VAR_D | KN_BUTTON):
				varabtn.Turn_Off();
				varbbtn.Turn_Off();
				varcbtn.Turn_Off();
				vardbtn.Turn_Off();
				switch (input) {
					case (BUTTON_VAR_A | KN_BUTTON):
						varp = SCEN_VAR_A;
						varabtn.Turn_On();
						break;

					case (BUTTON_VAR_B | KN_BUTTON):
						varp = SCEN_VAR_B;
						varbbtn.Turn_On();
						break;

					case (BUTTON_VAR_C | KN_BUTTON):
						varp = SCEN_VAR_C;
						varcbtn.Turn_On();
						break;

					case (BUTTON_VAR_D | KN_BUTTON):
						varp = SCEN_VAR_D;
						vardbtn.Turn_On();
						break;
				}
				break;

			/*
			**	Handle a click on the east/west variation group.
			*/
			case (BUTTON_EAST | KN_BUTTON):
			case (BUTTON_WEST | KN_BUTTON):
				westbtn.Turn_Off();
				eastbtn.Turn_Off();
				switch (input) {
					case (BUTTON_EAST | KN_BUTTON):
						dirp = SCEN_DIR_EAST;
						eastbtn.Turn_On();
						break;

					case (BUTTON_WEST | KN_BUTTON):
						dirp = SCEN_DIR_WEST;
						westbtn.Turn_On();
						break;
				}
				break;


			/*
			**	Handle a click on one of the player category
			**	group buttons.
			*/
			case (BUTTON_GDI | KN_BUTTON):
			case (BUTTON_NOD | KN_BUTTON):
			case (BUTTON_NEUTRAL | KN_BUTTON):
			case (BUTTON_MPLAYER | KN_BUTTON):
				gdibtn.Turn_Off();
				nodbtn.Turn_Off();
				neubtn.Turn_Off();
				playermbtn.Turn_Off();
				switch (input) {
					case (BUTTON_GDI | KN_BUTTON):
						playerp = SCEN_PLAYER_SPAIN;
						gdibtn.Turn_On();
						break;

					case (BUTTON_NOD | KN_BUTTON):
						playerp = SCEN_PLAYER_GREECE;
						nodbtn.Turn_On();
						break;

					case (BUTTON_NEUTRAL | KN_BUTTON):
						playerp = SCEN_PLAYER_USSR;
						neubtn.Turn_On();
						break;

					case (BUTTON_MPLAYER | KN_BUTTON):
						playerp = SCEN_PLAYER_MPLAYER;
						playermbtn.Turn_On();
						break;
				}
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
	**	Redraw the display
	*/
	HidPage.Clear();
	Flag_To_Redraw(true);
	Render();

	/*
	**	If cancel, just return
	*/
	if (cancel) {
		return(-1);
	}

	/*
	**	Save selections & return
	*/
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (scen_buf[0] <= '9' && scen_buf[1] <= '9') {
		scen_nump = atoi(scen_buf);
	} else {
		char first = scen_buf[0];
		char second = scen_buf[1];
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
		scen_nump = (first * 36) + second;
//Mono_Printf("Converted to: %d, %d = %d\n",first, second, scen_nump);Keyboard->Get();Keyboard->Get();
	}
#else
	scen_nump = atoi(scen_buf);
#endif

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
	/*
	**	Dialog & button dimensions
	*/
	enum {
		D_DIALOG_W = 350,											// dialog width
		D_DIALOG_H = 225,											// dialog height
		D_DIALOG_X = 0,				// centered x-coord
		D_DIALOG_Y = 0,				// centered y-coord
//		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),		// coord of x-center

		D_TXT8_H = 11,												// ht of 8-pt text
		D_MARGIN = 7,												// margin width/height

		D_BORD_X1 = D_DIALOG_X + 45,
//		D_BORD_X1 = D_DIALOG_X + (D_DIALOG_W / 2 - MAP_CELL_W) / 2,
		D_BORD_Y1 = D_DIALOG_Y + 25,
		D_BORD_X2 = D_BORD_X1 + MAP_CELL_W + 1,
		D_BORD_Y2 = D_BORD_Y1 + MAP_CELL_H + 1,

		D_OK_W = 45,												// OK width
		D_OK_H = 9,													// OK height
		D_OK_X = D_DIALOG_X + 45,					// OK x
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_OK_H - (D_MARGIN + 10),	// OK y

		D_CANCEL_W = 45,											// Cancel width
		D_CANCEL_H = 9,											// Cancel height
		D_CANCEL_X = D_DIALOG_X + D_DIALOG_W - (35 + D_CANCEL_W),	// Cancel x
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - D_CANCEL_H - (D_MARGIN + 10), // Cancel y
	};

	/*
	**	Button enumerations:
	*/
	enum {
		BUTTON_OK=100,
		BUTTON_CANCEL,
	};

	/*
	**	Redraw values: in order from "top" to "bottom" layer of the dialog
	*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_MAP,				// includes map interior & coord values
		REDRAW_BACKGROUND,	// includes box, map board, key, coord labels, btns
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;

	/*
	**	Dialog variables:
	*/
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
//	char txt[40];
	int txt_x,txt_y;								// for displaying text
//	unsigned index;								// for drawing map symbology
	CELL cell;										// for drawing map symbology
	int color;										// for drawing map symbology
	ObjectClass * occupier;						// cell's occupier
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	/*
	**	Buttons
	*/
	ControlClass * commands = NULL;

	TextButtonClass okbtn (BUTTON_OK, TXT_OK, TPF_EBUTTON, D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	TextButtonClass cancelbtn (BUTTON_CANCEL, TXT_CANCEL, TPF_EBUTTON, D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	/*
	**	Initialize
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Set up the actual map area relative to the map's border coords
	*/
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
	**	Build the button list
	*/
	commands = &okbtn;
	cancelbtn.Add_Tail(*commands);

	/*
	**	Main processing loop
	*/
	display = REDRAW_ALL;
	bool process = true;
	while (process) {

		/*
		**	Invoke game callback
		*/
		Call_Back();

		/*
		**	Refresh display if needed
		*/
		if (display) {
			Hide_Mouse();

			/*
			**	Redraw the background, map border, key, and coord labels
			*/
			if (display >= REDRAW_BACKGROUND) {

				/*
				**	Background
				*/
				Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
				Draw_Caption(TXT_SIZE_MAP, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);

				/*
				**	Draw the map border
				*/
				if (LogicPage->Lock()) {
					LogicPage->Draw_Rect(D_BORD_X1, D_BORD_Y1, D_BORD_X2, D_BORD_Y2, scheme->Shadow);
//					for (index = D_BORD_X1; index < D_BORD_X2;
//						index += (320/ICON_PIXEL_W)) {
//						LogicPage->Put_Pixel(index, D_BORD_Y1-1, scheme->Shadow);
//						LogicPage->Put_Pixel(index, D_BORD_Y2+1, scheme->Shadow);
//					}
//					for (index = D_BORD_Y1; index < D_BORD_Y2-8;
//						index += (200/ICON_PIXEL_H)) {
//						LogicPage->Put_Pixel(D_BORD_X1-1, index, scheme->Shadow);
//						LogicPage->Put_Pixel(D_BORD_X2+1, index, scheme->Shadow);
//					}

					/*
					**	Draw the map "key"
					*/
					txt_x = D_BORD_X2 + 15;
					txt_y = D_BORD_Y1;
					Plain_Text_Print("Clear Terrain", txt_x, txt_y, GroundColor[LAND_CLEAR], TBLACK, TPF_DROPSHADOW | TPF_EFNT);
					txt_y += 8;
					Plain_Text_Print("Water", txt_x, txt_y, GroundColor[LAND_WATER], TBLACK, TPF_DROPSHADOW | TPF_EFNT);
					txt_y += 8;
					Plain_Text_Print("Tiberium", txt_x, txt_y, GroundColor[LAND_TIBERIUM], TBLACK, TPF_DROPSHADOW | TPF_EFNT);
					txt_y += 8;
					Plain_Text_Print("Rock", txt_x, txt_y, GroundColor[LAND_ROCK], TBLACK, TPF_DROPSHADOW | TPF_EFNT);
					txt_y += 8;
					Plain_Text_Print("Wall", txt_x, txt_y, GroundColor[LAND_WALL], TBLACK, TPF_DROPSHADOW | TPF_EFNT);
					txt_y += 8;
					Plain_Text_Print("Beach", txt_x, txt_y, GroundColor[LAND_BEACH], TBLACK, TPF_DROPSHADOW | TPF_EFNT);
					txt_y += 8;
					Plain_Text_Print("Rough", txt_x, txt_y, GroundColor[LAND_ROUGH], TBLACK, TPF_DROPSHADOW | TPF_EFNT);
					txt_y += 8;
					Plain_Text_Print("River", txt_x, txt_y, GroundColor[LAND_RIVER], TBLACK, TPF_DROPSHADOW | TPF_EFNT);
//					txt_y += 8;
//					Plain_Text_Print("GDI Unit", txt_x, txt_y, YELLOW, TBLACK, TPF_DROPSHADOW | TPF_EFNT);
//					txt_y += 8;
//					Plain_Text_Print("Nod Unit", txt_x, txt_y, RED, TBLACK, TPF_DROPSHADOW | TPF_EFNT);
//					txt_y += 8;
//					Plain_Text_Print("Neutral Unit", txt_x, txt_y, PURPLE, TBLACK, TPF_DROPSHADOW | TPF_EFNT);
					txt_y += 8;
					Plain_Text_Print("Terrain Object", txt_x, txt_y, DKGREEN, TBLACK, TPF_DROPSHADOW | TPF_EFNT);
					txt_y += 8;
					Plain_Text_Print("Starting Cell", txt_x, txt_y, WHITE, TBLACK, TPF_DROPSHADOW | TPF_EFNT);

					/*
					**	Draw the coordinate labels
					*/
					txt_x = D_DIALOG_X + D_DIALOG_W / 8;
					txt_y = D_DIALOG_Y + D_DIALOG_H - D_OK_H - 43;
					Fancy_Text_Print("  X", txt_x, txt_y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_CENTER | TPF_EFNT | TPF_NOSHADOW);

					txt_x += (D_DIALOG_W - 20) / 4;
					Fancy_Text_Print("  Y", txt_x, txt_y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_CENTER | TPF_EFNT | TPF_NOSHADOW);

					txt_x += (D_DIALOG_W - 20) / 4;
					Fancy_Text_Print(" Width", txt_x, txt_y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_CENTER | TPF_EFNT | TPF_NOSHADOW);

					txt_x += (D_DIALOG_W - 20) / 4;
					Fancy_Text_Print(" Height", txt_x, txt_y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_CENTER | TPF_EFNT | TPF_NOSHADOW);

					LogicPage->Unlock();
				}

				/*
				**	Redraw the buttons
				*/
				commands->Flag_List_To_Redraw();
			}

			/*
			**	Redraw the map symbology & location
			*/
			if (display >= REDRAW_MAP) {

				if (LogicPage->Lock()) {

					/*
					**	Erase the map interior
					*/
					LogicPage->Fill_Rect(D_BORD_X1 + 1, D_BORD_Y1 + 1, D_BORD_X2 - 1, D_BORD_Y2 - 1, BLACK);

					/*
					**	Draw Land map symbols (use color according to Ground[] array).
					*/
					for (cell=0; cell < MAP_CELL_TOTAL; cell++) {
						occupier = (*this)[cell].Cell_Occupier();
						if (occupier == NULL) {
							color = GroundColor[(*this)[cell].Land_Type()];
							LogicPage->Put_Pixel(D_BORD_X1 + Cell_X(cell) + 1, D_BORD_Y1 + Cell_Y(cell) + 1, color);
						}
					}

					/*
					**	Draw the actual map location
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

					/*
					**	Draw Unit map symbols (Use the radar map color according to
					**	that specified in the house type class object.
					**	DKGREEN = terrain object
					*/
					for (cell=0; cell < MAP_CELL_TOTAL; cell++) {
						occupier = (*this)[cell].Cell_Occupier();
						if (occupier) {
							color = DKGREEN;
							if (occupier && occupier->Owner() != HOUSE_NONE) {
								color = ColorRemaps[HouseClass::As_Pointer(occupier->Owner())->RemapColor].Color;
							}
							LogicPage->Put_Pixel(D_BORD_X1 + Cell_X(cell) + 1, D_BORD_Y1 + Cell_Y(cell) + 1, color);
						}
					}

					/*
					**	Draw Home location
					*/
					LogicPage->Put_Pixel(D_BORD_X1 + Cell_X(Scen.Waypoint[WAYPT_HOME]) + 1, D_BORD_Y1 + Cell_Y(Scen.Waypoint[WAYPT_HOME]) + 1, WHITE);

					/*
					**	Erase old coordinates
					*/
//					LogicPage->Fill_Rect( D_DIALOG_X + 7,
//						D_DIALOG_Y + D_DIALOG_H - D_OK_H - 22,
//						D_DIALOG_X + D_DIALOG_W - 7,
//						D_DIALOG_Y + D_DIALOG_H - D_OK_H - 22 + 10, BLACK);

					/*
					**	Draw the coordinates
					*/
					txt_x = D_DIALOG_X + D_DIALOG_W / 8;
					txt_y = D_DIALOG_Y + D_DIALOG_H - D_OK_H - 32;
					Fancy_Text_Print("%5d", txt_x, txt_y, GadgetClass::Get_Color_Scheme(), BLACK, TPF_CENTER | TPF_EFNT | TPF_NOSHADOW, map_x1 - D_BORD_X1 - 1);

					txt_x += (D_DIALOG_W - 20) / 4;
					Fancy_Text_Print("%5d", txt_x, txt_y, GadgetClass::Get_Color_Scheme(), BLACK, TPF_CENTER | TPF_EFNT | TPF_NOSHADOW, map_y1 - D_BORD_Y1 - 1);

					txt_x += (D_DIALOG_W - 20) / 4;
					Fancy_Text_Print("%5d", txt_x, txt_y, GadgetClass::Get_Color_Scheme(), BLACK, TPF_CENTER | TPF_EFNT | TPF_NOSHADOW, map_x2 - map_x1 + 1);

					txt_x += (D_DIALOG_W - 20) / 4;
					Fancy_Text_Print("%5d", txt_x, txt_y, GadgetClass::Get_Color_Scheme(), BLACK, TPF_CENTER | TPF_EFNT | TPF_NOSHADOW, map_y2 - map_y1 + 1);

					LogicPage->Unlock();
				}
			}

			Show_Mouse();
			display = REDRAW_NONE;
		}

		/*
		**	Process user input
		*/
		input = commands->Input();

		/*
		**	Normal button processing: This is done when the mouse button is NOT
		**	being held down ('grabbed' is 0).
		*/
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
					**	Grab top left
					*/
					delta1 =  abs(Keyboard->MouseQX - map_x1);
					delta2 =  abs(Keyboard->MouseQY - map_y1);
					if (delta1 < 3 && delta2 < 3) {
						grabbed = 1;
						mx = Keyboard->MouseQX;
						my = Keyboard->MouseQY;
						display = REDRAW_MAP;
						break;
					}

					/*
					**	Grab top right
					*/
					delta1 =  abs(Keyboard->MouseQX - map_x2);
					delta2 =  abs(Keyboard->MouseQY - map_y1);
					if (delta1 < 3 && delta2 < 3) {
						grabbed = 2;
						mx = Keyboard->MouseQX;
						my = Keyboard->MouseQY;
						display = REDRAW_MAP;
						break;
					}

					/*
					**	Grab bottom right
					*/
					delta1 =  abs(Keyboard->MouseQX - map_x2);
					delta2 =  abs(Keyboard->MouseQY - map_y2);
					if (delta1 < 3 && delta2 < 3) {
						grabbed = 3;
						mx = Keyboard->MouseQX;
						my = Keyboard->MouseQY;
						display = REDRAW_MAP;
						break;
					}

					/*
					**	Grab bottom left
					*/
					delta1 =  abs(Keyboard->MouseQX - map_x1);
					delta2 =  abs(Keyboard->MouseQY - map_y2);
					if (delta1 < 3 && delta2 < 3) {
						grabbed = 4;
						mx = Keyboard->MouseQX;
						my = Keyboard->MouseQY;
						display = REDRAW_MAP;
						break;
					}

					/*
					**	Grab the whole map
					*/
					delta1 = abs(Keyboard->MouseQX - ((map_x1 + map_x2) / 2));
					delta2 = abs(Keyboard->MouseQY - ((map_y1 + map_y2) / 2));
					if (delta1 < (map_x2 - map_x1) / 4 &&
						delta2 < (map_y2 - map_y1) / 4) {
						grabbed = 5;
						mx = Keyboard->MouseQX;
						my = Keyboard->MouseQY;
						display = REDRAW_MAP;
					}
					break;

				default:
					break;
			}
		} else {

			/*
			**	Mouse motion processing: This is done while the left mouse button IS
			**	being held down.
			**	- First, check for the button release; if detected, un-grab
			**	- Then, handle mouse motion. WWLIB doesn't pass through a KN_MOUSE_MOVE
			**	  value while the button is being held down, so this case must be
			**	  trapped as a default.
			*/
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
					**	Move top left
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
					**	Move top right
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
					**	Move bottom right
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
					**	Move bottom left
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
					**	Move whole map
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
	**	Redraw the display
	*/
	HidPage.Clear();
	Flag_To_Redraw(true);
	Render();

	/*
	**	If cancel, just return
	*/
	if (cancel) {
		return(-1);
	}

	/*
	**	Save selections
	*/
	MapCellX = map_x1 - D_BORD_X1 - 1;
	MapCellY = map_y1 - D_BORD_Y1 - 1;
	MapCellWidth = map_x2 - map_x1 + 1;
	MapCellHeight = map_y2 - map_y1 + 1;

	/*
	**	Clip Home Cell to new map size
	*/
	if (Cell_X(Scen.Waypoint[WAYPT_HOME]) < MapCellX) {
		Scen.Waypoint[WAYPT_HOME] = XY_Cell(MapCellX, Cell_Y(Scen.Waypoint[WAYPT_HOME]));
	}

	if (Cell_X(Scen.Waypoint[WAYPT_HOME]) > MapCellX + MapCellWidth - 1) {
		Scen.Waypoint[WAYPT_HOME] = XY_Cell(MapCellX + MapCellWidth - 1, Cell_Y(Scen.Waypoint[WAYPT_HOME]));
	}

	if (Cell_Y(Scen.Waypoint[WAYPT_HOME]) < MapCellY) {
		Scen.Waypoint[WAYPT_HOME] = XY_Cell(Cell_X(Scen.Waypoint[WAYPT_HOME]), MapCellY);
	}

	if (Cell_Y(Scen.Waypoint[WAYPT_HOME]) > MapCellY + MapCellHeight - 1) {
		Scen.Waypoint[WAYPT_HOME] = XY_Cell(Cell_X(Scen.Waypoint[WAYPT_HOME]), MapCellY + MapCellHeight - 1);
	}

	return(0);
}


/***************************************************************************
 * MapEditClass::Scenario_Dialog -- scenario global parameters dialog      *
 *                                                                         *
 *    Edits the house specific and general scenario options.               *
 *                                                                         *
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
 *   02/13/1996 JLB : Revamped to new system.                              *
 *=========================================================================*/
int MapEditClass::Scenario_Dialog(void)
{
	TheaterType orig_theater = Scen.Theater;		// original theater
	HousesType house = PlayerPtr->Class->House;
	HousesType newhouse = house;
	HouseStaticClass hdata[HOUSE_COUNT];

	/*
	**	Fill in the house data for each house that exists.
	*/
	for (HousesType h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
		HouseClass * hptr = HouseClass::As_Pointer(h);
		if (hptr) {
			hdata[h] = hptr->Control;
		}
	}

	/*
	**	Dialog & button dimensions
	*/
	enum {
		D_DIALOG_W = 320 * RESFACTOR,
		D_DIALOG_H = 200 * RESFACTOR,
		D_DIALOG_X = ((320 * RESFACTOR - D_DIALOG_W) / 2),
		D_DIALOG_Y = ((200 * RESFACTOR - D_DIALOG_H) / 2),

		D_OK_W = 45,
		D_OK_H = 9,
		D_OK_X = D_DIALOG_X + 15 * RESFACTOR,
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - 15 * RESFACTOR,

		D_CANCEL_W = 45,
		D_CANCEL_H = 9,
		D_CANCEL_X = D_DIALOG_X + D_DIALOG_W - (D_CANCEL_W+15*RESFACTOR),
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - 15*RESFACTOR
	};

	/*
	**	Button enumerations:
	*/
	enum {
		LIST_THEATER=100,
		BUTTON_DESCRIPTION,
		BUTTON_ALLIES,
		BUTTON_CONTROL,
		BUTTON_SMARTIES,
		BUTTON_BASE,
		BUTTON_NOSPYPLANE,
		BUTTON_INHERIT,
		BUTTON_TIMER,
		BUTTON_THEME,
		BUTTON_RECORD,
		BUTTON_EVAC,
		BUTTON_MONEYTIB,
		BUTTON_TECH,
		BUTTON_TRUCKCRATE,
		BUTTON_ENDOFGAME,
		BUTTON_SKIPSCORE,
		BUTTON_ONETIME,
		BUTTON_NOMAPSEL,
		BUTTON_HOUSE,
		BUTTON_CREDITS,
		BUTTON_SOURCE,
		BUTTON_MAXUNIT,
		BUTTON_INTRO,
		BUTTON_BRIEFING,
		BUTTON_ACTION,
		BUTTON_WIN,
		BUTTON_LOSE,
		BUTTON_OK,
		BUTTON_CANCEL,
	};

	/*
	**	Initialize
	*/
	Set_Logic_Page(SeenBuff);

	ControlClass * commands = NULL;				// the button list

	/*
	**	Theater choice drop down list.
	*/
	char theatertext[45] = "";
	DropListClass theaterbtn(LIST_THEATER, theatertext, sizeof(theatertext)-1,
		TPF_EFNT|TPF_NOSHADOW,
		D_DIALOG_X+15*RESFACTOR, D_DIALOG_Y+30, 65, 8*5,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (TheaterType t = THEATER_FIRST; t < THEATER_COUNT; t++) {
		theaterbtn.Add_Item(Theaters[t].Name);
	}
	theaterbtn.Set_Selected_Index(orig_theater);

	char description[DESCRIP_MAX] = "";
	strcpy(description, Scen.Description);
	EditClass desc(BUTTON_DESCRIPTION, description, sizeof(description), TPF_EFNT|TPF_NOSHADOW, theaterbtn.X+theaterbtn.Width+15, theaterbtn.Y, 160);

	/*
	**	Button that tells if this scenario should inherit buildings from the previous.
	*/
	CheckBoxClass inherit(BUTTON_INHERIT, theaterbtn.X+theaterbtn.Width+15+250, theaterbtn.Y);
	if (Scen.IsToInherit) {
		inherit.Turn_On();
	} else {
		inherit.Turn_Off();
	}

	/*
	**	Records scenario disposition into holding slot.
	*/
	CheckBoxClass record(BUTTON_RECORD, inherit.X, inherit.Y+8);
	if (Scen.IsToCarryOver) {
		record.Turn_On();
	} else {
		record.Turn_Off();
	}

	/*
	**	Should Tanya/civilian be automatically evacuated?
	*/
	CheckBoxClass tanya(BUTTON_EVAC, record.X, record.Y+8);
	if (Scen.IsTanyaEvac) {
		tanya.Turn_On();
	} else {
		tanya.Turn_Off();
	}

	/*
	**	End of game with with scenario?
	*/
	CheckBoxClass endofgame(BUTTON_ENDOFGAME, tanya.X, tanya.Y+8);
	if (Scen.IsEndOfGame) {
		endofgame.Turn_On();
	} else {
		endofgame.Turn_Off();
	}

	/*
	**	Timer inherit logic.
	*/
	CheckBoxClass timercarry(BUTTON_TIMER, endofgame.X, endofgame.Y+8);
	if (Scen.IsInheritTimer) {
		timercarry.Turn_On();
	} else {
		timercarry.Turn_Off();
	}

	/*
	**	Disable spy plane option?
	*/
	CheckBoxClass nospyplane(BUTTON_NOSPYPLANE, timercarry.X, timercarry.Y+8);
	if (Scen.IsNoSpyPlane) {
		nospyplane.Turn_On();
	} else {
		nospyplane.Turn_Off();
	}

	/*
	**	Skip the score screen?
	*/
	CheckBoxClass skipscore(BUTTON_SKIPSCORE, nospyplane.X, nospyplane.Y+8);
	if (Scen.IsSkipScore) {
		skipscore.Turn_On();
	} else {
		skipscore.Turn_Off();
	}

	/*
	**	Skip the map selection screen for next mission. Presume goes to
	**	variation "B"?
	*/
	CheckBoxClass nomapsel(BUTTON_NOMAPSEL, skipscore.X, skipscore.Y+8);
	if (Scen.IsNoMapSel) {
		nomapsel.Turn_On();
	} else {
		nomapsel.Turn_Off();
	}

	/*
	**	Return to main menu after mission completes?
	*/
	CheckBoxClass onetime(BUTTON_ONETIME, nomapsel.X, nomapsel.Y+8);
	if (Scen.IsOneTimeOnly) {
		onetime.Turn_On();
	} else {
		onetime.Turn_Off();
	}

	/*
	**	Trucks carry a wood crate?
	*/
	CheckBoxClass truckcrate(BUTTON_TRUCKCRATE, onetime.X, onetime.Y+8);
	if (Scen.IsTruckCrate) {
		truckcrate.Turn_On();
	} else {
		truckcrate.Turn_Off();
	}

	/*
	**	Transfer credits into tiberium storage at scenario start?
	*/
	CheckBoxClass moneytib(BUTTON_MONEYTIB, truckcrate.X, truckcrate.Y+8);
	if (Scen.IsMoneyTiberium) {
		moneytib.Turn_On();
	} else {
		moneytib.Turn_Off();
	}

	/*
	**	Intro movie name.
	*/
	char introtext[_MAX_FNAME+_MAX_EXT];
	DropListClass intro(BUTTON_INTRO, introtext, sizeof(introtext),
		TPF_EFNT|TPF_NOSHADOW,
		theaterbtn.X, theaterbtn.Y+theaterbtn.Height+24, 50, 7*10,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	intro.Add_Item("<none>");
	for (VQType v = VQ_FIRST; v < VQ_COUNT; v++) {
		intro.Add_Item(VQName[v]);
	}
	intro.Set_Selected_Index((int)Scen.IntroMovie + 1);

	/*
	**	Briefing movie name.
	*/
	char brieftext[_MAX_FNAME+_MAX_EXT];
	DropListClass briefing(BUTTON_BRIEFING, brieftext, sizeof(brieftext),
		TPF_EFNT|TPF_NOSHADOW,
		intro.X+intro.Width+10, intro.Y, 50, 7*10,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	briefing.Add_Item("<none>");
	for (v = VQ_FIRST; v < VQ_COUNT; v++) {
		briefing.Add_Item(VQName[v]);
	}
	briefing.Set_Selected_Index((int)Scen.BriefMovie + 1);

	char actiontext[_MAX_FNAME+_MAX_EXT];
	DropListClass action(BUTTON_ACTION, actiontext, sizeof(actiontext),
		TPF_EFNT|TPF_NOSHADOW,
		briefing.X+briefing.Width+10, briefing.Y, 50, 7*10,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	action.Add_Item("<none>");
	for (v = VQ_FIRST; v < VQ_COUNT; v++) {
		action.Add_Item(VQName[v]);
	}
	action.Set_Selected_Index((int)Scen.ActionMovie + 1);

	char wintext[_MAX_FNAME+_MAX_EXT];
	DropListClass win(BUTTON_WIN, wintext, sizeof(wintext),
		TPF_EFNT|TPF_NOSHADOW,
		action.X+action.Width+10, action.Y, 50, 7*10,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	win.Add_Item("<none>");
	for (v = VQ_FIRST; v < VQ_COUNT; v++) {
		win.Add_Item(VQName[v]);
	}
	win.Set_Selected_Index((int)Scen.WinMovie + 1);

	char losetext[_MAX_FNAME+_MAX_EXT];
	DropListClass lose(BUTTON_LOSE, losetext, sizeof(losetext),
		TPF_EFNT|TPF_NOSHADOW,
		win.X+win.Width+10, win.Y, 50, 7*10,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	lose.Add_Item("<none>");
	for (v = VQ_FIRST; v < VQ_COUNT; v++) {
		lose.Add_Item(VQName[v]);
	}
	lose.Set_Selected_Index((int)Scen.LoseMovie + 1);

	/*
	**	House choice list.
	*/
	ListClass housebtn(BUTTON_HOUSE,
		D_DIALOG_X+15*RESFACTOR, D_DIALOG_Y+105, 55, 7*10,
		TPF_EFNT|TPF_NOSHADOW,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
		housebtn.Add_Item(HouseTypeClass::As_Reference(h).IniName);
	}
	housebtn.Set_Selected_Index(PlayerPtr->Class->House);

	/*
	**	Base house choice drop down list.
	*/
	char basetext[35];
	DropListClass basebtn(BUTTON_BASE, basetext, sizeof(basetext),
		TPF_EFNT|TPF_NOSHADOW,
		D_DIALOG_X+15*RESFACTOR, D_DIALOG_Y+80, 65, 7*10,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
		basebtn.Add_Item(HouseTypeClass::As_Reference(h).IniName);
	}
	if (Base.House != HOUSE_NONE) {
		basebtn.Set_Selected_Index(Base.House);
	}

	/*
	**	Opening scenario theme.
	*/
	char themetext[65];
	DropListClass themebtn(BUTTON_THEME, themetext, sizeof(themetext),
		TPF_EFNT|TPF_NOSHADOW,
		basebtn.X+basebtn.Width+15*RESFACTOR, basebtn.Y, 85, 7*10,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	themebtn.Add_Item("<none>");
	for (ThemeType th = THEME_FIRST; th < THEME_COUNT; th++) {
		themebtn.Add_Item(Theme.Full_Name(th));
	}
	if (Scen.TransitTheme != THEME_NONE) {
		themebtn.Set_Selected_Index(Scen.TransitTheme+1);
	} else {
		themebtn.Set_Selected_Index(0);
	}

	/*
	**	Build level (technology).
	*/
	SliderClass techlevel(BUTTON_TECH, housebtn.X+housebtn.Width+15, housebtn.Y, 100, 8);
	techlevel.Set_Maximum(16);

	char statictechbuff[15];
	StaticButtonClass techstatic(0, "999", TPF_EFNT|TPF_NOSHADOW, techlevel.X+techlevel.Width-20, techlevel.Y-7);

	/*
	**	Starting credits.
	*/
	SliderClass creditbtn(BUTTON_CREDITS, housebtn.X+housebtn.Width+15, techlevel.Y+20, 100, 8);
	creditbtn.Set_Maximum(201);

	char staticcreditbuff[15];
	StaticButtonClass creditstatic(0, "999999999", TPF_EFNT|TPF_NOSHADOW, creditbtn.X+creditbtn.Width-50, creditbtn.Y-7);

	/*
	**	Maximum unit/infantry slider.
	*/
	SliderClass maxunit(BUTTON_MAXUNIT, housebtn.X+housebtn.Width+15, creditbtn.Y+20, 100, 8);
	maxunit.Set_Maximum(501);

	char staticmaxunitbuff[15];
	StaticButtonClass maxunitstatic(0, "999999", TPF_EFNT|TPF_NOSHADOW, maxunit.X+maxunit.Width-30, maxunit.Y-7);

	/*
	**	Source of ground delivery reinforcements.
	*/
	char sourcetext[25] = "";
	ListClass sourcebtn(BUTTON_SOURCE,
		housebtn.X+housebtn.Width+15, maxunit.Y+20, 100, 7*4,
		TPF_EFNT|TPF_NOSHADOW,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (SourceType source = SOURCE_FIRST; source <= SOURCE_WEST; source++) {
		sourcebtn.Add_Item(SourceName[source]);
	}

	/*
	**	Smartness lider.
	*/
	SliderClass smarties(BUTTON_SMARTIES, sourcebtn.X, sourcebtn.Y+sourcebtn.Height+15, 35, 8);
	smarties.Set_Maximum(Rule.MaxIQ+1);

	char staticsmartiesbuff[15];
	StaticButtonClass smartiesstatic(0, "9999", TPF_EFNT|TPF_NOSHADOW, smarties.X+smarties.Width-20, smarties.Y-7);

	/*
	**	List box of who is allied with whom.
	*/
	CheckListClass allies(BUTTON_ALLIES,
		techlevel.X+techlevel.Width+5, housebtn.Y, 65, 7*10,
		TPF_EFNT|TPF_NOSHADOW,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
		allies.Add_Item(HouseTypeClass::As_Reference(h).IniName);
		if (hdata[house].Allies & (1L << h)) {
			allies.Check_Item(h, true);
		}
	}
	allies.Set_Selected_Index(0);

	/*
	**	List box of who the player can control.
	*/
	CheckListClass control(BUTTON_CONTROL,
		allies.X+allies.Width+10, housebtn.Y, 65, 7*10,
		TPF_EFNT|TPF_NOSHADOW,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
		control.Add_Item(HouseTypeClass::As_Reference(h).IniName);
		if (HouseClass::As_Pointer(h)->IsPlayerControl) {
			control.Check_Item(h, true);
		}
	}
	control.Set_Selected_Index(0);

	/*
	**	Create the ubiquitous "ok" and "cancel" buttons.
	*/
	TextButtonClass okbtn(BUTTON_OK, TXT_OK, TPF_EBUTTON, D_OK_X, D_OK_Y, D_OK_W, D_OK_H);
	TextButtonClass cancelbtn(BUTTON_CANCEL, TXT_CANCEL, TPF_EBUTTON, D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	/*
	**	Create the list
	*/
	commands = &okbtn;
	cancelbtn.Add_Tail(*commands);
	theaterbtn.Add_Tail(*commands);
	themebtn.Add_Tail(*commands);
	housebtn.Add_Tail(*commands);
	techlevel.Add_Tail(*commands);
	techstatic.Add_Tail(*commands);
	sourcebtn.Add_Tail(*commands);
	creditbtn.Add_Tail(*commands);
	creditstatic.Add_Tail(*commands);
	maxunitstatic.Add_Tail(*commands);
	moneytib.Add_Tail(*commands);
	smartiesstatic.Add_Tail(*commands);
	allies.Add_Tail(*commands);
	control.Add_Tail(*commands);
	maxunit.Add_Tail(*commands);
	nospyplane.Add_Tail(*commands);
	skipscore.Add_Tail(*commands);
	nomapsel.Add_Tail(*commands);
	onetime.Add_Tail(*commands);
	inherit.Add_Tail(*commands);
	timercarry.Add_Tail(*commands);
	tanya.Add_Tail(*commands);
	record.Add_Tail(*commands);
	truckcrate.Add_Tail(*commands);
	endofgame.Add_Tail(*commands);
	briefing.Add_Tail(*commands);
	intro.Add_Tail(*commands);
	action.Add_Tail(*commands);
	win.Add_Tail(*commands);
	lose.Add_Tail(*commands);
	basebtn.Add_Tail(*commands);
	smarties.Add_Tail(*commands);
	desc.Add_Tail(*commands);

	/*
	**	Main Processing Loop
	*/
	bool housechange = true;
	bool display = true;
	bool process = true;
	bool cancel = false;		// true = user cancels
	bool dotext = true;		// display the text.
	bool fetch = false;		// Fetch data from dialog into tracking structure.
	//Set_Logic_Page(SeenBuff);
	while (process) {

		/*
		**	Invoke game callback
		*/
		Call_Back();

		/*
		**	If the house changes, then all the gadgets that reflect the settings of the
		**	house should change as well.
		*/
		if (housechange) {
			HouseStaticClass * hstatic = &hdata[newhouse];
			creditbtn.Set_Value(hstatic->InitialCredits/100);
			techlevel.Set_Value(hstatic->TechLevel);
			sourcebtn.Set_Selected_Index(hstatic->Edge);
			maxunit.Set_Value(hstatic->MaxUnit + hstatic->MaxInfantry);
			for (h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
				allies.Check_Item(h, hstatic->Allies & (1L << h));
			}
			smarties.Set_Value(hstatic->IQ);

			house = newhouse;
			housechange = false;
			display = true;
		}

		/*
		**	Refresh display if needed
		*/
		if (display) {
			Hide_Mouse();

			/*
			**	Draw the background
			*/
			Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
			Draw_Caption(TXT_SCENARIO_OPTIONS, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);

			/*
			**	Display the text that doesn't need drawing except when the entire dialog
			**	needs to be redrawn.
			*/
			Fancy_Text_Print("Tech Level =", techlevel.X, techlevel.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Credits =", creditbtn.X, creditbtn.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Unit Max =", maxunit.X, maxunit.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("IQ =", smarties.X, smarties.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Prebuild Base:", basebtn.X, basebtn.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Theater:", theaterbtn.X, theaterbtn.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Scenario Name:", desc.X, desc.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Country:", housebtn.X, housebtn.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Home Edge:", sourcebtn.X, sourcebtn.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Allies:", allies.X, allies.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Plyr Control:", control.X, control.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Theme:", themebtn.X, themebtn.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Intro:", intro.X, intro.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Briefing:", briefing.X, briefing.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Action:", action.X, action.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Win:", win.X, win.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Lose:", lose.X, lose.Y-7, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Store scenario?", record.X+10, record.Y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Inherit stored scenario?", inherit.X+10, inherit.Y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Auto evac. Tanya (civilian)?", tanya.X+10, tanya.Y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Last mission of game?", endofgame.X+10, endofgame.Y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Inherit mission timer from last scenario?", timercarry.X+10, timercarry.Y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Disable spy plane?", nospyplane.X+10, nospyplane.Y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Skip the score screen?", skipscore.X+10, skipscore.Y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("No map selection (force var 'B')?", nomapsel.X+10, nomapsel.Y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Return to main menu after scenario finishes?", onetime.X+10, onetime.Y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Truck carries wood crate?", truckcrate.X+10, truckcrate.Y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);
			Fancy_Text_Print("Initial money is transferred to silos?", moneytib.X+10, moneytib.Y, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_EFNT|TPF_NOSHADOW);

			theaterbtn.Collapse();
			themebtn.Collapse();
			intro.Collapse();
			briefing.Collapse();
			action.Collapse();
			win.Collapse();
			lose.Collapse();
			basebtn.Collapse();
			commands->Draw_All(true);
			Show_Mouse();
			display = false;
			dotext = true;
		}

		/*
		**	Display the text of the buttons that could change their text as a
		**	result of slider interaction.
		*/
		if (dotext) {
			dotext = false;
			Hide_Mouse();

			sprintf(statictechbuff, "%2d", techlevel.Get_Value());
			techstatic.Set_Text(statictechbuff);
			techstatic.Draw_Me();

			sprintf(staticcreditbuff, "$%-7d", creditbtn.Get_Value() * 100);
			creditstatic.Set_Text(staticcreditbuff);
			creditstatic.Draw_Me();

			sprintf(staticmaxunitbuff, "%4d", maxunit.Get_Value());
			maxunitstatic.Set_Text(staticmaxunitbuff);
			maxunitstatic.Draw_Me();

			sprintf(staticsmartiesbuff, "%2d", smarties.Get_Value());
			smartiesstatic.Set_Text(staticsmartiesbuff);
			smartiesstatic.Draw_Me();

			Show_Mouse();
		}

		/*
		**	Get user input
		*/
		KeyNumType input = commands->Input();

		/*
		**	Process input
		*/
		switch (input) {
			case BUTTON_ALLIES|KN_BUTTON:
				allies.Check_Item(house, true);
				break;

			case BUTTON_CONTROL|KN_BUTTON:
				control.Check_Item(house, true);
				break;

			case BUTTON_THEME|KN_BUTTON:
			case BUTTON_INTRO|KN_BUTTON:
			case BUTTON_BRIEFING|KN_BUTTON:
			case BUTTON_ACTION|KN_BUTTON:
			case BUTTON_WIN|KN_BUTTON:
			case BUTTON_LOSE|KN_BUTTON:
			case BUTTON_BASE|KN_BUTTON:
			case LIST_THEATER|KN_BUTTON:
				briefing.Collapse();
				action.Collapse();
				win.Collapse();
				themebtn.Collapse();
				intro.Collapse();
				lose.Collapse();
				basebtn.Collapse();
				theaterbtn.Collapse();
				display = true;
				break;

			case BUTTON_SMARTIES|KN_BUTTON:
			case BUTTON_MAXUNIT|KN_BUTTON:
			case BUTTON_CREDITS|KN_BUTTON:
			case BUTTON_TECH|KN_BUTTON:
				briefing.Collapse();
				action.Collapse();
				win.Collapse();
				lose.Collapse();
				basebtn.Collapse();
				themebtn.Collapse();
				theaterbtn.Collapse();
				dotext = true;
				break;

			case BUTTON_HOUSE|KN_BUTTON:
				newhouse = HousesType(housebtn.Current_Index());
				housechange = true;
				briefing.Collapse();
				action.Collapse();
				themebtn.Collapse();
				win.Collapse();
				intro.Collapse();
				lose.Collapse();
				basebtn.Collapse();
				theaterbtn.Collapse();
				fetch = true;
				break;

			case (KN_RETURN):
			case (BUTTON_OK | KN_BUTTON):
				cancel = false;
				process = false;
				fetch = true;
				break;

			case (KN_ESC):
			case (BUTTON_CANCEL | KN_BUTTON):
				cancel = true;
				process = false;
				break;

			default:
				break;
		}


		/*
		**	If the house changes, then all the gadgets that reflect the settings of the
		**	house should change as well.
		*/
		if (fetch) {
			fetch = false;
			HouseStaticClass * hstatic = &hdata[house];

			Base.House = HousesType(basebtn.Current_Index());
			hstatic->InitialCredits = creditbtn.Get_Value() * 100;
			hstatic->Edge = SourceType(sourcebtn.Current_Index());
			hstatic->TechLevel = techlevel.Get_Value();
			hstatic->MaxUnit = maxunit.Get_Value()/2;
			hstatic->MaxInfantry = maxunit.Get_Value()/2;
			hstatic->IQ = smarties.Get_Value();
			for (h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
				if (allies.Is_Checked(h)) {
					hstatic->Allies |= (1L << h);
				} else {
					hstatic->Allies &= ~(1L << h);
				}
			}
		}
	}

	/*
	**	Redraw the map
	*/
	HidPage.Clear();
	Flag_To_Redraw(true);
	Render();

	/*
	**	If cancel, just return
	*/
	if (cancel) {
		return(-1);
	}

	/*
	**	Copy the dialog data back into the appropriate game data locations.
	*/
	for (h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
		HouseClass * hptr = HouseClass::As_Pointer(h);
		if (hptr != NULL) {
			hptr->Control = hdata[h];
			hptr->Allies = hdata[h].Allies;

			if (control.Is_Checked(h)) {
				hptr->IsPlayerControl = true;
			} else {
				hptr->IsPlayerControl = false;
			}
		}
	}
	PlayerPtr->IsPlayerControl = true;
	strncpy(Scen.Description, desc.Get_Text(), sizeof(Scen.Description));
	Scen.Description[sizeof(Scen.Description)-1] = '\0';
	Scen.IntroMovie = VQType(intro.Current_Index()-1);
	Scen.BriefMovie = VQType(briefing.Current_Index()-1);
	Scen.ActionMovie = VQType(action.Current_Index()-1);
	Scen.WinMovie = VQType(win.Current_Index()-1);
	Scen.LoseMovie = VQType(lose.Current_Index()-1);
	Scen.IsToInherit = inherit.IsOn;
	Scen.IsToCarryOver = record.IsOn;
	Scen.IsTanyaEvac = tanya.IsOn;
	Scen.IsEndOfGame = endofgame.IsOn;
	Scen.IsInheritTimer = timercarry.IsOn;
	Scen.IsNoSpyPlane = nospyplane.IsOn;
	Scen.IsSkipScore = skipscore.IsOn;
	Scen.IsNoMapSel = nomapsel.IsOn;
	Scen.IsOneTimeOnly = onetime.IsOn;
	Scen.IsTruckCrate = truckcrate.IsOn;
	Scen.IsMoneyTiberium = moneytib.IsOn;
	Scen.TransitTheme = ThemeType(themebtn.Current_Index()-1);

	/*
	**	Change the theater:
	**	- 1st set the Theater global
	**	- scan all cells to check their TType for compatibility with the new
	**	  theater; if not compatible, set TType to TEMPLATE_NONE & TIcon to 0
	**	- Then, re-initialize the TypeClasses for the new Theater
	*/
	TheaterType theater = TheaterType(theaterbtn.Current_Index());
	if (theater != orig_theater) {

		unsigned char theater_mask;	// template/terrain mask
		TerrainClass * terrain;	// cell's terrain pointer

		/*
		**	Loop through all cells
		*/
		for (CELL i = 0; i < MAP_CELL_TOTAL; i++) {

			/*
			**	If this cell has a template icon & that template isn't compatible
			**	with this theater, set the icon to NONE
			*/
			if ((*this)[i].TType != TEMPLATE_NONE) {
				theater_mask = TemplateTypeClass::As_Reference((*this)[i].TType).Theater;
				if ( (theater_mask & (1 << theater))==0) {
					(*this)[i].TType = TEMPLATE_NONE;
					(*this)[i].TIcon = 0;
				}
			}

			/*
			**	If this cell has terrain in it, and that terrain isn't compatible
			**	with this theater, delete the terrain object.
			*/
			terrain = (*this)[i].Cell_Terrain();
			if (terrain != NULL) {
				theater_mask = terrain->Class->Theater;
				if ( (theater_mask & (1<<theater))==0) {
					delete terrain;
				}
			}
		}

		/*
		** Remove any old theater specific uncompressed shapes
		*/
#ifdef WIN32
		Reset_Theater_Shapes();
#endif	//WIN32
		/*
		** Force shapes to reload
		*/
		LastTheater = THEATER_NONE;

		/*
		**	Re-init the object Type Classes for this theater
		*/
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
		VesselTypeClass::Init(theater);
		SmudgeTypeClass::Init(theater);

//		LastTheater = theater;
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

	/*
	**	Trigger dialog processing loop:
	**	- Invoke the trigger selection dialog. If a trigger's selected, break
	**	  & return
	**	- If user wants to edit the current trigger, do so
	**	- If user wants to create new trigger, new a TriggerClass & edit it
	**	- If user wants to delete trigger, delete the current trigger
	**	- Keep looping until 'OK'
	*/
	while (1) {

		/*
		**	Select trigger
		*/
		rc = Select_Trigger();

		/*
		**	'OK'; break
		*/
		if (rc==0) break;

		/*
		**	'Edit'
		*/
		if (rc==1 && CurTrigger) {
			if (CurTrigger->Edit()) {
				Changed = 1;
			}
			HidPage.Clear();
			Flag_To_Redraw(true);
			Render();
		}

		/*
		**	'New'
		*/
		if (rc==2) {

			/*
			**	Create a new trigger
			*/
			CurTrigger = new TriggerTypeClass();
			if (CurTrigger) {

				/*
				**	delete it if user cancels
				*/
				if (!CurTrigger->Edit()) {
					delete CurTrigger;
					CurTrigger = NULL;
				} else {
					Changed = 1;
				}
				HidPage.Clear();
				Flag_To_Redraw(true);
				Render();

			} else {

				/*
				**	Unable to create; issue warning
				*/
				WWMessageBox().Process("No more triggers available.");
				HidPage.Clear();
				Flag_To_Redraw(true);
				Render();
			}
		}

		/*
		**	'Delete'
		*/
		if (rc==3) {
			if (CurTrigger) {
				Detach_This_From_All(CurTrigger->As_Target(), true);
				delete CurTrigger;
				//CurTrigger->Remove();
				CurTrigger = NULL;
				Changed = 1;
			}
		}
	}

	/*
	**	Let the CurTrigger global exist if the trigger can be placed on the
	**	ground or on a game object.
	*/
	if (CurTrigger && !(CurTrigger->Attaches_To() & (ATTACH_OBJECT|ATTACH_CELL))) {
		CurTrigger = NULL;
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
 *   05/07/1996 JLB : Streamlined and sort trigger list.                   *
 *=========================================================================*/
int MapEditClass::Select_Trigger(void)
{
	/*
	**	Dialog & button dimensions
	*/
	enum {
		D_DIALOG_W = 400,
		D_DIALOG_H = 250,
		D_DIALOG_X = 0,
		D_DIALOG_Y = 0,

		D_TXT8_H = 11,
		D_MARGIN = 35,

		D_LIST_W = (D_DIALOG_W-(D_MARGIN*2))-10,
		D_LIST_H = D_DIALOG_H-70,
		D_LIST_X = D_DIALOG_X + (D_DIALOG_W-D_LIST_W)/2,
		D_LIST_Y = D_DIALOG_Y + 25,

		BUTTON_W = 45,
		BUTTON_H = 9,

		D_EDIT_W = BUTTON_W,
		D_EDIT_H = BUTTON_H,
		D_EDIT_X = D_DIALOG_X + D_DIALOG_W - (((D_EDIT_W+10)*4)+25),
		D_EDIT_Y = D_DIALOG_Y + D_DIALOG_H - 20 - D_EDIT_H,

		D_NEW_W = BUTTON_W,
		D_NEW_H = BUTTON_H,
		D_NEW_X = D_EDIT_X + D_EDIT_W + 10,
		D_NEW_Y = D_DIALOG_Y + D_DIALOG_H - 20 - D_NEW_H,

		D_DELETE_W = BUTTON_W,
		D_DELETE_H = BUTTON_H,
		D_DELETE_X = D_NEW_X + D_NEW_W + 10,
		D_DELETE_Y = D_DIALOG_Y + D_DIALOG_H - 20 - D_DELETE_H,

		D_OK_W = BUTTON_W,
		D_OK_H = BUTTON_H,
		D_OK_X = D_DELETE_X + D_DELETE_W + 10,
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - 20 - D_OK_H,

	};

	/*
	**	Button enumerations:
	*/
	enum {
		TRIGGER_LIST=100,
		BUTTON_EDIT,
		BUTTON_NEW,
		BUTTON_DELETE,
		BUTTON_OK,
	};

	/*
	**	Dialog variables:
	*/
	bool edit_trig = false;						// true = user wants to edit
	bool new_trig = false;						// true = user wants to new
	bool del_trig = false;						// true = user wants to new
	int i;											// loop counter

	/*
	**	Buttons
	*/
	ControlClass * commands = NULL;				// the button list

	TListClass<CCPtr<TriggerTypeClass> > triggerlist(TRIGGER_LIST, D_LIST_X, D_LIST_Y, D_LIST_W, D_LIST_H,
		TPF_EFNT|TPF_NOSHADOW,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));

	TextButtonClass editbtn(BUTTON_EDIT, "Edit", TPF_EBUTTON, D_EDIT_X, D_EDIT_Y, D_EDIT_W, D_EDIT_H);
	TextButtonClass newbtn(BUTTON_NEW, "New", TPF_EBUTTON, D_NEW_X, D_NEW_Y, D_NEW_W, D_NEW_H);
	TextButtonClass deletebtn(BUTTON_DELETE, "Delete", TPF_EBUTTON, D_DELETE_X, D_DELETE_Y, D_DELETE_W, D_DELETE_H);
	TextButtonClass okbtn(BUTTON_OK, TXT_OK, TPF_EBUTTON, D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	/*
	**	Initialize
	*/
	Set_Logic_Page(SeenBuff);

	/*
	**	Fill in the list box
	*/
	for (i = 0; i < TriggerTypes.Count(); i++) {
		triggerlist.Add_Item(TriggerTypes.Ptr(i));
	}

	PNBubble_Sort(&triggerlist[0], triggerlist.Count());

	if (CurTrigger) {
		triggerlist.Set_Selected_Index(CurTrigger);
	} else {
		triggerlist.Set_Selected_Index(0);
	}

	/*
	**	Set CurTrigger if it isn't
	*/
	if (TriggerTypes.Count()==0) {
		CurTrigger = NULL;
	} else {
		CurTrigger = triggerlist.Current_Item();
//		if (!CurTrigger) {
//			CurTrigger = &*triggerlist.Current_Item();
//		}
	}

	/*
	**	Create the list
	*/
	commands = &triggerlist;
	editbtn.Add_Tail(*commands);
	newbtn.Add_Tail(*commands);
	deletebtn.Add_Tail(*commands);
	okbtn.Add_Tail(*commands);

	/*
	**	Main Processing Loop
	*/
	bool display = true;
	bool process = true;
	while (process) {
		/*
		**	Invoke game callback
		*/
		Call_Back();

		/*
		**	Refresh display if requested.
		*/
		if (display /*&& LogicPage->Lock()*/) {
			Hide_Mouse();
			Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
			Draw_Caption(TXT_TRIGGER_EDITOR, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);
			commands->Flag_List_To_Redraw();
			commands->Draw_All();
			Show_Mouse();
			display = false;
//			LogicPage->Unlock();
		}

		/*
		**	Get user input
		*/
		KeyNumType input = commands->Input();

		/*
		**	Process input
		*/
		switch (input) {
			case (TRIGGER_LIST | KN_BUTTON):
				CurTrigger = &*triggerlist.Current_Item();
//				CurTrigger = (TriggerTypeClass *)&*triggerlist.Current_Item();
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
	**	Redraw the display
	*/
	HidPage.Clear();
	Flag_To_Redraw(true);
	Render();

	if (edit_trig) return(1);
	if (new_trig) return(2);
	if (del_trig) return(3);
	return(0);
}


#endif
