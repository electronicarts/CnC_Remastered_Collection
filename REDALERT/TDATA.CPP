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

/* $Header: /CounterStrike/TDATA.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 2, 1994                                                  *
 *                                                                                             *
 *                  Last Update : July 19, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TerrainTypeClass::As_Reference -- Fetches a reference to the terrain type object specified*
 *   TerrainTypeClass::Create_And_Place -- Creates and places terrain object on map.           *
 *   TerrainTypeClass::Create_On_Of -- Creates a terrain object from type.                     *
 *   TerrainTypeClass::Display -- Display a generic terrain object.                            *
 *   TerrainTypeClass::From_Name -- Convert name to terrain type.                              *
 *   TerrainTypeClass::Init -- Loads terrain object shape files.                               *
 *   TerrainTypeClass::Init_Heap -- Initialize the terrain object heap.                        *
 *   TerrainTypeClass::Occupy_List -- Returns with the occupy list for the terrain object type.*
 *   TerrainTypeClass::One_Time -- Performs any special one time processing for terrain types. *
 *   TerrainTypeClass::Overlap_List -- Fetches the overlap list for the terrain type.          *
 *   TerrainTypeClass::Prep_For_Add -- Prepares to add terrain object.                         *
 *   TerrainTypeClass::TerrainTypeClass -- The general constructor for the terrain type objects*
 *   TerrainTypeClass::operator delete -- Returns a terrain type object back to the mem pool.  *
 *   TerrainTypeClass::operator new -- Allocates a terrain type object from special pool.      *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"terrain.h"
#include	"type.h"


static short const _List000011101000[] = {MAP_CELL_W, MAP_CELL_W+1, MAP_CELL_W+2, MAP_CELL_W*2, REFRESH_EOL};
static short const _List000110[] = {MAP_CELL_W, MAP_CELL_W+1, REFRESH_EOL};
static short const _List001011100110[] = {2, MAP_CELL_W, MAP_CELL_W+1, MAP_CELL_W+2, MAP_CELL_W*2+1, MAP_CELL_W*2+2, REFRESH_EOL};
static short const _List0010[] = {MAP_CELL_W, REFRESH_EOL};
static short const _List0011[] = {MAP_CELL_W, MAP_CELL_W+1, REFRESH_EOL};
static short const _List001[] = {2, REFRESH_EOL};
static short const _List010110[] = {1, MAP_CELL_W, MAP_CELL_W+1, REFRESH_EOL};
static short const _List01[] = {1, REFRESH_EOL};
static short const _List11[] = {0, 1, REFRESH_EOL};
static short const _List1001[] = {0, MAP_CELL_W+1, REFRESH_EOL};
static short const _List1010[] = {0, MAP_CELL_W, REFRESH_EOL};
static short const _List101001[] = {0, 2, MAP_CELL_W+2, REFRESH_EOL};
static short const _List10[] = {0, REFRESH_EOL};
static short const _List110000011001[] = {0, 1, MAP_CELL_W+3, MAP_CELL_W*2, MAP_CELL_W*2+3, REFRESH_EOL};
static short const _List110001[] = {0, 1, MAP_CELL_W+2, REFRESH_EOL};
static short const _List1100[] = {0, 1, REFRESH_EOL};
static short const _List110110[] = {0, 1, MAP_CELL_W, MAP_CELL_W+1, REFRESH_EOL};
static short const _List1101[] = {0, 1, MAP_CELL_W+1, REFRESH_EOL};
static short const _List1111[] = {0, 1, MAP_CELL_W, MAP_CELL_W+1, REFRESH_EOL};
static short const _List111000010110[] = {0, 1, 2, MAP_CELL_W+3, MAP_CELL_W*2+1, MAP_CELL_W*2+2, REFRESH_EOL};


static TerrainTypeClass const Mine(
	TERRAIN_MINE,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(12,24),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"MINE",
	TXT_ORE_MINE,
	(short const *)_List10,
	NULL
);
static TerrainTypeClass const Boxes01(
	TERRAIN_BOXES01,
	THEATERF_INTERIOR,
	XYP_COORD(12,24),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"BOXES01",
	TXT_CRATES,
	(short const *)_List10,
	NULL
);
static TerrainTypeClass const Boxes02(
	TERRAIN_BOXES02,
	THEATERF_INTERIOR,
	XYP_COORD(12,24),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"BOXES02",
	TXT_CRATES,
	(short const *)_List10,
	NULL
);
static TerrainTypeClass const Boxes03(
	TERRAIN_BOXES03,
	THEATERF_INTERIOR,
	XYP_COORD(12,24),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"BOXES03",
	TXT_CRATES,
	(short const *)_List10,
	NULL
);
static TerrainTypeClass const Boxes04(
	TERRAIN_BOXES04,
	THEATERF_INTERIOR,
	XYP_COORD(12,24),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"BOXES04",
	TXT_CRATES,
	(short const *)_List10,
	NULL
);
static TerrainTypeClass const Boxes05(
	TERRAIN_BOXES05,
	THEATERF_INTERIOR,
	XYP_COORD(12,24),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"BOXES05",
	TXT_CRATES,
	(short const *)_List10,
	NULL
);
static TerrainTypeClass const Boxes06(
	TERRAIN_BOXES06,
	THEATERF_INTERIOR,
	XYP_COORD(12,24),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"BOXES06",
	TXT_CRATES,
	(short const *)_List10,
	NULL
);
static TerrainTypeClass const Boxes07(
	TERRAIN_BOXES07,
	THEATERF_INTERIOR,
	XYP_COORD(12,24),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"BOXES07",
	TXT_CRATES,
	(short const *)_List10,
	NULL
);
static TerrainTypeClass const Boxes08(
	TERRAIN_BOXES08,
	THEATERF_INTERIOR,
	XYP_COORD(12,24),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"BOXES08",
	TXT_CRATES,
	(short const *)_List10,
	NULL
);
static TerrainTypeClass const Boxes09(
	TERRAIN_BOXES09,
	THEATERF_INTERIOR,
	XYP_COORD(12,24),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"BOXES09",
	TXT_CRATES,
	(short const *)_List10,
	NULL
);

static TerrainTypeClass const Ice01(
	TERRAIN_ICE01,
	THEATERF_SNOW,
	XYP_COORD(24,24),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		true,					// Is based on the water?
	"ICE01",
	TXT_ICE,
	(short const *)_List1111,
	NULL
);
static TerrainTypeClass const Ice02(
	TERRAIN_ICE02,
	THEATERF_SNOW,
	XYP_COORD(12,24),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		true,					// Is based on the water?
	"ICE02",
	TXT_ICE,
	(short const *)_List1010,
	NULL
);
static TerrainTypeClass const Ice03(
	TERRAIN_ICE03,
	THEATERF_SNOW,
	XYP_COORD(24,12),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		true,					// Is based on the water?
	"ICE03",
	TXT_ICE,
	(short const *)_List11,
	NULL
);
static TerrainTypeClass const Ice04(
	TERRAIN_ICE04,
	THEATERF_SNOW,
	XYP_COORD(12,12),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		true,					// Is based on the water?
	"ICE04",
	TXT_ICE,
	(short const *)_List10,
	NULL
);
static TerrainTypeClass const Ice05(
	TERRAIN_ICE05,
	THEATERF_SNOW,
	XYP_COORD(12,12),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		true,					// Is based on the water?
	"ICE05",
	TXT_ICE,
	(short const *)_List10,
	NULL
);

static TerrainTypeClass const Tree1Class(
	TERRAIN_TREE1,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(11,41),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T01",
	TXT_TREE,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree2Class(
	TERRAIN_TREE2,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(11,44),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T02",
	TXT_TREE,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree3Class(
	TERRAIN_TREE3,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(12,45),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T03",
	TXT_TREE,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree5Class(
	TERRAIN_TREE5,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(15,41),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T05",
	TXT_TREE,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree6Class(
	TERRAIN_TREE6,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(16,37),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T06",
	TXT_TREE,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree7Class(
	TERRAIN_TREE7,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(15,41),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T07",
	TXT_TREE,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree8Class(
	TERRAIN_TREE8,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(14,22),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T08",
	TXT_TREE,
	(short const *)_List10,
	(short const *)_List01
);

static TerrainTypeClass const Tree10Class(
	TERRAIN_TREE10,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(25,43),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T10",
	TXT_TREE,
	(short const *)_List0011,
	(short const *)_List1100
);

static TerrainTypeClass const Tree11Class(
	TERRAIN_TREE11,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(23,44),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T11",
	TXT_TREE,
	(short const *)_List0011,
	(short const *)_List1100
);

static TerrainTypeClass const Tree12Class(
	TERRAIN_TREE12,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(14,36),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T12",
	TXT_TREE,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree13Class(
	TERRAIN_TREE13,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(19,40),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T13",
	TXT_TREE,
	(short const *)_List0010,
	(short const *)_List1101
);

static TerrainTypeClass const Tree14Class(
	TERRAIN_TREE14,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(19,40),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T14",
	TXT_TREE,
	(short const *)_List0011,
	(short const *)_List1100
);

static TerrainTypeClass const Tree15Class(
	TERRAIN_TREE15,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(19,40),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T15",
	TXT_TREE,
	(short const *)_List0011,
	(short const *)_List1100
);

static TerrainTypeClass const Tree16Class(
	TERRAIN_TREE16,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(13,36),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T16",
	TXT_TREE,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree17Class(
	TERRAIN_TREE17,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(18,44),		// Center base coordinate offset.
		false,				// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"T17",
	TXT_TREE,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Clump1Class(
	TERRAIN_CLUMP1,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(28,41),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"TC01",
	TXT_TREE,
	(short const *)_List000110,
	(short const *)_List110001
);

static TerrainTypeClass const Clump2Class(
	TERRAIN_CLUMP2,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(38,41),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"TC02",
	TXT_TREE,
	(short const *)_List010110,
	(short const *)_List101001
);

static TerrainTypeClass const Clump3Class(
	TERRAIN_CLUMP3,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(33,35),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"TC03",
	TXT_TREE,
	(short const *)_List110110,
	(short const *)_List001
);

static TerrainTypeClass const Clump4Class(
	TERRAIN_CLUMP4,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(44,49),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"TC04",
	TXT_TREE,
	(short const *)_List000011101000,
	(short const *)_List111000010110
);

static TerrainTypeClass const Clump5Class(
	TERRAIN_CLUMP5,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	XYP_COORD(49,58),		// Center base coordinate offset.
		true,					// Is it immune to normal combat damage?
		false,				// Is based on the water?
	"TC05",
	TXT_TREE,
	(short const *)_List001011100110,
	(short const *)_List110000011001
);


/***********************************************************************************************
 * TerrainTypeClass::TerrainTypeClass -- The general constructor for the terrain type objects. *
 *                                                                                             *
 *    This is the constructor for terrain type objects. It is only used to construct the       *
 *    static (constant) terrain type objects.                                                  *
 *                                                                                             *
 * INPUT:   see below..                                                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TerrainTypeClass::TerrainTypeClass(
	TerrainType terrain,
	int theater,
	COORDINATE centerbase,
	bool is_immune,
	bool is_water,
	char const * ininame,
	int fullname,
	short const * occupy,
	short const * overlap) :
		ObjectTypeClass(RTTI_TERRAINTYPE,
							int(terrain),
							true,
							true,
							false,
							false,
							true,
							is_immune,
							true,
							fullname,
							ininame),
	Type(terrain),
	CenterBase(centerbase),
	Theater(theater),
	IsWaterBased(is_water),
	Occupy(occupy),
	Overlap(overlap)
{
	MaxStrength = 800;
	Armor = ARMOR_WOOD;
}


/***********************************************************************************************
 * TerrainTypeClass::operator new -- Allocates a terrain type object from special pool.        *
 *                                                                                             *
 *    This routine will allocated a terrain type class object from the memory pool set up      *
 *    for that purpose.                                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the freshly allocated terrain type object block. If      *
 *          there was insufficient memory, then NULL is returned.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * TerrainTypeClass::operator new(size_t)
{
	return(TerrainTypes.Alloc());
}


/***********************************************************************************************
 * TerrainTypeClass::operator delete -- Returns a terrain type object back to the mem pool.    *
 *                                                                                             *
 *    This routine will return the supplied terrain type object back to the special memory     *
 *    pool for whence it was originally allocated.                                             *
 *                                                                                             *
 * INPUT:   pointer  -- Pointer to the terrain type object to return to the memory pool.       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainTypeClass::operator delete(void * pointer)
{
	TerrainTypes.Free((TerrainTypeClass *)pointer);
}


/***********************************************************************************************
 * TerrainTypeClass::Init_Heap -- Initialize the terrain object heap.                          *
 *                                                                                             *
 *    This routine preallocates the terrain type objects in the memory pool. It must be called *
 *    before the terrain type object data can be filled in.                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This must be called only once and before the rules.ini file is processed.       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainTypeClass::Init_Heap(void)
{
	/*
	**	These terrain type class objects must be allocated in the exact order that they
	**	are specified in the TerrainType enumeration. This is necessary because the heap
	**	allocation block index serves double duty as the type number index.
	*/
	new TerrainTypeClass(Tree1Class);		//	TERRAIN_TREE1
	new TerrainTypeClass(Tree2Class);		//	TERRAIN_TREE2
	new TerrainTypeClass(Tree3Class);		//	TERRAIN_TREE3
	new TerrainTypeClass(Tree5Class);		//	TERRAIN_TREE5
	new TerrainTypeClass(Tree6Class);		//	TERRAIN_TREE6
	new TerrainTypeClass(Tree7Class);		//	TERRAIN_TREE7
	new TerrainTypeClass(Tree8Class);		//	TERRAIN_TREE8
	new TerrainTypeClass(Tree10Class);		//	TERRAIN_TREE10
	new TerrainTypeClass(Tree11Class);		//	TERRAIN_TREE11
	new TerrainTypeClass(Tree12Class);		//	TERRAIN_TREE12
	new TerrainTypeClass(Tree13Class);		//	TERRAIN_TREE13
	new TerrainTypeClass(Tree14Class);		//	TERRAIN_TREE14
	new TerrainTypeClass(Tree15Class);		//	TERRAIN_TREE15
	new TerrainTypeClass(Tree16Class);		//	TERRAIN_TREE16
	new TerrainTypeClass(Tree17Class);		//	TERRAIN_TREE17
	new TerrainTypeClass(Clump1Class);		//	TERRAIN_CLUMP1
	new TerrainTypeClass(Clump2Class);		//	TERRAIN_CLUMP2
	new TerrainTypeClass(Clump3Class);		//	TERRAIN_CLUMP3
	new TerrainTypeClass(Clump4Class);		//	TERRAIN_CLUMP4
	new TerrainTypeClass(Clump5Class);		//	TERRAIN_CLUMP5
	new TerrainTypeClass(Ice01);				//	TERRAIN_ICE01
	new TerrainTypeClass(Ice02);				//	TERRAIN_ICE02
	new TerrainTypeClass(Ice03);				//	TERRAIN_ICE03
	new TerrainTypeClass(Ice04);				//	TERRAIN_ICE04
	new TerrainTypeClass(Ice05);				//	TERRAIN_ICE05
	new TerrainTypeClass(Boxes01);			//	TERRAIN_BOXES01
	new TerrainTypeClass(Boxes02);			//	TERRAIN_BOXES02
	new TerrainTypeClass(Boxes03);			//	TERRAIN_BOXES03
	new TerrainTypeClass(Boxes04);			//	TERRAIN_BOXES04
	new TerrainTypeClass(Boxes05);			//	TERRAIN_BOXES05
	new TerrainTypeClass(Boxes06);			//	TERRAIN_BOXES06
	new TerrainTypeClass(Boxes07);			//	TERRAIN_BOXES07
	new TerrainTypeClass(Boxes08);			//	TERRAIN_BOXES08
	new TerrainTypeClass(Boxes09);			//	TERRAIN_BOXES09
	new TerrainTypeClass(Mine);				// TERRAIN_MINE
}


