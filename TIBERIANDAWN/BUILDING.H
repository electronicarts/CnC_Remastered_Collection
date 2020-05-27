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

/* $Header:   F:\projects\c&c\vcs\code\building.h_v   2.20   16 Oct 1995 16:47:54   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BUILDING.H                                                   *
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

#ifndef BUILDING_H
#define BUILDING_H

#include	"tarcom.h"
#include	"radio.h"
#include	"cargo.h"
#include	"mission.h"
#include	"bullet.h"
#include	"target.h"
#include	"factory.h"

#define	MAX_DOOR_STAGE	18	// # of frames of door opening on weapons factory
#define	DOOR_OPEN_STAGE 9	// frame on which the door is entirely open
#define	MAX_REPAIR_ANIM_STAGE	5	// # of stages of anim for repair center cycling

/****************************************************************************
**	For each instance of a building in the game, there is one of
**	these structures. This structure holds information that is specific
**	and dynamic for a particular building.
*/
class BuildingClass : public TechnoClass
{
	public:
		BuildingTypeClass const * const Class;
		operator StructType(void) const {return Class->Type;};

		/*
		**	If this building is in the process of producing something, then this
		**	will point to the factory manager.
		*/
		FactoryClass * Factory;

		/*
		**	This is the house that originally owned this factory. Objects buildable
		**	by this house type will be produced from this factory regardless of who
		**	the current owner is.
		*/
		HousesType ActLike;

		/*
		**	If the building is at a good point to change orders, then this
		**	flag will be set to true.
		*/
		unsigned IsReadyToCommence:1;

		/*
		**	If this building is currently spending money to repair itself, then
		**	this flag is true. It will automatically be set to false when the building
		**	has reached full strength, when money is exhausted, or if the player
		**	specifically stops the repair process.
		*/
		unsigned IsRepairing:1;

		/*
		**	If repair is currently in progress and this flag is true, then a wrench graphic
		**	will be overlaid on the building to give visual feedback for the repair process.
		*/
		unsigned IsWrenchVisible:1;

		/*
		** This flag is set when a commando has raided the building and planted
		** plastic explosives.  When the CommandoCountDown timer expires, the
		** building takes massive damage.
		*/
		unsigned IsGoingToBlow:1;

		/*
		**	If this building was destroyed by some method that would prevent
		**	survivors, then this flag will be true.
		*/
		unsigned IsSurvivorless:1;

		/*
		**	These state control variables are used by the oblisk for the charging
		**	animation.
		*/
		unsigned IsCharging:1;
		unsigned IsCharged:1;

		/*
		**	A building that has been captured will not contain the full compliment
		**	of crew. This is true even if it subsiquently gets captured back.
		*/
		unsigned IsCaptured:1;

		/*
		**	Special countdown to destruction value. If the building is destroyed,
		**	it won't actually be removed from the map until this value reaches
		**	zero. This delay is for cosmetic reasons.
		*/
		TCountDownTimerClass CountDown;

		/*
		**	This is the current animation processing state that the building is
		**	in.
		*/
		BStateType BState;
		BStateType QueueBState;

		/*
		** For multiplayer games, this keeps track of the last house to damage
		** this building, so if it burns to death or otherwise gradually dies,
		** proper credit can be given for the kill.
		*/
		HousesType WhoLastHurtMe;

		/*
		**	This is the saboteur responsible for this building's destruction.
		*/
		TARGET WhomToRepay;

		/*
		**	This is a record of the last strength of the building. Every so often,
		**	it will compare this strength to the current strength. If there is a
		**	discrepency, then the owner power is adjusted accordingly.
		*/
		int LastStrength;

		/*
		**	This is the countdown timer that regulates placement retry logic
		**	for factory type buildings.
		*/
		TCountDownTimerClass PlacementDelay;

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		static void * BuildingClass::operator new(size_t size);
		static void BuildingClass::operator delete(void *ptr);
		BuildingClass(void) : Class(0) {};
		BuildingClass(StructType type, HousesType house);
		virtual ~BuildingClass(void);
		virtual RTTIType What_Am_I(void) const {return RTTI_BUILDING;};

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		static void Init(void);

		TARGET  Target_Scan(void);
		BuildingTypeClass::AnimControlType const *  Fetch_Anim_Control(void) {return (&Class->Anims[BState]);};

