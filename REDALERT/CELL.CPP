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

/* $Header: /CounterStrike/CELL.CPP 4     3/14/97 1:15p Joe_b $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
 *                  Last Update : October 6, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CellClass::Adjacent_Cell -- Determines the adjacent cell according to facing.             *
 *   CellClass::Adjust_Threat -- Allows adjustment of threat at cell level                     *
 *   CellClass::Can_Tiberium_Germinate -- Determines if Tiberium can begin growth in the cell. *
 *   CellClass::Can_Tiberium_Grow -- Determines if Tiberium can grow in this cell.             *
 *   CellClass::Can_Tiberium_Spread -- Determines if Tiberium can spread from this cell.       *
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
 *   CellClass::Cell_Vessel -- Returns with pointer to a vessel located in the cell.           *
 *   CellClass::Clear_Icon -- Calculates what the clear icon number should be.                 *
 *   CellClass::Closest_Free_Spot -- returns free spot closest to given coord                  *
 *   CellClass::Concrete_Calc -- Calculates the concrete icon to use for the cell.             *
 *   CellClass::Draw_It -- Draws the cell imagery at the location specified.                   *
 *   CellClass::Flag_Place -- Places a house flag down on the cell.                            *
 *   CellClass::Flag_Remove -- Removes the house flag from the cell.                           *
 *   CellClass::Goodie_Check -- Performs crate discovery logic.                                *
 *   CellClass::Grow_Tiberium -- Grows the tiberium in the cell.                               *
 *   CellClass::Incoming -- Causes objects in cell to "run for cover".                         *
 *   CellClass::Is_Bridge_Here -- Checks to see if this is a bridge occupied cell.             *
 *   CellClass::Is_Clear_To_Build -- Determines if cell can be built upon.                     *
 *   CellClass::Is_Clear_To_Move -- Determines if the cell is generally clear for travel       *
 *   CellClass::Occupy_Down -- Flag occupation of specified cell.                              *
 *   CellClass::Occupy_Up -- Removes occupation flag from the specified cell.                  *
 *   CellClass::Overlap_Down -- This routine is used to mark a cell as being spilled over (over*
 *   CellClass::Overlap_Unit -- Marks cell as being overlapped by unit.                        *
 *   CellClass::Overlap_Up -- Removes overlap flag for the cell.                               *
 *   CellClass::Read -- Reads a particular cell value from a save game file.                   *
 *   CellClass::Recalc_Attributes -- Recalculates the ground type attributes for the cell.     *
 *   CellClass::Redraw_Objects -- Redraws all objects overlapping this cell.                   *
 *   CellClass::Reduce_Tiberium -- Reduces the tiberium in the cell by the amount specified.   *
 *   CellClass::Reduce_Wall -- Damages a wall, if damage is high enough.                       *
 *   CellClass::Reserve_Cell -- Marks a cell as being occupied by the specified unit ID.       *
 *   CellClass::Shimmer -- Causes all objects in the cell to shimmer.                          *
 *   CellClass::Spot_Index -- returns cell sub-coord index for given COORDINATE                *
 *   CellClass::Spread_Tiberium -- Spread Tiberium from this cell to an adjacent cell.         *
 *   CellClass::Tiberium_Adjust -- Adjust the look of the Tiberium for smooth.                 *
 *   CellClass::Wall_Update -- Updates the imagery for wall objects in cell.                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"vortex.h"

/*
** New sidebar for GlyphX multiplayer. ST - 8/2/2019 2:50PM
*/
#include "SidebarGlyphx.h"


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
 *   02/20/1996 JLB : Uses initializer list.                                                   *
 *=============================================================================================*/
CellClass::CellClass(void) :
	ID(Map.ID(this)),
	IsPlot(false),
	IsCursorHere(false),
	IsMapped(false),
	IsVisible(false),
	IsWaypoint(false),
	IsRadarCursor(false),
	IsFlagged(false),
	IsToShroud(false),
	Jammed(0),
	Trigger(NULL),
	TType(TEMPLATE_NONE),
	TIcon(0),
	Overlay(OVERLAY_NONE),
	OverlayData(0),
	Smudge(SMUDGE_NONE),
	SmudgeData(0),
	Owner(HOUSE_NONE),
	InfType(HOUSE_NONE),
	OccupierPtr(0),
	Land(LAND_CLEAR),
	OverrideLand(LAND_NONE),
	IsMappedByPlayerMask(0),
	IsVisibleByPlayerMask(0)
{
	for (int zone = MZONE_FIRST; zone < MZONE_COUNT; zone++) {
		Zones[zone] = 0;
	}
	Flag.Composite = 0;
	for (int index = 0; index < ARRAY_SIZE(Overlapper); index++) {
		Overlapper[index] = 0;
	}
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	BuildingClass * object = Cell_Building();
	if (object && !object->Class->IsInvisible) {
		return(ColorRemaps[object->House->RemapColor].Bar);
	}

	if (override) {
		return(TBLACK);
	}
	if (LastTheater == THEATER_SNOW) {
		return(::SnowColor[Land_Type()]);
	} else {
		return(::GroundColor[Land_Type()]);
	}
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

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
				COORDINATE coord = Coord_Fraction(object->Center_Coord());
				long dist = Distance(coord, click);
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);
	assert(rtti != RTTI_NONE);

	ObjectClass * object = Cell_Occupier();

	while (object != NULL) {
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	CELL	cell = Cell_Number();

	if (Map.In_View(cell) && (forced || !Map.Is_Cell_Flagged(cell))) {

		/*
		**	Flag the icon to be redrawn.
		*/
		Map.Flag_Cell(cell);

		/*
		**	Flag the main object in the cell to be redrawn.
		*/
		if (Cell_Occupier() != NULL) {
			ObjectClass * optr = Cell_Occupier();
			while (optr != NULL && optr->IsActive) {

#ifdef SORTDRAW
				if (optr->Is_Techno() && ((TechnoClass *)optr)->Visual_Character() != VISUAL_NORMAL) {
					optr->Mark(MARK_CHANGE);
				}
#else
				optr->Mark(MARK_CHANGE);
#endif
				if (optr->Next != NULL && !optr->Next->IsActive) {
					optr->Next = NULL;
				}
				optr = optr->Next;
			}
		}

#ifdef SORTDRAW
		/*
		**	Flag any overlapping object in this cell to be redrawn.
		*/
		for (int index = 0; index < ARRAY_SIZE(Overlapper); index++) {
			if (Overlapper[index]) {
				assert(Overlapper[index]->IsActive);
				if (Overlapper[index]->Is_Techno() && ((TechnoClass *)Overlapper[index])->Visual_Character() != VISUAL_NORMAL) {
					Overlapper[index]->Mark(MARK_CHANGE);
				}
			}
		}
#else
		/*
		**	Flag any overlapping object in this cell to be redrawn.
		*/
		for (int index = 0; index < ARRAY_SIZE(Overlapper); index++) {
			if (Overlapper[index] != NULL) {
				if (!Overlapper[index]->IsActive) {
					Overlapper[index] = NULL;
				} else {
					Overlapper[index]->Mark(MARK_CHANGE);
				}
			}
		}
#endif
	}
}


/***********************************************************************************************
 * CellClass::Is_Clear_To_Build -- Determines if cell can be built upon.                       *
 *                                                                                             *
 *    This determines if the cell can become a proper foundation for                           *
 *    building placement.                                                                      *
 *                                                                                             *
 * INPUT:   loco     -- The locomotion of the object trying to consider if this cell is        *
 *                      generally clear. Buildings use the value of SPEED_NONE.                *
 *                                                                                             *
 * OUTPUT:  bool; Is this cell generally clear (usually for building purposes)?                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1994 JLB : Created.                                                                 *
 *   06/25/1996 JLB : Handles different locomotion types.                                      *
 *   10/05/1996 JLB : Checks for crushable walls and crushable object.                         *
 *=============================================================================================*/
