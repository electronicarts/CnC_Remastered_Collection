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

/* $Header:   F:\projects\c&c\vcs\code\hdata.cpv   2.17   16 Oct 1995 16:48:18   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : HDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 22, 1994                                                 *
 *                                                                                             *
 *                  Last Update : January 23, 1995 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   HouseTypeClass::From_Name -- Fetch house pointer from its name.                           *
 *   HouseTypeClass::As_Reference -- Fetches a reference to the house specified.               * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

/*
**	These are the colors used to identify the various owners.
*/
const int COLOR_GOOD = 180;  // GOLD
const int COLOR_BRIGHT_GOOD = 176;  // GOLD
const int COLOR_BAD = 123;	//RED;
const int COLOR_BRIGHT_BAD = 127;	//RED;
const int COLOR_NEUTRAL = 205;	//WHITE;
const int COLOR_BRIGHT_NEUTRAL = 202;	//WHITE;


static HouseTypeClass const HouseGood(
	HOUSE_GOOD,
	"GoodGuy",					//	NAME:			House name.
	TXT_GDI,						// FULLNAME:	Translated house name.
	"GDI",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	COLOR_GOOD,					// COLOR:		Dark Radar map color.
	COLOR_BRIGHT_GOOD,		// COLOR:		Bright Radar map color.
	REMAP_GOLD,				// Remap color ID number.
	RemapGold,				// Default remap table.
	'G'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseBad(
	HOUSE_BAD,
	"BadGuy",					//	NAME:			House name.
	TXT_NOD,						// FULLNAME:	Translated house name.
	"NOD",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	COLOR_BAD,					// COLOR:		Dark Radar map color.
	COLOR_BRIGHT_BAD,			// COLOR:		Bright Radar map color.
	REMAP_LTBLUE,				// Remap color ID number.
	RemapLtBlue,				// Default remap table.
	'B'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseCivilian(
	HOUSE_NEUTRAL,
	"Neutral",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"CIV",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	COLOR_NEUTRAL,				// COLOR:		Dark Radar map color.
	COLOR_BRIGHT_NEUTRAL,	// COLOR:		Bright Radar map color.
	REMAP_GOLD,				// Remap color ID number.
	RemapNone,					// Default remap table.
	'C'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseJP(
	HOUSE_JP,
	"Special",					//	NAME:			House name.
	TXT_JP,						// FULLNAME:	Translated house name.
	"JP",							// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	COLOR_NEUTRAL,				// COLOR:		Dark Radar map color.
	COLOR_BRIGHT_NEUTRAL,	// COLOR:		Bright Radar map color.
	REMAP_GOLD,				// Remap color ID number.
	RemapNone,					// Default remap table.
	'J'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti1(
	HOUSE_MULTI1,
	"Multi1",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP1",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	COLOR_NEUTRAL,				// COLOR:		Radar map color.
	COLOR_BRIGHT_NEUTRAL,	// COLOR:		Bright Radar map color.
	REMAP_LTBLUE,					// Remap color ID number.
	RemapLtBlue,			// Default remap table.
	'M'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti2(
	HOUSE_MULTI2,
	"Multi2",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP2",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	COLOR_NEUTRAL,				// COLOR:		Radar map color.
	COLOR_BRIGHT_NEUTRAL,	// COLOR:		Bright Radar map color.
	REMAP_ORANGE,				// Remap color ID number.
	RemapOrange,				// Default remap table.
	'M'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti3(
	HOUSE_MULTI3,
	"Multi3",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP3",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	COLOR_NEUTRAL,				// COLOR:		Radar map color.
	COLOR_BRIGHT_NEUTRAL,	// COLOR:		Bright Radar map color.
	REMAP_GREEN,				// Remap color ID number.
	RemapGreen,					// Default remap table.
	'M'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti4(
	HOUSE_MULTI4,
	"Multi4",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP4",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	COLOR_NEUTRAL,				// COLOR:		Radar map color.
	COLOR_BRIGHT_NEUTRAL,	// COLOR:		Bright Radar map color.
	REMAP_BLUE,					// Remap color ID number.
	RemapBlue,					// Default remap table.
	'M'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti5(
	HOUSE_MULTI5,
	"Multi5",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP5",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	COLOR_NEUTRAL,				// COLOR:		Radar map color.
	COLOR_BRIGHT_NEUTRAL,	// COLOR:		Bright Radar map color.
	REMAP_GOLD,				// Remap color ID number.
	RemapGold,				// Default remap table.
	'M'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti6(
	HOUSE_MULTI6,
	"Multi6",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP6",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	COLOR_NEUTRAL,				// COLOR:		Radar map color.
	COLOR_BRIGHT_NEUTRAL,	// COLOR:		Bright Radar map color.
	REMAP_RED,					// Remap color ID number.
	RemapRed,					// Default remap table.
	'M'							// VOICE:		Voice prefix character.
);

HouseTypeClass const * const HouseTypeClass::Pointers[HOUSE_COUNT] = {
	&HouseGood,
	&HouseBad,
	&HouseCivilian,
	&HouseJP,
	&HouseMulti1,
	&HouseMulti2,
	&HouseMulti3,
	&HouseMulti4,
	&HouseMulti5,
	&HouseMulti6,
};


/***********************************************************************************************
 * HouseTypeClass::HouseTypeClass -- Constructor for house type objects.                       *
 *                                                                                             *
 *    This is the constructor for house type objects. This object holds the constant data      *
 *    for the house type.                                                                      *
 *                                                                                             *
 * INPUT:   house    -- The ID number for this house type.                                     *
 *          ini      -- The INI name of this house.                                            *
 *          fullname -- The text number representing the complete name of the house.           *
 *          ext      -- The filename extension used when loading data files.                   *
 *          lemon    -- The percentage for objects of this ownership to be lemon.              *
 *          remapc   -- The remap color number to use.                                         *
 *          color    -- The radar color to use for this "house".                               *
 *          prefix   -- A unique prefix letter used when building custom filenames.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
HouseTypeClass::HouseTypeClass(HousesType house, 
				char const * ini, 
				int fullname, 
				char const * ext, 
				int lemon, 
				int color, 
				int bright_color, 
				PlayerColorType remapcolor,
				unsigned char const * remap, 
				char prefix)
{
	RemapTable = remap;
	RemapColor = remapcolor;
	House = house;
	IniName = ini;
	FullName = fullname;
	strncpy(Suffix, ext, 3);
	Suffix[3] = '\0';
	Lemon = lemon;
	Color = color;
	BrightColor = bright_color;
	Prefix = prefix;
	FirepowerBias = 1;
	GroundspeedBias = 1;
	AirspeedBias = 1;
	ArmorBias = 1;
	ROFBias = 1;
	CostBias = 1;
	BuildSpeedBias = 1;
}


/***********************************************************************************************
 * HouseTypeClass::From_Name -- Fetch house pointer from its name.                             *
 *                                                                                             *
 *    This routine will convert the ASCII house name specified into a                          *
 *    real house number. Typically, this is used when processing a                             *
 *    scenario INI file.                                                                       *
 *                                                                                             *
 * INPUT:   name  -- ASCII name of house to process.                                           *
 *                                                                                             *
 * OUTPUT:  Returns with actual house number represented by the ASCII                          *
 *          name specified.                                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   05/21/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
HousesType HouseTypeClass::From_Name(char const *name)
{
	if (name) {
		for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
			if (stricmp(Pointers[house]->IniName, name) == 0) {
				return(house);
			}
		}
	}
	return(HOUSE_NONE);
}


/***********************************************************************************************
 * HouseTypeClass::One_Time -- One-time initialization                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/21/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
void HouseTypeClass::One_Time(void)
{
	/*
	** Change the radar color for special units; otherwise, they'll be the same
	** color as the player!
	*/
	//if (Special.IsJurassic && AreThingiesEnabled) {			// Assume funpark mode might be required. ST - 10/14/2019 11:53AM
		((unsigned char &)HouseJP.Color) = (unsigned char)COLOR_BAD;
		((unsigned char &)HouseJP.BrightColor) = (unsigned char)COLOR_BRIGHT_BAD;
	//}
}


/*********************************************************************************************** 
 * HouseTypeClass::As_Reference -- Fetches a reference to the house specified.                 * 
 *                                                                                             * 
 *    Use this routine to fetch a reference to the house number specified.                     * 
 *                                                                                             * 
 * INPUT:   house -- The house number (HousesType) to look up.                                 * 
 *                                                                                             * 
 * OUTPUT:  Returns with a reference to the HouseTypeClass object that matches the house       * 
 *          number specified.                                                                  * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   01/23/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
HouseTypeClass const & HouseTypeClass::As_Reference(HousesType house) 
{
	return(*Pointers[house]);
}