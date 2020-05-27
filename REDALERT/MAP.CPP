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

/* $Header: /CounterStrike/MAP.CPP 3     3/14/97 5:15p Joe_b $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MAP.CPP                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : October 5, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   MapClass::Base_Region -- Finds the owner and base zone for specified cell.                *
 *   MapClass::Cell_Region -- Determines the region from a specified cell number.              *
 *   MapClass::Cell_Threat -- Gets a houses threat value for a cell                            *
 *   MapClass::Close_Object -- Finds a clickable close object to the specified coordinate.     *
 *   MapClass::Destroy_Bridge_At -- Destroyes the bridge at location specified.                *
 *   MapClass::Detach -- Remove specified object from map references.                          *
 *   MapClass::In_Radar -- Is specified cell in the radar map?                                 *
 *   MapClass::Init -- clears all cells                                                        *
 *   MapClass::Intact_Bridge_Count -- Determine the number of intact bridges.                  *
 *   MapClass::Logic -- Handles map related logic functions.                                   *
 *   MapClass::Nearby_Location -- Finds a generally clear location near a specified cell.      *
 *   MapClass::One_Time -- Performs special one time initializations for the map.              *
 *   MapClass::Overlap_Down -- computes & marks object's overlap cells                         *
 *   MapClass::Overlap_Up -- Computes & clears object's overlap cells                          *
 *   MapClass::Overpass -- Performs any final cleanup to a freshly constructed map.            *
 *   MapClass::Pick_Up -- Removes specified object from the map.                               *
 *   MapClass::Place_Down -- Places the specified object onto the map.                         *
 *   MapClass::Place_Random_Crate -- Places a crate at random location on map.                 *
 *   MapClass::Read_Binary -- Reads the binary data from the straw specified.                  *
 *   MapClass::Remove_Crate -- Remove a crate from the specified cell.                         *
 *   MapClass::Set_Map_Dimensions -- Initialize the map.                                       *
 *   MapClass::Sight_From -- Mark as visible the cells within a specified radius.              *
 *   MapClass::Validate -- validates every cell on the map                                     *
 *   MapClass::Write_Binary -- Pipes the map template data to the destination specified.       *
 *   MapClass::Zone_Reset -- Resets all zone numbers to match the map.                         *
 *   MapClass::Zone_Span -- Flood fills the specified zone from the cell origin.               *
 *   MapClass::Pick_Random_Location -- Picks a random location on the map.                     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

#define	MCW	MAP_CELL_W
int const MapClass::RadiusOffset[] = {
	/* 0  */	0,
	/* 1  */	(-MCW*1)-1,(-MCW*1)+0,(-MCW*1)+1,-1,1,(MCW*1)-1,(MCW*1)+0,(MCW*1)+1,
	/* 2  */	(-MCW*2)-1,(-MCW*2)+0,(-MCW*2)+1,(-MCW*1)-2,(-MCW*1)+2,-2,2,(MCW*1)-2,(MCW*1)+2,(MCW*2)-1,(MCW*2)+0,(MCW*2)+1,
	/* 3  */	(-MCW*3)-1,(-MCW*3)+0,(-MCW*3)+1,(-MCW*2)-2,(-MCW*2)+2,(-MCW*1)-3,(-MCW*1)+3,-3,3,(MCW*1)-3,(MCW*1)+3,(MCW*2)-2,(MCW*2)+2,(MCW*3)-1,(MCW*3)+0,(MCW*3)+1,
	/* 4  */	(-MCW*4)-1,(-MCW*4)+0,(-MCW*4)+1,(-MCW*3)-3,(-MCW*3)-2,(-MCW*3)+2,(-MCW*3)+3,(-MCW*2)-3,(-MCW*2)+3,(-MCW*1)-4,(-MCW*1)+4,-4,4,(MCW*1)-4,(MCW*1)+4,(MCW*2)-3,(MCW*2)+3,(MCW*3)-3,(MCW*3)-2,(MCW*3)+2,(MCW*3)+3,(MCW*4)-1,(MCW*4)+0,(MCW*4)+1,
	/* 5  */	(-MCW*5)-1,(-MCW*5)+0,(-MCW*5)+1,(-MCW*4)-3,(-MCW*4)-2,(-MCW*4)+2,(-MCW*4)+3,(-MCW*3)-4,(-MCW*3)+4,(-MCW*2)-4,(-MCW*2)+4,(-MCW*1)-5,(-MCW*1)+5,-5,5,(MCW*1)-5,(MCW*1)+5,(MCW*2)-4,(MCW*2)+4,(MCW*3)-4,(MCW*3)+4,(MCW*4)-3,(MCW*4)-2,(MCW*4)+2,(MCW*4)+3,(MCW*5)-1,(MCW*5)+0,(MCW*5)+1,
	/* 6  */	(-MCW*6)-1,(-MCW*6)+0,(-MCW*6)+1,(-MCW*5)-3,(-MCW*5)-2,(-MCW*5)+2,(-MCW*5)+3,(-MCW*4)-4,(-MCW*4)+4,(-MCW*3)-5,(-MCW*3)+5,(-MCW*2)-5,(-MCW*2)+5,(-MCW*1)-6,(-MCW*1)+6,-6,6,(MCW*1)-6,(MCW*1)+6,(MCW*2)-5,(MCW*2)+5,(MCW*3)-5,(MCW*3)+5,(MCW*4)-4,(MCW*4)+4,(MCW*5)-3,(MCW*5)-2,(MCW*5)+2,(MCW*5)+3,(MCW*6)-1,(MCW*6)+0,(MCW*6)+1,
	/* 7  */	(-MCW*7)-1,(-MCW*7)+0,(-MCW*7)+1,(-MCW*6)-3,(-MCW*6)-2,(-MCW*6)+2,(-MCW*6)+3,(-MCW*5)-5,(-MCW*5)-4,(-MCW*5)+4,(-MCW*5)+5,(-MCW*4)-5,(-MCW*4)+5,(-MCW*3)-6,(-MCW*3)+6,(-MCW*2)-6,(-MCW*2)+6,(-MCW*1)-7,(-MCW*1)+7,-7,7,(MCW*1)-7,(MCW*1)+7,(MCW*2)-6,(MCW*2)+6,(MCW*3)-6,(MCW*3)+6,(MCW*4)-5,(MCW*4)+5,(MCW*5)-5,(MCW*5)-4,(MCW*5)+4,(MCW*5)+5,(MCW*6)-3,(MCW*6)-2,(MCW*6)+2,(MCW*6)+3,(MCW*7)-1,(MCW*7)+0,(MCW*7)+1,
	/* 8  */	(-MCW*8)-1,(-MCW*8)+0,(-MCW*8)+1,(-MCW*7)-3,(-MCW*7)-2,(-MCW*7)+2,(-MCW*7)+3,(-MCW*6)-5,(-MCW*6)-4,(-MCW*6)+4,(-MCW*6)+5,(-MCW*5)-6,(-MCW*5)+6,(-MCW*4)-6,(-MCW*4)+6,(-MCW*3)-7,(-MCW*3)+7,(-MCW*2)-7,(-MCW*2)+7,(-MCW*1)-8,(-MCW*1)+8,-8,8,(MCW*1)-8,(MCW*1)+8,(MCW*2)-7,(MCW*2)+7,(MCW*3)-7,(MCW*3)+7,(MCW*4)-6,(MCW*4)+6,(MCW*5)-6,(MCW*5)+6,(MCW*6)-5,(MCW*6)-4,(MCW*6)+4,(MCW*6)+5,(MCW*7)-3,(MCW*7)-2,(MCW*7)+2,(MCW*7)+3,(MCW*8)-1,(MCW*8)+0,(MCW*8)+1,
	/* 9  */	(-MCW*9)-1,(-MCW*9)+0,(-MCW*9)+1,(-MCW*8)-3,(-MCW*8)-2,(-MCW*8)+2,(-MCW*8)+3,(-MCW*7)-5,(-MCW*7)-4,(-MCW*7)+4,(-MCW*7)+5,(-MCW*6)-6,(-MCW*6)+6,(-MCW*5)-7,(-MCW*5)+7,(-MCW*4)-7,(-MCW*4)+7,(-MCW*3)-8,(-MCW*3)+8,(-MCW*2)-8,(-MCW*2)+8,(-MCW*1)-9,(-MCW*1)+9,-9,9,(MCW*1)-9,(MCW*1)+9,(MCW*2)-8,(MCW*2)+8,(MCW*3)-8,(MCW*3)+8,(MCW*4)-7,(MCW*4)+7,(MCW*5)-7,(MCW*5)+7,(MCW*6)-6,(MCW*6)+6,(MCW*7)-5,(MCW*7)-4,(MCW*7)+4,(MCW*7)+5,(MCW*8)-3,(MCW*8)-2,(MCW*8)+2,(MCW*8)+3,(MCW*9)-1,(MCW*9)+0,(MCW*9)+1,
	/* 10 */	(-MCW*10)-1,(-MCW*10)+0,(-MCW*10)+1,(-MCW*9)-3,(-MCW*9)-2,(-MCW*9)+2,(-MCW*9)+3,(-MCW*8)-5,(-MCW*8)-4,(-MCW*8)+4,(-MCW*8)+5,(-MCW*7)-7,(-MCW*7)-6,(-MCW*7)+6,(-MCW*7)+7,(-MCW*6)-7,(-MCW*6)+7,(-MCW*5)-8,(-MCW*5)+8,(-MCW*4)-8,(-MCW*4)+8,(-MCW*3)-9,(-MCW*3)+9,(-MCW*2)-9,(-MCW*2)+9,(-MCW*1)-10,(-MCW*1)+10,-10,10,(MCW*1)-10,(MCW*1)+10,(MCW*2)-9,(MCW*2)+9,(MCW*3)-9,(MCW*3)+9,(MCW*4)-8,(MCW*4)+8,(MCW*5)-8,(MCW*5)+8,(MCW*6)-7,(MCW*6)+7,(MCW*7)-7,(MCW*7)-6,(MCW*7)+6,(MCW*7)+7,(MCW*8)-5,(MCW*8)-4,
			(MCW*8)+4,(MCW*8)+5,(MCW*9)-3,(MCW*9)-2,(MCW*9)+2,(MCW*9)+3,(MCW*10)-1,(MCW*10)+0,(MCW*10)+1,
};

