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

/* $Header: /CounterStrike/DRIVE.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DRIVE.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 22, 1994                                               *
 *                                                                                             *
 *                  Last Update : October 31, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   DriveClass::AI -- Processes unit movement and rotation.                                   *
 *   DriveClass::Approach_Target -- Handles approaching the target in order to attack it.      *
 *   DriveClass::Assign_Destination -- Set the unit's NavCom.                                  *
 *   DriveClass::Class_Of -- Fetches a reference to the class type for this object.            *
 *   DriveClass::Debug_Dump -- Displays status information to monochrome screen.               *
 *   DriveClass::Do_Turn -- Tries to turn the vehicle to the specified direction.              *
 *   DriveClass::DriveClass -- Constructor for drive class object.                             *
 *   DriveClass::Fixup_Path -- Adds smooth start path to normal movement path.                 *
 *   DriveClass::Force_Track -- Forces the unit to use the indicated track.                    *
 *   DriveClass::Lay_Track -- Handles track laying logic for the unit.                         *
 *   DriveClass::Limbo -- Prepares vehicle and then limbos it.                                 *
 *   DriveClass::Mark_Track -- Marks the midpoint of the track as occupied.                    *
 *   DriveClass::Ok_To_Move -- Checks to see if this object can begin moving.                  *
 *   DriveClass::Per_Cell_Process -- Handles when unit finishes movement into a cell.          *
 *   DriveClass::Response_Attack -- Voice feedback when ordering the unit to attack a target.  *
 *   DriveClass::Response_Move -- Voice feedback when ordering the unit to move.               *
 *   DriveClass::Response_Select -- Voice feedback when selecting the unit.                    *
 *   DriveClass::Scatter -- Causes the unit to travel to a nearby safe cell.                   *
 *   DriveClass::Smooth_Turn -- Handles the low level coord calc for smooth turn logic.        *
 *   DriveClass::Start_Of_Move -- Tries to get a unit to advance toward cell.                  *
 *   DriveClass::Stop_Driver -- Handles removing occupation bits when driving stops.           *
 *   DriveClass::Teleport_To -- Teleport object to specified location.                         *
 *   DriveClass::While_Moving -- Processes unit movement.                                      *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


#ifdef NEVER
void test(void)
{
	enum nums {one, two, three};

   nums x;
   nums *ptr;

   ptr = &x;
}
#endif


/***********************************************************************************************
 * DriveClass::Response_Select -- Voice feedback when selecting the unit.                      *
 *                                                                                             *
 *    This is the voice to play when the unit is selected.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/30/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void DriveClass::Response_Select(void)
{
	assert(IsActive);

	static VocType _response[] = {
		VOC_VEHIC,
		VOC_REPORT,
		VOC_YESSIR,
		VOC_YESSIR,
		VOC_YESSIR,
		VOC_AWAIT
	};
	VocType response = _response[Sim_Random_Pick(0, ARRAY_SIZE(_response)-1)];
	if (AllowVoice) {
		Sound_Effect(response, fixed(1), -(ID+1));
	}
}


/***********************************************************************************************
 * DriveClass::Response_Move -- Voice feedback when ordering the unit to move.                 *
 *                                                                                             *
 *    This plays the audio feedback when ordering this unit to move to a new destination.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/30/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void DriveClass::Response_Move(void)
{
	assert(IsActive);

	static VocType _response[] = {
		VOC_ACKNOWL,
		VOC_AFFIRM,
	};
	VocType response = _response[Sim_Random_Pick(0, ARRAY_SIZE(_response)-1)];
	if (AllowVoice) {
		Sound_Effect(response, fixed(1), -(ID+1));
	}
}


/***********************************************************************************************
 * DriveClass::Response_Attack -- Voice feedback when ordering the unit to attack a target.    *
 *                                                                                             *
 *    This plays the audio feedback when ordering this unit to attack.                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/30/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void DriveClass::Response_Attack(void)
{
	assert(IsActive);

	static VocType _response[] = {
		VOC_AFFIRM,
		VOC_ACKNOWL
	};
	VocType response = _response[Sim_Random_Pick(0, ARRAY_SIZE(_response)-1)];
	if (AllowVoice) {
		Sound_Effect(response, fixed(1), -(ID+1));
	}
}


/***********************************************************************************************
 * DriveClass::Scatter -- Causes the unit to travel to a nearby safe cell.                     *
 *                                                                                             *
 *    This routine is called when the unit discovers that it should get out of the "hot seat"  *
 *    and move to an adjacent cell. Since the safety of the adjacent cell is not determined    *
 *    before the move begins, it will appear that the unit is just scattering (which it        *
 *    should).                                                                                 *
 *                                                                                             *
 * INPUT:   threat   -- The coordinate of the source of the threat. The unit will try to move  *
 *                      roughly away from the threat.                                          *
 *                                                                                             *
 *          forced   -- The threat is real and a serious effort to scatter should be made.     *
 *                                                                                             *
 *          nokidding-- The scatter should affect the player's infantry even if it otherwise   *
 *                      wouldn't have.                                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/25/1994 JLB : Created.                                                                 *
 *   09/27/1995 JLB : Revised to never scatter if already moving.                              *
 *   07/09/1996 JLB : Moved to DriveClass so that ships will scatter too.                      *
 *   08/02/1996 JLB : Added the "nokidding" parameter.                                         *
 *=============================================================================================*/
void DriveClass::Scatter(COORDINATE threat, bool forced, bool nokidding)
{
	assert(IsActive);

	/*
	**	Certain missions prevent scattering regardless of whether it would be
	**	a good idea or not.
	*/
	if (MissionControl[Mission].IsParalyzed) return;

	if ((What_Am_I() != RTTI_UNIT || !((UnitClass *)this)->IsDumping) && (!Target_Legal(NavCom) || (nokidding && !IsRotating))) {
		if (!Target_Legal(TarCom) || forced || Random_Pick(1, 4) == 1) {
			FacingType	toface;
			FacingType	newface;
			CELL			newcell;

			if (threat != 0) {
				toface = Dir_Facing(Direction8(threat, Coord));
				toface = toface + FacingType(Random_Pick(0, 2)-1);
			} else {
				toface = Dir_Facing(PrimaryFacing.Current());
				toface = toface + FacingType(Random_Pick(0, 2)-1);
			}

			for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
				newface = toface + face;
				newcell = Adjacent_Cell(Coord_Cell(Coord), newface);

				if (Map.In_Radar(newcell) && Can_Enter_Cell(newcell) == MOVE_OK) {
					Assign_Destination(::As_Target(newcell));
				}
			}
		}
	}
}


/***********************************************************************************************
 * DriveClass::Limbo -- Prepares vehicle and then limbos it.                                   *
 *                                                                                             *
 *    This routine removes the occupation bits for the vehicle and also handles cleaning up    *
 *    any vehicle reservation bits. After this, it then proceeds with limboing the unit.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the vehicle limboed?                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool DriveClass::Limbo(void)
{
	if (!IsInLimbo) {
		Stop_Driver();
		TrackNumber = -1;
	}
	return(FootClass::Limbo());
}


/***********************************************************************************************
 * DriveClass::Stop_Driver -- Handles removing occupation bits when driving stops.             *
 *                                                                                             *
 *    This routine will remove the "reservation" flag (if present) when the vehicle is         *
 *    required to stop movement.                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the vehicle stopped?                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool DriveClass::Stop_Driver(void)
{
	assert(IsActive);

	/*
	** We only need to do something if the vehicle is actually going
	** somewhere.
	*/
	if (Head_To_Coord()) {

		/*
		** Safe off whether the vehicle is down or not so we know whether
		** we have to put it back down.
		*/
		int temp = IsDown;

		/*
		** If the vehicle is down, pick it up so it doesn't interfere with
		** our flags.
		*/
		if (temp) {
			Mark(MARK_UP);
		}

		/*
		** Call the drive class function which will let us release the
		** reserved track.
		*/
		Mark_Track(Head_To_Coord(), MARK_UP);

		/*
		** If it was down it should be down when we are done.
		*/
		if (temp) {
			Mark(MARK_DOWN);
		}
	}
	return(FootClass::Stop_Driver());
}


/***********************************************************************************************
 * DriveClass::Do_Turn -- Tries to turn the vehicle to the specified direction.                *
 *                                                                                             *
 *    This routine will set the vehicle to rotate to the direction specified. For tracked      *
 *    vehicles, it is just a simple rotation. For wheeled vehicles, it performs a series       *
 *    of short drives (three point turn) to face the desired direction.                        *
 *                                                                                             *
 * INPUT:   dir   -- The direction that this vehicle should face.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void DriveClass::Do_Turn(DirType dir)
{
	assert(IsActive);

	if (dir != PrimaryFacing) {

#ifdef TOFIX
		/*
		**	Special rotation track is needed for units that
		**	cannot rotate in place.
		*/
		if (Special.IsThreePoint && TrackNumber == -1 && Techno_Type_Class()->Speed == SPEED_WHEEL) {
			int			facediff;	// Signed difference between current and desired facing.
			FacingType	face;			// Current facing (ordinal value).

			facediff = PrimaryFacing.Difference(dir) >> 5;
			facediff = Bound(facediff, -2, 2);
			if (facediff) {
				face = Dir_Facing(PrimaryFacing);

				IsOnShortTrack = true;
				Force_Track(face*FACING_COUNT + (face + facediff), Coord);

				Path[0] = FACING_NONE;
				Set_Speed(0xFF);		// Full speed.
			}
		} else {
			PrimaryFacing.Set_Desired(dir);
		}
#else
			PrimaryFacing.Set_Desired(dir);
//			IsRotating = true;
#endif
	}
}


/***********************************************************************************************
 * DriveClass::Teleport_To -- Teleport object to specified location.                           *
 *                                                                                             *
 *    This will teleport the object to the specified location or as close as possible to it    *
 *    if the destination is blocked.                                                           *
 *                                                                                             *
 * INPUT:   cell  -- The desired destination cell to teleport to.                              *
 *                                                                                             *
 * OUTPUT:  bool; Was the teleport successful?                                                 *
 *                                                                                             *
 * WARNINGS:   All current activity of this object will be terminated by the teleport. It will *
 *             arrive at the destination in static guard mode.                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/21/1996 JLB : Created.                                                                 *
 *   10/31/1996 JLB : Handles flag teleport case.                                              *
 *=============================================================================================*/
