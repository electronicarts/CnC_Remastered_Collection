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

/* $Header: /CounterStrike/SMUDGE.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SMUDGE.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : August 9, 1994                                               *
 *                                                                                             *
 *                  Last Update : July 3, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   SmudgeClass::Disown -- Disowns (removes) a building bib piece.                            *
 *   SmudgeClass::Init -- Initialize the smudge tracking system.                               *
 *   SmudgeClass::Mark -- Marks a smudge down on the map.                                      *
 *   SmudgeClass::Read_INI -- Reads smudge data from an INI file.                              *
 *   SmudgeClass::SmudgeClass -- Constructor for smudge objects.                               *
 *   SmudgeClass::Write_INI -- Store all the smudge data to the INI database.                  *
 *   SmudgeClass::operator delete -- Deletes the smudge from the tracking system.              *
 *   SmudgeClass::operator new -- Creator of smudge objects.                                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"smudge.h"



HousesType SmudgeClass::ToOwn = HOUSE_NONE;


/***********************************************************************************************
 * SmudgeClass::operator new -- Creator of smudge objects.                                     *
 *                                                                                             *
 *    This routine will allocate a smudge object from the smudge tracking pool.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a newly allocated smudge object. If one couldn't be      *
 *          found, then NULL is returned.                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void * SmudgeClass::operator new(size_t )
{
	void * ptr = Smudges.Allocate();
	if (ptr != NULL) {
		((SmudgeClass *)ptr)->Set_Active();
	}
	return(ptr);
}


/***********************************************************************************************
 * SmudgeClass::operator delete -- Deletes the smudge from the tracking system.                *
 *                                                                                             *
 *    This routine is used to remove the smudge from the tracking system.                      *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the smudge to delete.                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SmudgeClass::operator delete(void * ptr)
{
	if (ptr != NULL) {
		((SmudgeClass *)ptr)->IsActive = false;
	}
	Smudges.Free((SmudgeClass *)ptr);
}


/***********************************************************************************************
 * SmudgeClass::SmudgeClass -- Constructor for smudge objects.                                 *
 *                                                                                             *
 *    This is the typical constructor for smudge objects. If the position to place the         *
 *    smudge is not given, then the smudge will be initialized in a limbo state. If the        *
 *    smudge is placed on the map, then this operation causes the smudge object itself to be   *
 *    deleted and special map values updated to reflect the presence of a smudge.              *
 *                                                                                             *
 * INPUT:   type  -- The type of smudge to construct.                                          *
 *                                                                                             *
 *          pos   -- The position to place the smudge. If -1, then the smudge is initialized   *
 *                   into a limbo state.                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
SmudgeClass::SmudgeClass(SmudgeType type, COORDINATE pos, HousesType house) :
	ObjectClass(RTTI_SMUDGE, Smudges.ID(this)),
	Class(SmudgeTypes.Ptr((int)type))
{
	if (pos != -1) {
		ToOwn = house;
		if (!Unlimbo(pos)) {
			delete this;
		} else {
			ToOwn = HOUSE_NONE;
		}
	}
}


/***********************************************************************************************
 * SmudgeClass::Init -- Initialize the smudge tracking system.                                 *
 *                                                                                             *
 *    This routine is used during the scenario clearing process to initialize the smudge       *
 *    object tracking system to a null state.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SmudgeClass::Init(void)
{
	Smudges.Free_All();
}


/***********************************************************************************************
 * SmudgeClass::Mark -- Marks a smudge down on the map.                                        *
 *                                                                                             *
 *    This routine will place the smudge on the map. If the map cell allows.                   *
 *                                                                                             *
 * INPUT:   mark  -- The type of marking to perform. Only MARK_DOWN is supported.              *
 *                                                                                             *
 * OUTPUT:  bool; Was the smudge marked successfully? Failure occurs if the smudge isn't       *
 *                marked DOWN.                                                                 *
 *                                                                                             *
 * WARNINGS:   The smudge object is DELETED by this routine.                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1994 JLB : Created.                                                                 *
 *   12/23/1994 JLB : Checks low level legality before proceeding.                             *
 *=============================================================================================*/
