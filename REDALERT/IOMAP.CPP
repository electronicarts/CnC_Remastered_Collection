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

/* $Header: /CounterStrike/IOMAP.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : IOMAP.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Bill Randolph                                                *
 *                                                                                             *
 *                   Start Date : January 16, 1995                                             *
 *                                                                                             *
 *                  Last Update : March 12, 1996 [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * All map-related loading/saving routines should go in this module, so it can be overlayed.   *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CellClass::Code_Pointers -- codes class's pointers for load/save                          *
 *   CellClass::Decode_Pointers -- decodes pointers for load/save                              *
 *   CellClass::Load -- Reads from a save game file.                                           *
 *   CellClass::Save -- Write to a save game file.                                             *
 *   CellClass::Should_Save -- Should the cell be written to disk?                             *
 *   DisplayClass::Code_Pointers -- codes class's pointers for load/save                       *
 *   DisplayClass::Decode_Pointers -- decodes pointers for load/save                           *
 *   MapClass::Code_Pointers -- codes class's pointers for load/save                           *
 *   MapClass::Decode_Pointers -- decodes pointers for load/save                               *
 *   MouseClass::Load -- Loads from a save game file.                                          *
 *   MouseClass::Save -- Saves to a save game file.                                            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


/***********************************************************************************************
 * CellClass::Should_Save -- Should the cell be written to disk?                               *
 *                                                                                             *
 *    This function will determine if the cell needs to be written to disk. Any cell that      *
 *    contains special data should be written to disk.                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Should this cell's data be written to disk?                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CellClass::Should_Save(void) const
{
	static CellClass const _identity_cell;

	return(memcmp(&_identity_cell, this, sizeof(*this)) != 0);
}


/***********************************************************************************************
 * CellClass::Load -- Loads from a save game file.                                             *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CellClass::Load(Straw & file)
{
	file.Get(this, sizeof(*this));
	return(true);
}


/***********************************************************************************************
 * CellClass::Save -- Write to a save game file.                                               *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CellClass::Save(Pipe & file) const
{
	file.Put(this, sizeof(*this));
	return(true);
}


/***********************************************************************************************
 * CellClass::Code_Pointers -- codes class's pointers for load/save                            *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void CellClass::Code_Pointers(void)
{
	if (Cell_Occupier() != NULL) {
		OccupierPtr = (ObjectClass *)OccupierPtr->As_Target();
	}

	for (int index = 0; index < ARRAY_SIZE(Overlapper); index++) {
		if (Overlapper[index] != NULL && Overlapper[index]->IsActive) {
			Overlapper[index] = (ObjectClass *)Overlapper[index]->As_Target();
		} else {
			Overlapper[index] = NULL;
		}
	}
}


/***********************************************************************************************
 * CellClass::Decode_Pointers -- decodes pointers for load/save                                *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/02/1995 BR : Created.                                                                  *
 *   03/12/1996 JLB : Simplified.                                                              *
 *=============================================================================================*/
void CellClass::Decode_Pointers(void)
{
	if (OccupierPtr != NULL) {
		OccupierPtr = As_Object((TARGET)OccupierPtr, false);
		assert(OccupierPtr != NULL);
	}

	for (int index = 0; index < ARRAY_SIZE(Overlapper); index++) {
		if (Overlapper[index] != NULL) {
			Overlapper[index] = As_Object((TARGET)Overlapper[index], false);
			assert(Overlapper[index] != NULL);
		}
	}
}


/***********************************************************************************************
 * MouseClass::Load -- Loads from a save game file.                                            *
 *                                                                                             *
 * Loading the map is very complicated.  Here are the steps:                                   *
 * - Read the Theater for this save-game                                                       *
 * - call Init_Theater to perform theater-specific inits                                       *
 * - call Free_Cells to free the cell array, because loading the map object will overwrite     *
 *   the pointer to the cell array                                                             *
 * - read the map object from disk                                                             *
 * - call Alloc_Cells to re-allocate the cell array                                            *
 * - call Init_Cells to set the cells to a known state, because not every cell will be loaded  *
 * - read the cell objects into the cell array                                                 *
 * - After the map & all objects have been loaded & the pointers decoded, Init_IO() >MUST< be  *
 *   called to restore the map's button list to the proper state.                              *
 *                                                                                             *
 * INPUT:   file  -- The file to read the cell's data from.                                    *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *   03/12/1996 JLB : Simplified.                                                              *
 *=============================================================================================*/
