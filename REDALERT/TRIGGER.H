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

/* $Header: /CounterStrike/TRIGGER.H 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TRIGGER.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 11/12/94                                                     *
 *                                                                                             *
 *                  Last Update : November 12, 1994 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef TRIGGER_H
#define TRIGGER_H

class TriggerClass {
	public:
		RTTIType RTTI;
		int ID;

		CCPtr<TriggerTypeClass> Class;

		/*
		**	Record of the "already sprung" flag for the events.
		*/
		TDEventClass Event1;
		TDEventClass Event2;

		/*
		**	Constructor/Destructor
		*/
		TriggerClass(TriggerTypeClass * trigtype=NULL);
		TriggerClass(NoInitClass const & x) : Class(x), Event1(x), Event2(x) {};
		~TriggerClass(void);

		/*
		**	Initialization: clears all triggers in preparation for new scenario
		*/
		static void Init(void);

		/*
		**	Processing routines
		*/
		bool  Spring(TEventType event=TEVENT_ANY, ObjectClass * object=0, CELL cell=0, bool forced=false);
		void Detach(TARGET target, bool all=true);

		/*
		**	File I/O routines
		*/
		bool Load(Straw & file);
		bool Save(Pipe & file) const;
		void Code_Pointers(void) {};
		void Decode_Pointers(void) {};

		/*
		**	Utility routines
		*/
		TARGET As_Target(void) const;
		char const * Description(void) const;
		void Draw_It(int , int x, int y, int width, int height, bool selected, TextPrintType flags) const;
		char const * Name(void) const {return(Class->Name());}

		/*
		**	Overloaded operators
		*/
		static void * operator new(size_t size);
		static void * operator new(size_t, void * ptr) {return(ptr);};
		static void operator delete(void * ptr);

		/*
		**	If this trigger object is active, then this flag will be true. Trigger
		**	objects that are not active are either not yet created or have been
		**	deleted after fulfilling their action.
		*/
		unsigned IsActive:1;

		/*
		**	This value tells how many objects or cells this trigger is attached
		**	to.  The Read_INI routine for all classes that point to a trigger must
		**	increment this value!
		*/
		int AttachCount;

		/*
		**	This value is used for triggers that can only exist in one cell. It is
		**	needed for such triggers that the exact location of the trigger is needed
		**	during processing but its location cannot be inferred from other data.
		**	For all other triggers, this value is ignored.
		*/
		CELL Cell;

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];
};




#endif