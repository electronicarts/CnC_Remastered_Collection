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

/* $Header:   F:\projects\c&c\vcs\code\target.h_v   2.16   16 Oct 1995 16:45:04   JOE_BOSTIC  $ */
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

/**************************************************************************
** When a unit proceeds with carrying out its mission, it can have several
**	intermediate goals. Each goal (or target if you will) can be one of the
**	following kinds.
*/
typedef enum KindType : unsigned char {
	KIND_NONE,
	KIND_CELL,
	KIND_UNIT,
	KIND_INFANTRY,
	KIND_BUILDING,
	KIND_TERRAIN,
	KIND_AIRCRAFT,
	KIND_TEMPLATE,
	KIND_BULLET,
	KIND_ANIMATION,
	KIND_TRIGGER,
	KIND_TEAM,
	KIND_TEAMTYPE
} KindType;


#define	TARGET_MANTISSA			12				// Bits of value precision.
#define	TARGET_MANTISSA_MASK		(~((~0)<<TARGET_MANTISSA))
#define	TARGET_EXPONENT			((sizeof(TARGET)*8)-TARGET_MANTISSA)
#define	TARGET_EXPONENT_MASK		(~(((unsigned)(~0))>>TARGET_EXPONENT))

inline KindType Target_Kind(TARGET a){return (KindType)(((unsigned)a)>>TARGET_MANTISSA);}
inline unsigned Target_Value(TARGET a){return (((unsigned)a) & TARGET_MANTISSA_MASK);}

inline bool Is_Target_Team(TARGET a) {return (Target_Kind(a) == KIND_TEAM);}
inline bool Is_Target_TeamType(TARGET a) {return (Target_Kind(a) == KIND_TEAMTYPE);}
inline bool Is_Target_Trigger(TARGET a) {return (Target_Kind(a) == KIND_TRIGGER);}
inline bool Is_Target_Infantry(TARGET a) {return (Target_Kind(a) == KIND_INFANTRY);}
inline bool Is_Target_Bullet(TARGET a){return (Target_Kind(a) == KIND_BULLET);}
inline bool Is_Target_Terrain(TARGET a){return (Target_Kind(a) == KIND_TERRAIN);}
inline bool Is_Target_Cell(TARGET a){return (Target_Kind(a) == KIND_CELL);}
inline bool Is_Target_Unit(TARGET a){return (Target_Kind(a) == KIND_UNIT);}
inline bool Is_Target_Building(TARGET a){return (Target_Kind(a) == KIND_BUILDING);}
inline bool Is_Target_Template(TARGET a){return (Target_Kind(a) == KIND_TEMPLATE);}
inline bool Is_Target_Aircraft(TARGET a){return (Target_Kind(a) == KIND_AIRCRAFT);}
inline bool Is_Target_Animation(TARGET a) {return (Target_Kind(a) == KIND_ANIMATION);}

inline TARGET Build_Target(KindType kind, int value) {return (TARGET)((((unsigned)kind) << TARGET_MANTISSA) | (unsigned)value);}
inline TARGET As_Target(CELL cell){return (TARGET)(((unsigned)KIND_CELL << TARGET_MANTISSA) | cell);}

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

#ifdef NEVER
class TargetClass
{
	public:
		TargetClass(void) {Target.Raw = 0;};

		/*
		**	This handles assignment from an integer and conversion
		**	to an integer.
		*/
		inline TargetClass(int val, KindType kind) {Target.Component.Value=val; Target.Component.Kind=kind;};
		inline TargetClass(int val) {Target.Raw = val;};
		inline operator int () {return Target.Raw;};
		//inline TargetClass & operator = (const int &val) {*((int*)this)=val; return *this;};

		inline bool Is_Filled(void) {return Target.Component.Kind != KIND_NONE;};
		inline void Invalidate(void) {Target.Component.Kind = 0;};
		inline bool Is_Cell(void) {return Target.Component.Kind == KIND_CELL;};
		inline bool Is_Unit(void) {return Target.Component.Kind == KIND_UNIT;};
		inline bool Is_Building(void) {return Target.Component.Kind == KIND_BUILDING;};
		inline bool Is_Aircraft(void) {return Target.Component.Kind == KIND_AIRCRAFT;};
		inline int As_Value(void) {return Target.Component.Value;};
		inline KindType As_Kind(void) {return Target.Component.Kind;};

		// Allows comparing one target to another (for equality).
		inline bool operator == (TargetClass t1) {
			return (Target.Raw == t1.Target.Raw);
		};

		UnitClass * As_Unit(void);
		BuildingClass * As_Building(void);
		bool Legal(void);
		CELL As_Cell(void);
		COORDINATE As_Coord(void);
		int Distance(TechnoClass *base);
		static int As_Target(UnitClass *unit);
		static int As_Target(BuildingClass *building);
		static int As_Target(CELL cell);

	private:

		/*
		**	This is the special encoded target value.
		*/
		union {
			struct {
				unsigned Value:TARGET_MANTISSA;
				KindType Kind:TARGET_EXPONENT;
			} Component;
			int Raw;
		} Target;


		static int Build(int value, KindType kind);
};
#endif

#endif