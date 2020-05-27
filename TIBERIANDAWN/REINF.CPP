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

/* $Header:   F:\projects\c&c\vcs\code\reinf.cpv   2.17   16 Oct 1995 16:48:58   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
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
 *                  Last Update : July 4, 1995 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Create_Air_Reinforcement -- Creates air strike reinforcement                              * 
 *   Create_Special_Reinforcement -- Ad hoc reinforcement handler.                             * 
 *   Do_Reinforcements -- Create and place a reinforcement team.                               * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/*********************************************************************************************** 
 * Do_Reinforcements -- Create and place a reinforcement team.                                 * 
 *                                                                                             * 
 *    This routine is called when a reinforcement team must be created and placed on the map.  * 
 *    It will create all members of the team and place them at the location determined from    * 
 *    the team composition. The reinforcement team should follow team orders until overriden   * 
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
 *=============================================================================================*/
bool Do_Reinforcements(TeamTypeClass *teamtype)
{
	/*
	**	preform some preliminary checks for validity.
	*/
	if (!teamtype || !teamtype->ClassCount) return(false);

	/*
	**	Create the controlling team. All the objects are grouped
	**	under this team control. If there are no missions for this team
	**	then don't actually create the team -- it won't serve a purpose.
	*/
	TeamClass * team = NULL;
	if (teamtype->MissionCount) {
		team = new TeamClass(teamtype, HouseClass::As_Pointer(teamtype->House));
		if (!team) return(false);
		team->Force_Active();
	}

	/*
	**	Determine if this team contains its own transport. In such a case, the
	**	transport is used as a loaner. This is only true, if there is other
	**	objects to be transport. Without such cargo objects, then the transport
	**	is presumed to be the reinforcement itself and thus should not be a
	**	loaner.
	*/
	bool okvoice = true;						// Presume ok to announce reinforcement?
	bool airtransport = false;				// Transport can fly in?
	bool watertransport = false;			// Transport needs a beach to land at?
	bool onlytransport = true;				// Just transport is in reinforcement?
	bool hastransport = false;				// Group comes with transport?
	int index;
	for (index=0; index < teamtype->ClassCount; index++) {
		if (teamtype->Class[index]->IsTransporter || teamtype->Class[index]->What_Am_I() == RTTI_AIRCRAFTTYPE) {
			hastransport = true;
			if (teamtype->Class[index]->What_Am_I() == RTTI_AIRCRAFTTYPE) {
				airtransport = true;
			} else {
				watertransport = (((UnitTypeClass const *)teamtype->Class[index])->Type == UNIT_HOVER);
			}
		} else {
			onlytransport = false;
		}
	}

	/*
	**	Now determine how the reinforcement should be delivered. This is largely determined
	**	by whether there is a transport with the reinforcements.
	*/
	SourceType source = SOURCE_NONE;
	if (airtransport) {
		source = SOURCE_AIR;
	} else {

		if (watertransport) {
			source = SOURCE_BEACH;
		} else {
			
			/*
			**	Special case for the gunboat. It always arrives according to the shipping source.
			*/
			if (teamtype->Class[0]->What_Am_I() == RTTI_UNITTYPE && ((UnitTypeClass const *)teamtype->Class[0])->Type == UNIT_GUNBOAT) {
				source = SOURCE_SHIPPING;
			} else {
				source = HouseClass::As_Pointer(teamtype->House)->Edge;
			}
		}
	}

	/*
	**	If we can't determine where the reinforcement should come from, then delete it
	**	and return a failure condition.
	*/
	if (source == SOURCE_NONE) {
		if (team) delete team;
		return(false);
	}

	/*
	**	Now that the official source for the reinforcement has been determined, the 
	**	objects themselves must be created.
	*/
	TechnoClass * transport = NULL;
	TechnoClass * object = NULL;
	for (index = 0; index < teamtype->ClassCount; index++) {
		TechnoTypeClass const * tclass = teamtype->Class[index];

		for (int sub = 0; sub < teamtype->DesiredNum[index]; sub++) {
			ScenarioInit++;
			FootClass * temp = (FootClass *)tclass->Create_One_Of(HouseClass::As_Pointer(teamtype->House));
			ScenarioInit--;

			if (temp) {

				/*
				**	Add the object to the team. This is true even for the transports. The one
				**	exception is for the hover lander which never becomes part of the team.
				*/
				if (team && (temp->What_Am_I() != RTTI_UNIT || *((UnitClass*)temp) != UNIT_HOVER)) {
					ScenarioInit++;
					team->Add(temp);
					ScenarioInit--;
				}

				/*
				**	Build the list of transporters and passengers.
				*/
				if (tclass->IsTransporter && (!airtransport || tclass->What_Am_I() == RTTI_AIRCRAFTTYPE)) {

					/*
					**	Transports are considered loaners if they are transporting 
					**	something. They are presumed to only serve as a delivery
					**	agent.
					*/
					if (!onlytransport && temp->What_Am_I() != RTTI_UNIT) {
						temp->IsALoaner = true;
					}

					/*
					**	Link to the list of transports.
					*/
					temp->Next = transport;
					transport = temp;
				} else {

					/*
					**	A-10s are always considered loaners since the player should
					**	never be allowed to control them.
					*/
					if (temp->What_Am_I() == RTTI_AIRCRAFT && *((AircraftClass *)temp) == AIRCRAFT_A10) {
						temp->IsALoaner = true;
					}

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
	**	Bail on this reinforcement if no reinforcements could be created.
	**	This is probably because the object maximum was reached.
	*/
	if (!object && !transport) {
		if (team) delete team;
		return(false);
	}

	/*
	**	Now it is time to place the objects on the map. If there is a transport, then the
	**	transported objects must be placed inside the transport at this time as well.
	*/
	if (transport) {
		if (object) {

			/*
			**	For cargo planes that carry reinforcements, don't announce arrival
			**	when the transport is created. The announcement will occur when the
			**	transport unloads.
			*/
			if (transport->What_Am_I() == RTTI_AIRCRAFT && *((AircraftClass *)transport) == AIRCRAFT_CARGO) {
				okvoice = false;
			}

			transport->Attach((FootClass *)object);
		}
		object = transport;
	}

	/*
	**	Pick the location where the reinforcements appear and then place
	**	them there.
	*/
	bool placed = false;
	CELL cell = 0;
	FacingType eface;
	switch (source) {

		case SOURCE_SHIPPING:
			cell = Map.Calculated_Cell(source, teamtype->House);
			object->IsALoaner = true;
			if (object->Unlimbo(Cell_Coord(cell), DIR_W)) {
				object->Assign_Mission(MISSION_GUARD);
				object->Assign_Destination(::As_Target(XY_Cell(Map.MapCellX-1, Cell_Y(Coord_Cell(object->Coord)) )));
			} else {
				if (team) delete team;
				delete object;
				return(false);
			}
			break;

		case SOURCE_NORTH:
		case SOURCE_SOUTH:
		case SOURCE_EAST:
		case SOURCE_WEST: {
			eface = (FacingType)(source << 1);	// Facing to enter map.

			if (airtransport) ScenarioInit++;
			cell = Map.Calculated_Cell(source, teamtype->House);
			if (airtransport) ScenarioInit--;
			CELL newcell = cell;

			FootClass * o = (FootClass *)object->Next;
			object->Next = 0;
			bool ok = true;
			while (newcell > 0 && object) {
				DirType desiredfacing = Facing_Dir(eface);
				if (object->What_Am_I() == RTTI_AIRCRAFT) {
					desiredfacing = Random_Pick(DIR_N, DIR_MAX);
				}

				if (ok && object->Unlimbo(Cell_Coord(newcell), desiredfacing)) {

					/*
					**	If this object is part of a team, then the mission for this
					**	object will be guard. The team handler will assign the proper 
					**	mission that it should follow.
					*/
					if (object->What_Am_I() == RTTI_AIRCRAFT) {
						ok = false;
					} else {
						if (team) {
							object->Assign_Mission(MISSION_GUARD);
						} else {
							object->Assign_Mission(MISSION_MOVE);
							object->Assign_Destination(Adjacent_Cell(newcell, eface));
						}
						object->Commence();
					}

				} else {
					ok = true;
					
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
					newcell = -1;
				}

				object = o;
				if (object) {
					o = (FootClass *)object->Next;
					object->Next = 0;
				}
			}

			/*
			**	If there are still objects that could not be placed, then delete them.
			*/
			if (o) {
				while (o) {
					FootClass * old = o;
					o = (FootClass *)o->Next;
					old->Next = 0;

					delete old;
				}

			}
		}
		break;

		/*
		**	Bring out the aircraft as separate "groups" of one.
		*/
		case SOURCE_AIR: {
			AircraftClass * thisone = (AircraftClass *)object;
			while (thisone) {
				AircraftClass * next = (AircraftClass *)thisone->Next;

				/*
				**	Find a suitable map entry location. Cargo planes will try to find a cell that
				**	exactly lines up with the airfield they will unload at.
				*/
				CELL newcell;
				ScenarioInit++;
				newcell = Map.Calculated_Cell(HouseClass::As_Pointer(teamtype->House)->Edge, teamtype->House);
				ScenarioInit--;
				if (*thisone == AIRCRAFT_CARGO) {
					BuildingClass const * building = thisone->Find_Docking_Bay(STRUCT_AIRSTRIP, false);
					if (building) {
						newcell = XY_Cell(Map.MapCellX+Map.MapCellWidth, Coord_YCell(building->Docking_Coord()+2));
					} 
				}
				thisone->Next = 0;

				ScenarioInit++;
				placed = thisone->Unlimbo(Cell_Coord(newcell), DIR_W);
				ScenarioInit--;
				if (placed) {
					if (!team) {
						if (thisone->Class->IsFixedWing) {
							thisone->Assign_Mission(MISSION_HUNT);
						} else {
							if (thisone->Class->IsTransporter && thisone->Is_Something_Attached()) {
								thisone->Assign_Mission(MISSION_UNLOAD);
							} else {
								thisone->Assign_Mission(MISSION_MOVE);
							}
							thisone->Assign_Destination(::As_Target(Map.Calculated_Cell(source, teamtype->House)));
						}
						thisone->Commence();
					}
				} else {
					delete thisone;
				}

				thisone = next;
			}
			if (!placed && team) delete team;


			/*
			**	Fixes bug that can happen if the reinforcement cannot be created.
			**	This prevent "phantom" teams and team types from being left around.
			*/
			if (GameToPlay == GAME_NORMAL && !placed) return(false);

		}
		break;

		case SOURCE_OCEAN:
		case SOURCE_BEACH:
			cell = Map.Calculated_Cell(SOURCE_BEACH, teamtype->House);
			if (cell) {
				CELL edge = XY_Cell(Cell_X(cell), Map.MapCellY+Map.MapCellHeight);

				placed = object->Unlimbo(Cell_Coord(edge), DIR_N);
				if (placed) {
					if (!team) {
						object->IsLocked = false;
						object->Assign_Mission(MISSION_UNLOAD);
						object->Commence();
						object->Assign_Destination(::As_Target(cell));
					}
				} else {
					if (team) delete team;
					delete object;
					return(false);
				}
			}
			break;

		default:
			break;
	}

	/*
	**	Announce when the reinforcements have arrived.
	*/
	if (okvoice && teamtype->House == PlayerPtr->Class->House) {
		Speak(VOX_REINFORCEMENTS, NULL, cell ? Cell_Coord(cell) : 0);
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
	if (house && type) {
		TeamTypeClass * team = new TeamTypeClass();

		if (team) {

			/*
			**	If a ground based reinforcement is desired, but the edge of the map that the
			**	reinforcement will arrive at is completely covered with water, then add
			**	a hover lander for transport.
			*/
			if (!another && (type->What_Am_I() == RTTI_UNITTYPE || type->What_Am_I() == RTTI_INFANTRYTYPE)) {

				/*
				**	Hover lander reinforcements can only arrive from the south. Yes, this is an
				**	arbitrary limitation, but that's the way it is (for now).
				*/
				if (house->Edge == SOURCE_SOUTH) {
					bool found = false;
					for (int index = Map.MapCellX; index < Map.MapCellX+Map.MapCellWidth-1; index++) {
						CELL cell = XY_Cell(index, Map.MapCellY+Map.MapCellHeight);
						if (Map[cell].Is_Generally_Clear() && Map[cell-MAP_CELL_W].Is_Generally_Clear()) {
							found = true;
							break;
						}
					}

					/*
					**	No land route was found for the reinforcement to drive itself onto the
					**	map. Assign it a hover lander. This presumes that if the south edge is
					**	non drivable, it will have water there instead. Risky, but is not a 
					**	problem with the current C&C maps.
					*/
					if (!found) {
						mission = TMISSION_NONE;
						another = &UnitTypeClass::As_Reference(UNIT_HOVER);
					}
				}

				if (!another) {
					mission = TMISSION_MOVECELL;
					argument = Map.Calculated_Cell(SOURCE_AIR, house->Class->House);
				}
			}

			/*
			**	If there is no overridden mission assign to this special reinforcement, then
			**	we must assign something. If not, the reinforcement will just sit at the edge
			**	of the map.
			*/
			if (!another && mission == TMISSION_NONE) {
				mission = TMISSION_MOVECELL;
				argument = Map.Calculated_Cell(SOURCE_AIR, house->Class->House);
			}

			/*
			**	Fill in the team characteristics.
			*/
			strcpy((char *)&team->IniName[0], "TEMP");
			team->IsReinforcable = false;
			team->IsTransient = true;
			team->ClassCount = 1;
			team->Class[0] = type;
			team->DesiredNum[0] = 1;
			team->MissionCount = 1;
			if (mission == TMISSION_NONE) {
				if (another && (another->What_Am_I() != RTTI_UNITTYPE || ((UnitTypeClass const *)another)->Type != UNIT_HOVER)) {
					team->MissionList[0].Mission	= TMISSION_UNLOAD;
					team->MissionList[0].Argument = WAYPT_REINF;
				}
			} else {
				team->MissionList[0].Mission	= mission;
				team->MissionList[0].Argument = argument;
			}
			team->House = house->Class->House;
			if (another) {
				team->ClassCount++;
				team->Class[1] = another;
				team->DesiredNum[1] = 1;
			}

			bool ok = Do_Reinforcements(team);
			if (!ok && GameToPlay == GAME_NORMAL) {
				delete team;
			}
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
 * INPUT:   house    -- The purpetrator of this air strike.                                    * 
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
int Create_Air_Reinforcement(HouseClass *house, AircraftType air, int number, MissionType mission, TARGET tarcom, TARGET navcom)
{
	/*
	** Get a pointer to the class of the object that we are going to create.
	*/
	TechnoTypeClass const * type = (TechnoTypeClass *)&AircraftTypeClass::As_Reference(air);

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
		SourceType source = house->Edge;
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
		CELL newcell = Map.Calculated_Cell(source, house->Class->House);

		/* 
		** Try and place the object onto the map.
		*/
		ScenarioInit++;
		int placed = obj->Unlimbo(Cell_Coord(newcell), DIR_N);
		ScenarioInit--;
		if (placed) {

			/*
			** If we suceeded in placing the obj onto the map then
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
