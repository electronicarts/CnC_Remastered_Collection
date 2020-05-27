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

/* $Header:   F:\projects\c&c\vcs\code\cell.cpv   2.18   16 Oct 1995 16:49:20   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CELL.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 29, 1994                                               *
 *                                                                                             *
 *                  Last Update : August 17, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CellClass::Adjacent_Cell -- Determines the adjacent cell according to facing.             *
 *   CellClass::Adjust_Threat -- Allows adjustment of threat at cell level                     *
 *   CellClass::CellClass -- Constructor for cell objects.                                     *
 *   CellClass::Cell_Building -- Return with building at specified cell.                       *
 *   CellClass::Cell_Color   -- Determine what radar color to use for this cell.               *
 *   CellClass::Cell_Coord -- Returns the coordinate of this cell.                             *
 *   CellClass::Cell_Find_Object -- Returns ptr to RTTI type occupying cell                    *
 *   CellClass::Cell_Infantry -- Returns with pointer of first infantry unit.                  *
 *   CellClass::Cell_Object -- Returns with clickable object in cell.                          *
 *   CellClass::Cell_Techno -- Return with the unit/building at specified cell.                *
 *   CellClass::Cell_Terrain -- Determines terrain object in cell.                             *
 *   CellClass::Cell_Unit -- Returns with pointer to unit occupying cell.                      *
 *   CellClass::Clear_Icon -- Calculates what the clear icon number should be.                 *
 *   CellClass::Closest_Free_Spot -- returns free spot closest to given coord                  *
 *   CellClass::Concrete_Calc -- Calculates the concrete icon to use for the cell.             *
 *   CellClass::Draw_It -- Draws the cell imagery at the location specified.                   *
 *   CellClass::Flag_Place -- Places a house flag down on the cell.                            *
 *   CellClass::Flag_Remove -- Removes the house flag from the cell.                           *
 *   CellClass::Cell_Occupier -- Fetches the occupier for the cell.                            *
 *   CellClass::Get_Trigger -- retrieves reference to the cell's trigger                       *
 *   CellClass::Goodie_Check -- Performs crate discovery logic.                                *
 *   CellClass::Incoming -- Causes objects in cell to "run for cover".                         *
 *   CellClass::Is_Generally_Clear -- Determines if cell can be built upon.                    *
 *   CellClass::Occupy_Down -- Flag occupation of specified cell.                              *
 *   CellClass::Occupy_Unit -- Marks cell as unit occupied.                                    *
 *   CellClass::Occupy_Up -- Removes occupation flag from the specified cell.                  *
 *   CellClass::Overlap_Down -- This routine is used to mark a cell as being spilled over (overla*
 *   CellClass::Overlap_Unit -- Marks cell as being overlapped by unit.                        *
 *   CellClass::Overlap_Up -- Removes overlap flag for the cell.                               *
 *   CellClass::Read -- Reads a particular cell value from a save game file.                   *
 *   CellClass::Recalc_Attributes -- Recalculates the ground type attributes for the cell.     *
 *   CellClass::Redraw_Objects -- Redraws all objects overlapping this cell.                   *
 *   CellClass::Reduce_Tiberium -- Reduces the tiberium in the cell by the amount specified.   *
 *   CellClass::Reduce_Wall -- Damages a wall, if damage is high enough.                       *
 *   CellClass::Reserve_Cell -- Marks a cell as being occupied by the specified unit ID.       *
 *   CellClass::Shimmer -- Causes all objects in the cell to shimmer.                          *
 *   CellClass::Spot_Index -- returns cell sub-coord index for given COORD                     *
 *   CellClass::Tiberium_Adjust -- Adjust the look of the Tiberium for smooth.                 *
 *   CellClass::Validate -- validates cell's number                                            *
 *   CellClass::Wall_Update -- Updates the imagery for wall objects in cell.                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

/*
** New sidebar for GlyphX multiplayer. ST - 3/26/2019 12:24PM
*/
#include "SidebarGlyphx.h"

#define	FIXUP		0


/***********************************************************************************************
 * CellClass::Validate -- validates cell's number                                              *
 *                                                                                             *
 * INPUT:                                                                                      *
 *		none.																												  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *		1 = ok, 0 = error																								  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *		none.																												  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/09/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
#ifdef CHEAT_KEYS
int CellClass::Validate(void) const
{
	int num;

	num = Cell_Number();
	if (num < 0 || num > 4095) {
		Validate_Error("CELL");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
#endif


/***********************************************************************************************
 * CellClass::CellClass -- Constructor for cell objects.                                       *
 *                                                                                             *
 *    A cell object is constructed into an empty state. It contains no specific objects,       *
 *    templates, or overlays.                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
CellClass::CellClass(void)
{
	memset(this, 0, sizeof(CellClass));
	Overlay = OVERLAY_NONE;
	Smudge = SMUDGE_NONE;
	TType = TEMPLATE_NONE;
	Owner = HOUSE_NONE;
	InfType = HOUSE_NONE;
	OverrideLand = LAND_COUNT;
}


/***********************************************************************************************
 * CellClass::Cell_Color   -- Determine what radar color to use for this cell.                 *
 *                                                                                             *
 *    Use this routine to determine what radar color to render a radar                         *
 *    pixel with. This routine is called many many times to render the                         *
 *    radar map, so it must be fast.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the color to display the radar pixel with.                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/01/1994 JLB : Created.                                                                 *
 *   04/30/1994 JLB : Converted to member function.                                            *
 *   05/31/1994 JLB : Takes into account any stealth characteristics of object.                *
 *=============================================================================================*/
int CellClass::Cell_Color(bool override) const
{
	Validate();
	BuildingClass * object = Cell_Building();
	if (object) {
		return(object->House->Class->Color);
	}

	if (override) {
		return(TBLACK);
	}
	return(::Ground[Land_Type()].Color);
}


/***********************************************************************************************
 * CellClass::Cell_Techno -- Return with the unit/building at specified cell.                  *
 *                                                                                             *
 *    Returns an object located in the cell. If there is a                                     *
 *    building present, it returns a pointer to that, otherwise it returns                     *
 *    a pointer to one of the units there. If nothing is present in the                        *
 *    specified cell, then it returns NULL.                                                    *
 *                                                                                             *
 * INPUT:   x,y   -- Coordinate offset (from upper left corner) to use as an aid in selecting  *
 *                   the desired object within the cell.                                       *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to a building or unit located in cell. If                        *
 *          nothing present, just returns NULL.                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1992 JLB : Created.                                                                 *
 *   04/30/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
TechnoClass * CellClass::Cell_Techno(int x, int y) const
{
	Validate();
	ObjectClass * object;
	COORDINATE		click;			// Coordinate of click relative to cell corner.
	TechnoClass * close = NULL;
	long		distance = 0;	// Recorded closest distance.

	/*
	**	Create a coordinate value that represent the pixel location within the cell. This is
	**	actually the lower significant bits (leptons) of a regular coordinate value.
	*/
	click = XY_Coord(Pixel_To_Lepton(x), Pixel_To_Lepton(y));

	if (Cell_Occupier()) {
		object = Cell_Occupier();
		while (object) {
			if (object->Is_Techno()) {
				COORDINATE		coord;		// Coordinate relative to cell corner.
				long		dist;

				coord = object->Center_Coord() & 0x00FF00FFL;
				dist = Distance(coord, click);
				if (!close || dist < distance) {
					close = (TechnoClass *)object;
					distance = dist;
				}
			}
			object = object->Next;
		}
	}
	return(close);
}


/***************************************************************************
 * CellClass::Cell_Find_Object -- Returns ptr to RTTI type occupying cell  *
 *                                                                         *
 * INPUT:		RTTIType the RTTI type we are searching for                 *
 *                                                                         *
 * OUTPUT:		none                                                        *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   03/17/1995 PWG : Created.                                             *
 *   06/12/1995 JLB : Returns object class pointer.                        *
 *=========================================================================*/
ObjectClass * CellClass::Cell_Find_Object(RTTIType rtti) const
{
	Validate();
	ObjectClass * object = Cell_Occupier();

	while (object) {
		if (object->What_Am_I() == rtti) {
			return(object);
		}
		object = object->Next;
	}
	return(NULL);
}


/***********************************************************************************************
 * CellClass::Cell_Building -- Return with building at specified cell.                         *
 *                                                                                             *
 *    Given a cell, determine if there is a building associated                                *
 *    and return with a pointer to this building.                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building associated with the                         *
 *          cell. If there is no building associated, then NULL is                             *
 *          returned.                                                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1992 JLB : Created.                                                                 *
 *   04/30/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
BuildingClass * CellClass::Cell_Building(void) const
{
	Validate();
	return((BuildingClass *)Cell_Find_Object(RTTI_BUILDING));
}


/***********************************************************************************************
 * CellClass::Cell_Terrain -- Determines terrain object in cell.                               *
 *                                                                                             *
 *    This routine is used to determine the terrain object (if any) that                       *
 *    overlaps this cell.                                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the terrain object that overlaps                         *
 *          this cell. If there is no terrain object present, then NULL                        *
 *          is returned.                                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TerrainClass * CellClass::Cell_Terrain(void) const
{
	Validate();
	return((TerrainClass *)Cell_Find_Object(RTTI_TERRAIN));
}


/***********************************************************************************************
 * CellClass::Cell_Object -- Returns with clickable object in cell.                            *
 *                                                                                             *
 *    This routine is used to determine which object is to be selected                         *
 *    by a player click upon the cell. Not all objects that overlap the                        *
 *    cell are selectable by the player. This routine sorts out which                          *
 *    is which and returns with the appropriate object pointer.                                *
 *                                                                                             *
 * INPUT:   x,y   -- Coordinate (from upper left corner of cell) to use as a guide when        *
 *                   selecting the object within the cell. This plays a role in those cases    *
 *                   where several objects (such as infantry) exist within the same cell.      *
 *                                                                                             *
 * OUTPUT:  Returns with pointer to the object clickable within the                            *
 *          cell. NULL is returned if there is no clickable object                             *
 *          present.                                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/13/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * CellClass::Cell_Object(int x, int y) const
{
	Validate();
	ObjectClass * ptr;

	/*
	**	Hack so that aircraft landed on helipads can still be selected if directly
	**	clicked on.
	*/
	ptr = (ObjectClass *)Cell_Find_Object(RTTI_AIRCRAFT);
	if (ptr) {
		return(ptr);
	}

	ptr = Cell_Techno(x, y);
	if (ptr) {
		return(ptr);
	}
	ptr = Cell_Terrain();
	if (ptr) return(ptr);
	return(ptr);
}


