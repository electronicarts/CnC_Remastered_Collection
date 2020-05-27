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

/* $Header:   F:\projects\c&c\vcs\code\trigger.cpv   2.17   16 Oct 1995 16:51:20   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TRIGGER.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 11/12/94                                                     *
 *                                                                                             *
 *                  Last Update : August 27, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Do_All_To_Hunt -- Forces all computer controlled units into hunt mode.                    *
 *   TriggerClass::Action_From_Name -- retrieves ActionType for given name                     *
 *   TriggerClass::Action_Need_Team -- Determines if this action event requires a team.        *
 *   TriggerClass::As_Pointer -- returns pointer for the given trigger name                    *
 *   TriggerClass::As_Target -- Converts trigger to a target value                             *
 *   TriggerClass::Event_From_Name -- retrieves EventType for given name                       *
 *   TriggerClass::Event_Need_Data -- Determines if this event requires a data value.          *
 *   TriggerClass::Event_Need_House -- Determines if this event requires a house identifier.   *
 *   TriggerClass::Event_Need_Object -- Determines if the specified event requires an object.  *
 *   TriggerClass::Init -- clears triggers for new scenario                                    *
 *   TriggerClass::Name_From_Action -- retrieves name for ActionType                           *
 *   TriggerClass::Name_From_Event -- retrieves name for EventType                             *
 *   TriggerClass::Read_INI -- reads triggers from the INI file                                *
 *   TriggerClass::Remove -- removes this trigger from the game                                *
 *   TriggerClass::Spring -- Trigger processing routine for cell-based triggers                *
 *   TriggerClass::Spring -- Trigger processing routine for house-based triggers               *
 *   TriggerClass::Spring -- Trigger processing routine for object-based triggers              *
 *   TriggerClass::TriggerClass -- constructor                                                 *
 *   TriggerClass::Validate -- validates trigger pointer													  *
 *   TriggerClass::Write_INI -- writes triggers to the INI file                                *
 *   TriggerClass::operator delete -- 'delete' operator                                        *
 *   TriggerClass::operator new -- 'new' operator                                              *
 *   TriggerClass::~TriggerClass -- Destructor for trigger objects.                            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"

static void Do_All_To_Hunt(void);

#define	FIXUP		0

/*
********************************** Globals **********************************
*/
static const char * EventText[EVENT_COUNT + 1] = {
	"None",
	"Player Enters",
	"Discovered",
	"Attacked",
	"Destroyed",
	"Any",
	"House Discov.",
	"Units Destr.",
	"Bldgs Destr.",
	"All Destr.",
	"Credits",
	"Time",
	"# Bldgs Dstr.",
	"# Units Dstr.",
	"No Factories",
	"Civ. Evac.",
	"Built It"
};


static const char * ActionText[TriggerClass::ACTION_COUNT + 1] = {
	"None",
	"Win",
	"Lose",
	"Production",
	"Create Team",
	"Dstry Teams",
	"All to Hunt",
	"Reinforce.",
	"DZ at 'Z'",
	"Airstrike",
	"Nuclear Missile",
	"Ion Cannon",
	"Dstry Trig 'XXXX'",
	"Dstry Trig 'YYYY'",
	"Dstry Trig 'ZZZZ'",
	"Autocreate",
	"Cap=Win/Des=Lose",
	"Allow Win"
};


