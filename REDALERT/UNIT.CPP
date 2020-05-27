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

/* $Header: /CounterStrike/UNIT.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : UNIT.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : November 3, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Recoil_Adjust -- Adjust pixel values in direction specified.                              *
 *   UnitClass::AI -- AI processing for the unit.                                              *
 *   UnitClass::APC_Close_Door -- Closes an APC door.                                          *
 *   UnitClass::APC_Open_Door -- Opens an APC door.                                            *
 *   UnitClass::Active_Click_With -- Intercepts the active click to see if deployment is possib*
 *   UnitClass::Active_Click_With -- Performs specified action on specified cell.              *
 *   UnitClass::Approach_Target -- Handles approaching the target in order to attack it.       *
 *   UnitClass::Assign_Destination -- Assign a destination to a unit.                          *
 *   UnitClass::Blocking_Object -- Determines how a object blocks a unit                       *
 *   UnitClass::Can_Enter_Cell -- Determines cell entry legality.                              *
 *   UnitClass::Can_Fire -- Determines if turret can fire upon target.                         *
 *   UnitClass::Click_With -- Handles player map clicking while this unit is selected.         *
 *   UnitClass::Credit_Load -- Fetch the full credit value of cargo carried.                   *
 *   UnitClass::Crew_Type -- Fetches the kind of crew that this object produces.               *
 *   UnitClass::Debug_Dump -- Displays the status of the unit to the mono monitor.             *
 *   UnitClass::Desired_Load_Dir -- Determines the best cell and facing for loading.           *
 *   UnitClass::Draw_It -- Draws a unit object.                                                *
 *   UnitClass::Edge_Of_World_AI -- Check for falling off the edge of the world.               *
 *   UnitClass::Enter_Idle_Mode -- Unit enters idle mode state.                                *
 *   UnitClass::Fire_Direction -- Determines the direction of firing.                          *
 *   UnitClass::Firing_AI -- Handle firing logic for this unit.                                *
 *   UnitClass::Flag_Attach -- Attaches a house flag to this unit.                             *
 *   UnitClass::Flag_Remove -- Removes the house flag from this unit.                          *
 *   UnitClass::Goto_Clear_Spot -- Finds a clear spot to deploy.                               *
 *   UnitClass::Goto_Tiberium -- Search for and head toward nearest available Tiberium patch.  *
 *   UnitClass::Greatest_Threat -- Fetches the greatest threat for this unit.                  *
 *   UnitClass::Harvesting -- Harvests tiberium at the current location.                       *
 *   UnitClass::Init -- Clears all units for scenario preparation.                             *
 *   UnitClass::Limbo -- Limbo this unit.                                                      *
 *   UnitClass::Mission_Guard -- Special guard mission override processor.                     *
 *   UnitClass::Mission_Guard_Area -- Guard area logic for units.                              *
 *   UnitClass::Mission_Harvest -- Handles the harvesting process used by harvesters.          *
 *   UnitClass::Mission_Hunt -- This is the AI process for aggressive enemy units.             *
 *   UnitClass::Mission_Move -- Handles special move mission overrides.                        *
 *   UnitClass::Mission_Repair -- Handles finding and proceeding on a repair mission.          *
 *   UnitClass::Mission_Unload -- Handles unloading cargo.                                     *
 *   UnitClass::Offload_Tiberium_Bail -- Offloads one Tiberium quantum from the object.        *
 *   UnitClass::Ok_To_Move -- Queries whether the vehicle can move.                            *
 *   UnitClass::Overlap_List -- Determines overlap list for units.                             *
 *   UnitClass::Overrun_Square -- Handles vehicle overrun of a cell.                           *
 *   UnitClass::Per_Cell_Process -- Performs operations necessary on a per cell basis.         *
 *   UnitClass::Pip_Count -- Fetches the number of pips to display on unit.                    *
 *   UnitClass::Random_Animate -- Handles random idle animation for the unit.                  *
 *   UnitClass::Read_INI -- Reads units from scenario INI file.                                *
 *   UnitClass::Receive_Message -- Handles receiving a radio message.                          *
 *   UnitClass::Reload_AI -- Perform reload logic for this unit.                               *
 *   UnitClass::Rotation_AI -- Process any turret or body rotation.                            *
 *   UnitClass::Scatter -- Causes the unit to scatter to a nearby location.                    *
 *   UnitClass::Set_Speed -- Initiate unit movement physics.                                   *
 *   UnitClass::Shape_Number -- Fetch the shape number to use for this unit.                   *
 *   UnitClass::Should_Crush_It -- Determines if this unit should crush an object.             *
 *   UnitClass::Sort_Y -- Give Y coordinate sort value for unit.                               *
 *   UnitClass::Start_Driver -- Starts driving and reserves destination cell.                  *
 *   UnitClass::Take_Damage -- Inflicts damage points on a unit.                               *
 *   UnitClass::Tiberium_Check -- Search for and head toward nearest available Tiberium patch. *
 *   UnitClass::Tiberium_Load -- Determine the Tiberium load as a percentage.                  *
 *   UnitClass::Try_To_Deploy -- The unit attempts to "deploy" at current location.            *
 *   UnitClass::UnitClass -- Constructor for units.                                            *
 *   UnitClass::Unlimbo -- Removes unit from stasis.                                           *
 *   UnitClass::What_Action -- Determines action to perform on specified cell.                 *
 *   UnitClass::What_Action -- Determines what action would occur if clicked on object.        *
 *   UnitClass::Write_INI -- Store the units to the INI database.                              *
 *   UnitClass::delete -- Deletion operator for units.                                         *
 *   UnitClass::new -- Allocate a unit slot and adjust access arrays.                          *
 *   UnitClass::~UnitClass -- Destructor for unit objects.                                     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include "COORDA.h"

/***********************************************************************************************
 * Recoil_Adjust -- Adjust pixel values in direction specified.                                *
 *                                                                                             *
 *    This is a helper routine that modifies the pixel coordinates provided according to the   *
 *    direction specified. The effect is the simulate recoil effects by moving an object 'back'*
 *    one pixel. Since the pixels moved depend on facing, this routine handles the pixel       *
 *    adjustment quickly.                                                                      *
 *                                                                                             *
 * INPUT:   dir   -- The direction to base the recoil on.                                      *
 *                                                                                             *
 *          x,y   -- References to the pixel coordinates that will be adjusted.                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Recoil_Adjust(DirType dir, int &x, int &y)
{
	static struct {
		signed char X,Y;
	} _adjust[32] = {
		{0,	1},	// N
		{0,	1},
		{0,	1},
		{-1,	1},
		{-1,	1},	// NE
		{-1,	1},
		{-1,	0},
		{-1,	0},
		{-1,	0},	// E
		{-1,	0},
		{-1,	-1},
		{-1,	-1},
		{-1,	-1},	// SE
		{-1,	-1},
		{-1,	-1},
		{0,	-1},
		{0,	-1},	// S
		{0,	-1},
		{0,	-1},
		{1,	-1},
		{1,	-1},	// SW
		{1,	-1},
		{1,	0},
		{1,	0},
		{1,	0},	// W
		{1,	0},
		{1,	1},
		{1,	1},
		{1,	1},	// NW
		{1,	1},
		{0,	1},
		{0,	1}
	};

	int index = Dir_To_32(dir);
	x += _adjust[index].X;
	y += _adjust[index].Y;
}


/***********************************************************************************************
 * UnitClass::new -- Allocate a unit slot and adjust access arrays.                            *
 *                                                                                             *
 *    This routine will allocate a unit from the available unit pool and                       *
 *    fixup all the access lists to match. It will allocate a unit slot                        *
 *    from within the range allowed for the specified unit type. If no                         *
 *    slot was found, then it will fail.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the allocated unit.                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/11/1994 JLB : Created.                                                                 *
 *   04/21/1994 JLB : Converted to operator new.                                               *
 *=============================================================================================*/
void * UnitClass::operator new(size_t)
{
	void * ptr = Units.Alloc();
	if (ptr != NULL) {
		((UnitClass *)ptr)->Set_Active();
	}
	return(ptr);
}


/***********************************************************************************************
 * UnitClass::delete -- Deletion operator for units.                                           *
 *                                                                                             *
 *    This removes the unit from the local allocation system. Since this                       *
 *    is a fixed block of memory, not much has to be done to delete the                        *
 *    unit. Merely marking it as inactive is enough.                                           *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the unit to delete.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::operator delete(void * ptr)
{
	if (ptr != NULL) {
		((UnitClass *)ptr)->IsActive = false;
	}
	Units.Free((UnitClass *)ptr);
}


/***********************************************************************************************
 * UnitClass::~UnitClass -- Destructor for unit objects.                                       *
 *                                                                                             *
 *    This destructor will lower the unit count for the owning house as well as inform any     *
 *    other units in communication, that this unit is about to leave reality.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
UnitClass::~UnitClass(void)
{
	if (GameActive && Class.Is_Valid()) {

		/*
		**	Remove this member from any team it may be associated with. This must occur at the
		**	top most level of the inheritance hierarchy because it may call virtual functions.
		*/
		if (Team.Is_Valid()) {
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

		Limbo();
	}
	ID = -1;
}


/***********************************************************************************************
 * UnitClass::UnitClass -- Constructor for units.                                              *
 *                                                                                             *
 *    This constructor for units will initialize the unit into the game                        *
 *    system. It will be placed in all necessary tracking lists. The initial condition will    *
 *    be in a state of limbo.                                                                  *
 *                                                                                             *
 * INPUT:   classid  -- The type of unit to create.                                            *
 *                                                                                             *
 *          house -- The house owner of this unit.                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
UnitClass::UnitClass(UnitType classid, HousesType house) :
	DriveClass(RTTI_UNIT, Units.ID(this), house),
	Class(UnitTypes.Ptr((int)classid)),
	Flagged(HOUSE_NONE),
	IsDumping(false),
	Gems(0),
	Gold(0),
	Tiberium(0),
	IsToScatter(false),
	ShroudBits(0xFFFFFFFFUL),
	ShroudCenter(0),
	Reload(0),
	SecondaryFacing(PrimaryFacing)
{
	Reload = 0;
	House->Tracking_Add(this);
	Ammo = Class->MaxAmmo;
	IsCloakable = Class->IsCloakable;
	if (Class->IsAnimating) Set_Rate(Options.Normalize_Delay(3));

	/*
	**	For two shooters, clear out the second shot flag -- it will be set the first time
	**	the object fires. For non two shooters, set the flag since it will never be cleared
	**	and the second shot flag tells the system that normal rearm times apply -- this is
	**	what is desired for non two shooters.
	*/
	IsSecondShot = !Class->Is_Two_Shooter();
	Strength = Class->MaxStrength;

	/*
	** Keep count of the number of units created.
	*/
//	if (Session.Type == GAME_INTERNET) {
//		House->UnitTotals->Increment_Unit_Total((int)classid);
//	}
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * UnitClass::Debug_Dump -- Displays the status of the unit to the mono monitor.               *
 *                                                                                             *
 *    This displays the current status of the unit class to the mono monitor. By this display  *
 *    bugs may be tracked down or prevented.                                                   *
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
void UnitClass::Debug_Dump(MonoClass * mono) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	mono->Set_Cursor(0, 0);
	mono->Print(Text_String(TXT_DEBUG_VEHICLE));
	mono->Set_Cursor(47, 5);mono->Printf("%02X:%02X", SecondaryFacing.Current(), SecondaryFacing.Desired());

	mono->Set_Cursor(1, 11);mono->Printf("%03", Gems);
	mono->Set_Cursor(7, 11);mono->Printf("%03", Gold);

	mono->Fill_Attrib(66, 13, 12, 1, IsDumping ? MonoClass::INVERSE : MonoClass::NORMAL);

	DriveClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * UnitClass::Sort_Y -- Give Y coordinate sort value for unit.                                 *
 *                                                                                             *
 *    This routine is used by the rendering system in order to sort the                        *
 *    game objects in a back to front order. This is now the correct                           *
 *    overlap effect is achieved.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a coordinate value that can be used for sorting.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE UnitClass::Sort_Y(void) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	return(Coord_Add(Coord, 0x00800000L));
}


/***********************************************************************************************
 * UnitClass::AI -- AI processing for the unit.                                                *
 *                                                                                             *
 *    This routine will perform the AI processing necessary for the unit. These are non-       *
 *    graphic related operations.                                                              *
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
void UnitClass::AI(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);
	/*
	**	Act on new orders if the unit is at a good position to do so.
	*/
	if (Height == 0 && !IsDumping && !IsDriving && Is_Door_Closed() /*&& Mission != MISSION_UNLOAD*/) {
//		if (MissionQueue == MISSION_NONE) Enter_Idle_Mode();
		Commence();
	}
	DriveClass::AI();
	if (!IsActive || Height > 0) {
		return;
	}

	/*
	**	Hack check to ensure that a harvester won't harvest if it is not harvesting.
	*/
	if (Mission != MISSION_HARVEST) {
		IsHarvesting = false;
	}

	/*
	**	Handle combat logic for this unit. It will determine if it has a target and
	**	if so, if conditions are favorable for firing. When conditions permit, the
	**	unit will fire upon its target.
	*/
	Firing_AI();
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (!IsActive) {
		return;
	}
#endif
	/*
	**	Turret rotation processing. Handles rotating radar dish
	**	as well as conventional turrets if present. If no turret present, but
	**	it decides that the body should face its target, then body rotation
	**	would occur by this process as well.
	*/
	Rotation_AI();

	/*
	**	Delete this unit if it finds itself off the edge of the map and it is in
	**	guard or other static mission mode.
	*/
	if (Edge_Of_World_AI()) {
		return;
	}

	/*
	**	Units will reload every so often if they are under the burden of
	**	being required to reload between shots.
	*/
	Reload_AI();

	/*
	**	Transporters require special logic handled here since there isn't a MISSION_WAIT_FOR_PASSENGERS
	**	mission that they can follow. Passenger loading is merely a part of their normal operation.
	*/
	if (Class->Max_Passengers() > 0) {

		/*
		**	Double check that there is a passenger that is trying to load or unload.
		**	If not, then close the door.
		*/
		if (!Is_Door_Closed() && Mission != MISSION_UNLOAD && Transmit_Message(RADIO_TRYING_TO_LOAD) != RADIO_ROGER) {
			APC_Close_Door();
		}
	}

	/*
	**	Don't start a new mission unless the vehicle is in the center of
	**	a cell (not driving) and the door (if any) is closed.
	*/
	if (!IsDumping && !IsDriving && Is_Door_Closed()/*&& Mission != MISSION_UNLOAD*/) {
		Commence();
	}

	/*
	**	A cloaked object that is carrying the flag will always shimmer.
	*/
	if (Cloak == CLOAKED && Flagged != HOUSE_NONE) {
		Do_Shimmer();
	}

	/*
	**	Mobile gap generators regenerate their gap every so often (just in case).
	*/
	if (Class->IsGapper && !IsDriving && (Frame % TICKS_PER_SECOND) == 0) {
		Shroud_Regen();
	}
}