bool DriveClass::Teleport_To(CELL cell)
{
	/*
	**	All cargo gets destroyed.
	*/
	if (Rule.IsChronoKill) {
		Kill_Cargo(NULL);
	}

	Stop_Driver();
	Force_Track(-1, 0);
	PrimaryFacing.Set_Current(PrimaryFacing.Desired());
	Transmit_Message(RADIO_OVER_OUT);
	Assign_Destination(TARGET_NONE);
	Assign_Target(TARGET_NONE);
	Assign_Mission(MISSION_NONE);
	Commence();
	Mark(MARK_UP);

	/*
	**	A teleported unit will drop the flag right where it's at.
	*/
	if (What_Am_I() == RTTI_UNIT && ((UnitClass *)this)->Flagged != HOUSE_NONE) {
		HouseClass::As_Pointer(((UnitClass *)this)->Flagged)->Flag_Attach(Coord_Cell(Coord));
	}

	if (Can_Enter_Cell(cell) != MOVE_OK) {
		cell = Map.Nearby_Location(cell, Techno_Type_Class()->Speed);
	}
	Coord = Cell_Coord(cell);
	Mark(MARK_DOWN);
	Look(false);
	Per_Cell_Process(PCP_END);
	return(true);
}


/***********************************************************************************************
 * DriveClass::Force_Track -- Forces the unit to use the indicated track.                      *
 *                                                                                             *
 *    This override (nuclear bomb) style routine is to be used when a unit needs to start      *
 *    on a movement track but is outside the normal movement system. This occurs when a        *
 *    harvester starts driving off of a refinery.                                              *
 *                                                                                             *
 * INPUT:   track -- The track number to start on.                                             *
 *                                                                                             *
 *          coord -- The coordinate that the unit will end up at when the movement track       *
 *                   is completed.                                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/17/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void DriveClass::Force_Track(int track, COORDINATE coord)
{
	assert(IsActive);

	TrackNumber = track;
	TrackIndex = 0;
	if (coord != 0) {
		Start_Driver(coord);
	}
}


/***********************************************************************************************
 * DriveClass::DriveClass -- Constructor for drive class object.                               *
 *                                                                                             *
 *    This will initialize the drive class to its default state. It is called as a result      *
 *    of creating a unit.                                                                      *
 *                                                                                             *
 * INPUT:   classid  -- The unit's ID class. It is passed on to the foot class constructor.    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/13/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
DriveClass::DriveClass(RTTIType rtti, int id, HousesType house) :
	FootClass(rtti, id, house),
	IsMoebius(false),
	IsHarvesting(false),
	IsTurretLockedDown(false),
	IsOnShortTrack(false),
	SpeedAccum(0),
	MoebiusCountDown(0),
	MoebiusCell(0),
	TrackNumber(-1),
	TrackIndex(0)
{
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * DriveClass::Debug_Dump -- Displays status information to monochrome screen.                 *
 *                                                                                             *
 *    This debug utility function will display the status of the drive class to the mono       *
 *    screen. It is through this information that bugs can be tracked down.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void DriveClass::Debug_Dump(MonoClass * mono) const
{
	assert(IsActive);

	mono->Fill_Attrib(66, 14, 12, 1, IsMoebius ? MonoClass::INVERSE : MonoClass::NORMAL);
	FootClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * DriveClass::Smooth_Turn -- Handles the low level coord calc for smooth turn logic.          *
 *                                                                                             *
 *    This routine calculates the new coordinate value needed for the                          *
 *    smooth turn logic. The adjustment and flag values must be                                *
 *    determined prior to entering this routine.                                               *
 *                                                                                             *
 * INPUT:   adj      -- The adjustment coordinate as lifted from the                           *
 *                      correct smooth turn table.                                             *
 *                                                                                             *
 *          dir      -- Pointer to dir for possible modification                               *
 *                      according to the flag bits.                                            *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate the unit should positioned to.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1994 JLB : Created.                                                                 *
 *   07/13/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
COORDINATE DriveClass::Smooth_Turn(COORDINATE adj, DirType & dir)
{
	assert(IsActive);

	DirType workdir = dir;
	int x,y;
	int temp;
	TrackControlType flags = TrackControl[TrackNumber].Flag;

	x = Coord_X(adj);
	y = Coord_Y(adj);

	if (flags & F_T) {
		temp	= x;
		x		= y;
		y 		= temp;
		workdir = (DirType)(DIR_W - workdir);
	}

	if (flags & F_X) {
		x 		 = -x;
		workdir = (DirType)-workdir;
	}

	if (flags & F_Y) {
		y = -y;
		workdir = (DirType)(DIR_S - workdir);
	}

	dir = workdir;

	return(XY_Coord( (LEPTON)(Coord_X(Head_To_Coord()) + x), (LEPTON)(Coord_Y(Head_To_Coord()) + y)));
}


/***********************************************************************************************
 * DriveClass::Assign_Destination -- Set the unit's NavCom.                                    *
 *                                                                                             *
 *    This routine is used to set the unit's navigation computer to the                        *
 *    specified target. Once the navigation computer is set, the unit                          *
 *    will start planning and moving toward the destination.                                   *
 *                                                                                             *
 * INPUT:   target   -- The destination target for the unit to head to.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/07/1992 JLB : Created.                                                                 *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
void DriveClass::Assign_Destination(TARGET target)
{
	assert(IsActive);

	/*
	**	Abort early if there is anything wrong with the parameters
	**	or the unit already is assigned the specified destination.
	*/
	if (target == NavCom) return;

	/*
	**	For harvesting type vehicles, it might go into a dock and unload procedure
	**	when the harvester is full and an empty refinery is selected as a target.
	*/
	BuildingClass * b = As_Building(target);

	/*
	**	If the player clicked on refinery but it is not busy, then assign
	**	it to unload at the refinery.
	*/
	if (b != NULL && *b == STRUCT_REFINERY && What_Am_I() == RTTI_UNIT && ((UnitTypeClass *)Techno_Type_Class())->IsToHarvest) {
		if (Contact_With_Whom() != b && !b->In_Radio_Contact()) {
			/*
			**	Establish radio contact protocol. If the facility responds correctly,
			**	then remain in radio contact and proceed toward the desired destination.
			*/
			if (Transmit_Message(RADIO_HELLO, b) == RADIO_ROGER) {
				if (Mission != MISSION_ENTER && Mission != MISSION_HARVEST) {
					Assign_Mission(MISSION_ENTER);
					target = TARGET_NONE;
				} else {
//					target = TARGET_NONE;
				}
			} else {
//				target = TARGET_NONE;
			}
		} else {
//			target = TARGET_NONE;
		}
	}

	/*
	**	Set the unit's navigation computer.
	*/
	FootClass::Assign_Destination(target);

	Path[0] = FACING_NONE;			// Force recalculation of path.
	if (!IsDriving && Mission != MISSION_UNLOAD) {
		Start_Of_Move();
	}
}


/***********************************************************************************************
 * DriveClass::While_Moving -- Processes unit movement.                                        *
 *                                                                                             *
 *    This routine is used to process movement for the units as they move.                     *
 *    It is called many times for each cell's worth of movement.   This                        *
 *    routine only applies after the next cell HeadTo has been determined.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  true/false; Should this routine be called again?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/02/1992 JLB : Created.                                                                 *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
bool DriveClass::While_Moving(void)
{
	assert(IsActive);

	/*
	**	Perform quick legality checks.
	*/
	if (!IsDriving || TrackNumber == -1 || (IsRotating && !Techno_Type_Class()->IsTurretEquipped)) {
		SpeedAccum = 0;		// Kludge?  No speed should accumulate if movement is on hold.
		return(false);
	}

	/*
	**	If enough movement has accumulated so that the unit can
	**	visibly move on the map, then process accordingly.
	** Slow the unit down if he's carrying a flag.
	*/
	MPHType maxspeed = MPHType(min(Techno_Type_Class()->MaxSpeed * SpeedBias * House->GroundspeedBias, (int)MPH_LIGHT_SPEED));
	if (IsFormationMove) maxspeed = FormationMaxSpeed;

	int actual;			// Working movement addition value.
	if (((UnitClass *)this)->Flagged != HOUSE_NONE) {
		actual = SpeedAccum + ((int)maxspeed/2) * fixed(Speed, 256);
	} else {
		actual = SpeedAccum + maxspeed * fixed(Speed, 256);
	}

	if (actual > PIXEL_LEPTON_W) {
		TurnTrackType	const * track;	// Track control pointer.
		TrackType		const	* ptr;		// Pointer to coord offset values.
		int				tracknum;		// The track number being processed.
		FacingType		nextface;		// Next facing queued in path.
		bool				adj;				// Is a turn coming up?

		track = &TrackControl[TrackNumber];
		if (IsOnShortTrack) {
			tracknum = track->StartTrack;
		} else {
			tracknum = track->Track;
		}
		ptr = RawTracks[tracknum-1].Track;
		nextface = Path[0];

		/*
		**	Determine if there is a turn coming up. If there is
		**	a turn, then track jumping might occur.
		*/
		adj = false;
		if (nextface != FACING_NONE && Dir_Facing(track->Facing) != nextface) {
			adj = true;
		}

		/*
		**	Skip ahead the number of track steps required (limited only
		**	by track length). Set the unit to the new position and
		**	flag the unit accordingly.
		*/
		Mark(MARK_UP);
		while (actual > PIXEL_LEPTON_W) {
			COORDINATE		offset;
			DirType	dir;

			actual -= PIXEL_LEPTON_W;

			offset = ptr[TrackIndex].Offset;
			if (offset || !TrackIndex) {
				dir = ptr[TrackIndex].Facing;
				Coord = Smooth_Turn(offset, dir);

				PrimaryFacing.Set(dir);

				/*
				**	See if "per cell" processing is necessary.
				*/
				if (TrackIndex && RawTracks[tracknum-1].Cell == TrackIndex) {
					Mark(MARK_DOWN);
					Per_Cell_Process(PCP_DURING);
					if (!IsActive) {
						return(false);
					}
					Mark(MARK_UP);
				}

				/*
				**	The unit could "jump tracks". Check to see if the unit should
				**	do so.
				*/
				if (/**this != UNIT_GUNBOAT &&*/ nextface != FACING_NONE && adj && RawTracks[tracknum-1].Jump == TrackIndex && TrackIndex) {
					TurnTrackType const * newtrack;		// Proposed jump-to track.
					int	tnum;

					tnum = (int)(Dir_Facing(track->Facing) * FACING_COUNT) + (int)nextface;
					newtrack = &TrackControl[tnum];
					if (newtrack->Track && RawTracks[newtrack->Track-1].Entry) {
						COORDINATE	c = Head_To_Coord();
						int oldspeed = Speed;

						c = Adjacent_Cell(c, nextface);

						switch (Can_Enter_Cell(Coord_Cell(c), nextface)) {
							case MOVE_OK:
								IsOnShortTrack = false;		// Shouldn't be necessary, but...
								TrackNumber = tnum;
								track = newtrack;

								tracknum = track->Track;
								TrackIndex = RawTracks[tracknum-1].Entry-1;	// Anticipate increment.
								ptr = RawTracks[tracknum-1].Track;
								adj = false;

								Stop_Driver();
								IsDriving = true;
								Per_Cell_Process(PCP_END);
								IsDriving = false;
								if (!IsActive) return(false);
								if (Start_Driver(c)) {
									Set_Speed(oldspeed);
									memcpy((char*)&Path[0], (char*)&Path[1], CONQUER_PATH_MAX-1);
									Path[CONQUER_PATH_MAX-1] = FACING_NONE;
								} else {
									Path[0] = FACING_NONE;
									TrackNumber = -1;
									actual = 0;
								}
								break;

							case MOVE_CLOAK:
								Map[c].Shimmer();
								break;

							case MOVE_TEMP:
#ifdef TOFIX
								if (*this == UNIT_HARVESTER || !House->IsHuman) {
#else
								if (!House->IsHuman) {
#endif
									Map[c].Incoming(0, true, true);
								}
								break;
						}
					}
				}
				TrackIndex++;

			} else {
				actual = 0;
				Coord = Head_To_Coord();
				Stop_Driver();
				TrackNumber = -1;
				TrackIndex = NULL;

				/*
				**	Perform "per cell" activities.
				*/
				Mark(MARK_DOWN);
				Per_Cell_Process(PCP_END);
				if (!IsActive) return(false);
				Mark(MARK_UP);

				break;
			}
		}
		if (IsActive) {
			Mark(MARK_DOWN);
		}
	}

	/*
	**	Replace any remainder back into the unit's movement
	**	accumulator to be processed next pass.
	*/
	SpeedAccum = actual;
	return(true);
}


