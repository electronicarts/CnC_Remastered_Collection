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

/* $Header:   F:\projects\c&c\vcs\code\trigger.h_v   2.15   16 Oct 1995 16:46:32   JOE_BOSTIC  $ */
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

typedef enum EventType : char  {
	EVENT_NONE=-1,

	/*
	.......................... Cell-specific events ..........................
	*/
	EVENT_PLAYER_ENTERED,					// player enters this square
	EVENT_CELLFIRST = EVENT_PLAYER_ENTERED,

	/*
	......................... Object-specific events .........................
	*/
	EVENT_DISCOVERED,							// player discovers this object
	EVENT_OBJECTFIRST = EVENT_DISCOVERED,
	EVENT_ATTACKED,							// player attacks this object
	EVENT_DESTROYED,							// player destroys this object
	EVENT_ANY,									// Any object event will cause the trigger.

	/*
	......................... House-specific events ..........................
	*/
	EVENT_HOUSE_DISCOVERED,					// any object in this house discovered
	EVENT_HOUSEFIRST = EVENT_HOUSE_DISCOVERED,
	EVENT_UNITS_DESTROYED,					// all house's units destroyed
	EVENT_BUILDINGS_DESTROYED,				// all house's buildings destroyed
	EVENT_ALL_DESTROYED,						// all house's units & buildings destroyed
	EVENT_CREDITS,								// house reaches this many credits
	EVENT_TIME,									// time elapses for this house
	EVENT_NBUILDINGS_DESTROYED,			// Number of buildings destroyed.
	EVENT_NUNITS_DESTROYED,					// Number of units destroyed.
	EVENT_NOFACTORIES,						// No factories left.
	EVENT_EVAC_CIVILIAN,						// Civilian has been evacuated.
	EVENT_BUILD,								// If specified building has been built.

	EVENT_COUNT,
	EVENT_FIRST=0
} EventType;


class TriggerClass {
	public:
		typedef enum ActionType {
			ACTION_NONE=-1,

			ACTION_WIN,									// player wins!
			ACTION_LOSE,								// player loses.
			ACTION_BEGIN_PRODUCTION,				// computer begins production
			ACTION_CREATE_TEAM,						// computer creates a certain type of team
			ACTION_DESTROY_TEAM,
			ACTION_ALL_HUNT,							// all enemy units go into hunt mode (teams destroyed).
			ACTION_REINFORCEMENTS,					// player gets reinforcements
															// (house that gets them is determined by
															// the Reinforcement instance)
			ACTION_DZ,									// Deploy drop zone smoke.
			ACTION_AIRSTRIKE,							// Enable airstrike.
			ACTION_NUKE,								// Enable nuke for computer.
			ACTION_ION,									// Give ion cannon to computer.
			ACTION_DESTROY_XXXX,						// Destroy trigger XXXX.
			ACTION_DESTROY_YYYY,						// Destroy trigger YYYY.
			ACTION_DESTROY_ZZZZ,						// Destroy trigger ZZZZ.
			ACTION_AUTOCREATE,						// Computer to autocreat teams.
			ACTION_WINLOSE,							// Win if captured, lose if destroyed.
			ACTION_ALLOWWIN,							// Allows winning if triggered.

			ACTION_COUNT,
			ACTION_FIRST=0
		} ActionType;

		typedef enum PersistantType {
			VOLATILE = 0,
			SEMIPERSISTANT = 1,
			PERSISTANT = 2,
		} PersistantType;

		/*
		** Functions:
		**
		**	Constructor/Destructor
		*/
		TriggerClass(void);
		~TriggerClass(void);

		/*
		**	Initialization: clears all triggers in preparation for new scenario
		*/
		static void Init(void);

		/*
		**	Processing routines
		*/
		bool  Spring(EventType event, ObjectClass * object);	// object-based
		bool  Spring(EventType event, CELL cell);						// cell-based
		bool  Spring(EventType event, HousesType house, long data=0); // house-based
		bool  Remove(void);

		/*
		**	File I/O routines
		*/
		static void Read_INI (char *buffer);
		void  Fill_In(char *name, char *entry);
		static void Write_INI (char *buffer, bool refresh);
		static char * INI_Name(void) {return "Triggers";};
		bool  Load(FileClass & file);
		bool  Save(FileClass & file);
		void  Code_Pointers(void);
		void  Decode_Pointers(void);

		/*
		**	As_Pointer gets a pointer to the trigger object give the mnemonic
		*/
		static TriggerClass *  As_Pointer(char const * name);

		/*
		**	Data Access routines
		*/
//		EventType Get_Event(void) const {return (Event);}
//		void Set_Event(EventType event) {Event = event;}
//		ActionType Get_Action(void) const {return (Action);}
//		void Set_Action(ActionType action) {Action = action;}
//		HousesType Get_House(void) const {return(House);}
//		void Set_House(HousesType house) {House = house;}
//		long Get_Data(void) const {return(Data);}
//		void Set_Data(long credits) {Data = credits;}
		char const * Get_Name(void) const {return (Name);}
		void  Set_Name(char const *buf) {strncpy(Name, buf, sizeof(Name)); Name[sizeof(Name)-1] = '\0';}

		/*
		**	Utility routines
		*/
		TARGET As_Target(void) const;
		static bool Event_Need_Object(EventType event);
		static bool Event_Need_House(EventType event);
		static bool Event_Need_Data(EventType event);
		static bool Action_Need_Team(ActionType action);
		static EventType Event_From_Name(char const *name);
		static char const *Name_From_Event(EventType event);
		static ActionType Action_From_Name(char const *name);
		static char const *Name_From_Action(ActionType action);

		/*
		**	Overloaded operators
		*/
		static void * operator new(size_t size);
		static void operator delete(void *ptr);

		/*
		**	Dee-buggin' support.
		*/
		int Validate(void) const;

		/*
		**	This is the pointer to the team that gets created or destroyed when
		**	a team-related trigger goes off, or for reinforcements. The house
		**	for reinforcements is determined by the house for that team.
		*/
		TeamTypeClass *Team;

		/*
		**	If this trigger object is active, then this flag will be true. Trigger
		**	objects that are not active are either not yet created or have been
		**	deleted after fulfilling their action.
		*/
		unsigned IsActive:1;

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
		**	This value tells how many objects or cells this trigger is attached
		**	to.  The Read_INI routine for all classes that point to a trigger must
		**	increment this value!
		*/
		int AttachCount;

		/*
		**	Each trigger must have an event which activates it. This is the event that is
		**	used to activate this trigger.
		*/
		EventType Event;

		/*
		**	This is the action to perform when the trigger event occurs.
		*/
		ActionType Action;

		/*
		**	For house-specific events, this is the house for that event.
		*/
		HousesType House;

		/*
		**	For credit-related triggers, this is the number of credits that
		**	generate the trigger. For time-based triggers, this is the number
		**	of minutes that must elapse.
		*/
		long Data;
		long DataCopy;

	private:

		/*
		**	Triggers can be referred to by their name, which can be up to 4
		**	characters.
		*/
		char Name[5];

		/*
		** Some additional padding in case we need to add data to the class and maintain backwards compatibility for save/load
		*/
		unsigned char SaveLoadPadding[32];

};


#endif