int const MapClass::RadiusCount[11] = {1,9,21,37,61,89,121,161,205,253,309};


CellClass * BlubCell;

/***********************************************************************************************
 * MapClass::One_Time -- Performs special one time initializations for the map.                *
 *                                                                                             *
 *    This routine is used by the game initialization function in order to perform any one     *
 *    time initializations required for the map. This includes allocation of the map and       *
 *    setting up its default dimensions.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine MUST be called once and only once.                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   12/01/1994 BR : Added CellTriggers initialization                                         *
 *=============================================================================================*/
void MapClass::One_Time(void)
{
	GScreenClass::One_Time();

	XSize = MAP_CELL_W;
	YSize = MAP_CELL_H;
	Size = XSize * YSize;

	/*
	**	Allocate the cell array.
	*/
	Alloc_Cells();
}


/***********************************************************************************************
 * MapClass::Init_Clear -- clears the map & buffers to a known state                           *
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
 *   03/17/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Init_Clear(void)
{
	GScreenClass::Init_Clear();
	Init_Cells();
	TiberiumScan = 0;
	TiberiumGrowthCount = 0;
	TiberiumGrowthExcess = 0;
	TiberiumSpreadCount = 0;
	TiberiumSpreadExcess = 0;
	for (int index = 0; index < ARRAY_SIZE(Crates); index++) {
		Crates[index].Init();
	}
}


/***********************************************************************************************
 * MapClass::Alloc_Cells -- allocates the cell array                                           *
 *                                                                                             *
 * This routine should be called at One_Time, and after loading the Map object from a save     *
 * game, but prior to loading the cell objects.                                                *
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
 *   03/17/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Alloc_Cells(void)
{
	/*
	**	Assume that whatever the contents of the VectorClass are is garbage
	**	(it may have been loaded from a save-game file), so zero it out first.
	*/
	new (&Array) VectorClass<CellClass>;
	Array.Resize(Size);
}


/***********************************************************************************************
 * MapClass::Free_Cells -- frees the cell array                                                *
 *                                                                                             *
 * This routine is used by the Load_Game routine to free the map's cell array before loading   *
 * the map object from disk; the array is then re-allocated & cleared before the cell objects  *
 * are loaded.                                                                                 *
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
 *   03/17/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Free_Cells(void)
{
	Array.Clear();
}


/***********************************************************************************************
 * MapClass::Init_Cells -- Initializes the cell array to a fresh state.                        *
 *                                                                                             *
 * This routine is used by Init_Clear to set the cells to a known state; it's also used by     *
 * the Load_Game routine to init all cells before loading a set of cells from disk, so it      *
 * needs to be called separately from the other Init_xxx() routines.                           *
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
 *   03/17/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Init_Cells(void)
{
	TotalValue = 0;
	for (int index = 0; index < MAP_CELL_TOTAL; index++) {
		new (&Array[index]) CellClass;
	}
}


/***********************************************************************************************
 * MapClass::Set_Map_Dimensions -- Set map dimensions.                                         *
 *                                                                                             *
 *    This routine is used to set the legal limits and position of the                         *
 *    map as it relates to the overall map array. Typically, this is                           *
 *    called by the scenario loading code.                                                     *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y coordinate of the "upper left" corner                         *
 *                   of the map.                                                               *
 *                                                                                             *
 *          w,h   -- The width and height of the legal map.                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Set_Map_Dimensions(int x, int y, int w, int h)
{
	MapCellX = x;
	MapCellY = y;
	MapCellWidth = w;
	MapCellHeight = h;
}


/***********************************************************************************************
 * MapClass::Sight_From -- Mark as visible the cells within a specified radius.                *
 *                                                                                             *
 *    This routine is used to reveal the cells around a specific location.                     *
 *    Typically, as a unit moves or is deployed, this routine will be                          *
 *    called. Since it deals with MANY cells, it needs to be extremely                         *
 *    fast.                                                                                    *
 *                                                                                             *
 * INPUT:   cell     -- The coordinate that the sighting originates from.                      *
 *                                                                                             *
 *          sightrange-- The distance in cells that sighting extends.                          *
 *                                                                                             *
 *          incremental-- Is this an incremental sighting. In other                            *
 *                      words, has this function been called before where                      *
 *                      the center coordinate is no more than one cell                         *
 *                      distant from the last time?                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/19/1992 JLB : Created.                                                                 *
 *   03/08/1994 JLB : Updated to use sight table and incremental flag.                         *
 *   05/18/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
void MapClass::Sight_From(CELL cell, int sightrange, HouseClass * house, bool incremental)
{
	int xx;				// Center cell X coordinate (bounds checking).
	int const * ptr;	// Offset pointer.
	int count;			// Counter for number of offsets to process.

	/*
	**	Units that are off-map cannot sight.
	*/
	if (!In_Radar(cell)) return;
	if (!sightrange || sightrange > 10) return;

	/*
	**	Determine logical cell coordinate for center scan point.
	*/
	xx = Cell_X(cell);

	/*
	**	Incremental scans only scan the outer rings. Full scans
	**	scan all internal cells as well.
	*/
	count = RadiusCount[sightrange];
	ptr = &RadiusOffset[0];
	if (incremental) {
		if (sightrange > 2) {
			ptr += RadiusCount[sightrange-3];
			count -= RadiusCount[sightrange-3];
		}
	}

	/*
	**	Process all offsets required for the desired scan.
	*/
	while (count--) {
		CELL	newcell;			// New cell with offset.
		int	xdiff;			// New cell's X coordinate distance from center.

		newcell = cell + *ptr++;

		/*
		**	Determine if the map edge has been wrapped. If so,
		**	then don't process the cell.
		*/
		if ((unsigned)newcell >= MAP_CELL_TOTAL) continue;
		xdiff = Cell_X(newcell) - xx;
		xdiff = ABS(xdiff);
		if (xdiff > sightrange) continue;
		if (Distance(Cell_Coord(newcell), Cell_Coord(cell)) > (sightrange * CELL_LEPTON_W)) continue;

		/*
		**	Map the cell. For incremental scans, then update
		**	adjacent cells as well. For full scans, just update
		**	the cell itself.
		*/
		//if (!(*this)[newcell].IsMapped) {  // ST - 8/7/2019 10:31AM		
		Map.Map_Cell(newcell, house, true, true);
	}
}


/***********************************************************************************************
 * MapClass::Shroud_From -- cloak a radius of cells														  *
 *                                                                                             *
 *    This routine is used to shroud the cells around a specific location.                     *
 *    Typically, as a gap generator refreshes (when Encroach_Shadow() is called) this routine's*
 *    called. Since it deals with MANY cells, it needs to be extremely                         *
 *    fast.                                                                                    *
 *                                                                                             *
 * INPUT:   cell     -- The coordinate that the shrouding originates from.                     *
 *                                                                                             *
 *          sightrange-- The distance in cells that sighting extends.                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/10/1995 BWG : Created.                                                                 *
 *   08/09/2019  ST : Added house parameter                                                    *
 *=============================================================================================*/
void MapClass::Shroud_From(CELL cell, int sightrange, HouseClass *house)
{
	int xx;				// Center cell X coordinate (bounds checking).
	int const * ptr;	// Offset pointer.
	int count;			// Counter for number of offsets to process.

	/*
	**	Units that are off-map cannot sight.
	*/
	if (!In_Radar(cell)) return;
	if (!sightrange || sightrange > Rule.GapShroudRadius) return;

	/*
	**	Determine logical cell coordinate for center scan point.
	*/
	xx = Cell_X(cell);

	/*
	**	Incremental scans only scan the outer rings. Full scans
	**	scan all internal cells as well.
	*/
	count = RadiusCount[sightrange];
	ptr = &RadiusOffset[0];

	/*
	**	Process all offsets required for the desired scan.
	*/
	while (count--) {
		CELL	newcell;			// New cell with offset.
		int	xdiff;			// New cell's X coordinate distance from center.

		newcell = cell + *ptr++;

		/*
		**	Determine if the map edge has been wrapped. If so,
		**	then don't process the cell.
		*/
		if ((unsigned)newcell >= MAP_CELL_TOTAL) continue;
		xdiff = Cell_X(newcell) - xx;
		xdiff = ABS(xdiff);
		if (xdiff > sightrange) continue;
		if (Distance(Cell_Coord(newcell), Cell_Coord(cell)) > (sightrange * CELL_LEPTON_W)) continue;

		/*
		**	Shroud the cell.
		*/
		Map.Shroud_Cell(newcell, house);
	}
}