/***********************************************************************************************
 * TriggerClass::Validate -- validates trigger pointer													  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *		none.																												  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *		1 = ok, 0 = error																								  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *		none.																												  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/09/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
#ifdef CHEAT_KEYS
int TriggerClass::Validate(void) const
{
	int num;

	num = Triggers.ID(this);
	if (num < 0 || num >= TRIGGER_MAX) {
		Validate_Error("TRIGGER");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
#endif


/***********************************************************************************************
 * TriggerClass::Event_Need_Object -- Determines if the specified event requires an object.    *
 *                                                                                             *
 *    This routine determines if the specified event must be attached to an object. Such       *
 *    events can only exist in a parasitic fashion attached to object(s) in the game.          *
 *                                                                                             *
 * INPUT:   event -- The event type to examine.                                                *
 *                                                                                             *
 * OUTPUT:  Does the specified event require attachement to an object?                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TriggerClass::Event_Need_Object(EventType event)
{
	switch (event) {
		case EVENT_PLAYER_ENTERED:
		case EVENT_DISCOVERED:
		case EVENT_ATTACKED:
		case EVENT_DESTROYED:
		case EVENT_ANY:
			return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TriggerClass::Event_Need_House -- Determines if this event requires a house identifier.     *
 *                                                                                             *
 *    This routine is used to determine if the specified event requires a house identifier.    *
 *    All trigger events that affect a house will require a house identifier.                  *
 *                                                                                             *
 * INPUT:   event -- The event type to examine.                                                *
 *                                                                                             *
 * OUTPUT:  Does the specified event type require a house identifier?                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TriggerClass::Event_Need_House(EventType event)
{
	switch (event) {
		case EVENT_PLAYER_ENTERED:
		case EVENT_HOUSE_DISCOVERED:
		case EVENT_UNITS_DESTROYED:
		case EVENT_BUILDINGS_DESTROYED:
		case EVENT_ALL_DESTROYED:
		case EVENT_CREDITS:
		case EVENT_TIME:
		case EVENT_NBUILDINGS_DESTROYED:
		case EVENT_NUNITS_DESTROYED:
		case EVENT_NOFACTORIES:
		case EVENT_EVAC_CIVILIAN:
		case EVENT_BUILD:
			return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TriggerClass::Event_Need_Data -- Determines if this event requires a data value.            *
 *                                                                                             *
 *    This routine will determine if the specified event requires a data number parameter.     *
 *    This is commonly needed for trigger events.                                              *
 *                                                                                             *
 * INPUT:   event -- The event to examine.                                                     *
 *                                                                                             *
 * OUTPUT:  Does the specified event require a data number parameter?                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TriggerClass::Event_Need_Data(EventType event)
{
	switch (event) {
		case EVENT_CREDITS:
		case EVENT_TIME:
		case EVENT_NBUILDINGS_DESTROYED:
		case EVENT_NUNITS_DESTROYED:
		case EVENT_BUILD:
			return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TriggerClass::Action_Need_Team -- Determines if this action event requires a team.          *
 *                                                                                             *
 *    This routine will determine if the specified action requires a team name parameter.      *
 *    Typically, this is needed for reinforcements or other trigger events that affect         *
 *    a particular team type.                                                                  *
 *                                                                                             *
 * INPUT:   action   -- The action that is to be examined.                                     *
 *                                                                                             *
 * OUTPUT:  Does the specified action require a team type name?                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TriggerClass::Action_Need_Team(TriggerClass::ActionType action)
{
	switch (action) {
		case ACTION_CREATE_TEAM:
		case ACTION_DESTROY_TEAM:
		case ACTION_REINFORCEMENTS:
			return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TriggerClass::TriggerClass -- constructor                                                   *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
TriggerClass::TriggerClass(void)
{
	IsPersistant = VOLATILE;
	AttachCount = 0;
	Event = EVENT_NONE;
	Action = ACTION_NONE;
	House = HOUSE_NONE;
	DataCopy = Data = 0L;
	Name[0] = '\0';
	Team = NULL;
}


/***********************************************************************************************
 * TriggerClass::~TriggerClass -- Destructor for trigger objects.                              *
 *                                                                                             *
 *    This destructor will update the house blockage value if necessary. No other action need  *
 *    be performed on trigger destruction.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TriggerClass::~TriggerClass(void)
{
	if (GameActive && House != HOUSE_NONE && Action == ACTION_ALLOWWIN) {
		if (Houses.Ptr(House)->Blockage) Houses.Ptr(House)->Blockage--;
		Houses.Ptr(House)->BorrowedTime = TICKS_PER_SECOND*4;
	}
}


/***********************************************************************************************
 * TriggerClass::Init -- clears triggers for new scenario                                      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void TriggerClass::Init(void)
{
	Triggers.Free_All();
}


/***********************************************************************************************
 * TriggerClass::Spring -- Trigger processing routine                                          *
 *                                                                                             *
 * Checks whether this trigger should "spring" for the given event & object;                   *
 * If it should, then some really cool undocumented stuff magically happens.                   *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      event      EventType: What happened?                                                   *
 *      object   Ptr to object containing this trigger: What did it happen to?                 *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      0 = nothing happened; 1 = the trigger was sprung                                       *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/06/1994 BR : Created.                                                                  *
 *   06/25/1995 JLB : Added more trigger events.                                               *
 *=============================================================================================*/
