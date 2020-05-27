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

/* $Header:   F:\projects\c&c\vcs\code\idata.cpv   2.12   02 Aug 1995 17:00:30   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : IDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : August 15, 1994                                              *
 *                                                                                             *
 *                  Last Update : June 29, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   InfantryTypeClass::Create_And_Place -- Creates and places infantry object onto the map.   *
 *   InfantryTypeClass::Create_One_Of -- Creates an infantry object.                           *
 *   InfantryTypeClass::Display -- Displays a generic infantry object.                         *
 *   InfantryTypeClass::From_Name -- Converts an ASCII name into an infantry type number.      *
 *   InfantryTypeClass::Full_Name -- Fetches the full name text number.                        *
 *   InfantryTypeClass::Get_Cameo_Data -- Fetches the small cameo shape for sidebar strip.     *
 *   InfantryTypeClass::InfantryTypeClass -- Constructor for infantry type class objects.      *
 *   InfantryTypeClass::Occupy_List -- Returns with default infantry occupation list.          *
 *   InfantryTypeClass::One_Time -- Performs any one time processing for infantry system.      *
 *   InfantryTypeClass::Prep_For_Add -- Prepares the scenario editor for adding of infantry object.*
 *   InfantryTypeClass::Who_Can_Build_Me -- Determines what can build the infantry object.     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"type.h"



/*
 * There were too many parameters for the InfantryTypeClass constructor so I have
 * created a table of Do Controls for each unit type and I am passing a pointer
 * to the table to the constructor instead of passing each value as it was before.
 *
 * If this offends anyones C++ sensibilities then please feel free to implement a
 * more elegant oop solution.
 *
 * Steve T. 10/3/95 10:13AM
 *
 */

// Minigunners

int MiniGunnerDos [DO_COUNT][3]={
	0,		1,	1,					// DO_STAND_READY
	8,		1,	1,					// DO_STAND_GUARD
	192,	1,	8,					// DO_PRONE
	16,	6,	6,					// DO_WALK
	64,	8,	8,					// DO_FIRE_WEAPON
	128,	2,	2,					// DO_LIE_DOWN
	144,	4,	4,					// DO_CRAWL
	176,	2,	2,					// DO_GET_UP
	192,	6,	8,					// DO_FIRE_PRONE
	256,	16,0,					// DO_IDLE1
	272,	16,0,					// DO_IDLE2
	288,	13,0,					// DO_ON_GUARD
	292,	10,0,					// DO_FIGHT_READY
	301,	2,	0,					// DO_PUNCH
	303,	6,	0,					// DO_KICK
	309,	2,	0,					// DO_PUNCH_HIT1
	311,	4,	0,					// DO_PUNCH_HIT2
	315,	5,	0,					// DO_PUNCH_DEATH
	319,	2,	0,					// DO_KICK_HIT1
	321,	4,	0,					// DO_KICK_HIT2
	325,	5,	0,					// DO_KICK_DEATH
	330,	5,	0,					// DO_READY_WEAPON
	382,	8,	0,					// DO_GUN_DEATH
	398,	8,	0,					// DO_EXPLOSION_DEATH
	398,	8,	0,					// DO_EXPLOSION2_DEATH
	406,	12,0,					// DO_GRENADE_DEATH
	418,	18,0,					// DO_FIRE_DEATH
	436,	3,	3,					// DO_GESTURE1
	460,	3,	3,					// DO_SALUTE1
	484,	3,	3,					// DO_GESTURE2
	508,	3,	3,					// DO_SALUTE2
	0,		1,	1,					// DO_PULL_GUN			// N/A
	0,		1,	1,					//	DO_PLEAD				// N/A
	0,		1, 1					//	DO_PLEAD_DEATH		// N/A
};


static InfantryTypeClass const E1(
	INFANTRY_E1,				// Infantry type number.
	TXT_E1,						// Translate name number for infantry type.
	"E1",							// INI name for infantry.
	1,								// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	true,							// Is a leader type?
	true,							// Has crawling animation frames?
	false,						// Is this a civlian?
	false,						// Always use the given name for the infantry?
	false,						// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	-1,							// Number of shots it has (default).
	&MiniGunnerDos[0][0],	// Ptr to minigunner 'DO' table above
	2,								// Frame of projectile launch.
	2,								// Frame of projectile launch while prone.
	50,							// Strength of infantry (in damage points).
	1,								// Sight range.
	100,							// Cost of infantry (in credits).
	1,								// Scenario when they first appear.
	80,10,						// Risk/Reward of this infantry unit.
 	HOUSEF_MULTI1|
 	HOUSEF_MULTI2|
 	HOUSEF_MULTI3|
 	HOUSEF_MULTI4|
 	HOUSEF_MULTI5|
 	HOUSEF_MULTI6|
 	HOUSEF_JP|
 	HOUSEF_GOOD|
 	HOUSEF_BAD,					// Who can own this infantry unit.
 	WEAPON_M16,WEAPON_NONE,
	MPH_SLOW						// Maximum speed of infantry.
);


// Grenadiers


int GrenadierDos [DO_COUNT][3]={
	0,		1,	1,					// DO_STAND_READY
	8,		1,	1,					// DO_STAND_GUARD
	288,	1,	12,				// DO_PRONE
	16,	6,	6,					// DO_WALK
	64,	20,20,				// DO_FIRE_WEAPON
	224,	2,	2,					// DO_LIE_DOWN
	240,	4,	4,					// DO_CRAWL
	272,	2,	2,					// DO_GET_UP
	288,	8,	12,				// DO_FIRE_PRONE
	384,	16,0,					// DO_IDLE1
	400,	16,0,					// DO_IDLE2
	416,	13,0,					// DO_ON_GUARD
	420,	10,0,					// DO_FIGHT_READY
	429,	2,	0,					// DO_PUNCH
	431,	6,	0,					// DO_KICK
	437,	2,	0,					// DO_PUNCH_HIT1
	439,	4,	0,					// DO_PUNCH_HIT2
	443,	4,	0,					// DO_PUNCH_DEATH
	447,	2,	0,					// DO_KICK_HIT1
	449,	4,	0,					// DO_KICK_HIT2
	453,	5,	0,					// DO_KICK_DEATH
	458,	5,	0,					// DO_READY_WEAPON
	510,	8,	0,					// DO_GUN_DEATH
	526,	8,	0,					// DO_EXPLOSION_DEATH
	526,	8,	0,					// DO_EXPLOSION2_DEATH
	534,	12,0,					// DO_GRENADE_DEATH
	546,	18,0,					// DO_FIRE_DEATH
	564,	3,	3,					// DO_GESTURE1
	588,	3,	3,					// DO_SALUTE1
	612,	3,	3,					// DO_GESTURE2
	636,	3,	3,					// DO_SALUTE2
	0,		1,	1,					// DO_PULL_GUN			// N/A
	0,		1,	1,					//	DO_PLEAD				// N/A
	0,		1, 1					//	DO_PLEAD_DEATH		// N/A
};

static InfantryTypeClass const E2(
	INFANTRY_E2,				// Infantry type number.
	TXT_E2,						// Translate name number for infantry type.
	"E2",							// INI name for infantry.
	1,								// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	true,							// Is a leader type?
	true,							// Has crawling animation frames?
	false,						// Is this a civlian?
	false,						// Always use the given name for the infantry?
	false,						// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	-1,							// Number of shots it has (default).
	&GrenadierDos[0][0],		// Ptr to grenadier DO table (above)
	14,							// Frame of projectile launch.
	6,								// Frame of projectile launch while prone.
	50,							// Strength of infantry (in damage points).
	1,								// Sight range.
	160,							// Cost of infantry (in credits).
	3,								// Scenario when they first appear.
	80,10,						// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD,				// Who can own this infantry unit.
	WEAPON_GRENADE,WEAPON_NONE,
	MPH_SLOW_ISH			// Maximum speed of infantry.
);




// Bazooka