/***********************************************************************************************
 * MapClass::Jam_From -- Mark as jammed the cells within a specified radius.                   *
 *                                                                                             *
 *    This routine is used to jam the cells around a specific location.                        *
 *    Typically, as a gap generator structure is created, this routine will be                 *
 *    called. Since it deals with MANY cells, it needs to be extremely                         *
 *    fast.                                                                                    *
 *                                                                                             *
 * INPUT:   cell     -- The coordinate that the jamming originates from.                       *
 *                                                                                             *
 *          jamrange -- The distance in cells that jamming extends.                            *
 *                                                                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/09/1995 BWG : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Jam_From(CELL cell, int jamrange, HouseClass * house)
{
	int xx;				// Center cell X coordinate (bounds checking).
	int const * ptr;	// Offset pointer.
	int count;			// Counter for number of offsets to process.

	/*
	**	Units that are off-map cannot jam.
	*/
	if (!jamrange || jamrange > Rule.GapShroudRadius) return;

	/*
	**	Determine logical cell coordinate for center scan point.
	*/
	xx = Cell_X(cell);

	/*
	**	Incremental scans only scan the outer rings. Full scans
	**	scan all internal cells as well.
	*/
	count = RadiusCount[jamrange];
	ptr = &RadiusOffset[0];

	/*
	**	Process all offsets required for the desired scan.
	*/
	while (count--) {
		CELL	newcell;			// New cell with offset.
		int	xdiff;			// New cell's X coordinate distance from center.

		newcell = cell + *ptr++;

		/*
		**	Determine if the map edge has been wrapped. If so,
		**	then don't process the cell.
		*/
		if ((unsigned)newcell >= MAP_CELL_TOTAL) continue;
		xdiff = Cell_X(newcell) - xx;
		xdiff = ABS(xdiff);
		if (xdiff > jamrange) continue;
		if (Distance(Cell_Coord(newcell), Cell_Coord(cell)) > (jamrange * CELL_LEPTON_W)) continue;

		/*
		**	Jam the cell. For incremental scans, then update
		**	adjacent cells as well. For full scans, just update
		**	the cell itself.
		*/
		Map.Jam_Cell(newcell, house/*KO, false*/);
	}

	/*
	** Updated for client/server multiplayer. ST - 8/12/2019 3:25PM
	*/
	if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {
		if (!house->IsPlayerControl) {
			Map.Constrained_Look(Cell_Coord(cell), Rule.GapShroudRadius * CELL_LEPTON_W, PlayerPtr);
		}
		
	} else {
		
		for (int i = 0; i < Session.Players.Count(); i++) {
			HouseClass *player_house = HouseClass::As_Pointer(Session.Players[i]->Player.ID);
			if (player_house->IsHuman && player_house != house) {
				Map.Constrained_Look(Cell_Coord(cell), Rule.GapShroudRadius * CELL_LEPTON_W, player_house);
			}
		}
	}

#ifdef OBSOLETE
	/*
	**	The objects on the map need to perform a manual look operation if they happen
	**	to have their sight range overlap the gap radius.
	*/
	if (!house->IsPlayerControl) {
//	if (house != PlayerPtr) {

		for (int index = 0; index < Map.Layer[LAYER_GROUND].Count(); index++) {
			ObjectClass * object = Map.Layer[LAYER_GROUND][index];
			if (object && object->Is_Techno()) {
				TechnoClass * tech = ((TechnoClass *)object);

				if (tech->IsDiscoveredByPlayer &&
					(tech->Distance(As_Target(cell)) / CELL_LEPTON_W) <= tech->Techno_Type_Class()->SightRange + Rule.GapShroudRadius &&
					(tech->House->IsPlayerControl ||
					(tech->What_Am_I() == RTTI_BUILDING && Rule.IsAllyReveal && tech->House->Is_Ally(PlayerPtr)))) {

					object->Look();
				}
			}
		}
	}
#endif

#ifdef OBSOLETE
		/*
		** Here all the player's vehicles will perform a look if they're within
		** the shadow.
		*/
		for (int index = 0; index < Units.Count(); index++) {
			UnitClass * unit = Units.Ptr(index);
			if (unit && !unit->IsInLimbo && unit->House == PlayerPtr) {
				int dist = (unit->Distance(As_Target(cell))) / CELL_LEPTON_W;
				if (dist <= unit->Class->SightRange + Rule.GapShroudRadius /*gap generator sightrange*/) {
					unit->Look();
				}
			}
		}
		for (index = 0; index < Infantry.Count(); index++) {
			InfantryClass * unit = Infantry.Ptr(index);
			if (unit && !unit->IsInLimbo && unit->House == PlayerPtr) {
				int dist = (unit->Distance(As_Target(cell))) / CELL_LEPTON_W;
				if (dist <= unit->Class->SightRange + Rule.GapShroudRadius /*gap generator sightrange*/) {
					unit->Look();
				}
			}
		}
		for (index = 0; index < Vessels.Count(); index++) {
			VesselClass * unit = Vessels.Ptr(index);
			if (unit && !unit->IsInLimbo && unit->House == PlayerPtr) {
				int dist = (unit->Distance(As_Target(cell))) / CELL_LEPTON_W;
				if (dist <= unit->Class->SightRange + Rule.GapShroudRadius /*gap generator sightrange*/) {
					unit->Look();
				}
			}
		}
	}
#endif

}


/***********************************************************************************************
 * MapClass::UnJam_From -- Remove jamming on the cells within a specified radius.              *
 *                                                                                             *
 *    This routine is used to jam the cells around a specific location.                        *
 *    Typically, as a gap generator structure is created, this routine will be                 *
 *    called. Since it deals with MANY cells, it needs to be extremely                         *
 *    fast.                                                                                    *
 *                                                                                             *
 * INPUT:   cell     -- The coordinate that the jamming originates from.                       *
 *                                                                                             *
 *          jamrange -- The distance in cells that jamming extends.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/09/1995 BWG : Created.                                                                 *
 *=============================================================================================*/
void MapClass::UnJam_From(CELL cell, int jamrange, HouseClass * house)
{
	int xx;				// Center cell X coordinate (bounds checking).
	int const * ptr;	// Offset pointer.
	int count;			// Counter for number of offsets to process.

	/*
	**	Units that are off-map cannot jam.
	*/
	if (!jamrange || jamrange > Rule.GapShroudRadius) return;

	/*
	**	Determine logical cell coordinate for center scan point.
	*/
	xx = Cell_X(cell);

	/*
	**	Incremental scans only scan the outer rings. Full scans
	**	scan all internal cells as well.
	*/
	count = RadiusCount[jamrange];
	ptr = &RadiusOffset[0];

	/*
	**	Process all offsets required for the desired scan.
	*/
	while (count--) {
		CELL	newcell;			// New cell with offset.
		int	xdiff;			// New cell's X coordinate distance from center.

		newcell = cell + *ptr++;

		/*
		**	Determine if the map edge has been wrapped. If so,
		**	then don't process the cell.
		*/
		if ((unsigned)newcell >= MAP_CELL_TOTAL) continue;
		xdiff = Cell_X(newcell) - xx;
		xdiff = ABS(xdiff);
		if (xdiff > jamrange) continue;
		if (Distance(Cell_Coord(newcell), Cell_Coord(cell)) > (jamrange * CELL_LEPTON_W)) continue;

		/*
		**	Jam the cell. For incremental scans, then update
		**	adjacent cells as well. For full scans, just update
		**	the cell itself.
		*/
		Map.UnJam_Cell(newcell, house);
	}
}


/***********************************************************************************************
 * MapClass::In_Radar -- Is specified cell in the radar map?                                   *
 *                                                                                             *
 *    This determines if the specified cell can be within the navigable                        *
 *    bounds of the map. Technically, this means, any cell that can be                         *
 *    scanned by radar. If a cell returns false from this function, then                       *
 *    the player could never move to or pass over this cell.                                   *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine.                                                      *
 *                                                                                             *
 * OUTPUT:  bool; Is this cell possible to be displayed on radar?                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   04/30/1994 JLB : Converted to member function.                                            *
 *   05/01/1994 JLB : Speeded up.                                                              *
 *=============================================================================================*/
bool MapClass::In_Radar(CELL cell) const
{
	/*
	**	If the cell value is WAY out of range, then it obviously can't be part of the game
	**	playfield.
	*/
	if ((unsigned)cell > MAP_CELL_TOTAL) return(false);

	/*
	**	If the cell is off the left or right edge of the playfield, then return the "not in
	**	radar" flag.
	*/
	if ((unsigned)(Cell_X(cell) - MapCellX) >= (unsigned)MapCellWidth) return(false);

	/*
	**	If the cell is off the top or bottom edge of the playfield, then return the "not in
	**	radar" flag.
	*/
	if ((unsigned)(Cell_Y(cell) - MapCellY) >= (unsigned)MapCellHeight) return(false);

	return(true);
}