bool CellClass::Is_Clear_To_Build(SpeedType loco) const
{
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	/*
	**	During scenario initialization, passability is always guaranteed.
	*/
	if (ScenarioInit) return(true);

	/*
	**	If there is an object there, then don't allow building.
	*/
	if (Cell_Object() != NULL) {
		return(false);
	}

	/*
	**	Prevents a building from being placed over a flag object.
	*/
#ifdef FIXIT_FLAG_CHECK
	if (IsFlagged)  {
		return(false);
	}
#endif

	/*
	**	Walls are always considered to block the terrain for general passability
	**	purposes. In normal game mode, all overlays are not buildable.
	*/
	if (Overlay != OVERLAY_NONE && (Overlay == OVERLAY_FLAG_SPOT || !Debug_Map || OverlayTypeClass::As_Reference(Overlay).IsWall)) {
		return(false);
	}

	/*
	**	Building over a bib is not allowed.
	*/
	if (Smudge != SMUDGE_NONE && SmudgeTypeClass::As_Reference(Smudge).IsBib /* && Owner != HOUSE_NONE*/) {
		return(false);
	}

	/*
	**	Building on certain kinds of terrain is prohibited -- bridges in particular.
	**	If the locomotion type is SPEED_NONE, then this check is presumed to be
	**	for the purposes of building.
	*/
	if (loco == SPEED_NONE) {
		if (Is_Bridge_Here()) {
			return(false);
		}

		return(::Ground[Land_Type()].Build);

	} else {

		if (::Ground[Land_Type()].Cost[loco] == fixed(0)) {
//		if (::Ground[Land_Type()].Cost[SPEED_TRACK] == fixed(0)) {
			return(false);
		}
		return(true);
	}
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	/*
	**	Special override for interior terrain set so that a non-template or a clear template
	**	is equivalent to impassable rock.
	*/
	if (LastTheater == THEATER_INTERIOR) {
		if (TType == TEMPLATE_NONE || TType == TEMPLATE_CLEAR1) {
			Land = LAND_ROCK;
			return;
		}
	}

	/*
	**	Check for wall effects.
	*/
	if (Overlay != OVERLAY_NONE) {
		Land = OverlayTypeClass::As_Reference(Overlay).Land;
		if (Land != LAND_CLEAR) return;
	}

	/*
	**	If there is a template associated with this cell, then fetch the
	**	land type given the template type and icon number.
	*/
	if (TType != TEMPLATE_NONE && TType != 255) {
		TemplateTypeClass const * ttype = &TemplateTypeClass::As_Reference(TType);
		Land = ttype->Land_Type(TIcon);
		return;
	}

	/*
	**	No template is the same as clear terrain.
	*/
	Land = LAND_CLEAR;
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);
	assert(object != NULL && object->IsActive);

	ObjectClass * optr;

	if (object == NULL) return;

	/*
	**	Always add buildings to the end of the occupation chain. This is necessary because
	**	the occupation chain is a single list even though buildings occupy more than one
	**	cell. If more than one building is allowed to occupy the same cell, then this chain
	**	logic will fail.
	*/
	if (object->What_Am_I() == RTTI_BUILDING && Cell_Occupier()) {
		optr = Cell_Occupier();
		while (optr->Next != NULL) {
			assert(optr != object);
			assert(optr->What_Am_I() != RTTI_BUILDING);
			optr = optr->Next;
		}
		optr->Next = object;
		object->Next = 0;
	} else {
		object->Next = Cell_Occupier();
		OccupierPtr = object;
	}
	Map.Radar_Pixel(Cell_Number());

	/*
	**	If being placed down on a visible square, then flag this
	**	techno object as being revealed to the player.
	*/
	// Changes for client/server multiplayer. ST - 8/2/2019 2:51PM
	//if (IsMapped || Session.Type != GAME_NORMAL) {
	//	object->Revealed(PlayerPtr);
	//}
	if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {
		if (IsMapped || Session.Type != GAME_NORMAL) {
			object->Revealed(PlayerPtr);
		}
	} else {
		
		for (int i = 0; i < Session.Players.Count(); i++) {
			HousesType house_type = Session.Players[i]->Player.ID;
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

		case RTTI_VESSEL:
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);
	assert(object != NULL && object->IsActive);

	if (object == NULL) return;

	ObjectClass * optr = Cell_Occupier();		// Working pointer to the objects in the chain.

	if (optr == object) {
		OccupierPtr = object->Next;
		object->Next = 0;
	} else {
		bool found = false;
		while (optr != NULL) {
			if (optr->Next == object) {
				optr->Next = object->Next;
				object->Next = 0;
				found = true;
				break;
			}
			optr = optr->Next;
		}
//		assert(found);
	}
	Map.Radar_Pixel(Cell_Number());

	/*
	**	Special occupy bit clear.
	*/
	switch (object->What_Am_I()) {
		case RTTI_BUILDING:
			Flag.Occupy.Building = false;
			break;

		case RTTI_VESSEL:
		case RTTI_AIRCRAFT:
		case RTTI_UNIT:
			Flag.Occupy.Vehicle = false;
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);
	assert(object != NULL && object->IsActive);

	ObjectClass ** ptr = 0;

	if (!object) return;

	int index;
	for (index = 0; index < ARRAY_SIZE(Overlapper); index++) {
		if (Overlapper[index] == object) return;
		if (!Overlapper[index]) ptr = &Overlapper[index];
	}

	/*
	**	Buildings must ALWAYS succeed in marking the cell as overlapped. Bump somebody
	**	else out in this case.
	*/
	if (!ptr && object->What_Am_I() == RTTI_BUILDING) {
		for (index = 0; index < ARRAY_SIZE(Overlapper); index++) {
			switch (Overlapper[index]->What_Am_I()) {
				case RTTI_BUILDING:
				case RTTI_TERRAIN:
					break;

				default:
					Overlapper[index] = object;
					index = ARRAY_SIZE(Overlapper);
					break;
			}
		}
	}
	if (ptr) *ptr = object;

	/*
	**	If being placed down on a visible square, then flag this
	**	techno object as being revealed to the player.
	*/
	if (IsMapped) {
		object->Revealed(PlayerPtr);
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
void CellClass::Overlap_Up(ObjectClass * object)
{
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);
	assert(object != NULL && object->IsActive);

	for (int index = 0; index < ARRAY_SIZE(Overlapper); index++) {
		if (Overlapper[index] == object) {
			Overlapper[index] = 0;
			break;
		}
	}
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	return((UnitClass*)Cell_Find_Object(RTTI_UNIT));
}


/***********************************************************************************************
 * CellClass::Cell_Vessel -- Returns with pointer to a vessel located in the cell.             *
 *                                                                                             *
 *    Call this routine to query and return a pointer to a vessel located in the cell. If      *
 *    there is no vessel present, then this routine will return NULL.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the vessel class object if one is present.               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
VesselClass * CellClass::Cell_Vessel(void) const
{
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	return((VesselClass*)Cell_Find_Object(RTTI_VESSEL));
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	return((InfantryClass*)Cell_Find_Object(RTTI_INFANTRY));
}


#ifdef SORTDRAW
static bool _Calc_Partial_Window(int cellx, int celly, int & drawx, int & drawy)
{
	int & px = WindowList[WINDOW_PARTIAL][WINDOWX];
	int & py = WindowList[WINDOW_PARTIAL][WINDOWY];
	int & pw = WindowList[WINDOW_PARTIAL][WINDOWWIDTH];
	int & ph = WindowList[WINDOW_PARTIAL][WINDOWHEIGHT];
	int & tx = WindowList[WINDOW_TACTICAL][WINDOWX];
	int & ty = WindowList[WINDOW_TACTICAL][WINDOWY];
	int & tw = WindowList[WINDOW_TACTICAL][WINDOWWIDTH];
	int & th = WindowList[WINDOW_TACTICAL][WINDOWHEIGHT];

	px = cellx + tx;
	py = celly + ty;
	pw = CELL_PIXEL_W;
	ph = CELL_PIXEL_H;

	if (px < tx) {
		pw -= tx - px;
		px = tx;
	}
	if (pw < 1) return(false);

	if (py < ty) {
		ph -= ty - py;
		py = ty;
	}
	if (ph < 1) return(false);

	if (px + pw > tx + tw) {
		pw -= (px + pw) - (tx + tw);
	}
	if (pw < 1) return(false);

	if (py + ph > ty + th) {
		ph -= (py + ph) - (ty + th);
	}
	if (ph < 1) return(false);

	drawx = drawx - (px-tx);
	drawy = drawy - (py-ty);
	return(true);
}


static int _ocompare(const void * left, const void * right)
{
	COORDINATE lcoord = (*((ObjectClass **)left))->Sort_Y();
	COORDINATE rcoord = (*((ObjectClass **)right))->Sort_Y();
	if (lcoord < rcoord) return(-1);
	if (lcoord > rcoord) return(1);
	return(0);
}
#endif


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

	if (TType != TEMPLATE_NONE && TType != TEMPLATE_CLEAR1 && TType != 255) {		// Not sure why it's checking for 255 here since that's a valid tile type. ST - 6/4/2019
		ttype = &TemplateTypeClass::As_Reference(TType);
		icon = TIcon;
	}
	else {
		ttype = &TemplateTypeClass::As_Reference(TEMPLATE_CLEAR1);
		icon = Clear_Icon();
	}

	if (ttype) {

		strcpy(template_name, ttype->IniName);
		image_data = (void*)ttype->ImageData;

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
 *   07/22/1996 JLB : Objects added to draw process.                                           *
 *=============================================================================================*/
void CellClass::Draw_It(int x, int y, bool objects) const
{
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	if (!objects) {
		BStart(BENCH_CELL);

		TemplateTypeClass const * ttype = 0;
		int	icon;		// The icon number to use from the template set.
		CELL	cell = Cell_Number();
		void * remap = NULL;
	#ifdef SCENARIO_EDITOR
		TemplateTypeClass * tptr;
//		TriggerClass * trig;
		int i;
		char waypt[3];
	#endif

		CellCount++;

		/*
		**	Fetch a pointer to the template type associated with this cell.
		*/
		if (TType != TEMPLATE_NONE && TType != TEMPLATE_CLEAR1 && TType != 255) {
			ttype = &TemplateTypeClass::As_Reference(TType);
			icon = TIcon;
		} else {
			ttype = &TemplateTypeClass::As_Reference(TEMPLATE_CLEAR1);
			icon = Clear_Icon();
		}

	#ifdef CHEAT_KEYS
		/*
		**	Draw the stamp of the template.
		*/
		if (Debug_Icon) {
			LogicPage->Fill_Rect(Map.TacPixelX+x, Map.TacPixelY+y, Map.TacPixelX+x+ICON_PIXEL_W-1, Map.TacPixelY+y+ICON_PIXEL_H-1, Sim_Random_Pick(1, 254));
			FontXSpacing -= 2;
			Fancy_Text_Print("%02X%02X\r%d%d%d\r%d %d", Map.TacPixelX+x+(ICON_PIXEL_W>>1), Map.TacPixelY+y, &GreyScheme, TBLACK, TPF_EFNT|TPF_CENTER|TPF_BRIGHT_COLOR|TPF_FULLSHADOW,
				Cell_Y(cell), Cell_X(cell),
				//(CurrentObject.Count() && CurrentObject[0]->Is_Techno()) ? ((TechnoClass *)CurrentObject[0])->House->Which_Zone(cell) : -1,
				Zones[MZONE_NORMAL],Zones[MZONE_CRUSHER],Zones[MZONE_DESTROYER],
				Overlay, OverlayData
				);
			FontXSpacing += 2;
		} else {
	#endif

	#ifdef SCENARIO_EDITOR
			/*
			**	Set up the remap table for this icon.
			*/
			if (Debug_Map && Debug_Passable) {
				if (::Ground[Land].Cost[0] == 0 || (Cell_Occupier() != NULL &&
					Cell_Occupier()->What_Am_I() != RTTI_INFANTRY)) {	// impassable
					remap = DisplayClass::FadingRed;
				} else {
					if (::Ground[Land].Cost[0] > fixed(1, 3)) {	// pretty passable
						remap = DisplayClass::FadingGreen;
					} else {
						remap = DisplayClass::FadingYellow;				// moderately passable
					}
				}
			}
	#endif

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

			/*
			**	Redraw any smudge.
			*/
			if (Smudge != SMUDGE_NONE) {
				SmudgeTypeClass::As_Reference(Smudge).Draw_It(x, y, SmudgeData);
			}

			/*
			**	Draw the overlay object.
			*/
			if (Overlay != OVERLAY_NONE) {
				OverlayTypeClass const & otype = OverlayTypeClass::As_Reference(Overlay);
				IsTheaterShape = (bool)otype.IsTheater;	//Tell Build_Frame if this overlay is theater specific
				CC_Draw_Shape(otype.Get_Image_Data(), OverlayData, (x+(CELL_PIXEL_W>>1)), (y+(CELL_PIXEL_H>>1)), WINDOW_TACTICAL, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_GHOST, NULL, DisplayClass::UnitShadow);
				IsTheaterShape = false;
			}

	#ifdef SCENARIO_EDITOR
			if (Debug_Map) {
				/*
				**	Draw the cell's Trigger mnemonic, if it has a trigger
				*/
				if (Trigger.Is_Valid()) {
					Fancy_Text_Print(Trigger->Class->IniName, x+Map.TacPixelX, y+Map.TacPixelY, &ColorRemaps[PCOLOR_RED], TBLACK, TPF_EFNT|TPF_FULLSHADOW);
				}

				/*
				**	Draw the cell's Waypoint designation if there is one.
				*/
				if (IsWaypoint) {
					for (i = 0; i < WAYPT_HOME; i++) {
						if (Scen.Waypoint[i] == Cell_Number()) {
							if (i < 26) {
								waypt[0] = 'A' + i;
								waypt[1] = 0;
							} else {
								waypt[0] = 'A' + (i/26)-1;
								waypt[1] = 'A' + (i % 26);
								waypt[2] = 0;
							}
							Fancy_Text_Print(waypt, Map.TacPixelX + x + CELL_PIXEL_W / 2,
								Map.TacPixelY + y + (CELL_PIXEL_H / 2) - 3,
								&ColorRemaps[PCOLOR_RED], TBLACK,
								TPF_EFNT | TPF_CENTER|TPF_FULLSHADOW);
							break;
						}
					}
					if (Scen.Waypoint[WAYPT_HOME] == Cell_Number()) {
							Fancy_Text_Print("Home", Map.TacPixelX + x, Map.TacPixelY + y + (CELL_PIXEL_H) - 7,
							&ColorRemaps[PCOLOR_GREY], TBLACK, TPF_EFNT|TPF_FULLSHADOW);
					}
					if (Scen.Waypoint[WAYPT_REINF] == Cell_Number()) {
							Fancy_Text_Print("Reinf", Map.TacPixelX + x, Map.TacPixelY + y + (CELL_PIXEL_H) - 7,
							&ColorRemaps[PCOLOR_GREY], TBLACK, TPF_EFNT|TPF_FULLSHADOW);
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
				SpeedType loco = SPEED_NONE;
				if (Map.PendingObjectPtr) {
					if (Map.PendingObjectPtr->What_Am_I() == RTTI_BUILDING) {
						BuildingClass * obj = (BuildingClass *)(Map.PendingObjectPtr);
						loco = obj->Class->Speed;
	//					if (*obj == STRUCT_SUB_PEN || *obj == STRUCT_SHIP_YARD ||
	//					    *obj == STRUCT_FAKE_PEN || *obj == STRUCT_FAKE_YARD) loco = SPEED_FLOAT;
					}
				}

				/*
				**	Draw the hash-mark cursor:
				*/
				if (Map.ProximityCheck && Is_Clear_To_Build(loco)) {
					LogicPage->Draw_Stamp(DisplayClass::TransIconset, 0, x, y, NULL, WINDOW_TACTICAL);
				} else {
					LogicPage->Draw_Stamp(DisplayClass::TransIconset, 2, x, y, NULL, WINDOW_TACTICAL);
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

						default:
							break;
					}
				}
	#endif
			}

			/*
			**	Draw the flag if there is one located at this cell.
			*/
			if (IsFlagged) {
				void const * flag_remap = HouseClass::As_Pointer(Owner)->Remap_Table(false, REMAP_NORMAL);
				CC_Draw_Shape(MFCD::Retrieve("FLAGFLY.SHP"), Frame % 14, x+(ICON_PIXEL_W/2), y+(ICON_PIXEL_H/2), WINDOW_TACTICAL, SHAPE_CENTER|SHAPE_GHOST|SHAPE_FADING, flag_remap, DisplayClass::UnitShadow);
			}

	#ifdef CHEAT_KEYS
		}
	#endif
		BEnd(BENCH_CELL);
	}

#ifdef SORTDRAW
	if (objects) {
		BStart(BENCH_OBJECTS);

		/*
		**	Build a list of objects to draw into a working buffer. There is a
		**	big presumption here -- it is presumed that if the cell is to be
		**	redrawn, then all objects in the cell should properly be flagged to
		**	be redrawn as well. Normally, this isn't a problem, but for subs
		**	the IsToDisplay flag MUST REMAIN SET. This is because there is a
		**	hack overpass after the cells are redrawn so that subs can be
		**	redrawn separately.
		*/
		ObjectClass * optr[20 + ARRAY_SIZE(Overlapper)];
		int count = 0;
		ObjectClass * object = Cell_Occupier();
		while (object != NULL) {
			if (!object->IsActive) break;
			optr[count] = object;
			object->IsToDisplay = true;
			object = object->Next;
			count++;
		}
		for (int index = 0; index < ARRAY_SIZE(Overlapper); index++) {
			object = Overlapper[index];
			if (object != NULL && object->IsActive) {
				object->IsToDisplay = true;
				optr[count] = object;
				count++;
			}
		}

		/*
		**	Sort the object list so that objects will be drawn from
		**	back to front.
		*/
		switch (count) {

			/*
			**	If there are zero or one object, then sorting is
			**	unnecessary.
			*/
			case 0:
			case 1:
				break;

			/*
			**	Two objects can be sorted with a single compare and swap.
			*/
			case 2:
				if (optr[0]->Sort_Y() > optr[1]->Sort_Y()) {
					swap(optr[0], optr[1]);
				}
				break;

			/*
			**	Three objects can be sorted with three compares and swaps.
			*/
			case 3:
				if (optr[0]->Sort_Y() > optr[2]->Sort_Y()) {
					swap(optr[0], optr[2]);
				}
				if (optr[0]->Sort_Y() > optr[1]->Sort_Y()) {
					swap(optr[0], optr[1]);
				}
				if (optr[1]->Sort_Y() > optr[2]->Sort_Y()) {
					swap(optr[1], optr[2]);
				}
				break;

			/*
			**	Large number of objects can be effeciently sorted by using
			**	a quicksort.
			*/
			default:
				qsort(optr, count, sizeof(optr[0]), _ocompare);
				break;
		}

		/*
		**	Draw any objects that happen to be in or overlapping this cell.
		*/
		for (int index = 0; index < count; index++) {
			object = optr[index];
			int xx,yy;
			if (object->IsToDisplay && (!object->Is_Techno() || ((TechnoClass *)object)->Visual_Character() == VISUAL_NORMAL) && Map.Coord_To_Pixel(object->Render_Coord(), xx, yy)) {
				if (_Calc_Partial_Window(x, y, xx, yy)) {
					object->Draw_It(xx, yy, WINDOW_PARTIAL);
					if (Debug_Map) {
						object->IsToDisplay = true;
					} else {
						object->IsToDisplay = false;
					}
				}
				object->IsToDisplay = false;
			}
		}
		BEnd(BENCH_OBJECTS);
	}
#endif

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
#ifdef OBSOLETE
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	static FacingType _even[5] = {FACING_N, FACING_S, FACING_SW, FACING_W, FACING_NW};
	static FacingType _odd[5] = {FACING_N, FACING_NE, FACING_E, FACING_SE, FACING_S};
	FacingType * ptr;		// Working pointer into adjacent cell list.
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
#endif
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
	if (Overlay == OVERLAY_NONE) {
		return;
	}

	OverlayTypeClass const & wall = OverlayTypeClass::As_Reference(Overlay);
	if (!wall.IsWall) {
		return;
	}

	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	static FacingType _offsets[5] = {FACING_N, FACING_E, FACING_S, FACING_W, FACING_NONE};

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

			/*
			**	Handle special cases for the incomplete damaged wall sets. If a damage stage
			**	is calculated, but there is no artwork for it, then consider the wall to be
			**	completely destroyed.
			*/
			if (newcell.Overlay == OVERLAY_BRICK_WALL && newcell.OverlayData == 48) {
				newcell.Overlay = OVERLAY_NONE;
				newcell.OverlayData = 0;
				Detach_This_From_All(::As_Target(newcell.Cell_Number()), true);
			}
			if (newcell.Overlay == OVERLAY_SANDBAG_WALL && newcell.OverlayData == 16) {
				newcell.Overlay = OVERLAY_NONE;
				newcell.OverlayData = 0;
				Detach_This_From_All(::As_Target(newcell.Cell_Number()), true);
			}
			if (newcell.Overlay == OVERLAY_CYCLONE_WALL && newcell.OverlayData == 32) {
				newcell.Overlay = OVERLAY_NONE;
				newcell.OverlayData = 0;
				Detach_This_From_All(::As_Target(newcell.Cell_Number()), true);
			}
			if (newcell.Overlay == OVERLAY_FENCE && (newcell.OverlayData == 16 || newcell.OverlayData == 32)) {
				newcell.Overlay = OVERLAY_NONE;
				newcell.OverlayData = 0;
				Detach_This_From_All(::As_Target(newcell.Cell_Number()), true);
			}
			if (newcell.Overlay == OVERLAY_BARBWIRE_WALL && newcell.OverlayData == 16) {
				newcell.Overlay = OVERLAY_NONE;
				newcell.OverlayData = 0;
				Detach_This_From_All(::As_Target(newcell.Cell_Number()), true);
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	int reducer = 0;

	if (levels > 0 && Land == LAND_TIBERIUM) {
		if (OverlayData+1 > levels) {
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
 * INPUT:   damage   -- The number of damage points the wall was hit with. If this value is    *
 *                      -1, then the entire wall at this cell will be destroyed.               *
 *                                                                                             *
 * OUTPUT:  bool; Was the wall destroyed?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/15/1995 BWG : Created.                                                                 *
 *   03/19/1995 JLB : Updates cell information if wall was destroyed.                          *
 *   10/06/1996 JLB : Updates zone as necessary.                                               *
 *=============================================================================================*/
int CellClass::Reduce_Wall(int damage)
{
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	if (Overlay != OVERLAY_NONE) {
		bool destroyed = false;
		OverlayTypeClass const & wall = OverlayTypeClass::As_Reference(Overlay);

		if (wall.IsWall) {

			/*
			**	If the damage was great enough to ensure wall destruction, reduce the wall by one
			**	level (no more). Otherwise determine wall reduction based on a percentage chance
			**	proportional to the damage received and the wall's strength.
			*/
			if (damage == -1 || damage >= wall.DamagePoints) {
				destroyed = true;
			} else {
				destroyed = Random_Pick(0, wall.DamagePoints) < damage;
			}

			/*
			**	If the wall is destroyed, destroy it and check for any adjustments to
			**	adjacent walls.
			*/
			if (destroyed) {
				OverlayData+=16;
				if (damage == -1 ||
					(OverlayData>>4) >= wall.DamageLevels ||
					((OverlayData>>4) == wall.DamageLevels-1 && (OverlayData & 0xF)==0)	) {

					Owner = HOUSE_NONE;
					Overlay = OVERLAY_NONE;
					OverlayData = 0;
					Recalc_Attributes();
					Redraw_Objects();
					Adjacent_Cell(FACING_N).Wall_Update();
					Adjacent_Cell(FACING_W).Wall_Update();
					Adjacent_Cell(FACING_S).Wall_Update();
					Adjacent_Cell(FACING_E).Wall_Update();
					Detach_This_From_All(As_Target());

					/*
					**	The zone calculation changes now for non-crushable zone sensitive
					**	travellers.
					*/
					if (wall.IsCrushable) {
						Map.Zone_Reset(MZONEF_NORMAL);
					} else {
						Map.Zone_Reset(MZONEF_CRUSHER|MZONEF_NORMAL);
					}
					return(true);
				}
			}
		}
	}
	return(false);
}


/***********************************************************************************************
 * CellClass::Spot_Index -- returns cell sub-coord index for given COORDINATE                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      coord      COORDINATE to compute index for                                             *
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
	COORDINATE rel = Coord_Fraction(coord);		// Sub coordinate value within cell.

	/*
	**	If the coordinate is close enough to the center of the cell, then return
	**	the center position index.
	*/
	if (Distance(rel, (COORDINATE)0x00800080L) < 60) {
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
 * the cell closest to the given coordinate, and returns the COORDINATE of                     *
 * that spot if it's available, NULL if it's not.                                              *
 *                                                                                             *
 * INPUT:                                                                                      *
 *  coord   coordinate to check (only sub cell position examined)                              *
 *                                                                                             *
 *          any   -- If only the closest spot is desired regardless of whether it is free or   *
 *                   not, then this parameter will be true.                                    *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *  COORDINATE of free spot, NULL if none. The coordinate return value does not alter the cell *
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	int spot_index = Spot_Index(coord);

	/*
	**	This precalculated sequence table records the closest spots to any given spot. Sequential
	**	examination of these spots for availability ensures that the closest available one is
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
	coord = Coord_Whole(coord);

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
	unsigned char * sequence;
	if (spot_index == 0) {
		sequence = &_alternate[Random_Pick(0, 3)][0];
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	CELL cell = Cell_Number();
	return((Cell_X(cell) & 0x03) | ((Cell_Y(cell) & 0x03) << 2));
//	return((cell & 0x03) | ((unsigned(cell)>>5) & 0x0C));
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
 *   08/02/1996 JLB : Added the "nokidding" parameter.                                         *
 *=============================================================================================*/
void CellClass::Incoming(COORDINATE threat, bool forced, bool nokidding)
{
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	ObjectClass * object = NULL;

	object = Cell_Occupier();
	while (object != NULL) {

		/*
		**	Special check to make sure that friendly units never scatter.
		*/
		if (nokidding || Rule.IsScatter || (object->Is_Techno() && ((TechnoClass *)object)->House->IQ >= Rule.IQScatter)) {
			object->Scatter(threat, forced, nokidding);
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	if ((unsigned)face >= FACING_COUNT) {
		return(*this);
	}

	//The top row doesn't have any adjacent cells to the north. - LLL
	if (ID < MAP_CELL_W && (face == FACING_N || face == FACING_NE || face == FACING_NW)) {
		return (*this);
	}

	//The bottom row doesn't have any adjacent cells to the south. - LLL
	if ((ID > MAP_CELL_TOTAL - MAP_CELL_W) && (face == FACING_S || face == FACING_SE || face == FACING_SW)) {
		return (*this);
	}

	CellClass const * ptr = this + AdjacentCell[face];
	if ((unsigned)ptr->Cell_Number() > MAP_CELL_TOTAL) return(*this);
	return(*ptr);
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	int region = Map.Cell_Region(Cell_Number());

	for (HousesType lp = HOUSE_FIRST; lp < HOUSE_COUNT; lp ++) {
		if (lp == house) continue;

		HouseClass * house_ptr = HouseClass::As_Pointer(lp);
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
 *   02/20/1996 JLB : Takes into account the ore type.                                         *
 *=============================================================================================*/
long CellClass::Tiberium_Adjust(bool pregame)
{
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);
	if (Overlay != OVERLAY_NONE) {
		if (OverlayTypeClass::As_Reference(Overlay).Land == LAND_TIBERIUM) {
			static int _adj[9] = {0,1,3,4,6,7,8,10,11};
			static int _adjgem[9] = {0,0,0,1,1,1,2,2,2};
			int	count = 0;

			/*
			**	Mixup the Tiberium overlays so that they don't look the same.
			**	Since the type of ore is known, also record the nominal
			**	value per step of that ore type.
			*/
			bool gems = false;
			int value = 0;
			if (pregame) {
				switch (Overlay) {
					case OVERLAY_GOLD1:
					case OVERLAY_GOLD2:
					case OVERLAY_GOLD3:
					case OVERLAY_GOLD4:
						value = Rule.GoldValue;
						Overlay = Random_Pick(OVERLAY_GOLD1, OVERLAY_GOLD4);
						break;

					case OVERLAY_GEMS1:
					case OVERLAY_GEMS2:
					case OVERLAY_GEMS3:
					case OVERLAY_GEMS4:
						gems = true;
						value = Rule.GemValue*4;
						Overlay = Random_Pick(OVERLAY_GEMS1, OVERLAY_GEMS4);
						break;

					default:
						break;
				}
			}

			/*
			**	Add up all adjacent cells that contain tiberium.
			** (Skip those cells which aren't on the map)
			*/
			for (FacingType face = FACING_FIRST; face < FACING_COUNT; face++) {
				if ((unsigned)::Adjacent_Cell(Cell_Number(), face) >= MAP_CELL_TOTAL) continue;
				CellClass & adj = Adjacent_Cell(face);

				if (adj.Overlay != OVERLAY_NONE &&
					OverlayTypeClass::As_Reference(adj.Overlay).Land == LAND_TIBERIUM) {
					count++;
				}
			}

			if (gems) {
				OverlayData = _adjgem[count];
				OverlayData = min(OverlayData, 2);
			} else {
				OverlayData = _adj[count];
			}
			return((OverlayData+1) * value);
		}
	}
	return(0);
}

extern bool MPSuperWeaponDisable;

/***********************************************************************************************
 * CellClass::Goodie_Check -- Performs crate discovery logic.                                  *
 *                                                                                             *
 *    Call this routine whenever an object enters a cell. It will check for the existence      *
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
 *   06/17/1996 JLB : Revamped for Red Alert                                                   *
 *=============================================================================================*/
bool CellClass::Goodie_Check(FootClass * object)
{
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	if (object != NULL && Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(Overlay).IsCrate) {
		bool force_mcv = false;
		int force_money = 0;
		int damage;
		COORDINATE coord;

		/*
		**	Determine the total number of shares for all the crate powerups. This is used as
		**	the base pool to determine the odds from.
		*/
		int total_shares = 0;
		for (int index = CRATE_FIRST; index < CRATE_COUNT; index++) {
			total_shares += CrateShares[index];
		}

		/*
		**	Pick a random crate powerup according to the shares allotted to each powerup.
		**	In solo play, the bonus item is dependant upon the rules control.
		*/
		CrateType powerup;
		if (Session.Type == GAME_NORMAL) {

			/*
			**	Solo play has money amount determined by rules.ini file.
			*/
			force_money = Rule.SoloCrateMoney;

			if (Overlay == OVERLAY_STEEL_CRATE) {
				powerup = Rule.SilverCrate;
			}

			if (Overlay == OVERLAY_WOOD_CRATE) {
				powerup = Rule.WoodCrate;
			}

			if (Overlay == OVERLAY_WATER_CRATE) {
//Mono_Printf("%d-%s.\n", __LINE__, __FILE__);
				powerup = Rule.WaterCrate;
			}

		} else {
			int pick = Random_Pick(1, total_shares);

			int share_count = 0;
			for (powerup = CRATE_FIRST; powerup < CRATE_COUNT; powerup++) {
				share_count += CrateShares[powerup];
				if (pick <= share_count) break;
			}
			assert(powerup != CRATE_COUNT);

			/*
			**	Depending on what was picked, there might be an alternate goodie if the selected
			**	goodie would have no effect.
			*/
			switch (powerup) {
				case CRATE_UNIT:
					if (object->House->CurUnits > 50) powerup = CRATE_MONEY;
					break;

				case CRATE_SQUAD:
					if (object->House->CurInfantry > 100) powerup = CRATE_MONEY;
					break;

				case CRATE_DARKNESS:
					if (object->House->IsGPSActive) powerup = CRATE_MONEY;
					break;

				case CRATE_ARMOR:
					if (object->ArmorBias != 1) powerup = CRATE_MONEY;
					break;

				case CRATE_SPEED:
					if (object->SpeedBias != 1 || object->What_Am_I() == RTTI_AIRCRAFT) powerup = CRATE_MONEY;
					break;

				case CRATE_FIREPOWER:
					if (object->FirepowerBias != 1 || !object->Is_Weapon_Equipped()) powerup = CRATE_MONEY;
					break;

				case CRATE_REVEAL:
					if (object->House->IsVisionary) {
						if (object->House->IsGPSActive) {
							powerup = CRATE_MONEY;
						} else {
							powerup = CRATE_DARKNESS;
						}
					}
					break;

				case CRATE_CLOAK:
					if (object->IsCloakable) powerup = CRATE_MONEY;
					break;

//				case CRATE_HEAL_BASE:
//					if (object->House->BScan == 0) powerup = CRATE_UNIT;

				case CRATE_MONEY:
					break;

				case CRATE_ICBM:
				case CRATE_PARA_BOMB:
				case CRATE_SONAR:
					if (Session.Type != GAME_NORMAL) {
						if (MPSuperWeaponDisable) {
							powerup = CRATE_MONEY;
						}
					}
					break;

				case CRATE_TIMEQUAKE:
					/*
					** For the time quake crate, scan through and count up all the
					** units (and infantry and ships and aircraft) and if either
					** side has very few, allow the time quake.  Otherwise,
					** change the crate to money or something.  Only do this for
					** multiplay - for solo play, they get what they get.  First,
					** check for time - the chance for getting a time quake crate
					** should be very very low when they first start the mission,
					** but as time goes on the chance goes up.
					*/
					if (Session.Type != GAME_NORMAL) {
						int i,ucount;
						int minunits = 1000;
						bool found = false;
						unsigned long minutes = (Score.ElapsedTime / TIMER_MINUTE);
						if (minutes > 100) minutes = 100;
						if (Random_Pick(0,100-(int)minutes) == 0) {
							for (i=0; i < (Session.Players.Count() + Session.Options.AIPlayers); i++) {
								ucount = 0;
								HouseClass * hptr = Houses.Ptr(i + HOUSE_MULTI1);
								if (hptr != NULL && !hptr->IsDefeated) {
									int j;
									for( j=0; j < UNIT_COUNT; j++) {
										ucount += hptr->QuantityU(j);
									}
									for( j=0; j < INFANTRY_COUNT; j++) {
										ucount += hptr->QuantityI(j);
									}
									for( j=0; j < AIRCRAFT_COUNT; j++) {
										ucount += hptr->QuantityA(j);
									}
									for( j=0; j < VESSEL_COUNT; j++) {
										ucount += hptr->QuantityV(j);
									}
									int bcount = 0;
									for( j=0; j < STRUCT_COUNT; j++) {
										bcount += hptr->QuantityB(j);
									}
									ucount += bcount/2;	// weight buildings less
									minunits = min(minunits, ucount);
								}
							}
							if (Random_Pick(0, minunits) == minunits) {
								found = true;
							}
						}

						if (!found) {
							powerup = CRATE_MONEY;
						}
					}
					break;
			}
			/*
			**	Possibly force it to be an MCV if there is
			**	sufficient money and no buildings left.
			*/
			if (	object->House->BScan == 0 &&
					object->House->Available_Money() > ( (BuildingTypeClass::As_Reference(STRUCT_REFINERY).Cost + BuildingTypeClass::As_Reference(STRUCT_POWER).Cost) * object->House->CostBias) &&
					Session.Options.Bases &&
					!(object->House->UScan & UNITF_MCV)) {
				powerup = CRATE_UNIT;
				force_mcv = true;
			}

			/*
			**	If the powerup is money but there is insufficient money to build a refinery but there is a construction
			**	yard available, then force the money to be enough to rebuild the refinery.
			*/
			if (powerup == CRATE_MONEY && (object->House->BScan & (STRUCTF_CONST|STRUCTF_REFINERY)) == STRUCTF_CONST &&
						object->House->Available_Money() < BuildingTypeClass::As_Reference(STRUCT_REFINERY).Cost * object->House->CostBias) {

				force_money = BuildingTypeClass::As_Reference(STRUCT_REFINERY).Cost * object->House->CostBias;
			}

			/*
			**	Special override for water crates so that illegal goodies items
			**	won't appear.
			*/
			if (Overlay == OVERLAY_WATER_CRATE) {
				switch (powerup) {
					case CRATE_UNIT:
					case CRATE_SQUAD:
						powerup = CRATE_MONEY;
						break;

					default:
						break;
				}
			}
		}

		/*
		** Keep track of the number of each type of crate found
		*/
		if (Session.Type == GAME_INTERNET) {
			object->House->TotalCrates->Increment_Unit_Total(powerup);
		}

		/*
		**	Remove the crate from the map.
		*/
		Map.Remove_Crate(Cell_Number());
//		Map[Cell_Number()].Overlay = OVERLAY_NONE;

		if (Session.Type != GAME_NORMAL && Rule.IsMPCrates) {
			Map.Place_Random_Crate();
		}

		/*
		**	Generate any corresponding animation associated with this crate powerup.
		*/
		if (CrateAnims[powerup] != ANIM_NONE) {
			new AnimClass(CrateAnims[powerup], Cell_Coord());
		}

		/*
		**	Create the effect requested.
		*/
		bool tospeak = false;
		switch (powerup) {
			case CRATE_TIMEQUAKE:
				TimeQuake = true;
				break;

			/*
			**	Give the player money.
			*/
			case CRATE_MONEY:
crate_money:
				if (force_money > 0) {
					object->House->Refund_Money(force_money);
				} else {
					object->House->Refund_Money(Random_Pick(CrateData[powerup], CrateData[powerup]+900));
				}
				break;

			/*
			**	Shroud the world in blackness.
			*/
			case CRATE_DARKNESS:
				/*
				** Updated for client/server multiplayer. ST - 8/12/2019 11:38AM
				*/
				if (object->House->IsHuman) {
					Map.Shroud_The_Map(object->House);
				}
				break;

			/*
			**	Reveal the entire map.
			*/
			case CRATE_REVEAL:
				/*
				** Updated for client/server multiplayer. ST - 8/12/2019 11:38AM
				*/
				object->House->IsVisionary = true;
				if (object->House->IsHuman) {
					for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
						Map.Map_Cell(cell, object->House);
					}
					Map.Flag_To_Redraw(true);
				}
				break;

			/*
			**	Try to create a unit where the crate was.
			*/
			case CRATE_UNIT: {
				UnitTypeClass const * utp = NULL;

				/*
				**	Give the player an MCV if he has no base left but does have more than enough
				**	money to rebuild a new base. Of course, if he already has an MCV, then don't
				**	give him another one.
				*/
				if (force_mcv) {
					utp = &UnitTypeClass::As_Reference(UNIT_MCV);
				}

				/*
				**	If the player has a base and a refinery, but no harvester, then give him
				**	a free one.
				*/
				if (utp == NULL && (object->House->BScan & STRUCTF_REFINERY) && !(object->House->UScan & UNITF_HARVESTER)) {
					utp = &UnitTypeClass::As_Reference(UNIT_HARVESTER);
				}

				/*
				**	Check for special unit type override value.
				*/
				if (Rule.UnitCrateType != UNIT_NONE) {
					utp = &UnitTypeClass::As_Reference(Rule.UnitCrateType);
				}

				/*
				**	If no unit type has been determined, then pick one at random.
				*/
				while (utp == NULL) {
#ifdef FIXIT_ANTS
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
					UnitType utype = Random_Pick(UNIT_FIRST, (UnitType)(UNIT_RA_COUNT-1 -3));
#else
					UnitType utype = Random_Pick(UNIT_FIRST, (UnitType)(UNIT_COUNT-1 -3));
#endif
#else
					UnitType utype = Random_Pick(UNIT_FIRST, (UnitType)(UNIT_COUNT-1));
#endif
					if (utype != UNIT_MCV || Session.Options.Bases) {
						utp = &UnitTypeClass::As_Reference(utype);
						if (utp->IsCrateGoodie && (utp->Ownable & (1 << HouseClass::As_Pointer(object->Owner())->ActLike))) {
							break;
						}
						utp = NULL;
					}
				}

				if (utp != NULL) {
					UnitClass * goodie_unit = (UnitClass *)utp->Create_One_Of(object->House);
					if (goodie_unit != NULL) {
						if (goodie_unit->Unlimbo(Cell_Coord())) {
							return(false);
						}

						/*
						**	Try to place the object into a nearby cell if something is preventing
						**	placement at the crate location.
						*/
						CELL cell = Map.Nearby_Location(Cell_Number(), goodie_unit->Class->Speed);
						if (goodie_unit->Unlimbo(::Cell_Coord(cell))) {
							return(false);
						}
						delete goodie_unit;
						goto crate_money;
					}
				}
			}
			break;

			/*
			**	Create a squad of miscellaneous composition.
			*/
			case CRATE_SQUAD:
				for (int index = 0; index < 5; index++) {
					static InfantryType _inf[] = {
						INFANTRY_E1,INFANTRY_E1,INFANTRY_E1,INFANTRY_E1,INFANTRY_E1,INFANTRY_E1,
						INFANTRY_E2,
						INFANTRY_E3,
						INFANTRY_RENOVATOR
					};
					if (!InfantryTypeClass::As_Reference(_inf[Random_Pick(0, ARRAY_SIZE(_inf)-1)]).Create_And_Place(Cell_Number(), object->Owner())) {
						if (index == 0) {
							goto crate_money;
						}
					}
				}
				return(false);

			/*
			**	A one para-bomb mission.
			*/
			case CRATE_PARA_BOMB:
				if (object->House->SuperWeapon[SPC_PARA_BOMB].Enable(true)) {
					// Changes for client/server multiplayer. ST - 8/2/2019 2:56PM
					if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
						if (object->House->IsHuman) {
							Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_PARA_BOMB, object->House);
						}
					} else {
						if (object->IsOwnedByPlayer) {
							Map.Add(RTTI_SPECIAL, SPC_PARA_BOMB);
							Map.Column[1].Flag_To_Redraw();
						}
					}					
				}
				break;

			/*
			**	A one time sonar pulse
			*/
			case CRATE_SONAR:
				if (object->House->SuperWeapon[SPC_SONAR_PULSE].Enable(true)) {
					// Changes for client/server multiplayer. ST - 8/2/2019 2:56PM
					if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
						if (object->House->IsHuman) {
							Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_SONAR_PULSE, object->House);
						}
					} else {
						if (object->IsOwnedByPlayer) {
							Map.Add(RTTI_SPECIAL, SPC_SONAR_PULSE);
							Map.Column[1].Flag_To_Redraw();
						}
					}					
				}
				break;

			/*
			**	A group of explosions are triggered around the crate.
			*/
			case CRATE_EXPLOSION:
				if (object != NULL) {
					int d = CrateData[powerup];
					object->Take_Damage(d, 0, WARHEAD_HE, 0, true);
				}
				for (int index = 0; index < 5; index++) {
					COORDINATE frag_coord = Coord_Scatter(Cell_Coord(), Random_Pick(0, 0x0200));
					new AnimClass(ANIM_FBALL1, frag_coord);
					damage = CrateData[powerup];
					Explosion_Damage(frag_coord, damage, NULL, WARHEAD_HE);
				}
				break;

			/*
			**	A napalm blast is triggered.
			*/
			case CRATE_NAPALM:
				coord = Coord_Mid(Cell_Coord(), object->Center_Coord());
				new AnimClass(ANIM_NAPALM3, coord);
				if (object != NULL) {
					int d = CrateData[powerup];
					object->Take_Damage(d, 0, WARHEAD_FIRE, 0, true);
				}
				damage = CrateData[powerup];
				Explosion_Damage(coord, damage, NULL, WARHEAD_FIRE);
				break;

			/*
			**	All objects within a certain range will gain the ability to cloak.
			*/
			case CRATE_CLOAK:
				for (int index = 0; index < DisplayClass::Layer[LAYER_GROUND].Count(); index++) {
					ObjectClass * obj = DisplayClass::Layer[LAYER_GROUND][index];

					if (obj && obj->Is_Techno() && Distance(Cell_Coord(), obj->Center_Coord()) < Rule.CrateRadius) {
						((TechnoClass *)obj)->IsCloakable = true;
					}
				}
				break;

			/*
			**	All of the player's objects heal up.
			*/
			case CRATE_HEAL_BASE:
				if (object->IsOwnedByPlayer) {
					Sound_Effect(VOC_HEAL, object->Center_Coord());
				}
				for (int index = 0; index < Logic.Count(); index++) {
					ObjectClass * obj = Logic[index];

					if (obj && object->Is_Techno() && object->House->Class->House == obj->Owner()) {
						obj->Strength = obj->Class_Of().MaxStrength;
					}
				}
				break;


			case CRATE_ICBM:
				if (object->House->SuperWeapon[SPC_NUCLEAR_BOMB].Enable(true)) {
					// Changes for client/server multiplayer. ST - 8/2/2019 2:56PM
					if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
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

			case CRATE_ARMOR:
				for (int index = 0; index < DisplayClass::Layer[LAYER_GROUND].Count(); index++) {
					ObjectClass * obj = DisplayClass::Layer[LAYER_GROUND][index];

					if (obj != NULL && obj->Is_Techno() && Distance(Cell_Coord(), obj->Center_Coord()) < Rule.CrateRadius && ((TechnoClass *)obj)->ArmorBias == 1) {
						fixed val = ((TechnoClass *)obj)->ArmorBias * Inverse(fixed(CrateData[powerup], 256));
						((TechnoClass *)obj)->ArmorBias = val;
						if (obj->Owner() == PlayerPtr->Class->House) tospeak = true;
					}
				}
				if (tospeak) Speak(VOX_UPGRADE_ARMOR);
				break;

			case CRATE_SPEED:
				for (int index = 0; index < DisplayClass::Layer[LAYER_GROUND].Count(); index++) {
					ObjectClass * obj = DisplayClass::Layer[LAYER_GROUND][index];

					if (obj && obj->Is_Foot() && Distance(Cell_Coord(), obj->Center_Coord()) < Rule.CrateRadius && ((FootClass *)obj)->SpeedBias == 1 && obj->What_Am_I() != RTTI_AIRCRAFT) {
						FootClass * foot = (FootClass *)obj;

						fixed val = foot->SpeedBias * fixed(CrateData[powerup], 256);
						foot->SpeedBias = val;
						if (foot->IsOwnedByPlayer) tospeak = true;
					}
				}
				if (tospeak) Speak(VOX_UPGRADE_SPEED);
				break;

			case CRATE_FIREPOWER:
				for (int index = 0; index < DisplayClass::Layer[LAYER_GROUND].Count(); index++) {
					ObjectClass * obj = DisplayClass::Layer[LAYER_GROUND][index];

					if (obj && obj->Is_Techno() && Distance(Cell_Coord(), obj->Center_Coord()) < Rule.CrateRadius && ((TechnoClass *)obj)->FirepowerBias == 1) {

						fixed val = ((TechnoClass *)obj)->FirepowerBias * fixed(CrateData[powerup], 256);
						((TechnoClass *)obj)->FirepowerBias = val;
						if (obj->Owner() == PlayerPtr->Class->House) tospeak = true;
					}
				}
				if (tospeak) Speak(VOX_UPGRADE_FIREPOWER);
				break;

			case CRATE_INVULN:
				for (int index = 0; index < DisplayClass::Layer[LAYER_GROUND].Count(); index++) {
					ObjectClass * obj = DisplayClass::Layer[LAYER_GROUND][index];

					if (obj && obj->Is_Techno() && Distance(Cell_Coord(), obj->Center_Coord()) < Rule.CrateRadius) {
						((TechnoClass *)obj)->IronCurtainCountDown = (TICKS_PER_MINUTE * fixed(CrateData[powerup], 256));
						obj->Mark(MARK_CHANGE);
					}
				}
				break;

			/*
			** A chronal vortex appears targetted at the triggering object.
			*/
			case CRATE_VORTEX:
				if ( !ChronalVortex.Is_Active()) {
					ChronalVortex.Appear ( Cell_Coord() );
					ChronalVortex.Set_Target ( (ObjectClass*) object );
					Sound_Effect(VOC_TESLA_ZAP, object->Center_Coord());
				}
				break;

			default:
				break;
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	if (!IsFlagged && Is_Clear_To_Move(SPEED_TRACK, false, false)) {
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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

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
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	ObjectClass * object = Cell_Occupier();

	while (object) {
		object->Do_Shimmer();
		object = object->Next;
	}
}


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
bool CellClass::Is_Clear_To_Move(SpeedType loco, bool ignoreinfantry, bool ignorevehicles, int zone, MZoneType check) const
{
	assert((unsigned)Cell_Number() <= MAP_CELL_TOTAL);

	/*
	**	Flying objects always consider every cell passable since they can fly over everything.
	*/
	if (loco == SPEED_WINGED) {
		return(true);
	}

	/*
	**	If a zone was specified, then see if the cell is in a legal
	**	zone to allow movement.
	*/
	if (zone != -1) {
		if (zone != Zones[check]) {
			return(false);
		}
	}

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
	LandType land = Land_Type();

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
		if (check != MZONE_DESTROYER && (check != MZONE_CRUSHER || !overlay->IsCrushable)) {
			return(false);
		}

		/*
		**	Crushing objects consider crushable walls as clear rather than the
		**	typical LAND_WALL setting.
		*/
		land = LAND_CLEAR;
	}

	/*
	**	See if the ground type is impassable to this locomotion type and if
	**	so, return the error condition.
	*/
	if (::Ground[land].Cost[loco] == 0) {
		return(false);
	}

	/*
	**	All checks passed, so this cell must be passable.
	*/
	return(true);
}


/***********************************************************************************************
 * CellClass::Is_Bridge_Here -- Checks to see if this is a bridge occupied cell.               *
 *                                                                                             *
 *    This routine will examine this cell and if there is a bridge here, it will return        *
 *    true.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is there a bridge located in this cell?                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CellClass::Is_Bridge_Here(void) const
{
	switch (TType) {
		case TEMPLATE_BRIDGE1:
		case TEMPLATE_BRIDGE1H:
		case TEMPLATE_BRIDGE1D:
		case TEMPLATE_BRIDGE2:
		case TEMPLATE_BRIDGE2H:
		case TEMPLATE_BRIDGE2D:
		case TEMPLATE_BRIDGE_1A:
		case TEMPLATE_BRIDGE_1B:
		case TEMPLATE_BRIDGE_2A:
		case TEMPLATE_BRIDGE_2B:
		case TEMPLATE_BRIDGE_3A:
		case TEMPLATE_BRIDGE_3B:
		case TEMPLATE_BRIDGE_3C:
		case TEMPLATE_BRIDGE_3D:
		case TEMPLATE_BRIDGE_3E:
		case TEMPLATE_BRIDGE_3F:
			return(true);
	}
	return(false);
}


/***********************************************************************************************
 * CellClass::Can_Tiberium_Grow -- Determines if Tiberium can grow in this cell.               *
 *                                                                                             *
 *    This checks the cell to see if Tiberium can grow at least one level in it. Tiberium can  *
 *    grow only if there is Tiberium already present. It can only grow to a certain level      *
 *    and then all further growth is suspended.                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can Tiberium grow in this cell?                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CellClass::Can_Tiberium_Grow(void) const
{
	if (!Rule.IsTGrowth) return(false);

	if (Session.Type != GAME_NORMAL) {
		if(!Session.Options.Tiberium) return(false);
	}

	if (Land_Type() != LAND_TIBERIUM) return(false);

	if (OverlayData >= 11) return(false);

	if (Overlay != OVERLAY_GOLD1 && Overlay != OVERLAY_GOLD2 && Overlay != OVERLAY_GOLD3 && Overlay != OVERLAY_GOLD4) return(false);

	return(true);
}


/***********************************************************************************************
 * CellClass::Can_Tiberium_Spread -- Determines if Tiberium can spread from this cell.         *
 *                                                                                             *
 *    This routine will examine the cell and determine if there is sufficient Tiberium         *
 *    present that Tiberium spores will spread to adjacent cells. If the Tiberium level is     *
 *    too low, spreading will not occur.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can Tiberium spread from this cell into adjacent cells?                      *
 *                                                                                             *
 * WARNINGS:   This routine does not check to see if, in fact, there are any adjacent cells    *
 *             available to spread to.                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CellClass::Can_Tiberium_Spread(void) const
{
	if (!Rule.IsTSpread) return(false);

	if (Session.Type != GAME_NORMAL) {
		if(!Session.Options.Tiberium) return(false);
	}

	if (Land_Type() != LAND_TIBERIUM) return(false);

	if (OverlayData <= 6) return(false);

	if (Overlay != OVERLAY_GOLD1 && Overlay != OVERLAY_GOLD2 && Overlay != OVERLAY_GOLD3 && Overlay != OVERLAY_GOLD4) return(false);

	return(true);
}


/***********************************************************************************************
 * CellClass::Grow_Tiberium -- Grows the tiberium in the cell.                                 *
 *                                                                                             *
 *    This routine will cause the tiberium to grow in the cell.                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Did Tiberium grow in the cell?                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CellClass::Grow_Tiberium(void)
{
	if (Can_Tiberium_Grow()) {
		OverlayData++;
		Redraw_Objects();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * CellClass::Spread_Tiberium -- Spread Tiberium from this cell to an adjacent cell.           *
 *                                                                                             *
 *    This routine will cause the Tiberium to spread from this cell into an adjacent (random)  *
 *    cell.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Did the Tiberium spread?                                                     *
 *                                                                                             *
 * WARNINGS:   If there are no adjacent cells that the tiberium can spread to, then this       *
 *             routine will fail.                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CellClass::Spread_Tiberium(bool forced)
{
	if (!forced) {
		if (!Can_Tiberium_Spread()) return(false);
	}
	FacingType offset = Random_Pick(FACING_N, FACING_NW);
	for (FacingType index = FACING_N; index < FACING_COUNT; index++) {
		CellClass * newcell = &Adjacent_Cell(index+offset);

		if (newcell != NULL && newcell->Can_Tiberium_Germinate()) {
			new OverlayClass(Random_Pick(OVERLAY_GOLD1, OVERLAY_GOLD4), newcell->Cell_Number());
			newcell->OverlayData = 0;
			return(true);
		}
	}
	return(false);
}


/***********************************************************************************************
 * CellClass::Can_Tiberium_Germinate -- Determines if Tiberium can begin growth in the cell.   *
 *                                                                                             *
 *    This routine will examine the cell and determine if Tiberium can start growth in it.     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can Tiberium grow in this cell?                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CellClass::Can_Tiberium_Germinate(void) const
{
	if (!Map.In_Radar(Cell_Number())) return(false);

	if (Is_Bridge_Here()) return(false);

	/*
	**	Don't allow Tiberium to grow on a cell with a building unless that building is
	**	invisible. In such a case, the Tiberium must grow or else the location of the
	**	building will be revealed.
	*/
	BuildingClass const * building = Cell_Building();
	if (building != NULL && !building->Class->IsInvisible) return(false);

	if (!Ground[Land_Type()].Build) return(false);

	if (Overlay != OVERLAY_NONE) return(false);

	return(true);
}










/*
**  Additions to CellClass to track visibility per-player. ST - 8/2/2019 2:59PM
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
		if (Session.Type == GAME_NORMAL && player->IsHuman) {
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
		if (Session.Type == GAME_NORMAL && player->IsHuman) {
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

bool CellClass::Is_Jamming(HousesType house) const
{
	int shift = (int)house;
	return (Jammed & (1 << shift)) ? true : false;
}

bool CellClass::Is_Jamming(HouseClass *player) const
{
	if (player && player->Class) {
		return Is_Jamming(player->Class->House);
	}
	return false;
}

void CellClass::Override_Land_Type(LandType type)
{
	OverrideLand = type;
}