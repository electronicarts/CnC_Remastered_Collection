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

/* $Header: /CounterStrike/ABSTRACT.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : ABSTRACT.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 01/26/95                                                     *
 *                                                                                             *
 *                  Last Update : July 10, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   AbstractClass::Debug_Dump -- Display debug information to mono screen.                    *
 *   AbstractClass::Distance -- Determines distance to target.                                 *
 *   AbstractTypeClass::AbstractTypeClass -- Constructor for abstract type objects.            *
 *   AbstractTypeClass::Coord_Fixup -- Performs custom adjustments to location coordinate.     *
 *   AbstractTypeClass::Full_Name -- Returns the full name (number) of this object type.       *
 *   AbstractTypeClass::Get_Ownable -- Fetch the ownable bits for this object.                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***********************************************************************************************
 * AbstractClass::Debug_Dump -- Display debug information to mono screen.                      *
 *                                                                                             *
 *    This debug only routine will display various information about this abstract class       *
 *    object to the monochrome screen specified.                                               *
 *                                                                                             *
 * INPUT:   mono  -- Pointer to the monochrome screen to display the debug information to.     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef CHEAT_KEYS
void AbstractClass::Debug_Dump(MonoClass * mono) const
{
	assert(IsActive);

	mono->Set_Cursor(11, 5);mono->Printf("%08X", As_Target());
	mono->Set_Cursor(20, 1);mono->Printf("%08X", Coord);
	mono->Set_Cursor(29, 1);mono->Printf("%3d", Height);
	if (Owner() != HOUSE_NONE) {
		mono->Set_Cursor(1, 3);
		mono->Printf("%-18s", Text_String(HouseTypeClass::As_Reference(Owner()).FullName));
	}
}
#endif


/***********************************************************************************************
 * AbstractClass::Distance -- Determines distance to target.                                   *
 *                                                                                             *
 *    This will determine the distance (direct line) to the target. The distance is in         *
 *    'leptons'. This routine is typically used for weapon range checks.                       *
 *                                                                                             *
 * INPUT:   target   -- The target to determine range to.                                      *
 *                                                                                             *
 * OUTPUT:  Returns with the range to the specified target (in leptons).                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int AbstractClass::Distance(TARGET target) const
{
	/*
	**	Should subtract a fudge-factor distance for building targets.
	*/
	BuildingClass * obj = As_Building(target);
	int dist = Distance(As_Coord(target));

	/*
	** If the object is a building the adjust it by the average radius
	** of the object.
	*/
	if (obj && obj->IsActive) {
		dist -= ((obj->Class->Width() + obj->Class->Height()) * (0x100 / 4));
		if (dist < 0) dist = 0;
	}

	/*
	** Return the distance to the target
	*/
	return(dist);
}


/***********************************************************************************************
 * AbstractTypeClass::AbstractTypeClass -- Constructor for abstract type objects.              *
 *                                                                                             *
 *    This is the constructor for AbstractTypeClass objects. It initializes the INI name and   *
 *    the text name for this object type.                                                      *
 *                                                                                             *
 * INPUT:   name  -- Text number for the full name of the object.                              *
 *                                                                                             *
 *          ini   -- The ini name for this object type.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
AbstractTypeClass::AbstractTypeClass(RTTIType rtti, int id, int name, char const * ini) :
	RTTI(rtti),
	ID(id),
	FullName(name)
{
	strncpy((char *)IniName, ini, sizeof(IniName));
	((char &)IniName[sizeof(IniName)-1]) = '\0';
}


/***********************************************************************************************
 * AbstractTypeClass::Coord_Fixup -- Performs custom adjustments to location coordinate.       *
 *                                                                                             *
 *    This routine is called when the placement coordinate should be fixed up according        *
 *    to any special rules specific to this object type. At this level, no transformation      *
 *    occurs. Derived classes will transform the coordinate as necessary.                      *
 *                                                                                             *
 * INPUT:   coord -- The proposed coordinate that this object type will be placed down at.     *
 *                                                                                             *
 * OUTPUT:  Returns with the adjusted coordinate that the object should be placed down at.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE AbstractTypeClass::Coord_Fixup(COORDINATE coord) const
{
	return(coord);
}


/***********************************************************************************************
 * AbstractTypeClass::Full_Name -- Returns the full name (number) of this object type.         *
 *                                                                                             *
 *    This routine is used to fetch the full name of this object type. The name value          *
 *    returned is actually the index number into the text array.                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the full name index number for this object type.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AbstractTypeClass::Full_Name(void) const
{
#ifdef FIXIT_NAME_OVERRIDE
	for (int index = 0; index < ARRAY_SIZE(NameOverride); index++) {
		if (NameIDOverride[index] == ((RTTI+1) * 100) + ID)  {
			return(-(index+1));
		}
	}
#endif
	return(FullName);
}


/***********************************************************************************************
 * AbstractTypeClass::Get_Ownable -- Fetch the ownable bits for this object.                   *
 *                                                                                             *
 *    This returns a bit flag that indicates which houses are allowed to own this object       *
 *    type. At this level, all houses have ownership rights. This routine will be overridden   *
 *    by object types that restrict ownership.                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a bit flag indicating which houses have ownership rights.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AbstractTypeClass::Get_Ownable(void) const
{
	return(HOUSEF_ALLIES | HOUSEF_SOVIET | HOUSEF_OTHERS);
}



