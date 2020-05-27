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

/* $Header: /CounterStrike/REINF.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : REINF.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 24, 1994                                                 *
 *                                                                                             *
 *                  Last Update : July 26, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Create_Air_Reinforcement -- Creates air strike reinforcement                              *
 *   Create_Special_Reinforcement -- Ad hoc reinforcement handler.                             *
 *   Do_Reinforcements -- Create and place a reinforcement team.                               *
 *   _Consists_Only_Of_Infantry -- Determine if this group consists only of infantry.          *
 *   _Create_Group -- Create a group given team specification.                                 *
 *   _Pop_Group_Out_Of_Object -- Process popping the group out of the object.                  *
 *   _Who_Can_Pop_Out_Of -- Find a suitable host for these reinforcements.                     *
 *   _Need_To_Take -- Examines unit to determine if it should be confiscated.                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***********************************************************************************************
 * _Pop_Group_Out_Of_Object -- Process popping the group out of the object.                    *
 *                                                                                             *
 *    This routine will cause the group to pop out of the object specified.                    *
 *                                                                                             *
 * INPUT:   group    -- Pointer to the first object in the group to be popped out.             *
 *                                                                                             *
 *          object   -- Pointer to the object that the group is to pop out of.                 *
 *                                                                                             *
 * OUTPUT:  bool; Was the group popped out of the specified object?                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static bool _Pop_Group_Out_Of_Object(FootClass * group, TechnoClass * object)
{
	assert(group != NULL && object != NULL);
	int quantity = 0;

	/*
	**	Take every infantry member of this group and detach it from the group list
	**	and then make it pop out of the candidate source.
	*/
	while (group != NULL) {
		TechnoClass * todo = group;
		group = (FootClass *)(ObjectClass *)group->Next;
		todo->Next = NULL;

		switch (object->What_Am_I()) {

			/*
			**	The infantry just walks out of a building.
			*/
			case RTTI_BUILDING:
				if (object->Exit_Object(todo) != 2) {
					delete todo;
				} else {
					++quantity;
				}
				break;

			/*
			**	Infantry get attached to transport vehicles and then unload.
			*/
			case RTTI_UNIT:
			case RTTI_VESSEL:
			case RTTI_AIRCRAFT:
				object->Attach((FootClass *)todo);
				object->Assign_Mission(MISSION_UNLOAD);
				++quantity;
				break;

			default:
				delete todo;
				break;
		}
	}

	return (quantity != 0);
}


/***********************************************************************************************
 * _Need_To_Take -- Examines unit to determine if it should be confiscated.                    *
 *                                                                                             *
 *    The unit is examined and if the owning house needs to confiscate it, then this routine   *
 *    will return TRUE. In other cases, the unit should be left to its own devices.            *
 *                                                                                             *
 * INPUT:   unit  -- Pointer to the object to examine.                                         *
 *                                                                                             *
 * OUTPUT:  bool; Should the object be confiscated by the player so that it becomes one of     *
 *                his normal game objects?                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool _Need_To_Take(AircraftClass const * air)
{
	if (*air == AIRCRAFT_YAK || *air == AIRCRAFT_MIG) {
		int deficit = air->House->Get_Quantity(STRUCT_AIRSTRIP);
//		int deficit = air->House->Get_Quantity(STRUCT_AIRSTRIP) - (air->House->Get_Quantity(AIRCRAFT_YAK)+air->House->Get_Quantity(AIRCRAFT_MIG));

		/*
		**	Loop through all aircraft and subtract all the ones that are NOT loaners.
		*/
		for (int index = 0; index < Aircraft.Count(); index++) {
			AircraftClass const * airptr = Aircraft.Ptr(index);
			if ((*airptr == AIRCRAFT_YAK || *airptr == AIRCRAFT_MIG) && airptr->IsOwnedByPlayer && !airptr->IsALoaner && airptr != air) {
				deficit -= 1;
				if (deficit == 0) break;
			}
		}

		if (deficit > 0) return(true);
	}
	return(false);
}


