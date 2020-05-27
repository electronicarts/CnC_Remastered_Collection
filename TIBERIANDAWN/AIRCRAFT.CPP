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

/* $Header:   F:\projects\c&c\vcs\code\aircraft.cpv   2.12   19 Jun 1995 09:27:22   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : AIRCRAFT.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : July 22, 1994                                                *
 *                                                                                             *
 *                  Last Update : August 13, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   AircraftClass::AI -- Processes the normal non-graphic AI for the aircraft.                *
 *   AircraftClass::Active_Click_With -- Handles clicking over specified cell.                 *
 *   AircraftClass::Active_Click_With -- Handles clicking over specified object.               *
 *   AircraftClass::AircraftClass -- The constructor for aircraft objects.                     *
 *   AircraftClass::As_Target -- Returns aircraft as a target number.                          *
 *   AircraftClass::Can_Enter_Cell -- Determines if the aircraft can land at this location.    *
 *   AircraftClass::Cell_Seems_Ok -- Checks to see if a cell is good to enter.                 *
 *   AircraftClass::Desired_Load_Dir -- Determines where passengers should line up.            *
 *   AircraftClass::Draw_It -- Renders an aircraft object at the location specified.           *
 *   AircraftClass::Enter_Idle_Mode -- Gives the aircraft an appropriate mission.              *
 *   AircraftClass::Exit_Object -- Unloads passenger from aircraft.                            *
 *   AircraftClass::Fire_At -- Handles firing a projectile from an aircraft.                   *
 *   AircraftClass::Fire_Coord -- Calculates the point of origin for a bullet.                 *
 *   AircraftClass::Fire_Direction -- Determines the direction of fire.                        *
 *   AircraftClass::Good_Fire_Location -- Searches for and finds a good spot to fire from.     *
 *   AircraftClass::Good_LZ -- Locates a good spot ot land.                                    *
 *   AircraftClass::In_Which_Layer -- Determine which render layer the aircraft lies.          *
 *   AircraftClass::Init -- Initialize the aircraft system to an empty state.                  *
 *   AircraftClass::Is_LZ_Clear -- Determines if landing zone is free for landing.             *
 *   AircraftClass::Mark -- Flags cells under the aircraft so that they will be redrawn.       *
 *   AircraftClass::Mission_Attack -- Handles the attack mission for aircraft.                 *
 *   AircraftClass::Mission_Enter -- Control aircraft to fly to the helipad or repair center.  *
 *   AircraftClass::Mission_Guard -- Handles aircraft in guard mode.                           *
 *   AircraftClass::Mission_Guard_Area -- Handles the aircraft guard area logic.               *
 *   AircraftClass::Mission_Hunt -- Maintains hunt AI for the aircraft.                        *
 *   AircraftClass::Mission_Move -- Handles movement mission.                                  *
 *   AircraftClass::Mission_Retreat -- Handles the aircraft logic for leaving the battlefield. *
 *   AircraftClass::Mission_Unload -- Handles unloading cargo.                                 *
 *   AircraftClass::New_LZ -- Find a good landing zone.                                        *
 *   AircraftClass::Overlap_List -- Returns with list of cells the aircraft overlaps.          *
 *   AircraftClass::Pip_Count -- Returns the number of "objects" in aircraft.                  *
 *   AircraftClass::Player_Assign_Mission -- Handles player input to assign a mission.         *
 *   AircraftClass::Pose_Dir -- Fetches the natural landing facing.                            *
 *   AircraftClass::Process_Fly_To -- Handles state machine for flying to destination.         *
 *   AircraftClass::Process_Landing -- Landing process state machine handler.                  *
 *   AircraftClass::Process_Take_Off -- State machine support for taking off.                  *
 *   AircraftClass::Read_INI -- Reads aircraft object data from an INI file.                   *
 *   AircraftClass::Rearm_Delay -- Returns the delay between shots for this aircraft.          *
 *   AircraftClass::Receive_Message -- Handles receipt of radio messages.                      *
 *   AircraftClass::Response_Attack -- Gives audio response to attack order.                   *
 *   AircraftClass::Response_Move -- Gives audio response to move request.                     *
 *   AircraftClass::Response_Select -- Gives audio response when selected.                     *
 *   AircraftClass::Scatter -- Causes the aircraft to move away a bit.                         *
 *   AircraftClass::Set_Speed -- Sets the speed for the aircraft.                              *
 *   AircraftClass::Sort_Y -- Figures the sorting coordinate.                                  *
 *   AircraftClass::Take_Damage -- Applies damage to the aircraft.                             *
 *   AircraftClass::Threat_Range -- Returns with a range to scan for targets.                  *
 *   AircraftClass::Unlimbo -- Removes an aircraft from the limbo state.                       *
 *   AircraftClass::What_Action -- Determines what action to perform.                          *
 *   AircraftClass::What_Action -- Determines what action to perform.                          *
 *   AircraftClass::Write_INI -- Writes the current aircraft objects to an INI file.           *
 *   AircraftClass::operator delete -- Deletes the aircraft object.                            *
 *   AircraftClass::operator new -- Allocates a new aircraft object from the pool              *
 *   AircraftClass::~AircraftClass -- Destructor for aircraft object.                          *
 *   AircraftClass::Validate -- validates aircraft pointer												  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/*
** This contains the value of the Virtual Function Table Pointer
*/
void * AircraftClass::VTable;


/***********************************************************************************************
 * AircraftClass::Validate -- validates aircraft pointer													  *
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
int AircraftClass::Validate(void) const
{
	int num;

	num = Aircraft.ID(this);
	if (num < 0 || num >= AIRCRAFT_MAX) {
		Validate_Error("AIRCRAFT");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
#endif


/***********************************************************************************************
 * AircraftClass::As_Target -- Returns aircraft as a target number.                            *
 *                                                                                             *
 *    This routine will convert the aircraft into a target number. This target number can      *
 *    then be assigned to a targeting or navigation computer.                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the aircraft as a target number.                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET AircraftClass::As_Target(void) const
{
	Validate();
	return(Build_Target(KIND_AIRCRAFT, Aircraft.ID(this)));
}


/***********************************************************************************************
 * AircraftClass::operator new -- Allocates a new aircraft object from the pool                *
 *                                                                                             *
 *    This routine will allocate an aircraft object from the free aircraft object pool. If     *
 *    there are no free object available, then this routine will fail (return NULL).           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the allocate aircraft object or NULL if none were        *
 *          available.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void * AircraftClass::operator new(size_t)
{
	void * ptr = Aircraft.Allocate();
	if (ptr) {
		((AircraftClass *)ptr)->Set_Active();
	}
	return(ptr);
}


/***********************************************************************************************
 * AircraftClass::operator delete -- Deletes the aircraft object.                              *
 *                                                                                             *
 *    This routine will return the aircraft object back to the free aircraft object pool.      *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the aircraft object to delete.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::operator delete(void *ptr)
{
	if (ptr) {
		((AircraftClass *)ptr)->IsActive = false;
	}
	Aircraft.Free((AircraftClass *)ptr);

	//Map.Validate();
}


/***********************************************************************************************
 * AircraftClass::AircraftClass -- The constructor for aircraft objects.                       *
 *                                                                                             *
 *    This routine is the constructor for aircraft objects. An aircraft object can be          *
 *    created and possibly placed into the game system by this routine.                        *
 *                                                                                             *
 * INPUT:   classid  -- The type of aircraft to create.                                        *
 *                                                                                             *
 *          house    -- The owner of this aircraft.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
AircraftClass::AircraftClass(AircraftType classid, HousesType house) :
	Class(&AircraftTypeClass::As_Reference(classid)),
	FootClass(house)
{
	/*
	**	For two shooters, clear out the second shot flag -- it will be set the first time
	**	the object fires. For non two shooters, set the flag since it will never be cleared
	**	and the second shot flag tells the system that normal rearm times apply -- this is
	**	what is desired for non two shooters.
	*/
	if (Class->IsTwoShooter) {
		IsSecondShot = false;
	} else {
		IsSecondShot = true;
	}
	Ammo = Class->MaxAmmo;
	AttacksRemaining = 3;
	Altitude = FLIGHT_LEVEL;
	IsLanding = false;
	IsTakingOff = false;
	IsHovering = false;
	IsHoming = false;
	Strength = Class->MaxStrength;
	NavCom = TARGET_NONE;
	SecondaryFacing = PrimaryFacing;
	Jitter = 0;

	/*
	** Keep count of the number of units created. Dont track cargo planes as they are created
	** automatically, not bought.
	*/
	if (classid != AIRCRAFT_CARGO && GameToPlay == GAME_INTERNET){
		House->AircraftTotals->Increment_Unit_Total((int)classid);
	}

#ifdef USE_RA_AI
	//
	// Added for RA AI in TD. ST - 7/26/2019 9:12AM
	//
	House->Tracking_Add(this);
#endif
}