/***********************************************************************************************
 * UnitClass::Rotation_AI -- Process any turret or body rotation.                              *
 *                                                                                             *
 *    This routine will handle the rotation logic for the unit's turret (if it has one) as     *
 *    well as its normal body shape.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Rotation_AI(void)
{
	if (Target_Legal(TarCom) && !IsRotating) {
		DirType dir = Direction(TarCom);

		if (Class->IsTurretEquipped) {
			SecondaryFacing.Set_Desired(dir);
		} else {

			/*
			**	Non turret equipped vehicles will rotate their body to face the target only
			**	if the vehicle isn't currently moving or facing the correct direction. This
			**	applies only to tracked vehicles. Wheeled vehicles never rotate to face the
			**	target, since they aren't maneuverable enough.
			*/
			if ((Class->Speed == SPEED_TRACK /* || *this == UNIT_BIKE */ ) && !Target_Legal(NavCom) && !IsDriving && PrimaryFacing.Difference(dir)) {
				PrimaryFacing.Set_Desired(dir);
			}
		}
	}

	if (Class->IsRadarEquipped) {
		Mark(MARK_CHANGE_REDRAW);
		SecondaryFacing.Set((DirType)(SecondaryFacing.Current() + 8));
		Mark(MARK_CHANGE_REDRAW);
	} else {

		IsRotating = false;
		if (Class->IsTurretEquipped) {
			if (IsTurretLockedDown) {
				SecondaryFacing.Set_Desired(PrimaryFacing.Current());
			}

			if (SecondaryFacing.Is_Rotating()) {
				Mark(MARK_CHANGE_REDRAW);
				if (SecondaryFacing.Rotation_Adjust(Class->ROT+1)) {
					Mark(MARK_CHANGE_REDRAW);
				}

				/*
				**	If no further rotation is necessary, flag that the rotation
				**	has stopped.
				*/
				if (!Class->IsRadarEquipped) {
					IsRotating = SecondaryFacing.Is_Rotating();
				}
			} else {
				if (!IsTurretLockedDown && !Target_Legal(TarCom)) {
					if (!Target_Legal(NavCom)) {
						SecondaryFacing.Set_Desired(PrimaryFacing.Current());
					} else {
						SecondaryFacing.Set_Desired(Direction(NavCom));
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * UnitClass::Edge_Of_World_AI -- Check for falling off the edge of the world.                 *
 *                                                                                             *
 *    When a unit leaves the map it will be eliminated. This routine checks for this case      *
 *    and eliminates the unit accordingly.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the unit eliminated by this routine?                                     *
 *                                                                                             *
 * WARNINGS:   Be sure to check for the return value and if 'true' abort any further processing*
 *             of the unit since it is dead. Only call this routine once per unit per          *
 *             game logic loop.                                                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Edge_Of_World_AI(void)
{
	if (Mission == MISSION_GUARD && !Map.In_Radar(Coord_Cell(Coord)) && IsLocked) {
		if (Team.Is_Valid()) Team->IsLeaveMap = true;
		Stun();
		delete this;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * UnitClass::Reload_AI -- Perform reload logic for this unit.                                 *
 *                                                                                             *
 *    Some units require special reload logic. The V2 rocket launcher in particular. Perform   *
 *    this reload logic with this routine.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per unit per game logic loop.                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Reload_AI(void)
{
	if (*this == UNIT_V2_LAUNCHER && Ammo < Class->MaxAmmo) {
		if (IsDriving) {
			Reload = Reload + 1;
		} else {
			if (Reload == 0) {
				Ammo++;
				if (Ammo < Class->MaxAmmo) {
					Reload = TICKS_PER_SECOND*30;
				}
				Mark(MARK_CHANGE);
			}
		}
	}
}


/***********************************************************************************************
 * UnitClass::Firing_AI -- Handle firing logic for this unit.                                  *
 *                                                                                             *
 *    This routine wil check for and perform any firing logic required of this unit.           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This should be called only once per unit per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Firing_AI(void)
{
	if (Target_Legal(TarCom) && Class->PrimaryWeapon != NULL) {

		/*
		**	Determine which weapon can fire. First check for the primary weapon. If that weapon
		**	cannot fire, then check any secondary weapon. If neither weapon can fire, then the
		**	failure code returned is that from the primary weapon.
		*/
		int primary = What_Weapon_Should_I_Use(TarCom);
		FireErrorType ok = Can_Fire(TarCom, primary);
		switch (ok) {
			case FIRE_OK:
				if (!((UnitClass *)this)->Class->IsFireAnim) {
					Mark(MARK_OVERLAP_UP);
					IsFiring = false;
					Mark(MARK_OVERLAP_DOWN);
				}

				Fire_At(TarCom, primary);
				break;

			case FIRE_FACING:
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				if (Class->IsLockTurret || Class->Type == UNIT_DEMOTRUCK) {
#else
				if (Class->IsLockTurret) {
#endif
					if (!Target_Legal(NavCom) && !IsDriving) {
						PrimaryFacing.Set_Desired(Direction(TarCom));
						SecondaryFacing.Set_Desired(PrimaryFacing.Desired());
					}
				} else {
					SecondaryFacing.Set_Desired(Direction(TarCom));
				}
				break;

			case FIRE_CLOAKED:
				Mark(MARK_OVERLAP_UP);
				IsFiring = false;
				Mark(MARK_OVERLAP_DOWN);
				Do_Uncloak();
				break;
		}
	}
}


/***********************************************************************************************
 * UnitClass::Receive_Message -- Handles receiving a radio message.                            *
 *                                                                                             *
 *    This is the handler function for when a unit receives a radio                            *
 *    message. Typical use of this is when a unit unloads from a hover                         *
 *    class so that clearing of the transport is successful.                                   *
 *                                                                                             *
 * INPUT:   from     -- Pointer to the originator of the message.                              *
 *                                                                                             *
 *          message  -- The radio message received.                                            *
 *                                                                                             *
 *          param    -- Reference to an optional parameter the might be needed to return       *
 *                      information back to the originator of the message.                     *
 *                                                                                             *
 * OUTPUT:  Returns with the radio message response.                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
RadioMessageType UnitClass::Receive_Message(RadioClass * from, RadioMessageType message, long & param)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	switch (message) {
		/*
		**	Checks to see if this object is in need of service depot processing.
		*/
		case RADIO_NEED_REPAIR:
			if (!IsDriving && !Target_Legal(NavCom) && (Health_Ratio() >= 1 && (*this != UNIT_MINELAYER || Ammo >= Class->MaxAmmo))) return(RADIO_NEGATIVE);
			break;
//			return(RADIO_ROGER);

		/*
		**	Asks if the passenger can load on this transport.
		*/
		case RADIO_CAN_LOAD:
			if (Class->Max_Passengers() == 0 || from == NULL || !House->Is_Ally(from->Owner())) return(RADIO_STATIC);
			if (How_Many() < Class->Max_Passengers()) {
				return(RADIO_ROGER);
			}
			return(RADIO_NEGATIVE);

		/*
		**	The refinery has told this harvester that it should begin the backup procedure
		**	so that proper unloading may take place.
		*/
		case RADIO_BACKUP_NOW:
			DriveClass::Receive_Message(from, message, param);
			if (!IsRotating && PrimaryFacing != DIR_W) {
				Do_Turn(DIR_W);
			} else {
				if (!IsDriving) {
					TechnoClass	* whom = Contact_With_Whom();
					if (IsTethered && whom != NULL) {
						if (whom->What_Am_I() == RTTI_BUILDING && Mission == MISSION_ENTER) {
							if (Transmit_Message(RADIO_IM_IN, whom) == RADIO_ROGER) {
								Transmit_Message(RADIO_UNLOADED, whom);
							}
						}
					}
				}
			}
			return(RADIO_ROGER);

		/*
		**	This message is sent by the passenger when it determines that it has
		**	entered the transport.
		*/
		case RADIO_IM_IN:
			if (How_Many() == Class->Max_Passengers()) {
				APC_Close_Door();
			}
			return(RADIO_ATTACH);

		/*
		**	Docking maintenance message received. Check to see if new orders should be given
		**	to the impatient unit.
		*/
		case RADIO_DOCKING:

			/*
			**	If this transport is moving, then always abort the docking request.
			*/
			if (IsDriving || Target_Legal(NavCom)) {
				return(RADIO_NEGATIVE);
			}

			/*
			**	Check for the case of a docking message arriving from a unit that does not
			**	have formal radio contact established. This might be a unit that is standing
			**	by. If this transport is free to proceed with normal docking operation, then
			**	establish formal contact now. If the transport is completely full, then break
			**	off contact. In all other cases, just tell the pending unit to stand by.
			*/
			if (Contact_With_Whom() != from) {

				/*
				**	Can't ever load up so tell the passenger to bug off.
				*/
				if (How_Many() >= Class->Max_Passengers()) {
					return(RADIO_NEGATIVE);
				}

				/*
				**	Establish contact and let the loading process proceed normally.
				*/
				if (!In_Radio_Contact()) {
					Transmit_Message(RADIO_HELLO, from);
				} else {

					/*
					**	This causes the potential passenger to think that all is ok and to
					**	hold on for a bit.
					*/
					return(RADIO_ROGER);
				}
			}

			if (Class->Max_Passengers() > 0 && How_Many() < Class->Max_Passengers()) {
				DriveClass::Receive_Message(from, message, param);

				if (!IsDriving && !IsRotating && !IsTethered) {

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
							Do_Turn(dir);

							/*
							**	If it is now facing the correct direction, then open the
							**	transport doors. Close the doors if the transport is or needs
							**	to rotate.
							*/
#ifdef FIXIT_PHASETRANSPORT	//	checked - ajw 9/28/98
							if (*this == UNIT_APC || *this == UNIT_PHASE) {
#else
							if (*this == UNIT_APC) {
#endif
								if (IsRotating) {
									if (!Is_Door_Closed()) {
										APC_Close_Door();
									}
								} else {
									if (!Is_Door_Open()) {
										APC_Open_Door();
									}
								}
							}

							/*
							**	Tell the potential passenger where it should go. If the passenger is
							**	already at the staging location, then tell it to move onto the transport
							**	directly.
							*/
							if (Transmit_Message(RADIO_MOVE_HERE, param, from) == RADIO_YEA_NOW_WHAT) {
#ifdef FIXIT_PHASETRANSPORT	//	checked - ajw 9/28/98
								if ( (*this != UNIT_APC && *this != UNIT_PHASE) || Is_Door_Open()) {
#else
								if (*this != UNIT_APC || Is_Door_Open()) {
#endif
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
				}
				return(RADIO_ROGER);
			}
			break;

		/*
		**	When this message is received, it means that the other object
		**	has already turned its radio off. Turn this radio off as well.
		*/
		case RADIO_OVER_OUT:
			if (Mission == MISSION_RETURN) {
				Assign_Mission(MISSION_GUARD);
			}
			DriveClass::Receive_Message(from, message, param);
			return(RADIO_ROGER);

	}
	return(DriveClass::Receive_Message(from, message, param));
}


/***********************************************************************************************
 * UnitClass::Unlimbo -- Removes unit from stasis.                                             *
 *                                                                                             *
 *    This routine will place a unit into the game and out of its limbo                        *
 *    state. This occurs whenever a unit is unloaded from a transport.                         *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate to make the unit appear.                                *
 *                                                                                             *
 *          dir      -- The initial facing to impart upon the unit.                            *
 *                                                                                             *
 * OUTPUT:  bool; Was the unit unlimboed successfully?  If the desired                         *
 *                coordinate is illegal, then this might very well return                      *
 *                false.                                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Unlimbo(COORDINATE coord, DirType dir)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	/*
	**	All units must start out facing one of the 8 major directions.
	*/
	dir = Facing_Dir(Dir_Facing(dir));
	if (DriveClass::Unlimbo(coord, dir)) {

		SecondaryFacing = dir;
		/*
		**	Ensure that the owning house knows about the
		**	new object.
		*/
		House->UScan |= (1L << Class->Type);
		House->ActiveUScan |= (1L << Class->Type);

		/*
		**	If it starts off the edge of the map, then it already starts cloaked.
		*/
		if (IsCloakable && !IsLocked) Cloak = CLOAKED;

		/*
		**	Units default to no special animation.
		*/
		Set_Rate(0);
		Set_Stage(0);

		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * UnitClass::Take_Damage -- Inflicts damage points on a unit.                                 *
 *                                                                                             *
 *    This routine will inflict the specified number of damage points on                       *
 *    the given unit. If the unit is destroyed, then this routine will                         *
 *    remove the unit cleanly from the game. The return value indicates                        *
 *    whether the unit was destroyed. This will allow appropriate death                        *
 *    animation or whatever.                                                                   *
 *                                                                                             *
 * INPUT:   damage-- The number of damage points to inflict.                                   *
 *                                                                                             *
 *          distance -- The distance from the damage center point to the object's center point.*
 *                                                                                             *
 *          warhead--The type of damage to inflict.                                            *
 *                                                                                             *
 *          source   -- Who is responsible for this damage?                                    *
 *                                                                                             *
 * OUTPUT:  Returns the result of the damage process. This can range from RESULT_NONE up to    *
 *          RESULT_DESTROYED.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/30/1991 JLB : Created.                                                                 *
 *   07/12/1991 JLB : Script initiated by unit destruction.                                    *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *   04/16/1994 JLB : Warhead modifier.                                                        *
 *   06/03/1994 JLB : Added the source of the damage target value.                             *
 *   06/20/1994 JLB : Source is a base class pointer.                                          *
 *   11/22/1994 JLB : Shares base damage handler for techno objects.                           *
 *   06/30/1995 JLB : Lasers do maximum damage against gunboat.                                *
 *   08/16/1995 JLB : Harvester crushing doesn't occur on early missions.                      *
 *=============================================================================================*/
ResultType UnitClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source, bool forced)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	ResultType res = RESULT_NONE;

	/*
	**	Remember if this object was selected. If it was and it gets destroyed and it has
	**	passengers that pop out, then the passengers will inherit the select state.
	*/
	//bool select = (IsSelected && House->IsPlayerControl);
	bool select = (Is_Selected_By_Player() );//&& House->IsPlayerControl);

	/*
	**	In order for a this to be damaged, it must either be a unit
	**	with a crew or a sandworm.
	*/
	res = DriveClass::Take_Damage(damage, distance, warhead, source, forced);

	if (res == RESULT_DESTROYED) {
		Death_Announcement(source);
		Shroud_Regen();	// remove the shroud if it's a gap generator
		if (Class->Explosion != ANIM_NONE) {
			AnimType anim = Class->Explosion;

			/*
			**	SSM launchers will really explode big if they are carrying
			**	missiles at the time of the explosion.
			*/
			if (*this == UNIT_V2_LAUNCHER && Ammo) {
				anim = ANIM_NAPALM3;
			}

			new AnimClass(anim, Coord);

			/*
			**	Harvesters explode with a force equal to the amount of
			**	Tiberium they are carrying.
			*/
			if (Tiberium > 0 && Rule.IsExplosiveHarvester) {
				Wide_Area_Damage(Coord, CELL_LEPTON_W + CELL_LEPTON_W/2, Credit_Load()+Class->MaxStrength, this, WARHEAD_HE);
			}

			/*
			**	Very strong units that have an explosion will also rock the
			**	screen when they are destroyed.
			*/
			if (Class->MaxStrength > 400) {
				Shake_The_Screen(3, Owner());
				if (source && Owner() != source->Owner()) {
					Shake_The_Screen(3, source->Owner());
				}
			}
		}

		/*
		**	Possibly have the crew member run away.
		*/
		CELL cell = Coord_Cell(Center_Coord());
		Mark(MARK_UP);

		if (Class->IsCrew && Class->Max_Passengers() == 0) {
			if (Percent_Chance(50)) {
				InfantryClass * i = 0;

				if (Class->PrimaryWeapon == NULL) {
					i = new InfantryClass(INFANTRY_C1, House->Class->House);
					if (i != NULL) i->IsTechnician = true;
				} else {
					i = new InfantryClass(INFANTRY_E1, House->Class->House);
				}
				if (i != NULL) {
					if (i->Unlimbo(Coord, DIR_N)) {
						i->Strength = Random_Pick(5, (int)i->Class->MaxStrength/2);
						i->Scatter(0, true);
						if (!House->IsHuman) {
							i->Assign_Mission(MISSION_HUNT);
						} else {
							i->Assign_Mission(MISSION_GUARD);
						}
						if (select) i->Select();
					} else {
						delete i;
					}
				}
			}
		} else {
			while (Is_Something_Attached()) {
				FootClass * object = Detach_Object();

				if (object == NULL) break;		// How can this happen?

				/*
				**	Only infantry can run from a destroyed vehicle. Even then, it is not a sure
				**	thing.
				*/
				if (object->Is_Infantry() && object->Unlimbo(Coord, DIR_N)) {
					object->Look(false);
					object->Scatter(0, true);
					if (select) object->Select();
				} else {
					object->Record_The_Kill(source);
					delete object;
				}
			}
		}

		/*
		**	If this is a truck, there is a possibility that a crate will drop out
		**	if the scenario so indicates and there is room.
		*/
		if (Scen.IsTruckCrate && *this == UNIT_TRUCK) {
			cell = Nearby_Location();
			if (cell != 0) {
				new OverlayClass(OVERLAY_WOOD_CRATE, cell);
			}
		}

		if (*this == UNIT_MCV) {
			if (House) {
				House->Check_Pertinent_Structures();
			}
		}

		/*
		**	Finally, delete the vehicle.
		*/
		delete this;

	} else {

		/*
		**	When damaged and below half strength, start smoking if
		**	it isn't already smoking.
		*/
		if (Health_Ratio() <= Rule.ConditionYellow && !IsAnimAttached) {
#ifdef FIXIT_ANTS
			if (*this != UNIT_ANT1 && *this != UNIT_ANT2 && *this != UNIT_ANT3)  {
#endif
				AnimClass * anim = new AnimClass(ANIM_SMOKE_M, Coord_Add(Coord, XYP_Coord(0, -8)));
				if (anim) anim->Attach_To(this);
#ifdef FIXIT_ANTS
			}
#endif
		   }

		/*
		**	Try to crush anyone that fires on this unit if possible. The harvester
		**	typically is the only one that will qualify here.
		*/
		if (!Team.Is_Valid() && source != NULL && !IsTethered && !House->Is_Ally(source) && (!House->IsHuman || Rule.IsAutoCrush)) {

			/*
			**	Try to crush the attacker if it can be crushed by this unit and this unit is
			**	not equipped with a flame type weapon. If this unit has a weapon and the target
			**	is not very close, then fire on it instead. In easy mode, they never run over the
			**	player. In hard mode, they always do. In normal mode, they only overrun past
			**	mission #8.
			*/
			if (Should_Crush_It(source)) {
				Assign_Destination(source->As_Target());
				Assign_Mission(MISSION_MOVE);
			} else {

				/*
				**	Try to return to base if possible.
				*/
				if (*this == UNIT_HARVESTER && Pip_Count() && Health_Ratio() <= Rule.ConditionYellow) {

					/*
					**	Find nearby refinery and head to it?
					*/
					BuildingClass * building = Find_Docking_Bay(STRUCT_REFINERY, false);

					/*
					**	Since the refinery said it was ok to load, establish radio
					**	contact with the refinery and then await docking orders.
					*/
					if (building != NULL && Transmit_Message(RADIO_HELLO, building) == RADIO_ROGER) {
						Assign_Mission(MISSION_ENTER);
					}
				}
			}
		}

		/*
		**	Computer controlled harvester will radio for help if they are attacked.
		*/
		if (*this == UNIT_HARVESTER && !House->IsHuman && source) {
			Base_Is_Attacked(source);
		}
	}
	return(res);
}


/***********************************************************************************************
 * UnitClass::Active_Click_With -- Intercepts the active click to see if deployment is possible*
 *                                                                                             *
 *    This routine intercepts the active click operation. It check to see if this is a self    *
 *    deployment request (MCV's have this ability). If it is, then the object is initiated     *
 *    to self deploy. In the other cases, it passes the operation down to the lower            *
 *    classes for processing.                                                                  *
 *                                                                                             *
 * INPUT:   action   -- The action requested of the unit.                                      *
 *                                                                                             *
 *          object   -- The object that the mouse pointer is over.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Active_Click_With(ActionType action, ObjectClass * object)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	if (action != What_Action(object)) {
		action = What_Action(object);
		switch (action) {
			case ACTION_SABOTAGE:
			case ACTION_CAPTURE:
				action = ACTION_ATTACK;
				break;

			case ACTION_ENTER:
				action = ACTION_MOVE;
				break;

			default:
				break;
		}
	}

	/*
	**	Short circuit out if trying to tell a unit to "nomove" to itself. This bypass of the
	**	normal active click with logic prevents any disturbance to the vehicle's state. Without
	**	this bypass, a unit on a repair bay would stop repairing because it would break radio
	**	contact.
	*/
	if (object == this && action == ACTION_NOMOVE) {
		return;
	}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (*this == UNIT_MAD && (IsDumping || Gold)) {
	} else {
		DriveClass::Active_Click_With(action, object);
	}
#else
	DriveClass::Active_Click_With(action, object);
#endif
}


/***********************************************************************************************
 * UnitClass::Active_Click_With -- Performs specified action on specified cell.                *
 *                                                                                             *
 *    This routine is called when the mouse has been clicked over a cell and this unit must    *
 *    now respond. Notice that this is merely a placeholder function that exists because there *
 *    is another function of the same name that needs to be overloaded. C++ has scoping        *
 *    restrictions when there are two identically named functions that are overridden in       *
 *    different classes -- it handles it badly, hence the existence of this routine.           *
 *                                                                                             *
 * INPUT:   action   -- The action to perform on the cell specified.                           *
 *                                                                                             *
 *          cell     -- The cell that the action is to be performed on.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Active_Click_With(ActionType action, CELL cell)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (*this == UNIT_MAD && (IsDumping || Gold)) {
	} else {
		DriveClass::Active_Click_With(action, cell);
	}
#else
	DriveClass::Active_Click_With(action, cell);
#endif
}


/***********************************************************************************************
 * UnitClass::Enter_Idle_Mode -- Unit enters idle mode state.                                  *
 *                                                                                             *
 *    This routine is called when the unit completes one mission but does not have a clear     *
 *    follow up mission to perform. In such a case, the unit should enter a default idle       *
 *    state. This idle state varies depending on what the current internal computer            *
 *    settings of the unit is as well as what kind of unit it is.                              *
 *                                                                                             *
 * INPUT:   initial  -- Is this called when the unit just leaves a factory or is initially     *
 *                      or is initially placed on the map?                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   06/03/1994 JLB : Fixed to handle non-combat vehicles.                                     *
 *   06/18/1995 JLB : Allows a harvester to stop harvesting.                                   *
 *=============================================================================================*/
void UnitClass::Enter_Idle_Mode(bool initial)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	MissionType	order = MISSION_GUARD;

	if (IsToScatter) {
		IsToScatter = false;
		Scatter(0, true);
	}

	/*
	**	A movement mission without a NavCom would be pointless to have a radio contact since
	**	no radio coordination occurs on a just a simple movement mission.
	*/
	if (Mission == MISSION_MOVE && !Target_Legal(NavCom)) {
		Transmit_Message(RADIO_OVER_OUT);
	}

	Handle_Navigation_List();
	if (Target_Legal(NavCom)) {
		order = MISSION_MOVE;
	} else {

		if (!Is_Weapon_Equipped()) {
			if (Class->IsToHarvest) {
				if (!In_Radio_Contact() && Mission != MISSION_HARVEST && MissionQueue != MISSION_HARVEST) {
					if (initial || !House->IsHuman || Map[Coord].Land_Type() == LAND_TIBERIUM) {
						order = MISSION_HARVEST;
					} else {
						order = MISSION_GUARD;
					}
					Assign_Target(TARGET_NONE);
					Assign_Destination(TARGET_NONE);
				} else {
					return;
				}
			} else {
				if (IsALoaner && Class->Max_Passengers() > 0 && Is_Something_Attached() && !Team.Is_Valid()) {
					order = MISSION_UNLOAD;
				} else {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if(*this == UNIT_MAD && Mission == MISSION_UNLOAD) {
		order = MISSION_UNLOAD;
	} else {
#endif
					order = MISSION_GUARD;
					Assign_Target(TARGET_NONE);
					Assign_Destination(TARGET_NONE);
				}
			}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	}
#endif
		} else {

			if (Mission == MISSION_GUARD || Mission == MISSION_GUARD_AREA || MissionControl[Mission].IsParalyzed || MissionControl[Mission].IsZombie) {
				return;
			}

			if (House->IQ < Rule.IQGuardArea || Team.Is_Valid()) {
				order = MISSION_GUARD;
			} else {
				order = MISSION_GUARD_AREA;
			}
		}
	}
	Assign_Mission(order);
}


/***********************************************************************************************
 * UnitClass::Goto_Clear_Spot -- Finds a clear spot to deploy.                                 *
 *                                                                                             *
 *    This routine is used by the MCV to find a clear spot to deploy. If a clear spot          *
 *    is found, then the MCV will assign that location to its navigation computer. This only   *
 *    occurs if the MCV isn't already heading toward a spot.                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool;  Is the located at a spot where it can deploy?                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Goto_Clear_Spot(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	Mark(MARK_UP);
	if (!Target_Legal(NavCom) && BuildingTypeClass::As_Reference(STRUCT_CONST).Legal_Placement(Adjacent_Cell(Coord_Cell(Center_Coord()), FACING_NW))) {
		Mark(MARK_DOWN);
		return(true);
	}

	if (!Target_Legal(NavCom)) {
		/*
		**	This scan table is skewed to north scanning only. This should
		**	probably be converted to a more flexible method.
		*/
		static int _offsets[] = {
			-MAP_CELL_W*1,
			-MAP_CELL_W*2,
			-(MAP_CELL_W*2)+1,
			-(MAP_CELL_W*2)-1,
			-MAP_CELL_W*3,
			-(MAP_CELL_W*3)+1,
			-(MAP_CELL_W*3)-1,
			-(MAP_CELL_W*3)+2,
			-(MAP_CELL_W*3)-2,
			-MAP_CELL_W*4,
			-(MAP_CELL_W*4)+1,
			-(MAP_CELL_W*4)-1,
			-(MAP_CELL_W*4)+2,
			-(MAP_CELL_W*4)-2,
//BG: Added south scanning
			MAP_CELL_W*1,
			MAP_CELL_W*2,
			(MAP_CELL_W*2)+1,
			(MAP_CELL_W*2)-1,
			MAP_CELL_W*3,
			(MAP_CELL_W*3)+1,
			(MAP_CELL_W*3)-1,
			(MAP_CELL_W*3)+2,
			(MAP_CELL_W*3)-2,
			MAP_CELL_W*4,
			(MAP_CELL_W*4)+1,
			(MAP_CELL_W*4)-1,
			(MAP_CELL_W*4)+2,
			(MAP_CELL_W*4)-2,

//BG: Added some token east/west scanning
			-1,-2,-3,-4,

			 1, 2, 3, 4,
			0
		};
		int * ptr;

		ptr = &_offsets[0];
		while (*ptr) {
			CELL	cell = Coord_Cell(Coord)+*ptr++;
			CELL	check_cell = Adjacent_Cell(cell, FACING_NW);
			if (BuildingTypeClass::As_Reference(STRUCT_CONST).Legal_Placement(check_cell)) {
				Assign_Destination(::As_Target(cell));
				break;
			}
		}
	}
	Mark(MARK_DOWN);

	/*
	** If we couldn't find a destination to go to, let's try random movement
	** to see if that brings us to a better spot.
	*/
	if(!Target_Legal(NavCom) && !House->IsHuman) {
		Scatter(0);
	}

	return(false);
}


/***********************************************************************************************
 * UnitClass::Try_To_Deploy -- The unit attempts to "deploy" at current location.              *
 *                                                                                             *
 *    Certain units have the ability to deploy into a building. When this routine is called    *
 *    for one of those units, it will attempt to deploy at its current location. If the unit   *
 *    is in motion to a destination or it isn't one of the special units that can deploy or    *
 *    it isn't allowed to deploy at this location for some reason it won't deploy. In all      *
 *    other cases, it will begin to deploy and once it begins only a player abort action will  *
 *    stop it.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was deployment begun?                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Try_To_Deploy(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	if (!Target_Legal(NavCom) && !IsRotating) {
		if (*this == UNIT_MCV) {

			/*
			**	Determine if it is legal to deploy at this location. If not, tell the
			**	player.
			*/
			Mark(MARK_UP);
			CELL cell = Coord_Cell(Adjacent_Cell(Center_Coord(), FACING_NW));
			if (!BuildingTypeClass::As_Reference(STRUCT_CONST).Legal_Placement(cell)) {
				if (PlayerPtr == House) {
					Speak(VOX_DEPLOY);
				}
				if (!House->IsHuman) {
					BuildingTypeClass::As_Reference(STRUCT_CONST).Flush_For_Placement(cell, House);
				}
				Mark(MARK_DOWN);
				IsDeploying = false;
				return(false);
			}
			Mark(MARK_DOWN);

			/*
			**	If the unit is not facing the correct direction, then start it rotating
			**	toward the right facing, but still flag it as if it had deployed. This is
			**	because it will deploy as soon as it reaches the correct facing.
			*/
			if (PrimaryFacing.Current() != DIR_SW) {
				Do_Turn(DIR_SW);
//				PrimaryFacing.Set_Desired(DIR_SW);
				IsDeploying = true;
				return(true);
			}

			/*
			**	Since the unit is already facing the correct direction, actually do the
			**	deploy logic. If for some reason this cannot occur, then don't delete the
			**	unit, just mark it as not deploying.
			*/
			Mark(MARK_UP);
			BuildingClass * building = new BuildingClass(STRUCT_CONST, House->Class->House);
			if (building != NULL) {
				if (building->Unlimbo(Adjacent_Cell(Coord, FACING_NW))) {

					/*
					**	Play the buildup sound for the player if this is the players
					**	MCV.
					*/
					if (building->House == PlayerPtr) {
						Sound_Effect(VOC_PLACE_BUILDING_DOWN, Center_Coord());
					} else {
						building->IsToRebuild = true;
						building->IsToRepair = true;
					}

					/*
					**	Always reveal the construction yard to the player that owned the
					**	mobile construction vehicle.
					*/
					building->Revealed(House);

					/*
					**	When the MCV deploys, always consider production to have started
					**	for the owning house. This ensures that in multiplay, computer
					**	opponents will begin construction as soon as they start their
					**	base.
					*/
					House->IsStarted = true;

					/*
					**	Force the newly placed construction yard to be in the same strength
					**	ratio as the MCV that deployed into it.
					*/
					building->Strength = Health_Ratio() * (int)building->Class->MaxStrength;

					/*
					** Force the MCV to drop any flag it was carrying.  This will also set
					** the owner house's flag home cell (since the house's FlagHome is
					** presumably 0 at this point).
					*/
					Stun();

					/*
					** If this MCV was teleported here, clear the gray flag so
					** the screen will go back to color.
					*/
					if (IsMoebius && !Scen.IsFadingColor) {
						Scen.IsFadingBW = false;
						Scen.IsFadingColor = true;
						Scen.FadeTimer = GRAYFADETIME;
					}
					delete this;
					return(true);
				} else {

					/*
					**	Could not deploy the construction yard at this location! Just revert
					**	back to normal "just sitting there" mode and await further instructions.
					*/
					delete building;
				}
			}
			Mark(MARK_DOWN);
			IsDeploying = false;
		}
	}
	return(false);
}