bool SmudgeClass::Mark(MarkType mark)
{
	assert(Smudges.ID(this) == ID);
	assert(IsActive);

	if (ObjectClass::Mark(mark)) {
		if (mark == MARK_DOWN) {
			CELL origin = Coord_Cell(Coord);

			for (int w = 0; w < Class->Width; w++) {
				for (int h = 0; h < Class->Height; h++) {
					CELL newcell = origin + w + (h*MAP_CELL_W);
					if (Map.In_Radar(newcell)) {
						CellClass * cell = &Map[newcell];

						if (Class->IsBib) {
							cell->Smudge = Class->Type;
							cell->SmudgeData = w + (h*Class->Width);
							cell->Owner = ToOwn;
						} else {
							if (cell->Is_Clear_To_Move(SPEED_TRACK, true, true)) {
								if (Class->IsCrater && cell->Smudge != SMUDGE_NONE && SmudgeTypeClass::As_Reference(cell->Smudge).IsCrater) {
									cell->SmudgeData++;
									cell->SmudgeData = (int)min((int)cell->SmudgeData, (int)4);
								}

								if (cell->Smudge == SMUDGE_NONE) {

									/*
									**	Special selection of a crater that starts as close to the
									**	specified coordinate as possible.
									*/
									if (Class->IsCrater) {
										cell->Smudge = (SmudgeType)(SMUDGE_CRATER1 + CellClass::Spot_Index(Coord));
									} else {
										cell->Smudge = Class->Type;
									}
									cell->SmudgeData = 0;
								}
							}
						}

						/*
						**	Flag everything that might be overlapping this cell to redraw itself.
						*/
						cell->Redraw_Objects();
					}
				}
			}

			/*
			**	Whether it was successful in placing, or not, delete the smudge object. It isn't
			**	needed once the map has been updated with the proper smudge data. Fake this object
			**	as if it were never placed down!
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
 * SmudgeClass::Disown -- Disowns (removes) a building bib piece.                              *
 *                                                                                             *
 *    This routine is used when a building is removed from the game. If there was any bib      *
 *    attached, this routine will be called to disown the cells and remove the bib artwork.    *
 *                                                                                             *
 * INPUT:   cell  -- The origin cell for this bib removal.                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This is actually working on a temporary bib object. It is created for the sole  *
 *             purpose of calling this routine. It will be deleted immediately afterward.      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void SmudgeClass::Disown(CELL cell)
{
	assert(Smudges.ID(this) == ID);
	assert(IsActive);

	if (Class->IsBib) {
		for (int w = 0; w < Class->Width; w++) {
			for (int h = 0; h < Class->Height; h++) {
				CellClass & cellptr = Map[(CELL)(cell + w + (h*MAP_CELL_W))];

				if (cellptr.Overlay == OVERLAY_NONE || !OverlayTypeClass::As_Reference(cellptr.Overlay).IsWall) {
					cellptr.Smudge = SMUDGE_NONE;
					cellptr.SmudgeData = 0;
					if (!cellptr.IsFlagged) {
						cellptr.Owner = HOUSE_NONE;
					}
					cellptr.Redraw_Objects();
				}
			}
		}
	}
}


/***********************************************************************************************
 * SmudgeClass::Read_INI -- Reads smudge data from an INI file.                                *
 *                                                                                             *
 *    This routine is used by the scenario loader to read the smudge data in an INI file and   *
 *    create the appropriate smudge objects on the map.                                        *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the INI file staging buffer.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *   07/24/1995 JLB : Sets the smudge data value as well.                                      *
 *=============================================================================================*/
void SmudgeClass::Read_INI(CCINIClass & ini)
{
	char	buf[128];	// Working string staging buffer.

	int len = ini.Entry_Count(INI_Name());
	for (int index = 0; index < len; index++) {
		char const * entry = ini.Get_Entry(INI_Name(), index);
		SmudgeType	smudge;		// Smudge type.

		ini.Get_String(INI_Name(), entry, NULL, buf, sizeof(buf));
		smudge = SmudgeTypeClass::From_Name(strtok(buf, ","));
		if (smudge != SMUDGE_NONE) {
			char * ptr = strtok(NULL, ",");
			if (ptr != NULL) {
				int data = 0;
				CELL cell = atoi(ptr);
				ptr = strtok(NULL, ",");
				if (ptr != NULL) data = atoi(ptr);
				new SmudgeClass(smudge, Cell_Coord(cell));
				if (Map[cell].Smudge == smudge && data != 0) {
					Map[cell].SmudgeData = data;
				}
			}
		}
	}
}


/***********************************************************************************************
 * SmudgeClass::Write_INI -- Store all the smudge data to the INI database.                    *
 *                                                                                             *
 *    This routine will output all the smudge data to the INI database.                        *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database object.                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void SmudgeClass::Write_INI(CCINIClass & ini)
{
	/*
	**	First, clear out all existing template data from the ini file.
	*/
	ini.Clear(INI_Name());

	/*
	**	Find all templates and write them to the file.
	*/
	for (CELL index = 0; index < MAP_CELL_TOTAL; index++) {
		CellClass * ptr;

		ptr = &Map[index];
		if (ptr->Smudge != SMUDGE_NONE) {
			SmudgeTypeClass const * stype = &SmudgeTypeClass::As_Reference(ptr->Smudge);
			if (!stype->IsBib) {
				char	uname[10];
				char	buf[127];

				sprintf(uname, "%d", index);
				sprintf(buf, "%s,%d,%d", stype->IniName, index, ptr->SmudgeData);
				ini.Put_String(INI_Name(), uname, buf);
			}
		}
	}
}