/***********************************************************************************************
 * MapClass::Place_Down -- Places the specified object onto the map.                           *
 *                                                                                             *
 *    This routine is used to place an object onto the map. It updates the "occupier" of the   *
 *    cells that this object covers. The cells are determined from the Occupy_List function    *
 *    provided by the object. Only one cell can have an occupier and this routine is the only  *
 *    place that sets this condition.                                                          *
 *                                                                                             *
 * INPUT:   cell     -- The cell to base object occupation around.                             *
 *                                                                                             *
 *          object   -- The object to place onto the map.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Place_Down(CELL cell, ObjectClass * object)
{
	if (!object) return;

	if (object->Class_Of().IsFootprint && object->In_Which_Layer() == LAYER_GROUND)  {
		short xlist[32];
		List_Copy(object->Occupy_List(), ARRAY_SIZE(xlist), xlist);
		short const * list = xlist;
		while (*list != REFRESH_EOL) {
			CELL newcell = cell + *list++;
			if ((unsigned)newcell < MAP_CELL_TOTAL) {
				(*this)[newcell].Occupy_Down(object);
				(*this)[newcell].Recalc_Attributes();
				(*this)[newcell].Redraw_Objects();
			}
		}

		List_Copy(object->Overlap_List(), ARRAY_SIZE(xlist), xlist);
		list = xlist;
		while (*list != REFRESH_EOL) {
			CELL newcell = cell + *list++;
			if ((unsigned)newcell < MAP_CELL_TOTAL) {
				(*this)[newcell].Overlap_Down(object);
				(*this)[newcell].Redraw_Objects();
			}
		}
	}
}


/***********************************************************************************************
 * MapClass::Pick_Up -- Removes specified object from the map.                                 *
 *                                                                                             *
 *    The object specified is removed from the map by this routine. This will remove the       *
 *    occupation flag for all the cells that the object covers. The cells that are covered     *
 *    are determined from the Occupy_List function.                                            *
 *                                                                                             *
 * INPUT:   cell     -- The cell that the object is centered about.                            *
 *                                                                                             *
 *          object   -- Pointer to the object that will be removed.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Pick_Up(CELL cell, ObjectClass * object)
{
	if (!object) return;

	if (object->Class_Of().IsFootprint && object->In_Which_Layer() == LAYER_GROUND)  {
		short xlist[32];
		List_Copy(object->Occupy_List(), ARRAY_SIZE(xlist), xlist);
		short const * list = xlist;
		while (*list != REFRESH_EOL) {
			CELL newcell = cell + *list++;
			if ((unsigned)newcell < MAP_CELL_TOTAL) {
				(*this)[newcell].Occupy_Up(object);
				(*this)[newcell].Recalc_Attributes();
				(*this)[newcell].Redraw_Objects();
			}
		}

		List_Copy(object->Overlap_List(), ARRAY_SIZE(xlist), xlist);
		list = xlist;
		while (*list != REFRESH_EOL) {
			CELL newcell = cell + *list++;
			if ((unsigned)newcell < MAP_CELL_TOTAL) {
				(*this)[newcell].Overlap_Up(object);
				(*this)[newcell].Redraw_Objects();
			}
		}
	}
}


/***********************************************************************************************
 * MapClass::Overlap_Down -- computes & marks object's overlap cells                           *
 *                                                                                             *
 * This routine is just like Place_Down, but it doesn't mark the cell's Occupier.              *
 * This routine is used to implement MARK_OVERLAP_DOWN, which is useful for changing           *
 * an object's render size, but not its logical size (ie when it's selected or an              *
 * animation is attached to it).                                                               *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      cell -- The cell to base object overlap around.                                        *
 *    object   -- The object to place onto the map.                                            *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/12/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Overlap_Down(CELL cell, ObjectClass * object)
{
	if (!object) return;

	if (object->Class_Of().IsFootprint && object->In_Which_Layer() == LAYER_GROUND)  {
		short xlist[32];
		List_Copy(object->Overlap_List(), ARRAY_SIZE(xlist), xlist);
		short const * list = xlist;
		while (*list != REFRESH_EOL) {
			CELL newcell = cell + *list++;
			if ((unsigned)newcell < MAP_CELL_TOTAL) {
				(*this)[newcell].Overlap_Down(object);
				(*this)[newcell].Redraw_Objects();
			}
		}
	}
}


/***********************************************************************************************
 * MapClass::Overlap_Up -- Computes & clears object's overlap cells                            *
 *                                                                                             *
 * This routine is just like Pick_Up, but it doesn't mark the cell's Occupier.                 *
 * This routine is used to implement MARK_OVERLAP_UP, which is useful for changing             *
 * an object's render size, but not its logical size (ie when it's selected or an              *
 * animation is attached to it).                                                               *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      cell -- The cell to base object overlap around.                                        *
 *    object   -- The object to place onto the map.                                            *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/12/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Overlap_Up(CELL cell, ObjectClass * object)
{
	if (!object) return;

	if (object->Class_Of().IsFootprint && object->In_Which_Layer() == LAYER_GROUND)  {
		short xlist[32];
		List_Copy(object->Overlap_List(), ARRAY_SIZE(xlist), xlist);
		short const * list = xlist;
		while (*list != REFRESH_EOL) {
			CELL newcell = cell + *list++;
			if ((unsigned)newcell < MAP_CELL_TOTAL) {
				(*this)[newcell].Overlap_Up(object);
				(*this)[newcell].Redraw_Objects();
			}
		}
	}
}


/***********************************************************************************************
 * MapClass::Overpass -- Performs any final cleanup to a freshly constructed map.              *
 *                                                                                             *
 *    This routine will clean up anything necessary with the presumption that the map has      *
 *    been freshly created. Such things to clean up include various tiberium concentrations.   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the total credit value of the tiberium on the map.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *   02/13/1995 JLB : Returns total tiberium worth.                                            *
 *   02/15/1995 JLB : Optimal scan.                                                            *
 *=============================================================================================*/
long MapClass::Overpass(void)
{
	long value = 0;

	/*
	**	Smooth out Tiberium. Cells that are not surrounded by other tiberium
	**	will be reduced in density.
	*/
	for (int y = 0; y < MapCellHeight; y++) {
		for (int x = 0; x < MapCellWidth; x++) {
			CELL cell = (MapCellY+y) * MAP_CELL_W + (MapCellX+x);
			value += (*this)[cell].Tiberium_Adjust(true);
			(*this)[cell].Recalc_Attributes();
		}
	}
	return(value);
}


/***********************************************************************************************
 * MapClass::Write_Binary -- Pipes the map template data to the destination specified.         *
 *                                                                                             *
 *    This stores the template data from the map to the output pipe specified. The template    *
 *    data consists of the template type number and template icon number for every cell on     *
 *    the map. The output is organized in such a way so as to get maximum compression.         *
 *                                                                                             *
 * INPUT:   pipe  -- Reference to the output pipe that will receive the map template data.     *
 *                                                                                             *
 * OUTPUT:  Returns with the total number of bytes output to the pipe.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int MapClass::Write_Binary(Pipe & pipe)
{
	int total = 0;

	LCWPipe comp(LCWPipe::COMPRESS);
	comp.Put_To(&pipe);

	CellClass * cellptr = &Array[0];
	for (int i = 0; i < MAP_CELL_TOTAL; i++) {
		total += comp.Put(&cellptr->TType, sizeof(cellptr->TType));
		cellptr++;
	}

	cellptr = &Array[0];
	for (int i = 0; i < MAP_CELL_TOTAL; i++) {
		total += comp.Put(&cellptr->TIcon, sizeof(cellptr->TIcon));
		cellptr++;
	}

	return(total);
}


/***********************************************************************************************
 * MapClass::Read_Binary -- Reads the binary data from the straw specified.                    *
 *                                                                                             *
 *    This routine will retrieve the map template data from the straw specified.               *
 *                                                                                             *
 * INPUT:   straw -- Reference to the straw that will supply the map template data.            *
 *                                                                                             *
 * OUTPUT:  bool; Was the template data retrieved?                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool MapClass::Read_Binary(Straw & straw)
{
	LCWStraw decomp(LCWStraw::DECOMPRESS);
	decomp.Get_From(&straw);

	CELL cell;
	CellClass * cellptr;
	switch (NewINIFormat) {
		default:
			cellptr = &Array[0];
			for (cell = 0; cell < MAP_CELL_TOTAL; cell++) {
				decomp.Get(&cellptr->TType, sizeof(cellptr->TType));
				cellptr++;
			}
			cellptr = &Array[0];
			for (cell = 0; cell < MAP_CELL_TOTAL; cell++) {
				decomp.Get(&cellptr->TIcon, sizeof(cellptr->TIcon));
				cellptr->Recalc_Attributes();
				cellptr++;
			}
			break;

		case 0:
		case 1:
		case 2:
			cellptr = &Array[0];
			for (cell = 0; cell < MAP_CELL_TOTAL; cell++) {
				decomp.Get(&cellptr->TType, sizeof(cellptr->TType));
				decomp.Get(&cellptr->TIcon, sizeof(cellptr->TIcon));
				cellptr->Recalc_Attributes();
				cellptr++;
			}
			break;
	}
	return(true);
}


/***********************************************************************************************
 * MapClass::Logic -- Handles map related logic functions.                                     *
 *                                                                                             *
 *    Manages tiberium growth and spread.                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/11/1995 JLB : Created.                                                                 *
 *   07/09/1995 JLB : Handles two directional scan.                                            *
 *   08/01/1995 JLB : Gives stronger weight to blossom trees.                                  *
 *=============================================================================================*/