/***********************************************************************************************
 * CellClass::Redraw_Objects -- Redraws all objects overlapping this cell.                     *
 *                                                                                             *
 *    This is a low level routine that marks all objects that overlap this                     *
 *    cell to be redrawn. It is necessary to call this routine whenever                        *
 *    the underlying icon has to be redrawn.                                                   *
 *                                                                                             *
 * INPUT:   forced   -- Should this redraw be forced even if flags                             *
 *                      indicate that it would be redundant?                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1994 JLB : Created.                                                                 *
 *   06/20/1994 JLB : Simplified to use object pointers.                                       *
 *   12/24/1994 JLB : Only checks if cell is in view and not flagged already.                  *
 *=============================================================================================*/
void CellClass::Redraw_Objects(bool forced)
{
	Validate();
	CELL	cell = Cell_Number();

	if (Map.In_View(cell) && (forced || !Map.Is_Cell_Flagged(cell))) {

		/*
		**	Flag the icon to be redrawn.
		*/
		Map.Flag_Cell(cell);

		/*
		**	Flag the main object in the cell to be redrawn.
		*/
		if (Cell_Occupier()) {
			ObjectClass * optr = Cell_Occupier();
			while (optr) {
				optr->Mark(MARK_CHANGE);
				optr = optr->Next;
			}
		}

		/*
		**	Flag any overlapping object in this cell to be redrawn.
		*/
		for (int index = 0; index < sizeof(Overlapper)/sizeof(Overlapper[0]); index++) {
			if (Overlapper[index]) {
				if (!Overlapper[index]->IsActive) {
					Overlapper[index] = 0;
				} else {
					Overlapper[index]->Mark(MARK_CHANGE);
				}
			}
		}
	}
}


/***********************************************************************************************
 * CellClass::Is_Generally_Clear -- Determines if cell can be built upon.                      *
 *                                                                                             *
 *    This determines if the cell can become a proper foundation for                           *
 *    building placement.                                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can the cell be built upon?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CellClass::Is_Generally_Clear(bool ignore_cloaked) const
{
	Validate();
	if (ScenarioInit) return(true);
	if (Flag.Composite || IsFlagged || Cell_Occupier()) {
		if (!ignore_cloaked || (Cell_Occupier() && Cell_Occupier()->Is_Techno() && ((TechnoClass *)Cell_Occupier())->Cloak != CLOAKED)) {
			return(false);
		}
	}
	if (Smudge != SMUDGE_NONE && SmudgeTypeClass::As_Reference(Smudge).IsBib && Owner != HOUSE_NONE) {
		return(false);
	}
	if (Overlay != OVERLAY_NONE && (Overlay == OVERLAY_FLAG_SPOT || OverlayTypeClass::As_Reference(Overlay).IsWall)) {
		return(false);
	}

#ifdef ADVANCED
	/*
	**	Building certain kinds of terrain is prohibited -- bridges in particular.
	*/
	switch (TType) {
		case TEMPLATE_BRIDGE1:
		case TEMPLATE_BRIDGE1D:
		case TEMPLATE_BRIDGE2:
		case TEMPLATE_BRIDGE2D:
		case TEMPLATE_BRIDGE3:
		case TEMPLATE_BRIDGE3D:
		case TEMPLATE_BRIDGE4:
		case TEMPLATE_BRIDGE4D:
		case TEMPLATE_FORD1:
		case TEMPLATE_FORD2:
			return(false);

		default:
			break;
	}
#endif

	return(::Ground[Land_Type()].Build);
}


/***********************************************************************************************
 * CellClass::Recalc_Attributes -- Recalculates the ground type attributes for the cell.       *
 *                                                                                             *
 *    This routine recalculates the ground type in the cell. The speeds the find path          *
 *    algorithm and other determinations of the cell type.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/29/1994 JLB : Created.                                                                 *
 *   06/20/1994 JLB : Knows about template pointer in cell object.                             *
 *=============================================================================================*/
void CellClass::Recalc_Attributes(void)
{
	Validate();
	/*
	**	Check for wall effects.
	*/
	if (Overlay != OVERLAY_NONE) {
		Land = OverlayTypeClass::As_Reference(Overlay).Land;
		if (Land != LAND_CLEAR) return;
	}

	/*
	**	If there is a template associated with this cell, then scan
	**	through the template exception list checking to see if this cell
	**	is one of the exception types. If it is, then return that ground type,
	**	otherwise return the template's default type.
	*/
	if (TType != TEMPLATE_NONE) {
		TemplateTypeClass const *ttype = &TemplateTypeClass::As_Reference(TType);

		/*
		**	If there is an exception type list for the icons of this template, then
		**	find out if the current icon is one of them. If so, apply the exception
		**	ground type to the cell.
		*/
		char const *ptr = ttype->AltIcons;
		if (ptr) {
			int icon = TIcon;

			while (*ptr != -1) {
				if (icon == *ptr++) {
					Land = ttype->AltLand;
					return;
				}
			}
		}

		/*
		**	No exception found, so just return the default ground type for this template.
		*/
		Land = ttype->Land;
		return;
	}

	/*
	**	No template is the same as clear terrain.
	*/
	Land = TemplateTypeClass::As_Reference(TEMPLATE_CLEAR1).Land;
}


/***********************************************************************************************
 * CellClass::Occupy_Down -- Flag occupation of specified cell.                                *
 *                                                                                             *
 *    This routine is used to mark the cell as being occupied by the specified object.         *
 *                                                                                             *
 * INPUT:   object   -- The object that is to occupy the cell                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *   11/29/1994 JLB : Simplified.                                                              *
 *=============================================================================================*/
void CellClass::Occupy_Down(ObjectClass * object)
{
	Validate();
	ObjectClass * optr;

	/*
	**	If the specified object is already part of the occupation list, then don't add
	**	it again -- bail instead.
	*/
	if (Cell_Occupier()) {
		optr = Cell_Occupier();
		while (optr) {
			if (optr == object) {
				return;
			}
			if (!optr->Next) break;
			optr = optr->Next;
		}
	}
	optr = Cell_Occupier();
	object->Next = optr;

	OccupierPtr = object;
	Map.Radar_Pixel(Cell_Number());

	/*
	**	If being placed down on a visible square, then flag this
	**	techno object as being revealed to the player.
	*/
	// Changes for GlyphX multiplayer. ST - 4/17/2019 3:02PM
	//if (IsVisible || GameToPlay != GAME_NORMAL) {
	//	object->Revealed(PlayerPtr);
	//}
	if (GameToPlay != GAME_GLYPHX_MULTIPLAYER) {
		if (IsVisible || GameToPlay != GAME_NORMAL) {
			object->Revealed(PlayerPtr);
		}
	} else {
		
		for (int i = 0; i < MPlayerCount; i++) {
			HousesType house_type = MPlayerHouses[i];
			if (Is_Visible(house_type)) {
				HouseClass *house = HouseClass::As_Pointer(house_type);
				object->Revealed(house);
			}
		}
	}	

	/*
	**	Special occupy bit set.
	*/
	switch (object->What_Am_I()) {
		case RTTI_BUILDING:
			Flag.Occupy.Building = true;
			break;

		case RTTI_AIRCRAFT:
		case RTTI_UNIT:
			Flag.Occupy.Vehicle = true;
			break;

		case RTTI_TERRAIN:
			Flag.Occupy.Monolith = true;
			break;

		default:
			break;
	}
}


/***********************************************************************************************
 * CellClass::Occupy_Up -- Removes occupation flag from the specified cell.                    *
 *                                                                                             *
 *    This routine will lift the object from the cell and free the cell to be occupied by      *
 *    another object. Only if the cell was previously marked with the object specified, will   *
 *    the object be lifted off. This routine is the counterpart to Occupy_Down().              *
 *                                                                                             *
 * INPUT:   object   -- The object that is being lifted off.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *   11/29/1994 JLB : Fixed to handle next pointer in previous object.                         *
 *=============================================================================================*/
void CellClass::Occupy_Up(ObjectClass * object)
{
	Validate();
	ObjectClass * optr = NULL;			// Working pointer to the objects in the chain.

	if (Cell_Occupier()) {
		optr = Cell_Occupier();
	}
	if (optr == object) {
		OccupierPtr = object->Next;
		object->Next = 0;
	} else {
		while (optr) {
			if (optr->Next == object) {
				optr->Next = object->Next;
				object->Next = 0;
				break;
			}
			if (!optr->Next) break;
			optr = optr->Next;
		}
	}
	Map.Radar_Pixel(Cell_Number());

	/*
	**	Special occupy bit clear.
	*/
	switch (object->What_Am_I()) {
		case RTTI_BUILDING:
			Flag.Occupy.Building = false;
			break;

		case RTTI_AIRCRAFT:
		case RTTI_UNIT:
			Flag.Occupy.Vehicle = false;
#ifdef NEVER
			int x,y;
			if (Map.Coord_To_Pixel(Cell_Coord(), x, y)) {
		 		SeenBuff.Put_Pixel(x, y, BLUE);
			}
#endif
			break;

		case RTTI_TERRAIN:
			Flag.Occupy.Monolith = false;
			break;

		default:
			break;
	}
}


/***********************************************************************************************
 * CellClass::Overlap_Down -- This routine is used to mark a cell as being spilled over (overla*
 *                                                                                             *
 *    Most game objects can often have their graphic imagery spill into more than one cell     *
 *    even though they are considered to "occupy" only one cell. All cells overlapped are      *
 *    flagged by this routine. Using this information it is possible to keep the tactical map  *
 *    display correct.                                                                         *
 *                                                                                             *
 * INPUT:   object   -- The object to mark as overlapping this cell.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *   07/04/1995 JLB : Ensures that buildings are always marked down.                           *
 *=============================================================================================*/