/***********************************************************************************************
 * DriveClass::Per_Cell_Process -- Handles when unit finishes movement into a cell.            *
 *                                                                                             *
 *    This routine is called when a unit has mostly or completely                              *
 *    entered a cell. The unit might be in the middle of a movement track                      *
 *    when this routine is called. It's primary purpose is to perform                          *
 *    sighting and other "per cell" activities.                                                *
 *                                                                                             *
 * INPUT:   why   -- Specifies the circumstances under which this routine was called.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/03/1993 JLB : Created.                                                                 *
 *   03/30/1994 JLB : Revamped for track system.                                               *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *   06/18/1994 JLB : Converted to virtual function.                                           *
 *   06/18/1994 JLB : Distinguishes between center and near-center conditions.                 *
 *=============================================================================================*/
void DriveClass::Per_Cell_Process(PCPType why)
{
	assert(IsActive);

	if (why == PCP_END) {
		CELL	cell = Coord_Cell(Coord);

		/*
		**	Check to see if it has reached its destination. If so, then clear the NavCom
		**	regardless of the remaining path list.
		*/
		if (As_Cell(NavCom) == cell) {
			IsTurretLockedDown = false;
			NavCom = TARGET_NONE;
			Path[0] = FACING_NONE;
		}

		Lay_Track();
	}

	FootClass::Per_Cell_Process(why);
}


/***********************************************************************************************
 * DriveClass::Start_Of_Move -- Tries to get a unit to advance toward cell.                    *
 *                                                                                             *
 *    This will try to start a unit advancing toward the cell it is                            *
 *    facing. It will check for and handle legality and reserving of the                       *
 *    necessary cell.                                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  true/false; Should this routine be called again because                            *
 *                      initial start operation is temporarily delayed?                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/02/1992 JLB : Created.                                                                 *
 *   10/18/1993 JLB : This should be called repeatedly until HeadTo is not NULL.               *
 *   03/16/1994 JLB : Revamped for track logic.                                                *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *   06/19/1995 JLB : Fixed so that it won't fire on ground unnecessarily.                     *
 *   07/13/1995 JLB : Handles bumping into cloaked objects.                                    *
 *   09/22/1995 JLB : Breaks out of hopeless hunt mode.                                        *
 *   07/10/1996 JLB : Sets scan limit if necessary.                                            *
 *=============================================================================================*/
bool DriveClass::Start_Of_Move(void)
{
	assert(IsActive);

	FacingType		facing;				// Direction movement will commence.
	DirType			dir;				// Desired actual facing toward destination.
	int				facediff;			// Difference between current and desired facing.
	int				speed;				// Speed of unit.
	CELL				destcell;			// Cell of destination.
	LandType			ground;				// Ground unit is entering.
	COORDINATE				dest;					// Destination coordinate.

	facing = Path[0];

	if (!Target_Legal(NavCom) && facing == FACING_NONE) {
		IsTurretLockedDown = false;
		Stop_Driver();
		if (Mission == MISSION_MOVE) {
			Enter_Idle_Mode();
		}
		return(false);		// Why is it calling this routine!?!
	}

	/*
	**	Reduce the path length if the target is a unit and the
	**	range to the unit is less than the precalculated path steps.
	*/
	if (facing != FACING_NONE) {
		int	dist;

		if (Is_Target_Vessel(NavCom) || Is_Target_Unit(NavCom) || Is_Target_Infantry(NavCom)) {
			dist = Lepton_To_Cell((LEPTON)Distance(NavCom));

			if (dist < ARRAY_SIZE(Path)) {
				Path[dist] = FACING_NONE;
				facing = Path[0];		// Maybe needed.
			}
		}
	}

	/*
	**	If the path is invalid at this point, then generate one. If
	**	generating a new path fails, then abort NavCom.
	*/
	if (facing == FACING_NONE) {

		/*
		**	If after a path search, there is still no valid path, then set the
		**	NavCom to null and let the script take care of assigning a new
		**	navigation target.
		*/
		if (PathDelay != 0) {
			return(false);
		}

		if (!Basic_Path()) {

			/*
			**	If the unit is close enough to the target then just stop
			**	driving now. This prevents the fidgeting that would occur
			**	if they mindlessly kept trying to get to the exact location
			**	desired. This is quite necessary since it is typical to move
			**	several units with the same mouse click.
			*/
			if (!Is_On_Priority_Mission() && Distance(NavCom) < Rule.CloseEnoughDistance && (Mission == MISSION_MOVE || Mission == MISSION_GUARD_AREA)) {
				Assign_Destination(TARGET_NONE);
				if (!IsActive) return(false);
			} else {
				/*
				**	If a basic path could not be found, but the immediate move destination is
				**	blocked by a friendly temporary blockage, then cause that blockage
				**	to scatter.
				*/
				CELL cell = Adjacent_Cell(Coord_Cell(Center_Coord()), PrimaryFacing.Current());
				if (Map.In_Radar(cell)) {
					MoveType ok = Can_Enter_Cell(cell);
					if (ok == MOVE_TEMP) {
						CellClass * cellptr = &Map[cell];
						TechnoClass * blockage = cellptr->Cell_Techno();
						if (blockage && House->Is_Ally(blockage)) {

							/*
							**	If the target can be told to get out of the way, only bother
							**	to do so if we aren't very close to the target and this
							**	object can just say "good enough" and stop here.
							*/
							if (Distance(NavCom) < Rule.CloseEnoughDistance && !In_Radio_Contact()) {
								Assign_Destination(TARGET_NONE);
								return(false);
							} else {
								cellptr->Incoming(0, true, false);
//								cellptr->Incoming(0, true, true);
							}
						}
					}
				}

				if (TryTryAgain > 0) {
					TryTryAgain--;
				} else {
					Assign_Destination(TARGET_NONE);
					if (!IsActive) return(false);
					if (IsNewNavCom) Sound_Effect(VOC_SCOLD);
					IsNewNavCom = false;
				}
			}

			/*
			**	Since the path was blocked, check to make sure that it was completely
			**	blocked. If so and it has a valid TarCom and it is out of range of the
			**	TarCom, then give this unit a range limit so that it might not pick
			**	a "can't reach" target again.
			*/
			if (!Target_Legal(NavCom) && Target_Legal(TarCom) && !In_Range(TarCom)) {
				IsScanLimited = true;
				if (Team.Is_Valid()) Team->Scan_Limit();
				Assign_Target(TARGET_NONE);
			}

			/*
			**	Stop the movement, for now, and let the subsequent logic in later game
			**	frames resume movement as appropriate.
			*/
			Stop_Driver();
			TrackNumber = -1;
			IsTurretLockedDown = false;
			return(false);
		}

		/*
		**	If a basic path could be found, but the immediate move destination is
		**	blocked by a friendly temporary blockage, then cause that blockage
		**	to scatter.
		*/
		CELL cell = Adjacent_Cell(Coord_Cell(Center_Coord()), Path[0]);
		if (Map.In_Radar(cell)) {
			MoveType ok = Can_Enter_Cell(cell);
			if (ok == MOVE_TEMP) {
				CellClass * cellptr = &Map[cell];
				TechnoClass * blockage = cellptr->Cell_Techno();
				if (blockage && House->Is_Ally(blockage)) {

					/*
					**	If the target can be told to get out of the way, only bother
					**	to do so if we aren't very close to the target and this
					**	object can just say "good enough" and stop here.
					*/
					if (Distance(NavCom) < Rule.CloseEnoughDistance && !In_Radio_Contact()) {
						Assign_Destination(TARGET_NONE);
						return(false);
					} else {
						cellptr->Incoming(0, true, false);
//						cellptr->Incoming(0, true, true);
					}
				}
			}
		}

		TryTryAgain = PATH_RETRY;
		facing = Path[0];
	}

	/*
	**	Determine the coordinate of the next cell to move into.
	*/
	dest = Adjacent_Cell(Coord, facing);
	dir = Facing_Dir(facing);

	/*
	**	Set the facing correctly if it isn't already correct. This
	**	means starting a rotation track if necessary.
	*/
	facediff = PrimaryFacing.Difference(dir);
	if (facediff) {

		/*
		**	Request a change of facing.
		*/
		Do_Turn(dir);
		return(true);

	} else {

		/* NOTE:  Beyond this point, actual track assignment can begin.
		**
		**	If the cell to move into is impassable (probably for some unexpected
		**	reason), then abort the path list and set the speed to zero. The
		** next time this routine is called, a new path will be generated.
		*/
		destcell = Coord_Cell(dest);
		Mark(MARK_UP);
		MoveType cando = Can_Enter_Cell(destcell, facing);
		Mark(MARK_DOWN);

		if (cando != MOVE_OK) {

			if (Mission == MISSION_MOVE /*KO&& House->IsHuman */&& Distance(NavCom) < Rule.CloseEnoughDistance) {
				Assign_Destination(TARGET_NONE);
				if (!IsActive) return(false);//BG
			}

			/*
			**	If a temporary friendly object is blocking the path, then cause it to
			**	get out of the way.
			*/
			if (cando == MOVE_TEMP) {
				Map[destcell].Incoming(0, true, true);
			}

			/*
			**	If a cloaked object is blocking, then shimmer the cell.
			*/
			if (cando == MOVE_CLOAK) {
				Map[destcell].Shimmer();
			}

			Stop_Driver();
			if (cando != MOVE_MOVING_BLOCK) {
			 	Path[0] = FACING_NONE;		// Path is blocked!
			}

			/*
			** If blocked by a moving block then just exit start of move and
			** try again next tick.
			*/
			if (cando == MOVE_DESTROYABLE) {
				if (Map[destcell].Cell_Object()) {
					if (!House->Is_Ally(Map[destcell].Cell_Object())) {
						Override_Mission(MISSION_ATTACK, Map[destcell].Cell_Object()->As_Target(), TARGET_NONE);
					}
				} else {
					if (Map[destcell].Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(Map[destcell].Overlay).IsWall) {
						Override_Mission(MISSION_ATTACK, ::As_Target(destcell), TARGET_NONE);
					}
				}
			} else {
				if (IsNewNavCom) Sound_Effect(VOC_SCOLD);
			}
			IsNewNavCom = false;
			TrackNumber = -1;
			return(true);
		}

		/*
		**	Determine the speed that the unit can travel to the desired square.
		*/
		ground = Map[destcell].Land_Type();
		speed = Ground[ground].Cost[Techno_Type_Class()->Speed] * 255;

		/* change speed if it's related to a team move */
		if (IsFormationMove) speed = Ground[ground].Cost[FormationSpeed] * 255;
		if (!speed) speed = 128;

#ifdef NEVER
		/*
		**	Set the jiggle flag if the terrain would cause the unit
		**	to jiggle when travelled over.
		*/
		BaseF &= ~BASEF_JIGGLE;
		if (Ground[ground].Jiggle) {
			BaseF |= BASEF_JIGGLE;
		}
#endif

		/*
		**	A damaged unit has a reduced speed.
		*/
		if (Health_Ratio() <= Rule.ConditionYellow /*(Techno_Type_Class()->MaxStrength>>1) > Strength*/) {
			speed -= (speed/4);	// Three quarters speed.
		}
		if ((speed != Speed)/* || !SpeedAdd*/) {
			Set_Speed(speed);		// Full speed.
		}

		/*
		**	Reserve the destination cell so that it won't become
		**	occupied AS this unit is moving into it.
		*/
		if (cando != MOVE_OK) {
		 	Path[0] = FACING_NONE;		// Path is blocked!
			TrackNumber = -1;
		 	dest = NULL;
		} else {

			Overrun_Square(Coord_Cell(dest), true);

			/*
			**	Determine which track to use (based on recorded path).
			*/
			FacingType nextface = Path[1];
			if (nextface == FACING_NONE) nextface = facing;

			IsOnShortTrack = false;
			TrackNumber = facing * FACING_COUNT + (int)nextface;
			if (TrackControl[TrackNumber].Track == 0) {
				Path[0] = FACING_NONE;
				TrackNumber = -1;
				return(true);
			} else {
				if (TrackControl[TrackNumber].Flag & F_D) {
					/*
					**	If the middle cell of a two cell track contains a crate,
					**	the check for goodies before movement starts.
					*/
					if (!Map[destcell].Goodie_Check(this)) {
						cando = MOVE_NO;
						if (!IsActive) return(false);
					} else {
						if (!IsActive) return(false);
						dest = Adjacent_Cell(dest, nextface);
						destcell = Coord_Cell(dest);
						cando = Can_Enter_Cell(destcell);
					}
					if (!IsActive) return(false);

					if (cando != MOVE_OK) {

						/*
						**	If a temporary friendly object is blocking the path, then cause it to
						**	get out of the way.
						*/
						if (cando == MOVE_TEMP) {
							Map[destcell].Incoming(0, true, true);
						}

						/*
						**	If a cloaked object is blocking, then shimmer the cell.
						*/
						if (cando == MOVE_CLOAK) {
							Map[destcell].Shimmer();
						}

						Path[0] = FACING_NONE;		// Path is blocked!
						TrackNumber = -1;
						dest = NULL;
						if (cando == MOVE_DESTROYABLE) {

							if (Map[destcell].Cell_Object()) {
								if (!House->Is_Ally(Map[destcell].Cell_Object())) {
									Override_Mission(MISSION_ATTACK, Map[destcell].Cell_Object()->As_Target(), TARGET_NONE);
								}
							} else {
								if (Map[destcell].Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(Map[destcell].Overlay).IsWall) {
									Override_Mission(MISSION_ATTACK, ::As_Target(destcell), TARGET_NONE);
								}
							}
							IsNewNavCom = false;
							TrackIndex = 0;
							return(true);
						}
					} else {
						memcpy((char*)&Path[0], (char*)&Path[2], CONQUER_PATH_MAX-2);
						Path[CONQUER_PATH_MAX-2] = FACING_NONE;
						IsPlanningToLook = true;
					}
				} else {
					memcpy((char*)&Path[0], (char*)&Path[1], CONQUER_PATH_MAX-1);
				}
				Path[CONQUER_PATH_MAX-1] = FACING_NONE;
			}
		}

		IsNewNavCom = false;
		TrackIndex = 0;
		if (!Start_Driver(dest)) {
			TrackNumber = -1;
			Path[0] = FACING_NONE;
			Set_Speed(0);
		}
	}
	return(false);
}