/***********************************************************************************************
 * AircraftClass::Unlimbo -- Removes an aircraft from the limbo state.                         *
 *                                                                                             *
 *    This routine is used to transition the aircraft from the limbo to the non limbo state.   *
 *    It occurs when the aircraft is placed on the map for whatever reason. When it is         *
 *    unlimboed, only then will normal game processing recognize it.                           *
 *                                                                                             *
 * INPUT:   coord -- The coordinate that the aircraft should appear at.                        *
 *                                                                                             *
 *          dir   -- The direction it should start facing.                                     *
 *                                                                                             *
 *				strength (optional) -- sets initial strength													  *
 *                                                                                             *
 *				mission (optional) -- sets initial mission													  *
 *                                                                                             *
 * OUTPUT:  bool; Was the aircraft unlimboed successfully?                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Unlimbo(COORDINATE coord, DirType dir)
{
	Validate();
	if (FootClass::Unlimbo(coord, dir)) {

		/*
		**	Ensure that the owning house knows about the
		**	new object.
		*/
		House->AScan |= (1L << Class->Type);
		House->ActiveAScan |= (1L << Class->Type);

		/*
		**	Forces the body of the helicopter to face the correct direction.
		*/
		SecondaryFacing = dir;

		/*
		**	Start rotor animation.
		*/
		Set_Rate(1);
		Set_Stage(0);

		/*
		**	Presume it starts in flight?
		*/
		if (Altitude == FLIGHT_LEVEL) {
			Set_Speed(0xFF);
		} else {
			Set_Speed(0);
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * AircraftClass::Draw_It -- Renders an aircraft object at the location specified.             *
 *                                                                                             *
 *    This routine is used to display the aircraft object at the coordinates specified.        *
 *    The tactical map display uses this routine for all aircraft rendering.                   *
 *                                                                                             *
 * INPUT:   x,y      -- The coordinates to render the aircraft at.                             *
 *                                                                                             *
 *          window   -- The window that the coordinates are based upon.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Draw_It(int x, int y, WindowNumberType window)
{
	Validate();
	void const * shapefile;		// Working shape file pointer.
	int shapenum = 0;
	int facing = Facing_To_32(SecondaryFacing);


	/*
	**	Verify the legality of the unit class.
	*/
	shapefile = Class->Get_Image_Data();
	if (!shapefile) return;
	shapenum = UnitClass::BodyShape[facing];

	/*
	**	The orca attack helicopter uses a special shape set when it is travelling
	**	forward above a certain speed.
	*/
	if (*this == AIRCRAFT_ORCA && Get_Speed() >= MPH_MEDIUM_FAST) {
		shapenum += 32;
	}

	/*
	**	If there is a door on this aircraft (Chinook), then adjust the
	**	shape number to match the door open state.
	*/
	if (!Is_Door_Closed()) {
		shapenum = 32 + Door_Stage();
	}

	/*
	**	Helicopters that are flying have a "bobbing" effect.
	*/
	int jitter = 0;
	if (Altitude == FLIGHT_LEVEL && !Class->IsFixedWing) {
		Jitter++;

		static int _jitter[] = {0,0,0,0,1,1,1,0,0,0,0,0,-1,-1,-1,0};
		jitter = _jitter[Jitter % 16];
	}

	// Virtual window needs to draw the body first so it's considered the primary object and the shadow is a sub-object
	if (window == WINDOW_VIRTUAL) {
		/*
		**	Draw the root body of the unit.
		*/
		Techno_Draw_Object(shapefile, shapenum, x, (y - Altitude) + jitter, window);

		/*
		**	Special manual shadow draw code.
		*/
		if (Visual_Character() <= VISUAL_DARKEN) {
			CC_Draw_Shape(this, shapefile, shapenum, x + 1, y + 2, window, SHAPE_PREDATOR | SHAPE_CENTER | SHAPE_WIN_REL | SHAPE_FADING, Map.FadingShade, NULL);
		}
	} else {
		/*
		**	Special manual shadow draw code.
		*/
		if (Visual_Character() <= VISUAL_DARKEN) {
			CC_Draw_Shape(this, shapefile, shapenum, x+1, y+2, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING, Map.FadingShade, NULL);
		}

		/*
		**	Draw the root body of the unit.
		*/
		Techno_Draw_Object(shapefile, shapenum, x, (y-Altitude)+jitter, window);
	}

	/*
	**	Draw rotor effects. The rotor art can be either generic or custom. Custom rotor
	**	art has a different rotor set for each facing. Rotor shapes occur after the first
	**	32 shapes of the helicopter body.
	*/
	if (Class->IsRotorEquipped) {
		ShapeFlags_Type flags = SHAPE_CENTER|SHAPE_WIN_REL;

		/*
		**	The rotor shape number depends on whether the helicopter is idling
		**	or not. A landed helicopter uses slow moving "idling" blades.
		*/
		if (Altitude == 0) {
			shapenum = (Fetch_Stage()%8)+4;
			flags = flags | SHAPE_GHOST;
		} else {
			shapenum = Fetch_Stage()%4;
			flags = flags | SHAPE_FADING|SHAPE_PREDATOR;
		}

		if (*this == AIRCRAFT_TRANSPORT) {
			int _stretch[FACING_COUNT] = {8, 9, 10, 9, 8, 9, 10, 9};

			/*
			**	Dual rotors offset along flight axis.
			*/
			short xx = x;
			short yy = y-Altitude;
			FacingType face = Dir_Facing(SecondaryFacing);
			Move_Point(xx, yy, SecondaryFacing.Current(), _stretch[face]);
			//CC_Draw_Shape(Class->RRotorData, shapenum, xx, yy-2, window, flags, NULL, Map.UnitShadow);		// 6/18/2019 - ST
			CC_Draw_Shape(this, "RROTOR", Class->RRotorData, shapenum, xx, yy - 2, window, flags, NULL, Map.UnitShadow);

			Move_Point(xx, yy, SecondaryFacing.Current()+DIR_S, _stretch[face]*2);
			//CC_Draw_Shape(this, Class->LRotorData, shapenum, xx, yy-2, window, flags, NULL, Map.UnitShadow);			// 6/18/2019 - ST
			CC_Draw_Shape(this, "LROTOR", Class->LRotorData, shapenum, xx, yy - 2, window, flags, NULL, Map.UnitShadow);

		} else {

			/*
			**	Single rotor centered about shape.
			*/
			//CC_Draw_Shape(this, Class->RRotorData, shapenum, x, (y-Altitude)-2, window, flags, NULL, Map.UnitShadow);	// 6/18/2019 - ST
			CC_Draw_Shape(this, "RROTOR", Class->RRotorData, shapenum, x, (y - Altitude) - 2, window, flags, NULL, Map.UnitShadow);
		}
	}

	FootClass::Draw_It(x, y-Altitude, window);
}


/***********************************************************************************************
 * AircraftClass::Read_INI -- Reads aircraft object data from an INI file.                     *
 *                                                                                             *
 *    This routine is used to read the aircraft object data from the INI file buffer           *
 *    specified. This is used by the scenario loader code to interpret the INI file and        *
 *    create the specified objects therein.                                                    *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the INI buffer.                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Read_INI(char *buffer)
{
	AircraftClass	*air;			// Working unit pointer.
	char				*tbuffer;		// Accumulation buffer of unit IDs.
	HousesType		inhouse;			// Unit house.
	AircraftType	classid;			// Unit class.
	int				len;				// Length of data in buffer.
	char				buf[128];

	len = strlen(buffer) + 2;
	tbuffer = buffer + len;

	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-len, buffer);
	while (*tbuffer != '\0') {

		WWGetPrivateProfileString(INI_Name(), tbuffer, NULL, buf, sizeof(buf)-1, buffer);
		inhouse = HouseTypeClass::From_Name(strtok(buf, ","));
		if (inhouse != HOUSE_NONE) {
			classid = AircraftTypeClass::From_Name(strtok(NULL, ","));

			if (classid != AIRCRAFT_NONE) {

				if (HouseClass::As_Pointer(inhouse) != NULL) {
					air = new AircraftClass(classid, inhouse);
					if (air) {
						COORDINATE	coord;
						int	strength;
						DirType dir;

						/*
						**	Read the raw data.
						*/
						strength = atoi(strtok(NULL, ","));
						coord = Cell_Coord((CELL)atoi(strtok(NULL, ",")));
						dir = (DirType)atoi(strtok(NULL, ","));

						if (!Map.In_Radar(Coord_Cell(coord))) {
							delete air;
						} else {

							air->Strength = Fixed_To_Cardinal(air->Class->MaxStrength, strength);
							if (air->Unlimbo(coord, dir)) {
								air->Assign_Mission(AircraftClass::Mission_From_Name(strtok(NULL, ",\n\r")));
							} else {
								delete air;
							}
						}
					}
				}
			}
		}
		tbuffer += strlen(tbuffer)+1;
	}
}


/***********************************************************************************************
 * AircraftClass::Write_INI -- Writes the current aircraft objects to an INI file.             *
 *                                                                                             *
 *    This routine is used to output the current list of aircraft objects to the INI file      *
 *    buffer specified. It is typically used by the scenario editor.                           *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer that holds the INI file data.                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Write_INI(char *buffer)
{
	int	index;
	char	uname[10];
	char	buf[128];
	char	*tbuffer;		// Accumulation buffer of unit IDs.

	/*
	**	First, clear out all existing unit data from the ini file.
	*/
	tbuffer = buffer + strlen(buffer) + 2;
	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-strlen(buffer), buffer);
	while (*tbuffer != '\0') {
		WWWritePrivateProfileString(INI_Name(), tbuffer, NULL, buffer);
		tbuffer += strlen(tbuffer)+1;
	}

	/*
	**	Write the unit data out.
	*/
	for (index = 0; index < Aircraft.Count(); index++) {
		AircraftClass * unit;

		unit = Aircraft.Ptr(index);
		if (!unit->IsInLimbo) {

			sprintf(uname, "%03d", index);
			sprintf(buf, "%s,%s,%d,%u,%d,%s",
				unit->House->Class->IniName,
				unit->Class->IniName,
				unit->Health_Ratio(),
				Coord_Cell(unit->Coord),
				unit->PrimaryFacing.Current(),
				MissionClass::Mission_Name(unit->Mission)
				);
			WWWritePrivateProfileString(INI_Name(), uname, buf, buffer);
		}
	}
}


/***********************************************************************************************
 * AircraftClass::Mission_Hunt -- Maintains hunt AI for the aircraft.                          *
 *                                                                                             *
 *    Hunt AI consists of finding a target and attacking it. If there is no target assigned    *
 *    and this unit doesn't automatically hunt for more targets, then it will change           *
 *    mission to a more passive (land and await further orders) type.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of ticks before calling this routine again.                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Mission_Hunt(void)
{
	Validate();
	if (Class->IsFixedWing) {
		enum {
			LOOK_FOR_TARGET,
			FLY_TO_TARGET,
			DROP_BOMBS
		};
		switch (Status) {

			/*
			**	Acquiring target stage.
			*/
			case LOOK_FOR_TARGET:
				if (Target_Legal(TarCom)) {
					Status = FLY_TO_TARGET;
					return(1);
				} else {
					Assign_Target(Greatest_Threat(THREAT_NORMAL));

					/*
					**	If there is no target, then this aircraft should just do its normal thing.
					*/
					if (!Target_Legal(TarCom)) {
						Enter_Idle_Mode();
					}
				}
				break;

			/*
			**	Homing in on target stage.
			*/
			case FLY_TO_TARGET:
				if (Target_Legal(TarCom)) {
					IsHoming = true;
					if (!PrimaryFacing.Is_Rotating()) {
						PrimaryFacing.Set_Desired(Direction(TarCom));
					}
					if (Distance(TarCom) < 0x0380) {
						IsHoming = false;
						Status = DROP_BOMBS;
						return(1);
					}
				} else {
					Status = LOOK_FOR_TARGET;
				}
				break;

			/*
			**	Dropping a stream of bombs phase.
			*/
			case DROP_BOMBS:
				if (!Ammo) {
					AttacksRemaining--;
					if (!AttacksRemaining) {
						Assign_Mission(MISSION_RETREAT);
						Commence();
					} else {
						Ammo = Class->MaxAmmo;
						Status = LOOK_FOR_TARGET;
					}
				}
				if (!Target_Legal(TarCom)) {
					Status = LOOK_FOR_TARGET;
				} else {
					Fire_At(TarCom, 0);
					Map[::As_Cell(TarCom)].Incoming(Coord, true);
					return(5);
				}
				break;
		}
	} else {
		if (!Ammo) {
			Enter_Idle_Mode();
		} else {
			Assign_Mission(MISSION_ATTACK);
			return(1);
//			return(FootClass::Mission_Hunt());
		}
	}
	return(TICKS_PER_SECOND);
}


/***********************************************************************************************
 * AircraftClass::AI -- Processes the normal non-graphic AI for the aircraft.                  *
 *                                                                                             *
 *    This handles the non-graphic AI processing for the aircraft. This usually entails        *
 *    maintenance and other AI functions.                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::AI(void)
{
	Validate();
	/*
	**	A Mission change can always occur if the aircraft is landed or flying.
	*/
	if (!IsLanding && !IsTakingOff) {
		Commence();
	}

	FootClass::AI();

	/*
	**	A Mission change can always occur if the aircraft is landed or flying.
	*/
	if (!IsLanding && !IsTakingOff) {
		Commence();
	}

	/*
	**	Handle any body rotation at this time. Body rotation can occur even if the
	**	flying object is not actually moving.
	*/
	if (PrimaryFacing.Is_Rotating()) {
		if (PrimaryFacing.Rotation_Adjust(Class->ROT)) {
			Mark();
		}
	}
	if (Class->IsFixedWing) {
		SecondaryFacing = PrimaryFacing;
	}
	if (SecondaryFacing.Is_Rotating()) {
		if (SecondaryFacing.Rotation_Adjust(Class->ROT)) {
			Mark();
		}
	}
	if (Physics(Coord, PrimaryFacing) != RESULT_NONE) {
		Mark();
	}

	/*
	**	Perform sighting every so often as controlled by the sight timer.
	*/
	//if (IsOwnedByPlayer && Class->SightRange && SightTimer.Expired()) {		// Changed for multiple player mapping
	if (House->IsHuman && Class->SightRange && SightTimer.Expired()) {
		Map.Sight_From(House, Coord_Cell(Coord), Class->SightRange, false);
		SightTimer = TICKS_PER_SECOND;
	}

	/*
	**	Handle landing and taking off logic. Helicopters are prime users of this technique. The
	**	aircraft will either gain or lose altitude as appropriate. As the aircraft transitions
	**	between flying level and ground level, it will be moved into the appropriate render
	**	layer.
	*/
	if (Is_Door_Closed() && (IsLanding || IsTakingOff)) {
		Mark();
		LayerType layer = In_Which_Layer();

		if (IsLanding) {
			if (Altitude) Altitude--;
			if (!Altitude) {
				IsLanding = false;
				Set_Speed(0);
				if (Target_Legal(NavCom) && As_Techno(NavCom) == Contact_With_Whom()) {
					if (In_Radio_Contact() && Transmit_Message(RADIO_IM_IN) != RADIO_ROGER) {
						Scatter(0, true);
					}
				}
			}
		}
		if (IsTakingOff) {
			Altitude++;
			if (Altitude >= FLIGHT_LEVEL) {
				Altitude = FLIGHT_LEVEL;
				IsTakingOff = false;
			}
		}

		/*
		**	Make adjustments for altitude by moving from one layer to another as
		**	necessary.
		*/
		if (layer != In_Which_Layer()) {

			/*
			**	When the aircraft is about to enter the ground layer, perform on last
			**	check to see if it is legal to enter that location. If not, then
			**	start the take off process. Let the normal logic handle this
			**	change of plans.
			*/
			bool ok = true;
			if (In_Which_Layer() == LAYER_GROUND) {
				if (!Is_LZ_Clear(::As_Target(Coord_Cell(Coord)))) {
					IsTakingOff = true;
					Altitude++;
					ok = false;
				}
			}

			if (ok) {
				/*
				**	If landing in a cell that already contains an object, then
				**	the landing attempt must be aborted.
				*/
				Map.Remove(this, layer);
				Map.Submit(this, In_Which_Layer());

				/*
				**	When the aircraft is close to the ground, it should exist as a ground object.
				**	This aspect is controlled by the Place_Down and Pick_Up functions.
				*/
				if (In_Which_Layer() == LAYER_GROUND) {
					Assign_Destination(TARGET_NONE);		// Clear the navcom.
					Transmit_Message(RADIO_TETHER);
					Map.Place_Down(Coord_Cell(Coord), this);
					//if (IsOwnedByPlayer) {				// Changed for multiple player mapping. ST - 3/6/2019 1:31PM
					if (House->IsHuman) {
						Map.Sight_From(House, Coord_Cell(Coord), 1, false);
					}
				} else  {
					Transmit_Message(RADIO_UNTETHER);
					Map.Pick_Up(Coord_Cell(Coord), this);

					/*
					**	If the navigation computer is not attached to the object this
					**	aircraft is in radio contact with, then assume that radio
					**	contact is now superfluous. Break radio contact.
					*/
					if (In_Radio_Contact() && Target_Legal(NavCom) && NavCom != Contact_With_Whom()->As_Target()) {
						Transmit_Message(RADIO_OVER_OUT);
					}
				}
			}
		}
	}

	/*
	**	Always flag the map draw process to occur if there is an aircraft in the view.
	**	This ensures that it will be rendered even if there is nothing else that flagged
	**	the map to be redrawn.
	*/
	if (Map.In_View(Coord_Cell(Coord))) {
		Map.Flag_To_Redraw(false);
		Map.DisplayClass::IsToRedraw = true;
	}

	/*
	**	When aircraft leave the edge of the map, they might get destroyed. This occurs if the
	**	aircraft is a non-player produced unit and it has completed its mission. A transport
	**	helicopter that has already delivered reinforcements is a good example of this.
	*/
	if (!Map.In_Radar(Coord_Cell(Coord))) {
		if (Mission == MISSION_RETREAT /*|| (*this == AIRCRAFT_CARGO && !Is_Something_Attached())*/) {

			/*
			**	Check to see if there are any civilians aboard. If so, then flag the house
			**	that the civilian evacuation trigger event has been fulfilled.
			*/
			while (Is_Something_Attached()) {
				FootClass * obj = Detach_Object();
				if (obj->What_Am_I() == RTTI_INFANTRY && ((InfantryClass *)obj)->Class->IsCivilian && !((InfantryClass *)obj)->IsTechnician) {
					House->IsCivEvacuated = true;
				}

				/*
				**	Transport planes that leave can only be because they carry purchased
				**	equipment and must be have their cost refunded.
				*/
				if (*this == AIRCRAFT_CARGO) {
					House->Refund_Money(obj->Class_Of().Cost_Of());
				}
				delete obj;
			}
			Stun();
			Delete_This();
			return;
		}
	} else {
		IsLocked = true;
//		House->NewAScan |= (1L << Class->Type);

#ifdef NEVER
		/*
		**	Transport helicopters must ensure that their passengers are properly
		**	considered "alive" by setting the appropriate scan bits.
		*/
		FootClass const * foot = Attached_Object();
		while (foot) {
			switch (foot->What_Am_I()) {
				case RTTI_UNIT:
					House->NewUScan |= (1L << ((UnitTypeClass const &)Class_Of()).Type);
					break;

				case RTTI_INFANTRY:
					House->NewIScan |= (1L << ((InfantryTypeClass const &)Class_Of()).Type);
					break;
			}

			foot = (FootClass const *)foot->Next;
		}
#endif
	}
}


