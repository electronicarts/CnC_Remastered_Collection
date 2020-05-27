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

/* $Header:   F:\projects\c&c\vcs\code\unit.h_v   2.19   16 Oct 1995 16:45:56   JOE_BOSTIC  $ */
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

#include "tarcom.h"
#include "radio.h"
#include "cargo.h"
#include "mission.h"
#include	"target.h"


/****************************************************************************
**	For each instance of a unit (vehicle) in the game, there is one of
**	these structures. This structure holds information that is specific
**	and dynamic for a particular unit.
*/
class UnitClass :	public TarComClass
{
	public:

		/*
		**	This records the house flag that this object is currently carrying.
		*/
		HousesType Flagged;

		/*---------------------------------------------------------------------
		**	Constructors, Destructors, and overloaded operators.
		*/
		static void * operator new(size_t size);
		static void operator delete(void *ptr);
		UnitClass(void) {};
		UnitClass(UnitType classid, HousesType house);
		operator UnitType(void) const {return Class->Type;};
		virtual ~UnitClass(void);
		virtual RTTIType What_Am_I(void) const;

		/*---------------------------------------------------------------------
		**	Member function prototypes.
		*/
		static void Init(void);

		bool  Goto_Clear_Spot(void);
		bool  Try_To_Deploy(void);

		bool  Tiberium_Check(CELL &center, int x, int y);
		bool  Flag_Attach(HousesType house);
		bool  Flag_Remove(void);
		void  Find_LZ(void);
		bool  Unload_Hovercraft_Process(void);
		bool  Goto_Tiberium(void);
		bool  Harvesting(void);
		void  APC_Close_Door(void);
		void  APC_Open_Door(void);

		/*
		**	Query functions.
		*/
		virtual bool Can_Player_Move(void) const;
		virtual int Pip_Count(void) const;
		virtual InfantryType Crew_Type(void) const;

		/*
		**	Coordinate inquiry functions. These are used for both display and
		**	combat purposes.
		*/
		virtual COORDINATE Sort_Y(void) const;

		/*
		**	Object entry and exit from the game system.
		*/
		virtual bool Unlimbo(COORDINATE , DirType facing=DIR_N);
		virtual bool Limbo(void);

		/*
		**	Display and rendering support functionality. Supports imagery and how
		**	object interacts with the map and thus indirectly controls rendering.
		*/
		virtual void const * Remap_Table(void);
		virtual void Look(bool incremental=false);
		virtual short const * Overlap_List(void) const;
		virtual void Draw_It(int x, int y, WindowNumberType window);

		/*
		** ST Added 1/15/2019 2:44PM
		*/
		//virtual bool Get_Draw_Parameters(void);

		/*
		**	User I/O.
		*/
		virtual ActionType What_Action(CELL cell) const;
		virtual ActionType What_Action(ObjectClass * object) const;
		virtual void Active_Click_With(ActionType action, ObjectClass * object);
		virtual void Active_Click_With(ActionType action, CELL cell);
		virtual void Response_Select(void);
		virtual void Response_Move(void);
		virtual void Response_Attack(void);

		/*
		**	Combat related.
		*/
		virtual COORDINATE Target_Coord(void) const;
		virtual ResultType Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source=0);
		virtual TARGET As_Target(void) const;
		virtual void Stun(void);

		/*
		**	Driver control support functions. These are used to control cell
		**	occupation flags and driver instructions.
		*/
		virtual bool Stop_Driver(void);
		virtual bool Start_Driver(COORDINATE & coord);

		/*
		**	AI.
		*/
		virtual DirType Desired_Load_Dir(ObjectClass * passenger, CELL & moveto) const;
		virtual RadioMessageType Receive_Message(RadioClass * from, RadioMessageType message, long & param);
		virtual void AI(void);
		virtual int Mission_Attack(void);
		virtual int Mission_Unload(void);
		virtual int Mission_Guard(void);
		virtual int Mission_Harvest(void);
		virtual int Mission_Hunt(void);
		virtual int UnitClass::Mission_Move(void);
		virtual FireErrorType Can_Fire(TARGET, int which) const;

		/*
		**	Scenario and debug support.
		*/
		#ifdef CHEAT_KEYS
		virtual void Debug_Dump(MonoClass *mono) const;
		#endif

		/*
		**	Movement and animation.
		*/
		virtual void Enter_Idle_Mode(bool initial=false);
		virtual MoveType Can_Enter_Cell(CELL cell, FacingType facing=FACING_NONE) const;
		virtual void Per_Cell_Process(bool center);
		virtual void Scatter(COORDINATE threat, bool forced=false, bool nokidding=false);
		void  Exit_Repair(void);
//		MoveType Blocking_Object(TechnoClass const *techno, CELL cell) const;

		/*
		**	File I/O.
		*/
		static void  Read_INI(char *buffer);
		static void  Write_INI(char *buffer);
		static char *INI_Name(void) {return "UNITS";};
		bool  Load(FileClass & file);
		bool  Save(FileClass & file);
		virtual void Code_Pointers(void);
		virtual void Decode_Pointers(void);

		/*
		**	Dee-buggin' support.
		*/
		int Validate(void) const;

	private:

		/*
		** Timer to prevent Harvesters from collecting too often
		*/
		TCountDownTimerClass HarvestTimer;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];

		/*
		** This contains the value of the Virtual Function Table Pointer
		*/
		static void * VTable;
};

#endif