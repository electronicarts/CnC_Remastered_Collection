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

/* $Header:   F:\projects\c&c\vcs\code\abstract.cpv   2.20   16 Oct 1995 16:49:04   JOE_BOSTIC  $ */
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
 *                  Last Update : May 22, 1995 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   AbstractClass::Distance -- Determines distance to target.                                 *
 *   AbstractTypeClass::AbstractTypeClass -- Constructor for abstract type objects.            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


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
	BuildingClass	*obj = As_Building(target);
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
AbstractTypeClass::AbstractTypeClass(int name, char const * ini)
{
	Name = name;
	strncpy((char *)IniName, ini, sizeof(IniName));
	((char &)IniName[sizeof(IniName)-1]) = '\0';
}

RTTIType AbstractTypeClass::What_Am_I(void) const {return RTTI_ABSTRACTTYPE;};
COORDINATE AbstractTypeClass::Coord_Fixup(COORDINATE coord) const {return coord;}
int AbstractTypeClass::Full_Name(void) const {return Name;};
unsigned short AbstractTypeClass::Get_Ownable(void) const {return 0xffff;};




void AbstractClass::Delete_This(void)
{
	/*
	** Apparently Watcom preserved the vtable through the virtual destructor chain, otherwise C&C would crash all the time.
	**
	** MSVC doesn't. It overwrites the vtable pointer with the vtable of the class currently being destructed - presumably to 
	** prevent virtual functions of the classes already destructed from being called.
	**
	** So this is a hacky workaround to restore the original vtable pointer after the destructor chain has finished, for when the
	** C&C code wants to call a virtual function on an object it just 'deleted'.
	**
	** ST - 1/9/2019 6:02PM
	*/
	unsigned long *this_ptr = (unsigned long*) this;
	unsigned long vtable_ptr = *this_ptr;

	/*
	** delete this calls the operator delete, it doesn't actually deallocate the memory.
	*/
	delete this;

	*this_ptr = vtable_ptr;
}			  