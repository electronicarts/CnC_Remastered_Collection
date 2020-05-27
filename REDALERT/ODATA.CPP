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

/* $Header: /CounterStrike/ODATA.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
 *                  Last Update : August 14, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   OverlayTypeClass::As_Reference -- Fetch a reference to the overlay type specified.        *
 *   OverlayTypeClass::Coord_Fixup -- Adjust the coord to be legal for assignment.             *
 *   OverlayTypeClass::Create_And_Place -- Creates and places a overlay object on the map.     *
 *   OverlayTypeClass::Create_One_Of -- Creates an object of this overlay type.                *
 *   OverlayTypeClass::Display -- Displays a generic representation of overlay.                *
 *   OverlayTypeClass::Draw_It -- Draws the overlay image at location specified.               *
 *   OverlayTypeClass::From_Name -- Determine overlay from ASCII name.                         *
 *   OverlayTypeClass::Init -- Initialize the overlay graphic data per theater.                *
 *   OverlayTypeClass::Init_Heap -- Initialize the overlay type class heap.                    *
 *   OverlayTypeClass::Occupy_List -- Determines occupation list.                              *
 *   OverlayTypeClass::One_Time -- Loads all the necessary general overlay shape data.         *
 *   OverlayTypeClass::OverlayTypeClass -- Constructor for overlay type objects.               *
 *   OverlayTypeClass::Prep_For_Add -- Prepares to add overlay to scenario.                    *
 *   OverlayTypeClass::Radar_Icon -- Gets a pointer to the radar icons                         *
 *   OverlayTypeClass::operator delete -- Returns an overlay type object back to the pool.     *
 *   OverlayTypeClass::operator new -- Allocate an overlay type class object from pool.        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"type.h"


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
	true,							// Crushable by tracked vehicle?
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
static OverlayTypeClass const Fence(
	OVERLAY_FENCE,	// Overlay type number.
	"FENC",						// INI name of overlay.
	TXT_FENCE,					// Full name of overlay.
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

static OverlayTypeClass const Gold1(
	OVERLAY_GOLD1,				// Overlay type number.
	"GOLD01",					// INI name of overlay.
	TXT_GOLD,					// Full name of overlay.
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
static OverlayTypeClass const Gold2(
	OVERLAY_GOLD2,				// Overlay type number.
	"GOLD02",					// INI name of overlay.
	TXT_GOLD,					// Full name of overlay.
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
static OverlayTypeClass const Gold3(
	OVERLAY_GOLD3,				// Overlay type number.
	"GOLD03",					// INI name of overlay.
	TXT_GOLD,					// Full name of overlay.
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
static OverlayTypeClass const Gold4(
	OVERLAY_GOLD4,				// Overlay type number.
	"GOLD04",					// INI name of overlay.
	TXT_GOLD,					// Full name of overlay.
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

static OverlayTypeClass const Gems1(
	OVERLAY_GEMS1,				// Overlay type number.
	"GEM01",						// INI name of overlay.
	TXT_GEMS,					// Full name of overlay.
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
static OverlayTypeClass const Gems2(
	OVERLAY_GEMS2,				// Overlay type number.
	"GEM02",						// INI name of overlay.
	TXT_GEMS,					// Full name of overlay.
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
static OverlayTypeClass const Gems3(
	OVERLAY_GEMS3,				// Overlay type number.
	"GEM03",						// INI name of overlay.
	TXT_GEMS,					// Full name of overlay.
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
static OverlayTypeClass const Gems4(
	OVERLAY_GEMS4,				// Overlay type number.
	"GEM04",						// INI name of overlay.
	TXT_GEMS,					// Full name of overlay.
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
static OverlayTypeClass const WaterCrate(
	OVERLAY_WATER_CRATE,		// Overlay type number.
	"WWCRATE",					// INI name of overlay.
	TXT_WATER_CRATE,			// Full name of overlay.
	LAND_WATER,					// What kind of ground is it?
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
	char const * ininame,
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
		ObjectTypeClass(RTTI_OVERLAYTYPE,
							int(iconset),
							false,
							true,
							false,
							istarget,
							true,
							false,
							false,
							fullname,
							ininame),
	Type(iconset),
	Land(ground),
	DamageLevels(damagelevels),
	DamagePoints(damagepoints),
	IsTheater(theater),
	IsWall(walltype),
	IsHigh(high),
	IsTiberium(istiberium),
	IsWooden(iswooden),
	IsCrate(iscrate),
	IsRadarVisible(isradarvisible)
{
	IsCrushable = iscrushable;
}


/***********************************************************************************************
 * OverlayTypeClass::operator new -- Allocate an overlay type class object from pool.          *
 *                                                                                             *
 *    This will allocate an overlay type class object from the special memory pool that is     *
 *    for that purpose.                                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the overlay type class object allocated. If there is     *
 *          insufficient memory to fulfill the request, then NULL is returned.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * OverlayTypeClass::operator new(size_t)
{
	return(OverlayTypes.Alloc());
}


/***********************************************************************************************
 * OverlayTypeClass::operator delete -- Returns an overlay type object back to the pool.       *
 *                                                                                             *
 *    This will return a previously allcoated overaly type object to the special memory        *
 *    pool that it was allocated from.                                                         *
 *                                                                                             *
 * INPUT:   pointer  -- Pointer to the overlay type class object to return the memory pool.    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void OverlayTypeClass::operator delete(void * pointer)
{
	OverlayTypes.Free((OverlayTypeClass *)pointer);
}


/***********************************************************************************************
 * OverlayTypeClass::Init_Heap -- Initialize the overlay type class heap.                      *
 *                                                                                             *
 *    This will initialize the overlay type heap by pre-allocated all overlay types known      *
 *    to exist.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   It should be called once and before the rules.ini file is processed.            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void OverlayTypeClass::Init_Heap(void)
{
	/*
	**	These overlay type class objects must be allocated in the exact order that they
	**	are specified in the OverlayType enumeration. This is necessary because the heap
	**	allocation block index serves double duty as the type number index.
	*/
	new OverlayTypeClass(Sandbag);		//	OVERLAY_SANDBAG_WALL
	new OverlayTypeClass(Cyclone);		//	OVERLAY_CYCLONE_WALL
	new OverlayTypeClass(Brick);			//	OVERLAY_BRICK_WALL
	new OverlayTypeClass(Barbwire);		//	OVERLAY_BARBWIRE_WALL
	new OverlayTypeClass(Wood);			//	OVERLAY_WOOD_WALL
	new OverlayTypeClass(Gold1);			// OVERLAY_GOLD1
	new OverlayTypeClass(Gold2);			// OVERLAY_GOLD2
	new OverlayTypeClass(Gold3);			// OVERLAY_GOLD3
	new OverlayTypeClass(Gold4);			// OVERLAY_GOLD4
	new OverlayTypeClass(Gems1);			// OVERLAY_GEMS1
	new OverlayTypeClass(Gems2);			// OVERLAY_GEMS2
	new OverlayTypeClass(Gems3);			// OVERLAY_GEMS3
	new OverlayTypeClass(Gems4);			// OVERLAY_GEMS4
	new OverlayTypeClass(V12);				//	OVERLAY_V12
	new OverlayTypeClass(V13);				//	OVERLAY_V13
	new OverlayTypeClass(V14);				//	OVERLAY_V14
	new OverlayTypeClass(V15);				//	OVERLAY_V15
	new OverlayTypeClass(V16);				//	OVERLAY_V16
	new OverlayTypeClass(V17);				//	OVERLAY_V17
	new OverlayTypeClass(V18);				//	OVERLAY_V18
	new OverlayTypeClass(FlagSpot);		//	OVERLAY_FLAG_SPOT
	new OverlayTypeClass(WoodCrate);		// OVERLAY_WOOD_CRATE
	new OverlayTypeClass(SteelCrate);	// OVERLAY_STEEL_CRATE
	new OverlayTypeClass(Fence);			// OVERLAY_FENCE
	new OverlayTypeClass(WaterCrate);	// OVERLAY_WATER_CRATE
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
OverlayType OverlayTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
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
	unsigned char * icon = (unsigned char *)Get_Radar_Data();		// Get pointer to radar icons
	if (icon != NULL) icon += (data * 9) + 2;			// move icon ptr to correct icon
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
	if (Get_Image_Data() != NULL) {
		int frame = 0;

		if (IsTiberium) {
			frame = 7;
		}
		if (Type == OVERLAY_GEMS1 || Type == OVERLAY_GEMS2 || Type == OVERLAY_GEMS3 || Type == OVERLAY_GEMS4) {
			frame = 2;
		}

		IsTheaterShape = IsTheater;
		CC_Draw_Shape(Get_Image_Data(), frame, x, y, window, SHAPE_NORMAL|SHAPE_CENTER|SHAPE_WIN_REL);
		IsTheaterShape = false;
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
		if (overlay.Get_Image_Data() != NULL &&
			!overlay.IsWall &&
			(!overlay.IsTiberium || index == OVERLAY_GOLD1 || index == OVERLAY_GEMS1)) {

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
	IsTheaterShape = IsTheater;
	CC_Draw_Shape(Get_Image_Data(), data, Map.TacPixelX+x+(CELL_PIXEL_W>>1), Map.TacPixelY+y+(CELL_PIXEL_H>>1), WINDOW_MAIN, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_GHOST, NULL, Map.UnitShadow);
	IsTheaterShape = false;
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
 *=============================================================================================*/
void OverlayTypeClass::Init(TheaterType theater)
{
	if (theater != LastTheater) {

		for (OverlayType index = OVERLAY_FIRST; index < OVERLAY_COUNT; index++) {
			OverlayTypeClass & overlay = As_Reference(index);
			char fullname[_MAX_FNAME+_MAX_EXT];	// Fully constructed iconset name.

			if (overlay.IsTheater) {
				_makepath(fullname, NULL, NULL, overlay.IniName, Theaters[theater].Suffix);
			} else {
				_makepath(fullname, NULL, NULL, overlay.IniName, ".SHP");
			}
			overlay.ImageData = MFCD::Retrieve(fullname);

			IsTheaterShape = overlay.IsTheater;		//Tell Build_Frame if this is a theater specific shape
			if (overlay.RadarIcon != NULL) delete[] (char *)overlay.RadarIcon;
			overlay.RadarIcon = Get_Radar_Icon(overlay.Get_Image_Data(), 0, -1, 3);
			IsTheaterShape = false;
		}
	}
}


/***********************************************************************************************
 * OverlayTypeClass::As_Reference -- Fetch a reference to the overlay type specified.          *
 *                                                                                             *
 *    Use this routine to get a reference that corresponds to the overlay type.                *
 *                                                                                             *
 * INPUT:   type  -- The overlay type to fetch a reference to.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the overlay type specified.                            *
 *                                                                                             *
 * WARNINGS:   Be sure that the overlay type specified is legal. Illegal type value will       *
 *             result in undefined behavior.                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
OverlayTypeClass & OverlayTypeClass::As_Reference(OverlayType type)
{
	return(*OverlayTypes.Ptr(type));
}


/***********************************************************************************************
 * OverlayTypeClass::Coord_Fixup -- Adjust the coord to be legal for assignment.               *
 *                                                                                             *
 *    This will adjust the coordinate specified so that it will be of legal format to          *
 *    assign as the coordinate of an overlay. Overlays are always relative to the upper left   *
 *    corner of the cell, so this routine drops the fractional cell components.                *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to fixup to be legal for assignment.                       *
 *                                                                                             *
 * OUTPUT:  Returns with a properly fixed up coordinate.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE OverlayTypeClass::Coord_Fixup(COORDINATE coord) const
{
	return Coord_Whole(coord);
}