/***********************************************************************************************
 * UnitClass::Per_Cell_Process -- Performs operations necessary on a per cell basis.           *
 *                                                                                             *
 *    This routine will perform the operations necessary that occur when a unit is at the      *
 *    center of a cell. These operations could entail deploying into a construction yard,      *
 *    radioing a transport unit, and looking around for the enemy.                             *
 *                                                                                             *
 * INPUT:   why   -- Specifies the circumstances under which this routine was called.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/18/1994 JLB : Created.                                                                 *
 *   06/17/1995 JLB : Handles case when building says "NO!"                                    *
 *   06/30/1995 JLB : Gunboats head back and forth now.                                        *
 *=============================================================================================*/
void UnitClass::Per_Cell_Process(PCPType why)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	CELL	cell = Coord_Cell(Coord);
	HousesType house;

	if (why == PCP_END || why == PCP_ROTATION) {
		/*
		**	Check to see if this is merely the end of a rotation for the MCV as it is
		**	preparing to deploy. In this case, it should begin its deploy process.
		*/
		if (IsDeploying) {
			Try_To_Deploy();
			if (!IsActive) return;			// Unit no longer exists -- bail.
		}
	}

	BStart(BENCH_PCP);
	if (why == PCP_END) {
		/*
		**	If this is a unit that is driving onto a building then the unit must enter
		**	the building as the final step.
		*/
		TechnoClass	* whom = Contact_With_Whom();
		if (IsTethered && whom != NULL) {
			if (whom->What_Am_I() == RTTI_BUILDING && Mission == MISSION_ENTER) {
				if (whom == Map[CELL(cell-MAP_CELL_W)].Cell_Building()) {
					switch (Transmit_Message(RADIO_IM_IN, whom)) {
						case RADIO_ROGER:
							break;

						case RADIO_ATTACH:
							break;

						default:
							Scatter(0, true);
							break;
					}
				}
			}
		}

		/*
		**	Unit entering a transport vehicle will break radio contact
		**	and attach itself to the transporter.
		*/
		TechnoClass * techno = Contact_With_Whom();
		if (Mission == MISSION_ENTER && techno && Coord_Cell(Coord) == Coord_Cell(techno->Coord) && techno == As_Techno(NavCom)) {
			if (Transmit_Message(RADIO_IM_IN) == RADIO_ATTACH) {
				Limbo();
				techno->Attach(this);
			}
			BEnd(BENCH_PCP);
			return;
		}

		/*
		**	When breaking away from a transport object or building, possibly
		**	scatter or otherwise begin normal unit operations.
		*/
		if (IsTethered && (Mission != MISSION_ENTER ||
				(As_Techno(NavCom) != NULL && Contact_With_Whom() != As_Techno(NavCom))
				) &&
				Mission != MISSION_UNLOAD) {

			/*
			**	Special hack check to make sure that even though it has moved one
			**	cell, if it is still on the building (e.g., service depot), have
			**	it scatter again.
			*/
			if	(Map[Coord].Cell_Building() != NULL && !Target_Legal(NavCom)) {
				Scatter(0, true, true);
			} else {
				TechnoClass * contact = Contact_With_Whom();
				if (Transmit_Message(RADIO_UNLOADED) == RADIO_RUN_AWAY) {
					if (*this == UNIT_HARVESTER && contact && contact->What_Am_I() == RTTI_BUILDING) {
						Assign_Mission(MISSION_HARVEST);
					} else if (!Target_Legal(NavCom)) {
						Scatter(0, true);
					} else {

						/*
						**	Special case hack to allow automatic transition to loading
						**	onto a transport (or other situation) if the destination
						**	so indicates.
						*/
						TechnoClass * techno = As_Techno(NavCom);
						if (techno != NULL) {
							Transmit_Message(RADIO_DOCKING, techno);
						}
					}
				} else {
					if (*this == UNIT_HARVESTER) {
						if (Target_Legal(ArchiveTarget)) {
							Assign_Mission(MISSION_HARVEST);
							Assign_Destination(ArchiveTarget);
							ArchiveTarget = TARGET_NONE;
						} else {

							/*
							**	Since there is no place to go, move away to clear
							**	the pad for another harvester.
							*/
							if (!Target_Legal(NavCom)) {
								Scatter(0, true);
							}
						}
					}
				}
			}
		}

		/*
		**	If this is a loaner unit and is is off the edge of the
		**	map, then it gets eliminated. That is, unless it is carrying cargo. This means that
		**	it is probably carrying an incoming reinforcement and it should not be eliminated.
		*/
		if (Edge_Of_World_AI()) {
			BEnd(BENCH_PCP);
			return;
		}

		/*
		**	The unit performs looking around at this time. If the
		**	unit moved further than one square during the last track
		**	move, don't do an incremental look. Do a full look around
		**	instead.
		*/
		if (IsPlanningToLook) {
			IsPlanningToLook = false;
			Look(false);
		} else {
			Look(true);
		}

		/*
		** If this is a mobile gap generator, restore the shroud where appropriate
		** and re-shroud around us.
		*/
		if (Class->IsGapper && !House->IsPlayerControl) {
			Shroud_Regen();
		}

		/*
		**	Act on new orders if the unit is at a good position to do so.
		*/
		if (!IsDumping) {
			Commence();
		}

		/*
		**	Certain units require some setup time after they come to a halt.
		*/
		if (!Target_Legal(NavCom) && Path[0] == FACING_NONE) {
			if (Class->IsNoFireWhileMoving) {
				Arm = Rearm_Delay(true)/4;
			}
		}

		/*
		**	If there is a house flag here, then this unit just might pick it up.
		*/
		if (Flagged == HOUSE_NONE) {

			if (Map[cell].IsFlagged && !House->Is_Ally(Map[cell].Owner)) {
				HouseClass::As_Pointer(Map[cell].Owner)->Flag_Attach(this);
			}
		}

		/*
		**	If this is the unit's own flag-home-cell and the unit is carrying
		** a flag, destroy the house of the flag the unit is carrying.
		*/
		if (Flagged != HOUSE_NONE) {

			/*
			**	If this vehicle is carrying your flag, then it will reveal the
			**	map for you as well as itself. This gives you and opportunity to
			**	attack the unit.
			*/
			if (!IsOwnedByPlayer && Flagged == PlayerPtr->Class->House) {
				Map.Sight_From(Coord_Cell(Coord), Class->SightRange, House, true);
			}

			/*
			**	If the flag reaches the home cell for the player, then the flag's
			**	owner will be destroyed.
			*/
			if (cell == HouseClass::As_Pointer(Owner())->FlagHome) {
				house = Flagged; // Flag_Remove will clear 'Flagged', so save it
				HouseClass::As_Pointer(house)->Flag_Remove(As_Target(), true);
				HouseClass::As_Pointer(house)->Flag_To_Die();
			}
		}

		/*
		** If entering a cell with a land mine in it, blow up the mine.
		*/
		BuildingClass * bldng = Map[cell].Cell_Building();
		if (bldng != NULL && (*bldng == STRUCT_AVMINE || *bldng == STRUCT_APMINE) && !bldng->House->Is_Ally(this)) {

			/*
			** Special case: if it's a land mine deployer, and it ran over the
			** type of mine it deploys (only possible if it just dropped it
			** down) then ignore the mine.
			*/
			if (*this != UNIT_MINELAYER || bldng->House != House) {

				COORDINATE blcoord = bldng->Center_Coord();

				new AnimClass(ANIM_MINE_EXP1, blcoord);
//				new AnimClass(Combat_Anim(Rule.AVMineDamage, WARHEAD_HE, Map[cell].Land_Type()), blcoord);

				/*
				** Vehicles blow up both mines, but they only take significant damage from AV mines.
				*/
				if (*bldng == STRUCT_AVMINE) {
					int damage = Rule.AVMineDamage;
					Take_Damage(damage, 0, WARHEAD_HE);
				} else {
					int damage = 10;
					Take_Damage(damage, 0, WARHEAD_HE);
				}
				delete bldng;
				if (!IsActive) {
					BEnd(BENCH_PCP);
					return;
				}
			}
		}

		/*
		**	If after all is said and done, the unit finishes its move on an impassable cell, then
		**	it must presume that it is in the case of a unit driving onto a bridge that blows up
		**	before the unit completes it's move. In such a case the unit should have been destroyed
		**	anyway, so blow it up now.
		*/
		LandType land = Map[Coord].Land_Type();
		if (!IsDriving && IsMovingOntoBridge && (land == LAND_ROCK || land == LAND_WATER || land == LAND_RIVER)) {
			new AnimClass(Combat_Anim(Strength, WARHEAD_AP, land), Coord);
			int damage = Strength;
			Take_Damage(damage, 0, WARHEAD_AP, NULL, true);
			return;
		}
	}

	/*
	**	Destroy any crushable wall that is driven over by a tracked vehicle.
	*/
	CellClass * cellptr = &Map[cell];
	if (Class->IsCrusher && cellptr->Overlay != OVERLAY_NONE) {
//	if (Class->Speed == SPEED_TRACK && cellptr->Overlay != OVERLAY_NONE) {
		OverlayTypeClass const * optr = &OverlayTypeClass::As_Reference(cellptr->Overlay);

		if (optr->IsCrushable) {
			if (optr->Type == OVERLAY_SANDBAG_WALL) {
				Sound_Effect(VOC_SANDBAG, Center_Coord());
			} else {
				Sound_Effect(VOC_WALLKILL2, Center_Coord());
			}
			cellptr->Reduce_Wall(-1);
		}
	}

	/*
	**	Check to see if crushing of any unfortunate infantry is warranted.
	*/
	Overrun_Square(Coord_Cell(Coord), false);

	if (!IsActive) {
		BEnd(BENCH_PCP);
		return;
	}
	DriveClass::Per_Cell_Process(why);
	BEnd(BENCH_PCP);
}