bool TriggerClass::Spring(EventType event, ObjectClass *obj)
{
	Validate();
	/*
	**	If this is not the event for this trigger, just return.
	*/
	if (event != Event && Event != EVENT_ANY) {
		return(false);
	}

	/*
	**	If time-based, decrement the minute counter; return if it's not time yet
	*/
	if (Event == EVENT_TIME) {
		Data--;
		if (Data > 0) {
			return(false);
		}
		Data = DataCopy;
	}

	/*
	**	Semi-persistant trigger: first detach it from the calling object, then
	**	see if this is the last object we're attached to; if so, the trigger
	**	will spring.
	*/
	if (IsPersistant == SEMIPERSISTANT) {

		/*
		** Detach ourselves from the object
		*/
		obj->Trigger = NULL;

		/*
		** Decrement our attachment counter
		*/
		AttachCount--;

		/*
		** If we're attached to more objects, don't spring; otherwise, spring.
		** And, mark ourselves as volatile so we'll completely remove ourselves
		** from the game after we go off.
		*/
		if (AttachCount > 0) {
			return(false);
		} else {
			IsPersistant = VOLATILE;
		}
	}

	/*
	**	Otherwise, take an appropriate action.
	*/
	bool success = true;
	TriggerClass * trig = NULL;
	switch (Action) {
		case ACTION_NUKE:
			HouseClass::As_Pointer(HOUSE_BAD)->NukeStrike.Enable(true, false);
			HouseClass::As_Pointer(HOUSE_BAD)->NukeStrike.Forced_Charge(PlayerPtr->Class->House == HOUSE_BAD);
			break;

		case ACTION_ION:
			HouseClass::As_Pointer(HOUSE_GOOD)->IonCannon.Enable(true, false);
			HouseClass::As_Pointer(HOUSE_GOOD)->IonCannon.Forced_Charge(PlayerPtr->Class->House == HOUSE_GOOD);
			break;

		case ACTION_WINLOSE:
			switch (event) {
				case EVENT_DESTROYED:
					if (!PlayerPtr->IsToWin || PlayerPtr->Blockage > 0) PlayerPtr->Flag_To_Lose();
					success = true;
					break;

				case EVENT_PLAYER_ENTERED:
					if (!PlayerPtr->IsToLose) PlayerPtr->Flag_To_Win();
					success = true;
					break;

				default:
					success = false;
					break;
			}
			break;

		case ACTION_DESTROY_XXXX:
			trig = As_Pointer("XXXX");
			if (trig) {
				trig->Remove();
			}
			delete trig;
			break;

		case ACTION_DESTROY_YYYY:
			trig = As_Pointer("YYYY");
			if (trig) {
				trig->Remove();
			}
			delete trig;
			break;

		case ACTION_DESTROY_ZZZZ:
			trig = As_Pointer("ZZZZ");
			if (trig) {
				trig->Remove();
			}
			delete trig;
			break;

		case ACTION_AIRSTRIKE:
			PlayerPtr->IsAirstrikePending = true;
//			PlayerPtr->Make_Air_Strike_Available(true);
			break;

		case ACTION_DZ:
			new AnimClass(ANIM_LZ_SMOKE, Cell_Coord(Waypoint[25]));
			break;

		case ACTION_NONE:
			break;

		case ACTION_WIN:
			PlayerPtr->Flag_To_Win();
			break;

		case ACTION_LOSE:
			PlayerPtr->Flag_To_Lose();
			break;

		case ACTION_BEGIN_PRODUCTION:
			HouseClass::As_Pointer(House)->Begin_Production();
			break;

		case ACTION_AUTOCREATE:
			if (obj && obj->Is_Techno()) {
				((TechnoClass *)obj)->House->IsAlerted = true;
			}
			break;

		case ACTION_CREATE_TEAM:
			if (Team) {
				ScenarioInit++;
				Team->Create_One_Of();
				ScenarioInit--;
			}
			break;

		case ACTION_DESTROY_TEAM:
			if (Team) {
				Team->Destroy_All_Of();
			}
			break;

		case ACTION_REINFORCEMENTS:
			if (Team) {
				success = Do_Reinforcements(Team);
			}
			break;

		case ACTION_ALL_HUNT:
			Do_All_To_Hunt();
			break;

		default:
			break;
	}

	if (!success && Event == EVENT_TIME) Data = 1;

	/*
	**	Remove trigger from the game.
	*/
	if (success && IsPersistant == VOLATILE) {
		Remove();
	}

	return(true);
}


/***********************************************************************************************
 * TriggerClass::Spring -- Trigger processing routine                                          *
 *                                                                                             *
 * This version of Spring is for cell-based triggers.                                          *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      data      elapsed time, or credits, depending on what 'Event' is.                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      0 = nothing happened; 1 = the trigger was sprung                                       *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/06/1994 BR : Created.                                                                  *
 *=============================================================================================*/