void MapClass::Logic(void)
{
	if (Debug_Force_Crash) { *((int *)0) = 1; }
	/*
	**	Crate regeneration is handled here.
	*/
	if (Session.Type != GAME_NORMAL && Session.Options.Goodies) {

		/*
		**	Find any crate that has expired and then regenerate it at a new
		**	spot.
		*/
		for (int index = 0; index < ARRAY_SIZE(Crates); index++) {
			if (Crates[index].Is_Expired()) {
				Crates[index].Remove_It();
				Place_Random_Crate();
			}
		}
	}

	/*
	**	Bail early if there is no allowed growth or spread of Tiberium.
	*/
	if (!Rule.IsTGrowth && !Rule.IsTSpread) return;

	/*
	**	Scan another block of the map in order to accumulate the potential
	**	Tiberium cells that can grow or spread.
	*/
	int subcount = MAP_CELL_TOTAL / (Rule.GrowthRate * TICKS_PER_MINUTE);
	subcount = max(subcount, 1);
	int index;
	for (index = TiberiumScan; index < MAP_CELL_TOTAL; index++) {
		CELL cell = index;
		if (In_Radar(cell)) {
			CellClass * ptr = &(*this)[cell];

			/*
			**	Tiberium cells can grow.
			*/
			if (ptr->Can_Tiberium_Grow()) {

				/*
				**	Either replace an existing recorded cell value or add the new cell value to
				**	the list.
				*/
				if (Random_Pick(0, TiberiumGrowthExcess) <= TiberiumGrowthCount) {
					if (TiberiumGrowthCount < sizeof(TiberiumGrowth)/sizeof(TiberiumGrowth[0])) {
						TiberiumGrowth[TiberiumGrowthCount++] = cell;
					} else {
						TiberiumGrowth[Random_Pick(0, TiberiumGrowthCount-1)] = cell;
					}
				}
				TiberiumGrowthExcess++;
			}

			/*
			**	Heavy Tiberium growth can spread.
			*/
			if (ptr->Can_Tiberium_Spread()) {
				/*
				**	Either replace an existing recorded cell value or add the new cell value to
				**	the list.
				*/
				if (Random_Pick(0, TiberiumSpreadExcess) <= TiberiumSpreadCount) {
					if (TiberiumSpreadCount < ARRAY_SIZE(TiberiumSpread)) {
						TiberiumSpread[TiberiumSpreadCount++] = cell;
					} else {
						TiberiumSpread[Random_Pick(0, TiberiumSpreadCount-1)] = cell;
					}
				}
				TiberiumSpreadExcess++;
			}
		}

		subcount--;
		if (subcount == 0) break;
	}
	TiberiumScan = index;

	/*
	**	When the entire map has been processed, proceed with tiberium (ore) growth
	**	and spread action.
	*/
	if (TiberiumScan >= MAP_CELL_TOTAL) {
		TiberiumScan = 0;

		/*
		**	Growth logic.
		*/
		if (TiberiumGrowthCount) {
			for (int i = 0; i < TiberiumGrowthCount; i++) {
				CELL cell = TiberiumGrowth[i];
				CellClass * newcell = &(*this)[cell];
				newcell->Grow_Tiberium();
			}
		}
		TiberiumGrowthCount = 0;
		TiberiumGrowthExcess = 0;

		/*
		**	Spread logic.
		*/
		if (TiberiumSpreadCount) {
			for (int i = 0; i < TiberiumSpreadCount; i++) {
				Map[TiberiumSpread[i]].Spread_Tiberium();
			}
		}
		TiberiumSpreadCount = 0;
		TiberiumSpreadExcess = 0;
	}
}


/***********************************************************************************************
 * MapClass::Cell_Region -- Determines the region from a specified cell number.                *
 *                                                                                             *
 *    Use this routine to determine what region a particular cell lies in.                     *
 *                                                                                             *
 * INPUT:   cell  -- The cell number to examine.                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the region that the specified cell occupies.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int MapClass::Cell_Region(CELL cell)
{
	return((Cell_X(cell) / REGION_WIDTH) + 1) +	(((Cell_Y(cell) / REGION_HEIGHT) + 1) * MAP_REGION_WIDTH);
}


/***************************************************************************
 * MapClass::Cell_Threat -- Gets a houses threat value for a cell          *
 *                                                                         *
 * INPUT:   CELL        cell    - the cell number to check                 *
 *            HouseType house   - the house to check                       *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/25/1995 PWG : Created.                                             *
 *=========================================================================*/
int MapClass::Cell_Threat(CELL cell, HousesType house)
{
	int threat = HouseClass::As_Pointer(house)->Regions[Map.Cell_Region(Map[cell].Cell_Number())].Threat_Value();
	//using function for IsVisible so we have different results for different players - JAS 2019/09/30
	if (!threat && Map[cell].Is_Visible(house)) {
		threat = 1;
	}
	return(threat);
}


/***********************************************************************************************
 * MapClass::Place_Random_Crate -- Places a crate at random location on map.                   *
 *                                                                                             *
 *    This routine will place a crate at a random location on the map. This routine will only  *
 *    make a limited number of attempts to place and if unsuccessful, it will not place any.   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was a crate successfully placed?                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool MapClass::Place_Random_Crate(void)
{
	/*
	**	Find a crate index that is free for assignment. If there are
	**	no free slots, then return with failure to place crate.
	*/
	int crateindex = 0;
	for (crateindex = 0; crateindex < ARRAY_SIZE(Crates); crateindex++) {
		if (!Crates[crateindex].Is_Valid()) break;
	}
	if (crateindex == ARRAY_SIZE(Crates)) {
		return(false);
	}

	/*
	**	Give a good effort to scan for and place a crate down on the map.
	*/
	for (int index = 0; index < 1000; index++) {
		CELL cell = Map.Pick_Random_Location();

		if (Crates[crateindex].Create_Crate(cell)) {
			return(true);
		}
	}
	return(false);
}


/***********************************************************************************************
 * MapClass::Remove_Crate -- Remove a crate from the specified cell.                           *
 *                                                                                             *
 *    This will examine the cell and remove any crates there.                                  *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine for crates and remove from.                           *
 *                                                                                             *
 * OUTPUT:  bool; Was a crate found at the location specified and was it removed?              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool MapClass::Remove_Crate(CELL cell)
{
	if (Session.Type != GAME_NORMAL) {
		for (int index = 0; index < ARRAY_SIZE(Crates); index++) {
			if (Crates[index].Is_Here(cell)) {
				return(Crates[index].Remove_It());
			}
		}
	}

//	if (Session.Type == GAME_NORMAL) {
		CellClass * cellptr = &(*this)[cell];
		if (cellptr->Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(cellptr->Overlay).IsCrate) {
			cellptr->Overlay = OVERLAY_NONE;
			cellptr->OverlayData = 0;
			return(true);
		}
//	} else {
//		for (int index = 0; index < ARRAY_SIZE(Crates); index++) {
//			if (Crates[index].Is_Here(cell)) {
//				return(Crates[index].Remove_It());
//			}
//		}
//	}
	return(false);
}


/***************************************************************************
 * MapClass::Validate -- validates every cell on the map                   *
 *                                                                         *
 * This is a debugging routine, designed to detect memory trashers that    *
 * alter the map.  This routine is slow, but thorough.                     *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = map is OK, false = an error was found                       *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/08/1995 BRR : Created.                                             *
 *=========================================================================*/
int MapClass::Validate(void)
{
	CELL cell;
	TemplateType ttype;
	unsigned char ticon;
	TemplateTypeClass const *tclass;
	unsigned char map[13*8];
	OverlayType overlay;
	SmudgeType smudge;
	ObjectClass * obj;
	LandType land;
	int i;

BlubCell = &Array[797];

if (BlubCell->Overlapper[1]) {
	obj = BlubCell->Overlapper[1];
	if (obj) {
		if (obj->IsInLimbo)
		obj = obj;
	}
}

	/*
	**	Check every cell on the map, even those that aren't displayed,
	**	in the hopes of detecting a memory trasher.
	*/
	for (cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		/*
		**	Validate Template & Icon data
		*/
		ttype = (*this)[cell].TType;
		ticon = (*this)[cell].TIcon;
		if (ttype >= TEMPLATE_COUNT && ttype != TEMPLATE_NONE)
			return(false);

		/*
		**	To validate the icon value, we have to get a copy of the template's
		**	"icon map"; this map will have 0xff's in spots where there is no
		**	icon.  If the icon value is out of range or points to an invalid spot,
		**	return an error.
		*/
		if (ttype != TEMPLATE_NONE) {
			tclass = &TemplateTypeClass::As_Reference(ttype);
			ticon = (*this)[cell].TIcon;
			Mem_Copy(Get_Icon_Set_Map(tclass->Get_Image_Data()), map, tclass->Width * tclass->Height);
			if (ticon < 0 || ticon >= (tclass->Width * tclass->Height) || map[ticon]==0xff) {
				return (false);
			}
		}

		/*
		**	Validate Overlay
		*/
		overlay = (*this)[cell].Overlay;
		if (overlay < OVERLAY_NONE || overlay >= OVERLAY_COUNT) {
			return(false);
		}

		/*
		**	Validate Smudge
		*/
		smudge = (*this)[cell].Smudge;
		if (smudge < SMUDGE_NONE || smudge >= SMUDGE_COUNT) {
			return(false);
		}

		/*
		**	Validate LandType
		*/
		land = (*this)[cell].Land_Type();
		if (land < LAND_CLEAR || land >= LAND_COUNT) {
			return(false);
		}

		/*
		**	Validate Occupier
		*/
		obj = (*this)[cell].Cell_Occupier();
		if (obj) {
			if (
				((unsigned int)obj & 0xff000000) ||
				((unsigned int)obj->Next & 0xff000000) ||
//				((unsigned int)obj->Trigger & 0xff000000) ||
				obj->IsInLimbo ||
				((unsigned int)Coord_Cell(obj->Coord) >= MAP_CELL_TOTAL)) {

				return (false);
			}
		}

		/*
		**	Validate Overlappers
		*/
		for (i = 0; i < ARRAY_SIZE((*this)[cell].CellClass::Overlapper); i++) {
			obj = (*this)[cell].Overlapper[i];
			if (obj) {
				if (
					((unsigned int)obj & 0xff000000) ||
					((unsigned int)obj->Next & 0xff000000) ||
//					((unsigned int)obj->Trigger & 0xff000000) ||
					obj->IsInLimbo ||
					((unsigned int)Coord_Cell(obj->Coord) >= MAP_CELL_TOTAL)) {

					return (false);
				}
			}
		}
	}

	return (true);
}