int BazookaDos [DO_COUNT][3]={
	0,		1,	1,					// DO_STAND_READY
	8,		1,	1,					// DO_STAND_GUARD
	192,	1,10,					// DO_PRONE
	16,	6,	6,					// DO_WALK
	64,	8,	8,					// DO_FIRE_WEAPON
	128,	2,	2,					// DO_LIE_DOWN
	144,	4,	4,					// DO_CRAWL
	176,	2,	2,					// DO_GET_UP
	192,	10,10,				// DO_FIRE_PRONE
	272,	16,0,					// DO_IDLE1
	288,	16,0,					// DO_IDLE2
	304,	13,0,					// DO_ON_GUARD
	308,	10,0,					// DO_FIGHT_READY
	307,	2,	0,					// DO_PUNCH
	319,	6,	0,					// DO_KICK
	325,	2,	0,					// DO_PUNCH_HIT1
	327,	4,	0,					// DO_PUNCH_HIT2
	331,	4,	0,					// DO_PUNCH_DEATH
	335,	2,	0,					// DO_KICK_HIT1
	337,	4,	0,					// DO_KICK_HIT2
	341,	5,	0,					// DO_KICK_DEATH
	346,	5,	0,					// DO_READY_WEAPON
	398,	8,	0,					// DO_GUN_DEATH
	414,	8,	0,					// DO_EXPLOSION_DEATH
	414,	8,	0,					// DO_EXPLOSION2_DEATH
	422,	12,0,					// DO_GRENADE_DEATH
	434,	18,0,					// DO_FIRE_DEATH
	452,	3,	3,					// DO_GESTURE1
	476,	3,	3,					// DO_SALUTE1
	500,	3,	3,					// DO_GESTURE2
	524,	3,	3,					// DO_SALUTE2
	0,		1,	1,					// DO_PULL_GUN			// N/A
	0,		1,	1,					//	DO_PLEAD				// N/A
	0,		1, 1					//	DO_PLEAD_DEATH		// N/A
};



static InfantryTypeClass const E3(
	INFANTRY_E3,				// Infantry type number.
	TXT_E3,						// Translate name number for infantry type.
	"E3",							// INI name for infantry.
	2,								// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	true,							// Is a leader type?
	true,							// Has crawling animation frames?
	false,						// Is this a civlian?
	false,						// Always use the given name for the infantry?
	false,						// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	-1,							// Number of shots it has (default).
	&BazookaDos[0][0],		// Ptr to DO table (above)
	3,								// Frame of projectile launch.
	3,								// Frame of projectile launch while prone.
	25,							// Strength of infantry (in damage points).
	2,								// Sight range.
	300,							// Cost of infantry (in credits).
	3,								// Scenario when they first appear.
	80,10,						// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD,					// Who can own this infantry unit.
	WEAPON_DRAGON,WEAPON_NONE,
	MPH_KINDA_SLOW				// Maximum speed of infantry.
);

// Flamethrower

int FlamethrowerDos [DO_COUNT][3]={
	0,		1,	1,					// DO_STAND_READY
	8,		1,	1,					// DO_STAND_GUARD
	256,	1,16,					// DO_PRONE
	16,	6,	6,					// DO_WALK
	64,	16,16,				// DO_FIRE_WEAPON
	192,	2,	2,					// DO_LIE_DOWN
	208,	4,	4,					// DO_CRAWL
	240,	2,	2,					// DO_GET_UP
	256,	16,16,				// DO_FIRE_PRONE
	384,	16,0,					// DO_IDLE1
	400,	16,0,					// DO_IDLE2
	416,	13,0,					// DO_ON_GUARD
	420,	10,0,					// DO_FIGHT_READY
	429,	2,	0,					// DO_PUNCH
	431,	6,	0,					// DO_KICK
	437,	2,	0,					// DO_PUNCH_HIT1
	439,	4,	0,					// DO_PUNCH_HIT2
	443,	4,	0,					// DO_PUNCH_DEATH
	447,	2,	0,					// DO_KICK_HIT1
	449,	4,	0,					// DO_KICK_HIT2
	453,	5,	0,					// DO_KICK_DEATH
	458,	5,	0,					// DO_READY_WEAPON
	510,	8,	0,					// DO_GUN_DEATH
	526,	8,	0,					// DO_EXPLOSION_DEATH
	526,	8,	0,					// DO_EXPLOSION2_DEATH
	534,	12,0,					// DO_GRENADE_DEATH
	546,	18,0,					// DO_FIRE_DEATH
	564,	3,	3,					// DO_GESTURE1
	588,	3,	3,					// DO_SALUTE1
	612,	3,	3,					// DO_GESTURE2
	636,	3,	3,					// DO_SALUTE2
	0,		1,	1,					// DO_PULL_GUN			// N/A
	0,		1,	1,					//	DO_PLEAD				// N/A
	0,		1, 1					//	DO_PLEAD_DEATH		// N/A
};



static InfantryTypeClass const E4(
	INFANTRY_E4,				// Infantry type number.
	TXT_E4,						// Translate name number for infantry type.
	"E4",							// INI name for infantry.
	1,								// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	true,							// Is a leader type?
	true,							// Has crawling animation frames?
	false,						// Is this a civlian?
	false,						// Always use the given name for the infantry?
	false,						// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	-1,							// Number of shots it has (default).
	&FlamethrowerDos[0][0],	// ptr to DO table (above)
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	70,							// Strength of infantry (in damage points).
	1,								// Sight range.
	200,							// Cost of infantry (in credits).
	5,								// Scenario when they first appear.
	80,10,						// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_BAD,					// Who can own this infantry unit.
	WEAPON_FLAMETHROWER,WEAPON_NONE,
	MPH_SLOW_ISH
);


// Chemwarrior

int ChemwarriorDos [DO_COUNT][3]={
	0,		1,	1,					// DO_STAND_READY
	8,		1,	1,					// DO_STAND_GUARD
	256,	1,16,					// DO_PRONE
	16,	6,	6,					// DO_WALK
	64,	16,16,				// DO_FIRE_WEAPON
	192,	2,	2,					// DO_LIE_DOWN
	208,	4,	4,					// DO_CRAWL
	240,	2,	2,					// DO_GET_UP
	256,	16,16,				// DO_FIRE_PRONE
	384,	16,0,					// DO_IDLE1
	400,	16,0,					// DO_IDLE2
	416,	13,0,					// DO_ON_GUARD
	420,	10,0,					// DO_FIGHT_READY
	429,	2,	0,					// DO_PUNCH
	431,	6,	0,					// DO_KICK
	437,	2,	0,					// DO_PUNCH_HIT1
	439,	4,	0,					// DO_PUNCH_HIT2
	443,	4,	0,					// DO_PUNCH_DEATH
	447,	2,	0,					// DO_KICK_HIT1
	449,	4,	0,					// DO_KICK_HIT2
	453,	5,	0,					// DO_KICK_DEATH
	458,	5,	0,					// DO_READY_WEAPON
	510,	8,	0,					// DO_GUN_DEATH
	526,	8,	0,					// DO_EXPLOSION_DEATH
	526,	8,	0,					// DO_EXPLOSION2_DEATH
	534,	12,0,					// DO_GRENADE_DEATH
	546,	18,0,					// DO_FIRE_DEATH
	564,	3,	3,					// DO_GESTURE1
	588,	3,	3,					// DO_SALUTE1
	612,	3,	3,					// DO_GESTURE2
	636,	3,	3,					// DO_SALUTE2
	0,		1,	1,					// DO_PULL_GUN			// N/A
	0,		1,	1,					//	DO_PLEAD				// N/A
	0,		1, 1					//	DO_PLEAD_DEATH		// N/A
};


static InfantryTypeClass const E5(
	INFANTRY_E5,				// Infantry type number.
	TXT_E5,						// Translate name number for infantry type.
	"E5",							// INI name for infantry.
	7,								// Build level.
	STRUCTF_EYE,				// Building prerequisite.
	false,						// Is this a female type?
	true,							// Is a leader type?
	true,							// Has crawling animation frames?
	false,						// Is this a civlian?
	false,						// Always use the given name for the infantry?
	false,						// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	-1,							// Number of shots it has (default).
	&ChemwarriorDos[0][0],	// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	70,							// Strength of infantry (in damage points).
	1,								// Sight range.
	300,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	80,10,						// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
//	HOUSEF_GOOD|
	HOUSEF_BAD,					// Who can own this infantry unit.
	WEAPON_CHEMSPRAY,WEAPON_NONE,
	MPH_SLOW
);


