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

/* $Header: /CounterStrike/HDATA.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
 *                  Last Update : September 4, 1996 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   HouseTypeClass::As_Reference -- Fetches a reference to the house specified.               *
 *   HouseTypeClass::From_Name -- Fetch house pointer from its name.                           *
 *   HouseTypeClass::HouseTypeClass -- Constructor for house type objects.                     *
 *   HouseTypeClass::Init_Heap -- Allocate all heap objects for the house types.               *
 *   HouseTypeClass::One_Time -- One-time initialization                                       *
 *   HouseTypeClass::Read_INI -- Fetch the house control values from ini database.             *
 *   HouseTypeClass::Remap_Table -- Fetches the remap table for this house.                    *
 *   HouseTypeClass::operator delete -- Returns a house type object back to the heap.          *
 *   HouseTypeClass::operator new -- Allocates a house type class object from special heap.    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

static HouseTypeClass const HouseEngland(
	HOUSE_ENGLAND,
	"England",					//	NAME:			House name.
	TXT_ENGLAND,				// FULLNAME:	Translated house name.
	"ENG",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_GREEN,				// Remap color ID number.
	'E'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseGermany(
	HOUSE_GERMANY,
	"Germany",					//	NAME:			House name.
	TXT_GERMANY,				// FULLNAME:	Translated house name.
	"GER",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_GREY,				// Remap color ID number.
	'G'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseFrance(
	HOUSE_FRANCE,
	"France",					//	NAME:			House name.
	TXT_FRANCE,					// FULLNAME:	Translated house name.
	"FRA",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_BLUE,				// Remap color ID number.
	'F'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseUkraine(
	HOUSE_UKRAINE,
	"Ukraine",					//	NAME:			House name.
	TXT_UKRAINE,				// FULLNAME:	Translated house name.
	"UKA",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_ORANGE,				// Remap color ID number.
	'K'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseUSSR(
	HOUSE_USSR,
	"USSR",						//	NAME:			House name.
	TXT_USSR,					// FULLNAME:	Translated house name.
	"RED",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_RED,					// Remap color ID number.
	'U'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseGreece(
	HOUSE_GREECE,
	"Greece",					//	NAME:			House name.
	TXT_GREECE,					// FULLNAME:	Translated house name.
	"GRE",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_LTBLUE,				// Remap color ID number.
	'G'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseTurkey(
	HOUSE_TURKEY,
	"Turkey",					//	NAME:			House name.
	TXT_TURKEY,					// FULLNAME:	Translated house name.
	"TRK",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_BROWN,				// Remap color ID number.
	'T'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseSpain(
	HOUSE_SPAIN,
	"Spain",						//	NAME:			House name.
	TXT_SPAIN,					// FULLNAME:	Translated house name.
	"SPN",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_GOLD,				// Remap color ID number.
	'S'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseGood(
	HOUSE_GOOD,
	"GoodGuy",					//	NAME:			House name.
	TXT_GOODGUY,				// FULLNAME:	Translated house name.
	"GDI",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_LTBLUE,				// Remap color ID number.
	'G'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseBad(
	HOUSE_BAD,
	"BadGuy",					//	NAME:			House name.
	TXT_BADGUY,					// FULLNAME:	Translated house name.
	"NOD",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_RED,					// Remap color ID number.
	'B'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseCivilian(
	HOUSE_NEUTRAL,
	"Neutral",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"CIV",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_GOLD,				// Remap color ID number.
	'C'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseJP(
	HOUSE_JP,
	"Special",					//	NAME:			House name.
	TXT_JP,						// FULLNAME:	Translated house name.
	"JP",							// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_GOLD,				// Remap color ID number.
	'J'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti1(
	HOUSE_MULTI1,
	"Multi1",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP1",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_GOLD,				// Remap color ID number.
	'M'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti2(
	HOUSE_MULTI2,
	"Multi2",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP2",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_LTBLUE,				// Remap color ID number.
	'M'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti3(
	HOUSE_MULTI3,
	"Multi3",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP3",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_RED,					// Remap color ID number.
	'M'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti4(
	HOUSE_MULTI4,
	"Multi4",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP4",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_GREEN,				// Remap color ID number.
	'M'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti5(
	HOUSE_MULTI5,
	"Multi5",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP5",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_ORANGE,				// Remap color ID number.
	'M'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti6(
	HOUSE_MULTI6,
	"Multi6",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP6",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_GREY,				// Remap color ID number.
	'M'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti7(
	HOUSE_MULTI7,
	"Multi7",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP7",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_BLUE,				// Remap color ID number.
	'M'							// VOICE:		Voice prefix character.
);

static HouseTypeClass const HouseMulti8(
	HOUSE_MULTI8,
	"Multi8",					//	NAME:			House name.
	TXT_CIVILIAN,				// FULLNAME:	Translated house name.
	"MP8",						// SUFFIX:		House file suffix.
	0,								// LEMON:		Lemon vehicle frequency.
	PCOLOR_BROWN,				// Remap color ID number.
	'M'							// VOICE:		Voice prefix character.
);

#ifdef OBSOLETE
HouseTypeClass const * const HouseTypeClass::Pointers[HOUSE_COUNT] = {
	&HouseSpain,
	&HouseGreece,
	&HouseUSSR,
	&HouseEngland,
	&HouseUkraine,
	&HouseGermany,
	&HouseFrance,
	&HouseTurkey,
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
	&HouseMulti7,
	&HouseMulti8,
};
#endif


/***********************************************************************************************
 * HouseTypeClass::HouseTypeClass -- Constructor for house type objects.                       *
 *                                                                                             *
 *    This is the constructor for house type objects. This object holds the constant data      *
 *    for the house type.                                                                      *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
HouseTypeClass::HouseTypeClass(
				HousesType house,
				char const * ini,
				int fullname,
				char const * ext,
				int lemon,
				PlayerColorType remapcolor,
				char prefix) :
	AbstractTypeClass(RTTI_HOUSETYPE, house, fullname, ini),
//	RTTI(RTTI_HOUSETYPE),
//	ID(house),
	House(house),
//	IniName(ini),
//	FullName(fullname),
	Lemon(lemon),
	RemapColor(remapcolor),
	Prefix(prefix),
	FirepowerBias(1),
	GroundspeedBias(1),
	AirspeedBias(1),
	ArmorBias(1),
	ROFBias(1),
	CostBias(1),
	BuildSpeedBias(1)
{
	strncpy(Suffix, ext, 3);
	Suffix[3] = '\0';
}


/***********************************************************************************************
 * HouseTypeClass::operator new -- Allocates a house type class object from special heap.      *
 *                                                                                             *
 *    This will allocate a house type object from the special heap that is used to maintain    *
 *    objects of this type.                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to the newly allocated house type object.                        *
 *                                                                                             *
 * WARNINGS:   If there is insufficient room, this routine may return NULL.                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * HouseTypeClass::operator new(size_t)
{
	return(HouseTypes.Alloc());
}


/***********************************************************************************************
 * HouseTypeClass::operator delete -- Returns a house type object back to the heap.            *
 *                                                                                             *
 *    This will return the house type object specified back into the special heap that         *
 *    is used to maintain house type objects.                                                  *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the house type object to delete.                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseTypeClass::operator delete(void * ptr)
{
	HouseTypes.Free((HouseTypeClass *)ptr);
}


/***********************************************************************************************
 * HouseTypeClass::Init_Heap -- Allocate all heap objects for the house types.                 *
 *                                                                                             *
 *    This will preallocate all the house types. They must be allocated in a particular order. *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine should be called only once at the beginning of the game.           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseTypeClass::Init_Heap(void)
{
	/*
	**	These house type class objects must be allocated in the exact order that they
	**	are specified in the HousesType enumeration. This is necessary because the heap
	**	allocation block index serves double duty as the type number index.
	*/
	new HouseTypeClass(HouseSpain);
	new HouseTypeClass(HouseGreece);
	new HouseTypeClass(HouseUSSR);
	new HouseTypeClass(HouseEngland);
	new HouseTypeClass(HouseUkraine);
	new HouseTypeClass(HouseGermany);
	new HouseTypeClass(HouseFrance);
	new HouseTypeClass(HouseTurkey);
	new HouseTypeClass(HouseGood);
	new HouseTypeClass(HouseBad);
	new HouseTypeClass(HouseCivilian);
	new HouseTypeClass(HouseJP);
	new HouseTypeClass(HouseMulti1);
	new HouseTypeClass(HouseMulti2);
	new HouseTypeClass(HouseMulti3);
	new HouseTypeClass(HouseMulti4);
	new HouseTypeClass(HouseMulti5);
	new HouseTypeClass(HouseMulti6);
	new HouseTypeClass(HouseMulti7);
	new HouseTypeClass(HouseMulti8);
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
HousesType HouseTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
			if (stricmp(As_Reference(house).IniName, name) == 0) {
//			if (stricmp(Pointers[house]->IniName, name) == 0) {
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
 *   06/19/1996 JLB : Converted to regular heap class management.                              *
 *=============================================================================================*/
void HouseTypeClass::One_Time(void)
{
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
HouseTypeClass & HouseTypeClass::As_Reference(HousesType house)
{
	return(*HouseTypes.Ptr(house));
}


/***********************************************************************************************
 * HouseTypeClass::Remap_Table -- Fetches the remap table for this house.                      *
 *                                                                                             *
 *    Use this routine to fetch the remap table assigned to this house. The remap table is     *
 *    what gives the house's units/buildings their distinctive color.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the remap table to use for this house.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
unsigned char const * HouseTypeClass::Remap_Table(void) const
{
	return(ColorRemaps[RemapColor].RemapTable);
}


/***********************************************************************************************
 * HouseTypeClass::Read_INI -- Fetch the house control values from ini database.               *
 *                                                                                             *
 *    This routine will fetch the rules controllable values for the house type from the        *
 *    INI database specified.                                                                  *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to fetch the house control values from.     *
 *                                                                                             *
 * OUTPUT:  bool; Was the house section found and processed?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseTypeClass::Read_INI(CCINIClass & ini)
{
	if (ini.Is_Present(Name())) {
		FirepowerBias = ini.Get_Fixed(Name(), "Firepower", FirepowerBias);
		GroundspeedBias = ini.Get_Fixed(Name(), "Groundspeed", GroundspeedBias);
		AirspeedBias = ini.Get_Fixed(Name(), "Airspeed", AirspeedBias);
		ArmorBias = ini.Get_Fixed(Name(), "Armor", ArmorBias);
		ROFBias = ini.Get_Fixed(Name(), "ROF", ROFBias);
		CostBias = ini.Get_Fixed(Name(), "Cost", CostBias);
		BuildSpeedBias = ini.Get_Fixed(Name(), "BuildTime", BuildSpeedBias);
		return(true);
	}
	return(false);
}
