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

/* $Header: /CounterStrike/TEAM.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TEAM.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/11/94                                                     *
 *                                                                                             *
 *                  Last Update : August 27, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TeamClass::AI -- Process team logic.                                                      *
 *   TeamClass::Add -- Adds specified object to team.                                          *
 *   TeamClass::Assign_Mission_Target -- Sets teams mission target and clears old target       *
 *   TeamClass::Calc_Center -- Determines average location of team members.                    *
 *   TeamClass::Can_Add -- Determines if the specified object can be added to team.            *
 *   TeamClass::Control -- Updates control on a member unit.                                   *
 *   TeamClass::Coordinate_Attack -- Handles coordinating a team attack.                       *
 *   TeamClass::Coordinate_Conscript -- Gives orders to new recruit.                           *
 *   TeamClass::Coordinate_Do -- Handles the team performing specified mission.                *
 *   TeamClass::Coordinate_Move -- Handles team movement coordination.                         *
 *   TeamClass::Coordinate_Regroup -- Handles team idling (regrouping).                        *
 *   TeamClass::Debug_Dump -- Displays debug information about the team.                       *
 *   TeamClass::Detach -- Removes specified target from team tracking.                         *
 *   TeamClass::Fetch_A_Leader -- Looks for a suitable leader member of the team.              *
 *   TeamClass::Has_Entered_Map -- Determines if the entire team has entered the map.          *
 *   TeamClass::Init -- Initializes the team objects for scenario preparation.                 *
 *   TeamClass::Is_A_Member -- Tests if a unit is a member of a team                           *
 *   TeamClass::Is_Leaving_Map -- Checks if team is in process of leaving the map              *
 *   TeamClass::Lagging_Units -- Finds and orders any lagging units to catch up.               *
 *   TeamClass::Recruit -- Attempts to recruit members to the team for the given index ID.     *
 *   TeamClass::Remove -- Removes the specified object from the team.                          *
 *   TeamClass::Scan_Limit -- Force all members of the team to have limited scan range.        *
 *   TeamClass::Suspend_Teams -- Suspends activity for low priority teams                      *
 *   TeamClass::TMision_Patrol -- Handles patrolling from one location to another.             *
 *   TeamClass::TMission_Attack -- Perform the team attack mission command.                    *
 *   TeamClass::TMission_Follow -- Perform the "follow friendlies" team command.               *
 *   TeamClass::TMission_Formation -- Process team formation change command.                   *
 *   TeamClass::TMission_Invulnerable -- Makes the entire team invulnerable for a period of tim*
 *   TeamClass::TMission_Load -- Tells the team to load onto the transport now.                *
 *   TeamClass::TMission_Loop -- Causes the team mission processor to jump to new location.    *
 *   TeamClass::TMission_Set_Global -- Performs a set global flag operation.                   *
 *   TeamClass::TMission_Spy -- Perform the team spy mission.                                  *
 *   TeamClass::TMission_Unload -- Tells the team to unload passengers now.                    *
 *   TeamClass::TeamClass -- Constructor for the team object type.                             *
 *   TeamClass::Took_Damage -- Informs the team when the team member takes damage.             *
 *   TeamClass::operator delete -- Deallocates a team object.                                  *
 *   TeamClass::operator new -- Allocates a team object.                                       *
 *   TeamClass::~TeamClass -- Team object destructor.                                          *
 *   _Is_It_Breathing -- Checks to see if unit is an active team member.                       *
 *   _Is_It_Playing -- Determines if unit is active and an initiated team member.              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include "mission.h"


/***********************************************************************************************
 * _Is_It_Breathing -- Checks to see if unit is an active team member.                         *
 *                                                                                             *
 *    A unit could be a team member, but not be active. Such a case would occur when a         *
 *    reinforcement team is inside a transport. It could also occur if a unit is in the        *
 *    process of dying. Call this routine to ensure that the specified unit is a will and      *
 *    able participant in the team.                                                            *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the unit/infantry/aircraft that is to be checked.           *
 *                                                                                             *
 * OUTPUT:  bool; Is the specified unit active and able to be given commands by the team?      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static inline bool _Is_It_Breathing(FootClass const * object)
{
	/*
	**	If the object is not present or appears to be dead, then it
	**	certainly isn't an active member of the team.
	*/
	if (object == NULL || !object->IsActive || object->Strength == 0) return(false);

	/*
	**	If the object is in limbo, then it isn't an active team member either. However, if the
	**	scenario init flag is on, then it is probably a reinforcement issue or scenario
	**	creation situation. In such a case, the members are considered active because they need to
	**	be given special orders and treatment.
	*/
	if (!ScenarioInit && object->IsInLimbo) return(false);

	/*
	**	Nothing eliminated this object from being considered an active member of the team (i.e.,
	**	"breathing"), then return that it is ok.
	*/
	return(true);
}


/***********************************************************************************************
 * _Is_It_Playing -- Determines if unit is active and an initiated team member.                *
 *                                                                                             *
 *    Use this routine to determine if the specified unit is an active participant of the      *
 *    team. When a unit is first recruited to the team, it must travel to the team's location  *
 *    before it can become an active player. Call this routine to determine if the specified   *
 *    unit can be considered an active player.                                                 *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that is to be checked to see if it is an         *
 *                      active player.                                                         *
 *                                                                                             *
 * OUTPUT:  bool; Is the specified unit an active, living, initiated member of the team?       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static inline bool _Is_It_Playing(FootClass const * object)
{
	/*
	**	If the object is not active, then it certainly can be a participating member of the
	**	team.
	*/
	if (!_Is_It_Breathing(object)) return(false);

	/*
	**	Only members that have been "Initiated" are considered "playing" participants of the
	**	team. This results in the team members that are racing to regroup with the team (i.e.,
	**	not initiated), will continue to catch up to the team even while the initiated team members
	**	carry out their team specific orders.
	*/
	if (!object->IsInitiated && object->What_Am_I() != RTTI_AIRCRAFT) return(false);

	/*
	**	If it reaches this point, then nothing appears to disqualify the specified object from
	**	being considered an active playing member of the team. In this case, return that
	**	information.
	*/
	return(true);
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * TeamClass::Debug_Dump -- Displays debug information about the team.                         *
 *                                                                                             *
 *    This routine will display information about the team. This is useful for debugging       *
 *    purposes.                                                                                *
 *                                                                                             *
 * INPUT:   mono  -- Pointer to the monochrome screen that the debugging information will      *
 *                   be displayed on.                                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamClass::Debug_Dump(MonoClass * mono) const
{
	mono->Set_Cursor(1, 20);mono->Printf("%8.8s", Class->IniName);
	mono->Set_Cursor(10, 20);mono->Printf("%3d", Total);
	mono->Set_Cursor(17, 20);mono->Printf("%3d", Quantity[Class->ID]);
	if (CurrentMission != -1) {
		mono->Set_Cursor(1, 22);
		mono->Printf("%-29s", Class->MissionList[CurrentMission].Description(CurrentMission));
	}
	mono->Set_Cursor(40, 20);mono->Printf("%-10s", FormationName[Formation]);
	mono->Set_Cursor(22, 20);mono->Printf("%08X", Zone);
	mono->Set_Cursor(31, 20);mono->Printf("%08X", Target);

	mono->Fill_Attrib(53, 20, 12, 1, IsUnderStrength ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(53, 21, 12, 1, IsFullStrength ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(53, 22, 12, 1, IsHasBeen ? MonoClass::INVERSE : MonoClass::NORMAL);

	mono->Fill_Attrib(66, 20, 12, 1, IsMoving ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 21, 12, 1, IsForcedActive ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 22, 12, 1, IsReforming ? MonoClass::INVERSE : MonoClass::NORMAL);
}
#endif


/***********************************************************************************************
 * TeamClass::Init -- Initializes the team objects for scenario preparation.                   *
 *                                                                                             *
 *    This routine clears out the team object array in preparation for starting a new          *
 *    scenario.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamClass::Init(void)
{
	Teams.Free_All();
}


/***********************************************************************************************
 * TeamClass::operator new -- Allocates a team object.                                         *
 *                                                                                             *
 *    This routine will allocate a team object from the team object pool.                      *
 *                                                                                             *
 * INPUT:   size  -- The size of the requested allocation.                                     *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the freshly allocated team object. If an allocation      *
 *          could not be made, then NULL is returned.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void * TeamClass::operator new(size_t)
{
	void * ptr = Teams.Allocate();
	if (ptr != NULL) {
		((TeamClass *)ptr)->Set_Active();
	}
	return(ptr);
}


/***********************************************************************************************
 * TeamClass::operator delete -- Deallocates a team object.                                    *
 *                                                                                             *
 *    This routine will return a team object to the team object pool.                          *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the team object to deallocate.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamClass::operator delete(void * ptr)
{
	if (ptr != NULL) {
		((TeamClass *)ptr)->IsActive = false;
	}
	Teams.Free((TeamClass *)ptr);
}


/***********************************************************************************************
 * TeamClass::~TeamClass -- Team object destructor.                                            *
 *                                                                                             *
 *    This routine is called when a team object is destroyed. It handles updating the total    *
 *    number count for this team object type.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *   07/04/1996 JLB : Keeps trigger if trigger still attached to objects.                      *
 *=============================================================================================*/
TeamClass::~TeamClass(void)
{
	if (GameActive && Class.Is_Valid()) {
		while (Member != NULL) {
			Remove(Member);
		}
		Class->Number--;

		/*
		**	When the team dies, any trigger associated with it, dies as well. This will only occur
		**	if there are no other objects linked to this trigger. Only player reinforcement
		**	members that broke off of the team earlier will have this occur.
		*/
		if (Trigger.Is_Valid()) {
			if (Trigger->AttachCount == 0) {
				delete (TriggerClass *)Trigger;
			}
			Trigger = NULL;
		}
	}
}


/***********************************************************************************************
 * TeamClass::TeamClass -- Constructor for the team object type.                               *
 *                                                                                             *
 *    This routine is called when the team object is created.                                  *
 *                                                                                             *
 * INPUT:   type  -- Pointer to the team type to make this team object from.                   *
 *                                                                                             *
 *          owner -- The owner of this team.                                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TeamClass::TeamClass(TeamTypeClass const * type, HouseClass * owner) :
	AbstractClass(RTTI_TEAM, Teams.ID(this)),
	Class((TeamTypeClass *)type),
	House(owner),
	IsForcedActive(false),
	IsHasBeen(false),
	IsFullStrength(false),
	IsUnderStrength(true),
	IsReforming(false),
	IsLagging(false),
	IsAltered(true),
	JustAltered(false),
	IsMoving(false),
	IsNextMission(true),
	IsLeaveMap(false),
	Suspended(false),
	Trigger(NULL),
	Zone(TARGET_NONE),
	ClosestMember(TARGET_NONE),
	MissionTarget(TARGET_NONE),
	Target(TARGET_NONE),
	Total(0),
	Risk(0),
	Formation(FORMATION_NONE),
	SuspendTimer(0),
	CurrentMission(-1),
	TimeOut(0),
	Member(0)
{
	assert(Class);
	assert(Class->IsActive);
	assert(Class->ClassCount > 0);

	if (owner == NULL) {
		House = HouseClass::As_Pointer(Class->House);
	}

	memset(Quantity, 0, sizeof(Quantity));
	if (Class->Origin != -1) {
		Zone = ::As_Target(Scen.Waypoint[Class->Origin]);
	}
	Class->Number++;

	/*
	**	If there is a trigger tightly associated with this team, then
	**	create an instance of that trigger and attach it to the team.
	*/
	if (Class->Trigger.Is_Valid()) {
		Trigger = new TriggerClass(Class->Trigger);
	}
}


/***************************************************************************
 * TeamClass::Assign_Mission_Target -- Sets mission target and clears old  *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/16/1995 PWG : Created.                                             *
 *=========================================================================*/
void TeamClass::Assign_Mission_Target(TARGET new_target)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	/*
	** First go through and find anyone who is currently targeting
	** the old mission target and clear their Tarcom.
	*/
	FootClass * unit = Member;
	if (MissionTarget != TARGET_NONE) {
		while (unit != NULL) {
			bool tar = (unit->TarCom == MissionTarget);
			bool nav = (unit->NavCom == MissionTarget);
			if (tar || nav) {

				/*
				** If the unit was doing something related to the team mission
				** then we kick him into guard mode so that he is easy to change
				** missions for.
				*/
				unit->Assign_Mission(MISSION_GUARD);

				/*
				** If the unit's tarcom is set to the old mission target, then
				** clear it, so that it will be reset by whatever happens next.
				*/
				if (nav) {
					unit->Assign_Destination(TARGET_NONE);
				}

				/*
				** If the unit's navcom is set to the old mission target, then
				** clear it, so that it will be reset by whatever happens next.
				*/
				if (tar) {
					unit->Assign_Target(TARGET_NONE);
				}
			}
			unit = unit->Member;
		}
	}

	/*
	**	If there is not currently an override on the current mission target
	** then assign both MissionTarget and Target to the new target.  If
	** there is an override, allow the team to keep fighting the override but
	** make sure they pick up on the new mission when they are ready.
	*/
	if (Target == MissionTarget || !Target_Legal(Target)) {
		MissionTarget = Target = new_target;
	} else {
		MissionTarget = new_target;
	}
}