/***********************************************************************************************
 * UnitClass::Shape_Number -- Fetch the shape number to use for this unit.                     *
 *                                                                                             *
 *    This routine will calculate the shape number for this unit. The shape number is used     *
 *    for the body of the unit.                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the shape number to be used for this unit.                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Shape_Number(void) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	int			shapenum;		// Working shape number.
	int			facing = Dir_To_32(PrimaryFacing);
	int			tfacing = Dir_To_32(SecondaryFacing);
	DirType		rotation = DIR_N;

#ifdef FIXIT_ANTS
	/*
	**	This handles the ant case.
	*/
	if (Class->Rotation == 8)  {

		/*
		**	The starting frame is based on the facing of the unit.
		*/
		shapenum = ((UnitClass::BodyShape[facing]+2)/4) & 0x07;

		/*
		**	If the unit is driving, then it has an animation adjustment to the frame number.
		*/
		if (IsDriving)  {
			shapenum = 8 + (shapenum * 8) + ((::Frame+ID)/2) % 8;
		} else {

			/*
			**	If in combat, then do combat anims.
			*/
			if (Arm > 0)  {
				shapenum = 8 + 64 + (shapenum * 4) + ((::Frame+ID)/2) % 4;
			}
		}
	} else {
#endif

	/*
	**	Fetch the harvesting animation stage as appropriate.
	*/
	if (IsHarvesting && !PrimaryFacing.Is_Rotating() && !NavCom && !IsDriving) {
//			static char _hstage[] = {0, 1, 2, 3, 4, 5, 6, 7, 0};
		unsigned stage = Fetch_Stage();
		if (stage >= ARRAY_SIZE(Class->Harvester_Load_List)) stage = ARRAY_SIZE(Class->Harvester_Load_List)-1;
		shapenum = 32 + (((UnitClass::BodyShape[facing]+2)/4)*Class->Harvester_Load_Count)+Class->Harvester_Load_List[stage];
	} else {
		/*
		** If the harvester's dumping a load of ore, show that animation
		*/
		if (IsDumping) {
			unsigned stage = Fetch_Stage();
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			if (*this == UNIT_MAD) {
				if (stage >= 8) {
					stage = 7;
				}
				shapenum = 32 + stage + (UnitClass::BodyShape[facing]/4)*8;
			} else {
				if (stage >= ARRAY_SIZE(Class->Harvester_Dump_List)) stage = ARRAY_SIZE(Class->Harvester_Dump_List)-1;
				shapenum = Class->Harvester_Dump_List[stage]+96;
			}
#else
			if (stage >= ARRAY_SIZE(Class->Harvester_Dump_List)) stage = ARRAY_SIZE(Class->Harvester_Dump_List)-1;
			shapenum = Class->Harvester_Dump_List[stage]+96;
#endif
		} else {
			shapenum = UnitClass::BodyShape[facing];

			if (Class->IsAnimating) {
				shapenum = Fetch_Stage();
			}

			/*
			**	Door opening and closing animation must be handled carefully. There are only
			**	certain directions where this door animation will work.
			*/
			if (!Is_Door_Closed() && (PrimaryFacing == DIR_NW || PrimaryFacing == DIR_NE)) {
				if (PrimaryFacing == DIR_NE) {
					shapenum = 32;
				} else {
					if (PrimaryFacing == DIR_NW) {
						shapenum = 35;
					}
				}
				shapenum += Door_Stage();
			}
		}
	}

#ifdef FIXIT_ANTS
	}
#endif

	/*
	**	The body of the V2 launcher indicates whether it is loaded with a missile
	**	or not.
	*/
	if (*this == UNIT_V2_LAUNCHER) {
		if (Ammo == 0) shapenum += 32;
	}

	return(shapenum);
}


/***********************************************************************************************
 * UnitClass::Draw_It -- Draws a unit object.                                                  *
 *                                                                                             *
 *    This routine is the one that actually draws a unit object. It displays the unit          *
 *    according to its current state flags and centered at the location specified.             *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y coordinate of where to draw the unit.                         *
 *                                                                                             *
 *          window   -- The clipping window to use.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1994 JLB : Created.                                                                 *
 *   06/27/1994 JLB : Takes a window parameter.                                                *
 *   08/15/1994 JLB : Removed infantry support.                                                *
 *   01/07/1995 JLB : Harvester animation support.                                             *
 *   07/08/1995 JLB : Uses general purpose draw routine.                                       *
 *=============================================================================================*/
void UnitClass::Draw_It(int x, int y, WindowNumberType window) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	int			shapenum;		// Working shape number.
	void const	* shapefile;		// Working shape file pointer.
	int			facing = Dir_To_32(PrimaryFacing);
	int			tfacing = Dir_To_32(SecondaryFacing);
	DirType		rotation = DIR_N;
	int			scale = 0x0100;

	/*
	**	Verify the legality of the unit class.
	*/
	shapefile = Get_Image_Data();
	if (shapefile == NULL) return;

	/*
	**	If drawing of this unit is not explicitly prohibited, then proceed
	**	with the render process.
	*/
	const bool is_hidden = (Visual_Character() == VISUAL_HIDDEN) && (window != WINDOW_VIRTUAL);
	if (!is_hidden) {
		shapenum = Shape_Number();

		/*
		**	The artillery unit should have its entire body recoil when it fires.
		*/
		if (*this == UNIT_ARTY && IsInRecoilState) {
			Recoil_Adjust(PrimaryFacing.Current(), x, y);
		}

		/*
		**	Actually perform the draw. Overlay an optional shimmer effect as necessary.
		*/
		Techno_Draw_Object(shapefile, shapenum, x, y, window, rotation, scale);

		/*
		**	If there is a rotating radar dish, draw it now.
		*/
		if (Class->IsRadarEquipped) {
			if (*this == UNIT_MGG) {
				int x2 = x, y2 = y;
				shapenum = 32 + (Frame & 7);
				Class->Turret_Adjust(PrimaryFacing, x2, y2);
				Techno_Draw_Object(shapefile, shapenum, x2, y2, window);
			} else {
//#ifdef FIXIT_PHASETRANSPORT	//	checked - ajw 9/28/98
//				if (*this == UNIT_PHASE) {
//					shapenum = 38 + (Frame & 7);
//				} else {
//					shapenum = 32 + (Frame % 32);
//				}
//#else
				shapenum = 32 + (Frame % 32);
//#endif
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				if (*this == UNIT_TESLATANK) {
					Techno_Draw_Object(shapefile, shapenum, x, y, window);
				} else {
					Techno_Draw_Object(shapefile, shapenum, x, y-5, window);
				}
#else
				Techno_Draw_Object(shapefile, shapenum, x, y-5, window);
#endif
			}
		}

		/*
		**	If there is a turret, then it must be rendered as well. This may include
		**	firing animation if required.
		*/
		if (/*!Class->IsChunkyShape &&*/ Class->IsTurretEquipped) {
			int xx = x;
			int yy = y;

			/*
			**	Determine which turret shape to use. This depends on if there
			**	is any firing animation in progress.
			*/
			shapenum = TechnoClass::BodyShape[tfacing]+32;
#ifdef FIXIT_PHASETRANSPORT	//	checked - ajw 9/28/98
			if (*this == UNIT_PHASE) {
				shapenum += 6;
			}
#endif
			/*
			**	A recoiling turret moves "backward" one pixel.
			*/
			if (IsInRecoilState) {
				Recoil_Adjust(SecondaryFacing, xx, yy);
			}

			Class->Turret_Adjust(PrimaryFacing, xx, yy);

			/*
			**	Actually perform the draw. Overlay an optional shimmer effect as necessary.
			*/
			Techno_Draw_Object(shapefile, shapenum, xx, yy, window);
		}
	}

	/*
	**	If this unit is carrying the flag, then draw that on top of everything else.
	*/
	if (Flagged != HOUSE_NONE) {
		CC_Draw_Shape(this, "FLAGFLY", MFCD::Retrieve("FLAGFLY.SHP"), Frame % 14, x, y, window, SHAPE_CENTER|SHAPE_FADING|SHAPE_GHOST, HouseClass::As_Pointer(Flagged)->Remap_Table(false, Class->Remap), Map.UnitShadow, DIR_N, 0x0100, Flagged);
	}

	DriveClass::Draw_It(x, y, window);
}


/***********************************************************************************************
 * UnitClass::Tiberium_Check -- Search for and head toward nearest available Tiberium patch.   *
 *                                                                                             *
 *    This routine is used to move a harvester to a place where it can load up with            *
 *    Tiberium. It will return true only if it can start harvesting. Otherwise, it sets        *
 *    the navigation computer toward the nearest Tiberium and lets the unit head there         *
 *    automatically.                                                                           *
 *                                                                                             *
 * INPUT:   center   -- Reference to the center of the radius scan.                            *
 *                                                                                             *
 *          x,y      -- Relative offset from the center cell to perform the check upon.        *
 *                                                                                             *
 * OUTPUT:  bool; Is it located directly over a Tiberium patch?                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Tiberium_Check(CELL & center, int x, int y)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	/*
	**	If the specified offset from the origin will cause it
	**	to spill past the map edge, then abort this cell check.
	*/
	if (Cell_X(center)+x < Map.MapCellX) return(false);
	if (Cell_X(center)+x >= Map.MapCellX+Map.MapCellWidth) return(false);
	if (Cell_Y(center)+y < Map.MapCellY) return(false);
	if (Cell_Y(center)+y >= Map.MapCellY+Map.MapCellHeight) return(false);

	center = XY_Cell(Cell_X(center)+x, Cell_Y(center)+y);

	if ((Session.Type != GAME_NORMAL || (!IsOwnedByPlayer || Map[center].IsMapped))) {
		if (Map[Coord].Zones[Class->MZone] != Map[center].Zones[Class->MZone]) return(false);
		if (!Map[center].Cell_Techno() && Map[center].Land_Type() == LAND_TIBERIUM) {
			return(true);
		}
	}
	return(false);
}


/***********************************************************************************************
 * UnitClass::Goto_Tiberium -- Searches for and heads toward tiberium.                         *
 *                                                                                             *
 *    This routine will cause the unit to search for and head toward nearby Tiberium. When     *
 *    the Tiberium is reached, then this routine should not be called again until such time    *
 *    as additional harvesting is required. When this routine returns false, then it should    *
 *    be called again until such time as it returns true.                                      *
 *                                                                                             *
 * INPUT:   rad = size of ring to search                                                       *
 *                                                                                             *
 * OUTPUT:  Has the unit reached Tiberium and harvesting should begin?                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Goto_Tiberium(int rad)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	if (!Target_Legal(NavCom)) {
		CELL center = Coord_Cell(Center_Coord());
		if (Map[center].Land_Type() == LAND_TIBERIUM) {
			return(true);
		} else {

			/*
			**	Perform a ring search outward from the center.
			*/
			for (int radius = 1; radius < rad; radius++) {
				for (int x = -radius; x <= radius; x++) {
					CELL cell = center;
					if (Tiberium_Check(cell, x, -radius)) {
						Assign_Destination(::As_Target(cell));
						return(false);
					}

					cell = center;
					if (Tiberium_Check(cell, x, +radius)) {
						Assign_Destination(::As_Target(cell));
						return(false);
					}

					cell = center;
					if (Tiberium_Check(cell, -radius, x)) {
						Assign_Destination(::As_Target(cell));
						return(false);
					}

					cell = center;
					if (Tiberium_Check(cell, +radius, x)) {
						Assign_Destination(::As_Target(cell));
						return(false);
					}
				}
			}
		}
	}

	return(false);
}


/***********************************************************************************************
 * UnitClass::Harvesting -- Harvests tiberium at the current location.                         *
 *                                                                                             *
 *    This routine is used to by the harvester to harvest Tiberium at the current location.    *
 *    When harvesting is complete, this routine will return true.                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is harvesting complete?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Harvesting(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	CELL	cell = Coord_Cell(Coord);
	CellClass * ptr = &Map[cell];

	/*
	**	Keep waiting if still heading toward a spot to harvest.
	*/
	if (Target_Legal(NavCom)) return(true);

	if (Tiberium_Load() < 1 && ptr->Land_Type() == LAND_TIBERIUM) {

		/*
		**	Lift some Tiberium from the ground. Try to lift a complete
		**	"level" of Tiberium. A level happens to be 6 steps. If there
		**	is a partial level, then lift that instead. Never lift more
		**	than the harvester can carry.
		*/
//		int reducer = (ptr->OverlayData % 6) + 1;
		int reducer = 1;
		OverlayType overlay = ptr->Overlay;
		reducer = ptr->Reduce_Tiberium(min(reducer, Rule.BailCount-Tiberium));
		Tiberium += reducer;
		switch (overlay) {
			case OVERLAY_GOLD1:
			case OVERLAY_GOLD2:
			case OVERLAY_GOLD3:
			case OVERLAY_GOLD4:
				Gold += reducer;
				break;

			case OVERLAY_GEMS1:
			case OVERLAY_GEMS2:
			case OVERLAY_GEMS3:
			case OVERLAY_GEMS4:
				Gems += reducer;
				if (Rule.BailCount > Tiberium) {Gems++;Tiberium++;}
				if (Rule.BailCount > Tiberium) {Gems++;Tiberium++;}
				if (Rule.BailCount > Tiberium) {Gems++;Tiberium++;}
				break;

			default:
				break;
		}
		Set_Stage(0);
		Set_Rate(Rule.OreDumpRate);

	} else {

		/*
		**	If the harvester is stopped on a non Tiberium field and the harvester
		**	isn't loaded with Tiberium, then no further action can be performed
		**	by this logic routine. Bail with a failure and thus cause a branch to
		**	a better suited logic processor.
		*/
		Set_Stage(0);
		Set_Rate(0);
		return(false);
	}
	return(true);
}


/***********************************************************************************************
 * UnitClass::Mission_Unload -- Handles unloading cargo.                                       *
 *                                                                                             *
 *    This is the AI control sequence for when a transport desires to unload its cargo and     *
 *    then exit the map.                                                                       *
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
extern void Logic_Switch_Player_Context(ObjectClass *object);
extern void Logic_Switch_Player_Context(HouseClass *object);
extern void On_Special_Weapon_Targetting(const HouseClass* player_ptr, SpecialWeaponType weapon_type);

int UnitClass::Mission_Unload(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	enum {
		INITIAL_CHECK,
		MANEUVERING,
		OPENING_DOOR,
		UNLOADING,
		CLOSING_DOOR
	};
	DirType	dir;
	CELL		cell;

	switch (Class->Type) {
		case UNIT_HARVESTER:
			if (PrimaryFacing != DIR_W) {
				if (!IsRotating) {
					Do_Turn(DIR_W);
				}
				return(5);
			}

			if (!IsDumping) {
				IsDumping = true;
				Set_Stage(0);
				Set_Rate(Rule.OreDumpRate);
				break;
			}
			if (Fetch_Stage() < ARRAY_SIZE(Class->Harvester_Dump_List)-1) break;

			IsDumping = false;
			if (Tiberium) {
				Tiberium = 0;
				int credits = Credit_Load();
				House->Harvested(credits);
				Tiberium = Gold = Gems = 0;
			}
			Transmit_Message(RADIO_OVER_OUT);

			Assign_Mission(MISSION_HARVEST);
			break;

		case UNIT_TRUCK:
			switch (Status) {
				case INITIAL_CHECK:
					dir = Desired_Load_Dir(NULL, cell);
					if (How_Many() && cell != 0) {
						Do_Turn(dir);
						Status = MANEUVERING;
						return(1);
					} else {
						Assign_Mission(MISSION_GUARD);
					}
					break;

				case MANEUVERING:
					if (!IsRotating) {
						Status = UNLOADING;
						return(1);
					}
					break;

				case UNLOADING:
					if (How_Many()) {
						FootClass * passenger = Detach_Object();

						if (passenger != NULL) {
							DirType toface = DIR_S + PrimaryFacing;
							bool placed = false;

							for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
								DirType newface = toface + Facing_Dir(face);
								CELL newcell = Adjacent_Cell(Coord_Cell(Coord), newface);

								if (passenger->Can_Enter_Cell(newcell) == MOVE_OK) {
									ScenarioInit++;
									passenger->Unlimbo(Coord_Move(Coord, newface, 0x0080), newface);
									ScenarioInit--;
									passenger->Assign_Mission(MISSION_MOVE);
									passenger->Assign_Destination(::As_Target(newcell));
									placed = true;
									break;
								}
							}

							/*
							** If the attached unit could NOT be deployed, then re-attach
							**	it and then bail out of this deploy process.
							*/
							if (!placed) {
								Attach(passenger);
								Status = CLOSING_DOOR;
							}
							else {
								passenger->Look(false);
							}
						}
					} else {
						Status = CLOSING_DOOR;
					}
					break;

				/*
				**	Close APC door in preparation for normal operation.
				*/
				case CLOSING_DOOR:
					Assign_Mission(MISSION_GUARD);
					break;
			}
			break;

		case UNIT_APC:
#ifdef FIXIT_PHASETRANSPORT	//	checked - ajw 9/28/98
		case UNIT_PHASE:
