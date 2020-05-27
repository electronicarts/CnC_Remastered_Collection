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

/* $Header: /CounterStrike/OVERLAY.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : OVERLAY.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 17, 1994                                                 *
 *                                                                                             *
 *                  Last Update : July 24, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   OverlayClass::Read_INI -- Reads the overlay data from an INI file.                        *
 *   OverlayClass::Init -- Resets the overlay object system.                                   *
 *   OverlayClass::Mark -- Marks the overlay down on the map.                                  *
 *   OverlayClass::OverlayClass -- Overlay object constructor.                                 *
 *   OverlayClass::delete -- Returns a overlay object to the pool.                             *
 *   OverlayClass::new -- Allocates a overlay object from pool                                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"overlay.h"


HousesType OverlayClass::ToOwn = HOUSE_NONE;



/***********************************************************************************************
 * OverlayClass::Init -- Resets the overlay object system.                                     *
 *                                                                                             *
 *    This routine resets the overlay object system. It is called                              *
 *    prior to loading a new scenario.                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void OverlayClass::Init(void)
{
	Overlays.Free_All();
}


/***********************************************************************************************
 * OverlayClass::new -- Allocates a overlay object from pool                                   *
 *                                                                                             *
 *    This routine is used to allocate a overlay object from the                               *
 *    overlay object pool.                                                                     *
 *                                                                                             *
 * INPUT:   size  -- The size of a overlay object (not used).                                  *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to an available overlay object.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void * OverlayClass::operator new(size_t )
{
	void * ptr = Overlays.Allocate();
	if (ptr) {
		((OverlayClass *)ptr)->Set_Active();
	}
	return(ptr);
}


/***********************************************************************************************
 * OverlayClass::delete -- Returns a overlay object to the pool.                               *
 *                                                                                             *
 *    This routine will return a overlay object to the overlay object                          *
 *    pool. A overlay so returned is available for allocation again.                           *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the object to be returned.                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void OverlayClass::operator delete(void * ptr)
{
	if (ptr) {
		((OverlayClass *)ptr)->IsActive = false;
	}
	Overlays.Free((OverlayClass *)ptr);
}


/***********************************************************************************************
 * OverlayClass::OverlayClass -- Overlay object constructor.                                   *
 *                                                                                             *
 *    This is the constructor for a overlay object.                                            *
 *                                                                                             *
 * INPUT:   type  -- The overlay object this is to become.                                     *
 *                                                                                             *
 *          pos   -- The position on the map to place the object.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
OverlayClass::OverlayClass(OverlayType type, CELL pos, HousesType house) :
	ObjectClass(RTTI_OVERLAY, Overlays.ID(this)),
	Class(OverlayTypes.Ptr((int)type))
{
	if (pos != -1) {
		ToOwn = house;
		Unlimbo(Cell_Coord(pos));
		ToOwn = HOUSE_NONE;
	}
}


/***********************************************************************************************
 * OverlayClass::Mark -- Marks the overlay down on the map.                                    *
 *                                                                                             *
 *    This routine will place the overlay onto the map. The overlay object is deleted by this  *
 *    operation. The map is updated to reflect the presence of the overlay.                    *
 *                                                                                             *
 * INPUT:   mark  -- The type of marking to perform. Only MARK_DOWN is supported.              *
 *                                                                                             *
 * OUTPUT:  bool; Was the overlay successfully marked? Failure occurs if it is not being       *
 *                marked down.                                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *   12/23/1994 JLB : Checks low level legality before proceeding.                             *
 *=============================================================================================*/
bool OverlayClass::Mark(MarkType mark)
{
	assert(Overlays.ID(this) == ID);
	assert(IsActive);

	if (ObjectClass::Mark(mark)) {
		if (mark == MARK_DOWN) {
			CELL	cell = Coord_Cell(Coord);
			CellClass * cellptr = &Map[cell];

			/*
			**	Walls have special logic when they are marked down.
			*/
			if (Class->IsWall) {
				if (cellptr->Is_Clear_To_Build()) {
					cellptr->Overlay = Class->Type;
					cellptr->OverlayData = 0;
					cellptr->Redraw_Objects();
					cellptr->Wall_Update();
					Map.Zone_Reset(Class->IsCrushable ? MZONE_NORMAL : MZONE_NORMAL|MZONE_CRUSHER);

					/*
					**	Flag ownership of the cell if the 'global' ownership flag indicates that this
					**	is necessary for the overlay.
					*/
					if (ToOwn != HOUSE_NONE) {
						cellptr->Owner = ToOwn;
					}

				} else {
					delete this;
					return(false);
				}
			} else {

				bool clear = false;
				if (!ScenarioInit) {
					if (Class->Type == OVERLAY_WATER_CRATE) {
						clear = cellptr->Is_Clear_To_Move(SPEED_FLOAT, false, false);
					} else {
						if (Class->Type == OVERLAY_STEEL_CRATE || Class->Type == OVERLAY_WOOD_CRATE) {
							clear = cellptr->Is_Clear_To_Move(SPEED_TRACK, false, false);
						} else {
							clear = cellptr->Is_Clear_To_Move(SPEED_TRACK, true, true);
						}
					}
				} else {
					clear = true;
				}

				if ((ScenarioInit || cellptr->Overlay == OVERLAY_NONE) && clear) {

					cellptr->Overlay = Class->Type;
					cellptr->OverlayData = 0;

					cellptr->Redraw_Objects();
					if (Class->Land == LAND_TIBERIUM) {
						cellptr->OverlayData = 1;
						cellptr->Tiberium_Adjust();
					}
				}
			}

			/*
			**	*****  Is this really needed?
			*/
			cellptr->Recalc_Attributes();

			/*
			**	Remove the overlay and make sure the system thinks it was never placed down!
			*/
			Map.Overlap_Up(Coord_Cell(Coord), this);
			IsDown = false;
			IsInLimbo = true;

			delete this;
			return(true);
		}
	}
	return(false);
}