/***********************************************************************************************
 * TeamClass::AI -- Process team logic.                                                        *
 *                                                                                             *
 *    General purpose team logic is handled by this routine. It should be called once per      *
 *    active team per game tick. This routine handles recruitment and assigning orders to      *
 *    member units.                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/29/1994 JLB : Created.                                                                 *
 *   01/06/1995 JLB : Choreographed gesture.                                                   *
 *=============================================================================================*/
void TeamClass::AI(void)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	int	desired		= 0;
	int	old_under	= IsUnderStrength;
	int	old_full		= IsFullStrength;

	/*
	** If the team has been suspended then we need to check if it's time for
	** us to reactivate the team.  If not, no team logic will be processed
	** for this team.
	*/
	if (Suspended) {
		if (SuspendTimer != 0) {
			return;
		}
		Suspended = false;
	}

	/*
	**	If this team senses that its composition has been altered, then it should
	**	recalculate the under strength and full strength flags.
	*/
	if (IsAltered) {

		/*
		**	Figure out the total number of objects that this team type requires.
		*/
		for (int index = 0; index < Class->ClassCount; index++) {
			desired += Class->Members[index].Quantity;
		}
		assert(desired != 0);

		if (Total) {
			IsFullStrength = (Total == desired);
			if (IsFullStrength) {
				IsHasBeen = true;
			}

			/*
			**	Reinforceable teams will revert (or snap out of) the under strength
			**	mode when the members transition the magic 1/3 strength threshold.
			*/
			if (Class->IsReinforcable) {
				if (desired > 2) {
					IsUnderStrength = (Total <= desired / 3);
				} else {
					IsUnderStrength = (Total < desired);
				}
			} else {

				/*
				**	Teams that are not flagged as reinforceable are never considered under
				**	strength if the team has already started its main mission. This
				**	ensures that once the team has started, it won't dally to pick up
				**	new members.
				*/
				IsUnderStrength = !IsHasBeen;
			}

			IsAltered = JustAltered = false;
		} else {
			IsUnderStrength = true;
			IsFullStrength = false;
			Zone = TARGET_NONE;

			/*
			**	A team that exists on the player's side is automatically destroyed
			**	when there are no team members left. This team was created as a
			**	result of reinforcement logic and no longer needs to exist when there
			**	are no more team members.
			*/
			if (IsHasBeen || Session.Type != GAME_NORMAL) {

				/*
				**	If this team had no members (i.e., the team object wasn't terminated by some
				**	outside means), then pass through the logic triggers to see if one that
				**	depends on this team leaving the map should be sprung.
				*/
				if (IsLeaveMap) {
					for (int index = 0; index < LogicTriggers.Count(); index++) {
						TriggerClass * trig = LogicTriggers[index];
						if (trig->Spring(TEVENT_LEAVES_MAP)) {
							index--;
							if (LogicTriggers.Count() == 0) break;
						}
					}
				}
				delete this;
				return;
			}
		}

		/*
		** If the team has gone from under strength to no longer under
		** strength than the team needs to reform.
		*/
		if (old_under != IsUnderStrength) {
			IsReforming = true;
		}
	}

	/*
	**	If the team is under strength, then flag it to regroup.
	*/
	if (IsMoving && IsUnderStrength) {
		IsMoving			= false;
		CurrentMission	= -1;
		if (Total) {
			Calc_Center(Zone, ClosestMember);

			/*
			** When a team is badly damaged and needs to regroup it should
			** pick a friendly building to go and regroup at.  Its first preference
			** should be somewhere near repair factory.  If it cannot find a repair
			** factory then it should pick another structure that is friendly to
			** its side.
			*/
			CELL 	dest	= As_Cell(Zone);
			int 	max	= 0x7FFFFFFF;

			for (int index = 0; index < Buildings.Count(); index++) {
				BuildingClass * b = Buildings.Ptr(index);

				if (b != NULL && !b->IsInLimbo && b->House == House && b->Class->PrimaryWeapon == NULL) {
					CELL cell = Coord_Cell(b->Center_Coord());
					int dist = ::Distance(b->Center_Coord(), As_Coord(Zone)) * (Map.Cell_Threat(cell, House->Class->House) + 1);

					if (*b == STRUCT_REPAIR) {
						dist /= 2;
					}
					if (dist < max) {
						cell = Fetch_A_Leader()->Safety_Point(As_Cell(Zone), cell, 2, 4);
//						cell = Member->Safety_Point(As_Cell(Zone), cell, 2, 4);
						if (cell != -1) {
							max = dist;
							dest = cell;
						}
					}
				}
			}

			// Should calculate a regroup location.
			Target = ::As_Target(dest);
			Coordinate_Move();
			return;
		} else {
			Zone = TARGET_NONE;
		}
	}

	/*
	**	Flag this team into action when it gets to full strength. Human owned teams are only
	**	used for reinforcement purposes -- always consider them at full strength.
	*/
	if (!IsMoving && (IsFullStrength || IsForcedActive)) {
		IsMoving = true;
		IsHasBeen = true;
		IsUnderStrength = false;

		/*
		**	Infantry can do a gesture when they start their mission. Pick
		**	a gesture at random.
		*/
		FootClass * techno = Member;
		DoType doaction = Percent_Chance(50) ? DO_GESTURE1 : DO_GESTURE2;
		while (techno) {
			if (_Is_It_Breathing(techno) && techno->What_Am_I() == RTTI_INFANTRY) {
				((InfantryClass *)techno)->Do_Action(doaction);
			}

			if (IsReforming || IsForcedActive) {
				techno->IsInitiated = true;
			}

			techno = techno->Member;
		}
		CurrentMission	= -1;
		IsNextMission	= true;
//		IsForcedActive = false;
	}

	/*
	**	If the team is moving or if there is no center position for
	**	the team, then the center position must be recalculated.
	*/
	if (IsReforming || IsMoving || Zone == TARGET_NONE || ClosestMember == TARGET_NONE) {
		Calc_Center(Zone, ClosestMember);
	}

	/*
	**	Try to recruit members if there is room to do so for this team.
	**	Only try to recruit members for a non player controlled team.
	*/
	if ((!IsMoving || (!IsFullStrength && Class->IsReinforcable)) && ((!House->IsHuman || !IsHasBeen) && Session.Type == GAME_NORMAL)) {
//	if ((!IsMoving || (!IsFullStrength && Class->IsReinforcable)) && ((/*!House->IsHuman ||*/ !IsHasBeen) && Session.Type == GAME_NORMAL)) {
		for (int index = 0; index < Class->ClassCount; index++) {
			if (Quantity[index] < Class->Members[index].Quantity) {
				Recruit(index);
			}
		}
	}

	/*
	**	If there are no members of the team and the team has reached
	**	full strength at one time, then delete the team.
	*/
	if (Member == NULL && IsHasBeen) {

		/*
		**	If this team had no members (i.e., the team object wasn't terminated by some
		**	outside means), then pass through the logic triggers to see if one that
		**	depends on this team leaving the map should be sprung.
		*/
		if (IsLeaveMap) {
			for (int index = 0; index < LogicTriggers.Count(); index++) {
				TriggerClass * trig = LogicTriggers[index];
				if (trig->Spring(TEVENT_LEAVES_MAP)) {
					index--;
					if (LogicTriggers.Count() == 0) break;
				}
			}
		}
		delete this;
		return;
	}

	/*
	**	If the mission should be advanced to the next entry, then do so at
	**	this time. Various events may cause the mission to advance, but it
	**	all boils down to the following change-mission code.
	*/
	if (IsMoving && !IsReforming && IsNextMission) {
		IsNextMission = false;
		CurrentMission++;
		if (CurrentMission < Class->MissionCount) {
			TeamMissionClass const * mission = &Class->MissionList[CurrentMission];

			TimeOut = mission->Data.Value * (TICKS_PER_MINUTE/10);
			Target = TARGET_NONE;
			switch (mission->Mission) {

				case TMISSION_MOVECELL:
					Assign_Mission_Target(::As_Target((CELL)(mission->Data.Value)));
					break;

				case TMISSION_MOVE:
					if ((unsigned)mission->Data.Value < WAYPT_COUNT && Member != NULL) {
						FootClass * leader = Fetch_A_Leader();
						CELL movecell = Scen.Waypoint[mission->Data.Value];
						if (!Is_Leaving_Map()) {
							if (leader->Can_Enter_Cell(movecell) != MOVE_OK) {
								movecell = Map.Nearby_Location(movecell, leader->Techno_Type_Class()->Speed);
							}
						}
						Assign_Mission_Target(::As_Target(movecell));
						Target = ::As_Target(movecell);
					}
					break;

				case TMISSION_ATT_WAYPT:
				case TMISSION_PATROL:
				case TMISSION_SPY:
					if ((unsigned)mission->Data.Value < WAYPT_COUNT) {
						Assign_Mission_Target(::As_Target(Scen.Waypoint[mission->Data.Value]));
					}
					break;

				case TMISSION_ATTACKTARCOM:
					Assign_Mission_Target(mission->Data.Value);
					break;

				case TMISSION_UNLOAD:
				default:
					Assign_Mission_Target(TARGET_NONE);
					break;
			}
		} else {
			delete this;
			return;
		}
	}

	/*
	**	Perform mission of the team. This depends on the mission list.
	*/
	if (Member != NULL && IsMoving && !IsReforming && !IsUnderStrength) {

		/*
		** If the current Target has been dealt with but the mission target
		** has not, then the current target needs to be reset to the mission
		** target.
		*/
		if (!Target_Legal(Target)) {
			Target = MissionTarget;
		}

		/*
		**	If the current mission is one that times out, then check for
		**	this case. If it has timed out then advance to the next
		**	mission in the list or disband the team.
		*/
		TeamMissionClass const * mission = &Class->MissionList[CurrentMission];
//		FootClass	* member = Member;

		switch (mission->Mission) {
			case TMISSION_PATROL:
				TMission_Patrol();
				break;

			case TMISSION_FORMATION:
				TMission_Formation();
				break;

			case TMISSION_ATTACKTARCOM:
			case TMISSION_ATTACK:
				TMission_Attack();
				break;

			case TMISSION_LOAD:
				TMission_Load();
				break;

			case TMISSION_DEPLOY:
				TMission_Deploy();
				break;

			case TMISSION_UNLOAD:
				TMission_Unload();
				break;

			case TMISSION_MOVE:
			case TMISSION_MOVECELL:
				Coordinate_Move();
				break;

			/*
			**	All members of this team become invulnerable as if by magic.
			*/
			case TMISSION_INVULNERABLE:
				TMission_Invulnerable();
				break;

			case TMISSION_GUARD:
				Coordinate_Regroup();
				break;

			case TMISSION_DO:
				Coordinate_Do();
				break;

			case TMISSION_SET_GLOBAL:
				TMission_Set_Global();
				break;

			case TMISSION_ATT_WAYPT:
				if (!Target_Legal(MissionTarget)) {
					Assign_Mission_Target(TARGET_NONE);
					IsNextMission = true;
				} else {
					Coordinate_Attack();
				}
				break;

			case TMISSION_SPY:
				TMission_Spy();
				break;

			case TMISSION_HOUND_DOG:
				TMission_Follow();
				break;

			case TMISSION_LOOP:
				TMission_Loop();
				break;
		}

		/*
		**	Check for mission time out condition. If the mission does in fact time out, then
		**	flag it so that the team mission list will advance.
		*/
		switch (mission->Mission) {
//			case TMISSION_UNLOAD:
			case TMISSION_GUARD:
				if (TimeOut == 0) {
					IsNextMission = true;
				}
				break;
		}

	} else {

		if (IsMoving) {
			IsReforming = !Coordinate_Regroup();
		} else {
			Coordinate_Move();
		}
	}
}


