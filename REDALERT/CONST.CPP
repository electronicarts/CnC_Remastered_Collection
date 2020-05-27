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

/* $Header: /CounterStrike/CONST.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CONST.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 20, 1993                                           *
 *                                                                                             *
 *                  Last Update : September 20, 1993   [JLB]                                   *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


/***************************************************************************
**	These are the access passwords used to activate cheat mode, editor mode,
**	and special game options.
*/
unsigned long const PlayCodes[] = {
	0xE0792D6D,					// Dwight Okahara
	0x90046ECF,					// Paul S. Mudra
	0xC3EE9A26,					// Frank Klepaki
	0xED382178,					// Ed Del Castillo
	0L
};

unsigned long const CheatCodes[] = {
	0xA0E2AB53,					// Joseph Hewitt
	0x00532693,					// Mike Lightner
	0x7DDFF824,					// Joe Bostic
	0x2CB5CF01,					// Phil Gorrow
	0xB5B63531,					// Bill Randolph
	0xDFABC23A,					// Adam Isgreen
	0x52B19A22,					// Erik Yeo
	0xBE79088C,					// David Dettmer
	0xB216AE7E,					// Barry Green
	0x0E07B213,					// Steve Tall
	0L
};


unsigned long const EditorCodes[] = {
	0xA2C09326,					// Erik Yeo
	0x1F944BB3,					// Mike Lightner
	0xDE07154D,					// Adam Isgreen
	0x0E07B213,					// Steve Tall
	0x16B170B1,					// Joe Bostic
	0L
};


/***********************************************************************************************
**	Unit order names. These names correspond to the player selectable orders
**	a unit can have. The system initiated orders have no use for the ASCII name
**	associated, but they are listed here for completeness sake.
*/
char const * Missions[MISSION_COUNT] = {
	"Sleep",
	"Attack",
	"Move",
	"QMove",
	"Retreat",
	"Guard",
	"Sticky",
	"Enter",
	"Capture",
	"Harvest",
	"Area Guard",
	"Return",
	"Stop",
	"Ambush",
	"Hunt",
	"Unload",
	"Sabotage",
	"Construction",
	"Selling",
	"Repair",
	"Rescue",
	"Missile",
	"Harmless"
};


/***************************************************************************
**	Special weapon names.
*/
#ifdef SCENARIO_EDITOR
char const * const SpecialWeaponName[SPC_COUNT] = {
	"Sonar Pulse",
	"Nuclear Missile",
	"Chronosphere",
	"Parachute Bomb",
	"Paratroopers",
	"Recon Plane",
	"Iron Curtain",
	"GPS Satellite"
};
#endif
int const SpecialWeaponHelp[SPC_COUNT] = {
	TXT_SONAR_PULSE,
	TXT_NUCLEAR_BOMB,
	TXT_CHRONOSHIFT,
	TXT_PARA_BOMB,
	TXT_PARA_INFANTRY,
	TXT_SPY_MISSION,
	TXT_INVUL,
	TXT_GPS_SATELLITE
};
char const * const SpecialWeaponFile[SPC_COUNT] = {
	"SONR",
	"ATOM",
	"WARP",
	"PBMB",
	"PINF",
	"CAM",
	"INFX",
	"GPSS"
};


/***************************************************************************
**	Type of quarry to search out and attack. These values are used for team
**	attack missions.
*/
char const * const QuarryName[QUARRY_COUNT] = {
	"N/A",
	"Anything",
	"Buildings - any",
	"Harvesters",
	"Infantry",
	"Vehicles - any",
	"Ships - any",
	"Factories",
	"Base Defenses",
	"Base Threats",
	"Power Facilities",
	"Fake Buildings"
};


/***************************************************************************
**	These are the text names for the formation types.
*/
char const * const FormationName[FORMATION_COUNT] = {
	"None",

	"Tight",
	"Loose",
	"Wedge North",
	"Wedge East",
	"Wedge South",
	"Wedge West",
	"Line N/S",
	"Line E/W"
};


/***************************************************************************
**	These are the ASCII names for the reinforcement sources.
*/
char const * const SourceName[SOURCE_COUNT] =
{
	"North",
	"East",
	"South",
	"West",
	"Air"
};


