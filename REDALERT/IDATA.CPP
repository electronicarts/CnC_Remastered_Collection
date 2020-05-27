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

/* $Header: /CounterStrike/IDATA.CPP 3     3/16/97 10:16p Joe_b $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
 *                  Last Update : July 19, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   InfantryTypeClass::As_Reference -- Fetches a reference to the infantry type specified.    *
 *   InfantryTypeClass::Create_And_Place -- Creates and places infantry object onto the map.   *
 *   InfantryTypeClass::Create_One_Of -- Creates an infantry object.                           *
 *   InfantryTypeClass::Display -- Displays a generic infantry object.                         *
 *   InfantryTypeClass::From_Name -- Converts an ASCII name into an infantry type number.      *
 *   InfantryTypeClass::Full_Name -- Fetches the full name text number.                        *
 *   InfantryTypeClass::Get_Cameo_Data -- Fetches the small cameo shape for sidebar strip.     *
 *   InfantryTypeClass::InfantryTypeClass -- Constructor for infantry type class objects.      *
 *   InfantryTypeClass::Init_Heap -- Initialize the infantry type class heap.                  *
 *   InfantryTypeClass::Occupy_List -- Returns with default infantry occupation list.          *
 *   InfantryTypeClass::One_Time -- Performs any one time processing for infantry system.      *
 *   InfantryTypeClass::Prep_For_Add -- Prepares the scenario editor for adding of infantry obj*
 *   InfantryTypeClass::Read_INI -- Fetches infantry override values from the INI database.    *
 *   InfantryTypeClass::operator delete -- Frees an infantry type class object.                *
 *   InfantryTypeClass::operator new -- Allocate an infanty type class object.                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"type.h"

static DoInfoStruct DogDoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{0,	1,	1},					// DO_STAND_GUARD
	{0,	1,	1},					// DO_PRONE			// NA
	{8,	6,	6},					// DO_WALK
	{104,	14,14},					// DO_FIRE_WEAPON
	{0,	0,	0},					// DO_LIE_DOWN		// NA
	{56,	6,	6},					// DO_CRAWL
	{0,	0,	0},					// DO_GET_UP
	{104,	14,14},					// DO_FIRE_PRONE
	{216,	18,0},					// DO_IDLE1
	{216,	18,0},					// DO_IDLE2
	{235,	7,	0},					// DO_GUN_DEATH
	{242,	9,	0},					// DO_EXPLOSION_DEATH
	{242,	9,	0},					// DO_EXPLOSION2_DEATH
	{242,	9, 0},					// DO_GRENADE_DEATH
	{251,	14,0},					// DO_FIRE_DEATH
	{0,	1,	0},					// DO_GESTURE1
	{0,	1,	0},					// DO_SALUTE1
	{0,	1,	0},					// DO_GESTURE2
	{0,	1,	0},					// DO_SALUTE2
	{106,	12,14},					//	DO_DOG_MAUL
};


//
// For the virtual do controls, we are using the TD infantry asset animation frame numbers, take out the -94 frame offsets that were added in RA
//

static DoInfoStruct E1DoControlsVirtual[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{8,	1,	1},					// DO_STAND_GUARD
	{192,	1,	8},					// DO_PRONE
	{16,	6,	6},					// DO_WALK
	{64,	8,	8},					// DO_FIRE_WEAPON
	{128,	2,	2},					// DO_LIE_DOWN
	{144,	4,	4},					// DO_CRAWL
	{176,	2,	2},					// DO_GET_UP
	{192,	6,	8},					// DO_FIRE_PRONE
	{256,	16,0},					// DO_IDLE1
	{272,	16,0},					// DO_IDLE2
	{382,	8,	0},					// DO_GUN_DEATH
	{398,	8,	0},					// DO_EXPLOSION_DEATH
	{398,	8,	0},					// DO_EXPLOSION2_DEATH
	{406,	12,0},					// DO_GRENADE_DEATH
	{418,	18,0},					// DO_FIRE_DEATH
	{436,	3,	3},					// DO_GESTURE1
	{460,	3,	3},					// DO_SALUTE1
	{484,	3,	3},					// DO_GESTURE2
	{508,	3,	3},					// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct E2DoControlsVirtual[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{8,	1,	1},					// DO_STAND_GUARD
	{288,	1,	12},					// DO_PRONE
	{16,	6,	6},					// DO_WALK
	{64,	20,20},					// DO_FIRE_WEAPON
	{224,	2,	2},					// DO_LIE_DOWN
	{240,	4,	4},					// DO_CRAWL
	{272,	2,	2},					// DO_GET_UP
	{288,	8,	12},					// DO_FIRE_PRONE
	{384,	16,0},					// DO_IDLE1
	{400,	16,0},					// DO_IDLE2
	{510,	8,	0},					// DO_GUN_DEATH
	{526,	8,	0},					// DO_EXPLOSION_DEATH
	{526,	8,	0},					// DO_EXPLOSION2_DEATH
	{534,	12,0},					// DO_GRENADE_DEATH
	{546,	18,0},					// DO_FIRE_DEATH
	{564,	3,	3},					// DO_GESTURE1
	{588,	3,	3},					// DO_SALUTE1
	{612,	3,	3},					// DO_GESTURE2
	{636,	3,	3},					// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct E3DoControlsVirtual[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{8,	1,	1},					// DO_STAND_GUARD
	{192,	1,10},					// DO_PRONE
	{16,	6,	6},					// DO_WALK
	{64,	8,	8},					// DO_FIRE_WEAPON
	{128,	2,	2},					// DO_LIE_DOWN
	{144,	4,	4},					// DO_CRAWL
	{176,	2,	2},					// DO_GET_UP
	{192,	10,10},					// DO_FIRE_PRONE
	{272,	16,0},					// DO_IDLE1
	{288,	16,0},					// DO_IDLE2
	{398,	8,	0},					// DO_GUN_DEATH
	{414,	8,	0},					// DO_EXPLOSION_DEATH
	{414,	8,	0},					// DO_EXPLOSION2_DEATH
	{422,	12,0},					// DO_GRENADE_DEATH
	{434,	18,0},					// DO_FIRE_DEATH
	{452,	3,	3},					// DO_GESTURE1
	{476,	3,	3},					// DO_SALUTE1
	{500,	3,	3},					// DO_GESTURE2
	{524,	3,	3},					// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct E4DoControlsVirtual[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{8,	1,	1},					// DO_STAND_GUARD
	{256,	1,16},					// DO_PRONE
	{16,	6,	6},					// DO_WALK
	{64,	16,16},					// DO_FIRE_WEAPON
	{192,	2,	2},					// DO_LIE_DOWN
	{208,	4,	4},					// DO_CRAWL
	{240,	2,	2},					// DO_GET_UP
	{256,	16,16},					// DO_FIRE_PRONE
	{384,	16,0},					// DO_IDLE1
	{400,	16,0},					// DO_IDLE2
	{510,	8,	0},					// DO_GUN_DEATH
	{526,	8,	0},					// DO_EXPLOSION_DEATH
	{526,	8,	0},					// DO_EXPLOSION2_DEATH
	{534,	12,0},					// DO_GRENADE_DEATH
	{546,	18,0},					// DO_FIRE_DEATH
	{564,	3,	3},					// DO_GESTURE1
	{588,	3,	3},					// DO_SALUTE1
	{612,	3,	3},					// DO_GESTURE2
	{636,	3,	3},					// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};



static DoInfoStruct E1DoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{8,	1,	1},					// DO_STAND_GUARD
	{192,	1,	8},					// DO_PRONE
	{16,	6,	6},					// DO_WALK
	{64,	8,	8},					// DO_FIRE_WEAPON
	{128,	2,	2},					// DO_LIE_DOWN
	{144,	4,	4},					// DO_CRAWL
	{176,	2,	2},					// DO_GET_UP
	{192,	6,	8},					// DO_FIRE_PRONE
	{256,	16,0},					// DO_IDLE1
	{272,	16,0},					// DO_IDLE2
	{382-94,	8,	0},				// DO_GUN_DEATH
	{398-94,	8,	0},				// DO_EXPLOSION_DEATH
	{398-94,	8,	0},				// DO_EXPLOSION2_DEATH
	{406-94,	12,0},				// DO_GRENADE_DEATH
	{418-94,	18,0},				// DO_FIRE_DEATH
	{436-94,	3,	3},				// DO_GESTURE1
	{460-94,	3,	3},				// DO_SALUTE1
	{484-94,	3,	3},				// DO_GESTURE2
	{508-94,	3,	3},				// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct E2DoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{8,	1,	1},					// DO_STAND_GUARD
	{288,	1,	12},					// DO_PRONE
	{16,	6,	6},					// DO_WALK
	{64,	20,20},					// DO_FIRE_WEAPON
	{224,	2,	2},					// DO_LIE_DOWN
	{240,	4,	4},					// DO_CRAWL
	{272,	2,	2},					// DO_GET_UP
	{288,	8,	12},					// DO_FIRE_PRONE
	{384,	16,0},					// DO_IDLE1
	{400,	16,0},					// DO_IDLE2
	{510-94,	8,	0},				// DO_GUN_DEATH
	{526-94,	8,	0},				// DO_EXPLOSION_DEATH
	{526-94,	8,	0},				// DO_EXPLOSION2_DEATH
	{534-94,	12,0},				// DO_GRENADE_DEATH
	{546-94,	18,0},				// DO_FIRE_DEATH
	{564-94,	3,	3},				// DO_GESTURE1
	{588-94,	3,	3},				// DO_SALUTE1
	{612-94,	3,	3},				// DO_GESTURE2
	{636-94,	3,	3},				// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct E3DoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{8,	1,	1},					// DO_STAND_GUARD
	{192,	1,10},					// DO_PRONE
	{16,	6,	6},					// DO_WALK
	{64,	8,	8},					// DO_FIRE_WEAPON
	{128,	2,	2},					// DO_LIE_DOWN
	{144,	4,	4},					// DO_CRAWL
	{176,	2,	2},					// DO_GET_UP
	{192,	10,10},					// DO_FIRE_PRONE
	{272,	16,0},					// DO_IDLE1
	{288,	16,0},					// DO_IDLE2
	{398-94,	8,	0},				// DO_GUN_DEATH
	{414-94,	8,	0},				// DO_EXPLOSION_DEATH
	{414-94,	8,	0},				// DO_EXPLOSION2_DEATH
	{422-94,	12,0},				// DO_GRENADE_DEATH
	{434-94,	18,0},				// DO_FIRE_DEATH
	{452-94,	3,	3},				// DO_GESTURE1
	{476-94,	3,	3},				// DO_SALUTE1
	{500-94,	3,	3},				// DO_GESTURE2
	{524-94,	3,	3},				// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct E4DoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{8,	1,	1},					// DO_STAND_GUARD
	{256,	1,16},					// DO_PRONE
	{16,	6,	6},					// DO_WALK
	{64,	16,16},					// DO_FIRE_WEAPON
	{192,	2,	2},					// DO_LIE_DOWN
	{208,	4,	4},					// DO_CRAWL
	{240,	2,	2},					// DO_GET_UP
	{256,	16,16},					// DO_FIRE_PRONE
	{384,	16,0},					// DO_IDLE1
	{400,	16,0},					// DO_IDLE2
	{510-94,	8,	0},				// DO_GUN_DEATH
	{526-94,	8,	0},				// DO_EXPLOSION_DEATH
	{526-94,	8,	0},				// DO_EXPLOSION2_DEATH
	{534-94,	12,0},				// DO_GRENADE_DEATH
	{546-94,	18,0},				// DO_FIRE_DEATH
	{564-94,	3,	3},				// DO_GESTURE1
	{588-94,	3,	3},				// DO_SALUTE1
	{612-94,	3,	3},				// DO_GESTURE2
	{636-94,	3,	3},				// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};


static DoInfoStruct E6DoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{8,	1,	1},					// DO_STAND_GUARD
	{82,	1,	4},					// DO_PRONE
	{16,	6,	6},					// DO_WALK
	{0,	0,	0},					// DO_FIRE_WEAPON
	{67,	2,	2},					// DO_LIE_DOWN
	{82,	4,	4},					// DO_CRAWL
	{114,	2,	2},					// DO_GET_UP
	{0,	0,	0},					// DO_FIRE_PRONE
	{130,	16,0},					// DO_IDLE1
	{130,	16,0},					// DO_IDLE2
	{146,	8,	0},					// DO_GUN_DEATH
	{154,	8,	0},					// DO_EXPLOSION_DEATH
	{162,	8,	0},					// DO_EXPLOSION2_DEATH
	{170,	12,0},					// DO_GRENADE_DEATH
	{182,	18,0},					// DO_FIRE_DEATH
	{200,	3,	3},					// DO_GESTURE1
	{224,	3,	3},					// DO_SALUTE1
	{200,	3,	3},					// DO_GESTURE2
	{224,	3,	3},					// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct E7DoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{0,	1,	1},					// DO_STAND_GUARD
	{128,	1,	4},					// DO_PRONE
	{8,	6,	6},					// DO_WALK
	{56,	7,	7},					// DO_FIRE_WEAPON
	{113,	2,	2},					// DO_LIE_DOWN
	{128,	4,	4},					// DO_CRAWL
	{161,	2,	2},					// DO_GET_UP
	{176,	7,	7},					// DO_FIRE_PRONE
	{232,	17,0},					// DO_IDLE1
	{249,	13,0},					// DO_IDLE2
	{262,	8,	0},					// DO_GUN_DEATH
	{270,	8,	0},					// DO_EXPLOSION_DEATH
	{278,	8,	0},					// DO_EXPLOSION2_DEATH
	{286,	12,0},					// DO_GRENADE_DEATH
	{298,	18,0},					// DO_FIRE_DEATH
	{0,	1,	0},					// DO_GESTURE1
	{0,	1,	0},					// DO_SALUTE1
	{0,	1,	0},					// DO_GESTURE2
	{0,	1,	0},					// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

//Spy
static DoInfoStruct SpyDoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{8,	1,	1},					// DO_STAND_GUARD
	{144,	1,	4},					// DO_PRONE
	{16,	6,	6},					// DO_WALK
	{64,	8,	8},					// DO_FIRE_WEAPON
	{128,	2,	2},					// DO_LIE_DOWN
	{144,	4,	4},					// DO_CRAWL
	{176,	2,	2},					// DO_GET_UP
	{192,	8,	8},					// DO_FIRE_PRONE
	{256,	14,0},					// DO_IDLE1
	{270,	18,0},					// DO_IDLE2
	{288,	8,	0},					// DO_GUN_DEATH
	{296,	8,	0},					// DO_EXPLOSION_DEATH
	{304,	8,	0},					// DO_EXPLOSION2_DEATH
	{312,	12,0},					// DO_GRENADE_DEATH
	{324,	18,0},					// DO_FIRE_DEATH
	{0,	1,	0},					// DO_GESTURE1
	{0,	1,	0},					// DO_SALUTE1
	{0,	1,	0},					// DO_GESTURE2
	{0,	1,	0},					// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct E9DoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{8,	1,	1},					// DO_STAND_GUARD
	{72,	1,	4},					// DO_PRONE
	{8,	6,	6},					// DO_WALK
	{0,	0,	0},					// DO_FIRE_WEAPON
	{56,	2,	2},					// DO_LIE_DOWN
	{72,	4,	4},					// DO_CRAWL
	{108,	2,	2},					// DO_GET_UP
	{0,	0,	0},					// DO_FIRE_PRONE
	{120,	19,0},					// DO_IDLE1
	{120,	19,0},					// DO_IDLE2
	{139,	8,	0},					// DO_GUN_DEATH
	{147,	8,	0},					// DO_EXPLOSION_DEATH
	{155,	8,	0},					// DO_EXPLOSION2_DEATH
	{163,	12,0},					// DO_GRENADE_DEATH
	{175,	18,0},					// DO_FIRE_DEATH
	{0,	1,	0},					// DO_GESTURE1
	{0,	1,	0},					// DO_SALUTE1
	{0,	1,	0},					// DO_GESTURE2
	{0,	1,	0},					// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct MedicDoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{0,	1,	1},					// DO_STAND_GUARD
	{130,	1,	4},					// DO_PRONE
	{8,	6,	6},					// DO_WALK
	{56,	28,0},					// DO_FIRE_WEAPON
	{114,	2,	2},					// DO_LIE_DOWN
	{130,	4,	4},					// DO_CRAWL
	{162,	2,	2},					// DO_GET_UP
	{56,	28,0},					// DO_FIRE_PRONE
	{178,	15,0},					// DO_IDLE1
	{178,	15,0},					// DO_IDLE2
	{193,	8,	0},					// DO_GUN_DEATH
	{210,	8,	0},					// DO_EXPLOSION_DEATH
	{202,	8,	0},					// DO_EXPLOSION2_DEATH
	{217,	12,0},					// DO_GRENADE_DEATH
	{229,	18,0},					// DO_FIRE_DEATH
	{0,	1,	0},					// DO_GESTURE1
	{0,	1,	0},					// DO_SALUTE1
	{0,	1,	0},					// DO_GESTURE2
	{0,	1,	0},					// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct GeneralDoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{0,	1,	1},					// DO_STAND_GUARD
	{104,	1,	4},					// DO_PRONE
	{8,	6,	6},					// DO_WALK
	{56,	4,	4},					// DO_FIRE_WEAPON
	{88,	2,	2},					// DO_LIE_DOWN
	{104,	4,	4},					// DO_CRAWL
	{136,	2,	2},					// DO_GET_UP
	{152,	4,	4},					// DO_FIRE_PRONE
	{184,	26,0},					// DO_IDLE1
	{184,	26,0},					// DO_IDLE2
	{210,	8,	0},					// DO_GUN_DEATH
	{226,	8,	0},					// DO_EXPLOSION_DEATH
	{218,	8,	0},					// DO_EXPLOSION2_DEATH
	{234,	12,0},					// DO_GRENADE_DEATH
	{246,	18,0},					// DO_FIRE_DEATH
	{0,	1,	0},					// DO_GESTURE1
	{0,	1,	0},					// DO_SALUTE1
	{0,	1,	0},					// DO_GESTURE2
	{0,	1,	0},					// DO_SALUTE2
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct CivilianDoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{0,	1,	1},					// DO_STAND_GUARD
	{0,	1,	1},					// DO_PRONE				// N/A
	{56, 	6,	6},					// DO_WALK
	{205-85,	4, 4},				// DO_FIRE_WEAPON
	{0, 	1,	1},					// DO_LIE_DOWN			// N/A
	{8, 	6,	6},					// DO_CRAWL
	{0, 	1,	1},					// DO_GET_UP			// N/A
	{205-85,	4,	4},				// DO_FIRE_PRONE
	{189-85,	10,0},				// DO_IDLE1
	{199-85,	6,	0},				// DO_IDLE2
	{152,	8,	0},					// DO_GUN_DEATH
	{160,	8,	0},					// DO_EXPLOSION_DEATH
	{160,	8,	0},					// DO_EXPLOSION2_DEATH
	{168,	12,0},					// DO_GRENADE_DEATH
	{180,	18,0},					// DO_FIRE_DEATH
	{0, 	1,	0},					// DO_GESTURE1			// N/A
	{0, 	1,	0},					// DO_SALUTE1			// N/A
	{0, 	1,	0},					// DO_GESTURE2			// N/A
	{0, 	1,	0},					// DO_SALUTE2			// N/A
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct CivilianDoControlsVirtual[DO_COUNT] = {
	{0,			1,	1},				// DO_STAND_READY
	{0,			1,	1},				// DO_STAND_GUARD
	{0,			1,	1},				// DO_PRONE				// N/A
	{56, 		6,	6},				// DO_WALK
	{205,		4, 4},				// DO_FIRE_WEAPON
	{0, 		1,	1},				// DO_LIE_DOWN			// N/A
	{8, 		6,	6},				// DO_CRAWL
	{0, 		1,	1},				// DO_GET_UP			// N/A
	{205,		4,	4},				// DO_FIRE_PRONE
	{189,		10,0},				// DO_IDLE1
	{199,		6,	0},				// DO_IDLE2
	{329,		8,	0},				// DO_GUN_DEATH
	{337,		8,	0},				// DO_EXPLOSION_DEATH
	{337,		8,	0},				// DO_EXPLOSION2_DEATH
	{345,		12,0},				// DO_GRENADE_DEATH
	{357,		18,0},				// DO_FIRE_DEATH
	{0, 		1,	0},				// DO_GESTURE1			// N/A
	{0, 		1,	0},				// DO_SALUTE1			// N/A
	{0, 		1,	0},				// DO_GESTURE2			// N/A
	{0, 		1,	0},				// DO_SALUTE2			// N/A
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

static DoInfoStruct EinsteinDoControls[DO_COUNT] = {
	{0,	1,	1},					// DO_STAND_READY
	{0,	1,	1},					// DO_STAND_GUARD
	{0,	1,	1},					// DO_PRONE				// N/A
	{56, 	6,	6},					// DO_WALK
	{205-92,	4, 4},				// DO_FIRE_WEAPON
	{0, 	1,	1},					// DO_LIE_DOWN			// N/A
	{8, 	6,	6},					// DO_CRAWL
	{0, 	1,	1},					// DO_GET_UP			// N/A
	{0,	0,	0},					// DO_FIRE_PRONE
	{104,	16,0},					// DO_IDLE1
	{104,	16,0},					// DO_IDLE2
	{212-92,	8,	0},				// DO_GUN_DEATH
	{220-92,	8,	0},				// DO_EXPLOSION_DEATH
	{228-92,	12,0},				// DO_EXPLOSION2_DEATH
	{228-92,	12,0},				// DO_GRENADE_DEATH
	{240-92,	17,0},				// DO_FIRE_DEATH
	{0, 	1,	0},					// DO_GESTURE1			// N/A
	{0, 	1,	0},					// DO_SALUTE1			// N/A
	{0, 	1,	0},					// DO_GESTURE2			// N/A
	{0, 	1,	0},					// DO_SALUTE2			// N/A
	{0,	0, 0},					//	DO_DOG_MAUL			// N/A
};

// Attack dogs
static InfantryTypeClass const Dog(
	INFANTRY_DOG,				// Infantry type number.
	TXT_GUARD_DOG,				// Translate name number for infantry type.
	"DOG",						// INI name for infantry.
	0x0015,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_FULL,					// Transport pip shape/color to use.
	DogDoControls,
	DogDoControls,
	1,								// Frame of projectile launch.
	1,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

// Minigunners
static InfantryTypeClass const E1(
	INFANTRY_E1,				// Infantry type number.
	TXT_E1,						// Translate name number for infantry type.
	"E1",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	true,							// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_FULL,					// Transport pip shape/color to use.
	E1DoControls,
	E1DoControlsVirtual,
	2,								// Frame of projectile launch.
	2,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

// Grenadiers
static InfantryTypeClass const E2(
	INFANTRY_E2,				// Infantry type number.
	TXT_E2,						// Translate name number for infantry type.
	"E2",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	true,							// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_FULL,					// Transport pip shape/color to use.
	E2DoControls,
	E2DoControlsVirtual,
	14,							// Frame of projectile launch.
	6,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

// Bazooka
static InfantryTypeClass const E3(
	INFANTRY_E3,				// Infantry type number.
	TXT_E3,						// Translate name number for infantry type.
	"E3",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	true,							// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_FULL,					// Transport pip shape/color to use.
	E3DoControls,
	E3DoControlsVirtual,
	3,								// Frame of projectile launch.
	3,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

// Flamethrower
static InfantryTypeClass const E4(
	INFANTRY_E4,				// Infantry type number.
	TXT_E4,						// Translate name number for infantry type.
	"E4",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	true,							// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_FULL,					// Transport pip shape/color to use.
	E4DoControls,
	E4DoControlsVirtual,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

// Renovator
static InfantryTypeClass const E6(
	INFANTRY_RENOVATOR,				// Infantry type number.
	TXT_E6,						// Translate name number for infantry type.
	"E6",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_ENGINEER,				// Transport pip shape/color to use.
	E6DoControls,
	E6DoControls,
	3,								// Frame of projectile launch.
	3,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

// Spy
static InfantryTypeClass const E8(
	INFANTRY_SPY,				// Infantry type number.
	TXT_E8,						// Translate name number for infantry type.
	"SPY",						// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_ENGINEER,				// Transport pip shape/color to use.
	SpyDoControls,
	SpyDoControls,
	3,								// Frame of projectile launch.
	3,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

// Thief
static InfantryTypeClass const E9(
	INFANTRY_THIEF,				// Infantry type number.
	TXT_THIEF,					// Translate name number for infantry type.
	"THF",						// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_ENGINEER,				// Transport pip shape/color to use.
	E9DoControls,
	E9DoControls,
	3,								// Frame of projectile launch.
	3,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

// Tanya
static InfantryTypeClass const E7(
	INFANTRY_TANYA,			// Infantry type number.
	TXT_E7,						// Translate name number for infantry type.
	"E7",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	true,							// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_COMMANDO,				// Transport pip shape/color to use.
	E7DoControls,
	E7DoControls,
	2,								// Frame of projectile launch.
	2,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

static InfantryTypeClass const Medic(
	INFANTRY_MEDIC,			// Infantry type number.
	TXT_MEDIC,					// Translate name number for infantry type.
	"MEDI",						// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	true,							// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_ENGINEER,				// Transport pip shape/color to use.
	MedicDoControls,
	MedicDoControls,
	25,							// Frame of projectile launch.
	25,							// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

static InfantryTypeClass const General(
	INFANTRY_GENERAL,			// Infantry type number.
	TXT_GENERAL,				// Translate name number for infantry type.
	"GNRL",						// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	true,							// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_ENGINEER,				// Transport pip shape/color to use.
	GeneralDoControls,
	GeneralDoControls,
	2,								// Frame of projectile launch.
	2,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

// Civilians
static InfantryTypeClass const C1(
	INFANTRY_C1,				// Infantry type number.
	TXT_C1,						// Translate name number for infantry type.
	"C1",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	false,						// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_CIVILIAN,				// Transport pip shape/color to use.
	CivilianDoControls,
	CivilianDoControlsVirtual,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

static InfantryTypeClass const C2(
	INFANTRY_C2,				// Infantry type number.
	TXT_C2,						// Translate name number for infantry type.
	"C2",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	true,							// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_CIVILIAN,				// Transport pip shape/color to use.
	CivilianDoControls,
	CivilianDoControlsVirtual,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	RemapCiv2						// pointer to override remap table
);

static InfantryTypeClass const C3(
	INFANTRY_C3,				// Infantry type number.
	TXT_C3,						// Translate name number for infantry type.
	"C3",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	true,							// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	false,						// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_CIVILIAN,				// Transport pip shape/color to use.
	CivilianDoControls,
	CivilianDoControlsVirtual,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

static InfantryTypeClass const C4(
	INFANTRY_C4,				// Infantry type number.
	TXT_C4,						// Translate name number for infantry type.
	"C4",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	true,							// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	true,							// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_CIVILIAN,				// Transport pip shape/color to use.
	CivilianDoControls,
	CivilianDoControlsVirtual,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	RemapCiv4						// pointer to override remap table
);

static InfantryTypeClass const C5(
	INFANTRY_C5,				// Infantry type number.
	TXT_C5,						// Translate name number for infantry type.
	"C5",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	true,							// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_CIVILIAN,				// Transport pip shape/color to use.
	CivilianDoControls,
	CivilianDoControlsVirtual,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	RemapCiv5						// pointer to override remap table
);

static InfantryTypeClass const C6(
	INFANTRY_C6,				// Infantry type number.
	TXT_C6,						// Translate name number for infantry type.
	"C6",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	true,							// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_CIVILIAN,				// Transport pip shape/color to use.
	CivilianDoControls,
	CivilianDoControlsVirtual,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	RemapCiv6						// pointer to override remap table
);

static InfantryTypeClass const C7(
	INFANTRY_C7,				// Infantry type number.
	TXT_C7,						// Translate name number for infantry type.
	"C7",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	true,							// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_CIVILIAN,				// Transport pip shape/color to use.
	CivilianDoControls,
	CivilianDoControlsVirtual,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	RemapCiv7						// pointer to override remap table
);

static InfantryTypeClass const C8(
	INFANTRY_C8,				// Infantry type number.
	TXT_C8,						// Translate name number for infantry type.
	"C8",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	true,							// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_CIVILIAN,				// Transport pip shape/color to use.
	CivilianDoControls,
	CivilianDoControlsVirtual,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	RemapCiv8						// pointer to override remap table
);

static InfantryTypeClass const C9(
	INFANTRY_C9,				// Infantry type number.
	TXT_C9,						// Translate name number for infantry type.
	"C9",							// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	true,							// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_CIVILIAN,				// Transport pip shape/color to use.
	CivilianDoControls,
	CivilianDoControlsVirtual,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	RemapCiv9						// pointer to override remap table
);

// Nikoomba
static InfantryTypeClass const C10(
	INFANTRY_C10,				// Infantry type number.
	TXT_C10,						// Translate name number for infantry type.
	"C10",						// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	true,							// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_ENGINEER,				// Transport pip shape/color to use.
	CivilianDoControls,
	CivilianDoControlsVirtual,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	RemapCiv10						// pointer to override remap table
);

static InfantryTypeClass const Einstein(
	INFANTRY_EINSTEIN,		// Infantry type number.
	TXT_EINSTEIN,				// Translate name number for infantry type.
	"EINSTEIN",					// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	false,						// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_ENGINEER,				// Transport pip shape/color to use.
	EinsteinDoControls,
	EinsteinDoControls,
	0,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

static InfantryTypeClass const Delphi(
	INFANTRY_DELPHI,			// Infantry type number.
	TXT_DELPHI,					// Translate name number for infantry type.
	"DELPHI",					// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	false,						// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_ENGINEER,				// Transport pip shape/color to use.
	CivilianDoControls,
	CivilianDoControlsVirtual,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

static InfantryTypeClass const DrChan(
	INFANTRY_CHAN,				// Infantry type number.
	TXT_CHAN,					// Translate name number for infantry type.
	"CHAN",						// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	false,						// Has crawling animation frames?
	true,							// Is this a civilian?
	false,						// Does this unit use the override remap table?
	true,							// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_ENGINEER,				// Transport pip shape/color to use.
	EinsteinDoControls,
	EinsteinDoControls,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	0								// pointer to override remap table
);

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98

// Shock Trooper
static InfantryTypeClass const ShockTrooper(
	INFANTRY_SHOCK,			// Infantry type number.
	TXT_SHOCKTROOPER,			// Translate name number for infantry type.
	"SHOK",						// INI name for infantry.
	-0x0010,					//	Vertical offset.
	0x0038,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	true,							// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_FULL,					// Transport pip shape/color to use.
	E4DoControls,
	E4DoControls,
	2,								// Frame of projectile launch.
	0,								// Frame of projectile launch while prone.
	0,								// pointer to override remap table
	0x0018						//	Horizontal offset.
);


static InfantryTypeClass const Mechanic(
	INFANTRY_MECHANIC,		// Infantry type number.
	TXT_MECHANIC,					// Translate name number for infantry type.
	"MECH",						// INI name for infantry.
	0x0035,						//	Vertical offset.
	0x0010,						// Primary weapon offset along turret centerline.
	false,						// Is this a female type?
	true,							// Has crawling animation frames?
	false,						// Is this a civilian?
	false,						// Does this unit use the override remap table?
	false,						// Always use the given name for the infantry?
	false,						// Theater specific graphic image?
	PIP_ENGINEER,				// Transport pip shape/color to use.
	MedicDoControls,
	MedicDoControls,
	25,							// Frame of projectile launch.
	25,							// Frame of projectile launch while prone.
	0								// pointer to override remap table
);
#endif

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
 *   02/16/1996 JLB : Greatly simplified.                                                      *
 *=============================================================================================*/
