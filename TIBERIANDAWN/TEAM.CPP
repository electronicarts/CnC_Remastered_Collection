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

/* $Header:   F:\projects\c&c\vcs\code\team.cpv   2.18   16 Oct 1995 16:48:34   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
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
 *                  Last Update : August 6, 1995 [JLB]                                         *
 *                                                                                             * 
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
 *   Assign_Mission_Target -- Sets teams mission target and clears old target                  *
 *   TeamClass::Add -- Adds specified object to team.                                          * 
 *   TeamClass::AI -- Process team logic.                                                      * 
 *   TeamClass::As_Target -- Converts this team object into a target number.                   * 
 *   TeamClass::Calc_Center -- Determines average location of team members.                    * 
 *   TeamClass::Control -- Updates control on a member unit.                                   * 
 *   TeamClass::Coordinate_Attack -- Handles coordinating a team attack.                       * 
 *   TeamClass::Coordinate_Conscript -- Gives orders to new recruit.                           * 
 *   TeamClass::Coordinate_Move -- Handles team movement coordination.                         * 
 *   TeamClass::Coordinate_Regroup -- Handles team idling (regrouping).                        * 
 *   TeamClass::Coordinate_Unload -- Tells the team to unload passengers now.                  * 
 *   TeamClass::Detach -- Removes specified target from team tracking.                         * 
 *   TeamClass::Init -- Initializes the team objects for scenario preparation.                 * 
 *   TeamClass::Is_A_Member -- Tests if a unit is a member of a team                           *
 *   TeamClass::Recruit -- Attempts to recruit members to the team for the given index ID.     * 
 *   TeamClass::Remove -- Removes the specified object from the team.                          * 
 *   TeamClass::Suspend_Teams -- Suspends activity for low priority teams                      *
 *   TeamClass::Took_Damage -- Informs the team when the team member takes damage.             * 
 *   TeamClass::Validate -- validates team pointer															  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"
#include "mission.h"

/*
**	This array records the number of teams in existance of each type.
*/
unsigned char TeamClass::Number[TEAMTYPE_MAX];

/*
**	This array records the success rating of each of the team types.
*/
unsigned char TeamClass::Success[TEAMTYPE_MAX];

/*
** This contains the value of the Virtual Function Table Pointer
*/
void * TeamClass::VTable;