/***************************************************************************
**	These are the text names for the various armor types a unit may possess.
*/
char const * const ArmorName[ARMOR_COUNT] = {
	"none",
	"wood",
	"light",
	"heavy",
	"concrete"
};


// HACK ALERT! This unused text string is here to stop Watcom from crashing. There is some
// magic text heap length that causes a crash before the code executes. This dummy string
// changes the text heap length enough to stop the crash. Who knows why, but it works.
char * __test__ = "alskdfjlasdfjkajsdfkja;sldjfklasj9awutreqjfnfdkvnldzlknvadsjgflkasdjfkajsdfas";


/***************************************************************************
**	The list of VQ filenames.
*/
char const * const VQName[VQ_COUNT] = {
	"AAGUN",
	"MIG",
	"SFROZEN",
	"AIRFIELD",
	"BATTLE",
	"BMAP",
	"BOMBRUN",
	"DPTHCHRG",
	"GRVESTNE",
	"MONTPASS",
	"MTNKFACT",
	"CRONTEST",
	"OILDRUM",
	"ALLIEND",
	"RADRRAID",
	"SHIPYARD", // MISSING
	"SHORBOMB",
	"SITDUCK",
	"SLNTSRVC",
	"SNOWBASE",
	"EXECUTE",
	"REDINTRO",			// low res.
	"NUKESTOK",
	"V2ROCKET",
	"SEARCH",
	"BINOC",
	"ELEVATOR",
	"FROZEN",
	"MCV",
	"SHIPSINK",
	"SOVMCV",
	"TRINITY",
	"ALLYMORF",
	"APCESCPE",
	"BRDGTILT",
	"CRONFAIL",
	"STRAFE",
	"DESTROYR",
	"DOUBLE",
	"FLARE",
	"SNSTRAFE",
	"LANDING",
	"ONTHPRWL",
	"OVERRUN",
	"SNOWBOMB",
	"SOVCEMET",
	"TAKE_OFF",
	"TESLA",
	"SOVIET8",
	"SPOTTER",
	"ALLY1",
	"ALLY2",
	"ALLY4",
	"SOVFINAL",
	"ASSESS",
	"SOVIET10",
	"DUD",
	"MCV_LAND",
	"MCVBRDGE",
	"PERISCOP",
	"SHORBOM1",
	"SHORBOM2",
	"SOVBATL",
	"SOVTSTAR",
	"AFTRMATH",
	"SOVIET11",
	"MASASSLT",
	"ENGLISH",		// High res. Intro
	"SOVIET1",
	"SOVIET2",
	"SOVIET3",
	"SOVIET4",
	"SOVIET5",
	"SOVIET6",
	"SOVIET7",
	"PROLOG",
	"AVERTED",
	"COUNTDWN",
	"MOVINGIN",
	"ALLY10",
	"ALLY12",
	"ALLY5",
	"ALLY6",
	"ALLY8",
	"TANYA1",
	"TANYA2",
	"ALLY10B",
	"ALLY11",
	"ALLY14",
	"ALLY9",
	"SPY",
	"TOOFAR",
	"SOVIET12",
	"SOVIET13",
	"SOVIET9",
	"BEACHEAD",
	"SOVIET14",
	"SIZZLE",   //MISSING
	"SIZZLE2",  //MISSING
	"ANTEND",
	"ANTINTRO",


		//2019/11/12 JAS - Added for Retaliation movies

		"RETALIATION_ALLIED1",
		"RETALIATION_ALLIED2",
		"RETALIATION_ALLIED3",
		"RETALIATION_ALLIED4",
		"RETALIATION_ALLIED5",
		"RETALIATION_ALLIED6",
		"RETALIATION_ALLIED7",
		"RETALIATION_ALLIED8",
		"RETALIATION_ALLIED9",
		"RETALIATION_ALLIED10",

		"RETALIATION_SOVIET1",
		"RETALIATION_SOVIET2",
		"RETALIATION_SOVIET3",
		"RETALIATION_SOVIET4",
		"RETALIATION_SOVIET5",
		"RETALIATION_SOVIET6",
		"RETALIATION_SOVIET7",
		"RETALIATION_SOVIET8",
		"RETALIATION_SOVIET9",
		"RETALIATION_SOVIET10",
		"RETALIATION_WINA",
		"RETALIATION_WINS",
		"RETALIATION_ANTS"
};


