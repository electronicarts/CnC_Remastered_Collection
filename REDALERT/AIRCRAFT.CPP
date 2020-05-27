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

/* $Header: /CounterStrike/AIRCRAFT.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
 *                  Last Update : November 2, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   AircraftClass::AI -- Processes the normal non-graphic AI for the aircraft.                *
 *   AircraftClass::Active_Click_With -- Handles clicking over specified cell.                 *
 *   AircraftClass::Active_Click_With -- Handles clicking over specified object.               *
 *   AircraftClass::AircraftClass -- The constructor for aircraft objects.                     *
 *   AircraftClass::Can_Enter_Cell -- Determines if the aircraft can land at this location.    *
 *   AircraftClass::Can_Fire -- Checks to see if the aircraft can fire.                        *
 *   AircraftClass::Cell_Seems_Ok -- Checks to see if a cell is good to enter.                 *
 *   AircraftClass::Desired_Load_Dir -- Determines where passengers should line up.            *
 *   AircraftClass::Draw_It -- Renders an aircraft object at the location specified.           *
 *   AircraftClass::Draw_Rotors -- Draw rotor blades on the aircraft.                          *
 *   AircraftClass::Edge_Of_World_AI -- Detect if aircraft has exited the map.                 *
 *   AircraftClass::Enter_Idle_Mode -- Gives the aircraft an appropriate mission.              *
 *   AircraftClass::Exit_Object -- Unloads passenger from aircraft.                            *
 *   AircraftClass::Fire_At -- Handles firing a projectile from an aircraft.                   *
 *   AircraftClass::Fire_Direction -- Determines the direction of fire.                        *
 *   AircraftClass::Good_Fire_Location -- Searches for and finds a good spot to fire from.     *
 *   AircraftClass::Good_LZ -- Locates a good spot ot land.                                    *
 *   AircraftClass::In_Which_Layer -- Calculates the display layer of the aircraft.            *
 *   AircraftClass::Init -- Initialize the aircraft system to an empty state.                  *
 *   AircraftClass::Is_LZ_Clear -- Determines if landing zone is free for landing.             *
 *   AircraftClass::Landing_Takeoff_AI -- Handle aircraft take off and landing processing.     *
 *   AircraftClass::Look -- Aircraft will look if they are on the ground always.               *
 *   AircraftClass::Mission_Attack -- Handles the attack mission for aircraft.                 *
 *   AircraftClass::Mission_Enter -- Control aircraft to fly to the helipad or repair center.  *
 *   AircraftClass::Mission_Guard -- Handles aircraft in guard mode.                           *
 *   AircraftClass::Mission_Guard_Area -- Handles the aircraft guard area logic.               *
 *   AircraftClass::Mission_Hunt -- Maintains hunt AI for the aircraft.                        *
 *   AircraftClass::Mission_Move -- Handles movement mission.                                  *
 *   AircraftClass::Mission_Retreat -- Handles the aircraft logic for leaving the battlefield. *
 *   AircraftClass::Mission_Unload -- Handles unloading cargo.                                 *
 *   AircraftClass::Movement_AI -- Handles aircraft physical movement logic.                   *
 *   AircraftClass::New_LZ -- Find a good landing zone.                                        *
 *   AircraftClass::Overlap_List -- Returns with list of cells the aircraft overlaps.          *
 *   AircraftClass::Paradrop_Cargo -- Drop a passenger by parachute.                           *
 *   AircraftClass::Per_Cell_Process -- Handle the aircraft per cell process.                  *
 *   AircraftClass::Pip_Count -- Returns the number of "objects" in aircraft.                  *
 *   AircraftClass::Player_Assign_Mission -- Handles player input to assign a mission.         *
 *   AircraftClass::Pose_Dir -- Fetches the natural landing facing.                            *
 *   AircraftClass::Process_Fly_To -- Handles state machine for flying to destination.         *
 *   AircraftClass::Process_Landing -- Landing process state machine handler.                  *
 *   AircraftClass::Process_Take_Off -- State machine support for taking off.                  *
 *   AircraftClass::Read_INI -- Reads aircraft object data from an INI file.                   *
 *   AircraftClass::Receive_Message -- Handles receipt of radio messages.                      *
 *   AircraftClass::Response_Attack -- Gives audio response to attack order.                   *
 *   AircraftClass::Response_Move -- Gives audio response to move request.                     *
 *   AircraftClass::Response_Select -- Gives audio response when selected.                     *
 *   AircraftClass::Rotation_AI -- Handle aircraft body and flight rotation.                   *
 *   AircraftClass::Scatter -- Causes the aircraft to move away a bit.                         *
 *   AircraftClass::Set_Speed -- Sets the speed for the aircraft.                              *
 *   AircraftClass::Shape_Number -- Fetch the shape number to use for the aircraft.            *
 *   AircraftClass::Sort_Y -- Figures the sorting coordinate.                                  *
 *   AircraftClass::Take_Damage -- Applies damage to the aircraft.                             *
 *   AircraftClass::Unlimbo -- Removes an aircraft from the limbo state.                       *
 *   AircraftClass::What_Action -- Determines what action to perform.                          *
 *   AircraftClass::What_Action -- Determines what action to perform.                          *
 *   AircraftClass::operator delete -- Deletes the aircraft object.                            *
 *   AircraftClass::operator new -- Allocates a new aircraft object from the pool              *
 *   AircraftClass::~AircraftClass -- Destructor for aircraft object.                          *
 *   _Counts_As_Civ_Evac -- Is the specified object a candidate for civilian evac logic?       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***********************************************************************************************
 * _Counts_As_Civ_Evac -- Is the specified object a candidate for civilian evac logic?         *
 *                                                                                             *
 *    Examines the specified object to see if it qualifies to be a civilian evacuation. This   *
 *    can only occur if it is a civilian (or Tanya) and the special evacuation flag has been   *
 *    set in the scenario control structure.                                                   *
 *                                                                                             *
 * INPUT:   candidate   -- Candidate object to examine for civilian evacuation legality.       *
 *                                                                                             *
 * OUTPUT:  bool; Is the specified object considered a civilian that must be auto-evacuated?   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static bool _Counts_As_Civ_Evac(ObjectClass const * candidate)
{
	/*
	**	If the candidate pointer is missing, then return with failure code.
	*/
	if (candidate == NULL) return(false);

	/*
	**	Only infantry objects can be considered for civilian evacuation action.
	*/
	if (candidate->What_Am_I() != RTTI_INFANTRY) return(false);

	/*
	**	Working infantry object pointer.
	*/
	InfantryClass const * inf = (InfantryClass const *)candidate;

	/*
	**	Certain infantry types will always be considered a civilian evacuation candidate. These
	**	include the special one-time infantry that appear in some missions.
	*/
	if (*inf == INFANTRY_EINSTEIN || *inf == INFANTRY_GENERAL || *inf == INFANTRY_DELPHI || *inf == INFANTRY_CHAN) return(true);

	/*
	**	Consider Tanya to be part of the civilian evacuation logic if the scenario is
	**	specially flagged for this.
	*/
	if (Scen.IsTanyaEvac && *inf == INFANTRY_TANYA) return(true);

	/*
	**	If the infantry is not a civilian, then it isn't allowed to be a civilian evacuation.
	*/
	if (!inf->Class->IsCivilian) return(false);

	/*
	**	Technicians look like civilians, but are not considered a legal evacuation candidate.
	*/
	if (inf->IsTechnician) return(false);

	/*
	**	All tests pass, so return the success of the infantry as a civilian evacuation candidate.
	*/
	return(true);
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
void AircraftClass::operator delete(void * ptr)
{
	if (ptr) {
		((AircraftClass *)ptr)->IsActive = false;
	}
	Aircraft.Free((AircraftClass *)ptr);
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
	FootClass(RTTI_AIRCRAFT, Aircraft.ID(this), house),
	Class(AircraftTypes.Ptr((int)classid)),
	SecondaryFacing(PrimaryFacing),
	Passenger(false),
	IsLanding(false),
	IsTakingOff(false),
	IsHovering(false),
	Jitter(0),
	SightTimer(0),
	AttacksRemaining(1)
{
	/*
	**	For two shooters, clear out the second shot flag -- it will be set the first time
	**	the object fires. For non two shooters, set the flag since it will never be cleared
	**	and the second shot flag tells the system that normal rearm times apply -- this is
	**	what is desired for non two shooters.
	*/
	IsSecondShot = !Class->Is_Two_Shooter();
	House->Tracking_Add(this);
	Ammo = Class->MaxAmmo;
	Height = FLIGHT_LEVEL;
	Strength = Class->MaxStrength;
	NavCom = TARGET_NONE;

	/*
	** Keep count of the number of units created. Dont track cargo planes as they are created
	** automatically, not bought.
	*/
//	if (/*classid != AIRCRAFT_CARGO && */ Session.Type == GAME_INTERNET) {
//		House->AircraftTotals->Increment_Unit_Total((int)classid);
//	}

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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (FootClass::Unlimbo(coord, dir)) {

		if (*this == AIRCRAFT_BADGER || (Class->PrimaryWeapon != NULL && Class->PrimaryWeapon->IsCamera)) {
			IsALoaner = true;
		}

		/*
		**	Ensure that the owning house knows about the
		**	new object.
		*/
		House->AScan |= (1L << Class->Type);
		House->ActiveAScan |= (1L << Class->Type);

		/*
		**	Hack it so that aircraft that are both passenger and cargo carrying
		**	will carry passengers at the expense of ammo.
		*/
		if (Is_Something_Attached()) {
			Ammo = 0;
			Passenger = true;
		}

		/*
		**	Forces the body of the helicopter to face the correct direction.
		*/
		SecondaryFacing = dir;

		/*
		**	Start rotor animation.
		*/
		if (!Class->IsFixedWing) {
			Set_Rate(1);
			Set_Stage(0);
		}

		/*
		**	When starting at flight level, then give it speed. When landed
		**	then it must be stationary.
		*/
		if (Height == FLIGHT_LEVEL) {
			Set_Speed(0xFF);
		} else {
			Set_Speed(0);
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * AircraftClass::Shape_Number -- Fetch the shape number to use for the aircraft.              *
 *                                                                                             *
 *    This will determine what shape number to use for the aircraft in its current state.      *
 *    The shape number can be used for drawing or determine shape rectangle size.              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the shape number to use for the aircraft body.                        *
 *                                                                                             *
 * WARNINGS:   Some aircraft, particularly helicopters, require other shapes attached to it.   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Shape_Number(void) const
{
	int shapenum = 0;

	switch (Class->Rotation) {
		case 32:
			shapenum = UnitClass::BodyShape[Dir_To_32(SecondaryFacing)];
			break;

		case 16:
			shapenum = UnitClass::BodyShape[Dir_To_16(SecondaryFacing)*2]/2;
			break;

		case 8:
			shapenum = UnitClass::BodyShape[Dir_To_8(SecondaryFacing)*4]/4;
			break;

		default:
			break;
	}

	/*
	**	If there is a door on this aircraft (Chinook), then adjust the
	**	shape number to match the door open state.
	*/
	if (!Is_Door_Closed()) {
		shapenum = Class->Rotation + Door_Stage();
	}

	return(shapenum);
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
void AircraftClass::Draw_It(int x, int y, WindowNumberType window) const
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	/*
	**	Verify the legality of the unit class.
	*/
	void const * shapefile = Get_Image_Data();
	if (!shapefile) return;

	int shapenum = Shape_Number();

	/*
	**	Certain aircraft use algorithmic rotation for some stages. Set the
	**	rotation value accordingly. A rotation of DIR_N means no rotation at all.
	*/
	DirType rotation = DIR_N;
	if (Class->Rotation == 16) {
		rotation = DirType(Rotation16[SecondaryFacing]);
	}

#ifdef TOFIX
	/*
	**	The orca attack helicopter uses a special shape set when it is travelling
	**	forward above a certain speed.
	*/
	if (*this == AIRCRAFT_HIND && Get_Speed() >= MPH_MEDIUM_FAST) {
		shapenum += Class->Rotation;
	}
#endif

	/*
	**	Helicopters that are flying have a "bobbing" effect.
	*/
	int jitter = 0;
	if (Height == FLIGHT_LEVEL && Get_Speed() < 3) {
		static int _jitter[] = {0,0,0,0,1,1,1,0,0,0,0,0,-1,-1,-1,0};
		jitter = _jitter[::Frame % 16];
	}

	// Virtual window needs to draw the body first so it's considered the primary object and the shadow is a sub-object
	if (window == WINDOW_VIRTUAL) {
		/*
		**	Draw the root body of the unit.
		*/
		Techno_Draw_Object(shapefile, shapenum, x, y+jitter, window, rotation);

		/*
		**	Special manual shadow draw code.
		*/
		if (Visual_Character() <= VISUAL_DARKEN) {
			CC_Draw_Shape(this, shapefile, shapenum, x+1, y+2, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING, DisplayClass::FadingShade, NULL);
		}
	} else {
		/*
		**	Special manual shadow draw code.
		*/
		if (Visual_Character() <= VISUAL_DARKEN) {
			CC_Draw_Shape(this, shapefile, shapenum, x+1, y+2, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING, DisplayClass::FadingShade, NULL);
		}

		/*
		**	Draw the root body of the unit.
		*/
		Techno_Draw_Object(shapefile, shapenum, x, y+jitter, window, rotation);
	}

	/*
	**	If this aircraft is equipped with rotor blades, then draw them at this time.
	*/
	if (Class->IsRotorEquipped) {
		Draw_Rotors(x, y+jitter, window);
	}

	/*
	**	This draws any overlay graphics on the aircraft.
	*/
	FootClass::Draw_It(x, y-Lepton_To_Pixel(Height), window);
}


/***********************************************************************************************
 * AircraftClass::Draw_Rotors -- Draw rotor blades on the aircraft.                            *
 *                                                                                             *
 *    This routine will draw rotor blades on the aircraft. It is presumed that the aircraft    *
 *    has already been drawn at the X and Y pixel coordinates specified.                       *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y pixel coordinates to draw the rotor blades.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Draw_Rotors(int x, int y, WindowNumberType window) const
{
	ShapeFlags_Type flags = SHAPE_CENTER|SHAPE_WIN_REL;
	int shapenum;

	/*
	**	The rotor shape number depends on whether the helicopter is idling
	**	or not. A landed helicopter uses slow moving "idling" blades.
	*/
	if (Height == 0) {
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
		short yy = y-Lepton_To_Pixel(Height);
		FacingType face = Dir_Facing(SecondaryFacing);
		Move_Point(xx, yy, SecondaryFacing.Current(), _stretch[face]);
		// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
		CC_Draw_Shape(this, "RROTOR", AircraftTypeClass::RRotorData, shapenum, xx, yy-2, window, flags, NULL, DisplayClass::UnitShadow);

		Move_Point(xx, yy, SecondaryFacing.Current()+DIR_S, _stretch[face]*2);
		// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
		CC_Draw_Shape(this, "LROTOR", AircraftTypeClass::LRotorData, shapenum, xx, yy-2, window, flags, NULL, DisplayClass::UnitShadow);

	} else {

		/*
		**	Single rotor centered about shape.
		*/
		// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
		CC_Draw_Shape(this, "RROTOR", AircraftTypeClass::RRotorData, shapenum, x, ((y-Lepton_To_Pixel(Height))-2), window, flags, NULL, DisplayClass::UnitShadow);
	}
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
void AircraftClass::Read_INI(CCINIClass & ini)
{
	AircraftClass	* air;			// Working unit pointer.
	HousesType		inhouse;			// Unit house.
	AircraftType	classid;			// Unit class.
	char				buf[128];

	int counter = ini.Entry_Count(INI_Name());
	for (int index = 0; index < counter; index++) {
		char const * entry = ini.Get_Entry(INI_Name(), index);

		ini.Get_String(INI_Name(), entry, NULL, buf, sizeof(buf)-1);
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
						char * token = strtok(NULL, ",");
						if (token) {
							strength = atoi(token);
						} else {
							strength = 0;
						}

						token = strtok(NULL, ",");
						if (token) {
							coord = Cell_Coord((CELL)atoi(token));
						} else {
							coord = 0xFFFFFFFFL;
						}

						token = strtok(NULL, ",");
						if (token) {
							dir = (DirType)atoi(token);
						} else {
							dir = DIR_N;
						}

						if (!Map.In_Radar(Coord_Cell(coord))) {
							delete air;
						} else {

							air->Strength = (int)air->Class->MaxStrength * fixed(strength, 256);		// Cast this to (int). ST - 5/8/19
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (Class->IsFixedWing) {

		if (TarCom != NavCom) {
			Assign_Destination(TarCom);
		}

		enum {
			LOOK_FOR_TARGET,
			TAKE_OFF,
			FLY_TO_TARGET,
			DROP_BOMBS,
			REGROUP
		};
		switch (Status) {

			/*
			**	Acquiring target stage.
			*/
			case LOOK_FOR_TARGET:
				if (Target_Legal(TarCom)) {
					Status = TAKE_OFF;
					return(1);
				} else {
					if (!Team.Is_Valid()) {
						if (Session.Type != GAME_NORMAL) {
							Assign_Target(Greatest_Threat(THREAT_TIBERIUM));
						}
						if (!Target_Legal(TarCom)) {
							Assign_Target(Greatest_Threat(THREAT_NORMAL));
						}

						/*
						**	If there is no target, then this aircraft should just do its normal thing.
						*/
						if (!Target_Legal(TarCom) && !Team.Is_Valid()) {
							Enter_Idle_Mode();
						}
					}
				}
				break;

			/*
			**	Make the aircraft take off from the airstrip.
			*/
			case TAKE_OFF:
				/*
				**	If the aircraft is high enough to begin its mission, then do so.
				*/
				if (Process_Take_Off())  {
					IsTakingOff = false;
					Set_Speed(0xFF);

					/*
					**	After takeoff is complete, break radio contact.
					*/
					if (In_Radio_Contact()/*KO && Map[Coord].Cell_Building() == Contact_With_Whom()*/) {
						Transmit_Message(RADIO_OVER_OUT);
					}

					Status = FLY_TO_TARGET;
				}
				return(1);

			/*
			**	Homing in on target stage.
			*/
			case FLY_TO_TARGET:
				switch (Can_Fire(TarCom, 0)) {
					case FIRE_FACING:
						/*
						**	Catch the case where it is tightly circling the target. In that
						**	case, increase the delay so that it has a chance to fly away and
						**	break the circle cycle.
						*/
						if (In_Range(TarCom, 0) || Passenger) {
							return(TICKS_PER_SECOND * 2);
						}
						if (!PrimaryFacing.Is_Rotating() && Target_Legal(TarCom)) {
							PrimaryFacing.Set_Desired(Direction(TarCom));
						}
						break;

					case FIRE_AMMO:
						Status = REGROUP;
						break;

					case FIRE_CANT:
					case FIRE_ILLEGAL:
						if (Mission == MISSION_ATTACK) {
							Status = REGROUP;
						} else {
							Status = LOOK_FOR_TARGET;
						}
						break;

					case FIRE_OK:
						Status = DROP_BOMBS;
						return(1);

					default:
						if (!PrimaryFacing.Is_Rotating() && Target_Legal(TarCom)) {
							PrimaryFacing.Set_Desired(Direction(TarCom));
						}
						break;
				}
				return(TICKS_PER_SECOND/2);

			/*
			**	Dropping a stream of bombs phase.
			*/
			case DROP_BOMBS:
				TARGET targ;
				switch (Can_Fire(TarCom, 0)) {
					case FIRE_OK:
						targ = ::As_Target(Coord_Move(Center_Coord(), SecondaryFacing, Weapon_Range(0)-0x0200));
						if (Class->PrimaryWeapon != NULL) {
							if (Class->PrimaryWeapon->IsCamera) {
								Status = REGROUP;
							} else {
								Map[::As_Cell(TarCom)].Incoming(Coord, true);
							}

							/*
							**	Force the target to be the actual target if this aircraft is
							**	equipped with homing projectile.
							*/
							if (Class->PrimaryWeapon->Bullet != NULL && Class->PrimaryWeapon->Bullet->ROT > 0) {
								targ = TarCom;
							}
						}
						Fire_At(targ, 0);
						if (Class->Is_Two_Shooter()) {
							Fire_At(targ, 0);
						}
						return(Arm);

					case FIRE_RANGE:
					case FIRE_FACING:
						Status = FLY_TO_TARGET;
						return(TICKS_PER_SECOND*4);

					case FIRE_ILLEGAL:
						if (Mission == MISSION_ATTACK) {
							Status = REGROUP;
						} else {
							Status = LOOK_FOR_TARGET;
						}
						break;

					case FIRE_CANT:
						Status = REGROUP;
						break;

					case FIRE_AMMO:
						AttacksRemaining--;
						Status = REGROUP;
						break;

					default:
						break;
				}
				return(1);

			/*
			**	Pull away to regroup for possibly another attack or a retreat.
			*/
			case REGROUP:
				if (Ammo == 0) {
					AttacksRemaining = 0;
					if (Team.Is_Valid()) Team->Remove(this);
					Enter_Idle_Mode();
				}

				if (Mission == MISSION_ATTACK || (Class->PrimaryWeapon != NULL && Class->PrimaryWeapon->IsCamera) || (!AttacksRemaining && !Is_Something_Attached())) {

					if (IsALoaner)  {
						if (Team) Team->Remove(this);
						Assign_Mission(MISSION_RETREAT);
						Commence();
					} else {
						if (!Team.Is_Valid()) Enter_Idle_Mode();
					}
					Commence();
				} else {
					Status = LOOK_FOR_TARGET;
				}
				break;

			default:
				break;
		}
	} else {
		if (!Ammo) {
			if (Team) Team->Remove(this);
			Enter_Idle_Mode();
		} else {
			if (!Target_Legal(TarCom)) {
				if (Session.Type != GAME_NORMAL) {
					Assign_Target(Greatest_Threat(THREAT_TIBERIUM));
				}
				if (!Target_Legal(TarCom)) {
					Assign_Target(Greatest_Threat(THREAT_NORMAL));
				}
				if (!Target_Legal(TarCom)) {
					Enter_Idle_Mode();
					return(1);
				}
			}

			Assign_Mission(MISSION_ATTACK);
			return(1);
		}
	}
	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	/*
	**	A Mission change can always occur if the aircraft is landed or flying.
	*/
	if (!IsLanding && !IsTakingOff) {
		Commence();
	}

	/*
	**	Perform any base class AI processing. If during this process, the aircraft was
	**	destroyed, then detect this and bail from this AI routine early.
	*/
	FootClass::AI();
	if (!IsActive) {
		return;
	}

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
	Rotation_AI();

	/*
	**	Handle any aircraft movement at this time.
	*/
	Movement_AI();

	/*
	**	Any aircraft that is not in the ground layer must be redrawn. This is a
	**	performance hit, but there is no other choice. The cells under an aircraft
	**	do not know if there is an aircraft above it. Thus, it cannot flag the
	**	aircraft to redraw. As a consequence, all aircraft must redraw.
	*/
	if (In_Which_Layer() != LAYER_GROUND) {
		Mark();
	}

	/*
	**	Perform sighting every so often as controlled by the sight timer.
	*/
	//if (IsOwnedByPlayer && Class->SightRange && SightTimer == 0) {  // Changed to facilitate client/server multiplayer. ST - 8/2/2019 2:29PM	
	if (House->IsHuman && Class->SightRange && SightTimer == 0) {
		Look();
		SightTimer = TICKS_PER_SECOND;
	}

	/*
	**	Handle landing and taking off logic. Helicopters are prime users of this technique. The
	**	aircraft will either gain or lose altitude as appropriate. As the aircraft transitions
	**	between flying level and ground level, it will be moved into the appropriate render
	**	layer.
	*/
	if (Landing_Takeoff_AI()) {
		return;
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
	if (Edge_Of_World_AI()) {
		return;
	}
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
short const * AircraftClass::Overlap_List(bool redraw) const
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	static short const _list[] = {
		-(MAP_CELL_W-1), -MAP_CELL_W, -(MAP_CELL_W+1),
		-1, 0, 1,
		(MAP_CELL_W-1), MAP_CELL_W, (MAP_CELL_W+1),
		-((MAP_CELL_W*2)-1), -(MAP_CELL_W*2), -((MAP_CELL_W*2)+1),
		-((MAP_CELL_W*3)-1), -(MAP_CELL_W*3), -((MAP_CELL_W*3)+1),
		REFRESH_EOL
	};

	static short const _listbadger[] = {
		-(MAP_CELL_W-2), -(MAP_CELL_W-1), -MAP_CELL_W, -(MAP_CELL_W+1), -(MAP_CELL_W+2),
		-2, -1, 0, 1, 2,
		(MAP_CELL_W-2), (MAP_CELL_W-1), MAP_CELL_W, (MAP_CELL_W+1), (MAP_CELL_W+2),
		-((MAP_CELL_W*2)-2), -((MAP_CELL_W*2)-1), -(MAP_CELL_W*2), -((MAP_CELL_W*2)+1), -((MAP_CELL_W*2)+2),
		-((MAP_CELL_W*3)-2), -((MAP_CELL_W*3)-1), -(MAP_CELL_W*3), -((MAP_CELL_W*3)+1), -((MAP_CELL_W*3)+2),
		REFRESH_EOL
	};

	if (redraw || Height != 0) {
#ifdef PARTIAL
		Rect rect;
		if (!IsSelected && Class->DimensionData != NULL && Class->IsFixedWing) {
			int shapenum = min(Shape_Number(), Get_Build_Frame_Count(Class->Get_Image_Data())-1);
			if (!Class->DimensionData[shapenum].Is_Valid()) {
				Class->DimensionData[shapenum] = Shape_Dimensions(Class->Get_Image_Data(), shapenum);
			}
			rect = Class->DimensionData[shapenum];

			/*
			**	Increase the rectangle for the aircraft since the aircraft could
			**	have its shape algorithmically rotated.
			*/
			rect.X -= 5;
			rect.Y -= 5;
			rect.Width += 10;
			rect.Height += 10;

			Rect hrect = rect;

			hrect.Y -= Lepton_To_Pixel(Height);

			return(Coord_Spillage_List(Coord, Union(rect, hrect), true));
		}
#endif

		if (*this == AIRCRAFT_BADGER) {
			return(_listbadger);
		} else {
			return(_list);
		}
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
	Aircraft.Free_All();
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (Class->IsFixedWing) {

		Assign_Target(NavCom);
		return(Mission_Hunt());

	} else {
		enum {
			SEARCH_FOR_LZ,
			FLY_TO_LZ,
			LAND_ON_LZ,
			UNLOAD_PASSENGERS,
			TAKE_OFF
		};

		switch (Status) {

			/*
			**	Search for an appropriate destination spot if one isn't already assigned.
			*/
			case SEARCH_FOR_LZ:
				if (Height == 0 && (Target_Legal(NavCom) || Coord == As_Coord(NavCom))) {
					Status = UNLOAD_PASSENGERS;
				} else {
					if (!Is_LZ_Clear(NavCom)) {

						FootClass * foot = Attached_Object();
						if (foot != NULL && foot->Team && foot->Team->Class->Origin != -1) {
							Assign_Destination(::As_Target(Scen.Waypoint[foot->Team->Class->Origin]));
						} else {
							Assign_Destination(New_LZ(::As_Target(Scen.Waypoint[WAYPT_REINF])));
							if (Team.Is_Valid()) {
								Team->Assign_Mission_Target(NavCom);
							}
						}
					} else {
						if (Height == FLIGHT_LEVEL) {
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
					int distance = Process_Fly_To(true, NavCom);

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
						if (In_Radio_Contact() && Map[Coord].Cell_Building() == Contact_With_Whom()) {
							Transmit_Message(RADIO_OVER_OUT);
						}
					} else  {
						Enter_Idle_Mode();
					}
				}
				return(1);
			}

			default:
				break;
		}
	}
	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (!Target_Legal(target)) return(false);
	CELL cell = ::As_Cell(target);
	if (!Map.In_Radar(cell)) return(false);

	/*
	**	If the requested landing location is occupied, then only consider that location
	**	legal if the occupying object is in radio contact with the aircraft. This presumes that
	**	the two objects know what they are doing.
	*/
	ObjectClass * object = Map[cell].Cell_Object();
	if (object) {
		if (object == this) return(true);

		if (In_Radio_Contact() && Contact_With_Whom() == object) {
			return(true);
		}
		return(false);
	}

	if (!Map[cell].Is_Clear_To_Move(SPEED_TRACK, false, false)) return(false);

	return(true);
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (Class->IsFixedWing) {
		if (Class->IsFixedWing && Height < FLIGHT_LEVEL) {
			Height += 1;
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
			PrimaryFacing.Set_Desired((DirType)((House->Control.Edge & 0x03) << 6));
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
	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

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
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * AircraftClass::Paradrop_Cargo -- Drop a passenger by parachute.                             *
 *                                                                                             *
 *    Call this routine when a passenger needs to be dropped off by parachute. One passenger   *
 *    is offloaded by a call to this routine.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay time that it is safe to wait before processing any further  *
 *          paradrop actions.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Paradrop_Cargo(void)
{
	FootClass * passenger = Detach_Object();
	if (passenger) {
		if (!passenger->Paradrop(Center_Coord())) {
			Attach(passenger);
		} else {

			/*
			**	Play a sound effect of the parachute opening.
			*/
			Sound_Effect(VOC_CHUTE1, Coord);

			if (Team.Is_Valid()) {
				Team->Remove(passenger);
				if (passenger->House->IsHuman) {
					Assign_Mission(MISSION_GUARD);
				} else {
					Assign_Mission(MISSION_HUNT);
				}
			}
//			Arm = Rearm_Delay(IsSecondShot);
			Arm = 0;
		}
	}
	return(Arm);
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	/*
	**	Passenger aircraft will actually paradrop their cargo instead of
	**	firing their weapon.
	*/
	if (Is_Something_Attached()) {
		Paradrop_Cargo();
		return(0);
	}

	/*
	**	If the weapon is actually a camera, then perform the "snapshot" of the
	**	ground instead of normal weapon fire.
	*/
	if (Class->PrimaryWeapon != NULL && Class->PrimaryWeapon->IsCamera) {
		
		if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {

			if (House->Is_Ally(PlayerPtr)) {
				Map.Sight_From(Coord_Cell(Center_Coord()), 9, House, false);
			}
			
		} else {
		
			for (int i = 0; i < Session.Players.Count(); i++) {
				HousesType house_type = Session.Players[i]->Player.ID;
				HouseClass *house = HouseClass::As_Pointer(house_type);

				if (house == House || House->Is_Ally(house)) {
					Map.Sight_From(Coord_Cell(Center_Coord()), 9, house, false);
				}
			}
		}
		
		Ammo = 0;
		Arm = Rearm_Delay(IsSecondShot);
		return(0);
	}


	BulletClass * bullet = FootClass::Fire_At(target, which);

	if (bullet) {

		/*
		**	Aircraft reveal when firing
		*/
		HouseClass *player = HouseClass::As_Pointer(Owner());
		if (player != nullptr && player->IsHuman) {
			Map.Sight_From(Coord_Cell(Center_Coord()), 1, player, false);
		}

		/*
		**	Falling bullets move at a speed proportionate to the delivery craft.
		*/
		if (bullet->Class->IsDropping) {
			bullet->Fly_Speed(40, MPH_MEDIUM_SLOW);		// TCTC To fix.
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
ResultType AircraftClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source, bool forced)
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	ResultType res = RESULT_NONE;

	/*
	**	Flying aircraft take half damage.
	*/
	if (Height) {
		damage /= 2;
	}

	/*
	**	Apply the damage to the aircraft.
	*/
	res = FootClass::Take_Damage(damage, distance, warhead, source, forced);

	/*
	**	Special action is performed if the aircraft is killed -- the cargo is destroyed
	**	as well.
	*/
	switch (res) {
		case RESULT_DESTROYED:
			Kill_Cargo(source);
			Death_Announcement();
			new AnimClass(ANIM_FBALL1, Target_Coord());

			/*
			**	Parachute a survivor if possible.
			*/
			if (Class->IsCrew && Percent_Chance(90) && Map[Center_Coord()].Is_Clear_To_Move(SPEED_FOOT, true, false)) {
				InfantryClass * infantry = new InfantryClass(INFANTRY_E1, House->Class->House);
				if (infantry != NULL) {
					if (!infantry->Paradrop(Center_Coord())) {
						delete infantry;
					}
				}
			}

			delete this;
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (Class->IsFixedWing) {

		enum  {
			TAKE_OFF,
			FLY_TOWARD_TARGET
		};

		switch (Status) {
			int distance;

			case TAKE_OFF:

				/*
				**	If the aircraft is high enough to begin its mission, then do so.
				*/
				if (Process_Take_Off())  {
					IsTakingOff = false;
					Set_Speed(0xFF);

					/*
					**	After takeoff is complete, break radio contact.
					*/
					if (In_Radio_Contact() && Map[Coord].Cell_Building() == Contact_With_Whom()) {
						Transmit_Message(RADIO_OVER_OUT);
					}

					Status = FLY_TOWARD_TARGET;
				}
				return(1);

			case FLY_TOWARD_TARGET:
				PrimaryFacing.Set_Desired(Direction(NavCom));
				distance = Distance(NavCom);

				if (distance < 0x00C0) {
					MissionType mission = MISSION_GUARD;

					if (!IsALoaner) {
						/*
						**	Normal aircraft try to find a good landing spot to rest.
						*/
						BuildingClass * building = Find_Docking_Bay(Class->Building, false);
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	if (!Class->IsFixedWing) {
		int dist = 0x7FFFFFFF;
		if (building) dist=Distance(building);
		for (int index = 0; index < Vessels.Count(); index++) {
			VesselClass *ship = Vessels.Ptr(index);
			if (ship != NULL && *ship == VESSEL_CARRIER && !ship->IsInLimbo && ship->IsActive && ship->House == House && ship->How_Many() < ship->Class->Max_Passengers() ) {
				if (Distance(ship) < dist || !building) {
					building = (BuildingClass *)ship;
					dist = Distance(ship);
				}
//				break;
			}
		}
	}
#endif
						Assign_Destination(TARGET_NONE);
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
						if (building && (Transmit_Message(RADIO_HELLO, building) == RADIO_ROGER || building->What_Am_I() == RTTI_VESSEL) ) {
#else
						if (building && Transmit_Message(RADIO_HELLO, building) == RADIO_ROGER) {
#endif
							mission = MISSION_ENTER;
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	if (building->What_Am_I() == RTTI_VESSEL) {
		Assign_Destination(building->As_Target());
	}
#endif
						} else {
							Assign_Destination(Good_LZ());

							/*
							** If this aircraft has nowhere else to go, meaning that
							** there is no airfield available, then it has to crash.
							*/
							if (Is_Target_Cell(NavCom)) {

								if (Process_Landing()) {
									Strength = 1;
									int damage = Strength;
									Take_Damage(damage, 0, WARHEAD_AP, 0, true);
									return(1);
								}
								return(500);
							}
							mission = MISSION_MOVE;
						}
						Assign_Mission(mission);
						Commence();
					} else {
						if (!Team.Is_Valid()) {
							Enter_Idle_Mode();
						}
					}
					return(1);
				}
				break;

			default:
				break;
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
					if (Team.Is_Valid()) {
						Team->Assign_Mission_Target(NavCom);
					}
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
					if (In_Radio_Contact() && Map[Coord].Cell_Building() == Contact_With_Whom()) {
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
				int distance = Process_Fly_To(true, NavCom);

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

//				SecondaryFacing.Set_Desired(::Direction(Fire_Coord(0), As_Coord(NavCom)));
				SecondaryFacing.Set_Desired(Direction(NavCom));

			} else {
				Assign_Destination(New_LZ(NavCom));
				if (Team.Is_Valid()) {
					Team->Assign_Mission_Target(NavCom);
				}
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
				if (Team.Is_Valid()) {
					Team->Assign_Mission_Target(NavCom);
				}
				Status = TAKE_OFF;
			}
			if (Process_Landing()) {
				if (MissionQueue == MISSION_NONE) {
					Enter_Idle_Mode();
				}
			}
			return(1);

		default:
			break;
	}

	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	MissionType mission = (Class->IsFixedWing && IsALoaner && Class->PrimaryWeapon != NULL) ? MISSION_HUNT : MISSION_GUARD;
	if (Class->IsFixedWing) {
		if (In_Which_Layer() == LAYER_GROUND) {
			if (IsALoaner) {
				mission = MISSION_RETREAT;
			} else {
				Assign_Destination(TARGET_NONE);
				Assign_Target(TARGET_NONE);
				mission = MISSION_GUARD;
			}
		} else {

			/*
			**	If this transport is a loaner and part of a team, then remove it from
			**	the team it is attached to.
			*/
			if ((IsALoaner && House->IsHuman) || (!House->IsHuman && !Ammo)) {
				if (Team.Is_Valid() && Team->Has_Entered_Map()) {
					Team->Remove(this);
				}
			}
			if (Team.Is_Valid()) return;

			/*
			**	Weapon equipped helicopters that run out of ammo and were
			**	brought in as reinforcements will leave the map.
			*/
			if (Mission != MISSION_ATTACK && IsALoaner && Ammo == 0 && Class->PrimaryWeapon != NULL) {
				mission = MISSION_HUNT;
			} else {

				if (!IsALoaner) {
					/*
					**	Normal aircraft try to find a good landing spot to rest.
					*/
					BuildingClass * building = Find_Docking_Bay(Class->Building, false);
					Assign_Destination(TARGET_NONE);
					if (building != NULL && Transmit_Message(RADIO_HELLO, building) == RADIO_ROGER) {
						if (Class->IsFixedWing) {
							Status = 0;	//BG - reset the mission status to avoid landing on the ground next to the airstrip
							if (IsLanding) {
								Process_Take_Off();
							}
						}
						mission = MISSION_ENTER;
					} else {
						mission = MISSION_RETREAT;
					}
				}
			}
		}

	} else {

		if (In_Which_Layer() == LAYER_GROUND) {
			if (IsALoaner) {
				if (Is_Something_Attached()) {

					/*
					**	In the case of a computer controlled helicopter that hold passengers,
					**	don't unload when landing. Wait for specific instructions from the
					**	controlling team.
					*/
					if (Team.Is_Valid()) {
//					if (Team.Is_Valid() && !House->IsHuman) {
						mission = MISSION_GUARD;
					} else {
						mission = MISSION_UNLOAD;
					}
				} else {
					mission = MISSION_RETREAT;
				}
			} else {
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
				if ((IsALoaner && House->IsHuman) || (!House->IsHuman && !Ammo)) {
					if (Team.Is_Valid() && Team->Has_Entered_Map()) {
						Team->Remove(this);
					}
				}

				if (Class->PrimaryWeapon != NULL) {

					/*
					**	Weapon equipped helicopters that run out of ammo and were
					**	brought in as reinforcements will leave the map.
					*/
					if (IsALoaner) {

						/*
						**	If it has no ammo, then break off of the team and leave the map.
						**	If it can fight, then give it fighting orders.
						*/
						if (Ammo == 0) {
							if (Team.Is_Valid()) Team->Remove(this);
							mission = MISSION_RETREAT;
						} else {
							if (!Team.Is_Valid()) {
								mission = MISSION_HUNT;
							}
						}

					} else {

						/*
						**	Normal aircraft try to find a good landing spot to rest.
						*/
						BuildingClass * building = Find_Docking_Bay(Class->Building, false);
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	if (!Class->IsFixedWing) {
		int dist = 0x7FFFFFFF;
		if (building) dist=Distance(building);
		for (int index = 0; index < Vessels.Count(); index++) {
			VesselClass *ship = Vessels.Ptr(index);
			if (ship != NULL && *ship == VESSEL_CARRIER && !ship->IsInLimbo && ship->IsActive && ship->House == House && ship->How_Many() < ship->Class->Max_Passengers()/* && !ship->In_Radio_Contact()*/) {
				if (Distance(ship) < dist || !building) {
					building = (BuildingClass *)ship;
					dist = Distance(ship);
				}
//				break;
			}
		}
	}
#endif
						Assign_Destination(TARGET_NONE);
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
						if (building && (Transmit_Message(RADIO_HELLO, building) == RADIO_ROGER || building->What_Am_I() == RTTI_VESSEL) ) {
#else
						if (building && Transmit_Message(RADIO_HELLO, building) == RADIO_ROGER) {
#endif
							mission = MISSION_ENTER;
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
							if (building->What_Am_I() == RTTI_VESSEL) {
								Assign_Destination(building->As_Target());
							}
#endif
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
 * WARNINGS:   Because the aircraft can move at a fast speed, the distance to target value     *
 *             will probably never be zero. The likely case will be that the aircraft          *
 *             overshoots the target.                                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/14/1995 JLB : Created.                                                                 *
 *   03/05/1996 JLB : Specifies destination target value.                                      *
 *=============================================================================================*/
int AircraftClass::Process_Fly_To(bool slowdown, TARGET dest)
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (Class->IsFixedWing) slowdown = false;

	COORDINATE coord;
	if (Is_Target_Building(dest)) {
		coord = As_Building(dest)->Docking_Coord();
	} else {
		coord = As_Coord(dest);
	}
	int distance = Distance(coord);

	PrimaryFacing.Set_Desired(Direction(coord));

	if (slowdown) {
		int speed = min(distance, 0x0300);
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
void AircraftClass::Debug_Dump(MonoClass * mono) const
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	mono->Set_Cursor(0, 0);
	mono->Print(Text_String(TXT_DEBUG_AIRCRAFT));
	mono->Set_Cursor(1, 11);mono->Printf("%3d", AttacksRemaining);

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
 *             propagated to all connected machines.                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Active_Click_With(ActionType action, ObjectClass * object)
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	action = What_Action(object);

	switch (action) {
		case ACTION_NOMOVE:
			return;

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
 *             propagated to all connected machines.                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Active_Click_With(ActionType action, CELL cell)
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

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
 *             This is controlled by net/modem propagation delay.                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Player_Assign_Mission(MissionType mission, TARGET target, TARGET destination)
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (AllowVoice) {
		if (mission == MISSION_ATTACK) {
			Response_Attack();
		} else {
			Response_Move();
		}
	}
	Queue_Mission(TargetClass(this), mission, target, destination);
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
ActionType AircraftClass::What_Action(ObjectClass const * target) const
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	ActionType action = FootClass::What_Action(target);

	if (action == ACTION_SELF && (!How_Many() || (Height > 0) || IsTethered)) {
		action = ACTION_NONE;
	}

	if (action == ACTION_ATTACK && Class->PrimaryWeapon == NULL) {
		action = ACTION_NONE;
	}

	if (House->IsPlayerControl && House->Is_Ally(target) && target->What_Am_I() == RTTI_BUILDING && ((AircraftClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (TechnoClass*)target) == RADIO_ROGER) {
		action = ACTION_ENTER;
	}
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	if (!Class->IsFixedWing && House->IsPlayerControl && House->Is_Ally(target) && target->What_Am_I() == RTTI_VESSEL && *(VesselClass *)target == VESSEL_CARRIER && ((AircraftClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (TechnoClass*)target) == RADIO_ROGER) {
		action = ACTION_ENTER;
	}
#endif

	if (Class->IsFixedWing && action == ACTION_MOVE) {
		action = ACTION_NOMOVE;
	}

	if (action == ACTION_NONE) {
		action = What_Action(Coord_Cell(target->Center_Coord()));
	}

	/*
	**	Special return to friendly repair factory action.
	*/
	if (House->IsPlayerControl && action == ACTION_SELECT && target->What_Am_I() == RTTI_BUILDING) {
		BuildingClass * building = (BuildingClass *)target;
		if (building->Class->Type == STRUCT_REPAIR && !building->In_Radio_Contact() && !building->Is_Something_Attached()) {
			action = ACTION_ENTER;
		}
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	ActionType action = FootClass::What_Action(cell);

	//using function for IsVisible so we have different results for different players - JAS 2019/09/30
	if (action == ACTION_MOVE && !Map[cell].Is_Visible(PlayerPtr)) {
		action = ACTION_NOMOVE;
	}

	if (action == ACTION_ATTACK && Class->PrimaryWeapon == NULL) {
		action = ACTION_NONE;
	}

	if (Class->IsFixedWing && action == ACTION_MOVE) {
		action = ACTION_NOMOVE;
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
 *   03/04/1996 JLB : Fixed wing aircraft always face down the runway.                         *
 *=============================================================================================*/
DirType AircraftClass::Pose_Dir(void) const
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (*this == AIRCRAFT_TRANSPORT) {
		return(DIR_N);
	}
	if (Class->IsFixedWing)  {
		return(DIR_E);
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
 *   09/22/1995 JLB : Fixes brain dead helicopter for Nod scen #7.                             *
 *=============================================================================================*/
int AircraftClass::Mission_Attack(void)
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (Class->IsFixedWing) {
		return(Mission_Hunt());
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
					if (In_Radio_Contact() && Map[Coord].Cell_Building() == Contact_With_Whom()) {
						Transmit_Message(RADIO_OVER_OUT);
					}

					/*
					**	Start flying toward the destination by skewing at first.
					**	As the flight progresses, the body will rotate to face
					**	the direction of travel.
					*/
					int diff = SecondaryFacing.Difference(Direction(NavCom));
					diff = Bound(diff, -128, 128);
					PrimaryFacing = DirType((int)SecondaryFacing.Current()+diff);
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

				int distance = Process_Fly_To(true, NavCom);

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

				case FIRE_REARM:
				case FIRE_FACING:
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
						Status = Rule.IsCurleyShuffle ? PICK_ATTACK_LOCATION : FIRE_AT_TARGET;
					} else {
						Status = RETURN_TO_BASE;
					}
					break;

				default:
					if (!Ammo) {
						Status = RETURN_TO_BASE;
					} else {
						if (!In_Range(TarCom)) {
							Status = PICK_ATTACK_LOCATION;
						} else {
							Status = Rule.IsCurleyShuffle ? PICK_ATTACK_LOCATION : FIRE_AT_TARGET;
						}
					}
					break;
			}
			break;

		/*
		**	Fly back to landing spot.
		*/
		case RETURN_TO_BASE:
			/*
			**	Break off of firing at the target if there is no more
			**	point in attacking it this mission. The player will
			**	reassign a target for the next mission.
			*/
			if (!Ammo && (IsALoaner || House->IsHuman)) {
				Assign_Target(TARGET_NONE);
			}
			Assign_Destination(TARGET_NONE);
			Enter_Idle_Mode();
			break;

		default:
			break;
	}

	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (Target_Legal(oldlz) && (!Is_LZ_Clear(oldlz) || !Cell_Seems_Ok(As_Cell(oldlz)))) {
		COORDINATE coord = As_Coord(oldlz);

		/*
		**	Scan outward in a series of concentric rings up to certain distance
		**	in cells.
		*/
		for (int radius = 0; radius < Rule.LZScanRadius / CELL_LEPTON_W; radius++) {
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	switch (message) {

		case RADIO_PREPARED:
			if (Target_Legal(TarCom)) return(RADIO_NEGATIVE);
			if ((Height == 0 && Ammo == Class->MaxAmmo) || (Height > 0 && Ammo > 0)) return(RADIO_ROGER);
			return(RADIO_NEGATIVE);

		/*
		**	Something disastrous has happened to the object in contact with. Fall back
		**	and regroup. This means that any landing process is immediately aborted.
		*/
		case RADIO_RUN_AWAY:
			if (IsLanding) {
				IsLanding = false;
				IsTakingOff = true;
			}
			if (Class->IsFixedWing) {
				Assign_Destination(Good_LZ());
				if (!Target_Legal(NavCom)) {
					Assign_Mission(MISSION_RETREAT);
				} else {
					Assign_Mission(MISSION_MOVE);
				}
			} else {
				Scatter(0, true);
			}
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
			if (_Counts_As_Civ_Evac(from)) {
				Assign_Mission(MISSION_RETREAT);
			}
			return(RADIO_ATTACH);

		/*
		**	Docking maintenance message received. Check to see if new orders should be given
		**	to the impatient unit.
		*/
		case RADIO_DOCKING:
			if (Class->Max_Passengers() > 0 && How_Many() < Class->Max_Passengers()) {
				FootClass::Receive_Message(from, message, param);

				if (!IsTethered && !IsLanding && !IsTakingOff && Height == 0) {

					Open_Door(5, 4);

					/*
					**	If the potential passenger needs someplace to go, then figure out a good
					**	spot and tell it to go.
					*/
					if (Transmit_Message(RADIO_NEED_TO_MOVE, from) == RADIO_ROGER) {
						CELL cell;
						/*DirType dir =*/ Desired_Load_Dir(from, cell);

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
			if (Class->Max_Passengers() == 0 || from == NULL || !House->Is_Ally(from->Owner()) || Height > 0) return(RADIO_STATIC);
			if (/*!In_Radio_Contact() &&*/ How_Many() < Class->Max_Passengers()) {
				return(RADIO_ROGER);
			}
			return(RADIO_NEGATIVE);

		default:
			break;
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	CELL center = Coord_Cell(Center_Coord());
	for (int sweep = FACING_N; sweep < FACING_S; sweep++) {
		moveto = Adjacent_Cell(center, FacingType(FACING_S+sweep));
		if (Map.In_Radar(moveto) && (Coord_Cell(object->Center_Coord()) == moveto || Map[moveto].Is_Clear_To_Move(SPEED_FOOT, false, false))) return(DIR_N);

		moveto = Adjacent_Cell(center, FacingType(FACING_S-sweep));
		if (Map.In_Radar(moveto) && (Coord_Cell(object->Center_Coord()) == moveto || Map[moveto].Is_Clear_To_Move(SPEED_FOOT, false, false))) return(DIR_N);
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	IsLanding = false;
	IsTakingOff = true;

	if (Class->IsFixedWing)  {
		Set_Speed(0xFF);
		if (Height == FLIGHT_LEVEL)  {
			return(true);
		}

	} else {

		switch (Height) {
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

			default:
				break;
		}
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
 *   03/04/1996 JLB : Handles fixed wing aircraft.                                             *
 *=============================================================================================*/
bool AircraftClass::Process_Landing(void)
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	IsTakingOff = false;
	IsLanding = true;

	if (Class->IsFixedWing)  {
		int distance = Distance(NavCom);

		if (distance > 0x0100)  {
			SecondaryFacing.Set_Desired(::Direction(Fire_Coord(0), As_Coord(NavCom)));
		}

		switch (Height) {
			case 0:
				Set_Speed(0);
				IsLanding = false;
				return(true);

			default:
//				if (distance*2 > Class->LandingSpeed) {
//					Set_Speed(Class->LandingSpeed);
//				} else {
//					Set_Speed(distance/2);
//				}

				Set_Speed(Class->LandingSpeed / House->AirspeedBias);
				break;
		}

	} else {
		switch (Height) {
			case 0:
				IsLanding = false;
				return(true);

			case FLIGHT_LEVEL/2:
				Set_Speed(0);
				break;

			case FLIGHT_LEVEL:
				break;

			default:
				break;
		}
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (!Map.In_Radar(cell)) return(MOVE_NO);

	CellClass * cellptr = &Map[cell];

	ObjectClass const * occupier = cellptr->Cell_Occupier();

	if (occupier == NULL ||
		!occupier->Is_Techno() ||
		((TechnoClass *)occupier)->House->Is_Ally(House) ||
		(((TechnoClass *)occupier)->Cloak != CLOAKED &&
			(ScenarioInit == 0 && (occupier->What_Am_I() != RTTI_BUILDING || !((BuildingClass*)occupier)->Class->IsInvisible)) )
		) {

		if (!cellptr->Is_Clear_To_Move(SPEED_TRACK, false, false)) return(MOVE_NO);
	}

	if (Session.Type == GAME_NORMAL && IsOwnedByPlayer && !cellptr->IsMapped) {
		return(MOVE_NO);
	}

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
 *   11/02/1996 JLB : Bias fire position to get closer to moving objects.                      *
 *=============================================================================================*/
TARGET AircraftClass::Good_Fire_Location(TARGET target) const
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (Target_Legal(target)) {
		int range = Weapon_Range(0);
		COORDINATE tcoord = As_Coord(target);
		CELL bestcell = 0;
		CELL best2cell = 0;
		int bestval = -1;
		int best2val = -1;

		/*
		**	Try to get closer to a target that is moving.
		*/
		COORDINATE altcoord = 0;
		if (Is_Target_Object(target) && As_Object(target)->Is_Foot()) {
			TARGET alttarg = ((FootClass *)As_Object(target))->NavCom;
			if (Target_Legal(alttarg)) {
				altcoord = As_Coord(alttarg);
			}
		}

		for (int r = range-0x0100; r > 0x0100; r -= 0x0100) {
			for (int face = 0; face < 255; face += 16) {
				COORDINATE newcoord = Coord_Move(tcoord, (DirType)face, r);
				CELL newcell = Coord_Cell(newcoord);

				//using function for IsVisible so we have different results for different players - JAS 2019/09/30
				if (Map.In_Radar(newcell) && (Session.Type != GAME_NORMAL || Map[newcell].Is_Visible(PlayerPtr)) && Cell_Seems_Ok(newcell, true)) {
					int dist;
					if (altcoord != 0) {
						dist = ::Distance(newcoord, altcoord);
					} else {
						dist = Distance(newcoord);
					}
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
			if (Percent_Chance(50)) {
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
 *    specified cell is safe to fly to. The intent of this routine is to avoid unnecessary     *
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	/*
	**	Make sure that no other aircraft are heading to the selected location. If they
	**	are, then don't consider the location as valid.
	*/
	TARGET astarget = ::As_Target(cell);
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	int retval = 0;

	bool carrying_passengers = (Class->Max_Passengers() > 0) && ((*this != AIRCRAFT_BADGER) || (Mission != MISSION_HUNT));
	if (carrying_passengers) {
		retval = How_Many();
	} else {
		if (Ammo) {
			retval = Class->Max_Pips() * fixed(Ammo, Class->MaxAmmo);
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	enum {
		INITIAL,
		TAKEOFF,
		ALTITUDE,
		STACK,
		DOWNWIND,
		CROSSWIND,
		TRAVEL,
		LANDING
	};

	/*
	**	Verify that it has a valid NavCom. If it doesn't then request one from the
	**	building this building is trying to land upon. If that fails, then enter
	**	idle mode.
	*/
	if (!Target_Legal(NavCom) && In_Which_Layer() != LAYER_GROUND) {
		if (Transmit_Message(RADIO_DOCKING) != RADIO_ROGER) {
			Enter_Idle_Mode();
			return(1);
		}
	}

	switch (Status) {
		case INITIAL:
			if (Height < FLIGHT_LEVEL || IsLanding) {
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
				if (In_Radio_Contact() && Map[Coord].Cell_Building() == Contact_With_Whom()) {
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
				if (!Target_Legal(NavCom)) {
					Transmit_Message(RADIO_DOCKING);
					if (!Target_Legal(NavCom)) {
						Enter_Idle_Mode();
						return(1);
					}
				}
				Status = STACK;
			} else {
				TechnoClass * tech = As_Techno(NavCom);
				if (tech && Transmit_Message(RADIO_CAN_LOAD, tech) == RADIO_ROGER) {
					Transmit_Message(RADIO_HELLO, tech);
					Transmit_Message(RADIO_DOCKING);
					Status = STACK;
				} else {
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
					if (tech->What_Am_I() != RTTI_VESSEL) {
						Assign_Destination(TARGET_NONE);
						Enter_Idle_Mode();
					}
#else
					Assign_Destination(TARGET_NONE);
					Enter_Idle_Mode();
#endif
				}
			}
			break;

		case STACK:
			if (Class->IsFixedWing)  {
				int distance;
				TARGET togo;

				BuildingClass const * building = As_Building(NavCom);
				if (building)  {
					togo = ::As_Target(building->Check_Point(CHECK_STACK));
				} else {
					togo = NavCom;
				}

				distance = Process_Fly_To(true, togo);
				if (distance < 0x0080)  {
					Status = DOWNWIND;
				}
			} else {
				Status = DOWNWIND;
			}
			break;

		case DOWNWIND:
			if (Class->IsFixedWing)  {
				int distance;
				TARGET togo;

				Set_Speed(200);
				BuildingClass const * building = As_Building(NavCom);
				if (building)  {
					togo = ::As_Target(building->Check_Point(CHECK_DOWNWIND));
				} else {
					togo = NavCom;
				}

				distance = Process_Fly_To(true, togo);
				if (distance < 0x0080)  {
					Status = CROSSWIND;
				}
			} else {
				Status = CROSSWIND;
			}
			break;

		case CROSSWIND:
			if (Class->IsFixedWing)  {
				int distance;
				TARGET togo;

				Set_Speed(140);
				BuildingClass const * building = As_Building(NavCom);
				if (building)  {
					togo = ::As_Target(building->Check_Point(CHECK_CROSSWIND));
				} else {
					togo = NavCom;
				}

				distance = Process_Fly_To(true, togo);
				if (distance < 0x0080)  {
					Status = TRAVEL;
				}
			} else {
				Status = TRAVEL;
			}
			break;

		case TRAVEL:
			Transmit_Message(RADIO_DOCKING);
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
			if (!In_Radio_Contact() && !Is_Target_Vessel(NavCom)) {
#else
			if (!In_Radio_Contact()) {
#endif
				Assign_Destination(TARGET_NONE);
				Enter_Idle_Mode();
			} else {
				int distance = Process_Fly_To(true, NavCom);

				if (Class->IsFixedWing)  {

					if (distance < 0x0400)  {
						Status = LANDING;
					}
					return(1);

				} else {
					if (distance < 0x0080) {
						if (Target_Legal(TarCom)) {
							SecondaryFacing.Set_Desired(Direction(TarCom));
						} else {
							SecondaryFacing.Set_Desired(Pose_Dir());
						}

#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
 if (Is_Target_Vessel(NavCom) && !In_Radio_Contact()) {
 	Enter_Idle_Mode();
 	break;
 }
#endif
						if (distance < 0x0010) {
							Status = LANDING;
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	if(Is_Target_Vessel(NavCom) && As_Vessel(NavCom)->NavCom) {
		Status = TRAVEL;
	}
#endif

						}
						break;
					} else {
						SecondaryFacing.Set_Desired(Direction(NavCom));
//						SecondaryFacing.Set_Desired(::Direction(Fire_Coord(0), As_Coord(NavCom)));
					}
				}
				return(3);
			}
			break;

		case LANDING:
			if (IsTakingOff && !Class->IsFixedWing) {
				Assign_Destination(TARGET_NONE);
				Enter_Idle_Mode();
			}
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
// If we were trying to land on a carrier and it moved, take off again
	if ( As_Vessel(NavCom) && !In_Radio_Contact()) {
		Status = INITIAL;
		break;
	}
#endif
			if (Process_Landing()) {
				switch (Transmit_Message(RADIO_IM_IN)) {
					case RADIO_ROGER:
						Assign_Mission(MISSION_GUARD);
						break;

					case RADIO_ATTACH:
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
						if(Contact_With_Whom()->What_Am_I() != RTTI_VESSEL) Limbo();
#else
						Limbo();
#endif
						Contact_With_Whom()->Attach(this);
						break;

					default:
						Enter_Idle_Mode();
				}
			}
			break;

		default:
			break;
	}
	return(1);
}


/***********************************************************************************************
 * AircraftClass::Good_LZ -- Locates a good spot to land.                                      *
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	/*
	**	Scan through all of the buildings and try to land near
	**	the helipad (if there is one) or the nearest friendly building.
	*/
	CELL bestcell = 0;
	int bestdist = -1;
	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass * building = Buildings.Ptr(index);

		if (building && !building->IsInLimbo && building->House == House) {
			int dist = Distance(building);
			if (*building == Class->Building) {
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	FootClass::Set_Speed(speed);

	MPHType sp = MPHType(min(Class->MaxSpeed * SpeedBias * House->AirspeedBias, MPH_LIGHT_SPEED));
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

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

		/*
		**	Remove this member from any team it may be associated with. This must occur at the
		**	top most level of the inheritance hierarchy because it may call virtual functions.
		*/
		if (Team) {
			Team->Remove(this);
			Team = NULL;
		}

		House->Tracking_Remove(this);

		/*
		**	If there are any cargo members, delete them.
		*/
		while (Is_Something_Attached()) {
			delete Detach_Object();
		}

		AircraftClass::Limbo();
		Class = 0;
	}
	ID = -1;
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
void AircraftClass::Scatter(COORDINATE , bool, bool )
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	/*
	**	Certain missions prevent scattering regardless of whether it would be
	**	a good idea or not.
	*/
	if (!MissionControl[Mission].IsScatter) return;

	/*
	**	Fixed wing aircraft never scatter.
	*/
	if (Class->IsFixedWing) return;

	if (IsLanding || Height == 0) {
		IsLanding = false;
		IsTakingOff = true;
	}
	Enter_Idle_Mode();
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
 *   10/10/1995 JLB : Hunts for harvesters that are unescorted.                                *
 *=============================================================================================*/
int AircraftClass::Mission_Guard(void)
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (Height == FLIGHT_LEVEL) {

		/*
		**	If part of a team, then do nothing, since the team
		**	handler will take care of giving this aircraft a
		**	mission.
		*/
		if (Team) {
			if (Target_Legal(NavCom)) {
				Assign_Mission(MISSION_MOVE);
			}
			return(MissionControl[Mission].Normal_Delay());
		}

		if (Class->PrimaryWeapon == NULL) {
			Assign_Destination(::As_Target(Coord_Cell(Coord)));
			Assign_Mission(MISSION_MOVE);
		} else {
			if (!Team.Is_Valid()) Enter_Idle_Mode();
		}
		return(1);
	}
	if (House->IsHuman) return(MissionControl[Mission].Normal_Delay());

	/*
	**	If the aircraft is very badly damaged, then it will search for a
	**	repair bay first.
	*/
	if (House->Available_Money() >= 100 && Health_Ratio() <= Rule.ConditionYellow) {
		if (!In_Radio_Contact() ||
			(Height == 0 &&
				(Contact_With_Whom()->What_Am_I() != RTTI_BUILDING || *((BuildingClass *)Contact_With_Whom()) != STRUCT_REPAIR))) {


			BuildingClass * building = Find_Docking_Bay(STRUCT_REPAIR, true);
			if (building != NULL) {
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
	if (Ammo == 0 && Is_Weapon_Equipped()) {
		if (!In_Radio_Contact()) {
			BuildingClass * building = Find_Docking_Bay(STRUCT_HELIPAD, false);
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	if (!Class->IsFixedWing) {
		int dist = 0x7FFFFFFF;
		if (building) dist=Distance(building);
		for (int index = 0; index < Vessels.Count(); index++) {
			VesselClass *ship = Vessels.Ptr(index);
			if (ship != NULL && *ship == VESSEL_CARRIER && !ship->IsInLimbo && ship->IsActive && ship->House == House && ship->How_Many() < ship->Class->Max_Passengers()) {
				if (Distance(ship) < dist || !building) {
					building = (BuildingClass *)ship;
					dist = Distance(ship);
				}
//				break;
			}
		}
	}
#endif
			if (building != NULL) {
				Assign_Destination(building->As_Target());
				Assign_Target(TARGET_NONE);
				Assign_Mission(MISSION_ENTER);
				return(1);
			}
		}
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
	if (!Is_Weapon_Equipped()) {
		return(TICKS_PER_SECOND*3);
	}

	/*
	**	Computer controlled helicopters will defend themselves by bouncing around
	**	and looking for a free helipad.
	*/
	if (Height == 0 && !In_Radio_Contact()) {
		Scatter(0, true);
		return(TICKS_PER_SECOND*3);
	}

	/*
	**	Perform a special check to hunt for harvesters that are outside of the protective
	**	shield of their base.
	*/
	if (House->State != STATE_ATTACKED) {
		TARGET target = House->Find_Juicy_Target(Coord);

		if (Target_Legal(target)) {
			Assign_Target(target);
			Assign_Mission(MISSION_ATTACK);
		}
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (Height == FLIGHT_LEVEL) {
		if (!Team.Is_Valid()) Enter_Idle_Mode();
		return(1);
	}
	if (House->IsHuman) return(TICKS_PER_SECOND);

	if (Height == 0 && !In_Radio_Contact()) {
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
	assert(Aircraft.ID(this) == ID);
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
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	static VocType _response[] = {
		VOC_ACKNOWL,
		VOC_AFFIRM
	};
	VocType response = _response[Sim_Random_Pick(0, ARRAY_SIZE(_response)-1)];
	if (AllowVoice) {
		Sound_Effect(response, fixed(1), -(ID+1));
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
	assert(Aircraft.ID(this) == ID);
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
 * AircraftClass::Can_Fire -- Checks to see if the aircraft can fire.                          *
 *                                                                                             *
 *    This routine is used to determine if the aircraft can fire its weapon at the target      *
 *    specified. If it cannot, then the reason why is returned.                                *
 *                                                                                             *
 * INPUT:   target   -- The target that the aircraft might fire upon.                          *
 *                                                                                             *
 *          which    -- The weapon that will be used to fire.                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the reason why it can't fire or with FIRE_OK.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/05/1996 JLB : Created.                                                                 *
 *   07/11/1996 JLB : Fixed for camera carrying aircraft.                                      *
 *=============================================================================================*/
FireErrorType AircraftClass::Can_Fire(TARGET target, int which) const
{
	assert(Aircraft.ID(this) == ID);
	assert(IsActive);

	if (Passenger && !Is_Something_Attached()) {
		return(FIRE_AMMO);
	}

	bool camera = (Class->PrimaryWeapon != NULL && Class->PrimaryWeapon->IsCamera);
	bool fudge = (Passenger || (Class->PrimaryWeapon != NULL && Class->PrimaryWeapon->Bullet != NULL && Class->PrimaryWeapon->Bullet->IsParachuted));

	if (fudge && !camera && !Ammo && !Passenger) {
		return(FIRE_AMMO);
	}

	/*
	**	Passenger aircraft that wish to 'fire' actually are requesting to
	**	paradrop or 'throw out' the cargo. This is always allowed if the terrain under the
	**	aircraft is generally clear.
	*/
	if (camera || (fudge && Passenger && Is_Something_Attached()))  {
		if (Arm != 0) return(FIRE_REARM);

		if (Distance(target) < (camera ? 0x0380 : 0x0200) && Map.In_Radar(Coord_Cell(Center_Coord()))) {
//		if (Distance(target) < (camera ? 0x0380 : 0x0280) && Map.In_Radar(Coord_Cell(Center_Coord()))) {
			return(FIRE_OK);
		}
		return(FIRE_RANGE);
	}

	FireErrorType canfire = FootClass::Can_Fire(target, which);

	if (canfire == FIRE_OK) {

		/*
		**	Double check to make sure that the facing is roughly toward
		**	the target. If the difference is too great, then firing is
		**	temporarily postponed.
		*/
		if (Class->IsFixedWing) {

			int diff = PrimaryFacing.Difference(Direction(TarCom));
			if (ABS(diff) > (fudge ? 16 : 8)) {
				return(FIRE_FACING);
			}
		}
	}
	return(canfire);
}


/***********************************************************************************************
 * AircraftClass::Landing_Takeoff_AI -- Handle aircraft take off and landing processing.       *
 *                                                                                             *
 *    This routine handles the tricky maneuver of taking off and landing. The process of       *
 *    landing is not entirely safe and thus the aircraft may be destroyed as a consequence.    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the aircraft destroyed by this process?                                  *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per aircraft per game logic loop. Be sure to        *
 *             examine the return value and if true, abort all further processing of this      *
 *             aircraft since it is now dead.                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Landing_Takeoff_AI(void)
{
	/*
	**	Handle landing and taking off logic. Helicopters are prime users of this technique. The
	**	aircraft will either gain or lose altitude as appropriate. As the aircraft transitions
	**	between flying level and ground level, it will be moved into the appropriate render
	**	layer.
	*/
	if (Is_Door_Closed() && (IsLanding || IsTakingOff)) {
		LayerType layer = In_Which_Layer();

		if (IsLanding) {
			Mark(MARK_UP);
			if (Height) Height -= Pixel_To_Lepton(1);
			if (Height <= 0) {
				Height = 0;
				IsLanding = false;
				Set_Speed(0);

				/*
				**	If the NavCom now equals the destination, then clear out the NavCom.
				*/
				if (Coord_Cell(Center_Coord()) == As_Cell(NavCom)) {
					Assign_Destination(TARGET_NONE);
				}

				/*
				** If a fixed-wing aircraft just landed on the ground, blow him up
				*/
				if (Class->IsFixedWing && Mission != MISSION_ENTER) {
					Strength = 1;

					int damage = Strength;
					Map.Remove(this, layer);
					Take_Damage(damage, 0, WARHEAD_AP, 0, true);
					return(true);
				}

				if (Target_Legal(NavCom) && As_Techno(NavCom) == Contact_With_Whom()) {
					if (In_Radio_Contact() && Transmit_Message(RADIO_IM_IN) != RADIO_ROGER) {
						Scatter(0, true);
					}
				}
			}
			Mark(MARK_DOWN);
		}
		if (IsTakingOff) {
			Mark(MARK_UP);
//			Map.Remove(this, layer);
			Height += Pixel_To_Lepton(1);
			if (Height >= FLIGHT_LEVEL) {
				Height = FLIGHT_LEVEL;
				IsTakingOff = false;
			}
//			Map.Submit(this, In_Which_Layer());
			Mark(MARK_DOWN);
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
			if (In_Which_Layer() == LAYER_GROUND && !IsTakingOff && !Class->IsFixedWing) {
				if (!Is_LZ_Clear(::As_Target(Coord_Cell(Coord)))) {
					IsTakingOff = true;
					Mark(MARK_UP);
					Height += Pixel_To_Lepton(1);
					Mark(MARK_DOWN);
					ok = false;
				}
			}

			if (ok) {

				Map.Remove(this, layer);
				Map.Submit(this, In_Which_Layer());

				/*
				**	When the aircraft is close to the ground, it should exist as a ground object.
				**	This aspect is controlled by the Place_Down and Pick_Up functions.
				*/
				if (In_Which_Layer() == LAYER_GROUND) {
					Assign_Destination(TARGET_NONE);		// Clear the navcom.
					Transmit_Message(RADIO_TETHER);
					Look();
//					Map.Sight_From(Coord_Cell(Coord), 1, House, false);
				} else  {
					Transmit_Message(RADIO_UNTETHER);

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
	return(false);
}


/***********************************************************************************************
 * AircraftClass::Edge_Of_World_AI -- Detect if aircraft has exited the map.                   *
 *                                                                                             *
 *    Certain aircraft will be eliminated when they leave the edge of the world presumably     *
 *    after completing their mission. An exception is for aircraft that have been newly        *
 *    created as reinforcements and have not yet completed their mission.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the aircraft deleted by this routine?                                    *
 *                                                                                             *
 * WARNINGS:   Be sure to call this routine only once per aircraft per game logic loop. If     *
 *             the return value is true, then abort any further processing of this aircraft    *
 *             since it has been eliminated.                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Edge_Of_World_AI(void)
{
	if (!Map.In_Radar(Coord_Cell(Coord))) {
		if (Mission == MISSION_RETREAT /*|| (*this == AIRCRAFT_CARGO && !Is_Something_Attached())*/) {

			/*
			**	Check to see if there are any civilians aboard. If so, then flag the house
			**	that the civilian evacuation trigger event has been fulfilled.
			*/
			while (Is_Something_Attached()) {
				FootClass * obj = Detach_Object();

				/*
				**	Flag the owning house that civ evacuation has occurred.
				*/
				if (_Counts_As_Civ_Evac(obj)) {
					obj->House->IsCivEvacuated = true;
				}

				if (obj->Team.Is_Valid()) obj->Team->IsLeaveMap = true;

#ifdef OLD
				/*
				**	Transport planes that leave can only be because they carry purchased
				**	equipment and must be have their cost refunded.
				*/
				if (*this == AIRCRAFT_CARGO) {
					House->Refund_Money(obj->Class_Of().Cost_Of());
				}
#endif
				delete obj;
			}
			if (Team.Is_Valid()) {
				Team->IsLeaveMap = true;
			}
			Stun();
			delete this;
			return(true);
		}
	} else {
		IsLocked = true;
	}
	return(false);
}


/***********************************************************************************************
 * AircraftClass::Movement_AI -- Handles aircraft physical movement logic.                     *
 *                                                                                             *
 *    This routine manages the aircraft movement across the map. If any movement occurred, the *
 *    aircraft will be flagged to be redrawn.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per aircraft per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Movement_AI(void)
{
	/*
	**	If for some strange reason, there is a valid NavCom, but this aircraft is not
	**	in a movement order, then give it a movement order.
	*/
	if (Target_Legal(NavCom) && Mission == MISSION_GUARD && MissionQueue == MISSION_NONE) {
		Assign_Mission(MISSION_MOVE);
	}

	if (Speed != 0) {
		if (In_Which_Layer() == LAYER_GROUND)  {
			Mark(MARK_UP);
			Physics(Coord, PrimaryFacing);
			Mark(MARK_DOWN);
		} else {
			Mark(MARK_CHANGE_REDRAW);
			if (Physics(Coord, PrimaryFacing) != RESULT_NONE) {
				Mark(MARK_CHANGE_REDRAW);
			}
		}
	}
}


/***********************************************************************************************
 * AircraftClass::Rotation_AI -- Handle aircraft body and flight rotation.                     *
 *                                                                                             *
 *    This will process the aircraft visible body and flight model rotation operations. If     *
 *    any rotation occurred, the aircraft will be flagged to be redrawn.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per aircraft per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Rotation_AI(void)
{
	if (PrimaryFacing.Is_Rotating()) {
		Mark(MARK_CHANGE_REDRAW);
		if (PrimaryFacing.Rotation_Adjust(Class->ROT)) {
			Mark(MARK_CHANGE_REDRAW);
		}
	}
	if (Class->IsFixedWing) {
		SecondaryFacing = PrimaryFacing;
	}
	if (SecondaryFacing.Is_Rotating()) {
		Mark(MARK_CHANGE_REDRAW);
		if (SecondaryFacing.Rotation_Adjust(Class->ROT)) {
			Mark(MARK_CHANGE_REDRAW);
		}
	}
}


/***********************************************************************************************
 * AircraftClass::Per_Cell_Process -- Handle the aircraft per cell process.                    *
 *                                                                                             *
 *    This is a seldom used function since its only purpose is to be called when an aircraft   *
 *    lands on the ground.                                                                     *
 *                                                                                             *
 * INPUT:   why   -- Why was this per cell process function called.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/15/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Per_Cell_Process(PCPType why)
{
	BStart(BENCH_PCP);
	FootClass::Per_Cell_Process(why);
	BEnd(BENCH_PCP);
}


/***********************************************************************************************
 * AircraftClass::Assign_Destination -- Assigns movement destination to the object.            *
 *                                                                                             *
 *    This routine is called when the object needs to have a new movement destination          *
 *    assigned.  Aircraft have their own version of this routine because a fixed-wing plane	  *
 *    trying to land will behave poorly if given a new destinatio while it's landing.			  *                                                                                         *
 *                                                                                             *
 * INPUT:   destination -- The destination to assign to this object.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Assign_Destination(TARGET dest)
{
	assert(IsActive);
	if (dest == NavCom) return;

	if (Target_Legal(dest) && Class->IsFixedWing && (IsLanding || (Target_Legal(NavCom) && dest != NavCom))) {
//	if (Target_Legal(dest) /*&& Class->IsFixedWing*/ && (IsLanding || (Target_Legal(NavCom) && dest != NavCom))) {

//		if (Class->IsFixedWing || As_Cell(dest) != Coord_Cell(Center_Coord())) {
			Process_Take_Off();
			Status = 0;
//		}
	}
	FootClass::Assign_Destination(dest);
}


/***********************************************************************************************
 * AircraftClass::In_Which_Layer -- Calculates the display layer of the aircraft.              *
 *                                                                                             *
 *    This examines the aircraft to determine what display layer it should be located          *
 *    in. Fixed wing aircraft must always be in the top layer if they are flying even though   *
 *    they may be low to the ground.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the layer that this aircraft resides in.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
LayerType AircraftClass::In_Which_Layer(void) const
{
	if (Class->IsFixedWing && Height > 0) {
		return(LAYER_TOP);
	}
	return(FootClass::In_Which_Layer());
}


/***********************************************************************************************
 * AircraftClass::Look -- Aircraft will look if they are on the ground always.                 *
 *                                                                                             *
 *    Aircraft perform a look operation according to their sight range. If the aircraft is     *
 *    on the ground, then it will look a distance of one cell regardless of what its           *
 *    specified sight range is.                                                                *
 *                                                                                             *
 * INPUT:   incremental -- Is this an incremental look?                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Look(bool incremental)
{
	assert(IsActive);
	assert(!IsInLimbo);

	int sight_range = Techno_Type_Class()->SightRange;
	if (Height == 0) {
		sight_range = 1;
	}

	if (sight_range) {
		Map.Sight_From(Coord_Cell(Coord), sight_range, House, incremental);
	}
}

unsigned AircraftClass::Spied_By() const
{
	unsigned spiedby = FootClass::Spied_By();

	/*
	** Aircraft inherit the spied by flags of the helipad they've landed on
	*/
	BuildingClass* building = Map[Coord].Cell_Building();
	if ((Height == 0) && In_Radio_Contact() && (building == Contact_With_Whom())) {
		spiedby |= building->Spied_By();
	}

	return spiedby;
}