// Engineer

int EngineerDos[DO_COUNT][3]={
	0,		1,	1,					// DO_STAND_READY
	8,		1,	1,					// DO_STAND_GUARD
	82,	1,	4,					// DO_PRONE
	16,	6,	6,					// DO_WALK
	0,		0,	0,					// DO_FIRE_WEAPON
	67,	2,	2,					// DO_LIE_DOWN
	82,	4,	4,					// DO_CRAWL
	114,	2,	2,					// DO_GET_UP
	0,		0,	0,					// DO_FIRE_PRONE
	130,	16,0,					// DO_IDLE1
	0,		0,	0,					// DO_IDLE2
	0,		0,	0,					// DO_ON_GUARD
	0,		0,	0,					// DO_FIGHT_READY
	0,		0,	0,					// DO_PUNCH
	0,		0,	0,					// DO_KICK
	0,		0,	0,					// DO_PUNCH_HIT1
	0,		0,	0,					// DO_PUNCH_HIT2
	0,		0,	0,					// DO_PUNCH_DEATH
	0,		0,	0,					// DO_KICK_HIT1
	0,		0,	0,					// DO_KICK_HIT2
	0,		0,	0,					// DO_KICK_DEATH
	0,		0,	0,					// DO_READY_WEAPON
	146,	8,	0,					// DO_GUN_DEATH
	154,	8,	0,					// DO_EXPLOSION_DEATH
	162,	8,	0,					// DO_EXPLOSION2_DEATH
	170,	12,0,					// DO_GRENADE_DEATH
	182,	18,0,					// DO_FIRE_DEATH
	200,	3,	3,					// DO_GESTURE1
	224,	3,	3,					// DO_SALUTE1
	200,	3,	3,					// DO_GESTURE2
	224,	3,	3,					// DO_SALUTE2
	0,		1,	1,					// DO_PULL_GUN			// N/A
	0,		1,	1,					//	DO_PLEAD				// N/A
	0,		1, 1					//	DO_PLEAD_DEATH		// N/A
};

static InfantryTypeClass const E7(
	INFANTRY_E7,				// Infantry type number.
	TXT_E7,						// Translate name number for infantry type.
	"E6",							// INI name for infantry.
	3,								// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	false,						// Is a leader type?
	false,						// Has crawling animation frames?
	false,						// Is this a civlian?
	false,						// Always use the given name for the infantry?
	false,						// Is this a "fraidycat" run-away type infantry?
	true,							// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	-1,							// Number of shots it has (default).
	&EngineerDos[0][0],		// ptr to DO table
	3,								// Frame of projectile launch.
	3,								// Frame of projectile launch while prone.
	25,							// Strength of infantry (in damage points).
	2,								// Sight range.
	500,							// Cost of infantry (in credits).
	2,								// Scenario when they first appear.
	80,75,						// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_BAD|
	HOUSEF_GOOD,				// Who can own this infantry unit.
	WEAPON_NONE,WEAPON_NONE,
	MPH_SLOW						// Maximum speed of infantry.
);

// Commandos

int CommandoDos[DO_COUNT][3]={
	0,		1,	1,					// DO_STAND_READY
	8,		1,	1,					// DO_STAND_GUARD
	160,	1,	4,					// DO_PRONE
	16,	6,	6,					// DO_WALK
	64,	4,	4,					// DO_FIRE_WEAPON
	96,	2,	2,					// DO_LIE_DOWN
	112,	4,	4,					// DO_CRAWL
	144,	2,	2,					// DO_GET_UP
	160,	4,	4,					// DO_FIRE_PRONE
	192,	16,0,					// DO_IDLE1
	208,	16,0,					// DO_IDLE2
	224,	13,0,					// DO_ON_GUARD
	228,	9,	0,					// DO_FIGHT_READY
	237,	2,	0,					// DO_PUNCH
	239,	6,	0,					// DO_KICK
	245,	2,	0,					// DO_PUNCH_HIT1
	247,	4,	0,					// DO_PUNCH_HIT2
	251,	4,	0,					// DO_PUNCH_DEATH
	255,	2,	0,					// DO_KICK_HIT1
	257,	4,	0,					// DO_KICK_HIT2
	261,	5,	0,					// DO_KICK_DEATH
	266,	5,	0,					// DO_READY_WEAPON
	318,	8,	0,					// DO_GUN_DEATH
	334,	8,	0,					// DO_EXPLOSION_DEATH
	334,	8,	0,					// DO_EXPLOSION2_DEATH
	342,	12,0,					// DO_GRENADE_DEATH
	354,	18,0,					// DO_FIRE_DEATH
	372,	3,	3,					// DO_GESTURE1
	396,	3,	3,					// DO_SALUTE1
	420,	3,	3,					// DO_GESTURE2
	444,	3,	3,					// DO_SALUTE2
	0,		1,	1,					// DO_PULL_GUN			// N/A
	0,		1,	1,					//	DO_PLEAD				// N/A
	0,		1, 1					//	DO_PLEAD_DEATH		// N/A

};
static InfantryTypeClass const Commando(
	INFANTRY_RAMBO,			// Infantry type number.
	TXT_RAMBO,					// Translate name number for infantry type.
	"RMBO",						// INI name for infantry.
	7,								// Build level.
	STRUCTF_EYE,				// Building prerequisite.
	false,						// Is this a female type?
	true,							// Is a leader type?
	true,							// Has crawling animation frames?
	false,						// Is this a civlian?
	false,						// Always use the given name for the infantry?
	false,						// Is this a "fraidycat" run-away type infantry?
	true,							// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	-1,							// Number of shots it has (default).
	&CommandoDos[0][0],		// ptr to DO table
	2,								// Frame of projectile launch.
	2,								// Frame of projectile launch while prone.
	80,							// Strength of infantry (in damage points).
	5,								// Sight range.
	1000,							// Cost of infantry (in credits).
	98,							// Scenario when they first appear.
	80,75,						// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD,					// Who can own this infantry unit.
	WEAPON_RIFLE,WEAPON_NONE,
	MPH_SLOW_ISH			// Maximum speed of infantry.
);

// Civilians

int CivilianDos1[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	205,		4,	4,				// DO_FIRE_PRONE
	189,		10,0,				// DO_IDLE1
	199,		6,	0,				// DO_IDLE2
	104,		3,	0,				// DO_ON_GUARD
	107,		7,	0,				// DO_FIGHT_READY
	114,		2,	0,				// DO_PUNCH
	116,		6,	0,				// DO_KICK
	122,		2,	0,				// DO_PUNCH_HIT1
	124,		4,	0,				// DO_PUNCH_HIT2
	128,		4,	0,				// DO_PUNCH_DEATH
	133,		2,	0,				// DO_KICK_HIT1
	135,		4,	0,				// DO_KICK_HIT2
	139,		5,	0,				// DO_KICK_DEATH
	144,		3,	0,				// DO_READY_WEAPON
	329,		8,	0,				// DO_GUN_DEATH
	337,		8,	0,				// DO_EXPLOSION_DEATH
	337,		8,	0,				// DO_EXPLOSION2_DEATH
	345,		12,0,				// DO_GRENADE_DEATH
	357,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	199,		6,	0,				// DO_PULL_GUN
	237,		40,0,				//	DO_PLEAD
	277,		6, 0				//	DO_PLEAD_DEATH
};

static InfantryTypeClass const C1(
	INFANTRY_C1,				// Infantry type number.
	TXT_C1,						// Translate name number for infantry type.
	"C1",							// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	true,							// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	10,							// Number of shots it has (default).
	&CivilianDos1[0][0],		// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	25,							// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,1,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_PISTOL,WEAPON_NONE,
	MPH_SLOW_ISH
);