bool TriggerClass::Spring(EventType event, CELL cell)
{
	Validate();
	/*
	**	If this is not the event for this trigger, just return.
	*/
	if (event != Event) {
		return(false);
	}

	/*
	**	If time-based, decrement the minute counter; return if it's not time yet
	*/
	if (Event == EVENT_TIME) {
		Data--;
		if (Data > 0) {
			return(false);
		}
		Data = DataCopy;
	}

	/*
	**	Semi-persistant trigger: first detach it from the calling cell, then
	**	see if this is the last cell we're attached to; if so, the trigger
	**	will spring.
	*/
	if (IsPersistant == SEMIPERSISTANT) {

		/*
		** Detach ourselves from the cell
		*/
		Map[cell].IsTrigger = 0;

		/*
		** Decrement our attachment counter
		*/
		AttachCount--;

		/*
		** If we're attached to more cells, don't spring; otherwise, spring.
		** And, mark ourselves as volatile so we'll completely remove ourselves
		** from the game after we go off.
		*/
		if (AttachCount > 0) {
			return(false);
		} else {
			IsPersistant = VOLATILE;
		}
	}

	/*
	**	Otherwise, take an appropriate action.
	*/
	bool success = true;
	TriggerClass * trig = NULL;
	int index;
	switch (Action) {
		case ACTION_NUKE:
			HouseClass::As_Pointer(HOUSE_BAD)->NukeStrike.Enable(true, false);
			HouseClass::As_Pointer(HOUSE_BAD)->NukeStrike.Forced_Charge(PlayerPtr->Class->House == HOUSE_BAD);
			break;

		case ACTION_ION:
			HouseClass::As_Pointer(HOUSE_GOOD)->IonCannon.Enable(true, false);
			HouseClass::As_Pointer(HOUSE_GOOD)->IonCannon.Forced_Charge(PlayerPtr->Class->House == HOUSE_GOOD);
			break;

		case ACTION_AUTOCREATE:
			for (index = 0; index < Houses.Count(); index++) {
				Houses.Ptr(index)->IsAlerted = true;
			}
			break;

		case ACTION_DESTROY_XXXX:
			trig = As_Pointer("XXXX");
			if (trig) {
				trig->Remove();
			}
			delete trig;
			break;

		case ACTION_DESTROY_YYYY:
			trig = As_Pointer("YYYY");
			if (trig) {
				trig->Remove();
			}
			delete trig;
			break;

		case ACTION_DESTROY_ZZZZ:
			trig = As_Pointer("ZZZZ");
			if (trig) {
				trig->Remove();
			}
			delete trig;
			break;

		case ACTION_AIRSTRIKE:
			HouseClass::As_Pointer(House)->AirStrike.Enable(false, true);
			if (House == PlayerPtr->Class->House) {
				PlayerPtr->AirStrike.Forced_Charge(true);
				Map.Add(RTTI_SPECIAL, SPC_AIR_STRIKE);
				Map.Column[1].Flag_To_Redraw();
			}
//			PlayerPtr->Make_Air_Strike_Available(true);
			break;

		case ACTION_DZ:
			new AnimClass(ANIM_LZ_SMOKE, Cell_Coord(Waypoint[25]));
			break;

		case ACTION_NONE:
			break;

		case ACTION_WIN:
			PlayerPtr->Flag_To_Win();
			break;

		case ACTION_LOSE:
			PlayerPtr->Flag_To_Lose();
			break;

		case ACTION_BEGIN_PRODUCTION:
			if (PlayerPtr->Class->House == HOUSE_GOOD) {
				HouseClass::As_Pointer(HOUSE_BAD)->Begin_Production();
			} else {
				HouseClass::As_Pointer(HOUSE_GOOD)->Begin_Production();
			}
			break;

		case ACTION_CREATE_TEAM:
			if (Team) {
				ScenarioInit++;
				Team->Create_One_Of();
				ScenarioInit--;
			}
			break;

		case ACTION_DESTROY_TEAM:
			if (Team) {
				Team->Destroy_All_Of();
			}
			break;

		case ACTION_REINFORCEMENTS:
			if (Team) {
				success = Do_Reinforcements(Team);
			}
			break;

		case ACTION_ALL_HUNT:
			Do_All_To_Hunt();
			break;

		default:
			break;
	}

	if (!success && Event == EVENT_TIME) Data = 1;

	/*
	**	Remove trigger from the game.
	*/
	if (success && IsPersistant == VOLATILE) {
		Remove();
	}

	return(true);
}