/***********************************************************************************************
 * DriveClass::AI -- Processes unit movement and rotation.                                     *
 *                                                                                             *
 *    This routine is used to process unit movement and rotation. It                           *
 *    functions autonomously from the script system. Thus, once a unit                         *
 *    is give rotation command or movement path, it will follow this                           *
 *    until specifically instructed to stop. The advantage of this                             *
 *    method is that it allows smooth movement of units, faster game                           *
 *    execution, and reduced script complexity (since actual movement                          *
 *    dynamics need not be controlled directly by the scripts).                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine relies on the process control bits for the                         *
 *             specified unit (for speed reasons). Thus, only setting                          *
 *             movement, rotation, or path list will the unit perform                          *
 *             any physics.                                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/26/1993 JLB : Created.                                                                 *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
void DriveClass::AI(void)
{
	assert(IsActive);

	FootClass::AI();
	if (!IsActive || Height > 0) return;

	/*
	** Is this a unit that's been teleported using the chronosphere, and if so,
	** has his timer expired such that he needs to teleport back?
	*/
	if (IsMoebius) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	 if (What_Am_I() != RTTI_UNIT || ((UnitClass *)this)->Class->Type != UNIT_CHRONOTANK) {
#endif
		if (MoebiusCountDown == 0) {
			IsMoebius = false;
			Teleport_To(MoebiusCell);
			MoebiusCell = 0;
		}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	 }
#endif
	}

	/*
	**	If the unit is following a track, then continue
	**	to do so -- mindlessly.
	*/
	if (TrackNumber != -1) {

		/*
		**	Perform the movement accumulation.
		*/
		While_Moving();
		if (!IsActive) return;
		if (TrackNumber == -1 && (Target_Legal(NavCom) || Path[0] != FACING_NONE) && (What_Am_I() != RTTI_UNIT || !((UnitClass*)this)->IsDumping)) {
			Start_Of_Move();
			if (!IsActive) return;
			While_Moving();
			if (!IsActive) return;
		}

	} else {

		/*
		**	For tracked units that are rotating in place, perform the rotation now.
		*/
#ifdef TOFIX
		if ((Class->Speed == SPEED_FLOAT || Class->Speed == SPEED_HOVER || Class->Speed == SPEED_TRACK || (Class->Speed == SPEED_WHEEL && !Special.IsThreePoint)) && PrimaryFacing.Is_Rotating()) {
			if (PrimaryFacing.Rotation_Adjust(Class->ROT)) {
				Mark(MARK_CHANGE);
			}
#else
		if (PrimaryFacing.Is_Rotating()) {
			Mark(MARK_CHANGE_REDRAW);
			if (PrimaryFacing.Rotation_Adjust(Techno_Type_Class()->ROT * House->GroundspeedBias)) {
				Mark(MARK_CHANGE_REDRAW);
			}
#endif
			if (!IsRotating) {
				Per_Cell_Process(PCP_ROTATION);
				if (!IsActive) return;
			}

		} else {

			/*
			**	The unit has no track to follow, but if there
			**	is a navigation target or a remaining path,
			**	then start on a new track.
			*/
			if ((Mission != MISSION_GUARD || Target_Legal(NavCom)) && Mission != MISSION_UNLOAD) {
				if (Target_Legal(NavCom) || Path[0] != FACING_NONE) {

					/*
					**	Double check to make sure that the movement destination is
					**	in a zone that this unit can travel to. If not, then abort
					**	the navigation target. Exception is to allow units to leave
					**	impassable cells regardless of zone checks.
					*/
					LandType land = LAND_NONE;
					if (What_Am_I() == RTTI_INFANTRY || What_Am_I() == RTTI_UNIT) {
						land = Map[Center_Coord()].Land_Type();
					}
					if (IsLocked && Mission != MISSION_ENTER && Target_Legal(NavCom) && !Is_In_Same_Zone(As_Cell(NavCom)) &&
						land != LAND_ROCK && land != LAND_WATER && land != LAND_RIVER) {
						Stop_Driver();
						Assign_Destination(TARGET_NONE);
					} else {
						Start_Of_Move();
						if (!IsActive) return;
						While_Moving();
						if (!IsActive) return;
					}
				} else {
					Stop_Driver();
				}
			}
		}
	}
}