/***********************************************************************************************
 * MapClass::Close_Object -- Finds a clickable close object to the specified coordinate.       *
 *                                                                                             *
 *    This routine is used by the mouse input processing code to find a clickable object       *
 *    close to coordinate specified. This is for targeting as well as selection determination. *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to scan for close object from.                             *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to an object that is nearby the specified coordinate.       *
 *                                                                                             *
 * WARNINGS:   There could be a cloaked object at the location, but it won't be considered     *
 *             if it is not owned by the player.                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * MapClass::Close_Object(COORDINATE coord) const
{
	ObjectClass * object = 0;
	int distance = 0;
	CELL cell = Coord_Cell(coord);

	/*
	**	Scan through current and adjacent cells, looking for the
	**	closest object (within reason) to the specified coordinate.
	*/
	static int _offsets[] = {0, -1, 1, -MAP_CELL_W, MAP_CELL_W, MAP_CELL_W-1, MAP_CELL_W+1, -(MAP_CELL_W-1), -(MAP_CELL_W+1)};
	for (int index = 0; index < (sizeof(_offsets) / sizeof(_offsets[0])); index++) {

		/*
		**	Examine the cell for close object. Make sure that the cell actually is a
		**	legal one.
		*/
		CELL newcell = cell + _offsets[index];
		if (In_Radar(newcell)) {

			/*
			**	Search through all objects that occupy this cell and then
			**	find the closest object. Check against any previously found object
			**	to ensure that it is actually closer.
			*/
			ObjectClass * o = Array[newcell].Cell_Occupier();
			while (o != NULL) {

				/*
				**	Special case check to ignore cloaked object if not owned by the player.
				*/
				// Change for client/server multiplayer. ST - 8/7/2019 10:35AM
				//if (!o->Is_Techno() || ((TechnoClass *)o)->IsOwnedByPlayer || ((TechnoClass *)o)->Cloak != CLOAKED) {
				if (!o->Is_Techno() || ((TechnoClass *)o)->Is_Owned_By_Player() || ((TechnoClass *)o)->Cloak != CLOAKED) {
					int d=-1;
					if (o->What_Am_I() == RTTI_BUILDING) {
						d = Distance(coord, Cell_Coord(newcell));
						if (d > 0x00C0) d = -1;
					} else {
						d = Distance(coord, o->Center_Coord());
					}
					if (d >= 0 && (!object || d < distance)) {
						distance = d;
						object = o;
					}
				}
				o = o->Next;
			}
		}
	}

	/*
	** Handle aircraft selection separately, since they aren't tracked in cells while flying
	*/
	for (int index = 0; index < Aircraft.Count(); index++) {
		AircraftClass * aircraft = Aircraft.Ptr(index);

		if (aircraft->In_Which_Layer() != LAYER_GROUND) {
			if (aircraft->Is_Owned_By_Player() || (aircraft->Cloak != CLOAKED)) {
				int d = Distance(coord, Coord_Add(aircraft->Center_Coord(), XY_Coord(0, -aircraft->Height)));
				if (d >= 0 && (!object || d < distance)) {
					distance = d;
					object = aircraft;
				}
			}
		}
	}

	/*
	**	Only return the object if it is within 1/4 cell distance from the specified
	**	coordinate.
	*/
	if (object && distance > 0xC0) {
		object = 0;
	}
	return(object);
}


/***********************************************************************************************
 * MapClass::Zone_Reset -- Resets all zone numbers to match the map.                           *
 *                                                                                             *
 *    This routine will rescan the map and fill in the zone values for each of the cells.      *
 *    All cells that are contiguous are given the same zone number.                            *
 *                                                                                             *
 * INPUT:   method   -- The method to recalculate the zones upon. If 1 then recalc non         *
 *                      crushable zone. If 2 then recalc crushable zone. If 3, then            *
 *                      recalc both zones.                                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This is a time consuming routine. Call it as infrequently as possible. It must  *
 *             be called whenever something that would affect contiguousness occurs. Example:  *
 *             when a bridge is built or destroyed.                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool MapClass::Zone_Reset(int method)
{
	/*
	**	Zero out all zones to a null state.
	*/
	for (int index = 0; index < MAP_CELL_TOTAL; index++) {
		if (method & MZONEF_NORMAL) {
			Array[index].Zones[MZONE_NORMAL] = 0;
		}
		if (method & MZONEF_CRUSHER) {
			Array[index].Zones[MZONE_CRUSHER] = 0;
		}
		if (method & MZONEF_DESTROYER) {
			Array[index].Zones[MZONE_DESTROYER] = 0;
		}
		if (method & MZONEF_WATER) {
			Array[index].Zones[MZONE_WATER] = 0;
		}
	}

	/*
	**	Normal zone recalculation.
	*/
	if (method & MZONEF_NORMAL) {
		int zone = 1;			// Starting zone number.
		for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
			if (Zone_Span(cell, zone, MZONE_NORMAL)) {
				zone++;
			}
		}
	}

	/*
	**	Crushable wall recalculation.
	*/
	if (method & MZONEF_CRUSHER) {
		int zone = 1;			// Starting zone number.
		for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
			if (Zone_Span(cell, zone, MZONE_CRUSHER)) {
				zone++;
			}
		}
	}

	/*
	**	Wall destroyer zone recalculation.
	*/
	if (method & MZONEF_DESTROYER) {
		int zone = 1;			// Starting zone number.
		for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
			if (Zone_Span(cell, zone, MZONE_DESTROYER)) {
				zone++;
			}
		}
	}

	/*
	**	Water based zone recalcuation.
	*/
	if (method & MZONEF_WATER) {
		int zone = 1;			// Starting zone number.
		for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
			if (Zone_Span(cell, zone, MZONE_WATER)) {
				zone++;
			}
		}
	}

	return(false);
}


/***********************************************************************************************
 * MapClass::Zone_Span -- Flood fills the specified zone from the cell origin.                 *
 *                                                                                             *
 *    This routine is used to fill a zone value into the map. The map is "flood filled" from   *
 *    the cell specified. All adjacent (8 connected) and generally passable terrain cells are  *
 *    given the zone number specified. This routine checks for legality before filling         *
 *    occurs. The routine is safe to call even if the legality of the cell is unknown at the   *
 *    time of the call.                                                                        *
 *                                                                                             *
 * INPUT:   cell  -- The cell to begin filling from.                                           *
 *                                                                                             *
 *          zone  -- The zone number to assign to all adjacent cells.                          *
 *                                                                                             *
 *          check -- The zone type to check against.                                           *
 *                                                                                             *
 * OUTPUT:  Returns with the number of cells marked by this routine.                           *
 *                                                                                             *
 * WARNINGS:   This routine is slow and recursive. Only use when necessary.                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/25/1995 JLB : Created.                                                                 *
 *   10/05/1996 JLB : Examines crushable walls.                                                *
 *=============================================================================================*/
int MapClass::Zone_Span(CELL cell, int zone, MZoneType check)
{
	int filled = 0;
	int xbegin = Cell_X(cell);
	int xend = xbegin;
	int y = Cell_Y(cell);

	/*
	**	Perform some preliminary legality checks. If the cell specified
	**	is illegal, then no further processing is necessary.
	*/
	if (y < MapCellY || y >= MapCellY+MapCellHeight || xbegin < MapCellX || xbegin >= MapCellX+MapCellWidth) {
		return(0);
	}

	/*
	**	Find the full extent of the current span by first scanning leftward
	**	until a boundary is reached.
	*/
	for (; xbegin >= MapCellX; xbegin--) {
		CellClass * cellptr = &(*this)[XY_Cell(xbegin, y)];
		if (cellptr->Zones[check] != 0 || (!cellptr->Is_Clear_To_Move(check == MZONE_WATER ? SPEED_FLOAT : SPEED_TRACK, true, true, -1, check))) {

			/*
			**	Special short circuit code to bail from this entire routine if
			**	it was called for a cell that is not a legal candidate for
			**	zone marking. This eliminates redundant processing and allows this
			**	routine to be called for illegal cells without causing an error.
			*/
			if (xbegin == Cell_X(cell)) return(0);

			/*
			**	Otherwise break out of the left scan since a boundary was discovered.
			*/
			xbegin++;
			break;
		}
	}
	xbegin = max(xbegin, MapCellX);

	/*
	**	Scan rightward until a boundary is reached. This will then define the
	**	extent of the current span.
	*/
	for (; xend < MapCellX+MapCellWidth; xend++) {
		CellClass * cellptr = &(*this)[XY_Cell(xend, y)];
		if (cellptr->Zones[check] != 0 || (!cellptr->Is_Clear_To_Move(check == MZONE_WATER ? SPEED_FLOAT : SPEED_TRACK, true, true, -1, check))) {
			xend--;
			break;
		}
	}
	xend = min(xend, MapCellX+MapCellWidth-1);

	/*
	**	At this point we know the bounds of the current span. Fill in the zone values
	**	for the entire span.
	*/
	for (int x = xbegin; x <= xend; x++) {
		(*this)[XY_Cell(x, y)].Zones[check] = zone;
		filled++;
	}

	/*
	**	Now scan the upper and lower shadow rows. If any of these rows contain
	**	candidate cells, then recursively call the span process for them. Take
	**	note that the adjacent span scanning starts one cell wider on each
	**	end of the scan. This is necessary because diagonals are considered
	**	adjacent.
	*/
	for (int x = xbegin-1; x <= xend; x++) {
		filled += Zone_Span(XY_Cell(x, y-1), zone, check);
		filled += Zone_Span(XY_Cell(x, y+1), zone, check);
	}
	return(filled);
}