/***************************************************************************
**	Relative coordinate offsets from the center of a cell for each
**	of the legal positions that an object in a cell may stop at. Only infantry
**	are allowed to stop at other than the center of the cell.
*/
COORDINATE const StoppingCoordAbs[5] = {
	0x00800080L,	// center
	0x00400040L,	// upper left
	0x004000C0L,	// upper right
	0x00C00040L,	// lower left
	0x00C000C0L		// lower right
};


/***************************************************************************
**	Converts pixel values (cell relative) into the appropriate lepton (sub cell)
**	value. This is used to convert pixel (screen) coordinates into the underlying
**	coordinate system.
*/
unsigned char const Pixel2Lepton[24] = {
	0x00,0x0B,0x15,0x20,0x2B,0x35,0x40,0x4B,
	0x55,0x60,0x6B,0x75,0x80,0x8B,0x95,0xA0,
	0xAB,0xB5,0xC0,0xCB,0xD5,0xE0,0xEB,0xF5
};


/***************************************************************************
**	This array is used to index a facing in order to retrieve a cell
**	offset that, when added to another cell, will achieve the adjacent cell
**	in the indexed direction.
*/
CELL const AdjacentCell[FACING_COUNT] = {
	-(MAP_CELL_W),				// North
	-(MAP_CELL_W-1),			// North East
	1,								// East
	MAP_CELL_W+1,				// South East
	MAP_CELL_W,					// South
	MAP_CELL_W-1,				// South West
	-1,							// West
	-(MAP_CELL_W+1)			// North West
};

COORDINATE const AdjacentCoord[FACING_COUNT] = {
	0xFF000000L,
	0xFF000100L,
	0x00000100L,
	0x01000100L,
	0x01000000L,
	0x0100FF00L,
	0x0000FF00L,
	0xFF00FF00L
};


/***************************************************************************
**	This specifies the odds of receiving the various random crate power
**	ups. The odds are expressed as "shares" of 100 percent.
*/
int CrateShares[CRATE_COUNT] = {
	50,		//	CRATE_MONEY
	20,		//	CRATE_UNIT
	3,			//	CRATE_PARA_BOMB
	1,			//	CRATE_HEAL_BASE
	3,			//	CRATE_CLOAK
	5,			//	CRATE_EXPLOSION
	5,			//	CRATE_NAPALM
	20,		//	CRATE_SQUAD
	1,			//	CRATE_DARKNESS
	1,			//	CRATE_REVEAL
	3,			//	CRATE_SONAR
	10,		//	CRATE_ARMOR
	10,		//	CRATE_SPEED
	10,		//	CRATE_FIREPOWER
	1,			//	CRATE_ICBM
	1,			//	CRATE_TIMEQUAKE
	3,			//	CRATE_INVULN
	5			// CRATE_VORTEX
};

AnimType CrateAnims[CRATE_COUNT] = {
	ANIM_NONE,	//	CRATE_MONEY
	ANIM_NONE,	//	CRATE_UNIT
	ANIM_NONE,	//	CRATE_PARA_BOMB
	ANIM_NONE,	//	CRATE_HEAL_BASE
	ANIM_NONE,	//	CRATE_CLOAK
	ANIM_NONE,	//	CRATE_EXPLOSION
	ANIM_NONE,	//	CRATE_NAPALM
	ANIM_NONE,	//	CRATE_SQUAD
	ANIM_NONE,	//	CRATE_DARKNESS
	ANIM_NONE,	//	CRATE_REVEAL
	ANIM_NONE,	//	CRATE_SONAR
	ANIM_NONE,	//	CRATE_ARMOR
	ANIM_NONE,	//	CRATE_SPEED
	ANIM_NONE,	//	CRATE_FIREPOWER
	ANIM_NONE,	//	CRATE_ICBM
	ANIM_NONE,	//	CRATE_TIMEQUAKE
	ANIM_NONE,	//	CRATE_INVULN
	ANIM_NONE   // CRATE_VORTEX
};

