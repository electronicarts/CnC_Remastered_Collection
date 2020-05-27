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

/* $Header: /CounterStrike/INFANTRY.H 1     3/03/97 10:24a Joe_bostic $ */
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


class InfantryClass : public FootClass
{
	public:
		CCPtr<InfantryTypeClass> Class;

		/*
		**	If the infantry is undergoing some choreographed animation sequence, then
		**	this holds the particular sequence number. The frame of animation is kept
		**	track of by the regular frame tracking system. When performing an animation
		**	sequence, the infantry cannot perform anything else (even move).
		*/
		DoType Doing;

		/*
		**	Certain infantry will either perform some comment or say something after an
		**	amount of time has expired subsequent to an significant event. This is the
		**	timer the counts down.
		*/
		CDTimerClass<FrameTimerClass> Comment;

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
		**	If the infantry is allowed to move one cell from one zone to another, then this
		**	flag will be true. It exists only so that when a bridge is destroyed, the bomb
		**	placer is allowed to run from the destroyed bridge cell back onto a real cell.
		*/
		unsigned IsZoneCheat:1;

		/*
		** This flag is set for the dogs, when they launch into bullet mode.
		** it's to remember if the unit was selected, and if it was, then
		** when the dog is re-enabled, he'll reselect himself.
		*/
		unsigned WasSelected:1;

		/*
		**	The fear rating of this infantry unit. The more afraid the infantry, the more
		**	likely it is to panic and seek cover.
		*/
		FearType Fear;

		/*
		** Track when movement last stopped.
		*/
		long StopDriverFrame;

		/*
		** Track the last cell we looked from.
		*/
		CELL LookCell;

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		static void * operator new(size_t size);
		static void * operator new(size_t , void * ptr) {return(ptr);};
		static void operator delete(void *ptr);
		InfantryClass(InfantryType classid, HousesType house);
		InfantryClass(NoInitClass const & x) : FootClass(x), Class(x), Comment(x) {};
		virtual ~InfantryClass(void);
		operator InfantryType(void) const {return Class->Type;};

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		static void Init(void);

		virtual void Assign_Destination(TARGET);

		/*
		**	Query functions.
		*/
		virtual bool Is_Ready_To_Random_Animate(void) const;
		void const * Get_Image_Data(void) const;
		int Shape_Number(WindowNumberType window = WINDOW_MAIN) const;
		virtual ObjectTypeClass const & Class_Of(void) const;
		virtual int Full_Name(void) const;

		/*
		**	Object entry and exit from the game system.
		*/
		virtual bool Unlimbo(COORDINATE coord, DirType facing);
		virtual bool Paradrop(COORDINATE coord);
		virtual bool Limbo(void);
		virtual void Detach(TARGET target, bool all);

		/*
		**	Display and rendering support functionality. Supports imagery and how
		**	object interacts with the map and thus indirectly controls rendering.
		*/
		virtual short const * Overlap_List(bool redraw = false) const;
		virtual void Draw_It(int x, int y, WindowNumberType window) const;

		/*
		**	User I/O.
		*/
		virtual void Response_Select(void);
		virtual void Response_Move(void);
		virtual void Response_Attack(void);
		virtual void Active_Click_With(ActionType action, ObjectClass * object);
		virtual void Active_Click_With(ActionType action, CELL cell) {FootClass::Active_Click_With(action, cell);}

		/*
		**	Combat related.
		*/
		virtual ActionType What_Action(ObjectClass const * object) const;
		virtual ActionType What_Action(CELL cell) const;
		virtual BulletClass * Fire_At(TARGET target, int which);
		virtual ResultType Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source=0, bool forced=false);
		virtual FireErrorType Can_Fire(TARGET target, int which) const;
		virtual COORDINATE Fire_Coord(int which) const;
		virtual void Assign_Target(TARGET);
		void Set_Occupy_Bit(COORDINATE coord) {Set_Occupy_Bit(Coord_Cell(coord), CellClass::Spot_Index(coord));};
		void Set_Occupy_Bit(CELL cell, int spot_index);
		void Clear_Occupy_Bit(COORDINATE coord) {Clear_Occupy_Bit(Coord_Cell(coord), CellClass::Spot_Index(coord));};
		void Clear_Occupy_Bit(CELL cell, int spot_index);

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
		void Fear_AI(void);
		virtual TARGET Greatest_Threat(ThreatType threat) const;
		virtual int Mission_Attack(void);
		bool Edge_Of_World_AI(void);
		void Firing_AI(void);
		void Doing_AI(void);
		void Movement_AI(void);

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
		static void Write_INI(CCINIClass & ini);
		static char *INI_Name(void) {return "INFANTRY";};
		bool Load(Straw & file);
		bool Save(Pipe & file) const;

		/*
		**	Movement and animation.
		*/
		virtual bool Do_Action(DoType todo, bool force=false);
		virtual bool Random_Animate(void);
		virtual MoveType Can_Enter_Cell(CELL , FacingType =FACING_NONE) const;
		virtual void Per_Cell_Process(PCPType why);
		virtual void Enter_Idle_Mode(bool initial=false);
		virtual void Scatter(COORDINATE threat, bool forced=false, bool nokidding=false);
		virtual void Look(bool incremental=false);

		/*
		**	Translation table to convert facing into infantry shape number. This special
		**	table is needed since several facing stages are reused and flipped about the Y
		**	axis.
		*/
		static int const HumanShape[32];

	private:

		static DoStruct const MasterDoControls[DO_COUNT];

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];
};

#endif