InfantryTypeClass::InfantryTypeClass (
						InfantryType type,
						int name,
						char const * ininame,
						int verticaloffset,
						int primaryoffset,
						bool is_female,
						bool is_crawling,
						bool is_civilian,
						bool is_remap_override,
						bool is_nominal,
						bool is_theater,
						PipEnum pip,
						DoInfoStruct const * control,
						DoInfoStruct const * virtual_control,
						int firelaunch,
						int pronelaunch,
						unsigned char const * override_remap,
						int horizontaloffset)
	: TechnoTypeClass(RTTI_INFANTRYTYPE,
							int(type),
							name,
							ininame,
							REMAP_NORMAL,
							verticaloffset,
							primaryoffset,
							0x0000,
							0x0000,
							0x0000,
							is_nominal,
							true,
							true,
							true,
							false,
							false,
							is_theater,
							false,
							true,
							true,
							8,
							SPEED_FOOT,
							horizontaloffset),
	IsFemale(is_female),
	IsCrawling(is_crawling),
	IsCapture(false),
	IsFraidyCat(false),
	IsCivilian(is_civilian),
	IsBomber(false),
	IsDog(false),
	IsRemapOverride(is_remap_override),
	Type(type),
	Pip(pip),
	DoControls(control),
	DoControlsVirtual(virtual_control),
	FireLaunch(firelaunch),
	ProneLaunch(pronelaunch),
	OverrideRemap(override_remap)
{
	/*
	**	Forced infantry overrides from the default.
	*/
	IsCrushable = true;
	IsScanner = true;
	IsRepairable = false;
	IsCrew = false;
	Speed = SPEED_FOOT;
}


