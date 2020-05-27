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

/* $Header: /CounterStrike/BDATA.CPP 2     3/03/97 10:37p Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : October 2, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BuildingTypeClass::As_Reference -- Fetches reference to the building type specified.      *
 *   BuildingTypeClass::Bib_And_Offset -- Determines the bib and appropriate cell offset.      *
 *   BuildingTypeClass::BuildingTypeClass -- This is the constructor for the building types.   *
 *   BuildingTypeClass::Coord_Fixup -- Adjusts coordinate to be legal for assignment.          *
 *   BuildingTypeClass::Cost_Of -- Fetches the cost of this building.                          *
 *   BuildingTypeClass::Create_And_Place -- Creates and places a building object onto the map. *
 *   BuildingTypeClass::Create_One_Of -- Creates a building of this type.                      *
 *   BuildingTypeClass::Dimensions -- Fetches the pixel dimensions of the building.            *
 *   BuildingTypeClass::Display -- Renders a generic view of building.                         *
 *   BuildingTypeClass::Flush_For_Placement -- Tries to clear placement area for this building *
 *   BuildingTypeClass::Full_Name -- Fetches the name to give this building.                   *
 *   BuildingTypeClass::Height -- Determines the height of the building in icons.              *
 *   BuildingTypeClass::Init -- Performs theater specific initialization.                      *
 *   BuildingTypeClass::Init_Anim -- Initialize an animation control for a building.           *
 *   BuildingTypeClass::Init_Heap -- Initialize the heap as necessary for the building type obj*
 *   BuildingTypeClass::Max_Pips -- Determines the maximum pips to display.                    *
 *   BuildingTypeClass::Occupy_List -- Fetches the occupy list for the building.               *
 *   BuildingTypeClass::One_Time -- Performs special one time action for buildings.            *
 *   BuildingTypeClass::Overlap_List -- Fetches the overlap list for the building.             *
 *   BuildingTypeClass::Prep_For_Add -- Prepares scenario editor for adding an object.         *
 *   BuildingTypeClass::Raw_Cost -- Fetches the raw (base) cost of this building type.         *
 *   BuildingTypeClass::Read_INI -- Fetch building type data from the INI database.            *
 *   BuildingTypeClass::Width -- Determines width of building in icons.                        *
 *   BuildingTypeClass::operator delete -- Deletes a building type object from the special heap*
 *   BuildingTypeClass::operator new -- Allocates a building type object from the special heap.*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


#define FATSHIP

#define	MCW	MAP_CELL_W

#define XYCELL(x,y)	(y*MAP_CELL_W+x)
static short const ExitPyle[] = {
	XYCELL(1,2),
	XYCELL(2,2),
	XYCELL(0,2),
	XYCELL(-1,2),
	XYCELL(-1,-1),
	XYCELL(0,-1),
	XYCELL(1,-1),
	XYCELL(2,-1),
	XYCELL(2,-1),
	XYCELL(-1,0),
	XYCELL(2,0),
	XYCELL(2,1),
	XYCELL(-1,1),
	REFRESH_EOL
};

static short const ExitSub[] = {
	XYCELL( 0, 2),
	XYCELL( 2, 2),
	XYCELL(-1, 2),
	XYCELL( 1, 2),
	XYCELL( 3, 2)
};

static short const ExitWeap[] = {
	XYCELL(1,2),
	XYCELL(-1,3),
	XYCELL(0,3),
	XYCELL(1,3),
	XYCELL(-2,3),
	XYCELL(2,3),
	REFRESH_EOL
};

static short const ComList[] = {0, 1, MCW, MCW+1, REFRESH_EOL};
static short const List000111111[] = {(MCW*1), (MCW*1)+1, (MCW*1)+2, (MCW*2), (MCW*2)+1, (MCW*2)+2, REFRESH_EOL};
static short const List0010[] = {MCW, REFRESH_EOL};
static short const List0011[] = {(MCW*1), (MCW*1)+1, REFRESH_EOL};
static short const List010111100[] = {1, (MCW*1), (MCW*1)+1, (MCW*1)+2, (MCW*2), REFRESH_EOL};
static short const List0111[] = {1, (MCW*1), (MCW*1)+1, REFRESH_EOL};
static short const List1000[] = {0, REFRESH_EOL};
static short const List101000011[] = {0, 2, (MCW*2)+1, (MCW*2)+2, REFRESH_EOL};
static short const List1100[] = {0, 1, REFRESH_EOL};
static short const List1101[] = {0, 1, (MCW*1)+1, REFRESH_EOL};
static short const List11[] = {0, 1, REFRESH_EOL};
static short const List12[] = {MCW, REFRESH_EOL};
static short const List1[] = {0, REFRESH_EOL};
static short const List21[] = {0, 1, REFRESH_EOL};
static short const List22[] = {0, 1, MCW, MCW+1, REFRESH_EOL};
static short const List22_0011[] = {MCW, MCW+1, REFRESH_EOL};
static short const List22_1100[] = {0, 1, REFRESH_EOL};
static short const List2[] = {0, 1, MCW+1, MCW, REFRESH_EOL};
static short const List32[] = {0, 1, 2, MCW, MCW+1, MCW+2, REFRESH_EOL};
//static short const List42[] = {0, 1, 2, 3, MCW, MCW+1, MCW+2, MCW+3, REFRESH_EOL};
static short const ListFix[] = {1, MCW, MCW+1, MCW+2, MCW+MCW+1, REFRESH_EOL};
static short const ListWeap[] = {0, 1, 2, (MCW*1), (MCW*1)+1, (MCW*1)+2, REFRESH_EOL};
static short const ListWestwood[] = {1, 2, 3, MCW+1, MCW+2, MCW+3, REFRESH_EOL};
static short const OListSAM[] = {-MCW, -(MCW-1), REFRESH_EOL};
#ifdef FATSHIP
static short const ListSPen[] = {0, 1, 2, MCW, MCW+1, MCW+2, MCW+MCW, MCW+MCW+1, MCW+MCW+2, REFRESH_EOL};
static short const OListSPen[] = {REFRESH_EOL};
#else
static short const ListSPen[] = {1, MCW, MCW+1, MCW+2, MCW+MCW+1, REFRESH_EOL};
static short const OListSPen[] = {0, 2, MCW+MCW, MCW+MCW+2, REFRESH_EOL};
#endif
static short const OListWestwood[] = {0, MCW, REFRESH_EOL};
static short const StoreList[] = {0, REFRESH_EOL};

static short const ListFactory[] = {0, 1, 2, (MCW*1), (MCW*1)+1, (MCW*1)+2, (MCW*2), (MCW*2)+1, (MCW*2)+2, REFRESH_EOL};

static short const OListFix[] = {0, 2, MCW+MCW, MCW+MCW+2, REFRESH_EOL};
static short const OListWeap[] = {REFRESH_EOL};
static short const OComList[] = {1, REFRESH_EOL};
static short const OList12[] = {0, REFRESH_EOL};
static short const OListTmpl[] = {0, 1, 2, REFRESH_EOL};


/***************************************************************************
*/
static BuildingTypeClass const ClassBarrel(
	STRUCT_BARREL,
	TXT_BARREL,						// NAME:			Short name of the structure.
	"BARL",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassBarrel3(
	STRUCT_BARREL3,
	TXT_BARREL,						// NAME:			Short name of the structure.
	"BRL3",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		false,						// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassAVMine(
	STRUCT_AVMINE,
	TXT_AVMINE,						// NAME:			Short name of the structure.
	"MINV",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_NORMAL,					// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		false,						// Can the player select this?
		false,						// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassAPMine(
	STRUCT_APMINE,
	TXT_APMINE,						// NAME:			Short name of the structure.
	"MINP",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_NORMAL,					// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		false,						// Can the player select this?
		false,						// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassIronCurtain(
	STRUCT_IRON_CURTAIN,
	TXT_IRON_CURTAIN,				// NAME:			Short name of the structure.
	"IRON",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List22_0011,// OCCUPYLIST:	List of active foundation squares.
	(short const *)List22_1100	// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassForwardCom(
	STRUCT_FORWARD_COM,
	TXT_FORWARD_COM,				// NAME:			Short name of the structure.
	"FCOM",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List22_0011,// OCCUPYLIST:	List of active foundation squares.
	(short const *)List22_1100	// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassAdvancedTech(
	STRUCT_ADVANCED_TECH,
	TXT_ADVANCED_TECH,			// NAME:			Short name of the structure.
	"ATEK",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List22,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassChronosphere(
	STRUCT_CHRONOSPHERE,
	TXT_CHRONOSPHERE,				// NAME:			Short name of the structure.
	"PDOX",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List22,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassWeapon(
	STRUCT_WEAP,
	TXT_WEAPON_FACTORY,			// NAME:			Short name of the structure.
	"WEAP",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XY_Coord(CELL_LEPTON_W+(CELL_LEPTON_W/2), CELL_LEPTON_H),			// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_UNITTYPE,					// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_32, 						// SIZE:			Building size.
	(short const *)ExitWeap,	// Preferred exit cell list.
	(short const *)ListWeap,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListWeap	// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassShipYard(
	STRUCT_SHIP_YARD,
	TXT_SHIP_YARD,					// NAME:			Short name of the structure.
	"SYRD",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(22+(CELL_PIXEL_W/2), ((CELL_PIXEL_H*2)-(CELL_PIXEL_H/2))),			// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_VESSELTYPE,				// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_33, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)ListSPen,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListSPen	// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassSubPen(
	STRUCT_SUB_PEN,
	TXT_SUB_PEN,					// NAME:			Short name of the structure.
	"SPEN",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(22+(CELL_PIXEL_W/2), ((CELL_PIXEL_H*2)-(CELL_PIXEL_H/2))),			// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_VESSELTYPE,				// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_33, 						// SIZE:			Building size.
	(short const *)ExitSub,		// Preferred exit cell list.
	(short const *)ListSPen,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListSPen	// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassPillbox(
	STRUCT_PILLBOX,
	TXT_PILLBOX,					// NAME:			Short name of the structure.
	"PBOX",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0010,							//	Vertical offset.
	0x0040,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassCamoPillbox(
	STRUCT_CAMOPILLBOX,
	TXT_CAMOPILLBOX,				// NAME:			Short name of the structure.
	"HBOX",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0010,							//	Vertical offset.
	0x0040,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassTesla(
	STRUCT_TESLA,
	TXT_TESLA,						// NAME:			Short name of the structure.
	"TSLA",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x00C8,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_12, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List12,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OList12		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassTurret(
	STRUCT_TURRET,
	TXT_TURRET,						// NAME:			Short name of the structure.
	"GUN",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0030,							//	Vertical offset.
	0x0080,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		true,							// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	(DirType)208,					// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassAAGun(
	STRUCT_AAGUN,
	TXT_AAGUN,						// NAME:			Short name of the structure.
	"AGUN",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		true,							// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_NE,							// Starting idle frame to match construction.
	BSIZE_12, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List12,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OList12		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassFlameTurret(
	STRUCT_FLAME_TURRET,
	TXT_FLAME_TURRET,				// NAME:			Short name of the structure.
	"FTUR",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassConst(
	STRUCT_CONST,
	TXT_CONST_YARD,				// NAME:			Short name of the structure.
	"FACT",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_BUILDINGTYPE,			// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_33, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)ListFactory,// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassFakeConst(
	STRUCT_FAKECONST,
	TXT_FAKE_CONST,				// NAME:			Short name of the structure.
	"FACF",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		true,							// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_33, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)ListFactory,// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassFakeWeapon(
	STRUCT_FAKEWEAP,
	TXT_FAKE_WEAP,					// NAME:			Short name of the structure.
	"WEAF",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(10+(CELL_PIXEL_W/2), ((CELL_PIXEL_H*3)-(CELL_PIXEL_H/2))-21),			// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		true,							// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_32, 						// SIZE:			Building size.
	(short const *)ExitWeap,	// Preferred exit cell list.
	(short const *)ListWeap,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListWeap	// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassRefinery(
	STRUCT_REFINERY,
	TXT_REFINERY,					// NAME:			Short name of the structure.
	"PROC",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_33, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List010111100,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)List101000011	// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassStorage(
	STRUCT_STORAGE,
	TXT_STORAGE,					// NAME:			Short name of the structure.
	"SILO",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)StoreList,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassHelipad(
	STRUCT_HELIPAD,
	TXT_HELIPAD,					// NAME:			Short name of the structure.
	"HPAD",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_AIRCRAFTTYPE,			// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List2,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassCommand(
	STRUCT_RADAR,
	TXT_COMMAND,					// NAME:			Short name of the structure.
	"DOME",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)ComList,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassGapGenerator(
	STRUCT_GAP,
	TXT_GAP_GENERATOR,			// NAME:			Short name of the structure.
	"GAP",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_12, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List0010,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassSAM(
	STRUCT_SAM,
	TXT_SAM,							// NAME:			Short name of the structure.
	"SAM",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0030,							//	Vertical offset.
	0x0080,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		true,							// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_21, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List21,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListSAM		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassMissileSilo(
	STRUCT_MSLO,
	TXT_MSLO,						// NAME:			Short name of the structure.
	"MSLO",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_21, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List21,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListSAM		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassAirStrip(
	STRUCT_AIRSTRIP,
	TXT_AIRSTRIP,					// NAME:			Short name of the structure.
	"AFLD",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_AIRCRAFTTYPE,			// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_32, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List32,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassPower(
	STRUCT_POWER,
	TXT_POWER,						// NAME:			Short name of the structure.
	"POWR",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List22,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)List22_1100	// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassAdvancedPower(
	STRUCT_ADVANCED_POWER,
	TXT_ADVANCED_POWER,			// NAME:			Short name of the structure.
	"APWR",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_33, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List000111111,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListTmpl	// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassSovietTech(
	STRUCT_SOVIET_TECH,
	TXT_SOVIET_TECH,				// NAME:			Short name of the structure.
	"STEK",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_33, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List000111111,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListTmpl	// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassHospital(
	STRUCT_HOSPITAL,
	TXT_HOSPITAL,					// NAME:			Short name of the structure.
	"HOSP",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List2,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassBioLab(
	STRUCT_BIO_LAB,
	TXT_BIO_LAB,					// NAME:			Short name of the structure.
	"BIO",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List2,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassBarracks(
	STRUCT_BARRACKS,
	TXT_BARRACKS,					// NAME:			Short name of the structure.
	"BARR",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
//	XYP_COORD(24,47),				// Exit point for produced units.
	XYP_COORD(18,47),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_INFANTRYTYPE,			// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	(short const *)ExitPyle,	// Preferred exit cell list.
	(short const *)List22,		// OCCUPYLIST:	List of active foundation squares.
	NULL								// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassTent(
	STRUCT_TENT,
	TXT_BARRACKS,					// NAME:			Short name of the structure.
	"TENT",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(24,47),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_INFANTRYTYPE,			// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	(short const *)ExitPyle,	// Preferred exit cell list.
	(short const *)List22,		// OCCUPYLIST:	List of active foundation squares.
	NULL								// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassKennel(
	STRUCT_KENNEL,
	TXT_KENNEL,						// NAME:			Short name of the structure.
	"KENN",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(8,16),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_INFANTRYTYPE,			// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
//	(short const *)ExitPyle,	// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	NULL								// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassFakeShipYard(
	STRUCT_FAKE_YARD,
	TXT_FAKE_YARD,					// NAME:			Short name of the structure.
	"SYRF",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(22+(CELL_PIXEL_W/2), ((CELL_PIXEL_H*2)-(CELL_PIXEL_H/2))),			// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		true,							// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_33, 						// SIZE:			Building size.
	(short const *)ExitWeap,	// Preferred exit cell list.
	(short const *)ListSPen,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListSPen	// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassFakeSubPen(
	STRUCT_FAKE_PEN,
	TXT_FAKE_PEN,					// NAME:			Short name of the structure.
	"SPEF",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(22+(CELL_PIXEL_W/2), ((CELL_PIXEL_H*2)-(CELL_PIXEL_H/2))),			// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		true,							// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_33, 						// SIZE:			Building size.
	(short const *)ExitSub,		// Preferred exit cell list.
	(short const *)ListSPen,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListSPen	// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassFakeCommand(
	STRUCT_FAKE_RADAR,
	TXT_FAKE_RADAR,				// NAME:			Short name of the structure.
	"DOMF",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		true,							// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)ComList,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OComList		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassRepair(
	STRUCT_REPAIR,
	TXT_FIX_IT,						// NAME:			Short name of the structure.
	"FIX",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		false,						// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_33, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)ListFix,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListFix		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV01(
	STRUCT_V01,
	TXT_CIV1,						// NAME:			Short name of the structure.
	"V01",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE: Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List0011,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1100		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV02(
	STRUCT_V02,
	TXT_CIV2,						// NAME:			Short name of the structure.
	"V02",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List0011,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1100		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV03(
	STRUCT_V03,
	TXT_CIV3,						// NAME:			Short name of the structure.
	"V03",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List0111,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1000		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV04(
	STRUCT_V04,
	TXT_CIV4,						// NAME:			Short name of the structure.
	"V04",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List0011,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1100		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV05(
	STRUCT_V05,
	TXT_CIV5,						// NAME:			Short name of the structure.
	"V05",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_21, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List11,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV06(
	STRUCT_V06,
	TXT_CIV6,						// NAME:			Short name of the structure.
	"V06",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_21, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List11,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV07(
	STRUCT_V07,
	TXT_CIV7,						// NAME:			Short name of the structure.
	"V07",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_21, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List11,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV08(
	STRUCT_V08,
	TXT_CIV8,						// NAME:			Short name of the structure.
	"V08",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV09(
	STRUCT_V09,
	TXT_CIV9,						// NAME:			Short name of the structure.
	"V09",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV10(
	STRUCT_V10,
	TXT_CIV10,						// NAME:			Short name of the structure.
	"V10",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV11(
	STRUCT_V11,
	TXT_CIV11,						// NAME:			Short name of the structure.
	"V11",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV12(
	STRUCT_V12,
	TXT_CIV12,						// NAME:			Short name of the structure.
	"V12",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV13(
	STRUCT_V13,
	TXT_CIV13,						// NAME:			Short name of the structure.
	"V13",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV14(
	STRUCT_V14,
	TXT_CIV14,						// NAME:			Short name of the structure.
	"V14",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV15(
	STRUCT_V15,
	TXT_CIV15,						// NAME:			Short name of the structure.
	"V15",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV16(
	STRUCT_V16,
	TXT_CIV16,						// NAME:			Short name of the structure.
	"V16",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV17(
	STRUCT_V17,
	TXT_CIV17,						// NAME:			Short name of the structure.
	"V17",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV18(
	STRUCT_V18,
	TXT_CIV18,						// NAME:			Short name of the structure.
	"V18",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV19(
	STRUCT_PUMP,
	TXT_PUMP,						// NAME:			Short name of the structure.
	"V19",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV20(
	STRUCT_V20,
	TXT_CIV20,						// NAME:			Short name of the structure.
	"V20",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List0011,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1100		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV21(
	STRUCT_V21,
	TXT_CIV21,						// NAME:			Short name of the structure.
	"V21",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1101,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)List0010		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV22(
	STRUCT_V22,
	TXT_CIV22,						// NAME:			Short name of the structure.
	"V22",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_21, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List11,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV23(
	STRUCT_V23,
	TXT_CIV23,						// NAME:			Short name of the structure.
	"V23",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV24(
	STRUCT_V24,
	TXT_CIV24,						// NAME:			Short name of the structure.
	"V24",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List0011,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1100		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV25(
	STRUCT_V25,
	TXT_CIV25,						// NAME:			Short name of the structure.
	"V25",							// NAME:			Short name of the structure.
	FACING_S,						// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_22, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List0111,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1000		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV26(
	STRUCT_V26,
	TXT_CIV26,						// NAME:			Short name of the structure.
	"V26",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_21, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List11,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV27(
	STRUCT_V27,
	TXT_CIV27,						// NAME:			Short name of the structure.
	"V27",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV28(
	STRUCT_V28,
	TXT_CIV28,						// NAME:			Short name of the structure.
	"V28",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV29(
	STRUCT_V29,
	TXT_CIV29,						// NAME:			Short name of the structure.
	"V29",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV30(
	STRUCT_V30,
	TXT_CIV30,						// NAME:			Short name of the structure.
	"V30",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_21, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List11,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV31(
	STRUCT_V31,
	TXT_CIV31,						// NAME:			Short name of the structure.
	"V31",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_21, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List11,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV32(
	STRUCT_V32,
	TXT_CIV32,						// NAME:			Short name of the structure.
	"V32",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_21, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List11,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV33(
	STRUCT_V33,
	TXT_CIV33,						// NAME:			Short name of the structure.
	"V33",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_21, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List11,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV34(
	STRUCT_V34,
	TXT_CIV34,						// NAME:			Short name of the structure.
	"V34",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV35(
	STRUCT_V35,
	TXT_CIV35,						// NAME:			Short name of the structure.
	"V35",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV36(
	STRUCT_V36,
	TXT_CIV36,						// NAME:			Short name of the structure.
	"V36",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);
static BuildingTypeClass const ClassV37(
	STRUCT_V37,
	TXT_CIV37,						// NAME:			Short name of the structure.
	"V37",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		true,							// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_42, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)ListWestwood,	// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListWestwood	// OVERLAPLIST:List of overlap cell offset.
);
static BuildingTypeClass const ClassMission(
	STRUCT_MISSION,
	TXT_CIVMISS,					// NAME:			Short name of the structure.
	"MISS",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_32, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List32,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

// Sandbag wall
static BuildingTypeClass const Sandbag(
	STRUCT_SANDBAG_WALL,
	TXT_SANDBAG_WALL,				// NAME:			Short name of the structure.
	"SBAG",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_NONE,						// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		true,							// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		false,						// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);
// Cyclone fence
static BuildingTypeClass const Cyclone(
	STRUCT_CYCLONE_WALL,
	TXT_CYCLONE_WALL,				// NAME:			Short name of the structure.
	"CYCL",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_NONE,						// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		true,							// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		false,						// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);
// Brick wall
static BuildingTypeClass const Brick(
	STRUCT_BRICK_WALL,
	TXT_BRICK_WALL,				// NAME:			Short name of the structure.
	"BRIK",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_NONE,						// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		true,							// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		false,						// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);
// Barbwire wall
static BuildingTypeClass const Barbwire(
	STRUCT_BARBWIRE_WALL,
	TXT_BARBWIRE_WALL,			// NAME:			Short name of the structure.
	"BARB",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_NONE,						// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		true,							// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		false,						// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);
// Wood wall
static BuildingTypeClass const Wood(
	STRUCT_WOOD_WALL,
	TXT_WOOD_WALL,					// NAME:			Short name of the structure.
	"WOOD",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_NONE,						// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		true,							// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		false,						// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);
static BuildingTypeClass const Fence(
	STRUCT_FENCE,
	TXT_FENCE,						// NAME:			Short name of the structure.
	"FENC",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_NONE,						// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		true,							// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		false,						// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);


#ifdef FIXIT_ANTS
static BuildingTypeClass const ClassQueen(
	STRUCT_QUEEN,
	TXT_NONE,						// NAME:			Short name of the structure.
	"QUEE",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(24,47),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		true,							// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		false,						// Simple (one frame) damage imagery?
		false,						// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		false,						// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		true,							// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_21, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List11,		// OCCUPYLIST:	List of active foundation squares.
	NULL								// OVERLAPLIST:List of overlap cell offset.
);
static BuildingTypeClass const ClassLarva1(
	STRUCT_LARVA1,
	TXT_NONE,						// NAME:			Short name of the structure.
	"LAR1",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);
static BuildingTypeClass const ClassLarva2(
	STRUCT_LARVA2,
	TXT_NONE,						// NAME:			Short name of the structure.
	"LAR2",							// NAME:			Short name of the structure.
	FACING_NONE,					// Foundation direction from center of building.
	XYP_COORD(0,0),				// Exit point for produced units.
	REMAP_ALTERNATE,				// Sidebar remap logic.
	0x0000,							//	Vertical offset.
	0x0000,							// Primary weapon offset along turret centerline.
	0x0000,							// Primary weapon lateral offset along turret centerline.
		false,						// Is this building a fake (decoy?)
		false,						// Animation rate is regulated for constant speed?
		true,							// Always use the given name for the building?
		false,						// Is this a wall type structure?
		true,							// Simple (one frame) damage imagery?
		true,							// Is it invisible to radar?
		true,							// Can the player select this?
		true,							// Is this a legal target for attack or move?
		true,							// Is this an insignificant building?
		false,						// Theater specific graphic image?
		false,						// Does it have a rotating turret?
		false,						// Can the building be color remapped to indicate owner?
	RTTI_NONE,						// The object type produced at this factory.
	DIR_N,							// Starting idle frame to match construction.
	BSIZE_11, 						// SIZE:			Building size.
	NULL,								// Preferred exit cell list.
	(short const *)List1,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);
#endif
void const * BuildingTypeClass::WarFactoryOverlay;
void const * LightningShapes;


/***********************************************************************************************
 * BuildingTypeClass::BuildingTypeClass -- This is the constructor for the building types.     *
 *                                                                                             *
 *    This is the constructor used to create the building types.                               *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingTypeClass::BuildingTypeClass(
						StructType type,
						int name,
						char const * ininame,
						FacingType foundation,
						COORDINATE exitpoint,
						RemapType remap,
						int verticaloffset,
						int primaryoffset,
						int primarylateral,
						bool is_fake,
						bool is_regulated,
						bool is_nominal,
						bool is_wall,
						bool is_simpledamage,
						bool is_stealthy,
						bool is_selectable,
						bool is_legal_target,
						bool is_insignificant,
						bool is_theater,
						bool is_turret_equipped,
						bool is_remappable,
						RTTIType tobuild,
						DirType sframe,
						BSizeType size,
						short const * exitlist,
						short const * sizelist,
						short const * overlap) :
								TechnoTypeClass(RTTI_BUILDINGTYPE,
													int(type),
													name,
													ininame,
													remap,
													verticaloffset,
													primaryoffset,
													primarylateral,
													primaryoffset,
													primarylateral,
													is_nominal,
													is_stealthy,
													is_selectable,
													is_legal_target,
													is_insignificant,
													false,
													is_theater,
													is_turret_equipped,
													is_remappable,
													true,
													(is_turret_equipped ? 32 : 1),
													SPEED_NONE),
	IsBase(true),
	IsFake(is_fake),
	IsBibbed(false),
	IsWall(is_wall),
	IsSimpleDamage(is_simpledamage),
	IsCaptureable(false),
	IsRegulated(is_regulated),
	IsPowered(false),
	IsUnsellable(false),
	FoundationFace(foundation),
	Adjacent(1),
	ToBuild(tobuild),
	ExitCoordinate(exitpoint),
	ExitList(exitlist),
	Type(type),
	StartFace(sframe),
	Capacity(0),
	Power(0),
	Drain(0),
	Size(size),
	OccupyList(sizelist),
	OverlapList(overlap),
	BuildupData(0)
{

	Anims[BSTATE_CONSTRUCTION].Start = 0;
	Anims[BSTATE_CONSTRUCTION].Count = 1;
	Anims[BSTATE_CONSTRUCTION].Rate = 0;

	Anims[BSTATE_IDLE].Start = 0;
	Anims[BSTATE_IDLE].Count = 1;
	Anims[BSTATE_IDLE].Rate = 0;

	Anims[BSTATE_ACTIVE].Start = 0;
	Anims[BSTATE_ACTIVE].Count = 1;
	Anims[BSTATE_ACTIVE].Rate = 0;

	Anims[BSTATE_AUX1].Start = 0;
	Anims[BSTATE_AUX1].Count = 1;
	Anims[BSTATE_AUX1].Rate = 0;

	Anims[BSTATE_AUX2].Start = 0;
	Anims[BSTATE_AUX2].Count = 1;
	Anims[BSTATE_AUX2].Rate = 0;
}


/***********************************************************************************************
 * BuildingTypeClass::operator new -- Allocates a building type object from the special heap.  *
 *                                                                                             *
 *    This routine will allocate a building type object from the special heap used just for    *
 *    allocation of object of this type.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the newly allocated object. If the allocation could not  *
 *          succeed, then NULL will be returned.                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * BuildingTypeClass::operator new(size_t)
{
	return(BuildingTypes.Alloc());
}


/***********************************************************************************************
 * BuildingTypeClass::operator delete -- Deletes a building type object from the special heap. *
 *                                                                                             *
 *    This will delete a previously allocated building type object. The memory is returned     *
 *    to the special heap that is used for that purpose.                                       *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the building type object to return to the special heap.        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingTypeClass::operator delete(void * ptr)
{
	BuildingTypes.Free((BuildingTypeClass *)ptr);
}


/***********************************************************************************************
 * BuildingTypeClass::Init_Heap -- Initialize the heap as necessary for the building type obje *
 *                                                                                             *
 *    This routine performs the necessary heap initializations. Since we know exactly what     *
 *    building type objects will be needed, they are pre-allocated at this time.               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Call this routine only once.                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingTypeClass::Init_Heap(void)
{
	/*
	**	These building type class objects must be allocated in the exact order that they
	**	are specified in the StructType enumeration. This is necessary because the heap
	**	allocation block index serves double duty as the type number index.
	*/
	new BuildingTypeClass(ClassAdvancedTech);	// STRUCT_ADVANCED_TECH
	new BuildingTypeClass(ClassIronCurtain);	// STRUCT_IRON_CURTAIN
	new BuildingTypeClass(ClassWeapon);			//	STRUCT_WEAP
	new BuildingTypeClass(ClassChronosphere);	// STRUCT_CHRONOSPHERE
	new BuildingTypeClass(ClassPillbox);		//	STRUCT_PILLBOX
	new BuildingTypeClass(ClassCamoPillbox);	//	STRUCT_CAMOPILLBOX
	new BuildingTypeClass(ClassCommand);		//	STRUCT_RADAR
	new BuildingTypeClass(ClassGapGenerator);	// STRUCT_GAP
	new BuildingTypeClass(ClassTurret);			//	STRUCT_TURRET
	new BuildingTypeClass(ClassAAGun);			// STRUCT_AAGUN
	new BuildingTypeClass(ClassFlameTurret);	//	STRUCT_FLAME_TURRET
	new BuildingTypeClass(ClassConst);			//	STRUCT_CONST
	new BuildingTypeClass(ClassRefinery);		//	STRUCT_REFINERY
	new BuildingTypeClass(ClassStorage);		//	STRUCT_STORAGE
	new BuildingTypeClass(ClassHelipad);		//	STRUCT_HELIPAD
	new BuildingTypeClass(ClassSAM);				//	STRUCT_SAM
	new BuildingTypeClass(ClassAirStrip);		//	STRUCT_AIRSTRIP
	new BuildingTypeClass(ClassPower);			//	STRUCT_POWER
	new BuildingTypeClass(ClassAdvancedPower);//	STRUCT_ADVANCED_POWER
	new BuildingTypeClass(ClassSovietTech);	// STRUCT_SOVIET_TECH
	new BuildingTypeClass(ClassHospital);		//	STRUCT_HOSPITAL
	new BuildingTypeClass(ClassBarracks);		//	STRUCT_BARRACKS
	new BuildingTypeClass(ClassTent);			//	STRUCT_TENT
	new BuildingTypeClass(ClassKennel);			// STRUCT_KENNEL
	new BuildingTypeClass(ClassRepair);			//	STRUCT_REPAIR
	new BuildingTypeClass(ClassBioLab);			//	STRUCT_BIO_LAB
	new BuildingTypeClass(ClassMission);		//	STRUCT_MISSION
	new BuildingTypeClass(ClassShipYard);		//	STRUCT_SHIP_YARD
	new BuildingTypeClass(ClassSubPen);			//	STRUCT_SUB_PEN
	new BuildingTypeClass(ClassMissileSilo);	// STRUCT_MSLO
	new BuildingTypeClass(ClassForwardCom);	// STRUCT_FORWARD_COM
	new BuildingTypeClass(ClassTesla);			//	STRUCT_TESLA
	new BuildingTypeClass(ClassFakeWeapon);	// STRUCT_FAKEWEAP
	new BuildingTypeClass(ClassFakeConst);		// STRUCT_FAKECONST
	new BuildingTypeClass(ClassFakeShipYard);	// STRUCT_FAKE_YARD
	new BuildingTypeClass(ClassFakeSubPen);	// STRUCT_FAKE_PEN
	new BuildingTypeClass(ClassFakeCommand);	// STRUCT_FAKE_RADAR
	new BuildingTypeClass(Sandbag);				// STRUCT_SANDBAG_WALL
	new BuildingTypeClass(Cyclone);				//	STRUCT_CYCLONE_WALL
	new BuildingTypeClass(Brick);					// STRUCT_BRICK_WALL
	new BuildingTypeClass(Barbwire);				// STRUCT_BARBWIRE_WALL
	new BuildingTypeClass(Wood);					//	STRUCT_WOOD_WALL
	new BuildingTypeClass(Fence);					// STRUCT_FENCE
	new BuildingTypeClass(ClassAVMine);			// STRUCT_AVMINE
	new BuildingTypeClass(ClassAPMine);			// STRUCT_APMINE
	new BuildingTypeClass(ClassV01);				//	STRUCT_V1
	new BuildingTypeClass(ClassV02);				//	STRUCT_V2
	new BuildingTypeClass(ClassV03);				//	STRUCT_V3
	new BuildingTypeClass(ClassV04);				//	STRUCT_V4
	new BuildingTypeClass(ClassV05);				//	STRUCT_V5
	new BuildingTypeClass(ClassV06);				//	STRUCT_V6
	new BuildingTypeClass(ClassV07);				//	STRUCT_V7
	new BuildingTypeClass(ClassV08);				//	STRUCT_V8
	new BuildingTypeClass(ClassV09);				//	STRUCT_V9
	new BuildingTypeClass(ClassV10);				//	STRUCT_V10
	new BuildingTypeClass(ClassV11);				//	STRUCT_V11
	new BuildingTypeClass(ClassV12);				//	STRUCT_V12
	new BuildingTypeClass(ClassV13);				//	STRUCT_V13
	new BuildingTypeClass(ClassV14);				//	STRUCT_V14
	new BuildingTypeClass(ClassV15);				//	STRUCT_V15
	new BuildingTypeClass(ClassV16);				//	STRUCT_V16
	new BuildingTypeClass(ClassV17);				//	STRUCT_V17
	new BuildingTypeClass(ClassV18);				//	STRUCT_V18
	new BuildingTypeClass(ClassV19);				//	STRUCT_PUMP
	new BuildingTypeClass(ClassV20);				//	STRUCT_V20
	new BuildingTypeClass(ClassV21);				//	STRUCT_V21
	new BuildingTypeClass(ClassV22);				//	STRUCT_V22
	new BuildingTypeClass(ClassV23);				//	STRUCT_V23
	new BuildingTypeClass(ClassV24);				//	STRUCT_V24
	new BuildingTypeClass(ClassV25);				//	STRUCT_V25
	new BuildingTypeClass(ClassV26);				//	STRUCT_V26
	new BuildingTypeClass(ClassV27);				//	STRUCT_V27
	new BuildingTypeClass(ClassV28);				//	STRUCT_V28
	new BuildingTypeClass(ClassV29);				//	STRUCT_V29
	new BuildingTypeClass(ClassV30);				//	STRUCT_V30
	new BuildingTypeClass(ClassV31);				//	STRUCT_V31
	new BuildingTypeClass(ClassV32);				//	STRUCT_V32
	new BuildingTypeClass(ClassV33);				//	STRUCT_V33
	new BuildingTypeClass(ClassV34);				//	STRUCT_V34
	new BuildingTypeClass(ClassV35);				//	STRUCT_V35
	new BuildingTypeClass(ClassV36);				//	STRUCT_V36
	new BuildingTypeClass(ClassV37);				//	STRUCT_V37
	new BuildingTypeClass(ClassBarrel);			// STRUCT_BARREL
	new BuildingTypeClass(ClassBarrel3);		// STRUCT_BARREL3

#ifdef FIXIT_ANTS
	new BuildingTypeClass(ClassQueen);			// STRUCT_QUEEN
	new BuildingTypeClass(ClassLarva1);			// STRUCT_LARVA1
	new BuildingTypeClass(ClassLarva2);			// STRUCT_LARVA2
#endif

}


/***********************************************************************************************
 * BuildingTypeClass::One_Time -- Performs special one time action for buildings.              *
 *                                                                                             *
 *    This routine is used to do the one time action necessary to handle building type class   *
 *    objects. This entails loading of the building shapes and the brain file used by          *
 *    buildings.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine should only be called ONCE.                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *   06/11/1994 JLB : Updated construction time and frame count logic.                         *
 *=============================================================================================*/
void BuildingTypeClass::One_Time(void)
{
	static const struct {
		StructType	Class;			// Building class number.
		BStateType	Stage;			// Animation sequence to assign animation range to.
		int			Start;			// Starting frame number.
		int			Length;			// Number of frames (-1 means use all frames).
		int			Rate;				// Rate of animation.
	} _anims[] = {
		{STRUCT_CHRONOSPHERE,	BSTATE_IDLE,	0, 4, 3},	// idling
		{STRUCT_CHRONOSPHERE,	BSTATE_ACTIVE,	4, 16,3},	// charging up and activating
		{STRUCT_MSLO,				BSTATE_IDLE,	0, 0, 0},
		{STRUCT_MSLO,				BSTATE_ACTIVE,	0, 5, 2},	// door opening
		{STRUCT_MSLO,				BSTATE_AUX1,	4, 1, 0},	// door held open
		{STRUCT_MSLO,				BSTATE_AUX2,	5, 3, 2},	// door closing
		{STRUCT_CAMOPILLBOX,		BSTATE_ACTIVE,	0,	2, 1},
		{STRUCT_GAP,				BSTATE_IDLE,	0,	32,3},
		{STRUCT_AIRSTRIP,			BSTATE_IDLE,	0,	0,	0},
		{STRUCT_AIRSTRIP,			BSTATE_AUX1,	0,	8,	3},
		{STRUCT_BARRACKS,			BSTATE_ACTIVE,	0,	10,3},
		{STRUCT_BARRACKS,			BSTATE_IDLE,	0,	10,3},
		{STRUCT_TENT,				BSTATE_ACTIVE,	0,	10,3},
		{STRUCT_TENT,				BSTATE_IDLE,	0,	10,3},
#ifdef FIXIT_ANTS
		{STRUCT_QUEEN,				BSTATE_IDLE,	0,	10,3},
#endif
		{STRUCT_CONST,				BSTATE_ACTIVE,	0,	26,3},
		{STRUCT_FAKECONST,		BSTATE_ACTIVE,	0,	26,3},
		{STRUCT_HELIPAD,			BSTATE_ACTIVE,	0,	7,	4},
		{STRUCT_HELIPAD,			BSTATE_IDLE,	0,	0,	0},
		{STRUCT_HOSPITAL,			BSTATE_IDLE,	0,	4,	3},
		{STRUCT_PUMP,				BSTATE_IDLE,	0,	14,4},
		{STRUCT_REPAIR,			BSTATE_ACTIVE,	0,	7,	2},
		{STRUCT_REPAIR,			BSTATE_IDLE,	0,	1,	0},
		{STRUCT_V20,				BSTATE_IDLE,	0,	3,	3},
		{STRUCT_V21,				BSTATE_IDLE,	0,	3,	3},
		{STRUCT_V22,				BSTATE_IDLE,	0,	3,	3},
		{STRUCT_V23,				BSTATE_IDLE,	0,	3,	3},
		{STRUCT_WEAP,				BSTATE_ACTIVE,	0,	1,	0},
		{STRUCT_WEAP,				BSTATE_IDLE,	0,	1,	0},
		{STRUCT_FAKEWEAP,			BSTATE_ACTIVE,	0,	1,	0},
		{STRUCT_FAKEWEAP,			BSTATE_IDLE,	0,	1,	0},
		{STRUCT_IRON_CURTAIN,	BSTATE_ACTIVE,	0,	11,3},
		{STRUCT_TESLA,				BSTATE_ACTIVE,	0,	10,2},
		{STRUCT_AIRSTRIP,			BSTATE_IDLE,	0,	8,	3}
	};

	for (int sindex = STRUCT_FIRST; sindex < STRUCT_COUNT; sindex++) {
		char	fullname[_MAX_FNAME+_MAX_EXT];
		char	buffer[_MAX_FNAME];
		BuildingTypeClass const & building = As_Reference((StructType)sindex);
		/*
		**	Fetch the sidebar cameo image for this building.
		*/
		if (building.Level != -1) {
//		if (building.IsBuildable) {
			sprintf(buffer, "%sICON", building.Graphic_Name());

			if (building.IsFake) {
				buffer[3] = 'F';
			}

			_makepath(fullname, NULL, NULL, buffer, ".SHP");
			((void const *&)building.CameoData) = MFCD::Retrieve(fullname);
		}

		/*
		**	Fetch the construction animation for this building.
		*/
		sprintf(buffer, "%sMAKE", building.Graphic_Name());
		_makepath(fullname, NULL, NULL, buffer, ".SHP");
		void const * dataptr;
		dataptr = MFCD::Retrieve(fullname);
		((void const *&)building.BuildupData) = dataptr;
		if (dataptr != NULL) {
			int timedelay = 1;
			int count = Get_Build_Frame_Count(dataptr);
			if (count > 0) {
				timedelay = (Rule.BuildupTime * TICKS_PER_MINUTE) / count;
			}
			building.Init_Anim(BSTATE_CONSTRUCTION, 0, count, timedelay);
		}

		/*
		**	Fetch the normal game shape for this building.
		*/
		_makepath(fullname, NULL, NULL, building.Graphic_Name(), ".SHP");
		((void const *&)building.ImageData) = MFCD::Retrieve(fullname);
	}

	// Try to load weap2.shp and tesla coil's lightning shapes
	char fullname[_MAX_FNAME+_MAX_EXT];
	_makepath(fullname, NULL, NULL, (char const *)"WEAP2",".SHP");
	WarFactoryOverlay = MFCD::Retrieve(fullname);
	_makepath(fullname, NULL, NULL, (char const *)"LITNING",".SHP");
	LightningShapes = MFCD::Retrieve(fullname);

	/*
	**	Install all the special animation sequences for the different building types.
	*/
	for (unsigned index = 0; index < (sizeof(_anims) / sizeof(_anims[0])); index++) {
		As_Reference(_anims[index].Class).Init_Anim(_anims[index].Stage, _anims[index].Start, _anims[index].Length, _anims[index].Rate);
	}

}


/***********************************************************************************************
 * Struct_From_Name -- Find BData structure from its name.                                     *
 *                                                                                             *
 *    This routine will convert an ASCII name for a building class into                        *
 *    the actual building class it represents.                                                 *
 *                                                                                             *
 * INPUT:   name  -- ASCII representation of a building class.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the actual building class number that the string                      *
 *          represents.                                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   05/02/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
StructType BuildingTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (int classid = STRUCT_FIRST; classid < STRUCT_COUNT; classid++) {
			if (stricmp(As_Reference((StructType)classid).IniName, name) == 0) {
				return((StructType)classid);
			}
		}
	}
	return(STRUCT_NONE);
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * BuildingTypeClass::Display -- Renders a generic view of building.                           *
 *                                                                                             *
 *    This routine is used to display a generic representation of the                          *
 *    building. Typical use of this occurs with the scenario editor.                           *
 *                                                                                             *
 * INPUT:   x,y      -- Coordinate to display the building (centered).                         *
 *                                                                                             *
 *          window   -- The window the building should be rendered                             *
 *                      relative to.                                                           *
 *                                                                                             *
 *          house    -- The house color to use for the building.                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingTypeClass::Display(int x, int y, WindowNumberType window, HousesType ) const
{
	void const * ptr = Get_Cameo_Data();
	if (ptr == NULL) {
		IsTheaterShape = IsTheater;
		ptr = Get_Image_Data();
	}
	CC_Draw_Shape(ptr, 0, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL);
	IsTheaterShape = false;
}


/***********************************************************************************************
 * BuildingTypeClass::Prep_For_Add -- Prepares scenario editor for adding a                    *
 *                                                                                             *
 *    This routine is used to prepare the scenario editor for the addition                     *
 *    of a building object to the game.                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *   06/04/1994 JLB : Uses map editing interface routines.                                     *
 *=============================================================================================*/
void BuildingTypeClass::Prep_For_Add(void)
{
	for (StructType index = STRUCT_FIRST; index < STRUCT_COUNT; index++) {
		if (As_Reference(index).Get_Image_Data()) {
			Map.Add_To_List(&As_Reference(index));
		}
	}
}
#endif


/***********************************************************************************************
 * BuildingTypeClass::Create_And_Place -- Creates and places a building object onto the map.   *
 *                                                                                             *
 *    This routine is used by the scenario editor to create and place buildings on the map.    *
 *                                                                                             *
 * INPUT:   cell     -- The cell that the building is to be placed upon.                       *
 *                                                                                             *
 *          house    -- The owner of the building.                                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the building successfully created and placed on the map?                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingTypeClass::Create_And_Place(CELL cell, HousesType house) const
{
	BuildingClass * ptr;

	ptr = new BuildingClass(Type, house);
	if (ptr != NULL) {
		return(ptr->Unlimbo(Cell_Coord(cell), DIR_N));
	}
	return(false);
}


/***********************************************************************************************
 * BuildingTypeClass::Create_One_Of -- Creates a building of this type.                        *
 *                                                                                             *
 *    This routine will create a building object of this type. The building object is in a     *
 *    limbo state. It is presumed that the building object will be unlimboed at the correct    *
 *    place and time. Typical use is when the building is created in a factory situation       *
 *    and will be placed on the map when construction completes.                               *
 *                                                                                             *
 * INPUT:   house -- Pointer to the house that is to be the owner of the building.             *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building. If the building could not be created       *
 *          then a NULL is returned.                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/07/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * BuildingTypeClass::Create_One_Of(HouseClass * house) const
{
	HousesType htype = HOUSE_NEUTRAL;
	if (house != NULL) {
		htype = house->Class->House;
	}
	return(new BuildingClass(Type, htype));
}


/***********************************************************************************************
 * BuildingTypeClass::Init_Anim -- Initialize an animation control for a building.             *
 *                                                                                             *
 *    This routine will initialize one animation control element for a                         *
 *    specified building. This modifies a "const" class and thus must                          *
 *    perform some strategic casting to get away with this.                                    *
 *                                                                                             *
 * INPUT:   state -- The animation state to apply these data values to.                        *
 *                                                                                             *
 *          start -- Starting frame for the building's animation.                              *
 *                                                                                             *
 *          count -- The number of frames in this animation.                                   *
 *                                                                                             *
 *          rate  -- The countdown timer between animation frames.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingTypeClass::Init_Anim(BStateType state, int start, int count, int rate) const
{
	((int &)Anims[state].Start) = start;
	((int &)Anims[state].Count) = count;
	((int &)Anims[state].Rate) = rate;
}


/***********************************************************************************************
 * BuildingTypeClass::Init -- Performs theater specific initialization.                        *
 *                                                                                             *
 *    This routine is used to perform any initialization that is custom per theater.           *
 *    Typically, this is fetching the building shape data for those building types that have   *
 *    theater specific art.                                                                    *
 *                                                                                             *
 * INPUT:   theater  -- The theater to base this initialization on.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingTypeClass::Init(TheaterType theater)
{
	if (theater != LastTheater) {
		char fullname[_MAX_FNAME+_MAX_EXT];

		for (int sindex = STRUCT_FIRST; sindex < STRUCT_COUNT; sindex++) {
			BuildingTypeClass const * classptr = &As_Reference((StructType)sindex);

			if (classptr->IsTheater) {
				_makepath(fullname, NULL, NULL, classptr->Graphic_Name(), Theaters[theater].Suffix);
				((void const *&)classptr->ImageData) = MFCD::Retrieve(fullname);

				/*
				**	Buildup data is probably theater specific as well. Fetch a pointer to the
				**	data at this time as well.
				*/
				sprintf(fullname, "%sMAKE.%s", classptr->Graphic_Name(), Theaters[theater].Suffix);
				((void const *&)classptr->BuildupData) = MFCD::Retrieve(fullname);
				if (classptr->BuildupData) {
					int timedelay = 1;
					int count = Get_Build_Frame_Count(classptr->BuildupData);
					if (count != 0) {
						timedelay = (5 * TICKS_PER_SECOND) / count;
					}
					classptr->Init_Anim(BSTATE_CONSTRUCTION, 0, count, timedelay);
				}
			}
		}
	}
}


/***********************************************************************************************
 * BuildingTypeClass::Dimensions -- Fetches the pixel dimensions of the building.              *
 *                                                                                             *
 *    This routine will fetch the dimensions of the building (in pixels). These dimensions are *
 *    used to render the selection rectangle and the health bar.                               *
 *                                                                                             *
 * INPUT:   width    -- Reference to the pixel width (to be filled in).                        *
 *                                                                                             *
 *          height   -- Reference to the pixel height (to be filled in).                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingTypeClass::Dimensions(int &width, int &height) const
{
	width = Width() * ICON_PIXEL_W;
	width -= (width/5);
	height = Height() * ICON_PIXEL_H;
	height -= (height/5);
}


/***********************************************************************************************
 * BuildingTypeClass::As_Reference -- Fetches reference to the building type specified.        *
 *                                                                                             *
 *    This routine will fetch a reference to the BuildingTypeClass as indicated by the         *
 *    building type number specified.                                                          *
 *                                                                                             *
 * INPUT:   type  -- The building type number to convert into a BuildingTypeClass reference.   *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the building type class as indicated by the            *
 *          parameter.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingTypeClass & BuildingTypeClass::As_Reference(StructType type)
{
	return(*BuildingTypes.Ptr(type));
}


/***********************************************************************************************
 * BuildingTypeClass::Occupy_List -- Fetches the occupy list for the building.                 *
 *                                                                                             *
 *    Use this routine to fetch the occupy list pointer for the building. The occupy list is   *
 *    used to determine what cells the building occupies and thus precludes other buildings    *
 *    or objects from using.                                                                   *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a cell offset list to be used to determine what cells    *
 *          this building occupies.                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * BuildingTypeClass::Occupy_List(bool placement) const
{
	SmudgeType bib = SMUDGE_NONE;
	CELL cell=0;

	if (placement && Bib_And_Offset(bib, cell)) {

		SmudgeTypeClass const & smudge = SmudgeTypeClass::As_Reference(bib);
		static short _list[25];
		short * dest = &_list[0];

		/*
		**	Copy the bib overlap list into the working buffer.
		*/
		short const * src = smudge.Occupy_List();
		while (*src != REFRESH_EOL) {
			*dest++ = (*src++) + cell;
		}

		/*
		**	Append the building occupy list to this working buffer.
		*/
		src = OccupyList;
		while (src && *src != REFRESH_EOL) {
			*dest++ = *src++;
		}
		*dest = REFRESH_EOL;

		return(&_list[0]);
	}

	if (OccupyList != NULL) {
		return(OccupyList);
	}

	static short const _templap[] = {REFRESH_EOL};
	return(&_templap[0]);
}


/***********************************************************************************************
 * BuildingTypeClass::Overlap_List -- Fetches the overlap list for the building.               *
 *                                                                                             *
 *    This routine will fetch the overlap list for the building. The overlap list is used      *
 *    to determine what cells the building's graphics cover, but is not considered to occupy   *
 *    for movement purposes.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the cell offset list that is used to determine the       *
 *          cells that this building overlaps.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * BuildingTypeClass::Overlap_List(void) const
{
	if (OverlapList != NULL) {
		return(OverlapList);
	}

	static short const _templap[] = {REFRESH_EOL};
	return(&_templap[0]);
}


/***********************************************************************************************
 * BuildingTypeClass::Width -- Determines width of building in icons.                          *
 *                                                                                             *
 *    Use this routine to determine the width of the building type in icons.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the building width in icons.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Width(void) const
{
	static int width[BSIZE_COUNT] = {
		1,
		2,
		1,
		2,
		2,
		3,
		3,
		4,
		5
	};
	return(width[Size]);
}


/***********************************************************************************************
 * BuildingTypeClass::Height -- Determines the height of the building in icons.                *
 *                                                                                             *
 *    Use this routine to find the height of the building in icons.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the building height in icons.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Height(bool bib) const
{
	static int height[BSIZE_COUNT] = {
		1,
		1,
		2,
		2,
		3,
		2,
		3,
		2,
		5
	};
	return(height[Size] + ((bib && IsBibbed) ? 1 : 0));
}


/***********************************************************************************************
 * BuildingTypeClass::Bib_And_Offset -- Determines the bib and appropriate cell offset.        *
 *                                                                                             *
 *    This routine is used to determine what (if any) bib should be used for this building     *
 *    and also the cell offset for the upper left corner of the bib smudge type.               *
 *                                                                                             *
 * INPUT:   bib   -- Reference to the bib that should be used for this building.               *
 *                                                                                             *
 *          cell  -- The cell offset for the upper left corner of the bib. This offset is      *
 *                   relative to the upper left corner of the building.                        *
 *                                                                                             *
 * OUTPUT:  Is a bib required for this building? If the result is true, then the correct       *
 *          bib and cell offset will be filled in.                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingTypeClass::Bib_And_Offset(SmudgeType & bib, CELL & cell) const
{
	bib = SMUDGE_NONE;

	if (IsBibbed) {
		switch (Width()) {
			case 2:
				bib = SMUDGE_BIB3;
				break;

			case 3:
				bib = SMUDGE_BIB2;
				break;

			case 4:
				bib = SMUDGE_BIB1;
				break;

			default:
				bib = SMUDGE_NONE;
				break;
		}

		/*
		**	Adjust the bib position for special buildings that have the bib as part
		**	of the building art itself.
		*/
		if (bib != SMUDGE_NONE) {
			cell += ((Height()-1)*MAP_CELL_W);
		}
	}
	return(bib != SMUDGE_NONE);
}


/***********************************************************************************************
 * BuildingTypeClass::Max_Pips -- Determines the maximum pips to display.                      *
 *                                                                                             *
 *    Use this routine to determine the maximum number of pips to display on this building     *
 *    when it is rendered. Typically, this is the tiberium capacity divided by 100.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of pips to display on this building when selected.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Max_Pips(void) const
{
	int maxpips = (Width() * ICON_PIXEL_W) / 4;
	return(Bound((int)(Capacity/100), 0, maxpips));
}


/***********************************************************************************************
 * BuildingTypeClass::Raw_Cost -- Fetches the raw (base) cost of this building type.           *
 *                                                                                             *
 *    This routine is used to fetch the real raw base cost of the building. The raw cost       *
 *    is the cost of the building less any free unit that would come with the building         *
 *    if it were built in the normal fashion. Specifically, the helicopter cost is subtracted  *
 *    from the helipad and the harvester cost is subtracted from the refinery. This cost       *
 *    is used for refunding.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the raw (base) cost to build the building of this type.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Raw_Cost(void) const
{
	int cost = TechnoTypeClass::Raw_Cost();

	if (Type == STRUCT_HELIPAD && !Rule.IsSeparate) {
		cost -= (AircraftTypeClass::As_Reference(AIRCRAFT_HIND).Cost + AircraftTypeClass::As_Reference(AIRCRAFT_HIND).Cost)/2;
	}
	if (Type == STRUCT_REFINERY) {
		cost -= UnitTypeClass::As_Reference(UNIT_HARVESTER).Cost;
	}
	return(cost);
}


/***********************************************************************************************
 * BuildingTypeClass::Cost_Of -- Fetches the cost of this building.                            *
 *                                                                                             *
 *    This routine will fetch the cost to build the building of this type.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the cost to produce this building.                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Cost_Of(void) const
{
	if (Rule.IsSeparate && Type == STRUCT_HELIPAD) {
		return(Raw_Cost());
	}
	return(TechnoTypeClass::Cost_Of());
}


/***********************************************************************************************
 * BuildingTypeClass::Flush_For_Placement -- Tries to clear placement area for this building t *
 *                                                                                             *
 *    This routine is called when a clear space for placement is desired at the cell location  *
 *    specified. Typical use of this routine is by the computer when it wants to build up      *
 *    its base.                                                                                *
 *                                                                                             *
 * INPUT:   cell  -- The cell that the building of this type would like to be placed down at.  *
 *                                                                                             *
 *          house -- Pointer to the house that want to clear the foundation zone.              *
 *                                                                                             *
 * OUTPUT:  Placement is temporarily blocked, please try again later?                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingTypeClass::Flush_For_Placement(CELL cell, HouseClass * house)  const
{
	bool again = false;
	if (cell > 0) {
		short const * list = Occupy_List(true);

		while (*list != REFRESH_EOL) {
			CELL newcell = cell + *list++;

			if (Map.In_Radar(newcell)) {
				TechnoClass * occupier = Map[newcell].Cell_Techno();
				if (occupier != NULL) {
					again = true;
					if (occupier->House->Is_Ally(house) && occupier->Is_Foot() && !Target_Legal(((FootClass *)occupier)->NavCom)) {
						Map[newcell].Incoming(0, true);
					} else {
//						Base_Is_Attacked(occupier);
					}
				}
			}
		}
	}
	return(again);
}


/***********************************************************************************************
 * BuildingTypeClass::Read_INI -- Fetch building type data from the INI database.              *
 *                                                                                             *
 *    This routine will fetch the building type class data from the INI database file.         *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that will be examined.                      *
 *                                                                                             *
 * OUTPUT:  bool; Was the building entry found and the data extracted?                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingTypeClass::Read_INI(CCINIClass & ini)
{
	if (TechnoTypeClass::Read_INI(ini)) {
		Speed = ini.Get_Bool(Name(), "WaterBound", (Speed == SPEED_FLOAT)) ? SPEED_FLOAT : SPEED_NONE;
		Capacity = ini.Get_Int(Name(), "Storage", Capacity);
		Adjacent = ini.Get_Int(Name(), "Adjacent", Adjacent);
		IsCaptureable = ini.Get_Bool(Name(), "Capturable", IsCaptureable);
		IsPowered = ini.Get_Bool(Name(), "Powered", IsPowered);
		IsBibbed = ini.Get_Bool(Name(), "Bib", IsBibbed);
		IsUnsellable = ini.Get_Bool(Name(), "Unsellable", IsUnsellable);
		IsBase = ini.Get_Bool(Name(), "BaseNormal", IsBase);
		Power = ini.Get_Int(Name(), "Power", (Power > 0) ? Power : -Drain);
		if (Power < 0) {
			Drain = -Power;
			Power = 0;
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * BuildingTypeClass::Coord_Fixup -- Adjusts coordinate to be legal for assignment.            *
 *                                                                                             *
 *    This routine will adjust the specified coordinate so that it will be legal for assignment*
 *    to this building. All buildings are given a coordinate that is in the upper left corner  *
 *    of a cell. This routine will drop the fractional component of the coordinate.            *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to fixup into a legal to assign value.                     *
 *                                                                                             *
 * OUTPUT:  Returns with a coordinate that can be assigned to the building.                    *
 *                                                                                             *
 * WARNINGS:   The coordinate is not examined to see if the cell is legal for placing the      *
 *             building. It merely adjusts the coordinate so that is legal at first glance.    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE BuildingTypeClass::Coord_Fixup(COORDINATE coord) const
{
	return Coord_Whole(coord);
}


/***********************************************************************************************
 * BuildingTypeClass::Full_Name -- Fetches the name to give this building.                     *
 *                                                                                             *
 *    This routine will return the displayable given name for this building type. Normally,    *
 *    this is the official name as well, however in the case of civilian buildings, the        *
 *    name will just be "Civilian Building" unless special options are in place.               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the text number of the building type.                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Full_Name(void) const
{
	if (Debug_Map || Rule.IsNamed || *this < STRUCT_V01 || *this > STRUCT_V37) {
		return(TechnoTypeClass::Full_Name());
	}
	return(TXT_CIVILIAN_BUILDING);
}
