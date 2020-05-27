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

/* $Header: /CounterStrike/CARRY.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CARRY.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 02/26/96                                                     *
 *                                                                                             *
 *                  Last Update : May 10, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CarryoverClass::CarryoverClass -- Constructor for carry over objects.                     *
 *   CarryoverClass::Create -- Creates a carried over object.                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"


/***********************************************************************************************
 * CarryoverClass::CarryoverClass -- Constructor for carry over objects.                       *
 *                                                                                             *
 *    This is the constructor for a carry over object. Such an object is used to record the    *
 *    object that will be "carried over" into a new scenario at some future time.              *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object that will be carried over.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
CarryoverClass::CarryoverClass(TechnoClass * techno) :
	RTTI(RTTI_NONE),
	Cell(0),
	Strength(0),
	House(HOUSE_NONE)
{
	if (techno) {
		RTTI = techno->What_Am_I();

		switch (RTTI) {
			case RTTI_UNIT:
				Type.Unit = ((UnitClass *)techno)->Class->Type;
				break;

			case RTTI_BUILDING:
				Type.Building = ((BuildingClass *)techno)->Class->Type;
				break;

			case RTTI_INFANTRY:
				Type.Infantry = ((InfantryClass *)techno)->Class->Type;
				break;

			case RTTI_VESSEL:
				Type.Vessel = ((VesselClass *)techno)->Class->Type;
				break;

			default:
				break;
		}

		House = techno->Owner();
		Strength = techno->Strength;
		Cell = Coord_Cell(techno->Coord);
	}
}


/***********************************************************************************************
 * CarryoverClass::Create -- Creates a carried over object.                                    *
 *                                                                                             *
 *    Use this routine to convert a carried over object into an actual object that will be     *
 *    placed on the map.                                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the object successfully created and placed on the map?                   *
 *                                                                                             *
 * WARNINGS:   This routine might not place the object if the old map location was invalid     *
 *             or there are other barriers to the object's creation and placement.             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool CarryoverClass::Create(void) const
{
	TechnoClass * techno = 0;
	TechnoTypeClass const * ttype = 0;

	switch (RTTI) {
		case RTTI_UNIT:
			ttype = &UnitTypeClass::As_Reference(Type.Unit);
			techno = new UnitClass(Type.Unit, House);
			break;

		case RTTI_INFANTRY:
			ttype = &InfantryTypeClass::As_Reference(Type.Infantry);
			techno = new InfantryClass(Type.Infantry, House);
			break;

		case RTTI_BUILDING:
			ttype = &BuildingTypeClass::As_Reference(Type.Building);
			techno = new BuildingClass(Type.Building, House);
			break;

		case RTTI_VESSEL:
			ttype = &VesselTypeClass::As_Reference(Type.Vessel);
			techno = new VesselClass(Type.Vessel, House);
			break;
	}

	if (techno) {
		bool oldscen = ScenarioInit;
		techno->Strength = Strength;
		if (RTTI == RTTI_INFANTRY) {
			ScenarioInit = 0;
		}
		techno->Unlimbo(Cell_Coord(Cell));
		if (RTTI == RTTI_INFANTRY) {
			ScenarioInit = oldscen;
		}
	}

	return(false);
}