int CrateData[CRATE_COUNT] = {
	0,		//	CRATE_MONEY
	0,		//	CRATE_UNIT
	0,		//	CRATE_PARA_BOMB
	0,		//	CRATE_HEAL_BASE
	0,		//	CRATE_CLOAK
	0,		//	CRATE_EXPLOSION
	0,		//	CRATE_NAPALM
	0,		//	CRATE_SQUAD
	0,		//	CRATE_DARKNESS
	0,		//	CRATE_REVEAL
	0,		//	CRATE_SONAR
	0,		//	CRATE_ARMOR
	0,		//	CRATE_SPEED
	0,		//	CRATE_FIREPOWER
	0,		//	CRATE_ICBM
	0,		//	CRATE_TIMEQUAKE
	0,		//	CRATE_INVULN
	0		//	CRATE_VORTEX
};

char const * const CrateNames[CRATE_COUNT] = {
	"Money",
	"Unit",
	"ParaBomb",
	"HealBase",
	"Cloak",
	"Explosion",
	"Napalm",
	"Squad",
	"Darkness",
	"Reveal",
	"Sonar",
	"Armor",
	"Speed",
	"Firepower",
	"ICBM",
	"TimeQuake",
	"Invulnerability",
	"ChronalVortex"
};


/***************************************************************************
** This converts 0..255 facing values into either 8, 16, or 32 facing values.
**	Note: a simple shift won't suffice because 0..255 facing values should
**	be converted to the CLOSEST appropriate facing, NOT rounded down to the
**	nearest facing.
*/
unsigned char const Facing8[256] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


unsigned char const Facing16[256] = {
	0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,
	12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,
	14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,0
};


signed char const Rotation16[256] = {
	0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,
	0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,
	0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,
	0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,
	0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,
	0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,
	0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,
	0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1
};


/*
**	This table incorporates a compensating factor for the distortion caused
**	by 3D-Studio when it tries to render 45% angles.
*/
unsigned char const Facing32[256] = {
	0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,
	3,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,
	8,8,8,9,9,9,9,9,9,9,10,10,10,10,10,10,10,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,
	13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,
	16,16,16,16,16,17,17,17,17,17,17,17,17,17,18,18,18,18,18,18,18,18,18,19,19,19,19,19,19,19,19,19,
	19,20,20,20,20,20,20,21,21,21,21,21,21,21,22,22,22,22,22,22,22,23,23,23,23,23,23,23,24,24,24,24,
	24,24,24,25,25,25,25,25,25,25,26,26,26,26,26,26,26,27,27,27,27,27,27,27,28,28,28,28,28,28,28,28,
	29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,30,30,31,31,31,31,31,31,31,31,31,0,0,0,0,0,0
};


#ifdef OBSOLETE
unsigned char const Facing32[256] = {
	0,0,0,0,
	1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,3,
	4,4,4,4,4,4,4,4,
	5,5,5,5,5,5,5,5,
	6,6,6,6,6,6,6,6,
	7,7,7,7,7,7,7,7,
	8,8,8,8,8,8,8,8,
	9,9,9,9,9,9,9,9,
	10,10,10,10,10,10,10,10,
	11,11,11,11,11,11,11,11,
	12,12,12,12,12,12,12,12,
	13,13,13,13,13,13,13,13,
	14,14,14,14,14,14,14,14,
	15,15,15,15,15,15,15,15,
	16,16,16,16,16,16,16,16,
	17,17,17,17,17,17,17,17,
	18,18,18,18,18,18,18,18,
	19,19,19,19,19,19,19,19,
	20,20,20,20,20,20,20,20,
	21,21,21,21,21,21,21,21,
	22,22,22,22,22,22,22,22,
	23,23,23,23,23,23,23,23,
	24,24,24,24,24,24,24,24,
	25,25,25,25,25,25,25,25,
	26,26,26,26,26,26,26,26,
	27,27,27,27,27,27,27,27,
	28,28,28,28,28,28,28,28,
	29,29,29,29,29,29,29,29,
	30,30,30,30,30,30,30,30,
	31,31,31,31,31,31,31,31,
	0,0,0,0
};
#endif


/***************************************************************************
**	These are the movement costs (in ticks at fastest speed) to enter each
**	of the given terrain cells.
*/

int const GroundColor[LAND_COUNT] = {
	141,			// "Clear" terrain.
	141,			// Road terrain.
	172,			// Water.
	21,			// Impassable rock.
	21,			// Wall (blocks movement).
	158,			// Tiberium field.
	141,			//	Beach terrain.
	141,			// Rocky terrain.
	174			// Rocky riverbed.
};