#endif
			switch (Status) {
				case INITIAL_CHECK:
					dir = Desired_Load_Dir(NULL, cell);
					if (How_Many() && cell != 0) {
						Do_Turn(dir);
						Status = MANEUVERING;
						return(1);
					} else {
						Assign_Mission(MISSION_GUARD);
					}
					break;

				case MANEUVERING:
					if (!IsRotating) {
						APC_Open_Door();
						if (Is_Door_Opening()) {
							Status = OPENING_DOOR;
							return(1);
						}
					}
					break;

				case OPENING_DOOR:
					if (Is_Door_Open()) {
						Status = UNLOADING;
						return(1);
					} else {
						if (!Is_Door_Opening()) {
							Status = INITIAL_CHECK;
						}
					}
					break;

				case UNLOADING:
					if (How_Many()) {
						FootClass * passenger = Detach_Object();

						if (passenger != NULL) {
							DirType toface = DIR_S + PrimaryFacing;
							bool placed = false;

							for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
								DirType newface = toface + Facing_Dir(face);
								CELL newcell = Adjacent_Cell(Coord_Cell(Coord), newface);

								if (passenger->Can_Enter_Cell(newcell) == MOVE_OK) {
									ScenarioInit++;
									passenger->Unlimbo(Coord_Move(Coord, newface, 0x0080), newface);
									ScenarioInit--;
									passenger->Assign_Mission(MISSION_MOVE);
									passenger->Assign_Destination(::As_Target(newcell));
									placed = true;
									break;
								}
							}

							/*
							** If the attached unit could NOT be deployed, then re-attach
							**	it and then bail out of this deploy process.
							*/
							if (!placed) {
								Attach(passenger);
								Status = CLOSING_DOOR;
							}
							else {
								passenger->Look(false);
							}
						}
					} else {
						Status = CLOSING_DOOR;
					}
					break;

				/*
				**	Close APC door in preparation for normal operation.
				*/
				case CLOSING_DOOR:
					if (Is_Door_Open()) {
						APC_Close_Door();
					}
					if (Is_Door_Closed()) {
						Assign_Mission(MISSION_GUARD);
					}
					break;
			}
			break;

		case UNIT_MCV:
			switch (Status) {
				case 0:
					Path[0] = FACING_NONE;
					Status = 1;
					break;

				case 1:
					if (!IsDriving) {
						Try_To_Deploy();
						if (IsActive) {
							if (IsDeploying) {
								Status = 2;
							} else {
								if (!House->IsHuman && Session.Type != GAME_NORMAL) {
									Assign_Mission(MISSION_HUNT);
								} else {
									Assign_Mission(MISSION_GUARD);
								}
							}
						}
					}
					break;

				case 2:
					if (!IsDeploying) {
						Assign_Mission(MISSION_GUARD);
					}
					break;
			}
			return(1);

		case UNIT_MINELAYER:
			switch (Status) {
				case INITIAL_CHECK:
					dir = DIR_NE;
					if (Ammo > 0) {
						Do_Turn(dir);
						Status = MANEUVERING;
						return(1);
					} else {
						Assign_Mission(MISSION_GUARD);
					}
					break;

				case MANEUVERING:
					if (!IsRotating) {
						APC_Open_Door();
						if (Is_Door_Opening()) {
							Status = OPENING_DOOR;
							return(1);
						}
					}
					break;

				case OPENING_DOOR:
					if (Is_Door_Open()) {
						Status = UNLOADING;
						return(1);
					} else {
						if (!Is_Door_Opening()) {
							Status = INITIAL_CHECK;
						}
					}
					break;

				case UNLOADING:
					if (Ammo > 0) {
						if (!Map[Center_Coord()].Cell_Building()) {
							Mark(MARK_UP);
							BuildingClass * building = new BuildingClass((House->ActLike == HOUSE_USSR || House->ActLike == HOUSE_UKRAINE || House->ActLike == HOUSE_BAD) ? STRUCT_APMINE : STRUCT_AVMINE, House->Class->House);
							if (building != NULL) {
								ScenarioInit = true;
								if (building->Unlimbo(Coord)) {
									Sound_Effect(VOC_MINELAY1, Coord);
									ScenarioInit = false;
									building->Revealed(House);
									Ammo--;
								}
								ScenarioInit = false;
							}
							Status = CLOSING_DOOR;
							Mark(MARK_DOWN);
						} else {
							Status = CLOSING_DOOR;
						}
					} else {
						Status = CLOSING_DOOR;
					}
					break;

				/*
				**	Close APC door in preparation for normal operation.
				*/
				case CLOSING_DOOR:
					if (Is_Door_Open()) {
						APC_Close_Door();
					}
					if (Is_Door_Closed()) {
						Assign_Mission(MISSION_GUARD);
					}
					break;
			}
			break;

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		case UNIT_MAD:
			if (!Gems && !IsDumping) {
				Gems = 1;
				Gold = 0;
				Arm = QuakeDelay * House->ROFBias;
#ifdef ENGLISH
				Speak(VOX_MADTANK_DEPLOYED);	// this voice only exists in English
#else
				Sound_Effect(VOC_BUZZY1,Center_Coord());
#endif
				Set_Stage(0);
				Set_Rate(Rule.OreDumpRate*2);
				IsDumping = true;

#if 1
				InfantryClass *crew = new InfantryClass(INFANTRY_C1, House->Class->House);
				if (crew != NULL) crew->IsTechnician = true;

				if (crew != NULL) {
					DirType toface = DIR_S + PrimaryFacing;

					for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
						DirType newface = toface + Facing_Dir(face);
						CELL newcell = Adjacent_Cell(Coord_Cell(Coord), newface);
						if (crew->Can_Enter_Cell(newcell) == MOVE_OK) {
							ScenarioInit++;
							crew->Unlimbo(Coord_Move(Coord, newface, 0x0080), newface);
							ScenarioInit--;
							crew->Assign_Mission(MISSION_MOVE);
							crew->Assign_Destination(::As_Target(newcell));
							break;
						}
					}
				}
#endif
			}

			if ( (Arm && !Gold) || IronCurtainCountDown) {
				Set_Stage(Fetch_Stage() & 1);
				return(1);
			}

			if (!Gold) {
				Sound_Effect(VOC_MAD_CHARGE, Center_Coord());
				Set_Stage(0);
				Gold = 1;
				return(1);
			}

			if (Fetch_Stage() < 7) {
				return(1);
			}

			IsDumping = false;

			Sound_Effect(VOC_MAD_EXPLODE, Center_Coord());

			Strength = 1;			// assure destruction
			PendingTimeQuake = true;		// trigger a time quake
			TimeQuakeCenter = ::As_Target(Center_Coord());
			break;

		case UNIT_CHRONOTANK:
			if (IsOwnedByPlayer) {
				Map.IsTargettingMode = SPC_CHRONO2;
				HouseClass* old_player_ptr = PlayerPtr;
				Logic_Switch_Player_Context(this);
				Unselect_All();
				On_Special_Weapon_Targetting(PlayerPtr, Map.IsTargettingMode);
				Logic_Switch_Player_Context(old_player_ptr);
			}
			House->UnitToTeleport = As_Target();

			Assign_Mission(MISSION_GUARD);
			break;
#endif
		default:
			break;
	}
	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
}


/***********************************************************************************************
 * UnitClass::Mission_Harvest -- Handles the harvesting process used by harvesters.            *
 *                                                                                             *
 *    This is the AI process used by harvesters when they are doing their harvesting action.   *
 *    This entails searching for nearby Tiberium, heading there, harvesting, and then          *
 *    returning to a refinery for unloading.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before calling this routine again.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *   06/21/1995 JLB : Force guard mode if no Tiberium found.                                   *
 *   09/28/1995 JLB : Aborts harvesting if there are no more refineries.                       *
 *=============================================================================================*/
int UnitClass::Mission_Harvest(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	enum {
		LOOKING,
		HARVESTING,
		FINDHOME,
		HEADINGHOME,
		GOINGTOIDLE,
	};

	/*
	**	A non-harvesting type unit will just sit still if it is given the harvest mission. This
	**	allows combat units to act "brain dead".
	*/
	if (!Class->IsToHarvest) return(TICKS_PER_SECOND*30);

	/*
	**	If there are no more refineries, then drop into guard mode.
	*/
	if (!(House->ActiveBScan & STRUCTF_REFINERY)) {
		Assign_Mission(MISSION_GUARD);
		return(1);
	}

	switch (Status) {

		/*
		**	Go and find a Tiberium field to harvest.
		*/
		case LOOKING:
			/*
			**	When full of tiberium, just skip to finding a free refinery
			**	to unload at.
			*/
			if (Tiberium_Load() == 1) {
				Status = FINDHOME;
				return(1);
			}

			/*
			** Look for ore where we last found some - mine the same patch
			*/
			if (Target_Legal(ArchiveTarget)) {
				Assign_Destination(ArchiveTarget);
				ArchiveTarget = 0;
			}
			IsHarvesting = false;
			if (Goto_Tiberium(Rule.TiberiumLongScan / CELL_LEPTON_W)) {
				IsHarvesting = true;
				Set_Rate(2);
				Set_Stage(0);
				Status = HARVESTING;
				return(1);
			} else {

				/*
				**	If the harvester isn't on Tiberium and it is not heading toward Tiberium, then
				**	force it to go into guard mode. This will prevent the harvester from repeatedly
				**	searching for Tiberium.
				*/
				if (!Target_Legal(NavCom)) {

					/*
					**	If the archive target is legal, then head there since it is presumed
					**	that the archive target points to the last place it harvested at. This might
					**	solve the case where the harvester gets stuck and can't find Tiberium just because
					**	it is greater than 32 squares away.
					*/
					if (Target_Legal(ArchiveTarget)) {
						Assign_Destination(ArchiveTarget);
					} else {
						Status = GOINGTOIDLE;
						IsUseless = true;
						House->IsTiberiumShort = true;
						return(TICKS_PER_SECOND*7);
					}
				} else {
					IsUseless = false;
				}
			}
			break;

		/*
		**	Harvest at current location until full or Tiberium exhausted.
		*/
		case HARVESTING:
//			if (Fetch_Stage() > ARRAY_SIZE(Class->Harvester_Load_List)) {
//				Set_Stage(0);
//			}
			if (Fetch_Rate() == 0) {
				Set_Stage(0);
				Set_Rate(Rule.OreDumpRate);
			}

			if (Fetch_Stage() < ARRAY_SIZE(Class->Harvester_Load_List)) return(1);
			if (!Harvesting()) {
				IsHarvesting = false;
				if (Tiberium_Load() == 1) {
					Status = FINDHOME;
				  	ArchiveTarget = ::As_Target(Coord_Cell(Coord));
				} else {
					if (!Goto_Tiberium(Rule.TiberiumShortScan / CELL_LEPTON_W) && !Target_Legal(NavCom))	{
					  	ArchiveTarget = TARGET_NONE;
						Status = FINDHOME;
					} else {
						Status = HARVESTING;
						IsHarvesting = true;
					}
				}
				return(1);
			}
			return(1);
//			return(TICKS_PER_SECOND*Rule.OreDumpRate);

		/*
		**	Find and head to refinery.
		*/
		case FINDHOME:
			if (!Target_Legal(NavCom)) {

				/*
				**	Find nearby refinery and head to it?
				*/
				BuildingClass * nearest = Find_Docking_Bay(STRUCT_REFINERY, false);

				/*
				**	Since the refinery said it was ok to load, establish radio
				**	contact with the refinery and then await docking orders.
				*/
				if (nearest != NULL && Transmit_Message(RADIO_HELLO, nearest) == RADIO_ROGER) {
					Status = HEADINGHOME;
					if (nearest->House == PlayerPtr && (PlayerPtr->Capacity - PlayerPtr->Tiberium) < 300 && PlayerPtr->Capacity > 500 && (PlayerPtr->ActiveBScan & (STRUCTF_REFINERY | STRUCTF_CONST))) {
						Speak(VOX_NEED_MO_CAPACITY);
					}
				} else {
					ScenarioInit++;
					nearest = Find_Docking_Bay(STRUCT_REFINERY, false);
					ScenarioInit--;
					if (nearest != NULL) {
						Assign_Destination(::As_Target(Nearby_Location(nearest)));
					}
				}
			}
			break;

		/*
		**	In communication with refinery so that it will successfully dock and
		**	unload. If, for some reason, radio contact was lost, then hunt for
		**	another refinery to unload at.
		*/
		case HEADINGHOME:
			Assign_Mission(MISSION_ENTER);
			return(1);

		/*
		**	The harvester has nothing to do. There is no Tiberium nearby and
		**	no where to go.
		*/
		case GOINGTOIDLE:
			if (IsUseless) {
				if (House->ActiveBScan & STRUCTF_REPAIR) {
					Assign_Mission(MISSION_REPAIR);
				} else {
					Assign_Mission(MISSION_HUNT);
				}
			}
			Assign_Mission(MISSION_GUARD);
			break;

	}
	return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
}


/***********************************************************************************************
 * UnitClass::Mission_Hunt -- This is the AI process for aggressive enemy units.               *
 *                                                                                             *
 *    Computer controlled units must be intelligent enough to find enemies as well as to       *
 *    attack them. This AI process will handle both the simple attack process as well as the   *
 *    scanning for enemy units to attack.                                                      *
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
int UnitClass::Mission_Hunt(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	if (*this == UNIT_MCV) {
		enum {
			FIND_SPOT,
			WAITING
		};

		switch (Status) {

			/*
			**	This stage handles locating a convenient spot, rotating to face the correct
			**	direction and then commencing the deployment operation.
			*/
			case FIND_SPOT:
				if (Goto_Clear_Spot()) {
					if (Try_To_Deploy()) {
						Status = WAITING;
					}
				}
				break;

			/*
			**	This stage watchdogs the deployment operation and if for some reason, the deployment
			**	is aborted (the IsDeploying flag becomes false), then it reverts back to hunting for
			**	a convenient spot to deploy.
			*/
			case WAITING:
				if (!IsDeploying) {
					Status = FIND_SPOT;
				}
				break;
		}
	} else {

		return(DriveClass::Mission_Hunt());
	}
	return(MissionControl[Mission].Normal_Delay()+Random_Pick(0, 2));
}


/***********************************************************************************************
 * UnitClass::Overlap_List -- Determines overlap list for units.                               *
 *                                                                                             *
 *    The unit overlap list is used to keep track of which cells are to                        *
 *    be marked for redraw. This is critical in order to keep the units                        *
 *    displayed correctly.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the overlap list pointer for the unit at its                          *
 *          present position.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/26/1994 JLB : Created.                                                                 *
 *   06/19/1994 JLB : Uses Coord_Spillable_List function.                                      *
 *=============================================================================================*/
short const * UnitClass::Overlap_List(bool redraw) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

#ifdef PARTIAL
	if (Height == 0 && redraw && Class->DimensionData != NULL) {
		Rect rect;
		int shapenum = Shape_Number();
		if (Class->DimensionData[shapenum].Is_Valid()) {
			rect = Class->DimensionData[shapenum];
		} else {
			rect = Class->DimensionData[shapenum] = Shape_Dimensions(Get_Image_Data(), shapenum);
		}

		if (Is_Selected_By_Player()) {
			rect = Union(rect, Rect(-15, -15, 30, 30));
		}

		if (Class->IsTurretEquipped || Class->IsRadarEquipped) {
			rect = Union(rect, Rect(-15, -15, 30, 30));
		}

		return(Coord_Spillage_List(Coord, rect, true));
	}
#else
	redraw = redraw;
#endif

	int size = ICON_PIXEL_W;

	if (redraw && (Is_Selected_By_Player() || IsFiring)) {
		size += 24;
	}
	if (Class->IsGigundo || IsAnimAttached) {
		size = ICON_PIXEL_W*2;
	}

	return(Coord_Spillage_List(Coord, size)+1);
}


/***********************************************************************************************
 * UnitClass::Can_Enter_Cell -- Determines cell entry legality.                                *
 *                                                                                             *
 *    Use this routine to determine if the unit can enter the cell                             *
 *    specified and given the direction of entry specified. Typically,                         *
 *    this is used when determining unit travel path.                                          *
 *                                                                                             *
 * INPUT:   cell     -- The cell to examine.                                                   *
 *                                                                                             *
 *          facing   -- The facing that the unit would enter the specified                     *
 *                      cell. If this value is -1, then don't consider                         *
 *                      facing when performing the check.                                      *
 *                                                                                             *
 * OUTPUT:  Returns the reason why it couldn't enter the cell or MOVE_OK if movement is        *
 *          allowed.                                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/07/1992 JLB : Created.                                                                 *
 *   04/16/1994 JLB : Converted to member function.                                            *
 *   07/04/1995 JLB : Allowed to drive on building trying to enter it.                         *
 *=============================================================================================*/
