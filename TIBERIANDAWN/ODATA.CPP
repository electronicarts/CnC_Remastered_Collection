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

/* $Header:   F:\projects\c&c\vcs\code\odata.cpv   2.16   16 Oct 1995 16:50:36   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : ODATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 16, 1994                                                 *
 *                                                                                             *
 *                  Last Update : April 19, 1995   [PWG]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   OverlayTypeClass::Display -- Displays a generic representation of overlay.                *
 *   OverlayTypeClass::From_Name -- Determine overlay from ASCII name.                         *
 *   OverlayTypeClass::Init -- Loads graphic data for overlays.                                *
 *   OverlayTypeClass::Occupy_List -- Determines occupation list.                              *
 *   OverlayTypeClass::Prep_For_Add -- Prepares to add overlay to scenario.                    *
 *   OverlayTypeClass::Create_And_Place -- Creates and places a overlay object on the map.     *
 *   OverlayTypeClass::Create_One_Of -- Creates an object of this overlay type.                *
 *   OverlayTypeClass::OverlayTypeClass -- Constructor for overlay type objects.               *
 *   OverlayTypeClass::Draw_It -- Draws the overlay image at location specified.               *
 *   OverlayTypeClass::One_Time -- Loads all the necessary general overlay shape data.         *
 *   OverlayTypeClass::Init -- Initialize the overlay graphic data per theater.                *
 *   OverlayTypeClass::Radar_Icon -- Gets a pointer to the radar icons                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"type.h"


static OverlayTypeClass const Road(
	OVERLAY_ROAD,				// Overlay type number.
	"ROAD",						// INI name of overlay.
	TXT_CONCRETE,				// Full name of overlay.
	LAND_ROAD,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	false,						// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Concrete(
	OVERLAY_CONCRETE,			// Overlay type number.
	"CONC",						// INI name of overlay.
	TXT_CONCRETE,				// Full name of overlay.
	LAND_ROAD,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	false,						// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Sandbag(
	OVERLAY_SANDBAG_WALL,	// Overlay type number.
	"SBAG",						// INI name of overlay.
	TXT_SANDBAG_WALL,			// Full name of overlay.
	LAND_WALL,					// What kind of ground is it?
	1,								// If this is a wall, how many damage levels?
	20,							// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	true,							// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	false,						// Theater specific art?
	true,							// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Cyclone(
	OVERLAY_CYCLONE_WALL,	// Overlay type number.
	"CYCL",						// INI name of overlay.
	TXT_CYCLONE_WALL,			// Full name of overlay.
	LAND_WALL,					// What kind of ground is it?
	2,								// If this is a wall, how many damage levels?
	10,							// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	true,							// Targetable as a destroyable overlay?
	true,							// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	false,						// Theater specific art?
	true,							// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Brick(
	OVERLAY_BRICK_WALL,		// Overlay type number.
	"BRIK",						// INI name of overlay.
	TXT_BRICK_WALL,			// Full name of overlay.
	LAND_WALL,					// What kind of ground is it?
	3,								// If this is a wall, how many damage levels?
	70,							// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	true,							// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	true,							// Stops low level bullets in flight?
	false,						// Theater specific art?
	true,							// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Barbwire(
	OVERLAY_BARBWIRE_WALL,	// Overlay type number.
	"BARB",						// INI name of overlay.
	TXT_BARBWIRE_WALL,		// Full name of overlay.
	LAND_WALL,					// What kind of ground is it?
	1,								// If this is a wall, how many damage levels?
	2,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	true,							// Targetable as a destroyable overlay?
	true,							// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	false,						// Theater specific art?
	true,							// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Wood(
	OVERLAY_WOOD_WALL,		// Overlay type number.
	"WOOD",						// INI name of overlay.
	TXT_WOOD_WALL,				// Full name of overlay.
	LAND_WALL,					// What kind of ground is it?
	1,								// If this is a wall, how many damage levels?
	2,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	true,							// Is it a wooden overlay (affected by fire)?
	true,							// Targetable as a destroyable overlay?
	true,							// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	false,						// Theater specific art?
	true,							// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Tiberium1(
	OVERLAY_TIBERIUM1,		// Overlay type number.
	"TI1",						// INI name of overlay.
	TXT_TIBERIUM,				// Full name of overlay.
	LAND_TIBERIUM,				// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	true,							// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Tiberium2(
	OVERLAY_TIBERIUM2,		// Overlay type number.
	"TI2",						// INI name of overlay.
	TXT_TIBERIUM,				// Full name of overlay.
	LAND_TIBERIUM,				// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	true,							// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Tiberium3(
	OVERLAY_TIBERIUM3,		// Overlay type number.
	"TI3",						// INI name of overlay.
	TXT_TIBERIUM,				// Full name of overlay.
	LAND_TIBERIUM,				// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	true,							// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Tiberium4(
	OVERLAY_TIBERIUM4,		// Overlay type number.
	"TI4",						// INI name of overlay.
	TXT_TIBERIUM,				// Full name of overlay.
	LAND_TIBERIUM,				// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	true,							// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Tiberium5(
	OVERLAY_TIBERIUM5,		// Overlay type number.
	"TI5",						// INI name of overlay.
	TXT_TIBERIUM,				// Full name of overlay.
	LAND_TIBERIUM,				// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	true,							// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Tiberium6(
	OVERLAY_TIBERIUM6,		// Overlay type number.
	"TI6",						// INI name of overlay.
	TXT_TIBERIUM,				// Full name of overlay.
	LAND_TIBERIUM,				// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	true,							// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Tiberium7(
	OVERLAY_TIBERIUM7,		// Overlay type number.
	"TI7",						// INI name of overlay.
	TXT_TIBERIUM,				// Full name of overlay.
	LAND_TIBERIUM,				// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	true,							// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Tiberium8(
	OVERLAY_TIBERIUM8,		// Overlay type number.
	"TI8",						// INI name of overlay.
	TXT_TIBERIUM,				// Full name of overlay.
	LAND_TIBERIUM,				// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	true,							// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Tiberium9(
	OVERLAY_TIBERIUM9,		// Overlay type number.
	"TI9",						// INI name of overlay.
	TXT_TIBERIUM,				// Full name of overlay.
	LAND_TIBERIUM,				// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	true,							// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Tiberium10(
	OVERLAY_TIBERIUM10,		// Overlay type number.
	"TI10",						// INI name of overlay.
	TXT_TIBERIUM,				// Full name of overlay.
	LAND_TIBERIUM,				// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	true,							// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Tiberium11(
	OVERLAY_TIBERIUM11,		// Overlay type number.
	"TI11",						// INI name of overlay.
	TXT_TIBERIUM,				// Full name of overlay.
	LAND_TIBERIUM,				// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	true,							// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Tiberium12(
	OVERLAY_TIBERIUM12,		// Overlay type number.
	"TI12",						// INI name of overlay.
	TXT_TIBERIUM,				// Full name of overlay.
	LAND_TIBERIUM,				// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	true,							// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const Squish(
	OVERLAY_SQUISH,			// Overlay type number.
	"SQUISH",					// INI name of overlay.
	TXT_SQUISH,					// Full name of overlay.
	LAND_CLEAR,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	false,						// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	false,						// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);

static OverlayTypeClass const V12(
	OVERLAY_V12,				// Overlay type number.
	"V12",						// INI name of overlay.
	TXT_CIV12,					// Full name of overlay.
	LAND_ROCK,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	false,						// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	true,							// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const V13(
	OVERLAY_V13,				// Overlay type number.
	"V13",						// INI name of overlay.
	TXT_CIV13,					// Full name of overlay.
	LAND_ROCK,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	false,						// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	true,							// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const V14(
	OVERLAY_V14,				// Overlay type number.
	"V14",						// INI name of overlay.
	TXT_CIV14,					// Full name of overlay.
	LAND_ROCK,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	false,						// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	true,							// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const V15(
	OVERLAY_V15,				// Overlay type number.
	"V15",						// INI name of overlay.
	TXT_CIV15,					// Full name of overlay.
	LAND_ROCK,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	false,						// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	true,							// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const V16(
	OVERLAY_V16,				// Overlay type number.
	"V16",						// INI name of overlay.
	TXT_CIV16,					// Full name of overlay.
	LAND_ROCK,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	false,						// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	true,							// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const V17(
	OVERLAY_V17,				// Overlay type number.
	"V17",						// INI name of overlay.
	TXT_CIV17,					// Full name of overlay.
	LAND_ROCK,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	false,						// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	true,							// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const V18(
	OVERLAY_V18,				// Overlay type number.
	"V18",						// INI name of overlay.
	TXT_CIV18,					// Full name of overlay.
	LAND_ROCK,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	false,						// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	true,							// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	true,							// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const FlagSpot(
	OVERLAY_FLAG_SPOT,		// Overlay type number.
	"FPLS",						// INI name of overlay.
	TXT_FLAG_SPOT,				// Full name of overlay.
	LAND_CLEAR,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	true,							// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	false,						// Theater specific art?
	false,						// Is this a wall type?
	false							// Is this a crate?
);
static OverlayTypeClass const WoodCrate(
	OVERLAY_WOOD_CRATE,		// Overlay type number.
	"WCRATE",					// INI name of overlay.
	TXT_WOOD_CRATE,			// Full name of overlay.
	LAND_CLEAR,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	false,						// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	false,						// Theater specific art?
	false,						// Is this a wall type?
	true							// Is this a crate?
);
static OverlayTypeClass const SteelCrate(
	OVERLAY_STEEL_CRATE,		// Overlay type number.
	"SCRATE",					// INI name of overlay.
	TXT_STEEL_CRATE,			// Full name of overlay.
	LAND_CLEAR,					// What kind of ground is it?
	0,								// If this is a wall, how many damage levels?
	0,								// If this is a wall, how many damage points can it take per level?
	false,						// Visible on the radar map?
	false,						// Is it a wooden overlay (affected by fire)?
	false,						// Targetable as a destroyable overlay?
	false,						// Crushable by tracked vehicle?
	false,						// Is this harvestable Tiberium?
	false,						// Stops low level bullets in flight?
	false,						// Theater specific art?
	false,						// Is this a wall type?
	true							// Is this a crate?
);


OverlayTypeClass const * const OverlayTypeClass::Pointers[OVERLAY_COUNT] = {
	&Concrete,					//	OVERLAY_CONCRETE
	&Sandbag,					//	OVERLAY_SANDBAG_WALL
	&Cyclone,					//	OVERLAY_CYCLONE_WALL
	&Brick,						//	OVERLAY_BRICK_WALL
	&Barbwire,					//	OVERLAY_BARBWIRE_WALL
	&Wood,						//	OVERLAY_WOOD_WALL
	&Tiberium1,					//	OVERLAY_TIBERIUM1
	&Tiberium2,					//	OVERLAY_TIBERIUM2
	&Tiberium3,					//	OVERLAY_TIBERIUM3
	&Tiberium4,					//	OVERLAY_TIBERIUM4
	&Tiberium5,					//	OVERLAY_TIBERIUM5
	&Tiberium6,					//	OVERLAY_TIBERIUM6
	&Tiberium7,					//	OVERLAY_TIBERIUM7
	&Tiberium8,					//	OVERLAY_TIBERIUM8
	&Tiberium9,					//	OVERLAY_TIBERIUM9
	&Tiberium10,				//	OVERLAY_TIBERIUM10
	&Tiberium11,				//	OVERLAY_TIBERIUM11
	&Tiberium12,				//	OVERLAY_TIBERIUM12
	&Road,						// OVERLAY_ROAD
	&Squish,						// OVERLAY_SQUISH
	&V12,							//	OVERLAY_V12
	&V13,							//	OVERLAY_V13
	&V14,							//	OVERLAY_V14
	&V15,							//	OVERLAY_V15
	&V16,							//	OVERLAY_V16
	&V17,							//	OVERLAY_V17
	&V18,							//	OVERLAY_V18
	&FlagSpot,					//	OVERLAY_FLAG_SPOT
	&WoodCrate,					// OVERLAY_WOOD_CRATE
	&SteelCrate,				// OVERLAY_STEEL_CRATE
};


/***********************************************************************************************
 * OverlayTypeClass::OverlayTypeClass -- Constructor for overlay type objects.                 *
 *                                                                                             *
 *    This is the constructor for the overlay types.                                           *
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
OverlayTypeClass::OverlayTypeClass(
	OverlayType iconset,
	char const *ininame,
	int fullname,
	LandType ground,
	int  damagelevels,
	int  damagepoints,
	bool isradarvisible,
	bool iswooden,
	bool istarget,
	bool iscrushable,
	bool istiberium,
	bool high,
	bool theater,
	bool walltype,
	bool iscrate) :
		ObjectTypeClass(false,
							false,
							iscrushable,
							true,
							false,
							istarget,
							true,
							false,
							fullname,
							ininame,
							ARMOR_NONE,
							0)
{
	IsRadarVisible = isradarvisible;
	IsCrate = iscrate;
	IsWooden = iswooden;
	IsHigh = high;
	IsTheater = theater;
	IsTiberium = istiberium;
	Type = iconset;
	Land = ground;
	IsWall = walltype;
	DamageLevels = damagelevels;
	DamagePoints = damagepoints;
}


/***********************************************************************************************
 * OverlayTypeClass::One_Time -- Loads all the necessary general overlay shape data.           *
 *                                                                                             *
 *    This routine should be called once when the game first starts. It will establish         *
 *    pointers to the graphic data of the overlay objects.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void OverlayTypeClass::One_Time(void)
{
}


/***********************************************************************************************
 * OverlayTypeClass::From_Name -- Determine overlay from ASCII name.                           *
 *                                                                                             *
 *    This routine is used to determine the overlay number given only                          *
 *    an ASCII representation. The scenario loader uses this routine                           *
 *    to construct the map from the INI control file.                                          *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII name of the overlay.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the overlay number. If the name had no match,                         *
 *          then returns with OVERLAY_NONE.                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
OverlayType OverlayTypeClass::From_Name(char const *name)
{
	if (name) {
		for (OverlayType index = OVERLAY_FIRST; index < OVERLAY_COUNT; index++) {
			if (stricmp(As_Reference(index).IniName, name) == 0) {
				return(index);
			}
		}
	}
	return(OVERLAY_NONE);
}


/***********************************************************************************************
 * OverlayTypeClass::Occupy_List -- Determines occupation list.                                *
 *                                                                                             *
 *    This routine is used to examine the overlay map and build an                             *
 *    occupation list. This list is used to render a overlay cursor as                         *
 *    well as placement of icon numbers.                                                       *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the overlay occupation list.                             *
 *                                                                                             *
 * WARNINGS:   The return pointer is valid only until the next time that                       *
 *             this routine is called.                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * OverlayTypeClass::Occupy_List(bool) const
{
	static short _simple[] = {0, REFRESH_EOL};

	return(_simple);
}


/***************************************************************************
 * OverlayTypeClass::Radar_Icon -- Gets a pointer to the radar icons       *
 *                                                                         *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/19/1995 PWG : Created.                                             *
 *=========================================================================*/
