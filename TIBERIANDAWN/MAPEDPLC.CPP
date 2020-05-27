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

/* $Header:   F:\projects\c&c\vcs\code\mapedplc.cpv   2.16   16 Oct 1995 16:51:00   JOE_BOSTIC  $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : MAPEDPLC.CPP                             *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : November 18, 1994                        *
 *                                                                         *
 *                  Last Update : July 4, 1995 [JLB]                       *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Object-placement routines                                               *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   MapEditClass::Placement_Dialog -- adds an object to the scenario      *
 *   MapEditClass::Start_Placement -- enters placement mode                *
 *   MapEditClass::Place_Object -- attempts to place the current object    *
 *   MapEditClass::Cancel_Placement -- cancels placement mode              *
 *   MapEditClass::Place_Next -- while placing object, goes to next        *
 *   MapEditClass::Place_Prev -- while placing object, goes to previous    *
 *   MapEditClass::Place_Next_Category -- places next object category      *
 *   MapEditClass::Place_Prev_Category -- places previous object category  *
 *   MapEditClass::Place_Home -- homes the placement object                *
 *   MapEditClass::Toggle_House -- toggles current placement object's house*
 *   MapEditClass::Set_House_Buttons -- toggles house buttons for btn list *
 *   MapEditClass::Start_Trigger_Placement -- enters trigger placement mode*
 *   MapEditClass::Stop_Trigger_Placement -- exits trigger placement mode  *
 *   MapEditClass::Place_Trigger -- assigns trigger to object or cell      *
 *   MapEditClass::Start_Base_Building -- starts base-building mode        *
 *   MapEditClass::Cancel_Base_Building -- stops base-building mode        *
 *   MapEditClass::Build_Base_To -- builds the AI base to the given percent*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

#ifdef SCENARIO_EDITOR


/***************************************************************************
 * MapEditClass::Placement_Dialog -- adds an object to the scenario        *
 *                                                                         *
 * This function sets LastChoice & LastHouse to the values chosen          *
 * by the user. It's up to the caller to call Start_Placement to enter     *
 * placement mode.                                                         *
 *   This routine does not modify PendingObject or PendingHouse.           *
 *                                                                         *
 *  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                   *
 *  ³   [GDI]  [NOD]  [Neutral]                        ³                   *
 *  ³                                                  ³                   *
 *  ³   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿              ³                   *
 *  ³   ³                               ³ [Template]   ³                   *
 *  ³   ³                               ³ [Overlay ]   ³                   *
 *  ³   ³                               ³ [Smudge  ]   ³                   *
 *  ³   ³                               ³ [Terrain ]   ³                   *
 *  ³   ³      (Object picture)         ³ [Unit    ]   ³                   *
 *  ³   ³                               ³ [Infantry]   ³                   *
 *  ³   ³                               ³ [Aircraft]   ³                   *
 *  ³   ³                               ³ [Building]   ³                   *
 *  ³   ³                               ³              ³                   *
 *  ³   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ     ÚÄÄÄÄÄÄ¿ ³                   *
 *  ³             [<-]  [->]                  ³(Grid)³ ³                   *
 *  ³                                         ³      ³ ³                   *
 *  ³         [OK]        [Cancel]            ÀÄÄÄÄÄÄÙ ³                   *
 *  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                   *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
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
int MapEditClass::Placement_Dialog(void)
{
	HousesType house;

	/*........................................................................
	Dialog & button dimensions
	........................................................................*/
	enum {
		D_DIALOG_W = 480,
		D_DIALOG_H = 360,
		D_DIALOG_X = ((640 - D_DIALOG_W) / 2),
		D_DIALOG_Y = ((400 - D_DIALOG_H) / 2),
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),

		D_TXT8_H = 22,
		D_MARGIN = 14,

		D_PICTURE_W = 304,					// must be divisible by 8!
		D_PICTURE_H = 210,
		D_PICTURE_X = D_DIALOG_X + 16,		// must start on a byte boundary!
		D_PICTURE_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H + D_MARGIN,
		D_PICTURE_CX = D_PICTURE_X + D_PICTURE_W / 2,

		D_GDI_W = 90,
		D_GDI_H = 18,
		D_GDI_X = D_DIALOG_X + D_MARGIN,
		D_GDI_Y = D_DIALOG_Y + D_MARGIN,

		D_NOD_W = 90,
		D_NOD_H = 18,
		D_NOD_X = D_GDI_X + D_GDI_W,
		D_NOD_Y = D_DIALOG_Y + D_MARGIN,

		D_NEUTRAL_W = 90,
		D_NEUTRAL_H = 18,
		D_NEUTRAL_X = D_NOD_X + D_NOD_W,
		D_NEUTRAL_Y = D_DIALOG_Y + D_MARGIN,

		D_MULTI1_W = 44,
		D_MULTI1_H = 18,
		D_MULTI1_X = D_GDI_X,
		D_MULTI1_Y = D_GDI_Y,

		D_MULTI2_W = 44,
		D_MULTI2_H = 18,
		D_MULTI2_X = D_MULTI1_X + D_MULTI1_W,
		D_MULTI2_Y = D_GDI_Y,

		D_MULTI3_W = 44,
		D_MULTI3_H = 18,
		D_MULTI3_X = D_MULTI2_X + D_MULTI2_W,
		D_MULTI3_Y = D_GDI_Y,

		D_MULTI4_W = 44,
		D_MULTI4_H = 18,
		D_MULTI4_X = D_MULTI3_X + D_MULTI3_W,
		D_MULTI4_Y = D_GDI_Y,

		D_LEFT_W = 90,
		D_LEFT_H = 18,
		D_LEFT_X = D_PICTURE_CX - 5 - D_LEFT_W,
		D_LEFT_Y = D_PICTURE_Y + D_PICTURE_H + D_MARGIN,

		D_RIGHT_W = 90,
		D_RIGHT_H = 18,
		D_RIGHT_X = D_PICTURE_CX + 5,
		D_RIGHT_Y = D_PICTURE_Y + D_PICTURE_H + D_MARGIN,

		D_TEMPLATE_W = 140,
		D_TEMPLATE_H = 18,
		D_TEMPLATE_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_TEMPLATE_W,
		D_TEMPLATE_Y = D_PICTURE_Y,

		D_OVERLAY_W = 140,
		D_OVERLAY_H = 18,
		D_OVERLAY_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_OVERLAY_W,
		D_OVERLAY_Y = D_TEMPLATE_Y + D_TEMPLATE_H,

		D_SMUDGE_W = 140,
		D_SMUDGE_H = 18,
		D_SMUDGE_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_SMUDGE_W,
		D_SMUDGE_Y = D_OVERLAY_Y + D_OVERLAY_H,

		D_TERRAIN_W = 140,
		D_TERRAIN_H = 18,
		D_TERRAIN_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_TERRAIN_W,
		D_TERRAIN_Y = D_SMUDGE_Y + D_SMUDGE_H,

		D_UNIT_W = 140,
		D_UNIT_H = 18,
		D_UNIT_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_UNIT_W,
		D_UNIT_Y = D_TERRAIN_Y + D_TERRAIN_H,

		D_INFANTRY_W = 140,
		D_INFANTRY_H = 18,
		D_INFANTRY_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_INFANTRY_W,
		D_INFANTRY_Y = D_UNIT_Y + D_UNIT_H,

		D_AIRCRAFT_W = 140,
		D_AIRCRAFT_H = 18,
		D_AIRCRAFT_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_AIRCRAFT_W,
		D_AIRCRAFT_Y = D_INFANTRY_Y + D_INFANTRY_H,

		D_BUILDING_W = 140,
		D_BUILDING_H = 18,
		D_BUILDING_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_BUILDING_W,
		D_BUILDING_Y = D_AIRCRAFT_Y + D_AIRCRAFT_H,

		D_OK_W = 90,
		D_OK_H = 18,
		D_OK_X = D_PICTURE_CX - D_OK_W - 5,
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_OK_H - D_MARGIN,

		D_CANCEL_W = 90,
		D_CANCEL_H = 18,
		D_CANCEL_X = D_PICTURE_CX + 5,
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - D_CANCEL_H - D_MARGIN,

	};
	/*........................................................................
	Grid Dimensions
	........................................................................*/
	enum {
		GRIDSIZE = 10,
		GRIDBLOCK_W = 6,
		GRIDBLOCK_H = 6,
		D_GRID_X = D_DIALOG_X + D_DIALOG_W - (GRIDSIZE * GRIDBLOCK_W) - D_MARGIN,
		D_GRID_Y = D_DIALOG_Y + D_DIALOG_H - (GRIDSIZE * GRIDBLOCK_H) - D_MARGIN,
	};
	/*........................................................................
	Button enumerations:
	........................................................................*/
	enum {
		BUTTON_GDI=100,
		BUTTON_NOD,
		BUTTON_NEUTRAL,
		BUTTON_JP,			// placeholder
		BUTTON_MULTI1,
		BUTTON_MULTI2,
		BUTTON_MULTI3,
		BUTTON_MULTI4,
		BUTTON_NEXT,
		BUTTON_PREV,
		BUTTON_OK,
		BUTTON_CANCEL,
		BUTTON_TEMPLATE,
		BUTTON_OVERLAY,
		BUTTON_SMUDGE,
		BUTTON_TERRAIN,
		BUTTON_UNIT,
		BUTTON_INFANTRY,
		BUTTON_AIRCRAFT,
		BUTTON_BUILDING,
	};
	/*........................................................................
	Redraw values: in order from "top" to "bottom" layer of the dialog
	........................................................................*/
	typedef enum {
		REDRAW_NONE = 0,
		REDRAW_BUTTONS,
		REDRAW_OBJECT,
		REDRAW_BACKGROUND,
		REDRAW_ALL = REDRAW_BACKGROUND
	} RedrawType;
	/*........................................................................
	Dialog variables
	........................................................................*/
	RedrawType display;									// display level
	bool process;											// loop while true
	bool cancel = false;							// true = user cancels
	const ObjectTypeClass * curobj;			// Working object pointer.
	int x,y;											// for drawing the grid
	KeyNumType input;								// user input
	short const *occupy;							// ptr into object's OccupyList
	int cell;										// cell index for parsing OccupyList
	int i;
	int typeindex;									// index of class type
	/*........................................................................
	Buttons
	........................................................................*/
	ControlClass *commands;

	TextButtonClass gdibtn (BUTTON_GDI, "GDI",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_GDI_X, D_GDI_Y, D_GDI_W, D_GDI_H);

	TextButtonClass nodbtn (BUTTON_NOD, "NOD",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NOD_X, D_NOD_Y, D_NOD_W, D_NOD_H);

	TextButtonClass neutbtn (BUTTON_NEUTRAL, "Neutral",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_NEUTRAL_X, D_NEUTRAL_Y, D_NEUTRAL_W, D_NEUTRAL_H);

	TextButtonClass multi1btn (BUTTON_MULTI1, "M1",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI1_X, D_MULTI1_Y, D_MULTI1_W, D_MULTI1_H);

	TextButtonClass multi2btn (BUTTON_MULTI2, "M2",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI2_X, D_MULTI2_Y, D_MULTI2_W, D_MULTI2_H);

	TextButtonClass multi3btn (BUTTON_MULTI3, "M3",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI3_X, D_MULTI3_Y, D_MULTI3_W, D_MULTI3_H);

	TextButtonClass multi4btn (BUTTON_MULTI4, "M4",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_MULTI4_X, D_MULTI4_Y, D_MULTI4_W, D_MULTI4_H);

	TextButtonClass nextbtn (BUTTON_NEXT, TXT_RIGHT,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_RIGHT_X, D_RIGHT_Y, D_RIGHT_W, D_RIGHT_H);

	TextButtonClass prevbtn (BUTTON_PREV, TXT_LEFT,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_LEFT_X, D_LEFT_Y, D_LEFT_W, D_LEFT_H);

	TextButtonClass okbtn (BUTTON_OK, TXT_OK,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_OK_X, D_OK_Y, D_OK_W, D_OK_H);

	TextButtonClass cancelbtn (BUTTON_CANCEL, TXT_CANCEL,
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);

	TextButtonClass templatebtn (BUTTON_TEMPLATE, "Template",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_TEMPLATE_X, D_TEMPLATE_Y, D_TEMPLATE_W, D_TEMPLATE_H);

	TextButtonClass overlaybtn (BUTTON_OVERLAY, "Overlay",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_OVERLAY_X, D_OVERLAY_Y, D_OVERLAY_W, D_OVERLAY_H);

	TextButtonClass smudgebtn (BUTTON_SMUDGE, "Smudge",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_SMUDGE_X, D_SMUDGE_Y, D_SMUDGE_W, D_SMUDGE_H);

	TextButtonClass terrainbtn (BUTTON_TERRAIN, "Terrain",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_TERRAIN_X, D_TERRAIN_Y, D_TERRAIN_W, D_TERRAIN_H);

	TextButtonClass unitbtn (BUTTON_UNIT, "Unit",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_UNIT_X, D_UNIT_Y, D_UNIT_W, D_UNIT_H);

	TextButtonClass infantrybtn (BUTTON_INFANTRY, "Infantry",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_INFANTRY_X, D_INFANTRY_Y, D_INFANTRY_W, D_INFANTRY_H);

	TextButtonClass aircraftbtn (BUTTON_AIRCRAFT, "Aircraft",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_AIRCRAFT_X, D_AIRCRAFT_Y, D_AIRCRAFT_W, D_AIRCRAFT_H);

	TextButtonClass buildingbtn (BUTTON_BUILDING, "Building",
		TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW,
		D_BUILDING_X, D_BUILDING_Y, D_BUILDING_W, D_BUILDING_H);

	/*------------------------------------------------------------------------
	Initialize addable objects list; we must do this every time in case one
	of the object pools has become exhausted; that object won't be available
	for adding.  (Skip aircraft, since they won't be used in the editor.)
	------------------------------------------------------------------------*/
	Clear_List();
	TemplateTypeClass::Prep_For_Add();
	OverlayTypeClass::Prep_For_Add();
	SmudgeTypeClass::Prep_For_Add();
	TerrainTypeClass::Prep_For_Add();
	UnitTypeClass::Prep_For_Add();
	InfantryTypeClass::Prep_For_Add();
	BuildingTypeClass::Prep_For_Add();

	/*........................................................................
	Compute offset of each class type in the Objects array
	........................................................................*/
	TypeOffset[0] = 0;
	for (i=1; i<NUM_EDIT_CLASSES; i++) {
		TypeOffset[i] = TypeOffset[i-1] + NumType[i-1];
	}

	/*
	--------------------- Return if no objects to place ----------------------
	*/
	if (!ObjCount)  {
		return(-1);
	}

	/*
	------------------------------- Initialize -------------------------------
	*/
	Set_Logic_Page(SeenBuff);
	if (LastChoice >= ObjCount) {
		LastChoice = 0;
	}
	curobj = Objects[LastChoice];		// current object to choose

	commands = &neutbtn;
	if (ScenPlayer == SCEN_PLAYER_MPLAYER) {
		multi1btn.Add_Tail(*commands);
		multi2btn.Add_Tail(*commands);
		multi3btn.Add_Tail(*commands);
		multi4btn.Add_Tail(*commands);
	} else {
		gdibtn.Add_Tail(*commands);
		nodbtn.Add_Tail(*commands);
	}
	nextbtn.Add_Tail(*commands);
	prevbtn.Add_Tail(*commands);
	okbtn.Add_Tail(*commands);
	cancelbtn.Add_Tail(*commands);
	templatebtn.Add_Tail(*commands);
	overlaybtn.Add_Tail(*commands);
	smudgebtn.Add_Tail(*commands);
	terrainbtn.Add_Tail(*commands);
	unitbtn.Add_Tail(*commands);
	infantrybtn.Add_Tail(*commands);
	aircraftbtn.Add_Tail(*commands);
	buildingbtn.Add_Tail(*commands);

	/*........................................................................
	If the current house isn't valid for the current object type, cycle to
	the next house.
	........................................................................*/
	if (!Verify_House(LastHouse,curobj)) {
		LastHouse = Cycle_House(LastHouse,curobj);
	}

	/*
	..................... Set the buttons for this house .....................
	*/
	Set_House_Buttons(LastHouse, commands, BUTTON_GDI);

	/*
	-------------------------- Main processing loop --------------------------
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
		---------------------- Refresh display if needed ----------------------
		*/
		if (display > REDRAW_NONE) {
			/*
			---------------------- Display the dialog box ----------------------
			*/
			Hide_Mouse();
			if (display >= REDRAW_BACKGROUND) {
				Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
				Draw_Caption(TXT_NONE, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);
			}

			/*------------------------------------------------------------------
			Display the current object:
			- save the current window dimensions
			- adjust the window size to the actual drawable area
			- draw the shape
			- reset the window dimensions
			------------------------------------------------------------------*/
			if (display >= REDRAW_OBJECT) {
				WindowList[WINDOW_EDITOR][WINDOWX] = D_PICTURE_X >> 3;
				WindowList[WINDOW_EDITOR][WINDOWY] = D_PICTURE_Y;
				WindowList[WINDOW_EDITOR][WINDOWWIDTH] = D_PICTURE_W >> 3;
				WindowList[WINDOW_EDITOR][WINDOWHEIGHT] = D_PICTURE_H;
				Change_Window((int)WINDOW_EDITOR);
				Draw_Box(D_PICTURE_X, D_PICTURE_Y, D_PICTURE_W, D_PICTURE_H,
					BOXSTYLE_GREEN_DOWN, true);
				curobj->Display(WinW<<2, WinH>>1, WINDOW_EDITOR, LastHouse);

				/*
				........................ Erase the grid .........................
				*/
				LogicPage->Fill_Rect(D_GRID_X - GRIDBLOCK_W * 2, D_GRID_Y,
					D_GRID_X + GRIDSIZE * GRIDBLOCK_W,
					D_GRID_Y + GRIDSIZE * GRIDBLOCK_H, BLACK);

				/*
				.............. Draw a box for every cell occupied ...............
				*/
				occupy = curobj->Occupy_List();
				while ( (*occupy) != REFRESH_EOL) {
					cell = (*occupy);
					occupy++;
					x = D_GRID_X + ((cell % MAP_CELL_W) * GRIDBLOCK_W);
					y = D_GRID_Y + ((cell / MAP_CELL_W) * GRIDBLOCK_H);
					LogicPage->Fill_Rect(x, y, x + GRIDBLOCK_W - 1, y + GRIDBLOCK_H - 1,
						CC_BRIGHT_GREEN);
				}

				/*
				..................... Draw the grid itself ......................
				*/
				for (y = 0; y <= GRIDSIZE; y++) {
					for (x = 0; x <= GRIDSIZE; x++) {
						LogicPage->Draw_Line(D_GRID_X + x * GRIDBLOCK_W, D_GRID_Y,
							D_GRID_X + x * GRIDBLOCK_W,
							D_GRID_Y + GRIDSIZE * GRIDBLOCK_H, CC_GREEN_SHADOW);
					}
					LogicPage->Draw_Line(D_GRID_X, D_GRID_Y + y * GRIDBLOCK_H,
						D_GRID_X + GRIDSIZE * GRIDBLOCK_W, D_GRID_Y + y * GRIDBLOCK_H,
						CC_GREEN_SHADOW);
				}

				/*...............................................................
				Print the object's label from the class's Full_Name().
				Warning: Text_String returns an EMS pointer, so standard string
				functions won't work!
				...............................................................*/
				Fancy_Text_Print (curobj->Full_Name(),
					D_PICTURE_CX, D_PICTURE_Y + D_MARGIN,
					CC_GREEN, TBLACK,
					TPF_CENTER | TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_NOSHADOW);
			}

			/*
			-------------------------- Redraw buttons --------------------------
			*/
			if (display >= REDRAW_BUTTONS) {
				/*...............................................................
				Figure out which class category we're in & highlight that button
				This updates 'typeindex', which is used below, and it also updates
				the category button states.
				...............................................................*/
				i = 0;
				for (typeindex = 0; typeindex < NUM_EDIT_CLASSES; typeindex++) {
					i += NumType[typeindex];
					if (LastChoice < i) break;
				}
				templatebtn.Turn_Off();
				overlaybtn.Turn_Off();
				smudgebtn.Turn_Off();
				terrainbtn.Turn_Off();
				unitbtn.Turn_Off();
				infantrybtn.Turn_Off();
				aircraftbtn.Turn_Off();
				buildingbtn.Turn_Off();
				switch (typeindex + BUTTON_TEMPLATE) {
					case BUTTON_TEMPLATE:
						templatebtn.Turn_On();
						break;

					case BUTTON_OVERLAY:
						overlaybtn.Turn_On();
						break;

					case BUTTON_SMUDGE:
						smudgebtn.Turn_On();
						break;

					case BUTTON_TERRAIN:
						terrainbtn.Turn_On();
						break;

					case BUTTON_UNIT:
						unitbtn.Turn_On();
						break;

					case BUTTON_INFANTRY:
						infantrybtn.Turn_On();
						break;

					case BUTTON_AIRCRAFT:
						aircraftbtn.Turn_On();
						break;

					case BUTTON_BUILDING:
						buildingbtn.Turn_On();
						break;
				}
			}

			/*
			.......................... Redraw buttons ..........................
			*/
			commands->Draw_All();
			Show_Mouse();
			display = REDRAW_NONE;

		}

		/*
		........................... Get user input ............................
		*/
		input = commands->Input();

		/*
		------------------------- Process user input --------------------------
		*/
		switch (input) {
			/*
			---------------------------- GDI House -----------------------------
			*/
			case (BUTTON_GDI | KN_BUTTON):
			case (BUTTON_NOD | KN_BUTTON):
			case (BUTTON_NEUTRAL | KN_BUTTON):
			case (BUTTON_MULTI1 | KN_BUTTON):
			case (BUTTON_MULTI2 | KN_BUTTON):
			case (BUTTON_MULTI3 | KN_BUTTON):
			case (BUTTON_MULTI4 | KN_BUTTON):
				house = (HousesType)( (input & (~KN_BUTTON)) - BUTTON_GDI);
				/*
				............... ignore if invalid for this object ...............
				*/
				if (!Verify_House(house,curobj)) {
					Set_House_Buttons(LastHouse, commands, BUTTON_GDI);
					break;
				}

				/*
				...................... Set flags & buttons ......................
				*/
				LastHouse = house;
				Set_House_Buttons(LastHouse, commands, BUTTON_GDI);
				display = REDRAW_OBJECT;
				break;

			/*
			--------------------------- Next in list ---------------------------
			*/
			case (KN_RIGHT):
			case (BUTTON_NEXT | KN_BUTTON):
				/*
				..................... Increment to next obj .....................
				*/
				LastChoice++;
				if (LastChoice == ObjCount) {
					LastChoice = 0;
				}
				curobj = Objects[LastChoice];

				/*
				.................... Get valid house for obj ....................
				*/
				if (!Verify_House(LastHouse,curobj)) {
					LastHouse = Cycle_House(LastHouse,curobj);
					Set_House_Buttons(LastHouse, commands, BUTTON_GDI);
				}

				nextbtn.Turn_Off();
				display = REDRAW_OBJECT;
				break;

			/*
			------------------------- Previous in list -------------------------
			*/
			case (KN_LEFT):
			case (BUTTON_PREV | KN_BUTTON):
				/*
				..................... Decrement to prev obj .....................
				*/
				LastChoice--;
				if (LastChoice < 0) {
					LastChoice = ObjCount-1;
				}
				curobj = Objects[LastChoice];

				/*
				.................... Get valid house for obj ....................
				*/
				if (!Verify_House(LastHouse,curobj)) {
					LastHouse = Cycle_House(LastHouse,curobj);
					Set_House_Buttons(LastHouse, commands, BUTTON_GDI);
				}

				prevbtn.Turn_Off();
				display = REDRAW_OBJECT;
				break;

			/*
			----------------------- Select a class type ------------------------
			*/
			case (BUTTON_TEMPLATE | KN_BUTTON):
			case (BUTTON_OVERLAY | KN_BUTTON):
			case (BUTTON_SMUDGE | KN_BUTTON):
			case (BUTTON_TERRAIN | KN_BUTTON):
			case (BUTTON_UNIT | KN_BUTTON):
			case (BUTTON_INFANTRY | KN_BUTTON):
			case (BUTTON_AIRCRAFT | KN_BUTTON):
			case (BUTTON_BUILDING | KN_BUTTON):
				/*
				...................... Find index of class ......................
				*/
				typeindex = input - (BUTTON_TEMPLATE | KN_BUTTON);

				/*
				............ If no objects of that type, do nothing .............
				*/
				if (NumType[typeindex]==0) {
					display = REDRAW_BUTTONS;			// force to reset button states
					break;
				}

				/*
				...................... Set current object .......................
				*/
				LastChoice = TypeOffset[typeindex];
				curobj = Objects[LastChoice];

				/*
				.................... Get valid house for obj ....................
				*/
				if (!Verify_House(LastHouse,curobj)) {
					LastHouse = Cycle_House(LastHouse,curobj);
					Set_House_Buttons(LastHouse, commands, BUTTON_GDI);
				}

				display = REDRAW_OBJECT;
				break;

			/*
			-------------------------- Next category ---------------------------
			*/
			case KN_PGDN:
				typeindex++;
				if (typeindex==NUM_EDIT_CLASSES) {
					typeindex = 0;
				}

				/*
				...................... Set current object .......................
				*/
				LastChoice = TypeOffset[typeindex];
				curobj = Objects[LastChoice];

				/*
				.................... Get valid house for obj ....................
				*/
				if (!Verify_House(LastHouse,curobj)) {
					LastHouse = Cycle_House(LastHouse,curobj);
					Set_House_Buttons(LastHouse, commands, BUTTON_GDI);
				}

				display = REDRAW_OBJECT;
				break;

			/*
			------------------------ Previous category -------------------------
			*/
			case KN_PGUP:
				typeindex--;
				if (typeindex < 0) {
					typeindex = NUM_EDIT_CLASSES - 1;
				}

				/*
				...................... Set current object .......................
				*/
				LastChoice = TypeOffset[typeindex];
				curobj = Objects[LastChoice];

				/*
				.................... Get valid house for obj ....................
				*/
				if (!Verify_House(LastHouse,curobj)) {
					LastHouse = Cycle_House(LastHouse,curobj);
					Set_House_Buttons(LastHouse, commands, BUTTON_GDI);
				}

				display = REDRAW_OBJECT;
				break;

			/*
			------------------------ Jump to 1st choice ------------------------
			*/
			case KN_HOME:
				LastChoice = 0;
				/*
				...................... Set current object .......................
				*/
				curobj = Objects[LastChoice];
				/*
				.................... Get valid house for obj ....................
				*/
				if (!Verify_House(LastHouse,curobj)) {
					LastHouse = Cycle_House(LastHouse,curobj);
					Set_House_Buttons(LastHouse, commands, BUTTON_GDI);
				}
				display = REDRAW_OBJECT;
				break;

			/*
			-------------------------------- OK --------------------------------
			*/
			case (KN_RETURN):
			case (BUTTON_OK | KN_BUTTON):
				cancel = false;
				process = false;
				break;

			/*
			------------------------------ Cancel ------------------------------
			*/
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
	--------------------------- Redraw the display ---------------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();

	if (cancel) {
		return(-1);
	}

	return(0);
}