/***********************************************************************************************
 * DriveClass::Fixup_Path -- Adds smooth start path to normal movement path.                   *
 *                                                                                             *
 *    This routine modifies the path of the specified unit so that it                          *
 *    will not start out with a rotation. This is necessary for those                          *
 *    vehicles that have difficulty with rotating in place. Typically,                         *
 *    this includes wheeled vehicles.                                                          *
 *                                                                                             *
 * INPUT:   unit  -- Pointer to the unit to adjust.                                            *
 *                                                                                             *
 *          path  -- Pointer to path structure.                                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only units that require a fixup get modified. The                               *
 *             modification only occurs, if there is a legal path to                           *
 *             do so.                                                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/03/1994 JLB : Created.                                                                 *
 *   04/06/1994 JLB : Uses path structure.                                                     *
 *   04/10/1994 JLB : Diagonal smooth turn added.                                              *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
void DriveClass::Fixup_Path(PathType * path)
{
	assert(IsActive);

	FacingType stage[6]={FACING_N,FACING_N,FACING_N,FACING_N,FACING_N,FACING_N};		// Prefix path elements.
	int	facediff;		// The facing difference value (0..4 | 0..-4).
	static FacingType _path[4][6] = {
		{(FacingType)2,(FacingType)0,(FacingType)2,(FacingType)0,(FacingType)0,(FacingType)0},
		{(FacingType)3,(FacingType)0,(FacingType)2,(FacingType)2,(FacingType)0,(FacingType)0},
		{(FacingType)4,(FacingType)0,(FacingType)2,(FacingType)2,(FacingType)0,(FacingType)0},
		{(FacingType)4,(FacingType)0,(FacingType)2,(FacingType)2,(FacingType)1,(FacingType)0}
	};
	static FacingType _dpath[4][6] = {
		{(FacingType)0,(FacingType)0,(FacingType)0,(FacingType)0,(FacingType)0,(FacingType)0},
		{(FacingType)3,(FacingType)0,(FacingType)2,(FacingType)2,(FacingType)0,(FacingType)0},
		{(FacingType)4,(FacingType)0,(FacingType)2,(FacingType)2,(FacingType)1,(FacingType)0},
		{(FacingType)5,(FacingType)0,(FacingType)2,(FacingType)2,(FacingType)1,(FacingType)0}
	};

	int			index;
	int			counter;			// Path addition
	FacingType	* ptr;				// Path list pointer.
	FacingType	* ptr2;			// Copy of new path list pointer.
	FacingType	nextpath;		// Next path value.
	CELL			cell;				// Working cell value.
	bool			ok;

	/*
	**	Verify that the unit is valid and there is a path problem to resolve.
	*/
	if (!path || path->Command[0] == FACING_NONE) {
		return;
	}

	/*
	**	Only wheeled vehicles need a path fixup -- to avoid 3 point turns.
	*/
#ifdef TOFIX
	if (!Special.IsThreePoint || Class->Speed != SPEED_WHEEL) {
#else
	if (What_Am_I() == RTTI_UNIT || What_Am_I() == RTTI_VESSEL) {
//	if (What_Am_I() == RTTI_UNIT) {
#endif
		return;
	}

	/*
	**	If the original path starts in the same direction as the unit, then
	**	there is no problem to resolve -- abort.
	*/
	facediff = PrimaryFacing.Difference((DirType)(path->Command[0]<<5)) >> 5;

	if (!facediff) return;

	if (Dir_Facing(PrimaryFacing) & FACING_NE) {
		ptr = &_dpath[(FacingType)ABS((int)facediff)-FACING_NE][1];			// Pointer to path adjust list.
 		counter = (int)_dpath[(FacingType)ABS((int)facediff)-FACING_NE][0];		// Number of path adjusts.
	} else {
		ptr = &_path[(FacingType)ABS((int)facediff)-FACING_NE][1];			// Pointer to path adjust list.
 		counter = (int)_path[(FacingType)ABS((int)facediff)-FACING_NE][0];		// Number of path adjusts.
	}
	ptr2 = ptr;

	ok = true;										// Presume adjustment is all ok.
	cell = Coord_Cell(Coord);	// Starting cell.
	nextpath = Dir_Facing(PrimaryFacing);	// Starting path.
	for (index = 0; index < counter; index++) {

		/*
		**	Determine next path element and add it to the
		**	working path list.
		*/
		if (facediff > 0) {
			nextpath = nextpath + *ptr++;
		} else {
			nextpath = nextpath - *ptr++;
		}
		stage[index] = nextpath;
		cell = Adjacent_Cell(cell, nextpath);
		//cell = Coord_Cell(Adjacent_Cell(Cell_Coord(cell), nextpath));

		/*
		**	If it can't enter this cell, then abort the path
		**	building operation without adjusting the unit's
		**	path.
		*/
		if (Can_Enter_Cell(cell, nextpath) != MOVE_OK) {
			ok = false;
			break;
		}
	}

	/*
	**	If veering to the left was not successful, then try veering
	**	to the right. This only makes sense if the vehicle is trying
	**	to turn 180 degrees.
	*/
	if (!ok && ABS(facediff) == 4) {
		ptr = ptr2;											// Pointer to path adjust list.
		facediff = -facediff;
		ok = true;											// Presume adjustment is all ok.
		cell = Coord_Cell(Coord);						// Starting cell.
		nextpath = Dir_Facing(PrimaryFacing);		// Starting path.
		for (index = 0; index < counter; index++) {

			/*
			**	Determine next path element and add it to the
			**	working path list.
			*/
			if (facediff > 0) {
				nextpath = nextpath + *ptr++;
			} else {
				nextpath = nextpath - *ptr++;
			}
			stage[index] = nextpath;
			cell = Coord_Cell(Adjacent_Cell(Cell_Coord(cell), nextpath));

			/*
			**	If it can't enter this cell, then abort the path
			**	building operation without adjusting the unit's
			**	path.
			*/
			if (Can_Enter_Cell(cell, nextpath) != MOVE_OK) {
				ok = false;
				break;
			}
		}
	}

	/*
	**	If a legal path addition was created, then install it in place
	**	of the first path value. The initial path entry is to be replaced
	**	with a sequence of path entries that create smooth turning.
	*/
	if (ok) {
		if (path->Length <= 1) {
			memmove((char *)&stage[0], (char*)path->Command, max(counter, 1));
			path->Length = counter;
		} else {

			/*
			**	Optimize the transition path step from the smooth turn
			**	first part as it joins with the rest of the normal
			**	path. The normal prefix path steps are NOT to be optimized.
			*/
			if (counter) {
				counter--;
				path->Command[0] = stage[counter];
				Optimize_Moves(path, MOVE_OK);
			}

			/*
			**	If there is more than one prefix path element, then
			**	insert the rest now.
			*/
			if (counter) {
				memmove((char*)&path->Command[0], (char*)&path->Command[counter], 40-counter);
				memmove((char*)&stage[0], (char*)&path->Command[0], counter);
				path->Length += counter;
			}
		}
		path->Command[path->Length] = FACING_NONE;
	}
}


/***********************************************************************************************
 * DriveClass::Lay_Track -- Handles track laying logic for the unit.                           *
 *                                                                                             *
 *    This routine handles the track laying for the unit. This entails examining the unit's    *
 *    current location as well as the direction and whether this unit is allowed to lay        *
 *    tracks in the first place.                                                               *
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
void DriveClass::Lay_Track(void)
{
	assert(IsActive);

#ifdef NEVER
	static IconCommandType * _trackdirs[8] = {
		TrackN_S,
		TrackNE_SW,
		TrackE_W,
		TrackNW_SE,
		TrackN_S,
		TrackNE_SW,
		TrackE_W,
		TrackNW_SE
	};

	if (!(ClassF & CLASSF_TRACKS)) return;

	Icon_Install(Coord_Cell(Coord), _trackdirs[Facing_To_8(BodyFacing)]);
#endif
}


/***********************************************************************************************
 * DriveClass::Mark_Track -- Marks the midpoint of the track as occupied.                      *
 *                                                                                             *
 *    This routine will ensure that the midpoint (if any) of the track that the unit is        *
 *    following, will be marked according to the mark type specified.                          *
 *                                                                                             *
 * INPUT:   headto   -- The head to coordinate.                                                *
 *                                                                                             *
 *          type     -- The type of marking to perform.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void DriveClass::Mark_Track(COORDINATE headto, MarkType type)
{
	assert(IsActive);

	int value;

	if (type == MARK_UP) {
		value = false;
	} else {
		value = true;
	}

	if (headto) {
		if (!IsOnShortTrack && TrackNumber != -1) {

			/*
			** If we have not passed the per cell process point we need
			** to deal with it.
			*/
			int tracknum = TrackControl[TrackNumber].Track;
			if (tracknum) {
				TrackType const * ptr = RawTracks[tracknum - 1].Track;
				int cellidx = RawTracks[tracknum - 1].Cell;
				if (cellidx > -1) {
					DirType dir = ptr[cellidx].Facing;

					if (TrackIndex < cellidx && cellidx != -1) {
						COORDINATE offset = Smooth_Turn(ptr[cellidx].Offset, dir);
						Map[offset].Flag.Occupy.Vehicle = value;
					}
				}
			}
		}
		Map[headto].Flag.Occupy.Vehicle = value;
	}
}


/***********************************************************************************************
 * DriveClass::Ok_To_Move -- Checks to see if this object can begin moving.                    *
 *                                                                                             *
 *    This routine is used to verify that this object is allowed to move. Some objects can     *
 *    be temporarily occupied and thus cannot move until the situation permits.                *
 *                                                                                             *
 * INPUT:   direction   -- The direction that movement would be desired.                       *
 *                                                                                             *
 * OUTPUT:  Can the unit move in the direction specified?                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool DriveClass::Ok_To_Move(DirType ) const
{
	assert(IsActive);

	return true;
}


/***************************************************************************
**	Smooth turn track tables. These are coordinate offsets from the center
**	of the destination cell. These are the raw tracks that are modified
**	by negating the X and Y portions as necessary. Also for reverse travelling
**	direction, the track list can be processed backward.
**
**	Track 1 = N
**	Track 2 = NE
**	Track 3 = N->NE 45 deg (double path consumption)
**	Track 4 = N->E 90 deg (double path consumption)
**	Track 5 = NE->SE 90 deg (double path consumption)
** Track 6 = NE->N 45 deg (double path consumption)
**	Track 7 = N->NE (facing change only)
**	Track 8 = NE->E (facing change only)
**	Track 9 = N->E (facing change only)
**	Track 10= NE->SE (facing change only)
**	Track 11= back up into refinery
**	Track 12= drive out of refinery
*/
//#pragma warn -ias
DriveClass::TrackType const DriveClass::Track1[24] = {
	{0x00F50000L,(DirType)0},
	{0x00EA0000L,(DirType)0},
	{0x00DF0000L,(DirType)0},
	{0x00D40000L,(DirType)0},
	{0x00C90000L,(DirType)0},
	{0x00BE0000L,(DirType)0},
	{0x00B30000L,(DirType)0},
	{0x00A80000L,(DirType)0},
	{0x009D0000L,(DirType)0},
	{0x00920000L,(DirType)0},
	{0x00870000L,(DirType)0},
	{0x007C0000L,(DirType)0},		// Track jump check here.
	{0x00710000L,(DirType)0},
	{0x00660000L,(DirType)0},
	{0x005B0000L,(DirType)0},
	{0x00500000L,(DirType)0},
	{0x00450000L,(DirType)0},
	{0x003A0000L,(DirType)0},
	{0x002F0000L,(DirType)0},
	{0x00240000L,(DirType)0},
	{0x00190000L,(DirType)0},
	{0x000E0000L,(DirType)0},
	{0x00030000L,(DirType)0},
	{0x00000000L,(DirType)0}
};