/***********************************************************************************************
 * InfantryTypeClass::operator new -- Allocate an infanty type class object.                   *
 *                                                                                             *
 *    This will allocate an infantry type class object from the special memory pool of that    *
 *    purpose.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the infantry type class object allocated. If there was   *
 *          insufficient memory to fulfill the request, then NULL is returned.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * InfantryTypeClass::operator new(size_t)
{
	return(InfantryTypes.Alloc());
}


/***********************************************************************************************
 * InfantryTypeClass::operator delete -- Frees an infantry type class object.                  *
 *                                                                                             *
 *    This will return a previously allocated infantry type class object back to the memory    *
 *    pool from whence it came.                                                                *
 *                                                                                             *
 * INPUT:   pointer  -- The pointer to the infantry type class object to free.                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryTypeClass::operator delete(void * pointer)
{
	InfantryTypes.Free((InfantryTypeClass *)pointer);
}


/***********************************************************************************************
 * InfantryTypeClass::Init_Heap -- Initialize the infantry type class heap.                    *
 *                                                                                             *
 *    This will pre-allocate all known infantry types.                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Call this routine once and before the rules.ini file is processed.              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryTypeClass::Init_Heap(void)
{
	/*
	**	These infantry type class objects must be allocated in the exact order that they
	**	are specified in the InfantryType enumeration. This is necessary because the heap
	**	allocation block index serves double duty as the type number index.
	*/
	new InfantryTypeClass(E1);
	new InfantryTypeClass(E2);
	new InfantryTypeClass(E3);
	new InfantryTypeClass(E4);
	new InfantryTypeClass(E6);
	new InfantryTypeClass(E7);
	new InfantryTypeClass(E8);
	new InfantryTypeClass(E9);
	new InfantryTypeClass(Medic);
	new InfantryTypeClass(General);
	new InfantryTypeClass(Dog);
	new InfantryTypeClass(C1);
	new InfantryTypeClass(C2);
	new InfantryTypeClass(C3);
	new InfantryTypeClass(C4);
	new InfantryTypeClass(C5);
	new InfantryTypeClass(C6);
	new InfantryTypeClass(C7);
	new InfantryTypeClass(C8);
	new InfantryTypeClass(C9);
	new InfantryTypeClass(C10);
	new InfantryTypeClass(Einstein);
	new InfantryTypeClass(Delphi);
	new InfantryTypeClass(DrChan);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	new InfantryTypeClass(ShockTrooper);
	new InfantryTypeClass(Mechanic);