		/*
		**	Query functions.
		*/
		virtual CELL Find_Exit_Cell(TechnoClass const * techno) const;
		virtual InfantryType Crew_Type(void) const;
		virtual int Pip_Count(void) const;
		virtual bool Can_Player_Move(void) const;
		virtual ActionType What_Action(ObjectClass * target) const;
		virtual ActionType What_Action(CELL cell) const;
		virtual bool Can_Demolish(void) const;
		virtual bool Can_Demolish_Unit(void) const;
		virtual ObjectTypeClass const & Class_Of(void) const {return *Class;};
		virtual int Refund_Amount(void) const;
		virtual DirType Fire_Direction(void) const;
		int  Power_Output(void) const;

		/*
		**	Coordinate inquiry functions. These are used for both display and
		**	combat purposes.
		*/
		virtual COORDINATE Docking_Coord(void) const;
		virtual COORDINATE Fire_Coord(int which) const;
		virtual COORDINATE Center_Coord(void) const;
		virtual COORDINATE Sort_Y(void) const;
		virtual COORDINATE Target_Coord(void) const;

		/*
		**	Object entry and exit from the game system.
		*/
		virtual void Detach(TARGET target, bool all);
		virtual void Detach_All(bool all=true);
		virtual void Grand_Opening(bool captured = false);
		virtual void Update_Buildables(void);
		virtual MoveType Can_Enter_Cell(CELL cell, FacingType = FACING_NONE) const;
		virtual bool Unlimbo(COORDINATE , DirType dir = DIR_N);
		virtual bool Limbo(void);
		bool Passes_Proximity_Check(CELL homecell);

		/*
		**	Display and rendering support functionality. Supports imagery and how
		**	object interacts with the map and thus indirectly controls rendering.
		*/
		virtual void const * Remap_Table(void);
		virtual int Exit_Object(TechnoClass * base);
		virtual void Draw_It(int x, int y, WindowNumberType window);
		virtual bool Mark(MarkType mark);
		virtual void Look(bool incremental=false);
		virtual void Fire_Out(void);
		void  Begin_Mode(BStateType bstate);

		/*
		**	User I/O.
		*/
		virtual void Active_Click_With(ActionType action, ObjectClass * object);
		virtual void Active_Click_With(ActionType action, CELL cell);

		/*
		**	Combat related.
		*/
		virtual void Death_Announcement(TechnoClass const * source=0) const;
		virtual FireErrorType Can_Fire(TARGET, int which) const;
		virtual TARGET Greatest_Threat(ThreatType threat) const;
		virtual ResultType Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source=0);
		virtual TARGET As_Target(void) const;
		virtual bool Captured(HouseClass * newowner);

		/*
		**	AI.
		*/
		virtual void Hidden(void);
		virtual bool Revealed(HouseClass * house);
		virtual void Repair(int control);
		virtual void Sell_Back(int control);
		virtual RadioMessageType Receive_Message(RadioClass * from, RadioMessageType message, long & param);
		virtual void AI(void);
		virtual void Assign_Target(TARGET target);
		virtual bool Toggle_Primary(void);
		bool  Flush_For_Placement(TechnoClass * techno, CELL cell);

		virtual int Mission_Unload(void);
		virtual int Mission_Repair(void);
		virtual int Mission_Attack(void);
		virtual int Mission_Harvest(void);
		virtual int Mission_Guard(void);
		virtual int Mission_Construction(void);
		virtual int Mission_Deconstruction(void);
		virtual int Mission_Missile(void);
		virtual void Enter_Idle_Mode(bool initial=false);

		/*
		**	Scenario and debug support.
		*/
		#ifdef CHEAT_KEYS
		virtual void Debug_Dump(MonoClass *mono) const;
		#endif

		/*
		**	File I/O.
		*/
		static void  Read_INI(char *buffer);
		static void  Write_INI(char *buffer);
		static char *INI_Name(void) {return "STRUCTURES";};
		bool  Load(FileClass & file);
		bool  Save(FileClass & file);
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);
		void  Update_Specials(void);

		/*
		**	Dee-buggin' support.
		*/
		int Validate(void) const;

	private:
		void  Drop_Debris(TARGET source = TARGET_NONE);
		virtual BulletClass * Fire_At(TARGET target, int which);

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];

		static COORDINATE const CenterOffset[BSIZE_COUNT];

		/*
		** This contains the value of the Virtual Function Table Pointer
		*/
		static void * VTable;
};

#endif