/***********************************************************************************************
 * _Create_Group -- Create a group given team specification.                                   *
 *                                                                                             *
 *    This routine will create all members of the group as specified by the team type.         *
 *                                                                                             *
 * INPUT:   teamtype -- Pointer to the team type that specifies what objects should be         *
 *                      created in this group.                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the first member of the group created.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static FootClass * _Create_Group(TeamTypeClass const * teamtype)
{
	assert(teamtype != NULL);

	TeamClass * team = new TeamClass(teamtype);
	if (team != NULL) {
		team->Force_Active();
	}

	bool hasunload = false;
	for (int tm = 0; tm < teamtype->MissionCount; tm++) {
		if (teamtype->MissionList[tm].Mission == TMISSION_UNLOAD) {
			hasunload = true;
			break;
		}
	}

	/*
	**	Now that the official source for the reinforcement has been determined, the
	**	objects themselves must be created.
	*/
	FootClass * transport = NULL;
	FootClass * object = NULL;
	for (int index = 0; index < teamtype->ClassCount; index++) {
		TechnoTypeClass const * tclass = teamtype->Members[index].Class;

		for (int sub = 0; sub < teamtype->Members[index].Quantity; sub++) {
			ScenarioInit++;
			FootClass * temp = (FootClass *)tclass->Create_One_Of(HouseClass::As_Pointer(teamtype->House));
			ScenarioInit--;

			if (temp != NULL) {

				/*
				**	Add the member to the team.
				*/
				if (team != NULL) {
					ScenarioInit++;
					bool ok = team->Add(temp);
//Mono_Printf("Added to team = %d.\n", ok);Keyboard->Get();

					ScenarioInit--;
					temp->IsInitiated = true;
				}

				if (temp->What_Am_I() == RTTI_AIRCRAFT && !_Need_To_Take((AircraftClass const *)temp)) {
					temp->IsALoaner = true;
				}

				/*
				**	Build the list of transporters and passengers.
				*/
				if (tclass->Max_Passengers() > 0) {

					/*
					**	Link to the list of transports.
					*/
					temp->Next = transport;
					transport = temp;

				} else {

					/*
					**	Link to the list of normal objects.
					*/
					temp->Next = object;
					object = temp;
				}
			}
		}
	}

	/*
	**	If the group consists of transports and normal objects, then assign the normal
	**	objects to be passengers on the transport.
	*/
	if (transport != NULL && object != NULL) {
		transport->Attach(object);

		/*
		**	HACK ALERT! If the this team has an unload mission, then flag the transport
		**	as a loaner so that it will exit from the map when the unload process is
		**	complete, but only if the transport is an aircraft type.
		*/
		if (hasunload && (transport->What_Am_I() == RTTI_AIRCRAFT || transport->What_Am_I() == RTTI_VESSEL)) {
			transport->IsALoaner = true;
		}
	}

	/*
	**	For JUST transport helicopters, consider the loaner a gift if there are
	**	no passengers.
	*/
	if (transport != NULL && object == NULL && transport->What_Am_I() == RTTI_AIRCRAFT && *((AircraftClass *)transport) == AIRCRAFT_TRANSPORT) {
		transport->IsALoaner = false;
	}

	if (transport == 0 && object == 0) {
		if (team != NULL) delete team;
		return(NULL);
	}

	/*
	**	If this group consists only of non-transport object, then just return with a pointer
	**	to the first member of the group.
	*/
	if (transport == NULL) {
		return(object);
	}

	return(transport);
}


/***********************************************************************************************
 * _Consists_Only_Of_Infantry -- Determine if this group consists only of infantry.            *
 *                                                                                             *
 *    Use this to determine if the specified group only contains infantry. Such a reinforcement*
 *    group is a candidate for popping out of a building or transport vehicle rather than      *
 *    driving/walking/sailing/flying onto the map under its own power.                         *
 *                                                                                             *
 * INPUT:   first -- Pointer to the first object in the group to examine.                      *
 *                                                                                             *
 * OUTPUT:  bool; Is the entire group composed of infantry type units?                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static bool _Consists_Only_Of_Infantry(FootClass const * first)
{
	while (first != NULL) {
		if (first->What_Am_I() != RTTI_INFANTRY) {
			return(false);
		}
		first = (FootClass const *)((ObjectClass *)first->Next);
	}
	return(true);
}


/***********************************************************************************************
 * _Who_Can_Pop_Out_Of -- Find a suitable host for these reinforcements.                       *
 *                                                                                             *
 *    This routine is used to scan nearby locations to determine if there is a suitable host   *
 *    for these reinforcements to "pop out of" (apologies to Aliens). Typical hosts include    *
 *    buildings and transport vehicles (of any kind).                                          *
 *                                                                                             *
 * INPUT:   origin   -- The cell that should be scanned from. Only this location and immediate *
 *                      adjacent locations will be scanned.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a suitable host. If none could be found then NULL is     *
 *          returned.                                                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static TechnoClass * _Who_Can_Pop_Out_Of(CELL origin)
{
	CellClass * cellptr = &Map[origin];
	TechnoClass * candidate = NULL;

	for (int f = -1; f < 8; f++) {
		CellClass * ptr = cellptr;
		if (f != -1) {
			ptr = &ptr->Adjacent_Cell(FacingType(f));
		}

		BuildingClass * building = ptr->Cell_Building();
		if (building && building->Strength > 0) {
			candidate = building;
		}

		UnitClass * unit = ptr->Cell_Unit();
		if (unit && unit->Strength && unit->Class->Max_Passengers() > 0) {
			return(unit);
		}
	}
	return(candidate);
}


/***********************************************************************************************
 * Do_Reinforcements -- Create and place a reinforcement team.                                 *
 *                                                                                             *
 *    This routine is called when a reinforcement team must be created and placed on the map.  *
 *    It will create all members of the team and place them at the location determined from    *
 *    the team composition. The reinforcement team should follow team orders until overridden  *
 *    by AI or player intervention.                                                            *
 *                                                                                             *
 * INPUT:   teamtype -- Pointer to the team type to create as a reinforcement.                 *
 *                                                                                             *
 * OUTPUT:  Was the reinforcement successfully created and placed?                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *   05/18/1995 JLB : Returns success or failure condition.                                    *
 *   06/19/1995 JLB : Announces reinforcements.                                                *
 *   02/15/1996 JLB : Recognizes team reinforcement location.                                  *
 *=============================================================================================*/