/***************************************************************************
 * MapEditClass::Start_Placement -- enters placement mode                  *
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
 *   11/04/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Start_Placement(void)
{
	int i;

	/*------------------------------------------------------------------------
	Initialize addable objects list; we must do this every time in case one
	of the object pools has become exhausted; that object won't be available
	for adding.
	------------------------------------------------------------------------*/
	Clear_List();
	TemplateTypeClass::Prep_For_Add();
	OverlayTypeClass::Prep_For_Add();
	SmudgeTypeClass::Prep_For_Add();
	TerrainTypeClass::Prep_For_Add();
	UnitTypeClass::Prep_For_Add();
	InfantryTypeClass::Prep_For_Add();
	//AircraftTypeClass::Prep_For_Add();
	BuildingTypeClass::Prep_For_Add();
	/*........................................................................
	Compute offset of each class type in the Objects array
	........................................................................*/
	TypeOffset[0] = 0;
	for (i=1; i<NUM_EDIT_CLASSES; i++) {
		TypeOffset[i] = TypeOffset[i-1] + NumType[i-1];
	}

	/*
	---------------------- Create the placement object -----------------------
	*/
	/*------------------------------------------------------------------------
	Create the placement object:
	- For normal placement mode, use the last-used index into Objects
	  (LastChoice), and the last-used house (LastHouse).
	- For base-building mode, force the object to be a building, and use the
	  House specified in the Base object
	------------------------------------------------------------------------*/
	if (!BaseBuilding) {
		if (LastChoice >= ObjCount)
			LastChoice = ObjCount - 1;
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(LastHouse));
	} else {
		if (LastChoice < TypeOffset[7])
			LastChoice = TypeOffset[7];
		if (LastChoice >= ObjCount)
			LastChoice = ObjCount - 1;
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse = Base.House;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(LastHouse));
	}


	/*
	------------------- Error if no more objects available -------------------
	*/
	if (!PendingObjectPtr) {
		CCMessageBox().Process("No more objects of this type available.");
		HiddenPage.Clear();
		Flag_To_Redraw(true);
		Render();
		PendingObject = NULL;
		if (BaseBuilding)
			Cancel_Base_Building();
		return;
	}

	/*
	------------------------ Set the placement cursor ------------------------
	*/
	Set_Cursor_Pos();
	Set_Cursor_Shape(PendingObject->Occupy_List());
}