int CivilianDos2[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	205,		4,	4,				// DO_FIRE_PRONE
	189,		10,0,				// DO_IDLE1
	199,		6,	0,				// DO_IDLE2
	104,		3,	0,				// DO_ON_GUARD
	107,		7,	0,				// DO_FIGHT_READY
	114,		2,	0,				// DO_PUNCH
	116,		6,	0,				// DO_KICK
	122,		2,	0,				// DO_PUNCH_HIT1
	124,		4,	0,				// DO_PUNCH_HIT2
	128,		4,	0,				// DO_PUNCH_DEATH
	133,		2,	0,				// DO_KICK_HIT1
	135,		4,	0,				// DO_KICK_HIT2
	139,		5,	0,				// DO_KICK_DEATH
	144,		3,	0,				// DO_READY_WEAPON
	329,		8,	0,				// DO_GUN_DEATH
	337,		8,	0,				// DO_EXPLOSION_DEATH
	337,		8,	0,				// DO_EXPLOSION2_DEATH
	345,		12,0,				// DO_GRENADE_DEATH
	357,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	199,		6,	0,				// DO_PULL_GUN
	237,		40,0,				//	DO_PLEAD
	277,		6, 0				//	DO_PLEAD_DEATH
};

static InfantryTypeClass const C2(
	INFANTRY_C2,				// Infantry type number.
	TXT_C2,						// Translate name number for infantry type.
	"C2",							// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	false,						// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	0,								// Number of shots it has (default).
	&CivilianDos2[0][0],		// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	5,								// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,1,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_NONE,WEAPON_NONE,
	MPH_SLOW_ISH
);


int CivilianDos3[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	205,		4,	4,				// DO_FIRE_PRONE
	189,		10,0,				// DO_IDLE1
	199,		6,	0,				// DO_IDLE2
	104,		3,	0,				// DO_ON_GUARD
	107,		7,	0,				// DO_FIGHT_READY
	114,		2,	0,				// DO_PUNCH
	116,		6,	0,				// DO_KICK
	122,		2,	0,				// DO_PUNCH_HIT1
	124,		4,	0,				// DO_PUNCH_HIT2
	128,		4,	0,				// DO_PUNCH_DEATH
	133,		2,	0,				// DO_KICK_HIT1
	135,		4,	0,				// DO_KICK_HIT2
	139,		5,	0,				// DO_KICK_DEATH
	144,		3,	0,				// DO_READY_WEAPON
	329,		8,	0,				// DO_GUN_DEATH
	337,		8,	0,				// DO_EXPLOSION_DEATH
	337,		8,	0,				// DO_EXPLOSION2_DEATH
	345,		12,0,				// DO_GRENADE_DEATH
	357,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	199,		6,	0,				// DO_PULL_GUN
	237,		40,0,				//	DO_PLEAD
	277,		6, 0				//	DO_PLEAD_DEATH

};

static InfantryTypeClass const C3(
	INFANTRY_C3,				// Infantry type number.
	TXT_C3,						// Translate name number for infantry type.
	"C3",							// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	true,							// Is this a female type?
	false,						// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	0,								// Number of shots it has (default).
	&CivilianDos3[0][0],		// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	5,								// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,1,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_NONE,WEAPON_NONE,
	MPH_SLOW_ISH
);


int CivilianDos4[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	205,		4,	4,				// DO_FIRE_PRONE
	189,		10,0,				// DO_IDLE1
	199,		6,	0,				// DO_IDLE2
	104,		3,	0,				// DO_ON_GUARD
	107,		7,	0,				// DO_FIGHT_READY
	114,		2,	0,				// DO_PUNCH
	116,		6,	0,				// DO_KICK
	122,		2,	0,				// DO_PUNCH_HIT1
	124,		4,	0,				// DO_PUNCH_HIT2
	128,		4,	0,				// DO_PUNCH_DEATH
	133,		2,	0,				// DO_KICK_HIT1
	135,		4,	0,				// DO_KICK_HIT2
	139,		5,	0,				// DO_KICK_DEATH
	144,		3,	0,				// DO_READY_WEAPON
	329,		8,	0,				// DO_GUN_DEATH
	337,		8,	0,				// DO_EXPLOSION_DEATH
	337,		8,	0,				// DO_EXPLOSION2_DEATH
	345,		12,0,				// DO_GRENADE_DEATH
	357,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	199,		6,	0,				// DO_PULL_GUN
	237,		40,0,				//	DO_PLEAD
	277,		6, 0,				//	DO_PLEAD_DEATH
};


static InfantryTypeClass const C4(
	INFANTRY_C4,				// Infantry type number.
	TXT_C4,						// Translate name number for infantry type.
	"C4",							// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	true,							// Is this a female type?
	false,						// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	0,								// Number of shots it has (default).
	&CivilianDos4[0][0],		// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	5,								// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,1,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_NONE,WEAPON_NONE,
	MPH_SLOW_ISH
);


int CivilianDos5[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	205,		4,	4,				// DO_FIRE_PRONE
	189,		10,0,				// DO_IDLE1
	199,		6,	0,				// DO_IDLE2
	104,		3,	0,				// DO_ON_GUARD
	107,		7,	0,				// DO_FIGHT_READY
	114,		2,	0,				// DO_PUNCH
	116,		6,	0,				// DO_KICK
	122,		2,	0,				// DO_PUNCH_HIT1
	124,		4,	0,				// DO_PUNCH_HIT2
	128,		4,	0,				// DO_PUNCH_DEATH
	133,		2,	0,				// DO_KICK_HIT1
	135,		4,	0,				// DO_KICK_HIT2
	139,		5,	0,				// DO_KICK_DEATH
	144,		3,	0,				// DO_READY_WEAPON
	329,		8,	0,				// DO_GUN_DEATH
	337,		8,	0,				// DO_EXPLOSION_DEATH
	337,		8,	0,				// DO_EXPLOSION2_DEATH
	345,		12,0,				// DO_GRENADE_DEATH
	357,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	199,		6,	0,				// DO_PULL_GUN
	237,		40,0,				//	DO_PLEAD
	277,		6, 0				//	DO_PLEAD_DEATH
};

static InfantryTypeClass const C5(
	INFANTRY_C5,				// Infantry type number.
	TXT_C5,						// Translate name number for infantry type.
	"C5",							// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	false,						// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	0,								// Number of shots it has (default).
	&CivilianDos5[0][0],		// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	5,								// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,1,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_NONE,WEAPON_NONE,
	MPH_SLOW_ISH
);


int CivilianDos6[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	205,		4,	4,				// DO_FIRE_PRONE
	189,		10,0,				// DO_IDLE1
	199,		6,	0,				// DO_IDLE2
	104,		3,	0,				// DO_ON_GUARD
	107,		7,	0,				// DO_FIGHT_READY
	114,		2,	0,				// DO_PUNCH
	116,		6,	0,				// DO_KICK
	122,		2,	0,				// DO_PUNCH_HIT1
	124,		4,	0,				// DO_PUNCH_HIT2
	128,		4,	0,				// DO_PUNCH_DEATH
	133,		2,	0,				// DO_KICK_HIT1
	135,		4,	0,				// DO_KICK_HIT2
	139,		5,	0,				// DO_KICK_DEATH
	144,		3,	0,				// DO_READY_WEAPON
	329,		8,	0,				// DO_GUN_DEATH
	337,		8,	0,				// DO_EXPLOSION_DEATH
	337,		8,	0,				// DO_EXPLOSION2_DEATH
	345,		12,0,				// DO_GRENADE_DEATH
	357,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	199,		6,	0,				// DO_PULL_GUN
	237,		40,0,				//	DO_PLEAD
	277,		6, 0				//	DO_PLEAD_DEATH
};

static InfantryTypeClass const C6(
	INFANTRY_C6,				// Infantry type number.
	TXT_C6,						// Translate name number for infantry type.
	"C6",							// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	false,						// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	0,								// Number of shots it has (default).
	&CivilianDos6[0][0],		// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	5,								// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,1,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_NONE,WEAPON_NONE,
	MPH_SLOW_ISH
);