void CellClass::Overlap_Down(ObjectClass * object)
{
	Validate();
	ObjectClass **ptr = 0;

	if (!object) return;
	int index;
	for (index = 0; index < sizeof(Overlapper)/sizeof(Overlapper[0]); index++) {
		if (Overlapper[index] == object) return;
		if (!Overlapper[index]) ptr = &Overlapper[index];
	}

	/*
	**	Buildings must ALWAYS succeed in marking the cell as overlapped. Bump somebody
	**	else out in this case.
	*/
	if (!ptr && object->What_Am_I() == RTTI_BUILDING) {
		for (index = 0; index < sizeof(Overlapper)/sizeof(Overlapper[0]); index++) {
			switch (Overlapper[index]->What_Am_I()) {
				case RTTI_BUILDING:
				case RTTI_TERRAIN:
					break;

				default:
					Overlapper[index] = object;
					index = sizeof(Overlapper)/sizeof(Overlapper[0]);
					break;
			}
		}
	}
	if (ptr) *ptr = object;

	/*
	**	If being placed down on a visible square, then flag this
	**	techno object as being revealed to the player.
	*/
	// Changes for GlyphX multiplayer. ST - 4/18/2019 9:50AM
	//if (IsVisible) {
	//	object->Revealed(PlayerPtr);
	//}
	if (GameToPlay != GAME_GLYPHX_MULTIPLAYER) {
		if (IsVisible) {
			object->Revealed(PlayerPtr);
		}
	} else {
		
		if (object->Is_Techno()) {
			TechnoClass *tech = static_cast<TechnoClass*>(object);
			object->Revealed(tech->House);
		} else {
		
			for (int i = 0; i < MPlayerCount; i++) {
				HousesType house_type = MPlayerHouses[i];
				HouseClass *house = HouseClass::As_Pointer(house_type);
				object->Revealed(house);
			}
		}	
	}	
}


/***********************************************************************************************
 * CellClass::Overlap_Up -- Removes overlap flag for the cell.                                 *
 *                                                                                             *
 *    This is the counterpart to Overlap_Down and is used to remove the overlap flag for the   *
 *    specified unit on the cell.                                                              *
 *                                                                                             *
 * INPUT:   object   -- The object to remove the overlap flag for.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void CellClass::Overlap_Up(ObjectClass *object)
{
	Validate();
	for (int index = 0; index < sizeof(Overlapper)/sizeof(Overlapper[0]); index++) {
		if (Overlapper[index] == object) {
			Overlapper[index] = 0;
			break;
		}
	}

	//Map.Validate();
}


/***********************************************************************************************
 * CellClass::Cell_Unit -- Returns with pointer to unit occupying cell.                        *
 *                                                                                             *
 *    This routine will determine if a unit is occupying the cell and if so, return a pointer  *
 *    to it. If there is no unit occupying the cell, then NULL is returned.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with pointer to unit occupying cell, else NULL.                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
UnitClass * CellClass::Cell_Unit(void) const
{
	Validate();
	return((UnitClass*)Cell_Find_Object(RTTI_UNIT));
}


/***********************************************************************************************
 * CellClass::Cell_Infantry -- Returns with pointer of first infantry unit occupying the cell. *
 *                                                                                             *
 *    This routine examines the cell and returns a pointer to the first infantry unit          *
 *    that occupies it. If there is no infantry unit in the cell, then NULL is returned.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with pointer to infantry unit occupying the cell or NULL if none are       *
 *          present.                                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryClass * CellClass::Cell_Infantry(void) const
{
	Validate();
	return((InfantryClass*)Cell_Find_Object(RTTI_INFANTRY));
}



/***********************************************************************************************
 * CellClass::Get_Template_Info -- Get some info about a template for external use             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   Ref to info required                                                               *
 *                                                                                             *
 * OUTPUT:  True if image info available                                                       *
 *                                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   1/10/2019 5:57PM ST : Created.                                                            *
 *=============================================================================================*/
bool CellClass::Get_Template_Info(char *template_name, int &icon, void *&image_data)
{
	TemplateTypeClass const *ttype = NULL;
	
	if (TType != TEMPLATE_NONE) {
		ttype = &TemplateTypeClass::As_Reference(TType);
		icon = TIcon;
	} else {
		ttype = &TemplateTypeClass::As_Reference(TEMPLATE_CLEAR1);
		icon = Clear_Icon();
	}
			  
	if (ttype) {

		strcpy(template_name, ttype->IniName);
		image_data = (void*) ttype->ImageData;

		return true;
	}

	return false;
}




/***********************************************************************************************
 * CellClass::Draw_It -- Draws the cell imagery at the location specified.                     *
 *                                                                                             *
 *    This is the gruntwork cell rendering code. It draws the cell at the screen location      *
 *    specified. This routine doesn't draw any overlapping or occupying units. It only         *
 *    deals with the ground (cell) layer -- icon level.                                        *
 *                                                                                             *
 * INPUT:   x,y   -- The screen coordinates to render the cell imagery at.                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *   08/21/1994 JLB : Revised for simple template objects.                                     *
 *   11/01/1994 BRR : Updated placement cursor; draws actual object                            *
 *   11/14/1994 BRR : Added remapping code to show passable areas                              *
 *   12/02/1994 BRR : Added trigger display                                                    *
 *   12/11/1994 JLB : Mixes up clear terrain through pseudo-random table.                      *
 *   04/25/1995 JLB : Smudges drawn BELOW overlays.                                            *
 *=============================================================================================*/
void CellClass::Draw_It(int x, int y, int draw_type) const
{
	Validate();
	TemplateTypeClass const *ttype = 0;
	int	icon;		// The icon number to use from the template set.
	CELL	cell = Cell_Number();
	void * remap = NULL;
#ifdef SCENARIO_EDITOR
	TemplateTypeClass * tptr;
	TriggerClass * trig;
	int i;
	char waypt[2];
#endif

	/*
	**	Fetch a pointer to the template type associated with this cell.
	*/
	if (TType != TEMPLATE_NONE) {
		ttype = &TemplateTypeClass::As_Reference(TType);
		icon = TIcon;
	} else {
		ttype = &TemplateTypeClass::As_Reference(TEMPLATE_CLEAR1);
		icon = Clear_Icon();
	}


	/*
	**	Draw the stamp of the template.
	*/
	if (Debug_Icon) {
		LogicPage->Fill_Rect(Map.TacPixelX+x, Map.TacPixelY+y, Map.TacPixelX+x+ICON_PIXEL_W-1, Map.TacPixelY+y+ICON_PIXEL_H-1, Sim_Random_Pick(1, 254));
		FontXSpacing -= 2;
		Fancy_Text_Print("%d\r%2X%c\r%02X.%02X", Map.TacPixelX+x+(ICON_PIXEL_W>>1), Map.TacPixelY+y, WHITE, TBLACK, TPF_6POINT|TPF_NOSHADOW|TPF_CENTER, cell, Flag.Composite, (Cell_Occupier() ? '*' : ' '), Overlay, OverlayData);
		FontXSpacing += 2;
	} else {


		if (!draw_type || draw_type == CELL_BLIT_ONLY){


#ifdef SCENARIO_EDITOR
			/*
			**	Set up the remap table for this icon.
			*/
			if (Debug_Map && Debug_Passable) {
				if (::Ground[Land].Cost[0] == 0 || (Cell_Occupier() != NULL &&
					Cell_Occupier()->What_Am_I() != RTTI_INFANTRY)) {	// impassable
					remap = Map.FadingRed;
				} else {
					if (::Ground[Land].Cost[0] > 0x70) {	// pretty passable
						remap = Map.FadingGreen;
					} else {
						remap = Map.FadingYellow;				// moderately passable
					}
				}
			}
#endif

	// ****** maybe this icon shouldn't be drawn if it is known that the cell will be covered
	//			 with shadow.
			/*
			**	This is the underlying terrain icon.
			*/
			if (ttype->Get_Image_Data()) {
				LogicPage->Draw_Stamp(ttype->Get_Image_Data(), icon, x, y, NULL, WINDOW_TACTICAL);
				if (remap) {
					LogicPage->Remap(x+Map.TacPixelX, y+Map.TacPixelY, ICON_PIXEL_W, ICON_PIXEL_H, remap);
				}
			}


#ifdef SCENARIO_EDITOR
			/*
			**	Draw the map editor's "current" cell. This is the cell that can be
			**	assigned attributes such as tag labels.
			**	This must be draw before the placement cursor, but after drawing the
			**	objects in the cell.
			*/
			if (Debug_Map && CurrentCell == Cell_Number()) {
				LogicPage->Draw_Rect(x+Map.TacPixelX, y+Map.TacPixelY, Map.TacPixelX + x + CELL_PIXEL_W - 1, Map.TacPixelY + y + CELL_PIXEL_H - 1, YELLOW);
			}
#endif

#ifdef NEVER
			/*
			**	Special concrete render. It always renders over the underlying
			**	terrain unless this concrete piece will cover the entire terrain
			**	piece.
			*/
			if (Concrete) {
				LogicPage->Draw_Stamp(TemplateTypeClass::As_Pointer(TEMPLATE_CONCRETE_GDI)->Get_Image_Data(), Concrete-1, x, y, NULL, WINDOW_TACTICAL);
			}
#endif
		}

		/*
		**	Redraw any smudge.
		*/
		if (Smudge != SMUDGE_NONE) {
#ifdef NEVER
			switch (Smudge) {
				case SMUDGE_BIB1:
					CC_Draw_Shape(Bib1, SmudgeData, x, y, WINDOW_TACTICAL, SHAPE_WIN_REL);
					break;

				case SMUDGE_BIB2:
					CC_Draw_Shape(Bib2, SmudgeData, x, y, WINDOW_TACTICAL, SHAPE_WIN_REL);
					break;

				case SMUDGE_BIB3:
					CC_Draw_Shape(Bib3, SmudgeData, x, y, WINDOW_TACTICAL, SHAPE_WIN_REL);
					break;
			}
#endif
			SmudgeTypeClass::As_Reference(Smudge).Draw_It(x, y, SmudgeData);
		}


		if (!draw_type || draw_type == CELL_DRAW_ONLY){

			/*
			**	Draw the overlay object.
			*/
			if (Overlay != OVERLAY_NONE) {
				OverlayTypeClass const & otype = OverlayTypeClass::As_Reference(Overlay);
				IsTheaterShape = (bool)otype.IsTheater;
				CC_Draw_Shape(otype.Get_Image_Data(), OverlayData, (x+(CELL_PIXEL_W>>1)), (y+(CELL_PIXEL_H>>1)), WINDOW_TACTICAL, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_GHOST, NULL, Map.UnitShadow);
				IsTheaterShape = false;
			}

	#ifdef SCENARIO_EDITOR
			if (Debug_Map) {
				/*
				**	Draw the cell's Trigger mnemonic, if it has a trigger
				*/
				if (IsTrigger) {
					trig = Get_Trigger();
					Fancy_Text_Print(trig->Get_Name(), x+Map.TacPixelX, y+Map.TacPixelY, PINK, TBLACK, TPF_NOSHADOW|TPF_6POINT);
				}

				/*
				**	Draw the cell's Waypoint designation if there is one.
				*/
				if (IsWaypoint) {
					for (i = 0; i < 26; i++) {
						if (Waypoint[i]==Cell_Number()) {
							waypt[0] = 'A' + i;
							waypt[1] = 0;
							Fancy_Text_Print(waypt, Map.TacPixelX + x + CELL_PIXEL_W / 2,
								Map.TacPixelY + y + (CELL_PIXEL_H / 2) - 3, YELLOW, TBLACK,
								TPF_NOSHADOW | TPF_6POINT | TPF_CENTER);
							break;
						}
					}
					if (Waypoint[WAYPT_HOME] == Cell_Number()) {
							Fancy_Text_Print("Home", Map.TacPixelX + x, Map.TacPixelY + y + (CELL_PIXEL_H) - 7,
							WHITE, TBLACK, TPF_NOSHADOW | TPF_6POINT);
					}
					if (Waypoint[WAYPT_REINF] == Cell_Number()) {
							Fancy_Text_Print("Reinf", Map.TacPixelX + x, Map.TacPixelY + y + (CELL_PIXEL_H) - 7,
							WHITE, TBLACK, TPF_NOSHADOW | TPF_6POINT);
					}
				}
			}
	#endif

			/*
			**	Draw the placement cursor:
			**	- First, draw the hash-mark cursor, so it will appear underneath
			**	  any cursor being drawn
			**	- If the PendingObject is a template, overlay, or smudge, draw it
			**	- Otherwise, it's up to the Display.Refresh_Map() routine to draw it
			*/
			if (IsCursorHere) {

				/*
				**	Draw the hash-mark cursor:
				*/
				if (Map.ProximityCheck && Is_Generally_Clear()) {
					LogicPage->Draw_Stamp(Map.TransIconset, 0, x, y, NULL, WINDOW_TACTICAL);
				} else {
					LogicPage->Draw_Stamp(Map.TransIconset, 2, x, y, NULL, WINDOW_TACTICAL);
				}

	#ifdef SCENARIO_EDITOR
				if (Debug_Map && Map.PendingObject) {

					switch (Map.PendingObject->What_Am_I()) {

						/*
						**	Draw a template:
						**	- Compute the icon offset of this cell for this template, using
						**	  ZoneCell+ZoneOffset to get the upper-left corner of the placement
						**	  cursor
						**	- Draw the icon
						*/
						case RTTI_TEMPLATETYPE:
							tptr = (TemplateTypeClass *)Map.PendingObject;
							if (tptr->Get_Image_Data()) {
								icon = (Cell_X(cell) - Cell_X(Map.ZoneCell + Map.ZoneOffset)) +
									(Cell_Y(cell) - Cell_Y(Map.ZoneCell + Map.ZoneOffset)) *
									tptr->Width;
								LogicPage->Draw_Stamp(tptr->Get_Image_Data(), icon, x, y, NULL, WINDOW_TACTICAL);
							}
							break;

						/*
						**	Draw an overlay; just use the existing 'OverlayData' even though
						**	it means nothing.
						*/
						case RTTI_OVERLAYTYPE:
							OverlayTypeClass::As_Reference(((OverlayTypeClass *)Map.PendingObject)->Type).Draw_It(x, y, OverlayData);
							break;

						/*
						**	Draw a smudge
						*/
						case RTTI_SMUDGETYPE:
							SmudgeTypeClass::As_Reference(((SmudgeTypeClass *)Map.PendingObject)->Type).Draw_It(x, y, 0);
							break;
					}
				}
	#endif
			}

			/*
			**	Draw the flag if there is one located at this cell.
			*/
			if (IsFlagged) {
				void const * remap = HouseClass::As_Pointer(Owner)->Remap_Table(false, false);
				CC_Draw_Shape(MixFileClass::Retrieve("FLAGFLY.SHP"), Frame % 14, x+(ICON_PIXEL_W/2), y+(ICON_PIXEL_H/2), WINDOW_TACTICAL, SHAPE_CENTER|SHAPE_GHOST|SHAPE_FADING, remap, Map.UnitShadow);
			}
		}
	}
}


