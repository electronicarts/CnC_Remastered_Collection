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

/* $Header: /CounterStrike/AIRCRAFT.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : AIRCRAFT.H                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : July 22, 1994                                                *
 *                                                                                             *
 *                  Last Update : November 28, 1994 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include	"radio.h"
#include	"fly.h"
#include	"target.h"


/*
**	This aircraft class is used for all flying sentient objects. This includes fixed wing
**	aircraft as well as helicopters. It excludes bullets even though some bullets might
**	be considered to be "flying" in a loose interpretatin of the word.
*/
class AircraftClass : public FootClass, public FlyClass
{
	public:
		/*
		**	This is a pointer to the class control structure for the aircraft.
		*/
		CCPtr<AircraftTypeClass> Class;

		//-----------------------------------------------------------------------------
		static void * operator new(size_t);
		static void * operator new(size_t, void * ptr) {return(ptr);};
		static void operator delete(void *);
		operator AircraftType(void) const {return Class->Type;};
		AircraftClass(AircraftType classid, HousesType house);
		AircraftClass(NoInitClass const & x) : FootClass(x), FlyClass(x), Class(x), SecondaryFacing(x), SightTimer(x) {};
		virtual ~AircraftClass(void);

		static void Init(void);

		virtual int Mission_Attack(void);
		virtual int Mission_Unload(void);
		virtual int Mission_Hunt(void);
		virtual int Mission_Retreat(void);
		virtual int Mission_Move(void);
		virtual int Mission_Enter(void);
		virtual int Mission_Guard(void);
		virtual int Mission_Guard_Area(void);

		virtual void Assign_Destination(TARGET target);
		/*
		**	State machine support routines.
		*/
		bool Process_Take_Off(void);
		bool Process_Landing(void);
		int Process_Fly_To(bool slowdown, TARGET dest);

		/*
		**	Query functions.
		*/
		virtual LayerType In_Which_Layer(void) const;
		virtual DirType Turret_Facing(void) const {return(SecondaryFacing.Current());}
		int Shape_Number(void) const;
		virtual MoveType Can_Enter_Cell(CELL cell, FacingType facing=FACING_NONE) const;
		virtual ObjectTypeClass const & Class_Of(void) const {return *Class;};
		virtual ActionType What_Action(ObjectClass const * target) const;
		virtual ActionType What_Action(CELL cell) const;
		virtual DirType Desired_Load_Dir(ObjectClass * passenger, CELL & moveto) const;
		virtual int Pip_Count(void) const;
		TARGET Good_Fire_Location(TARGET target) const;
		bool Cell_Seems_Ok(CELL cell, bool landing=false) const;
		DirType Pose_Dir(void) const;
		TARGET Good_LZ(void) const;
		virtual DirType Fire_Direction(void) const;
		virtual FireErrorType Can_Fire(TARGET target, int which) const;

		/*
		**	Landing zone support functionality.
		*/
		virtual void Per_Cell_Process(PCPType why);
		bool Is_LZ_Clear(TARGET target) const;
		TARGET New_LZ(TARGET oldlz) const;

		/*
		**	Coordinate inquiry functions. These are used for both display and
		**	combat purposes.
		*/
		virtual COORDINATE Sort_Y(void) const;

		/*
		**	Object entry and exit from the game system.
		*/
		virtual bool Unlimbo(COORDINATE , DirType facing = DIR_N);

		/*
		**	Display and rendering support functionality. Supports imagery and how
		**	object interacts with the map and thus indirectly controls rendering.
		*/
		virtual void Look(bool incremental=false);
		void Draw_Rotors(int x, int y, WindowNumberType window) const;
		virtual int Exit_Object(TechnoClass *);
		virtual short const * Overlap_List(bool redraw=false) const;
		virtual void Draw_It(int x, int y, WindowNumberType window) const;
		virtual void Set_Speed(int speed);