/***********************************************************************************************
 * TeamClass::Add -- Adds specified object to team.                                            *
 *                                                                                             *
 *    Use this routine to add the specified object to the team. The object is checked to make  *
 *    sure that it can be assigned to the team. If it can't, then the object will be left      *
 *    alone and false will be returned.                                                        *
 *                                                                                             *
 * INPUT:   obj      -- Pointer to the object that is to be assigned to this team.             *
 *                                                                                             *
 * OUTPUT:  bool; Was the unit added to the team?                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/29/1994 JLB : Created.                                                                 *
 *   01/02/1995 JLB : Initiation flag setup.                                                   *
 *   08/06/1995 JLB : Allows member stealing from lesser priority teams.                       *
 *=============================================================================================*/
bool TeamClass::Add(FootClass * obj)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);
	if (!obj) return(false);

	int typeindex;
	if (!Can_Add(obj, typeindex)) return(false);

	/*
	**	All is ok to add the object to the team, but if the object is already part of
	**	another team, then it must be removed from that team first.
	*/
	if (obj->Team.Is_Valid()) {
		obj->Team->Remove(obj);
	}

	/*
	**	Actually add the object to the team.
	*/
	Quantity[typeindex]++;
	obj->IsInitiated = (Member == NULL);
	obj->Member = Member;
	Member = obj;
	obj->Team = this;

	/*
	**	If a common trigger is designated for this team type, then attach the
	**	trigger to this team member.
	*/
	if (Trigger.Is_Valid()) {
		obj->Attach_Trigger(Trigger);
	}

	Total++;
	Risk += obj->Risk();
	if (Zone == TARGET_NONE) {
		Calc_Center(Zone, ClosestMember);
	}

	/*
	**	Return with success, since the object was added to the team.
	*/
	IsAltered = JustAltered = true;
	return(true);
}


/***********************************************************************************************
 * TeamClass::Can_Add -- Determines if the specified object can be added to team.              *
 *                                                                                             *
 *    This routine will examine the team and determine if the specified object can be          *
 *    properly added to this team. This is a security check to filter out those objects that   *
 *    should not be added because of conflicting priorities or other restrictions.             *
 *                                                                                             *
 * INPUT:   obj      -- Pointer to the candidate object that is being checked for legal        *
 *                      adding to this team.                                                   *
 *                                                                                             *
 *          typeindex-- The class index number (according to the team type's class array) that *
 *                      the candidate object is classified as. The routine processes much      *
 *                      faster if you can provide this information, but if you don't, the      *
 *                      routine will figure it out.                                            *
 *                                                                                             *
 * OUTPUT:  bool; Can the specified object be added to this team?                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/27/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TeamClass::Can_Add(FootClass * obj, int & typeindex) const
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	/*
	**	Trying to add the team member to itself is an error condition.
	*/
	if (obj->Team == this) {
		return(false);
	}

	/*
	**	The object must be active, a member of this house. A special dispensation is given to
	**	units that are in radio contact. It is presumed that they are very busy and should
	**	not be disturbed.
	*/
	if (!_Is_It_Breathing(obj) || obj->In_Radio_Contact() || obj->House != House) {
		return(false);
	}

	/*
	**	If the object is doing some mission that precludes it from joining
	**	a team then don't add it.
	*/
	if (obj->Mission != MISSION_NONE && !MissionClass::Is_Recruitable_Mission(obj->Mission)) {
		return(false);
	}

	/*
	**	If this object is part of another team, then check to make sure that it
	**	is permitted to leave the other team in order to join this one. If not,
	**	then no further processing is allowed -- bail.
	*/
	if (obj->Team.Is_Valid() && (obj->Team->Class->RecruitPriority >= Class->RecruitPriority)) {
		return(false);
	}

	/*
	**	Aircraft that have no ammo for their weapons cannot be recruited into a team.
	*/
	if (obj->What_Am_I() == RTTI_AIRCRAFT && obj->Techno_Type_Class()->PrimaryWeapon != NULL && !obj->Ammo) {
		return(false);
	}

	/*
	**	Search for the exact member index that the candidate object matches.
	**	If no match could be found, then adding the object to the team cannot
	**	occur.
	*/
	for (typeindex = 0; typeindex < Class->ClassCount; typeindex++) {
		if (Class->Members[typeindex].Class == &obj->Class_Of()) {
			break;
		}
	}
	if (typeindex == Class->ClassCount) {
		return(false);
	}

	/*
	**	If the team is already full of this type, then adding the object is not allowed.
	**	Return with a failure flag in this case.
	*/
	if (Quantity[typeindex] >= Class->Members[typeindex].Quantity) {
		return(false);
	}

	return(true);
}



/***********************************************************************************************
 * TeamClass::Remove -- Removes the specified object from the team.                            *
 *                                                                                             *
 *    Use this routine to remove an object from a team. Objects removed from the team are      *
 *    then available to be recruited by other teams, or even by the same team at a later time. *
 *                                                                                             *
 * INPUT:   obj      -- Pointer to the object that is to be removed from this team.            *
 *                                                                                             *
 *          typeindex-- Optional index of where this object type is specified in the type      *
 *                      type class. This parameter can be omitted. It only serves to make      *
 *                      the removal process faster.                                            *
 *                                                                                             *
 * OUTPUT:  bool; Was the object removed from this team?                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/29/1994 JLB : Created.                                                                 *
 *   01/02/1995 JLB : Initiation tracking and team captain selection.                          *
 *=============================================================================================*/
