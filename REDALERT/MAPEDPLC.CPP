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

/* $Header: /CounterStrike/MAPEDPLC.CPP 1     3/03/97 10:25a Joe_bostic $ */
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
 *                  Last Update : May 12, 1996 [JLB]                       *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Object-placement routines                                               *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   MapEditClass::Build_Base_To -- builds the AI base to the given percent*
 *   MapEditClass::Cancel_Base_Building -- stops base-building mode        *
 *   MapEditClass::Cancel_Placement -- cancels placement mode              *
 *   MapEditClass::Place_Home -- homes the placement object                *
 *   MapEditClass::Place_Next -- while placing object, goes to next        *
 *   MapEditClass::Place_Next_Category -- places next object category      *
 *   MapEditClass::Place_Object -- attempts to place the current object    *
 *   MapEditClass::Place_Prev -- while placing object, goes to previous    *
 *   MapEditClass::Place_Prev_Category -- places previous object category  *
 *   MapEditClass::Place_Trigger -- assigns trigger to object or cell      *
 *   MapEditClass::Placement_Dialog -- adds an object to the scenario      *
 *   MapEditClass::Set_House_Buttons -- toggles house buttons for btn list *
 *   MapEditClass::Start_Base_Building -- starts base-building mode        *
 *   MapEditClass::Start_Placement -- enters placement mode                *
 *   MapEditClass::Start_Trigger_Placement -- enters trigger placement mode*
 *   MapEditClass::Stop_Trigger_Placement -- exits trigger placement mode  *
 *   MapEditClass::Toggle_House -- toggles current placement object's house*
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
 *   12/13/1995 JLB : Fixed house buttons to handle expanded house list.   *
 *   05/12/1996 JLB : Handles hi-res.                                      *
 *=========================================================================*/
