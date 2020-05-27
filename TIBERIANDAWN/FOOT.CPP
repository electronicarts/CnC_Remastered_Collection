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

/* $Header:   F:\projects\c&c\vcs\code\foot.cpv   2.17   16 Oct 1995 16:51:42   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : August 13, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   FootClass::Active_Click_With -- Intiates attack or move according to target clicked on.   *
 *   FootClass::Active_Click_With -- Performs action as a result of left mouse click.          *
 *   FootClass::Approach_Target -- Sets the navigation computer to approach target object.     *
 *   FootClass::Assign_Destination -- Assigns specified destination to NavCom.                 *
 *   FootClass::Assign_Mission -- Assign mission to foot class object.                         *
 *   FootClass::Basic_Path -- Finds the basic path for a ground object.                        *
 *   FootClass::Body_Facing -- Set the body rotation/facing.                                   *
 *   FootClass::Can_Demolish -- Checks to see if this object can be sold back.                 *
 *   FootClass::Can_Enter_Cell -- Checks to see if the object can enter cell specified.        *
 *   FootClass::Death_Announcement -- Announces the death of a unit.                           *
 *   FootClass::Debug_Dump -- Displays the status of the FootClass to the mono monitor.        *
 *   FootClass::Detach -- Detaches a target from tracking systems.                             *
 *   FootClass::Detach_All -- Removes this object from the game system.                        *
 *   FootClass::Enters_Building -- When unit enters a building for some reason.                *
 *   FootClass::FootClass -- Default constructor for foot class objects.                       *
 *   FootClass::FootClass -- Normal constructor for the foot class object.                     *
 *   FootClass::Greatest_Threat -- Fetches the greatest threat to this object.                 *
 *   FootClass::Likely_Coord -- Fetches the coordinate the object will be at shortly.          *
 *   FootClass::Limbo -- Intercepts limbo event and handles FootClass processing.              *
 *   FootClass::Mark -- Unit interface to map rendering system.                                *
 *   FootClass::Mission_Attack -- AI for heading towards and firing upon target.               *
 *   FootClass::Mission_Capture -- Handles the capture mission.                                *
 *   FootClass::Mission_Enter -- Enter (cooperatively) mission handler.                        *
 *   FootClass::Mission_Guard_Area -- Causes unit to guard an area about twice weapon range.   *
 *   FootClass::Mission_Hunt -- Handles the default hunt order.                                *
 *   FootClass::Mission_Move -- AI process for moving a vehicle to its destination.            *
 *   FootClass::Offload_Tiberium_Bail -- Fetches the Tiberium to offload per step.             *
 *   FootClass::Override_Mission -- temporarily overides a units mission                       *
 *   FootClass::Per_Cell_Process -- Perform action based on once-per-cell condition.           *
 *   FootClass::Receive_Message -- Movement related radio messages are handled here.           *
 *   FootClass::Rescue_Mission -- Calls this unit to the rescue.                               *
 *   FootClass::Restore_Mission -- Restores an overidden mission                               *
 *   FootClass::Sell_Back -- Causes this object to be sold back.                               *
 *   FootClass::Set_Speed -- Initiate unit movement physics.                                   *
 *   FootClass::Sort_Y -- Determine the sort coordinate for foot class objects.                *
 *   FootClass::Start_Driver -- This starts the driver heading to the destination desired.     *
 *   FootClass::Stop_Driver -- This routine clears the driving state of the object.            *
 *   FootClass::Stun -- Prepares a ground travelling object for removal.                       *
 *   FootClass::Take_Damage -- Handles taking damage to this object.                           *
 *   FootClass::Unlimbo -- Unlimbos object and performs special fixups.                        *
 *   FootClass::~FootClass -- Default destructor for foot class objects.                       *
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
FootClass::FootClass(void) : Speed(0)
{
	IsDriving = false;
	IsInitiated = false;
	IsPlanningToLook = false;
	IsDeploying = false;
	IsNewNavCom = false;
	IsFiring = false;
	IsRotating = false;
	IsUnloading = false;
	NavCom = TARGET_NONE;
	SuspendedNavCom = TARGET_NONE;
	Path[0] = FACING_NONE;
	HeadToCoord = NULL;
	Member = 0;
	Team = 0;
	PathDelay = 0;
	TryTryAgain = PATH_RETRY;
	if (House) {
		House->CurUnits++;
	}
	Group = -1;
}


/***********************************************************************************************
 * FootClass::~FootClass -- Default destructor for foot class objects.                         *
 *                                                                                             *
 *    At this level of the destruction process, the house record of the number of units        *
 *    currently in inventory is decremented to reflect this units destruction.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
FootClass::~FootClass(void)
{
	if (GameActive && House) {
		House->CurUnits--;
	}
}


/***********************************************************************************************
 * FootClass::FootClass -- Normal constructor for the foot class object.                       *
 *                                                                                             *
 *    This is the normal constructor used when creating a foot class object.                   *
 *                                                                                             *
 * INPUT:   house    -- The house that owns this object.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
FootClass::FootClass(HousesType house) :
			TechnoClass(house),
			Speed(0)
{
	Member = 0;
	Team = 0;
	Path[0] = FACING_NONE;
	NavCom = TARGET_NONE;
	SuspendedNavCom = TARGET_NONE;
	IsUnloading = false;
	IsDriving = false;
	IsInitiated = false;
	IsRotating = false;
	IsFiring = false;
	IsDeploying = false;
	IsNewNavCom = false;
	IsPlanningToLook = false;
	HeadToCoord = 0L;
	PathDelay = 0;
	Group = -1;
	TryTryAgain = PATH_RETRY;
	House->CurUnits++;
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
void FootClass::Debug_Dump(MonoClass *mono) const
{
	static char	const * _p2c[9] = {"-","0","1","2","3","4","5","6","7"};
#define	Path_To_String(a)	_p2c[((ABS((int)a+1))%9)]

	/*
	**	Display the common data for all objects that inherity from FootClass.
	*/
	mono->Set_Cursor(63, 7);
	if (Team) {
		mono->Printf("%s(%d)", Team->Class->IniName, Teams.ID(Team));
	} else {
		mono->Printf("(none)");
	}
	mono->Set_Cursor(73, 7);mono->Printf("%04X", ArchiveTarget);
	mono->Set_Cursor(42, 1);mono->Printf("%04X", NavCom);
	mono->Set_Cursor(44, 3);mono->Printf("%d", Speed);

	/*
	**	Although aircraft inherit from FootClass, some of the variables are not
	**	used and thus should not be displayed.
	*/
	if (What_Am_I() != RTTI_AIRCRAFT) {
		mono->Set_Cursor(50, 3);
		mono->Printf("%s%s%s%s%s%s%s%s%s%s%s%s",
			Path_To_String(Path[0]),
			Path_To_String(Path[1]),
			Path_To_String(Path[2]),
			Path_To_String(Path[3]),
			Path_To_String(Path[4]),
			Path_To_String(Path[5]),
			Path_To_String(Path[6]),
			Path_To_String(Path[7]),
			Path_To_String(Path[8]),
			Path_To_String(Path[9]),
			Path_To_String(Path[10]),
			Path_To_String(Path[11]),
			Path_To_String(Path[12]));

		mono->Set_Cursor(65, 1);mono->Printf("%08lX", Head_To_Coord());
		mono->Text_Print("X", 16 + (IsDeploying?2:0), 12);
		mono->Text_Print("X", 16 + (IsRotating?2:0), 13);
		mono->Text_Print("X", 16 + (IsDriving?2:0), 15);
		mono->Text_Print("X", 16 + (IsFiring?2:0), 14);
		mono->Text_Print("X", 16 + (IsPlanningToLook?2:0), 16);
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
	if (TechnoClass::Mark(mark)) {
		CELL cell = Coord_Cell(Coord);

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
	PathType		*path;			// Pointer to path control structure.
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
//		IsFindPath = true;
		if (Can_Enter_Cell(cell) == MOVE_NO && Distance(NavCom) > 0x0300) {
			static int	_faceadjust[8] = {0, 1, -1, 2, -2, 3, -3, 4};
			FacingType	f2 = (FacingType)(((unsigned)::Direction(cell, Coord_Cell(Coord)))>>5);

			for (unsigned index = 0; index < (sizeof(_faceadjust) / sizeof(_faceadjust[0])); index++) {
				CELL	cell2;

				cell2 = Adjacent_Cell(cell, (FacingType)((f2+_faceadjust[index])&0x7));
				if (Can_Enter_Cell(cell2, FACING_NONE) <= MOVE_CLOAK) {
					cell = cell2;
					break;
				}
			}
		}
//		IsFindPath = false;

#ifdef SPECIAL
		if (What_Am_I() == RTTI_INFANTRY) {
			CELL mycell = Coord_Cell(Center_Coord());
//			Mark(MARK_UP);
			ObjectClass *obj = Map[mycell].Cell_Occupier();
			while (obj) {
				if (obj != this && obj->What_Am_I() == RTTI_INFANTRY) {
					InfantryClass *inf = (InfantryClass *)obj;
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
//			Mark(MARK_DOWN);
		}
#endif

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
			FacingType	workpath2[200];	// Staging area for path list.
			MoveType		maxtype = MOVE_TEMP;
			if (!House->IsHuman) {
				maxtype = MOVE_DESTROYABLE;
			} else {

				/*
				**	For simple movement missions by the human player, then don't
				**	consider friendly units as passable if close to the destination.
				**	This will prevent a human controlled unit from just sitting next
				**	to a destination just because there is another friendly unit
				**	occupying the destination location.
				*/
				if (Mission == MISSION_MOVE && Distance(NavCom) < 0x0280) {
					maxtype = MOVE_DESTROYABLE;
				}
			}

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
				**	comparison with the most agressive path. If they are very close, then
				**	go with the best (easiest) path method.
				*/
				path = Find_Path(cell, &workpath2[0], sizeof(workpath2), MOVE_CLOAK);
				if (path && path->Cost && path->Cost < MAX((path1.Cost + (path1.Cost/2)), 3)) {
					memcpy(&path1, path, sizeof(path1));
					memcpy(workpath1, workpath2, sizeof(workpath1));
				} else {

					/*
					**	The easiest path method didn't result in a satisfactory path. Scan through
					**	the rest of the path options, looking for the best one.
					*/
					for (MoveType move = MOVE_MOVING_BLOCK; move < maxtype; move++) {
						path = Find_Path(cell, &workpath2[0], sizeof(workpath2), move);
						if (path && path->Cost && path->Cost < MAX((path1.Cost + (path1.Cost/2)), 3)) {
							memcpy(&path1, path, sizeof(path1));
							memcpy(workpath1, workpath2, sizeof(workpath1));
						}
					}
				}
			}

#ifdef OBSOLETE
			for (MoveType move = MOVE_CLOAK; move <= maxtype; move++) {
				if (!found1) {
					path = Find_Path(cell, &workpath1[0], sizeof(workpath1), move);
					if (path && path->Cost) {
						memcpy(&path1, path, sizeof(path1));
						found1 = true;
						if (path1.Cost < 5) break;
					}
				} else {
					path = Find_Path(cell, &workpath2[0], sizeof(workpath2), move);

					if (path) {
						if (path->Cost && path->Cost <= path1.Cost/2) {
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
				memcpy(&Path[0], &workpath1[0], MIN(path->Length, (int)sizeof(Path)));
			}

			Mark(MARK_DOWN);
		}


#ifdef NEVER
		/*
		** Patch at this point to see if we are moving directly into a
		** MOVE_TEMP.  This allows units to bunch up at a bridge even if there
		** is an enormously long way around.  This also allows units to give
		** up trying to move into the MOVE_TEMP using the normal movement
		** retry logic.
		*/
		CELL cell = Adjacent_Cell(Coord_Cell(Coord), Path[0]);
		if (Can_Enter_Cell(cell, FACING_NONE) == MOVEF_TEMP) {
			Path[0] = FACING_NONE;
		}
#endif

		PathDelay = PATH_DELAY;
		if (Path[0] != FACING_NONE) return(true);

		/*
		**	If a basic path couldn't be determined, then abort the navigation process.
		*/
//		NavCom = TARGET_NONE;
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
 *=============================================================================================*/
int FootClass::Mission_Move(void)
{
	if (!Target_Legal(NavCom) && !IsDriving && MissionQueue == MISSION_NONE) {
		Enter_Idle_Mode();
	}
	if (!Target_Legal(TarCom) && !House->IsHuman) {
		Target_Something_Nearby(THREAT_RANGE);
	}
	return(TICKS_PER_SECOND+3);
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
	if (!Target_Legal(NavCom) && !In_Radio_Contact()) {
		Enter_Idle_Mode();
		if (Map[Coord_Cell(Center_Coord())].Cell_Building()) {
			Scatter(0, true);
		}
	}
	return(TICKS_PER_SECOND-2);
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
	if (Target_Legal(TarCom)) {
		Approach_Target();
	} else {
		Enter_Idle_Mode();
	}
	return(TICKS_PER_SECOND+2);
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
	if (!Target_Something_Nearby(THREAT_RANGE)) {
		Random_Animate();
	}
	return(TICKS_PER_SECOND+Random_Picky((int)0, (int)4, (char*)NULL, (int)0));
}


/***********************************************************************************************
 * FootClass::Mission_Hunt -- Handles the default hunt order.                                  *
 *                                                                                             *
 *    This routine is the default hunt order for game objects. It handles searching for a      *
 *    nearby object and heading toward it. The act of targetting will cause it to attack       *
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
	if (!Target_Something_Nearby(THREAT_NORMAL)) {
		Random_Animate();
	} else {
		if (What_Am_I() == RTTI_INFANTRY && ((InfantryTypeClass const &)Class_Of()).Type == INFANTRY_E7) {
			Assign_Destination(TarCom);
			Assign_Mission(MISSION_CAPTURE);
		} else {
			Approach_Target();
		}
	}
	return(TICKS_PER_SECOND+5);
}


/***********************************************************************************************
 * FootClass::Mission_Timed_Hunt -- This is the AI process for multiplayer computer units.     *
 *                                                                                             *
 * For multiplayer games, the computer AI can't just blitz the human players; the humans       *
 * need a little time to set up their base, or whatever.  This state just waits for            *
 * a certain period of time, then goes into hunt mode.                                         *
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
int FootClass::Mission_Timed_Hunt(void)
{
	int rndmax;
	int changed = 0;					// has the unit changed into Hunt mode?

	if (!House->IsHuman) {

		/*
		** Jump into HUNT mode if we're supposed to Blitz, and the EndCountDown
		** has expired, or if our owning house has lost more than 1/4 of its units
		** (it gets mad at you)
		*/
		if ( (MPlayerBlitz && House->BlitzTime==0) ||
			House->CurUnits < ((House->MaxUnit * 4) / 5)) {
			Assign_Mission(MISSION_HUNT);
			changed = 1;
		}

		/*
		** Jump into HUNT mode on a random die roll; the computer units will periodically
		** "snap out" of their daze, and begin hunting.  Try to time it so that all
		** units will be hunting within 10 minutes (600 calls to this routine).
		*/
		if (MPlayerBases) {
			rndmax = 5000;
		} else {
			rndmax = 1000;
		}

		if (IRandom(0,rndmax) == 1) {
			Assign_Mission(MISSION_HUNT);
			changed = 1;
		}

		/*
		** If this unit is still just sitting in Timed Hunt mode, call Guard Area
		** so it doesn't just sit there stupidly.
		*/
		if (!changed) {
			Mission_Guard_Area();
		}
	}

	return(TICKS_PER_SECOND+Random_Pick(0, 4));		// call me back in 1 second.

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
	if (HeadToCoord) {
		HeadToCoord = NULL;
		Set_Speed(0);
		IsDriving = false;
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
	Stop_Driver();
	if (headto) {
		HeadToCoord = headto;
		IsDriving = true;

		/*
		**	Check for crate goodie finder here.
		*/
		if (Map[Coord_Cell(headto)].Goodie_Check(this)) {
			return(true);
		}

		HeadToCoord = NULL;
		IsDriving = false;
	}
	return(false);
}


/***********************************************************************************************
 * FootClass::Sort_Y -- Determine the sort coordinate for foot class objects.                  *
 *                                                                                             *
 *    This routine will determine the sort coordinate for foot class object. This coordinate   *
 *    is usually the coordinate of the object. The exception is if the object is teathered.    *
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
	/*
	**	Determine that if there is an existing target it is still legal
	**	and within range.
	*/
	if (Target_Legal(TarCom)) {

		/*
		**	If the target is too far away then head toward it.
		*/
		int maxrange = MAX(Weapon_Range(0), Weapon_Range(1));

		if (!Target_Legal(NavCom) && (!In_Range(TarCom) || !IsLocked)) {
//		if (!Target_Legal(NavCom) && (Distance(TarCom) > maxrange || !IsLocked)) {

			/*
			** If the object that we are attacking is a building adjust the units
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
			maxrange = MAX(maxrange, 0);

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
						if (Can_Enter_Cell(trycell) <= MOVE_CLOAK && Map.In_Radar(trycell)) {
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
				Assign_Destination(TarCom);
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
	if (What_Am_I() == RTTI_UNIT && ((UnitClass *)this)->Class->IsToHarvest) {
		Assign_Mission(MISSION_HARVEST);
		return(1+Random_Pick(1, 10));
	}

	/*
	**	Ensure that the archive target is valid.
	*/
	if (!Target_Legal(ArchiveTarget)) {
		ArchiveTarget = ::As_Target(Coord_Cell(Coord));
	}

	/*
	**	Make sure that the unit has not strayed too far from the home position.
	**	If it has, then race back to it.
	*/
	int maxrange = MAX(Weapon_Range(0), Weapon_Range(1))+0x0100;
	if (!Target_Legal(NavCom) && (Distance(ArchiveTarget) > maxrange || (!Target_Legal(TarCom) && Distance(ArchiveTarget) > 0x0200))) {
		Assign_Target(TARGET_NONE);
		Assign_Destination(ArchiveTarget);
	}

	if (!Target_Legal(TarCom)) {
		COORDINATE old = Coord;
		Coord = As_Coord(ArchiveTarget);
		Target_Something_Nearby(THREAT_AREA);
		Coord = old;
		if (Target_Legal(TarCom)) return(1);
	} else {
		Approach_Target();
	}
	return(TICKS_PER_SECOND+Random_Picky((int)0, (int)4, (char*)NULL, (int)0));
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
 * FootClass::Assign_Mission -- Assign mission to foot class object.                           *
 *                                                                                             *
 *    When a new mission is assigned, any precalculated path should be truncated. This is      *
 *    in anticipation that the new mission will result in a change of path.                    *
 *                                                                                             *
 * INPUT:   order -- The new mission to assign to the unit.                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Assign_Mission(MissionType order)
{
	if (What_Am_I() != RTTI_UNIT || *(UnitClass*)this != UNIT_GUNBOAT) {
		Path[0] = FACING_NONE;
	}
	TechnoClass::Assign_Mission(order);
}


/***********************************************************************************************
 * FootClass::Limbo -- Intercepts limbo event and handles FootClass processing.                *
 *                                                                                             *
 *    When an object of FootClass type is limboed, then it must be removed from any team       *
 *    it may be a member of.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the object successfully limboed?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Limbo(void)
{
	if (!IsInLimbo) {
		if (Team) {
			Team->Remove(this);
		}
	}
	return(TechnoClass::Limbo());
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
 *          source      -- The purpitrator of the damage. By knowing who caused the damage,    *
 *                         the team know's who to "get even with".                             *
 *                                                                                             *
 * OUTPUT:  Returns with the result type of the damage.                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/30/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ResultType FootClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source)
{
	ResultType result = TechnoClass::Take_Damage(damage, distance, warhead, source);

	if (result != RESULT_NONE && Team) {

		if (GameToPlay != GAME_NORMAL || (source && !House->Is_Ally(source))) {
			Team->Took_Damage(this, result, source);
		}

	} else {

		if (result != RESULT_DESTROYED) {
			/*
			**	Determine if the target that is current being attacked has a weapon that can
			**	do harm to a ground based unit. This information is needed so that an appropriate
			**	response will occur when damage is taken.
			*/
			WeaponType weap = WEAPON_NONE;
			if (As_Techno(TarCom)) {
				weap = As_Techno(TarCom)->Techno_Type_Class()->Primary;
			}
			bool tweap = (weap != WEAPON_NONE && weap != WEAPON_NIKE);

			/*
			**	This ensures that if a unit is in sticky mode, then it will snap out of
			**	it when it takes damage.
			*/
			if (source && Mission == MISSION_STICKY) {
				Enter_Idle_Mode();
			}

			/*
			**	If this object is not part of a team and it can retaliate for the damage, then have
			**	it try to do so. This prevents it from just sitting there and taking damage.
			*/
			if (
				source &&
				!House->Is_Ally(source) &&
				Techno_Type_Class()->Primary != WEAPON_NONE &&
				(source->What_Am_I() != RTTI_AIRCRAFT || BulletTypeClass::As_Reference(Weapons[Techno_Type_Class()->Primary].Fires).IsAntiAircraft) &&
				(!Target_Legal(TarCom) || ((!House->IsHuman || Special.IsSmartDefense) && (!tweap || !In_Range(TarCom)))) &&
//				!Target_Legal(NavCom) &&
					(Mission == MISSION_AMBUSH ||
					Mission == MISSION_GUARD ||
					Mission == MISSION_RESCUE ||
					Mission == MISSION_GUARD_AREA ||
					Mission == MISSION_ATTACK ||
					Mission == MISSION_TIMED_HUNT)) {

				/*
				**	Assign the source of the damage as the new target. This occurs for the computer
				**	controled units. For the player, this only occurs if the source of the damage
				**	is within range.
				*/
				if (!House->IsHuman) {

					/*
					** If this unit is in TIMED_HUNT (multiplayer computer-controlled)
					** mode, "snap out of it" into HUNT mode; otherwise, assign
					** HUNT as the next mission through the normal mission queue.
					*/
					if (Mission == MISSION_TIMED_HUNT) {
						Set_Mission(MISSION_HUNT);
					} else {
						Assign_Mission(MISSION_HUNT);
					}
					Assign_Target(source->As_Target());
				} else {
					if (In_Range(source)) {
						Assign_Target(source->As_Target());
					} else {

						/*
						**	Simple retaliation cannot occur because the source of the damage
						**	is too far away. If scatter logic is enabled, then scatter now.
						*/
						if (Special.IsScatter) {
							Scatter(0, true);
						}
					}
				}
			} else {

				/*
				**	If this object isn't doing anything important, then scatter.
				*/
				if (!IsDriving && !Target_Legal(TarCom) && !Target_Legal(NavCom) && Special.IsScatter && What_Am_I() != RTTI_AIRCRAFT) {
					Scatter(0, true);
				}
			}
		}
	}
	return(result);
}


/***********************************************************************************************
 * FootClass::Active_Click_With -- Intiates attack or move according to target clicked on.     *
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
	switch (action) {
		case ACTION_GUARD_AREA:
			if (Can_Player_Fire() && Can_Player_Move()) {
				Player_Assign_Mission(MISSION_GUARD_AREA, object->As_Target());
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
			if (Can_Player_Move() && object && object->Is_Techno() && !((RadioClass *)object)->In_Radio_Contact()) {
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
				Player_Assign_Mission(MISSION_MOVE, TARGET_NONE, object->As_Target());
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
				Player_Assign_Mission(MISSION_MOVE, TARGET_NONE, ::As_Target(cell));
			}
			break;

		case ACTION_ATTACK:
			Player_Assign_Mission(MISSION_ATTACK, ::As_Target(cell));
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
 * INPUT:   center   -- Is this the center of the cell?                                        *
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
void FootClass::Per_Cell_Process(bool center)
{
//	if (center) {

		/*
		**	Clear any unloading flag if necessary.
		*/
		IsUnloading = false;

		/*
		**	If adjacent to an enemy building that has the ability to reveal a stealth tank,
		**	then shimmer the cloaked object.
		*/
		if (Cloak == CLOAKED) {
			for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
				CELL cell = Adjacent_Cell(Coord_Cell(Coord), face);

				if (Map.In_Radar(cell)) {
					TechnoClass const * techno = Map[cell].Cell_Techno();

					if (techno && !House->Is_Ally(techno) && techno->Techno_Type_Class()->IsScanner) {
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
		if (What_Am_I() != RTTI_UNIT || *((UnitClass *)this) != UNIT_GUNBOAT) {
			bool inrange = In_Range(TarCom);
			TechnoClass const * techno = As_Techno(TarCom);
			if (techno && techno->What_Am_I() != RTTI_BUILDING) {
				FootClass const * foot = (FootClass const *)techno;
				MPHType speed = ((TechnoTypeClass const &)techno->Class_Of()).MaxSpeed;
				COORDINATE rangecoord = (speed > MPH_SLOW) ? foot->Likely_Coord() : foot->Target_Coord();
				inrange = In_Range(rangecoord);
			}

			if (Target_Legal(TarCom) && (Mission == MISSION_RESCUE || Mission == MISSION_GUARD_AREA || Mission == MISSION_ATTACK || Mission == MISSION_HUNT) && inrange) {
				Assign_Destination(TARGET_NONE);
				Path[0] = FACING_NONE;
			}
		}

		/*
		**	Trigger event associated with the player entering the cell.
		*/
		TriggerClass * trigger = Map[Coord_Cell(Coord)].Get_Trigger();
		if (Cloak != CLOAKED && trigger && trigger->House == Owner()) {
			trigger->Spring(EVENT_PLAYER_ENTERED, Coord_Cell(Coord));
		}
//	}

	TechnoClass::Per_Cell_Process(center);
}


/***************************************************************************
 * FootClass::Override_Mission -- temporarily overides a units mission     *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:		MissionType mission - the mission we want to overide        *
 *					TARGET	   tarcom  - the new target we want to overide		*
 *					TARGET		navcom  - the new navigation point to overide	*
 *                                                                         *
 * OUTPUT:		none                                                        *
 *                                                                         *
 * WARNINGS:   If a mission is already overidden, the current mission is   *
 *					just re-assigned.															*
 *                                                                         *
 * HISTORY:                                                                *
 *   04/28/1995 PWG : Created.                                             *
 *=========================================================================*/
void FootClass::Override_Mission(MissionType mission, TARGET tarcom, TARGET navcom)
{
 	SuspendedNavCom = NavCom;
	TechnoClass::Override_Mission(mission, tarcom, navcom);

	Assign_Destination(navcom);
}


/***************************************************************************
 * FootClass::Restore_Mission -- Restores an overidden mission             *
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
	switch (message) {

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
				Scatter(0, true);
			}
			break;

		/*
		**	Checks to see if this unit needs to move somewhere. If it is already in motion,
		**	then it doesn't need furthur movement instructions.
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
 *=============================================================================================*/
int FootClass::Mission_Enter(void)
{
	/*
	**	If radio contact has not yet been established with the transport, try to
	**	establish contact now.
	*/
	if (!In_Radio_Contact()) {
		TechnoClass * techno = As_Techno(ArchiveTarget);
		if (!techno) techno = As_Techno(NavCom);
		if (techno) {

			/*
			**	If the transport is already in radio contact, do nothing. Try to
			**	establish radio contact later.
			*/
			if (Transmit_Message(RADIO_HELLO, techno) == RADIO_ROGER) {
				Assign_Destination(TARGET_NONE);
			}
		} else {
			ArchiveTarget = TARGET_NONE;
			Enter_Idle_Mode();
		}

	} else {

		/*
		**	Since radio contact exists with the transport, maintain a dialogue so that
		**	the transport can give proper instructions to the passenger.
		*/
		if (Transmit_Message(RADIO_DOCKING) != RADIO_ROGER) {
			Transmit_Message(RADIO_OVER_OUT);
			Enter_Idle_Mode();
		}
	}
	return(TICKS_PER_SECOND/2);
}


/***********************************************************************************************
z * FootClass::Assign_Destination -- Assigns specified destination to NavCom.                   *
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
	NavCom = target;
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
	if (Team) Team->Remove(this);
	Team = NULL;

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
		if (techno && techno->Techno_Type_Class()->Primary != WEAPON_NONE) {
			return(0);
		}
	}

	/*
	** If the unit is in a harvest mission or is currently attacking
	** something, or is not very effective, then it will be of no help
	** at all.
	*/
	if (Team || Mission == MISSION_HARVEST || !Risk()) {
		return(0);
	}

	/*
	** Find the distance to the target modified by the range.  If the
	** the distance is 0, then things are ok.
	*/
	int dist = Distance(tarcom) - Weapon_Range(0);
	int threat = Risk() * 256;
	int speed = -1;
	if (dist > 0) {

		/*
		** Next we need to figure out how fast the unit moves because this
		** decreases the distance penalty.
		*/
		speed = Max((unsigned)Techno_Type_Class()->MaxSpeed, (unsigned)1);

		int ratio = (speed > 0) ? Max(dist / speed, 1) : 1;

		/*
		** Finally modify the threat by the distance the unit is away.
		*/
		threat = Max(threat/ratio, 1);
	}
	return(threat);
}


/***********************************************************************************************
 * FootClass::Death_Announcement -- Announces the death of a unit.                             *
 *                                                                                             *
 *    This routine is called when a unit (infantry, vehicle, or aircraft) is destroyed.        *
 *                                                                                             *
 * INPUT:   source   -- The purpetrator of this death.                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Death_Announcement(TechnoClass const * source) const
{
	// Changed for multiplayer. ST - 3/13/2019 5:36PM
	if ((GameToPlay == GAME_GLYPHX_MULTIPLAYER && House->IsHuman) || (GameToPlay != GAME_GLYPHX_MULTIPLAYER && Is_Discovered_By_Player() || Is_Owned_By_Player())) { // ST
		//if (Is_Discovered_By_Player() || Is_Owned_By_Player()) {
		//if (IsDiscoveredByPlayer || IsOwnedByPlayer) {
		if (!source || source->What_Am_I() != RTTI_INFANTRY || *((InfantryClass const *)source) != INFANTRY_RAMBO) {
			if (What_Am_I() == RTTI_INFANTRY && ((InfantryTypeClass const &)Class_Of()).IsCivilian && !((InfantryClass *)this)->IsTechnician) {
				// if (Options.IsDeathAnnounce) Speak(VOX_DEAD_CIV); // MBL 02.06.2020
				if (Options.IsDeathAnnounce) Speak(VOX_DEAD_CIV, House, Center_Coord());
			} 
			else {
				if (House != PlayerPtr && GameToPlay != GAME_NORMAL) {
					// if (Options.IsDeathAnnounce) Speak(VOX_ENEMY_UNIT); // MBL 02.06.2020
					if (Options.IsDeathAnnounce) Speak(VOX_ENEMY_UNIT, House);
				} 
				else {
					if (((GameToPlay == GAME_GLYPHX_MULTIPLAYER && House->IsHuman) || House == PlayerPtr) || Options.IsDeathAnnounce) { // ST
						if (!Options.IsDeathAnnounce) { // MBL 02.06.2020
							// Speak(VOX_UNIT_LOST);
							Speak(VOX_UNIT_LOST, House, Center_Coord());
						} 
						else {
							switch (House->ActLike) {
								case HOUSE_GOOD:
									// Speak(VOX_DEAD_GDI); // MBL 02.06.2020
									Speak(VOX_DEAD_GDI, House, Center_Coord());
									break;
		
								case HOUSE_BAD:
									// Speak(VOX_DEAD_NOD); // MBL 02.06.2020
									 Speak(VOX_DEAD_NOD, House, Center_Coord());
									break;
		
								default:
									break;
							}
						}
					}
				}
			}
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
 *=============================================================================================*/
TARGET FootClass::Greatest_Threat(ThreatType method) const
{
	/*
	**	If this object can cloak, then it won't select a target automatically.
	*/
	if (House->IsHuman && IsCloakable && Mission == MISSION_GUARD) {
		return(TARGET_NONE);
	}

	if (Techno_Type_Class()->Primary != WEAPON_NONE && BulletTypeClass::As_Reference(Weapons[Techno_Type_Class()->Primary].Fires).IsAntiAircraft) {
		method = method | THREAT_AIR;
	}
	if (Techno_Type_Class()->Secondary != WEAPON_NONE && BulletTypeClass::As_Reference(Weapons[Techno_Type_Class()->Secondary].Fires).IsAntiAircraft) {
		method = method | THREAT_AIR;
	}

	return(TechnoClass::Greatest_Threat(method|THREAT_GROUND));
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
 *=============================================================================================*/
void FootClass::Detach(TARGET target, bool all)
{
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
 *          The value returned will indicatd the most severe reason why entry into the cell    *
 *          is blocked.                                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
MoveType FootClass::Can_Enter_Cell(CELL , FacingType) const
{
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
	switch (What_Am_I()) {
		case RTTI_UNIT:
		case RTTI_AIRCRAFT:
			if (In_Radio_Contact() &&
				Contact_With_Whom()->What_Am_I() == RTTI_BUILDING &&
				*((BuildingClass *)Contact_With_Whom()) == STRUCT_REPAIR &&
				Distance(Contact_With_Whom()) < 0x0080) {

				return(true);
			}
			break;

		default:
			break;
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
	if (control != 0) {
		if (House == PlayerPtr) {
			Sound_Effect(VOC_CASHTURN);
		}
		House->Refund_Money(Refund_Amount());
		Stun();
		Limbo();
		Delete_This();
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
	if (Head_To_Coord()) {
		return(Head_To_Coord());
	}
	return(Target_Coord());
}