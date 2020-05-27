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

/* $Header: /CounterStrike/UNIT.H 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : UNIT.H                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 14, 1994                                               *
 *                                                                                             *
 *                  Last Update : April 14, 1994   [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef UNIT_H
#define UNIT_H

#include "drive.h"
#include "radio.h"
#include "cargo.h"
#include "mission.h"
#include	"target.h"


/****************************************************************************
**	For each instance of a unit (vehicle) in the game, there is one of
**	these structures. This structure holds information that is specific
**	and dynamic for a particular unit.
*/
class UnitClass :	public DriveClass
{
	public:

		/*
		**	This points to the static control data that gives 'this' unit its characteristics.
		*/
		CCPtr<UnitTypeClass> Class;

		/*
		**	This records the house flag that this object is currently carrying.
		*/
		HousesType Flagged;

		/*
		** This flag is used for when the harvester dumps ore, to track its
		** special animation.
		*/
		unsigned IsDumping:1;

		/*
		** This is a count of the # of loads of the various minerals that the
		** unit has harvested.
		*/
		unsigned Gold:5;
		unsigned Gems:5;

		/*
		** This flag tells a unit that, if after reaching its destination, it
		** should scatter away.  It's meant to help a LST unload its units by
		** having its previous passengers get out of the way.
		*/
		unsigned IsToScatter:1;

		/*
		**	This records the number of "loads" of Tiberium the unit is carrying. Only
		**	harvesters use this field.
		*/
		int Tiberium;

		/*
		** This is the area where a mobile gap generator stores the previously-held
		** shroud values for the cells surrounding itself.
		*/
		unsigned long ShroudBits;

		/*
		** This is the center coordinate for the mobile gap generator, as to
		** what cells should be revealed (according to ShroudBits)
		*/
		CELL ShroudCenter;

		/*
		**	This is the timer that controls the reload rate. The MSAM rocket
		**	launcher is the primary user of this.
		*/
		CDTimerClass<FrameTimerClass> Reload;

		/*
		**	This is the facing of the turret. It can be, and usually is,
		**	rotated independently of the body it is attached to.
		*/
		FacingClass SecondaryFacing;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		static void * operator new(size_t size);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void *ptr);
		UnitClass(UnitType classid, HousesType house);
		UnitClass(NoInitClass const & x) : DriveClass(x), Class(x), Reload(x), SecondaryFacing(x) {};
		operator UnitType(void) const {return Class->Type;};
		virtual ~UnitClass(void);

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		virtual ObjectTypeClass const & Class_Of(void) const;
		static void Init(void);

		bool Goto_Clear_Spot(void);
		bool Try_To_Deploy(void);
		virtual void Scatter(COORDINATE threat, bool forced=false, bool nokidding=false);

		bool Tiberium_Check(CELL &center, int x, int y);
		bool Flag_Attach(HousesType house);
		bool Flag_Remove(void);
		bool Goto_Tiberium(int radius);
		bool Harvesting(void);
		void APC_Close_Door(void);
		void APC_Open_Door(void);

		/*
		**	Query functions.
		*/
		bool Should_Crush_It(TechnoClass const * it) const;
		int Credit_Load(void) const;
		virtual DirType Turret_Facing(void) const {if (Class->IsTurretEquipped) return(SecondaryFacing.Current());return(PrimaryFacing.Current());}
		int Shape_Number(void) const;
		virtual int Pip_Count(void) const;
		virtual InfantryType Crew_Type(void) const;
		virtual DirType Fire_Direction(void) const;
		virtual bool Ok_To_Move(DirType facing) const;
		virtual FireErrorType Can_Fire(TARGET target, int which) const;
		virtual fixed Tiberium_Load(void) const;

		/*
		**	Coordinate inquiry functions. These are used for both display and
		**	combat purposes.
		*/
		virtual COORDINATE Sort_Y(void) const;

		/*
		**	Object entry and exit from the game system.
		*/
		virtual bool Limbo(void);
		virtual bool Unlimbo(COORDINATE , DirType facing=DIR_N);

		/*
		**	Display and rendering support functionality. Supports imagery and how
		**	object interacts with the map and thus indirectly controls rendering.
		*/
		virtual short const * Overlap_List(bool redraw=false) const;
		virtual void Draw_It(int x, int y, WindowNumberType window) const;

		/*
		**	User I/O.
		*/
		virtual ActionType What_Action(CELL cell) const;
		virtual ActionType What_Action(ObjectClass const * object) const;
		virtual void Active_Click_With(ActionType action, ObjectClass * object);
		virtual void Active_Click_With(ActionType action, CELL cell);

		/*
		**	Combat related.
		*/
		virtual ResultType Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source=0, bool forced=false);
		virtual BulletClass * Fire_At(TARGET target, int which=0);

		/*
		**	Driver control support functions. These are used to control cell
		**	occupation flags and driver instructions.
		*/
		virtual bool Start_Driver(COORDINATE & coord);

		/*
		**	AI.
		*/
		virtual TARGET Greatest_Threat(ThreatType threat) const;
		virtual DirType Desired_Load_Dir(ObjectClass * passenger, CELL & moveto) const;
		virtual RadioMessageType Receive_Message(RadioClass * from, RadioMessageType message, long & param);
		virtual void AI(void);
		virtual int Mission_Guard_Area(void);
		virtual int Mission_Unload(void);
		virtual int Mission_Guard(void);
		virtual int Mission_Harvest(void);
		virtual int Mission_Hunt(void);
		virtual int Mission_Repair(void);
		virtual int Mission_Move(void);
		void Rotation_AI(void);
		void Firing_AI(void);
		void Reload_AI(void);
		bool Edge_Of_World_AI(void);

		/*
		**	Scenario and debug support.
		*/
		#ifdef CHEAT_KEYS
		virtual void Debug_Dump(MonoClass *mono) const;
		#endif

		/*
		**	Movement and animation.
		*/
		virtual void Assign_Destination(TARGET target);
		virtual void Overrun_Square(CELL cell, bool threaten=true);
		virtual void Approach_Target(void);
		virtual int Offload_Tiberium_Bail(void);
		virtual void Enter_Idle_Mode(bool initial=false);
		virtual MoveType Can_Enter_Cell(CELL cell, FacingType facing=FACING_NONE) const;
		virtual void Per_Cell_Process(PCPType why);
		void Exit_Repair(void);
		void Shroud_Regen(void);

		/*
		**	File I/O.
		*/
		static void Read_INI(CCINIClass & ini);
		static void Write_INI(CCINIClass & ini);
		static char *INI_Name(void) {return "UNITS";};
		bool Load(Straw & file);
		bool Save(Pipe & file) const;
};

#endif