bool MouseClass::Load(Straw & file)
{
	/*
	**	Load Theater:  Even though this value is located in the DisplayClass,
	**	it must be loaded first so initialization can be done before any other
	**	map data is loaded.  If initialization isn't done first, data read from
	**	disk will be over-written when initialization occurs.  This code must
	**	go in the most-derived Map class.
	*/
	TheaterType theater;
	if (file.Get(&theater, sizeof(theater)) != sizeof(theater)) {
		return(false);
	}

#ifdef WIN32
LastTheater = THEATER_NONE;
#endif

	/*
	** Remove any old theater specific uncompressed shapes
	*/
#ifdef WIN32
//	if (theater != LastTheater) {
		Reset_Theater_Shapes();
//	}
#endif	//WIN32

	/*
	**	Init display mixfiles
	*/
	Init_Theater(theater);
	TerrainTypeClass::Init(Scen.Theater);
	TemplateTypeClass::Init(Scen.Theater);
	OverlayTypeClass::Init(Scen.Theater);
	UnitTypeClass::Init(Scen.Theater);
	InfantryTypeClass::Init(Scen.Theater);
	BuildingTypeClass::Init(Scen.Theater);
	BulletTypeClass::Init(Scen.Theater);
	AnimTypeClass::Init(Scen.Theater);
	AircraftTypeClass::Init(Scen.Theater);
	VesselTypeClass::Init(Scen.Theater);
	SmudgeTypeClass::Init(Scen.Theater);

	//LastTheater = Scen.Theater;

	/*
	** Free the cell array, because we're about to overwrite its pointers
	*/
	Free_Cells();

	/*
	** Read the entire map object in.  Only read in sizeof(MouseClass), so if we're
	** in editor mode, none of the map editor object is read in.
	*/
	file.Get(this, sizeof(*this));
#ifdef SCENARIO_EDITOR
	new(this) MapEditClass(NoInitClass());
#else
	new(this) MouseClass(NoInitClass());
#endif

	/*
	** Reallocate the cell array
	*/
	Alloc_Cells();

	/*
	** Init all cells to empty
	*/
	Init_Cells();

	/*
	**	Read # cells saved
	*/
	int count;
	if (file.Get(&count, sizeof(count)) != sizeof(count)) {
		return(false);
	}

	/*
	**	Read cells
	*/
	for (int index = 0; index < count; index++) {
		CELL cell = 0;
		if (file.Get(&cell, sizeof(cell)) != sizeof(cell)) {
			return(false);
		}

		if (!(*this)[cell].Load(file)) {
			return(false);
		}
	}

	LastTheater = Scen.Theater;
	return(true);
}


/***********************************************************************************************
 * MouseClass::Save -- Save to a save game file.                                               *
 *                                                                                             *
 * INPUT:   file  -- The file to write the cell's data to.                                     *
 *                                                                                             *
 * OUTPUT:  true = success, false = failure                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *   02/26/1996 JLB : Cleaned up.                                                              *
 *=============================================================================================*/
bool MouseClass::Save(Pipe & file) const
{
	/*
	**	Save Theater >first<
	*/
	TheaterType theater = Scen.Theater;
	file.Put(&theater, sizeof(theater));

	file.Put(this, sizeof(*this));

	/*
	**	Count how many cells will be saved.
	*/
	int count = 0;
	CellClass const * cellptr = &(*this)[(CELL)0];
	for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		if (cellptr->Should_Save()) {
			count++;
		}
		cellptr++;
	}

	/*
	**	write out count of the cells.
	*/
	file.Put(&count, sizeof(count));

	/*
	**	Save cells that need it
	*/
	cellptr = &(*this)[(CELL)0];
	for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		if (cellptr->Should_Save()) {
			file.Put(&cell, sizeof(cell));
			cellptr->Save(file);
			count--;
		}
		cellptr++;
	}

	if (count != 0) return(false);

	return(true);
}


/***********************************************************************************************
 * DisplayClass::Code_Pointers -- codes class's pointers for load/save                         *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void DisplayClass::Code_Pointers(void)
{
	/*
	**	Code PendingObjectPtr.
	*/
	if (PendingObjectPtr) {
		PendingObjectPtr = (ObjectClass *)PendingObjectPtr->As_Target();
	}

	/*
	** Fix for saving game while in structure placement mode. ST - 4/15/2020 2:41PM
	*/
	memset(CursorShapeSave, 0, sizeof(CursorShapeSave));
	if (CursorSize && CursorSize != CursorShapeSave) {

		int save_buffer_element_size = sizeof(CursorShapeSave) / sizeof(CursorShapeSave[0]);
		
		int index = 0;

		while (index < save_buffer_element_size - 2 && CursorSize[index] != REFRESH_EOL) {
			CursorShapeSave[index] = CursorSize[index];
			index++;
		}
		CursorShapeSave[index] = REFRESH_EOL;
	}

	/*
	**	Chain to parent.
	*/
	MapClass::Code_Pointers();
}


/***********************************************************************************************
 * DisplayClass::Decode_Pointers -- decodes pointers for load/save                             *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void DisplayClass::Decode_Pointers(void)
{
	/*
	**	Decode PendingObjectPtr.  We can't decode PendingObject here, because we'd
	**	have to reference PendingObjectPtr->Class_Of(), and the object that
	**	PendingObjectPtr is pointing to hasn't been decoded yet.  Since we can't
	**	decode PendingObjectPtr, we can't set the placement cursor shape here
	**	either.  These have to be done as last-minute fixups.
	*/
	if (PendingObjectPtr) {
		PendingObjectPtr = As_Object((TARGET)PendingObjectPtr, false);
		assert(PendingObjectPtr != NULL);
	}

	if (CursorSize) {
		CursorSize = CursorShapeSave;
	}

	/*
	**	Chain to parent.
	*/
	MapClass::Decode_Pointers();
}


/***********************************************************************************************
 * MapClass::Code_Pointers -- codes class's pointers for load/save                             *
 *                                                                                             *
 * This routine "codes" the pointers in the class by converting them to a number               *
 * that still represents the object pointed to, but isn't actually a pointer.  This            *
 * allows a saved game to properly load without relying on the games data still                *
 * being in the exact same location.                                                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void MapClass::Code_Pointers(void)
{
	CellClass * cellptr = &(*this)[(CELL)0];
	for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		cellptr->Code_Pointers();
		cellptr++;
	}
}


/***********************************************************************************************
 * MapClass::Decode_Pointers -- decodes pointers for load/save                                 *
 *                                                                                             *
 * This routine "decodes" the pointers coded in Code_Pointers by converting the                *
 * code values back into object pointers.                                                      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/02/1995 BR : Created.                                                                  *
 *=============================================================================================*/
void MapClass::Decode_Pointers(void)
{
	CellClass * cellptr = &(*this)[(CELL)0];
	for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		cellptr->Decode_Pointers();
		cellptr++;
	}
}

