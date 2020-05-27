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

/* $Header:   F:\projects\c&c\vcs\code\udata.cpv   2.17   16 Oct 1995 16:50:42   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : June 26, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   UnitTypeClass::As_Reference -- Fetches a reference to the unit type class specified.      *
 *   UnitTypeClass::Create_And_Place -- Creates and places a unit object onto the map.         *
 *   UnitTypeClass::Create_One_Of -- Creates a unit in limbo.                                  *
 *   UnitTypeClass::Dimensions -- Determines the unit's pixel dimensions.                      *
 *   UnitTypeClass::Display -- Displays a generic unit shape.                                  *
 *   UnitTypeClass::From_Name -- Fetch class pointer from specified name.                      *
 *   UnitTypeClass::Max_Pips -- Fetches the maximum pips allowed for this unit.                *
 *   UnitTypeClass::Occupy_List -- Returns with unit occupation list.                          *
 *   UnitTypeClass::One_Time -- Performs one time processing for unit type class objects.      *
 *   UnitTypeClass::Prep_For_Add -- Prepares scenario editor to add unit.                      *
 *   UnitTypeClass::UnitTypeClass -- Constructor for unit types.                               *
 *   UnitTypeClass::Who_Can_Build_Me -- Determines which factory can build this unit type.     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

void const * UnitTypeClass::WakeShapes = 0;

// Visceroid
static UnitTypeClass const UnitVisceroid(
	UNIT_VICE,
	TXT_VISCEROID,							// NAME:			Text name of this unit type.
	"VICE",									// NAME:			Text name of this unit type.
	ANIM_NAPALM2,							// EXPLOSION:	Type of explosion when destroyed.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		false,			// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		true,				// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		true,				// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		false,			// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		false,			// Does it lay tracks while moving?
		false,			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		true,				// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	150,										// STRENGTH:	Strength (in damage points).
	4,											// SIGHTRANGE:	Range of sighting.
	800,										// COST:			Cost to build (Credits).
	1,											// SCENARIO:	Starting availability scenario.
	80,20,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_GOOD|
	HOUSEF_BAD|
	HOUSEF_NEUTRAL|
	HOUSEF_JP,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_CHEMSPRAY,WEAPON_NONE,
	ARMOR_WOOD,								// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_MEDIUM,								// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Flame tank
static UnitTypeClass const UnitFTank(
	UNIT_FTANK,
	TXT_FTANK,								// NAME:			Text name of this unit type.
	"FTNK",									// NAME:			Text name of this unit type.
	ANIM_NAPALM3,							// EXPLOSION:	Type of explosion when destroyed.
	4,							// Build level.
	STRUCTF_RADAR,			// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		true,				// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		false,			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	300,										// STRENGTH:	Strength (in damage points).
	4,											// SIGHTRANGE:	Range of sighting.
	800,										// COST:			Cost to build (Credits).
	9,											// SCENARIO:	Starting availability scenario.
	80,66,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_FLAME_TONGUE,WEAPON_NONE,
	ARMOR_STEEL,							// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_MEDIUM,								// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Stealth tank
static UnitTypeClass const UnitSTank(
	UNIT_STANK,
	TXT_STANK,								// NAME:			Text name of this unit type.
	"STNK",									// NAME:			Text name of this unit type.
	ANIM_FRAG2,							// EXPLOSION:	Type of explosion when destroyed.
	5,								// Build level.
	STRUCTF_RADAR,				// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		true,				// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		true,				// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		false,			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		true,				// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	110,										// STRENGTH:	Strength (in damage points).
	4,											// SIGHTRANGE:	Range of sighting.
	900,										// COST:			Cost to build (Credits).
	12,										// SCENARIO:	Starting availability scenario.
	80,81,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_DRAGON,WEAPON_NONE,
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_MEDIUM_FAST,						// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Light tank
static UnitTypeClass const UnitLTank(
	UNIT_LTANK,
	TXT_LTANK,								// NAME:			Text name of this unit type.
	"LTNK",									// NAME:			Text name of this unit type.
	ANIM_FRAG1,							// EXPLOSION:	Type of explosion when destroyed.
	3,								// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		false,			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	300,										// STRENGTH:	Strength (in damage points).
	3,											// SIGHTRANGE:	Range of sighting.
	600,										// COST:			Cost to build (Credits).
	5,											// SCENARIO:	Starting availability scenario.
	80,56,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_75MM,WEAPON_NONE,
	ARMOR_STEEL,							// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_MEDIUM,								// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Medium tank
static UnitTypeClass const UnitMTank(
	UNIT_MTANK,
	TXT_MTANK,								// NAME:			Text name of this unit type.
	"MTNK",									// NAME:			Text name of this unit type.
	ANIM_FRAG2,							// EXPLOSION:	Type of explosion when destroyed.
	3,								// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		true,				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	400,										// STRENGTH:	Strength (in damage points).
	3,											// SIGHTRANGE:	Range of sighting.
	800,										// COST:			Cost to build (Credits).
	7,											// SCENARIO:	Starting availability scenario.
	80,62,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD,							// OWNABLE:		Ownable by house (bit field).
	WEAPON_105MM,WEAPON_NONE,
	ARMOR_STEEL,							// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_MEDIUM,								// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Mastadon tank
static UnitTypeClass const UnitHTank(
	UNIT_HTANK,
	TXT_HTANK,								// NAME:			Text name of this unit type.
	"HTNK",									// NAME:			Text name of this unit type.
	ANIM_ART_EXP1,							// EXPLOSION:	Type of explosion when destroyed.
	5,								// Build level.
	STRUCTF_REPAIR,			// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		true,				// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		true,				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	600,										// STRENGTH:	Strength (in damage points).
	4,											// SIGHTRANGE:	Range of sighting.
	1500,										// COST:			Cost to build (Credits).
	13,										// SCENARIO:	Starting availability scenario.
	80,80, 									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD,							// OWNABLE:		Ownable by house (bit field).
	WEAPON_120MM,WEAPON_MAMMOTH_TUSK,
	ARMOR_STEEL,							// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_MEDIUM_SLOW,						// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Mobile HQ
static UnitTypeClass const UnitMHQ(
	UNIT_MHQ,
	TXT_MHQ,									// NAME:			Text name of this unit type.
	"MHQ",									// NAME:			Text name of this unit type.
	ANIM_FRAG2,							// EXPLOSION:	Type of explosion when destroyed.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		false,			// Can this be a goodie surprise from a crate?
		false,			// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		true,				// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		false,			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	110,										// STRENGTH:	Strength (in damage points).
	5,											// SIGHTRANGE:	Range of sighting.
	600,										// COST:			Cost to build (Credits).
	99,										// SCENARIO:	Starting availability scenario.
	80,100,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_NONE,WEAPON_NONE,
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_MEDIUM,								// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Landing craft
static UnitTypeClass const UnitHover(
	UNIT_HOVER,
	TXT_HOVER,								// NAME:			Text name of this unit type.
	"LST",									// NAME:			Text name of this unit type.
	ANIM_FBALL1,							// EXPLOSION:	Type of explosion when destroyed.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		false,			// Can this be a goodie surprise from a crate?
		false,			// Is a leader type?
		false,			// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		true,				//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		false,			// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
//		true,			// Is selectable by player?
		false,			// Is selectable by player?
		false,			// Can it be a target for attack or move?
		true,				// Is it insignificant (won't be announced)?
		true,				// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		false,			// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		false,			// Does it lay tracks while moving?
		true,				// Is this a gigundo-rotund-enormous unit?
		true,				// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	400,										// STRENGTH:	Strength (in damage points).
	3,											// SIGHTRANGE:	Range of sighting.
	300,										// COST:			Cost to build (Credits).
	99,										// SCENARIO:	Starting availability scenario.
	80,40,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_NONE,WEAPON_NONE,
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	SPEED_HOVER,							// MOVE:			Locomotion type.
	MPH_MEDIUM_FAST,						// SPEED:		Miles per hour.
	127,										// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Mobile sam launcher
static UnitTypeClass const UnitSAM(
	UNIT_MSAM,
	TXT_MSAM,								// NAME:			Text name of this unit type.
	"MLRS",									// NAME:			Text name of this unit type.
	ANIM_FRAG2,							// EXPLOSION:	Type of explosion when destroyed.
	7,								// Build level.
	STRUCTF_ATOWER,			// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		false,			// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		true,				// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		true,				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	2,											// AMMO:			Number of shots it has (default).
	120,										// STRENGTH:	Strength (in damage points).
	4,											// SIGHTRANGE:	Range of sighting.
	750,										// COST:			Cost to build (Credits).
	98,										// SCENARIO:	Starting availability scenario.
	80,30,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
//	HOUSEF_GOOD|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_HONEST_JOHN,WEAPON_NONE,
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_MEDIUM,								// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Artillery
static UnitTypeClass const UnitArty(
	UNIT_ARTY,
	TXT_ARTY,								// NAME:			Text name of this unit type.
	"ARTY",									// NAME:			Text name of this unit type.
	ANIM_ART_EXP1,							// EXPLOSION:	Type of explosion when destroyed.
	6,								// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		false,			// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		false,			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	75,										// STRENGTH:	Strength (in damage points).
	4,											// SIGHTRANGE:	Range of sighting.
	450,										// COST:			Cost to build (Credits).
	9,											// SCENARIO:	Starting availability scenario.
	80,73,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_155MM,WEAPON_NONE,
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_MEDIUM_SLOW,						// SPEED:		Miles per hour.
	2,											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Harvester
static UnitTypeClass const UnitHarvester(
	UNIT_HARVESTER,
	TXT_HARVESTER,							// NAME:			Text name of this unit type.
	"HARV",									// NAME:			Text name of this unit type.
	ANIM_FBALL1,							// EXPLOSION:	Type of explosion when destroyed.
	2,								// Build level.
	STRUCTF_REFINERY,			// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		false,			// Is a leader type?
		false,			// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		true,				// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		true,				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	600,										// STRENGTH:	Strength (in damage points).
//	300,										// STRENGTH:	Strength (in damage points).
	2,											// SIGHTRANGE:	Range of sighting.
	1400,										// COST:			Cost to build (Credits).
	7,											// SCENARIO:	Starting availability scenario.
	80,85,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_NONE,WEAPON_NONE,
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	SPEED_WHEEL,							// MOVE:			Locomotion type.
	MPH_MEDIUM_SLOW,						// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HARVEST						// ORDERS:		Default order to give new unit.
);

// Mobile construction vehicle
static UnitTypeClass const UnitMCV(
	UNIT_MCV,
	TXT_MCV,									// NAME:			Text name of this unit type.
	"MCV",									// NAME:			Text name of this unit type.
	ANIM_FBALL1,							// EXPLOSION:	Type of explosion when destroyed.
	7,							// Build level.
	STRUCTF_EYE,			// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		false,			// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		true,				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	600,										// STRENGTH:	Strength (in damage points).
	2,											// SIGHTRANGE:	Range of sighting.
	5000,										// COST:			Cost to build (Credits).
	15,										// SCENARIO:	Starting availability scenario.
	80,86,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_NONE,WEAPON_NONE,
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	SPEED_WHEEL,							// MOVE:			Locomotion type.
	MPH_MEDIUM_SLOW,						// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Jeep (hummer)
static UnitTypeClass const UnitJeep(
	UNIT_JEEP,
	TXT_JEEP,								// NAME:			Text name of this unit type.
	"JEEP",									// NAME:			Text name of this unit type.
	ANIM_FRAG1,							// EXPLOSION:	Type of explosion when destroyed.
	2,								// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		false,			// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		false,			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	150,										// STRENGTH:	Strength (in damage points).
	2,											// SIGHTRANGE:	Range of sighting.
	400,										// COST:			Cost to build (Credits).
	5,											// SCENARIO:	Starting availability scenario.
	80,41,	 								// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD,							// OWNABLE:		Ownable by house (bit field).
	WEAPON_M60MG,WEAPON_NONE,
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	SPEED_WHEEL,							// MOVE:			Locomotion type.
	MPH_MEDIUM_FAST,						// SPEED:		Miles per hour.
	10,										// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Buggy
static UnitTypeClass const UnitBuggy(
	UNIT_BUGGY,
	TXT_DUNE_BUGGY,						// NAME:			Text name of this unit type.
	"BGGY",									// NAME:			Text name of this unit type.
	ANIM_FRAG1,							// EXPLOSION:	Type of explosion when destroyed.
	4,								// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		false,			// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		false,			// Does it lay tracks while moving?
		false,			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	140,										// STRENGTH:	Strength (in damage points).
	2,											// SIGHTRANGE:	Range of sighting.
	300,										// COST:			Cost to build (Credits).
	5,											// SCENARIO:	Starting availability scenario.
	80,42, 									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_M60MG,WEAPON_NONE,
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	SPEED_WHEEL,							// MOVE:			Locomotion type.
	MPH_MEDIUM_FAST,						// SPEED:		Miles per hour.
	10,										// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Attack cycle
static UnitTypeClass const UnitBike(
	UNIT_BIKE,
	TXT_BIKE,								// NAME:			Text name of this unit type.
	"BIKE",									// NAME:			Text name of this unit type.
	ANIM_FRAG1,							// EXPLOSION:	Type of explosion when destroyed.
	2,								// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false/*true*/,	// Can it be crushed by a heavy vehicle?
		false,			// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		false,			// Does it lay tracks while moving?
		false,			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