/***********************************************************************************************
 * TriggerClass::Spring -- Trigger processing routine                                          *
 *                                                                                             *
 * This version of Spring is for house-specific triggers.                                      *
 * For a time-based trigger, 'data' will the the current TickCount.                            *
 * For a credit-based trigger, 'data' will be the credits for the HouseClass                   *
 * containing this trigger.                                                                    *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      event      the event that happened                                                     *
 *      house      house that this event relates to                                            *
 *      data      elapsed time, or credits, depending on what 'Event' is.                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      0 = nothing happened; 1 = the trigger was sprung                                       *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/06/1994 BR : Created.                                                                  *
 *   06/25/1995 JLB : Added more trigger events.                                               *
 *=============================================================================================*/
bool TriggerClass::Spring(EventType event, HousesType house, long data)
{
	Validate();
	/*
	**	If this is not the event for this trigger, just return.
	*/
	if (event != Event || house != House) {
		return(false);
	}

	/*
	**	If credits-based, check 'data'
	*/
	if (Event == EVENT_CREDITS && data < Data) {
		return(false);
	}

	/*
	**	Building event check to ensure that the building number matches.
	*/
	if (Event == EVENT_BUILD && data != Data) {
		return(false);
	}

	/*
	**	Number of objects destroyed checker. If the data supplied indicates that
	**	the correct number of objects have been destroyed, then this trigger
	**	will succeed.
	*/
	if (Event == EVENT_NBUILDINGS_DESTROYED || Event == EVENT_NUNITS_DESTROYED) {
		if (data < Data) {
			return(false);
		}
	}

	/*
	**	If time-based, decrement the minute counter; return if it's not time yet
	*/
	if (Event == EVENT_TIME) {
		Data--;
		if (Data > 0) {
			return(false);
		}
		Data = DataCopy;
	}

	/*
	**	The trigger has gone off; take appropriate action
	*/
	bool success = true;
	TriggerClass * trig = NULL;
	switch (Action) {

		case ACTION_NUKE:
			HouseClass::As_Pointer(HOUSE_BAD)->NukeStrike.Enable(true, false);
			HouseClass::As_Pointer(HOUSE_BAD)->NukeStrike.Forced_Charge(PlayerPtr->Class->House == HOUSE_BAD);
			break;

		case ACTION_ION:
			HouseClass::As_Pointer(HOUSE_GOOD)->IonCannon.Enable(true, false);
			HouseClass::As_Pointer(HOUSE_GOOD)->IonCannon.Forced_Charge(PlayerPtr->Class->House == HOUSE_GOOD);
			break;

		/*
		**	This will remove a blockage to the win condition. No action need
		**	be performed here since the act of deleting the trigger will
		**	remove the blockage.
		*/
		case ACTION_ALLOWWIN:
			break;

		case ACTION_AUTOCREATE:
			HouseClass::As_Pointer(House)->IsAlerted = true;
			break;

		case ACTION_DESTROY_XXXX:
			trig = As_Pointer("XXXX");
			if (trig) {
				trig->Remove();
			}
			delete trig;
			break;

		case ACTION_DESTROY_YYYY:
			trig = As_Pointer("YYYY");
			if (trig) {
				trig->Remove();
			}
			delete trig;
			break;

		case ACTION_DESTROY_ZZZZ:
			trig = As_Pointer("ZZZZ");
			if (trig) {
				trig->Remove();
			}
			delete trig;
			break;

		case ACTION_AIRSTRIKE:
			PlayerPtr->AirStrike.Enable(false, true);
			if (House == PlayerPtr->Class->House) {
				PlayerPtr->AirStrike.Forced_Charge(true);
				Map.Add(RTTI_SPECIAL, SPC_AIR_STRIKE);
				Map.Column[1].Flag_To_Redraw();
			}
			break;

		case ACTION_NONE:
			break;

		case ACTION_DZ:
			new AnimClass(ANIM_LZ_SMOKE, Cell_Coord(Waypoint[25]));
			break;

		case ACTION_WIN:
			PlayerPtr->Flag_To_Win();
			break;

		case ACTION_LOSE:
			PlayerPtr->Flag_To_Lose();
			break;

		case ACTION_BEGIN_PRODUCTION:
			HouseClass::As_Pointer(House)->Begin_Production();
			break;

		case ACTION_CREATE_TEAM:
			if (Team) {
				ScenarioInit++;
				Team->Create_One_Of();
				ScenarioInit--;
			}
			break;

		case ACTION_DESTROY_TEAM:
			if (Team) {
				Team->Destroy_All_Of();
			}
			break;

		case ACTION_REINFORCEMENTS:
			if (Team) {
				success = Do_Reinforcements(Team);
			}
			break;

		case ACTION_ALL_HUNT:
			Do_All_To_Hunt();
			break;

		default:
			break;
	}

	if (!success && Event == EVENT_TIME) Data = 1;

	/*
	**	Remove trigger from the game.
	*/
	if (success && IsPersistant == VOLATILE) {
		Remove();
	}

	return(true);
}