int const SnowColor[LAND_COUNT] = {
	141,			// "Clear" terrain.
	141,			// Road terrain.
	172,			// Water.
	21,			// Impassable rock.
	21,			// Wall (blocks movement).
	158,			// Tiberium field.
	141,			//	Beach terrain.
	141,			// Rocky terrain.
	174			// Rocky riverbed.
};

#ifdef NEVER
int const GroundColor[LAND_COUNT] = {
	46,			// "Clear" terrain.
	44,			// Road terrain.
	BLUE,			// Water.
	DKGREY,		// Impassable rock.
	DKGREY,		// Wall (blocks movement).
	158,			// Tiberium field.
	64,			//	Beach terrain.
	DKGREY,		// Rocky terrain.
	DKGREY		// Rocky riverbed.
};

int const SnowColor[LAND_COUNT] = {
	WHITE, 		// "Clear" terrain.
	LTGRAY,		// Road terrain.
	BLUE,			// Water.
	DKGREY,		// Impassable rock.
	DKGREY,		// Wall (blocks movement).
	158,			// Tiberium field.
	LTGRAY,		//	Beach terrain.
	DKGREY,		// Rocky terrain.
	DKGREY		// Rocky riverbed.
};
#endif

GroundType Ground[LAND_COUNT];


/***************************************************************************
**	These are the names of the theaters.
*/
TheaterDataType const Theaters[THEATER_COUNT] = {
	{"TEMPERATE","TEMPERAT","TEM"},
	{"SNOW","SNOW","SNO"},
	{"INTERIOR","INTERIOR","INT"},
};


unsigned char const RemapCiv2[256] = {
	0,1,2,3,4,5,6,209,8,9,10,11,12,13,12,15,									// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,187,188,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,209,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	176,177,178,179,180,181,182,183,184,185,186,167, 13,189,190,191,	// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapCiv4[256] = {
	0,1,2,3,4,5,6,187,8,9,10,11,12,13,14,15,									// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,118,110,119,		// 96..111
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,	// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,188,207,	// 192..207
	208,209,182,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapCiv5[256] = {
	0,1,2,3,4,5,6,109,8,9,10,11,131,13,14,15,									// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,177,110,178,		// 96..111
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,	// 176..191
	192,193,194,195,196,197,198,199,111,201,202,203,204,205,111,207,	// 192..207
	208,209,182,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapCiv6[256] = {
	0,1,2,3,4,5,6,120,8,9,10,11,12,13,238,15,									// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,236,206,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,111,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,	// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapCiv7[256] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,131,15,									// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,157,212,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,7,		// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	176,177,178,179,180,181,182,183,184,185,186,118,119,189,190,191,	// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapCiv8[256] = {
	0,1,2,3,4,5,6,182,8,9,10,11,12,13,131,15,									// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,215,7,120,121,122,123,124,125,126,127,		// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,182,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	176,177,178,179,180,181,182,183,184,185,186,198,199,189,190,191,	// 176..191
	192,193,194,195,196,197,198,199,111,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapCiv9[256] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,7,15,										// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,163,165,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,200,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	176,177,178,179,180,181,182,183,184,185,186,111,13,189,190,191,	// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapCiv10[256] = {
	0,1,2,3,4,5,6,137,8,9,10,11,12,13,15,15,									// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,129,131,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,137,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	176,177,178,179,180,181,182,183,184,185,186,163,165,189,190,191,	// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapEmber[256] = {
#define	CEC	CC_EMBER_COLOR
	  0,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,BLACK,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,
	CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC,CEC
};


//char const Keys[] =
//	"[PublicKey]\n"
//	"1=AgkCbXo9sKMHOBk=\n"
//#ifdef CHEAT_KEYS
//	"[PrivateKey]\n"
//	"1=AggxFU55vc7LYQ==\n"
//#endif
//	"\n";

char const Keys[] =
"[PublicKey]\n"
"1=AihRvNoIbTn85FZRYNZRcT+i6KpU+maCsEqr3Q5q+LDB5tH7Tz2qQ38V\n"
#ifdef CHEAT_KEYS
"[PrivateKey]\n"
"1=AigKVje8mROcR8QixnxUEF5b29Curkq01DNDWCdOG99XBqH79OaCiTCB\n"
#endif
"\n";