#endif
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
	if (i != NULL) {
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
		if (ptr == NULL) {
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
InfantryType InfantryTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (InfantryType classid = INFANTRY_FIRST; classid < INFANTRY_COUNT; classid++) {
			if (stricmp(As_Reference(classid).IniName, name) == 0) {
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
	for (InfantryType index = INFANTRY_FIRST; index < INFANTRY_COUNT; index++) {
		char fullname[_MAX_FNAME+_MAX_EXT];
		InfantryTypeClass	const * uclass;
		CCFileClass	file;

		uclass = &As_Reference(index);

		/*
		**	Generic shape for all houses load method.
		*/
		_makepath(fullname, NULL, NULL, uclass->Graphic_Name(), ".SHP");

		#ifndef NDEBUG
			RawFileClass sfile(fullname);
			if (sfile.Is_Available()) {
				((void const *&)uclass->ImageData) = Load_Alloc_Data(sfile);
			} else {
				((void const *&)uclass->ImageData) = MFCD::Retrieve(fullname);
			}
		#else
			((void const *&)uclass->ImageData) = MFCD::Retrieve(fullname);
		#endif

		/*
		**	The small build image icon sized shapes are always generic.
		*/
		char buffer[_MAX_FNAME];
		sprintf(buffer, "%.4sICON", uclass->Graphic_Name());
		_makepath(fullname, NULL, NULL, buffer, ".SHP");

		#ifndef NDEBUG
			RawFileClass ifile(fullname);
			if (ifile.Is_Available()) {
				((void const *&)uclass->CameoData) = Load_Alloc_Data(ifile);
			} else {
				((void const *&)uclass->CameoData) = MFCD::Retrieve(fullname);
			}
		#else
			((void const *&)uclass->CameoData) = MFCD::Retrieve(fullname);
		#endif
	}
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
	if (Debug_Map || !IsNominal || Rule.IsNamed || Type == INFANTRY_C10 || Type == INFANTRY_DELPHI || Type == INFANTRY_EINSTEIN) {
		return(TechnoTypeClass::Full_Name());
	}
	return(TXT_CIVILIAN);
}


/***********************************************************************************************
 * InfantryTypeClass::As_Reference -- Fetches a reference to the infantry type specified.      *
 *                                                                                             *
 *    Use this routine to convert an infantry type number into a reference to the infantry     *
 *    type class object it represents.                                                         *
 *                                                                                             *
 * INPUT:   type  -- The infantry type number to convert into a infantry type class object.    *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the infantry type class object specified.              *
 *                                                                                             *
 * WARNINGS:   Be sure that the type parameter is legal, otherwise the results are undefined.  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryTypeClass & InfantryTypeClass::As_Reference(InfantryType type)
{
	return(*InfantryTypes.Ptr(type));
}


/***********************************************************************************************
 * InfantryTypeClass::Read_INI -- Fetches infantry override values from the INI database.      *
 *                                                                                             *
 *    This routine will retrieve the override values for this infantry type class object from  *
 *    the INI database specified.                                                              *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to retrieve the data from.                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the infantry section for this type found and data retrieved from it?     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryTypeClass::Read_INI(CCINIClass & ini)
{
	if (TechnoTypeClass::Read_INI(ini)) {
		IsFraidyCat = ini.Get_Bool(Name(), "Fraidycat", IsFraidyCat);
		IsCapture = ini.Get_Bool(Name(), "Infiltrate", IsCapture);
		IsBomber = ini.Get_Bool(Name(), "C4", IsBomber);
		IsDog = ini.Get_Bool(Name(), "IsCanine", IsDog);
		if (IsBomber) IsCapture = true;
		if (IsDog) IsLeader = false;
		return(true);
	}
	return(false);
}


void InfantryTypeClass::Dimensions(int & width, int & height) const
{
#ifdef WIN32
	width = 14;
	height = 20;
#else
	width = 12;
	height = 16;
#endif
}