/***********************************************************************************************
 * TerrainTypeClass::One_Time -- Performs any special one time processing for terrain types.   *
 *                                                                                             *
 *    This routine will perform any special one time processing needed for the terrain         *
 *    object types. Typically, this would load up artwork for terrain objects that have        *
 *    artwork independant of the theater they appear in.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainTypeClass::One_Time(void)
{
}

/***********************************************************************************************
 * TerrainTypeClass::Init -- Loads terrain object shape files.                                 *
 *                                                                                             *
 *    This routine is used to load up the terrain object shape files.                          *
 *    The shape files loaded depends on theater.                                               *
 *                                                                                             *
 * INPUT:   theater  -- The theater to load the terrain shape data for.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/16/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainTypeClass::Init(TheaterType theater)
{
	if (theater != LastTheater) {

		for (TerrainType index = TERRAIN_FIRST; index < TERRAIN_COUNT; index++) {
			TerrainTypeClass const & terrain = As_Reference(index);
			char	fullname[_MAX_FNAME+_MAX_EXT];

			/*
			**	Clear any existing shape pointer. All terrain is theater specific, thus if
			**	it isn't loaded in this routine, it shouldn't exist at all.
			*/
			((void const *&)terrain.ImageData) = NULL;

			if (terrain.Theater & (1 << theater)) {

				/*
				**	Load in the appropriate object shape data.
				*/
				_makepath(fullname, NULL, NULL, terrain.IniName, Theaters[theater].Suffix);
				((void const *&)terrain.ImageData) = MFCD::Retrieve(fullname);

				IsTheaterShape = true;	//Let Build_Frame know that this is a theater specific shape
				if (terrain.RadarIcon != NULL) delete[] (char *)terrain.RadarIcon;
				((void const *&)terrain.RadarIcon) = Get_Radar_Icon(terrain.Get_Image_Data(), 0, 1, 3);
				IsTheaterShape = false;

			}
		}
	}
}


