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

/* $Header: /CounterStrike/TEVENT.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TEVENT.H                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 11/28/95                                                     *
 *                                                                                             *
 *                  Last Update : November 28, 1995 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef TEVENT_H
#define TEVENT_H

/*
**	These are the trigger events that are checked for and if qualified, they will signal
**	a successful trigger event. This might result in the trigger action being performed.
*/
typedef enum TEventType : unsigned char {
	TEVENT_NONE,
	TEVENT_PLAYER_ENTERED,					// player enters this square
	TEVENT_SPIED,								// Spied by.
	TEVENT_THIEVED,							// Thieved by (raided or stolen vehicle).
	TEVENT_DISCOVERED,						// player discovers this object
	TEVENT_HOUSE_DISCOVERED,				// House has been discovered.
	TEVENT_ATTACKED,							// player attacks this object
	TEVENT_DESTROYED,							// player destroys this object
	TEVENT_ANY,									// Any object event will cause the trigger.
	TEVENT_UNITS_DESTROYED,					// all house's units destroyed
	TEVENT_BUILDINGS_DESTROYED,			// all house's buildings destroyed
	TEVENT_ALL_DESTROYED,					// all house's units & buildings destroyed
	TEVENT_CREDITS,							// house reaches this many credits
	TEVENT_TIME,								// Scenario elapsed time from start.
	TEVENT_MISSION_TIMER_EXPIRED,			// Pre expired mission timer.
	TEVENT_NBUILDINGS_DESTROYED,			// Number of buildings destroyed.
	TEVENT_NUNITS_DESTROYED,				// Number of units destroyed.
	TEVENT_NOFACTORIES,						// No factories left.
	TEVENT_EVAC_CIVILIAN,					// Civilian has been evacuated.
	TEVENT_BUILD,								// Specified building has been built.
	TEVENT_BUILD_UNIT,						// Specified unit has been built.
	TEVENT_BUILD_INFANTRY,					// Specified infantry has been built.
	TEVENT_BUILD_AIRCRAFT,					// Specified aircraft has been built.
	TEVENT_LEAVES_MAP,						// Specified team member leaves map.
	TEVENT_ENTERS_ZONE,						// Enters same zone as waypoint 'x'.
	TEVENT_CROSS_HORIZONTAL,				// Crosses horizontal trigger line.
	TEVENT_CROSS_VERTICAL,					// Crosses vertical trigger line.
	TEVENT_GLOBAL_SET,						// If specified global has been set.
	TEVENT_GLOBAL_CLEAR,						// If specified global has been cleared.
	TEVENT_FAKES_DESTROYED,					// If all fake structures are gone.
	TEVENT_LOW_POWER,							// When power drops below 100%.
	TEVENT_ALL_BRIDGES_DESTROYED,			// All bridges destroyed.
	TEVENT_BUILDING_EXISTS,					// Check for building existing.

	TEVENT_COUNT,
	TEVENT_FIRST=0
} TEventType;

TEventType Event_From_Name(char const * name);
NeedType Event_Needs(TEventType event);
char const * Name_From_Event(TEventType event);

/*
**	This holds the changable data that is associated with an event as
**	it relates to a trigger.
*/
struct TDEventClass {
	/*
	**	If this event has been triggered by something that is temporal, then
	**	this flag will be set to true so that subsequent trigger examination
	**	will return a successful event trigger flag. Typical use of this is
	**	for when objects of a specific type are built.
	*/
	unsigned IsTripped:1;

	/*
	**	Timer based events require a special timer control handler.
	*/
	CDTimerClass<FrameTimerClass> Timer;

	TDEventClass(void) : IsTripped(false), Timer(0) {};
	TDEventClass(NoInitClass const & x) : Timer(x) {};
};


/*
**	This elaborates the information necessary to trigger
**	an event.
*/
class TeamTypeClass;
struct TEventClass {

	/*
	**	This is the event that will controls how this event gets triggered.
	*/
	TEventType Event;

	/*
	**	If this event needs to reference a team type, then this is the pointer
	**	to the team type object. This must be separated from the following
	**	union because Watcom compiler won't allow a class that has a
	**	constructor to be declared in a union.
	*/
	CCPtr<TeamTypeClass> Team;

	union {
		StructType				Structure;	// Used for structure type checking.
		UnitType					Unit;			// Used for unit type checking.
		InfantryType			Infantry;	//	Used for infantry type checking.
		AircraftType			Aircraft;	// Used for aircraft type checking.
		HousesType				House;		// Used for house specific events.
		long	Value;							// Used for other events that need data.
	} Data;

	TEventClass(void) : Event(TEVENT_NONE) {Data.Value = 0;};
	TEventClass(TEventType event) : Event(event) {Data.Value = 0;};
	TEventClass(NoInitClass const & x) : Team(x) {};

	void Code_Pointers(void);
	void Decode_Pointers(void);
	void Reset(TDEventClass & td) const;
	bool operator () (TDEventClass & td, TEventType event, HousesType house, ObjectClass const * object, bool forced);
	void Read_INI(void);
	void Build_INI_Entry(char * buffer) const;
};


typedef enum AttachType : unsigned char {
	ATTACH_NONE=0x00,				// Trigger doesn't attach to anything (orphan trigger types).
	ATTACH_CELL=0x01,				// Trigger can only attach to a cell.
	ATTACH_OBJECT=0x02,			// Trigger can attach only to object (usually building or vehicle).
	ATTACH_MAP=0x04,				// Trigger applies to the general map (usually zone or parallel triggers).
	ATTACH_HOUSE=0x08,			// Trigger applies only to a house.
	ATTACH_GENERAL=0x10,			// General purpose trigger attached to game state.
	ATTACH_TEAM=0x20				// Trigger applies to team object.
} AttachType;

AttachType Attaches_To(TEventType event);



class EventChoiceClass {
	public:
		EventChoiceClass(TEventType event=TEVENT_NONE) : Event(event) {}

		operator TEventType (void) const {return(Event);}
		bool operator == (EventChoiceClass const & rvalue) const {return(Event == rvalue.Event);}
		bool operator != (EventChoiceClass const & rvalue) const {return(Event != rvalue.Event);}
		bool operator > (EventChoiceClass const & rvalue) const {return(stricmp(Description(), rvalue.Description()) > 0);}
		bool operator < (EventChoiceClass const & rvalue) const {return(stricmp(Description(), rvalue.Description()) < 0);}
		bool operator <= (EventChoiceClass const & rvalue) const {return(Event == rvalue.Event || stricmp(Description(), rvalue.Description()) < 0);}
		bool operator >= (EventChoiceClass const & rvalue) const {return(Event == rvalue.Event || stricmp(Description(), rvalue.Description()) > 0);}
		char const * Description(void) const {return(Name_From_Event(Event));}
		void Draw_It(int index, int x, int y, int width, int height, bool selected, TextPrintType flags) const;

		TEventType Event;
};

extern EventChoiceClass EventChoices[TEVENT_COUNT];


#endif