int CivilianDos7[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	205,		4,	4,				// DO_FIRE_PRONE
	189,		10,0,				// DO_IDLE1
	199,		6,	0,				// DO_IDLE2
	104,		3,	0,				// DO_ON_GUARD
	107,		7,	0,				// DO_FIGHT_READY
	114,		2,	0,				// DO_PUNCH
	116,		6,	0,				// DO_KICK
	122,		2,	0,				// DO_PUNCH_HIT1
	124,		4,	0,				// DO_PUNCH_HIT2
	128,		4,	0,				// DO_PUNCH_DEATH
	133,		2,	0,				// DO_KICK_HIT1
	135,		4,	0,				// DO_KICK_HIT2
	139,		5,	0,				// DO_KICK_DEATH
	144,		3,	0,				// DO_READY_WEAPON
	329,		8,	0,				// DO_GUN_DEATH
	337,		8,	0,				// DO_EXPLOSION_DEATH
	337,		8,	0,				// DO_EXPLOSION2_DEATH
	345,		12,0,				// DO_GRENADE_DEATH
	357,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	199,		6,	0,				// DO_PULL_GUN
	237,		40,0,				//	DO_PLEAD
	277,		6, 0				//	DO_PLEAD_DEATH
};

static InfantryTypeClass const C7(
	INFANTRY_C7,				// Infantry type number.
	TXT_C7,						// Translate name number for infantry type.
	"C7",							// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	true,							// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	10,							// Number of shots it has (default).
	&CivilianDos7[0][0],		// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	5,								// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,1,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_PISTOL,WEAPON_NONE,
	MPH_SLOW_ISH
);

int CivilianDos8[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	205,		4,	4,				// DO_FIRE_PRONE
	189,		10,0,				// DO_IDLE1
	199,		6,	0,				// DO_IDLE2
	104,		3,	0,				// DO_ON_GUARD
	107,		7,	0,				// DO_FIGHT_READY
	114,		2,	0,				// DO_PUNCH
	116,		6,	0,				// DO_KICK
	122,		2,	0,				// DO_PUNCH_HIT1
	124,		4,	0,				// DO_PUNCH_HIT2
	128,		4,	0,				// DO_PUNCH_DEATH
	133,		2,	0,				// DO_KICK_HIT1
	135,		4,	0,				// DO_KICK_HIT2
	139,		5,	0,				// DO_KICK_DEATH
	144,		3,	0,				// DO_READY_WEAPON
	329,		8,	0,				// DO_GUN_DEATH
	337,		8,	0,				// DO_EXPLOSION_DEATH
	337,		8,	0,				// DO_EXPLOSION2_DEATH
	345,		12,0,				// DO_GRENADE_DEATH
	357,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	199,		6,	0,				// DO_PULL_GUN
	237,		40,0,				//	DO_PLEAD
	277,		6, 0				//	DO_PLEAD_DEATH
};

static InfantryTypeClass const C8(
	INFANTRY_C8,				// Infantry type number.
	TXT_C8,						// Translate name number for infantry type.
	"C8",							// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	false,						// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	0,								// Number of shots it has (default).
	&CivilianDos8[0][0],		// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	5,								// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,1,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_NONE,WEAPON_NONE,
	MPH_SLOW_ISH
);


int CivilianDos9[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	205,		4,	4,				// DO_FIRE_PRONE
	189,		10,0,				// DO_IDLE1
	199,		6,	0,				// DO_IDLE2
	104,		3,	0,				// DO_ON_GUARD
	107,		7,	0,				// DO_FIGHT_READY
	114,		2,	0,				// DO_PUNCH
	116,		6,	0,				// DO_KICK
	122,		2,	0,				// DO_PUNCH_HIT1
	124,		4,	0,				// DO_PUNCH_HIT2
	128,		4,	0,				// DO_PUNCH_DEATH
	133,		2,	0,				// DO_KICK_HIT1
	135,		4,	0,				// DO_KICK_HIT2
	139,		5,	0,				// DO_KICK_DEATH
	144,		3,	0,				// DO_READY_WEAPON
	329,		8,	0,				// DO_GUN_DEATH
	337,		8,	0,				// DO_EXPLOSION_DEATH
	337,		8,	0,				// DO_EXPLOSION2_DEATH
	345,		12,0,				// DO_GRENADE_DEATH
	357,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	199,		6,	0,				// DO_PULL_GUN
	237,		40,0,				//	DO_PLEAD
	277,		6, 0				//	DO_PLEAD_DEATH
};

static InfantryTypeClass const C9(
	INFANTRY_C9,				// Infantry type number.
	TXT_C9,						// Translate name number for infantry type.
	"C9",							// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	false,						// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	0,								// Number of shots it has (default).
	&CivilianDos9[0][0],		// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	5,								// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,1,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_NONE,WEAPON_NONE,
	MPH_SLOW_ISH
);


int NikoombaDos[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	205,		4,	4,				// DO_FIRE_PRONE
	189,		10,0,				// DO_IDLE1
	199,		6,	0,				// DO_IDLE2
	104,		3,	0,				// DO_ON_GUARD
	107,		7,	0,				// DO_FIGHT_READY
	114,		2,	0,				// DO_PUNCH
	116,		6,	0,				// DO_KICK
	122,		2,	0,				// DO_PUNCH_HIT1
	124,		4,	0,				// DO_PUNCH_HIT2
	128,		4,	0,				// DO_PUNCH_DEATH
	133,		2,	0,				// DO_KICK_HIT1
	135,		4,	0,				// DO_KICK_HIT2
	139,		5,	0,				// DO_KICK_DEATH
	144,		3,	0,				// DO_READY_WEAPON
	329,		8,	0,				// DO_GUN_DEATH
	337,		8,	0,				// DO_EXPLOSION_DEATH
	337,		8,	0,				// DO_EXPLOSION2_DEATH
	345,		12,0,				// DO_GRENADE_DEATH
	357,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	199,		6,	0,				// DO_PULL_GUN
	237,		40,0,				//	DO_PLEAD
	277,		6, 0,				//	DO_PLEAD_DEATH

};

// Nikoomba
static InfantryTypeClass const C10(
	INFANTRY_C10,				// Infantry type number.
	TXT_C10,						// Translate name number for infantry type.
	"C10",						// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	false,						// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	0,								// Number of shots it has (default).
	&NikoombaDos[0][0],		// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	50,							// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,1,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_NONE,WEAPON_NONE,
	MPH_SLOW_ISH
);

int MoebiusDos[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	0,			0,	0,				// DO_FIRE_PRONE
	104,		16,0,				// DO_IDLE1
	120,		20,0,				// DO_IDLE2
	0,			0,	0,				// DO_ON_GUARD
	0,			0,	0,				// DO_FIGHT_READY
	0,			0,	0,				// DO_PUNCH
	0,			0,	0,				// DO_KICK
	0,			0,	0,				// DO_PUNCH_HIT1
	0,			0,	0,				// DO_PUNCH_HIT2
	0,			0,	0,				// DO_PUNCH_DEATH
	0,			0,	0,				// DO_KICK_HIT1
	0,			0,	0,				// DO_KICK_HIT2
	0,			0,	0,				// DO_KICK_DEATH
	0,			0,	0,				// DO_READY_WEAPON
	212,		8,	0,				// DO_GUN_DEATH
	220,		8,	0,				// DO_EXPLOSION_DEATH
	228,		12,0,				// DO_EXPLOSION2_DEATH
	228,		12,0,				// DO_GRENADE_DEATH
	240,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	0,			0,	0,				// DO_PULL_GUN
	120,		31,0,				//	DO_PLEAD
	151,		14,0				//	DO_PLEAD_DEATH
};

static InfantryTypeClass const Moebius(
	INFANTRY_MOEBIUS,			// Infantry type number.
	TXT_MOEBIUS,				// Translate name number for infantry type.
	"MOEBIUS",					// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	false,						// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	0,								// Number of shots it has (default).
	&MoebiusDos[0][0],		// ptr to DO table
	0,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	50,							// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,10,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_NONE,WEAPON_NONE,
	MPH_SLOW_ISH
);