/***************************************************************************
 * MapEditClass::Place_Object -- attempts to place the current object      *
 *                                                                         *
 * Placement of "real" objects is simply checked via their Unlimbo routine.*
 * Placement of templates is more complex:                                 *
 * - for every cell in the template's OccupyList, check for objects        *
 *     already in that cell by looking at the cell's OccupyList &          *
 *     OverlapList                                                         *
 * - "lift" all the objects in the cell by Mark'ing them                   *
 * - temporarily place the template in that cell                           *
 * - try to Unlimbo all the objects that were in the cell. If any          *
 *     objects fail to Unlimbo onto that template, the template cannot     *
 *     be placed here                                                      *
 *                                                                         *
 * It is assumed that the object being placed is a "new" object; the       *
 * object's strength & mission are not set during Unlimbo.                 *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = OK, -1 = unable to place                                       *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/04/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Place_Object(void)
{
	CELL template_cell;						// cell being checked for template
	COORDINATE obj_coord;							// coord of occupier object
	int okflag;									// OK to place a template?
	short const *occupy;						// ptr into template's OccupyList
	ObjectClass *occupier;					// occupying object
	TemplateType save_ttype;				// for saving cell's TType
	unsigned char save_ticon;				// for saving cell's TIcon
	BaseNodeClass node;						// for adding to an AI Base

	/*------------------------------------------------------------------------
	Placing a template:
	- first lift up any objects in the cell
	- place the template, and try to replace the objects; if they won't go
	  back, the template can't go there
	------------------------------------------------------------------------*/
	//ScenarioInit++;
	if (PendingObject->What_Am_I() == RTTI_TEMPLATETYPE) {

		/*
		.......... Loop through all cells this template will occupy ...........
		*/
		okflag = true;
		occupy = PendingObject->Occupy_List();
		while ((*occupy) != REFRESH_EOL) {

			/*
			................. Check this cell for an occupier ..................
			*/
			template_cell = (ZoneCell+ZoneOffset) + (*occupy);
			if ((*this)[template_cell].Cell_Occupier()) {
				occupier = (*this)[template_cell].Cell_Occupier();

				/*
				.................. Save object's coordinates ....................
				*/
				obj_coord = occupier->Coord;

				/*
				................... Place the object in limbo ...................
				*/
				occupier->Mark(MARK_UP);

				/*
				................ Set the cell's template values .................
				*/
				save_ttype = (*this)[template_cell].TType;
				save_ticon = (*this)[template_cell].TIcon;
				(*this)[template_cell].TType =
					((TemplateTypeClass *)PendingObject)->Type;
				(*this)[template_cell].TIcon = Cell_X(*occupy) + Cell_Y(*occupy) *
					((TemplateTypeClass *)PendingObject)->Width;
				(*this)[template_cell].Recalc_Attributes();
				/*
				................ Try to put the object back down ................
				*/
				if (occupier->Can_Enter_Cell(Coord_Cell(obj_coord)) != MOVE_OK) {
					okflag = false;
				}

				/*
				.............. Put everything back the way it was ...............
				*/
				(*this)[template_cell].TType = save_ttype;
				(*this)[template_cell].TIcon = save_ticon;
				(*this)[template_cell].Recalc_Attributes();

				/*
				.......... Major error if can't replace the object now ..........
				*/
				occupier->Mark(MARK_DOWN);
			}
			occupy++;
		}

		/*
		......... If it's still OK after ALL THAT, place the template .........
		*/
		if (okflag) {
			if (PendingObjectPtr->Unlimbo(Cell_Coord(ZoneCell + ZoneOffset))) {
				/*...............................................................
				Loop through all cells occupied by this template, and clear the
				smudge & overlay.
				...............................................................*/
				occupy = PendingObject->Occupy_List();
				while ((*occupy) != REFRESH_EOL) {
					/*
					............... Get cell for this occupy item ................
					*/
					template_cell = (ZoneCell+ZoneOffset) + (*occupy);

					/*
					................... Clear smudge & overlay ...................
					*/
					(*this)[template_cell].Overlay = OVERLAY_NONE;
					(*this)[template_cell].OverlayData = 0;
					(*this)[template_cell].Smudge = SMUDGE_NONE;

					/*
					............ make adjacent cells recalc attrib's .............
					*/
					(*this)[template_cell].Recalc_Attributes();
					(*this)[template_cell].Wall_Update();
					(*this)[template_cell].Concrete_Calc();

					occupy++;
				}

				/*
				......................... Set flags etc .........................
				*/
				PendingObjectPtr = 0;
				PendingObject = 0;
				PendingHouse = HOUSE_NONE;
				Set_Cursor_Shape(0);
				//ScenarioInit--;
				TotalValue = Overpass();
				Flag_To_Redraw(false);
				return(0);
			}

			/*
			**	Failure to deploy results in a returned failure code.
			*/
			//ScenarioInit--;
			return(-1);
		}

		/*
		........................ Not OK; return error .........................
		*/
		//ScenarioInit--;
		return(-1);
	}

	/*------------------------------------------------------------------------
	Placing infantry: Infantry can go into cell sub-positions, so find the
	sub-position closest to the mouse & put him there
	------------------------------------------------------------------------*/
	if (PendingObject->What_Am_I() == RTTI_INFANTRYTYPE) {
		/*
		....................... Find cell sub-position ........................
		*/
		if (Is_Spot_Free(Pixel_To_Coord(Get_Mouse_X(),Get_Mouse_Y()))) {
			obj_coord = Closest_Free_Spot(Pixel_To_Coord(Get_Mouse_X(),
				Get_Mouse_Y()));
		} else {
			obj_coord = NULL;
		}

		/*
		................ No free spots; don't place the object ................
		*/
		if (obj_coord == NULL) {
			//ScenarioInit--;
			return(-1);
		}

		/*
		......................... Unlimbo the object ..........................
		*/
		if (PendingObjectPtr->Unlimbo(obj_coord)) {
			((InfantryClass *)PendingObjectPtr)->Set_Occupy_Bit(obj_coord);
//			Map[Coord_Cell(obj_coord)].Flag.Composite |=
//				(1 << CellClass::Spot_Index(obj_coord));
			PendingObjectPtr = 0;
			PendingObject = 0;
			PendingHouse = HOUSE_NONE;
			Set_Cursor_Shape(0);
			//ScenarioInit--;
			return(0);
		}

		//ScenarioInit--;
		return(-1);
	}

	/*------------------------------------------------------------------------
	Placing an object
	------------------------------------------------------------------------*/
	if (PendingObjectPtr->Unlimbo(Cell_Coord(ZoneCell + ZoneOffset))) {

		/*
		** Update the Tiberium computation if we're placing an overlay
		*/
		if (PendingObject->What_Am_I() == RTTI_OVERLAYTYPE &&
			((OverlayTypeClass *)PendingObject)->IsTiberium) {

			TotalValue = Overpass();
			Flag_To_Redraw(false);
		}

		/*
		** If we're building a base, add this building to the base's Node list.
		*/
		if (BaseBuilding && PendingObject->What_Am_I() == RTTI_BUILDINGTYPE) {
			node.Type = ((BuildingTypeClass *)PendingObject)->Type;
			node.Coord = PendingObjectPtr->Coord;
			Base.Nodes.Add(node);
		}

		PendingObjectPtr = 0;
		PendingObject = 0;
		PendingHouse = HOUSE_NONE;
		Set_Cursor_Shape(0);
		//ScenarioInit--;
		return(0);
	}

	return(-1);
}