/***********************************************************************************************
 * AircraftClass::Mark -- Flags cells under the aircraft so that they will be redrawn.         *
 *                                                                                             *
 *    This routine is used to flag the cells under the aircraft so that those cells will       *
 *    be redrawn during the next map drawing process. This is a necessary step whenever the    *
 *    aircraft moves or changes shape.                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Mark(MarkType mark)
{
	Validate();
	if (FootClass::Mark(mark)) {
		Map.Refresh_Cells(Coord_Cell(Coord), Occupy_List());
		Map.Refresh_Cells(Coord_Cell(Coord), Overlap_List());
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * AircraftClass::Overlap_List -- Returns with list of cells the aircraft overlaps.            *
 *                                                                                             *
 *    When aircraft are flying, they can overlap quite a number of cells. These cells can      *
 *    be determined from the coordinate where the aircraft is centered and the size of the     *
 *    aircraft's shape. Landed aircraft are a special case and are usually much smaller        *
 *    than when flying.                                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a cell offset list that specifies all cells that         *
 *          the aircraft overlaps given the aircraft's current state.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
short const *AircraftClass::Overlap_List(void) const
{
	Validate();
	static short const _list[] = {
		-(MAP_CELL_W-1), -MAP_CELL_W, -(MAP_CELL_W+1),
		-1, 0, 1,
		(MAP_CELL_W-1), MAP_CELL_W, (MAP_CELL_W+1),
		-((MAP_CELL_W*2)-1), -(MAP_CELL_W*2), -((MAP_CELL_W*2)+1),
		-((MAP_CELL_W*3)-1), -(MAP_CELL_W*3), -((MAP_CELL_W*3)+1),
		REFRESH_EOL
	};

	if (Altitude) {
		return(_list);
		//return Coord_Spillage_List(Coord, 25);
	}
	return(Class->Overlap_List());
}


/***********************************************************************************************
 * AircraftClass::Init -- Initialize the aircraft system to an empty state.                    *
 *                                                                                             *
 *    This routine is used to clear out the aircraft allocation system. It is called in        *
 *    preparation for a scenario load or save game load.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Init(void)
{
	AircraftClass *ptr;

	Aircraft.Free_All();

	ptr = new AircraftClass();
	VTable = ((void **)(((char *)ptr) + sizeof(AbstractClass) - 4))[0];
	delete ptr;
}


/***********************************************************************************************
 * AircraftClass::Mission_Unload -- Handles unloading cargo.                                   *
 *                                                                                             *
 *    This function is used to handle finding, heading toward, landing, and unloading the      *
 *    cargo from the aircraft. Once unloading of cargo has occurred, then the aircraft follows *
 *    a different mission.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of game ticks to delay before calling this function again.      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/31/94   JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Mission_Unload(void)
{
	Validate();
	if (Class->IsFixedWing) {
		enum {
			PICK_AIRSTRIP,
			FLY_TO_AIRSTRIP,
			BUG_OUT
		};

		switch (Status) {

			/*
			**	Find a suitable airfield to land at.
			*/
			case PICK_AIRSTRIP:
				if (!Target_Legal(NavCom) || !In_Radio_Contact()) {

					BuildingClass * target_building = As_Building(NavCom);
					BuildingClass * building = (target_building != NULL && *target_building == STRUCT_AIRSTRIP) ? target_building : Find_Docking_Bay(STRUCT_AIRSTRIP, false);
					if (building) {
						if (Transmit_Message(RADIO_HELLO, building) == RADIO_ROGER) {
							Set_Speed(0xFF);
							Assign_Destination(building->As_Target());
							if (Team) {
								Team->Target = NavCom;
							}
							Status = FLY_TO_AIRSTRIP;
						}
					}

					/*
					**	If a suitable airfield could not be found, then just randomly change
					**	direction and then try again later.
					*/
					if (Status == PICK_AIRSTRIP) {

						/*
						**	If there are no more airstrips, regardless of busy state, then
						**	abort this transport plane completely.
						*/
						if (!(House->ActiveBScan & STRUCTF_AIRSTRIP)) {
							Assign_Mission(MISSION_RETREAT);
						}

						/*
						**	Pick a new direction and fly off.
						*/
						PrimaryFacing.Set_Desired(Random_Pick(DIR_N, DIR_MAX));
						SecondaryFacing.Set_Desired(PrimaryFacing.Desired());
						return(TICKS_PER_SECOND*3);
					}
				} else {
					Status = FLY_TO_AIRSTRIP;
				}
				break;

			/*
			**	Home in on target. When close enough, drop the cargo.
			*/
			case FLY_TO_AIRSTRIP:
				if (!Target_Legal(NavCom) || !In_Radio_Contact()) {
					Status = PICK_AIRSTRIP;
				} else {

					/*
					**	If, for some reason, there is no cargo, then don't stick around.
					*/
					if (!Is_Something_Attached()) {
						Status = BUG_OUT;
						return(1);
					}

					if (!PrimaryFacing.Is_Rotating()) {
						PrimaryFacing.Set_Desired(Direction(As_Movement_Coord(NavCom)));
					}
					SecondaryFacing.Set_Desired(PrimaryFacing.Desired());

					int navdist = Distance(As_Movement_Coord(NavCom));
					Altitude = FLIGHT_LEVEL;
					if (navdist < 0x0600) {
						Altitude = Fixed_To_Cardinal(FLIGHT_LEVEL, Cardinal_To_Fixed(0x0600, navdist));
					}

					if (navdist < 0x0080) {
						FootClass * unit = (FootClass *)Detach_Object();

						if (unit) {
							CELL cell = Contact_With_Whom()->Find_Exit_Cell(unit);
							if (cell) {
								ScenarioInit++;
								if (!unit->Unlimbo(Cell_Coord(cell))) {
									Attach(unit);
								} else {

									/*
									**	Cargo planes announce reinforcements when they unload
									**	their cargo.
									*/
									if (*this == AIRCRAFT_CARGO && House == PlayerPtr) {
										Speak(VOX_REINFORCEMENTS, NULL, Cell_Coord(cell));
									}
									unit->IsALoaner = false;
									unit->IsLocked = true;
									unit->Scatter(0, true);
								}
								ScenarioInit--;

								Transmit_Message(RADIO_OVER_OUT);
								Assign_Target(TARGET_NONE);
							} else {
								Attach(unit);
							}

//							if (Is_Something_Attached()) {
//								Status = PICK_AIRSTRIP;
//							} else {
								Status = BUG_OUT;
//							}
						} else {
							Status = BUG_OUT;
						}
					}
					return(1);
				}
				break;

			/*
			**	All cargo unloaded, head off the map.
			*/
			case BUG_OUT:
				Assign_Mission(MISSION_RETREAT);
				return(1);
		}

	} else {
		enum {
			SEARCH_FOR_LZ,
			FLY_TO_LZ,
			LAND_ON_LZ,
			UNLOAD_PASSENGERS,
			TAKE_OFF,
		};

		switch (Status) {

			/*
			**	Search for an appropriate destination spot if one isn't already assigned.
			*/
			case SEARCH_FOR_LZ:
				if (Altitude == 0 && (Target_Legal(NavCom) || Coord == As_Coord(NavCom))) {
					Status = UNLOAD_PASSENGERS;
				} else {
					if (!Is_LZ_Clear(NavCom)) {
						Assign_Destination(New_LZ(::As_Target(Waypoint[WAYPT_REINF])));
					} else {
						if (Altitude == FLIGHT_LEVEL) {
							Status = FLY_TO_LZ;
						} else {
							Status = TAKE_OFF;
						}
					}
				}
				break;

			/*
			**	Fly to destination.
			*/
			case FLY_TO_LZ:
				if (Is_LZ_Clear(NavCom)) {
					int distance = Process_Fly_To(true);

					if (distance < 0x0100) {
						SecondaryFacing.Set_Desired(Pose_Dir());

						if (distance < 0x0010) {
							Status = LAND_ON_LZ;
						}
						return(1);
					} else {
						SecondaryFacing.Set_Desired(PrimaryFacing.Desired());
						return(5);
					}
				} else {
					Status = SEARCH_FOR_LZ;
				}
				break;

			/*
			**	Landing phase. Just delay until landing is complete. At that time,
			**	transition to the unloading phase.
			*/
			case LAND_ON_LZ:
				if (IsTakingOff) {
					Status = TAKE_OFF;
				} else {
					if (Process_Landing()) {
						Status = UNLOAD_PASSENGERS;
					}
				}
				return(1);

			/*
			**	Hold while unloading passengers. When passengers are unloaded the order for this
			**	transport gets changed to MISSION_RETREAT.
			*/
			case UNLOAD_PASSENGERS:
				if (!IsTethered) {
					if (Is_Something_Attached()) {
						FootClass * unit = (FootClass *)Detach_Object();

						/*
						**	First thing is to lift the transport off of the map so that the unlimbo
						**	process for the passengers is more likely to succeed.
						*/
						Map.Pick_Up(Coord_Cell(Coord), this);

						if (!Exit_Object(unit)) {
							delete unit;
						}

						/*
						**	Restore the transport back down on the map.
						*/
						Map.Place_Down(Coord_Cell(Coord), this);

						if (!Is_Something_Attached()) {
							Enter_Idle_Mode();
						}

					} else  {

						Enter_Idle_Mode();
					}
				}
				break;

			/*
			**	Aircraft is now taking off. Once the aircraft reaches flying altitude then it
			**	will either take off or look for another landing spot to try again.
			*/
			case TAKE_OFF:  {
				if (Process_Take_Off()) {
					if (Is_Something_Attached()) {
						Status = SEARCH_FOR_LZ;

						/*
						**	Break off radio contact with the helipad it is taking off from.
						*/
						if (In_Radio_Contact() && Map[Coord_Cell(Coord)].Cell_Building() == Contact_With_Whom()) {
							Transmit_Message(RADIO_OVER_OUT);
						}
					} else  {
						Enter_Idle_Mode();
					}
				}
				return(1);
			}
		}
	}
	return(10);
}


