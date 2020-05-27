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

/* $Header: /CounterStrike/UDATA.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : UDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : July 19, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   UnitTypeClass::As_Reference -- Fetches a reference to the unit type class specified.      *
 *   UnitTypeClass::Create_And_Place -- Creates and places a unit object onto the map.         *
 *   UnitTypeClass::Create_One_Of -- Creates a unit in limbo.                                  *
 *   UnitTypeClass::Dimensions -- Determines the unit's pixel dimensions.                      *
 *   UnitTypeClass::Display -- Displays a generic unit shape.                                  *
 *   UnitTypeClass::From_Name -- Fetch class pointer from specified name.                      *
 *   UnitTypeClass::Init_Heap -- Initialize the unit type class heap.                          *
 *   UnitTypeClass::Max_Pips -- Fetches the maximum pips allowed for this unit.                *
 *   UnitTypeClass::One_Time -- Performs one time processing for unit type class objects.      *
 *   UnitTypeClass::Prep_For_Add -- Prepares scenario editor to add unit.                      *
 *   UnitTypeClass::Read_INI -- Fetch the unit type data from the INI database.                *
 *   UnitTypeClass::Turret_Adjust -- Turret adjustment routine for MLRS and MSAM units.        *
 *   UnitTypeClass::UnitTypeClass -- Constructor for unit types.                               *
 *   UnitTypeClass::operator delete -- Return a unit type class object back to the pool.       *
 *   UnitTypeClass::operator new -- Allocates an object from the unit type class heap.         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

/*
**	This is the list of animation stages to use when the harvester
**	is to dump its load into the refinery. The offsets are based from the
**	start of the dump animation.
*/
const int UnitTypeClass::Harvester_Dump_List[22] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,6,5,4,3,2,1,0};
const int UnitTypeClass::Harvester_Load_List[9] = {0, 1, 2, 3, 4, 5, 6, 7, 0};
const int UnitTypeClass::Harvester_Load_Count = 8;