/***************************************************************************
 * MapEditClass::Cancel_Placement -- cancels placement mode                *
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
 *   11/04/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Cancel_Placement(void)
{
	/*
	---------------------- Delete the placement object -----------------------
	*/
	delete PendingObjectPtr;
	PendingObject = 0;
	PendingObjectPtr = 0;
	PendingHouse = HOUSE_NONE;

	/*
	-------------------------- Restore cursor shape --------------------------
	*/
	Set_Cursor_Shape(0);

	/*
	----------------- Redraw the map to erase old leftovers ------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();
}


/***************************************************************************
 * MapEditClass::Place_Next -- while placing object, goes to next          *
 *                                                                         *
 * - Deletes the current 'PendingObjectPtr'                                *
 * - Increments LastChoice                                                 *
 * - Tries to create a new 'PendingObjectPtr'; if fails, keeps             *
 *   incrementing until it gets it                                         *
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
 *   11/03/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Place_Next(void)
{
	delete PendingObjectPtr;
	PendingObjectPtr = NULL;
	PendingObject = NULL;

	/*
	------------------ Loop until we create a valid object -------------------
	*/
	while (!PendingObjectPtr) {
		/*
		................. Go to next object in Objects list ...................
		*/
		LastChoice++;
		if (LastChoice == ObjCount) {
			/*
			** If we're in normal placement mode, wrap to the 1st object;
			** if we're in base-building mode, wrap to the 1st building
			*/
			if (!BaseBuilding) {
				LastChoice = 0;
			} else {
				LastChoice = TypeOffset[7];
			}
		}

		/*
		................... Get house for this object type ....................
		*/
		if (!Verify_House(LastHouse,Objects[LastChoice])) {
			/*
			** If we're in normal placement mode, change the current
			** placement house to the one that can own this object.
			** If we're building a base, skip ahead to the next object if the
			** base's house can't own this one.
			*/
			if (!BaseBuilding) {
				LastHouse = Cycle_House(LastHouse,Objects[LastChoice]);
			} else {
				continue;
			}
		}

		/*
		....................... Create placement object .......................
		*/
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(PendingHouse));
		if (!PendingObjectPtr) {
			PendingObject = NULL;
		}
	}

	/*
	------------------------ Set the new cursor shape ------------------------
	*/
	Set_Cursor_Pos();
	Set_Cursor_Shape(0);
	Set_Cursor_Shape(PendingObject->Occupy_List());

	/*
	----------------- Redraw the map to erase old leftovers ------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();
}


/***************************************************************************
 * MapEditClass::Place_Prev -- while placing object, goes to previous      *
 *                                                                         *
 * - Deletes the current 'PendingObjectPtr'                                *
 * - Decrements LastChoice                                                 *
 * - Tries to create a new 'PendingObjectPtr'; if fails, keeps             *
 *   decrementing until it gets it                                         *
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
 *   11/03/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Place_Prev(void)
{
	delete PendingObjectPtr;
	PendingObjectPtr = NULL;
	PendingObject = NULL;

	/*
	------------------ Loop until we create a valid object -------------------
	*/
	while (!PendingObjectPtr) {

		/*
		................. Go to prev object in Objects list ..................
		*/
		LastChoice--;
		/*
		** If we're in normal placement mode, wrap at the 1st object.
		** If we're building a base, wrap at the 1st building.
		*/
		if (!BaseBuilding) {
			if (LastChoice < 0)
				LastChoice = ObjCount - 1;
		} else {
			if (LastChoice < TypeOffset[7])
				LastChoice = ObjCount - 1;
		}

		/*
		................... Get house for this object type ....................
		*/
		if (!Verify_House(LastHouse,Objects[LastChoice])) {
			/*
			** If we're in normal placement mode, change the current
			** placement house to the one that can own this object.
			** If we're building a base, skip ahead to the next object if the
			** base's house can't own this one.
			*/
			if (!BaseBuilding) {
				LastHouse = Cycle_House(LastHouse,Objects[LastChoice]);
			} else {
				continue;
			}
		}

		/*
		....................... Create placement object .......................
		*/
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(PendingHouse));
		if (!PendingObjectPtr) {
			PendingObject = NULL;
		}
	}

	/*
	------------------------ Set the new cursor shape ------------------------
	*/
	Set_Cursor_Pos();
	Set_Cursor_Shape(0);
	Set_Cursor_Shape(PendingObject->Occupy_List());

	/*
	----------------- Redraw the map to erase old leftovers ------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();
}


/***************************************************************************
 * MapEditClass::Place_Next_Category -- places next category of object     *
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
 *   11/03/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Place_Next_Category(void)
{
	int i;

	/*
	** Don't allow this command if we're building a base; the only valid
	** category for base-building is buildings.
	*/
	if (BaseBuilding) {
		return;
	}

	delete PendingObjectPtr;
	PendingObjectPtr = NULL;
	PendingObject = NULL;

	/*
	------------------ Go to next category in Objects list -------------------
	*/
	i = LastChoice;
	while (Objects[i]->What_Am_I() == Objects[LastChoice]->What_Am_I()) {
		i++;
		if (i == ObjCount) {
			i = 0;
		}
	}
	LastChoice = i;

	/*
	------------------ Loop until we create a valid object -------------------
	*/
	while (!PendingObjectPtr) {

		/*
		................... Get house for this object type ....................
		*/
		if (!Verify_House(LastHouse,Objects[LastChoice])) {
			LastHouse = Cycle_House(LastHouse,Objects[LastChoice]);
		}

		/*
		....................... Create placement object .......................
		*/
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(PendingHouse));

		/*
		.................. If this one failed, try the next ...................
		*/
		if (!PendingObjectPtr) {
			PendingObject = NULL;
			LastChoice++;
			if (LastChoice == ObjCount) {
				LastChoice = 0;
			}
		}
	}

	/*
	------------------------ Set the new cursor shape ------------------------
	*/
	Set_Cursor_Pos();
	Set_Cursor_Shape(0);
	Set_Cursor_Shape(PendingObject->Occupy_List());

	/*
	----------------- Redraw the map to erase old leftovers ------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();
}


/***************************************************************************
 * MapEditClass::Place_Prev_Category -- places previous category of object *
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
 *   11/03/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Place_Prev_Category(void)
{
	int i;

	/*
	** Don't allow this command if we're building a base; the only valid
	** category for base-building is buildings.
	*/
	if (BaseBuilding) {
		return;
	}

	delete PendingObjectPtr;
	PendingObjectPtr = NULL;
	PendingObject = NULL;

	/*
	------------------ Go to prev category in Objects list -------------------
	*/
	i = LastChoice;
	/*
	..................... Scan for the previous category .....................
	*/
	while (Objects[i]->What_Am_I() == Objects[LastChoice]->What_Am_I()) {
		i--;
		if (i < 0) {
			i = ObjCount - 1;
		}
	}
	/*
	.................... Scan for start of this category .....................
	*/
	LastChoice = i;
	while (Objects[i]->What_Am_I() == Objects[LastChoice]->What_Am_I()) {
		LastChoice = i;
		i--;
		if (i < 0) {
			i = ObjCount - 1;
		}
	}

	/*
	------------------ Loop until we create a valid object -------------------
	*/
	while (!PendingObjectPtr) {
		/*
		................... Get house for this object type ....................
		*/
		if (!Verify_House(LastHouse,Objects[LastChoice])) {
			LastHouse = Cycle_House(LastHouse,Objects[LastChoice]);
		}

		/*
		....................... Create placement object .......................
		*/
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(PendingHouse));

		/*
		.................. If this one failed, try the next ...................
		*/
		if (!PendingObjectPtr) {
			PendingObject = NULL;
			LastChoice--;
			if (LastChoice < 0) {
				LastChoice = ObjCount - 1;
			}
		}
	}

	/*
	------------------------ Set the new cursor shape ------------------------
	*/
	Set_Cursor_Pos();
	Set_Cursor_Shape(0);
	Set_Cursor_Shape(PendingObject->Occupy_List());

	/*
	----------------- Redraw the map to erase old leftovers ------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();
}


/***************************************************************************
 * MapEditClass::Place_Home -- homes the placement object                  *
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
 *   11/03/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Place_Home(void)
{
	delete PendingObjectPtr;
	PendingObjectPtr = NULL;
	PendingObject = NULL;

	/*
	** Don't allow this command if we're building a base; the only valid
	** category for base-building is buildings.
	*/
	if (BaseBuilding) {
		return;
	}

	/*
	------------------ Loop until we create a valid object -------------------
	*/
	LastChoice = 0;
	while (!PendingObjectPtr) {
		/*
		................... Get house for this object type ....................
		*/
		if (!Verify_House(LastHouse,Objects[LastChoice])) {
			LastHouse = Cycle_House(LastHouse,Objects[LastChoice]);
		}

		/*
		....................... Create placement object .......................
		*/
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(PendingHouse));

		/*
		.................. If this one failed, try the next ...................
		*/
		if (!PendingObjectPtr) {
			PendingObject = NULL;
			LastChoice++;
			if (LastChoice == ObjCount) {
				LastChoice = 0;
			}
		}
	}

	/*
	------------------------ Set the new cursor shape ------------------------
	*/
	Set_Cursor_Pos();
	Set_Cursor_Shape(0);
	Set_Cursor_Shape(PendingObject->Occupy_List());

	/*
	----------------- Redraw the map to erase old leftovers ------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
	Render();
}


/***************************************************************************
 * MapEditClass::Toggle_House -- toggles current placement object's house  *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/04/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Toggle_House(void)
{
	TechnoClass *tp;

	/*
	** Don't allow this command if we're building a base; the only valid
	** house for base-building is the one assigned to the base.
	*/
	if (BaseBuilding) {
		return;
	}

	/*------------------------------------------------------------------------
	Only techno objects can be owned by a house; return if not a techno
	------------------------------------------------------------------------*/
	if (!PendingObjectPtr->Is_Techno()) {
		return;
	}

	/*------------------------------------------------------------------------
	Select the house that will own this object
	------------------------------------------------------------------------*/
	LastHouse = Cycle_House(PendingObjectPtr->Owner(), PendingObject);

	/*------------------------------------------------------------------------
	Change the house
	------------------------------------------------------------------------*/
	tp = (TechnoClass *)PendingObjectPtr;
	tp->House = HouseClass::As_Pointer(LastHouse);

	/*------------------------------------------------------------------------
	Set house variables to new house
	------------------------------------------------------------------------*/
	PendingHouse = LastHouse;
}


