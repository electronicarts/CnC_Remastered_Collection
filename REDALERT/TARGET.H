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

/* $Header: /CounterStrike/TARGET.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TARGET.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 25, 1994                                               *
 *                                                                                             *
 *                  Last Update : April 25, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef TARGET_H
#define TARGET_H


inline RTTIType Target_Kind(TARGET a)
{
	return(RTTIType(((TARGET_COMPOSITE &)a).Sub.Exponent));
}

inline unsigned Target_Value(TARGET a)
{
	return(((TARGET_COMPOSITE &)a).Sub.Mantissa);
}

inline bool Is_Target_Team(TARGET a) {return (Target_Kind(a) == RTTI_TEAM);}
inline bool Is_Target_TeamType(TARGET a) {return (Target_Kind(a) == RTTI_TEAMTYPE);}
inline bool Is_Target_Trigger(TARGET a) {return (Target_Kind(a) == RTTI_TRIGGER);}
inline bool Is_Target_TriggerType(TARGET a) {return (Target_Kind(a) == RTTI_TRIGGERTYPE);}
inline bool Is_Target_Infantry(TARGET a) {return (Target_Kind(a) == RTTI_INFANTRY);}
inline bool Is_Target_Bullet(TARGET a) {return (Target_Kind(a) == RTTI_BULLET);}
inline bool Is_Target_Terrain(TARGET a) {return (Target_Kind(a) == RTTI_TERRAIN);}
inline bool Is_Target_Cell(TARGET a) {return (Target_Kind(a) == RTTI_CELL);}
inline bool Is_Target_Unit(TARGET a) {return (Target_Kind(a) == RTTI_UNIT);}
inline bool Is_Target_Vessel(TARGET a) {return (Target_Kind(a) == RTTI_VESSEL);}
inline bool Is_Target_Building(TARGET a) {return (Target_Kind(a) == RTTI_BUILDING);}
inline bool Is_Target_Template(TARGET a) {return (Target_Kind(a) == RTTI_TEMPLATE);}
inline bool Is_Target_Aircraft(TARGET a) {return (Target_Kind(a) == RTTI_AIRCRAFT);}
inline bool Is_Target_Animation(TARGET a) {return (Target_Kind(a) == RTTI_ANIM);}
inline bool Is_Target_Object(TARGET a)
{
	return (Target_Kind(a) == RTTI_TERRAIN ||
			Target_Kind(a) == RTTI_UNIT ||
			Target_Kind(a) == RTTI_VESSEL ||
			Target_Kind(a) == RTTI_INFANTRY ||
			Target_Kind(a) == RTTI_BUILDING ||
			Target_Kind(a) == RTTI_AIRCRAFT);
}


TARGET As_Target(CELL cell);
TARGET As_Target(COORDINATE coord);
//inline TARGET As_Target(CELL cell) {return (TARGET)(((unsigned)RTTI_CELL << TARGET_MANTISSA) | cell);}

class UnitClass;
class BuildingClass;
class TechnoClass;
class TerrainClass;
class ObjectClass;
class InfantryClass;
class BulletClass;
class TriggerClass;
class TeamClass;
class TeamTypeClass;
class AnimClass;
class AircraftClass;
class VesselClass;
class CellClass;
class TriggerTypeClass;

/*
** Must not have a constructor since Watcom cannot handle a class that has a constructor if
** that class object is in a union. Don't use this class for normal purposes. Use the TargetClass
**	instead. The xTargetClass is only used in one module for a special reason -- keep it that way.
*/
class xTargetClass
{
	protected:

		TARGET_COMPOSITE Target;

	public:

		// conversion operator to RTTIType
		operator RTTIType (void) const {return(RTTIType(Target.Sub.Exponent));}

		// comparison operator
		int operator == (xTargetClass & tgt) {return (tgt.Target.Target==Target.Target ? 1 : 0);}

		// conversion operator to regular TARGET type
		TARGET As_TARGET(void) const {return(Target.Target);}

		unsigned Value(void) const {return(Target.Sub.Mantissa);};

		void Invalidate(void) {Target.Sub.Exponent = RTTI_NONE;Target.Sub.Mantissa = -1;}
		bool Is_Valid(void) const {return (Target.Sub.Exponent != RTTI_NONE);}

		TARGET As_Target(void) const {return(Target.Target);}
		AbstractTypeClass * As_TypeClass(void) const;
		AbstractClass * As_Abstract(bool check_active = true) const;
		TechnoClass * As_Techno(bool check_active = true) const;
		ObjectClass * As_Object(bool check_active = true) const;
		CellClass * As_Cell(void) const;

		/*
		**	Helper routines to combine testing for, and fetching a pointer to, the
		**	type of object indicated.
		*/
		TriggerTypeClass * As_TriggerType(void) const {if (*this == RTTI_TRIGGERTYPE) return((TriggerTypeClass *)As_TypeClass());return(0);}
		TeamTypeClass * As_TeamType(void) const {if (*this == RTTI_TEAMTYPE) return((TeamTypeClass *)As_TypeClass());return(0);}
		TerrainClass * As_Terrain(bool check_active = true) const {if (*this == RTTI_TERRAIN) return((TerrainClass *)As_Abstract(check_active));return(0);}
		BulletClass * As_Bullet(bool check_active = true) const {if (*this == RTTI_BULLET) return((BulletClass *)As_Abstract(check_active));return(0);}
		AnimClass * As_Anim(bool check_active = true) const {if (*this == RTTI_ANIM) return((AnimClass *)As_Abstract(check_active));return(0);}
		TeamClass * As_Team(bool check_active = true) const {if (*this == RTTI_TEAM) return((TeamClass *)As_Abstract(check_active));return(0);}
		InfantryClass * As_Infantry(bool check_active = true) const {if (*this == RTTI_INFANTRY) return((InfantryClass *)As_Techno(check_active));return(0);}
		UnitClass * As_Unit(bool check_active = true) const {if (*this == RTTI_UNIT) return((UnitClass *)As_Techno(check_active));return(0);}
		BuildingClass * As_Building(bool check_active = true) const {if (*this == RTTI_BUILDING) return((BuildingClass *)As_Techno(check_active));return(0);}
		AircraftClass * As_Aircraft(bool check_active = true) const {if (*this == RTTI_AIRCRAFT) return((AircraftClass *)As_Techno(check_active));return(0);}
		VesselClass * As_Vessel(bool check_active = true) const {if (*this == RTTI_VESSEL) return((VesselClass *)As_Techno(check_active));return(0);}
};

/*
**	This class only serves as a wrapper to the xTargetClass. This class must not define any members except
**	for the constructors. This is because the xTargetClass is used in a union and this target object is
**	used as its initializer. If this class had any extra members they would not be properly copied and
**	communicated to the other machines in a network/modem game. Combining this class with xTargetClass would
**	be more efficient, but Watcom doesn't allow class objects that have a constructor to be part of a union [even
**	if the class object has a default constructor!].
*/
class TargetClass : public xTargetClass
{
	public:

		TargetClass(void) {Invalidate();}
		TargetClass(NoInitClass const &) {}
		TargetClass(RTTIType rtti, int id) {
			Target.Sub.Exponent = rtti;
			Target.Sub.Mantissa = id;
		}
		TargetClass(CELL cell) {
			Target.Sub.Exponent = RTTI_CELL;
			Target.Sub.Mantissa = cell;
		}
		TargetClass(TARGET target);
		TargetClass(AbstractClass const * ptr);
		TargetClass(AbstractTypeClass const * ptr);
		TargetClass(CellClass const * ptr);
};

#endif
