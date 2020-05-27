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

/* $Header: /CounterStrike/MAPEDSEL.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : MAPEDSEL.CPP                             *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : November 18, 1994                        *
 *                                                                         *
 *                  Last Update : April 30, 1996 [JLB]                     *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Object-selection & manipulation routines                                *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   MapEditClass::Change_House -- changes CurrentObject's house           *
 *   MapEditClass::Grab_Object -- grabs the current object                 *
 *   MapEditClass::Move_Grabbed_Object -- moves the grabbed object         *
 *   MapEditClass::Popup_Controls -- shows/hides the pop-up object controls*
 *   MapEditClass::Select_Next -- selects next object on the map           *
 *   MapEditClass::Select_Object -- selects an object for processing       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

#ifdef SCENARIO_EDITOR


/***************************************************************************
 * Select_Object -- selects an object for processing                       *
 *                                                                         *
 * INPUT:                                                                  *
 *   none.                                                                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *   0 = object selected, -1 = none                                        *
 *                                                                         *
 * WARNINGS:                                                               *
 *   none.                                                                 *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/04/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Select_Object(void)
{
	ObjectClass	* object=NULL;		// Generic object clicked on.
	int			x,y;
	CELL			cell;					// Cell that was selected.
	int			rc=0;

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
	**	If no object, unselect the current one
	*/
	if (!object) {
		if (CurrentObject.Count()) {

			/*
			**	Unselect all current objects
			*/
			Unselect_All();

			/*
			**	Turn off object controls
			*/
			Popup_Controls();
		}
		rc = -1;
	} else {

		/*
		**	Select object only if it's different
		*/
		if (!CurrentObject.Count() || (CurrentObject.Count() && object != CurrentObject[0])) {

			/*
			**	Unselect all current objects
			*/
			Unselect_All();
			object->Select();

			/*
			**	Set mouse shape back to normal
			*/
			Set_Default_Mouse(MOUSE_NORMAL);
			Override_Mouse_Shape(MOUSE_NORMAL);

			/*
			**	Show the popup controls
			*/
			Popup_Controls();
		}
	}

	/*
	**	Force map to redraw
	*/
	HidPage.Clear();
	Flag_To_Redraw(true);

	return(rc);
}


/***************************************************************************
 * MapEditClass::Select_Next -- selects next object on the map             *
 *                                                                         *
 * INPUT:                                                                  *
 *   none.                                                                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *   none                                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *   none.                                                                 *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/22/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Select_Next(void)
{
	ObjectClass * obj;
	CELL obj_cell;
	int smap_w;						// screen map width in icons
	int smap_h;						// screen map height in icons
	int cell_x;						// cell-x of next object
	int cell_y;						// cell-y of next object
	int tcell_x;					// cell-x of TacticalCell
	int tcell_y;					// cell-y of TacticalCell

	/*
	**	Get next object on the map
	*/
	obj = Map.Next_Object(CurrentObject[0]);

	if (obj) {
		/*
		**	Unselect current object if there is one
		*/
		Unselect_All();

	 	/*
		**	Select this object
		*/
		obj->Select();
	}

	/*
	**	Restore mouse shape to normal
	*/
	Set_Default_Mouse(MOUSE_NORMAL);
	Override_Mouse_Shape(MOUSE_NORMAL);

	/*
	**	Show pop-up controls
	*/
	Popup_Controls();

	/*
	**	Make sure object is shown on the screen
	*/
	/*
	**	compute screen map dimensions
	*/
	smap_w = Lepton_To_Cell(TacLeptonWidth);
	smap_h = Lepton_To_Cell(TacLeptonHeight);

	/*
	**	compute x,y of object's cell
	*/
	obj_cell = Coord_Cell(CurrentObject[0]->Coord);
	cell_x = Cell_X(obj_cell);
	cell_y = Cell_Y(obj_cell);
	tcell_x = Coord_XCell(TacticalCoord);
	tcell_y = Coord_YCell(TacticalCoord);

	/*
	**	If object is off-screen, move map
	*/
	if (cell_x < tcell_x) {
		tcell_x = cell_x;
	} else {
		if (cell_x >= tcell_x + smap_w) {
			tcell_x = cell_x - smap_w + 1;
		}
	}

	if (cell_y < tcell_y) {
		tcell_y = cell_y;
	} else {
		if (cell_y >= tcell_y + smap_h) {
			tcell_y = cell_y - smap_h + 1;
		}
	}

	ScenarioInit++;
	Set_Tactical_Position(XY_Coord(Cell_To_Lepton(tcell_x), Cell_To_Lepton(tcell_y)));
	ScenarioInit--;

	/*
	**	Force map to redraw
	*/
	HidPage.Clear();
	Flag_To_Redraw(true);
}