		/*
		**	User I/O.
		*/
		virtual void Active_Click_With(ActionType action, ObjectClass * object);
		virtual void Active_Click_With(ActionType action, CELL cell);
		virtual void Player_Assign_Mission(MissionType mission, TARGET target=TARGET_NONE, TARGET destination=TARGET_NONE);
		virtual void Response_Select(void);
		virtual void Response_Move(void);
		virtual void Response_Attack(void);

		/*
		**	Combat related.
		*/
		virtual ResultType Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source, bool forced=false);
		virtual BulletClass * Fire_At(TARGET target, int which);

		/*
		**	AI.
		*/
		bool Landing_Takeoff_AI(void);
		bool Edge_Of_World_AI(void);
		void Movement_AI(void);
		void Rotation_AI(void);
		int Paradrop_Cargo(void);
		virtual void AI(void);
		virtual void Enter_Idle_Mode(bool initial = false);
		virtual RadioMessageType Receive_Message(RadioClass * from, RadioMessageType message, long & param);
		virtual void Scatter(COORDINATE threat, bool forced=false, bool nokidding=false);

		/*
		**	Scenario and debug support.
		*/
		#ifdef CHEAT_KEYS
		virtual void Debug_Dump(MonoClass *mono) const;
		#endif

		/*
		**	File I/O.
		*/
		static void Read_INI(CCINIClass & ini);
		static char * INI_Name(void) {return "AIRCRAFT";};
		bool Load(Straw & file);
		bool Save(Pipe & file) const;

		virtual unsigned Spied_By() const;

	public:

		/*
		**	This is the facing used for the body of the aircraft. Typically, this is the same
		**	as the PrimaryFacing, but in the case of helicopters, it can be different.
		*/
		FacingClass SecondaryFacing;

		/*
		**	If this is a passenger carrying aircraft then this flag will be set. This is
		**	necessary because once the passengers are unloaded, the fact that it was a
		**	passenger carrier must still be known.
		*/
		bool Passenger;

	private:

		/*
		**	Aircraft can be in either state of landing, taking off, or in steady altitude.
		**	These flags are used to control transition between flying and landing. It is
		**	necessary to handle the transition in this manner so that it occurs smoothly
		**	during the graphic processing section.
		*/
		unsigned IsLanding:1;
		unsigned IsTakingOff:1;

		/*
		**	It is very common for aircraft to be homing in on a target. When this flag is
		**	true, the aircraft will constantly adjust its facing toward the TarCom. When the
		**	target is very close (one cell away or less), then this flag is automatically cleared.
		**	This is because the homing algorithm is designed to get the aircraft to the destination
		**	but no more. Checking when this flag is cleared is a way of flagging transition into
		**	a new mode. Example: Transport helicopters go into a hovering into correct position
		**	mode when the target is reached.
		*/
		unsigned IsHoming:1;

		/*
		**	Helicopters that are about to land must hover into a position exactly above the landing
		**	zone. When this flag is true, the aircraft will be adjusted so that it is exactly over
		**	the TarCom. The facing of the aircraft is not altered by this movement. The affect
		**	like the helicopter is hovering and shifting sideways to position over the landing
		**	zone. When the position is over the landing zone, then this flag is set to false.
		*/
		unsigned IsHovering:1;

		/*
		**	This is the jitter tracker to be used when the aircraft is a helicopter and
		**	is flying. It is most noticeable when the helicopter is hovering.
		*/
		unsigned char Jitter;

	private:

		/*
		**	This timer controls when the aircraft will reveal the terrain around itself.
		**	When this timer expires and this aircraft has a sight range, then the
		**	look around process will occur.
		*/
		CDTimerClass<FrameTimerClass> SightTimer;

		/*
		**	Most attack aircraft can make several attack runs. This value contains the
		**	number of attack runs the aircraft has left. When this value reaches
		**	zero then the aircraft is technically out of ammo.
		*/
		char AttacksRemaining;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];
};

#endif