/***********************************************************************************************
 * AircraftClass::Is_LZ_Clear -- Determines if landing zone is free for landing.               *
 *                                                                                             *
 *    This routine examines the landing zone (as specified by the target parameter) in order   *
 *    to determine if it is free to be landed upon. Call this routine when it is necessary     *
 *    to double check this. Typically this occurs right before a helicopter lands and also     *
 *    when determining the landing zone in the first place.                                    *
 *                                                                                             *
 * INPUT:   target   -- The target that is the "landing zone".                                 *
 *                                                                                             *
 * OUTPUT:  bool; Is the landing zone clear for landing?                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/31/94   JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Is_LZ_Clear(TARGET target) const
{
	Validate();
	if (!Target_Legal(target)) return(false);
	CELL cell = ::As_Cell(target);
	if (!Map.In_Radar(cell)) return(false);

	ObjectClass * object = Map[cell].Cell_Object();
	if (object) {
		if (object == this) return(true);

		if (In_Radio_Contact() && Contact_With_Whom() == object) {
			return(true);
		}
		return(false);
	}

	if (!Map[cell].Is_Generally_Clear()) return(false);

	return(true);
}


/***********************************************************************************************
 * AircraftClass::In_Which_Layer -- Determine which render layer the aircraft lies.            *
 *                                                                                             *
 *    This routine is used to figure out which rendering layer the aircraft is located in.     *
 *    It can be determined from the aircraft's height. The layer value is used to handle the   *
 *    display sequence. Objects in lower layers appear beneath objects in higher layers.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the layer that the aircraft is currently located in.                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
LayerType AircraftClass::In_Which_Layer(void) const
{
	Validate();
	if (Class->IsFixedWing) return(LAYER_TOP);

	if (Altitude < FLIGHT_LEVEL - (FLIGHT_LEVEL/3)) {
		return(LAYER_GROUND);
	}
	return(LAYER_TOP);
}


/***********************************************************************************************
 * AircraftClass::Sort_Y -- Figures the sorting coordinate.                                    *
 *                                                                                             *
 *    This routine is used to determine the coordinate to use for sorting the aircraft. This   *
 *    sorting value is used when the aircraft is on the ground. At that time the aircraft      *
 *    must be rendered in proper relationship to the other ground objects.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate to use when sorting the aircraft with other ground     *
 *          objects.                                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE AircraftClass::Sort_Y(void) const
{
	Validate();
	return(Coord_Add(Coord, 0x00800000L));
}


/***********************************************************************************************
 * AircraftClass::Mission_Retreat -- Handles the aircraft logic for leaving the battlefield.   *
 *                                                                                             *
 *    This mission will be followed when the aircraft decides that it is time to leave the     *
 *    battle. Typically, this occurs when a loaner transport has dropped off its load or when  *
 *    an attack air vehicle has expended its ordinance.                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game ticks to delay before calling this routine again.  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1995 JLB : Created.                                                                 *
 *   08/13/1995 JLB : Handles aircraft altitude gain after takeoff logic.                      *
 *=============================================================================================*/
int AircraftClass::Mission_Retreat(void)
{
	Validate();
	if (Class->IsFixedWing) {
		if (Class->IsFixedWing && Altitude < FLIGHT_LEVEL) {
			Altitude++;
			return(3);
		}
		return(TICKS_PER_SECOND*10);
	}

	enum {
		TAKE_OFF,
		FACE_MAP_EDGE,
		KEEP_FLYING
	};
	switch (Status) {

		/*
		**	Take off if landed.
		*/
		case TAKE_OFF:
			if (Process_Take_Off()) {
				Status = FACE_MAP_EDGE;
			}
			return(1);

		/*
		**	Set facing and speed toward the friendly map edge.
		*/
		case FACE_MAP_EDGE:
			Set_Speed(0xFF);

			/*
			**	Take advantage of the fact that the source map edge enumerations happen to
			**	occur in a clockwise order and are the first four enumerations of the map
			**	edge default for the house. If this value is masked and then shifted, a
			**	normalized direction value results. Use this value to head the aircraft
			**	toward the "friendly" map edge.
			*/
			PrimaryFacing.Set_Desired((DirType)((House->Edge & 0x03) << 6));
			SecondaryFacing.Set_Desired(PrimaryFacing.Desired());
			Status = KEEP_FLYING;
			break;

		/*
		**	Just do nothing since we are headed toward the map edge. When the edge is
		**	reached, the aircraft should be automatically eliminated.
		*/
		case KEEP_FLYING:
			break;

		default:
			break;
	}
	return(TICKS_PER_SECOND);
}


/***********************************************************************************************
 * AircraftClass::Exit_Object -- Unloads passenger from aircraft.                              *
 *                                                                                             *
 *    This routine is called when the aircraft is to unload a passenger. The passenger must    *
 *    be able to move under its own power. Typical situation is when a transport helicopter    *
 *    is to unload an infantry unit.                                                           *
 *                                                                                             *
 * INPUT:   unit  -- Pointer to the unit that is to be unloaded from this aircraft.            *
 *                                                                                             *
 * OUTPUT:  bool; Was the unit unloaded successfully?                                          *
 *                                                                                             *
 * WARNINGS:   The unload process is merely started by this routine. Radio contact is          *
 *             established with the unloading unit and when the unit is clear of the aircraft  *
 *             the radio contact will be broken and then the aircraft is free to pursue        *
 *             other.                                                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Exit_Object(TechnoClass * unit)
{
	Validate();
	static FacingType _toface[FACING_COUNT] = {FACING_S, FACING_SW, FACING_SE, FACING_NW, FACING_NE, FACING_N, FACING_W, FACING_E};
	CELL	cell;

	/*
	**	Find a free cell to drop the unit off at.
	*/
	FacingType face;
	for (face = FACING_N; face < FACING_COUNT; face++) {
		cell = Adjacent_Cell(Coord_Cell(Coord), _toface[face]);
		if (unit->Can_Enter_Cell(cell) == MOVE_OK) break;
	}

	// Should perform a check here to see if no cell could be found.

	/*
	**	If the passenger can be placed on the map, then start it moving toward the
	**	destination cell and establish radio contact with the transport. This is used
	**	to make sure that the transport waits until the passenger is clear before
	**	unloading the next passenger or taking off.
	*/
	if (unit->Unlimbo(Coord, Facing_Dir(_toface[face]))) {
		unit->Assign_Mission(MISSION_MOVE);
		unit->Assign_Destination(::As_Target(cell));
		if (Transmit_Message(RADIO_HELLO, unit) == RADIO_ROGER) {
			Transmit_Message(RADIO_UNLOAD);
		}
		unit->Look(false);
		return(1);
	}
	return(0);
}


/***********************************************************************************************
 * AircraftClass::Fire_At -- Handles firing a projectile from an aircraft.                     *
 *                                                                                             *
 *    Sometimes, aircraft firing needs special handling. Example: for napalm bombs, the        *
 *    bomb travels forward at nearly the speed of the delivery aircraft, not necessarily the   *
 *    default speed defined in the BulletTypeClass structure.                                  *
 *                                                                                             *
 * INPUT:   target   -- The target that the projectile is heading for.                         *
 *                                                                                             *
 *          which    -- Which weapon to use in the attack. 0=primary, 1=secondary.             *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the bullet that was created as a result of this attack.  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
BulletClass * AircraftClass::Fire_At(TARGET target, int which)
{
	Validate();
	BulletClass * bullet = FootClass::Fire_At(target, which);

	if (bullet) {

		/*
		**	Aircraft reveal when firing
		*/
		HouseClass *player = HouseClass::As_Pointer(Owner());
		if (player != nullptr && player->IsHuman) {
			Map.Sight_From(player, Coord_Cell(Center_Coord()), 1, false);
		}

		/*
		**	Play the sound effect associated with this weapon.
		*/
		WeaponTypeClass const * weapon = (which == 0) ? &Weapons[Class->Primary] : &Weapons[Class->Secondary];
		Sound_Effect(weapon->Sound, Coord);


		/*
		**	Falling bullets move at a speed proportionate to the delivery craft.
		*/
		if (bullet->Class->IsDropping) {
			bullet->Fly_Speed(40, bullet->Class->MaxSpeed);
		}
	}
	return(bullet);
}


/***********************************************************************************************
 * AircraftClass::Take_Damage -- Applies damage to the aircraft.                               *
 *                                                                                             *
 *    This routine is used to apply damage to the specified aircraft. This is where any        *
 *    special crash animation will be initiated.                                               *
 *                                                                                             *
 * INPUT:   damage   -- Reference to the damage that will be applied to the aircraft.          *
 *                      This value will be filled in with the actual damage that was           *
 *                      applied.                                                               *
 *                                                                                             *
 *          distance -- Distance from the source of the explosion to this aircraft.            *
 *                                                                                             *
 *          warhead  -- The warhead type that the damage occurs from.                          *
 *                                                                                             *
 *          source   -- Pointer to the originator of the damage. This can be used so that      *
 *                      proper "thank you" can be delivered.                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the result of the damage as it affects this aircraft.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/26/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ResultType AircraftClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source)
{
	Validate();
	ResultType res = RESULT_NONE;

	/*
	**	Flying aircraft take half damage.
	*/
	if (Altitude) {
		damage /= 2;
	}

	/*
	**	In order for a this to be damaged, it must either be a unit
	**	with a crew or a sandworm.
	*/
	res = FootClass::Take_Damage(damage, distance, warhead, source);

	switch (res) {
		case RESULT_DESTROYED:
			Kill_Cargo(source);
			Death_Announcement();
			new AnimClass(ANIM_FBALL1, Target_Coord());
			Delete_This();
			break;

		default:
		case RESULT_HALF:
			break;
	}

	return(res);
}


/***********************************************************************************************
 * AircraftClass::Mission_Move -- Handles movement mission.                                    *
 *                                                                                             *
 *    This state machine routine is used when an aircraft (usually helicopter) is to move      *
 *    from one location to another. It will handle any necessary take off and landing this     *
 *    may require.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames that should elapse before this routine      *
 *          is called again.                                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Mission_Move(void)
{
	Validate();
	if (Class->IsFixedWing) {

		/*
		**	Force aircraft in movement mission into a retreat
		**	mission so that it leaves the map.
		*/
		if (*this == AIRCRAFT_A10) {
			Assign_Mission(MISSION_RETREAT);
			Commence();
			return(1);
		}

		enum {
			FLY_TO_AIRSTRIP,
			BUG_OUT
		};
		switch (Status) {
			/*
			**	Home in on target. When close enough, drop the cargo.
			*/
			case FLY_TO_AIRSTRIP:
				if (!Target_Legal(NavCom) || !In_Radio_Contact()) {
					return(TICKS_PER_SECOND);
				} else {

					/*
					**	If, for some reason, there is no cargo, then don't stick around.
					*/
					if (!Is_Something_Attached()) {
						Status = BUG_OUT;
						return(1);
					}


					if (!PrimaryFacing.Is_Rotating()) {
						PrimaryFacing.Set_Desired(Direction(NavCom));
					}
					SecondaryFacing.Set_Desired(PrimaryFacing.Desired());
					if (Distance(NavCom) < 0x0080) {
						FootClass * unit = (FootClass *)Detach_Object();

						if (unit) {
							ScenarioInit++;
							if (!unit->Unlimbo(Coord_Snap(Contact_With_Whom()->Coord))) {
								Attach(unit);
							}
							ScenarioInit--;

							Transmit_Message(RADIO_OVER_OUT);
							Assign_Target(TARGET_NONE);
						}
						Status = BUG_OUT;
					}
				}
				return(1);

			case BUG_OUT:
				return(TICKS_PER_SECOND);
		}
		return(5);
	}

	enum {
		VALIDATE_LZ,
		TAKE_OFF,
		FLY_TO_LZ,
		LAND
	};
	switch (Status) {

		/*
		**	Double check and change LZ if necessary.
		*/
		case VALIDATE_LZ:
			if (!Target_Legal(NavCom)) {
				Enter_Idle_Mode();
			} else {
				if (!Is_LZ_Clear(NavCom) || !Cell_Seems_Ok(As_Cell(NavCom))) {
					Assign_Destination(New_LZ(NavCom));
				} else {
					Status = TAKE_OFF;
				}
			}
			break;

		/*
		**	Take off if necessary.
		*/
		case TAKE_OFF:
			if (!Target_Legal(NavCom)) {
				Status = VALIDATE_LZ;
			} else {
				if (Process_Take_Off()) {
					/*
					**	After takeoff is complete, break radio contact with any helipad that this
					**	helicopter is taking off from.
					*/
					if (In_Radio_Contact() && Map[Coord_Cell(Coord)].Cell_Building() == Contact_With_Whom()) {
						Transmit_Message(RADIO_OVER_OUT);
					}

					Status = FLY_TO_LZ;
				}
				return(1);
			}
			break;

		/*
		**	Fly toward target.
		*/
		case FLY_TO_LZ:
			if (Is_LZ_Clear(NavCom)) {
				int distance = Process_Fly_To(true);

				if (distance < 0x0080) {
					if (Target_Legal(TarCom)) {
						SecondaryFacing.Set_Desired(Direction(TarCom));
					} else {
						SecondaryFacing.Set_Desired(Pose_Dir());
					}

					if (distance < 0x0010) {
						Status = LAND;
					}
					return(1);
				}

				SecondaryFacing.Set_Desired(::Direction(Fire_Coord(0), As_Coord(NavCom)));
			} else {
				Assign_Destination(New_LZ(NavCom));
				if (!Target_Legal(NavCom)) {
					Status = LAND;
				}
			}
			return(1);

		/*
		**	Land on target.
		*/
		case LAND:
			if (IsTakingOff) {
				Assign_Destination(New_LZ(NavCom));
				Status = TAKE_OFF;
			}
			if (Process_Landing()) {
				if (MissionQueue == MISSION_NONE) {
					Enter_Idle_Mode();
				}
			}
			return(1);
	}

	return(TICKS_PER_SECOND);
}