/*********************************************************************************************** 
 * TeamClass::Validate -- validates team pointer															  *
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
int TeamClass::Validate(void) const
{
	int num;

	num = Teams.ID(this);
	if (num < 0 || num >= TEAM_MAX) {
		Validate_Error("TEAM");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
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
	TeamClass *ptr;

	Teams.Free_All();
	memset(Number, 0, sizeof(Number));
	memset(Success, 0, sizeof(Success));

	ptr = new TeamClass();
	VTable = ((void **)(((char *)ptr) + sizeof(AbstractClass) - 4))[0];
	delete ptr;
}	


void * TeamClass::operator new (size_t)
{
	void * ptr = Teams.Allocate();
	if (ptr) {
		((TeamClass *)ptr)->Set_Active();
	}
	return(ptr);
}	


void TeamClass::operator delete(void * ptr)
{
	if (ptr) {
		((TeamClass *)ptr)->IsActive = false;
	}
	Teams.Free((TeamClass *)ptr);
}	


TeamClass::~TeamClass(void)
{
	if (GameActive && Class) {
		Number[TeamTypes.ID(Class)]--;
		while (Member) {
			Remove(Member);
		}

		if (Class->IsTransient && !Number[TeamTypes.ID(Class)]) {
			delete (TeamTypeClass *)Class;
		}
	}
}	


TeamClass::TeamClass(TeamTypeClass const * type, HouseClass * owner) :
	Class(type),
	House(owner)
{
	memset(Quantity, 0, sizeof(Quantity));
	IsAltered = true;
	IsForcedActive = false;
	IsFullStrength = false;
	IsUnderStrength = true;
	IsReforming = false;
	IsLagging = false;
	IsMoving = false;
	IsHasBeen = false;
	Center = 0;
	Target = TARGET_NONE;
	ObjectiveCenter = 0;
	MissionTarget = TARGET_NONE;
	Member = 0;	
	Total = 0;
	Risk = 0;
	CurrentMission = -1;
	IsNextMission = true;
	TimeOut = 0;
	SuspendTimer.Clear();
	Suspended = false;
	Number[TeamTypes.ID(Class)]++;
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
	Validate();
	/*
	** First go through and find anyone who is currently targetting
	** the old mission target and clear their Tarcom.
	*/
	FootClass * unit = Member;
	while (unit) {
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
		unit = (FootClass *)unit->Member;
	}

	/*
	**	If there is not currently an override on the current mission target
	** then assign both MissionTarget and Target to the new target.  If
	** there is an overide, allow the team to keep fighting the overide but
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
	Validate();
	int	desired		= 0;
	int	old_under	= IsUnderStrength;
	int	old_full		= IsFullStrength;

	/*
	** If the team has been suspended then we need to check if its time for
	** us to reactivate the team.  If not, no team logic will be processed
	** for this team.
	*/
	if (Suspended) {
		if (!SuspendTimer.Expired()) {
			return;
		}
		Suspended = false;
	}

	/*
	**	If this team senses that its composition has been altered, then it should
	**	recalculate the under strength and full strength flags.
	*/
	if (IsAltered) {

		for (int index = 0; index < Class->ClassCount; index++) {
			desired += Class->DesiredNum[index];
		}
		
		if (Total) {
			IsFullStrength = (Total == desired);

			/*
			**	Human controlled teams are always considered full strength. This ensures
			**	that no new team members will be recruited and the team won't go into
			**	regroup logic.
			*/
			if (House->IsHuman) {
				IsUnderStrength = false;
			} else {

				/*
				**	Reinforcable teams will revert (or snap out of) the under strength
				**	mode when the members transition the magic 1/3 strength threshhold.
				*/
				if (Class->IsReinforcable) {
					IsUnderStrength = (Total <= desired / 3);
				} else {

					/*
					**	Teams that are not flagged as reinforcable are never considered under
					**	strength if the team has already started its main mission. This 
					**	ensures that once the team has started, it won't dally to pick up
					**	new members.
					*/
					IsUnderStrength = !IsHasBeen;
				}
			}
			IsAltered = false;
		} else {
			IsUnderStrength = true;
			IsFullStrength = false;
			Center = 0;

			/*
			**	A team that exists on the player's side is automatically destroyed
			**	when there are no team members left. This team was created as a 
			**	result of reinforcement logic and no longer needs to exist when there
			**	are no more team members.
			*/
			if (House->IsHuman || IsHasBeen) {
				Delete_This();
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
			Calc_Center(Center, ObjectiveCenter);

			/*
			** When a team is badly damaged and needs to regroup it should
			** pick a friendly building to go and regroup at.  Its first preference
			** should be somewhere near repair factory.  If it cannot find a repair
			** factory then it should pick another structure that is friendly to 
			** its side.
			*/
			CELL 	dest	= Center;
			int 	max	= 0x7FFFFFFF;

			for (int index = 0; index < Buildings.Count(); index++) {
				BuildingClass * b = Buildings.Ptr(index);
		
				if (b && !b->IsInLimbo && b->House == House && b->Class->Primary == WEAPON_NONE) {
					CELL cell = Coord_Cell(b->Center_Coord());
					int dist = Map.Cell_Distance(cell, Center) * (Map.Cell_Threat(cell, House->Class->House) + 1);

					if (*b == STRUCT_REPAIR) {
						dist >>= 1;
					}
					if (dist < max) {
						cell = Member->Safety_Point(Center, cell, 2, 4);
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
			Center = 0;
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
		DoType doaction = (Random_Pick(1, 2) == 1) ? DO_GESTURE1 : DO_GESTURE2;
		while (techno) {
			if (!techno->IsInLimbo && techno->What_Am_I() == RTTI_INFANTRY) {
				((InfantryClass *)techno)->Do_Action(doaction);
			}

			if (IsReforming || IsForcedActive) {
				techno->IsInitiated = true;
			}

			techno = techno->Member;
		}	
		CurrentMission	= -1;
		IsNextMission	= true;
		IsForcedActive = false;
	}

	/*
	**	If the team is moving or if there is no center position for
	**	the team, then the center position must be recalculated.
	*/
	if (IsReforming || IsMoving || Center == 0) {
		Calc_Center(Center, ObjectiveCenter);
	}

	/*
	**	Try to recruit members if there is room to do so for this team.
	**	Only try to recruit members for a non player controlled team.
	*/
	if (!IsMoving || (!IsFullStrength && Class->IsReinforcable) && !House->IsHuman) {
		for (int index = 0; index < Class->ClassCount; index++) {
			if (Quantity[index] < Class->DesiredNum[index]) {
				Recruit(index);
			}
		}
	}

	/*
	**	If there are no members of the team and the team has reached
	**	full strength at one time, then delete the team.
	*/
	if (!Member && IsHasBeen) {
		Delete_This();
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
			TeamMissionStruct const * mission = &Class->MissionList[CurrentMission];

			TimeOut = mission->Argument * (TICKS_PER_MINUTE/10);
			Target = TARGET_NONE;
			switch (mission->Mission) {
				case TMISSION_MOVECELL:
					Assign_Mission_Target(::As_Target((CELL)mission->Argument));
					break;

				case TMISSION_MOVE:
				case TMISSION_UNLOAD:
					/*
					**	Argument can be a waypoint index or a direct target.
					*/
					if (mission->Argument < WAYPT_COUNT) {
						Assign_Mission_Target(::As_Target((CELL)Waypoint[mission->Argument]));
					} else {
						Assign_Mission_Target((TARGET)mission->Argument);
					}
					break;

				case TMISSION_ATTACKTARCOM:
					Assign_Mission_Target(mission->Argument);
					break;

				default:
					Assign_Mission_Target(TARGET_NONE);
					break;
			}
		} else {
			Delete_This();
			return;
		}
	}

	/*
	**	Perform mission of the team. This depends on the mission list.
	*/
	if (Member && IsMoving && !IsReforming && !IsUnderStrength) {
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
		TeamMissionStruct const * mission = &Class->MissionList[CurrentMission];
		switch (mission->Mission) {
			case TMISSION_ATTACKBASE:
				if (!Target_Legal(MissionTarget)) {
					Assign_Mission_Target(Member->Greatest_Threat(THREAT_BUILDINGS));
					if (!Target_Legal(MissionTarget)) IsNextMission = true;
				}
				Coordinate_Attack();
				break;

			case TMISSION_ATTACKUNITS:
				if (!Target_Legal(MissionTarget)) {
					Assign_Mission_Target(Member->Greatest_Threat(THREAT_VEHICLES|THREAT_INFANTRY));
					if (!Target_Legal(MissionTarget)) IsNextMission = true;
				}
				Coordinate_Attack();
				break;

			case TMISSION_ATTACKCIVILIANS:
				if (!Target_Legal(MissionTarget)) {
					Assign_Mission_Target(Member->Greatest_Threat(THREAT_CIVILIANS));
					if (!Target_Legal(MissionTarget)) IsNextMission = true;
				}
				Coordinate_Attack();
				break;

			case TMISSION_ATTACKTARCOM:
			case TMISSION_RAMPAGE:
				if (!Target_Legal(MissionTarget)) {
					Assign_Mission_Target(Member->Greatest_Threat(THREAT_NORMAL));
					if (!Target_Legal(MissionTarget)) IsNextMission = true;
				}
				Coordinate_Attack();
				break;

			case TMISSION_DEFENDBASE:
				Coordinate_Move();
				break;

//			case TMISSION_HARVEST:
//				Coordinate_Move();
//				break;

			case TMISSION_UNLOAD:
				Coordinate_Unload();
				break;

			case TMISSION_MOVE:
				Coordinate_Move();
				break;

			case TMISSION_RETREAT:
				Coordinate_Move();
				break;

			case TMISSION_GUARD:
				Coordinate_Regroup();
				break;

			case TMISSION_LOOP:
				CurrentMission = mission->Argument-1;
				IsNextMission = true;
				break;
		}

		/*
		**	Check for mission time out condition. If the mission does in fact time out, then
		**	flag it so that the team mission list will advance.
		*/
		switch (mission->Mission) {
			case TMISSION_ATTACKBASE:
			case TMISSION_ATTACKUNITS:
			case TMISSION_ATTACKCIVILIANS:
			case TMISSION_RAMPAGE:
			case TMISSION_DEFENDBASE:
			case TMISSION_UNLOAD:
			case TMISSION_RETREAT:
			case TMISSION_GUARD:
				if (TimeOut.Expired()) {
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
 *          typeindex-- Optional value that specifies the index in the team type class array   * 
 *                      that this object belongs.                                              * 
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
bool TeamClass::Add(FootClass * obj, int typeindex)
{
	Validate();
	/*
	**	If this team doesn't accept new members, then don't accept this one either.
	*/
//	if (!Class->IsReinforcable && IsMoving) {
//		return(false);
//	}

	if (!obj || !obj->Strength || (obj->IsInLimbo && !ScenarioInit) || obj->In_Radio_Contact() || obj->House != House) {
		return(false);
	}

	TeamClass * team = obj->Team;

	/*
	**	Trying to add the team member to itself is an error condition. Just return
	**	with success, since the end result is the same.
	*/
	if (team == this) {
		return(true);
	}

	/*
	**	If the object is doing some mission that precludes it from joining
	**	a team then don't add it.
	*/
	if (obj->Mission == MISSION_STICKY || obj->Mission == MISSION_SLEEP || obj->Mission == MISSION_GUARD_AREA || obj->Mission == MISSION_HUNT || obj->Mission == MISSION_HARVEST) {
		return(false);
	}

	/*
	**	If this object is part of another team, then check to make sure that it
	**	is permitted to leave the other team in order to join this one. If not,
	**	then no further processing is allowed -- bail.
	*/
	if (team && 
			(/*team->Total >= Total || team->IsMoving ||*/
			team->Class->RecruitPriority >= Class->RecruitPriority)) {
		return(false);
	}

	/*
	**	If the proper team index was not provided, then find it in the type type class.
	**	On the chance that a match could not be found, then it is illegal to add this
	**	object to this team -- return with failure flag.
	*/
	if (typeindex == -1) {
		for (typeindex = 0; typeindex < Class->ClassCount; typeindex++) {
			if (Class->Class[typeindex] == &obj->Class_Of()) {
				break;
			}
		}
	}

	/*
	**	If the team is already full of this type, then adding the object is not allowed.
	**	Return with a failure flag in this case.
	*/
	if (Quantity[typeindex] >= Class->DesiredNum[typeindex]) {
		return(false);
	}

	/*
	**	All is ok to add the object to the team, but if the object is already part of
	**	another team, then it must be removed from that team first.
	*/
	if (team) {
		team->Remove(obj);
	}

	/*
	**	Actually add the object to the team.
	*/
	Quantity[typeindex]++;
	obj->IsInitiated = (Member == NULL);
	obj->Member = Member;
	Member = obj;
	obj->Team = this;
	Total++;
	Risk += obj->Risk();
	if (!Center) {
		Calc_Center(Center, ObjectiveCenter);
	}

	/*
	**	Return with success, since the object was added to the team.
	*/
	IsAltered = true;
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
	Validate();
	/*
	**	Make sure that the object is in fact a member of this team. If not, then it can't
	**	be removed. Return success because the end result is the same.
	*/
	if (obj->Team != this) {
		return(true);
	}

	/*
	**	If the proper team index was not provided, then find it in the type type class. The
	**	team type class will not be set if the appropriate type could not be found
	**	for this object. This indicates that the object was illegally added. Continue to
	**	process however, since removing this object from the team is a good idea.
	*/
	if (typeindex == -1) {
		for (typeindex = 0; typeindex < Class->ClassCount; typeindex++) {
			if (Class->Class[typeindex] == &obj->Class_Of()) {
				break;
			}
		}
	}

	/*
	**	Decrement the counter for the team class. There is now one less of this object type.
	*/
	if ((unsigned)typeindex < Class->ClassCount) {
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
	while (curr && (!found || !initiated)) {
		if (curr == obj) {
			if (prev) {
				prev->Member = curr->Member;
			} else {
				Member = curr->Member;
			}
			FootClass * temp = curr->Member;
			curr->Member = 0;
			curr->Team = 0;
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
	**	If, after removing the team member, there are no initiated members left
	**	in the team, then just make the first remaining member of the team the
	**	team captain. Mark the center location of the team as invalid so that
	**	it will be centered around the captain.
	*/
	if (!initiated && Member) {
		Member->IsInitiated = true;
		Center = 0;
	}

	/*
	**	Must record that the team composition has changed. At the next opportunity,
	**	the team members will be counted and appropriate AI adjustments made.
	*/
	IsAltered = true;
	return(true);
}	


/*********************************************************************************************** 
 * TeamClass::Recruit -- Attempts to recruit members to the team for the given index ID.       * 
 *                                                                                             * 
 *    This routine will take the given index ID and scan for available objects of that type    * 
 *    to recruit to the team. Recruiting will continue until that object type has either       * 
 *    been exhausted or if the team's requirment for that type has been filled.                * 
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
	Validate();
	int added = 0;				// Total number added to team.

	/*
	**	Quick check to see if recruiting is really allowed for this index or not.
	*/
	if (Class->DesiredNum[typeindex] > Quantity[typeindex]) {

		/*
		**	For infantry objects, sweep through the infantry in the game looking for
		**	ones owned by the house that owns the team. When found, try to add.
		*/
		if (Class->Class[typeindex]->What_Am_I() == RTTI_INFANTRYTYPE) {

			for (int index = 0; index < Infantry.Count(); index++) {
				InfantryClass * infantry = Infantry.Ptr(index);

				if (infantry->House == House && infantry->Class == Class->Class[typeindex]) {
					if (Add(infantry, typeindex)) {
						added++;
					}
				}

				/*
				**	If there is sufficient quantity of this type of object recruited to the
				**	team, then abort further scanning for members.
				*/
				if (Quantity[typeindex] >= Class->DesiredNum[typeindex]) {
					break;
				}
			}
		}

		if (Class->Class[typeindex]->What_Am_I() == RTTI_UNITTYPE) {

			for (int index = 0; index < Units.Count(); index++) {
				UnitClass * unit = Units.Ptr(index);

				if (unit->House == House && unit->Class == Class->Class[typeindex]) {
					if (Add(unit, typeindex)) {
						added++;

						/*
						**	If a transport is added to the team, the occupants
						**	are added by default.
						*/
						FootClass * f = unit->Attached_Object();
						while (f) {
							Add(f);
							f = (FootClass *)f->Next;
						}
					}
				}

				/*
				**	If there is sufficient quantity of this type of object recruited to the
				**	team, then abort further scanning for members.
				*/
				if (Quantity[typeindex] >= Class->DesiredNum[typeindex]) {
					break;
				}
			}
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
	Validate();

	/*
	**	If the target to detatch matches the target of this team, then remove
	**	the target from this team's Tar/Nav com and let the chips fall
	**	where they may.
	*/
	if (Target == target) {
		Target = TARGET_NONE;
	}
	if (MissionTarget == target) {
		MissionTarget = TARGET_NONE;
	}

}	


/*********************************************************************************************** 
 * TeamClass::As_Target -- Converts this team object into a target number.                     * 
 *                                                                                             * 
 *    This routine is used by the save/load code to produce a persistant identifier for this   * 
 *    team object.                                                                             * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Returns the team represented as a target number.                                   * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/14/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
TARGET TeamClass::As_Target(void) const
{
	Validate();
	return(Build_Target(KIND_TEAM, Teams.ID(this)));
}


/*********************************************************************************************** 
 * TeamClass::Calc_Center -- Determines average location of team members.                      * 
 *                                                                                             * 
 *    Use this routine to calculate the "center" location of the team. This is the average     * 
 *    position of all members of the team. Using this center value it is possible to tell      * 
 *    if a team member is too far away and where to head to in order to group up.              * 
 *                                                                                             * 
 * INPUT:   none                                                                               * 
 *                                                                                             * 
 * OUTPUT:  Returns with the cell number of the team's center point. If the team contains      * 
 *          no members, then the return value will be zero.                                    * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   12/29/1994 JLB : Created.                                                                 * 
 *=============================================================================================*/
void TeamClass::Calc_Center(CELL &center, CELL &obj_center) const
{
	Validate();
	long	x = 0;
	long	y = 0;
	int   dist = 0x7FFFFFFF;
	int	quantity = 0;
	FootClass * unit;

	obj_center	= 0;
	center 		= 0;

	unit = Member;
	while (unit) {
		if (unit->IsInitiated && !unit->IsInLimbo) {
			CELL c = Coord_Cell(unit->Center_Coord());
			if (unit->Distance(Target) < dist) {
				dist = unit->Distance(Target);
				obj_center = c;
			}
			x += Cell_X(c);
			y += Cell_Y(c);
			quantity++;
		}
		unit = unit->Member;
	}
	if (quantity) {
		x /= quantity;
		y /= quantity;
		CELL cell = XY_Cell((int)x, (int)y);
		center = cell;
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
 *          source   -- The purpetrator of the damage.                                         * 
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
	Validate();
	if ((result != RESULT_NONE) && (!Class->IsSuicide)) {
		if (!IsMoving) {
			// Should run to a better hiding place or disband into a group of hunting units.
		} else {

			if (source && !Is_A_Member(source) && Member && Member->What_Am_I() != RTTI_AIRCRAFT) {
				if (Target != source->As_Target()) {

					/*
					**	Don't change target if the team's target is one that can fire as well. There is
					**	no point in endlessly shuffling between targets that have firepower.
					*/
					if (Target_Legal(Target)) {
						TechnoClass * techno = As_Techno(Target);

						if (techno && ((TechnoTypeClass const &)techno->Class_Of()).Primary != WEAPON_NONE) {
							if (techno->In_Range(Cell_Coord(Center), 0)) {
								return;
							}
						}
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
	Validate();
	if (!Target_Legal(Target)) {
		Target = MissionTarget;
	}

	if (!Target_Legal(Target)) {
		IsNextMission = true;

	} else {

		FootClass * unit = Member;
		while (unit) {

			Coordinate_Conscript(unit);

			if (unit->IsInitiated && !unit->IsInLimbo) {

				if (unit->Mission != MISSION_ATTACK && unit->Mission != MISSION_ENTER && unit->Mission != MISSION_CAPTURE) {
					unit->Assign_Mission(MISSION_ATTACK);
					unit->Assign_Target(TARGET_NONE);
					unit->Assign_Destination(TARGET_NONE);
				}

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
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   04/06/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
bool TeamClass::Coordinate_Regroup(void)
{
	Validate();
	FootClass * unit   = Member;
	bool			retval = true;

	/*
	**	Regroup default logic.
	*/
	while (unit) {

		Coordinate_Conscript(unit);

		if (unit->IsInitiated && !unit->IsInLimbo) {

			if (unit->Distance(Center) > STRAY_DISTANCE && (unit->Mission != MISSION_GUARD_AREA || !Target_Legal(unit->TarCom))) {
				if (unit->Mission != MISSION_MOVE || !Target_Legal(unit->NavCom) || ::Distance(As_Cell(unit->NavCom), Center) > STRAY_DISTANCE) {
					unit->Assign_Mission(MISSION_MOVE);
					unit->Assign_Destination(::As_Target(Center));
				}
				retval = false;
			} else {

				/*
				**	This unit has gotten close enough to the team center so that it is
				**	now considered intiated. An initiated unit is considered when calculating
				**	the center of the team.
				*/
				unit->IsInitiated = true;

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
	Validate();
	FootClass * unit = Member;
	bool finished = true;

	if (!Target_Legal(Target)) {
		Target = MissionTarget;
	}

	if (Target_Legal(Target)) {

		if (!Lagging_Units()) {


			while (unit) {

				Coordinate_Conscript(unit);

				if (unit->IsInitiated && !unit->IsInLimbo) {

					if (unit->What_Am_I() != RTTI_AIRCRAFT && unit->Distance(Center) > STRAY_DISTANCE) {
						IsLagging = true;
						finished = false;
					} else {

						if ((unit->Distance(Target)/ICON_LEPTON_W) > STRAY_DISTANCE || 
							(unit->What_Am_I() == RTTI_AIRCRAFT && 
							((AircraftClass *)unit)->Altitude > 0 && 
							Class->MissionList[CurrentMission+1].Mission != TMISSION_MOVE)) {

							if (unit->Mission != MISSION_MOVE) {
								unit->Assign_Mission(MISSION_MOVE);
							}
							if (unit->NavCom != Target) {
								unit->Assign_Destination(Target);
							}
							finished = false;
						} else {
							if (unit->Mission == MISSION_MOVE && !Target_Legal(unit->NavCom)) {
								unit->Enter_Idle_Mode();
							}
						}
					}
				}

				unit = unit->Member;
			}
		} else {
			finished = false;
		}
	}

	/*
	**	If all the team members are close enough to the desired destination, then
	**	move to the next mission.
	*/
	if (finished && IsMoving) {
		IsNextMission = true;
	}
}	


/***************************************************************************
 * Lagging_Units -- processes units that cant keep up with the pack        *
 *                                                                         *
 * INPUT:      none                                                        *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/01/1995 PWG : Created.                                             *
 *=========================================================================*/
bool TeamClass::Lagging_Units(void)
{
	Validate();
	FootClass * unit = Member;
	bool lag = false;

	/*
	** If the IsLagging bit is not set, then obviously there are no lagging
	** units.
	*/
	if (!IsLagging) return(false);

	/*
	**	Scan through all of the units, searching for units who are having 
	** trouble keeping up with the pack.
	*/ 
	while (unit) {

		if (!unit->IsInLimbo) {
			/*
			** If we find a unit who has fallen to far away from the center of
			** the pack, then we need to order that unit to catch up with the
			** first unit.
			*/
			if (unit->Distance(ObjectiveCenter) > STRAY_DISTANCE) {
				if (unit->Mission != MISSION_MOVE || !Target_Legal(unit->NavCom) || ::Distance(As_Cell(unit->NavCom), ObjectiveCenter) > STRAY_DISTANCE) {
					unit->Assign_Mission(MISSION_MOVE);
					unit->Assign_Destination(::As_Target(ObjectiveCenter));
				}
				lag = true;
			} else {
				/*
				** We need to order all of the other units to hold there
				** position until all lagging units catch up.
				*/
				unit->Assign_Mission(MISSION_GUARD);
				unit->Assign_Destination(TARGET_NONE);
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
 * TeamClass::Coordinate_Unload -- Tells the team to unload passengers now.                    * 
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
void TeamClass::Coordinate_Unload(void)
{
	Validate();
	FootClass * unit = Member;
	bool finished = true;

	while (unit) {

		Coordinate_Conscript(unit);

		if (unit->IsInitiated && !unit->IsInLimbo) {
			if (unit->Is_Something_Attached()) {

				/*
				**	Loaner transports will break off of the team at this time. The normal
				**	unload logic for the transport will proceed normally. The rest of the team
				**	members will be in a dormant state until they are unloaded.
				*/
				if (unit->IsALoaner) {
					Remove(unit);
					unit->Commence();
					unit->Assign_Mission(MISSION_UNLOAD);
					unit->Assign_Destination(Target);
				} else {
					if (unit->Mission != MISSION_UNLOAD) {
						unit->Assign_Mission(MISSION_UNLOAD);
						unit->Assign_Destination(Target);
					}
				}
				finished = false;
			}
		}

		unit = unit->Member;
	}

	if (finished) {
		IsNextMission = true;
	}
}	


/*********************************************************************************************** 
 * TeamClass::Coordinate_Conscript -- Gives orders to new recruit.                             * 
 *                                                                                             * 
 *    This routine will give the movement orders to the conscript so that it will group        * 
 *    with the other members of the team.                                                      * 
 *                                                                                             * 
 * INPUT:   unit  -- Pointer to the conscript unit.                                            * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   04/06/1995 JLB : Created.                                                                 * 
 *=============================================================================================*/
void TeamClass::Coordinate_Conscript(FootClass * unit)
{
	Validate();
	if (unit && !unit->IsInitiated && !unit->IsInLimbo) {
		if (unit->Distance(Center) > STRAY_DISTANCE) {
			if (!Target_Legal(unit->NavCom)) {
				unit->Assign_Mission(MISSION_MOVE);
				unit->Assign_Target(TARGET_NONE);
				unit->Assign_Destination(::As_Target(Center));
			}
		} else {

			/*
			**	This unit has gotten close enough to the team center so that it is
			**	now considered intiated. An initiated unit is considered when calculating
			**	the center of the team.
			*/
			unit->IsInitiated = true;
		}
	}
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
	Validate();
	FootClass * unit = Member;
	while (unit) {
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
void TeamClass::Suspend_Teams(int priority)
{
	for (int index = 0; index < Teams.Count(); index++) {
		TeamClass *team =	Teams.Ptr(index);

		/*
		**	If a team is below the "survival priority level", then it gets
		**	destroyed. The team members are then free to be reassigned.
		*/
		if (team && team->Class->RecruitPriority < priority) {
			FootClass * unit = team->Member;
			while (team->Member) {
				team->Remove(team->Member);
			}
			team->IsAltered = true;
			team->SuspendTimer = TICKS_PER_MINUTE*2;
			team->Suspended = true;
		}
	}
}