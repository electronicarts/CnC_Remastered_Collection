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

/* $Header: /CounterStrike/FOOT.CPP 2     3/06/97 1:46p Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : FOOT.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 22, 1994                                               *
 *                                                                                             *
 *                  Last Update : October 5, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   FootClass::AI -- Handle general movement AI.                                              *
 *   FootClass::Active_Click_With -- Initiates attack or move according to target clicked on.  *
 *   FootClass::Active_Click_With -- Performs action as a result of left mouse click.          *
 *   FootClass::Adjust_Dest -- Adjust candidate movement cell to account for formation.        *
 *   FootClass::Approach_Target -- Sets the navigation computer to approach target object.     *
 *   FootClass::Assign_Destination -- Assigns specified destination to NavCom.                 *
 *   FootClass::Basic_Path -- Finds the basic path for a ground object.                        *
 *   FootClass::Body_Facing -- Set the body rotation/facing.                                   *
 *   FootClass::Can_Demolish -- Checks to see if this object can be sold back.                 *
 *   FootClass::Can_Enter_Cell -- Checks to see if the object can enter cell specified.        *
 *   FootClass::Clear_Navigation_List -- Clears out the navigation queue.                      *
 *   FootClass::Death_Announcement -- Announces the death of a unit.                           *
 *   FootClass::Debug_Dump -- Displays the status of the FootClass to the mono monitor.        *
 *   FootClass::Detach -- Detaches a target from tracking systems.                             *
 *   FootClass::Detach_All -- Removes this object from the game system.                        *
 *   FootClass::Enters_Building -- When unit enters a building for some reason.                *
 *   FootClass::FootClass -- Normal constructor for the foot class object.                     *
 *   FootClass::Greatest_Threat -- Fetches the greatest threat to this object.                 *
 *   FootClass::Handle_Navigation_List -- Processes the navigation queue.                      *
 *   FootClass::Is_Allowed_To_Leave_Map -- Checks to see if it can leave the map and the game. *
 *   FootClass::Is_On_Priority_Mission -- Checks to see if this object should be given priority*
 *   FootClass::Is_Recruitable -- Determine if this object is recruitable as a team members.   *
 *   FootClass::Likely_Coord -- Fetches the coordinate the object will be at shortly.          *
 *   FootClass::Mark -- Unit interface to map rendering system.                                *
 *   FootClass::Mission_Attack -- AI for heading towards and firing upon target.               *
 *   FootClass::Mission_Capture -- Handles the capture mission.                                *
 *   FootClass::Mission_Enter -- Enter (cooperatively) mission handler.                        *
 *   FootClass::Mission_Guard_Area -- Causes unit to guard an area about twice weapon range.   *
 *   FootClass::Mission_Hunt -- Handles the default hunt order.                                *
 *   FootClass::Mission_Move -- AI process for moving a vehicle to its destination.            *
 *   FootClass::Mission_Retreat -- Handle reatreat from map mission for mobile objects.        *
 *   FootClass::Offload_Tiberium_Bail -- Fetches the Tiberium to offload per step.             *
 *   FootClass::Override_Mission -- temporarily overrides a units mission                      *
 *   FootClass::Per_Cell_Process -- Perform action based on once-per-cell condition.           *
 *   FootClass::Queue_Navigation_List -- Add a target to the objects navigation list.          *
 *   FootClass::Receive_Message -- Movement related radio messages are handled here.           *
 *   FootClass::Rescue_Mission -- Calls this unit to the rescue.                               *
 *   FootClass::Restore_Mission -- Restores an overridden mission                              *
 *   FootClass::Sell_Back -- Causes this object to be sold back.                               *
 *   FootClass::Set_Speed -- Initiate unit movement physics.                                   *
 *   FootClass::Sort_Y -- Determine the sort coordinate for foot class objects.                *
 *   FootClass::Start_Driver -- This starts the driver heading to the destination desired.     *
 *   FootClass::Stop_Driver -- This routine clears the driving state of the object.            *
 *   FootClass::Stun -- Prepares a ground travelling object for removal.                       *
 *   FootClass::Take_Damage -- Handles taking damage to this object.                           *
 *   FootClass::Unlimbo -- Unlimbos object and performs special fixups.                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***********************************************************************************************
 * FootClass::FootClass -- Default constructor for foot class objects.                         *
 *                                                                                             *
 *    This is the default constructor for FootClass objects. It sets the foot class values to  *
 *    their default starting settings.                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
FootClass::FootClass(RTTIType rtti, int id, HousesType house) :
	TechnoClass(rtti, id, house),
	IsScanLimited(false),
	IsInitiated(false),
	IsNewNavCom(false),
	IsPlanningToLook(false),
	IsDeploying(false),
	IsFiring(false),
	IsRotating(false),
	IsDriving(false),
	IsUnloading(false),
	IsFormationMove(false),
	IsNavQueueLoop(false),
	IsScattering(false),
	IsMovingOntoBridge(false),
	Speed(0),
	SpeedBias(1),
	XFormOffset(0x80000000),
	YFormOffset(0x80000000),
	NavCom(TARGET_NONE),
	SuspendedNavCom(TARGET_NONE),
	Team(0),
	Group(255),
	Member(0),
	PathThreshhold(MOVE_CLOAK),
	PathDelay(0),
	TryTryAgain(PATH_RETRY),
	BaseAttackTimer(0),
	FormationSpeed(SPEED_FOOT),
	FormationMaxSpeed(MPH_IMMOBILE),
	HeadToCoord(0)
{
	Path[0] = FACING_NONE;
	for (int index = 0; index < ARRAY_SIZE(NavQueue); index++) {
		NavQueue[index] = TARGET_NONE;
	}
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * FootClass::Debug_Dump -- Displays the status of the FootClass to the mono monitor.          *
 *                                                                                             *
 *    This routine is used to output the current status of the foot class to the mono          *
 *    monitor. Through this display bugs may be tracked down or eliminated.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *   07/04/1995 JLB : Handles aircraft special case.                                           *
 *=============================================================================================*/