/***********************************************************************************************
 * AircraftClass::Enter_Idle_Mode -- Gives the aircraft an appropriate mission.                *
 *                                                                                             *
 *    Use this routine when the mission for the aircraft is in doubt. This routine will find   *
 *    an appropriate mission for the aircraft and dispatch it.                                 *
 *                                                                                             *
 * INPUT:   initial  -- Is this called when the unit just leaves a factory or is initially     *
 *                      or is initially placed on the map?                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/05/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Enter_Idle_Mode(bool )
{
	Validate();
	MissionType mission = MISSION_GUARD;
	if (In_Which_Layer() == LAYER_GROUND) {
		if (IsALoaner) {
			if (Is_Something_Attached()) {
				mission = MISSION_UNLOAD;
			} else {
				if (Team) {
					Team->Remove(this);
				}
				mission = MISSION_RETREAT;
			}
		} else {
#ifdef NEVER
			if (In_Radio_Contact() && Contact_With_Whom() == Map[Coord_Cell(Coord)].Cell_Techno()) {
				Transmit_Message(RADIO_IM_IN);
			}
#endif
			Assign_Destination(TARGET_NONE);
			Assign_Target(TARGET_NONE);
			mission = MISSION_GUARD;
		}
	} else {
		if (Is_Something_Attached()) {
			if (IsALoaner) {
				if (Team) {
					mission = MISSION_GUARD;
				} else {
					mission = MISSION_UNLOAD;
					Assign_Destination(Good_LZ());
				}
			} else {
				Assign_Destination(Good_LZ());
				mission = MISSION_MOVE;
			}
		} else {

			/*
			**	If this transport is a loaner and part of a team, then remove it from
			**	the team it is attached to.
			*/
			if (IsALoaner) {
				if (Team) {
					Team->Remove(this);
				}
			}

			if (Class->Primary != WEAPON_NONE) {

				/*
				**	Weapon equipped helicopters that run out of ammo and were
				**	brought in as reinforcements will leave the map.
				*/
				if (Ammo == 0 && !House->IsHuman && IsALoaner) {
					mission = MISSION_RETREAT;
				} else {

					/*
					**	Normal aircraft try to find a good landing spot to rest.
					*/
					BuildingClass * building = Find_Docking_Bay(STRUCT_HELIPAD, false);
					Assign_Destination(TARGET_NONE);
					if (building && Transmit_Message(RADIO_HELLO, building) == RADIO_ROGER) {
						mission = MISSION_ENTER;
					} else {
						Assign_Destination(Good_LZ());
						mission = MISSION_MOVE;
					}
				}
			} else {
				if (Team) return;

				Assign_Destination(Good_LZ());
				mission = MISSION_MOVE;
			}
		}
	}
	Assign_Mission(mission);
	Commence();
}


/***********************************************************************************************
 * AircraftClass::Process_Fly_To -- Handles state machine for flying to destination.           *
 *                                                                                             *
 *    This support routine is used when the helicopter is to fly to the destination. It can    *
 *    optionally slow the helicopter down as it approaches the destination.                    *
 *                                                                                             *
 * INPUT:   slowdown -- Should the aircraft be slowed down when it approaches the dest?        *
 *                                                                                             *
 * OUTPUT:  Returns with the distance remaining between the aircraft and the destination.      *
 *                                                                                             *
 * WARNINGS:   Because the aircraft can be move at a fast speed, the distance to target value  *
 *             will probably never be zero. The likely case will be that the aircraft          *
 *             overshoots the target.                                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/14/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Process_Fly_To(bool slowdown)
{
	Validate();
	COORDINATE coord;
	if (Is_Target_Building(NavCom)) {
		coord = As_Building(NavCom)->Docking_Coord();
	} else {
		coord = As_Coord(NavCom);
	}
	int distance = Distance(coord);

	PrimaryFacing.Set_Desired(Direction(coord));

	if (slowdown) {
		int speed = MIN(distance, 0x0300);
		speed = Bound(speed/3, 0x0020, 0x00FF);
		if (Speed != speed) {
			Set_Speed(speed);
		}
	}

	if (distance < 0x0010) {
		if (slowdown) {
			Set_Speed(0);
		}
		distance = 0;
	}
	return(distance);
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * AircraftClass::Debug_Dump -- Displays the status of the aircraft to the mono monitor.       *
 *                                                                                             *
 *    This displays the current status of the aircraft class to the mono monitor. By this      *
 *    display bugs may be tracked down or prevented.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Debug_Dump(MonoClass *mono) const
{
	Validate();
	mono->Set_Cursor(0, 0);
	mono->Print(
		"ÚName:ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂMission:ÄÄÄÂTarCom:ÂNavCom:ÂRadio:ÂCoord:ÄÄÂAltitudeÂSt:Ä¿\n"
		"³                   ³           ³       ³       ³      ³        ³        ³    ³\n"
		"ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂNÂYÂHealth:ÄÂFdir:ÂÄBdir:ÄÂSpeed:ÂÄÄÄÄÄÁÄÄÄÄÄÄÂCargo:ÄÄÄÄÁÄÄÄÄ´\n"
		"³Active........³ ³ ³        ³     ³       ³      ³            ³               ³\n"
		"³Limbo.........³ ³ ÃÄÄÄÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÄÄÁÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n"
		"³Owned.........³ ³ ³Last Message:                                             ³\n"
		"³Discovered....³ ³ ÃTimer:ÂArm:ÂÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÂFlash:ÂStage:ÂTeam:ÄÄÄÄÂArch:´\n"
		"³Selected......³ ³ ³      ³    ³      ³         ³      ³      ³         ³     ³\n"
		"³Teathered.....³ ³ ÃÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÁÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÙ\n"
		"³Locked on Map.³ ³ ³                                                           \n"
		"³              ³ ³ ³                                                           \n"
		"³Is A Loaner...³ ³ ³                                                           \n"
		"³Is Landing....³ ³ ³                                                           \n"
		"³Is Taking Off.³ ³ ³                                                           \n"
		"³              ³ ³ ³                                                           \n"
		"³              ³ ³ ³                                                           \n"
		"³              ³ ³ ³                                                           \n"
		"³Recoiling.....³ ³ ³                                                           \n"
		"³To Display....³ ³ ³                                                           \n"
		"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÁÄÙ                                                           \n");
	mono->Set_Cursor(1, 1);mono->Printf("%s:%s", House->Class->IniName, Class->IniName);
	mono->Set_Cursor(36, 3);mono->Printf("%02X:%02X", SecondaryFacing.Current(), SecondaryFacing.Desired());
	mono->Set_Cursor(42, 1);mono->Printf("%04X", NavCom);
	mono->Set_Cursor(66, 1);mono->Printf("%d", Altitude);
	mono->Set_Cursor(44, 3);mono->Printf("%d", Get_Speed());
	mono->Text_Print("X", 16 + (IsLanding?2:0), 12);
	mono->Text_Print("X", 16 + (IsTakingOff?2:0), 13);
	FootClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * AircraftClass::Active_Click_With -- Handles clicking over specified object.                 *
 *                                                                                             *
 *    This routine is used when the player clicks over the speicifed object. It will assign    *
 *    the appropriate mission to the aircraft.                                                 *
 *                                                                                             *
 * INPUT:   action   -- The action that was nominally determined by the What_Action function.  *
 *                                                                                             *
 *          object   -- The object over which the mouse was clicked.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine will alter the game sequence and causes an event packet to be      *
 *             propogated to all connected machines.                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Active_Click_With(ActionType action, ObjectClass * object)
{
	Validate();
	switch (action) {
		case ACTION_ENTER:
			Player_Assign_Mission(MISSION_ENTER, TARGET_NONE, object->As_Target());
			break;

		case ACTION_SELF:
			Player_Assign_Mission(MISSION_UNLOAD, TARGET_NONE, TARGET_NONE);
			break;

		default:
			break;
	}
	FootClass::Active_Click_With(action, object);
}


/***********************************************************************************************
 * AircraftClass::Active_Click_With -- Handles clicking over specified cell.                   *
 *                                                                                             *
 *    This routine is used when the player clicks the mouse of the specified cell. It will     *
 *    assign the appropriate mission to the aircraft.                                          *
 *                                                                                             *
 * INPUT:   action   -- The action nominally determined by What_Action().                      *
 *                                                                                             *
 *          cell     -- The cell over which the mouse was clicked.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine will affect the game sequence and causes an event object to be     *
 *             propogated to all connected machines.                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Active_Click_With(ActionType action, CELL cell)
{
	Validate();
#ifdef NEVER
	switch (action) {
		case ACTION_MOVE:
			if (Map[cell].IsVisible) {
				Player_Assign_Mission(MISSION_MOVE, TARGET_NONE, ::As_Target(cell));
			}
			break;

		case ACTION_NOMOVE:
			break;

		case ACTION_ATTACK:
			Player_Assign_Mission(MISSION_ATTACK, ::As_Target(cell));
			break;
	}
#endif
	FootClass::Active_Click_With(action, cell);
}


/***********************************************************************************************
 * AircraftClass::Player_Assign_Mission -- Handles player input to assign a mission.           *
 *                                                                                             *
 *    This routine is called as a result of player input with the intent to change the         *
 *    mission of the aircraft.                                                                 *
 *                                                                                             *
 * INPUT:   mission  -- The mission requested of the aircraft.                                 *
 *                                                                                             *
 *          target   -- The value to assign to the aircraft's targeting computer.              *
 *                                                                                             *
 *          dest.    -- The value to assign to the aircraft's navigation computer.             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The mission specified will be executed at an indeterminate future game frame.   *
 *             This is controlled by net/modem propogation delay.                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Player_Assign_Mission(MissionType mission, TARGET target, TARGET destination)
{
	Validate();
	if (AllowVoice) {
		if (mission == MISSION_ATTACK) {
			Response_Attack();
		} else {
			Response_Move();
		}
	}
	Queue_Mission(As_Target(), mission, target, destination);
}


/***********************************************************************************************
 * AircraftClass::What_Action -- Determines what action to perform.                            *
 *                                                                                             *
 *    This routine is used to determine what action will likely be performed if the mouse      *
 *    were clicked over the object specified. The display system calls this routine to         *
 *    control the mouse shape.                                                                 *
 *                                                                                             *
 * INPUT:   target   -- Pointer to the object that the mouse is currently over.                *
 *                                                                                             *
 * OUTPUT:  Returns with the action that will occur if the mouse were clicked over the         *
 *          object specified.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType AircraftClass::What_Action(ObjectClass * target) const
{
	Validate();
	ActionType action = FootClass::What_Action(target);

	if (action == ACTION_SELF && (!How_Many() || (Altitude > 0) || IsTethered)) {
		action = ACTION_NONE;
	}

	if (action == ACTION_ATTACK && Class->Primary == WEAPON_NONE) {
		action = ACTION_NONE;
	}

	//Changed for multiplayer ST - 3/13/2019 5:31PM
	//if (IsOwnedByPlayer && House->Is_Ally(target) && target->What_Am_I() == RTTI_BUILDING && ((AircraftClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (TechnoClass*)target) == RADIO_ROGER) {
	if (Is_Owned_By_Player() && House->Is_Ally(target) && target->What_Am_I() == RTTI_BUILDING && ((AircraftClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (TechnoClass*)target) == RADIO_ROGER) {
		action = ACTION_ENTER;
	}
	return(action);
}


/***********************************************************************************************
 * AircraftClass::What_Action -- Determines what action to perform.                            *
 *                                                                                             *
 *    This routine will determine what action would occur if the mouse were clicked over the   *
 *    cell specified. The display system calls this routine to determine what mouse shape      *
 *    to use.                                                                                  *
 *                                                                                             *
 * INPUT:   cell  -- The cell over which the mouse is currently positioned.                    *
 *                                                                                             *
 * OUTPUT:  Returns with the action that will be performed if the mouse were clicked at the    *
 *          specified cell location.                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType AircraftClass::What_Action(CELL cell) const
{
	Validate();
	ActionType action = FootClass::What_Action(cell);

	//using function for IsVisible so we have different results for different players - JAS 2019/09/30
	if (action == ACTION_MOVE && !Map[cell].Is_Visible(PlayerPtr)) {
		action = ACTION_NOMOVE;
	}

	if (action == ACTION_ATTACK && Class->Primary == WEAPON_NONE) {
		action = ACTION_NONE;
	}
	return(action);
}


/***********************************************************************************************
 * AircraftClass::Pose_Dir -- Fetches the natural landing facing.                              *
 *                                                                                             *
 *    Use this routine to get the desired facing the aircraft should assume when landing.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the normal default facing the aircraft should have when landed.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType AircraftClass::Pose_Dir(void) const
{
	Validate();
	if (*this == AIRCRAFT_TRANSPORT) {
		return(DIR_N);
	}
	return(DIR_NE);
}


/***********************************************************************************************
 * AircraftClass::Mission_Attack -- Handles the attack mission for aircraft.                   *
 *                                                                                             *
 *    This routine is the state machine that handles the attack mission for aircraft. It will  *
 *    handling homing in on and firing on the target in the aircraft's targeting computer.     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game ticks to pass before this routine must be called   *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Mission_Attack(void)
{
	Validate();
	if (Class->IsFixedWing) {
		Assign_Mission(MISSION_HUNT);
		return(1);
	}

	enum {
		VALIDATE_AZ,
		PICK_ATTACK_LOCATION,
		TAKE_OFF,
		FLY_TO_POSITION,
		FIRE_AT_TARGET,
		FIRE_AT_TARGET2,
		RETURN_TO_BASE
	};
	switch (Status) {

		/*
		**	Double check target and validate the attack zone.
		*/
		case VALIDATE_AZ:
			if (!Target_Legal(TarCom)) {
				Status = RETURN_TO_BASE;
			} else {
				Status = PICK_ATTACK_LOCATION;
			}
			break;

		/*
		**	Pick a good location to attack from.
		*/
		case PICK_ATTACK_LOCATION:
			if (!Target_Legal(TarCom)) {
				Status = RETURN_TO_BASE;
			} else {
				Assign_Destination(Good_Fire_Location(TarCom));
				if (Target_Legal(NavCom)) {
					Status = TAKE_OFF;
				} else {
					Status = RETURN_TO_BASE;
				}
			}
			break;

		/*
		**	Take off (if necessary).
		*/
		case TAKE_OFF:
			if (!Target_Legal(TarCom)) {
				Status = RETURN_TO_BASE;
			} else {
				if (Process_Take_Off()) {
					Status = FLY_TO_POSITION;

					/*
					**	Break off radio contact with the helipad it is taking off from.
					*/
					if (In_Radio_Contact() && Map[Coord_Cell(Coord)].Cell_Building() == Contact_With_Whom()) {
						Transmit_Message(RADIO_OVER_OUT);
					}

					/*
					**	Start flying toward the destination by skewing at first.
					**	As the flight progresses, the body will rotate to face
					**	the direction of travel.
					*/
					int diff = SecondaryFacing.Difference(Direction(NavCom));
					diff = Bound(diff, -128, 128);
					PrimaryFacing = SecondaryFacing.Current()+diff;
				}
				return(1);
			}
			break;

		/*
		**	Fly to attack location.
		*/
		case FLY_TO_POSITION:
			if (Target_Legal(TarCom)) {

				/*
				**	If the navcom was cleared mysteriously, then try to pick
				**	a new attack location. This is a likely event if the player
				**	clicks on a new target while in flight to an existing target.
				*/
				if (!Target_Legal(NavCom)) {
					Status = PICK_ATTACK_LOCATION;
					return(1);
				}

				int distance = Process_Fly_To(true);

				if (distance < 0x0200) {
					SecondaryFacing.Set_Desired(Direction(TarCom));

					if (distance < 0x0010) {
						Status = FIRE_AT_TARGET;
						Assign_Destination(TARGET_NONE);
					}
				} else {
					SecondaryFacing.Set_Desired(::Direction(Fire_Coord(0), As_Coord(NavCom)));
					return(1);
				}
			} else {
				Status = RETURN_TO_BASE;
			}
			return(1);

		/*
		**	Fire at the target.
		*/
		case FIRE_AT_TARGET:
			if (!Target_Legal(TarCom)) {
				Status = RETURN_TO_BASE;
				return(1);
			}

			PrimaryFacing.Set_Desired(Direction(TarCom));
			SecondaryFacing.Set_Desired(Direction(TarCom));
			switch (Can_Fire(TarCom, 0)) {
				case FIRE_CLOAKED:
					Do_Uncloak();
					break;

				case FIRE_OK:
					Fire_At(TarCom, 0);
					Map[::As_Cell(TarCom)].Incoming(Coord, true);
					Status = FIRE_AT_TARGET2;
					break;

				default:
					if (!Ammo) {
						Status = RETURN_TO_BASE;
					} else {
						Status = FIRE_AT_TARGET2;
					}
					break;
			}
			return(1);

		/*
		**	Fire at the target.
		*/
		case FIRE_AT_TARGET2:
			if (!Target_Legal(TarCom)) {
				Status = RETURN_TO_BASE;
				return(1);
			}

			PrimaryFacing.Set_Desired(Direction(TarCom));
			SecondaryFacing.Set_Desired(Direction(TarCom));
			switch (Can_Fire(TarCom, 0)) {
				case FIRE_CLOAKED:
					Do_Uncloak();
					break;

				case FIRE_REARM:
					break;

				case FIRE_OK:
					Fire_At(TarCom, 0);
					Map[::As_Cell(TarCom)].Incoming(Coord, true);

					if (Ammo) {
						Status = PICK_ATTACK_LOCATION;
					} else {
						Status = RETURN_TO_BASE;
					}
					break;

				default:
					if (!Ammo) {
						Status = RETURN_TO_BASE;
					} else {
						Status = PICK_ATTACK_LOCATION;
					}
					break;
			}
			break;

		/*
		**	Fly back to landing spot.
		*/
		case RETURN_TO_BASE:
			Assign_Destination(TARGET_NONE);
			Enter_Idle_Mode();
			break;
	}

	return(TICKS_PER_SECOND/2);
}


