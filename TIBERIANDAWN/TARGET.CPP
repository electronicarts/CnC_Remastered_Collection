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

/* $Header:   F:\projects\c&c\vcs\code\target.cpv   2.17   16 Oct 1995 16:51:06   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TARGET.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : August 27, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   As_Aircraft -- Converts the target value into an aircraft pointer.                        *
 *   As_Animation -- Converts target value into animation pointer.                             *
 *   As_Building -- Converts a target value into a building object pointer.                    *
 *   As_Bullet -- Converts the target into a bullet pointer.                                   *
 *   As_Cell -- Converts a target value into a cell number.                                    *
 *   As_Coord -- Converts a target value into a coordinate value.                              *
 *   As_Infantry -- If the target is infantry, return a pointer to it.                         *
 *   As_Movement_Coord -- Fetches coordinate if trying to move to this target.                 *
 *   As_Object -- Converts a target value into an object pointer.                              *
 *   As_Team -- Converts a target number into a team pointer.                                  *
 *   As_TeamType -- Converts a target into a team type pointer.                                *
 *   As_Techno -- Converts a target value into a TechnoClass pointer.                          *
 *   As_Trigger -- Converts specified target into a trigger pointer.                           *
 *   As_Unit -- Converts a target value into a unit pointer.                                   *
 *   Target_Legal -- Determines if the specified target is legal.                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"target.h"


/***********************************************************************************************
 * As_Trigger -- Converts specified target into a trigger pointer.                             *
 *                                                                                             *
 *    This routine will convert the specified target number into a trigger pointer.            *
 *                                                                                             *
 * INPUT:   target   -- The target number to convert.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with the trigger pointer that the specified target number represents. If   *
 *          it doesn't represent a legal trigger object, then NULL is returned.                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TriggerClass * As_Trigger(TARGET target, bool check_active)
{
	TriggerClass* trigger = Is_Target_Trigger(target) ? Triggers.Raw_Ptr(Target_Value(target)) : NULL;
	if (check_active && trigger != NULL && !trigger->IsActive) {
		trigger = NULL;
	}
	return(trigger);
}


/***********************************************************************************************
 * As_Team -- Converts a target number into a team pointer.                                    *
 *                                                                                             *
 *    This routine will convert the specified target number into a team pointer.               *
 *                                                                                             *
 * INPUT:   target   -- The target number to convert.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with the team object that the specified target number represents. If it    *
 *          doesn't represent a legal team then NULL is returned.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TeamClass * As_Team(TARGET target, bool check_active)
{
	TeamClass* team = Is_Target_Team(target) ? Teams.Raw_Ptr(Target_Value(target)) : NULL;
	if (check_active && team != NULL && !team->IsActive) {
		team = NULL;
	}
	return(team);
}


/***********************************************************************************************
 * As_TeamType -- Converts a target into a team type pointer.                                  *
 *                                                                                             *
 *    This routine will convert the specified target number into a team type pointer.          *
 *                                                                                             *
 * INPUT:   target   -- The target number to convert.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the team type represented by the target number. If the   *
 *          target number doesn't represent a legal team type, then NULL is returned.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TeamTypeClass * As_TeamType(TARGET target)
{
	return(Is_Target_TeamType(target) ? TeamTypes.Raw_Ptr(Target_Value(target)) : NULL);
}


/***********************************************************************************************
 * As_Animation -- Converts target value into animation pointer.                               *
 *                                                                                             *
 *    This routine will convert the specified target number into an animation pointer.         *
 *                                                                                             *
 * INPUT:   target   -- The target number to convert into an animation pointer.                *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the legal animation that this target represents. If it   *
 *          doesn't represent a legal animation, then NULL is returned.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
AnimClass * As_Animation(TARGET target, bool check_active)
{
	AnimClass* anim = Is_Target_Animation(target) ? Anims.Raw_Ptr(Target_Value(target)) : NULL;
	if (check_active && anim != NULL && !anim->IsActive) {
		anim = NULL;
	}
	return(anim);
}


/***********************************************************************************************
 * As_Bullet -- Converts the target into a bullet pointer.                                     *
 *                                                                                             *
 *    This routine will convert the specified target number into a bullet pointer.             *
 *                                                                                             *
 * INPUT:   target   -- The target number to convert.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the bullet it specifies. If the target doesn't refer to  *
 *          a legal bullet, then NULL is returned.                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
BulletClass * As_Bullet(TARGET target, bool check_active)
{
	BulletClass* bullet = Is_Target_Bullet(target) ? Bullets.Raw_Ptr(Target_Value(target)) : NULL;
	if (check_active && bullet != NULL && !bullet->IsActive) {
		bullet = NULL;
	}
	return(bullet);
}


/***********************************************************************************************
 * As_Aircraft -- Converts the target value into an aircraft pointer.                          *
 *                                                                                             *
 *    This routine will convert the specified target value into an aircraft object pointer.    *
 *                                                                                             *
 * INPUT:   target   -- The target value to convert.                                           *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the aircraft that this target value represents. If the   *
 *          specified target value doesn't represent an aircraft, then NULL is returned.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
AircraftClass * As_Aircraft(TARGET target, bool check_active)
{
	AircraftClass* aircraft = Is_Target_Aircraft(target) ? Aircraft.Raw_Ptr(Target_Value(target)) : NULL;
	if (check_active && aircraft != NULL && !aircraft->IsActive) {
		aircraft = NULL;
	}
	return(aircraft);
}


/***********************************************************************************************
 * As_Techno -- Converts a target value into a TechnoClass pointer.                            *
 *                                                                                             *
 *    This routine will take the target value specified and convert it into a TechnoClass      *
 *    pointer if the target represents an object that has a TechnoClass.                       *
 *                                                                                             *
 * INPUT:   target   -- The target value to convert into a TechnoClass pointer.                *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the associated object's TechnoClass. If the target       *
 *          cannot be converted into a TechnoClass pointer, then NULL is returned.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TechnoClass * As_Techno(TARGET target, bool check_active)
{
	ObjectClass * obj = As_Object(target, check_active);

	if (obj && obj->Is_Techno()) {
		return(TechnoClass *)obj;
	}
	return(NULL);
}


/***********************************************************************************************
 * As_Object -- Converts a target value into an object pointer.                                *
 *                                                                                             *
 *    This routine is used to convert the target value specified into an object pointer. If    *
 *    the target doesn't represent an object or the target value is illegal, then NULL is      *
 *    returned.                                                                                *
 *                                                                                             *
 * INPUT:   target   -- The target value to convert from.                                      *
 *          check_active -- Check if the target is active, return NULL if not.                 *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the object it represent, or NULL if not an object.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * As_Object(TARGET target, bool check_active)
{
	int val = Target_Value(target);
	ObjectClass * object = NULL;

	switch (Target_Kind(target)) {
		case KIND_INFANTRY:
			object = Infantry.Raw_Ptr(val);
			break;

		case KIND_UNIT:
			object = Units.Raw_Ptr(val);
			break;

		case KIND_BUILDING:
			object = Buildings.Raw_Ptr(val);
			break;

		case KIND_AIRCRAFT:
			object = Aircraft.Raw_Ptr(val);
			break;

		case KIND_TERRAIN:
			object = Terrains.Raw_Ptr(val);
			break;

		case KIND_BULLET:
			object = Bullets.Raw_Ptr(val);
			break;

		case KIND_ANIMATION:
			object = Anims.Raw_Ptr(val);
			break;

		default:
			break;
	}

	/*
	**	Special check to ensure that a target value that references an
	**	invalid object will not be converted back into an object pointer.
	**	This condition is rare, but could occur in a network game if the
	**	object it refers to is destroyed between the time an event message
	**	is sent and when it is received.
	*/
	if (check_active && object != NULL && !object->IsActive) {
		object = NULL;
	}

	return(object);
}