/***********************************************************************************************
 * TriggerClass::Remove -- removes this trigger from the game                                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      1 = trigger was removed, 0 = it wasn't                                                 *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/06/1994 BR : Created.                                                                  *
 *=============================================================================================*/
bool TriggerClass::Remove(void)
{
	Validate();
	CELL cell;
	HousesType h;
	int index;

	/*
	**	Loop through all cells; remove any reference to this trigger
	*/
	for (cell = 0; cell < MAP_CELL_TOTAL; cell++) {
		if (Map[cell].IsTrigger) {
			if (CellTriggers[cell] == this) {
				Map[cell].IsTrigger = 0;
				CellTriggers[cell] = NULL;
			}
		}
	}

	/*
	**	Loop through all objects, removing any reference to this trigger
	*/
	for (index = 0; index < Infantry.Count(); index++) {
		if (Infantry.Ptr(index)->Trigger == this) {
			Infantry.Ptr(index)->Trigger = NULL;
		}
	}
	for (index = 0; index < Buildings.Count(); index++) {
		if (Buildings.Ptr(index)->Trigger == this) {
			Buildings.Ptr(index)->Trigger = NULL;
		}
	}
	for (index = 0; index < Units.Count(); index++) {
		if (Units.Ptr(index)->Trigger == this) {
			Units.Ptr(index)->Trigger = NULL;
		}
	}
	for (index = 0; index < Terrains.Count(); index++) {
		if (Terrains.Ptr(index)->Trigger == this) {
			Terrains.Ptr(index)->Trigger = NULL;
		}
	}

	/*
	**	Remove this trigger from any house list it's in. Invoking '-=' with a
	**	pointer not in the list has no effect; loop through all houses just to
	**	be on the safe side.
	*/
	for (h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
		HouseTriggers[h].Delete(this);
	}

	delete this;

	return(true);
}


/***********************************************************************************************
 * TriggerClass::Read_INI -- reads triggers from the INI file                                  *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Triggername = Eventname, Actionname, Data, Housename, TeamName, IsPersistant           *
 *                                                                                             *
 * This routine reads in the triggers & creates them. Then, other classes can                  *
 * get pointers to the triggers they're linked to.                                             *
 *                                                                                             *
 * The routine relies on the TeamTypeClasses already being loaded so it can resolve            *
 * references to teams in this function.                                                       *
 *                                                                                             *
 * Cell Trigger pointers & IsTrigger flags are set in DisplayClass::Read_INI(),                *
 * and cleared in the Map::Init() routine (which clears all cell objects to 0's).              *
 *                                                                                             *
 * Object's pointers are set in:                                                               *
 *      InfantryClass::Read_INI()                                                              *
 *      BuildingClass::Read_INI()                                                              *
 *      UnitClass::Read_INI()                                                                  *
 *      TerrainClass::Read_INI()                                                               *
 * The object trigger pointers are cleared in the ObjectClass constructor.                     *
 *                                                                                             *
 * The House's EMSListOf triggers is set in this routine, and cleared in the                   *
 * HouseClass::Init() routine.                                                                 *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      buffer      buffer to hold the INI data                                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      This function must be called before any other class's Read_INI.                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void TriggerClass::Read_INI(char *buffer)
{
	TriggerClass *trigger;				// Working trigger pointer.
	char *tbuffer;							// Accumulation buffer of trigger IDs.
	int len;									// Length of data in buffer.
	char buf[128];

	/*
	**	Set 'tbuffer' to point just past the INI buffer
	*/
	len = strlen(buffer) + 2;
	tbuffer = buffer + len;

	/*
	**	Read all TRIGGER entry names into 'tbuffer'
	*/
	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-len, buffer);

	/*
	**	Loop for all trigger entries.
	*/
	while (*tbuffer != '\0') {

		/*
		**	Create a new trigger.
		*/
		trigger = new TriggerClass();

		/*
		**	Set its name.
		*/
		trigger->Set_Name (tbuffer);

		/*
		**	Get the trigger entry.
		*/
		WWGetPrivateProfileString(INI_Name(), tbuffer, NULL, buf, sizeof(buf)-1, buffer);

		/*
		**	Fill in the trigger.
		*/
		trigger->Fill_In(tbuffer,buf);

		/*
		**	Add 'trigger' to the House's list.
		*/
//		if (trigger->House != HOUSE_NONE && trigger->Event != EVENT_PLAYER_ENTERED) {
//		if (Event_Need_House(trigger->Event) && !Event_Need_Object(trigger->Event)) {
		if (trigger->House != HOUSE_NONE) {
			if (trigger->Action == ACTION_ALLOWWIN) HouseClass::As_Pointer(trigger->House)->Blockage++;
			HouseTriggers[trigger->House].Add(trigger);
			trigger->AttachCount++;
		}

		/*
		**	Go to next entry.
		*/
		tbuffer += strlen(tbuffer)+1;
	}
}


