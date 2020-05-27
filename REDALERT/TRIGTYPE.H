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

/* $Header: /CounterStrike/TRIGTYPE.H 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TRIGTYPE.H                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 06/05/96                                                     *
 *                                                                                             *
 *                  Last Update : June 5, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef TRIGTYPE_H
#define TRIGTYPE_H

#include	"tevent.h"
#include	"taction.h"

class TriggerClass;

/*
**	There can be multiple trigger events and trigger actions. This enumeration is used to
**	indicate if there are multiple events/actions and what their relationship is.
*/
typedef enum MultiStyleType : unsigned char {
	MULTI_ONLY,					// "Only" main trigger action/event?
	MULTI_AND,					// "And" secondary trigger action/event?
	MULTI_OR,					// "Or" secondary event?
	MULTI_LINKED				// Cause and effect pairs are linked?
} MultiStyleType;


class TriggerTypeClass : public AbstractTypeClass
{
	public:
		unsigned IsActive:1;

		typedef enum PersistantType : unsigned char {
			VOLATILE = 0,
			SEMIPERSISTANT = 1,
			PERSISTANT = 2
		} PersistantType;

		/*
		**	This flag controls whether the trigger destroys itself after it goes
		**	off.
		**	0 = trigger destroys itself immediately after going off, and removes
		**	    itself from all objects it's attached to
		**	1 = trigger is "Semi-Persistent"; it maintains a count of all objects
		**	    it's attached to, and only actually "springs" after its been
		**		 triggered from all the objects; then, it removes itself.
		**	2 = trigger is Fully Persistent; it just won't go away.
		*/
		PersistantType IsPersistant;

		/*
		**	For house-specific events, this is the house for that event.
		*/
		HousesType House;

		/*
		**	Each trigger must have an event which activates it. This is the event that is
		**	used to activate this trigger.
		*/
		TEventClass Event1;
		TEventClass Event2;
		MultiStyleType EventControl;

		/*
		**	This is the action to perform when the trigger event occurs.
		*/
		TActionClass Action1;
		TActionClass Action2;
		MultiStyleType ActionControl;


		TriggerTypeClass(void);
		TriggerTypeClass(NoInitClass const & x) : AbstractTypeClass(x), Event1(x), Event2(x), Action1(x), Action2(x) {};
		virtual ~TriggerTypeClass(void);

		static void * operator new(size_t );
		static void * operator new(size_t, void * ptr) {return(ptr);};
		static void operator delete(void *ptr);

		/*
		**	Initialization: clears all trigger types in preparation for new scenario
		*/
		static void Init(void);

		/*
		**	File I/O routines
		*/
		static void Read_INI(CCINIClass & ini);
		static void Write_INI(CCINIClass & ini);
		void Fill_In(char * name, char * entry);
		void Build_INI_Entry(char * buf) const;

		static char * INI_Name(void) {return "Trigs";};
		bool Load(Straw & file);
		bool Save(Pipe & file) const;
		void Code_Pointers(void);
		void Decode_Pointers(void);

		/*
		**	Processing routines
		*/
		TriggerClass * Create_One_Of(void) const;
		void Destroy_All_Of(void) const;

		/*
		**	Utility routines
		*/
		void Detach(TARGET target, bool all=true);
		AttachType Attaches_To(void) const;
		TARGET As_Target(void) const;
		static TriggerTypeClass * From_Name(char const * name);
		bool Edit(void);
#if defined(CHEAT_KEYS) || defined(SCENARIO_EDITOR)
		char const * Description(void) const;
		operator const char * (void) const {return(Description());};
#endif
		void Draw_It(int index, int x, int y, int width, int height, bool selected, TextPrintType flags) const;
};


#endif