bool TeamClass::Remove(FootClass * obj, int typeindex)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	/*
	**	Make sure that the object is in fact a member of this team. If not, then it can't
	**	be removed. Return success because the end result is the same.
	*/
	if (this != obj->Team) {
		return(true);
	}

	/*
	**	Detach the common trigger for this team type. Only current and active members of the
	**	team have that trigger attached. The exception is for player team members that
	**	get removed from a reinforcement team.
	*/
	if (obj->Trigger == Trigger && !obj->House->IsPlayerControl) {
		obj->Attach_Trigger(NULL);
	}

	/*
	**	If the proper team index was not provided, then find it in the type type class. The
	**	team type class will not be set if the appropriate type could not be found
	**	for this object. This indicates that the object was illegally added. Continue to
	**	process however, since removing this object from the team is a good idea.
	*/
	if (typeindex == -1) {
		for (typeindex = 0; typeindex < Class->ClassCount; typeindex++) {
			if (Class->Members[typeindex].Class == &obj->Class_Of()) {
				break;
			}
		}
	}

	/*
	**	Decrement the counter for the team class. There is now one less of this object type.
	*/
	if (typeindex < Class->ClassCount) {
		Quantity[typeindex]--;
	}

	/*
	**	Actually remove the object from the team. Scan through the team members
	**	looking for the one that matches the one specified. If it is found, it
	**	is unlinked from the member chain. During this scan, a check is made to
	**	ensure that at least one remaining member is still initiated. If not, then
	**	a new team captain must be chosen.
	*/
	bool initiated = false;
	FootClass * prev = 0;
	FootClass * curr = Member;
	bool found = false;
	while (curr != NULL && (!found || !initiated)) {
		if (curr == obj) {
			if (prev != NULL) {
				prev->Member = curr->Member;
			} else {
				Member = curr->Member;
			}
			FootClass * temp = curr->Member;
			curr->Member = 0;
			curr->Team = 0;
			curr->SuspendedMission = MISSION_NONE;
			curr->SuspendedNavCom = TARGET_NONE;
			curr->SuspendedTarCom = TARGET_NONE;
			curr = temp;
			Total--;
			found = true;
			Risk -= obj->Risk();
			continue;
		}

		/*
		**	If this (remaining) member is initiated, then keep a record of this.
		*/
		initiated |= curr->IsInitiated;

		prev = curr;
		curr = curr->Member;
	}

	/*
	**	A unit that breaks off of a team will enter idle mode.
	*/
	obj->Enter_Idle_Mode();

	/*
	**	If, after removing the team member, there are no initiated members left
	**	in the team, then just make the first remaining member of the team the
	**	team captain. Mark the center location of the team as invalid so that
	**	it will be centered around the captain.
	*/
	if (!initiated && Member != NULL) {
		Member->IsInitiated = true;
		Zone = TARGET_NONE;
	}

	/*
	**	Must record that the team composition has changed. At the next opportunity,
	**	the team members will be counted and appropriate AI adjustments made.
	*/
	IsAltered = JustAltered = true;
	return(true);
}


/***********************************************************************************************
 * TeamClass::Recruit -- Attempts to recruit members to the team for the given index ID.       *
 *                                                                                             *
 *    This routine will take the given index ID and scan for available objects of that type    *
 *    to recruit to the team. Recruiting will continue until that object type has either       *
 *    been exhausted or if the team's requirement for that type has been filled.               *
 *                                                                                             *
 * INPUT:   typeindex   -- The index for the object type to recruit. The index is used to      *
 *                         look into the type type's array of object types that make up this   *
 *                         team.                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of objects added to this team.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/29/1994 JLB : Created.                                                                 *
 *   04/10/1995 JLB : Scans for units too.                                                     *
 *=============================================================================================*/
int TeamClass::Recruit(int typeindex)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);
	COORDINATE center = As_Coord(Zone);

	if (Class->Origin != -1) {
		center = Cell_Coord(Scen.Waypoint[Class->Origin]);
	}

	int added = 0;				// Total number added to team.

	/*
	**	Quick check to see if recruiting is really allowed for this index or not.
	*/
	if (Class->Members[typeindex].Quantity > Quantity[typeindex]) {
		switch (Class->Members[typeindex].Class->What_Am_I()) {

			/*
			**	For infantry objects, sweep through the infantry in the game looking for
			**	ones owned by the house that owns the team. When found, try to add.
			*/
			case RTTI_INFANTRYTYPE:
			case RTTI_INFANTRY:
				{
					InfantryClass * best = 0;
					int bestdist = -1;

					for (int index = 0; index < Infantry.Count(); index++) {
						InfantryClass * infantry = Infantry.Ptr(index);
						int d = infantry->Distance(center);

						if ((d < bestdist || bestdist == -1) && Can_Add(infantry, typeindex)) {
							best = infantry;
							bestdist = d;
						}
					}

					if (best) {
						best->Assign_Target(TARGET_NONE);
						Add(best);
						added++;
					}
				}
				break;

			case RTTI_AIRCRAFTTYPE:
			case RTTI_AIRCRAFT:
				{
					AircraftClass * best = 0;
					int bestdist = -1;

					for (int index = 0; index < Aircraft.Count(); index++) {
						AircraftClass * aircraft = Aircraft.Ptr(index);
						int d = aircraft->Distance(center);

						if ((d < bestdist || bestdist == -1) && Can_Add(aircraft, typeindex)) {
							best = aircraft;
							bestdist = d;
						}
					}

					if (best) {
						best->Assign_Target(TARGET_NONE);
						Add(best);
						added++;
					}
				}
				break;

			case RTTI_UNITTYPE:
			case RTTI_UNIT:
				{
					UnitClass * best = 0;
					int bestdist = -1;

					for (int index = 0; index < Units.Count(); index++) {
						UnitClass * unit = Units.Ptr(index);
						int d = unit->Distance(center);

						if (unit->House == House && unit->Class == Class->Members[typeindex].Class) {

							if ((d < bestdist || bestdist == -1) && Can_Add(unit, typeindex)) {
								best = unit;
								bestdist = d;
							}

						}

						if (best) {
							best->Assign_Target(TARGET_NONE);
							Add(best);
							added++;

							/*
							**	If a transport is added to the team, the occupants
							**	are added by default.
							*/
							FootClass * f = best->Attached_Object();
							while (f) {
								Add(f);
								f = (FootClass *)(ObjectClass *)f->Next;
							}
						}
					}
				}
				break;

			case RTTI_VESSELTYPE:
			case RTTI_VESSEL:
				{
					VesselClass * best = 0;
					int bestdist = -1;

					for (int index = 0; index < Vessels.Count(); index++) {
						VesselClass * vessel = Vessels.Ptr(index);
						int d = vessel->Distance(center);

						if (vessel->House == House && vessel->Class == Class->Members[typeindex].Class) {

							if ((d < bestdist || bestdist == -1) && Can_Add(vessel, typeindex)) {
								best = vessel;
								bestdist = d;
							}

						}

						if (best) {
							best->Assign_Target(TARGET_NONE);
							Add(best);
							added++;

							/*
							**	If a transport is added to the team, the occupants
							**	are added by default.
							*/
							FootClass * f = best->Attached_Object();
							while (f) {
								Add(f);
								f = (FootClass *)(ObjectClass *)f->Next;
							}
						}
					}
				}
				break;
		}
	}
	return(added);
}


/***********************************************************************************************
 * TeamClass::Detach -- Removes specified target from team tracking.                           *
 *                                                                                             *
 *    When a target object is about to be removed from the game (e.g., it was killed), then    *
 *    any team that is looking at that target must abort from that target.                     *
 *                                                                                             *
 * INPUT:   target   -- The target object that is going to be removed from the game.           *
 *                                                                                             *
 *          all      -- Is the target going away for good as opposed to just cloaking/hiding?  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamClass::Detach(TARGET target, bool )
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	/*
	**	If the target to detach matches the target of this team, then remove
	**	the target from this team's Tar/Nav com and let the chips fall
	**	where they may.
	*/
	if (Target == target) {
		Target = TARGET_NONE;
	}
	if (MissionTarget == target) {
		MissionTarget = TARGET_NONE;
	}
	if (Trigger.Is_Valid() && Trigger->As_Target() == target) {
		Trigger = 0;
	}
}


/***********************************************************************************************
 * TeamClass::Calc_Center -- Determines average location of team members.                      *
 *                                                                                             *
 *    Use this routine to calculate the "center" location of the team. This is the average     *
 *    position of all members of the team. Using this center value it is possible to tell      *
 *    if a team member is too far away and where to head to in order to group up.              *
 *                                                                                             *
 * INPUT:   center   -- Average center target location of the team. Only initiated members     *
 *                      will be considered.                                                    *
 *                                                                                             *
 *          close_member--Location (as target) of the unit that is closest to the team's       *
 *                      target.                                                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamClass::Calc_Center(TARGET & center, TARGET & close_member) const
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	/*
	**	Presume there is no center. This will be confirmed in the following scanning
	**	operation.
	*/
	close_member = TARGET_NONE;
	center = TARGET_NONE;

	FootClass const * team_member = Member;		// Working team member pointer.

	/*
	**	If there are no members of the team, then there can be no center point of the team.
	*/
	if (team_member == NULL) return;

	/*
	**	If the team is supposed to follow a nearby friendly unit, then the
	**	team's "center" will actually be that unit. Otherwise, calculated the
	**	average center location for the team.
	*/
	if (Class->MissionList[CurrentMission].Mission == TMISSION_HOUND_DOG) {

		/*
		**	First pick a member of the team. The closest friendly object to that member
		**	will be picked.
		*/
		if (!team_member) return;

		FootClass const * closest = NULL;	// Current closest friendly object.
		int distance = -1;					// Record of last closest distance calc.

		/*
		**	Scan through all vehicles.
		*/
		for (int unit_index = 0; unit_index < Units.Count(); unit_index++) {
			FootClass const * trial_unit = Units.Ptr(unit_index);

			if (_Is_It_Breathing(trial_unit) && trial_unit->House->Is_Ally(House) && trial_unit->Team != this) {
				int trial_distance = team_member->Distance(trial_unit);

				if (distance == -1 || trial_distance < distance) {
					distance = trial_distance;
					closest = trial_unit;
				}
			}
		}

		/*
		**	Scan through all infantry.
		*/
		for (int infantry_index = 0; infantry_index < Infantry.Count(); infantry_index++) {
			FootClass const * trial_infantry = Infantry.Ptr(infantry_index);

			if (_Is_It_Breathing(trial_infantry) && trial_infantry->House->Is_Ally(House) && trial_infantry->Team != this) {
				int trial_distance = team_member->Distance(trial_infantry);

				if (distance == -1 || trial_distance < distance) {
					distance = trial_distance;
					closest = trial_infantry;
				}
			}
		}

		/*
		**	Scan through all vessels.
		*/
		for (int vessel_index = 0; vessel_index < Vessels.Count(); vessel_index++) {
			FootClass const * trial_vessel = Vessels.Ptr(vessel_index);

			if (_Is_It_Breathing(trial_vessel) && trial_vessel->House->Is_Ally(House) && trial_vessel->Team != this) {
				int trial_distance = team_member->Distance(trial_vessel);

				if (distance == -1 || trial_distance < distance) {
					distance = trial_distance;
					closest = trial_vessel;
				}
			}
		}

		/*
		**	Set the center location as actually the friendly object that is closest. If there
		**	is no friendly object, then don't set any center location at all.
		*/
		if (closest) {
			center = closest->As_Target();
			close_member = Member->As_Target();
		}

	} else {

		long	x = 0;								// Accumulated X coordinate.
		long	y = 0;								// Accumulated Y coordinate.
		int   dist = 0;							// Closest recorded distance to team target.
		int	quantity = 0;						// Number of team members counted.
		FootClass const * closest = 0;		// Closest member to target.

		/*
		**	Scan through all team members and accumulate the X and Y component of their
		**	location. Only team members that are active will be considered. Also keep
		**	track of the team member that is closest to the team's target.
		*/
		while (team_member != NULL) {
			if (_Is_It_Playing(team_member)) {

				/*
				**	Accumulate X and Y components of qualified team members.
				*/
				x += Coord_X(team_member->Coord);
				y += Coord_Y(team_member->Coord);
				quantity++;

				/*
				**	Keep a record of the team member that is nearest to the team's
				**	target.
				*/
				int try_dist = team_member->Distance(Target);
				if (!dist || try_dist < dist) {
					dist = try_dist;
					closest = team_member;
				}
			}

			team_member = team_member->Member;
		}

		/*
		**	If there were any qualifying members, then the team's center point can be
		**	determined.
		*/
		if (quantity) {
			x /= quantity;
			y /= quantity;
			COORDINATE coord = XY_Coord((int)x, (int)y);
			center = ::As_Target(coord);


			/*
			**	If the center location is impassable, then just pick the location of
			** one of the team members.
			*/
			if (!closest->Can_Enter_Cell(As_Cell(center))) {
//				if (Class->Origin != -1) {
//					center = ::As_Target(Scen.Waypoint[Class->Origin]);
//				} else {
					center = ::As_Target(Coord_Cell(closest->Center_Coord()));
//				}
			}
		}

		/*
		**	Record the position of the closest member to the team's target and
		**	that will be used as the regroup point.
		*/
		if (closest != NULL) {
			close_member = ::As_Target(Coord_Cell(closest->Center_Coord()));
		}
	}
}