/***********************************************************************************************
 * As_Unit -- Converts a target value into a unit pointer.                                     *
 *                                                                                             *
 *    This routine is used to convert the target value specified into a pointer to a unit      *
 *    object.                                                                                  *
 *                                                                                             *
 * INPUT:   target   -- The target value to convert into a unit pointer.                       *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the unit the target value represents or NULL if not      *
 *          a unit.                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
UnitClass * As_Unit(TARGET target, bool check_active)
{
	UnitClass* unit = Is_Target_Unit(target) ? Units.Raw_Ptr(Target_Value(target)) : NULL;
	if (check_active && unit != NULL && !unit->IsActive) {
		unit = NULL;
	}
	return(unit);
}


/***********************************************************************************************
 * As_Infantry -- If the target is infantry, return a pointer to it.                           *
 *                                                                                             *
 *    This routine will translate the specified target value into an infantry pointer if the   *
 *    target actually represents an infantry object.                                           *
 *                                                                                             *
 * INPUT:   target   -- The target to convert to a pointer.                                    *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to the infantry object that this target value represents. If     *
 *          the target doesn't represent an infantry object, then return NULL.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryClass * As_Infantry(TARGET target, bool check_active)
{
	InfantryClass* infantry = Is_Target_Infantry(target) ? Infantry.Raw_Ptr(Target_Value(target)) : NULL;
	if (check_active && infantry != NULL && !infantry->IsActive) {
		infantry = NULL;
	}
	return(infantry);
}


#ifdef NEVER
TerrainClass * As_Terrain(TARGET target)
{
	return(Is_Target_Terrain(target) ? &Terrains[Target_Value(target)] : NULL);
}
#endif


/***********************************************************************************************
 * As_Building -- Converts a target value into a building object pointer.                      *
 *                                                                                             *
 *    This routine is used to convert the target value specified into a building pointer.      *
 *                                                                                             *
 * INPUT:   target   -- The target value to convert from.                                      *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building object that the target value represents.    *
 *          If it doesn't represent a building, then return NULL.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingClass * As_Building(TARGET target, bool check_active)
{
	BuildingClass* building = Is_Target_Building(target) ? Buildings.Raw_Ptr(Target_Value(target)) : NULL;
	if (check_active && building != NULL && !building->IsActive) {
		building = NULL;
	}
	return(building);
}


/***********************************************************************************************
 * Target_Legal -- Determines if the specified target is legal.                                *
 *                                                                                             *
 *    This routine is used to check for the legality of the target value specified. It is      *
 *    necessary to call this routine if there is doubt about the the legality of the target.   *
 *    It is possible for the unit that a target value represents to be eliminated and thus     *
 *    rendering the target value invalid.                                                      *
 *                                                                                             *
 * INPUT:   target   -- The target value to check.                                             *
 *                                                                                             *
 * OUTPUT:  bool; Is the target value legal?                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Target_Legal(TARGET target)
{
	if (target == TARGET_NONE) return(false);

	ObjectClass * obj = As_Object(target, false);
	if (obj) {
		return(obj->IsActive);
	}
	return(true);
}


/***********************************************************************************************
 * As_Cell -- Converts a target value into a cell number.                                      *
 *                                                                                             *
 *    This routine is used to convert the target value specified, into a cell value. This is   *
 *    necessary for find path and other procedures that need a cell value.                     *
 *                                                                                             *
 * INPUT:   target   -- The target value to convert to a cell value.                           *
 *                                                                                             *
 * OUTPUT:  Returns with the target value expressed as a cell location.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL As_Cell(TARGET target)
{
	return(Coord_Cell(As_Coord(target)));
}


/***********************************************************************************************
 * As_Coord -- Converts a target value into a coordinate value.                                *
 *                                                                                             *
 *    This routine is used to convert the target value specified into a coordinate value. It   *
 *    is necessary for those procedures that require a coordinate value.                       *
 *                                                                                             *
 * INPUT:   target   -- The target value to convert.                                           *
 *                                                                                             *
 * OUTPUT:  Returns with the target expressed as a COORD value.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1994 JLB : Created.                                                                 *
 *   11/16/1994 JLB : Simplified.                                                              *
 *=============================================================================================*/
