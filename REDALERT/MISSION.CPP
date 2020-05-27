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

/* $Header: /CounterStrike/MISSION.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MISSION.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 23, 1994                                               *
 *                                                                                             *
 *                  Last Update : September 14, 1996 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   MissionClass::AI -- Processes order script.                                               *
 *   MissionClass::Assign_Mission -- Give an order to a unit.                                  *
 *   MissionClass::Commence -- Start script with new order.                                    *
 *   MissionClass::Debug_Dump -- Dumps status values to mono screen.                           *
 *   MissionClass::Get_Mission -- Fetches the mission that this object is acting under.        *
 *   MissionClass::MissionClass -- Default constructor for the mission object type.            *
 *   MissionClass::Mission_???  -- Stub mission functions that do nothing.                     *
 *   MissionClass::Mission_From_Name -- Fetch order pointer from its name.                     *
 *   MissionClass::Mission_Name -- Converts a mission number into an ASCII string.             *
 *   MissionClass::Override_Mission -- temporarily overrides the units mission                 *
 *   MissionClass::Restore_Mission -- Restores overridden mission                              *
 *   MissionClass::Set_Mission -- Sets the mission to the specified value.                     *
 *   MissionClass::Is_Recruitable_Mission -- Determines if this mission is recruitable for a te*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***********************************************************************************************
 * MissionClass::MissionClass -- Default constructor for the mission object type.              *
 *                                                                                             *
 *    This is the default constructor for the mission class object. It sets the mission        *
 *    handler into a default -- do nothing -- state.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *   03/01/1996 JLB : Uses initializer lists.                                                  *
 *=============================================================================================*/
MissionClass::MissionClass(RTTIType rtti, int id) :
	ObjectClass(rtti, id),
	Mission(MISSION_NONE),
	SuspendedMission(MISSION_NONE),
	MissionQueue(MISSION_NONE),
	Status(0),
	Timer(0)
{
}


/***********************************************************************************************
 * MissionClass::Mission_???  -- Stub mission functions that do nothing.                       *
 *                                                                                             *
 *    These are the stub routines that handle the mission logic. They do nothing at this       *
 *    level. Derived classes will override these routine as necessary.                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this mission        *
 *          handler again.                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int MissionClass::Mission_Sleep(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Ambush(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Attack(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Capture(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Guard(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Guard_Area(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Harvest(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Hunt(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Move(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Retreat(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Return(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Stop(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Unload(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Enter(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Construction(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Deconstruction(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Repair(void) {return TICKS_PER_SECOND*30;};
int MissionClass::Mission_Missile(void) {return TICKS_PER_SECOND*30;};


/***********************************************************************************************
 * MissionClass::Set_Mission -- Sets the mission to the specified value.                       *
 *                                                                                             *
 *    Use this routine to set the current mission for this object. This routine will blast     *
 *    over the current mission, bypassing the queue method. Call it when the mission needs     *
 *    to be changed immediately.                                                               *
 *                                                                                             *
 * INPUT:   mission  -- The mission to set to.                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void MissionClass::Set_Mission(MissionType mission)
{
	assert(IsActive);

	Mission = mission;
	MissionQueue = MISSION_NONE;
}


/***********************************************************************************************
 * MissionClass::Get_Mission -- Fetches the mission that this object is acting under.          *
 *                                                                                             *
 *    Use this routine to fetch the mission that this object is CURRENTLY acting under. The    *
 *    mission queue may be filled with a imminent mission change, but this routine does not    *
 *    consider that. It only returns the CURRENT mission.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the mission that this unit is currently following.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
MissionType MissionClass::Get_Mission(void) const
{
	assert(IsActive);

	return(Mission == MISSION_NONE ? MissionQueue : Mission);
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * MissionClass::Debug_Dump -- Dumps status values to mono screen.                             *
 *                                                                                             *
 *    This is a debugging function that dumps this class' status to the monochrome screen      *
 *    for review.                                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void MissionClass::Debug_Dump(MonoClass * mono) const
{
	assert(IsActive);

	mono->Set_Cursor(1, 9);mono->Printf("%-14s", MissionClass::Mission_Name(Mission));
	mono->Set_Cursor(16, 9);mono->Printf("%-12s", MissionClass::Mission_Name(MissionQueue));
	mono->Set_Cursor(1, 7);mono->Printf("%3d", (long)Timer);
	mono->Set_Cursor(6, 7);mono->Printf("%2d", Status);

	ObjectClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * MissionClass::AI -- Processes order script.                                                 *
 *                                                                                             *
 *    This routine will process the order script for as much time as                           *
 *    possible or until a script delay is detected. This routine should                        *
 *    be called for every unit once per game loop (if possible).                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/23/1994 JLB : Created.                                                                 *
 *   06/25/1995 JLB : Added new missions.                                                      *
 *=============================================================================================*/