/***********************************************************************************************
 * CellClass::Concrete_Calc -- Calculates the concrete icon to use for the cell.               *
 *                                                                                             *
 *    This routine examines the cells around the current one and from this, determines what    *
 *    concrete icon shape to use (if any). The cell data is adjusted and the cell is marked    *
 *    for redraw if the icon changed.                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void CellClass::Concrete_Calc(void)
{
	Validate();
	static FacingType _even[5] = {FACING_N, FACING_S, FACING_SW, FACING_W, FACING_NW};
	static FacingType _odd[5] = {FACING_N, FACING_NE, FACING_E, FACING_SE, FACING_S};
	FacingType *ptr;		// Working pointer into adjacent cell list.
	int	index;	// Constructed bit index.
	int	icon;		// Icon number.
	bool	isodd;	// Is this for the odd column?

#define	OF_N	0x01
#define	OF_NE	0x02
#define	OF_E	0x04
#define	OF_SE	0x08
#define	OF_S	0x10

#define	EF_N	0x01
#define	EF_NW	0x10
#define	EF_W	0x08
#define	EF_SW	0x04
#define	EF_S	0x02

	/*
	**	Determine if the even or odd row logic is necessary.
	*/
	isodd = ((Cell_Number() & 0x01) != 0);

	/*
	**	Fetch correct pointer depending on whether this is for an
	**	odd or even row.
	*/
	ptr = (isodd) ? _odd : _even;

	/*
	**	Build an index according to the presence of concrete in the special
	**	adjacent cells. This is a short list of adjacent cell flags since
	**	only 5 adjacent cells need to be examined. The choice of which 5
	**	depends on whether this is for an even or odd column.
	*/
	index = 0;
	for (int i = 0; i < (sizeof(_even)/sizeof(_even[0])); i++) {
		CellClass & cellptr = Adjacent_Cell(*ptr++);

//		if ((cellptr->IsConcrete) ||
//					cellptr->Concrete == C_UPDOWN_RIGHT ||
//					cellptr->Concrete == C_UPDOWN_LEFT) {

		if (cellptr.Overlay == OVERLAY_CONCRETE) {
			index |= (1<<i);
		}
	}

	/*
	**	Special logic occurs for cells that are concrete filled.
	*/
	if (Overlay == OVERLAY_CONCRETE) {

		/*
		**	Process the index value and place the appropriate concrete icon
		**	in the cell.
		*/
		if (isodd) {
			switch (index) {
				case OF_NE:
				case OF_N|OF_NE:
				case OF_E|OF_N:
				case OF_E|OF_NE:
				case OF_N|OF_NE|OF_E:
				case OF_S|OF_N|OF_NE:
					icon = C_RIGHT_UP;		// right - up
					break;

				case OF_SE:
				case OF_E|OF_SE:
				case OF_S|OF_SE:
				case OF_S|OF_E:
				case OF_S|OF_SE|OF_E:
				case OF_S|OF_SE|OF_N:
					icon = C_RIGHT_DOWN;		// right - down
					break;

				case OF_SE|OF_NE:
				case OF_SE|OF_NE|OF_N:
				case OF_SE|OF_NE|OF_S:
				case OF_SE|OF_NE|OF_S|OF_N:
				case OF_SE|OF_E|OF_N:
				case OF_SE|OF_E|OF_NE|OF_N:
				case OF_S|OF_E|OF_N:
				case OF_S|OF_E|OF_NE:
				case OF_S|OF_E|OF_NE|OF_N:
				case OF_S|OF_SE|OF_E|OF_N:
				case OF_S|OF_SE|OF_E|OF_NE|OF_N:
				case OF_S|OF_SE|OF_E|OF_NE:
					icon = C_RIGHT_UPDOWN;		// right - up - down
					break;

				default:
					icon = C_RIGHT;		// right
					break;
			}
		} else {
			switch (index) {
				case EF_NW:
				case EF_NW|EF_N:
				case EF_W|EF_N:
				case EF_NW|EF_W|EF_N:
				case EF_NW|EF_W:
				case EF_NW|EF_S|EF_N:
					icon = C_LEFT_UP;		// left - up
					break;

				case EF_SW:
				case EF_SW|EF_S:
				case EF_W|EF_S:
				case EF_W|EF_SW|EF_S:
				case EF_W|EF_SW:
				case EF_SW|EF_S|EF_N:
					icon = C_LEFT_DOWN;		// left - down
					break;

				case EF_NW|EF_SW:
				case EF_NW|EF_SW|EF_N:
				case EF_NW|EF_SW|EF_S:
				case EF_NW|EF_SW|EF_S|EF_N:
				case EF_W|EF_S|EF_N:
				case EF_W|EF_SW|EF_N:
				case EF_W|EF_SW|EF_S|EF_N:
				case EF_NW|EF_W|EF_S:
				case EF_NW|EF_W|EF_S|EF_N:
				case EF_NW|EF_W|EF_SW|EF_S|EF_N:
				case EF_NW|EF_W|EF_SW|EF_N:
				case EF_NW|EF_W|EF_SW|EF_S:
					icon = C_LEFT_UPDOWN;		// left - up - down
					break;

				default:
					icon = C_LEFT;		// left
					break;
			}
		}

	} else {

		// Presume that no concrete piece is needed.
		icon = C_NONE;
		if (isodd) {
			index &= ~(OF_NE|OF_SE);		// Ignore diagonals.
			switch (index) {
				case OF_N|OF_E:
					icon = C_UP_RIGHT;		// up right
					break;

				case OF_E|OF_S:
					icon = C_DOWN_RIGHT;		// down right
					break;

				case OF_N|OF_E|OF_S:
					icon = C_UPDOWN_RIGHT;	// up/down right
					break;

				default:
					break;
			}
		} else {
			index &= ~(EF_NW|EF_SW);		// Ignore diagonals.
			switch (index) {
				case EF_N|EF_W:
					icon = C_UP_LEFT;		// up left
					break;

				case EF_W|EF_S:
					icon = C_DOWN_LEFT;		// down left
					break;

				case EF_N|EF_W|EF_S:
					icon = C_UPDOWN_LEFT;		// up/down left
					break;

				default:
					break;
			}
		}

		/*
		**	If any kind of fixup piece is needed, then add concrete
		**	to this location RECURSIVELY!
		*/
		if (icon != C_NONE) {
			OverlayTypeClass::As_Reference(OVERLAY_CONCRETE).Create_And_Place(Cell_Number());
			icon = C_NONE;
		}

	}

	/*
	**	Update the icon on the map.
	*/
	if (icon != C_NONE && OverlayData != icon) {
		OverlayData = icon;
		//Array[cell].Base = 0;
		Redraw_Objects();
	}
}


