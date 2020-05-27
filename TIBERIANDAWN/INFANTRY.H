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

/* $Header:   F:\projects\c&c\vcs\code\infantry.h_v   2.18   16 Oct 1995 16:48:08   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : INFANTRY.H                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : August 15, 1994                                              *
 *                                                                                             *
 *                  Last Update : August 15, 1994   [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef INFANTRY_H
#define INFANTRY_H

/**********************************************************************
**	Infantry can be afraid. These defines are for the various infantry
**	fear levels. When infantry be come scared enough they take cover and
**	even run away in panic.
*/
#define	FEAR_ANXIOUS	10			// Something makes them scared.
#define	FEAR_SCARED		100		// Scared enough to take cover.
#define	FEAR_PANIC		200		// Run away! Run away!
#define	FEAR_MAXIMUM	255		// Scared to death.


class InfantryClass : public FootClass
{
	public:
		InfantryTypeClass const * const Class;
		operator InfantryType(void) const {return Class->Type;};

		/*
		**	If the infantry is undergoing some choreographed animation sequence, then
		**	this holds the particular sequence number. The frame of animation is kept
		**	track of by the regular frame tracking system. When performing an animation
		**	sequence, the infantry cannot perform anything else (even move).
		*/
		DoType Doing;

		/*
		**	Certain infantry will either perform some comment or say something after an
		**	amount of time has expired subsiquent to an significant event. This is the
		**	timer the counts down.
		*/
		TCountDownTimerClass Comment;

		/*
		**	If this civilian is actually a technician, then this flag will be true.
		**	It should only be set for the civilian type infantry. Typically, the
		**	technician appears after a building is destroyed.
		*/
		unsigned IsTechnician:1;

		/*
		**	If the infantry just performed some feat, then it may respond with an action.
		**	This flag will be true if an action is to be performed when the Comment timer
		**	has expired.
		*/
		unsigned IsStoked:1;

		/*
		**	This flag indicates if the infantry unit is prone. Prone infantry become that way
		**	when they are fired upon. Infantry in the prone position are less vulnerable to
		**	combat.
		*/
		unsigned IsProne:1;

		/*
		** This flag is set when the infantryman is engaged in hand-to-hand
		** combat.  By setting this flag, it'll play the put-down-the-gun
		** sequence only once, and it'll know to pick up the gun when the
		** fight is over.
		*/
		unsigned	IsBoxing:1;

		/*
		** Track when movement last stopped.
		*/
		long StopDriverFrame;

		/*
		**	The fear rating of this infantry unit. The more afraid the infantry, the more
		**	likely it is to panic and seek cover.
		*/
		unsigned char Fear;

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		static void * operator new(size_t size);
		static void operator delete(void *ptr);
		InfantryClass(void);
		InfantryClass(InfantryType classid, HousesType house);
		virtual ~InfantryClass(void);
		virtual RTTIType What_Am_I(void) const;

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		static void Init(void);

		virtual void Assign_Destination(TARGET);

		/*
		**	Query functions.
		*/
		virtual bool Is_Infantry(void) const;
		virtual ObjectTypeClass const & Class_Of(void) const;
		virtual int Full_Name(void) const;

		/*
		**	Coordinate inquiry functions. These are used for both display and
		**	combat purposes.
		*/
		virtual COORDINATE Fire_Coord(int which) const;

		/*
		**	Object entry and exit from the game system.
		*/
		virtual bool Unlimbo(COORDINATE coord, DirType facing);
		virtual bool Limbo(void);
		virtual void Detach(TARGET target, bool all);

		/*
		**	Display and rendering support functionality. Supports imagery and how
		**	object interacts with the map and thus indirectly controls rendering.
		*/
		virtual short const * Overlap_List(void) const;
		virtual void Draw_It(int x, int y, WindowNumberType window);
		virtual void Look(bool incremental=false);

		/*
		**	User I/O.
		*/
		virtual void Response_Select(void);
		virtual void Response_Move(void);
		virtual void Response_Attack(void);
		virtual void Active_Click_With(ActionType action, ObjectClass * object);

		/*
		**	Combat related.
		*/
		virtual int Made_A_Kill(void);
		virtual ActionType What_Action(ObjectClass * object) const;
		virtual ActionType What_Action(CELL cell) const;
		virtual void Assign_Mission(MissionType order);
		virtual BulletClass * Fire_At(TARGET target, int which);
		virtual ResultType Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source=0);
		virtual TARGET As_Target(void) const;
		virtual FireErrorType Can_Fire(TARGET target, int which) const;
		virtual void Assign_Target(TARGET);
		virtual RadioMessageType Receive_Message(RadioClass * from, RadioMessageType message, long & param);
		virtual int Rearm_Delay(bool second) const;
		void  Set_Occupy_Bit(COORDINATE coord) {Set_Occupy_Bit(Coord_Cell(coord), CellClass::Spot_Index(coord));};
		void  Set_Occupy_Bit(CELL cell, int spot_index);
		void  Clear_Occupy_Bit(COORDINATE coord) {Clear_Occupy_Bit(Coord_Cell(coord), CellClass::Spot_Index(coord));};
		void  Clear_Occupy_Bit(CELL cell, int spot_index);

		/*
		**	Driver control support functions. These are used to control cell
		**	occupation flags and driver instructions.
		*/
		virtual bool Stop_Driver(void);
		virtual bool Start_Driver(COORDINATE & coord);

		/*
		**	AI.
		*/
		virtual void AI(void);
		virtual TARGET Greatest_Threat(ThreatType threat) const;
		virtual int Mission_Attack(void);

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
		static char *INI_Name(void) {return "INFANTRY";};
		bool  Load(FileClass & file);
		bool  Save(FileClass & file);
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		/*
		**	Movement and animation.
		*/
		virtual bool Do_Action(DoType todo, bool force=false);
		virtual void Random_Animate(void);
		virtual MoveType Can_Enter_Cell(CELL , FacingType =FACING_NONE) const;
		virtual void Per_Cell_Process(bool center);
		virtual void Enter_Idle_Mode(bool initial=false);
		virtual void Scatter(COORDINATE threat, bool forced =false, bool nokidding =false);

		/*
		**	Dee-buggin' support.
		*/
		int Validate(void) const;

		/*
		**	Translation table to convert facing into infantry shape number. This special
		**	table is needed since several facing stages are reused and flipped about the Y
		**	axis.
		*/
		static int const HumanShape[32];

	private:

		static DoStruct const MasterDoControls[DO_COUNT];

		/*
		** This contains the value of the Virtual Function Table Pointer
		*/
		static void * VTable;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];
};

#endif