void MissionClass::AI(void)
{
	assert(IsActive);

	ObjectClass::AI();

	/*
	**	If this is the kind of object that is "paralyzed with fear" while it is above
	**	ground level (such as when be paradropped), it will perform no mission AI
	**	processing.
	*/
	if ((What_Am_I() == RTTI_INFANTRY || What_Am_I() == RTTI_UNIT || What_Am_I() == RTTI_VESSEL) && Height > 0) {
		return;
	}

	/*
	**	This is the script AI equivalent processing.
	*/
	BStart(BENCH_MISSION);
	if (Timer == 0 && Strength > 0) {
		switch (Mission) {
			default:
				Timer = Mission_Sleep();
				break;

			case MISSION_HARMLESS:
			case MISSION_SLEEP:
				Timer = Mission_Sleep();
				break;

			case MISSION_STICKY:
			case MISSION_GUARD:
				Timer = Mission_Guard();
				break;

			case MISSION_ENTER:
				Timer = Mission_Enter();
				break;

			case MISSION_CONSTRUCTION:
				Timer = Mission_Construction();
				break;

			case MISSION_DECONSTRUCTION:
				Timer = Mission_Deconstruction();
				break;

			case MISSION_CAPTURE:
			case MISSION_SABOTAGE:
				Timer = Mission_Capture();
				break;

			case MISSION_QMOVE:
			case MISSION_MOVE:
				Timer = Mission_Move();
				break;

			case MISSION_ATTACK:
				Timer = Mission_Attack();
				break;

			case MISSION_RETREAT:
				Timer = Mission_Retreat();
				break;

			case MISSION_HARVEST:
				Timer = Mission_Harvest();
				break;

			case MISSION_GUARD_AREA:
				Timer = Mission_Guard_Area();
				break;

			case MISSION_RETURN:
				Timer = Mission_Return();
				break;

			case MISSION_STOP:
				Timer = Mission_Stop();
				break;

			case MISSION_AMBUSH:
				Timer = Mission_Ambush();
				break;

			case MISSION_HUNT:
			case MISSION_RESCUE:
				Timer = Mission_Hunt();
				break;

//			case MISSION_TIMED_HUNT:
//				Timer = Mission_Timed_Hunt();
//				break;

			case MISSION_UNLOAD:
				Timer = Mission_Unload();
				break;

			case MISSION_REPAIR:
				Timer = Mission_Repair();
				break;

			case MISSION_MISSILE:
				Timer = Mission_Missile();
				break;
		}
	}
	BEnd(BENCH_MISSION);
}


/***********************************************************************************************
 * MissionClass::Commence -- Start script with new order.                                      *
 *                                                                                             *
 *    This routine will start script processing according to any queued                        *
 *    order it may have. If there is no queued order, then this routine                        *
 *    does nothing. Call this routine whenever the unit is in a good                           *
 *    position to change its order (such as when it is stopped).                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Did the mission actually change?                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/23/1994 JLB : Created.                                                                 *
 *   07/14/1994 JLB : Simplified.                                                              *
 *   06/17/1995 JLB : Returns success flag.                                                    *
 *=============================================================================================*/