/***********************************************************************************************
 * TeamClass::Took_Damage -- Informs the team when the team member takes damage.               *
 *                                                                                             *
 *    This routine is used when a team member takes damage. Usually the team will react in     *
 *    some fashion to the attack. This reaction can range from running away to assigning this  *
 *    new target as the team's target.                                                         *
 *                                                                                             *
 * INPUT:   obj      -- The team member that was damaged.                                      *
 *                                                                                             *
 *          result   -- The severity of the damage taken.                                      *
 *                                                                                             *
 *          source   -- The perpetrator of the damage.                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamClass::Took_Damage(FootClass * , ResultType result, TechnoClass * source)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	if ((result != RESULT_NONE) && (!Class->IsSuicide)) {
		if (!IsMoving) {

			// TCTCTC
			// Should run to a better hiding place or disband into a group of hunting units.

		} else {
			/*
			** Respond to the attack, but not if we're an aircraft or a LST.
			*/
			if (source && !Is_A_Member(source) && Member && Member->What_Am_I() != RTTI_AIRCRAFT && (Member->What_Am_I() != RTTI_VESSEL || *(VesselClass *)((FootClass *)Member) != VESSEL_TRANSPORT)) {
				if (Target != source->As_Target()) {

					/*
					**	Don't change target if the team's target is one that can fire as well. There is
					**	no point in endlessly shuffling between targets that have firepower.
					*/
					if (Target_Legal(Target)) {
						TechnoClass * techno = As_Techno(Target);

						if (techno && ((TechnoTypeClass const &)techno->Class_Of()).PrimaryWeapon != NULL) {
							if (techno->In_Range(As_Coord(Zone), 0)) {
								return;
							}
						}
					}

					/*
					**	Don't change target to aggressor if the aggressor cannot normally be attacked.
					*/
					if (source->What_Am_I() == RTTI_AIRCRAFT || (source->What_Am_I() == RTTI_VESSEL && (Member->What_Am_I() == RTTI_UNIT || Member->What_Am_I() == RTTI_INFANTRY))) {
						return;
					}

					Target = source->As_Target();
				}
			}
		}
	}
}


/***********************************************************************************************
 * TeamClass::Coordinate_Attack -- Handles coordinating a team attack.                         *
 *                                                                                             *
 *    This function is called when the team knows what it should attack. This routine will     *
 *    give the necessary orders to the members of the team.                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/06/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamClass::Coordinate_Attack(void)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	if (!Target_Legal(Target)) {
		Target = MissionTarget;
	}

	/*
	** Check if they're attacking a cell.  If the contents of the cell are
	** a bridge or a building/unit/techno, then it's a valid target.  Otherwise,
	** the target is invalid. This only applies to non-aircraft teams. An aircraft team
	**	can "attack" an empty cell and this is perfectly ok (paratrooper drop and parabombs
	**	are prime examples).
	*/
	if (Is_Target_Cell(Target) && Member != NULL && Fetch_A_Leader()->What_Am_I() != RTTI_AIRCRAFT) {
		CellClass *cellptr = &Map[As_Cell(Target)];
		TemplateType tt = cellptr->TType;
		if (cellptr->Cell_Object()) {
			Target = cellptr->Cell_Object()->As_Target();
		} else {
			if (tt != TEMPLATE_BRIDGE1   && tt != TEMPLATE_BRIDGE2   &&
				 tt != TEMPLATE_BRIDGE1H  && tt != TEMPLATE_BRIDGE2H  &&
				 tt != TEMPLATE_BRIDGE_1A && tt != TEMPLATE_BRIDGE_1B &&
				 tt != TEMPLATE_BRIDGE_2A && tt != TEMPLATE_BRIDGE_2B &&
				 tt != TEMPLATE_BRIDGE_3A && tt != TEMPLATE_BRIDGE_3B ) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	FootClass *unit = Member;
	TeamMissionClass const * mission = &Class->MissionList[CurrentMission];
	if(unit->What_Am_I() != RTTI_UNIT || 
		*(UnitClass *)unit != UNIT_CHRONOTANK || 
		mission->Mission != TMISSION_SPY)
#endif
				Target = 0;		// invalidize the target so it'll go to next mission.
			}
		}
	}

	if (!Target_Legal(Target)) {
		IsNextMission = true;

	} else {

		TeamMissionClass const * mission = &Class->MissionList[CurrentMission];

		FootClass * unit = Member;
		while (unit != NULL) {

			Coordinate_Conscript(unit);

			if (_Is_It_Playing(unit)) {
				if (mission->Mission == TMISSION_SPY && unit->What_Am_I() == RTTI_INFANTRY && *(InfantryClass *)unit == INFANTRY_SPY) {
					unit->Assign_Mission(MISSION_CAPTURE);
					unit->Assign_Target(Target);
				} else {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				if (mission->Mission == TMISSION_SPY && unit->What_Am_I() == RTTI_UNIT && *(UnitClass *)unit == UNIT_CHRONOTANK) {
					UnitClass *tank = (UnitClass *)unit;
					tank->Teleport_To(::As_Cell(Target));
					tank->MoebiusCountDown = ChronoTankDuration * TICKS_PER_MINUTE;
					Scen.Do_BW_Fade();
					Sound_Effect(VOC_CHRONOTANK1, unit->Coord);
					tank->Assign_Target(TARGET_NONE);
					tank->Assign_Mission(MISSION_GUARD);
				} else {
#endif
					if (unit->Mission != MISSION_ATTACK && unit->Mission != MISSION_ENTER && unit->Mission != MISSION_CAPTURE) {
						unit->Transmit_Message(RADIO_OVER_OUT);
						unit->Assign_Mission(MISSION_ATTACK);
						unit->Assign_Target(TARGET_NONE);
						unit->Assign_Destination(TARGET_NONE);
					}
				}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				}
#endif
				if (unit->TarCom != Target) {
					unit->Assign_Target(Target);
				}
			}

			unit = unit->Member;
		}
	}
}


/***********************************************************************************************
 * TeamClass::Coordinate_Regroup -- Handles team idling (regrouping).                          *
 *                                                                                             *
 *    This routine is called when the team must delay at its current location. Team members    *
 *    are grouped together by this function. It is called when the team needs to sit and       *
 *    wait.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Has the team completely regrouped?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/06/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TeamClass::Coordinate_Regroup(void)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	FootClass * unit   = Member;
	bool retval = true;

	/*
	**	Regroup default logic.
	*/
	while (unit != NULL) {

		Coordinate_Conscript(unit);

		if (_Is_It_Playing(unit)) {

			if (unit->Distance(Zone) > Rule.StrayDistance && (unit->Mission != MISSION_GUARD_AREA || !Target_Legal(unit->TarCom))) {
				if (!Target_Legal(unit->NavCom)) {
// TCTCTC
//				if (!Target_Legal(unit->NavCom) || ::Distance(unit->NavCom, Zone) > Rule.StrayDistance) {
					unit->Assign_Mission(MISSION_MOVE);
					unit->Assign_Destination(Zone);

					retval = false;
					if (!unit->IsFormationMove) {
						unit->Assign_Mission(MISSION_MOVE);
						CELL dest = unit->Adjust_Dest(As_Cell(Zone));
						unit->Assign_Destination(::As_Target(dest));
					} else {
						retval = true;	// formations are always considered regrouped.
					}
				}
			} else {

				/*
				**	The team is regrouping, so just sit here and wait.
				*/
				if (unit->Mission != MISSION_GUARD_AREA) {
					unit->Assign_Mission(MISSION_GUARD);
					unit->Assign_Destination(TARGET_NONE);
				}
			}

		}

		unit = unit->Member;
	}
	return(retval);
}


/***********************************************************************************************
 * TeamClass::Coordinate_Do -- Handles the team performing specified mission.                  *
 *                                                                                             *
 *    This will assign the specified mission to the team. If there are team members that are   *
 *    too far away from the center of the team, then they will be told to move to the team's   *
 *    location.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This only works if the special mission the team members are to perform does not *
 *             require extra parameters. The ATTACK and MOVE missions are particularly bad.    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamClass::Coordinate_Do(void)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	FootClass * unit = Member;
	MissionType do_mission = Class->MissionList[CurrentMission].Data.Mission;

	/*
	**	For each unit either head it back to the team center or give it the main
	**	team mission order as appropriate.
	*/
	while (unit != NULL) {

		Coordinate_Conscript(unit);

		if (_Is_It_Playing(unit)) {

			if (!Target_Legal(unit->TarCom) && !Target_Legal(unit->NavCom) && unit->Distance(Zone) > Rule.StrayDistance * 2) {

				/*
				**	Only if the unit isn't already heading to regroup with the team, will it
				**	be given orders to do so.
				*/
				unit->Assign_Mission(MISSION_MOVE);
				unit->Assign_Destination(Zone);
				unit->Assign_Mission(MISSION_MOVE);
				CELL dest = unit->Adjust_Dest(As_Cell(Zone));
				unit->Assign_Destination(::As_Target(dest));

			} else {

				/*
				**	The team is regrouping, so just sit here and wait.
				*/
				if (!Target_Legal(unit->TarCom) && !Target_Legal(unit->NavCom) && unit->Mission != do_mission) {
					unit->ArchiveTarget = TARGET_NONE;
					unit->Assign_Mission(do_mission);
					unit->Assign_Target(TARGET_NONE);
					unit->Assign_Destination(TARGET_NONE);
				}
			}

		}

		unit = unit->Member;
	}
}