/***************************************************************************
 * MapEditClass::Set_House_Buttons -- toggles house buttons for btn list   *
 *                                                                         *
 * Looks in the given button list for the given GDI, NOD & Neutral button  *
 * id's. Sets the On/Off state of the buttons based on the given house,    *
 * only if that button is found in the list.                               *
 *                                                                         *
 * INPUT:                                                                  *
 *      house            house to set buttons to                           *
 *      btnlist         ptr to button list to search                       *
 *      base_id         button ID for GDI; assumes other id's are sequential*
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/23/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Set_House_Buttons(HousesType house, GadgetClass *btnlist, int base_id)
{
	HousesType h;
	int id;
	TextButtonClass *btn;

	/*
	**	Loop through all houses, searching the button list for each one.
	*/
	for (h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {

		/*
		**	Compute the desired button ID; get a pointer to the button
		*/
		id = (int)h + base_id;
		btn = (TextButtonClass *)btnlist->Extract_Gadget(id);
		if (btn) {

			/*
			**	If this house value is the desired one, turn the button on;
			**	otherwise, turn it off.
			*/
			if (h == house) {
				btn->Turn_On();
			} else {
				btn->Turn_Off();
			}
		}
	}
}


/***************************************************************************
 * MapEditClass::Start_Trigger_Placement -- enters trigger placement mode  *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/01/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Start_Trigger_Placement(void)
{
	Set_Default_Mouse(MOUSE_CAN_MOVE);
	Override_Mouse_Shape(MOUSE_CAN_MOVE);
}


/***************************************************************************
 * MapEditClass::Stop_Trigger_Placement -- exits trigger placement mode    *
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
 *   12/01/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Stop_Trigger_Placement(void)
{
	CurTrigger = NULL;
	Set_Default_Mouse(MOUSE_NORMAL);
	Override_Mouse_Shape(MOUSE_NORMAL);
}


/***************************************************************************
 * MapEditClass::Place_Trigger -- assigns trigger to object or cell        *
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
 *   12/01/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Place_Trigger(void)
{
	ObjectClass * object=NULL;		// Generic object clicked on.
	int x,y;
	CELL cell;									// Cell that was selected.

	/*
	-------------------- See if an object was clicked on ---------------------
	*/
	x = _Kbd->MouseQX;
	y = _Kbd->MouseQY;

	/*
	............................ Get cell for x,y ............................
	*/
	cell = Click_Cell_Calc(x, y);

	/*
	............... Convert x,y to offset from cell upper-left ...............
	*/
	x = (x-TacPixelX) % ICON_PIXEL_W;
	y = (y-TacPixelY) % ICON_PIXEL_H;

	/*
	......................... Get object at that x,y .........................
	*/
	object = Cell_Object(cell, x, y);

	/*
	---------------------- Assign trigger to an object -----------------------
	*/
	if (object && TriggerClass::Event_Need_Object(CurTrigger->Event)) {
		object->Trigger = CurTrigger;
	} else {

		/*
		------------------------ Assign trigger to a cell ------------------------
		*/
		if (CurTrigger->Event <= EVENT_OBJECTFIRST) {
			Map[cell].IsTrigger = 1;
			CellTriggers[cell] = CurTrigger;
		}
	}

	/*
	-------------------------- Force map to redraw ---------------------------
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
}


/***************************************************************************
 * MapEditClass::Start_Base_Building -- starts base-building mode          *
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
 *   12/01/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Start_Base_Building(void)
{
	/*
	** Fully build the base so the user can edit it
	*/
	Build_Base_To(100);

	/*
	** Start placement mode
	*/
	BaseBuilding = 1;
	Start_Placement();

	/*
	** Force map to redraw
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
}


/***************************************************************************
 * MapEditClass::Cancel_Base_Building -- stops base-building mode          *
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
 *   12/01/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Cancel_Base_Building(void)
{
	/*
	** Build the base to the proper amount
	*/
	Build_Base_To(BasePercent);

	/*
	** Cancel placement mode
	*/
	Cancel_Placement();
	BaseBuilding = 0;

	/*
	** Force map to redraw
	*/
	HiddenPage.Clear();
	Flag_To_Redraw(true);
}