#ifdef ADVANCED
	90,										// STRENGTH:	Strength (in damage points).
#else
	160,										// STRENGTH:	Strength (in damage points).
#endif
	2,											// SIGHTRANGE:	Range of sighting.
	500,										// COST:			Cost to build (Credits).
	5,											// SCENARIO:	Starting availability scenario.
	80,45, 									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_DRAGON,WEAPON_NONE,
	ARMOR_WOOD,								// ARMOR:		Armor type
	SPEED_WHEEL,							// MOVE:			Locomotion type.
	MPH_FAST,								// SPEED:		Miles per hour.
	10,										// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Rocket launcher
static UnitTypeClass const UnitMLRS(
	UNIT_MLRS,
	TXT_MLRS,								// NAME:			Text name of this unit type.
	"MSAM",									// NAME:			Text name of this unit type.
	ANIM_ART_EXP1,							// EXPLOSION:	Type of explosion when destroyed.
	7,								// Build level.
	STRUCTF_EYE,				// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		false,			// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		true,				// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		true,				// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		false,			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	100,										// STRENGTH:	Strength (in damage points).
	4,											// SIGHTRANGE:	Range of sighting.
	800,										// COST:			Cost to build (Credits).
	11,										// SCENARIO:	Starting availability scenario.
	80,72, 									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_BAD|
	HOUSEF_GOOD,							// OWNABLE:		Ownable by house (bit field).
	WEAPON_MLRS,WEAPON_NONE,
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_MEDIUM,								// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_GUARD							// ORDERS:		Default order to give new unit.
);