bool MissionClass::Commence(void)
{
	assert(IsActive);

	if (MissionQueue != MISSION_NONE) {
		Mission = MissionQueue;
		MissionQueue = MISSION_NONE;

		/*
		**	Force immediate state machine processing at the first state machine state value.
		*/
		Timer = 0;
		Status = 0;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * MissionClass::Assign_Mission -- Give an order to a unit.                                    *
 *                                                                                             *
 *    This routine will assign the specified mission to the mission queue for this object.     *
 *    The actual mission logic will then be performed at the first available and legal         *
 *    opportunity.                                                                             *
 *                                                                                             *
 * INPUT:   order -- Mission to give the unit.                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/04/1991 JLB : Created.                                                                 *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
void MissionClass::Assign_Mission(MissionType order)
{
	assert(IsActive);

	/*
	**	Ensure that a MISSION_QMOVE is translated into a MISSION_MOVE.
	*/
	if (order == MISSION_QMOVE) order = MISSION_MOVE;

	if (order != MISSION_NONE && Mission != order)  {
		MissionQueue = order;
	}
}


/***********************************************************************************************
 * MissionClass::Mission_From_Name -- Fetch order pointer from its name.                       *
 *                                                                                             *
 *    This routine is used to convert an ASCII order name into the actual                      *
 *    order number it represents. Typically, this is used when processing                      *
 *    a scenario INI file.                                                                     *
 *                                                                                             *
 * INPUT:   name  -- The ASCII order name to process.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with the actual order number that the ASCII name                           *
 *          represents.                                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   04/22/1994 JLB : Converted to static member function.                                     *
 *=============================================================================================*/
MissionType MissionClass::Mission_From_Name(char const * name)
{
	MissionType	order;

	if (name) {
		for (order = MISSION_FIRST; order < MISSION_COUNT; order++) {
			if (stricmp(Missions[order], name) == 0) {
				return(order);
			}
		}
	}
	return(MISSION_NONE);
}


/***********************************************************************************************
 * MissionClass::Mission_Name -- Converts a mission number into an ASCII string.               *
 *                                                                                             *
 *    Use this routine to convert a mission number into the ASCII string that represents       *
 *    it. Typical use of this is when generating an INI file.                                  *
 *                                                                                             *
 * INPUT:   mission  -- The mission number to convert.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the ASCII string that represents the mission type.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * MissionClass::Mission_Name(MissionType mission)
{
	if (mission != MISSION_NONE)  {
		return(Missions[mission]);
	}
	return("None");
}


/***********************************************************************************************
 * MissionClass::Override_Mission -- temporarily overrides the units mission                   *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:      MissionType mission - the mission we want to override                           *
 *               TARGET      tarcom  - the new target we want to override                      *
 *               TARGET      navcom  - the new navigation point to override                    *
 *                                                                                             *
 * OUTPUT:      none                                                                           *
 *                                                                                             *
 * WARNINGS:   If a mission is already overridden, the current mission is                      *
 *               just re-assigned.                                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/28/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void MissionClass::Override_Mission(MissionType mission, TARGET, TARGET)
{
	assert(IsActive);

	if (MissionQueue != MISSION_NONE) {
		SuspendedMission = MissionQueue;
	} else {
		SuspendedMission = Mission;
	}

	Assign_Mission(mission);
}


/***********************************************************************************************
 * MissionClass::Restore_Mission -- Restores overridden mission                                *
 *                                                                                             *
 * INPUT:      none                                                                            *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/28/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
bool MissionClass::Restore_Mission(void)
{
	assert(IsActive);

	if (SuspendedMission != MISSION_NONE) {
		Assign_Mission(SuspendedMission);
	 	SuspendedMission= MISSION_NONE;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * MissionClass::Is_Recruitable_Mission -- Determines if this mission is recruitable for a tea *
 *                                                                                             *
 *    Some missions preclude recruitment into a team. This routine will examine the mission    *
 *    specified and if not allowed for a team, it will return false.                           *
 *                                                                                             *
 * INPUT:   mission  -- The mission type to examine.                                           *
 *                                                                                             *
 * OUTPUT:  bool; Is an object following this mission allowed to be recruited into a team?     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool MissionClass::Is_Recruitable_Mission(MissionType mission)
{
	if (mission == MISSION_NONE) {
		return(true);
	}
	return(MissionControl[mission].IsRecruitable);
}



MissionControlClass::MissionControlClass(void) :
	Mission(MISSION_NONE),
	IsNoThreat(false),
	IsZombie(false),
	IsRecruitable(true),
	IsParalyzed(false),
	IsRetaliate(true),
	IsScatter(true),
	Rate(".016"),
	AARate(".016")
{
}


char const * MissionControlClass::Name(void) const
{
	if (Mission == MISSION_NONE) {
		return("<none>");
	}
	return(Missions[Mission]);
}



bool MissionControlClass::Read_INI(CCINIClass & ini)
{
	if (ini.Is_Present(Name())) {
		IsNoThreat = ini.Get_Bool(Name(), "NoThreat", IsNoThreat);
		IsZombie = ini.Get_Bool(Name(), "Zombie", IsZombie);
		IsRecruitable = ini.Get_Bool(Name(), "Recruitable", IsRecruitable);
		IsParalyzed = ini.Get_Bool(Name(), "Paralyzed", IsParalyzed);
		IsRetaliate = ini.Get_Bool(Name(), "Retaliate", IsRetaliate);
		IsScatter = ini.Get_Bool(Name(), "Scatter", IsScatter);
		Rate = ini.Get_Fixed(Name(), "Rate", Rate);
		AARate = ini.Get_Fixed(Name(), "AARate", 0);
		if (AARate == 0) {
			AARate = Rate;
		}
		return(true);
	}
	return(false);
}