/***********************************************************************************************
 * TriggerClass::Fill_In -- fills in trigger from the given INI entry                          *
 *                                                                                             *
 * This routine fills in the given trigger with the given name, and values from                *
 * the given INI entry.                                                                        *
 *                                                                                             *
 * (This routine is used by the scenario editor, to import teams from the MASTER.INI file.)    *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Triggername = Eventname, Actionname, Data, Housename, TeamName, IsPersistant           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      name      mnemonic for the desired trigger                                             *
 *      entry      INI entry to parse                                                          *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void TriggerClass::Fill_In(char * name, char *entry)
{
	Validate();
	char *p;

	/*
	**	Set its name.
	*/
	Set_Name(name);

	/*
	**	1st token: Event.
	*/
	Event = Event_From_Name(strtok(entry, ","));

	/*
	**	2nd token: Action.
	*/
	Action = Action_From_Name(strtok(NULL, ","));

	/*
	**	3rd token: Data.
	*/
	DataCopy = Data = atol(strtok(NULL, ","));

	/*
	**	4th token: House.
	*/
	House = HouseTypeClass::From_Name(strtok(NULL, ","));
	if (House == HOUSE_NONE && Event == EVENT_PLAYER_ENTERED) {
		House = PlayerPtr->Class->House;
	}

	/*
	**	5th token: Team.
	*/
	Team = TeamTypeClass::As_Pointer(strtok(NULL, ","));

	/*
	** 6th token: IsPersistant.  This token was added later, so we must check
	** for its existence.
	*/
	p = strtok(NULL, ",");
	if (p) {
		IsPersistant = (PersistantType)atoi(p);
	} else {
		IsPersistant = VOLATILE;
	}
}


/***********************************************************************************************
 * TriggerClass::Write_INI -- writes triggers to the INI file                                  *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Triggername = Eventname, Actionname, Data, Housename, TeamName, IsPersistant           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      buffer      buffer to hold the INI data                                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void TriggerClass::Write_INI(char *buffer, bool refresh)
{
	int index;
	char buf[128];
	TriggerClass *trigger;
	char const *hname;
	char const *tname;

	/*
	**	First, clear out all existing trigger data from the INI file.
	*/
	if (refresh) {
		WWWritePrivateProfileString(INI_Name(), NULL, NULL, buffer);
	}

	/*
	**	Now write all the trigger data out
	*/
	for (index = 0; index < Triggers.Count(); index++) {

		/*
		**	Get ptr to next active trigger.
		*/
		trigger = Triggers.Ptr(index);

		/*
		**	Generate INI entry.
		*/
		if (trigger->House==HOUSE_NONE) {
			hname = "None";
		} else {
			hname = HouseClass::As_Pointer(trigger->House)->Class->IniName;
		}

		if (trigger->Team==NULL) {
			tname = "None";
		} else {
			tname = trigger->Team->IniName;
		}

		sprintf(buf,"%s,%s,%ld,%s,%s,%d",
			TriggerClass::Name_From_Event(trigger->Event),
			TriggerClass::Name_From_Action(trigger->Action),
			trigger->Data,
			hname,
			tname,
			trigger->IsPersistant);
		WWWritePrivateProfileString(INI_Name(), trigger->Get_Name(), buf, buffer);
	}
}