/***********************************************************************************************
 * TeamClass::Coordinate_Move -- Handles team movement coordination.                           *
 *                                                                                             *
 *    This routine is called when the team must move to a new location. Movement and grouping  *
 *    commands associated with this task are initiated here.                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/06/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamClass::Coordinate_Move(void)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	FootClass * unit = Member;
	bool finished = true;
	bool found = false;

	if (!Target_Legal(Target)) {
		Target = MissionTarget;
	}

	if (Target_Legal(Target)) {

		if (!Lagging_Units()) {

			while (unit != NULL) {

				/*
				**	Tell the unit, if necessary, that it should regroup
				**	with the main team location. If the unit is regrouping, then
				**	the team should continue NOT qualify as fully reaching the desired
				**	location.
				*/
				if (Coordinate_Conscript(unit)) {
					finished = false;
				}

				if (unit->Mission == MISSION_UNLOAD || unit->MissionQueue == MISSION_UNLOAD) {
					finished = false;
				}

				if (_Is_It_Playing(unit) && unit->Mission != MISSION_UNLOAD && unit->MissionQueue != MISSION_UNLOAD) {
					int stray = Rule.StrayDistance;
					if (unit->What_Am_I() == RTTI_AIRCRAFT) {
						stray *= 3;
					}
					if (unit->What_Am_I() == RTTI_INFANTRY && ((InfantryClass const *)unit)->Class->IsDog) {
						if (Target_Legal(unit->TarCom)) stray = unit->Techno_Type_Class()->ThreatRange;
						if (Target_Legal(unit->TarCom) && unit->Distance(unit->TarCom) > stray) {
							unit->Assign_Target(TARGET_NONE);
						}
					}
					found = true;

					int dist = unit->Distance(Target);
					if (unit->IsFormationMove) {
						if (::As_Target(Coord_Cell(unit->Coord)) != unit->NavCom) {
							dist = Rule.StrayDistance + 1;	// formation moves must be exact.
						}
					}

					if (dist > stray ||
						(unit->What_Am_I() == RTTI_AIRCRAFT &&
//						(unit->In_Which_Layer() == LAYER_TOP &&
						((AircraftClass *)unit)->Height > 0 &&
						Coord_Cell(unit->Center_Coord()) != As_Cell(Target) &&
						!((AircraftClass *)unit)->Class->IsFixedWing &&
						Class->MissionList[CurrentMission+1].Mission != TMISSION_MOVE)) {

						bool wasform = false;


						if (unit->Mission != MISSION_MOVE) {
							unit->Assign_Mission(MISSION_MOVE);
						}

						if (unit->NavCom != Target) {

							/*
							** Check if this destination should be adjusted for
							** a formation move
							*/
							if (Is_Target_Cell(Target) && unit->IsFormationMove) {
								CELL newcell = unit->Adjust_Dest(As_Cell(Target));
								if (Coord_Cell(unit->Coord) != newcell) {
									unit->Assign_Destination(::As_Target(newcell));
								} else {
									unit->Assign_Mission(MISSION_GUARD);
									unit->Assign_Destination(TARGET_NONE);
									wasform = true;
								}
							} else {
								unit->Assign_Destination(Target);
							}
						}

						if (!wasform) {
							finished = false;
						}

					} else {
						if (unit->Mission == MISSION_MOVE && (!Target_Legal(unit->NavCom) || Distance(unit->NavCom) < CELL_LEPTON_W)) {
							unit->Assign_Destination(TARGET_NONE);
							unit->Enter_Idle_Mode();
						}
					}

					/*
					**	If any member still has a valid NavCom then consider this
					**	movement mission to still be in progress. This will ensure
					**	that the members come to a complete stop before the next
					**	mission commences. Without this, the team will prematurely
					**	start on the next mission even when all members aren't yet
					**	in their proper spot.
					*/
					if (Target_Legal(unit->NavCom)) {
						finished = false;
					}
				}

				unit = unit->Member;
			}
		} else {
			finished = false;
		}
	}

	/*
	**	If there are no initiated members to this team, then it certainly
	**	could not have managed to move to the target destination.
	*/
	if (!found) {
		finished = false;
	}

	/*
	**	If all the team members are close enough to the desired destination, then
	**	move to the next mission.
	*/
	if (finished && IsMoving) {
		IsNextMission = true;
	}
}


/***********************************************************************************************
 * TeamClass::Lagging_Units -- Finds and orders any lagging units to catch up.                 *
 *                                                                                             *
 *    This routine will examine the team and find any lagging units. The units are then        *
 *    ordered to catch up to the team member that is closest to the team's destination. This   *
 *    routine will not do anything unless lagging members are suspected. This fact is          *
 *    indicated by setting the IsLagging flag. The flag is set by some outside agent.          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure to set IsLagging for the team if a lagging member is suspected.         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/01/1995 PWG : Created.                                                                 *
 *   04/11/1996 JLB : Modified.                                                                *
 *=============================================================================================*/
bool TeamClass::Lagging_Units(void)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	FootClass * unit = Member;
	bool lag = false;

	//BG: HACK - if it's in a formation move, then disable the check for
	// VG added NULL check 	 laggers, 'cause they're all moving simultaneously.
	if (unit != NULL && unit->IsFormationMove) IsLagging = false;

	/*
	** If the IsLagging bit is not set, then obviously there are no lagging
	** units.
	*/
	if (!IsLagging) return(false);

	/*
	**	Scan through all of the units, searching for units who are having
	** trouble keeping up with the pack.
	*/
	while (unit != NULL) {

		if (_Is_It_Playing(unit)) {
			int stray = Rule.StrayDistance;
			if (unit->What_Am_I() ==  RTTI_AIRCRAFT) {
				stray *= 3;
			}

			/*
			** If we find a unit who has fallen too far away from the center of
			** the pack, then we need to order that unit to catch up with the
			** first unit.
			*/
			if (unit->Distance(ClosestMember) > stray) {
// TCTCTC
				if (!Target_Legal(unit->NavCom)) {
//				if (!Target_Legal(unit->NavCom) || ::Distance(unit->NavCom, ClosestMember) > Rule.StrayDistance) {
					unit->Assign_Mission(MISSION_MOVE);
					unit->Assign_Destination(ClosestMember);
				}
				lag = true;

			} else {
				/*
				** We need to order all of the other units to hold their
				** position until all lagging units catch up.
				*/
				if (unit->Mission != MISSION_GUARD) {
					unit->Assign_Mission(MISSION_GUARD);
					unit->Assign_Destination(TARGET_NONE);
				}
			}
		}
		unit = unit->Member;
	}

	/*
	** Once we have handled the loop we know whether there are any lagging
	** units or not.
	*/
	IsLagging = lag;
	return(lag);
}


/***********************************************************************************************
 * TeamClass::TMission_Unload -- Tells the team to unload passengers now.                      *
 *                                                                                             *
 *    This routine tells all transport vehicles to unload passengers now.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/14/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TeamClass::TMission_Unload(void)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	FootClass * unit = Member;
	bool finished = true;

	while (unit != NULL) {

		Coordinate_Conscript(unit);

		if (_Is_It_Playing(unit)) {
			/*
			** Only assign the mission if the unit is carrying a passenger, OR
			** if the unit is a minelayer, with mines in it, and the cell it's
			** on doesn't have a building (read: mine) in it already.
			*/
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			/* Also, allow unload if it's a MAD Tank. */
			if (unit->Is_Something_Attached() || (unit->What_Am_I() == RTTI_UNIT && *(UnitClass *)unit == UNIT_MINELAYER && unit->Ammo) || (unit->What_Am_I() == RTTI_UNIT && *(UnitClass *)unit == UNIT_MAD )) {
#else
			if (unit->Is_Something_Attached() || (unit->What_Am_I() == RTTI_UNIT && *(UnitClass *)unit == UNIT_MINELAYER && unit->Ammo) ) {
#endif
				if (unit->Is_Something_Attached()) {
					/*
					** Passenger-carrying vehicles will always return false until
					** they've unloaded all passengers.
					*/
					finished = false;
				}

				/*
				**	The check for a building is located here because the mine layer may have
				**	already unloaded the mine but is still in the process of retracting
				**	the mine layer. During this time, it should not be considered to have
				**	finished its unload mission.
				*/
				if (Map[unit->Center_Coord()].Cell_Building() == NULL && unit->Mission != MISSION_UNLOAD) {
					unit->Assign_Destination(TARGET_NONE);
					unit->Assign_Target(TARGET_NONE);
					unit->Assign_Mission(MISSION_UNLOAD);
					finished = false;
				}

			} else {

				/*
				**	A loaner transport should vacate the map when all transported objects
				**	have been offloaded.
				*/
				if (unit->IsALoaner) {
					Remove(unit);
					unit->Assign_Mission(MISSION_RETREAT);
					unit->Commence();
				}
			}
		}

		unit = unit->Member;
	}

	if (finished) {
		IsNextMission = true;
	}
	return(1);
}


/***********************************************************************************************
 * TeamClass::TMission_Load -- Tells the team to load onto the transport now.                  *
 *                                                                                             *
 *    This routine tells all non-transport units in the team to climb onto the transport in the*
 *    team.  Note the transport must be a member of this team.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/28/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
int TeamClass::TMission_Load(void)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	FootClass * unit = Member;
	FootClass * trans = 0;

	/*
	** First locate the transport in the team, if there is one.  There should
	** only be one transport in the team.
	*/
	while(unit != NULL && trans == NULL) {
		if (unit->Techno_Type_Class()->Max_Passengers() > 0) {
			trans = unit;
			break;
		}
		unit = unit->Member;
	}

	/*
	**	In the case of no transport available, then consider the mission complete
	**	since it can never complete otherwise.
	*/
	if (trans == NULL) {
		IsNextMission = true;
		return(1);
	}

	/*
	**	If the transport is already in radio contact, then this means that
	**	it is in the process of loading. During this time, don't bother to assign
	**	the enter mission to the other team members.
	*/
	if (trans->In_Radio_Contact()) {
		return(1);
	}

	/*
	**	Find a member to assign the entry logic for.
	*/
	bool finished = true;
	unit = Member;	// re-point at the first member of the team again.
	while (unit != NULL && Total > 1) {
		Coordinate_Conscript(unit);

		/*
		** Only assign the mission if the unit is not the transport.
		*/
		if (_Is_It_Playing(unit) && unit != trans) {
			if (unit->Mission != MISSION_ENTER) {
				unit->Assign_Mission(MISSION_ENTER);
				unit->Assign_Target(TARGET_NONE);
				unit->Assign_Destination(trans->As_Target());
				finished = false;
				break;
			}
			finished = false;
		}

		unit = unit->Member;
	}

	if (finished) {
		IsNextMission = true;
	}
	return(1);
}