/***********************************************************************************************
 * AircraftClass::New_LZ -- Find a good landing zone.                                          *
 *                                                                                             *
 *    Use this routine to locate a good landing zone that is nearby the location specified.    *
 *    By using this routine it is possible to assign the same landing zone to several          *
 *    aircraft and they will land nearby without conflict.                                     *
 *                                                                                             *
 * INPUT:   oldlz -- Target value of desired landing zone (usually a cell target value).       *
 *                                                                                             *
 * OUTPUT:  Returns with the new good landing zone. It might be the same value passed in.      *
 *                                                                                             *
 * WARNINGS:   The landing zone might be a goodly distance away from the ideal if there is     *
 *             extensive blocking terrain in the vicinity.                                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET AircraftClass::New_LZ(TARGET oldlz) const
{
	Validate();
	if (Target_Legal(oldlz) && (!Is_LZ_Clear(oldlz) || !Cell_Seems_Ok(As_Cell(oldlz)))) {
		COORDINATE coord = As_Coord(oldlz);

		/*
		**	Scan outward in a series of concentric rings up to certain distance
		**	in cells.
		*/
		for (int radius = 0; radius < 16; radius++) {
			FacingType modifier = Random_Pick(FACING_N, FACING_NW);
			CELL lastcell = -1;

			/*
			**	Perform a radius scan out from the original center location. Try to
			**	find a cell that is allowed to be a legal LZ.
			*/
			for (FacingType facing = FACING_N; facing < FACING_COUNT; facing++) {
				CELL newcell = Coord_Cell(Coord_Move(coord, Facing_Dir(facing+modifier), radius * ICON_LEPTON_W));
				if (Map.In_Radar(newcell)) {
					TARGET newtarget = ::As_Target(newcell);

					if (newcell != lastcell && Is_LZ_Clear(newtarget) && Cell_Seems_Ok(newcell)) {
						return(newtarget);
					}
					lastcell = newcell;
				}
			}
		}
	}
	return(oldlz);
}


/***********************************************************************************************
 * AircraftClass::Fire_Coord -- Calculates the point of origin for a bullet.                   *
 *                                                                                             *
 *    This routine is used to find the exact coordinate where the bullet should appear if      *
 *    fired from this object.                                                                  *
 *                                                                                             *
 * INPUT:   which -- Which weapon to consider.                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate of where the projectile will appear if fired.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE AircraftClass::Fire_Coord(int ) const
{
	Validate();
	return(Coord_Move(Coord_Add(XYP_Coord(0, -Altitude), Coord), SecondaryFacing, 0x040));
}


COORDINATE AircraftClass::Target_Coord(void) const
{
	Validate();
	return(Coord_Add(XYP_Coord(0, -Altitude), Coord));
}


/***********************************************************************************************
 * AircraftClass::Receive_Message -- Handles receipt of radio messages.                        *
 *                                                                                             *
 *    This routine receives all radio messages directed at this aircraft. It is used to handle *
 *    all inter-object coordination. Typically, this would be for transport helicopters and    *
 *    other complex landing operations required of helicopters.                                *
 *                                                                                             *
 * INPUT:   from     -- The source of this radio message.                                      *
 *                                                                                             *
 *          message  -- The message itself.                                                    *
 *                                                                                             *
 *          param    -- An optional parameter that may be used to transfer additional          *
 *                      data.                                                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the radio response from the aircraft.                                 *
 *                                                                                             *
 * WARNINGS:   Some radio messages are handled by the base classes.                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
RadioMessageType AircraftClass::Receive_Message(RadioClass * from, RadioMessageType message, long & param)
{
	Validate();
	switch (message) {

		case RADIO_PREPARED:
			if (Target_Legal(TarCom)) return(RADIO_NEGATIVE);
			if ((Altitude == 0 && Ammo == Class->MaxAmmo) || (Altitude > 0 && Ammo > 0)) return(RADIO_ROGER);
			return(RADIO_NEGATIVE);

		/*
		**	Something disasterous has happened to the object in contact with. Fall back
		**	and regroup. This means that any landing process is immediately aborted.
		*/
		case RADIO_RUN_AWAY:
			if (IsLanding) {
				IsLanding = false;
				IsTakingOff = true;
			}
			Scatter(0, true);
			break;

		/*
		**	The ground control requests that this specified landing spot be used.
		*/
		case RADIO_MOVE_HERE:
			FootClass::Receive_Message(from, message, param);
			if (Is_Target_Building(param)) {
				if (Transmit_Message(RADIO_CAN_LOAD, As_Techno(param)) != RADIO_ROGER) {
					return(RADIO_NEGATIVE);
				}
				Assign_Mission(MISSION_ENTER);
				Assign_Destination((TARGET)param);
			} else {
				Assign_Mission(MISSION_MOVE);
				Assign_Destination((TARGET)param);
			}
			Commence();
			return(RADIO_ROGER);

		/*
		**	Ground control is requesting if the aircraft requires navigation direction.
		*/
		case RADIO_NEED_TO_MOVE:
			FootClass::Receive_Message(from, message, param);
			if (!Target_Legal(NavCom) && !IsTakingOff && !IsLanding) {
				return(RADIO_ROGER);
			}
			return(RADIO_NEGATIVE);

		/*
		**	This message is sent by the passenger when it determines that it has
		**	entered the transport.
		*/
		case RADIO_IM_IN:
			if (How_Many() == Class->Max_Passengers()) {
				Close_Door(5, 4);
			}

			/*
			**	If a civilian has entered the transport, then the transport will immediately
			**	fly off the map.
			*/
			if (from->What_Am_I() == RTTI_INFANTRY && ((InfantryClass *)from)->Class->IsCivilian && !((InfantryClass *)from)->IsTechnician) {
				Assign_Mission(MISSION_RETREAT);
			}
			return(RADIO_ATTACH);

		/*
		**	Docking maintenance message received. Check to see if new orders should be given
		**	to the impatient unit.
		*/
		case RADIO_DOCKING:
			if (Class->IsTransporter && How_Many() < Class->Max_Passengers()) {
				FootClass::Receive_Message(from, message, param);

				if (!IsTethered && !IsLanding && !IsTakingOff && Altitude == 0) {

					Open_Door(5, 4);

					/*
					**	If the potential passenger needs someplace to go, then figure out a good
					**	spot and tell it to go.
					*/
					if (Transmit_Message(RADIO_NEED_TO_MOVE, from) == RADIO_ROGER) {
						CELL cell;
						DirType dir = Desired_Load_Dir(from, cell);

						/*
						**	If no adjacent free cells are detected, then passenger loading
						**	cannot occur. Break radio contact.
						*/
						if (cell == 0) {
							Transmit_Message(RADIO_OVER_OUT, from);
						} else {
							param = (long)::As_Target(cell);

							/*
							**	Tell the potential passenger where it should go. If the passenger is
							**	already at the staging location, then tell it to move onto the transport
							**	directly.
							*/
							if (Transmit_Message(RADIO_MOVE_HERE, param, from) == RADIO_YEA_NOW_WHAT) {
								param = (long)As_Target();
								Transmit_Message(RADIO_TETHER);
								if (Transmit_Message(RADIO_MOVE_HERE, param, from) != RADIO_ROGER) {
									Transmit_Message(RADIO_OVER_OUT, from);
								} else {
									Contact_With_Whom()->Unselect();
								}
							}
						}
					}
				}
				return(RADIO_ROGER);
			}
			break;

		/*
		**	Asks if the passenger can load on this transport.
		*/
		case RADIO_CAN_LOAD:
			if (!In_Radio_Contact() && Class->IsTransporter && How_Many() < Class->Max_Passengers() && from && House->Class->House == from->Owner() && Altitude == 0) {
				return(RADIO_ROGER);
			}
			return(RADIO_NEGATIVE);
	}

	/*
	**	Let the base class take over processing this message.
	*/
	return(FootClass::Receive_Message(from, message, param));
}