bool Do_Reinforcements(TeamTypeClass const * teamtype)
{
	assert(teamtype != 0);

	/*
	**	perform some preliminary checks for validity.
	*/
	if (!teamtype || !teamtype->ClassCount) return(false);

	/*
	**	HACK ALERT!
	**	Give this team an attack waypoint mission that will attack the waypoint location of this
	**	team if there are no team missions previously assigned.
	*/
	if (teamtype->MissionCount == 0) {
		TeamTypeClass * tt = (TeamTypeClass *)teamtype;
		tt->MissionCount = 1;
		tt->MissionList[0].Mission = TMISSION_ATT_WAYPT;
		tt->MissionList[0].Data.Value = teamtype->Origin;
	}

	FootClass * object = _Create_Group(teamtype);


//Mono_Printf("%d-%s (object=%p, team=%d).\n", __LINE__, __FILE__, object, object->Team.Is_Valid());Keyboard->Get();


	/*
	**	Bail on this reinforcement if no reinforcements could be created.
	**	This is probably because the object maximum was reached.
	*/
	if (!object) {
		return(false);
	}

	/*
	**	Special case code to handle infantry types that run from a building. This presumes
	**	that infantry are never a transport (which is safe to do).
	*/
	if (object != NULL && teamtype->Origin != -1 && _Consists_Only_Of_Infantry(object)) {

		/*
		**	Search for an object that these infantry can pop out of.
		*/
		TechnoClass * candidate = _Who_Can_Pop_Out_Of(Scen.Waypoint[teamtype->Origin]);

		if (candidate != NULL) {
			return(_Pop_Group_Out_Of_Object(object, candidate));
		}
	}

	/*
	**	The reinforcements must be delivered the old fashioned way -- by moving onto the
	**	map using their own power. First order of business is to determine where they
	**	should arrive from.
	*/
	SourceType source = HouseClass::As_Pointer(teamtype->House)->Control.Edge;
	if (source == SOURCE_NONE) {
		source = SOURCE_NORTH;
	}

	/*
	**	Pick the location where the reinforcements appear and then place
	**	them there.
	*/
	bool placed = false;

	FacingType eface = (FacingType)(source << 1);	// Facing to enter map.

	CELL cell = Map.Calculated_Cell(source, teamtype->Origin, -1, object->Techno_Type_Class()->Speed);
#ifdef FIXIT_ANTS
	/*
	**	For the ants, they will pop out of the ant hill directly.
	*/
	if (teamtype->Origin != -1 && object->What_Am_I() == RTTI_UNIT && 
			(*((UnitClass*)object) == UNIT_ANT1 ||
			*((UnitClass*)object) == UNIT_ANT2 ||
			*((UnitClass*)object) == UNIT_ANT3))  {
		CELL newcell = Scen.Waypoint[teamtype->Origin];
		if (newcell != -1)  {
			if (Map[newcell].TType == TEMPLATE_HILL01)  {
				cell = newcell;
			}
		}
	}
#endif

	CELL newcell = cell;

	FootClass * o = (FootClass *)(ObjectClass *)object->Next;
	object->Next = 0;
	bool okvoice = false;
	while (newcell > 0 && object != NULL) {
		DirType desiredfacing = Facing_Dir(eface);
		if (object->What_Am_I() == RTTI_AIRCRAFT) {
			desiredfacing = Random_Pick(DIR_N, DIR_MAX);
		}

		ScenarioInit++;
		if (object->Unlimbo(Cell_Coord(newcell), desiredfacing)) {
			okvoice = true;

			/*
			**	If this object is part of a team, then the mission for this
			**	object will be guard. The team handler will assign the proper
			**	mission that it should follow.
			*/
			if (object->What_Am_I() != RTTI_AIRCRAFT) {
				object->Assign_Mission(MISSION_GUARD);
				object->Commence();
			}

		} else {

			/*
			**	Could not unlimbo at location specified so find an adjacent location that it can
			**	be unlimboed at. If this fails, then abort the whole placement process.
			*/
			FacingType adj;
			for (adj = FACING_N; adj < FACING_COUNT; adj++) {
				CELL trycell = Adjacent_Cell(newcell, adj);
				if (!Map.In_Radar(trycell) && object->Can_Enter_Cell(trycell, adj) == MOVE_OK) {
					newcell = trycell;
					break;
				}
			}
			if (adj < FACING_COUNT) continue;
			newcell = 0;
		}
		ScenarioInit--;

		object = o;
		if (object != NULL) {
			o = (FootClass *)(ObjectClass *)object->Next;
			object->Next = 0;
		}
	}

	/*
	**	If there are still objects that could not be placed, then delete them.
	*/
	if (o != NULL) {
		while (o != NULL) {
			FootClass * old = o;
			o = (FootClass *)(ObjectClass *)o->Next;
			old->Next = 0;

			delete old;
		}
	}

	/*
	**	Announce when the reinforcements have arrived.
	*/
	if (okvoice && teamtype->House == PlayerPtr->Class->House) {
		Speak(VOX_REINFORCEMENTS, NULL, newcell ? Cell_Coord(newcell) : 0);
	}

	return(true);
}