COORDINATE As_Coord(TARGET target)
{
	if (Target_Legal(target)) {
		/*
		**	Cell target values are handled as a special case. The value of the target number is
		**	actually the cell index number.
		*/
		if (Is_Target_Cell(target)) {
			return(Cell_Coord((CELL)Target_Value(target)));
		}

		/*
		**	Normal targets correspond to game objects. Fetch the object pointer and then ask it
		**	for the center coordinate. Return the center coordinate as the target's coordinate.
		*/
		ObjectClass * obj = As_Object(target);
		if (obj) {

			/*
			** If this is invalid memory or the object is dead then return 0
			** This is a kludge to fix the problem of team target objects being assigned after
			** the object is already destroyed - 1/15/97 3:13PM
			*/
			if (IsBadReadPtr ((void*)obj, sizeof (ObjectClass) ) || !obj->IsActive){
//OutputDebugString ("C&C95 - As_Coord called for invalid target object\m");
				return(0x00000000L);
			}

			return(obj->Target_Coord());
		}
	}

	/*
	**	An unrecognized target value results in a null coordinate value.
	*/
	return(0x00000000L);
}


/***********************************************************************************************
 * As_Movement_Coord -- Fetches coordinate if trying to move to this target.                   *
 *                                                                                             *
 *    This routine will convert the specified target into a coordinate location. This location *
 *    is used when moving to the target specified. For cells, this is the center of the cell.  *
 *    For special buildings that allow docking, it is the center location of the docking       *
 *    bay.                                                                                     *
 *                                                                                             *
 * INPUT:   target   -- The target to convert into a coordinate value.                         *
 *                                                                                             *
 * OUTPUT:  Returns with the docking coordinate of the target value specified.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE As_Movement_Coord(TARGET target)
{
	if (Target_Legal(target)) {
		/*
		**	Cell target values are handled as a special case. The value of the target number is
		**	actually the cell index number.
		*/
		if (Is_Target_Cell(target)) {
			return(Cell_Coord((CELL)Target_Value(target)));
		}

		/*
		**	Normal targets correspond to game objects. Fetch the object pointer and then ask it
		**	for the center coordinate. Return the center coordinate as the target's coordinate.
		*/
		ObjectClass * obj = As_Object(target);
		if (obj) {
			return(obj->Docking_Coord());
		}
	}

	/*
	**	An unrecognized target value results in a null coordinate value.
	*/
	return(0x00000000L);
}