void FootClass::Debug_Dump(MonoClass * mono) const
{
	assert(IsActive);

	mono->Fill_Attrib(53, 13, 12, 1, IsInitiated ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 18, 12, 1, IsPlanningToLook ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(53, 14, 12, 1, IsDeploying ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(53, 15, 12, 1, IsFiring ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(53, 16, 12, 1, IsRotating ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(53, 17, 12, 1, IsDriving ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(53, 18, 12, 1, IsUnloading ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 18, 12, 1, IsFormationMove ? MonoClass::INVERSE : MonoClass::NORMAL);

	mono->Set_Cursor(45, 1);mono->Printf("%02X", Speed);
	if (NavCom) {
		mono->Set_Cursor(29, 5);
		mono->Printf("%08X", NavCom);
	}
	if (SuspendedNavCom) {
		mono->Set_Cursor(38, 5);
		mono->Printf("%08X", SuspendedNavCom);
	}

	if (Team) Team->Debug_Dump(mono);
	if (Group != 255) {
		mono->Set_Cursor(59, 1);mono->Printf("%d", Group);
	}

	static char	const * _p2c[9] = {"-","0","1","2","3","4","5","6","7"};
	for (int index = 0; index < min(12, ARRAY_SIZE(Path)); index++) {
		mono->Set_Cursor(54+index, 3);
		mono->Printf("%s", _p2c[((ABS((int)Path[index]+1)) % ARRAY_SIZE(_p2c))]);
	}
	mono->Set_Cursor(54, 5);mono->Printf("%2d", PathThreshhold);
	mono->Set_Cursor(72, 3);mono->Printf("%4d", (long)PathDelay);
	mono->Set_Cursor(67, 3);mono->Printf("%3d", TryTryAgain);
	if (HeadToCoord) {
		mono->Set_Cursor(60, 5);mono->Printf("%08X", HeadToCoord);
	}

	TechnoClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * FootClass::Set_Speed -- Initiate unit movement physics.                                     *
 *                                                                                             *
 *    This routine is used to set a unit's velocity control structure.                         *
 *    The game will then process the unit's movement during the momentum                       *
 *    physics calculation.                                                                     *
 *                                                                                             *
 * INPUT:   unit  -- Pointer to the unit to alter.                                             *
 *                                                                                             *
 *          speed -- Throttle setting (0=stop, 255=full throttle).                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/07/1992 JLB : Created.                                                                 *
 *   09/24/1993 JLB : Revised for faster speed.                                                *
 *   04/02/1994 JLB : Revised for new system.                                                  *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *   07/21/1994 JLB : Simplified.                                                              *
 *=============================================================================================*/
void FootClass::Set_Speed(int speed)
{
	assert(IsActive);

	speed &= 0xFF;
	((unsigned char &)Speed) = speed;
}


/***********************************************************************************************
 * FootClass::Mark -- Unit interface to map rendering system.                                  *
 *                                                                                             *
 *    This routine is the interface function for units as they relate to                       *
 *    the map rendering system. Whenever a unit's imagery changes, this                        *
 *    function is called.                                                                      *
 *                                                                                             *
 * INPUT:   mark  -- Type of image change (MARK_UP, _DOWN, _CHANGE)                            *
 *             MARK_UP  -- Unit is removed.                                                    *
 *             MARK_CHANGE -- Unit alters image but doesn't move.                              *
 *             MARK_DOWN -- Unit is overlaid onto existing icons.                              *
 *                                                                                             *
 * OUTPUT:  bool; Did the marking operation succeed? Failure could be the result of marking    *
 *                down when it is already down, or visa versa.                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/14/1991 JLB : Created.                                                                 *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *   12/23/1994 JLB : Performs low level check before processing.                              *
 *=============================================================================================*/
bool FootClass::Mark(MarkType mark)
{
	assert(this != 0);
	assert(IsActive);

	if (TechnoClass::Mark(mark)) {
//		short list[32];
		CELL cell = Coord_Cell(Coord);

#ifndef PARTIAL
		if (In_Which_Layer() != LAYER_GROUND && (mark == MARK_UP || mark == MARK_DOWN)) mark = MARK_CHANGE;
#endif

		/*
		**	Inform the map of the refresh, occupation, and overlap
		**	request.
		*/
		switch (mark) {
			case MARK_UP:
				Map.Pick_Up(cell, this);
				break;

			case MARK_DOWN:
				Map.Place_Down(cell, this);
				break;

			case MARK_CHANGE_REDRAW:
				Map.Refresh_Cells(cell, Overlap_List(true));
				Map.Refresh_Cells(cell, Occupy_List());
				break;

			default:
				Map.Refresh_Cells(cell, Overlap_List());
				Map.Refresh_Cells(cell, Occupy_List());
				break;
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * FootClass::Basic_Path -- Finds the basic path for a ground object.                          *
 *                                                                                             *
 *    This is a common routine used by both infantry and other ground travelling units. It     *
 *    will fill in the unit's basic path to the NavCom destination.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was a path found? A failure to find a path means either the target cannot    *
 *                be found or the terrain prohibits the unit's movement.                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Basic_Path(void)
{
	assert(IsActive);

	PathType		* path;			// Pointer to path control structure.
	CELL			cell;
	int 			skip_path = false;

	Path[0] = FACING_NONE;

	if (Target_Legal(NavCom)) {
		cell = As_Cell(NavCom);

		/*
		**	When the navigation computer is set to a location that is impassible, then
		**	find a nearby cell that can be entered and try to head toward that instead.
		**	EXCEPT when that cell is very close -- then just bail.
		*/
		int dist = Distance(NavCom);
		int checkdist = Team.Is_Valid() ? Rule.StrayDistance : Rule.CloseEnoughDistance;
		if (Can_Enter_Cell(cell) > MOVE_CLOAK && dist > checkdist) {
			CELL cell2 = Map.Nearby_Location(cell, Techno_Type_Class()->Speed, Map[Coord].Zones[Techno_Type_Class()->MZone], Techno_Type_Class()->MZone);
			if (cell2 != 0 && ::Distance(Cell_Coord(cell), Cell_Coord(cell2)) < dist) cell = cell2;
		}

		if (What_Am_I() == RTTI_INFANTRY) {
			CELL mycell = Coord_Cell(Center_Coord());
			ObjectClass * obj = Map[mycell].Cell_Occupier();
			while (obj) {
				if (obj != this && obj->What_Am_I() == RTTI_INFANTRY) {
					InfantryClass * inf = (InfantryClass *)obj;
					if (inf->NavCom == NavCom && inf->Path[0] != FACING_NONE) {
						if (Coord_Cell(inf->Head_To_Coord()) == Coord_Cell(inf->Coord)) {
							Mem_Copy(&inf->Path[1], Path, sizeof(Path)-sizeof(Path[0]));
						} else {
							Mem_Copy(inf->Path, Path, sizeof(Path));
						}
						if (Path[0] != FACING_NONE) {
							skip_path = true;
						}
						break;
					}
				}
				obj = obj->Next;
			}
		}

		if (!skip_path) {
			Mark(MARK_UP);
			Path[0] = FACING_NONE;		// Probably not necessary, but...

			/*
			**	Try to find a path to the destination. If a failure occurs, then keep trying
			**	with greater determination until either a complete failure occurs, or a decent
			**	path was found.
			*/
			bool			found1=false;		// Found a best path yet?
			PathType	   path1;
			FacingType	workpath1[200];	// Staging area for path list.
//			FacingType	workpath2[200];	// Staging area for path list.
			MoveType		maxtype = MOVE_TEMP;
			if (!House->IsHuman) {
				maxtype = MOVE_TEMP;
//				maxtype = MOVE_DESTROYABLE;
			} else {

				/*
				**	For simple movement missions by the human player, then don't
				**	consider friendly units as passable if close to the destination.
				**	This will prevent a human controlled unit from just sitting next
				**	to a destination just because there is another friendly unit
				**	occupying the destination location.
				*/
				if (Mission == MISSION_MOVE && Distance(NavCom) < Rule.CloseEnoughDistance) {
					maxtype = MOVE_DESTROYABLE;
				}
			}

			/*
			**	Try to find a path to the destination. If there is a path
			**	failure, then try a more severe path method until the
			**	maximum severity is reached.
			*/
			for (;;) {
				path = Find_Path(cell, &workpath1[0], sizeof(workpath1), PathThreshhold);
				if (path && path->Cost) {
					memcpy(&path1, path, sizeof(path1));
					found1 = true;
					break;
				}

				/*
				**	A valid path was not found. Try the next greater path severity
				**	level if the severity can be increased. If not, then consider this
				**	a total failure.
				*/
				PathThreshhold++;
				if (PathThreshhold > maxtype) break;
			}

#ifdef NEVER
			/*
			**	Determine if ANY path could be calculated by first examining the most
			**	aggressive case. If this fails, then no path will succeed. Further
			**	scanning is unnecessary.
			*/
			path = Find_Path(cell, &workpath1[0], sizeof(workpath1), maxtype);
			if (path && path->Cost) {
				memcpy(&path1, path, sizeof(path1));
				found1 = true;

				/*
				**	Scan for the best path possible. If this succeeds, then do a simple
				**	comparison with the most aggressive path. If they are very close, then
				**	go with the best (easiest) path method.
				*/
				path = Find_Path(cell, &workpath2[0], sizeof(workpath2), MOVE_CLOAK);
				if (path && path->Cost && path->Cost < max((path1.Cost + (path1.Cost/2)), 3)) {
					memcpy(&path1, path, sizeof(path1));
					memcpy(workpath1, workpath2, sizeof(workpath1));
				} else {

					/*
					**	The easiest path method didn't result in a satisfactory path. Scan through
					**	the rest of the path options, looking for the best one.
					*/
					for (MoveType move = (MoveType)(MOVE_CLOAK+1); move < (MoveType)(maxtype-1); move++) {
//					for (MoveType move = MOVE_MOVING_BLOCK; move < maxtype-1; move++) {
						path = Find_Path(cell, &workpath2[0], sizeof(workpath2), move);
						if (path && path->Cost && path->Cost < max((path1.Cost + (path1.Cost/2)), 3)) {
							memcpy(&path1, path, sizeof(path1));
							memcpy(workpath1, workpath2, sizeof(workpath1));
						}
					}
				}
			}
#endif

			/*
			**	If a good path was found, then record it in the object's path
			**	list.
			*/
			if (found1) {
				Fixup_Path(&path1);
				memcpy(&Path[0], &workpath1[0], min(path->Length, (int)sizeof(Path)));
			}

			Mark(MARK_DOWN);
		}

		PathDelay = Rule.PathDelay * TICKS_PER_MINUTE;
		if (Path[0] != FACING_NONE) return(true);

		/*
		**	If a basic path couldn't be determined, then abort the navigation process.
		*/
		Stop_Driver();
	}
	return(false);
}


/***********************************************************************************************
 * FootClass::Mission_Move -- AI process for moving a vehicle to its destination.              *
 *                                                                                             *
 *    This simple AI script handles moving the vehicle to its desired destination. Since       *
 *    simple movement is handled directly by the engine, this routine merely waits until       *
 *    the unit has reached its destination, and then causes the unit to enter idle mode.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before calling this routine again.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *   10/02/1996 JLB : Player controlled or human owned units don't scan for targets.           *
 *=============================================================================================*/
int FootClass::Mission_Move(void)
{
	assert(IsActive);

	if (!Target_Legal(NavCom) && !IsDriving && MissionQueue == MISSION_NONE) {
		Enter_Idle_Mode();
		return(1);
	}
//	if (!Target_Legal(TarCom) && !House->IsPlayerControl && !House->IsHuman) {
	if (!Target_Legal(TarCom) && !House->IsPlayerControl && !House->IsHuman && (!Team.Is_Valid() || !Team->Class->IsSuicide)) {
		Target_Something_Nearby(THREAT_RANGE);
	}
	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
}


/***********************************************************************************************
 * FootClass::Mission_Capture -- Handles the capture mission.                                  *
 *                                                                                             *
 *    Capture missions are nearly the same as normal movement missions. The only difference    *
 *    is that the final destination is handled in a special way so that it is not marked as    *
 *    impassable. This allows the object (usually infantry) the ability to walk onto the       *
 *    object and thus capture it.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game ticks to delay before calling this routine.        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int FootClass::Mission_Capture(void)
{
	assert(IsActive);

	/*
	**	If there is a valid TarCom but the NavCom isn't set, then set the NavCom accordingly.
	*/
	if (Is_Target_Building(TarCom) && !Target_Legal(NavCom) && What_Am_I() == RTTI_INFANTRY && ((InfantryClass *)this)->Class->IsBomber) {
		Assign_Destination(TarCom);
	}

	if (!Target_Legal(NavCom) /*&& !In_Radio_Contact()*/) {
		Enter_Idle_Mode();
		if (Map[Center_Coord()].Cell_Building()) {
			Scatter(0, true);
		}
	}
	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
}


/***********************************************************************************************
 * FootClass::Mission_Attack -- AI for heading towards and firing upon target.                 *
 *                                                                                             *
 *    This AI routine handles heading to within range of the target and then firing upon       *
 *    it until it is destroyed. If the target is destroyed, then the unit will change          *
 *    missions to match its "idle mode" of operation (usually guarding).                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before calling this routine again.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int FootClass::Mission_Attack(void)
{
	assert(IsActive);
	if (Target_Legal(TarCom)) {
		Approach_Target();
	} else {
		Enter_Idle_Mode();
	}
	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
}


/***********************************************************************************************
 * FootClass::Mission_Guard -- Handles the AI for guarding in place.                           *
 *                                                                                             *
 *    Units that are performing stationary guard duty use this AI process. They will sit       *
 *    still and target any enemies that get within range.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before calling this routine again.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int FootClass::Mission_Guard(void)
{
	assert(IsActive);

	/*
	**	If this unit is on an impassable cell for any reason, it needs to scatter immediately
	*/
	if (What_Am_I() == RTTI_INFANTRY || What_Am_I() == RTTI_UNIT) {
		LandType land = Map[Coord].Land_Type();
		if (!Target_Legal(NavCom) && (land == LAND_ROCK || land == LAND_WATER || land == LAND_RIVER)) {
			Scatter(0, true, true);
			Shorten_Mission_Timer();
		}
	}

	if (!Target_Something_Nearby(THREAT_RANGE)) {
		Random_Animate();
	}

	int dtime = MissionControl[Mission].Normal_Delay();
	if (What_Am_I() == RTTI_VESSEL) {
		switch (((VesselClass *)this)->Class->Type) {
			case VESSEL_DD:
			case VESSEL_PT:
				dtime = MissionControl[Mission].AA_Delay();
				break;

			case VESSEL_CA:
				dtime *= 2;
				break;

			default:
				break;
		}
	}
	if (What_Am_I() == RTTI_INFANTRY) {

		/*
		**	If this is a bomber type infantry and the current target is a building, then go into
		**	sabotage mode if not already.
		*/
		if (!House->IsHuman && Is_Target_Building(TarCom) && ((InfantryClass *)this)->Class->IsBomber && Mission != MISSION_SABOTAGE) {
			Assign_Mission(MISSION_SABOTAGE);
		}

		switch (((InfantryClass *)this)->Class->Type) {
			case INFANTRY_E1:
			case INFANTRY_E3:
				dtime = MissionControl[Mission].AA_Delay();
				break;

			default:
				break;
		}
	}

	return((Arm != 0) ? (int)Arm : (dtime+Random_Pick(0, 2)));
}


/***********************************************************************************************
 * FootClass::Mission_Hunt -- Handles the default hunt order.                                  *
 *                                                                                             *
 *    This routine is the default hunt order for game objects. It handles searching for a      *
 *    nearby object and heading toward it. The act of targeting will cause it to attack        *
 *    the target it selects.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the game tick delay before calling this routine again.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int FootClass::Mission_Hunt(void)
{
	assert(IsActive);
	if (!Target_Something_Nearby(THREAT_NORMAL)) {
#if(0)
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (What_Am_I() == RTTI_INFANTRY && *(InfantryClass *)this == INFANTRY_GENERAL && House->Class->House==HOUSE_UKRAINE && Scen.Scenario==47) {
		for(int index=0; index < Buildings.Count(); index++) {
			if(Buildings.Ptr(index)->IsOwnedByPlayer) {
				Assign_Target(Buildings.Ptr(index)->As_Target());
				break;
			}
		}
		for(index=0; index < Units.Count(); index++) {
			if(Units.Ptr(index)->IsOwnedByPlayer) {
				Assign_Target(Units.Ptr(index)->As_Target());
				break;
			}
		}
		for(index=0; index < Infantry.Count(); index++) {
			if(Infantry.Ptr(index)->IsOwnedByPlayer) {
				Assign_Target(Infantry.Ptr(index)->As_Target());
				break;
			}
		}
		for(index=0; index < Aircraft.Count(); index++) {
			if(Aircraft.Ptr(index)->IsOwnedByPlayer) {
				Assign_Target(Aircraft.Ptr(index)->As_Target());
				break;
			}
		}
	}
#endif
#endif
		Random_Animate();
	} else {
		if (What_Am_I() == RTTI_INFANTRY && ( ((InfantryTypeClass const &)Class_Of()).Type == INFANTRY_RENOVATOR || ((InfantryTypeClass const &)Class_Of()).Type == INFANTRY_THIEF) ) {
			Assign_Destination(TarCom);
			Assign_Mission(MISSION_CAPTURE);
		} else {
			if (What_Am_I() == RTTI_INFANTRY && ((InfantryClass *)this)->Class->IsBomber && Is_Target_Building(TarCom)) {
				Assign_Destination(TarCom);
				Assign_Mission(MISSION_SABOTAGE);
			} else {
				Approach_Target();
			}
		}
	}
	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
}


/***********************************************************************************************
 * FootClass::Stop_Driver -- This routine clears the driving state of the object.              *
 *                                                                                             *
 *    This is the counterpart routine to the Start_Driver function. It clears the driving      *
 *    status flags and destination coordinate record.                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was driving stopped?                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   12/12/1994 JLB : Greatly simplified.                                                      *
 *=============================================================================================*/
bool FootClass::Stop_Driver(void)
{
	assert(IsActive);

	if (HeadToCoord) {
		HeadToCoord = NULL;
		Set_Speed(0);
		IsDriving = false;
		IsMovingOntoBridge = false;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * FootClass::Start_Driver -- This starts the driver heading to the destination desired.       *
 *                                                                                             *
 *    Before a unit can move it must be started by this routine. This routine handles          *
 *    reserving the cell and setting the driving flag.                                         *
 *                                                                                             *
 * INPUT:   headto   -- The coordinate of the immediate drive destination. This is one cell    *
 *                      away from the unit's current location.                                 *
 *                                                                                             *
 * OUTPUT:  bool; Was driving initiated?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   12/12/1994 JLB : Uses simple spot index finder.                                           *
 *=============================================================================================*/
bool FootClass::Start_Driver(COORDINATE &headto)
{
	assert(IsActive);

	Stop_Driver();
	if (headto) {
		HeadToCoord = headto;
		IsDriving = true;

		CellClass * cellptr = &Map[headto];
		TemplateType ttype = cellptr->TType;
		IsMovingOntoBridge = (ttype >= TEMPLATE_BRIDGE1 && ttype <= TEMPLATE_BRIDGE2D) || (ttype >= TEMPLATE_BRIDGE_1A && ttype <= TEMPLATE_BRIDGE_3F);

		/*
		**	Check for crate goodie finder here.
		*/
		if (Map[headto].Goodie_Check(this)) {
			return(true);
		}
		if (!IsActive) return(false);

		HeadToCoord = NULL;
		IsDriving = false;
	}
	return(false);
}


/***********************************************************************************************
 * FootClass::Sort_Y -- Determine the sort coordinate for foot class objects.                  *
 *                                                                                             *
 *    This routine will determine the sort coordinate for foot class object. This coordinate   *
 *    is usually the coordinate of the object. The exception is if the object is tethered.     *
 *    In this case (presumes offloading to the north), the sorting coordinate is adjusted      *
 *    so that the object will be drawn on top of the transport unit.                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate to use for sorting.                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   11/04/1994 JLB : Sort value is different when unloading from aircraft.                    *
 *=============================================================================================*/
COORDINATE FootClass::Sort_Y(void) const
{
	assert(IsActive);

	if (IsUnloading) {
		return(Coord_Add(Coord, 0x01000000L));
	}
	if (In_Radio_Contact() && IsTethered && Contact_With_Whom()->What_Am_I() == RTTI_UNIT) {
		return(Coord_Add(Coord, 0x01000000L));
	}
	return(Coord_Add(Coord, 0x00300000L));
}


/***********************************************************************************************
 * FootClass::Stun -- Prepares a ground travelling object for removal.                         *
 *                                                                                             *
 *    This routine clears the units' navigation computer in preparation for removal from the   *
 *    game. This is probably called as a result of unit destruction in combat. Clearing the    *
 *    navigation computer ensures that the normal AI process won't start it moving again while *
 *    the object is undergoing any death animations.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Stun(void)
{
	assert(IsActive);

	Assign_Destination(TARGET_NONE);
	Path[0] = FACING_NONE;
	Stop_Driver();
	TechnoClass::Stun();
}


/***********************************************************************************************
 * FootClass::Approach_Target -- Sets the navigation computer to approach target object.       *
 *                                                                                             *
 *    This routine will set the navigation computer to approach the target indicated by the    *
 *    targeting computer. It is through this function that the unit nears the target so        *
 *    that weapon firing may occur.                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   12/13/1994 JLB : Made part of TechnoClass.                                                *
 *   12/22/1994 JLB : Enhanced search algorithm.                                               *
 *   05/20/1995 JLB : Always approaches if the object is off the map.                          *
 *=============================================================================================*/
void FootClass::Approach_Target(void)
{
	assert(IsActive);

	/*
	**	Determine that if there is an existing target it is still legal
	**	and within range.
	*/
	if (Target_Legal(TarCom)) {
		int primary = What_Weapon_Should_I_Use(TarCom);

		/*
		**	If the target is too far away then head toward it.
		*/
		int maxrange = Weapon_Range(primary);
//		int maxrange = max(Weapon_Range(0), Weapon_Range(1));

		if (!Target_Legal(NavCom) && (!In_Range(TarCom, primary) || !IsLocked)) {
//		if (!Target_Legal(NavCom) && (Distance(TarCom) > maxrange || !IsLocked)) {

			/*
			** If the object that we are attacking is a building adjust the unit's
			** max range so that people can stand far away from the buildings and
			** hit them.
			*/
			BuildingClass * obj = As_Building(TarCom);
			if (obj) {
				maxrange += ((obj->Class->Width() + obj->Class->Height()) * (0x100 / 4));
			}

			/*
			** Adjust the max range of an infantry unit for where he is standing
			** in the room.
			*/
			maxrange -= 0x00B7;
#ifdef OBSOLETE
			if (What_Am_I() == RTTI_INFANTRY) {
				maxrange -= 0x0111;
			} else {
				maxrange -= 0x00B7;
			}
#endif
			maxrange = max(maxrange, 0);

			COORDINATE tcoord = ::As_Coord(TarCom);
			COORDINATE trycoord = 0;
			CELL tcell = Coord_Cell(tcoord);
			CELL trycell = tcell;
			DirType dir = Direction256(tcoord, Center_Coord());
			bool found = false;

			/*
			**	Sweep through the cells between the target and the unit, looking for
			**	a cell that the unit can enter but which is also within weapon range
			**	of the target. If after a reasonable search, no appropriate cell could
			**	be found, then the target will be assigned as the movement destination
			**	and let "the chips fall where they may."
			*/
			for (int range = maxrange; range > 0x0080; range -= 0x0100) {
				static int _angles[] = {0, 8, -8, 16, -16, 24, -24, 32, -32, 48, -48, 64, -64};

				for (int index = 0; index < (sizeof(_angles)/sizeof(_angles[0])); index++) {
					trycoord = Coord_Move(tcoord, (DirType)(dir + _angles[index]), range);

					if (::Distance(trycoord, tcoord) < range) {
						trycell = Coord_Cell(trycoord);
						if (Map.In_Radar(trycell) && Map[trycell].Is_Clear_To_Move(Techno_Type_Class()->Speed, false, false, Map[Coord].Zones[Techno_Type_Class()->MZone], Techno_Type_Class()->MZone)) {
//						if (Can_Enter_Cell(trycell) <= MOVE_CLOAK && Map.In_Radar(trycell)) {
							found = true;
							break;
						}
					}
				}
				if (found) break;
			}

			/*
			**	If a suitable intermediate location was found, then head toward it.
			**	Otherwise, head toward the enemy unit directly.
			*/
			if (found) {
				Assign_Destination(::As_Target(trycell));
			} else {

				trycell = Map.Nearby_Location(trycell, Techno_Type_Class()->Speed, Map[Coord].Zones[Techno_Type_Class()->MZone], Techno_Type_Class()->MZone);
				Assign_Destination(::As_Target(trycell));
//				Assign_Destination(TarCom);
			}
		}
	}
}


/***********************************************************************************************
 * FootClass::Mission_Guard_Area -- Causes unit to guard an area about twice weapon range.     *
 *                                                                                             *
 *    This mission routine causes the unit to scan for targets out to twice its weapon range   *
 *    from the home point. If a target was found, then it will be attacked. The unit will      *
 *    chase the target until it gets up to to its weapon range from the home position.         *
 *    In that case, it will return to home position and start scanning for another target.     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with time delay before calling this routine again.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/23/1994 JLB : Created.                                                                 *
 *   07/27/1995 JLB : Greatly simplified.                                                      *
 *=============================================================================================*/
int FootClass::Mission_Guard_Area(void)
{
	assert(IsActive);

	/*
	**	If this unit is on an impassable cell for any reason, it needs to scatter immediately
	*/
	if (What_Am_I() == RTTI_INFANTRY || What_Am_I() == RTTI_UNIT) {
		LandType land = Map[Coord].Land_Type();
		if (!Target_Legal(NavCom) && (land == LAND_ROCK || land == LAND_WATER || land == LAND_RIVER)) {
			Scatter(0, true, true);
			Shorten_Mission_Timer();
		}
	}

	if (What_Am_I() == RTTI_UNIT && ((UnitClass *)this)->Class->IsToHarvest) {
		Assign_Mission(MISSION_HARVEST);
		return(1+Random_Pick(1, 10));
	}

	/*
	**	Ensure that the archive target is valid.
	*/
	if (!Target_Legal(ArchiveTarget)) {
		ArchiveTarget = ::As_Target(Coord);
	}

	/*
	**	If this is a bomber type infantry and the current target is a building, then go into
	**	sabotage mode if not already.
	*/
	if (!House->IsHuman && What_Am_I() ==  RTTI_INFANTRY && Is_Target_Building(TarCom) && ((InfantryClass *)this)->Class->IsBomber && Mission != MISSION_SABOTAGE) {
		Assign_Mission(MISSION_SABOTAGE);
		return(1);
	}

	/*
	**	Make sure that the unit has not strayed too far from the home position.
	**	If it has, then race back to it.
	*/
	int maxrange = Threat_Range(1)/2;

	if (!IsFiring && !Target_Legal(NavCom) && Distance(ArchiveTarget) > maxrange) {
		Assign_Target(TARGET_NONE);
		Assign_Destination(ArchiveTarget);
	}

	if (!Target_Legal(TarCom)) {
		COORDINATE old = Coord;
		Coord = As_Coord(ArchiveTarget);
		Target_Something_Nearby(THREAT_AREA);
		Coord = old;
		if (Target_Legal(TarCom)) {
			return(1);
		}
		Random_Animate();
	} else {
		Approach_Target();
	}

	int dtime = MissionControl[Mission].Normal_Delay();
	if (What_Am_I() == RTTI_AIRCRAFT) {
		dtime *= 2;
	}
	return(dtime + Random_Pick(1, 5));
}


/***********************************************************************************************
 * FootClass::Unlimbo -- Unlimbos object and performs special fixups.                          *
 *                                                                                             *
 *    This routine will make sure that the home position for the foot class object gets        *
 *    reset. This is necessary since the home position may change depending on the unit's      *
 *    transition between limbo and non-limbo condition.                                        *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate to unlimbo the unit at.                                 *
 *                                                                                             *
 *          dir      -- The initial direction to give the unit.                                *
 *                                                                                             *
 * OUTPUT:  bool; Was the unit unlimboed successfully?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Unlimbo(COORDINATE coord, DirType dir)
{
	assert(IsActive);

	/*
	**	Try to unlimbo the unit.
	*/
	if (TechnoClass::Unlimbo(coord, dir)) {

		/*
		**	Mobile units are always revealed to the house that owns them.
		*/
		Revealed(House);

		/*
		**	Start in a still (non-moving) state.
		*/
		Path[0] = FACING_NONE;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * FootClass::Take_Damage -- Handles taking damage to this object.                             *
 *                                                                                             *
 *    This routine intercepts the damage assigned to this object and if this object is         *
 *    a member of a team, it informs the team that the damage has occurred. The team may       *
 *    change it's priority or action based on this event.                                      *
 *                                                                                             *
 * INPUT:   damage      -- The damage points inflicted on the unit.                            *
 *                                                                                             *
 *          distance    -- The distance from the point of damage to the unit itself.           *
 *                                                                                             *
 *          warhead     -- The type of damage that is inflicted.                               *
 *                                                                                             *
 *          source      -- The perpetrator of the damage. By knowing who caused the damage,    *
 *                         the team know's who to "get even with".                             *
 *                                                                                             *
 * OUTPUT:  Returns with the result type of the damage.                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/30/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ResultType FootClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source, bool forced)
{
	assert(IsActive);

	ResultType result = TechnoClass::Take_Damage(damage, distance, warhead, source, forced);

	if (result != RESULT_NONE && Team) {

		Team->Took_Damage(this, result, source);

	} else {

		if (result != RESULT_DESTROYED && result != RESULT_NONE) {

			/*
			**	Determine if the target that is currently being attacked has a weapon that can
			**	do harm to a ground based unit. This information is needed so that an appropriate
			**	response will occur when damage is taken.
			*/
//			bool tweap = false;
//			if (As_Techno(TarCom)) {
//				tweap = (As_Techno(TarCom)->Techno_Type_Class()->PrimaryWeapon != NULL);
//			}

			/*
			**	This ensures that if a unit is in sticky mode, then it will snap out of
			**	it when it takes damage.
			*/
			if (source != NULL && MissionControl[Mission].IsNoThreat && !MissionControl[Mission].IsZombie) {
				Enter_Idle_Mode();
			}

			/*
			**	If this object is not part of a team and it can retaliate for the damage, then have
			**	it try to do so. This prevents it from just sitting there and taking damage.
			*/
			if (Is_Allowed_To_Retaliate(source)) {

				int primary = What_Weapon_Should_I_Use(source->As_Target());
				if (In_Range(source, primary) || !House->IsHuman) {
					Assign_Target(source->As_Target());
				}

				if (Mission == MISSION_AMBUSH) {
					Assign_Mission(MISSION_HUNT);
				}

				/*
				**	Simple retaliation cannot occur because the source of the damage
				**	is too far away. If scatter logic is enabled, then scatter now.
				*/
				if (!Target_Legal(TarCom) && !Target_Legal(NavCom) && Rule.IsScatter) {
					Scatter(0, true);
				}

			} else {

				/*
				**	If this object isn't doing anything important, then scatter.
				*/
				if (MissionControl[Mission].IsScatter && !IsTethered && !IsDriving && !Target_Legal(TarCom) && !Target_Legal(NavCom) && What_Am_I() != RTTI_AIRCRAFT && What_Am_I() != RTTI_VESSEL) {
					if (!House->IsHuman || Rule.IsScatter) {
						Scatter(0, true);
					}
				}
			}
		}
	}
	return(result);
}


/***********************************************************************************************
 * FootClass::Active_Click_With -- Initiates attack or move according to target clicked on.    *
 *                                                                                             *
 *    At this level, the object is known to have the ability to attack or move to the          *
 *    target specified (in theory). Perform the attack or move as indicated.                   *
 *                                                                                             *
 * INPUT:   target   -- The target clicked upon that will precipitate action.                  *
 *                                                                                             *
 * OUTPUT:  Returns with the type of action performed.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/06/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Active_Click_With(ActionType action, ObjectClass * object)
{
	assert(IsActive);
	assert(object != NULL);

	switch (action) {
		case ACTION_GUARD_AREA:
			if (Can_Player_Fire() && Can_Player_Move()) {
				if (What_Am_I() == RTTI_INFANTRY &&
						((InfantryClass *)this)->Class->IsBomber &&
						object->What_Am_I() == RTTI_BUILDING &&
						!House->Is_Ally(object)) {

					Player_Assign_Mission(MISSION_SABOTAGE, TARGET_NONE, object->As_Target());
				} else {
					Player_Assign_Mission(MISSION_GUARD_AREA, object->As_Target());
				}
			}
			break;

		case ACTION_SELF:
			Player_Assign_Mission(MISSION_UNLOAD);
			break;

		case ACTION_ATTACK:
			if (Can_Player_Fire()) {
				Player_Assign_Mission(MISSION_ATTACK, object->As_Target());
			}
			break;

		case ACTION_ENTER:
			if (Can_Player_Move() && object && object->Is_Techno() /*&& !((RadioClass *)object)->In_Radio_Contact()*/) {
				Player_Assign_Mission(MISSION_ENTER, TARGET_NONE, object->As_Target());
			}
			break;

		case ACTION_CAPTURE:
			if (Can_Player_Move()) {
				Player_Assign_Mission(MISSION_CAPTURE, TARGET_NONE, object->As_Target());
			}
			break;

		case ACTION_SABOTAGE:
			if (Can_Player_Move()) {
				Player_Assign_Mission(MISSION_SABOTAGE, TARGET_NONE, object->As_Target());
			}
			break;

		case ACTION_NOMOVE:
		case ACTION_MOVE:
			if (Can_Player_Move()) {

				TARGET targ = object->As_Target();

				/*
				**	If the destination object is not the same zone, then pick a nearby location.
				*/
				if (object->What_Am_I() != RTTI_AIRCRAFT && Techno_Type_Class()->Speed != SPEED_WINGED && Map[Coord].Zones[Techno_Type_Class()->MZone] != Map[object->Center_Coord()].Zones[Techno_Type_Class()->MZone]) {

#ifdef FIXIT_MINE_PASSABLE
					// Fixes units not driving onto mines.
					if (Can_Enter_Cell(Coord_Cell(object->Center_Coord())) > MOVE_OK) {
						targ = ::As_Target(Map.Nearby_Location(Coord_Cell(object->Center_Coord()), Techno_Type_Class()->Speed, Map[Coord].Zones[Techno_Type_Class()->MZone], Techno_Type_Class()->MZone));
					}
#else
					targ = ::As_Target(Map.Nearby_Location(Coord_Cell(object->Center_Coord()), Techno_Type_Class()->Speed, Map[Coord].Zones[Techno_Type_Class()->MZone], Techno_Type_Class()->MZone));
#endif
				}

				Player_Assign_Mission(MISSION_MOVE, TARGET_NONE, targ);
			}
			break;

		case ACTION_NO_DEPLOY:
			Speak(VOX_DEPLOY);
			break;

		default:
			break;
	}
}


/***********************************************************************************************
 * FootClass::Active_Click_With -- Performs action as a result of left mouse click.            *
 *                                                                                             *
 *    This routine performs the action requested when the left mouse button was clicked over   *
 *    a cell. Typically, this is just a move command.                                          *
 *                                                                                             *
 * INPUT:   action   -- The predetermined action that should occur.                            *
 *                                                                                             *
 *          cell     -- The cell number that the action should occur at.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Active_Click_With(ActionType action, CELL cell)
{
	assert(IsActive);

	action = What_Action(cell);
	switch (action) {
		case ACTION_HARVEST:
			Player_Assign_Mission(MISSION_HARVEST, TARGET_NONE, ::As_Target(cell));
			break;

		case ACTION_MOVE:
			if (AllowVoice) {
				COORDINATE coord = Map.Pixel_To_Coord(Get_Mouse_X(), Get_Mouse_Y());
				OutList.Add(EventClass(ANIM_MOVE_FLASH, PlayerPtr->Class->House, coord));
			}
			// Fall into next case.

		case ACTION_NOMOVE:
			//using function for IsVisible so we have different results for different players - JAS 2019/09/30
			if (What_Am_I() != RTTI_AIRCRAFT || Map[cell].Is_Visible(PlayerPtr)) {

				/*
				** Find the closest same-zoned cell to where the unit currently is.
				** This will allow the unit to come as close to the destination cell
				** as is reasonably possible, when clicking on an impassable cell
				** (as is likely when clicking in the shroud.)  It looks for the
				** nearest cell using an expanding-radius box, and ignores cells
				** off the edge of the map.
				*/
				CellClass const * cellptr = &Map[::As_Cell(::As_Target(Center_Coord()))];
				if (What_Am_I() != RTTI_AIRCRAFT) {

					if (Can_Enter_Cell(Coord_Cell(Center_Coord())) == MOVE_OK) {
						cell = Map.Nearby_Location(cell, Techno_Type_Class()->Speed, cellptr->Zones[Techno_Type_Class()->MZone], Techno_Type_Class()->MZone);
					} else {
						cell = Map.Nearby_Location(cell, Techno_Type_Class()->Speed);
					}
#ifdef OBSOLETE
					cell = Map.Nearby_Location(cell, Techno_Type_Class()->Speed, cellptr->Zones[Techno_Type_Class()->MZone], Techno_Type_Class()->MZone);
#endif
				}

				Player_Assign_Mission(MISSION_MOVE, TARGET_NONE, ::As_Target(cell));
			}
			break;

		case ACTION_ATTACK:
			Player_Assign_Mission(MISSION_ATTACK, ::As_Target(cell));
			break;

		/*
		** Engineer attempting to capture bridge to repair it
		*/
		case ACTION_CAPTURE:
			if (Can_Player_Move()) {
				Player_Assign_Mission(MISSION_CAPTURE, TARGET_NONE, ::As_Target(cell));
			}
			break;

		case ACTION_SABOTAGE:
			Player_Assign_Mission(MISSION_SABOTAGE, TARGET_NONE, ::As_Target(cell) );
			break;

		// MBL 05.15.2020 - Adding support for CTRL+ALT clicking the ground to have units move to an area and guard it
		case ACTION_GUARD_AREA:
			if (Can_Player_Fire() && Can_Player_Move()) {
				Player_Assign_Mission(MISSION_GUARD_AREA, ::As_Target(cell));
			}
			break;
		// END MBL 05.15.2020 
	}
}


/***********************************************************************************************
 * FootClass::Per_Cell_Process -- Perform action based on once-per-cell condition.             *
 *                                                                                             *
 *    This routine is called as this object moves from cell to cell. When the center of the    *
 *    cell is reached, check to see if any trigger should be sprung. For moving units, reduce  *
 *    the path to the distance to the target. This forces path recalculation in an effort to   *
 *    avoid units passing each other.                                                          *
 *                                                                                             *
 * INPUT:   why   -- Specifies the circumstances under which this routine was called.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *   07/08/1995 JLB : Handles generic enter trigger event.                                     *
 *   07/16/1995 JLB : If next to a scanner and cloaked, then shimmer.                          *
 *=============================================================================================*/
void FootClass::Per_Cell_Process(PCPType why)
{
	assert(IsActive);

	if (why == PCP_END) {

		IsScattering = false;

		/*
		**	Clear any unloading flag if necessary.
		*/
		IsUnloading = false;

		/*
		**	If adjacent to an enemy techno that has the ability to reveal a sub,
		**	then shimmer the cloaked object.
		*/
		if (Cloak == CLOAKED) {
			for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
				CELL cell = Adjacent_Cell(Coord_Cell(Coord), face);

				if (Map.In_Radar(cell)) {
					TechnoClass const * techno = Map[cell].Cell_Techno();

					if (techno && !techno->House->Is_Ally(this) && techno->Techno_Type_Class()->IsScanner) {
						Do_Shimmer();
						break;
					}
				}
			}
		}

		/*
		**	Shorten the path if the target is now within weapon range of this
		**	unit and this unit is on an attack type mission. But only if the target
		**	is slow enough for leading to make sense.
		*/
		if (Target_Legal(TarCom) && (What_Am_I() != RTTI_INFANTRY || !((InfantryClass *)this)->Class->IsDog)) {
			int primary = What_Weapon_Should_I_Use(TarCom);
			bool inrange = In_Range(TarCom, primary);
			TechnoClass const * techno = As_Techno(TarCom);
			if (techno != NULL && techno->Is_Foot()) {
				FootClass const * foot = (FootClass const *)techno;
				MPHType speed = ((TechnoTypeClass const &)techno->Class_Of()).MaxSpeed;
				COORDINATE rangecoord = (speed > MPH_SLOW) ? foot->Likely_Coord() : foot->Target_Coord();
				inrange = In_Range(rangecoord, primary);
			}

			if ((Mission == MISSION_RESCUE || Mission == MISSION_GUARD_AREA || Mission == MISSION_ATTACK || Mission == MISSION_HUNT) && inrange) {
				Assign_Destination(TARGET_NONE);
				Path[0] = FACING_NONE;
			}
		}

		/*
		**	Trigger event associated with the player entering the cell.
		*/
		if (Cloak != CLOAKED) {
			TriggerClass * trigger = Map[Coord].Trigger;
			if (trigger != NULL) {
				trigger->Spring(TEVENT_PLAYER_ENTERED, this, Coord_Cell(Coord));
				if (!IsActive) return;
			}

			/*
			**	Check for horizontal trigger crossing.
			*/
			int x = Cell_X(Coord_Cell(Coord));
			int y = Cell_Y(Coord_Cell(Coord));
			int index;
			for (index = 0; index < Map.MapCellWidth; index++) {
				trigger = Map[XY_Cell(index+Map.MapCellX, y)].Trigger;
				if (trigger != NULL) {
					if (trigger->Class->Event1.Event == TEVENT_CROSS_HORIZONTAL || (trigger->Class->EventControl != MULTI_ONLY && trigger->Class->Event2.Event == TEVENT_CROSS_HORIZONTAL)) {
						trigger->Spring(TEVENT_CROSS_HORIZONTAL, this, Coord_Cell(Coord));
						if (!IsActive) return;
					}
				}
			}

			/*
			**	Check for vertical trigger crossing.
			*/
			for (index = 0; index < Map.MapCellHeight; index++) {
				trigger = Map[XY_Cell(x, index+Map.MapCellY)].Trigger;
				if (trigger != NULL) {
					if (trigger->Class->Event1.Event == TEVENT_CROSS_VERTICAL || (trigger->Class->EventControl != MULTI_ONLY && trigger->Class->Event2.Event == TEVENT_CROSS_VERTICAL)) {
						trigger->Spring(TEVENT_CROSS_VERTICAL, this, Coord_Cell(Coord));
						if (!IsActive) return;
					}
				}
			}

			/*
			**	Check for zone entry trigger events.
			*/
			for (MapTriggerID = 0; MapTriggerID < MapTriggers.Count(); MapTriggerID++) {
				trigger = MapTriggers[MapTriggerID];
				if (trigger->Class->Event1.Event == TEVENT_ENTERS_ZONE || (trigger->Class->EventControl != MULTI_ONLY && trigger->Class->Event2.Event == TEVENT_ENTERS_ZONE)) {
					if (Map[trigger->Cell].Zones[Techno_Type_Class()->MZone] == Map[Coord].Zones[Techno_Type_Class()->MZone]) {
						trigger->Spring(TEVENT_ENTERS_ZONE, this, Coord_Cell(Coord));
						if (!IsActive) return;
					}
				}
			}

			/*
			**	If any of these triggers cause this unit to be destroyed, then
			**	stop all further processing for this unit.
			*/
			if (!IsActive) return;
		}

#ifdef OBSOLETE
		/*
		** Flag any gap generators to re-draw
		*/
		for (int index = 0; index <Buildings.Count(); index++) {
			BuildingClass * obj = Buildings.Ptr(index);
			if (obj && *obj == STRUCT_GAP && !obj->IsInLimbo && (HouseClass *)obj->House != PlayerPtr) {
				int dist = Distance(obj) / CELL_LEPTON_W;
				if (dist < (6 + Rule.GapShroudRadius) ) {
	//			if (dist < (6 + obj->Class->SightRange) ) {
					obj->IsJamming = false;	// lie so it'll re-jam now
				}
			}
		}
#endif

	}

	TechnoClass::Per_Cell_Process(why);
}


/***************************************************************************
 * FootClass::Override_Mission -- temporarily overrides a units mission    *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:		MissionType mission - the mission we want to override       *
 *					TARGET	   tarcom  - the new target we want to override		*
 *					TARGET		navcom  - the new navigation point to override	*
 *                                                                         *
 * OUTPUT:		none                                                        *
 *                                                                         *
 * WARNINGS:   If a mission is already overridden, the current mission is  *
 *					just re-assigned.															*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/28/1995 PWG : Created.                                             *
 *=========================================================================*/
void FootClass::Override_Mission(MissionType mission, TARGET tarcom, TARGET navcom)
{
	assert(IsActive);

 	SuspendedNavCom = NavCom;
	TechnoClass::Override_Mission(mission, tarcom, navcom);

	Assign_Destination(navcom);
}


/***************************************************************************
 * FootClass::Restore_Mission -- Restores an overridden mission            *
 *                                                                         *
 * INPUT:		none                                                        *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/28/1995 PWG : Created.                                             *
 *=========================================================================*/
bool FootClass::Restore_Mission(void)
{
	assert(IsActive);

	if (TechnoClass::Restore_Mission()) {
		Assign_Destination(SuspendedNavCom);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * FootClass::Receive_Message -- Movement related radio messages are handled here.             *
 *                                                                                             *
 *    This routine handles radio message that are related to movement. These are used for      *
 *    complex coordinated maneuvers.                                                           *
 *                                                                                             *
 * INPUT:   from     -- Pointer to the originator of this radio message.                       *
 *                                                                                             *
 *          message  -- The radio message that is being received.                              *
 *                                                                                             *
 *          param    -- The optional parameter (could be a movement destination).              *
 *                                                                                             *
 * OUTPUT:  Returns with the radio response appropriate to the message received. Usually the   *
 *          response is RADIO_ROGER.                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
RadioMessageType FootClass::Receive_Message(RadioClass * from, RadioMessageType message, long & param)
{
	assert(IsActive);

	switch (message) {

		/*
		**	Answers if this object is located on top of a service depot.
		*/
		case RADIO_ON_DEPOT:
			if (Map[Center_Coord()].Cell_Building() != NULL) {
				BuildingClass const * building = Map[Center_Coord()].Cell_Building();
				if (*building == STRUCT_REPAIR) {
					return(RADIO_ROGER);
				}
			}
			return(RADIO_NEGATIVE);

		/*
		**	Intercept the repair request and if this object is moving, then no repair
		**	is possible.
		*/
		case RADIO_REPAIR:
			if (Target_Legal(NavCom)) return(RADIO_NEGATIVE);
			break;

		/*
		**	Something bad has happened to the object in contact with. Abort any coordinated
		**	activity with this object. Basically, ... run away! Run away!
		*/
		case RADIO_RUN_AWAY:
			if (In_Radio_Contact()) {
				if (NavCom == Contact_With_Whom()->As_Target()) {
					Assign_Destination(TARGET_NONE);
				}
			}
			if (Mission == MISSION_SLEEP) {
				Assign_Mission(MISSION_GUARD);
				Commence();
			}
			if (Mission == MISSION_ENTER) {
				Assign_Mission(MISSION_GUARD);
			}
			if (!IsRotating && !Target_Legal(NavCom)) {
				Scatter(0, true, true);
			}
			break;

		/*
		**	Checks to see if this unit needs to move somewhere. If it is already in motion,
		**	then it doesn't need further movement instructions.
		*/
		case RADIO_NEED_TO_MOVE:
			param = (long)NavCom;
			if (!Target_Legal(NavCom)) {
				return(RADIO_ROGER);
			}
			return(RADIO_NEGATIVE);

		/*
		**	Radio request to move to location specified. Typically this is used
		**	for complex loading and unloading missions.
		*/
		case RADIO_MOVE_HERE:
			if (NavCom != (TARGET)param) {
				if (::As_Target(Coord_Cell(Coord)) == (TARGET)param) {
					return(RADIO_YEA_NOW_WHAT);
				} else {
					if (Mission == MISSION_GUARD && MissionQueue == MISSION_NONE) {
						Assign_Mission(MISSION_MOVE);
					}
					Assign_Destination((TARGET)param);
					Shorten_Mission_Timer();
				}
			}
			return(RADIO_ROGER);

		/*
		** Requests if this unit is trying to cooperatively load up. Typically, this occurs
		**	for passengers and when vehicles need to be repaired.
		*/
		case RADIO_TRYING_TO_LOAD:
			if (Mission == MISSION_ENTER || MissionQueue == MISSION_ENTER) {
				TechnoClass::Receive_Message(from, message, param);
				return(RADIO_ROGER);
			}
			break;
	}
	return(TechnoClass::Receive_Message(from, message, param));
}


/***********************************************************************************************
 * FootClass::Mission_Enter -- Enter (cooperatively) mission handler.                          *
 *                                                                                             *
 *    This mission handler will cooperatively coordinate the object to maneuver into the       *
 *    object it is in radio contact with. This is used by infantry when they wish to load      *
 *    into an APC as well as by vehicles when they wish to enter a repair facility.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of game ticks before this routine should be called again.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/15/1995 JLB : Created.                                                                 *
 *   09/22/1995 JLB : Modified to handle the "on hold" condition.                              *
 *=============================================================================================*/
int FootClass::Mission_Enter(void)
{
	assert(IsActive);

	/*
	**	Find out who to coordinate with. If in radio contact, then this the transporter is
	**	defined. If not in radio contact, then try the archive target value to see if that
	**	is suitable.
	*/
	TechnoClass * contact = Contact_With_Whom();
	if (contact == NULL) {
		contact = As_Techno(ArchiveTarget);
	}

	/*
	**	If in contact, then let the transporter handle the movement coordination.
	*/
	if (contact != NULL) {

		/*
		**	If the transport says to "bug off", then abort the enter mission. The transport may
		**	likely say all is 'ok' with the "RADIO ROGER", then try again later.
		*/
		if (Transmit_Message(RADIO_DOCKING, contact) != RADIO_ROGER && !IsTethered) {
			Transmit_Message(RADIO_OVER_OUT);
			Enter_Idle_Mode();
		}

	} else {

		/*
		**	Since there is no potential object to enter, then abort this
		**	mission with some default standby mission.
		*/
		Enter_Idle_Mode();
	}

	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
}


/***********************************************************************************************
 * FootClass::Assign_Destination -- Assigns specified destination to NavCom.                   *
 *                                                                                             *
 *    This routine will assign the specified target to the navigation computer. No legality    *
 *    checks are performed.                                                                    *
 *                                                                                             *
 * INPUT:   target   -- The target value to assign to the navigation computer.                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Assign_Destination(TARGET target)
{
	assert(IsActive);

	NavCom = target;

	/*
	**	Presume that the easiest path is tried first. As the findpath proceeds, when
	**	a failure occurs, this threshhold will be increased until path failure
	**	cannot be prevent. At this point, all movement should cease.
	*/
	PathThreshhold = MOVE_CLOAK;
}


/***********************************************************************************************
 * FootClass::Detach_All -- Removes this object from the game system.                          *
 *                                                                                             *
 *    This routine will remove this object from the game system. This routine is called when   *
 *    this object is about to be deleted. All other objects should no longer reference this    *
 *    object in that case.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Detach_All(bool all)
{
	assert(IsActive);

	if (Team && !ScenarioInit) {
		Team->Remove(this);
		Team = NULL;
	}

	TechnoClass::Detach_All(all);
}


/***********************************************************************************************
 * FootClass::Rescue_Mission -- Calls this unit to the rescue.                                 *
 *                                                                                             *
 *    This routine is called when the house determines that it should attack the specified     *
 *    target. This routine will determine if it can attack the target specified and if so,     *
 *    the amount of power it can throw at it. This returned power value is used to allow       *
 *    intelligent distribution of retaliation.                                                 *
 *                                                                                             *
 * INPUT:   target   -- The target that this object just might be assigned to attack and thus  *
 *                      how much power it can bring to bear should be returned.                *
 *                                                                                             *
 * OUTPUT:  Returns with the amount of power that this object can bring to bear against the    *
 *          potential target specified.                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int FootClass::Rescue_Mission(TARGET tarcom)
{
	assert(IsActive);

	/*
	**	If the target specified is not legal, then it cannot be attacked. Always return
	**	zero in this case.
	*/
	if (!Target_Legal(tarcom)) return(0);

	/*
	** If the unit is already assigned to destroy the tarcom then we need
	** to return a negative value which tells the computer to lower the
	** desired threat rating.
	*/
	if (TarCom == tarcom) {
		return(-Risk());
	}

	/*
	** If the unit is currently attacking a target that has a weapon then we
	** cannot abandon it as it will destroy us if we return to base.
	*/
	if (Target_Legal(TarCom)) {
		TechnoClass * techno = As_Techno(TarCom);
		if (techno != NULL && techno->Is_Weapon_Equipped()) {
			return(0);
		}
	}

	/*
	** If the unit is in a harvest mission or is currently attacking
	** something, or is not very effective, then it will be of no help
	** at all.
	*/
	if (Team.Is_Valid() || Mission == MISSION_HARVEST || !Risk()) {
		return(0);
	}

	/*
	** Find the distance to the target modified by the range.  If the
	** the distance is 0, then things are ok.
	*/
	int dist = Distance(tarcom) - Weapon_Range(0);
	int threat = Risk() * 1024;
	int speed = -1;
	if (dist > 0) {

		/*
		** Next we need to figure out how fast the unit moves because this
		** decreases the distance penalty.
		*/
		speed = max((unsigned)Techno_Type_Class()->MaxSpeed, (unsigned)1);

		int ratio = (speed > 0) ? Max(dist / speed, 1) : 1;

		/*
		** Finally modify the threat by the distance the unit is away.
		*/
		threat = max(threat/ratio, 1);
	}
	return(threat);
}


/***********************************************************************************************
 * FootClass::Death_Announcement -- Announces the death of a unit.                             *
 *                                                                                             *
 *    This routine is called when a unit (infantry, vehicle, or aircraft) is destroyed.        *
 *                                                                                             *
 * INPUT:   source   -- The perpetrator of this death.                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Death_Announcement(TechnoClass const * ) const
{
	assert(IsActive);

	//if (IsOwnedByPlayer) {
	if ((Session.Type == GAME_GLYPHX_MULTIPLAYER && House->IsHuman) || (Session.Type != GAME_GLYPHX_MULTIPLAYER && IsOwnedByPlayer)) {
		if (What_Am_I() == RTTI_VESSEL) {
			// Speak(VOX_SHIP_LOST); // MBL 02.06.2020
			Speak(VOX_SHIP_LOST, House, Center_Coord());
		} else {
			// Speak(VOX_UNIT_LOST); // MBL 02.06.2020
			Speak(VOX_UNIT_LOST, House, Center_Coord());
		}
	}
}


/***********************************************************************************************
 * FootClass::Greatest_Threat -- Fetches the greatest threat to this object.                   *
 *                                                                                             *
 *    This routine will return with the greatest threat (best target) for this object. For     *
 *    movable ground object, they won't automatically return ANY target if this object is      *
 *    cloaked. Otherwise, cloaking is relatively useless.                                      *
 *                                                                                             *
 * INPUT:   method   -- The request method (bit flags) to use when scanning for a target.      *
 *                                                                                             *
 * OUTPUT:  Returns with the best target to attack. If there is no target that qualifies, then *
 *          TARGET_NONE is returned.                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *   07/10/1996 JLB : Handles scan range limitation.                                           *
 *=============================================================================================*/
TARGET FootClass::Greatest_Threat(ThreatType method) const
{
	assert(IsActive);

	/*
	**	If the scan is forced to be limited, then limit the scan now.
	*/
	if (IsScanLimited) {
		method = method & ~THREAT_AREA;
		method = method | THREAT_RANGE;
	}

	/*
	**	If this object can cloak, then it won't select a target automatically.
	*/
	if (House->IsHuman && IsCloakable && Mission == MISSION_GUARD) {
		return(TARGET_NONE);
	}

	if (!(method & (THREAT_INFANTRY|THREAT_VEHICLES|THREAT_BUILDINGS|THREAT_TIBERIUM|THREAT_BOATS|THREAT_CIVILIANS|THREAT_POWER|THREAT_FAKES|THREAT_FACTORIES|THREAT_BASE_DEFENSE))) {
		if (What_Am_I() != RTTI_VESSEL) {
			method = method | THREAT_GROUND;
		} else {
			method = method | THREAT_BOATS|THREAT_GROUND;
		}
	}

	/*
	**	Perform the search for the target.
	*/
	TARGET target = TechnoClass::Greatest_Threat(method);

	/*
	**	If no target could be located and this object is under scan range
	**	restrictions, then this restriction must be lifted now.
	*/
	if (IsScanLimited && target == TARGET_NONE) {
		const_cast<FootClass*>(this)->IsScanLimited = false;		// const_cast ST - 5/8/2019
	}

	/*
	**	Return with final target found.
	*/
	return(target);
}


/***********************************************************************************************
 * FootClass::Detach -- Detaches a target from tracking systems.                               *
 *                                                                                             *
 *    This routine will detach the specified target from the tracking systems of this object.  *
 *    It will be removed from the navigation computer and any queued mission record.           *
 *                                                                                             *
 * INPUT:   target   -- The target to be removed from this object.                             *
 *                                                                                             *
 *          all      -- Is the unit really about to be eliminated? If this is true then even   *
 *                      friendly contact (i.e., radio) must be eliminated.                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *   07/24/1996 JLB : Removes target from NavQueue list.                                       *
 *=============================================================================================*/
void FootClass::Detach(TARGET target, bool all)
{
	assert(IsActive);

	TechnoClass::Detach(target, all);

	if (!SpecialFlag) {
		if (ArchiveTarget == target) {
			ArchiveTarget = TARGET_NONE;
		}
	}

	if (SuspendedNavCom == target) {
		SuspendedNavCom = TARGET_NONE;
		SuspendedMission = MISSION_NONE;
	}

	/*
	**	If the navigation computer is assigned to the target, then the navigation
	**	computer must be cleared.
	*/
	if (NavCom == target) {
		NavCom = TARGET_NONE;
		Path[0] = FACING_NONE;
		Restore_Mission();
	}

	/*
	**	Remove the target from the NavQueue list as well.
	*/
	for (int index = 0; index < ARRAY_SIZE(NavQueue); index++) {
		if (NavQueue[index] == target) {
			NavQueue[index] = TARGET_NONE;
			if (index < ARRAY_SIZE(NavQueue)-1) {
				memmove(&NavQueue[index], &NavQueue[index+1], ((ARRAY_SIZE(NavQueue)-index)-1) * sizeof(NavQueue[0]));
				index--;
			}
		}
	}

	/*
	**	If targeting the specified object and this unit is obviously heading
	**	toward the target to get within range, then abort the path.
	*/
	if (TarCom == target && House->IsHuman) {
		Path[0] = FACING_NONE;
	}
}


/***********************************************************************************************
 * FootClass::Offload_Tiberium_Bail -- Fetches the Tiberium to offload per step.               *
 *                                                                                             *
 *    This routine is called when a packet/package/bail of Tiberium needs to be offloaded      *
 *    from the object. This function is overridden for those objects that can contain          *
 *    Tiberium.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of credits offloaded from the object.                      *
 *                                                                                             *
 * WARNINGS:   This routine must be called multiple times in order to completely offload the   *
 *             Tiberium. When this routine return 0, all Tiberium has been offloaded.          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int FootClass::Offload_Tiberium_Bail(void)
{
	assert(IsActive);

	return(0);
}


/***********************************************************************************************
 * FootClass::Can_Enter_Cell -- Checks to see if the object can enter cell specified.          *
 *                                                                                             *
 *    This routine examines the specified cell to see if the object can enter it. This         *
 *    function is to be overridden for objects that could have the possibility of not being    *
 *    allowed to enter the cell. Typical objects at the FootClass level always return          *
 *    MOVE_OK.                                                                                 *
 *                                                                                             *
 * INPUT:   cell     -- The cell to examine.                                                   *
 *                                                                                             *
 *          facing   -- The direction that this cell might be entered from.                    *
 *                                                                                             *
 * OUTPUT:  Returns with the move check result type. This will be MOVE_OK if there is not      *
 *          blockage. There are various other values that represent other blockage types.      *
 *          The value returned will indicated the most severe reason why entry into the cell   *
 *          is blocked.                                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
MoveType FootClass::Can_Enter_Cell(CELL , FacingType) const
{
	assert(IsActive);

	return MOVE_OK;
}


/***********************************************************************************************
 * FootClass::Can_Demolish -- Checks to see if this object can be sold back.                   *
 *                                                                                             *
 *    This routine determines if it is legal to sell the object back. A foot class object can  *
 *    only be sold back if it is sitting on a repair bay.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was the object successfully sold back?                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Can_Demolish(void) const
{
	assert(IsActive);

	StructType sell_struct = STRUCT_NONE;
	switch (What_Am_I()) {
		case RTTI_UNIT:
			sell_struct = STRUCT_REPAIR;
			break;
		case RTTI_AIRCRAFT:
			sell_struct = STRUCT_AIRSTRIP;
			break;
		default:
			break;
	}
	if (sell_struct != STRUCT_NONE) {
		if (In_Radio_Contact() &&
			Contact_With_Whom()->What_Am_I() == RTTI_BUILDING &&
			*((BuildingClass *)Contact_With_Whom()) == sell_struct &&
			Distance(Contact_With_Whom()) < 0x0080) {
			return(true);
		}
	}
	return(TechnoClass::Can_Demolish());
}


/***********************************************************************************************
 * FootClass::Sell_Back -- Causes this object to be sold back.                                 *
 *                                                                                             *
 *    When an object is sold back, a certain amount of money is refunded to the owner and then *
 *    the object is removed from the game system.                                              *
 *                                                                                             *
 * INPUT:   control  -- The action to perform. The only supported action is "1", which means   *
 *                      to sell back.                                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Sell_Back(int control)
{
	assert(IsActive);

	if (control != 0) {
		if (House == PlayerPtr) {
			Speak(VOX_UNIT_SOLD);
			Sound_Effect(VOC_CASHTURN);
		}
		House->Refund_Money(Refund_Amount());
		Stun();
		Limbo();
		delete this;
	}
}


/***********************************************************************************************
 * FootClass::Likely_Coord -- Fetches the coordinate the object will be at shortly.            *
 *                                                                                             *
 *    This routine comes in handy when determining where a travelling object will be at        *
 *    when considering the amount of time it would take for a normal unit to travel one cell.  *
 *    Using this information, an intelligent "approach target" logic can be employed.          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate the object is at or soon will be.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE FootClass::Likely_Coord(void) const
{
	assert(IsActive);

	if (Head_To_Coord()) {
		return(Head_To_Coord());
	}
	return(Target_Coord());
}


/***********************************************************************************************
 * FootClass::Adjust_Dest -- Adjust candidate movement cell to account for formation.          *
 *                                                                                             *
 *    This routine modify the specified cell if the unit is part of a formation. The           *
 *    adjustment will take into consideration the formation relative offset from the           *
 *    (presumed) center cell specified.                                                        *
 *                                                                                             *
 * INPUT:   cell  -- The cell to presume as the desired center point of the formation.         *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that should be used as the actual destination. If this       *
 *          object is part of a formation, then the cell location will be appropriately        *
 *          adjusted.                                                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL FootClass::Adjust_Dest(CELL cell) const
{
	assert(IsActive);

	if (IsFormationMove) {
		int xdest = Cell_X(cell);
		int ydest = Cell_Y(cell);

		int newx = Bound(XFormOffset + xdest, Map.MapCellX, Map.MapCellX + Map.MapCellWidth -1);
		int newy = Bound(YFormOffset + ydest, Map.MapCellY, Map.MapCellY + Map.MapCellHeight -1);

		cell = XY_Cell(newx, newy);
	}
	return(cell);
}


/***********************************************************************************************
 * FootClass::Handle_Navigation_List -- Processes the navigation queue.                        *
 *                                                                                             *
 *    This routine will process the navigation queue. If the queue is present and valid and    *
 *    there is currently no navigation target assigned to this object, then the first entry    *
 *    of the queue will be assigned. The remaining entries will move down. If the queue is     *
 *    to be processed as a circular list, then the first entry is appended to the end.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine might end up assigning a movement destination.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Handle_Navigation_List(void)
{
	/*
	**	The navigation queue only needs to be processed if there is
	**	currently no navigation target for this object.
	*/
	if (!Target_Legal(NavCom)) {
		TARGET target = NavQueue[0];

		/*
		**	Check to see if the navigation queue even exists and
		**	has at least one valid entry. If it does, then process it by
		**	assigning the object's NavCom to the first entry on the list.
		*/
		if (Target_Legal(target)) {
			Assign_Destination(target);
			memmove(&NavQueue[0], &NavQueue[1], sizeof(NavQueue)-sizeof(NavQueue[0]));
			NavQueue[ARRAY_SIZE(NavQueue)-1] = TARGET_NONE;

			/*
			**	If the navigation queue is to loop (indefinately), then append the
			**	target value from the first part to the end of the queue.
			*/
			if (IsNavQueueLoop) {
				for (int index = 0; index < ARRAY_SIZE(NavQueue); index++) {
					if (NavQueue[index] == TARGET_NONE) {
						NavQueue[index] = target;
						break;
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * FootClass::Queue_Navigation_List -- Add a target to the objects navigation list.            *
 *                                                                                             *
 *    This routine will append the destination target to the object's NavQueue list. After     *
 *    doing so, if the object is not doing anything important, then it will be started on      *
 *    that destination. This is functionally the same as Assign_Destination, but it stores     *
 *    the target to the NavQueue first.                                                        *
 *                                                                                             *
 * INPUT:   target   -- The movement target destination to append the queue.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The queue is of finite size and any queue requests that would exceed that size  *
 *             are ignored. If there are no queue entries pending and the unit is not          *
 *             otherwise occupied, then the queue target might be carried directly into the    *
 *             NavCom.                                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Queue_Navigation_List(TARGET target)
{
	if (Target_Legal(target)) {
		int count;
		for (count = 0; count < ARRAY_SIZE(NavQueue); count++) {
			if (!Target_Legal(NavQueue[count])) break;
		}

		/*
		**	If the target is this object itself, then this indicates that the
		**	queue list is to be processed as a loop. Otherwise, just tack the
		**	navigation target to the end of the list.
		*/
		if (target == As_Target() && count > 0) {
			IsNavQueueLoop = true;
		} else {
			if (count == 0) {
				IsNavQueueLoop = false;
			}
			if (count < ARRAY_SIZE(NavQueue)) {
				NavQueue[count] = target;
			}
		}

		/*
		**	If this object isn't doing anything, then start acting on the
		**	navigation queue now.
		*/
		if (!Target_Legal(NavCom) && Mission == MISSION_GUARD) {
			Enter_Idle_Mode();
		}
	}
}


/***********************************************************************************************
 * FootClass::Clear_Navigation_List -- Clears out the navigation queue.                        *
 *                                                                                             *
 *    This routine will clear out any values in the navigation queue. This is the preferred    *
 *    way of aborting a navigation queue for a unit. If the unit is already travelling, it     *
 *    won't be interrupted by this routine.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This will clear the navigation list but not the navigation computer. Thus a     *
 *             unit will still travel to its current immediate destination.                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Clear_Navigation_List(void)
{
	for (int index = 0; index < ARRAY_SIZE(NavQueue); index++) {
		NavQueue[index] = TARGET_NONE;
	}
}


/***********************************************************************************************
 * FootClass::Is_Allowed_To_Leave_Map -- Checks to see if it can leave the map and the game.   *
 *                                                                                             *
 *    This routine will determine if this object has permission to leave the map and thus      *
 *    leave the game. Typical objects with this permission are transports used to drop of      *
 *    reinforcements.                                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Does this object have permission to travel off the map edge and leave the    *
 *                game?                                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Is_Allowed_To_Leave_Map(void) const
{
	/*
	**	If the unit hasn't entered the map yet, then don't allow leave the game.
	*/
	if (!IsLocked) return(false);

	/*
	**	A unit that isn't marked as a loaner is a gift to the player. Such objects can never
	**	leave the map unless they are part of a team that gives it special permision.
	*/
	if (!IsALoaner && Mission != MISSION_RETREAT && (!Team.Is_Valid() || !Team->Is_Leaving_Map())) return(false);

	return(true);
}


/***********************************************************************************************
 * FootClass::Is_Recruitable -- Determine if this object is recruitable as a team members.     *
 *                                                                                             *
 *    This will examine this object to determine if it is suitable as a team recruit. Some     *
 *    objects are disqualified if they are otherwise premptively occupied.                     *
 *                                                                                             *
 * INPUT:   house -- Pointer to the house that is trying to recruit this object.               *
 *                                                                                             *
 * OUTPUT:  bool; Is this object suitable for recruitment by a team.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Is_Recruitable(HouseClass const * house) const
{
	/*
	**	If not of the correct house presuasion, then recruitment is not allowed.
	*/
	if (house != NULL && house != House) {
		return(false);
	}

	/*
	**	If the object is not a playing member of the game, then don't consider it available.
	*/
	if (IsInLimbo) {
		return(false);
	}

	/*
	**	If it is already part of another team, then it is not available for
	**	general recruitment.
	*/
	if (Team.Is_Valid()) {
		return(false);
	}

	/*
	**	If it is currently in a mission the precludes recruitment into a team, then
	**	return with this information.
	*/
	if (!Is_Recruitable_Mission(Mission)) {
		return(false);
	}

	/*
	**	It was not disqualified for general team recruitment, so return that
	**	it is available.
	*/
	return(true);
}


/***********************************************************************************************
 * FootClass::AI -- Handle general movement AI.                                                *
 *                                                                                             *
 *    This basically just sees if this object is within weapon range of the target and if      *
 *    so, it will stop movement so that firing may commence. This prevents the occasional      *
 *    case of an attacker driving right up to the defender before firing.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::AI(void)
{
	TechnoClass::AI();

// FootClass::Per_Cell_Process does this function already.
#ifdef OBSOLETE
	if (IsActive) {
		if (!IsScattering && !IsTethered && !IsInLimbo && What_Am_I() != RTTI_AIRCRAFT && Target_Legal(TarCom) && In_Range(TarCom)) {
			Assign_Destination(TARGET_NONE);
		}
	}
#endif
}


/***********************************************************************************************
 * FootClass::Is_On_Priority_Mission -- Checks to see if this object should be given priority. *
 *                                                                                             *
 *    Some objects are on an important mission that must succeed. If the object is on such     *
 *    a mission, then it will be more aggressive in its movement action.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this object on a priority mission?                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Is_On_Priority_Mission(void) const
{
	if (Mission == MISSION_ENTER) return(true);
	return(false);
}


/***********************************************************************************************
 * FootClass::Mission_Retreat -- Handle reatreat from map mission for mobile objects.          *
 *                                                                                             *
 *    This will try to make this mobile object leave the map. It does this by assigning a      *
 *    movement destination that is located off the edge of the map.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine        *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int FootClass::Mission_Retreat(void)
{
	assert(IsActive);

	enum {
		FIND_EDGE,
		TRAVELLING
	};

	switch (Status) {

		/*
		**	Find a suitable edge to travel to and then assign destination there.
		*/
		case FIND_EDGE:
			if (Target_Legal(NavCom)) {
				Status = TRAVELLING;
			} else {

				CELL cell = 0;

				/*
				**	If this is part of a team, then pick the edge where the team as likely
				**	entered from.
				*/
				if (Team.Is_Valid() && Team->Class->Origin != -1) {
					cell = Map.Calculated_Cell(House->Control.Edge, Team->Class->Origin, Coord_Cell(Center_Coord()), Techno_Type_Class()->Speed);
				}

				/*
				**	If an edge hasn't been found, then try to find one that is not based on any
				**	team information.
				*/
				if (cell == 0) {
					cell = Map.Calculated_Cell(House->Control.Edge, -1, Coord_Cell(Center_Coord()), Techno_Type_Class()->Speed);
				}

				assert(cell == 0);		// An edge cell must be found!

				Assign_Destination(::As_Target(cell));
				Status = TRAVELLING;
			}
			break;

		/*
		**	While travelling, monitor that all is proceeding according to plan.
		*/
		case TRAVELLING:
			if (!Target_Legal(NavCom)) {
				Status = FIND_EDGE;
			}
			break;
	}

	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
}