MoveType UnitClass::Can_Enter_Cell(CELL cell, FacingType ) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	bool cancrush = false;

	CellClass const * cellptr = &Map[cell];

	if ((unsigned)cell >= MAP_CELL_TOTAL) return(MOVE_NO);

	/*
	**	Moving off the edge of the map is not allowed unless
	**	this is a loaner vehicle.
	*/
	if (!ScenarioInit && !Map.In_Radar(cell) && !Is_Allowed_To_Leave_Map() && IsLocked) {
		return(MOVE_NO);
	}

	MoveType retval = MOVE_OK;

	/*
	**	Certain vehicles can drive over walls. Check for this case and
	**	and return the appropriate flag. Other units treat walls as impassable.
	*/
	if (cellptr->Overlay != OVERLAY_NONE) {
		OverlayTypeClass const * optr = &OverlayTypeClass::As_Reference(cellptr->Overlay);

		if (optr->IsCrate && !((Session.Type == GAME_NORMAL) ? House->IsPlayerControl : House->IsHuman) && Session.Type == GAME_NORMAL) {
			return(MOVE_NO);
		}

		if (optr->IsWall) {

			/*
			**	If the blocking wall is crushable (and not owned by this player or one of this players
			**	allies, then record that it is crushable and let the normal logic take over. The end
			**	result should cause this unit to consider the cell passable.
			*/
			if (optr->IsCrushable && Class->IsCrusher) {
				cancrush = !House->Is_Ally(cellptr->Owner);
			}

			if (!cancrush && Is_Weapon_Equipped()) {
				WarheadTypeClass const * whead = Class->PrimaryWeapon->WarheadPtr;

				if (whead->IsWallDestroyer || (whead->IsWoodDestroyer && optr->IsWooden)) {
//					if (!House->IsHuman && !House->Is_Ally(cellptr->Owner)) {
						if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE;
//					} else {
//						return(MOVE_NO);
//					}
				} else {
					return(MOVE_NO);
				}
			}
		}
	}

	/*
	** Loop through all of the objects in the square setting a bit
	** for how they affect movement.
	*/
	bool crushable = false;
	ObjectClass * obj = cellptr->Cell_Occupier();
	while (obj != NULL) {

		if (obj != this) {

			/*
			** If object is a land mine, allow movement if possible.
			*/
			if (obj->What_Am_I() == RTTI_BUILDING && (!Rule.IsMineAware || !((BuildingClass *)obj)->House->Is_Ally(House))) {
				if ((*(BuildingClass *)obj) == STRUCT_APMINE) return(MOVE_OK);
				if ((*(BuildingClass *)obj) == STRUCT_AVMINE) return(MOVE_OK);
			}

			/*
			**	Always allow entry if trying to move on a cell with
			**	authorization from the occupier.
			*/
			if (obj == Contact_With_Whom() && (IsTethered || (obj->What_Am_I() ==  RTTI_BUILDING && *((BuildingClass *)obj) == STRUCT_REPAIR))) {
				return(MOVE_OK);
			}

			/*
			**	Special check to allow entry into the sea transport this vehicle
			**	is trying to enter.
			*/
			if (Mission == MISSION_ENTER && obj->As_Target() == NavCom && IsTethered) {
				return(MOVE_OK);
			}

			/*
			**	Guard area should not allow the guarding unit to enter the cell with the
			**	guarded unit.
			*/
			if (Mission == MISSION_GUARD_AREA && ArchiveTarget == obj->As_Target()) {
				return(MOVE_NO);
			}

			bool is_moving = obj->Is_Foot() &&
						(Target_Legal(((FootClass *)obj)->NavCom) || ((FootClass *)obj)->PrimaryFacing.Is_Rotating() || ((FootClass *)obj)->IsDriving);
//						(((FootClass *)obj)->PrimaryFacing.Is_Rotating() || ((FootClass *)obj)->IsDriving);
//						(((FootClass *)obj)->IsRotating || ((FootClass *)obj)->IsDriving);
//						(Target_Legal(((FootClass *)obj)->NavCom) || ((FootClass *)obj)->IsDriving);

			if (House->Is_Ally(obj)) {
				if (is_moving) {
					int face 		= Dir_Facing(PrimaryFacing);
					int techface	= Dir_Facing(((FootClass const *)obj)->PrimaryFacing) ^4;
					if (face == techface && Distance((AbstractClass const *)obj) <= 0x1FF) {
						return(MOVE_NO);
					}
					if (retval < MOVE_MOVING_BLOCK) retval = MOVE_MOVING_BLOCK;
				} else {
					if (obj->What_Am_I() == RTTI_BUILDING) return(MOVE_NO);

					/*
					**	If the blocking object is not in the same zone, then it certainly
					**	isn't a temporary block, it is a permanent one.
					*/
					if (Map[Coord].Zones[Class->MZone] != cellptr->Zones[Class->MZone]) return(MOVE_NO);

					if (retval < MOVE_TEMP) retval = MOVE_TEMP;
				}
			} else {

				/*
				**	Cloaked enemy objects are not considered if this is a Find_Path()
				**	call.
				*/
				if (!obj->Is_Techno() || ((TechnoClass *)obj)->Cloak != CLOAKED) {

					/*
					**	If this unit can crush infantry, and there is an enemy infantry in the
					**	cell, don't consider the cell impassible. This is true even if the unit
					**	doesn't contain a legitimate weapon.
					*/
					bool crusher = Class->IsCrusher;
					if (!crusher || !obj->Class_Of().IsCrushable) {

						/*
						**	Any non-allied blockage is considered impassable if the unit
						**	is not equipped with a weapon.
						*/
						if (Class->PrimaryWeapon == NULL) return(MOVE_NO);

						/*
						**	Some kinds of terrain are considered destroyable if the unit is equipped
						**	with the weapon that can destroy it. Otherwise, the terrain is considered
						**	impassable.
						*/
						switch (obj->What_Am_I()) {
							case RTTI_TERRAIN:

#ifdef TOFIX
								if (((TerrainClass *)obj)->Class->Armor == ARMOR_WOOD &&
										Class->PrimaryWeapon->WarheadPtr->IsWoodDestroyer) {

									if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE;
								} else {
									return(MOVE_NO);
								}
								break;
#else
								return(MOVE_NO);
#endif

							default:
								if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE;
								break;
						}
					} else {
						crushable = true;
					}
				} else {
					if (retval < MOVE_CLOAK) retval = MOVE_CLOAK;
				}
			}
		}

		/*
		**	Move to next object in chain.
		*/
		obj = obj->Next;
	}

	/*
	**	If the cell is out and out impassable because of underlying terrain, then
	**	return this immutable fact.
	*/
	if (!cancrush && retval != MOVE_DESTROYABLE && Ground[cellptr->Land_Type()].Cost[Class->Speed] == 0) {
		return(MOVE_NO);
	}

	/*
	**	If some allied object has reserved the cell, then consider the cell
	**	as blocked by a moving object.
	*/
	if (retval == MOVE_OK && !crushable && (cellptr->Flag.Composite & 0x3F) != 0) {

		/*
		**	If reserved by a vehicle, then consider this blocked terrain.
		*/
		if (cellptr->Flag.Occupy.Vehicle) {
			retval = MOVE_MOVING_BLOCK;
		} else {
			if (cellptr->InfType != HOUSE_NONE && House->Is_Ally(cellptr->InfType)) {
				retval = MOVE_MOVING_BLOCK;
			} else {

				/*
				**	Enemy infantry have reserved the cell. If this unit can crush
				**	infantry, consider the cell passable. If not, then consider the
				**	cell destroyable if it has a weapon. If neither case applies, then
				**	this vehicle should avoid the cell altogether.
				*/
				if (!Class->IsCrusher) {
					if (Class->PrimaryWeapon != NULL && Class->PrimaryWeapon->Bullet->IsAntiGround) {
						retval = MOVE_DESTROYABLE;
					} else {
						return(MOVE_NO);
					}
				}
			}
		}
	}

	/*
	** If its ok to move into the cell because we can crush whats in the cell, then
	** make sure no one else is already moving into the cell to crush something.
	*/
	if (retval == MOVE_OK && crushable && cellptr->Flag.Occupy.Vehicle) {

		/*
		**	However, if the cell is occupied by a crushable vehicle, then we can
		**	never be sure if some other friendly vehicle is also trying to crush
		**	the cell at the same time. In the case of a crushable vehicle in the
		**	cell, then allow entry.
		*/
		if (!cellptr->Cell_Unit() || !cellptr->Cell_Unit()->Class->IsCrushable) {
			return(MOVE_MOVING_BLOCK);
		}
	}

	/*
	**	Return with the most severe reason why this cell would be impassable.
	*/
	return(retval);
}


/***********************************************************************************************
 * UnitClass::Init -- Clears all units for scenario preparation.                               *
 *                                                                                             *
 *    This routine will zero out the unit list and unit objects. This routine is typically     *
 *    used in preparation for a new scenario load. All units are guaranteed to be eliminated   *
 *    by this routine.                                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Init(void)
{
	Units.Free_All();
}


/***********************************************************************************************
 * UnitClass::Start_Driver -- Starts driving and reserves destination cell.                    *
 *                                                                                             *
 *    This routine will start the vehicle moving by marking the destination cell as            *
 *    reserved. Cells must be reserved in this fashion or else they might become occupied as   *
 *    the vehicle is proceeding toward it.                                                     *
 *                                                                                             *
 * INPUT:   headto   -- The location where the vehicle will be heading.                        *
 *                                                                                             *
 * OUTPUT:  bool; Was the vehicle started to move? Failure could be the result of the cell     *
 *                being occupied.                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Start_Driver(COORDINATE & headto)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	if (DriveClass::Start_Driver(headto) && IsActive) {//BG IsActive can be cleared by Start_Driver
		Mark_Track(headto, MARK_DOWN);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * UnitClass::What_Action -- Determines what action would occur if clicked on object.          *
 *                                                                                             *
 *    Use this function to determine what action would likely occur if the specified object    *
 *    were clicked on while this unit was selected as current. This function controls, not     *
 *    only the action to perform, but indirectly controls the cursor shape to use as well.     *
 *                                                                                             *
 * INPUT:   object   -- The object that to check for against "this" object.                    *
 *                                                                                             *
 * OUTPUT:  Returns with the default action to perform. If no clear action can be determined,  *
 *          then ACTION_NONE is returned.                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/11/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType UnitClass::What_Action(ObjectClass const * object) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	ActionType action = DriveClass::What_Action(object);

	/*
	** Allow units to move onto land mines.
	*/
	if (action == ACTION_NONE && object->What_Am_I() == RTTI_BUILDING) {
		StructType blah = *((BuildingClass *)object);
		if (blah == STRUCT_AVMINE || blah == STRUCT_APMINE) return(ACTION_MOVE);
	}

	/*
	**	If the unit doesn't have a weapon, but can crush the object, then consider
	**	the object as a movable location.
	*/
	if (action == ACTION_ATTACK && !Can_Player_Fire()) {
		if (Class->IsCrusher && object->Class_Of().IsCrushable) {
			action = ACTION_MOVE;
		} else {
			action = ACTION_SELECT;
		}
	}

	/*
	**	Don't allow special deploy action unless there is something to deploy.
	*/
	if (action == ACTION_SELF) {
		if (*this == UNIT_MCV) {

			/*
			**	The MCV will get the no-deploy cursor if it couldn't
			**	deploy at its current location.
			*/
			((ObjectClass &)(*this)).Mark(MARK_UP);
			if (!BuildingTypeClass::As_Reference(STRUCT_CONST).Legal_Placement(Coord_Cell(Adjacent_Cell(Center_Coord(), FACING_NW)))) {
				action = ACTION_NO_DEPLOY;
			}
			((ObjectClass &)(*this)).Mark(MARK_DOWN);

		} else {

			/*
			**	The mine layer can "deploy" its mines if it currently isn't
			**	sitting on top of a mine and it still has mines available.
			*/
			if (*this == UNIT_MINELAYER) {
				if (!Ammo || Map[Center_Coord()].Cell_Building() || (Map[Center_Coord()].Smudge != SMUDGE_NONE && SmudgeTypeClass::As_Reference(Map[Center_Coord()].Smudge).IsBib)) {
					action = ACTION_NO_DEPLOY;
				}
			} else {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				if (*this == UNIT_CHRONOTANK || *this == UNIT_MAD) {
					if (*this == UNIT_CHRONOTANK) {
// If the chrono tank's counter is still charging up, don't allow deploy.  Or,
// if it's a player-controlled chrono tank, and the player's currently trying
// to teleport a different unit, don't allow teleporting this unit.
						if(MoebiusCountDown || (IsOwnedByPlayer && House->UnitToTeleport && Map.IsTargettingMode == SPC_CHRONO2)) {
							action = ACTION_NO_DEPLOY;
						}
					}
				} else {
#endif
				/*
				**	All other units can "deploy" their passengers if they in-fact have
				**	passengers and are a transport vehicle. Otherwise, they cannot
				**	perform any self action.
				*/
				if (Class->Max_Passengers() > 0) {
					if (How_Many() == 0) {
						action = ACTION_NO_DEPLOY;
					}
				} else {
					action = ACTION_NONE;
				}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				}
#endif
			}
		}
	}

	/*
	**	Special return to friendly refinery action.
	*/
	if (House->IsPlayerControl && object->Is_Techno() && ((TechnoClass const *)object)->House->Is_Ally(this)) {
		if (object->What_Am_I() == RTTI_BUILDING && ((UnitClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (TechnoClass*)object) == RADIO_ROGER) {
			action = ACTION_ENTER;
		}
	}

	/*
	**	Special return to friendly repair factory action.
	*/
	if (House->IsPlayerControl && action == ACTION_SELECT && object->What_Am_I() == RTTI_BUILDING) {
		BuildingClass * building = (BuildingClass *)object;
		if (building->Class->Type == STRUCT_REPAIR && ((UnitClass *)this)->Transmit_Message(RADIO_CAN_LOAD, building) == RADIO_ROGER && !building->In_Radio_Contact() && !building->Is_Something_Attached()) {
			action = ACTION_MOVE;
		}
	}

	/*
	**	Check to see if it can enter a transporter.
	*/
	if (
		House->Is_Ally(object) &&
		House->IsPlayerControl && object->Is_Techno() && object->What_Am_I() == RTTI_VESSEL) {
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
 if( *(VesselClass *)object != VESSEL_CARRIER) {
#endif
		switch (((UnitClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (TechnoClass*)object)) {
			case RADIO_ROGER:
				action = ACTION_ENTER;
				break;

			case RADIO_NEGATIVE:
				action = ACTION_NO_ENTER;
				break;

			default:
				action = ACTION_NONE;
				break;
		}
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
 }
#endif
	}

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (*this == UNIT_MAD && (IsDumping || Gold)) {
		action = ACTION_NONE;
	}
#endif
	/*
	**	If it doesn't know what to do with the object, then just
	**	say it can't move there.
	*/
	if (action == ACTION_NONE) action = ACTION_NOMOVE;

	return(action);
}


/***********************************************************************************************
 * UnitClass::What_Action -- Determines action to perform on specified cell.                   *
 *                                                                                             *
 *    This routine will determine what action to perform if the mouse were clicked over the    *
 *    cell specified. At the unit level, only the harvester is checked for. The lower          *
 *    classes determine the regular action response.                                           *
 *                                                                                             *
 * INPUT:   cell  -- The cell that the mouse might be clicked on.                              *
 *                                                                                             *
 * OUTPUT:  Returns with the action type that this unit will perform if the mouse were         *
 *          clicked of the cell specified.                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType UnitClass::What_Action(CELL cell) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	ActionType action = DriveClass::What_Action(cell);
	if (action == ACTION_MOVE && Map[cell].Land_Type() == LAND_TIBERIUM && Class->IsToHarvest) {
		return(ACTION_HARVEST);
	}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (*this == UNIT_MAD && (IsDumping || Gold)) {
		action = ACTION_NOMOVE;
	}
#endif
	return(action);
}


/***********************************************************************************************
 * UnitClass::Exit_Repair -- Drive the unit off the repair facility.                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/03/1995 BWG : Created.                                                                 *
 *=============================================================================================*/
#define XYCELL(x, y)	(y*MAP_CELL_W+x)
void UnitClass::Exit_Repair(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	int	i;
	CELL	cell;
	bool	found = false;
	static short const ExitRepair[] = {
		XYCELL(0,	-2),
		XYCELL(1,	-1),
		XYCELL(2,	0),
		XYCELL(1,	1),
		XYCELL(0,	2),
		XYCELL(-1,	1),
		XYCELL(-2,	0),
		XYCELL(-1,	-1)
	};

	cell = Coord_Cell(Coord) + ExitRepair[Dir_Facing(PrimaryFacing.Current())];
	if (Can_Enter_Cell(cell) == MOVE_OK) found = true;

	if (!found) for (i=0; i<8; i++) {
		cell = Coord_Cell(Coord) + ExitRepair[i];
		if (Can_Enter_Cell(cell) == MOVE_OK) {
			found = true;
			break;
		}
	}
	if (found) {
//		DirType	dir = Direction(cell);

		Assign_Mission(MISSION_MOVE);
		Assign_Destination(::As_Target(cell));
	}
}


/***********************************************************************************************
 * UnitClass::Mission_Guard -- Special guard mission override processor.                       *
 *                                                                                             *
 *    Handles the guard mission for the unit. If the IQ is high enough and the unit is         *
 *    a harvester, it will begin to harvest automatically. An MCV might autodeploy.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the time delay before this command is executed again.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *   05/08/1995 JLB : Fixes gunboat problems.                                                  *
 *=============================================================================================*/
int UnitClass::Mission_Guard(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);
	if (/*House->IsBaseBuilding &&*/ !House->IsHuman && Class->IsToHarvest && House->Get_Quantity(STRUCT_REFINERY) > 0 && !House->IsTiberiumShort) {
		Assign_Mission(MISSION_HARVEST);
		return(1);
//		return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
	}

	if (*this == UNIT_MCV && House->IsBaseBuilding) {
		Assign_Mission(MISSION_UNLOAD);
		return(MissionControl[Mission].Normal_Delay() + Random_Pick(0, 2));
	}
	return(DriveClass::Mission_Guard());
}


/***********************************************************************************************
 * UnitClass::Mission_Move -- Handles special move mission overrides.                          *
 *                                                                                             *
 *    This routine intercepts the normal move mission and if a gunboat is being processed,     *
 *    changes its mission to hunt. This is an attempt to keep the gunboat on the hunt mission  *
 *    regardless of what the player did.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of ticks before this routine should be called again.            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/09/1995 JLB : Created.                                                                 *
 *   09/28/1995 JLB : Harvester stick in guard mode if no more refineries.                     *
 *=============================================================================================*/
int UnitClass::Mission_Move(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	IsHarvesting = false;

	/*
	**	Always make sure that that transport door is closed if the vehicle is moving.
	*/
	if (!Is_Door_Closed()) {
		APC_Close_Door();
	}

	return(DriveClass::Mission_Move());
}