/***********************************************************************************************
 * CellClass::Wall_Update -- Updates the imagery for wall objects in cell.                     *
 *                                                                                             *
 *    This routine will examine the cell and the adjacent cells to determine what the wall     *
 *    should look like with the cell. It will then update the wall's imagery value and flag    *
 *    the cell to be redrawn if necessary. This routine should be called whenever the wall     *
 *    or an adjacent wall is created or destroyed.                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *   09/19/1994 BWG : Updated to handle partially-damaged walls.                               *
 *=============================================================================================*/
void CellClass::Wall_Update(void)
{
	Validate();
	static FacingType _offsets[5] = {FACING_N, FACING_E, FACING_S, FACING_W, FACING_NONE};

	if (Overlay == OVERLAY_NONE) {
		return;
	}

	OverlayTypeClass const & wall = OverlayTypeClass::As_Reference(Overlay);
	if (!wall.IsWall) {
		return;
	}

	for (unsigned index = 0; index < (sizeof(_offsets)/sizeof(_offsets[0])); index++) {
		CellClass & newcell = Adjacent_Cell(_offsets[index]);

		if (newcell.Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(newcell.Overlay).IsWall) {
			int	icon = 0;

			/*
			**	Build the icon number according to walls located in the adjacent
			**	cells.
			*/
			for (unsigned i = 0; i < 4; i++) {
				if (newcell.Adjacent_Cell(_offsets[i]).Overlay == newcell.Overlay) {
					icon |= 1 << i;
				}
			}
			newcell.OverlayData = (newcell.OverlayData & 0xFFF0) | icon;
//			newcell.OverlayData = icon;

			/*
			**	Handle special cases for the incomplete damaged wall sets. If a damage stage
			**	is calculated, but there is no artwork for it, then consider the wall to be
			**	completely destroyed.
			*/
			if (newcell.Overlay == OVERLAY_BRICK_WALL && newcell.OverlayData == 48) {
				newcell.Overlay = OVERLAY_NONE;
				newcell.OverlayData = 0;
				ObjectClass::Detach_This_From_All(::As_Target(newcell.Cell_Number()), true);
			}
			if (newcell.Overlay == OVERLAY_SANDBAG_WALL && newcell.OverlayData == 16) {
				newcell.Overlay = OVERLAY_NONE;
				newcell.OverlayData = 0;
				ObjectClass::Detach_This_From_All(::As_Target(newcell.Cell_Number()), true);
			}
			if (newcell.Overlay == OVERLAY_CYCLONE_WALL && newcell.OverlayData == 32) {
				newcell.Overlay = OVERLAY_NONE;
				newcell.OverlayData = 0;
				ObjectClass::Detach_This_From_All(::As_Target(newcell.Cell_Number()), true);
			}
			if (newcell.Overlay == OVERLAY_BARBWIRE_WALL && newcell.OverlayData == 16) {
				newcell.Overlay = OVERLAY_NONE;
				newcell.OverlayData = 0;
				ObjectClass::Detach_This_From_All(::As_Target(newcell.Cell_Number()), true);
			}

			newcell.Recalc_Attributes();
			newcell.Redraw_Objects();
		}
	}
}


/***********************************************************************************************
 * CellClass::Cell_Coord -- Returns the coordinate of this cell.                               *
 *                                                                                             *
 *    This support function will determine the coordinate of this cell and return it.          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with coordinate value of cell.                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE CellClass::Cell_Coord(void) const
{
	Validate();
	return(::Cell_Coord(Cell_Number()));
}


/***********************************************************************************************
 * CellClass::Reduce_Tiberium -- Reduces the tiberium in the cell by the amount specified.     *
 *                                                                                             *
 *    This routine will lower the tiberium level in the cell. It is used by the harvesting     *
 *    process as well as by combat damage to the tiberium fields.                              *
 *                                                                                             *
 * INPUT:   levels   -- The number of levels to reduce the tiberium.                           *
 *                                                                                             *
 * OUTPUT:  bool; Was the tiberium level reduced by at least one level?                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int CellClass::Reduce_Tiberium(int levels)
{
	Validate();
	int reducer = 0;

	if (levels && Land == LAND_TIBERIUM) {
		if (OverlayData > levels) {
			OverlayData -= levels;
			reducer = levels;
		} else {
			Overlay = OVERLAY_NONE;
			reducer = OverlayData;
			OverlayData = 0;
			Recalc_Attributes();
		}
	}
	return(reducer);
}


/***********************************************************************************************
 * CellClass::Reduce_Wall -- Damages a wall, if damage is high enough.                         *
 *                                                                                             *
 *    This routine will change the wall shape used for a wall if it's damaged.                 *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   damage   -- The number of damage points the wall was hit with.                     *
 *                                                                                             *
 * OUTPUT:  bool; Was the wall destroyed?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/15/1995 BWG : Created.                                                                 *
 *   03/19/1995 JLB : Updates cell information if wall was destroyed.                          *
 *=============================================================================================*/
int CellClass::Reduce_Wall(int damage)
{
	Validate();
	if (Overlay != OVERLAY_NONE) {
		bool destroyed = false;
		OverlayTypeClass const & wall = OverlayTypeClass::As_Reference(Overlay);

		if (wall.IsWall) {

			/*
			**	If the damage was great enough to ensure wall destruction, reduce the wall by one
			**	level (no more). Otherwise determine wall reduction based on a percentage chance
			**	proportional to the damage received and the wall's strength.
			*/
			if (damage >= wall.DamagePoints) {
				destroyed = true;
			} else {
				destroyed = Random_Pick(0, wall.DamagePoints) < damage;
			}

			if (destroyed) {
				OverlayData+=16;
				if ((OverlayData>>4) >= wall.DamageLevels) {
					ObjectClass::Detach_This_From_All(As_Target());
					Owner = HOUSE_NONE;
					Overlay = OVERLAY_NONE;
					OverlayData = 0;
					Recalc_Attributes();
					Redraw_Objects();
					Adjacent_Cell(FACING_N).Wall_Update();
					Adjacent_Cell(FACING_W).Wall_Update();
					Adjacent_Cell(FACING_S).Wall_Update();
					Adjacent_Cell(FACING_E).Wall_Update();
					return(true);
				}
			}
		}
	}
	return(false);
}


/***********************************************************************************************
 * CellClass::Get_Trigger -- retrieves reference to the cell's trigger                         *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      TriggerClass reference                                                                 *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      Never call this function unless the IsTrigger flag is set for the cell!                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/01/1994 BR : Created.                                                                  *
 *=============================================================================================*/
TriggerClass * CellClass::Get_Trigger(void) const
{
	Validate();
	if (IsTrigger) {
		return(CellTriggers[Cell_Number()]);
	}
	return(NULL);
}


/***********************************************************************************************
 * CellClass::Spot_Index -- returns cell sub-coord index for given COORD                       *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      coord      COORD to compute index for                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      index into StoppingCoord that's closest to this coord                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/1994 BR : Created.                                                                  *
 *   12/10/1994 JLB : Uses alternate sub-position algorithm.                                   *
 *=============================================================================================*/
int CellClass::Spot_Index(COORDINATE coord)
{
	COORDINATE rel = coord & 0x00FF00FFL;		// Sub coordinate value within cell.

	/*
	**	If the coordinate is close enough to the center of the cell, then return
	**	the center position index.
	*/
	if (Distance(rel, 0x00800080L) < 60) {
		return(0);
	}

	/*
	**	Since the center cell position has been eliminated, a simple comparison
	**	as related to the center of the cell can be used to determine the sub
	**	position. Take advantage of the fact that the sub positions are organized
	**	from left to right, top to bottom.
	*/
	int index = 0;
	if (Coord_X(rel) > 0x80) index |= 0x01;
	if (Coord_Y(rel) > 0x80) index |= 0x02;
	return(index+1);
}


/***********************************************************************************************
 * CellClass::Closest_Free_Spot -- returns free spot closest to given coord                    *
 *                                                                                             *
 * Similar to the CellClass::Free_Spot; this routine finds the spot in                         *
 * the cell closest to the given coordinate, and returns the COORD of                          *
 * that spot if it's available, NULL if it's not.                                              *
 *                                                                                             *
 * INPUT:                                                                                      *
 *  coord   coordinate to check (only sub cell position examined)                              *
 *                                                                                             *
 *          any   -- If only the closest spot is desired regardless of whether it is free or   *
 *                   not, then this parameter will be true.                                    *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *  COORD of free spot, NULL if none. The coordinate return value does not alter the cell      *
 *             coordinate data portions of the coordinate passed in. Only the lower sub-cell   *
 *             data is altered.                                                                *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *  none.                                                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/08/1994 BR : Created.                                                                  *
 *   12/10/1994 JLB : Picks best of closest stopping positions.                                *
 *   12/21/1994 JLB : Adds a mix-up factor if center location is occupied.                     *
 *=============================================================================================*/
COORDINATE CellClass::Closest_Free_Spot(COORDINATE coord, bool any) const
{
	Validate();
	int spot_index = Spot_Index(coord);

	/*
	**	This precalculated sequence table records the closest spots to any given spot. Sequential
	**	examination of these spots for availability ensures that the closes available one is
	**	discovered first.
	*/
	static unsigned char _sequence[5][4] = {
		{1,2,3,4},
		{0,2,3,4},
		{0,1,4,3},
		{0,1,4,2},
		{0,2,3,1}
	};

	/*
	**	In the case of the center coordinate being requested, but is occupied, then all other
	**	sublocations are equidistant. Instead of picking a static sequence of examination, the
	**	order is mixed up by way of this table.
	*/
	static unsigned char _alternate[4][4] = {
		{1,2,3,4},
		{2,3,4,1},
		{3,4,1,2},
		{4,1,2,3},
	};
	coord &= 0xFF00FF00L;

	/*
	**	Cells occupied by buildings or vehicles don't have any free spots.
	*/
	if (!any && (Flag.Occupy.Vehicle || Flag.Occupy.Monolith)) {
		return(NULL);
	}

	/*
	**	If just the nearest position is desired regardless of whether occupied or not,
	**	then just return with the stopping coordinate value.
	*/
	if (any || Is_Spot_Free(spot_index)) {
		return(Coord_Add(coord, StoppingCoordAbs[spot_index]));
	}

	/*
	**	Scan through all available sub-locations in the cell in order to determine
	**	the closest one to the coordinate requested. Use precalculated table so that
	**	when the first free position is found, bail.
	*/
	unsigned char *sequence;
	if (spot_index == 0) {
		sequence = &_alternate[Random_Pick(0,3)][0];
	} else {
		sequence = &_sequence[spot_index][0];
	}
	for (int index = 0; index < 4; index++) {
		int pos = *sequence++;

		if (Is_Spot_Free(pos)) {
			return(Coord_Add(coord, StoppingCoordAbs[pos]));
		}
	}

	/*
	**	No free spot could be found so return a NULL coordinate.
	*/
	return(0x00000000L);
}