/***********************************************************************************************
 * AircraftClass::Desired_Load_Dir -- Determines where passengers should line up.              *
 *                                                                                             *
 *    This routine is used by the transport helicopter to determine the location where the     *
 *    infantry passengers should line up before loading.                                       *
 *                                                                                             *
 * INPUT:   object   -- The object that is trying to load up on this transport.                *
 *                                                                                             *
 *                   -- Reference to the cell that the passengers should move to before the    *
 *                      actual load process may begin.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the direction that the helicopter should face for the load operation. *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *   07/30/1995 JLB : Revamped to scan all adjacent cells.                                     *
 *=============================================================================================*/
DirType AircraftClass::Desired_Load_Dir(ObjectClass * object, CELL & moveto) const
{
	Validate();
	CELL center = Coord_Cell(Center_Coord());
	for (int sweep = FACING_N; sweep < FACING_S; sweep++) {
		moveto = Adjacent_Cell(center, FACING_S+sweep);
		if (Map.In_Radar(moveto) && (Coord_Cell(object->Center_Coord()) == moveto || Map[moveto].Is_Generally_Clear())) return(DIR_N);

		moveto = Adjacent_Cell(center, FACING_S-sweep);
		if (Map.In_Radar(moveto) && (Coord_Cell(object->Center_Coord()) == moveto || Map[moveto].Is_Generally_Clear())) return(DIR_N);
	}
	return(DIR_N);
}


/***********************************************************************************************
 * AircraftClass::Process_Take_Off -- State machine support for taking off.                    *
 *                                                                                             *
 *    This routine is used by the main game state machine processor. This utility routine      *
 *    handles a helicopter as it transitions from landed to flying state.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Has the helicopter reached flight level now?                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Process_Take_Off(void)
{
	Validate();
	IsLanding = false;
	IsTakingOff = true;
	switch (Altitude) {
		case 0:
			Close_Door(5, 4);
			PrimaryFacing = SecondaryFacing;
			break;

		case FLIGHT_LEVEL/2:
			PrimaryFacing.Set_Desired(Direction(NavCom));
			break;

		case FLIGHT_LEVEL-(FLIGHT_LEVEL/3):
			SecondaryFacing.Set_Desired(PrimaryFacing.Desired());
			Set_Speed(0x20);
			break;

		case FLIGHT_LEVEL-(FLIGHT_LEVEL/5):
			Set_Speed(0x40);
			break;

		case FLIGHT_LEVEL:
			Set_Speed(0xFF);
			IsTakingOff = false;
			return(true);
	}
	return(false);
}


/***********************************************************************************************
 * AircraftClass::Process_Landing -- Landing process state machine handler.                    *
 *                                                                                             *
 *    This is a support routine that is called by the main state machine routines. This        *
 *    routine is responsible for handling the helicopter as it transitions from flight to      *
 *    landing.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Has the helicopter completely landed now?                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Process_Landing(void)
{
	Validate();
	IsTakingOff = false;
	IsLanding = true;
	switch (Altitude) {
		case 0:
			IsLanding = false;
			return(true);

		case FLIGHT_LEVEL/2:
			Set_Speed(0);
			break;

		case FLIGHT_LEVEL:
			break;
	}
	return(false);
}


/***********************************************************************************************
 * AircraftClass::Can_Enter_Cell -- Determines if the aircraft can land at this location.      *
 *                                                                                             *
 *    This routine is used when the passability of a cell needs to be determined. This is      *
 *    necessary when scanning for a location that the aircraft can land.                       *
 *                                                                                             *
 * INPUT:   cell  -- The cell location to check for landing.                                   *
 *                                                                                             *
 * OUTPUT:  Returns a value indicating if the cell is a legal landing spot or not.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
MoveType AircraftClass::Can_Enter_Cell(CELL cell, FacingType ) const
{
	Validate();
	if (!Map.In_Radar(cell)) return(MOVE_NO);

	CellClass * cellptr = &Map[cell];

	if (!cellptr->Is_Generally_Clear(true)) return(MOVE_NO);

	if (GameToPlay == GAME_NORMAL && IsOwnedByPlayer && !cellptr->Is_Visible(PlayerPtr)) return(MOVE_NO);

	return(MOVE_OK);
}


/***********************************************************************************************
 * AircraftClass::Good_Fire_Location -- Searches for and finds a good spot to fire from.       *
 *                                                                                             *
 *    Given the specified target, this routine will locate a good spot for the aircraft to     *
 *    fire at the target.                                                                      *
 *                                                                                             *
 * INPUT:   target   -- The target that is desired to be attacked.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the target location of the place that firing should be made from.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *   06/14/1995 JLB : Finer resolution on ring scan.                                           *
 *=============================================================================================*/
TARGET AircraftClass::Good_Fire_Location(TARGET target) const
{
	Validate();
	if (Target_Legal(target)) {
		int range = Weapon_Range(0);
		COORDINATE tcoord = As_Coord(target);
		CELL bestcell = 0;
		CELL best2cell = 0;
		int bestval = -1;
		int best2val = -1;

		for (int r = range-0x0180; r > 0x0180; r -= 0x0100) {
			for (int face = 0; face < 255; face += 16) {
				COORDINATE newcoord = Coord_Move(tcoord, (DirType)face, r);
				CELL newcell = Coord_Cell(newcoord);

				if (Map.In_Radar(newcell) && (GameToPlay != GAME_NORMAL || Map[newcell].Is_Visible(PlayerPtr)) && Cell_Seems_Ok(newcell, true)) {
					int dist = Distance(newcoord);
					if (bestval == -1 || dist < bestval) {
						best2val = bestval;
						best2cell = bestcell;
						bestval = dist;
						bestcell = newcell;
					}
				}
			}
			if (bestval != -1) break;
		}

		if (best2val == -1) {
			best2cell = bestcell;
		}

		/*
		**	If it found a good firing location, then return this location as
		**	a target value.
		*/
		if (bestval != -1) {
			if (Random_Pick(0, 1) == 0) {
				return(::As_Target(bestcell));
			} else {
				return(::As_Target(best2cell));
			}
		}
	}
	return(TARGET_NONE);
}


/***********************************************************************************************
 * AircraftClass::Cell_Seems_Ok -- Checks to see if a cell is good to enter.                   *
 *                                                                                             *
 *    This routine examines the navigation computers of other aircraft in order to see if the  *
 *    specified cell is safe to fly to. The intent of this routine is to avoid unneccessary    *
 *    mid-air collisions.                                                                      *
 *                                                                                             *
 * INPUT:   cell     -- The cell to examine for clear airspace.                                *
 *                                                                                             *
 *          strict   -- Should the scan consider the aircraft, that is making this check, a    *
 *                      blocking aircraft. Typically, the aircraft itself is not considered    *
 *                      a blockage -- an aircraft can always exist where it is currently       *
 *                      located. A strict check is useful for helicopters that need to move    *
 *                      around at the slightest provocation.                                   *
 *                                                                                             *
 * OUTPUT:  Is the specified cell free from airspace conflicts?                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Cell_Seems_Ok(CELL cell, bool strict) const
{
	Validate();
	/*
	**	Make sure that no other aircraft are heading to the selected location. If they
	**	are, then don't consider the location as valid.
	*/
	TARGET astarget = ::As_Target(cell);
	bool ok = true;
	for (int index = 0; index < Aircraft.Count(); index++) {
		AircraftClass * air = Aircraft.Ptr(index);
		if (air && (strict || air != this) && !air->IsInLimbo) {
			if (Coord_Cell(air->Coord) == cell || air->NavCom == astarget) {
				return(false);
			}
		}
	}
	return(true);
}


/***********************************************************************************************
 * AircraftClass::Pip_Count -- Returns the number of "objects" in aircraft.                    *
 *                                                                                             *
 *    This routine is used by the render logic to draw the little container "pips". This       *
 *    corresponds to the number of passengers for a transport helicopter or the number of      *
 *    shots remaining for an attack helicopter.                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of "pips" to render on the aircraft.                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/11/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Pip_Count(void) const
{
	Validate();
	int retval = 0;

	if (Class->IsTransporter) {
		retval = How_Many();
	} else {
		if (Ammo) {
			retval = Cardinal_To_Fixed(Class->MaxAmmo, Ammo);
			retval = Fixed_To_Cardinal(Class->Max_Pips(), retval);
			if (!retval) retval = 1;
		}
	}
	return(retval);
}


/***********************************************************************************************
 * AircraftClass::Mission_Enter -- Control aircraft to fly to the helipad or repair center.    *
 *                                                                                             *
 *    This routine is used when the aircraft needs to fly for either rearming or repairing.    *
 *    It tries to establish contact with the support building. Once contact is established     *
 *    the ground controller takes care of commanding the aircraft.                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before this routine should be called again.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *   07/04/1995 JLB : Ground controller gives orders.                                          *
 *=============================================================================================*/
int AircraftClass::Mission_Enter(void)
{
	Validate();
	enum {
		INITIAL,
		TAKEOFF,
		ALTITUDE,
		TRAVEL,
		LANDING
	};
	switch (Status) {
		case INITIAL:
			if (Altitude < FLIGHT_LEVEL || IsLanding) {
				Status = TAKEOFF;
			} else {
				Status = ALTITUDE;
			}
			break;

		case TAKEOFF:
			if (Process_Take_Off()) {
				/*
				**	After takeoff is complete, break radio contact with any helipad that this
				**	helicopter is taking off from.
				*/
				if (In_Radio_Contact() && Map[Coord_Cell(Coord)].Cell_Building() == Contact_With_Whom()) {
					Transmit_Message(RADIO_OVER_OUT);
				}
				Status = ALTITUDE;
			}
			break;

		case ALTITUDE:
			/*
			**	Establish radio contact with the building this helicopter is trying
			**	to land at.
			*/
			if (In_Radio_Contact()) {
				Status = TRAVEL;
			} else {
				TechnoClass * tech = As_Techno(NavCom);
				if (tech && Transmit_Message(RADIO_CAN_LOAD, tech) == RADIO_ROGER) {
					Transmit_Message(RADIO_HELLO, tech);
					Transmit_Message(RADIO_DOCKING);
					Status = TRAVEL;
				} else {
					Assign_Destination(TARGET_NONE);
					Enter_Idle_Mode();
				}
			}
			break;

		case TRAVEL:
			Transmit_Message(RADIO_DOCKING);
			if (!In_Radio_Contact()) {
				Assign_Destination(TARGET_NONE);
				Enter_Idle_Mode();
			} else {
				int distance = Process_Fly_To(true);

				if (distance < 0x0080) {
					if (Target_Legal(TarCom)) {
						SecondaryFacing.Set_Desired(Direction(TarCom));
					} else {
						SecondaryFacing.Set_Desired(Pose_Dir());
					}

					if (distance < 0x0010) {
						Status = LANDING;
					}
					break;
				} else {
					SecondaryFacing.Set_Desired(::Direction(Fire_Coord(0), As_Coord(NavCom)));
				}
				return(3);
			}
			break;

		case LANDING:
			if (IsTakingOff) {
				Assign_Destination(TARGET_NONE);
				Enter_Idle_Mode();
			}
			if (Process_Landing()) {
				switch (Transmit_Message(RADIO_IM_IN)) {
					case RADIO_ROGER:
						Assign_Mission(MISSION_GUARD);
						break;

					case RADIO_ATTACH:
						Limbo();
						Contact_With_Whom()->Attach(this);
						break;

					default:
						Enter_Idle_Mode();
				}
			}
			break;
	}
	return(1);
}