/***********************************************************************************************
 * UnitClass::Desired_Load_Dir -- Determines the best cell and facing for loading.             *
 *                                                                                             *
 *    This routine examines the unit and adjacent cells in order to find the best facing       *
 *    for the transport and best staging cell for the potential passengers. This location is   *
 *    modified by adjacent cell passability and direction of the potential passenger.          *
 *                                                                                             *
 * INPUT:   passenger   -- Pointer to the potential passenger.                                 *
 *                                                                                             *
 *          moveto      -- Reference to the cell number that specifies where the potential     *
 *                         passenger should move to first.                                     *
 *                                                                                             *
 * OUTPUT:  Returns with the direction the transport should face before opening the transport  *
 *          door.                                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType UnitClass::Desired_Load_Dir(ObjectClass * passenger, CELL & moveto) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	/*
	**	Determine the ideal facing that provides the least resistance. This would be the direction
	**	of the potential passenger or the current transport facing if it is going to unload.
	*/
	DirType faceto;
	if (passenger != NULL) {
		faceto = Direction(passenger);
	} else {
		faceto = PrimaryFacing.Current() + DIR_S;
	}

	/*
	**	Sweep through the adjacent cells in order to find the best candidate.
	*/
	FacingType bestdir = FACING_N;
	int bestval = -1;
	for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
		int value = 0;
		CELL cellnum = Adjacent_Cell(Coord_Cell(Coord), face);

		/*
		**	Base the initial value of the potential cell according to whether the passenger is
		**	allowed to enter the cell. If it can't, then give such a negative value to the
		**	cell so that it is prevented from ever choosing that cell for load/unload.
		*/
		if (passenger != NULL) {
			value = (passenger->Can_Enter_Cell(cellnum) == MOVE_OK || Coord_Cell(passenger->Coord) == cellnum) ? 128 : -128;
		} else {
			CellClass * cell = &Map[cellnum];
			if (Ground[cell->Land_Type()].Cost[SPEED_FOOT] == 0 || cell->Flag.Occupy.Building || cell->Flag.Occupy.Vehicle || cell->Flag.Occupy.Monolith || (cell->Flag.Composite & 0x01F) == 0x01F) {
				value = -128;
			} else {
				if (cell->Cell_Techno() && !House->Is_Ally(cell->Cell_Techno())) {
					value = -128;
				} else {
					value = 128;
				}
			}
		}

		/*
		**	Give more weight to the cells that require the least rotation of the transport or the
		**	least roundabout movement for the potential passenger.
		*/
		value -= (int)ABS((int)(signed char)Facing_Dir(face) - (int)(signed char)faceto);
		if (face == FACING_S) {
			value -= 100;
		}
		if (face == FACING_SW || face == FACING_SE) value += 64;

		/*
		**	If the value for the potential cell is greater than the last recorded potential
		**	value, then record this cell as the best candidate.
		*/
		if (bestval == -1 || value > bestval) {
			bestval = value;
			bestdir = face;
		}
	}

	/*
	**	If a suitable direction was found, then return with the direction value.
	*/
	moveto = 0;
	if (bestval > 0) {
		static DirType _desired_to_actual[FACING_COUNT] = {DIR_S, DIR_SW, DIR_NW, DIR_NW, DIR_NE, DIR_NE, DIR_NE, DIR_SE};

		moveto = Adjacent_Cell(Coord_Cell(Coord), bestdir);
		return(_desired_to_actual[bestdir]);
	}
	return(DIR_S);
}


/***********************************************************************************************
 * UnitClass::Flag_Attach -- Attaches a house flag to this unit.                               *
 *                                                                                             *
 *    This routine will attach a house flag to this unit.                                      *
 *                                                                                             *
 * INPUT:   house -- The house that is having its flag attached to it.                         *
 *                                                                                             *
 * OUTPUT:  Was the house flag successfully attached to this unit?                             *
 *                                                                                             *
 * WARNINGS:   A unit can only carry one flag at a time. This might be a reason for failure    *
 *             of this routine.                                                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Flag_Attach(HousesType house)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	if (house != HOUSE_NONE && Flagged == HOUSE_NONE)	{
		Flagged = house;
		Mark(MARK_CHANGE);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * UnitClass::Flag_Remove -- Removes the house flag from this unit.                            *
 *                                                                                             *
 *    This routine will remove the house flag that is attached to this unit.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was the flag successfully removed?                                                 *
 *                                                                                             *
 * WARNINGS:   This routine doesn't put the flag into a new location. That operation must      *
 *             be performed or else the house flag will cease to exist.                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Flag_Remove(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	if (Flagged != HOUSE_NONE) {
		Flagged = HOUSE_NONE;
		Mark(MARK_CHANGE);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * UnitClass::Pip_Count -- Fetches the number of pips to display on unit.                      *
 *                                                                                             *
 *    This routine is used to fetch the number of "fullness" pips to display on the unit.      *
 *    This will either be the number of passengers or the percentage full (in 1/5ths) of       *
 *    a harvester.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of pips to draw on this unit.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Pip_Count(void) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	if (Class->Max_Passengers() > 0) {
		return(How_Many());
	}

	if (*this == UNIT_MINELAYER) {
		int retval = 0;
		if (Ammo > 0) {
			retval = Class->Max_Pips() * fixed(Ammo, Class->MaxAmmo);
			if (!retval) retval = 1;
		}
		return(retval);
	}

	if (*this == UNIT_HARVESTER) {
		return((Gold + Gems) / 4);
	}

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (*this == UNIT_CHRONOTANK) {
		int fulldur = ChronoTankDuration * TICKS_PER_MINUTE;
		return( (fulldur - MoebiusCountDown) / (fulldur / 5));
	}
#endif
	return(0);
}


/***********************************************************************************************
 * UnitClass::APC_Close_Door -- Closes an APC door.                                            *
 *                                                                                             *
 *    This routine will initiate closing of the APC door.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::APC_Close_Door(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	Close_Door(10, 2);
}


/***********************************************************************************************
 * UnitClass::APC_Open_Door -- Opens an APC door.                                              *
 *                                                                                             *
 *    This routine will initiate opening of the APC door.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::APC_Open_Door(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	if (!IsDriving && !IsRotating) {
		if (PrimaryFacing == DIR_NW || PrimaryFacing == DIR_NE) {
			Open_Door(10, 2);
		} else {
			Open_Door(1, 2);
		}
	}
}


/***********************************************************************************************
 * UnitClass::Crew_Type -- Fetches the kind of crew that this object produces.                 *
 *                                                                                             *
 *    When a unit is destroyed, a crew member might be generated. This routine will return     *
 *    with the infantry type to produce for this unit. This routine will be called for every   *
 *    survivor that is generated.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a suggested infantry type to generate as a survivor from this unit.   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryType UnitClass::Crew_Type(void) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	if (Class->PrimaryWeapon == NULL) {
		if (Percent_Chance(50)) {
			return(INFANTRY_C1);
		} else {
			return(INFANTRY_C7);
		}
	}
	return(DriveClass::Crew_Type());
}


/***********************************************************************************************
 * UnitClass::Mission_Repair -- Handles finding and proceeding on a repair mission.            *
 *                                                                                             *
 *    This mission handler will look for a repair facility. If one is found then contact       *
 *    is established and then the normal Mission_Enter logic is performed. The repair facility *
 *    will take over the actual repair coordination process.                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of game frames to delay before calling this routine again.      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Mission_Repair(void)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	BuildingClass * nearest = Find_Docking_Bay(STRUCT_REFINERY, true);

	IsHarvesting = false;

	/*
	**	If there is no available repair facility, then check to see if there
	**	are any repair facilities at all. If not, then enter this unit
	**	into idle state.
	*/
	if (nearest == NULL) {
		if (!(House->ActiveBScan & STRUCTF_REFINERY)) {
			Enter_Idle_Mode();
		}
	} else {

		/*
		**	Try to establish radio contact with the repair facility. If contact
		**	was established, then proceed with normal enter mission, which handles
		**	the repair process.
		*/
		if (Transmit_Message(RADIO_HELLO, nearest) == RADIO_ROGER) {
			Assign_Mission(MISSION_ENTER);
			return(1);
		}
	}

	/*
	**	If no action could be performed at this time, then wait
	**	around for a bit before trying again.
	*/
	return(MissionControl[Mission].Normal_Delay());
}


/***********************************************************************************************
 * UnitClass::Fire_Direction -- Determines the direction of firing.                            *
 *                                                                                             *
 *    This routine will return with the facing that a projectile will travel if it was         *
 *    fired at this instant. The facing should match the turret facing for those units         *
 *    equipped with a turret. If the unit doesn't have a turret, then it will be the facing    *
 *    of the body.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the default firing direction for a projectile.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType UnitClass::Fire_Direction(void) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	if (Class->IsTurretEquipped) {
		if (*this == UNIT_V2_LAUNCHER) {
			int diff1 = SecondaryFacing.Difference(DIR_E);
			int diff2 = SecondaryFacing.Difference(DIR_W);
			diff1 = ABS(diff1);
			diff2 = ABS(diff2);
			int diff = min(diff1, diff2);
			int adj = Fixed_To_Cardinal(ABS(SecondaryFacing.Difference(DIR_N)), 64-diff);
			if (SecondaryFacing.Difference(DIR_N) < 0) {
				return(DirType)(SecondaryFacing - (DirType)adj);
			} else {
				return(DirType)(SecondaryFacing + (DirType)adj);
			}
		}
		return(SecondaryFacing.Current());
	}

	return(DriveClass::Fire_Direction());
}


/***********************************************************************************************
 * UnitClass::Ok_To_Move -- Queries whether the vehicle can move.                              *
 *                                                                                             *
 *    This virtual routine is used to determine if the vehicle is allowed                      *
 *    to start moving. It is typically called when the vehicle desires                         *
 *    to move but needs confirmation from the turret logic before                              *
 *    proceeding. This happens when dealing with a vehicle that must have                      *
 *    its turret face the same direction as the body before the vehicle                        *
 *    may begin movement.                                                                      *
 *                                                                                             *
 * INPUT:   dir      -- The facing the unit wants to travel in.                                *
 *                                                                                             *
 * OUTPUT:  bool; Can the unit begin forward movement now?                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Ok_To_Move(DirType dir) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	if (Class->IsLockTurret) {
		if (IsRotating) {
			return(false);
		} else {
			if (SecondaryFacing.Difference(dir)) {
				((UnitClass *)this)->SecondaryFacing.Set_Desired(dir);
				return(false);
			}
		}
	}
	return(true);
}


/***********************************************************************************************
 * UnitClass::Can_Fire -- Determines if turret can fire upon target.                           *
 *                                                                                             *
 *    This routine determines if the turret can fire upon the target                           *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   target   -- The target to fire upon.                                               *
 *                                                                                             *
 *          which    -- Which weapon to use to determine legality to fire. 0=primary,          *
 *                      1=secondary.                                                           *
 *                                                                                             *
 * OUTPUT:  Returns the fire status type that indicates if firing is allowed and if not, why.  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/26/1994 JLB : Created.                                                                 *
 *   06/01/1994 JLB : Returns reason why it can't fire.                                        *
 *=============================================================================================*/
FireErrorType UnitClass::Can_Fire(TARGET target, int which) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	DirType			dir;					// The facing to impart upon the projectile.
	int				diff;
	FireErrorType	fire = DriveClass::Can_Fire(target, which);

	if (fire == FIRE_OK) {
		WeaponTypeClass const * weapon = (which == 0) ? Class->PrimaryWeapon : Class->SecondaryWeapon;

		/*
		**	If this unit cannot fire while moving, then bail.
		*/
		if ((Class->IsNoFireWhileMoving /*!Class->IsTurretEquipped || Class->IsLockTurret*/) && Target_Legal(NavCom)) {
			return(FIRE_MOVING);
		}

		/*
		**	If the turret is rotating and the projectile isn't a homing type, then
		**	firing must be delayed until the rotation stops.
		*/
		if (!IsFiring && IsRotating && weapon->Bullet->ROT == 0) {
			return(FIRE_ROTATING);
		}

		dir = Direction(target);

		/*
		**	Determine if the turret facing isn't too far off of facing the target.
		*/
		if (Class->IsTurretEquipped) {
			diff = SecondaryFacing.Difference(dir);
		} else {
			diff = PrimaryFacing.Difference(dir);
		}
		diff = ABS(diff);

		if (weapon->Bullet->ROT != 0) {
			diff >>= 2;
		}
		if (diff < 8) {
			return(DriveClass::Can_Fire(target, which));
		}
		return(FIRE_FACING);
	}
	return(fire);
}


/***********************************************************************************************
 * UnitClass::Fire_At -- Try to fire upon the target specified.                                *
 *                                                                                             *
 *    This routine is the auto-fire logic for the turret. It will check                        *
 *    to see if firing is technically legal given the specified target.                        *
 *    If it is legal to fire, it does so. It is safe to call this routine                      *
 *    every game tick.                                                                         *
 *                                                                                             *
 * INPUT:   target   -- The target to fire upon.                                               *
 *                                                                                             *
 *          which    -- Which weapon to use when firing. 0=primary, 1=secondary.               *
 *                                                                                             *
 * OUTPUT:  bool; Did firing occur?                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BulletClass * UnitClass::Fire_At(TARGET target, int which)
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	BulletClass * bullet = NULL;
	WeaponTypeClass const * weap = (which == 0) ? Class->PrimaryWeapon : Class->SecondaryWeapon;
	if (weap == NULL) return(NULL);

	if (Can_Fire(target, which) == FIRE_OK) {
		bullet = DriveClass::Fire_At(target, which);

		if (bullet != NULL) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
if(Class->Type == UNIT_DEMOTRUCK && IsActive) delete this;
#endif
			/*
			**	Possible reload timer set.
			*/
			if ((*this == UNIT_V2_LAUNCHER) && Reload == 0) {
				Reload = TICKS_PER_SECOND * 30;
			}
		}
	}

	return(bullet);
}


/***********************************************************************************************
 * UnitClass::Class_Of -- Fetches a reference to the class type for this object.               *
 *                                                                                             *
 *    This routine will fetch a reference to the TypeClass of this object.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with reference to the type class of this object.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectTypeClass const & UnitClass::Class_Of(void) const
{
	assert(Units.ID(this) == ID);
	assert(IsActive);

	return(*Class);
}


/***********************************************************************************************
 * UnitClass::Tiberium_Load -- Determine the Tiberium load as a percentage.                    *
 *                                                                                             *
 *    Use this routine to determine what the Tiberium load is (as a fixed point percentage).   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current "fullness" rating for the object.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/17/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
fixed UnitClass::Tiberium_Load(void) const
{
	assert(IsActive);

	if (*this == UNIT_HARVESTER) {
		return(fixed(Tiberium, Rule.BailCount));
	}
	return(0);
}


/***********************************************************************************************
 * UnitClass::Offload_Tiberium_Bail -- Offloads one Tiberium quantum from the object.          *
 *                                                                                             *
 *    This routine will offload one Tiberium packet/quantum/bail from the object. Multiple     *
 *    calls to this routine are needed in order to fully offload all Tiberium.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of credits offloaded for the one call. If zero is returned,*
 *          then this indicates that all Tiberium has been offloaded.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Offload_Tiberium_Bail(void)
{
	assert(IsActive);

	if (Tiberium) {
		Tiberium--;

// MBL 05.15.2020: Note, if the below code is ever reeanbled for some ready, make sure to see fix in 
// Tiberian Dawn's DriveClass::Offload_Tiberium_Bail() for AI players

#ifdef TOFIX
		if (House->IsHuman) {
			return(UnitTypeClass::FULL_LOAD_CREDITS/UnitTypeClass::STEP_COUNT);
		}
		return(UnitTypeClass::FULL_LOAD_CREDITS+(UnitTypeClass::FULL_LOAD_CREDITS/3)/UnitTypeClass::STEP_COUNT);
#endif
	}
	return(0);
}


/***********************************************************************************************
 * UnitClass::Approach_Target -- Handles approaching the target in order to attack it.         *
 *                                                                                             *
 *    This routine will check to see if the target is infantry and it can be overrun. It will  *
 *    try to overrun the infantry rather than attack it. This only applies to computer         *
 *    controlled vehicles. If it isn't the infantry overrun case, then it falls into the       *
 *    base class for normal (complex) approach algorithm.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/17/1995 JLB : Created.                                                                 *
 *   07/12/1995 JLB : Flamethrower tanks don't overrun -- their weapon is better.              *
 *=============================================================================================*/
void UnitClass::Approach_Target(void)
{
	assert(IsActive);

	/*
	**	Only if there is a legal target should the approach check occur.
	*/
	if (!House->IsHuman && Target_Legal(TarCom) && !Target_Legal(NavCom)) {

		/*
		**	Special case:
		**	If this is for a unit that can crush infantry, and the target is
		**	infantry, AND the infantry is pretty darn close, then just try
		**	to drive over the infantry instead of firing on it.
		*/
		TechnoClass * target = As_Techno(TarCom);
		if (Class->IsCrusher && Distance(TarCom) < Rule.CrushDistance && target && ((TechnoTypeClass const &)(target->Class_Of())).IsCrushable) {
			Assign_Destination(TarCom);
			return;
		}
	}

	/*
	**	In the other cases, uses the more complex "get to just within weapon range"
	**	algorithm.
	*/
	DriveClass::Approach_Target();
}


/***********************************************************************************************
 * DriveClass::Overrun_Square -- Handles vehicle overrun of a cell.                            *
 *                                                                                             *
 *    This routine is called when a vehicle enters a square or when it is about to enter a     *
 *    square (controlled by parameter). When a vehicle that can crush infantry enters a        *
 *    cell that contains infantry, then the infantry will be destroyed (regardless of          *
 *    affiliation). When a vehicle threatens to overrun a square, all occupying infantry       *
 *    will attempt to get out of the way.                                                      *
 *                                                                                             *
 * INPUT:   cell     -- The cell that is, or soon will be, entered by a vehicle.               *
 *                                                                                             *
 *          threaten -- Don't kill, but just threaten to enter the cell.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Overrun_Square(CELL cell, bool threaten)
{
	assert(IsActive);

	CellClass * cellptr = &Map[cell];

	if (Class->IsCrusher) {
		if (threaten) {

			/*
			**	If the cell contains infantry, then they will panic when a vehicle tries
			**	drive over them. Have the infantry run away instead.
			*/
			if (cellptr->Flag.Composite & 0x1F) {

				/*
				**	Scattering is controlled by the game difficulty level.
				*/
				cellptr->Incoming(0, true);
			}
		} else {
			ObjectClass * object = cellptr->Cell_Occupier();
			int crushed = false;
			while (object != NULL) {
				if (object->Class_Of().IsCrushable && !House->Is_Ally(object) && Distance(object->Center_Coord()) < CELL_LEPTON_W/2) {

#ifdef OBSOLETE
					/*
					** If we're running over infantry, let's see if the infantry we're
					** squashing is a thief trying to capture us.  If so, let him succeed.
					*/
					if (object->What_Am_I() == RTTI_INFANTRY && *((InfantryClass *)object) == INFANTRY_THIEF && ((InfantryClass *)object)->NavCom == As_Target()) {
						ObjectClass * next = object->Next;
						IsOwnedByPlayer = ((InfantryClass *)object)->IsOwnedByPlayer;
						House = ((InfantryClass *)object)->House;
						delete object;
						object = next;
					} else {
#endif
						ObjectClass * next = object->Next;
						crushed = true;

						/*
						** Record credit for the kill(s)
						*/
						Sound_Effect(VOC_SQUISH, Coord);
						if (object->Height == 0) {
							AnimClass* anim = new AnimClass(ANIM_CORPSE1, object->Center_Coord());
							if (anim != NULL) {
								anim->OwnerHouse = object->Owner();
							}
						}
						object->Record_The_Kill(this);
						object->Mark(MARK_UP);
						object->Limbo();
						delete object;
						//new OverlayClass(OVERLAY_SQUISH, Coord_Cell(Coord));

						object = next;
#ifdef OBSOLETE
					}
#endif
				} else {
					object = object->Next;
				}
			}
			if (crushed) Do_Uncloak();
		}
	}
}