int DelphiDos[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	205,		4,	4,				// DO_FIRE_PRONE
	189,		10,0,				// DO_IDLE1
	199,		6,	0,				// DO_IDLE2
	104,		3,	0,				// DO_ON_GUARD
	107,		7,	0,				// DO_FIGHT_READY
	114,		2,	0,				// DO_PUNCH
	116,		6,	0,				// DO_KICK
	122,		2,	0,				// DO_PUNCH_HIT1
	124,		4,	0,				// DO_PUNCH_HIT2
	128,		4,	0,				// DO_PUNCH_DEATH
	133,		2,	0,				// DO_KICK_HIT1
	135,		4,	0,				// DO_KICK_HIT2
	139,		5,	0,				// DO_KICK_DEATH
	144,		3,	0,				// DO_READY_WEAPON
	329,		8,	0,				// DO_GUN_DEATH
	337,		8,	0,				// DO_EXPLOSION_DEATH
	337,		8,	0,				// DO_EXPLOSION2_DEATH
	345,		12,0,				// DO_GRENADE_DEATH
	357,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	199,		6,	0,				// DO_PULL_GUN
	237,		40,0,				//	DO_PLEAD
	277,		6, 0,				//	DO_PLEAD_DEATH
};

static InfantryTypeClass const Delphi(
	INFANTRY_DELPHI,			// Infantry type number.
	TXT_DELPHI,					// Translate name number for infantry type.
	"DELPHI",					// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	false,						// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	10,							// Number of shots it has (default).
	&DelphiDos[0][0],			// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	25,							// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,0,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_PISTOL,WEAPON_NONE,
	MPH_SLOW_ISH
);


int DrChanDos[DO_COUNT][3]={
	0,			1,	1,				// DO_STAND_READY
	0,			1,	1,				// DO_STAND_GUARD
	0,			1,	1,				// DO_PRONE				// N/A
	56, 		6,	6,				// DO_WALK
	205,		4, 4,				// DO_FIRE_WEAPON
	0, 		1,	1,				// DO_LIE_DOWN			// N/A
	8, 		6,	6,				// DO_CRAWL
	0, 		1,	1,				// DO_GET_UP			// N/A
	0,			0,	0,				// DO_FIRE_PRONE
	104,		16,0,				// DO_IDLE1
	120,		20,0,				// DO_IDLE2
	0,			0,	0,				// DO_ON_GUARD
	0,			0,	0,				// DO_FIGHT_READY
	0,			0,	0,				// DO_PUNCH
	0,			0,	0,				// DO_KICK
	0,			0,	0,				// DO_PUNCH_HIT1
	0,			0,	0,				// DO_PUNCH_HIT2
	0,			0,	0,				// DO_PUNCH_DEATH
	0,			0,	0,				// DO_KICK_HIT1
	0,			0,	0,				// DO_KICK_HIT2
	0,			0,	0,				// DO_KICK_DEATH
	0,			0,	0,				// DO_READY_WEAPON
	212,		8,	0,				// DO_GUN_DEATH
	220,		8,	0,				// DO_EXPLOSION_DEATH
	228,		12,0,				// DO_EXPLOSION2_DEATH
	228,		12,0,				// DO_GRENADE_DEATH
	240,		18,0,				// DO_FIRE_DEATH
	0, 		0,	0,				// DO_GESTURE1			// N/A
	0, 		0,	0,				// DO_SALUTE1			// N/A
	0, 		0,	0,				// DO_GESTURE2			// N/A
	0, 		0,	0,				// DO_SALUTE2			// N/A
	0,			0,	0,				// DO_PULL_GUN
	120,		31,0,				//	DO_PLEAD
	151,		14,0				//	DO_PLEAD_DEATH
};

static InfantryTypeClass const DrChan(
	INFANTRY_CHAN,				// Infantry type number.
	TXT_CHAN,					// Translate name number for infantry type.
	"CHAN",						// INI name for infantry.
	99,							// Build level.
	STRUCTF_NONE,				// Building prerequisite.
	false,						// Is this a female type?
	false,						// Is a leader type?
	false,						// Has crawling animation frames?
	true,							// Is this a civlian?
	true,							// Always use the given name for the infantry?
	true,							// Is this a "fraidycat" run-away type infantry?
	false,						// Can this infantry type capture a building?
	false,						// Theater specific graphic image?
	10,							// Number of shots it has (default).
	&DrChanDos[0][0],			// ptr to DO table
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	25,							// Strength of infantry (in damage points).
	0,								// Sight range.
	10,							// Cost of infantry (in credits).
	99,							// Scenario when they first appear.
	0,1,							// Risk/Reward of this infantry unit.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_NEUTRAL,				// Who can own this infantry unit.
	WEAPON_NONE,WEAPON_NONE,
	MPH_SLOW_ISH
);



/*
**	This is the array of pointers to the static data associated with each
**	infantry type.
*/
InfantryTypeClass const * const InfantryTypeClass::Pointers[INFANTRY_COUNT] = {
	&E1,
	&E2,
	&E3,
	&E4,
	&E5,
//	&E6,
	&E7,
	&Commando,
	&C1,
	&C2,
	&C3,
	&C4,
	&C5,
	&C6,
	&C7,
	&C8,
	&C9,
	&C10,
	&Moebius,
	&Delphi,
	&DrChan
};


/***********************************************************************************************
 * InfantryTypeClass::InfantryTypeClass -- Constructor for infantry type class objects.        *
 *                                                                                             *
 *    This routine will construct the infantry type objects. It is use to create the static    *
 *    infantry types that are used to give each of the infantry objects their characteristics. *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryTypeClass::InfantryTypeClass	(
						InfantryType type, int name, char const *ininame,
						unsigned char level, long pre,
						bool is_female,
						bool is_leader,
						bool is_crawling,
						bool is_civilian,
						bool is_nominal,
						bool is_fraidycat,
						bool is_capture,
						bool is_theater,
						int ammo,
						int *do_table,
						int firelaunch, int pronelaunch,
						unsigned short strength, int sightrange,
						int cost, int scenario, int risk, int reward, int ownable,
						WeaponType primary, WeaponType secondary,
						MPHType maxspeed)
	: TechnoTypeClass(name, ininame, level, pre,
							is_leader, true, is_nominal, false, false, true, true, true, true, false, false,
							is_theater, false, false, false, true, false,
							ammo, strength, maxspeed, sightrange, cost,
							scenario, risk, reward, ownable,
							primary, secondary,
							ARMOR_NONE)
{
	IsFemale = is_female;
	IsCrawling = is_crawling;
	IsCapture = is_capture;
	IsFraidyCat = is_fraidycat;
	IsCivilian = is_civilian;
	Type = type;
	FireLaunch = firelaunch;
	ProneLaunch = pronelaunch;

	/*
	**	Set the animation sequence custom values.
	*/

	for ( int i=0 ; i<DO_COUNT ; i++ ){
		DoControls[i].Frame = *do_table++;
		DoControls[i].Count = *do_table++;
		DoControls[i].Jump =  *do_table++;
	}

