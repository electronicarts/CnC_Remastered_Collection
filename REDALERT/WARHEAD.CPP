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

/* $Header: /CounterStrike/WARHEAD.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : WARHEAD.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/20/96                                                     *
 *                                                                                             *
 *                  Last Update : July 19, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   WarheadTypeClass::As_Pointer -- Convert a warhead type number into a pointer.             *
 *   WarheadTypeClass::Read_INI -- Fetches the warhead data from the INI database.             *
 *   WarheadTypeClass::WarheadTypeClass -- Default constructor for warhead objects.            *
 *   WarheadTypeClass::operator delete -- Returns warhead object back to special memory pool.  *
 *   WarheadTypeClass::operator new -- Allocate a warhead object from the special heap.        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"


/***************************************************************************
**	This is the warhead data object array.
*/
TFixedIHeapClass<WarheadTypeClass> Warheads;


/***********************************************************************************************
 * WarheadTypeClass::WarheadTypeClass -- Default constructor for warhead objects.              *
 *                                                                                             *
 *    This default constructor for a warhead object will fill in all the default values        *
 *    for a warhead. It is presumed that these values will be normal unless specifically       *
 *    overridden by the INI database.                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
WarheadTypeClass::WarheadTypeClass(char const * name) :
	ID(Warheads.ID(this)),
	IniName(name),
	SpreadFactor(1),
	IsWallDestroyer(false),
	IsWoodDestroyer(false),
	IsTiberiumDestroyer(false),
	IsOrganic(false),
	ExplosionSet(0),
	InfantryDeath(0)
{
	for (ArmorType armor = ARMOR_FIRST; armor < ARMOR_COUNT; armor++) {
		Modifier[armor] = 1;
	}
}


/***********************************************************************************************
 * WarheadTypeClass::operator new -- Allocate a warhead object from the special heap.          *
 *                                                                                             *
 *    This will allocate a warhead object from the special heap that is maintained for         *
 *    this purpose.                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the newly allocated warhead type object. If there is     *
 *          insufficient memory for the allocation, then NULL is returned.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * WarheadTypeClass::operator new(size_t)
{
	return(Warheads.Alloc());
}


/***********************************************************************************************
 * WarheadTypeClass::operator delete -- Returns warhead object back to special memory pool.    *
 *                                                                                             *
 *    This routine will return the warhead object to the memory pool from whence it came.      *
 *                                                                                             *
 * INPUT:   pointer  -- Pointer to the warhead object to return to the memory pool.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void WarheadTypeClass::operator delete(void * pointer)
{
	Warheads.Free((WarheadTypeClass *)pointer);
}


/***********************************************************************************************
 * WarheadTypeClass::As_Pointer -- Convert a warhead type number into a pointer.               *
 *                                                                                             *
 *    This will locate the warhead type specified and return a pointer to it.                  *
 *                                                                                             *
 * INPUT:   warhead  -- The warhead to convert into a pointer.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the warhead type object that is represented by the       *
 *          warhead type number specified.                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
WarheadTypeClass * WarheadTypeClass::As_Pointer(WarheadType warhead)
{
	if (warhead != WARHEAD_NONE) {
		return(Warheads.Ptr(warhead));
	}
	return(NULL);
}


/***********************************************************************************************
 * WarheadTypeClass::Read_INI -- Fetches the warhead data from the INI database.               *
 *                                                                                             *
 *    Use this routine to retrieve the data specific to this warhead type class object from    *
 *    the INI database specified. Typical use of this is when processing the rules.ini         *
 *    file.                                                                                    *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to fetch the values from.                   *
 *                                                                                             *
 * OUTPUT:  bool; Was the warhead entry found and the data retrieved?                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool WarheadTypeClass::Read_INI(CCINIClass & ini)
{
	if (ini.Is_Present(Name())) {
		SpreadFactor = ini.Get_Int(Name(), "Spread", SpreadFactor);
		IsWallDestroyer = ini.Get_Bool(Name(), "Wall", IsWallDestroyer);
		IsWoodDestroyer = ini.Get_Bool(Name(), "Wood", IsWoodDestroyer);
		IsTiberiumDestroyer = ini.Get_Bool(Name(), "Ore", IsTiberiumDestroyer);
		ExplosionSet = ini.Get_Int(Name(), "Explosion", ExplosionSet);
		InfantryDeath = ini.Get_Int(Name(), "InfDeath", InfantryDeath);

		char buffer[128];
		if (ini.Get_String(Name(), "Verses", "100%%,100%%,100%%,100%%,100%%", buffer, sizeof(buffer))) {
			char * aval = strtok(buffer, ",");
			for (ArmorType armor = ARMOR_FIRST; armor < ARMOR_COUNT; armor++) {
				Modifier[armor] = fixed(aval);
				aval = strtok(NULL, ",");
			}
		}

		IsOrganic = (Modifier[ARMOR_STEEL] == 0);
		return(true);
	}
	return(false);
}