/***********************************************************************************************
 * CellClass::Clear_Icon -- Calculates what the clear icon number should be.                   *
 *                                                                                             *
 *    This support routine will determine what the clear icon number would be for the cell.    *
 *    The icon number is determined by converting the cell number into an index into a         *
 *    lookup table. This yields what appears to be a randomized map without the necessity of   *
 *    generating and recording randomized map numbers.                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the icon number for clear terrain if it were displayed at the         *
 *          cell.                                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/26/1994 JLB : Created.                                                                 *
 *   06/09/1995 JLB : Uses 16 entry scramble algorithm.                                        *
 *=============================================================================================*/
int CellClass::Clear_Icon(void) const
{
	Validate();
	CELL cell = Cell_Number();
	return((cell & 0x03) | ((cell>>4) & 0x0C));
}


/***********************************************************************************************
 * CellClass::Incoming -- Causes objects in cell to "run for cover".                           *
 *                                                                                             *
 *    This routine is called whenever a great, but slow moving, threat is presented to the     *
 *    occupants of a cell. The occupants will, in most cases, stop what they are doing and     *
 *    try to get out of the way.                                                               *
 *                                                                                             *
 * INPUT:   threat      -- The coordinate source of the threat.                                *
 *                                                                                             *
 *          forced      -- If this threat is so major that the occupants should stop what      *
 *                         they are doing, then this parameter should be set to true.          *
 *                                                                                             *
 *          nokidding   -- Override the scatter to also affect human controlled objects.       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void CellClass::Incoming(COORDINATE threat, bool forced, bool nokidding)
{
	Validate();
	ObjectClass * object = NULL;

	object = Cell_Occupier();
	while (object) {

		/*
		**	Special check to make sure that friendly units never scatter.
		*/
		if (nokidding || Special.IsScatter || (object->Is_Techno() && !((TechnoClass *)object)->House->IsHuman)) {
			if (object->What_Am_I() == RTTI_INFANTRY) {
				object->Scatter(threat, forced, nokidding);
			} else {
				object->Scatter(threat, forced, nokidding);
//				object->Scatter(threat, false);
			}
		}
		object = object->Next;
	}
}


/***********************************************************************************************
 * CellClass::Adjacent_Cell -- Determines the adjacent cell according to facing.               *
 *                                                                                             *
 *    Use this routine to return a reference to the adjacent cell in the direction specified.  *
 *                                                                                             *
 * INPUT:   face  -- The direction to use when determining the adjacent cell.                  *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the adjacent cell.                                     *
 *                                                                                             *
 * WARNINGS:   If the facing value is invalid, then a reference to the same cell is returned.  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
CellClass const & CellClass::Adjacent_Cell(FacingType face) const
{
	Validate();
	if ((unsigned)face >= FACING_COUNT) {
		return(*this);
	}

	CELL id = Cell_Number();
	//The top row doesn't have any adjacent cells to the north. - LLL
	if (id < MAP_CELL_W && (face == FACING_N || face == FACING_NE || face == FACING_NW)) {
		return (*this);
	}

	//The bottom row doesn't have any adjacent cells to the south. - LLL
	if ((id > MAP_CELL_TOTAL - MAP_CELL_W) && (face == FACING_S || face == FACING_SE || face == FACING_SW)) {
		return (*this);
	}

	CellClass const * ptr = this + AdjacentCell[face];
	if (ptr->Cell_Number() & 0xF000) return(*this);
	return(*ptr);
//	return(*(this + AdjacentCell[face]));
}


/***************************************************************************
 * CellClass::Adjust_Threat -- Allows adjustment of threat at cell level   *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/24/1995 PWG : Created.                                             *
 *=========================================================================*/
void CellClass::Adjust_Threat(HousesType house, int threat_value)
{
	Validate();
	int region = Map.Cell_Region(Cell_Number());

	for (HousesType lp = HOUSE_FIRST; lp < HOUSE_COUNT; lp ++) {
		if (lp == house) continue;

		HouseClass *house_ptr = HouseClass::As_Pointer(lp);
		if (house_ptr && (!house_ptr->IsHuman || !house_ptr->Is_Ally(house))) {
			house_ptr->Adjust_Threat(region, threat_value);
		}
	}
	if (Debug_Threat) {
		Map.Flag_To_Redraw(true);
	}
}


/***********************************************************************************************
 * CellClass::Tiberium_Adjust -- Adjust the look of the Tiberium for smoothing purposes.       *
 *                                                                                             *
 *    This routine will adjust the level of the Tiberium in the cell so that it will           *
 *    smoothly blend with the adjacent Tiberium. This routine should only be called for        *
 *    new Tiberium cells. Existing cells that contain Tiberium follow a different growth       *
 *    pattern.                                                                                 *
 *                                                                                             *
 * INPUT:   pregame  -- Is this a pregame call? Such a call will mixup the Tiberium overlay    *
 *                      used.                                                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the added Tiberium value that is now available for harvesting.        *
 *                                                                                             *
 * WARNINGS:   The return value is only valid for the initial placement. Tiberium growth will  *
 *             increase the net worth of the existing Tiberium.                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
long CellClass::Tiberium_Adjust(bool pregame)
{
	Validate();
	if (Overlay != OVERLAY_NONE) {
		if (OverlayTypeClass::As_Reference(Overlay).Land == LAND_TIBERIUM) {
			static int _adj[9] = {0,1,3,4,6,7,8,10,11};
			int	count = 0;

			/*
			**	Mixup the Tiberium overlays so that they don't look the same.
			*/
			if (pregame) {
				Overlay = Random_Pick(OVERLAY_TIBERIUM1, OVERLAY_TIBERIUM12);
			}

			/*
			**	Add up all adjacent cells that contain tiberium.
			** (Skip those cells which aren't on the map)
			*/
			for (FacingType face = FACING_FIRST; face < FACING_COUNT; face++) {
				CELL cell = Cell_Number() + AdjacentCell[face];
				if ((unsigned)cell >= MAP_CELL_TOTAL) continue;

				CellClass & adj = Adjacent_Cell(face);

				if (adj.Overlay != OVERLAY_NONE &&
					OverlayTypeClass::As_Reference(adj.Overlay).Land == LAND_TIBERIUM) {
					count++;
				}
			}

			OverlayData = _adj[count];
			return((OverlayData+1) * UnitTypeClass::TIBERIUM_STEP);
		}
	}
	return(0);
}


/***********************************************************************************************
 * CellClass::Goodie_Check -- Performs crate discovery logic.                                  *
 *                                                                                             *
 *    Call this routine whenever an object enters a cell. It will check for the existance      *
 *    of a crate and generate any "goodie" it might contain.                                   *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that is triggering this crate.                   *
 *                                                                                             *
 * OUTPUT:  Can the object continue to enter this cell? A false return value means that the    *
 *          cell is now occupied and must not be entered.                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1995 JLB : Created.                                                                 *
 *   07/08/1995 JLB : Added a bunch of goodies to the crates.                                  *
 *=============================================================================================*/