#ifdef cuts		//ST - 10/3/95 10:09AM
	DoControls[DO_STAND_READY].Frame = dostandready;
	DoControls[DO_STAND_READY].Count = dostandreadyframe;
	DoControls[DO_STAND_READY].Jump = dostandreadyjump;

	DoControls[DO_STAND_GUARD].Frame = dostandguard;
	DoControls[DO_STAND_GUARD].Count = dostandguardframe;
	DoControls[DO_STAND_GUARD].Jump = dostandguardjump;

	DoControls[DO_PRONE].Frame = doprone;
	DoControls[DO_PRONE].Count = doproneframe;
	DoControls[DO_PRONE].Jump = dopronejump;

	DoControls[DO_WALK].Frame = dowalk;
	DoControls[DO_WALK].Count = dowalkframe;
	DoControls[DO_WALK].Jump = dowalkjump;

	DoControls[DO_FIRE_WEAPON].Frame = dofireweapon;
	DoControls[DO_FIRE_WEAPON].Count = dofireweaponframe;
	DoControls[DO_FIRE_WEAPON].Jump = dofireweaponjump;

	DoControls[DO_LIE_DOWN].Frame = doliedown;
	DoControls[DO_LIE_DOWN].Count = doliedownframe;
	DoControls[DO_LIE_DOWN].Jump = doliedownjump;

	DoControls[DO_CRAWL].Frame = docrawl;
	DoControls[DO_CRAWL].Count = docrawlframe;
	DoControls[DO_CRAWL].Jump = docrawljump;

	DoControls[DO_GET_UP].Frame = dogetup;
	DoControls[DO_GET_UP].Count = dogetupframe;
	DoControls[DO_GET_UP].Jump = dogetupjump;

	DoControls[DO_FIRE_PRONE].Frame = dofireprone;
	DoControls[DO_FIRE_PRONE].Count = dofireproneframe;
	DoControls[DO_FIRE_PRONE].Jump = dofirepronejump;

	DoControls[DO_IDLE1].Frame = doidle1;
	DoControls[DO_IDLE1].Count = doidle1frame;
	DoControls[DO_IDLE1].Jump = doidle1jump;

	DoControls[DO_IDLE2].Frame = doidle2;
	DoControls[DO_IDLE2].Count = doidle2frame;
	DoControls[DO_IDLE2].Jump = doidle2jump;

	DoControls[DO_ON_GUARD].Frame = doonguard;
	DoControls[DO_ON_GUARD].Count = doonguardframe;
	DoControls[DO_ON_GUARD].Jump = doonguardjump;

	DoControls[DO_FIGHT_READY].Frame = dofightready;
	DoControls[DO_FIGHT_READY].Count = dofightreadyframe;
	DoControls[DO_FIGHT_READY].Jump = dofightreadyjump;

	DoControls[DO_PUNCH].Frame = dopunch;
	DoControls[DO_PUNCH].Count = dopunchframe;
	DoControls[DO_PUNCH].Jump = dopunchjump;

	DoControls[DO_KICK].Frame = dokick;
	DoControls[DO_KICK].Count = dokickframe;
	DoControls[DO_KICK].Jump = dokickjump;

	DoControls[DO_PUNCH_HIT1].Frame = dopunchhit1;
	DoControls[DO_PUNCH_HIT1].Count = dopunchhit1frame;
	DoControls[DO_PUNCH_HIT1].Jump = dopunchhit1jump;

	DoControls[DO_PUNCH_HIT2].Frame = dopunchhit2;
	DoControls[DO_PUNCH_HIT2].Count = dopunchhit2frame;
	DoControls[DO_PUNCH_HIT2].Jump = dopunchhit2jump;

	DoControls[DO_PUNCH_DEATH].Frame = dopunchdeath;
	DoControls[DO_PUNCH_DEATH].Count = dopunchdeathframe;
	DoControls[DO_PUNCH_DEATH].Jump = dopunchdeathjump;

	DoControls[DO_KICK_HIT1].Frame = dokickhit1;
	DoControls[DO_KICK_HIT1].Count = dokickhit1frame;
	DoControls[DO_KICK_HIT1].Jump = dokickhit1jump;

	DoControls[DO_KICK_HIT2].Frame = dokickhit2;
	DoControls[DO_KICK_HIT2].Count = dokickhit2frame;
	DoControls[DO_KICK_HIT2].Jump = dokickhit2jump;

	DoControls[DO_KICK_DEATH].Frame = dokickdeath;
	DoControls[DO_KICK_DEATH].Count = dokickdeathframe;
	DoControls[DO_KICK_DEATH].Jump = dokickdeathjump;

	DoControls[DO_READY_WEAPON].Frame = doreadyweapon;
	DoControls[DO_READY_WEAPON].Count = doreadyweaponframe;
	DoControls[DO_READY_WEAPON].Jump = doreadyweaponjump;

	DoControls[DO_GUN_DEATH].Frame = dogundeath;
	DoControls[DO_GUN_DEATH].Count = dogundeathframe;
	DoControls[DO_GUN_DEATH].Jump = dogundeathjump;

	DoControls[DO_EXPLOSION_DEATH].Frame = doexplosiondeath;
	DoControls[DO_EXPLOSION_DEATH].Count = doexplosiondeathframe;
	DoControls[DO_EXPLOSION_DEATH].Jump = doexplosiondeathjump;

	DoControls[DO_EXPLOSION2_DEATH].Frame = doexplosion2death;
	DoControls[DO_EXPLOSION2_DEATH].Count = doexplosion2deathframe;
	DoControls[DO_EXPLOSION2_DEATH].Jump = doexplosion2deathjump;

	DoControls[DO_GRENADE_DEATH].Frame = dogrenadedeath;
	DoControls[DO_GRENADE_DEATH].Count = dogrenadedeathframe;
	DoControls[DO_GRENADE_DEATH].Jump = dogrenadedeathjump;

	DoControls[DO_FIRE_DEATH].Frame = dofiredeath;
	DoControls[DO_FIRE_DEATH].Count = dofiredeathframe;
	DoControls[DO_FIRE_DEATH].Jump = dofiredeathjump;

	DoControls[DO_GESTURE1].Frame = dogesture1;
	DoControls[DO_GESTURE1].Count = dogesture1frame;
	DoControls[DO_GESTURE1].Jump = dogesture1jump;

	DoControls[DO_SALUTE1].Frame = dosalute1;
	DoControls[DO_SALUTE1].Count = dosalute1frame;
	DoControls[DO_SALUTE1].Jump = dosalute1jump;

	DoControls[DO_GESTURE2].Frame = dogesture2;
	DoControls[DO_GESTURE2].Count = dogesture2frame;
	DoControls[DO_GESTURE2].Jump = dogesture2jump;

	DoControls[DO_SALUTE2].Frame = dosalute2;
	DoControls[DO_SALUTE2].Count = dosalute2frame;
	DoControls[DO_SALUTE2].Jump = dosalute2jump;

	DoControls[DO_PULL_GUN].Frame = dopullgun;
	DoControls[DO_PULL_GUN].Count = dopullgunframe;
	DoControls[DO_PULL_GUN].Jump = dopullgunjump;

	DoControls[DO_PLEAD].Frame = doplead;
	DoControls[DO_PLEAD].Count = dopleadframe;
	DoControls[DO_PLEAD].Jump = dopleadjump;

	DoControls[DO_PLEAD_DEATH].Frame = dopleaddeath;
	DoControls[DO_PLEAD_DEATH].Count = dopleaddeathframe;
	DoControls[DO_PLEAD_DEATH].Jump = dopleaddeathjump;
#endif	//cuts
}