int MapEditClass::Placement_Dialog(void)
{
	HousesType house;
	RemapControlType * scheme = GadgetClass::Get_Color_Scheme();

	/*
	**	Dialog & button dimensions
	*/
	enum {
		D_DIALOG_W = 400,
		D_DIALOG_H = 180,
		D_DIALOG_X = 0,
		D_DIALOG_Y = 0,
		D_DIALOG_CX = D_DIALOG_X + (D_DIALOG_W / 2),

		D_TXT8_H = 11,
		D_MARGIN = 7,

		D_PICTURE_W = 152,					// must be divisible by 8!
		D_PICTURE_H = 105,
		D_PICTURE_X = D_DIALOG_X + 35,		// must start on a byte boundary!
		D_PICTURE_Y = D_DIALOG_Y + D_MARGIN + D_TXT8_H + D_MARGIN,
		D_PICTURE_CX = D_PICTURE_X + D_PICTURE_W / 2,

		D_GDI_W = 65,
		D_GDI_H = 9,
		D_GDI_X = D_PICTURE_X+D_PICTURE_W+5,
		D_GDI_Y = D_PICTURE_Y,

		D_LEFT_W = 45,
		D_LEFT_H = 9,
		D_LEFT_X = D_PICTURE_CX - 5 - D_LEFT_W,
		D_LEFT_Y = D_PICTURE_Y + D_PICTURE_H + D_MARGIN,

		D_RIGHT_W = 45,
		D_RIGHT_H = 9,
		D_RIGHT_X = D_PICTURE_CX + 5,
		D_RIGHT_Y = D_PICTURE_Y + D_PICTURE_H + D_MARGIN,

		D_TEMPLATE_W = 70,
		D_TEMPLATE_H = 9,
		D_TEMPLATE_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_TEMPLATE_W - 30,
		D_TEMPLATE_Y = D_PICTURE_Y,

		D_OVERLAY_W = 70,
		D_OVERLAY_H = 9,
		D_OVERLAY_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_OVERLAY_W - 30,
		D_OVERLAY_Y = D_TEMPLATE_Y + D_TEMPLATE_H,

		D_SMUDGE_W = 70,
		D_SMUDGE_H = 9,
		D_SMUDGE_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_SMUDGE_W - 30,
		D_SMUDGE_Y = D_OVERLAY_Y + D_OVERLAY_H,

		D_TERRAIN_W = 70,
		D_TERRAIN_H = 9,
		D_TERRAIN_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_TERRAIN_W - 30,
		D_TERRAIN_Y = D_SMUDGE_Y + D_SMUDGE_H,

		D_UNIT_W = 70,
		D_UNIT_H = 9,
		D_UNIT_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_UNIT_W - 30,
		D_UNIT_Y = D_TERRAIN_Y + D_TERRAIN_H,

		D_INFANTRY_W = 70,
		D_INFANTRY_H = 9,
		D_INFANTRY_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_INFANTRY_W - 30,
		D_INFANTRY_Y = D_UNIT_Y + D_UNIT_H,

		D_AIRCRAFT_W = 70,
		D_AIRCRAFT_H = 9,
		D_AIRCRAFT_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_AIRCRAFT_W - 30,
		D_AIRCRAFT_Y = D_INFANTRY_Y + D_INFANTRY_H,

		D_BUILDING_W = 70,
		D_BUILDING_H = 9,
		D_BUILDING_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_BUILDING_W - 30,
		D_BUILDING_Y = D_AIRCRAFT_Y + D_AIRCRAFT_H,

		D_AIR_W = 70,
		D_AIR_H = 9,
		D_AIR_X = D_DIALOG_X + D_DIALOG_W - D_MARGIN - D_AIR_W - 30,
		D_AIR_Y = D_BUILDING_Y + D_BUILDING_H,

		D_OK_W = 45,
		D_OK_H = 9,
		D_OK_X = D_PICTURE_CX - D_OK_W - 5,
		D_OK_Y = D_DIALOG_Y + D_DIALOG_H - D_OK_H - D_MARGIN - 15,

		D_CANCEL_W = 45,
		D_CANCEL_H = 9,
		D_CANCEL_X = D_PICTURE_CX + 5,
		D_CANCEL_Y = D_DIALOG_Y + D_DIALOG_H - D_CANCEL_H - D_MARGIN - 15,

		GRIDSIZE = 10,
		GRIDBLOCK_W = 3,
		GRIDBLOCK_H = 3,
		D_GRID_X = D_DIALOG_X + D_DIALOG_W - (GRIDSIZE * GRIDBLOCK_W) - D_MARGIN - 35,
		D_GRID_Y = D_DIALOG_Y + D_DIALOG_H - (GRIDSIZE * GRIDBLOCK_H) - D_MARGIN - 35,
	};

	/*
	**	Button enumerations:
	*/
	enum {
		BUTTON_GDI=100,
		BUTTON_HOUSE,
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
		BUTTON_AIR,
	};

	/*
	**	Dialog variables
	*/
	bool cancel = false;							// true = user cancels
	const ObjectTypeClass * curobj;			// Working object pointer.
	int x,y;											// for drawing the grid
	KeyNumType input;								// user input
	short const * occupy;							// ptr into object's OccupyList
	int cell;										// cell index for parsing OccupyList
	int i;
	int typeindex;									// index of class type

	/*
	**	Buttons
	*/
	ControlClass * commands;

	ListClass housebtn(BUTTON_HOUSE,
		D_GDI_X, D_GDI_Y, 60, 8*16,
		TPF_EFNT | TPF_NOSHADOW,
		MFCD::Retrieve("EBTN-UP.SHP"),
		MFCD::Retrieve("EBTN-DN.SHP"));
	for (house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
		housebtn.Add_Item(HouseTypeClass::As_Reference(house).IniName);
	}
	house = HOUSE_FIRST;

	TextButtonClass nextbtn(BUTTON_NEXT, TXT_RIGHT, TPF_EBUTTON, D_RIGHT_X, D_RIGHT_Y, D_RIGHT_W, D_RIGHT_H);
	TextButtonClass prevbtn(BUTTON_PREV, TXT_LEFT, TPF_EBUTTON, D_LEFT_X, D_LEFT_Y, D_LEFT_W, D_LEFT_H);
	TextButtonClass okbtn(BUTTON_OK, "OK", TPF_EBUTTON, D_OK_X, D_OK_Y, D_OK_W, D_OK_H);
	TextButtonClass cancelbtn(BUTTON_CANCEL, "Cancel", TPF_EBUTTON, D_CANCEL_X, D_CANCEL_Y, D_CANCEL_W, D_CANCEL_H);
	TextButtonClass templatebtn(BUTTON_TEMPLATE, "Template", TPF_EBUTTON, D_TEMPLATE_X, D_TEMPLATE_Y, D_TEMPLATE_W, D_TEMPLATE_H);
	TextButtonClass overlaybtn(BUTTON_OVERLAY, "Overlay", TPF_EBUTTON, D_OVERLAY_X, D_OVERLAY_Y, D_OVERLAY_W, D_OVERLAY_H);
	TextButtonClass smudgebtn(BUTTON_SMUDGE, "Smudge", TPF_EBUTTON, D_SMUDGE_X, D_SMUDGE_Y, D_SMUDGE_W, D_SMUDGE_H);
	TextButtonClass terrainbtn(BUTTON_TERRAIN, "Terrain", TPF_EBUTTON, D_TERRAIN_X, D_TERRAIN_Y, D_TERRAIN_W, D_TERRAIN_H);
	TextButtonClass unitbtn(BUTTON_UNIT, "Unit", TPF_EBUTTON, D_UNIT_X, D_UNIT_Y, D_UNIT_W, D_UNIT_H);
	TextButtonClass infantrybtn(BUTTON_INFANTRY, "Infantry", TPF_EBUTTON, D_INFANTRY_X, D_INFANTRY_Y, D_INFANTRY_W, D_INFANTRY_H);
	TextButtonClass aircraftbtn(BUTTON_AIRCRAFT, "Ships", TPF_EBUTTON, D_AIRCRAFT_X, D_AIRCRAFT_Y, D_AIRCRAFT_W, D_AIRCRAFT_H);
	TextButtonClass buildingbtn(BUTTON_BUILDING, "Building", TPF_EBUTTON, D_BUILDING_X, D_BUILDING_Y, D_BUILDING_W, D_BUILDING_H);
	TextButtonClass airbtn(BUTTON_AIR, "Aircraft", TPF_EBUTTON, D_AIR_X, D_AIR_Y, D_AIR_W, D_AIR_H);

	/*
	**	Initialize addable objects list; we must do this every time in case one
	**	of the object pools has become exhausted; that object won't be available
	**	for adding.  (Skip aircraft, since they won't be used in the editor.)
	*/
	Clear_List();
	TemplateTypeClass::Prep_For_Add();
	OverlayTypeClass::Prep_For_Add();
	SmudgeTypeClass::Prep_For_Add();
	TerrainTypeClass::Prep_For_Add();
	UnitTypeClass::Prep_For_Add();
	InfantryTypeClass::Prep_For_Add();
	VesselTypeClass::Prep_For_Add();
	BuildingTypeClass::Prep_For_Add();
	AircraftTypeClass::Prep_For_Add();

	/*
	**	Compute offset of each class type in the Objects array
	*/
	TypeOffset[0] = 0;
	for (i = 1; i < NUM_EDIT_CLASSES; i++) {
		TypeOffset[i] = TypeOffset[i-1] + NumType[i-1];
	}

	/*
	**	Return if no objects to place
	*/
	if (!ObjCount)  {
		return(-1);
	}

	/*
	**	Initialize
	*/
	Set_Logic_Page(SeenBuff);
	if (LastChoice >= ObjCount) {
		LastChoice = 0;
	}
	curobj = Objects[LastChoice];		// current object to choose

	commands = &nextbtn;
	housebtn.Add_Tail(*commands);
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
	airbtn.Add_Tail(*commands);

	/*
	**	Make sure the recorded house selection matches the house list
	**	box selection.
	*/
	LastHouse = HousesType(housebtn.Current_Index());

	/*
	**	Main processing loop
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

			/*
			**	Display the dialog box
			*/
			Hide_Mouse();
			Dialog_Box(D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W, D_DIALOG_H);
			Draw_Caption(TXT_PLACE_OBJECT, D_DIALOG_X, D_DIALOG_Y, D_DIALOG_W);

			/*
			**	Display the current object:
			**	- save the current window dimensions
			**	- adjust the window size to the actual drawable area
			**	- draw the shape
			**	- reset the window dimensions
			*/
			WindowList[WINDOW_EDITOR][WINDOWX] = D_PICTURE_X;
			WindowList[WINDOW_EDITOR][WINDOWY] = D_PICTURE_Y;
			WindowList[WINDOW_EDITOR][WINDOWWIDTH] = D_PICTURE_W;
			WindowList[WINDOW_EDITOR][WINDOWHEIGHT] = D_PICTURE_H;
			Change_Window((int)WINDOW_EDITOR);
			Draw_Box(D_PICTURE_X, D_PICTURE_Y, D_PICTURE_W, D_PICTURE_H, BOXSTYLE_DOWN, false);
			curobj->Display(WinW/2, WinH>>1, WINDOW_EDITOR, LastHouse);
//			curobj->Display(WinW<<2, WinH>>1, WINDOW_EDITOR, LastHouse);

			/*
			**	Erase the grid
			*/
			LogicPage->Fill_Rect(D_GRID_X - GRIDBLOCK_W * 2, D_GRID_Y,
				D_GRID_X + GRIDSIZE * GRIDBLOCK_W,
				D_GRID_Y + GRIDSIZE * GRIDBLOCK_H, BLACK);

			/*
			**	Draw a box for every cell occupied
			*/
			occupy = curobj->Occupy_List();
			while ( (*occupy) != REFRESH_EOL) {
				cell = (*occupy);
				occupy++;
				x = D_GRID_X + ((cell % MAP_CELL_W) * GRIDBLOCK_W);
				y = D_GRID_Y + ((cell / MAP_CELL_W) * GRIDBLOCK_H);
				LogicPage->Fill_Rect(x, y, x + GRIDBLOCK_W - 1, y + GRIDBLOCK_H - 1, scheme->Bright);
			}

			/*
			**	Draw the grid itself
			*/
			for (y = 0; y <= GRIDSIZE; y++) {
				for (x = 0; x <= GRIDSIZE; x++) {
					LogicPage->Draw_Line(D_GRID_X + x * GRIDBLOCK_W, D_GRID_Y,
						D_GRID_X + x * GRIDBLOCK_W,
						D_GRID_Y + GRIDSIZE * GRIDBLOCK_H, scheme->Shadow);
				}
				LogicPage->Draw_Line(D_GRID_X, D_GRID_Y + y * GRIDBLOCK_H,
					D_GRID_X + GRIDSIZE * GRIDBLOCK_W, D_GRID_Y + y * GRIDBLOCK_H,
					scheme->Shadow);
			}

			/*
			**	Print the object's label from the class's Full_Name().
			**	Warning: Text_String returns an EMS pointer, so standard string
			**	functions won't work!
			*/
			Fancy_Text_Print (curobj->Full_Name(),
				D_PICTURE_CX, D_PICTURE_Y + D_MARGIN, scheme, TBLACK,
				TPF_CENTER | TPF_EFNT | TPF_NOSHADOW);

			/*
			**	Redraw buttons
			**	Figure out which class category we're in & highlight that button
			**	This updates 'typeindex', which is used below, and it also updates
			**	the category button states.
			*/
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
			airbtn.Turn_Off();
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

				case BUTTON_AIR:
					airbtn.Turn_On();
					break;

				case BUTTON_BUILDING:
					buildingbtn.Turn_On();
					break;
			}

			/*
			**	Redraw buttons
			*/
			commands->Draw_All();
			Show_Mouse();
			display = false;

		}

		/*
		**	Get user input
		*/
		input = commands->Input();

		/*
		**	Process user input
		*/
		switch (input) {

			/*
			**	GDI House
			*/
			case (BUTTON_HOUSE | KN_BUTTON):
				house = HousesType(housebtn.Current_Index());

				/*
				**	Set flags & buttons
				*/
				LastHouse = house;
				display = true;
				break;

			/*
			**	Next in list
			*/
			case (KN_RIGHT):
			case (BUTTON_NEXT | KN_BUTTON):
				/*
				**	Increment to next obj
				*/
				LastChoice++;
				if (LastChoice == ObjCount) {
					LastChoice = 0;
				}
				curobj = Objects[LastChoice];

				nextbtn.Turn_Off();
				display = true;
				break;

			/*
			**	Previous in list
			*/
			case (KN_LEFT):
			case (BUTTON_PREV | KN_BUTTON):

				/*
				**	Decrement to prev obj
				*/
				LastChoice--;
				if (LastChoice < 0) {
					LastChoice = ObjCount-1;
				}
				curobj = Objects[LastChoice];
				prevbtn.Turn_Off();
				display = true;
				break;

			/*
			**	Select a class type
			*/
			case (BUTTON_TEMPLATE | KN_BUTTON):
			case (BUTTON_OVERLAY | KN_BUTTON):
			case (BUTTON_SMUDGE | KN_BUTTON):
			case (BUTTON_TERRAIN | KN_BUTTON):
			case (BUTTON_UNIT | KN_BUTTON):
			case (BUTTON_INFANTRY | KN_BUTTON):
			case (BUTTON_AIRCRAFT | KN_BUTTON):
			case (BUTTON_BUILDING | KN_BUTTON):
			case (BUTTON_AIR | KN_BUTTON):

				/*
				**	Find index of class
				*/
				typeindex = input - (BUTTON_TEMPLATE | KN_BUTTON);

				/*
				**	If no objects of that type, do nothing
				*/
				if (NumType[typeindex]==0) {
					display = true;
					break;
				}

				/*
				**	Set current object
				*/
				LastChoice = TypeOffset[typeindex];
				curobj = Objects[LastChoice];
				display = true;
				break;

			/*
			**	Next category
			*/
			case KN_PGDN:
				typeindex++;
				if (typeindex==NUM_EDIT_CLASSES) {
					typeindex = 0;
				}

				/*
				**	Set current object
				*/
				LastChoice = TypeOffset[typeindex];
				curobj = Objects[LastChoice];
				display = true;
				break;

			/*
			**	Previous category
			*/
			case KN_PGUP:
				typeindex--;
				if (typeindex < 0) {
					typeindex = NUM_EDIT_CLASSES - 1;
				}

				/*
				**	Set current object
				*/
				LastChoice = TypeOffset[typeindex];
				curobj = Objects[LastChoice];
				display = true;
				break;

			/*
			**	Jump to 1st choice
			*/
			case KN_HOME:
				LastChoice = 0;

				/*
				**	Set current object
				*/
				curobj = Objects[LastChoice];
				display = true;
				break;

			/*
			**	OK
			*/
			case (KN_RETURN):
			case (BUTTON_OK | KN_BUTTON):
				cancel = false;
				process = false;
				break;

			/*
			**	Cancel
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
	**	Redraw the display
	*/
	HidPage.Clear();
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

	/*
	**	Initialize addable objects list; we must do this every time in case one
	**	of the object pools has become exhausted; that object won't be available
	**	for adding. These must be added in the same order expected by the
	**	object selection dialog (same as button order).
	*/
	Clear_List();
	TemplateTypeClass::Prep_For_Add();
	OverlayTypeClass::Prep_For_Add();
	SmudgeTypeClass::Prep_For_Add();
	TerrainTypeClass::Prep_For_Add();
	UnitTypeClass::Prep_For_Add();
	InfantryTypeClass::Prep_For_Add();
	VesselTypeClass::Prep_For_Add();
	BuildingTypeClass::Prep_For_Add();
	AircraftTypeClass::Prep_For_Add();

	/*
	**	Compute offset of each class type in the Objects array
	*/
	TypeOffset[0] = 0;
	for (int i = 1; i < NUM_EDIT_CLASSES; i++) {
		TypeOffset[i] = TypeOffset[i-1] + NumType[i-1];
	}

	/*
	**	Create the placement object:
	**	- For normal placement mode, use the last-used index into Objects
	**	  (LastChoice), and the last-used house (LastHouse).
	**	- For base-building mode, force the object to be a building, and use the
	**	  House specified in the Base object
	*/
	if (!BaseBuilding) {
		if (LastChoice >= ObjCount) {
			LastChoice = ObjCount - 1;
		}
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(LastHouse));
	} else {
		if (LastChoice < TypeOffset[7]) {
			LastChoice = TypeOffset[7];
		}
		if (LastChoice >= ObjCount) {
			LastChoice = ObjCount - 1;
		}
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse = Base.House;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(LastHouse));
	}

	/*
	**	Error if no more objects available
	*/
	if (!PendingObjectPtr) {
		WWMessageBox().Process("No more objects of this type available.");
		HidPage.Clear();
		Flag_To_Redraw(true);
		Render();
		PendingObject = NULL;
		if (BaseBuilding) {
			Cancel_Base_Building();
		}
		return;
	}

	/*
	**	Set the placement cursor
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
	short const * occupy;						// ptr into template's OccupyList
	ObjectClass * occupier;					// occupying object
	TemplateType save_ttype;				// for saving cell's TType
	unsigned char save_ticon;				// for saving cell's TIcon
//	BaseNodeClass node;						// for adding to an AI Base

	/*
	**	Placing a template:
	**	- first lift up any objects in the cell
	**	- place the template, and try to replace the objects; if they won't go
	**	  back, the template can't go there
	*/
	//ScenarioInit++;
	if (PendingObject->What_Am_I() == RTTI_TEMPLATETYPE) {

		/*
		**	Loop through all cells this template will occupy
		*/
		okflag = true;
		occupy = PendingObject->Occupy_List();
		while ((*occupy) != REFRESH_EOL) {

			/*
			**	Check this cell for an occupier
			*/
			template_cell = (ZoneCell+ZoneOffset) + (*occupy);
			if ((*this)[template_cell].Cell_Occupier()) {
				occupier = (*this)[template_cell].Cell_Occupier();

				/*
				**	Save object's coordinates
				*/
				obj_coord = occupier->Coord;

				/*
				**	Place the object in limbo
				*/
				occupier->Mark(MARK_UP);

				/*
				**	Set the cell's template values
				*/
				save_ttype = (*this)[template_cell].TType;
				save_ticon = (*this)[template_cell].TIcon;
				(*this)[template_cell].TType =
					((TemplateTypeClass *)PendingObject)->Type;
				(*this)[template_cell].TIcon = Cell_X(*occupy) + Cell_Y(*occupy) *
					((TemplateTypeClass *)PendingObject)->Width;
				(*this)[template_cell].Recalc_Attributes();

				/*
				**	Try to put the object back down
				*/
				if (occupier->Can_Enter_Cell(Coord_Cell(obj_coord)) != MOVE_OK) {
					okflag = false;
				}

				/*
				**	Put everything back the way it was
				*/
				(*this)[template_cell].TType = save_ttype;
				(*this)[template_cell].TIcon = save_ticon;
				(*this)[template_cell].Recalc_Attributes();

				/*
				**	Major error if can't replace the object now
				*/
				occupier->Mark(MARK_DOWN);
			}
			occupy++;
		}

		/*
		**	If it's still OK after ALL THAT, place the template
		*/
		if (okflag) {
			if (PendingObjectPtr->Unlimbo(Cell_Coord(ZoneCell + ZoneOffset))) {

				/*
				**	Loop through all cells occupied by this template, and clear the
				**	smudge & overlay.
				*/
				occupy = PendingObject->Occupy_List();
				while ((*occupy) != REFRESH_EOL) {

					/*
					**	Get cell for this occupy item
					*/
					template_cell = (ZoneCell+ZoneOffset) + (*occupy);

					/*
					**	Clear smudge & overlay
					*/
					(*this)[template_cell].Overlay = OVERLAY_NONE;
					(*this)[template_cell].OverlayData = 0;
					(*this)[template_cell].Smudge = SMUDGE_NONE;

					/*
					**	make adjacent cells recalc attrib's
					*/
					(*this)[template_cell].Recalc_Attributes();
					(*this)[template_cell].Wall_Update();
					(*this)[template_cell].Concrete_Calc();

					occupy++;
				}

				/*
				**	Set flags etc
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
		**	Not OK; return error
		*/
		//ScenarioInit--;
		return(-1);
	}

	/*
	**	Placing infantry: Infantry can go into cell sub-positions, so find the
	**	sub-position closest to the mouse & put him there
	*/
	if (PendingObject->What_Am_I() == RTTI_INFANTRYTYPE) {

		/*
		**	Find cell sub-position
		*/
		if (Is_Spot_Free(Pixel_To_Coord(Get_Mouse_X(), Get_Mouse_Y()))) {
			obj_coord = Closest_Free_Spot(Pixel_To_Coord(Get_Mouse_X(), Get_Mouse_Y()));
		} else {
			obj_coord = NULL;
		}

		/*
		**	No free spots; don't place the object
		*/
		if (obj_coord == NULL) {
			//ScenarioInit--;
			return(-1);
		}

		/*
		**	Unlimbo the object
		*/
		if (PendingObjectPtr->Unlimbo(obj_coord)) {
			((InfantryClass *)PendingObjectPtr)->Set_Occupy_Bit(obj_coord);
//			Map[obj_coord].Flag.Composite |=
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

	/*
	**	Placing an object
	*/
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
//			node.Type = ((BuildingTypeClass *)PendingObject)->Type;
//			node.Cell = Coord_Cell(PendingObjectPtr->Coord);
			Base.Nodes.Add(BaseNodeClass(((BuildingTypeClass *)PendingObject)->Type, Coord_Cell(PendingObjectPtr->Coord)));
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
	**	Delete the placement object
	*/
	delete PendingObjectPtr;
	PendingObject = 0;
	PendingObjectPtr = 0;
	PendingHouse = HOUSE_NONE;

	/*
	**	Restore cursor shape
	*/
	Set_Cursor_Shape(0);

	/*
	**	Redraw the map to erase old leftovers
	*/
	HidPage.Clear();
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
	**	Loop until we create a valid object
	*/
	while (!PendingObjectPtr) {
		/*
		**	Go to next object in Objects list
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
		**	Create placement object
		*/
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(PendingHouse));
		if (!PendingObjectPtr) {
			PendingObject = NULL;
		}
	}

	/*
	**	Set the new cursor shape
	*/
	Set_Cursor_Pos();
	Set_Cursor_Shape(0);
	Set_Cursor_Shape(PendingObject->Occupy_List());

	/*
	**	Redraw the map to erase old leftovers
	*/
	HidPage.Clear();
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
	**	Loop until we create a valid object
	*/
	while (!PendingObjectPtr) {

		/*
		**	Go to prev object in Objects list
		*/
		LastChoice--;

		/*
		** If we're in normal placement mode, wrap at the 1st object.
		** If we're building a base, wrap at the 1st building.
		*/
		if (!BaseBuilding) {
			if (LastChoice < 0) {
				LastChoice = ObjCount - 1;
			}
		} else {
			if (LastChoice < TypeOffset[7]) {
				LastChoice = ObjCount - 1;
			}
		}

		/*
		**	Create placement object
		*/
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(PendingHouse));
		if (!PendingObjectPtr) {
			PendingObject = NULL;
		}
	}

	/*
	**	Set the new cursor shape
	*/
	Set_Cursor_Pos();
	Set_Cursor_Shape(0);
	Set_Cursor_Shape(PendingObject->Occupy_List());

	/*
	**	Redraw the map to erase old leftovers
	*/
	HidPage.Clear();
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
	**	Go to next category in Objects list
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
	**	Loop until we create a valid object
	*/
	while (!PendingObjectPtr) {

		/*
		**	Get house for this object type
		*/
//		if (!Verify_House(LastHouse, Objects[LastChoice])) {
//			LastHouse = Cycle_House(LastHouse, Objects[LastChoice]);
//		}

		/*
		**	Create placement object
		*/
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(PendingHouse));

		/*
		**	If this one failed, try the next
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
	**	Set the new cursor shape
	*/
	Set_Cursor_Pos();
	Set_Cursor_Shape(0);
	Set_Cursor_Shape(PendingObject->Occupy_List());

	/*
	**	Redraw the map to erase old leftovers
	*/
	HidPage.Clear();
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
	**	Go to prev category in Objects list
	*/
	i = LastChoice;

	/*
	**	Scan for start of this category
	*/
	while (Objects[i]->What_Am_I() == Objects[LastChoice]->What_Am_I()) {
		i--;
		if (i < 0) {
			i = ObjCount - 1;
		}
	}

	i--;
	if (i < 0) i = ObjCount-1;
	LastChoice = i;

	/*
	**	Scan for the previous category
	*/
	while (Objects[i]->What_Am_I() == Objects[LastChoice]->What_Am_I()) {
		i--;
		if (i < 0) {
			i = ObjCount - 1;
		}
	}

	i++;
	if (i >= ObjCount) i = 0;
	LastChoice = i;

	/*
	**	Loop until we create a valid object
	*/
	while (!PendingObjectPtr) {

		/*
		**	Get house for this object type
		*/
//		if (!Verify_House(LastHouse, Objects[LastChoice])) {
//			LastHouse = Cycle_House(LastHouse, Objects[LastChoice]);
//		}

		/*
		**	Create placement object
		*/
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(PendingHouse));

		/*
		**	If this one failed, try the next
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
	**	Set the new cursor shape
	*/
	Set_Cursor_Pos();
	Set_Cursor_Shape(0);
	Set_Cursor_Shape(PendingObject->Occupy_List());

	/*
	**	Redraw the map to erase old leftovers
	*/
	HidPage.Clear();
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
	**	Loop until we create a valid object
	*/
	LastChoice = 0;
	while (!PendingObjectPtr) {

		/*
		**	Get house for this object type
		*/
		if (!Verify_House(LastHouse, Objects[LastChoice])) {
			LastHouse = Cycle_House(LastHouse, Objects[LastChoice]);
		}

		/*
		**	Create placement object
		*/
		PendingObject = Objects[LastChoice];
		PendingHouse = LastHouse;
		PendingObjectPtr = PendingObject->Create_One_Of(HouseClass::As_Pointer(PendingHouse));

		/*
		**	If this one failed, try the next
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
	**	Set the new cursor shape
	*/
	Set_Cursor_Pos();
	Set_Cursor_Shape(0);
	Set_Cursor_Shape(PendingObject->Occupy_List());

	/*
	**	Redraw the map to erase old leftovers
	*/
	HidPage.Clear();
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

	/*
	**	Only techno objects can be owned by a house; return if not a techno
	*/
	if (!PendingObjectPtr->Is_Techno()) {
		return;
	}

	/*
	**	Select the house that will own this object
	*/
	LastHouse = Cycle_House(PendingObjectPtr->Owner(), PendingObject);

	/*
	**	Change the house
	*/
	tp = (TechnoClass *)PendingObjectPtr;
	tp->House = HouseClass::As_Pointer(LastHouse);

	/*
	**	Set house variables to new house
	*/
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
 *   01/26/1996 JLB : Uses new house selection list method.                *
 *=========================================================================*/
void MapEditClass::Set_House_Buttons(HousesType house, GadgetClass *, int )
//void MapEditClass::Set_House_Buttons(HousesType house, GadgetClass * btnlist, int base_id)
{
	HouseList->Set_Selected_Index(house);

#ifdef NEVER
	HousesType h;
	int id;
	TextButtonClass * btn;

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
#endif
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
	**	See if an object was clicked on
	*/
	x = Keyboard->MouseQX;
	y = Keyboard->MouseQY;

	/*
	**	Get cell for x,y
	*/
	cell = Click_Cell_Calc(x, y);

	/*
	**	Convert x,y to offset from cell upper-left
	*/
	x = (x-TacPixelX) % ICON_PIXEL_W;
	y = (y-TacPixelY) % ICON_PIXEL_H;

	/*
	**	Get object at that x,y
	*/
	object = Cell_Object(cell, x, y);

	/*
	**	Assign trigger to an object
	*/
	AttachType a1 = CurTrigger->Attaches_To();
	if (object && (a1 & ATTACH_OBJECT) != 0) {
		if (CurTrigger) {
			TriggerClass * tt = Find_Or_Make(CurTrigger);
			if (tt) {
				object->Trigger = tt;
			}
		}
	} else {

		/*
		**	Assign trigger to a cell
		*/
		if ((a1 & ATTACH_CELL) != 0) {
			if (CurTrigger) {
				TriggerClass * tt = Find_Or_Make(CurTrigger);
				Map[cell].Trigger = tt;
			}
//			CellTriggers[cell] = CurTrigger;
		}
	}

	/*
	**	Force map to redraw
	*/
	HidPage.Clear();
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
	BaseBuilding = true;
	Start_Placement();

	/*
	** Force map to redraw
	*/
	HidPage.Clear();
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
	Build_Base_To(Scen.Percent);

	/*
	** Cancel placement mode
	*/
	Cancel_Placement();
	BaseBuilding = false;

	/*
	** Force map to redraw
	*/
	HidPage.Clear();
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
	BuildingTypeClass const * objtype;
	BuildingClass * obj;

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
		ScenarioInit++;
		if (!obj->Unlimbo(Cell_Coord(Base.Nodes[i].Cell))) {
			delete obj;
			WWMessageBox().Process("Unable to build base!");
			ScenarioInit--;
			return;
		}
		ScenarioInit--;
	}

	//ScenarioInit--;
}


#endif