/***********************************************************************************************
 * Create_Special_Reinforcement -- Ad hoc reinforcement handler.                               *
 *                                                                                             *
 *    Use this routine to bring on a reinforcement that hasn't been anticipated by the trigger *
 *    system. An example of this would be replacement harvesters or airfield ordered units.    *
 *    The appropriate transport is created (if necessary) and a mission is assigned such that  *
 *    the object will legally bring itself onto the playing field.                             *
 *                                                                                             *
 * INPUT:   house    -- The owner of this reinforcement.                                       *
 *                                                                                             *
 *          type     -- The object to bring on.                                                *
 *                                                                                             *
 *          another  -- This is reserved for the transport class in those cases where the      *
 *                      transport MUST be forced to a specific type.                           *
 *                                                                                             *
 *          mission  -- The mission to assign this reinforcement team.                         *
 *                                                                                             *
 *          argument -- Optional team mission argument (usually a waypoint).                   *
 *                                                                                             *
 * OUTPUT:  Was the special reinforcement created without error?                               *
 *                                                                                             *
 * WARNINGS:   This routine will fail if a team type cannot be created.                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Create_Special_Reinforcement(HouseClass * house, TechnoTypeClass const * type, TechnoTypeClass const * another, TeamMissionType mission, int argument)
{
	assert(house != 0);
	assert(type != 0);

	if (house && type) {
		TeamTypeClass * team = new TeamTypeClass();

		if (team) {

			/*
			**	If there is no overridden mission assign to this special reinforcement, then
			**	we must assign something. If not, the reinforcement will just sit at the edge
			**	of the map.
			*/
			if (!another && mission == TMISSION_NONE) {
				mission = TMISSION_MOVECELL;
				argument = Map.Calculated_Cell(house->Control.Edge);
			}

			/*
			**	Fill in the team characteristics.
			*/
			strcpy((char *)&team->IniName[0], "TEMP");
			team->IsReinforcable = false;
			team->IsTransient = true;
			team->ClassCount = 1;
			team->Members[0].Class = type;
			team->Members[0].Quantity = 1;
			team->MissionCount = 1;
			if (mission == TMISSION_NONE) {
				team->MissionList[0].Mission	= TMISSION_UNLOAD;
				team->MissionList[0].Data.Value = WAYPT_REINF;
			} else {
				team->MissionList[0].Mission	= mission;
				team->MissionList[0].Data.Value = argument;
			}
			team->House = house->Class->House;
			if (another) {
				team->ClassCount++;
				team->Members[1].Class = another;
				team->Members[1].Quantity = 1;
			}

			bool ok = Do_Reinforcements(team);
			if (!ok) delete team;
			return(ok);
		}
	}
	return(false);
}


