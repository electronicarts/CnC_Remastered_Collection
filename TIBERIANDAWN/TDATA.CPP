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

/* $Header:   F:\projects\c&c\vcs\code\tdata.cpv   2.16   16 Oct 1995 16:52:04   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : May 8, 1995 [JLB]                                            *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TerrainTypeClass::Prep_For_Add -- Prepares to add terrain object.                         *
 *   TerrainTypeClass::Display -- Display a generic terrain object.                            *
 *   TerrainTypeClass::From_Name -- Convert name to terrain type.                              *
 *   TerrainTypeClass::Init -- Loads terrain object shape files.                               *
 *   TerrainTypeClass::Create_And_Place -- Creates and places terrain object on map.           *
 *   TerrainTypeClass::Create_On_Of -- Creates a terrain object from type.                     *
 *   TerrainTypeClass::TerrainTypeClass -- The general constructor for the terrain type objects*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"terrain.h"
#include	"type.h"


#define	TREE_NORMAL		600
#define	TREE_WEAK		400
#define	TREE_STRONG		800

static short const _List1[] = {0, REFRESH_EOL};
static short const _List000010[] = {MAP_CELL_W+1, REFRESH_EOL};
static short const _List000011101000[] = {MAP_CELL_W, MAP_CELL_W+1, MAP_CELL_W+2, MAP_CELL_W*2, REFRESH_EOL};
static short const _List00001[] = {4, REFRESH_EOL};
static short const _List000110[] = {MAP_CELL_W, MAP_CELL_W+1, REFRESH_EOL};
static short const _List000111[] = {MAP_CELL_W, MAP_CELL_W+1, MAP_CELL_W+2, REFRESH_EOL};
static short const _List001011100110[] = {2, MAP_CELL_W, MAP_CELL_W+1, MAP_CELL_W+2, MAP_CELL_W*2+1, MAP_CELL_W*2+2, REFRESH_EOL};
static short const _List0010[] = {MAP_CELL_W, REFRESH_EOL};
static short const _List0011[] = {MAP_CELL_W, MAP_CELL_W+1, REFRESH_EOL};
static short const _List001[] = {2, REFRESH_EOL};
static short const _List010110[] = {1, MAP_CELL_W, MAP_CELL_W+1, REFRESH_EOL};
static short const _List01[] = {1, REFRESH_EOL};
static short const _List1001[] = {0, MAP_CELL_W+1, REFRESH_EOL};
static short const _List101001[] = {0, 2, MAP_CELL_W+2, REFRESH_EOL};
static short const _List10[] = {0, REFRESH_EOL};
static short const _List110000011001[] = {0, 1, MAP_CELL_W+3, MAP_CELL_W*2, MAP_CELL_W*2+3, REFRESH_EOL};
static short const _List110000[] = {0, 1, REFRESH_EOL};
static short const _List110001[] = {0, 1, MAP_CELL_W+2, REFRESH_EOL};
static short const _List1100[] = {0, 1, REFRESH_EOL};
static short const _List110110[] = {0, 1, MAP_CELL_W, MAP_CELL_W+1, REFRESH_EOL};
static short const _List1101[] = {0, 1, MAP_CELL_W+1, REFRESH_EOL};
static short const _List111000010110[] = {0, 1, 2, MAP_CELL_W+3, MAP_CELL_W*2+1, MAP_CELL_W*2+2, REFRESH_EOL};
static short const _List111001[] = {0, 1, 2, MAP_CELL_W+2, REFRESH_EOL};
static short const _List111101[] = {0, 1, 2, MAP_CELL_W, MAP_CELL_W+2, REFRESH_EOL};
static short const _List11110[] = {0, 1, 2, 3, REFRESH_EOL};


static TerrainTypeClass const Tree1Class(
	TERRAIN_TREE1,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(11,41),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T01",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree2Class(
	TERRAIN_TREE2,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(11,44),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T02",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree3Class(
	TERRAIN_TREE3,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(12,45),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T03",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree4Class(
	TERRAIN_TREE4,
	THEATERF_DESERT,
	XYP_COORD(8,9),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T04",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List1,
	NULL
);


static TerrainTypeClass const Tree5Class(
	TERRAIN_TREE5,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(15,41),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T05",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree6Class(
	TERRAIN_TREE6,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(16,37),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T06",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree7Class(
	TERRAIN_TREE7,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(15,41),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T07",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree8Class(
	TERRAIN_TREE8,
	THEATERF_WINTER|THEATERF_TEMPERATE|THEATERF_DESERT,
	XYP_COORD(14,22),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T08",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List10,
	(short const *)_List01
);

static TerrainTypeClass const Tree9Class(
	TERRAIN_TREE9,
	THEATERF_DESERT,
	XYP_COORD(11,22),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T09",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List10,
	(short const *)_List01
);

static TerrainTypeClass const Tree10Class(
	TERRAIN_TREE10,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(25,43),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T10",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0011,
	(short const *)_List1100
);

static TerrainTypeClass const Tree11Class(
	TERRAIN_TREE11,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(23,44),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T11",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0011,
	(short const *)_List1100
);

static TerrainTypeClass const Tree12Class(
	TERRAIN_TREE12,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(14,36),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T12",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree13Class(
	TERRAIN_TREE13,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(19,40),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T13",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0010,
	(short const *)_List1101
);

static TerrainTypeClass const Tree14Class(
	TERRAIN_TREE14,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(19,40),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T14",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0011,
	(short const *)_List1100
);

static TerrainTypeClass const Tree15Class(
	TERRAIN_TREE15,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(19,40),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T15",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0011,
	(short const *)_List1100
);

static TerrainTypeClass const Tree16Class(
	TERRAIN_TREE16,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(13,36),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T16",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree17Class(
	TERRAIN_TREE17,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(18,44),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T17",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0010,
	(short const *)_List1001
);

static TerrainTypeClass const Tree18Class(
	TERRAIN_TREE18,
	THEATERF_DESERT,
	XYP_COORD(33,40),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		true,			// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		true,			// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		false,		// Is it immune to normal combat damage?
	"T18",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List000010,
	(short const *)_List111101
);

static TerrainTypeClass const Split1Class(
	TERRAIN_BLOSSOMTREE1,
	THEATERF_TEMPERATE|THEATERF_WINTER,
	XYP_COORD(18,44),	// Center base coordinate offset.
		true,			// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		true,			// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"SPLIT2",
	TXT_BLOSSOM_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0010,
	(short const *)_List1101
);

static TerrainTypeClass const Split2Class(
	TERRAIN_BLOSSOMTREE2,
	THEATERF_TEMPERATE|THEATERF_WINTER|THEATERF_DESERT,
	XYP_COORD(18,44),	// Center base coordinate offset.
		true,			// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		true,			// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"SPLIT3",
	TXT_BLOSSOM_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List0010,
	(short const *)_List1101
);

static TerrainTypeClass const Clump1Class(
	TERRAIN_CLUMP1,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(28,41),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"TC01",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List000110,
	(short const *)_List110001
);

static TerrainTypeClass const Clump2Class(
	TERRAIN_CLUMP2,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(38,41),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"TC02",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List010110,
	(short const *)_List101001
);

static TerrainTypeClass const Clump3Class(
	TERRAIN_CLUMP3,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(33,35),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"TC03",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List110110,
	(short const *)_List001
);

static TerrainTypeClass const Clump4Class(
	TERRAIN_CLUMP4,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(44,49),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"TC04",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List000011101000,
	(short const *)_List111000010110
);

static TerrainTypeClass const Clump5Class(
	TERRAIN_CLUMP5,
	THEATERF_WINTER|THEATERF_TEMPERATE,
	XYP_COORD(49,58),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"TC05",
	TXT_TREE,
	TREE_NORMAL,
	ARMOR_WOOD,
	(short const *)_List001011100110,
	(short const *)_List110000011001
);

static TerrainTypeClass const Rock1Class(
	TERRAIN_ROCK1,
	THEATERF_DESERT,
	XYP_COORD(33,41),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"ROCK1",
	TXT_ROCK,
	1000,
	ARMOR_STEEL,
	(short const *)_List0011,
	(short const *)_List111001
);

static TerrainTypeClass const Rock2Class(
	TERRAIN_ROCK2,
	THEATERF_DESERT,
	XYP_COORD(24,23),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"ROCK2",
	TXT_ROCK,
	1000,
	ARMOR_STEEL,
	(short const *)_List1100,
	(short const *)_List001
);

static TerrainTypeClass const Rock3Class(
	TERRAIN_ROCK3,
	THEATERF_DESERT,
	XYP_COORD(20,39),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"ROCK3",
	TXT_ROCK,
	1000,
	ARMOR_STEEL,
	(short const *)_List000110,
	(short const *)_List110001
);

static TerrainTypeClass const Rock4Class(
	TERRAIN_ROCK4,
	THEATERF_DESERT,
	XYP_COORD(12,20),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"ROCK4",
	TXT_ROCK,
	1000,
	ARMOR_STEEL,
	(short const *)_List10,
	(short const *)_List01
);

static TerrainTypeClass const Rock5Class(
	TERRAIN_ROCK5,
	THEATERF_DESERT,
	XYP_COORD(17,19),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"ROCK5",
	TXT_ROCK,
	1000,
	ARMOR_STEEL,
	(short const *)_List10,
	(short const *)_List01
);

static TerrainTypeClass const Rock6Class(
	TERRAIN_ROCK6,
	THEATERF_DESERT,
	XYP_COORD(28,40),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"ROCK6",
	TXT_ROCK,
	1000,
	ARMOR_STEEL,
	(short const *)_List000111,
	(short const *)_List110000
);

static TerrainTypeClass const Rock7Class(
	TERRAIN_ROCK7,
	THEATERF_DESERT,
	XYP_COORD(57,22),	// Center base coordinate offset.
		false,		// Spawns Tiberium spontaneously?
		false,		// Does it have destruction animation?
		false,		// Does it have transformation (blossom tree) anim?
		false,		// Does it catch fire?
		false,		// Is this object crushable by heavy vehicles?
		false,		// Can this object be selected by the player?
		false,		// Can it be the target of a move or attack order?
		true,			// Don't make a big deal about it if it gets destroyed?
		true,			// Is it immune to normal combat damage?
	"ROCK7",
	TXT_ROCK,
	1000,
	ARMOR_STEEL,
	(short const *)_List11110,
	(short const *)_List00001
);


TerrainTypeClass const * const TerrainTypeClass::Pointers[TERRAIN_COUNT] = {
	&Tree1Class,		//	TERRAIN_TREE1
	&Tree2Class,		//	TERRAIN_TREE2
	&Tree3Class,		//	TERRAIN_TREE3
	&Tree4Class,		//	TERRAIN_TREE4
	&Tree5Class,		//	TERRAIN_TREE5
	&Tree6Class,		//	TERRAIN_TREE6
	&Tree7Class,		//	TERRAIN_TREE7
	&Tree8Class,		//	TERRAIN_TREE8
	&Tree9Class,		//	TERRAIN_TREE9
	&Tree10Class,		//	TERRAIN_TREE10
	&Tree11Class,		//	TERRAIN_TREE11
	&Tree12Class,		//	TERRAIN_TREE12
	&Tree13Class,		//	TERRAIN_TREE13
	&Tree14Class,		//	TERRAIN_TREE14
	&Tree15Class,		//	TERRAIN_TREE15
	&Tree16Class,		//	TERRAIN_TREE16
	&Tree17Class,		//	TERRAIN_TREE17
	&Tree18Class,		//	TERRAIN_TREE18
	&Split1Class,		// TERRAIN_BLOSSOMTREE1
	&Split2Class,		// TERRAIN_BLOSSOMTREE2
	&Clump1Class,		//	TERRAIN_CLUMP1
	&Clump2Class,		//	TERRAIN_CLUMP2
	&Clump3Class,		//	TERRAIN_CLUMP3
	&Clump4Class,		//	TERRAIN_CLUMP4
	&Clump5Class,		//	TERRAIN_CLUMP5
	&Rock1Class,		//	TERRAIN_ROCK1
	&Rock2Class,		//	TERRAIN_ROCK2
	&Rock3Class,		//	TERRAIN_ROCK3
	&Rock4Class,		//	TERRAIN_ROCK4
	&Rock5Class,		//	TERRAIN_ROCK5
	&Rock6Class,		//	TERRAIN_ROCK6
	&Rock7Class			//	TERRAIN_ROCK7
};


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
	bool is_spawn,
	bool is_destroyable,
	bool is_transformable,
	bool is_flammable,
	bool is_crushable,
	bool is_selectable,
	bool is_legal_target,
	bool is_insignificant,
	bool is_immune,
	char const *ininame,
	int fullname, unsigned short strength, ArmorType armor, short const *occupy, short const *overlap) :
		ObjectTypeClass(true, is_flammable, is_crushable, true, is_selectable,
								is_legal_target, is_insignificant, is_immune,
								fullname, ininame, armor, strength)
{
	CenterBase = centerbase;
	IsTiberiumSpawn = is_spawn;
	IsDestroyable = is_destroyable;
	IsTransformable = is_transformable;
	Theater = theater;
	Type = terrain;
	Occupy = occupy;
	Overlap = overlap;
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
	if (theater != LastTheater){
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
				((void const *&)terrain.ImageData) = MixFileClass::Retrieve(fullname);

				IsTheaterShape = true;
				if (terrain.RadarIcon) delete[] (char *)terrain.RadarIcon;
				((void const *&)terrain.RadarIcon) = Get_Radar_Icon(terrain.Get_Image_Data(), 0, 1, 3);
				IsTheaterShape = false;
			}
		}
	}
}


/***********************************************************************************************
 * TerrainTypeClass::From_Name -- Convert name to terrain type.                                *
 *                                                                                             *
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

	if (name) {
		for (index = TERRAIN_FIRST; index < TERRAIN_COUNT; index++) {
			if (stricmp(name, Pointers[index]->IniName) == 0) {
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
	CC_Draw_Shape(Get_Image_Data(), 0, x, y, window, SHAPE_NORMAL|SHAPE_CENTER|SHAPE_WIN_REL);
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


short const * TerrainTypeClass::Occupy_List(bool ) const
{
	if (Occupy) return(Occupy);

	static short const _simple[1] = {
		REFRESH_EOL
	};
	return(&_simple[0]);
}


short const * TerrainTypeClass::Overlap_List(void) const
{
	if (Overlap) return(Overlap);

	static short const _simple[1] = {
		REFRESH_EOL
	};
	return(&_simple[0]);
}