/***********************************************************************************************
 * TerrainTypeClass::From_Name -- Convert name to terrain type.                                *
 *                                                                                             *
 *    This routine is used to convert a text name into the matching                            *
 *    terrain type number. This is used during scenario initialization.                        *
 *                                                                                             *
 * INPUT:   name  -- The name to convert.                                                      *
 *                                                                                             *
 * OUTPUT:  Returns the TerrainType that matches the name specified. If                        *
 *          no match was found, then TERRAIN_NONE is returned.                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/16/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TerrainType TerrainTypeClass::From_Name(char const * name)
{
	TerrainType	index;

	if (name != NULL) {
		for (index = TERRAIN_FIRST; index < TERRAIN_COUNT; index++) {
			if (stricmp(name, As_Reference(index).IniName) == 0) {
				return(index);
			}
		}
	}
	return(TERRAIN_NONE);
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * TerrainTypeClass::Display -- Display a generic terrain object.                              *
 *                                                                                             *
 *    This routine is used to display a generic terrain object. Typical                        *
 *    use is during scenario editing.                                                          *
 *                                                                                             *
 * INPUT:   x,y   -- Pixel coordinates to display object at (centered).                        *
 *                                                                                             *
 *          window-- The window to display the object within.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/16/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainTypeClass::Display(int x, int y, WindowNumberType window, HousesType) const
{
	IsTheaterShape = true;
	CC_Draw_Shape(Get_Image_Data(), 0, x, y, window, SHAPE_NORMAL|SHAPE_CENTER|SHAPE_WIN_REL);
	IsTheaterShape = false;
}


/***********************************************************************************************
 * TerrainTypeClass::Prep_For_Add -- Prepares to add terrain object.                           *
 *                                                                                             *
 *    Submits all of the valid terrain objects to the scenario editor for possible selection   *
 *    and subsequent placement on the map. All terrain objects, that have a valid shape        *
 *    file available, are added.                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainTypeClass::Prep_For_Add(void)
{
	for (TerrainType index = TERRAIN_FIRST; index < TERRAIN_COUNT; index++) {
		if (As_Reference(index).Get_Image_Data()) {
			Map.Add_To_List(&As_Reference(index));
		}
	}
}
#endif


/***********************************************************************************************
 * TerrainTypeClass::Create_And_Place -- Creates and places terrain object on map.             *
 *                                                                                             *
 *    This support routine is used by the scenario editor to add a terrain object to the map.  *
 *                                                                                             *
 * INPUT:   cell  -- The cell to place the terrain object in.                                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the placement successful?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TerrainTypeClass::Create_And_Place(CELL cell, HousesType ) const
{
	if (new TerrainClass(Type, cell)) {
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TerrainTypeClass::Create_On_Of -- Creates a terrain object from type.                       *
 *                                                                                             *
 *    This is used to create a terrain object by using the terrain type as a guide. This       *
 *    routine is typically used by the scenario editor in order to place a terrain object      *
 *    onto the map.                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the created terrain object or NULL if one couldn't be    *
 *          created.                                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * TerrainTypeClass::Create_One_Of(HouseClass *) const
{
	return(new TerrainClass(Type, -1));
}


/***********************************************************************************************
 * TerrainTypeClass::Occupy_List -- Returns with the occupy list for the terrain object type.  *
 *                                                                                             *
 *    This routine will return with the occupy list for the terrain object type. If there is   *
 *    no occupy list for this terrain object type, then a special zero length occupy list      *
 *    pointer is returned.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the terrain object's occupy list. A zero length list is  *
 *          returned in the case of no occupy list.                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * TerrainTypeClass::Occupy_List(bool ) const
{
	if (Occupy != NULL) return(Occupy);

	static short const _simple[1] = {
		REFRESH_EOL
	};
	return(&_simple[0]);
}


/***********************************************************************************************
 * TerrainTypeClass::Overlap_List -- Fetches the overlap list for the terrain type.            *
 *                                                                                             *
 *    This routine will return with the overlap list for the terrain object type. If there     *
 *    is no overlap list for the terrain object, then a null length list is returned in order  *
 *    to ensure that a non-null pointer is returned.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the overlap list for this object type. A special zero    *
 *          length list pointer is returned if there is no overlap list associated with the    *
 *          object type.                                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * TerrainTypeClass::Overlap_List(void) const
{
	if (Overlap != NULL) return(Overlap);

	static short const _simple[1] = {
		REFRESH_EOL
	};
	return(&_simple[0]);
}


/***********************************************************************************************
 * TerrainTypeClass::As_Reference -- Fetches a reference to the terrain type object specified. *
 *                                                                                             *
 *    Use this routine to convert the terrain type number into a reference to a terrain        *
 *    type class object.                                                                       *
 *                                                                                             *
 * INPUT:   type  -- The terrain type number to convert.                                       *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the terrain type class object that is referred to by   *
 *          the terrain type number.                                                           *
 *                                                                                             *
 * WARNINGS:   Be sure that the terrain type number is valid. Using an invalid value causes    *
 *             undefined behavior.                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TerrainTypeClass & TerrainTypeClass::As_Reference(TerrainType type)
{
	return(*TerrainTypes.Ptr(type));
}


COORDINATE TerrainTypeClass::Coord_Fixup(COORDINATE coord) const
{
	return Coord_Whole(coord);
}