/***********************************************************************************************
 * MapClass::Nearby_Location -- Finds a generally clear location near a specified cell.        *
 *                                                                                             *
 *    This routine is used to find a location that probably will be ok to move to that is      *
 *    located as close as possible to the specified cell. The computer uses this when it has   *
 *    determined the ideal location for an object, but then needs to give a valid movement     *
 *    destination to a unit.                                                                   *
 *                                                                                             *
 * INPUT:   cell  -- The cell that scanning should radiate out from.                           *
 *                                                                                             *
 *          zone  -- The zone that must be matched to find a legal location (value of -1 means *
 *                   any zone will do).                                                        *
 *                                                                                             *
 *                                                                                             *
 *          check -- The type of zone to check against. Only valid if a zone value is given.   *
 *                                                                                             *
 *          checkflagged -- Whether the cell's flagged status is checked (used when dropping). *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that is generally clear (legal to move to) that is close     *
 *          to the specified cell.                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/05/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL MapClass::Nearby_Location(CELL cell, SpeedType speed, int zone, MZoneType check, bool checkflagged) const
{
	CELL topten[10];
	int count = 0;
	int xx = Cell_X(cell);
	int yy = Cell_Y(cell);

	/*
	**	Determine the limits of the scanning in the four directions so that
	**	it won't scan past the edge of the world.
	*/
	int left = MapCellX;
	int right = MapCellX + MapCellWidth - 1;
	int top = MapCellY;
	int bottom = MapCellY + MapCellHeight - 1;

	/*
	**	Radiate outward from the specified location, looking for the closest
	**	location that is generally clear.
	*/
	for (int radius = 0; radius < MAP_CELL_W; radius++) {
		CELL newcell;
		CellClass const * cellptr;

		/*
		**	Scan the top and bottom rows of the "box".
		*/
		for (int x = xx-radius; x <= xx+radius; x++) {
			if (x >= left && x <= right) {
				int y = yy-radius;
				if (y >= top) {
					newcell = XY_Cell(x, y);
					cellptr = &Map[newcell];
					if (Map.In_Radar(newcell) && cellptr->Is_Clear_To_Move(speed, false, false, zone, check) && (!checkflagged || !cellptr->IsFlagged)) {
						topten[count++] = newcell;
					}
				}
				if (count == ARRAY_SIZE(topten)) break;

				y = yy+radius;
				if (y <= bottom) {
					newcell = XY_Cell(x, y);
					cellptr = &Map[newcell];
					if (Map.In_Radar(newcell) && cellptr->Is_Clear_To_Move(speed, false, false, zone, check) && (!checkflagged || !cellptr->IsFlagged)) {
						topten[count++] = newcell;
					}
				}
				if (count == ARRAY_SIZE(topten)) break;
			}
		}

		if (count == ARRAY_SIZE(topten)) break;

		/*
		**	Scan the left and right columns of the "box".
		*/
		for (int y = yy-radius; y <= yy+radius; y++) {
			if (y >= top && y <= bottom) {
				int x = xx-radius;
				if (x >= left) {
					newcell = XY_Cell(x, y);
					cellptr = &Map[newcell];
					if (Map.In_Radar(newcell) && cellptr->Is_Clear_To_Move(speed, false, false, zone, check) && (!checkflagged || !cellptr->IsFlagged)) {
						topten[count++] = newcell;
					}
				}
				if (count == ARRAY_SIZE(topten)) break;

				x = xx+radius;
				if (x <= right) {
					newcell = XY_Cell(x, y);
					cellptr = &Map[newcell];
					if (Map.In_Radar(newcell) && cellptr->Is_Clear_To_Move(speed, false, false, zone, check) && (!checkflagged || !cellptr->IsFlagged)) {
						topten[count++] = newcell;
					}
				}
				if (count == ARRAY_SIZE(topten)) break;
			}
		}

		if (count > 0) break;
	}

	if (count > 0) {
		return(topten[Frame % count]);
	}
	return(0);
}


/***********************************************************************************************
 * MapClass::Base_Region -- Finds the owner and base zone for specified cell.                  *
 *                                                                                             *
 *    This routine is used to determine what base the specified cell is close to and what      *
 *    zone of that base the cell lies in. This routine is particularly useful in letting the   *
 *    computer know when the player targets a destination near a computer's base.              *
 *                                                                                             *
 * INPUT:   cell     -- The cell that is to be checked.                                        *
 *                                                                                             *
 *          house    -- Reference to the house type number. This value will be set if a base   *
 *                      was found nearby the specified cell.                                   *
 *                                                                                             *
 *          zone     -- The zone that the cell is located in IF the cell is near a base.       *
 *                                                                                             *
 *                                                                                             *
 * OUTPUT:  Was a base near the specified cell found? If not, then the 'house' and 'zone'      *
 *          reference values are left in an undefined state and the return value will be       *
 *          false.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/05/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool MapClass::Base_Region(CELL cell, HousesType & house, ZoneType & zone) const
{
	if ((unsigned)cell < MAP_CELL_TOTAL && In_Radar(cell)) {
		for (house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
			HouseClass * h = HouseClass::As_Pointer(house);

			if (h && h->IsActive && !h->IsDefeated && h->Center) {
				zone = h->Which_Zone(cell);
				if (zone != ZONE_NONE) {
					return(true);
				}
			}
		}
	}
	return(false);
}


/***********************************************************************************************
 * MapClass::Destroy_Bridge_At -- Destroyes the bridge at location specified.                  *
 *                                                                                             *
 *    This routine will destroy the bridge at the location specified.                          *
 *                                                                                             *
 * INPUT:   cell  -- A cell that can uniquely identify the bridge.                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the bridge destroyed?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/

// Need to inform the server of the cell change so it can communicate with the clients - SKY
extern void On_Update_Map_Cell(int cell_x, int cell_y, const char* template_type_name);

struct CellUpdateStruct
{
	const TemplateTypeClass* Type;
	CELL Cell;
};

static const int MAX_UPDATES = 8;

static void Add_Cell_Update(CellUpdateStruct* updates, int& count, TemplateType type, CELL cell)
{
	new TemplateClass(type, cell);

	assert(count < MAX_UPDATES);
	if (count < MAX_UPDATES)
	{
		updates[count].Type = TemplateTypes.Ptr((int)type);
		updates[count].Cell = cell;
		count++;
	}
}

bool MapClass::Destroy_Bridge_At(CELL cell)
{
	bool destroyed = false;
	if (In_Radar(cell) && !Special.IsCaptureTheFlag) {
		CellClass * cellptr = &(*this)[cell];
		TemplateType ttype = cellptr->TType;

		CellUpdateStruct cell_updates[MAX_UPDATES];
		int update_count = 0;

		if (ttype == TEMPLATE_BRIDGE1 || ttype == TEMPLATE_BRIDGE2) {
			int icon = cellptr->TIcon;
			int w = TemplateTypeClass::As_Reference(ttype).Width;
			int h = TemplateTypeClass::As_Reference(ttype).Height;

			cell -= icon % w;
			cell -= MAP_CELL_W * (icon / w);

			if (ttype == TEMPLATE_BRIDGE1) {
				Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE1H, cell);
			} else {
				Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE2H, cell);
			}

			new AnimClass(ANIM_NAPALM3, Cell_Coord(cell + w/2 + (h/2)*MAP_CELL_W));
		}

		if (ttype == TEMPLATE_BRIDGE1H || ttype == TEMPLATE_BRIDGE2H) {
			int icon = cellptr->TIcon;
			int bridge_w = TemplateTypeClass::As_Reference(ttype).Width;
			int bridge_h = TemplateTypeClass::As_Reference(ttype).Height;

			cell -= icon % bridge_w;
			cell -= MAP_CELL_W * (icon / bridge_w);

			if (ttype == TEMPLATE_BRIDGE1H) {
				Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE1D, cell);
			} else {
				Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE2D, cell);
			}

			Scen.BridgeCount--;
			Scen.IsBridgeChanged = true;
			new AnimClass(ANIM_NAPALM3, Cell_Coord(cell + bridge_w/2 + (bridge_h/2)*MAP_CELL_W));
			Map.Zone_Reset(MZONEF_ALL);

			/*
			** Now, loop through all the bridge cells and find anyone standing
			** on a destroyed part (which is now river), and nuke them.
			*/
			CELL cell2 = cell;
			for (int y = 0; y < bridge_h; y++) {
				for (int x = 0; x < bridge_w; x++) {
					CellClass * bridge_cell = &(*this)[cell2];
					if (bridge_cell->TType == ttype) {

						/*
						**	Any unit that is firing on the bridge at this location, will stop
						**	firing because the bridge has been destroyed.
						*/
						Detach_This_From_All(As_Target(cell2), true);

						ObjectClass * obj = bridge_cell->Cell_Occupier();
						while (obj != NULL) {
							ObjectClass * next = obj->Next;
							if (obj->Is_Techno()) {
								int damage = obj->Strength;
								obj->Take_Damage(damage, 0, WARHEAD_HE, NULL, true);
							}
							obj = next;
						}
					}
					cell2++;
				}
				cell2 += MAP_CELL_W - bridge_w;
			}
			Shake_The_Screen(3);

			destroyed = true;
		} else {
			/*
			** All this code is for the multi-part bridges.
			*/
			if (ttype >= TEMPLATE_BRIDGE_1A && ttype <= TEMPLATE_BRIDGE_3E) {
				int icon = cellptr->TIcon;
				int w = TemplateTypeClass::As_Reference(ttype).Width;
				int h = TemplateTypeClass::As_Reference(ttype).Height;

				cell -= icon % w;
				cell -= MAP_CELL_W * (icon / w);
				switch (ttype) {
					case TEMPLATE_BRIDGE_1A:
					case TEMPLATE_BRIDGE_1B:
					case TEMPLATE_BRIDGE_2A:
					case TEMPLATE_BRIDGE_2B:
					case TEMPLATE_BRIDGE_3A:
					case TEMPLATE_BRIDGE_3B:
						ttype++;
						Add_Cell_Update(cell_updates, update_count, ttype, cell);
						break;
		 		}

				/*
				** If we were a middle piece that just got blown up, update the
				** adjoining pieces to make sure they're shaped properly.
				*/
				if (ttype == TEMPLATE_BRIDGE_3C) {
					// check the template below us, at x-1, y+1
					CELL cell2 = cell + (MAP_CELL_W - 1);
					CellClass * celptr = &(*this)[cell2];
					if (celptr->TType == TEMPLATE_BRIDGE_3C) {
						// It was also destroyed.  Update us and it.
						Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE_3D, cell);
						Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE_3E, cell2);
					}

					// Now check the template above us, at x+1, y-1.
					cell2 = cell - (MAP_CELL_W - 1);
					celptr = &(*this)[cell2];
					if (celptr->TType == TEMPLATE_BRIDGE_3C) {
						if (cellptr->TType == TEMPLATE_BRIDGE_3D) {
							// if we're already one-sided, turn us to all water
							Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE_3F, cell);
						} else {
							Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE_3E, cell);
						}
						Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE_3D, cell2);
					}
					Map.Zone_Reset(MZONEF_ALL);
				}

				/*
				** If we're an end bridge piece, update the adjoining piece to
				** be the proper shape.
				*/
				if (cellptr->TType == TEMPLATE_BRIDGE_1C) {
					Scen.BridgeCount--;
					Scen.IsBridgeChanged = true;

					// Point to the template below us, x-1, y+2
					CELL cell2 = cell + (MAP_CELL_W * 2) - 1;
					switch ((*this)[cell2].TType) {
						case TEMPLATE_BRIDGE_3A:
						case TEMPLATE_BRIDGE_3B:
						case TEMPLATE_BRIDGE_3C:
							Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE_3E, cell2);
							break;
						case TEMPLATE_BRIDGE_3D:
							Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE_3F, cell2);
							break;
					}
				} else {
					if (cellptr->TType == TEMPLATE_BRIDGE_2C) {
						// Point to the template above us, x+2, y-1
						CELL cell2 = cell - (MAP_CELL_W - 2);
						switch ((*this)[cell2].TType) {
							case TEMPLATE_BRIDGE_3A:
							case TEMPLATE_BRIDGE_3B:
							case TEMPLATE_BRIDGE_3C:
								Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE_3D, cell2);
								break;
							case TEMPLATE_BRIDGE_3E:
								Add_Cell_Update(cell_updates, update_count, TEMPLATE_BRIDGE_3F, cell2);
								break;
						}
					}
				}
				if (cellptr->TType == TEMPLATE_BRIDGE_1C ||
					cellptr->TType == TEMPLATE_BRIDGE_2C ||
				  (cellptr->TType >= TEMPLATE_BRIDGE_3C && cellptr->TType <= TEMPLATE_BRIDGE_3E)) {
					int x, y, tdata = 0;
					for (y = 0; y < h; y++) {
						for (x = 0; x < w; x++) {
							CellClass * ptr = &(*this)[(CELL)(cell + x)];
							if (ptr->TType == cellptr->TType || ptr->Land_Type() == LAND_RIVER || ptr->Land_Type() == LAND_WATER) {
								Detach_This_From_All(As_Target((CELL)(cell+tdata)), true);

								ObjectClass * obj = ptr->Cell_Occupier();
								while (obj != NULL) {
									ObjectClass * next = obj->Next;
									if (obj->Is_Techno()) {
										int damage = obj->Strength;
										obj->Take_Damage(damage, 0, WARHEAD_HE, NULL, true);
									}
									obj = next;
								}

							}
							tdata++;
						}
						cell += MAP_CELL_W;
					}
					Map.Zone_Reset(MZONEF_ALL);
					destroyed = true;
				}
				Shake_The_Screen(3);
			}
		}

		int cell_index = 0;
		char cell_name[_MAX_PATH] = { 0 };
		char icon_number[32] = { 0 };
		int icon = 0;
		void *image_data = 0;
		for (int i = 0; i < update_count; i++) {
			const TemplateTypeClass* type = cell_updates[i].Type;
			CELL cell = cell_updates[i].Cell;
			for (int y = 0; y < type->Height; y++) {
				for (int x = 0; x < type->Width; x++) {
					CELL newcell = cell + y * MAP_CELL_W + x;
					if (Map.In_Radar(newcell)) {
						CellClass * newcellptr = &Map[newcell];
						if (newcellptr->Get_Template_Info(cell_name, icon, image_data)) {
							itoa(icon, icon_number, 10);
							strncat(cell_name, "_i", 32);
							strncat(cell_name, icon_number, 32);
							strncat(cell_name, ".tga", 32);
							On_Update_Map_Cell(Cell_X(newcell), Cell_Y(newcell), cell_name);
						}
					}
				}
			}
		}
	}
	return(destroyed);
}