DriveClass::TrackType const DriveClass::Track2[] = {
	{0x00F8FF08L,(DirType)32},
	{0x00F0FF10L,(DirType)32},
	{0x00E8FF18L,(DirType)32},
	{0x00E0FF20L,(DirType)32},
	{0x00D8FF28L,(DirType)32},
	{0x00D0FF30L,(DirType)32},
	{0x00C8FF38L,(DirType)32},
	{0x00C0FF40L,(DirType)32},
	{0x00B8FF48L,(DirType)32},
	{0x00B0FF50L,(DirType)32},
	{0x00A8FF58L,(DirType)32},
	{0x00A0FF60L,(DirType)32},
	{0x0098FF68L,(DirType)32},
	{0x0090FF70L,(DirType)32},
	{0x0088FF78L,(DirType)32},
	{0x0080FF80L,(DirType)32},		// Track jump check here.
	{0x0078FF88L,(DirType)32},
	{0x0070FF90L,(DirType)32},
	{0x0068FF98L,(DirType)32},
	{0x0060FFA0L,(DirType)32},
	{0x0058FFA8L,(DirType)32},
	{0x0050FFB0L,(DirType)32},
	{0x0048FFB8L,(DirType)32},
	{0x0040FFC0L,(DirType)32},
	{0x0038FFC8L,(DirType)32},
	{0x0030FFD0L,(DirType)32},
	{0x0028FFD8L,(DirType)32},
	{0x0020FFE0L,(DirType)32},
	{0x0018FFE8L,(DirType)32},
	{0x0010FFF0L,(DirType)32},
	{0x0008FFF8L,(DirType)32},
	{0x00000000L,(DirType)32}
};

DriveClass::TrackType const DriveClass::Track3[] = {
	{0x01F5FF00L,(DirType)0},
	{0x01EAFF00L,(DirType)0},
	{0x01DFFF00L,(DirType)0},
	{0x01D4FF00L,(DirType)0},
	{0x01C9FF00L,(DirType)0},
	{0x01BEFF00L,(DirType)0},
	{0x01B3FF00L,(DirType)0},
	{0x01A8FF00L,(DirType)0},
	{0x019DFF00L,(DirType)0},
	{0x0192FF00L,(DirType)0},
	{0x0187FF00L,(DirType)0},
	{0x0180FF00L,(DirType)0},
	{0x0175FF00L,(DirType)0},		// Jump entry point here.
	{0x016BFF00L,(DirType)0},
	{0x0160FF02L,(DirType)1},
	{0x0155FF04L,(DirType)3},
	{0x014CFF06L,(DirType)4},
	{0x0141FF08L,(DirType)5},
	{0x0137FF0BL,(DirType)7},
	{0x012EFF0FL,(DirType)8},
	{0x0124FF13L,(DirType)9},
	{0x011AFF17L,(DirType)11},
	{0x0110FF1BL,(DirType)12},
	{0x0107FF1FL,(DirType)13},		// Center cell processing here.
	{0x00FCFF24L,(DirType)15},
	{0x00F3FF28L,(DirType)16},
	{0x00ECFF2CL,(DirType)17},
	{0x00E0FF32L,(DirType)19},
	{0x00D7FF36L,(DirType)20},
	{0x00CFFF3DL,(DirType)21},
	{0x00C6FF42L,(DirType)23},
	{0x00BAFF49L,(DirType)24},
	{0x00B0FF4DL,(DirType)25},
	{0x00A8FF58L,(DirType)27},
	{0x00A0FF60L,(DirType)28},
	{0x0098FF68L,(DirType)29},
	{0x0090FF70L,(DirType)31},
	{0x0088FF78L,(DirType)32},
	{0x0080FF80L,(DirType)32},		// Track jump check here.
	{0x0078FF88L,(DirType)32},
	{0x0070FF90L,(DirType)32},
	{0x0068FF98L,(DirType)32},
	{0x0060FFA0L,(DirType)32},
	{0x0058FFA8L,(DirType)32},
	{0x0050FFB0L,(DirType)32},
	{0x0048FFB8L,(DirType)32},
	{0x0040FFC0L,(DirType)32},
	{0x0038FFC8L,(DirType)32},
	{0x0030FFD0L,(DirType)32},
	{0x0028FFD8L,(DirType)32},
	{0x0020FFE0L,(DirType)32},
	{0x0018FFE8L,(DirType)32},
	{0x0010FFF0L,(DirType)32},
	{0x0008FFF8L,(DirType)32},
	{0x00000000L,(DirType)32}
};

DriveClass::TrackType const DriveClass::Track4[] = {
	{0x00F5FF00L,(DirType)0},
	{0x00EBFF00L,(DirType)0},
	{0x00E0FF00L,(DirType)0},
	{0x00D5FF00L,(DirType)0},
	{0x00CBFF01L,(DirType)0},
	{0x00C0FF03L,(DirType)0},
	{0x00B5FF05L,(DirType)1},
	{0x00ABFF07L,(DirType)1},
	{0x00A0FF0AL,(DirType)2},
	{0x0095FF0DL,(DirType)3},
	{0x008BFF10L,(DirType)4},
	{0x0080FF14L,(DirType)5},		// Track entry here.
	{0x0075FF18L,(DirType)8},
	{0x006DFF1CL,(DirType)12},
	{0x0063FF22L,(DirType)16},
	{0x005AFF25L,(DirType)20},
	{0x0052FF2BL,(DirType)23},
	{0x0048FF32L,(DirType)27},
	{0x0040FF37L,(DirType)32},
	{0x0038FF3DL,(DirType)36},
	{0x0030FF46L,(DirType)39},
	{0x002BFF4FL,(DirType)43},
	{0x0024FF58L,(DirType)47},
	{0x0020FF60L,(DirType)51},
	{0x001BFF6DL,(DirType)54},
	{0x0017FF79L,(DirType)57},
	{0x0014FF82L,(DirType)60},		// Track jump here.
	{0x0011FF8FL,(DirType)62},
	{0x000DFF98L,(DirType)63},
	{0x0009FFA2L,(DirType)64},
	{0x0006FFACL,(DirType)64},
	{0x0004FFB5L,(DirType)66},
	{0x0003FFC0L,(DirType)64},
	{0x0002FFCBL,(DirType)64},
	{0x0001FFD5L,(DirType)64},
	{0x0000FFE0L,(DirType)64},
	{0x0000FFEBL,(DirType)64},
	{0x0000FFF5L,(DirType)64},
	{0x00000000L,(DirType)64}
};

DriveClass::TrackType const DriveClass::Track5[] = {
	{0xFFF8FE08L,(DirType)32},
	{0xFFF0FE10L,(DirType)32},
	{0xFFE8FE18L,(DirType)32},
	{0xFFE0FE20L,(DirType)32},
	{0xFFD8FE28L,(DirType)32},
	{0xFFD0FE30L,(DirType)32},
	{0xFFC8FE38L,(DirType)32},
	{0xFFC0FE40L,(DirType)32},
	{0xFFB8FE48L,(DirType)32},
	{0xFFB0FE50L,(DirType)32},
	{0xFFA8FE58L,(DirType)32},
	{0xFFA0FE60L,(DirType)32},
	{0xFF98FE68L,(DirType)32},
	{0xFF90FE70L,(DirType)32},
	{0xFF88FE78L,(DirType)32},
	{0xFF80FE80L,(DirType)32},		// Track entry here.
	{0xFF78FE88L,(DirType)32},
	{0xFF71FE90L,(DirType)32},
	{0xFF6AFE97L,(DirType)32},
	{0xFF62FE9FL,(DirType)32},
	{0xFF5AFEA8L,(DirType)32},
	{0xFF53FEB0L,(DirType)35},
	{0xFF4BFEB7L,(DirType)38},
	{0xFF44FEBEL,(DirType)41},
	{0xFF3EFEC4L,(DirType)44},
	{0xFF39FECEL,(DirType)47},
	{0xFF34FED8L,(DirType)50},
	{0xFF30FEE0L,(DirType)53},
	{0xFF2DFEEBL,(DirType)56},
	{0xFF2CFEF5L,(DirType)59},
	{0xFF2BFF00L,(DirType)62},
	{0xFF2CFF0BL,(DirType)66},
	{0xFF2DFF15L,(DirType)69},
	{0xFF30FF1FL,(DirType)72},
	{0xFF34FF28L,(DirType)75},
	{0xFF39FF30L,(DirType)78},
	{0xFF3EFF3AL,(DirType)81},
	{0xFF44FF44L,(DirType)84},
	{0xFF4BFF4BL,(DirType)87},
	{0xFF53FF50L,(DirType)90},
	{0xFF5AFF58L,(DirType)93},
	{0xFF62FF60L,(DirType)96},
	{0xFF6AFF68L,(DirType)96},
	{0xFF71FF70L,(DirType)96},
	{0xFF78FF78L,(DirType)96},
	{0xFF80FF80L,(DirType)96},		// Track jump check here.
	{0xFF88FF88L,(DirType)96},
	{0xFF90FF90L,(DirType)96},
	{0xFF98FF98L,(DirType)96},
	{0xFFA0FFA0L,(DirType)96},
	{0xFFA8FFA8L,(DirType)96},
	{0xFFB0FFB0L,(DirType)96},
	{0xFFB8FFB8L,(DirType)96},
	{0xFFC0FFC0L,(DirType)96},
	{0xFFC8FFC8L,(DirType)96},
	{0xFFD0FFD0L,(DirType)96},
	{0xFFD8FFD8L,(DirType)96},
	{0xFFE0FFE0L,(DirType)96},
	{0xFFE8FFE8L,(DirType)96},
	{0xFFF0FFF0L,(DirType)96},
	{0xFFF8FFF8L,(DirType)96},
	{0x00000000L,(DirType)96}
};