/***********************************************************************************************
 * InfantryTypeClass::Create_One_Of -- Creates an infantry object.                             *
 *                                                                                             *
 *    This creates an infantry object, but does not attempt to place it on the map. It is      *
 *    typically used by the scenario editor when an object is needed, but the location has     *
 *    not yet been specified for where it should appear on the map.                            *
 *                                                                                             *
 * INPUT:   house -- The owner of the infantry object.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the created infantry object. If an object could not be   *
 *          created, then NULL is returned.                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * InfantryTypeClass::Create_One_Of(HouseClass * house) const
{
	return(new InfantryClass(Type, house->Class->House));
}


/***********************************************************************************************
 * InfantryTypeClass::Create_And_Place -- Creates and places infantry object onto the map.     *
 *                                                                                             *
 *    This routine is used by the scenario editor to create and place an infantry object onto  *
 *    the map at the location specified.                                                       *
 *                                                                                             *
 * INPUT:   cell     -- The cell location to place the infantry object at.                     *
 *                                                                                             *
 *          house    -- The owner of the infantry object.                                      *
 *                                                                                             *
 * OUTPUT:  bool; Was the infantry object successfully created and placed at the location      *
 *                specified?                                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryTypeClass::Create_And_Place(CELL cell, HousesType house) const
{
	InfantryClass * i = new InfantryClass(Type, house);
	if (i) {
		COORDINATE coord = Map[cell].Closest_Free_Spot(Cell_Coord(cell));
		if (coord) {
			return(i->Unlimbo(coord, DIR_E));
		} else {
			delete i;
		}
	}
	return(false);
}


/***********************************************************************************************
 * InfantryTypeClass::Occupy_List -- Returns with default infantry occupation list.            *
 *                                                                                             *
 *    This routine will return with a cell offset occupation list for a generic infantry       *
 *    object. This is typically just a single cell since infantry are never bigger than one    *
 *    cell and this routine presumes the infantry is located in the center of the cell.        *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a cell offset list for the infantry object as if it were located      *
 *          in the center of a cell.                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * InfantryTypeClass::Occupy_List(bool) const
{
	static short const _list[] = {0, REFRESH_EOL};

	return(&_list[0]);
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * InfantryTypeClass::Display -- Displays a generic infantry object.                           *
 *                                                                                             *
 *    This routine is used by the scenario editor to display a generic representation of the   *
 *    infantry object for the scenario editor. It simply draws a single (nice profile) view    *
 *    of the infantry type.                                                                    *
 *                                                                                             *
 * INPUT:   x,y      -- The display coordinates to render the infantry object at.              *
 *                                                                                             *
 *          window   -- The window that the display coordinates are relative to.               *
 *                                                                                             *
 *          house    -- The house colors to use when rendering this infantry object.           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryTypeClass::Display(int x, int y, WindowNumberType window, HousesType house) const
{
	if (house != HOUSE_NONE) {

		int shape = 0;
		void const * ptr = Get_Cameo_Data();
		if (!ptr) {
			ptr = Get_Image_Data();
			shape = 2;
		}

		CC_Draw_Shape(ptr, shape, x, y, window, SHAPE_NORMAL|SHAPE_CENTER|SHAPE_WIN_REL);
	}
}


/***********************************************************************************************
 * InfantryTypeClass::Prep_For_Add -- Prepares the scenario editor for adding of infantry object.*
 *                                                                                             *
 *    This routine will prepare the scenario editor so that the infantry objects appear on     *
 *    the object list.                                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryTypeClass::Prep_For_Add(void)
{
	for (InfantryType index = INFANTRY_FIRST; index < INFANTRY_COUNT; index++) {
		Map.Add_To_List(&As_Reference(index));
	}
}
#endif


/***********************************************************************************************
 * InfantryTypeClass::From_Name -- Converts an ASCII name into an infantry type number.        *
 *                                                                                             *
 *    This routine is used to convert the infantry ASCII name as specified into an infantry    *
 *    type number. This is called from the INI reader routine in the process if creating the   *
 *    infantry objects needed for the scenario.                                                *
 *                                                                                             *
 * INPUT:   name  -- The ASCII name to convert into an infantry type number.                   *
 *                                                                                             *
 * OUTPUT:  Returns with the infantry type number that corresponds to the infantry ASCII name  *
 *          specified. If no match could be found, then INFANTRY_NONE is returned.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryType InfantryTypeClass::From_Name(char const *name)
{
	if (name) {
		for (InfantryType classid = INFANTRY_FIRST; classid < INFANTRY_COUNT; classid++) {
			if (stricmp(Pointers[classid]->IniName, name) == 0) {
				return(classid);
			}
		}
	}
	return(INFANTRY_NONE);
}


/***********************************************************************************************
 * InfantryTypeClass::One_Time -- Performs any one time processing for infantry system.        *
 *                                                                                             *
 *    This routine will perform one time processing for the infantry type system. This is      *
 *    generally restricted to loading of the infantry shape data.                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryTypeClass::One_Time(void)
{
	InfantryType index;

	for (index = INFANTRY_FIRST; index < INFANTRY_COUNT; index++) {
		char fullname[_MAX_FNAME+_MAX_EXT];
		InfantryTypeClass	const *uclass;
		CCFileClass	file;

		uclass = &As_Reference(index);

		/*
		**	Generic shape for all houses load method.
		*/
		_makepath(fullname, NULL, NULL, uclass->IniName, ".SHP");
		((void const *&)uclass->ImageData) = MixFileClass::Retrieve(fullname);

		/*
		**	The small build image icon sized shapes are always generic.
		*/
		char buffer[_MAX_FNAME];
		if ( Get_Resolution_Factor() ) {
			sprintf(buffer, "%.4sICNH", uclass->IniName);
		} else {
			sprintf(buffer, "%.4sICON", uclass->IniName);
		}
		_makepath(fullname, NULL, NULL, buffer, ".SHP");
		((void const *&)uclass->CameoData) = MixFileClass::Retrieve(fullname);
	}
}






/***********************************************************************************************
 * ITC::Init -- load up terrain set dependant sidebar icons                                    *
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
 *    4/25/96 0:33AM ST : Created                                                              *
 *=============================================================================================*/

void InfantryTypeClass::Init(TheaterType theater)
{
	if ( Get_Resolution_Factor() ) {

		if (theater != LastTheater){
			InfantryType index;
			char buffer[_MAX_FNAME];
			char fullname[_MAX_FNAME+_MAX_EXT];
			void const * cameo_ptr;

			for (index = INFANTRY_FIRST; index < INFANTRY_COUNT; index++) {
				InfantryTypeClass	const *uclass;
				CCFileClass	file;

				uclass = &As_Reference(index);

				((void const *&)uclass->CameoData) = NULL;

				sprintf(buffer, "%.4sICNH", uclass->IniName);
				_makepath (fullname, NULL, NULL, buffer, Theaters[theater].Suffix);
				cameo_ptr = MixFileClass::Retrieve(fullname);
				if (cameo_ptr){
					((void const *&)uclass->CameoData) = cameo_ptr;
				}
			}
		}
	}
}









/***********************************************************************************************
 * InfantryTypeClass::Who_Can_Build_Me -- Determines what can build the infantry object.       *
 *                                                                                             *
 *    Use this routine to determine what building can produce the infantry object. This is     *
 *    typically used to maintain the construction list sidebar.                                *
 *                                                                                             *
 * INPUT:   intheory -- If no regard is to be given to whether the construction building is    *
 *                      currently busy, then this flag should be true. In such a case, only    *
 *                      the existance of the building is sufficient to achieve success.        *
 *                                                                                             *
 *          legal    -- Should building prerequisite legality checks be performed as well?     *
 *                      For building placements, this is usually false. For sidebar button     *
 *                      adding, this is usually true.                                          *
 *                                                                                             *
 *          house    -- The house of the infantry to be produced. Only construction buildings  *
 *                      of the same house are considered.                                      *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the object (building) that can produce the infantry      *
 *          type. If there are no available buildings then the return value is NULL.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingClass * InfantryTypeClass::Who_Can_Build_Me(bool intheory, bool legal, HousesType house) const
{
	BuildingClass * anybuilding = NULL;

	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass * building = Buildings.Ptr(index);

		if (building &&
			!building->IsInLimbo &&
			building->House->Class->House == house &&
			building->Mission != MISSION_DECONSTRUCTION &&
			building->Class->ToBuild == RTTI_INFANTRYTYPE &&
			((1L << building->ActLike) & Ownable) &&
			(!legal || building->House->Can_Build(Type, building->ActLike)) &&
			(intheory || !building->In_Radio_Contact())) {

			anybuilding = building;
			if (building->IsLeader) return(building);
		}
	}
	return(anybuilding);
}


/***********************************************************************************************
 * InfantryTypeClass::Full_Name -- Fetches the full name text number.                          *
 *                                                                                             *
 *    This routine will fetch the full name text number for this infantry type. It examines    *
 *    the special custom name flag to determine whether the custom name or the generic name    *
 *    is to be used.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with text number for the name to give this infantry type object.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int InfantryTypeClass::Full_Name(void) const
{
	if (Debug_Map || !IsNominal || Special.IsNamed || Type == INFANTRY_C10 || Type == INFANTRY_DELPHI || Type == INFANTRY_MOEBIUS) {
		return(TechnoTypeClass::Full_Name());
	}
	return(TXT_CIVILIAN);
}