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

/* $Header:   F:\projects\c&c\vcs\code\overlay.cpv   2.17   16 Oct 1995 16:50:44   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *   OverlayClass::Write_INI -- Writes the overlay data to an INI file.                        *
 *   OverlayClass::delete -- Returns a overlay object to the pool.                             *
 *   OverlayClass::Init -- Resets the overlay object system.                                   *
 *   OverlayClass::new -- Allocates a overlay object from pool                                 *
 *   OverlayClass::OverlayClass -- Overlay object constructor.                                 *
 *   OverlayClass::Mark -- Marks the overlay down on the map.                                  *
 *   OverlayClass::Validate -- validates overlay															  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"overlay.h"

/*
** This contains the value of the Virtual Function Table Pointer
*/
void * OverlayClass::VTable;

HousesType OverlayClass::ToOwn = HOUSE_NONE;

OverlayClass::OverlayClass(void) : Class(0) {ToOwn = HOUSE_NONE;};


/***********************************************************************************************
 * OverlayClass::Validate -- validates overlay																  *
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
int OverlayClass::Validate(void) const
{
	int num;

	num = Overlays.ID(this);
	if (num < 0 || num >= OVERLAY_MAX) {
		Validate_Error("OVERLAY");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
#endif


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
	OverlayClass *ptr;

	Overlays.Free_All();

	ptr = new OverlayClass();
	VTable = ((void **)(((char *)ptr) + sizeof(AbstractClass) - 4))[0];
	delete ptr;
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
void OverlayClass::operator delete(void *ptr)
{
	if (ptr) {
		((OverlayClass *)ptr)->IsActive = false;
	}
	Overlays.Free((OverlayClass *)ptr);

	//Map.Validate();
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
	Class(&OverlayTypeClass::As_Reference(type))
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
	Validate();
	if (ObjectClass::Mark(mark)) {
		if (mark == MARK_DOWN) {
			CELL	cell = Coord_Cell(Coord);
			CellClass * cellptr = &Map[cell];

			/*
			**	Road placement occurs in two steps. First the foundation is placed, but only
			**	on buildable terrain. Second, the road is completed, but only if the foundation
			**	was previously placed.
			*/
			if (*this == OVERLAY_ROAD) {
				if ((cellptr->Overlay == OVERLAY_ROAD && cellptr->OverlayData == 0) ||
					(cellptr->Overlay == OVERLAY_NONE && cellptr->Is_Generally_Clear())) {

					if (cellptr->Overlay == OVERLAY_ROAD) {
						cellptr->OverlayData = 1;
					} else {
						cellptr->OverlayData = 0;
					}
					cellptr->Overlay = Class->Type;
					cellptr->Redraw_Objects();
				}
			} else {

				/*
				**	Walls have special logic when they are marked down.
				*/
				if (Class->IsWall) {
					if (cellptr->Is_Generally_Clear() && cellptr->Overlay != OVERLAY_FLAG_SPOT) {
						cellptr->Overlay = Class->Type;
						cellptr->OverlayData = 0;
						cellptr->Redraw_Objects();
						cellptr->Wall_Update();

						/*
						**	Flag ownership of the cell if the 'global' ownership flag indicates that this
						**	is necessary for the overlay.
						*/
						if (ToOwn != HOUSE_NONE) {
							cellptr->Owner = ToOwn;
						}

					} else {
						Delete_This();
						return(false);
					}
				} else {
					if ((cellptr->Overlay == OVERLAY_NONE || cellptr->Overlay == OVERLAY_SQUISH) && !cellptr->Cell_Terrain() && Ground[cellptr->Land_Type()].Build) {

						/*
						**	Increment the global crate counter. This is used to regulate
						**	the crate generation.
						*/
						if (Class->IsCrate) CrateCount++;

						/*
						**	Don't show the squish unless the gross flag is active.
						*/
						if (!Special.IsGross && Class->Type != OVERLAY_SQUISH) {
							cellptr->Overlay = Class->Type;
							cellptr->OverlayData = 0;
						}
						cellptr->Redraw_Objects();
						if (Class->Land == LAND_TIBERIUM) {
							cellptr->OverlayData = 1;
							cellptr->Tiberium_Adjust();
						} else {
							if (*this == OVERLAY_CONCRETE) {
								CELL	newcell;

								/*
								**	Smudges go away when concrete is laid down.
								*/
								cellptr->Smudge = SMUDGE_NONE;
								cellptr->SmudgeData = 0;
								cellptr->Concrete_Calc();

								/*
								**	Possibly add concrete to adjacent cells depending on whether this
								**	concrete is in an odd or even row.
								*/
								if (Cell_X(cell) & 0x01) {
									newcell = Adjacent_Cell((CELL)(cellptr->Cell_Number()), FACING_W);
								} else {
									newcell = Adjacent_Cell((CELL)(cellptr->Cell_Number()), FACING_E);
								}
								if (Map[newcell].Overlay != OVERLAY_CONCRETE) {
									Class->Create_And_Place(newcell);
								}

								/*
								**	The display attributes must be recalculated for all adjacent
								**	cells since their shape can be altered by the presence of
								**	concrete at this location.
								*/
								static FacingType _face[4] = {FACING_N, FACING_E, FACING_S, FACING_W};

								for (int index = 0; index < (sizeof(_face)/sizeof(_face[0])); index++) {
									cellptr->Adjacent_Cell(_face[index]).Concrete_Calc();
								}
							}
						}
					}
				}

				/*
				**	*****  Is this really needed?
				*/
				cellptr->Recalc_Attributes();
			}
			Delete_This();
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
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *   07/24/1995 JLB : Specifically forbid manual crates in multiplayer scenarios.              *
 *=============================================================================================*/