// Armored personnel carrier
static UnitTypeClass const UnitAPC(
	UNIT_APC,
	TXT_APC,									// NAME:			Text name of this unit type.
	"APC",									// NAME:			Text name of this unit type.
	ANIM_FRAG2,							// EXPLOSION:	Type of explosion when destroyed.
	4,								// Build level.
	STRUCTF_BARRACKS,			// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		true,				//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		false,			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	200,										// STRENGTH:	Strength (in damage points).
	4,											// SIGHTRANGE:	Range of sighting.
	700,										// COST:			Cost to build (Credits).
	5,											// SCENARIO:	Starting availability scenario.
	80,15,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_BAD|
	HOUSEF_JP|
	HOUSEF_GOOD,							// OWNABLE:		Ownable by house (bit field).
	WEAPON_M60MG,WEAPON_NONE,
	ARMOR_STEEL,							// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_MEDIUM_FASTER,					// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Gunboat
static UnitTypeClass const UnitGunBoat(
	UNIT_GUNBOAT,
	TXT_GUNBOAT,							// NAME:			Text name of this unit type.
	"BOAT",									// NAME:			Text name of this unit type.
	ANIM_FBALL1,							// EXPLOSION:	Type of explosion when destroyed.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		false,			// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		false,			// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		true,				// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		true,				// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		false,			// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		false,			// Does it lay tracks while moving?
		true,				// Is this a gigundo-rotund-enormous unit?
		true,				// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	700,										// STRENGTH:	Strength (in damage points).
	5,											// SIGHTRANGE:	Range of sighting.
	300,										// COST:			Cost to build (Credits).
	99,										// SCENARIO:	Starting availability scenario.
	80,40,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_TOMAHAWK,WEAPON_NONE,
	ARMOR_STEEL,							// ARMOR:		Armor type
	SPEED_FLOAT,							// MOVE:			Locomotion type.
	MPH_SLOW,								// SPEED:		Miles per hour.
	1,											// ROT:			Rate of turn (degrees per tick).
	14,										// Turret center offset along body centerline.
	MISSION_GUARD							// ORDERS:		Default order to give new unit.
);

// Triceratops
static UnitTypeClass const UnitTric(
	UNIT_TRIC,
	TXT_TRIC,								// NAME:			Text name of this unit type.
	"TRIC",									// NAME:			Text name of this unit type.
	ANIM_TRIC_DIE,							// EXPLOSION:	Type of explosion when destroyed.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		false,			// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		true,				// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		true,				// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		false,			// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		true,				// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		false,			// Does it lay tracks while moving?
		true,				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	700,										// STRENGTH:	Strength (in damage points).
	5,											// SIGHTRANGE:	Range of sighting.
	0,											// COST:			Cost to build (Credits).
	99,										// SCENARIO:	Starting availability scenario.
	50,50,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_JP,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_STEG,WEAPON_NONE,
	ARMOR_STEEL,							// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_SLOW,								// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_GUARD							// ORDERS:		Default order to give new unit.
);

// Tyrannosaurus Rex
static UnitTypeClass const UnitTrex(
	UNIT_TREX,
	TXT_TREX,								// NAME:			Text name of this unit type.
	"TREX",									// NAME:			Text name of this unit type.
	ANIM_TREX_DIE,							// EXPLOSION:	Type of explosion when destroyed.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		false,			// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		true,				// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		true,				// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		false,			// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		true,				// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		false,			// Does it lay tracks while moving?
		true,				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	750,										// STRENGTH:	Strength (in damage points).
	5,											// SIGHTRANGE:	Range of sighting.
	0,											// COST:			Cost to build (Credits).
	99,										// SCENARIO:	Starting availability scenario.
	50,50,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_JP,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_TREX,WEAPON_NONE,
	ARMOR_STEEL,							// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_MEDIUM,								// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_GUARD							// ORDERS:		Default order to give new unit.
);

// Velociraptor
static UnitTypeClass const UnitRapt(
	UNIT_RAPT,
	TXT_RAPT,								// NAME:			Text name of this unit type.
	"RAPT",									// NAME:			Text name of this unit type.
	ANIM_RAPT_DIE,							// EXPLOSION:	Type of explosion when destroyed.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		false,			// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		true,				// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false/*true*/,	// Can it be crushed by a heavy vehicle?
		false,			// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		true,				// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		false,			// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		true,				// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		false,			// Does it lay tracks while moving?
		true,				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	180,										// STRENGTH:	Strength (in damage points).
	5,											// SIGHTRANGE:	Range of sighting.
	0,											// COST:			Cost to build (Credits).
	99,										// SCENARIO:	Starting availability scenario.
	50,50,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_JP,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_TREX,WEAPON_NONE,
	ARMOR_STEEL,							// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_FAST,								// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_GUARD							// ORDERS:		Default order to give new unit.
);

// Stegosaurus
static UnitTypeClass const UnitSteg(
	UNIT_STEG,
	TXT_STEG,								// NAME:			Text name of this unit type.
	"STEG",									// NAME:			Text name of this unit type.
	ANIM_STEG_DIE,							// EXPLOSION:	Type of explosion when destroyed.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
		false,			// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		true,				// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		true,				// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		false,			// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		true,				// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		false,			// Does it lay tracks while moving?
		true,				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	600,										// STRENGTH:	Strength (in damage points).
	5,											// SIGHTRANGE:	Range of sighting.
	0,											// COST:			Cost to build (Credits).
	99,										// SCENARIO:	Starting availability scenario.
	50,50,									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_JP,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_STEG,WEAPON_NONE,
	ARMOR_STEEL,							// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_SLOW,								// SPEED:		Miles per hour.
	5,											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_GUARD							// ORDERS:		Default order to give new unit.
);


#ifdef PETROGLYPH_EXAMPLE_MOD

// Nuke tank
static UnitTypeClass const UnitNukeTank(
	UNIT_NUKE_TANK,
	TXT_HTANK,								// NAME:			Text name of this unit type.
	"NTNK",									// NAME:			Text name of this unit type.
	ANIM_ART_EXP1,							// EXPLOSION:	Type of explosion when destroyed.
	7,								// Build level.
	STRUCTF_TEMPLE,			// Building prerequisite.
		true,				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		false,			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		false,			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		true,				// Can this unit squash infantry?
		false,			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		true,				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		false,			// Does it have a rotating radar dish?
		false,			// Is there an associated firing animation?
		false,			// Must the turret be in a locked down position while moving?
		true,				// Does it lay tracks while moving?
		true,				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		false,			// Is the unit capable of cloaking?
		false,			// Does the unit have a constant animation?
	-1,										// AMMO:			Number of shots it has (default).
	600,										// STRENGTH:	Strength (in damage points).
	6,											// SIGHTRANGE:	Range of sighting.
	1500,										// COST:			Cost to build (Credits).
	10,										// SCENARIO:	Starting availability scenario.
	80,80, 									// RISK/RWRD:	Risk/reward rating values.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_BAD,								// OWNABLE:		Ownable by house (bit field).
	WEAPON_NUKE_LOB,WEAPON_NONE,
	ARMOR_STEEL,							// ARMOR:		Armor type
	SPEED_TRACK,							// MOVE:			Locomotion type.
	MPH_KINDA_SLOW,						// SPEED:		Miles per hour.
	3,											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

#endif PETROGLYPH_EXAMPLE_MOD

/*
**	This is the array of pointers to the static data associated with each
**	vehicle type.
*/
UnitTypeClass const * const UnitTypeClass::Pointers[UNIT_COUNT] = {
	&UnitHTank,			//	UNIT_HTANK
	&UnitMTank,			//	UNIT_MTANK
	&UnitLTank,			//	UNIT_LTANK
	&UnitSTank,			//	UNIT_STANK
	&UnitFTank,			//	UNIT_FTANK
	&UnitVisceroid,	// UNIT_VICE
	&UnitAPC,			//	UNIT_APC
	&UnitMLRS,			//	UNIT_MLRS
	&UnitJeep,			//	UNIT_JEEP
	&UnitBuggy,			//	UNIT_BUGGY
	&UnitHarvester,	//	UNIT_HARVESTER
	&UnitArty,			//	UNIT_ARTY
	&UnitSAM,			//	UNIT_MSAM
	&UnitHover,			//	UNIT_HOVER
	&UnitMHQ,			//	UNIT_MHQ
	&UnitGunBoat,		//	UNIT_GUNBOAT
	&UnitMCV,			// UNIT_MCV
	&UnitBike,			// UNIT_BIKE
	&UnitTric,			// UNIT_TRIC
	&UnitTrex,			// UNIT_TREX
	&UnitRapt,			// UNIT_RAPT
	&UnitSteg,			// UNIT_STEG
#ifdef PETROGLYPH_EXAMPLE_MOD
	&UnitNukeTank,		// UNIT_NUKE_TANK
#endif //PETROGLYPH_EXAMPLE_MOD
			
};


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
UnitTypeClass::UnitTypeClass(UnitType type,
				int name,
				char const *ininame,
				AnimType exp,
				unsigned char level,
				long pre,
				bool is_goodie,
				bool is_leader,
				bool is_eight,
				bool is_nominal,
				bool is_transporter,
				bool is_crushable,
				bool is_crusher,
				bool is_harvest,
				bool is_stealthy,
				bool is_selectable,
				bool is_legal_target,
				bool is_insignificant,
				bool is_immune,
				bool is_turret_equipped,
				bool is_twoshooter,
				bool is_repairable,
				bool is_buildable,
				bool is_crew,
				bool is_radar_equipped,
				bool is_fire_anim,
				bool is_lock_turret,
				bool is_tracked,
				bool is_gigundo,
				bool is_chunky,
				bool is_cloakable,
				bool is_animating,
				int ammo,
				unsigned short strength,
				int sightrange,
				int cost,
				int scenario,
				int risk,
				int reward,
				int ownable,
				WeaponType primary, WeaponType secondary,
				ArmorType armor,
				SpeedType speed,
				MPHType maxSpeed,
				unsigned rot,
				int toffset,
				MissionType order) :
					TechnoTypeClass(name,
										ininame,
										level,
										pre,
										is_leader,
										false,
										is_nominal,
										is_transporter,
										false,
										is_crushable,
										is_stealthy,
										is_selectable,
										is_legal_target,
										is_insignificant,
										is_immune,
										false,
										is_twoshooter,
										is_turret_equipped,
										is_repairable,
										is_buildable,
										is_crew,
										ammo,
										strength,
										maxSpeed,
										sightrange,
										cost,
										scenario,
										risk,
										reward,
										ownable,
										primary,secondary,
										armor)
{
	Explosion = exp;
	IsCrateGoodie = is_goodie;
	IsPieceOfEight = is_eight;
	IsCloakable = is_cloakable;
	IsChunkyShape = is_chunky;
	IsCrusher = is_crusher;
	IsFireAnim = is_fire_anim;
	IsGigundo = is_gigundo;
	IsLockTurret = is_lock_turret;
	IsRadarEquipped = is_radar_equipped;
	IsToHarvest = is_harvest;
	IsTracked = is_tracked;
	IsAnimating = is_animating;
	Mission = order;
	ROT = rot;
	Speed = speed;
	TurretOffset = toffset;
	Type = type;
}


/***********************************************************************************************
 * UnitTypeClass::Occupy_List -- Returns with unit occupation list.                            *
 *                                                                                             *
 *    This routine returns with an occupation list for the unit type.                          *
 *    The unit occupation list is used for placing units.                                      *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the unit occupation list.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * UnitTypeClass::Occupy_List(bool ) const
{
	static short const _simple[] = {0, REFRESH_EOL};
	static short const _gun[] = {0, -1, 1, REFRESH_EOL};

	if (Type == UNIT_GUNBOAT) {
		return(&_gun[0]);
	}
	return(&_simple[0]);
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
UnitType UnitTypeClass::From_Name(char const *name)
{
	if (name) {
		for (UnitType classid = UNIT_FIRST; classid < UNIT_COUNT; classid++) {
			if (stricmp(Pointers[classid]->IniName, name) == 0) {
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
void UnitTypeClass::Display(int x, int y, WindowNumberType window, HousesType house) const
{
	int shape = 0;
	void const * ptr = Get_Cameo_Data();
	if (!ptr) {
		ptr = Get_Image_Data();
		shape = IsChunkyShape ? 0 : 5;
	}
	CC_Draw_Shape(ptr, shape, x, y, window, SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL, HouseTypeClass::As_Reference(house).RemapTable);
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
		int			largest;			// Largest dimension of shape (so far).

		void const 	*ptr = NULL;		// Shape pointer and set pointer.

		largest = 0;
		if (uclass.IsBuildable) {

			/*
			**	Fetch the supporting data files for the unit.
			*/
			if ( Get_Resolution_Factor() ) {
				sprintf(buffer, "%sICNH", uclass.IniName);
			} else {
				sprintf(buffer, "%sICON", uclass.IniName);
			}
			_makepath(fullname, NULL, NULL, buffer, ".SHP");
			((void const *&)uclass.CameoData) = MixFileClass::Retrieve(fullname);
		}

		/*
		**	Fetch a pointer to the unit's shape data.
		*/
		// Assume funpark mode might be required. ST - 10/14/2019 11:53AM
		//if (!uclass.IsPieceOfEight || (Special.IsJurassic && AreThingiesEnabled) ) {
			_makepath(fullname, NULL, NULL, uclass.IniName, ".SHP");
			ptr = MixFileClass::Retrieve(fullname);
		//} else {
		//	ptr = NULL;
		//}

#ifdef PETROGLYPH_EXAMPLE_MOD
		/*
		** Need some kind of shape data for our modded unit
		*/
		if (index == UNIT_NUKE_TANK && ptr == NULL) {
			_makepath(fullname, NULL, NULL, "HTNK", ".SHP");
			ptr = MixFileClass::Retrieve(fullname);
		}
#endif //PETROGLYPH_EXAMPLE_MOD

		((void const *&)uclass.ImageData) = ptr;
		if (ptr) {

			if (index == UNIT_MLRS || index == UNIT_MSAM) {
				largest = 26;
			} else {
				largest = MAX(largest, (int)Get_Build_Frame_Width(ptr));
				largest = MAX(largest, (int)Get_Build_Frame_Height(ptr));
			}
		}

		((int &)uclass.MaxSize) = MAX(largest, 8);
	}

	/*
	**	Load the wake shapes in at this time.
	*/
	if (!WakeShapes) {
		WakeShapes = MixFileClass::Retrieve("WAKE.SHP");
	}
}