/***********************************************************************************************
 * AircraftClass::Good_LZ -- Locates a good spot ot land.                                      *
 *                                                                                             *
 *    This routine is used when helicopters need a place to land, but there are no obvious     *
 *    spots (i.e., helipad) available. It will try to land near a friendly helipad or friendly *
 *    building if there are no helipads anywhere. In the event that there are no friendly      *
 *    buildings anywhere on the map, then just land right where it is flying.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the target location where this aircraft should land. This value may   *
 *          not be a clear cell, but the normal landing logic will resolve that problem.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET AircraftClass::Good_LZ(void) const
{
	Validate();
	/*
	**	Scan through all of the buildings and try to land near
	**	the helipad (if there is one) or the nearest friendly building.
	*/
	CELL bestcell;
	int bestdist = -1;
	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass * building = Buildings.Ptr(index);

		if (building && !building->IsInLimbo && building->House == House) {
			int dist = Distance(building);
			if (*building == STRUCT_HELIPAD) {
				dist /= 4;
			}
			if (bestdist == -1 || dist < bestdist) {
				bestdist = dist;
				bestcell = Coord_Cell(building->Center_Coord());
			}
		}
	}

	/*
	**	Return with the suitable location if one was found.
	*/
	if (bestdist != -1) {
		return(::As_Target(bestcell));
	}

	/*
	**	No good location was found. Just try to land here.
	*/
	return(::As_Target(Coord_Cell(Coord)));
}


/***********************************************************************************************
 * AircraftClass::Set_Speed -- Sets the speed for the aircraft.                                *
 *                                                                                             *
 *    This routine will set the speed for the aircraft. The speed is specified as a fraction   *
 *    of full speed.                                                                           *
 *                                                                                             *
 * INPUT:   speed -- The fixed point fractional speed setting. 0x00 is stopped, 0xFF is full   *
 *                   speed.                                                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Set_Speed(int speed)
{
	Validate();
	MPHType sp = MPHType(min((unsigned)(Class->MaxSpeed * House->AirspeedBias), MPH_LIGHT_SPEED));
	Fly_Speed(speed, sp);
}


/***********************************************************************************************
 * AircraftClass::Fire_Direction -- Determines the direction of fire.                          *
 *                                                                                             *
 *    This routine will determine what direction a projectile would take if it were fired      *
 *    from the aircraft. This is the direction that the aircraft's body is facing.             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the direction of projectile fire.                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType AircraftClass::Fire_Direction(void) const
{
	Validate();
	return(SecondaryFacing.Current());
}


/***********************************************************************************************
 * AircraftClass::~AircraftClass -- Destructor for aircraft object.                            *
 *                                                                                             *
 *    This is the destructor for aircraft. It will limbo the aircraft if it isn't already      *
 *    and also removes the aircraft from any team it may be attached to.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
AircraftClass::~AircraftClass(void)
{
	if (GameActive && Class) {
		
#ifdef USE_RA_AI
		//
		// Added for RA AI in TD. ST - 7/26/2019 9:12AM
		//
		House->Tracking_Remove(this);
#endif

		/*
		**	If there are any cargo members, delete them.
		*/
		while (Is_Something_Attached()) {
			delete Detach_Object();
		}

		Limbo();
	}

	if (GameActive && Class && Team) Team->Remove(this);
}


/***********************************************************************************************
 * AircraftClass::Scatter -- Causes the aircraft to move away a bit.                           *
 *                                                                                             *
 *    This routine will cause the aircraft to move away from its current location and then     *
 *    enter some idle mode. Typically this is called when the aircraft is attacked while on    *
 *    the ground.                                                                              *
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
void AircraftClass::Scatter(COORDINATE , bool , bool )
{
	Validate();
	if (IsLanding || Altitude == 0) {
		IsLanding = false;
		IsTakingOff = true;
	}
	Enter_Idle_Mode();
}


/***********************************************************************************************
 * AircraftClass::Rearm_Delay -- Returns the delay between shots for this aircraft.            *
 *                                                                                             *
 *    Aircraft have a faster rearm delay than their weapon would otherwise indicate. This is   *
 *    necessary to give helicopters a combat edge while still allowing them to share the       *
 *    weapon types used by ground units.                                                       *
 *                                                                                             *
 * INPUT:   second   -- Is this for the second shot? The second shot uses the full rearm       *
 *                      delay. The first shot, if part of a two shot weapon, is given an       *
 *                      abbreviated rearm time.                                                *
 *                                                                                             *
 * OUTPUT:  Returns with the game frames to delay before the next shot can fire.               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Rearm_Delay(bool second) const
{
	Validate();
	return(FootClass::Rearm_Delay(second)/2);
}


/***********************************************************************************************
 * AircraftClass::Threat_Range -- Returns with a range to scan for targets.                    *
 *                                                                                             *
 *    This routine returns with the distance to scan for targets according to the type of      *
 *    search requested. The search type is typically the weapon (or short) range and vicinity  *
 *    distances. This is used by Guard and Guard Area missions. Aircraft never consider their  *
 *    weapon range in this determination since they can fly so fast, weapon range is           *
 *    meaningless.                                                                             *
 *                                                                                             *
 * INPUT:   control  -- The range control parameter;                                           *
 *                      -1 = range doesn't matter -- return -1 for compatability reasons.      *
 *                      0  = short range scan                                                  *
 *                      1  = long range scan                                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the lepton distance to use for the scan operation.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Threat_Range(int control) const
{
	Validate();
	if (control == -1) return(-1);

	int range = 20 * ICON_LEPTON_W;
	if (control == 1) {
		range *= 2;
	}
	return(range);
}


/***********************************************************************************************
 * AircraftClass::Mission_Guard -- Handles aircraft in guard mode.                             *
 *                                                                                             *
 *    Aircraft don't like to be in guard mode if in flight. If this situation is detected,     *
 *    then figure out what the aircraft should be doing and go do it.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine again. *
 *                                                                                             *
 * WARNINGS:   This routine typically calls the normal guard logic for ground units.           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Mission_Guard(void)
{
	Validate();
	if (Altitude == FLIGHT_LEVEL) {

		/*
		**	If part of a team, then do nothing, since the team
		**	handler will take care of giving this aircraft a
		**	mission.
		*/
		if (Team) {
			if (Target_Legal(NavCom)) {
				Assign_Mission(MISSION_MOVE);
			}
			return(TICKS_PER_SECOND);
		}

		if (Class->Primary == WEAPON_NONE) {
			Assign_Destination(::As_Target(Coord_Cell(Coord)));
			Assign_Mission(MISSION_MOVE);
		} else {
			Enter_Idle_Mode();
		}
		return(1);
	}
	if (House->IsHuman) return(TICKS_PER_SECOND);

	/*
	**	Special case to force the GDI helicopter to be brain dead in the Nod
	**	mission where it is supposed to be captured.
	*/
	if (GameToPlay == GAME_NORMAL && Scenario == 7 && House->Class->House == HOUSE_GOOD) {
		return(TICKS_PER_SECOND*20);
	}

	/*
	**	If the aircraft is very badly damaged, then it will search for a
	**	repair bay first.
	*/
	if (House->Available_Money() >= 100 && Health_Ratio() <= 0x0080) {
		if (!In_Radio_Contact() ||
			(Altitude == 0 &&
				(Contact_With_Whom()->What_Am_I() != RTTI_BUILDING || *((BuildingClass *)Contact_With_Whom()) != STRUCT_REPAIR))) {


			BuildingClass * building = Find_Docking_Bay(STRUCT_REPAIR, true);
			if (building) {
				Assign_Destination(building->As_Target());
				Assign_Target(TARGET_NONE);
				Assign_Mission(MISSION_ENTER);
				return(1);
			}
		}
	}

	/*
	**	If the aircraft cannot attack anything because of lack of ammo,
	**	abort any normal guard logic in order to look for a helipad
	**	to rearm.
	*/
	if (Ammo == 0 && Class->Primary != WEAPON_NONE) {
		if (!In_Radio_Contact()) {
			BuildingClass * building = Find_Docking_Bay(STRUCT_HELIPAD, false);
			if (building) {
				Assign_Destination(building->As_Target());
				Assign_Target(TARGET_NONE);
				Assign_Mission(MISSION_ENTER);
				return(1);
			}
		}
//		return(TICKS_PER_SECOND*3);
	}

	/*
	**	If the aircraft already has a target, then attack it if possible.
	*/
	if (Target_Legal(TarCom)) {
		Assign_Mission(MISSION_ATTACK);
		return(1);
	}

	/*
	**	Transport helicopters don't really do anything but just sit there.
	*/
	if (Class->Primary == WEAPON_NONE) {
		return(TICKS_PER_SECOND*3);
	}

	/*
	**	Computer controlled helicopters will defend themselves by bouncing around
	**	and looking for a free helipad.
	*/
	if (Altitude == 0 && !In_Radio_Contact()) {
		Scatter(0, true);
		return(TICKS_PER_SECOND*3);
	}

	return(FootClass::Mission_Guard());
}


/***********************************************************************************************
 * AircraftClass::Mission_Guard_Area -- Handles the aircraft guard area logic.                 *
 *                                                                                             *
 *    This routine handles area guard logic for aircraft. Aircraft require special handling    *
 *    for this mode since they are to guard area only if they are in a position to do so.      *
 *    Otherwise they just defend themselves.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine        *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Mission_Guard_Area(void)
{
	Validate();
	if (Altitude == FLIGHT_LEVEL) {
		Enter_Idle_Mode();
		return(1);
	}
	if (House->IsHuman) return(TICKS_PER_SECOND);

	if (Altitude == 0 && !In_Radio_Contact()) {
		Scatter(0, true);
		return(TICKS_PER_SECOND*3);
	}

	if (Target_Legal(TarCom)) {
		Assign_Mission(MISSION_ATTACK);
		return(1);
	}
	return(FootClass::Mission_Guard_Area());
}


/***********************************************************************************************
 * AircraftClass::Response_Attack -- Gives audio response to attack order.                     *
 *                                                                                             *
 *    This routine is used to give an audio response to an attack order.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Response_Attack(void)
{
	Validate();
	static VocType _response[] = {
		VOC_AFFIRM,
		VOC_ACKNOWL,
		VOC_YESSIR,
		VOC_YESSIR,
		VOC_YESSIR
	};
	VocType response = _response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)];
	if (AllowVoice) {
		Sound_Effect(response, 0, -(Aircraft.ID(this)+1));
	}
}


/***********************************************************************************************
 * AircraftClass::Response_Move -- Gives audio response to move request.                       *
 *                                                                                             *
 *    This routine is used to give an audio response to movement orders.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Response_Move(void)
{
	Validate();
	static VocType _response[] = {
		VOC_MOVEOUT,
		VOC_MOVEOUT,
		VOC_MOVEOUT,
		VOC_ACKNOWL,
		VOC_AFFIRM,
		VOC_AFFIRM
	};
	VocType response = _response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)];
	if (AllowVoice) {
		Sound_Effect(response, 0, -(Aircraft.ID(this)+1));
	}
}


/***********************************************************************************************
 * AircraftClass::Response_Select -- Gives audio response when selected.                       *
 *                                                                                             *
 *    This routine is called when an audio response for selection is desired.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Response_Select(void)
{
	Validate();
	static VocType _response[] = {
		VOC_VEHIC,
		VOC_UNIT,
		VOC_YESSIR,
		VOC_YESSIR,
		VOC_YESSIR,
		VOC_AWAIT
	};
	VocType response = _response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)];
	if (AllowVoice) {
		Sound_Effect(response, 0, -(Aircraft.ID(this)+1));
	}
}