/***********************************************************************************************
 * TeamClass::Coordinate_Conscript -- Gives orders to new recruit.                             *
 *                                                                                             *
 *    This routine will give the movement orders to the conscript so that it will group        *
 *    with the other members of the team.                                                      *
 *                                                                                             *
 * INPUT:   unit  -- Pointer to the conscript unit.                                            *
 *                                                                                             *
 * OUTPUT:  bool; Is the unit still scurrying to reach the team's current location?            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/06/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TeamClass::Coordinate_Conscript(FootClass * unit)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	if (_Is_It_Breathing(unit) && !unit->IsInitiated) {
		if (unit->Distance(Zone) > Rule.StrayDistance) {
			if (!Target_Legal(unit->NavCom)) {
				unit->Assign_Mission(MISSION_MOVE);
				unit->Assign_Target(TARGET_NONE);
				unit->IsFormationMove = false;
				unit->Assign_Destination(Zone);
			}
			return(true);

		} else {

			/*
			**	This unit has gotten close enough to the team center so that it is
			**	now considered initiated. An initiated unit is considered when calculating
			**	the center of the team.
			*/
			unit->IsInitiated = true;
		}
	}
	return(false);
}


/***************************************************************************
 * TeamClass::Is_A_Member -- Tests if a unit is a member of a team         *
 *                                                                         *
 * INPUT:		none                                                        *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/16/1995 PWG : Created.                                             *
 *=========================================================================*/
bool TeamClass::Is_A_Member(void const * who) const
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	FootClass * unit = Member;
	while (unit != NULL) {
		if (unit == who) {
			return(true);
		}
		unit = unit->Member;
	}
	return(false);
}


/***************************************************************************
 * TeamClass::Suspend_Teams -- Suspends activity for low priority teams    *
 *                                                                         *
 * INPUT:	int priority - determines what is considered low priority.     *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/19/1995 PWG : Created.                                             *
 *=========================================================================*/
void TeamClass::Suspend_Teams(int priority, HouseClass const * house)
{
	for (int index = 0; index < Teams.Count(); index++) {
		TeamClass * team = Teams.Ptr(index);

		/*
		**	If a team is below the "survival priority level", then it gets
		**	destroyed. The team members are then free to be reassigned.
		*/
		if (team != NULL && team->House == house && team->Class->RecruitPriority < priority) {
			FootClass * unit = team->Member;
			while (team->Member) {
				team->Remove(team->Member);
			}
			team->IsAltered = team->JustAltered = true;
			team->SuspendTimer = Rule.SuspendDelay * TICKS_PER_MINUTE;
			team->Suspended = true;
		}
	}
}


/***********************************************************************************************
 * TeamClass::Is_Leaving_Map -- Checks if team is in process of leaving the map                *
 *                                                                                             *
 *    This routine is used to see if the team is leaving the map. A team that is leaving the   *
 *    map gives implicit permission for its members to leave the map.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this team trying to leave the map?                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TeamClass::Is_Leaving_Map(void) const
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	if (IsMoving && CurrentMission >= 0) {
		TeamMissionClass const * mission = &Class->MissionList[CurrentMission];

		if (mission->Mission == TMISSION_MOVE && !Map.In_Radar(Scen.Waypoint[mission->Data.Value])) {
			return(true);
		}
	}
	return(false);
}


/***********************************************************************************************
 * TeamClass::Has_Entered_Map -- Determines if the entire team has entered the map.            *
 *                                                                                             *
 *    This will examine all team members and only if all of them have entered the map, will    *
 *    it return true. This routine is used to recognize the case of a team that has been       *
 *    generated off map and one that has already entered game play. This knowledge can lead    *
 *    to more intelligent behavior regarding team and member disposition.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Have all members of this team entered the map?                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TeamClass::Has_Entered_Map(void) const
{
	bool ok = true;
	FootClass * foot = Member;
	while (foot != NULL) {
		if (!foot->IsLocked) {
			ok = false;
			break;
		}
		foot = (FootClass *)(ObjectClass *)(foot->Next);
	}
	return(ok);
}


/***********************************************************************************************
 * TeamClass::Scan_Limit -- Force all members of the team to have limited scan range.          *
 *                                                                                             *
 *    This routine is used when one of the team members cannot get within range of the team's  *
 *    target. In such a case, the team must be assigned a new target and all members of that   *
 *    team must recognize that a restricted target scan is required. This is done by clearing  *
 *    out the team's target so that it will be forced to search for a new one. Also, since the *
 *    members are flagged for short scanning, whichever team member is picked to scan for a    *
 *    target will scan for one that is within range.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The team will reassign its target as a result of this routine.                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TeamClass::Scan_Limit(void)
{
	Assign_Mission_Target(TARGET_NONE);
	FootClass * foot = Member;
	while (foot != NULL) {
		foot->Assign_Target(TARGET_NONE);
		foot->IsScanLimited = true;
		foot = foot->Member;
	}
}


/***********************************************************************************************
 * TeamClass::TMission_Formation -- Process team formation change command.                     *
 *                                                                                             *
 *    This routine will change the team's formation to that specified in the team command      *
 *    parameter. It is presumed that the team will have further movement orders so that the    *
 *    formation can serve some purpose. Merely changing the formation doesn't alter the        *
 *    member's location. The team must be given a movement order before team member            *
 *    repositioning will occur.                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the time to delay before further team actions should occur.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TeamClass::TMission_Formation(void)
{
	FootClass * member = Member;
	TeamMissionClass const * mission = &Class->MissionList[CurrentMission];
	Formation = mission->Data.Formation;
	int group = ID + 10;
	int xdir = 0;
	int ydir = 0;
	bool evenodd = 1;

	/*
	**	Assign appropriate formation offsets for each of the members
	**	of this team.
	*/
	switch (Formation) {
		case FORMATION_NONE:
			while (member != NULL) {
				member->Group = 0xFF;
				member->XFormOffset = 0x80000000;
				member->YFormOffset = 0x80000000;
				member->IsFormationMove = false;
				member = member->Member;
			}
			break;
		case FORMATION_TIGHT:
			while (member != NULL) {
				member->Group = group;
				member->XFormOffset = 0;
				member->YFormOffset = 0;
				member->IsFormationMove = true;
				member = member->Member;
			}
			break;
		case FORMATION_LOOSE:
			break;
		case FORMATION_WEDGE_N:
			ydir = -(Total / 2);
			xdir = 0;
			while (member != NULL) {
				member->Group = group;
				member->XFormOffset = xdir;
				member->YFormOffset = ydir;
				member->IsFormationMove = true;
				xdir = -xdir;
				evenodd ^= 1;
				if (!evenodd) {
					xdir -= 2;
					ydir += 2;
				}
				member = member->Member;
			}
			break;
		case FORMATION_WEDGE_E:
			xdir = (Total / 2);
			ydir = 0;
			while (member != NULL) {
				member->Group = group;
				member->XFormOffset = xdir;
				member->YFormOffset = ydir;
				member->IsFormationMove = true;
				ydir = -ydir;
				evenodd ^= 1;
				if (!evenodd) {
					xdir -= 2;
					ydir -= 2;
				}
				member = member->Member;
			}
			break;
		case FORMATION_WEDGE_S:
			ydir = (Total / 2);
			xdir = 0;
			while (member != NULL) {
				member->Group = group;
				member->XFormOffset = xdir;
				member->YFormOffset = ydir;
				member->IsFormationMove = true;
				xdir = -xdir;
				evenodd ^= 1;
				if (!evenodd) {
					xdir -= 2;
					ydir -= 2;
				}
				member = member->Member;
			}
			break;
		case FORMATION_WEDGE_W:
			xdir = -(Total / 2);
			ydir = 0;
			while (member != NULL) {
				member->Group = group;
				member->XFormOffset = xdir;
				member->YFormOffset = ydir;
				member->IsFormationMove = true;
				ydir = -ydir;
				evenodd ^= 1;
				if (!evenodd) {
					xdir += 2;
					ydir -= 2;
				}
				member = member->Member;
			}
			break;
		case FORMATION_LINE_NS:
			ydir = -(Total/2);
			while (member != NULL) {
				member->Group = group;
				member->XFormOffset = 0;
				member->YFormOffset = ydir;
				member->IsFormationMove = true;
				member = member->Member;
				ydir += 2;
			}
			break;
		case FORMATION_LINE_EW:
			xdir = -(Total/2);
			while (member != NULL) {
				member->Group = group;
				member->XFormOffset = xdir;
				member->YFormOffset = 0;
				member->IsFormationMove = true;
				member = member->Member;
				xdir += 2;
			}
			break;
	}

	/*
	** Now calculate the group's movement type and speed
	*/
	if (Formation != FORMATION_NONE) {
		TeamSpeed[group] = SPEED_WHEEL;
		TeamMaxSpeed[group] = MPH_LIGHT_SPEED;
		member = Member;
		while (member != NULL) {
			RTTIType mytype = member->What_Am_I();
			SpeedType	memspeed;
			MPHType		memmax;
			bool			speedcheck = false;

			if (mytype == RTTI_INFANTRY) {
				memspeed = SPEED_FOOT;
				memmax = ((InfantryClass *)member)->Class->MaxSpeed;
				speedcheck = true;
			}
			if (mytype == RTTI_UNIT) {
				memspeed = ((UnitClass *)member)->Class->Speed;
				memmax = ((UnitClass *)member)->Class->MaxSpeed;
				speedcheck = true;
			}

			if (mytype == RTTI_VESSEL) {
				memspeed = ((VesselClass *)member)->Class->Speed;
				memmax = ((VesselClass *)member)->Class->MaxSpeed;
				speedcheck = true;
			}

			if (speedcheck) {
				if (memmax < TeamMaxSpeed[group]) {
					TeamMaxSpeed[group] = memmax;
					TeamSpeed[group] = memspeed;
				}
			}
			member = member->Member;
		}

		/*
		** Now that it's all calculated, assign the movement type and
		** speed to every member of the team.
		*/
		member = Member;
		while (member != NULL) {
			member->FormationSpeed = TeamSpeed[group];
			member->FormationMaxSpeed = TeamMaxSpeed[group];
			if (member->What_Am_I() == RTTI_INFANTRY) {
				member->FormationSpeed = SPEED_FOOT;
				member->FormationMaxSpeed = MPH_SLOW_ISH;
			}
			member = member->Member;
		}
	}

	// Advance past the formation-setting command.
	IsNextMission = true;

	return(1);
}


