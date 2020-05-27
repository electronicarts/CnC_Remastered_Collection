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

/* $Header:   F:\projects\c&c\vcs\code\iomap.cpv   2.18   16 Oct 1995 16:50:34   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : January 16, 1995   [BR]                                      *
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
 *   GScreenClass::Code_Pointers -- codes class's pointers for load/save                       *
 *   GScreenClass::Decode_Pointers -- decodes pointers for load/save                           *
 *   HelpClass::Code_Pointers -- codes class's pointers for load/save                          *
 *   HelpClass::Decode_Pointers -- decodes pointers for load/save                              *
 *   MapClass::Code_Pointers -- codes class's pointers for load/save                           *
 *   MapClass::Decode_Pointers -- decodes pointers for load/save                               *
 *   MouseClass::Code_Pointers -- codes class's pointers for load/save                         *
 *   MouseClass::Decode_Pointers -- decodes pointers for load/save                             *
 *   MouseClass::Load -- Loads from a save game file.                                          *
 *   MouseClass::Save -- Saves to a save game file.                                            *
 *   PowerClass::Code_Pointers -- codes class's pointers for load/save                         *
 *   PowerClass::Decode_Pointers -- decodes pointers for load/save                             *
 *   RadarClass::Code_Pointers -- codes class's pointers for load/save                         *
 *   RadarClass::Decode_Pointers -- decodes pointers for load/save                             *
 *   ScrollClass::Code_Pointers -- codes class's pointers for load/save                        *
 *   ScrollClass::Decode_Pointers -- decodes pointers for load/save                            *
 *   SidebarClass::Code_Pointers -- codes class's pointers for load/save                       *
 *   SidebarClass::Decode_Pointers -- decodes pointers for load/save                           *
 *   SidebarClass::StripClass::Code_Pointers -- codes class's pointers for load/save           *
 *   SidebarClass::StripClass::Decode_Pointers -- decodes pointers for load/save               *
 *   TabClass::Code_Pointers -- codes class's pointers for load/save                           *
 *   TabClass::Decode_Pointers -- decodes pointers for load/save                               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

#pragma warning (disable : 4302)			// Truncation from pointer to TARGET

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
	return(
			(Smudge != SMUDGE_NONE) ||
			(TType != TEMPLATE_NONE) ||
			(Overlay != OVERLAY_NONE) ||
			IsMapped ||
			IsVisible ||
			IsMappedByPlayerMask ||
			IsVisibleByPlayerMask ||
			IsTrigger ||
			Flag.Composite ||
			OccupierPtr ||
			Overlapper[0] || Overlapper[1] || Overlapper[2]
			);
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
bool CellClass::Load(FileClass & file)
{
	int rc;
	TriggerClass * trig;

	/*
	-------------------------- Load the object data --------------------------
	*/
	rc = Read_Object(this, sizeof(CellClass), file, false);

	/*
	------------------------ Load the trigger pointer ------------------------
	*/
	if (rc) {
		if (IsTrigger) {
			if (file.Read(&trig,sizeof(trig)) != sizeof(trig))
				return(false);
			CellTriggers[Cell_Number()] = trig;
		}
	}

	return(rc);
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
bool CellClass::Save(FileClass & file)
{
	int rc;
	TriggerClass * trig;

	/*
	-------------------------- Save the object data --------------------------
	*/
	rc = Write_Object(this, sizeof(CellClass), file);

	/*
	------------------------ Save the trigger pointer ------------------------
	*/
	if (rc) {
		if (IsTrigger) {
			trig = CellTriggers[Cell_Number()];
			if (file.Write(&trig,sizeof(trig)) != sizeof(trig))
				return(false);
		}
	}

	return(rc);
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
	if (Cell_Occupier()) {
		OccupierPtr = (ObjectClass *)OccupierPtr->As_Target();
	}

	if (Overlapper[0]) {
		Overlapper[0] = (ObjectClass *)Overlapper[0]->As_Target();
	}

	if (Overlapper[1]) {
		Overlapper[1] = (ObjectClass *)Overlapper[1]->As_Target();
	}

	if (Overlapper[2]) {
		Overlapper[2] = (ObjectClass *)Overlapper[2]->As_Target();
	}

	/*
	------------------------ Convert trigger pointer -------------------------
	*/
	if (IsTrigger) {
		CellTriggers[Cell_Number()] = (TriggerClass *)CellTriggers[Cell_Number()]->As_Target();
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
 *=============================================================================================*/
void CellClass::Decode_Pointers(void)
{
	char bad[128];
	
	if (OccupierPtr) {
		OccupierPtr = As_Object((TARGET)OccupierPtr, false);
		Check_Ptr((void *)OccupierPtr,__FILE__,__LINE__);

		/*
		** Check for bad occupier that was saved. ST - 10/3/2019 11:50AM
		*/
		ObjectClass * optr = Cell_Occupier();
		if (optr->IsActive == false) {
			CellClass *cell0 = &Map[0];
			int cell_number = this - cell0;
			sprintf(bad, "Found bad cell occupier in cell %d", cell_number);
			GlyphX_Debug_Print(bad);
			OccupierPtr = NULL;
		}
	}

	if (Overlapper[0]) {
		Overlapper[0] = As_Object((TARGET)Overlapper[0], false);
		Check_Ptr((void *)Overlapper[0],__FILE__,__LINE__);
	}

	if (Overlapper[1]) {
		Overlapper[1] = As_Object((TARGET)Overlapper[1], false);
		Check_Ptr((void *)Overlapper[1],__FILE__,__LINE__);
	}

	if (Overlapper[2]) {
		Overlapper[2] = As_Object((TARGET)Overlapper[2], false);
		Check_Ptr((void *)Overlapper[2],__FILE__,__LINE__);
	}

	/*
	** Check for bad overlappers that were saved. ST - 10/3/2019 11:50AM
	*/
	for (int i=0 ; i <= 2 ; i++) {
		if (Overlapper[i]) {
			ObjectClass * optr = Overlapper[i];
			if (optr->IsActive == false) {
				CellClass *cell0 = &Map[0];
				int cell_number = this - cell0;
				sprintf(bad, "Found bad cell overlapper in slot %d of cell %d", i, cell_number);
				GlyphX_Debug_Print(bad);
				Overlapper[i] = NULL;
			}
		}
	}


	/*
	**	Convert trigger pointer.
	*/
	if (IsTrigger) {
		CellTriggers[Cell_Number()] = As_Trigger( (TARGET)CellTriggers[Cell_Number()], false );
		Check_Ptr((void *)CellTriggers[Cell_Number()],__FILE__,__LINE__);
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
 *=============================================================================================*/
bool MouseClass::Load(FileClass & file)
{
	unsigned count;
	CELL cell = 0;
	int index;
//	int rc;
//	int i;
//	int j;

	/*------------------------------------------------------------------------
	Load Theater:  Even though this value is located in the DisplayClass,
	it must be loaded first so initialization can be done before any other
	map data is loaded.  If initialization isn't done first, data read from
	disk will be over-written when initialization occurs.  This code must
	go in the most-derived Map class.
	------------------------------------------------------------------------*/
	if (file.Read (&Theater,sizeof(Theater)) != sizeof(Theater))
		return(false);

	/*
	** Remove any old theater specific uncompressed shapes
	*/
	if (Theater != LastTheater){
		Reset_Theater_Shapes();
	}

	/*
	------------------------- Init display mixfiles --------------------------
	*/
	Init_Theater(Theater);
	TerrainTypeClass::Init(Theater);
	TemplateTypeClass::Init(Theater);
	OverlayTypeClass::Init(Theater);
	UnitTypeClass::Init(Theater);
	InfantryTypeClass::Init(Theater);
	BuildingTypeClass::Init(Theater);
	BulletTypeClass::Init(Theater);
	AnimTypeClass::Init(Theater);
	AircraftTypeClass::Init(Theater);
	SmudgeTypeClass::Init(Theater);

	LastTheater = Theater;

	/*
	** Free the cell array, because we're about to overwrite its pointers
	*/
	Free_Cells();

	/*
	** Read the entire map object in.  Only read in sizeof(MouseClass), so if we're
	** in editor mode, none of the map editor object is read in.
	*/
	if (!Read_Object(this, sizeof(MouseClass), file, true)) {
		return(false);
	}

	/*
	** Reallocate the cell array
	*/
	Alloc_Cells();

	/*
	** Init all cells to empty
	*/
	Init_Cells();

	/*
	--------------------------- Read # cells saved ---------------------------
	*/
	if (file.Read(&count, sizeof(count)) != sizeof(count)) {
		return(false);
	}

	/*
	------------------------------- Read cells -------------------------------
	*/
	for (index = 0; index < (int)count; index++) {
		if (file.Read(&cell, sizeof(cell)) != sizeof(cell))
			return(false);

		if (!(*this)[cell].Load(file))
			return(false);
	}

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
 *=============================================================================================*/
bool MouseClass::Save(FileClass & file)
{
	unsigned count;
	long pos;

	/*
	-------------------------- Save Theater >first< --------------------------
	*/
	if (file.Write (&Theater,sizeof(Theater)) != sizeof(Theater))
		return(false);

	if (!Write_Object(this, sizeof(MouseClass), file))
		return(false);

	/*
	---------------------- Record current file position ----------------------
	*/
	pos = file.Seek(0, SEEK_CUR);

	/*
	---------------------- write out placeholder bytes -----------------------
	*/
	if (file.Write(&count, sizeof(count)) != sizeof(count))
		return(false);

	/*
	------------------------ Save cells that need it -------------------------
	*/
	count = 0;
	for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		
		if ((*this)[cell].Should_Save()) {
			if (file.Write(&cell, sizeof(cell)) != sizeof(cell))
				return(false);

			count++;

			if (!(*this)[cell].Save(file))
				return(false);
		}
	}

	/*
	-------------------------- Save # cells written --------------------------
	*/
	file.Seek(pos, SEEK_SET);

	if (file.Write(&count, sizeof(count)) != sizeof(count))
		return(false);

	file.Seek(0, SEEK_END);

	return(true);
}


/***********************************************************************************************
 * MouseClass::Code_Pointers -- codes class's pointers for load/save                           *
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
void MouseClass::Code_Pointers(void)
{
//	Control.Code_Pointers();

	ScrollClass::Code_Pointers();
}


/***********************************************************************************************
 * MouseClass::Decode_Pointers -- decodes pointers for load/save                               *
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
void MouseClass::Decode_Pointers(void)
{
//	Control.Decode_Pointers();

	ScrollClass::Decode_Pointers();
}


/***********************************************************************************************
 * ScrollClass::Code_Pointers -- codes class's pointers for load/save                          *
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
void ScrollClass::Code_Pointers(void)
{
	HelpClass::Code_Pointers();
}


/***********************************************************************************************
 * ScrollClass::Decode_Pointers -- decodes pointers for load/save                              *
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
void ScrollClass::Decode_Pointers(void)
{
	HelpClass::Decode_Pointers();
}


/***********************************************************************************************
 * HelpClass::Code_Pointers -- codes class's pointers for load/save                            *
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
void HelpClass::Code_Pointers(void)
{
	TabClass::Code_Pointers();
}


/***********************************************************************************************
 * HelpClass::Decode_Pointers -- decodes pointers for load/save                                *
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
void HelpClass::Decode_Pointers(void)
{
	TabClass::Decode_Pointers();
}


/***********************************************************************************************
 * TabClass::Code_Pointers -- codes class's pointers for load/save                             *
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
void TabClass::Code_Pointers(void)
{
	SidebarClass::Code_Pointers();
}


/***********************************************************************************************
 * TabClass::Decode_Pointers -- decodes pointers for load/save                                 *
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
void TabClass::Decode_Pointers(void)
{
	SidebarClass::Decode_Pointers();
}


/***********************************************************************************************
 * PowerClass::Code_Pointers -- codes class's pointers for load/save                           *
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
void PowerClass::Code_Pointers(void)
{
	RadarClass::Code_Pointers();
}


/***********************************************************************************************
 * PowerClass::Decode_Pointers -- decodes pointers for load/save                               *
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
void PowerClass::Decode_Pointers(void)
{
	RadarClass::Decode_Pointers();
}


/***********************************************************************************************
 * SidebarClass::Code_Pointers -- codes class's pointers for load/save                         *
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
void SidebarClass::Code_Pointers(void)
{
	for (int i = 0; i < COLUMNS; i++) {
		Column[i].Code_Pointers();
	}

	PowerClass::Code_Pointers();
}


/***********************************************************************************************
 * SidebarClass::Decode_Pointers -- decodes pointers for load/save                             *
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
void SidebarClass::Decode_Pointers(void)
{
	for (int i = 0; i < COLUMNS; i++) {
		Column[i].Decode_Pointers();
	}

	PowerClass::Decode_Pointers();
}


/***********************************************************************************************
 * SidebarClass::StripClass::Code_Pointers -- codes class's pointers for load/save             *
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
void SidebarClass::StripClass::Code_Pointers(void)
{
}


/***********************************************************************************************
 * SidebarClass::StripClass::Decode_Pointers -- decodes pointers for load/save                 *
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
void SidebarClass::StripClass::Decode_Pointers(void)
{
}


/***********************************************************************************************
 * RadarClass::Code_Pointers -- codes class's pointers for load/save                           *
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
void RadarClass::Code_Pointers(void)
{
	DisplayClass::Code_Pointers();
}


/***********************************************************************************************
 * RadarClass::Decode_Pointers -- decodes pointers for load/save                               *
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
void RadarClass::Decode_Pointers(void)
{
	DisplayClass::Decode_Pointers();
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
		Check_Ptr((void *)PendingObjectPtr,__FILE__,__LINE__);
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
	CELL cell;

	/*
	------------------------- Code the cell pointers -------------------------
	*/
	for (cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		(*this)[cell].Code_Pointers();
	}

	GScreenClass::Code_Pointers();
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
	CELL cell;

	/*
	------------------------ Decode the cell pointers ------------------------
	*/
	for (cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		(*this)[cell].Decode_Pointers();
	}

	GScreenClass::Decode_Pointers();
}


/***********************************************************************************************
 * GScreenClass::Code_Pointers -- codes class's pointers for load/save                         *
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
void GScreenClass::Code_Pointers(void)
{
}


/***********************************************************************************************
 * GScreenClass::Decode_Pointers -- decodes pointers for load/save                             *
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
void GScreenClass::Decode_Pointers(void)
{
}