bool CellClass::Goodie_Check(FootClass * object)
{
	Validate();
	enum {
		MONEY,			// Cash award.
		UNIT,				// A free unit.
		NUKE,				// A nuclear device that explodes.
		ION,				// Calls forth an ion blast on discoverer.
		NUKE_MISSILE,	// Gets a one time nuclear missile options.
		ION_BLAST,		// Gets a one time ion blast option.
		AIR_STRIKE,		// Gets a one time air strike option.
		HEAL_BASE,		// Heals the player's entire base.
		CLOAK,			// Units in region gain cloak ability.
		EXPLOSION,		// Conventional explosion.
		NAPALM,			// A napalm explosion.
		SQUAD,			// A mixed squad of friendly infantry appear.
		VISCEROID,		// A visceroid appears!
		DARKNESS,		// Shroud the entire map.
		REVEAL,			// Reveal the entire map.
		TOTAL_CRATES,
	};
	static int _what[] = {
		DARKNESS,DARKNESS,
		REVEAL,REVEAL,
		NUKE,
//		ION,ION,
		NUKE_MISSILE,
		ION_BLAST,ION_BLAST,
		AIR_STRIKE,AIR_STRIKE,AIR_STRIKE,AIR_STRIKE,
		HEAL_BASE,HEAL_BASE,
		CLOAK,CLOAK,
		EXPLOSION,EXPLOSION,EXPLOSION,EXPLOSION,
		NAPALM,NAPALM,NAPALM,
		SQUAD,SQUAD,SQUAD,SQUAD,SQUAD,
		UNIT,UNIT,UNIT,UNIT,UNIT,
		VISCEROID
	};


	/*
	** Crate types are only defined here so it needs to match my new global crate total ST - 6/4/96 2:16PM
	*/
#if (TOTAL_CRATES) == (TOTAL_CRATE_TYPES)

#else
	//Huge_Errrrror..... Oh NO!

#endif


	if (object && Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(Overlay).IsCrate) {
		bool steel = (Overlay == OVERLAY_STEEL_CRATE);
		COORDINATE coord;			// Temporary working coordinate value.

		/*
		**	A triggered crate is automatically destroyed regardless of who or how
		**	it was triggered.
		*/
		Redraw_Objects();
		Overlay = OVERLAY_NONE;
		OverlayData = 0;

		if (steel) {
			if (object->Owner() == HOUSE_BAD) {
				object->House->Add_Nuke_Piece();
				new AnimClass(ANIM_CRATE_EMPULSE, Cell_Coord());
			}

		} else {

			int index;
			UnitClass * unit = 0;
			unsigned damage = 0;
			int what = MONEY;

			if (GameToPlay != GAME_NORMAL && (Random_Pick(1, 2) == 1 || !object->House->BScan)) {
				what = -1;

				/*
				**	If the player has a construction yeard and practically no money and no legitmate means
				**	to make any more money, then give money to build a refinery.
				*/
				if ((object->House->BScan & (STRUCTF_CONST|STRUCTF_REFINERY)) == STRUCTF_CONST &&
					object->House->Available_Money() < BuildingTypeClass::As_Reference(STRUCT_REFINERY).Cost) {

					what = MONEY;
				}

				/*
				**	If the player should get an MCV replacement, then give it now (probably).
				*/
				if (Random_Pick(0, 1) == 0 &&
					MPlayerBases &&
					!(object->House->UScan & UNITF_MCV) &&
					object->House->BScan == 0 &&
					object->House->Available_Money() > ((BuildingTypeClass::As_Reference(STRUCT_REFINERY).Cost + BuildingTypeClass::As_Reference(STRUCT_POWER).Cost) * object->House->CostBias)) {

					what = UNIT;
				}

				bool allow_super = true;
				if (GameToPlay == GAME_GLYPHX_MULTIPLAYER && !Rule.AllowSuperWeapons) {
					allow_super = false;
				}

				while (what == -1) {
					what = _what[Random_Pick((unsigned)0, sizeof(_what)/sizeof(_what[0])-1)];

					if (what == REVEAL && object->House->IsVisionary) what = -1;
					if (what == AIR_STRIKE && (!allow_super || object->House->AirStrike.Is_Present())) what = -1;
					if (what == NUKE_MISSILE && (!allow_super || object->House->NukeStrike.Is_Present())) what = -1;
					if (what == ION_BLAST && (!allow_super || object->House->IonCannon.Is_Present())) what = -1;
					if (what == CLOAK && object->IsCloakable) what = -1;
				}
			}

			/*
			** Keep track of the number of each type of crate found
			*/
			if (GameToPlay == GAME_INTERNET){
				object->House->TotalCrates->Increment_Unit_Total(what);
			}

			/*
			**	Update the crate count and when all the crates have been discovered, flag
			**	to generate a new one.
			*/
			CrateCount--;
			if (!CrateMaker && CrateCount <= 0 && GameToPlay != GAME_NORMAL) {
				CrateMaker = true;
				CrateTimer = 1;
			}

			/*
			**	Create the effect requested.
			*/
			switch (what) {
				default:

				/*
				**	Give the player money.
				*/
				case MONEY:
					new AnimClass(ANIM_CRATE_DOLLAR, Cell_Coord());
					if (GameToPlay == GAME_NORMAL) {
						HouseClass::As_Pointer(object->Owner())->Refund_Money(2000);
					} else {
						HouseClass::As_Pointer(object->Owner())->Refund_Money(100 + (Random_Pick(0,19)*100));
					}
					break;

				/*
				**	Shroud the world in blackness.
				*/
				case DARKNESS:
				{	
					new AnimClass(ANIM_CRATE_EMPULSE, Cell_Coord());
					bool shroud = false;
					if (GameToPlay != GAME_GLYPHX_MULTIPLAYER) {
						if (object->House == PlayerPtr) {
							shroud = true;
						}
					} else {
						if (object->House->IsHuman) {
							shroud = true;
						}
					}	
					if (shroud) {
						for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
							CellClass * cellptr = &Map[cell];
							if (cellptr->Is_Mapped(object->House) || cellptr->Is_Visible(object->House)) {
								cellptr->Redraw_Objects();
								cellptr->Set_Mapped(object->House, false);
								cellptr->Set_Visible(object->House, false);
							}
						}
						HouseClass *find_house = object->House;
						for (int index = 0; index < Map.Layer[LAYER_GROUND].Count(); index++) {
							ObjectClass * object = Map.Layer[LAYER_GROUND][index];
							if (object && object->Is_Techno() && ((TechnoClass *)object)->House == find_house) {
								object->Look();
							}
						}
						Map.Flag_To_Redraw(true);
					}
					break;
				}
				/*
				**	Reveal the entire map.
				*/
				case REVEAL:
				{	
					bool reveal = false;
					if (GameToPlay != GAME_GLYPHX_MULTIPLAYER) {
						if (object->House == PlayerPtr) {
							reveal = true;
						}
					} else {
						if (object->House->IsHuman) {
							reveal = true;
						}
					}	
					
					new AnimClass(ANIM_CRATE_EMPULSE, Cell_Coord());
					object->House->IsVisionary = true;
					if (reveal) {
						for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
							Map.Map_Cell(cell, object->House, true);
						}
					}
					break;
				}
				/*
				**	Try to create a unit where the crate was.
				*/
				case UNIT: {
					UnitTypeClass const * utp = NULL;

					/*
					**	Give the player an MCV if he has no base left but does have more than enough
					**	money to rebuild a new base. Of course, if he already has an MCV, then don't
					**	give him another one.
					*/
					if (MPlayerBases &&
						!(object->House->UScan & UNITF_MCV) &&
						object->House->BScan == 0 &&
						object->House->Available_Money() > (BuildingTypeClass::As_Reference(STRUCT_REFINERY).Cost + BuildingTypeClass::As_Reference(STRUCT_POWER).Cost)) {

						utp = &UnitTypeClass::As_Reference(UNIT_MCV);
					}

					/*
					**	If the player has a base and a refinery, but no harvester, then give him
					**	a free one.
					*/
					if (!utp && (object->House->BScan & STRUCTF_REFINERY) && !(object->House->UScan & UNITF_HARVESTER)) {
						utp = &UnitTypeClass::As_Reference(UNIT_HARVESTER);
					}

					while (!utp) {
						UnitType utype = Random_Pick(UNIT_FIRST, (UnitType)(UNIT_COUNT-1));
						if (utype != UNIT_MCV || MPlayerBases) {
							utp = &UnitTypeClass::As_Reference(utype);
							if (utp->IsCrateGoodie && (utp->Ownable & (1 << object->Owner())) && utp->Level <= BuildLevel+2) break;
							utp = NULL;
						}
					}

					UnitClass * unit = (UnitClass *)utp->Create_One_Of(object->House);
					if (unit) {
						if (unit->Unlimbo(Cell_Coord())) {
							return(false);
						}
						delete unit;
					}
				}
				break;

				/*
				**	Create a squad of miscellanous composition.
				*/
				case SQUAD:
					for (index = 0; index < 5; index++) {
						static InfantryType _inf[] = {
							INFANTRY_E1,INFANTRY_E1,INFANTRY_E1,INFANTRY_E1,INFANTRY_E1,INFANTRY_E1,
							INFANTRY_E2,
							INFANTRY_E3,
							INFANTRY_E4,
							INFANTRY_E5,
							INFANTRY_E7,
							INFANTRY_RAMBO
						};
						InfantryTypeClass::As_Reference(_inf[Random_Pick((unsigned)0, sizeof(_inf)/sizeof(_inf[0])-1)]).Create_And_Place(Cell_Number(), object->Owner());
					}
					return(false);

				/*
				**	Sometimes an explosion of great magnitude occurs.
				*/
				case NUKE:
					new AnimClass(ANIM_ATOM_BLAST, Cell_Coord());
					break;

				/*
				**	Sometimes an explosion of great magnitude occurs.
				*/
				case ION:
					new AnimClass(ANIM_ION_CANNON, Cell_Coord());
					break;

				/*
				**	A nuclear missile was discovered. Add it to the sidebar.
				*/
				case NUKE_MISSILE:
					new AnimClass(ANIM_CRATE_MISSILE, Cell_Coord());
					if (object->House->NukeStrike.Enable(true)) {
						// Add to Glyphx multiplayer sidebar. ST - 3/22/2019 1:50PM
						if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
							if (object->House->IsHuman) {
								Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_NUCLEAR_BOMB, object->House);
							}
						} else {
							if (object->IsOwnedByPlayer) {
								Map.Add(RTTI_SPECIAL, SPC_NUCLEAR_BOMB);
								Map.Column[1].Flag_To_Redraw();
							}
						}
					}
					break;

				/*
				**	A one time ion blast was discovered. Add it to the sidebar.
				*/
				case ION_BLAST:
					new AnimClass(ANIM_CRATE_EARTH, Cell_Coord());
					if (object->House->IonCannon.Enable(true)) {
						// Add to Glyphx multiplayer sidebar. ST - 3/22/2019 1:50PM
						if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
							if (object->House->IsHuman) {
								Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_ION_CANNON, object->House);
							}
						} else {
							if (object->IsOwnedByPlayer) {
								Map.Add(RTTI_SPECIAL, SPC_ION_CANNON);
								Map.Column[1].Flag_To_Redraw();
							}
						}
					}
					break;

				/*
				**	A one time air strike can be called int. Add it to the sidebar.
				*/
				case AIR_STRIKE:
					new AnimClass(ANIM_CRATE_DEVIATOR, Cell_Coord());
					if (object->House->AirStrike.Enable(true)) {
						// Add to Glyphx multiplayer sidebar. ST - 3/22/2019 1:50PM
						if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
							if (object->House->IsHuman) {
								Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_AIR_STRIKE, object->House);
							}
						} else {
							if (object->IsOwnedByPlayer) {
								Map.Add(RTTI_SPECIAL, SPC_AIR_STRIKE);
								Map.Column[1].Flag_To_Redraw();
							}
						}
					}
					break;

				/*
				**	A group of explosions are triggered around the crate.
				*/
				case EXPLOSION:
					damage = 400;
					object->Take_Damage((int&)damage, 0, WARHEAD_HE);
					for (index = 0; index < 5; index++) {
						COORDINATE coord = Coord_Scatter(Cell_Coord(), Random_Pick(0, 0x0200));
						new AnimClass(ANIM_FBALL1, coord);
						damage = 400;
						Explosion_Damage(coord, damage, NULL, WARHEAD_HE);
					}
					break;

				/*
				**	A napalm blast is triggered.
				*/
				case NAPALM:
					coord = Coord_Mid(Cell_Coord(), object->Center_Coord());
					new AnimClass(ANIM_NAPALM3, coord);
					damage = 600;
					Explosion_Damage(coord, damage, NULL, WARHEAD_FIRE);
					break;

				/*
				**	A visceroid appears and, boy, he's angry!
				*/
				case VISCEROID:
					unit = new UnitClass(UNIT_VICE, HOUSE_JP);
					if (unit) {
						if (unit->Unlimbo(Cell_Coord())) {
							return(false);
						}
						delete unit;
					}
					break;

				/*
				**	All objects within a certain range will gain the ability to cloak.
				*/
				case CLOAK:
					new AnimClass(ANIM_CRATE_STEALTH, Cell_Coord());
					for (index = 0; index < Map.Layer[LAYER_GROUND].Count(); index++) {
						ObjectClass * obj = Map.Layer[LAYER_GROUND][index];

						if (obj && obj->Is_Techno() && Distance(Cell_Coord(), obj->Center_Coord()) < 0x0300) {
							((TechnoClass *)obj)->IsCloakable = true;
						}
					}
					break;

				/*
				**	All of the player's objects heal up.
				*/
				case HEAL_BASE:
					new AnimClass(ANIM_CRATE_INVUN, Cell_Coord());
					for (index = 0; index < Logic.Count(); index++) {
						ObjectClass * obj = Logic[index];

						if (obj && object->Is_Techno() && object->House->Class->House == obj->Owner()) {
							obj->Strength = obj->Class_Of().MaxStrength;
						}
					}
					break;

			}
		}
	}
	return(true);
}