/***********************************************************************************************
 * TeamClass::TMission_Attack -- Perform the team attack mission command.                      *
 *                                                                                             *
 *    This will tell the team to attack the quarry specified in the team command. If the team  *
 *    already has a target, this it is presumed that this target take precidence and it won't  *
 *    be changed.                                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before the next team logic operation should occur.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TeamClass::TMission_Attack(void)
{
	if (!Target_Legal(MissionTarget) && Member != NULL) {
		TeamMissionClass const * mission = &Class->MissionList[CurrentMission];

		/*
		**	Pick a team leader that has a weapon. Only in the case of no
		**	team members having any weapons, will a member without a weapon
		**	be chosen.
		*/
		FootClass const * candidate = Fetch_A_Leader();

		/*
		**	Have the team leader pick what the next team target will be.
		*/
		switch (mission->Data.Quarry) {
			case QUARRY_ANYTHING:
				Assign_Mission_Target(candidate->Greatest_Threat(THREAT_NORMAL));
				break;

			case QUARRY_BUILDINGS:
				Assign_Mission_Target(candidate->Greatest_Threat(THREAT_BUILDINGS));
				break;

			case QUARRY_HARVESTERS:
				Assign_Mission_Target(candidate->Greatest_Threat(THREAT_TIBERIUM));
				break;

			case QUARRY_INFANTRY:
				Assign_Mission_Target(candidate->Greatest_Threat(THREAT_INFANTRY));
				break;

			case QUARRY_VEHICLES:
				Assign_Mission_Target(candidate->Greatest_Threat(THREAT_VEHICLES));
				break;

			case QUARRY_FACTORIES:
				Assign_Mission_Target(candidate->Greatest_Threat(THREAT_FACTORIES));
				break;

			case QUARRY_DEFENSE:
				Assign_Mission_Target(candidate->Greatest_Threat(THREAT_BASE_DEFENSE));
				break;

			case QUARRY_THREAT:
				Assign_Mission_Target(candidate->Greatest_Threat(THREAT_NORMAL));
				break;

			case QUARRY_POWER:
				Assign_Mission_Target(candidate->Greatest_Threat(THREAT_POWER));
				break;

			case QUARRY_FAKES:
				Assign_Mission_Target(candidate->Greatest_Threat(THREAT_FAKES));
				break;

			default:
				break;
		}
		if (!Target_Legal(MissionTarget)) IsNextMission = true;
	}
	Coordinate_Attack();
	return(1);
}


/***********************************************************************************************
 * TeamClass::TMission_Spy -- Perform the team spy mission.                                    *
 *                                                                                             *
 *    This will give the team a spy mission to the location specified. It is presumed that     *
 *    the location of the team mission actually resides under the building to be spied. If     *
 *    no building exists at the location, then the spy operation is presumed to be a mere      *
 *    move operation.                                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before the next team logic operation should occur.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TeamClass::TMission_Spy(void)
{
	if (Is_Target_Cell(MissionTarget))
		{
		CELL cell = ::As_Cell(MissionTarget);
		CellClass * cellptr = &Map[cell];
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		ObjectClass * bldg = cellptr->Cell_Building();
#else
		ObjectClass * bldg = cellptr->Cell_Object();
#endif
		if (bldg != NULL)
			{
			Assign_Mission_Target(bldg->As_Target());
			Coordinate_Attack();
			}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		else
			{
			FootClass *member = Member;
			if(member->What_Am_I() == RTTI_UNIT && *(UnitClass *)member == UNIT_CHRONOTANK)
				{
				bool finished = true;
				while (member)
					{
					if ( !((UnitClass *)member)->MoebiusCountDown) finished = false;
					member = member->Member;
					}
				
				if (!finished)
					{
					Coordinate_Attack();
					}
				else
					{
					Assign_Mission_Target(TARGET_NONE);
					IsNextMission = true;
					}
				}
			}
#endif
		}
	else
		{
		if (!Target_Legal(MissionTarget))
			{
			Assign_Mission_Target(TARGET_NONE);
			IsNextMission = true;
			}
		else
			{
			Coordinate_Attack();
			}
		}
	return(1);
}


/***********************************************************************************************
 * TeamClass::TMission_Follow -- Perform the "follow friendlies" team command.                 *
 *                                                                                             *
 *    This will cause the team members to search out and follow the nearest friendly object.   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before the next team logic operation should be performed.   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TeamClass::TMission_Follow(void)
{
	Calc_Center(Zone, ClosestMember);
	Target = Zone;
	Coordinate_Move();
	return(1);
}


/***********************************************************************************************
 * TeamClass::TMission_Loop -- Causes the team mission processor to jump to new location.      *
 *                                                                                             *
 *    This is equivalent to a jump or goto command. It will alter the team command processing  *
 *    such that it will continue processing at the command number specified.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before the next team logic operation should be performed.   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TeamClass::TMission_Loop(void)
{
	TeamMissionClass const * mission = &Class->MissionList[CurrentMission];
	CurrentMission = mission->Data.Value-1;
	IsNextMission = true;
	return(1);
}


/***********************************************************************************************
 * TeamClass::TMission_Invulnerable -- Makes the entire team invulnerable for a period of time *
 *                                                                                             *
 *    This is a team mission that simulates the Iron Curtain device. It will make all team     *
 *    members invlunerable for a temporary period of time.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the time delay before the next team logic operation should occur.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TeamClass::TMission_Invulnerable(void)
{
	FootClass * foot = Member;
	while (foot != NULL) {
		foot->IronCurtainCountDown = Rule.IronCurtainDuration * TICKS_PER_MINUTE;
		foot->Mark(MARK_CHANGE);
		foot = foot->Member;
	}
	IsNextMission = true;
	return(1);
}


/***********************************************************************************************
 * TeamClass::TMission_Set_Global -- Performs a set global flag operation.                     *
 *                                                                                             *
 *    This routine is used by the team to set a global variable but otherwise perform no       *
 *    visible effect on the team. By using this routine, sophisticated trigger dependencies    *
 *    can be implemented.                                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before the next team logic operation should occur.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TeamClass::TMission_Set_Global(void)
{
	TeamMissionClass const * mission = &Class->MissionList[CurrentMission];
	Scen.Set_Global_To(mission->Data.Value, true);
	IsNextMission = true;
	return(1);
}


/***********************************************************************************************
 * TeamClass::TMision_Patrol -- Handles patrolling from one location to another.               *
 *                                                                                             *
 *    A patrolling team will move to the designated waypoint, but along the way it will        *
 *    periodically scan for nearby enemies. If an enemy is found, the patrol mission turns     *
 *    into an attack mission until the target is destroyed -- after which it resumes its       *
 *    patrol duties.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before the next call to this routine is needed.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TeamClass::TMission_Patrol(void)
{
	/*
	**	Reassign the movement destination if the target has been prematurely
	**	cleared (probably because the object has been destroyed).
	*/
	if (!Target_Legal(Target)) {
		TeamMissionClass const * mission = &Class->MissionList[CurrentMission];
		if ((unsigned)mission->Data.Value < WAYPT_COUNT) {
			Assign_Mission_Target(::As_Target(Scen.Waypoint[mission->Data.Value]));
		}
	}

	/*
	**	Every so often, scan for a nearby enemy.
	*/
	if (Frame % (Rule.PatrolTime * TICKS_PER_MINUTE) == 0) {
		FootClass * leader = Fetch_A_Leader();
		if (leader != NULL) {
			TARGET target = leader->Greatest_Threat(THREAT_NORMAL|THREAT_RANGE);

			if (Target_Legal(target)) {
				Assign_Mission_Target(target);
			} else {
				Assign_Mission_Target(TARGET_NONE);
			}
		}
	}

	/*
	**	If the mission target looks like it should be attacked, then do so, otherwise
	**	treat it as a movement destination.
	*/
	if (Is_Target_Object(Target)) {
		Coordinate_Attack();
	} else {
		Coordinate_Move();
	}
	return(1);
}


int TeamClass::TMission_Deploy(void)
{
	assert(IsActive);
	assert(Teams.ID(this) == ID);

	FootClass * unit = Member;
	bool finished = true;

	while (unit != NULL) {

		Coordinate_Conscript(unit);

		if (_Is_It_Playing(unit)) {

			if (unit->What_Am_I() == RTTI_UNIT && *(UnitClass *)unit == UNIT_MCV) {
				if (unit->Mission != MISSION_UNLOAD) {
					unit->Assign_Destination(TARGET_NONE);
					unit->Assign_Target(TARGET_NONE);
					unit->Assign_Mission(MISSION_UNLOAD);
					finished = false;
				}
			}

			if (unit->What_Am_I() == RTTI_UNIT && *(UnitClass *)unit == UNIT_MINELAYER && unit->Ammo != 0) {
				/*
				**	The check for a building is located here because the mine layer may have
				**	already unloaded the mine but is still in the process of retracting
				**	the mine layer. During this time, it should not be considered to have
				**	finished its unload mission.
				*/
				if (!Map[unit->Center_Coord()].Cell_Building() && unit->Mission != MISSION_UNLOAD) {
					unit->Assign_Destination(TARGET_NONE);
					unit->Assign_Target(TARGET_NONE);
					unit->Assign_Mission(MISSION_UNLOAD);
					finished = false;
				}
			}

		}

		unit = unit->Member;
	}

	if (finished) {
		IsNextMission = true;
	}
	return(1);
}


/***********************************************************************************************
 * TeamClass::Fetch_A_Leader -- Looks for a suitable leader member of the team.                *
 *                                                                                             *
 *    This will scan through the team members looking for one that is suitable as a leader     *
 *    type. A team can sometimes contain limboed or unarmed members. These members are not     *
 *    suitable for leadership roles.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a suitable leader type unit.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/27/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
FootClass * TeamClass::Fetch_A_Leader(void) const
{
	FootClass * leader = Member;

	/*
	**	Scan through the team members trying to find one that is an active member and
	**	is equipped with a weapon.
	*/
	while (leader != NULL) {
		if (_Is_It_Playing(leader) && leader->Is_Weapon_Equipped()) break;
		leader = leader->Member;
	}

	/*
	**	If no suitable leader was found, then just return with the first conveniently
	**	accessable team member. This presumes that some member is better than no member
	**	at all.
	*/
	if (leader == NULL) {
		leader = Member;
	}

	return(leader);
}