DriveClass::TrackType const DriveClass::Track6[] = {
	{0x0100FE00L,(DirType)32},
	{0x00F8FE08L,(DirType)32},
	{0x00F0FE10L,(DirType)32},
	{0x00E8FE18L,(DirType)32},
	{0x00E0FE20L,(DirType)32},
	{0x00D8FE28L,(DirType)32},
	{0x00D0FE30L,(DirType)32},
	{0x00C8FE38L,(DirType)32},
	{0x00C0FE40L,(DirType)32},
	{0x00B8FE48L,(DirType)32},
	{0x00B0FE50L,(DirType)32},
	{0x00A8FE58L,(DirType)32},
	{0x00A0FE60L,(DirType)32},
	{0x0098FE68L,(DirType)32},
	{0x0090FE70L,(DirType)32},
	{0x0088FE78L,(DirType)32},
	{0x0080FE80L,(DirType)32},		// Jump entry point here.
	{0x0078FE88L,(DirType)32},
	{0x0070FE90L,(DirType)32},
	{0x0068FE98L,(DirType)32},
	{0x0060FEA0L,(DirType)32},
	{0x0058FEA8L,(DirType)32},
	{0x0055FEAEL,(DirType)32},
	{0x004EFEB8L,(DirType)35},
	{0x0048FEC0L,(DirType)37},
	{0x0042FEC9L,(DirType)40},
	{0x003BFED2L,(DirType)43},
	{0x0037FEDAL,(DirType)45},
	{0x0032FEE3L,(DirType)48},
	{0x002BFEEBL,(DirType)51},
	{0x0026FEF5L,(DirType)53},
	{0x0022FEFEL,(DirType)56},
	{0x001CFF08L,(DirType)59},
	{0x0019FF12L,(DirType)61},
	{0x0015FF1BL,(DirType)64},
	{0x0011FF26L,(DirType)64},
	{0x000EFF30L,(DirType)64},
	{0x000BFF39L,(DirType)64},
	{0x0009FF43L,(DirType)64},
	{0x0007FF4EL,(DirType)64},
	{0x0005FF57L,(DirType)64},
	{0x0003FF62L,(DirType)64},
	{0x0001FF6DL,(DirType)64},
	{0x0000FF77L,(DirType)64},
	{0x0000FF80L,(DirType)64},		// Track jump check here.
	{0x0000FF8BL,(DirType)64},
	{0x0000FF95L,(DirType)64},
	{0x0000FFA0L,(DirType)64},
	{0x0000FFABL,(DirType)64},
	{0x0000FFB5L,(DirType)64},
	{0x0000FFC0L,(DirType)64},
	{0x0000FFCBL,(DirType)64},
	{0x0000FFD5L,(DirType)64},
	{0x0000FFE0L,(DirType)64},
	{0x0000FFEBL,(DirType)64},
	{0x0000FFF5L,(DirType)64},
	{0x00000000L,(DirType)64}
};

DriveClass::TrackType const DriveClass::Track7[] = {
	{0x0006FFFFL,(DirType)0},
	{0x000CFFFEL,(DirType)4},
	{0x0011FFFCL,(DirType)8},
	{0x0018FFFAL,(DirType)12},
	{0x001FFFF6L,(DirType)16},
	{0x0024FFF3L,(DirType)19},
	{0x002BFFF0L,(DirType)22},
	{0x0030FFFDL,(DirType)23},
	{0x0035FFEBL,(DirType)24},
	{0x0038FFE8L,(DirType)25},
	{0x003CFFE6L,(DirType)26},
	{0x0040FFE3L,(DirType)27},
	{0x0043FFE0L,(DirType)28},
	{0x0046FFDDL,(DirType)29},
	{0x0043FFDFL,(DirType)30},
	{0x0040FFE1L,(DirType)30},
	{0x003CFFE3L,(DirType)30},
	{0x0038FFE5L,(DirType)30},
	{0x0035FFE7L,(DirType)31},
	{0x0030FFE9L,(DirType)31},
	{0x002BFFEBL,(DirType)31},
	{0x0024FFEDL,(DirType)31},
	{0x001FFFF1L,(DirType)31},
	{0x0018FFF4L,(DirType)32},
	{0x0011FFF7L,(DirType)32},
	{0x000CFFFAL,(DirType)32},
	{0x0006FFFDL,(DirType)32},
	{0x00000000L,(DirType)32}
};

DriveClass::TrackType const DriveClass::Track8[] = {
	{0x0003FFFCL,(DirType)32},
	{0x0006FFF7L,(DirType)36},
	{0x000AFFF1L,(DirType)40},
	{0x000CFFEBL,(DirType)44},
	{0x000DFFE4L,(DirType)46},
	{0x000EFFDCL,(DirType)48},
	{0x000FFFD5L,(DirType)50},
	{0x0010FFD0L,(DirType)52},
	{0x0011FFC9L,(DirType)54},
	{0x0012FFC2L,(DirType)56},
	{0x0011FFC0L,(DirType)58},
	{0x0010FFC2L,(DirType)60},
	{0x000EFFC9L,(DirType)62},
	{0x000CFFCFL,(DirType)64},
	{0x000AFFD5L,(DirType)64},
	{0x0008FFDAL,(DirType)64},
	{0x0006FFE2L,(DirType)64},
	{0x0004FFE9L,(DirType)64},
	{0x0002FFEFL,(DirType)64},
	{0x0001FFF5L,(DirType)64},
	{0x0000FFF9L,(DirType)64},
	{0x00000000L,(DirType)64}
};

DriveClass::TrackType const DriveClass::Track9[] = {
	{0xFFF50002L,(DirType)0},
	{0xFFEB0004L,(DirType)2},
	{0xFFE00006L,(DirType)4},
	{0xFFD50009L,(DirType)6},
	{0xFFCE000CL,(DirType)9},
	{0xFFC8000FL,(DirType)11},
	{0xFFC00012L,(DirType)13},
	{0xFFB80015L,(DirType)16},
	{0xFFC00012L,(DirType)18},
	{0xFFC8000EL,(DirType)20},
	{0xFFCE000AL,(DirType)22},
	{0xFFD50004L,(DirType)24},
	{0xFFDE0000L,(DirType)26},
	{0xFFE9FFF8L,(DirType)28},
	{0xFFEEFFF2L,(DirType)30},
	{0xFFF5FFEBL,(DirType)32},
	{0xFFFDFFE1L,(DirType)34},
	{0x0002FFD8L,(DirType)36},
	{0x0007FFD2L,(DirType)39},
	{0x000BFFCBL,(DirType)41},
	{0x0010FFC5L,(DirType)43},
	{0x0013FFBEL,(DirType)45},
	{0x0015FFB7L,(DirType)48},
	{0x0013FFBEL,(DirType)50},
	{0x0011FFC5L,(DirType)52},
	{0x000BFFCCL,(DirType)54},
	{0x0008FFD4L,(DirType)56},
	{0x0005FFDFL,(DirType)58},
	{0x0003FFEBL,(DirType)62},
	{0x0001FFF5L,(DirType)64},
	{0x00000000L,(DirType)64}
};

DriveClass::TrackType const DriveClass::Track10[] = {
	{0xFFF6000BL,(DirType)32},
	{0xFFF00015L,(DirType)37},
	{0xFFEB0020L,(DirType)42},
	{0xFFE9002BL,(DirType)47},
	{0xFFE50032L,(DirType)52},
	{0xFFE30038L,(DirType)57},
	{0xFFE00040L,(DirType)60},
	{0xFFE20038L,(DirType)62},
	{0xFFE40032L,(DirType)64},
	{0xFFE5002AL,(DirType)68},
	{0xFFE6001EL,(DirType)70},
	{0xFFE70015L,(DirType)72},
	{0xFFE8000BL,(DirType)74},
	{0xFFE90000L,(DirType)76},
	{0xFFE8FFF5L,(DirType)78},
	{0xFFE7FFEBL,(DirType)80},
	{0xFFE6FFE0L,(DirType)82},
	{0xFFE5FFD5L,(DirType)84},
	{0xFFE4FFCEL,(DirType)86},
	{0xFFE2FFC5L,(DirType)88},
	{0xFFE0FFC0L,(DirType)90},
	{0xFFE3FFC5L,(DirType)92},
	{0xFFE5FFCEL,(DirType)94},
	{0xFFE9FFD5L,(DirType)95},
	{0xFFEBFFE0L,(DirType)96},
	{0xFFF0FFEBL,(DirType)96},
	{0xFFF6FFF5L,(DirType)96},
	{0x00000000L,(DirType)96}
};

DriveClass::TrackType const DriveClass::Track11[] = {
	{0x01000000L,DIR_SW},
	{0x00F30008L,DIR_SW},
	{0x00E50010L,DIR_SW_X1},
	{0x00D60018L,DIR_SW_X1},
	{0x00C80020L,DIR_SW_X1},
	{0x00B90028L,DIR_SW_X1},
	{0x00AB0030L,DIR_SW_X2},
	{0x009C0038L,DIR_SW_X2},
	{0x008D0040L,DIR_SW_X2},
	{0x007F0048L,DIR_SW_X2},
	{0x00710050L,DIR_SW_X2},
	{0x00640058L,DIR_SW_X2},
	{0x00550060L,DIR_SW_X2},

	{0x00000000L,DIR_SW_X2}
};

DriveClass::TrackType const DriveClass::Track12[] = {
	{0xFF550060L,DIR_SW_X2},
	{0xFF640058L,DIR_SW_X2},
	{0xFF710050L,DIR_SW_X2},
	{0xFF7F0048L,DIR_SW_X2},
	{0xFF8D0040L,DIR_SW_X2},
	{0xFF9C0038L,DIR_SW_X2},
	{0xFFAB0030L,DIR_SW_X2},
	{0xFFB90028L,DIR_SW_X1},
	{0xFFC80020L,DIR_SW_X1},
	{0xFFD60018L,DIR_SW_X1},
	{0xFFE50010L,DIR_SW_X1},
	{0xFFF30008L,DIR_SW},

	{0x00000000L,DIR_SW}
};