unsigned char * OverlayTypeClass::Radar_Icon(int data) const
{
	unsigned char *icon = (unsigned char *)Get_Radar_Data();		// Get pointer to radar icons
	icon 					 += (data * 9) + 2;			// move icon ptr to correct icon
	return(icon);											// Return the correct icon
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * OverlayTypeClass::Display -- Displays a generic representation of overlay.                  *
 *                                                                                             *
 *    This routine is used to display a generic view of the overlay                            *
 *    object. This is necessary for selection in the scenario editor.                          *
 *                                                                                             *
 * INPUT:   x,y   -- The coordinates to center the display about.                              *
 *                                                                                             *
 *          window-- The window to base the coordinates upon.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void OverlayTypeClass::Display(int x, int y, WindowNumberType window, HousesType ) const
{
	/*
	---------------------------- Draw the shape ------------------------------
	*/
	if (Get_Image_Data()) {
		int frame = 0;

		if (IsTiberium) {
			frame = 7;
		}

		CC_Draw_Shape(Get_Image_Data(), frame, x, y, window, SHAPE_NORMAL|SHAPE_CENTER|SHAPE_WIN_REL);
	}
}


/***********************************************************************************************
 * OverlayTypeClass::Prep_For_Add -- Prepares to add overlay to scenario.                      *
 *                                                                                             *
 *    This routine prepares a list of overlay objects so that the                              *
 *    scenario editor can use this list to display a dialog box. The                           *
 *    selection of a overlay object will allow its placement upon the                          *
 *    map.                                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1994 JLB : Created                                                                  *
 *=============================================================================================*/
void OverlayTypeClass::Prep_For_Add(void)
{
	for (OverlayType index = OVERLAY_FIRST; index < OVERLAY_COUNT; index++) {
		OverlayTypeClass const & overlay = As_Reference(index);
		if (overlay.Get_Image_Data() && !overlay.IsWall && (!overlay.IsTiberium || index == OVERLAY_TIBERIUM1)) {
			Map.Add_To_List(&overlay);
		}
	}
}
#endif


/***********************************************************************************************
 * OverlayTypeClass::Create_And_Place -- Creates and places a overlay object on the map.       *
 *                                                                                             *
 *    This support routine is used by the scenario editor to add a overlay object to the map   *
 *    and to the game.                                                                         *
 *                                                                                             *
 * INPUT:   cell  -- The cell to place the overlay object.                                     *
 *                                                                                             *
 * OUTPUT:  bool; Was the overlay object placed successfully?                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool OverlayTypeClass::Create_And_Place(CELL cell, HousesType ) const
{
	if (new OverlayClass(Type, cell)) {
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * OverlayTypeClass::Create_One_Of -- Creates an object of this overlay type.                  *
 *                                                                                             *
 *    This routine will create an object of this type. For certain overlay objects, such       *
 *    as walls, it is actually created as a building. The "building" wall is converted into    *
 *    a overlay at the moment of placing down on the map.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the appropriate object for this overlay type.            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * OverlayTypeClass::Create_One_Of(HouseClass *) const
{
	return(new OverlayClass(Type, -1));
}


/***********************************************************************************************
 * OverlayTypeClass::Draw_It -- Draws the overlay image at location specified.                 *
 *                                                                                             *
 *    This routine will draw the overlay shape at the coordinates specified. It is presumed    *
 *    that all the underlying layers have already been rendered by the time this routine is    *
 *    called.                                                                                  *
 *                                                                                             *
 * INPUT:   x, y  -- Coordinate (upper left) of cell where overlay image is to be drawn.       *
 *                                                                                             *
 *          data  -- Cell specific data that controls the imagery of the overlay.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void OverlayTypeClass::Draw_It(int x, int y, int data) const
{
	CC_Draw_Shape(Get_Image_Data(), data, Map.TacPixelX+x+(CELL_PIXEL_W>>1), Map.TacPixelY+y+(CELL_PIXEL_H>>1), WINDOW_MAIN, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_GHOST, NULL, Map.UnitShadow);
}


/***********************************************************************************************
 * OverlayTypeClass::Init -- Initialize the overlay graphic data per theater.                  *
 *                                                                                             *
 *    This routine will update the overlay graphic data according to the theater specified.    *
 *    It is typically called when the scenario is first loaded (theater change).               *
 *                                                                                             *
 * INPUT:   theater  -- The theater to load specific data for.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *    4/25/1996 ST : Modified to load theater specific sidebar icons if available              *
 *=============================================================================================*/
void OverlayTypeClass::Init(TheaterType theater)
{
	if (theater != LastTheater){
		for (OverlayType index = OVERLAY_FIRST; index < OVERLAY_COUNT; index++) {
			OverlayTypeClass const & overlay = As_Reference(index);
			char fullname[_MAX_FNAME+_MAX_EXT];	// Fully constructed iconset name.

			if (overlay.IsTheater) {
				_makepath(fullname, NULL, NULL, overlay.IniName, Theaters[theater].Suffix);
			} else {
				_makepath(fullname, NULL, NULL, overlay.IniName, ".SHP");
			}
			((void const *&)overlay.ImageData) = MixFileClass::Retrieve(fullname);

			IsTheaterShape = overlay.IsTheater;
			if (overlay.RadarIcon) delete[] (char *)overlay.RadarIcon;
			((void const *&)overlay.RadarIcon) = Get_Radar_Icon(overlay.Get_Image_Data(), 0, -1, 3);
			IsTheaterShape = false;
		}
	}
}