/***********************************************************************************************
 * UTC::Init -- fetches the sidebar icons for the unittypeclass objects                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    theater type                                                                      *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    4/26/96 4:07PM ST : Created                                                              *
 *=============================================================================================*/

void UnitTypeClass::Init(TheaterType theater)
{

	if (Get_Resolution_Factor()){

		if (theater != LastTheater){

			void const * cameo_ptr;
			char	fullname[_MAX_FNAME+_MAX_EXT];
			char	buffer[_MAX_FNAME];

			for (UnitType index = UNIT_FIRST; index < UNIT_COUNT; index++) {

				UnitTypeClass const & uclass = As_Reference(index);

				((void const *&)uclass.CameoData) = NULL;

				if (uclass.IsBuildable) {
					sprintf(buffer, "%sICNH", uclass.IniName);
					_makepath(fullname, NULL, NULL, buffer, Theaters[theater].Suffix);
					cameo_ptr = MixFileClass::Retrieve(fullname);
					if (cameo_ptr){
						((void const *&)uclass.CameoData) = cameo_ptr;
					}
				}
			}
		}
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
	if (unit) {
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
 * UnitTypeClass::Who_Can_Build_Me -- Determines which factory can build this unit type.       *
 *                                                                                             *
 *    Use this routine to examine the buildings on the map in order to determine which one     *
 *    can build the unit type.                                                                 *
 *                                                                                             *
 * INPUT:   intheory -- If this parameter is true, then no examination of whether the factory  *
 *                      is currently busy is performed. It just checks to see if the unit      *
 *                      could be produced "in theory".                                         *
 *                                                                                             *
 *          legal    -- Should building prerequisite legality checks be performed as well?     *
 *                      For building placements, this is usually false. For sidebar button     *
 *                      adding, this is usually true.                                          *
 *                                                                                             *
 *          house    -- The owner of the unit to be produced.                                  *
 *                                                                                             *
 * OUTPUT:  Returns with pointer to the factory that can produce the unit. If no suitable      *
 *          factory could be found then NULL is returned.                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingClass * UnitTypeClass::Who_Can_Build_Me(bool intheory, bool legal, HousesType house) const
{
	BuildingClass * anybuilding = NULL;

	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass * building = Buildings.Ptr(index);

		if (building &&
				!building->IsInLimbo &&
				building->House->Class->House == house &&
				building->Class->ToBuild == RTTI_UNITTYPE &&
				building->Mission != MISSION_DECONSTRUCTION &&
				((1L << building->ActLike) & Ownable) &&
				(!legal || building->House->Can_Build(Type, building->ActLike)) &&
				(intheory || !building->In_Radio_Contact())) {

			if (building->IsLeader) return(building);
			anybuilding = building;
		}
	}
	return(anybuilding);
}


/***********************************************************************************************
 * UnitTypeClass::As_Reference -- Fetches a reference to the unit type class specified.        *
 *                                                                                             *
 *    Use this routine to return a reference to the UnitTypeClass object as indicated by       *
 *    the unit type number speicified.                                                         *
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
UnitTypeClass const & UnitTypeClass::As_Reference(UnitType type)
{
	return(*Pointers[type]);
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
	if (Type == UNIT_GUNBOAT) {
		width = 46;
		height = 18;
	} else {
		width = MaxSize-(MaxSize/4);
		height = MaxSize-(MaxSize/4);
	}
}

/***********************************************************************************************
 * UnitTypeClass::Repair_Cost -- Determines cost per "step" of repair.                         *
 *                                                                                             *
 *    Use this routine to determine how much it will cost to repair the unit one               *
 *    step. A step is defined as the number of hit points returned from the Repair_Step()      *
 *    function.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the credit cost to repair this unit one step.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/03/1995 BWG : Created.                                                                 *
 *=============================================================================================*/
int UnitTypeClass::Repair_Cost(void) const
{
	return(Fixed_To_Cardinal(Cost/(MaxStrength/REPAIR_STEP), REPAIR_PERCENT));
}


/***********************************************************************************************
 * UnitTypeClass::Repair_Step -- Determines the repair step rate.                              *
 *                                                                                             *
 *    This routine will determine how many strength points get healed for each "step". The     *
 *    cost to repair one step is determine from the Repair_Cost() function.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of health points repaired for each "step".                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/03/1995 BWG : Created.                                                                 *
 *=============================================================================================*/
int UnitTypeClass::Repair_Step(void) const
{
	return(REPAIR_STEP);
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
		return(FULL_LOAD_CREDITS/100);
	}

	if (IsTransporter) {
		return(Max_Passengers());
	}
	return(0);
}