/***************************************************************************
 * MapEditClass::Popup_Controls -- shows/hides the pop-up object controls  *
 *                                                                         *
 * Call this routine whenever the CurrentObject changes. The routine will  *
 * selectively enable or disable the popup controls based on whether       *
 * CurrentObject is NULL, or if it's a Techno object, or what type of      *
 * Techno object it is.                                                    *
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
 *   11/22/1994 BR : Created.                                              *
 *   04/30/1996 JLB : Revamped for new buttons and stuff.                  *
 *=========================================================================*/
void MapEditClass::Popup_Controls(void)
{
	const TechnoTypeClass * objtype = NULL;
	HousesType owner;							// object's current owner
	int mission_index;						// object's current mission
	int strength;								// object's 0-255 strength value
	int i;

	/*
	**	Remove all buttons from GScreen's button list (so none of them provide
	**	input any more); then, destroy the list by Zapping each button.  Then,
	**	we'll have to add at least the MapArea button back to the Input button
	**	list before we return, plus any other buttons to process input for.  We
	**	always must add MapArea LAST in the list, so it doesn't intercept the
	**	other buttons' input.
	*/
	Remove_A_Button(*HouseList);
	Remove_A_Button(*MissionList);
	Remove_A_Button(*HealthGauge);
	Remove_A_Button(*HealthText);
	Remove_A_Button(*FacingDial);
	Remove_A_Button(*BaseGauge);
	Remove_A_Button(*BaseLabel);
	Remove_A_Button(*MapArea);

	Remove_A_Button(*Sellable);
	Remove_A_Button(*Rebuildable);

	/*
	**	If no current object, hide the list
	*/
	if (!CurrentObject.Count()) {
		Add_A_Button(*BaseGauge);
		Add_A_Button(*BaseLabel);
		Add_A_Button(*MapArea);
		return;
	}

	/*
	**	If not Techno, no need for editing buttons
	*/
	if (!CurrentObject[0]->Is_Techno()) {
		Add_A_Button(*BaseGauge);
		Add_A_Button(*BaseLabel);
		Add_A_Button(*MapArea);
		return;
	}

	objtype = (TechnoTypeClass const *)&CurrentObject[0]->Class_Of();

	/*
	**	Get object's current values
	*/
	owner = CurrentObject[0]->Owner();
	mission_index = 0;
	for (i = 0; i < NUM_EDIT_MISSIONS; i++) {
		if (CurrentObject[0]->Get_Mission() == MapEditMissions[i]) {
			mission_index = i;
		}
	}
	strength = CurrentObject[0]->Health_Ratio()*256;

	switch (objtype->What_Am_I()) {
		case RTTI_VESSELTYPE:
		case RTTI_UNITTYPE:
		case RTTI_INFANTRYTYPE:
		case RTTI_AIRCRAFTTYPE:
			MissionList->Set_Selected_Index(mission_index);
			HealthGauge->Set_Value(strength);
			sprintf(HealthBuf, "%d", CurrentObject[0]->Strength);
			FacingDial->Set_Direction(((TechnoClass *)CurrentObject[0])->PrimaryFacing);

			/*
			**	Make the list.
			*/
			Add_A_Button(*HealthGauge);
			Add_A_Button(*HouseList);
			HouseList->Set_Selected_Index(owner);
			Add_A_Button(*MissionList);
			Add_A_Button(*HealthText);
			Add_A_Button(*FacingDial);
			break;

		case RTTI_BUILDINGTYPE:
			HealthGauge->Set_Value(strength);
			sprintf(HealthBuf, "%d", CurrentObject[0]->Strength);
			Add_A_Button(*HealthGauge);
			Add_A_Button(*HouseList);
			HouseList->Set_Selected_Index(owner);
			Add_A_Button(*HealthText);

			Add_A_Button(*Sellable);
			if (((BuildingClass*)CurrentObject[0])->IsAllowedToSell) {
				Sellable->Turn_On();
			} else {
				Sellable->Turn_Off();
			}
			Add_A_Button(*Rebuildable);
			if (((BuildingClass*)CurrentObject[0])->IsToRebuild) {
				Rebuildable->Turn_On();
			} else {
				Rebuildable->Turn_Off();
			}

			if (objtype->IsTurretEquipped) {
				FacingDial->Set_Direction(((TechnoClass *) CurrentObject[0])->PrimaryFacing);
				Add_A_Button(*FacingDial);
			}
			break;
	}

	/*
	**	Add the map area last, so it's "underneath" the other buttons, and won't
	**	intercept input for those buttons.
	*/
	Add_A_Button(*BaseGauge);
	Add_A_Button(*BaseLabel);
	Add_A_Button(*MapArea);
}