/***********************************************************************************************
 * TriggerClass::As_Pointer -- returns pointer for the given trigger name                      *
 *                                                                                             *
 * This function is designed for use at initialization time, ie when the                       *
 * trigger mnemonics are read from the INI and the Read_INI functions need                     *
 * to get a pointer to that trigger.                                                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      name      mnemonic for the desired trigger                                             *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      near pointer to that trigger, NULL if not found                                        *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
TriggerClass * TriggerClass::As_Pointer(char const * name)
{
	if (name == NULL) {
		return(NULL);
	}

	for (int i = 0; i < Triggers.Count(); i++) {
		TriggerClass * trigger = Triggers.Ptr(i);

		if (!stricmp(name, trigger->Name)) {
			return(trigger);
		}
	}

	return(NULL);
}


/***********************************************************************************************
 * TriggerClass::operator new -- 'new' operator                                                *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      pointer to new trigger                                                                 *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void * TriggerClass::operator new(size_t )
{
	void * ptr = Triggers.Allocate();
	if (ptr) {
		((TriggerClass *)ptr)->IsActive = true;
	}
	return(ptr);
}


/***********************************************************************************************
 * TriggerClass::operator delete -- 'delete' operator                                          *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      ptr      pointer to delete                                                             *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
void TriggerClass::operator delete(void *ptr)
{
	if (ptr) {
		((TriggerClass *)ptr)->IsActive = false;
	}
	Triggers.Free((TriggerClass *)ptr);
}


/***********************************************************************************************
 * TriggerClass::Event_From_Name -- retrieves EventType for given name                         *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      name      name to get event for                                                        *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      EventType for given name                                                               *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 BR : Created.                                                                  *
 *=============================================================================================*/
EventType TriggerClass::Event_From_Name (char const *name)
{
	int i;

	if (name == NULL) {
		return(EVENT_NONE);
	}

	for (i = EVENT_NONE; i < EVENT_COUNT; i++) {
		if (!stricmp(name,EventText[i + 1])) {
			return((EventType)i);
		}
	}

	return(EVENT_NONE);
}


/***********************************************************************************************
 * TriggerClass::Name_From_Event -- retrieves name for EventType                               *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      event      EventType to get name for                                                   *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      name for EventType                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 BR : Created.                                                                  *
 *=============================================================================================*/
char const *TriggerClass::Name_From_Event(EventType event)
{
	return(EventText[event + 1]);
}


/***********************************************************************************************
 * TriggerClass::Action_From_Name -- retrieves ActionType for given name                       *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      name         name to get ActionType for                                                *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      ActionType for given name                                                              *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 BR : Created.                                                                  *
 *=============================================================================================*/
TriggerClass::ActionType TriggerClass::Action_From_Name (char const *name)
{
	int i;

	if (name == NULL) {
		return(ACTION_NONE);
	}

	for (i = ACTION_NONE; i < ACTION_COUNT; i++) {
		if (!stricmp(name,ActionText[i + 1])) {
			return((ActionType)i);
		}
	}

	return(ACTION_NONE);
}


/***********************************************************************************************
 * TriggerClass::Name_From_Action -- retrieves name for ActionType                             *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      action      ActionType to get name for                                                 *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      name of ActionType                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 BR : Created.                                                                  *
 *=============================================================================================*/
char const *TriggerClass::Name_From_Action(ActionType action)
{
	return(ActionText[action + 1]);
}


/***********************************************************************************************
 * TriggerClass::As_Target -- Converts trigger to a target value                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  TARGET value                                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET TriggerClass::As_Target(void) const
{
	Validate();
	return(Build_Target(KIND_TRIGGER, Triggers.ID(this)));
}


/***********************************************************************************************
 * Do_All_To_Hunt -- Forces all computer controlled units into hunt mode.                      *
 *                                                                                             *
 *    This trigger action will cause the computer units and infantry to go into hunt mode.     *
 *    Use it to bring a scenario to a sudden conclusion.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/20/1995 JLB : Created.                                                                 *
 *   08/14/1995 JLB : Removes the member from a team if necessary.                             *
 *=============================================================================================*/
static void Do_All_To_Hunt(void)
{
	int index;

	for (index = 0; index < Units.Count(); index++) {
		UnitClass * unit = Units.Ptr(index);

		if (!unit->House->IsHuman && unit->IsDown && !unit->IsInLimbo) {
			if (unit->Team) unit->Team->Remove(unit);
			unit->Assign_Mission(MISSION_HUNT);
		}
	}

	for (index = 0; index < Infantry.Count(); index++) {
		InfantryClass * infantry = Infantry.Ptr(index);

		if (!infantry->House->IsHuman && infantry->IsDown && !infantry->IsInLimbo) {
			if (infantry->Team) infantry->Team->Remove(infantry);
			infantry->Assign_Mission(MISSION_HUNT);
		}
	}
}