void OverlayClass::Read_INI(char *buffer)
{
	char	*tbuffer;
	int	len;				// Length of data in buffer.
	char	buf[128];

	len = strlen(buffer) + 2;
	tbuffer = buffer + len;

	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-len, buffer);
	while (*tbuffer != '\0') {
		CELL			cell;
		OverlayType	classid;

		cell = atoi(tbuffer);
		WWGetPrivateProfileString(INI_Name(), tbuffer, NULL, buf, sizeof(buf)-1, buffer);
		classid = OverlayTypeClass::From_Name(strtok(buf, ",\n\r"));

		/*
		**	Don't allow placement of crates in the multiplayer scenarios.
		*/
		if (classid != OVERLAY_NONE && (GameToPlay == GAME_NORMAL || !OverlayTypeClass::As_Reference(classid).IsCrate)) {

			/*
			**	Don't allow placement of overlays on the top or bottom rows of
			**	the map.
			*/
			if (cell >= MAP_CELL_W && cell <= MAP_CELL_TOTAL - MAP_CELL_W) {
				new OverlayClass(classid, cell);
			}
		}
		tbuffer += strlen(tbuffer)+1;
	}
}


/***********************************************************************************************
 * OverlayClass::Write_INI -- Writes the overlay data to an INI file.                          *
 *                                                                                             *
 *    This is used to output the overlay data to a scenario INI file. Typically, this is       *
 *    only used by the scenario editor.                                                        *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the INI file staging buffer.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void OverlayClass::Write_INI(char *buffer)
{
	int	index;
	char	uname[10];
	char	buf[128];
	char	*tbuffer;		// Accumulation buffer of unit IDs.

	/*
	**	First, clear out all existing unit data from the ini file.
	*/
	tbuffer = buffer + strlen(buffer) + 2;
	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-strlen(buffer), buffer);
	while (*tbuffer != '\0') {
		WWWritePrivateProfileString(INI_Name(), tbuffer, NULL, buffer);
		tbuffer += strlen(tbuffer)+1;
	}

	/*
	**	Write the unit data out.
	*/
	for (index = 0; index < MAP_CELL_TOTAL; index++) {
		CellClass	* cellptr = &Map[index];

		if (cellptr->Overlay != OVERLAY_NONE) {
			sprintf(uname, "%03d", index);
			sprintf(buf, "%s", OverlayTypeClass::As_Reference(cellptr->Overlay).IniName);
			WWWritePrivateProfileString(INI_Name(), uname, buf, buffer);
		}
	}
}