/***************************************************************************
 * MapEditClass::Grab_Object -- grabs the current object                   *
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
 *   11/07/1994 BR : Created.                                              *
 *=========================================================================*/
void MapEditClass::Grab_Object(void)
{
	CELL cell;

	if (CurrentObject.Count()) {
		GrabbedObject = CurrentObject[0];

		/*
		**	Find out which cell 'ZoneCell' is in relation to the object's current cell
		*/
		cell = Coord_Cell(GrabbedObject->Coord);
		GrabOffset = cell - ZoneCell;
	}
}


/***************************************************************************
 * MapEditClass::Move_Grabbed_Object -- moves the grabbed object           *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = object moved, -1 = it didn't                                   *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/07/1994 BR : Created.                                              *
 *=========================================================================*/
int MapEditClass::Move_Grabbed_Object(void)
{
	COORDINATE new_coord = 0;
	int retval = -1;

	/*
	**	Lift up the object
	*/
	GrabbedObject->Mark(MARK_UP);

	/*
	**	If infantry, use a free spot in this cell
	*/
	if (GrabbedObject->Is_Infantry()) {

		if (Is_Spot_Free(Pixel_To_Coord(Get_Mouse_X(), Get_Mouse_Y()))) {
			new_coord = Closest_Free_Spot(Pixel_To_Coord(Get_Mouse_X(), Get_Mouse_Y()));

			/*
			**	Clear the occupied bit in this infantry's cell.
			*/
			((InfantryClass *)GrabbedObject)->Clear_Occupy_Bit(GrabbedObject->Coord);
		} else {
			new_coord = NULL;
		}

	} else {

		/*
		**	Non-infantry: use cell's center coordinate
		*/
		new_coord = Cell_Coord(ZoneCell + GrabOffset);

		if (GrabbedObject->What_Am_I() == RTTI_BUILDING ||
			GrabbedObject->What_Am_I() == RTTI_TERRAIN) {

			new_coord = Coord_Whole(new_coord);
		}

		/*
		**	Try to place object at new coordinate
		*/
		if (GrabbedObject->Can_Enter_Cell(Coord_Cell(new_coord)) != MOVE_OK) {
			new_coord = NULL;
		}
	}
	if (new_coord != NULL) {

		/*
		** If this object is part of the AI's Base list, change the coordinate
		** in the Base's Node list.
		*/
		if (GrabbedObject->What_Am_I()==RTTI_BUILDING &&
			Base.Get_Node((BuildingClass *)GrabbedObject))
				Base.Get_Node((BuildingClass *)GrabbedObject)->Cell = Coord_Cell(new_coord);

		GrabbedObject->Coord = new_coord;
		retval = 0;
	}
	GrabbedObject->Mark(MARK_DOWN);

	/*
	**	For infantry, set the bit in its new cell marking that spot as occupied.
	*/
	if (GrabbedObject->Is_Infantry()) {
		((InfantryClass *)GrabbedObject)->Set_Occupy_Bit(new_coord);
	}

	/*
	**	Re-select the object, and reset the mouse pointer
	*/
	Set_Default_Mouse(MOUSE_NORMAL);
	Override_Mouse_Shape(MOUSE_NORMAL);

	Flag_To_Redraw(true);

	return(retval);
}


/***************************************************************************
 * MapEditClass::Change_House -- changes CurrentObject's house             *
 *                                                                         *
 * INPUT:                                                                  *
 *      newhouse      house to change to                                   *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = house was changed, 0 = it wasn't                               *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/17/1994 BR : Created.                                              *
 *=========================================================================*/
bool MapEditClass::Change_House(HousesType newhouse)
{
	TechnoClass *tp;

	/*
	**	Return if no current object
	*/
	if (!CurrentObject.Count()) {
		return(false);
	}

	/*
	**	Only techno objects can be owned by a house; return if not a techno
	*/
	if (!CurrentObject[0]->Is_Techno()) {
		return(false);
	}

	/*
	**	You can't change the house if the object is part of the AI's Base.
	*/
	if (CurrentObject[0]->What_Am_I()==RTTI_BUILDING && Base.Is_Node((BuildingClass *)CurrentObject[0])) {
		return(false);
	}

	/*
	**	Verify that the target house exists
	*/
	if (HouseClass::As_Pointer(newhouse)==NULL) {
		return(false);
	}

	/*
	**	Verify that this is a valid owner
	*/
//	if (!Verify_House(newhouse, &CurrentObject[0]->Class_Of())) {
//		return(false);
//	}

	/*
	**	Change the house
	*/
	tp = (TechnoClass *)CurrentObject[0];
	tp->House = HouseClass::As_Pointer(newhouse);

	tp->IsOwnedByPlayer = false;
	if (tp->House == PlayerPtr) {
		tp->IsOwnedByPlayer = true;
	}

	return(true);
}


#endif