#if(1)
/*
**	Drive out of weapon's factory.
*/
DriveClass::TrackType const DriveClass::Track13[] = {
	{XYP_COORD(0,-35),DIR_S},
	{XYP_COORD(0,-34),DIR_S},
	{XYP_COORD(0,-33),DIR_S},
	{XYP_COORD(0,-32),DIR_S},
	{XYP_COORD(0,-31),DIR_S},
	{XYP_COORD(0,-30),DIR_S},
	{XYP_COORD(0,-29),DIR_S},
	{XYP_COORD(0,-28),DIR_S},
	{XYP_COORD(0,-27),DIR_S},
	{XYP_COORD(0,-26),DIR_S},
	{XYP_COORD(0,-25),DIR_S},
	{XYP_COORD(0,-24),DIR_S},
	{XYP_COORD(0,-23),DIR_S},
	{XYP_COORD(0,-22),DIR_S},
	{XYP_COORD(0,-21),DIR_S},
	{XYP_COORD(0,-20),DIR_S},
	{XYP_COORD(0,-19),DIR_S},
	{XYP_COORD(0,-18),DIR_S},
	{XYP_COORD(0,-17),DIR_S},
	{XYP_COORD(0,-16),DIR_S},
	{XYP_COORD(0,-15),DIR_S},
	{XYP_COORD(0,-14),DIR_S},
	{XYP_COORD(0,-13),DIR_S},
	{XYP_COORD(0,-12),DIR_S},
	{XYP_COORD(0,-11),DIR_S},
	{XYP_COORD(0,-10),DIR_S},
	{XYP_COORD(0,-9),DIR_S},
	{XYP_COORD(0,-8),DIR_S},
	{XYP_COORD(0,-7),DIR_S},
	{XYP_COORD(0,-6),DIR_S},
	{XYP_COORD(0,-5),DIR_S},
	{XYP_COORD(0,-4),DIR_S},
	{XYP_COORD(0,-3),DIR_S},
	{XYP_COORD(0,-2),DIR_S},
	{XYP_COORD(0,-1),DIR_S},

	{0x00000000L,DIR_S}
};
#else
/*
**	Drive out of weapon's factory.
*/
DriveClass::TrackType const DriveClass::Track13[] = {
	{XYP_COORD(10,-21),(DirType)(DIR_SW-10)},
	{XYP_COORD(10,-21),(DirType)(DIR_SW-10)},
	{XYP_COORD(10,-20),(DirType)(DIR_SW-10)},
	{XYP_COORD(10,-20),(DirType)(DIR_SW-10)},
	{XYP_COORD(9,-18),(DirType)(DIR_SW-10)},
	{XYP_COORD(9,-18),(DirType)(DIR_SW-10)},
	{XYP_COORD(9,-17),(DirType)(DIR_SW-10)},
	{XYP_COORD(8,-16),(DirType)(DIR_SW-10)},
	{XYP_COORD(8,-15),(DirType)(DIR_SW-10)},
	{XYP_COORD(7,-14),(DirType)(DIR_SW-10)},
	{XYP_COORD(7,-13),(DirType)(DIR_SW-10)},
	{XYP_COORD(6,-12),(DirType)(DIR_SW-10)},
	{XYP_COORD(6,-11),(DirType)(DIR_SW-10)},
	{XYP_COORD(5,-10),(DirType)(DIR_SW-10)},
	{XYP_COORD(5,-9),(DirType)(DIR_SW-10)},
	{XYP_COORD(4,-8),(DirType)(DIR_SW-10)},
	{XYP_COORD(4,-7),(DirType)(DIR_SW-10)},
	{XYP_COORD(3,-6),(DirType)(DIR_SW-10)},
	{XYP_COORD(3,-5),(DirType)(DIR_SW-9)},
	{XYP_COORD(2,-4),(DirType)(DIR_SW-7)},
	{XYP_COORD(2,-3),(DirType)(DIR_SW-5)},
	{XYP_COORD(1,-2),(DirType)(DIR_SW-3)},
	{XYP_COORD(1,-1),(DirType)(DIR_SW-1)},

	{0x00000000L,DIR_SW}
};
#endif

/*
**	There are a limited basic number of tracks that a vehicle can follow. These
**	are they. Each track can be interpreted differently but this is controlled
**	by the TrackControl structure elaborated elsewhere.
*/
DriveClass::RawTrackType const DriveClass::RawTracks[13] = {
	{Track1, -1, 0, -1},
	{Track2, -1, 0, -1},
	{Track3, 37, 12, 22},
	{Track4, 26, 11, 19},
	{Track5, 45, 15, 31},
	{Track6, 44, 16, 27},
	{Track7, -1, 0, -1},
	{Track8, -1, 0, -1},
	{Track9, -1, 0, -1},
	{Track10, -1, 0, -1},
	{Track11, -1, 0, -1},
	{Track12, -1, 0, -1},
	{Track13, -1, 0, -1}
};


/***************************************************************************
**	Smooth turning control table. Given two directions in a path list, this
**	table determines which track to use and what modifying operations need
**	be performed on the track data.
*/
DriveClass::TurnTrackType const DriveClass::TrackControl[67] = {
	{1,	0,		DIR_N,	F_},																//	0-0
	{3,	7,		DIR_NE,	F_D},																//	0-1 (raw chart)
	{4,	9,		DIR_E,	F_D},																//	0-2 (raw chart)
	{0,	0,		DIR_SE,	F_},																//	0-3 !
	{0,	0,		DIR_S,	F_},																//	0-4 !
	{0,	0,		DIR_SW,	F_},																//	0-5 !
	{4,	9,		DIR_W,	(DriveClass::TrackControlType)(F_X|F_D)},				//	0-6
	{3,	7,		DIR_NW,	(DriveClass::TrackControlType)(F_X|F_D)},				//	0-7
	{6,	8,		DIR_N,	(DriveClass::TrackControlType)(F_T|F_X|F_Y|F_D)},	//	1-0
	{2,	0,		DIR_NE,	F_},																//	1-1 (raw chart)
	{6,	8,		DIR_E,	F_D},																//	1-2 (raw chart)
	{5,	10,	DIR_SE,	F_D},																//	1-3 (raw chart)
	{0,	0,		DIR_S,	F_},																//	1-4 !
	{0,	0,		DIR_SW,	F_},																//	1-5 !
	{0,	0,		DIR_W,	F_},																//	1-6 !
	{5,	10,	DIR_NW,	(DriveClass::TrackControlType)(F_T|F_X|F_Y|F_D)},	//	1-7
	{4,	9,		DIR_N,	(DriveClass::TrackControlType)(F_T|F_X|F_Y|F_D)},	//	2-0
	{3,	7,		DIR_NE,	(DriveClass::TrackControlType)(F_T|F_X|F_Y|F_D)},	//	2-1
	{1,	0,		DIR_E,	(DriveClass::TrackControlType)(F_T|F_X)},				//	2-2
	{3,	7,		DIR_SE,	(DriveClass::TrackControlType)(F_T|F_X|F_D)},		//	2-3
	{4,	9,		DIR_S,	(DriveClass::TrackControlType)(F_T|F_X|F_D)},		//	2-4
	{0,	0,		DIR_SW,	F_},																//	2-5 !
	{0,	0,		DIR_W,	F_},																//	2-6 !
	{0,	0,		DIR_NW,	F_},																//	2-7 !
	{0,	0,		DIR_N,	F_},																//	3-0 !
	{5,	10,	DIR_NE,	(DriveClass::TrackControlType)(F_Y|F_D)},				//	3-1
	{6,	8,		DIR_E,	(DriveClass::TrackControlType)(F_Y|F_D)},				//	3-2
	{2,	0,		DIR_SE,	F_Y},																//	3-3
	{6,	8,		DIR_S,	(DriveClass::TrackControlType)(F_T|F_X|F_D)},		//	3-4
	{5,	10,	DIR_SW,	(DriveClass::TrackControlType)(F_T|F_X|F_D)},		//	3-5
	{0,	0,		DIR_W,	F_},																//	3-6 !
	{0,	0,		DIR_NW,	F_},																//	3-7 !
	{0,	0,		DIR_N,	F_},																//	4-0 !
	{0,	0,		DIR_NE,	F_},																//	4-1 !
	{4,	9,		DIR_E,	(DriveClass::TrackControlType)(F_Y|F_D)},				//	4-2
	{3,	7,		DIR_SE,	(DriveClass::TrackControlType)(F_Y|F_D)},				//	4-3
	{1,	0,		DIR_S,	F_Y},																//	4-4
	{3,	7,		DIR_SW,	(DriveClass::TrackControlType)(F_X|F_Y|F_D)},		//	4-5
	{4,	9,		DIR_W,	(DriveClass::TrackControlType)(F_X|F_Y|F_D)},		//	4-6
	{0,	0,		DIR_NW,	F_},																//	4-7 !
	{0,	0,		DIR_N,	F_},																//	5-0 !
	{0,	0,		DIR_NE,	F_},																//	5-1 !
	{0,	0,		DIR_E,	F_},																//	5-2 !
	{5,	10,	DIR_SE,	(DriveClass::TrackControlType)(F_T|F_D)},				//	5-3
	{6,	8,		DIR_S,	(DriveClass::TrackControlType)(F_T|F_D)},				//	5-4
	{2,	0,		DIR_SW,	F_T},																//	5-5
	{6,	8,		DIR_W,	(DriveClass::TrackControlType)(F_X|F_Y|F_D)},		//	5-6
	{5,	10,	DIR_NW,	(DriveClass::TrackControlType)(F_X|F_Y|F_D)},		//	5-7
	{4,	9,		DIR_N,	(DriveClass::TrackControlType)(F_T|F_Y|F_D)},		//	6-0
	{0,	0,		DIR_NE,	F_},																//	6-1 !
	{0,	0,		DIR_E,	F_},																//	6-2 !
	{0,	0,		DIR_SE,	F_},																//	6-3 !
	{4,	9,		DIR_S,	(DriveClass::TrackControlType)(F_T|F_D)},				//	6-4
	{3,	7,		DIR_SW,	(DriveClass::TrackControlType)(F_T|F_D)},				//	6-5
	{1,	0,		DIR_W,	F_T},																//	6-6
	{3,	7,		DIR_NW,	(DriveClass::TrackControlType)(F_T|F_Y|F_D)},		//	6-7
	{6,	8,		DIR_N,	(DriveClass::TrackControlType)(F_T|F_Y|F_D)},		//	7-0
	{5,	10,	DIR_NE,	(DriveClass::TrackControlType)(F_T|F_Y|F_D)},		//	7-1
	{0,	0,		DIR_E,	F_},																//	7-2 !
	{0,	0,		DIR_SE,	F_},																//	7-3 !
	{0,	0,		DIR_S,	F_},																//	7-4 !
	{5,	10,	DIR_SW,	(DriveClass::TrackControlType)(F_X|F_D)},				//	7-5
	{6,	8,		DIR_W,	(DriveClass::TrackControlType)(F_X|F_D)},				//	7-6
	{2,	0,		DIR_NW,	F_X},																//	7-7

	{11,	11,	DIR_SW,	F_},																// Backup harvester into refinery.
	{12,	12,	DIR_SW_X2,	F_},															// Drive back into refinery.
	{13,	13,	DIR_SW,	F_}																// Drive out of weapons factory.
};