/***********************************************************************************************
 * OverlayClass::Read_INI -- Reads the overlay data from an INI file.                          *
 *                                                                                             *
 *    This routine is used to load a scenario's overlay data. The overlay objects are read     *
 *    from the INI file and then created on the map.                                           *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the INI file staging buffer.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Requires that all the buildings be placed first, so the scan for assigning wall *
 *             ownership to the nearest building will work.                                    *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *   07/24/1995 JLB : Specifically forbid manual crates in multiplayer scenarios.              *
 *=============================================================================================*/
void OverlayClass::Read_INI(CCINIClass & ini)
{
	if (NewINIFormat > 1) {
		int len = ini.Get_UUBlock("OverlayPack", _staging_buffer, sizeof(_staging_buffer));

		if (len > 0) {
			BufferStraw bpipe(_staging_buffer, len);
			LCWStraw uncomp(LCWStraw::DECOMPRESS);
			uncomp.Get_From(&bpipe);

			for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
				OverlayType classid;

				uncomp.Get(&classid, sizeof(classid));

				if (classid != OVERLAY_NONE) {

					if (Session.Type == GAME_NORMAL || !OverlayTypeClass::As_Reference(classid).IsCrate) {

						/*
						**	Don't allow placement of overlays on the top or bottom rows of
						**	the map.
						*/
						if (cell >= MAP_CELL_W && cell <= MAP_CELL_TOTAL - MAP_CELL_W) {
							new OverlayClass(classid, cell);

							// Assign house ownership to cells with walls in 'em.
							if (OverlayTypeClass::As_Reference(classid).IsWall) {
								HousesType owner = HOUSE_NONE;
								int distance = 0x7FFFFFFF;
								for (int index = 0; index < Buildings.Count(); index++) {
									BuildingClass * building = Buildings.Ptr(index);
									int newdist = ::Distance(building->Center_Coord(), Cell_Coord(cell));
									if (newdist < distance) {
										distance = newdist;
										owner = building->Owner();
									}
								}
								Map[cell].Owner = owner;
							}
						}
					}
				}
			}
		}
	}

	if (NewINIFormat < 2 || ini.Is_Present("Overlay")) {
		int len = ini.Entry_Count(INI_Name());
		for (int index = 0; index < len; index++) {
			char const * entry = ini.Get_Entry(INI_Name(), index);
			CELL cell = atoi(entry);
			OverlayType classid = ini.Get_OverlayType(INI_Name(), entry, OVERLAY_NONE);

			/*
			**	Don't allow placement of crates in the multiplayer scenarios.
			*/
			if (classid != OVERLAY_NONE && (Session.Type == GAME_NORMAL || !OverlayTypeClass::As_Reference(classid).IsCrate)) {

				/*
				**	Don't allow placement of overlays on the top or bottom rows of
				**	the map.
				*/
				if (cell >= MAP_CELL_W && cell <= MAP_CELL_TOTAL - MAP_CELL_W) {
					new OverlayClass(classid, cell);

					// Assign house ownership to cells with walls in 'em.
					if (OverlayTypeClass::As_Reference(classid).IsWall) {
						HousesType owner = HOUSE_NONE;
						int distance = 0x7FFFFFFF;
						for (int index = 0; index < Buildings.Count(); index++) {
							BuildingClass * building = Buildings.Ptr(index);
							int newdist = ::Distance(building->Center_Coord(), Cell_Coord(cell));
							if (newdist < distance) {
								distance = newdist;
								owner = building->Owner();
							}
						}
						Map[cell].Owner = owner;
					}
				}
			}
		}
	}
}


void OverlayClass::Write_INI(CCINIClass & ini)
{
	/*
	**	First, clear out all existing unit data from the ini file.
	*/
	ini.Clear(INI_Name());
	ini.Clear("OverlayPack");

	BufferPipe bpipe(_staging_buffer, sizeof(_staging_buffer));
	LCWPipe comppipe(LCWPipe::COMPRESS);

	comppipe.Put_To(&bpipe);

	int total = 0;
	CellClass * cellptr = &Map[(CELL)0];
	for (CELL index = 0; index < MAP_CELL_TOTAL; index++) {
		total += comppipe.Put(&cellptr->Overlay, sizeof(cellptr->Overlay));
		cellptr++;
	}
	if (total) {
		ini.Put_UUBlock("OverlayPack", _staging_buffer, total);
	}

//	for (CELL index = 0; index < MAP_CELL_TOTAL; index++) {
//		CellClass * cellptr = &Map[index];
//		if (cellptr->Overlay != OVERLAY_NONE) {
//			char	uname[10];
//			sprintf(uname, "%d", index);
//			ini.Put_Overlay(INI_Name(), uname, cellptr->Overlay);
//		}
//	}
}