/***********************************************************************************************
 * Create_Air_Reinforcement -- Creates air strike reinforcement                                *
 *                                                                                             *
 *    This routine is used to launch an airstrike. It will create the necessary aircraft and   *
 *    assign them to attack the target specified. This routine bypasses the normal             *
 *    reinforcement logic since it doesn't need the sophistication of unloading and following  *
 *    team mission lists.                                                                      *
 *                                                                                             *
 * INPUT:   house    -- The perpetrator of this air strike.                                    *
 *                                                                                             *
 *          air      -- The type of aircraft to make up this airstrike.                        *
 *                                                                                             *
 *          number   -- The number of aircraft in this airstrike.                              *
 *                                                                                             *
 *          mission  -- The mission to assign the aircraft.                                    *
 *                                                                                             *
 *          tarcom   -- The target to assign these aircraft.                                   *
 *                                                                                             *
 *          navcom   -- The navigation target to assign (if necessary).                        *
 *                                                                                             *
 * OUTPUT:  Returns the number of aircraft created for this airstrike.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Commented.                                                               *
 *=============================================================================================*/
int Create_Air_Reinforcement(HouseClass * house, AircraftType air, int number, MissionType mission, TARGET tarcom, TARGET navcom, InfantryType passenger)
{
	assert(house != 0);
	assert((unsigned)air < AIRCRAFT_COUNT);
	assert(number != 0);
	assert((unsigned)mission < MISSION_COUNT);
	/*
	** Get a pointer to the class of the object that we are going to create.
	*/
	TechnoTypeClass const * type = (TechnoTypeClass *)&AircraftTypeClass::As_Reference(air);

	/*
	** Abort the airstrike if Tanya is the passenger and she's dead.
	*/
	if (passenger == INFANTRY_TANYA && IsTanyaDead) {
		number = 0;
	}

	/*
	** Loop through the number of objects we are supposed to create and
	** 	create and place them on the map.
	*/
	int sub;
	for (sub = 0; sub < number; sub++) {

		/*
		** Create one of the required objects.  If this fails we could have
		** a real problem.
		*/
		ScenarioInit++;
		TechnoClass * obj = (TechnoClass *)type->Create_One_Of(house);
		ScenarioInit--;
		if (!obj) return(sub);

		/*
		** Flying objects always have the IsALoaner bit set.
		*/
		obj->IsALoaner = true;

		/*
		** Find a cell for the object to come in on.  This is stolen from the
		** the code that handles a SOURCE_AIR in the normal logic.
		*/
		SourceType source = house->Control.Edge;
		switch (source) {
			case SOURCE_NORTH:
			case SOURCE_EAST:
			case SOURCE_SOUTH:
			case SOURCE_WEST:
				break;

			default:
				source = SOURCE_NORTH;
				break;
		}
		CELL newcell = Map.Calculated_Cell(source, -1, -1, SPEED_WINGED);

		/*
		** Try and place the object onto the map.
		*/
		ScenarioInit++;
		int placed = obj->Unlimbo(Cell_Coord(newcell), DIR_N);
		ScenarioInit--;
		if (placed) {

			/*
			** If we succeeded in placing the obj onto the map then
			** now we need to give it a mission and destination.
			*/
			obj->Assign_Mission(mission);

			/*
			** If a navcom was specified then set it.
			*/
			if (navcom != TARGET_NONE) {
				obj->Assign_Destination(navcom);
			}

			/*
			** If a tarcom was specified then set it.
			*/
			if (tarcom != TARGET_NONE) {
				obj->Assign_Target(tarcom);
			}

			/*
			**	Assign generic passenger value here. This value is used to determine
			**	if this aircraft should drop parachute reinforcements.
			*/
			if (obj->What_Am_I() == RTTI_AIRCRAFT) {
				AircraftClass * aircraft = (AircraftClass *)obj;
				if (passenger != INFANTRY_NONE) {
					aircraft->Passenger = passenger;
				}
//				if (Passenger == INFANTRY_TANYA) {
//					aircraft->Ammo = 1;
					//aircraft->AttacksRemaining = 1;
//				}
			}

			/*
			** Start the object into action.
			*/
			obj->Commence();
		} else {
			delete obj;
			sub--;
			return(sub);
		}
	}
	return(sub);
}