// V2 rocket launcher
static UnitTypeClass const UnitV2Launcher(
	UNIT_V2_LAUNCHER,
	TXT_V2_LAUNCHER,		// NAME:			Text name of this unit type.
	"V2RL",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		true,					// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Light tank
static UnitTypeClass const UnitLTank(
	UNIT_LTANK,
	TXT_LTANK,				// NAME:			Text name of this unit type.
	"1TNK",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0020,					//	Vertical offset.
	0x00C0,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		true,					// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		true,					// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		false,				// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Heavy tank
static UnitTypeClass const UnitMTank(
	UNIT_MTANK,
	TXT_MTANK,				// NAME:			Text name of this unit type.
	"3TNK",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0040,					//	Vertical offset.
	0x0080,					// Primary weapon offset along turret centerline.
	0x0018,					// Primary weapon lateral offset along turret centerline.
	0x0080,					// Secondary weapon offset along turret centerline.
	0x0018,					// Secondary weapon lateral offset along turret centerling.
		true,					// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		true,					// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Medium tank
static UnitTypeClass const UnitMTank2(
	UNIT_MTANK2,
	TXT_MTANK2,				// NAME:			Text name of this unit type.
	"2TNK",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0030,					//	Vertical offset.
	0x00C0,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x00C0,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		true,					// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		true,					// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Mastadon tank
static UnitTypeClass const UnitHTank(
	UNIT_HTANK,
	TXT_HTANK,				// NAME:			Text name of this unit type.
	"4TNK",					// NAME:			Text name of this unit type.
	ANIM_ART_EXP1,			// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0020,					//	Vertical offset.
	0x00C0,					// Primary weapon offset along turret centerline.
	0x0028,					// Primary weapon lateral offset along turret centerline.
	0x0008,					// Secondary weapon offset along turret centerline.
	0x0040,					// Secondary weapon lateral offset along turret centerling.
		true,					// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		true,					// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Mobile Radar Jammer
static UnitTypeClass const UnitMRJammer(
	UNIT_MRJ,
	TXT_MRJ,					// NAME:			Text name of this unit type.
	"MRJ",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		true,					// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		true,					// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		false,				// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		true,					// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Mobile Gap Generator
static UnitTypeClass const UnitMGG(
	UNIT_MGG,
	TXT_MGG,					// NAME:			Text name of this unit type.
	"MGG",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		true,					// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		true,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Artillery
static UnitTypeClass const UnitArty(
	UNIT_ARTY,
	TXT_ARTY,				// NAME:			Text name of this unit type.
	"ARTY",					// NAME:			Text name of this unit type.
	ANIM_ART_EXP1,			// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0040,					//	Vertical offset.
	0x0060,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		true,					// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		false,				// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		false,				// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Harvester
static UnitTypeClass const UnitHarvester(
	UNIT_HARVESTER,
	TXT_HARVESTER,			// NAME:			Text name of this unit type.
	"HARV",					// NAME:			Text name of this unit type.
	ANIM_FBALL1,			// EXPLOSION:	Type of explosion when destroyed.
	REMAP_ALTERNATE,		// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		true,					// Can this be a goodie surprise from a crate?
		true,					// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		true,					// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HARVEST		// ORDERS:		Default order to give new unit.
);

// Mobile construction vehicle
static UnitTypeClass const UnitMCV(
	UNIT_MCV,
	TXT_MCV,					// NAME:			Text name of this unit type.
	"MCV",					// NAME:			Text name of this unit type.
	ANIM_FBALL1,			// EXPLOSION:	Type of explosion when destroyed.
	REMAP_ALTERNATE,		// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		true,					// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Jeep (hummer)
static UnitTypeClass const UnitJeep(
	UNIT_JEEP,
	TXT_JEEP,				// NAME:			Text name of this unit type.
	"JEEP",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0030,					//	Vertical offset.
	0x0030,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0030,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		true,					// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		false,				// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		true,					// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		false,				// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Armored personnel carrier
static UnitTypeClass const UnitAPC(
	UNIT_APC,
	TXT_APC,					// NAME:			Text name of this unit type.
	"APC",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0030,					//	Vertical offset.
	0x0030,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0030,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		true,					// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		false,				// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Mine laying truck
static UnitTypeClass const UnitMineLayer(
	UNIT_MINELAYER,
	TXT_MINE_LAYER,		// NAME:			Text name of this unit type.
	"MNLY",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		true,					// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		false,				// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Convoy Truck
static UnitTypeClass const UnitConvoyTruck(
	UNIT_TRUCK,
	TXT_TRUCK,				// NAME:			Text name of this unit type.
	"TRUK",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		false,				// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		false,				// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_GUARD			// ORDERS:		Default order to give new unit.
);

#ifdef FIXIT_ANTS
/*
[ANT]
Name=Warrior Ant
Primary=Mandible
Strength=150
Armor=light
TechLevel=-1
Sight=2
Speed=5
Cost=700
Points=40
ROT=5
Tracked=yes
Crewed=no
NoMovingFire=yes

; Ant mandible
[Mandible]
Damage=50
ROF=5
Range=1.5
Projectile=Invisible
Speed=100
Warhead=HollowPoint
Report=none


*/

// Warrior ant
static UnitTypeClass const UnitAnt1(
	UNIT_ANT1,
	TXT_NONE,				// NAME:			Text name of this unit type.
	"ANT1",					// NAME:			Text name of this unit type.
	ANIM_ANT1_DEATH,		// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Can this be a goodie surprise from a crate?
		true,					// Always use the given name for the vehicle?
		false,				// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		true,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	8,							// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);
static UnitTypeClass const UnitAnt2(
	UNIT_ANT2,
	TXT_NONE,				// NAME:			Text name of this unit type.
	"ANT2",					// NAME:			Text name of this unit type.
	ANIM_ANT2_DEATH,		// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Can this be a goodie surprise from a crate?
		true,					// Always use the given name for the vehicle?
		false,				// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		true,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	8,							// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);
static UnitTypeClass const UnitAnt3(
	UNIT_ANT3,
	TXT_NONE,				// NAME:			Text name of this unit type.
	"ANT3",					// NAME:			Text name of this unit type.
	ANIM_ANT3_DEATH,		// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Can this be a goodie surprise from a crate?
		true,					// Always use the given name for the vehicle?
		false,				// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		true,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	8,							// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);
#endif

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
// Chrono Tank
static UnitTypeClass const UnitChrono(
	UNIT_CHRONOTANK,
	TXT_CHRONOTANK,		// NAME:			Text name of this unit type.
	"CTNK",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// Tesla Tank
static UnitTypeClass const UnitTesla(
	UNIT_TESLATANK,
	TXT_TESLATANK,			// NAME:			Text name of this unit type.
	"TTNK",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		true,					// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		true,					// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		true,					// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

// M.A.D. Tank
static UnitTypeClass const UnitMAD(
	UNIT_MAD,
	TXT_MAD,					// NAME:			Text name of this unit type.
	"QTNK",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		true,					// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);


// Demolition Truck
static UnitTypeClass const UnitDemoTruck(
	UNIT_DEMOTRUCK,
	TXT_DEMOTRUCK,			// NAME:			Text name of this unit type.
	"DTRK",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		false,				// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
		false,				// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_GUARD			// ORDERS:		Default order to give new unit.
);
#ifdef FIXIT_PHASETRANSPORT	//	checked - ajw 9/28/98
static UnitTypeClass const UnitPhase(
	UNIT_PHASE,
	TXT_PHASETRANSPORT,	// NAME:			Text name of this unit type.
	"STNK",					// NAME:			Text name of this unit type.
	ANIM_FRAG1,				// EXPLOSION:	Type of explosion when destroyed.
	REMAP_NORMAL,			// Sidebar remap logic.
	0x0030,					//	Vertical offset.
	0x0030,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0030,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Can this be a goodie surprise from a crate?
		false,				// Always use the given name for the vehicle?
		true,					// Can this unit squash infantry?
		false,				// Does this unit harvest Tiberium?
		false,				// Is invisible to radar?
		false,				// Is it insignificant (won't be announced)?
		true,					// Is it equipped with a combat turret?
		false,				// Does it have a rotating radar dish?
		false,				// Is there an associated firing animation?
		false,				// Must the turret be in a locked down position while moving?
true,//		false,				// Is this a gigundo-rotund-enormous unit?
		false,				// Does the unit have a constant animation?
		false,				// Is the unit capable of jamming radar?
		false,				// Is the unit a mobile gap generator?
	32,						// Rotation stages.
	0,							// Turret center offset along body centerline.
	MISSION_HUNT			// ORDERS:		Default order to give new unit.
);

#endif
#endif

/***********************************************************************************************
 * UnitTypeClass::UnitTypeClass -- Constructor for unit types.                                 *
 *                                                                                             *
 *    This is the constructor for the unit types. It is used to initialize the unit type class *
 *    structure. The unit type class is used to control the behavior of the various types      *
 *    of units in the game. This constructor is called for every unique unit type as it        *
 *    exists in the array of unit types.                                                       *
 *                                                                                             *
 * INPUT:   bla bla bla... see below                                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
UnitTypeClass::UnitTypeClass(
				UnitType type,
				int name,
				char const * ininame,
				AnimType exp,
				RemapType remap,
				int verticaloffset,
				int primaryoffset,
				int primarylateral,
				int secondaryoffset,
				int secondarylateral,
				bool is_goodie,
				bool is_nominal,
				bool is_crusher,
				bool is_harvest,
				bool is_stealthy,
				bool is_insignificant,
				bool is_turret_equipped,
				bool is_radar_equipped,
				bool is_fire_anim,
				bool is_lock_turret,
				bool is_gigundo,
				bool is_animating,
				bool is_jammer,
				bool is_gapper,
				int rotation,
				int toffset,
				MissionType order) :
					TechnoTypeClass(RTTI_UNITTYPE,
										int(type),
										name,
										ininame,
										remap,
										verticaloffset,
										primaryoffset,
										primarylateral,
										secondaryoffset,
										secondarylateral,
										is_nominal,
										is_stealthy,
										true,
										true,
										is_insignificant,
										false,
										false,
										is_turret_equipped,
										true,
										true,
										rotation,
										SPEED_TRACK),
	IsCrateGoodie(is_goodie),
	IsCrusher(is_crusher),
	IsToHarvest(is_harvest),
	IsRadarEquipped(is_radar_equipped),
	IsFireAnim(is_fire_anim),
	IsLockTurret(is_lock_turret),
	IsGigundo(is_gigundo),
	IsAnimating(is_animating),
	IsJammer(is_jammer),
	IsGapper(is_gapper),
	IsNoFireWhileMoving(false),
	Type(type),
	TurretOffset(toffset),
	Mission(order),
	Explosion(exp),
	MaxSize(0)
{
	/*
	**	Forced unit overrides form the default.
	*/
	Speed = SPEED_WHEEL;
}


/***********************************************************************************************
 * UnitTypeClass::operator new -- Allocates an object from the unit type class heap.           *
 *                                                                                             *
 *    Use this routine to allocate a unit type class object from the special heap that is      *
 *    maintained for this purpose.                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the newly allocated unit type class object. If there is  *
 *          no more room to allocate another unit type class object, then NULL will be         *
 *          returned.                                                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * UnitTypeClass::operator new(size_t)
{
	return(UnitTypes.Alloc());
}


/***********************************************************************************************
 * UnitTypeClass::operator delete -- Return a unit type class object back to the pool.         *
 *                                                                                             *
 *    This will return a previously allocated unit to the memory pool from whence it came.     *
 *                                                                                             *
 * INPUT:   pointer  -- A Pointer to the unit type class object to return to the memory pool.  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitTypeClass::operator delete(void * pointer)
{
	UnitTypes.Free((UnitTypeClass *)pointer);
}


/***********************************************************************************************
 * UnitTypeClass::Init_Heap -- Initialize the unit type class heap.                            *
 *                                                                                             *
 *    This initializes the unit type class heap by pre-allocated all the known unit types.     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this once and call it before processing the rules.ini file.           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitTypeClass::Init_Heap(void)
{
	/*
	**	These unit type class objects must be allocated in the exact order that they
	**	are specified in the UnitType enumeration. This is necessary because the heap
	**	allocation block index serves double duty as the type number index.
	*/
	new UnitTypeClass(UnitHTank);			//	UNIT_HTANK
	new UnitTypeClass(UnitMTank);			//	UNIT_MTANK
	new UnitTypeClass(UnitMTank2);		//	UNIT_MTANK2
	new UnitTypeClass(UnitLTank);			//	UNIT_LTANK
	new UnitTypeClass(UnitAPC);			//	UNIT_APC
	new UnitTypeClass(UnitMineLayer);	// UNIT_MINELAYER
	new UnitTypeClass(UnitJeep);			//	UNIT_JEEP
	new UnitTypeClass(UnitHarvester);	//	UNIT_HARVESTER
	new UnitTypeClass(UnitArty);			//	UNIT_ARTY
	new UnitTypeClass(UnitMRJammer);		//	UNIT_MRJ
	new UnitTypeClass(UnitMGG);			//	UNIT_MGG
	new UnitTypeClass(UnitMCV);			// UNIT_MCV
	new UnitTypeClass(UnitV2Launcher);	//	UNIT_V2_LAUNCHER
	new UnitTypeClass(UnitConvoyTruck);	// UNIT_TRUCK
#ifdef FIXIT_ANTS
	new UnitTypeClass(UnitAnt1);			// UNIT_ANT1
	new UnitTypeClass(UnitAnt2);			// UNIT_ANT2
	new UnitTypeClass(UnitAnt3);			// UNIT_ANT3
#endif

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	new UnitTypeClass(UnitChrono);		// UNIT_CHRONOTANK
	new UnitTypeClass(UnitTesla);			// UNIT_TESLATANK
	new UnitTypeClass(UnitMAD);			// UNIT_MAD
	new UnitTypeClass(UnitDemoTruck);	// UNIT_DEMOTRUCK
#ifdef FIXIT_PHASETRANSPORT	//	checked - ajw 9/28/98
	new UnitTypeClass(UnitPhase);			//	UNIT_PHASETRANSPORT
#endif
#endif

}


/***********************************************************************************************
 * UnitTypeClass::From_Name -- Fetch class pointer from specified name.                        *
 *                                                                                             *
 *    This routine converts an ASCII representation of a unit class and                        *
 *    converts it into a real unit class number.                                               *
 *                                                                                             *
 * INPUT:   name  -- ASCII name representing a unit class.                                     *
 *                                                                                             *
 * OUTPUT:  Returns with the actual unit class number that the string                          *
 *          represents.                                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   05/02/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
UnitType UnitTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (UnitType classid = UNIT_FIRST; classid < UNIT_COUNT; classid++) {
			if (stricmp(As_Reference(classid).IniName, name) == 0) {
				return(classid);
			}
		}
	}
	return(UNIT_NONE);
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * UnitTypeClass::Display -- Displays a generic unit shape.                                    *
 *                                                                                             *
 *    This routine displays a generic representation of a unit of this                         *
 *    type. Typically, it is used when adding objects to the game map.                         *
 *                                                                                             *
 * INPUT:   x,y   -- Coordinate to render the unit shape.                                      *
 *                                                                                             *
 *          window-- Window to render within.                                                  *
 *                                                                                             *
 *          house -- House to render the unit colors.                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *   11/08/1994 JLB : Handles chunky type vehicles now.                                        *
 *=============================================================================================*/
void UnitTypeClass::Display(int x, int y, WindowNumberType window, HousesType ) const
{
	int shape = 0;
	void const * ptr = Get_Cameo_Data();
	if (ptr == NULL) {
		ptr = Get_Image_Data();
		shape = Rotation/6;
	}
	CC_Draw_Shape(ptr, shape, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL);
}


/***********************************************************************************************
 * UnitTypeClass::Prep_For_Add -- Prepares scenario editor to add unit.                        *
 *                                                                                             *
 *    This routine is used to prepare the generic object adder dialog                          *
 *    box so that it will be able to add a unit object.                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *   06/04/1994 JLB : Uses map editing interface functions.                                    *
 *=============================================================================================*/
void UnitTypeClass::Prep_For_Add(void)
{
	for (UnitType index = UNIT_FIRST; index < UNIT_COUNT; index++) {
		if (As_Reference(index).Get_Image_Data() != NULL) {
			Map.Add_To_List(&As_Reference(index));
		}
	}
}
#endif


/***********************************************************************************************
 * UnitTypeClass::One_Time -- Performs one time processing for unit type class objects.        *
 *                                                                                             *
 *    This routine is used to perform the action necessary only once for the unit type class.  *
 *    It loads unit shapes and brain files.   This routine should only be called once.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once.                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitTypeClass::One_Time(void)
{
	for (UnitType index = UNIT_FIRST; index < UNIT_COUNT; index++) {
		char	fullname[_MAX_FNAME+_MAX_EXT];
		char	buffer[_MAX_FNAME];
		UnitTypeClass const & uclass = As_Reference(index);
		CCFileClass	file;

		void const * ptr;		// Shape pointer and set pointer.

		int largest = 0;
//		if (uclass.Level != -1) {
//		if (uclass.IsBuildable) {

			/*
			**	Fetch the supporting data files for the unit.
			*/
			sprintf(buffer, "%sICON", uclass.Graphic_Name());
			_makepath(fullname, NULL, NULL, buffer, ".SHP");
			#ifndef NDEBUG
				RawFileClass datafile(fullname);
				if (datafile.Is_Available()) {
					((void const *&)uclass.CameoData) = Load_Alloc_Data(datafile);
				} else {
					((void const *&)uclass.CameoData) = MFCD::Retrieve(fullname);
				}
			#else
				((void const *&)uclass.CameoData) = MFCD::Retrieve(fullname);
			#endif
//		}

		/*
		**	Fetch a pointer to the unit's shape data.
		*/
		_makepath(fullname, NULL, NULL, uclass.Graphic_Name(), ".SHP");
		#ifndef NDEBUG
			RawFileClass shpfile(fullname);
			if (shpfile.Is_Available()) {
				ptr = Load_Alloc_Data(shpfile);
			} else {
				ptr = MFCD::Retrieve(fullname);
			}
		#else
			ptr = MFCD::Retrieve(fullname);
		#endif

		((void const *&)uclass.ImageData) = ptr;
		if (ptr != NULL) {

			largest = max(largest, (int)Get_Build_Frame_Width(ptr));
			largest = max(largest, (int)Get_Build_Frame_Height(ptr));
		}

		((int &)uclass.MaxSize) = max(largest, 8);
	}

	/*
	**	Load any custom shapes at this time.
	*/
	if (WakeShapes == NULL) {
		WakeShapes = MFCD::Retrieve("WAKE.SHP");
	}
	if (TurretShapes == NULL) {
		TurretShapes = MFCD::Retrieve("TURR.SHP");
	}
	if (SamShapes == NULL) {
		SamShapes = MFCD::Retrieve("SSAM.SHP");
	}
	if (MGunShapes == NULL) {
		MGunShapes = MFCD::Retrieve("MGUN.SHP");
	}
}


/***********************************************************************************************
 * UnitTypeClass::Create_And_Place -- Creates and places a unit object onto the map.           *
 *                                                                                             *
 *    This routine is used by the scenario editor to create and place a unit object of this    *
 *    type onto the map.                                                                       *
 *                                                                                             *
 * INPUT:   cell     -- The cell that the unit is to be placed into.                           *
 *                                                                                             *
 *          house    -- The house that the unit belongs to.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the unit created and placed successfully?                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitTypeClass::Create_And_Place(CELL cell, HousesType house) const
{
	UnitClass * unit = new UnitClass(Type, house);
	if (unit != NULL) {
		return(unit->Unlimbo(Cell_Coord(cell), Random_Pick(DIR_N, DIR_MAX)));
	}
	return(false);
}


/***********************************************************************************************
 * UnitTypeClass::Create_One_Of -- Creates a unit in limbo.                                    *
 *                                                                                             *
 *    This function creates a unit of this type and keeps it in limbo. A pointer to the        *
 *    created unit object is returned. It is presumed that this object will later be           *
 *    unlimboed at the correct time and place.                                                 *
 *                                                                                             *
 * INPUT:   house -- Pointer to the house that is to own the unit.                             *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the created unit object. If the unit object              *
 *          could not be created, then NULL is returned.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/07/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * UnitTypeClass::Create_One_Of(HouseClass * house) const
{
	return(new UnitClass(Type, house->Class->House));
}


/***********************************************************************************************
 * UnitTypeClass::As_Reference -- Fetches a reference to the unit type class specified.        *
 *                                                                                             *
 *    Use this routine to return a reference to the UnitTypeClass object as indicated by       *
 *    the unit type number specified.                                                          *
 *                                                                                             *
 * INPUT:   type  -- The unit type number to convert into a UnitTypeClass object reference.    *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the unit type class object specified.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
UnitTypeClass & UnitTypeClass::As_Reference(UnitType type)
{
	return(*UnitTypes.Ptr(type));
}


/***********************************************************************************************
 * UnitTypeClass::Dimensions -- Determines the unit's pixel dimensions.                        *
 *                                                                                             *
 *    This routine will fill in the width and height for this unit type. This width and        *
 *    height are used to render the selection rectangle and the positioning of the health      *
 *    bargraph.                                                                                *
 *                                                                                             *
 * INPUT:   width    -- Reference to the width of the unit (to be filled in).                  *
 *                                                                                             *
 *          height   -- Reference to the height of the unit (to be filled in).                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitTypeClass::Dimensions(int &width, int &height) const
{
	width = MaxSize-(MaxSize/4);
	width = min(width, 48);
	height = MaxSize-(MaxSize/4);
	height = min(height, 48);
}


/***********************************************************************************************
 * UnitTypeClass::Max_Pips -- Fetches the maximum pips allowed for this unit.                  *
 *                                                                                             *
 *    This routine will determine the number of pips (maximum) allowed for this unit type.     *
 *    Typically, this is the number of passengers allowed, but for harvesters, it is the       *
 *    number of credits it holds divided by 100.                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the maximum number of pips allowed for this unit type.                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/26/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitTypeClass::Max_Pips(void) const
{
	if (Type == UNIT_HARVESTER) {
		return(7);
	}

	if (Type == UNIT_MINELAYER) {
		return(MaxAmmo);
	}
	return(Max_Passengers());
}


/***********************************************************************************************
 * UnitTypeClass::Turret_Adjust -- Turret adjustment routine for MLRS and MSAM units.          *
 *                                                                                             *
 *    This routine adjusts the pixel coordinates specified to account for the displacement of  *
 *    the turret on the MLRS and MSAM vehicles.                                                *
 *                                                                                             *
 * INPUT:   dir   -- The direction of the body of the vehicle.                                 *
 *                                                                                             *
 *          x,y   -- References to the turret center pixel position. These will be modified as *
 *                   necessary.                                                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitTypeClass::Turret_Adjust(DirType dir, int &x, int &y) const
{
	static struct {
		signed char X,Y;
	} _adjust[32] = {
	  	{1,2},	// N
	  	{-1,1},
	  	{-2,0},
	  	{-3,0},
	  	{-3,1},	// NW
	  	{-4,-1},
	  	{-4,-1},
	  	{-5,-2},
	  	{-5,-3},	// W
	  	{-5,-3},
	  	{-3,-3},
	  	{-3,-4},
	  	{-3,-4},	// SW
	  	{-3,-5},
	  	{-2,-5},
	  	{-1,-5},
	  	{0,-5},	// S
	  	{1,-6},
	  	{2,-5},
	  	{3,-5},
	  	{4,-5},	// SE
	  	{6,-4},
	  	{6,-3},
	  	{6,-3},
	  	{6,-3},	// E
	  	{5,-1},
	  	{5,-1},
	  	{4,0},
	  	{3,0},	// NE
	  	{2,0},
	  	{2,1},
	  	{1,2}
	};

	int index = 0;
	switch (Type) {
		case UNIT_JEEP:
			y -= 4;
			break;

		case UNIT_MGG:
			index = Dir_To_32(dir);
			x += _adjust[index].X;
			y += _adjust[index].Y;
			break;

		default:
			break;
	}
}


/***********************************************************************************************
 * UnitTypeClass::Read_INI -- Fetch the unit type data from the INI database.                  *
 *                                                                                             *
 *    This routine will find the section in the INI database for this unit type object and     *
 *    then fill in the override values specified.                                              *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that will be examined.                      *
 *                                                                                             *
 * OUTPUT:  bool; Was the section for this unit found in the database and the data extracted?  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitTypeClass::Read_INI(CCINIClass & ini)
{
	if (TechnoTypeClass::Read_INI(ini)) {
		IsNoFireWhileMoving = ini.Get_Bool(IniName, "NoMovingFire", IsNoFireWhileMoving);
		Speed = ini.Get_Bool(IniName, "Tracked", (Speed == SPEED_TRACK)) ? SPEED_TRACK : SPEED_WHEEL;

		/*
		**	If this unit can drive over walls, then mark it as recognizing the crusher zone.
		*/
		if (MZone < MZONE_CRUSHER && IsCrusher) {
			MZone = MZONE_CRUSHER;
		}
		return(true);
	}
	return(false);
}