/***********************************************************************************************
 * MapClass::Detach -- Remove specified object from map references.                            *
 *                                                                                             *
 *    This routine will take the object (represented by a target value) and remove all         *
 *    references to it from the map. Typically, this is used to remove trigger reference.      *
 *                                                                                             *
 * INPUT:   target   -- The target object to remove from the map.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/28/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Detach(TARGET target, bool)
{
	/*
	**	Remove this trigger from the map zone/line tracking list.
	*/
	if (Is_Target_Trigger(target)) {
		for (int index = 0; index < MapTriggers.Count(); index++) {
			if (MapTriggers[index] == As_Trigger(target)) {
				MapTriggers.Delete(index);
				break;
			}
		}

		/*
		**	Loop through all cells; remove any reference to this trigger
		*/
		for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
			if ((*this)[cell].Trigger == As_Trigger(target)) {
				(*this)[cell].Trigger = NULL;
			}
		}
	}
}


/***********************************************************************************************
 * MapClass::Intact_Bridge_Count -- Determine the number of intact bridges.                    *
 *                                                                                             *
 *    This will examine the entire map and return the number of bridges that are intact. An    *
 *    intact bridge is one that units can travel over.                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of intact bridges on the map.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/28/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int MapClass::Intact_Bridge_Count(void) const
{
	/*
	**	Count all non-destroyed bridges on the map.
	*/
	int count = 0;
	CellClass const * cellptr = &(*this)[(CELL)0];
	for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		switch (cellptr->TType) {
			case TEMPLATE_BRIDGE1:
			case TEMPLATE_BRIDGE1H:
			case TEMPLATE_BRIDGE2:
			case TEMPLATE_BRIDGE2H:
			case TEMPLATE_BRIDGE_1A:
			case TEMPLATE_BRIDGE_1B:
				if (cellptr->TIcon == 6) {
					count++;
				}
				break;

			default:
				break;
		}

		cellptr++;
	}

	return(count);
}


/***********************************************************************************************
 * MapClass::Pick_Random_Location -- Picks a random location on the map.                       *
 *                                                                                             *
 *    This routine will pick a random location on the map. It performs no legality checking    *
 *    other than forcing the cell to be on the map proper.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a cell that is within the map.                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/25/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL MapClass::Pick_Random_Location(void) const
{
	int x = Map.MapCellX + Random_Pick(0, Map.MapCellWidth-1);
	int y = Map.MapCellY + Random_Pick(0, Map.MapCellHeight-1);

	return(XY_Cell(x, y));
}


#if (1)

/***********************************************************************************************
 * MapClass::Shroud_The_Map -- cover the whole map in darkness (usually from blackout crate)	  *
 *                                                                                             *
 * INPUT:   House to shroud                                                                    *
 *                                                                                             *
 * OUTPUT:  None
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/19/1996 BWG : Created.                                                                 *
 *   08/12/2019  ST : Updated for client/server multiplayer                                    *
 *=============================================================================================*/
void MapClass::Shroud_The_Map(HouseClass *house)
{
	for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		CellClass * cellptr = &Map[cell];
		if (cellptr->Is_Mapped(house) || cellptr->Is_Visible(house)) {
			
			cellptr->Redraw_Objects();
			
			/*
			** BG: remove "ring of darkness" around edge of map.
			*/
			int x = Cell_X(cell);
			int y = Cell_Y(cell);
			if (x >= Map.MapCellX && x < (Map.MapCellX + Map.MapCellWidth) &&
				y >= Map.MapCellY && y < (Map.MapCellY + Map.MapCellHeight)) {
				cellptr->Set_Mapped(house, false);
				cellptr->Set_Visible(house, false);
			}
		}
	}
	for (int obj_index = 0; obj_index < DisplayClass::Layer[LAYER_GROUND].Count(); obj_index++) {
		ObjectClass * layer_object = DisplayClass::Layer[LAYER_GROUND][obj_index];
		if (layer_object && layer_object->Is_Techno() && ((TechnoClass *)layer_object)->House == house) {
			layer_object->Look();
		}
	}
	Flag_To_Redraw(true);
}


#else

//
// Old code for posterity. ST - 8/12/2019 11:34AM
//

/***********************************************************************************************
 * MapClass::Shroud_The_Map -- cover the whole map in darkness (usually from blackout crate)	  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a cell that is within the map.                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/19/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void MapClass::Shroud_The_Map(void)
{
	for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		CellClass * cellptr = &Map[cell];
		if (cellptr->IsMapped || cellptr->IsVisible) {
			cellptr->Redraw_Objects();
			/*
			** BG: remove "ring of darkness" around edge of map.
			*/
			int x = Cell_X(cell);
			int y = Cell_Y(cell);
			if (x >= Map.MapCellX && x < (Map.MapCellX + Map.MapCellWidth) &&
				y >= Map.MapCellY && y < (Map.MapCellY + Map.MapCellHeight)) {
				cellptr->IsMapped = false;
				cellptr->IsVisible = false;
			}
		}
	}
	for (int obj_index = 0; obj_index < DisplayClass::Layer[LAYER_GROUND].Count(); obj_index++) {
		ObjectClass * layer_object = DisplayClass::Layer[LAYER_GROUND][obj_index];
		if (layer_object && layer_object->Is_Techno() && ((TechnoClass *)layer_object)->House == PlayerPtr) {
			layer_object->Look();
		}
	}
	Flag_To_Redraw(true);
}
#endif