/***********************************************************************************************
 * UnitClass::Assign_Destination -- Assign a destination to a unit.                            *
 *                                                                                             *
 *    This will assign the specified destination to the unit. It is presumed that doing is     *
 *    is all that is needed in order to cause the unit to move to the specified destination.   *
 *                                                                                             *
 * INPUT:   target   -- The target (location) to move to.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Assign_Destination(TARGET target)
{
	assert(IsActive);

	/*
	**	Abort early if there is anything wrong with the parameters
	**	or the unit already is assigned the specified destination.
	*/
	if (target == NavCom) return;

	/*
	**	Transport vehicles must tell all passengers that are about to load, that they
	**	cannot proceed. This is accomplished with a radio message to this effect.
	*/
	if (In_Radio_Contact() && Class->Max_Passengers() > 0 && Contact_With_Whom()->Is_Infantry()) {
		Transmit_Message(RADIO_OVER_OUT);
	}

	BuildingClass * b = As_Building(target);

	/*
	**	Handle entry logic here.
	*/
	if (Mission == MISSION_ENTER || MissionQueue == MISSION_ENTER) {

		/*
		**	If not already in radio contact (presumed with the transport), then
		**	either try to establish contact if allowed, or just move close and
		**	wait until radio contact can be established.
		*/
		if (!In_Radio_Contact()) {
			if (b != NULL) {

				/*
				**	Determine if the transport is already in radio contact. If so, then just move
				**	toward the transport and try to establish contact at a later time.
				*/
				if (b->In_Radio_Contact()) {
// TCTCTC -- call for an update from the transport to get a good rendezvous position.
					ArchiveTarget = target;

/*
**	HACK ALERT: The repair bay is counting on the assignment of the NavCom by this routine.
**	The refinery must NOT have the navcom assigned by this routine.
*/
if (*b != STRUCT_REPAIR) {
	target = TARGET_NONE;
}
				} else {
					if (Transmit_Message(RADIO_DOCKING, b) != RADIO_ROGER) {
						Transmit_Message(RADIO_OVER_OUT);
						if (*b == STRUCT_REPAIR) {
							ArchiveTarget = target;
						}
					}
if (*b != STRUCT_REPAIR) {
	ArchiveTarget = target;
	target = TARGET_NONE;
}
				}
			} else {
				TechnoClass * techno = As_Techno(target);
				if (techno != NULL) {

					/*
					**	Determine if the transport is already in radio contact. If so, then just move
					**	toward the transport and try to establish contact at a later time.
					*/
					if (techno->In_Radio_Contact()) {
	// TCTCTC -- call for an update from the transport to get a good rendezvous position.

						ArchiveTarget = target;
					} else {
						if (Transmit_Message(RADIO_HELLO, techno) == RADIO_ROGER) {
							if (Transmit_Message(RADIO_DOCKING) != RADIO_ROGER) {
								Transmit_Message(RADIO_OVER_OUT);
							} else {
								//BG: keep retransmitted navcom from radio-move-here.
								return;
							}
						}
					}
				}

			}
		} else {
			Path[0] = FACING_NONE;
		}
	} else {
		Path[0] = FACING_NONE;
	}

	/*
	**	If the player clicked on a friendly repair facility and the repair
	**	facility is currently not involved with some other unit (radio or unloading).
	*/
	if (b != NULL && *b == STRUCT_REPAIR) {
		if (b->In_Radio_Contact() && (b->Contact_With_Whom() != this) ) {
//			if (target != NULL) {
				ArchiveTarget = target;
//			}
//			target = TARGET_NONE;
		} else {

			/*
			**	Establish radio contact protocol. If the facility responds correctly,
			**	then remain in radio contact and proceed toward the desired destination.
			*/
			if (Transmit_Message(RADIO_HELLO, b) == RADIO_ROGER) {

				/*
				**	Last check to make sure that the loading square is free from permanent
				**	occupation (such as a building).
				*/
				CELL cell = (CELL)(Coord_Cell(b->Center_Coord()) + (MAP_CELL_W-1));
				if (Ground[Map[cell].Land_Type()].Cost[Techno_Type_Class()->Speed] > 0) {
					if (Transmit_Message(RADIO_DOCKING) == RADIO_ROGER) {
						FootClass::Assign_Destination(target);
						Path[0] = FACING_NONE;
						return;
					}

					/*
					**	Failure to establish a docking relationship with the refinery.
					**	Bail & await further instructions.
					*/
					Transmit_Message(RADIO_OVER_OUT);
				}
			}
		}
	}

	DriveClass::Assign_Destination(target);
}


/***********************************************************************************************
 * UnitClass::Greatest_Threat -- Fetches the greatest threat for this unit.                    *
 *                                                                                             *
 *    This routine will search the map looking for a good target to attack. It takes into      *
 *    consideration the type of weapon it is equipped with.                                    *
 *                                                                                             *
 * INPUT:   threat   -- The threat type to search for.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with a target value of the target that this unit should pursue. If there   *
 *          is no suitable target, then TARGET_NONE is returned.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET UnitClass::Greatest_Threat(ThreatType threat) const
{
	assert(IsActive);
	if (Class->PrimaryWeapon != NULL) {
		threat = threat | Class->PrimaryWeapon->Allowed_Threats();
	}
	if (Class->SecondaryWeapon != NULL) {
		threat = threat | Class->SecondaryWeapon->Allowed_Threats();
	}

#ifdef OBSOLETE
	if (House->IsHuman) {
		threat = threat & ~THREAT_BUILDINGS;
	}
#endif

	return(FootClass::Greatest_Threat(threat));
}


/***********************************************************************************************
 * UnitClass::Read_INI -- Reads units from scenario INI file.                                  *
 *                                                                                             *
 *    This routine is used to read all the starting units from the                             *
 *    scenario control INI file. The units are created and placed on the                       *
 *    map by this routine.                                                                     *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Housename, Typename, Strength, Coord, Facingnum, Missionname, Triggername              *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the loaded scenario INI file.                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Read_INI(CCINIClass & ini)
{
	UnitClass	* unit;			// Working unit pointer.
	HousesType	inhouse;			// Unit house.
	UnitType		classid;			// Unit class.
	char			buf[128];

	int len = ini.Entry_Count(INI_Name());

	for (int index = 0; index < len; index++) {
		char const * entry = ini.Get_Entry(INI_Name(), index);

		ini.Get_String(INI_Name(), entry, NULL, buf, sizeof(buf));

		inhouse = HouseTypeClass::From_Name(strtok(buf, ","));
		if (inhouse != HOUSE_NONE) {
			classid = UnitTypeClass::From_Name(strtok(NULL, ","));

			if (classid != UNIT_NONE) {

				if (HouseClass::As_Pointer(inhouse) != NULL) {
					unit = new UnitClass(classid, inhouse);
					if (unit != NULL) {

						/*
						**	Read the raw data.
						*/
						int strength = atoi(strtok(NULL, ",\r\n"));

						CELL cell = atoi(strtok(NULL, ",\r\n"));

						COORDINATE coord = Cell_Coord(cell);

						DirType dir = (DirType)atoi(strtok(NULL, ",\r\n"));
						MissionType mission = MissionClass::Mission_From_Name(strtok(NULL, ",\n\r"));

						unit->Trigger = NULL;
						TriggerTypeClass * tp = TriggerTypeClass::From_Name(strtok(NULL,",\r\n"));
						if (tp != NULL) {
							TriggerClass * tt = Find_Or_Make(tp);
							if (tt != NULL) {
								tt->AttachCount++;
								unit->Trigger = tt;
							}
						}

						if (unit->Unlimbo(coord, dir)) {
							unit->Strength = (int)unit->Class->MaxStrength * fixed(strength, 256);
							if (unit->Strength > unit->Class->MaxStrength-3) unit->Strength = unit->Class->MaxStrength;
							if (Session.Type == GAME_NORMAL || unit->House->IsHuman) {
								unit->Assign_Mission(mission);
								unit->Commence();
							} else {
								unit->Enter_Idle_Mode();
							}

						} else {

							/*
							**	If the unit could not be unlimboed, then this is a catastrophic error
							**	condition. Delete the unit.
							*/
							delete unit;
						}
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * UnitClass::Write_INI -- Store the units to the INI database.                                *
 *                                                                                             *
 *    This routine will store all the unit data to the INI database.                           *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database object to store to.                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Write_INI(CCINIClass & ini)
{
	/*
	**	First, clear out all existing unit data from the ini file.
	*/
	ini.Clear(INI_Name());

	/*
	**	Write the unit data out.
	*/
	for (int index = 0; index < Units.Count(); index++) {
		UnitClass * unit = Units.Ptr(index);
		if (unit != NULL && !unit->IsInLimbo && unit->IsActive) {
			char	uname[10];
			char	buf[128];

			sprintf(uname, "%d", index);
			sprintf(buf, "%s,%s,%d,%u,%d,%s,%s",
				unit->House->Class->IniName,
				unit->Class->IniName,
				unit->Health_Ratio()*256,
				Coord_Cell(unit->Coord),
				unit->PrimaryFacing.Current(),
				MissionClass::Mission_Name(unit->Mission),
				unit->Trigger.Is_Valid() ? unit->Trigger->Class->IniName : "None"
				);
			ini.Put_String(INI_Name(), uname, buf);
		}
	}
}


/***********************************************************************************************
 * UnitClass::Credit_Load -- Fetch the full credit value of cargo carried.                     *
 *                                                                                             *
 *    This will determine the value of the cargo carried (limited to considering only gold     *
 *    and gems) and return that value. Use this to determine how 'valuable' a harvester is.    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the credit value of the cargo load of this unit (harvester).          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Credit_Load(void) const
{
	return((Gold * Rule.GoldValue) + (Gems * Rule.GemValue));
}


/***********************************************************************************************
 * UnitClass::Should_Crush_It -- Determines if this unit should crush an object.               *
 *                                                                                             *
 *    Call this routine to determine if this unit should crush the object specified. The       *
 *    test for crushable action depends on proximity and ability of the unit. If a unit        *
 *    should crush the object, then it should be given a movement order to enter the cell      *
 *    where the object is located.                                                             *
 *                                                                                             *
 * INPUT:   it -- The object to see if it should be crushed.                                   *
 *                                                                                             *
 * OUTPUT:  bool; Should "it" be crushed by this unit?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Should_Crush_It(TechnoClass const * it) const
{
	assert(IsActive);

	/*
	**	If this unit cannot crush anything or the candidate object cannot be crushed,
	**	then it obviously should not try to crush it -- return negative answer.
	*/
	if (!Class->IsCrusher || it == NULL || !it->Techno_Type_Class()->IsCrushable) return(false);

	/*
	**	Objects that are far away should really be fired upon rather than crushed.
	*/
	if (Distance(it) > Rule.CrushDistance) return(false);

	/*
	**	Human controlled units don't automatically crush. Neither do computer controlled ones
	**	if they are at difficult setting.
	*/
	if (House->IsHuman || House->Difficulty == DIFF_HARD) return(false);

	/*
	**	If the weapon this unit is equipped with is very good against crushable objects then
	**	fire the weapon instead. It is presumed that a wood destroying weapon is good against
	**	most crushable object types (infantry).
	*/
	if (Class->PrimaryWeapon != NULL && Class->PrimaryWeapon->WarheadPtr->IsWoodDestroyer) return(false);

	/*
	**	If the house IQ indicates that crushing should not be allowed, then don't
	**	suggest that crushing be done.
	*/
	if (House->IQ < Rule.IQCrush) return(false);

	/*
	** Don't allow crushing of spies by computer-controlled vehicles.
	*/
	if (it->What_Am_I() == RTTI_INFANTRY && *(InfantryClass *)it == INFANTRY_SPY) {
		return(false);
	}

	return(true);
}


/***********************************************************************************************
 * UnitClass::Scatter -- Causes the unit to scatter to a nearby location.                      *
 *                                                                                             *
 *    This scatter logic will actually look for a nearby location rather than an adjacent      *
 *    free location. This is necessary because sometimes a unit is required to scatter more    *
 *    than one cell. A vehicle on a service depot is a prime example.                          *
 *                                                                                             *
 * INPUT:   threat   -- The coordinate that a potential threat resides. If this is a non       *
 *                      threat related scatter, then this parameter will be zero.              *
 *                                                                                             *
 *          forced   -- Should the scatter be performed even if it would be otherwise          *
 *                      inconvenient?                                                          *
 *                                                                                             *
 *          nokidding-- Should the scatter be performed even if it would otherwise be          *
 *                      illegal?                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Scatter(COORDINATE threat, bool forced, bool nokidding)
{
	assert(IsActive);

	if (Mission == MISSION_SLEEP || Mission == MISSION_STICKY || Mission == MISSION_UNLOAD) return;

	/*
	**	Certain missions prevent scattering regardless of whether it would be
	**	a good idea or not.
	*/
	if (!MissionControl[Mission].IsScatter && !forced) return;

	if (PrimaryFacing.Is_Rotating()) return;
//	if (IsRotating) return;

	if (Target_Legal(NavCom) && !nokidding) return;

	if (threat == 0) {
		Assign_Destination(::As_Target(Map.Nearby_Location(Coord_Cell(Coord), Class->Speed)));
	} else {
		DriveClass::Scatter(threat, forced, nokidding);
	}
}


/***********************************************************************************************
 * UnitClass::Limbo -- Limbo this unit.                                                        *
 *                                                                                             *
 *    This will cause the unit to go into a limbo state. If it was carrying a flag, then       *
 *    the flag will be dropped where the unit is at.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was this unit limboed?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Limbo(void)
{
	if (DriveClass::Limbo()) {
		if (Flagged != HOUSE_NONE) {
			HouseClass::As_Pointer(Flagged)->Flag_Attach(Coord_Cell(Coord));
			Flagged = HOUSE_NONE;
		}
		return(true);
	}
	return(false);
}

/*
** Updated for client/server multiplayer - ST 8/12/2019 11:46AM
*/
void UnitClass::Shroud_Regen(void)
{
	if (Class->IsGapper/*KO && !House->IsPlayerControl*/) {
		static int _xtab[]={
			   -1, 0, 1,
			-2,-1, 0, 1, 2,
			-2,-1, 0, 1, 2,
			-2,-1, 0, 1, 2,
			-2,-1, 0, 1, 2,
			-2,-1, 0, 1, 2,
			   -1, 0, 1
		};
		static int _ytab[]={
			   -3,-3,-3,
			-2,-2,-2,-2,-2,
			-1,-1,-1,-1,-1,
			 0, 0, 0, 0, 0,
			 1, 1, 1, 1, 1,
			 2, 2, 2, 2, 2,
			    3, 3, 3
		};
		int index;
		int centerx, centery;
		CELL trycell;

		// Only restore under the shroud if it's a valid field.
		if (ShroudBits != (unsigned)-1L) {
			centerx = Cell_X(ShroudCenter);
			centery = Cell_Y(ShroudCenter);
			for (index = 30; index >= 0 && ShroudBits; index--) {
				if (ShroudBits & 1) {
					trycell = XY_Cell(centerx + _xtab[index], centery + _ytab[index]);
#if(0)
					Map.Map_Cell(trycell, PlayerPtr);
#else
				Map.UnJam_Cell(trycell, House);
				Map.Map_Cell(trycell, House);
#endif
				}
				ShroudBits >>= 1;
			}
		}

		if(IsActive && Strength) {
			// Now shroud around the new center
			ShroudBits = 0L;
			ShroudCenter = Coord_Cell(Center_Coord());
			centerx = Cell_X(ShroudCenter);
			centery = Cell_Y(ShroudCenter);
			for (index = 0; index < 31; index++) {
				ShroudBits <<= 1;
				trycell = XY_Cell(centerx + _xtab[index], centery + _ytab[index]);
				if (Map[trycell].Is_Mapped(House)) {
					Map.Jam_Cell(trycell, House);
					ShroudBits |= 1;
				}
			}
		}

		/*
		** Updated for client/server multiplayer. ST - 8/12/2019 3:25PM
		*/
		if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {
			if (House->IsPlayerControl) {
				Map.Constrained_Look(Coord, 5 * CELL_LEPTON_W, PlayerPtr);
			}
		
		} else {
		
			for (int i = 0; i < Session.Players.Count(); i++) {
				HouseClass *player_house = HouseClass::As_Pointer(Session.Players[i]->Player.ID);
				if (player_house->IsHuman && player_house != House) {
					Map.Constrained_Look(Coord, 5 * CELL_LEPTON_W, player_house);
				}
			}
		}
	
	}
}


/***********************************************************************************************
 * UnitClass::Mission_Guard_Area -- Guard area logic for units.                                *
 *                                                                                             *
 *    This logic is similar to normal guard area except that APCs owned by the computer will   *
 *    try to load up with nearby infantry. This will give the computer some fake intelligence  *
 *    when playing in skirmish mode.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay to use before calling this routine again.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Mission_Guard_Area(void)
{
	assert(IsActive);

	/*
	**	Check to see if this is an APC that is largely empty and not otherwise doing anything.
	**	Such an APC should load up with infantry.
	*/
	if (Session.Type != GAME_NORMAL &&
#ifdef FIXIT_PHASETRANSPORT	//	checked - ajw 9/28/98
			(*this == UNIT_APC || *this == UNIT_PHASE ) &&
#else
			*this == UNIT_APC &&
#endif
			!Target_Legal(TarCom) &&
			!In_Radio_Contact() &&
			House->Which_Zone(this) != ZONE_NONE &&
			!House->IsHuman) {


		int needed = Class->Max_Passengers() - How_Many();
		for (int index = 0; index < Infantry.Count(); index++) {
			if (needed == 0) break;

			InfantryClass * infantry = Infantry.Ptr(index);

			if (infantry != NULL &&
					infantry->IsActive &&
					!infantry->IsInLimbo &&
					infantry->Strength > 0 &&
					infantry->House == House &&
					!Target_Legal(infantry->TarCom) &&
					!Target_Legal(infantry->NavCom) &&
					Distance(infantry) < 7 * CELL_LEPTON_W &&
					(infantry->Mission == MISSION_GUARD || infantry->Mission == MISSION_GUARD_AREA)) {

				infantry->Assign_Mission(MISSION_ENTER);
				infantry->ArchiveTarget = As_Target();
				needed--;
			}
		}
	}
	return(DriveClass::Mission_Guard_Area());
}