/***********************************************************************************************
 * CellClass::Flag_Place -- Places a house flag down on the cell.                              *
 *                                                                                             *
 *    This routine will place the house flag at this cell location.                            *
 *                                                                                             *
 * INPUT:   house -- The house that is having its flag placed here.                            *
 *                                                                                             *
 * OUTPUT:  Was the flag successfully placed here?                                             *
 *                                                                                             *
 * WARNINGS:   Failure to place means that the cell is impassable for some reason.             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CellClass::Flag_Place(HousesType house)
{
	Validate();
	if (!IsFlagged && Is_Generally_Clear()) {
		IsFlagged = true;
		Owner = house;
		Redraw_Objects();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * CellClass::Flag_Remove -- Removes the house flag from the cell.                             *
 *                                                                                             *
 *    This routine will free the cell of any house flag that may be located there.             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was there a flag here that was removed?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CellClass::Flag_Remove(void)
{
	Validate();
	if (IsFlagged) {
		IsFlagged = false;
		Owner = HOUSE_NONE;
		Redraw_Objects();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * CellClass::Shimmer -- Causes all objects in the cell to shimmer.                            *
 *                                                                                             *
 *    This routine is called when some event would cause a momentary disruption in the         *
 *    cloaking device. All objects that are cloaked in the cell will have their cloaking       *
 *    device shimmer.                                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void CellClass::Shimmer(void)
{
	Validate();
	ObjectClass * object = Cell_Occupier();

	while (object) {
		object->Do_Shimmer();
		object = object->Next;
	}
}


/***********************************************************************************************
 * CellClass::Cell_Occupier -- Fetches the occupier for the cell.                              *
 *                                                                                             *
 *    This routine returns with the first recorded occupier of this cell. A special validity   *
 *    check is performed to ensure that there are no dead objects still marked on the          *
 *    map.                                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the first occupier object.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/17/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * CellClass::Cell_Occupier(void) const
{
	ObjectClass * ptr = OccupierPtr;

	while (ptr && !ptr->IsActive) {
		ptr = ptr->Next;
		((ObjectClass *&)OccupierPtr) = 0;
	}

	return(ptr);
}









/*
**  Additions to CellClass to track visibility per-player. ST - 3/5/2019 3:08PM
** 
** 
** 
** 
** 
*/

/***********************************************************************************************
 * CellClass::Set_Mapped -- Set the cell mapped for the given player                           *
 *                                                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   3/5/2019 3:09PM - ST                                                                      *
 *=============================================================================================*/
void CellClass::Set_Mapped(HousesType house, bool set)
{
	int shift = (int) house;
	if (set) {
		IsMappedByPlayerMask |= (1 << shift);
	} else {
		IsMappedByPlayerMask &= ~(1 << shift);
	}	
}			  
	

/***********************************************************************************************
 * CellClass::Set_Mapped -- Set the cell mapped for the given player                           *
 *                                                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   3/5/2019 3:09PM - ST                                                                      *
 *=============================================================================================*/
void CellClass::Set_Mapped(HouseClass *player, bool set)
{
	if (player && player->Class) {
		Set_Mapped(player->Class->House, set);
		if (GameToPlay == GAME_NORMAL && player->IsHuman) {
			IsMapped = set;			// Also set the regular flag in single player
		}
	}
}			  

/***********************************************************************************************
 * CellClass::Is_Mapped -- Is the cell mapped for the given player                             *
 *                                                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   3/5/2019 3:13PM - ST                                                                      *
 *=============================================================================================*/
bool CellClass::Is_Mapped(HousesType house) const
{
	int shift = (int) house;
	return (IsMappedByPlayerMask & (1 << shift)) ? true : false;
}			  

/***********************************************************************************************
 * CellClass::Is_Mapped -- Is the cell mapped for the given player                             *
 *                                                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   3/5/2019 3:13PM - ST                                                                      *
 *=============================================================================================*/
bool CellClass::Is_Mapped(HouseClass *player) const
{
	if (player && player->Class) {
		return Is_Mapped(player->Class->House);
	}
	return false;
}			  

/***********************************************************************************************
 * CellClass::Set_Visible -- Set the cell visible for the given player                         *
 *                                                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   3/5/2019 3:16PM - ST                                                                      *
 *=============================================================================================*/
void CellClass::Set_Visible(HousesType house, bool set)
{
	int shift = (int) house;
	if (set) {
		IsVisibleByPlayerMask |= (1 << shift);
	} else {
		IsVisibleByPlayerMask &= ~(1 << shift);
	}	
}			  


/***********************************************************************************************
 * CellClass::Set_Visible -- Set the cell visible for the given player                         *
 *                                                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   3/5/2019 3:16PM - ST                                                                      *
 *=============================================================================================*/
void CellClass::Set_Visible(HouseClass *player, bool set)
{
	if (player && player->Class) {
		Set_Visible(player->Class->House, set);
		if (GameToPlay == GAME_NORMAL && player->IsHuman) {
			IsVisible = set;			// Also set the regular flag in single player. This is needed for rendering
		}
	}
}			  

/***********************************************************************************************
 * CellClass::Is_Visible -- Is the cell visible for the given player                           *
 *                                                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   3/5/2019 3:16PM - ST                                                                      *
 *=============================================================================================*/
bool CellClass::Is_Visible(HousesType house) const
{
	int shift = (int) house;
	return (IsVisibleByPlayerMask & (1 << shift)) ? true : false;
}			  

/***********************************************************************************************
 * CellClass::Is_Visible -- Is the cell visible for the given player                           *
 *                                                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   3/5/2019 3:16PM - ST                                                                      *
 *=============================================================================================*/
bool CellClass::Is_Visible(HouseClass *player) const
{
	if (player && player->Class) {
		return Is_Visible(player->Class->House);
	}
	return false;
}			  

void CellClass::Override_Land_Type(LandType type)
{
	OverrideLand = type;
}

#ifdef USE_RA_AI

/*
** Addition from Red Alert for AI. ST - 7/24/2019 5:33PM
*/
/***********************************************************************************************
 * CellClass::Is_Clear_To_Move -- Determines if the cell is generally clear for travel         *
 *                                                                                             *
 *    This routine is called when determining general passability for purposes of zone         *
 *    calculation. Only blockages that cannot be circumvented are considered to make a cell    *
 *    impassable. All other obstructions can either be destroyed or are temporary.             *
 *                                                                                             *
 * INPUT:   loco     -- The locomotion type to use when determining passablility.              *
 *                                                                                             *
 *          ignoreinfantry -- Should infantry in the cell be ignored for movement purposes?    *
 *                                                                                             *
 *          ignorevehicles -- If vehicles should be ignored, then this flag will be true.      *
 *                                                                                             *
 *          zone     -- If specified, the zone must match this value or else movement is       *
 *                      presumed disallowed.                                                   *
 *                                                                                             *
 *          check    -- This specifies the zone type that this check applies to.               *
 *                                                                                             *
 * OUTPUT:  Is the cell generally passable to ground targeting?                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/25/1995 JLB : Created.                                                                 *
 *   06/25/1996 JLB : Uses tracked vehicles as a basis for zone check.                         *
 *   10/05/1996 JLB : Allows checking for crushable blockages.                                 *
 *=============================================================================================*/
bool CellClass::Is_Clear_To_Move(bool ignoreinfantry, bool ignorevehicles) const
{
	//assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	/*
	**	Flying objects always consider every cell passable since they can fly over everything.
	*/
	//if (loco == SPEED_WINGED) {
	//	return(true);
	//}

	/*
	**	If a zone was specified, then see if the cell is in a legal
	**	zone to allow movement.
	*/
	//if (zone != -1) {
	//	if (zone != Zones[check]) {
	//		return(false);
	//	}
	//}

	/*
	**	Check the occupy bits for passable legality. If ignore infantry is true, then
	**	don't consider infnatry.
	*/
	int composite = Flag.Composite;
	if (ignoreinfantry) {
		composite &= 0xE0;			// Drop the infantry occupation bits.
	}
	if (ignorevehicles) {
		composite &= 0x5F;			// Drop the vehicle/building bit.
	}
	if (composite != 0) {
		return(false);
	}

	/*
	**	Fetch the land type of the cell -- to be modified and used later.
	*/
	//LandType land = Land_Type();

	/*
	**	Walls are always considered to block the terrain for general passability
	**	purposes unless this is a wall crushing check or if the checking object
	**	can destroy walls.
	*/
	OverlayTypeClass const * overlay = NULL;
	if (Overlay != OVERLAY_NONE) {
	 	overlay = &OverlayTypeClass::As_Reference(Overlay);
	}
	if (overlay != NULL && overlay->IsWall) {
		//if (check != MZONE_DESTROYER && (check != MZONE_CRUSHER || !overlay->IsCrushable)) {
			return(false);
		//}

		/*
		**	Crushing objects consider crushable walls as clear rather than the
		**	typical LAND_WALL setting.
		*/
		//land = LAND_CLEAR;
	}

	/*
	**	See if the ground type is impassable to this locomotion type and if
	**	so, return the error condition.
	*/
	//if (::Ground[land].Cost[loco] == 0) {
	//	return(false);
	//}

	/*
	**	All checks passed, so this cell must be passable.
	*/
	return(true);
}

#endif //USE_RA_AI