/***************************************************************************
 * MapEditClass::Build_Base_To -- builds the AI base to the given percent  *
 *                                                                         *
 * INPUT:                                                                  *
 *      percent      percentage to build base to                           *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/01/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Build_Base_To(int percent)
{
	int i;
	int num_buildings;
	BuildingTypeClass const *objtype;
	BuildingClass *obj;

	//ScenarioInit++;

	/*
	** Completely dismantle the base, so we start at a known point
	*/
	for (i = 0; i < Base.Nodes.Count(); i++) {
		if (Base.Is_Built(i)) {
			obj = Base.Get_Building(i);
			delete obj;
		}
	}

	/*
	** Compute number of buildings to build
	*/
	num_buildings = (Base.Nodes.Count() * percent) / 100;

	/*
	** Build the base to the desired amount
	*/
	for (i = 0; i < num_buildings; i++) {
		/*
		** Get a ptr to the type of building to build, create one, and unlimbo it.
		*/
		objtype = &BuildingTypeClass::As_Reference(Base.Nodes[i].Type);
		obj = (BuildingClass *)objtype->Create_One_Of(HouseClass::As_Pointer(Base.House));
		/*
		** If unlimbo fails, error out
		*/
		if (!obj->Unlimbo(Base.Nodes[i].Coord)) {
			delete obj;
			CCMessageBox().Process("Unable to build base!");
			return;
		}
	}

	//ScenarioInit--;
}


#endif
