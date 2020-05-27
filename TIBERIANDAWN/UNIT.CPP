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

/* $Header:   F:\projects\c&c\vcs\code\unit.cpv   2.17   16 Oct 1995 16:48:28   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : August 16, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Recoil_Adjust -- Adjust pixel values in direction specified.                              *
 *   Turret_Adjust -- Turret adjustment routine for MLRS and MSAM units.                       *
 *   UnitClass::AI -- AI processing for the unit.                                              *
 *   UnitClass::APC_Close_Door -- Closes an APC door.                                          *
 *   UnitClass::APC_Open_Door -- Opens an APC door.                                            *
 *   UnitClass::Active_Click_With -- Intercepts the active click to see if deployment is possib*
 *   UnitClass::As_Target -- Returns the unit as a target value.                               *
 *   UnitClass::Blocking_Object -- Determines how a object blocks a unit                       *
 *   UnitClass::Can_Enter_Building -- Determines building entry legality.                      *
 *   UnitClass::Can_Fire -- Determines if this unit can fire.                                  *
 *   UnitClass::Can_Player_Move -- Determines if the player is legally allowed to move it.     *
 *   UnitClass::Click_With -- Handles player map clicking while this unit is selected.         *
 *   UnitClass::Crew_Type -- Fetches the kind of crew that this object produces.               *
 *   UnitClass::Debug_Dump -- Displays the status of the unit to the mono monitor.             *
 *   UnitClass::Desired_Load_Dir -- Determines the best cell and facing for loading.           *
 *   UnitClass::Draw_It -- Draws a unit object.                                                *
 *   UnitClass::Enter_Idle_Mode -- Unit enters idle mode state.                                *
 *   UnitClass::Find_LZ -- Maintenance function for transport units.                           *
 *   UnitClass::Flag_Attach -- Attaches a house flag to this unit.                             *
 *   UnitClass::Flag_Remove -- Removes the house flag from this unit.                          *
 *   UnitClass::Goto_Clear_Spot -- Finds a clear spot to deploy.                               *
 *   UnitClass::Goto_Tiberium -- Search for and head toward nearest available Tiberium patch.  *
 *   UnitClass::Harvesting -- Harvests tiberium at the current location.                       *
 *   UnitClass::Init -- Clears all units for scenario preparation.                             *
 *   UnitClass::Limbo -- Prepares vehicle and then limbos it.                                  *
 *   UnitClass::Look -- Perform map revelation from a unit's position.                         *
 *   UnitClass::Mission_Attack -- Handles the mission attack logic.                            *
 *   UnitClass::Mission_Guard -- Special guard mission override processor.                     *
 *   UnitClass::Mission_Harvest -- Handles the harvesting process used by harvesters.          *
 *   UnitClass::Mission_Hunt -- This is the AI process for aggressive enemy units.             *
 *   UnitClass::Mission_Move -- Handles special move mission overrides.                        *
 *   UnitClass::Mission_Unload -- Handles unloading cargo.                                     *
 *   UnitClass::Overlap_List -- Determines overlap list for units.                             *
 *   UnitClass::Per_Cell_Process -- Performs operations necessary on a per cell basis.         *
 *   UnitClass::Pip_Count -- Fetchs the number of pips to display on unit.                     *
 *   UnitClass::Random_Animate -- Handles random idle animation for the unit.                  *
 *   UnitClass::Read_INI -- Reads units from scenario INI file.                                *
 *   UnitClass::Receive_Message -- Handles receiving a radio message.                          *
 *   UnitClass::Remap_Table -- Fetches the remap table to use for this object.                 *
 *   UnitClass::Response_Attack -- Voice feedback when ordering the unit to attack a target.   *
 *   UnitClass::Response_Move -- Voice feedback when ordering the unit to move.                *
 *   UnitClass::Response_Select -- Voice feedback when selecting the unit.                     *
 *   UnitClass::Scatter -- Causes the unit to travel to a nearby safe cell.                    *
 *   UnitClass::Set_Speed -- Initiate unit movement physics.                                   *
 *   UnitClass::Sort_Y -- Give Y coordinate sort value for unit.                               *
 *   UnitClass::Start_Driver -- Starts driving and reserves destination cell.                  *
 *   UnitClass::Stop_Driver -- Handles removing occupation bits when driving stops.            *
 *   UnitClass::Stun -- Stuns the unit in preparation for unit removal.                        *
 *   UnitClass::Take_Damage -- Inflicts damage points on a unit.                               *
 *   UnitClass::Target_Coord -- The coordinate to use when targeting this unit.                *
 *   UnitClass::Try_To_Deploy -- The unit attempts to "deploy" at current location.            *
 *   UnitClass::UnitClass -- Constructor for units.                                            *
 *   UnitClass::Unlimbo -- Removes unit from stasis.                                           *
 *   UnitClass::Unload_Hovercraft_Process -- Handles unloading hovercraft transport.           *
 *   UnitClass::Validate -- validates unit pointer.														  *
 *   UnitClass::What_Action -- Determines what action would occur if clicked on object.        *
 *   UnitClass::What_Am_I -- Returns with the RTTI type this object is.                        *
 *   UnitClass::Write_INI -- Writes all the units out to an INI file.                          *
 *   UnitClass::delete -- Deletion operator for units.                                         *
 *   UnitClass::new -- Allocate a unit slot and adjust access arrays.                          *
 *   UnitClass::~UnitClass -- Destructor for unit objects.                                     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

/*
** This contains the value of the Virtual Function Table Pointer
*/
void * UnitClass::VTable;


/***********************************************************************************************
 * UnitClass::Validate -- validates unit pointer.															  *
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
int UnitClass::Validate(void) const
{
	int num;

	num = Units.ID(this);
	if (num < 0 || num >= UNIT_MAX) {
		Validate_Error("UNIT");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
#endif


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
		{0,1},	// N
		{0,1},
		{0,1},
		{-1,1},
		{-1,1},	// NE
		{-1,1},
		{-1,0},
		{-1,0},
		{-1,0},	// E
		{-1,0},
		{-1,-1},
		{-1,-1},
		{-1,-1},	// SE
		{-1,-1},
		{-1,-1},
		{0,-1},
		{0,-1},	// S
		{0,-1},
		{0,-1},
		{1,-1},
		{1,-1},	// SW
		{1,-1},
		{1,0},
		{1,0},
		{1,0},	// W
		{1,0},
		{1,1},
		{1,1},
		{1,1},	// NW
		{1,1},
		{0,1},
		{0,1}
	};

	int index = Facing_To_32(dir);
	x += _adjust[index].X;
	y += _adjust[index].Y;
}


/***********************************************************************************************
 * Turret_Adjust -- Turret adjustment routine for MLRS and MSAM units.                         *
 *                                                                                             *
 *    This routine adjusts the pixel coordinates specified to account for the displacement of  *
 *    the turret on the MLRS and MSAM vehicles.                                                *
 *                                                                                             *
 * INPUT:   dir   -- The direction of the body of the vehicle.                                 *
 *                                                                                             *
 *          x,y   -- References to the turret center pixel position. These will be modified as *
 *                   necessary.                                                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Turret_Adjust(DirType dir, int &x, int &y)
{
	static struct {
		signed char X,Y;
	} _adjust[32] = {
	  	{1,2},	// N
	  	{-1,1},
	  	{-2,0},
	  	{-3,0},
	  	{-3,1},	// NW
	  	{-4,-1},
	  	{-4,-1},
	  	{-5,-2},
	  	{-5,-3},	// W
	  	{-5,-3},
	  	{-3,-3},
	  	{-3,-4},
	  	{-3,-4},	// SW
	  	{-3,-5},
	  	{-2,-5},
	  	{-1,-5},
	  	{0,-5},	// S
	  	{1,-6},
	  	{2,-5},
	  	{3,-5},
	  	{4,-5},	// SE
	  	{6,-4},
	  	{6,-3},
	  	{6,-3},
	  	{6,-3},	// E
	  	{5,-1},
	  	{5,-1},
	  	{4,0},
	  	{3,0},	// NE
	  	{2,0},
	  	{2,1},
	  	{1,2}
	};

	int index = Facing_To_32(dir);
	x += _adjust[index].X;
	y += _adjust[index].Y;
}


/***********************************************************************************************
 * UnitClass::As_Target -- Returns the unit as a target value.                                 *
 *                                                                                             *
 *    This routine will convert the unit into a target value that is then returned. Target     *
 *    values are typically used for navigation and other computer uses.                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with target value of unit.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET UnitClass::As_Target(void) const
{
	Validate();
	return(Build_Target(KIND_UNIT, Units.ID(this)));
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
void UnitClass::Debug_Dump(MonoClass *mono) const
{
	Validate();
	mono->Set_Cursor(0, 0);
	mono->Print(
		"ÚName:ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂMission:ÄÄÄÂTarCom:ÂNavCom:ÂRadio:ÂCoord:ÄÄÂHeadTo:ÄÂSt:Ä¿\n"
		"³                   ³           ³       ³       ³      ³        ³        ³    ³\n"
		"ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂNÂYÂHealth:ÄÂBody:ÂTurret:ÂSpeed:ÂPath:ÁÄÄÄÄÄÄÂCargo:ÄÄÄÄÁÄÄÄÄ´\n"
		"³Active........³ ³ ³        ³     ³       ³      ³            ³               ³\n"
		"³Limbo.........³ ³ ÃÄÄÄÄÄÄÄÄÁÄÄÄÄÄÁÄÄÄÄÄÄÄÁÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n"
		"³Owned.........³ ³ ³Last Message:                                             ³\n"
		"³Discovered....³ ³ ÃTimer:ÂArm:ÂTrack:ÂTiberium:ÂFlash:ÂStage:ÂTeam:ÄÄÄÄÂArch:´\n"
		"³Selected......³ ³ ³      ³    ³      ³         ³      ³      ³         ³     ³\n"
		"³Teathered.....³ ³ ÃÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÁÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÙ\n"
		"³Locked on Map.³ ³ ³                                                           \n"
		"³Turret Locked.³ ³ ³                                                           \n"
		"³Is A Loaner...³ ³ ³                                                           \n"
		"³Deploying.....³ ³ ³                                                           \n"
		"³Rotating......³ ³ ³                                                           \n"
		"³Firing........³ ³ ³                                                           \n"
		"³Driving.......³ ³ ³                                                           \n"
		"³To Look.......³ ³ ³                                                           \n"
		"³Recoiling.....³ ³ ³                                                           \n"
		"³To Display....³ ³ ³                                                           \n"
		"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÁÄÙ                                                           \n");
	mono->Set_Cursor(1, 1);mono->Printf("%s:%s", House->Class->IniName, Class->IniName);
	CargoClass::Debug_Dump(mono);
	MissionClass::Debug_Dump(mono);
	TarComClass::Debug_Dump(mono);
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
	Validate();
	if (IsTethered && *this == UNIT_HOVER) {
		return(Coord_Add(Coord, 0xFF800000L));
	}
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
	Validate();

	/*
	**	Act on new orders if the unit is at a good position to do so.
	*/
	if (!IsDriving && Is_Door_Closed() /*Mission != MISSION_UNLOAD*/) {
		Commence();
	}

	TarComClass::AI();

	/*
	**	Delete this unit if it finds itself off the edge of the map and it is in
	**	guard or other static mission mode.
	*/
	if (!Team && Mission == MISSION_GUARD && !Map.In_Radar(Coord_Cell(Coord))) {
		Stun();
		Delete_This();
		return;
	}

	/*
	**	Rocket launchers will reload every so often.
	*/
	if (*this == UNIT_MSAM && Ammo < Class->MaxAmmo) {
		if (IsDriving) {
			Reload = Reload + 1;
		} else {
			if (Reload.Expired()) {
				Ammo++;
				if (Ammo < Class->MaxAmmo) {
					Reload = TICKS_PER_SECOND*30;
				}
				Mark(MARK_CHANGE);
			}
		}
	}

	/*
	**	Hover landers always are flagged to redraw since they don't record themselves
	**	on the map in the normal fashion.
	*/
	if (*this == UNIT_HOVER) {
//		Mark_For_Redraw();
//if (IsDown) Mono_Printf("*");
		Mark(MARK_CHANGE);
	}

	/*
	**	If this is a vehicle that heals itself (e.g., Mammoth Tank), then it will perform
	**	the heal logic here.
	*/
	if (*this == UNIT_HTANK && (Frame % 16) == 0 && Health_Ratio() < 0x0080) {
		Strength++;
		Mark(MARK_CHANGE);
	}
	if (*this == UNIT_VICE && Map[Coord_Cell(Coord)].Land_Type() == LAND_TIBERIUM && Health_Ratio() < 0x0100 && (Frame % 16) == 0) {
		Strength++;
		Mark(MARK_CHANGE);
	}

	/*
	**	Transporters require special logic handled here since there isn't a MISSION_WAIT_FOR_PASSENGERS
	**	mission that they can follow. Passenger loading is merely a part of their normal operation.
	*/
	if (Class->IsTransporter) {

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
	if (!IsDriving && Is_Door_Closed()/*&& Mission != MISSION_UNLOAD*/) {
		Commence();
	}

	/*
	**	Handle recoil recovery here.
	*/
	if (IsInRecoilState) {
		IsInRecoilState = false;
		Mark(MARK_CHANGE);
	}

	/*
	** For animating objects such as Visceroids, max-out their animation
	** stages here
	*/
	if (Class->IsAnimating) {
		if (!Fetch_Rate()) Set_Rate(2);
		StageClass::Graphic_Logic();
		if (Fetch_Stage() >= Get_Build_Frame_Count(Class->Get_Image_Data())-1) {
			Set_Stage(0);
		}
	}

	/*
	** for Jurassic objects, animate them if they're walking
	*/
	// Assume funpark mode might be required. ST - 10/14/2019 11:53AM
	if (Class->IsPieceOfEight) {		// && Special.IsJurassic && AreThingiesEnabled) {
		// Only animate if they're walking
		if (IsDriving || IsFiring) {
			if (!Fetch_Rate()) {
				Set_Rate(Options.Normalize_Delay(2));
				Set_Stage(0);
			}
			StageClass::Graphic_Logic();
			if (Fetch_Stage() >= ( (IsDriving || *this == UNIT_TREX || *this == UNIT_RAPT) ? 8 : 12) ) {
				Set_Stage(0);
				if (IsFiring) {
					Set_Rate(0);
					IsFiring = false;
				}
			}
		}
	}

	/*
	**	A cloaked object that is carrying the flag will always shimmer.
	*/
	if (Cloak == CLOAKED && Flagged != HOUSE_NONE) {
		Do_Shimmer();
	}

}


/***********************************************************************************************
 * UnitClass::Can_Fire -- Determines if this unit can fire.                                    *
 *                                                                                             *
 * INPUT:   target   -- The target that is desired to fire upon.                               *
 *                                                                                             *
 *          which    -- The weapon (primary=0, secondary=1) to fire.                           *
 *                                                                                             *
 * OUTPUT:  Returns with the fire error number if it cannot fire or else FIRE_OK.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
FireErrorType UnitClass::Can_Fire(TARGET target, int which) const
{
	Validate();
	FireErrorType cf;

	cf = TarComClass::Can_Fire(target, which);
	if (cf == FIRE_OK) {

		/*
		** If it's a dinosaur, when it's OK to fire we should start the firing
		** animation, but wait for the proper attack stage before starting the
		** bullet, so things won't die prematurely.
		*/
		if (Class->IsFireAnim) {
			if (!IsFiring) {
				UnitClass *nonconst;

				nonconst = (UnitClass *)this;
				nonconst->Set_Rate(Options.Normalize_Delay(2));
				nonconst->Set_Stage(0);
// Cast out the const. ST - 12/18/2018 1:03PM
((UnitClass*)this)->IsFiring = true;
				cf = FIRE_BUSY;
			} else {
				if (Fetch_Stage() < 4) cf = FIRE_BUSY;
			}
		}
	}
	return(cf);
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
	Validate();
	switch (message) {

		/*
		**	Asks if the passenger can load on this transport.
		*/
		case RADIO_CAN_LOAD:
			if (Class->IsTransporter && How_Many() < Class->Max_Passengers() && from && House->Class->House == from->Owner()) {
				return(RADIO_ROGER);
			}
			return(RADIO_NEGATIVE);

		/*
		**	The refinery has told this harvester that it should begin the backup procedure
		**	so that proper unloading may take place.
		*/
		case RADIO_BACKUP_NOW:
			TarComClass::Receive_Message(from, message, param);
			if (!IsRotating && PrimaryFacing != DIR_SW) {
				Do_Turn(DIR_SW);
			} else {
				if (!IsDriving) {
					Force_Track(BACKUP_INTO_REFINERY, Adjacent_Cell(Center_Coord(), FACING_N));
					Set_Speed(128);
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
			if (Class->IsTransporter && *this == UNIT_APC && How_Many() < Class->Max_Passengers()) {
				TarComClass::Receive_Message(from, message, param);

				/*
				**	Check if the APC is busy with an order
				*/
				bool is_busy = Mission == MISSION_MOVE;

				if ((!IsRotating || Target_Legal(TarCom)) && !IsTethered) {

					/*
					**	If the potential passenger needs someplace to go, then figure out a good
					**	spot and tell it to go.
					*/
					if (Transmit_Message(RADIO_NEED_TO_MOVE, from) == RADIO_ROGER) {

						/*
						**	If the unit has a target, don't bother with turning logic
						*/
						CELL cell;
						DirType dir;
						if (Target_Legal(TarCom)) {
							dir = PrimaryFacing.Current();
							cell = Adjacent_Cell(Coord_Cell(Coord), Direction(from));
						}
						else {
							dir = Desired_Load_Dir(from, cell);
						}

						/*
						**	If no adjacent free cells are detected, then passenger loading
						**	cannot occur. Break radio contact.
						*/
						if (cell == 0) {
							Transmit_Message(RADIO_OVER_OUT, from);
						} else {
							param = (long)::As_Target(cell);

							if (!is_busy && !IsDriving) {
								Do_Turn(dir);

								/*
								**	If it is now facing the correct direction, then open the
								**	transport doors. Close the doors if the transport is or needs
								**	to rotate.
								*/
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
								if (Is_Door_Open() || Target_Legal(TarCom)) {
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
			TarComClass::Receive_Message(from, message, param);
			return(RADIO_ROGER);

	}
	return(TarComClass::Receive_Message(from, message, param));
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
	Validate();
	/*
	**	All units must start out facing one of the 8 major directions.
	*/
	dir = Facing_Dir(Dir_Facing(dir));

	if (TarComClass::Unlimbo(coord, dir)) {

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

		/*
		**	A gun boat and a hover craft are allowed to exit the map thus we
		** flag them so they can.  This undoes the work of Techno::Unlimbo which
		** stole their IsALoaner flag.
		*/
		if (*this == UNIT_GUNBOAT || *this == UNIT_HOVER) {
			IsALoaner = true;
		}

		/*
		**	Start the gunboat animating when it is unlimboed.
		*/
		if (*this == UNIT_GUNBOAT) {
			Set_Rate(2);
			Set_Stage(0);
		}
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
ResultType UnitClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source)
{
	Validate();
	ResultType res = RESULT_NONE;

	/*
	**	Special case: If this is a laser attacking a gunboat, then the gunboat is ALWAYS toasted.
	*/
	if (*this == UNIT_GUNBOAT && warhead == WARHEAD_LASER) {
		damage = Strength*3;
	}

	/*
	**	Remember if this object was selected. If it was and it gets destroyed and it has
	**	passengers that pop out, then the passengers will inherit the select state.
	*/
	bool select = (Is_Selected_By_Player() && Is_Owned_By_Player());			// Fox for GlyphX multiplayer. ST - 4/16/2019 9:46AM

	/*
	**	In order for a this to be damaged, it must either be a unit
	**	with a crew or a sandworm.
	*/
	res = TarComClass::Take_Damage(damage, distance, warhead, source);

	if (res == RESULT_DESTROYED) {
		Death_Announcement(source);
		if (Class->Explosion != ANIM_NONE) {
			AnimType anim = Class->Explosion;

			/*
			**	SSM launchers will really explode big if they are carrying
			**	missiles at the time of the explosion.
			*/
			if (*this == UNIT_MSAM && Ammo) {
				anim = ANIM_NAPALM3;
			}

			if (*this == UNIT_TRIC || *this == UNIT_TREX || *this == UNIT_RAPT || *this == UNIT_STEG) {
				Sound_Effect(VOC_DINODIE1, Coord);
			}

			new AnimClass(anim, Coord);

			/*
			**	When the flame tank blows up, it really blows up. It is
			**	equivalent to a napalm strike.
			*/
			if (Class->Explosion == ANIM_NAPALM3) {
				Explosion_Damage(Coord, 200, source, WARHEAD_FIRE);
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
		Mark(MARK_UP);
		if (Class->IsCrew && !Class->IsTransporter) {
			if (Random_Pick(0, 1) == 0) {
				InfantryClass * i = 0;
				if (Class->Primary == WEAPON_NONE) {
					i = new InfantryClass(INFANTRY_C1, House->Class->House);
					i->IsTechnician = true;
				} else {
					i = new InfantryClass(INFANTRY_E1, House->Class->House);
				}
				if (i) {
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
			if (*this != UNIT_HOVER) {
				while (Is_Something_Attached()) {
					FootClass * object = Detach_Object();

					if (!object) break;		// How can this happen?

					/*
					**	Only infantry can run from a destroyed vehicle. Even then, it is not a sure
					**	thing.
					*/
					if (object->Is_Infantry() && object->Unlimbo(Coord, DIR_N)) {
	//					object->Strength = Random_Pick(5, (int)((InfantryClass*)object)->Class->MaxStrength/2);
						object->Look(false);
						object->Scatter(0, true);
						if (select) object->Select();
					} else {
						object->Record_The_Kill(source);
						delete object;
					}
				}
			} else {
				Kill_Cargo(source);
			}
		}

		/*
		**	When the mobile head quarters blows up, the entire side blows up.
		*/
		if (*this == UNIT_MHQ) {
			House->Flag_To_Die();
		}

		if (*this == UNIT_MCV) {
			if (House) {
				House->Check_Pertinent_Structures();
			}
		}

		/*
		**	Finally, delete the vehicle.
		*/
		Delete_This();

	} else {

		/*
		**	When damaged and below half strength, start smoking if
		**	it isn't already smoking.
		*/
		if (Health_Ratio() < 0x0080 && !IsAnimAttached && *this != UNIT_VICE && *this != UNIT_STEG && *this != UNIT_TREX && *this != UNIT_TRIC && *this != UNIT_RAPT) {
			AnimClass * anim = new AnimClass(ANIM_SMOKE_M, Coord_Add(Coord, XYP_Coord(0, -8)));
			if (anim) anim->Attach_To(this);
		}

		/*
		**	If at half damage, then start smoking or burning as appropriate.
		*/
		if (res == RESULT_HALF) {
			if (*this == UNIT_GUNBOAT) {
				AnimClass * anim = new AnimClass(ANIM_ON_FIRE_MED, Coord_Add(Coord, XYP_Coord(Random_Pick(0, 16)-8, -2)));
				if (anim) anim->Attach_To(this);
			}
		}

		/*
		**	Try to crush anyone that fires on this unit if possible. The harvester
		**	typically is the only one that will qualify here.
		*/
		if (!Team && source && !IsTethered && !House->Is_Ally(source) && (!House->IsHuman || Special.IsSmartDefense)) {

			/*
			**	Try to crush the attacker if it can be crushed by this unit and this unit is
			**	not equipped with a flame type weapon. If this unit has a weapon and the target
			**	is not very close, then fire on it instead. In easy mode, they never run over the
			**	player. In hard mode, they always do. In normal mode, they only overrun past
			**	mission #8.
			*/
			if ((Class->Primary == WEAPON_NONE || (Distance(source) < 0x0180 && BulletTypeClass::As_Reference(Weapons[Class->Primary].Fires).Warhead != WARHEAD_FIRE)) &&
						(GameToPlay != GAME_NORMAL ||
						*this != UNIT_HARVESTER ||
						BuildLevel > 8 ||
						PlayerPtr->Difficulty == DIFF_HARD) &&
						!(GameToPlay == GAME_NORMAL && PlayerPtr->Difficulty == DIFF_EASY) &&
						Class->IsCrusher &&  source->Is_Techno() && ((TechnoTypeClass const &)source->Class_Of()).IsCrushable) {

				Assign_Destination(source->As_Target());
				Assign_Mission(MISSION_MOVE);
			} else {

				/*
				**	Try to return to base if possible.
				*/
				if (*this == UNIT_HARVESTER && Pip_Count() && Health_Ratio() < 0x0080) {
					/*
					**	Find nearby refinery and head to it?
					*/
					BuildingClass * building = Find_Docking_Bay(STRUCT_REFINERY, false);

					/*
					**	Since the refinery said it was ok to load, establish radio
					**	contact with the refinery and then await docking orders.
					*/
					if (building && Transmit_Message(RADIO_HELLO, building) == RADIO_ROGER) {
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
	void * ptr = (UnitClass *)Units.Allocate();
	if (ptr) {
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
void UnitClass::operator delete(void *ptr)
{
	if (ptr) {
		((UnitClass *)ptr)->IsActive = false;
	}
	Units.Free((UnitClass *)ptr);

	//Map.Validate();
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
	if (GameActive && Class) {

#ifdef USE_RA_AI
		//
		// Added for RA AI in TD. ST - 7/26/2019 9:12AM
		//
		House->Tracking_Remove(this);
#endif // USE_RA_AI
		
		/*
		**	If there are any cargo members, delete them.
		*/
		while (Is_Something_Attached()) {
			delete Detach_Object();
		}

		Limbo();
	}
	if (GameActive && Team) Team->Remove(this);
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
	TarComClass(classid, house)
{
	Flagged = HOUSE_NONE;
	Reload = 0;
	Ammo = Class->MaxAmmo;
	IsCloakable = Class->IsCloakable;
	if (Class->IsAnimating) Set_Rate(Options.Normalize_Delay(3));

	/*
	** Keep count of the number of units created.
	*/
	if (GameToPlay == GAME_INTERNET){
		House->UnitTotals->Increment_Unit_Total((int)classid);
	}

#ifdef USE_RA_AI
	//
	// Added for RA AI in TD. ST - 7/26/2019 9:12AM
	//
	House->Tracking_Add(this);
#endif // USE_RA_AI
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
	Validate();
	if (action != What_Action(object)) {
		switch (action) {
			case ACTION_SABOTAGE:
			case ACTION_CAPTURE:
				action = ACTION_ATTACK;
				break;

			case ACTION_ENTER:
				action = ACTION_MOVE;
				break;

			default:
				action = ACTION_NONE;
				break;
		}
	}
	TarComClass::Active_Click_With(action, object);
}


void UnitClass::Active_Click_With(ActionType action, CELL cell) {TarComClass::Active_Click_With(action, cell);};


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
	Validate();
	MissionType	order;

	/*
	**	A movement mission without a NavCom would be pointless to have a radio contact since
	**	no radio coordination occurs on a just a simple movement mission.
	*/
	if (Mission == MISSION_MOVE && !Target_Legal(NavCom)) {
		Transmit_Message(RADIO_OVER_OUT);
	}

	if (Class->Primary == WEAPON_NONE) {
		if (Class->IsToHarvest) {
			if (!In_Radio_Contact() && Mission != MISSION_HARVEST) {
				if (initial || !House->IsHuman || Map[Coord_Cell(Coord)].Land_Type() == LAND_TIBERIUM) {
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
			if (IsALoaner && Class->IsTransporter && Is_Something_Attached() && !Team) {
				order = MISSION_UNLOAD;
			} else {
				order = MISSION_GUARD;
				Assign_Target(TARGET_NONE);
				Assign_Destination(TARGET_NONE);
			}
		}
	} else {
		if (Target_Legal(TarCom)) {
			order = MISSION_ATTACK;
		} else {
			if (Target_Legal(NavCom)) {
				order = MISSION_MOVE;
			} else {
				if (GameToPlay == GAME_NORMAL || House->IsHuman) {
					order = MISSION_GUARD;
				} else {
					//if (GameToPlay != GAME_NORMAL) {
#ifndef USE_RA_AI
						// Don't use MISSION_TIMED_HUNT since this can trigger the Blitz behavior
						order = MISSION_TIMED_HUNT;
#else
						//
						// Added for RA AI in TD. ST - 7/26/2019 9:12AM
						//
						// This applies only to non-human houses in a non-normal game type
						//
						order = MISSION_GUARD;

						if (Mission == MISSION_GUARD || Mission == MISSION_GUARD_AREA) {
							return;
						}

						if (!Team) {
							if (House->IQ >= Rule.IQGuardArea) {
								if (Is_Weapon_Equipped()) {
									order = MISSION_GUARD_AREA;
								}
							}
						}

#endif
					//} else {
					//	order = MISSION_HUNT;
					//}
				}
			}
		}
	}
	Assign_Mission(order);
}


/***********************************************************************************************
 * UnitClass::Find_LZ -- Maintenance function for transport units.                             *
 *                                                                                             *
 *    This is a maintenance function for transport units. It checks to see if it is loaded     *
 *    with cargo, but it doesn't know where to put it. In such a case, the unit must look      *
 *    for a landing zone (LZ) to unload the cargo. This routine should be called every so      *
 *    often. Typically, calling this routine is controlled by the scripts.                     *
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
void UnitClass::Find_LZ(void)
{
	Validate();
	CELL	cell;		// Map exit cell number.

	if (*this == UNIT_HOVER) {

		if (!IsRotating && Is_Something_Attached() && !Target_Legal(NavCom)) {
			cell = Map.Calculated_Cell(SOURCE_BEACH, House->Class->House);
			Assign_Destination(::As_Target(cell));
		}
	}
}


/***********************************************************************************************
 * UnitClass::Unload_Hovercraft_Process -- Handles unloading hovercraft transport.             *
 *                                                                                             *
 *    This is a maintenance routine to handle the special operations necessary for a           *
 *    hovercraft transport. This routine checks to see if special unloading operations are     *
 *    necessary and performs them. These operations can include unloading the transported      *
 *    object, finding a new beach cell, and rotating to a convenient unloading facing.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Has the transport finished its unloading mission?  This is true after the    *
 *                hovercraft has completely dispatched its cargo.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   07/31/1995 JLB : Second infantry unloaded MUST be the one tethered.                       *
 *=============================================================================================*/
bool UnitClass::Unload_Hovercraft_Process(void)
{
	Validate();
	bool			unloaded = false;
	FootClass	*unit;		// The unit to be unloaded.
	CELL			cell;				// Cell to unload to.

	/*
	**	If the hovercraft is currently waiting for the last unit
	**	to completely unload, then don't do anything.
	*/
	if (IsTethered || IsRotating) {
		return(false);
	}

	if (Is_Something_Attached()) {

		/*
		**	Only unload if the hovercraft has reached the beach.
		*/
		if (!Target_Legal(NavCom)) {

			cell = Coord_Cell(Adjacent_Cell(Coord, Dir_Facing(PrimaryFacing.Current())));

			unit = (FootClass *)Attached_Object();

			Mark(MARK_UP);
			if (Map.In_Radar(cell) && !Map[cell].Cell_Unit()) {

				if (unit->Can_Enter_Cell(cell, FACING_NONE) == MOVE_OK) {

					/*
					**	Place all the transported units onto the map.
					*/
					int count = 0;
					bool first = true;
					FootClass * secondary = 0;
					while (Attached_Object()) {
						FootClass * u = (FootClass *)Detach_Object();

						if (!first && !secondary) secondary = u;
						first = false;

						/*
						**	Place the unit on the map and start it heading in the right direction.
						*/
						ScenarioInit++;
						if (u->Unlimbo(Coord_Add(Coord & 0xFF00FF00L, StoppingCoordAbs[count]), DIR_N)) {
							u->Assign_Mission(MISSION_MOVE);
							u->NavCom = ::As_Target(cell);
							u->Path[0] = Dir_Facing(u->PrimaryFacing.Current());
							u->Path[1] = FACING_NONE;
							u->Set_Speed(0x80);
							u->IsUnloading = true;
							u->Look(false);
						} else {

							/*
							**	Couldn't unlimbo for some strange reason. Kill the unit off.
							*/
							delete u;
						}
						ScenarioInit--;
						count++;
					}
					if (secondary) unit = secondary;

					/*
					**	Establish radio contact bond with the transport
					**	hovercraft. This bond tells the hovercraft to
					**	not move until the unit has completely unloaded.
					*/
					if (Transmit_Message(RADIO_HELLO, unit) == RADIO_ROGER) {
						Transmit_Message(RADIO_UNLOAD);
					}
					Mark(MARK_DOWN);
					Map.Layer[LAYER_GROUND].Sort();
					Map.Layer[LAYER_GROUND].Sort();
					Map.Layer[LAYER_GROUND].Sort();
					return(false);
				} else {

					/*
					**	If the attached object cannot unload because the desired destination
					**	cell is impassable, then abort the landing. This is faked by pretending
					**	that the unload was successful. The controlling routine will cause
					**	the transport to leave.
					*/
Mark(MARK_DOWN);
return(false);
//					return(true);
				}
			}

			Mark(MARK_DOWN);
		}
	} else {
		return(true);
	}

	return(unloaded);
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
	Validate();
	Mark(MARK_UP);
	if (!Target_Legal(NavCom) && BuildingTypeClass::As_Reference(STRUCT_CONST).Legal_Placement(Coord_Cell(Adjacent_Cell(Center_Coord(), FACING_NW)))) {
		Mark(MARK_DOWN);
		return(true);
	}

	if (!Target_Legal(NavCom)) {
		
#ifndef USE_RA_AI
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
			0
		};
		int *ptr;

		ptr = &_offsets[0];
		while (*ptr) {
			CELL	cell = Coord_Cell(Coord)+*ptr++;

			if (BuildingTypeClass::As_Reference(STRUCT_CONST).Legal_Placement(cell)) {
				Assign_Destination(::As_Target(cell));
				break;
			}
		}

#else //USE_RA_AI

		//
		// Pulled in the extra offsets from RA, and started at a random position in the array. ST - 7/26/2019 2:07PM
		//
		static int _offset_count = 36;
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

		int offset_index = Random_Pick(0, _offset_count);

		for (int i=0 ; i<_offset_count ; i++) {
			CELL	cell = Coord_Cell(Coord) + _offsets[offset_index++];
			if (offset_index >= _offset_count || _offsets[offset_index] == 0) {
				offset_index = 0;
			}

			if (BuildingTypeClass::As_Reference(STRUCT_CONST).Legal_Placement(cell)) {
				Assign_Destination(::As_Target(cell));
				break;
			}
		}

#endif //USE_RA_AI
	}
	Mark(MARK_DOWN);

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
	Validate();
	if (!Target_Legal(NavCom) && !IsRotating) {
		if (*this == UNIT_MCV) {

			/*
			**	Determine if it is legal to deploy at this location. If not, tell the
			**	player.
			*/
			Mark(MARK_UP);
			if (!BuildingTypeClass::As_Reference(STRUCT_CONST).Legal_Placement(Coord_Cell(Adjacent_Cell(Center_Coord(), FACING_NW)))) {
				if (PlayerPtr == House) {
					Speak(VOX_DEPLOY);
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
			if (building) {
				if (building->Unlimbo(Adjacent_Cell(Coord, FACING_NW))) {

					/*
					**	Always reveal the construction yard to the player that owned the
					**	mobile construction vehicle.
					*/
					building->Revealed(House);

					/*
					**	Force the newly placed construction yard to be in the same strength
					**	ratio as the MCV that deployed into it.
					*/
					int ratio = Health_Ratio();
					building->Strength = Fixed_To_Cardinal(building->Class->MaxStrength, ratio);
					/*
					** Force the MCV to drop any flag it was carrying.  This will also set
					** the owner house's flag home cell (since the house's FlagHome is
					** presumably 0 at this point).
					*/
					Stun();
					Delete_This();
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
 * INPUT:   center   -- Is the unit safely at the center of a cell?  If it is merely "close"   *
 *                      to the center, then this parameter will be false.                      *
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
void UnitClass::Per_Cell_Process(bool center)
{
	Validate();
	CELL	cell = Coord_Cell(Coord);
	TechnoClass	* whom;
	HousesType house;

	/*
	**	If this is a unit that is driving onto a building then the unit must enter
	**	the building as the final step.
	*/
	whom = Contact_With_Whom();
	if (IsTethered && whom && center) {
		if (whom->What_Am_I() == RTTI_BUILDING && Mission == MISSION_ENTER) {
			if (whom == Map[cell].Cell_Building()) {
				switch (Transmit_Message(RADIO_IM_IN, whom)) {
					case RADIO_ROGER:
						break;

					case RADIO_ATTACH:
						Mark(MARK_UP);
						SpecialFlag = true;
						Limbo();
						SpecialFlag = false;
						whom->Attach(this);
						return;

					default:
						Scatter(true);
						break;
				}
			}
		}
	}

	/*
	**	When breaking away from a transport object or building, possibly
	**	scatter or otherwise begin normal unit operations.
	*/
	if (IsTethered && center && Mission != MISSION_ENTER && (*this != UNIT_APC || IsDriving || !Target_Legal(TarCom))) {
		TechnoClass * contact = Contact_With_Whom();
		if (Transmit_Message(RADIO_UNLOADED) == RADIO_RUN_AWAY) {
			if (*this == UNIT_HARVESTER && contact && contact->What_Am_I() == RTTI_BUILDING) {
				Assign_Mission(MISSION_HARVEST);
			} else if (!Target_Legal(NavCom)) {
				Scatter(0, true);
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

#ifdef OBSOLETE
	/*
	**	If this unit is on a teather, then cut it at this time so that
	**	the "parent" unit is free to proceed. Note that the parent
	**	unit might actually be a building.
	*/
	if (IsTethered && center && !Map[cell].Cell_Building()) {
		if (!Tiberium || *this != UNIT_HARVESTER) {
			Transmit_Message(RADIO_UNLOADED);
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
					Scatter(0, true);
				}
			}
		}
	}
#endif

#ifdef OBSOLETE
	/*
	** If the unit is at the center of the repair facility, and that was his
	** destination, then start him repairing.
	*/
	if (center && !IsRepairing) {
		BuildingClass * b = As_Building(NavCom);
		if (b && *b==STRUCT_REPAIR && Coord == b->Center_Coord()) {
			NavCom = 0;
			IsRepairing = true;
			Transmit_Message(RADIO_REPAIR_BEGIN_ANIM);
		}
	}
#endif

	/*
	**	Check to see if this is merely the end of a rotation for the MCV as it is
	**	preparing to deploy. In this case, it should begin its deploy process.
	*/
	if (center && IsDeploying) {
		Try_To_Deploy();
		if (!IsActive) return;			// Unit no longer exists -- bail.
	}

	/*
	**	If this is a loaner unit and is is off the edge of the
	**	map, then it gets eliminated. That is, unless it is carrying cargo. This means that
	**	it is probably carrying an incoming reinforcement and it should not be eliminated.
	*/
	if (center && IsALoaner && !Map.In_Radar(cell)) {
		if (IsReturning || !Is_Something_Attached()) {
			if (*this == UNIT_GUNBOAT) {
				CELL cell = Coord_Cell(Coord);
				if (Cell_X(cell) <= Map.MapCellX) {
					Assign_Mission(MISSION_HUNT);
					Assign_Destination(::As_Target(XY_Cell(Map.MapCellX+Map.MapCellWidth, Cell_Y(cell))));
					Set_Speed(255);
					PrimaryFacing = DIR_E;
				} else {
					Assign_Mission(MISSION_HUNT);
					Assign_Destination(::As_Target(XY_Cell(Map.MapCellX-1, Cell_Y(cell))));
					Set_Speed(255);
					PrimaryFacing = DIR_W;
				}
				Mark(MARK_DOWN);
			} else {
				Mark(MARK_DOWN);
				Stun();
				Delete_This();
				return;
			}
		}
	}

#ifdef OBSOLETE
	/*
	**	Destroy any crushable wall that is driven over by a tracked vehicle.
	*/
	CellClass * cellptr = &Map[cell];
	if (center && Class->Speed == SPEED_TRACK && cellptr->Overlay != OVERLAY_NONE) {
		OverlayTypeClass const * optr = &OverlayTypeClass::As_Reference(cellptr->Overlay);

		if (optr->IsCrushable) {
			cellptr->Reduce_Wall(100);
			cellptr->Reduce_Wall(100);
			cellptr->Reduce_Wall(100);
			cellptr->Reduce_Wall(100);
			cellptr->Reduce_Wall(100);
			cellptr->Reduce_Wall(100);
		}
	}
#endif

	/*
	**	Check to see if crushing of any unfortunate infantry is warranted.
	*/
	Overrun_Square(Coord_Cell(Coord), false);

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
	**	Act on new orders if the unit is at a good position to do so.
	*/
	if (center) {
		Commence();
	}

	/*
	**	Certain units require some setup time after they come to a halt.
	*/
	if (Special.IsDefenderAdvantage && /*center &&*/ !Target_Legal(NavCom) && Path[0] == FACING_NONE) {
		if (*this == UNIT_MLRS || *this == UNIT_ARTY || *this == UNIT_MSAM) {
			Arm = Rearm_Delay(false)*2;
		}
	}

	/*
	**	If there is a house flag here, then this unit just might pick it up.
	*/
	if (center && Flagged == HOUSE_NONE) {

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
			Map.Sight_From(House, Coord_Cell(Coord), Class->SightRange, true);		// Passed our house into Map.Sight_From since it now needs to know who it is performing the action on behalf of. ST - 3/28/2019 2:55PM
		}

		/*
		**	If the flag reaches the home cell for the player, then the flag's
		**	owner will be destroyed.
		*/
		if (cell == HouseClass::As_Pointer(Owner())->FlagHome && center) {
			house = Flagged; // Flag_Remove will clear 'Flagged', so save it
			HouseClass::As_Pointer(house)->Flag_Remove(As_Target(),true);
			HouseClass::As_Pointer(house)->Flag_To_Die();
		}
	}

	TarComClass::Per_Cell_Process(center);
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
void UnitClass::Draw_It(int x, int y, WindowNumberType window)
{
	Validate();
	int			shapenum;		// Working shape number.
	void const	*shapefile;		// Working shape file pointer.
	int			facing = Facing_To_32(PrimaryFacing.Current());
	int			tfacing = Facing_To_32(SecondaryFacing.Current());
	int			shapestart;
	int			xx, yy;

	/*
	**	Verify the legality of the unit class.
	*/
	shapefile = Class->Get_Image_Data();
	if (!shapefile) return;

	/*
	**	If drawing of this unit is not explicitly prohibited, then proceed
	**	with the render process.
	*/
	const bool is_hidden = (Visual_Character() == VISUAL_HIDDEN) && (window != WINDOW_VIRTUAL);
	if (!is_hidden) {

		/*
		**	For eight facing units, adjust the facing number accordingly.
		*/
		if (Class->IsPieceOfEight) {
			facing = Dir_Facing(PrimaryFacing.Current());
		}

		/*
		**	Calculations for special wake drawing.
		*/
		xx = x;
		yy = y;
		switch (Dir_Facing(PrimaryFacing.Current())) {
			case FACING_NE:
			case FACING_E:
			case FACING_SE:
				shapenum = UnitClass::BodyShape[tfacing] + 96;
				shapestart = 0;
				//xx -= 4;
				break;

			case FACING_W:
			default:
				shapenum = UnitClass::BodyShape[tfacing];
				shapestart = 6;
				xx += 4;
				break;
		}

		/*
		**	Some units have only four facings, but are equipped with a turret
		**	that has 32 facings.
		*/
		if (Class->IsChunkyShape) {

			/*
			**	Special wake drawing occurs here for non-virtual rendering
			*/
			if (*this == UNIT_GUNBOAT) {

				if (window != WINDOW_VIRTUAL) {
					//Added this and wake name parameters. ST - 8/20/2019 10:54AM
					CC_Draw_Shape(this, "WAKE", UnitTypeClass::WakeShapes, shapestart + (Fetch_Stage() % 6), xx-1, yy+3, window, SHAPE_CENTER|SHAPE_WIN_REL);
				}

				if (Health_Ratio() < 0x0080) shapenum += 32;
				if (Health_Ratio() < 0x0040) shapenum += 32;

			} else  {

				/*
				**	Special hovercraft shape is ALWAYS N/S.
				*/
				shapenum = 0;

				//shapenum = ((UnitClass::BodyShape[facing] + 4) / 8) & 0x03;
			}

		} else {

			/*
			**	Fetch the harvesting animation stage as appropriate.
			*/
			if (IsHarvesting && !PrimaryFacing.Is_Rotating() && !NavCom && !IsDriving) {
				static char _hstage[6] = {
					0, 1, 2, 3, 2, 1
				};
				shapenum = 32 + (((UnitClass::BodyShape[facing]+2)/4)*4)+_hstage[Fetch_Stage()%sizeof(_hstage)];
			} else {
				shapenum = UnitClass::BodyShape[facing];
				if (Class->IsAnimating) {
					shapenum = Fetch_Stage();
				}
				if (Class->IsPieceOfEight) {
					shapenum = 0;
					int numshapes = (IsDriving || (*this == UNIT_TREX) || (*this == UNIT_RAPT)) ? 8 : 12;
					if (facing)    shapenum = UnitClass::BodyShape[24+facing];
					if (IsDriving) shapenum = (Fetch_Stage()%numshapes) + 16 + (shapenum*numshapes);
					else if (IsFiring)  shapenum = (Fetch_Stage()%numshapes) + 80 + (shapenum*numshapes);
				} else {

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
		}

		/*
		**	The artillery unit should have its entire body recoil when it fires.
		*/
		if (*this == UNIT_ARTY && IsInRecoilState) {
			Recoil_Adjust(PrimaryFacing.Current(), x, y);
		}

		/*
		**	Actually perform the draw. Overlay an optional shimmer effect as necessary.
		*/
//if (*this == UNIT_HOVER) {
//	Mono_Printf("Display hover %p %d.\n", shapefile, shapenum);
//}
		Techno_Draw_Object(shapefile, shapenum, x, y, window);

		/*
		**	Special wake drawing occurs here for virtual rendering
		*/
		if (Class->IsChunkyShape && (*this == UNIT_GUNBOAT) && (window == WINDOW_VIRTUAL)) {
			//Added this and wake name parameters. ST - 8/20/2019 10:54AM
			CC_Draw_Shape(this, "WAKE", UnitTypeClass::WakeShapes, shapestart + (Fetch_Stage() % 6), xx-1, yy+3, window, SHAPE_CENTER|SHAPE_WIN_REL);
		}

		/*
		**	If there is a rotating radar dish, draw it now.
		*/
		if (Class->IsRadarEquipped) {
			shapenum = 32 + (Frame % 32);
			Techno_Draw_Object(shapefile, shapenum, x, y-5, window);
		}

		/*
		**	If there is a turret, then it must be rendered as well. This may include
		**	firing animation if required.
		*/
		if (!Class->IsChunkyShape && Class->IsTurretEquipped) {
			int x1 = x;
			int y1 = y;

			/*
			**	Determine which turret shape to use. This depends on if there
			**	is any firing animation in progress.
			*/
			shapenum = TechnoClass::BodyShape[tfacing]+32;

			/*
			**	The shape to use for the rocket launcher is dependant on the
			**	ammo remaining.
			*/
			if (*this == UNIT_MSAM) {
				if (Ammo == 0) shapenum += 64;
				if (Ammo == 1) shapenum += 32;
			}

			/*
			**	A recoiling turret moves "backward" one pixel.
			*/
			if (IsInRecoilState) {
				Recoil_Adjust(SecondaryFacing.Current(), x1, y1);
			}

			/*
			** The Mobile SAM and the Missile Launchers need their turrets moved based
			** on the facing of the vehicle.
			*/
			if (*this == UNIT_MSAM || *this == UNIT_MLRS) {
				Turret_Adjust(PrimaryFacing, x1, y1);
			}
			if (*this == UNIT_JEEP || *this == UNIT_BUGGY) {
				y1 -= 4;
			}

			/*
			**	Actually perform the draw. Overlay an optional shimmer effect as necessary.
			*/
			Techno_Draw_Object(shapefile, shapenum, x1, y1, window);
		}

		/*
		**	If this unit has "piggy back" unit(s), then render it at the same time.
		*/
		if (*this == UNIT_HOVER && Is_Something_Attached()) {
			TechnoClass * u = (TechnoClass *)Attached_Object();

			int counter = 0;
			for (;;) {
				int x1,y1;

				if (Map.Coord_To_Pixel(Coord_Add(Coord_Add(Coord, 0xFF80FF80L), StoppingCoordAbs[counter++]), x1, y1)) {
					// Pass the window through, so that the virtual window will also work. ST - 6/18/2019 12:00PM
					//u->Draw_It(x1, y1, WINDOW_TACTICAL);
					u->Draw_It(x1, y1, window);
				}
				if (!u->Next) break;
				u = (TechnoClass *)u->Next;
			}
		}
	}

	/*
	**	If this unit is carrying the flag, then draw that on top of everything else.
	*/
	if (Flagged != HOUSE_NONE) {
		CC_Draw_Shape(this, "FLAGFLY", MixFileClass::Retrieve("FLAGFLY.SHP"), Frame % 14, x, y, window, SHAPE_CENTER|SHAPE_FADING|SHAPE_GHOST, HouseClass::As_Pointer(Flagged)->Remap_Table(false, false), Map.UnitShadow, Flagged);
	}

	TarComClass::Draw_It(x, y, window);
}


/***********************************************************************************************
 * UnitClass::Goto_Tiberium -- Search for and head toward nearest available Tiberium patch.    *
 *                                                                                             *
 *    This routine is used to move a harvester to a place where it can load up with            *
 *    Tiberium. It will return true only if it can start harvesting. Otherwise, it sets        *
 *    the navigation computer toward the nearest Tiberium and lets the unit head there         *
 *    automatically.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is it located directly over a Tiberium patch?                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Tiberium_Check(CELL &center, int x, int y)
{
	Validate();
	/*
	**	If the specified offset from the origin will cause it
	**	to spill past the map edge, then abort this cell check.
	*/
	if (Cell_X(center)+x < Map.MapCellX) return(false);
	if (Cell_X(center)+x >= Map.MapCellX+Map.MapCellWidth) return(false);
	if (Cell_Y(center)+y < Map.MapCellY) return(false);
	if (Cell_Y(center)+y >= Map.MapCellY+Map.MapCellHeight) return(false);

	center = XY_Cell(Cell_X(center)+x, Cell_Y(center)+y);

	//using function for IsVisible so we have different results for different players - JAS 2019/09/30
	if ((GameToPlay != GAME_NORMAL || (!IsOwnedByPlayer || Map[center].Is_Visible(PlayerPtr)))) {
		if (!Map[center].Cell_Techno() && Map[center].Land_Type() == LAND_TIBERIUM) {
			return(true);
		}
	}
	return(false);
}


bool UnitClass::Goto_Tiberium(void)
{
	Validate();
	if (!Target_Legal(NavCom)) {
		CELL center = Coord_Cell(Center_Coord());
		if (Map[center].Land_Type() == LAND_TIBERIUM) {
			return(true);
		} else {

			/*
			**	Perform a ring search outward from the center.
			*/
			for (int radius = 1; radius < 64; radius++) {
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
	Validate();
	CELL	cell = Coord_Cell(Coord);
	CellClass * ptr = &Map[cell];

	/*
	**	Keep waiting if still heading toward a spot to harvest or the harvest timer hasn't expired yet.
	*/
	if (Target_Legal(NavCom) || !HarvestTimer.Expired() || IsDriving || IsRotating) return(true);

	if (Tiberium_Load() < 0x0100 && ptr->Land_Type() == LAND_TIBERIUM) {

		/*
		**	Lift some Tiberium from the ground. Try to lift a complete
		**	"level" of Tiberium. A level happens to be 6 steps. If there
		**	is a partial level, then lift that instead. Never lift more
		**	than the harvester can carry.
		*/
		int reducer = (ptr->OverlayData % 6) + 1;
		reducer = ptr->Reduce_Tiberium(MIN(reducer, UnitTypeClass::STEP_COUNT-Tiberium));
		Tiberium += reducer;
		Set_Stage(0);
		Set_Rate(2);

		HarvestTimer = TICKS_PER_SECOND;

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
int UnitClass::Mission_Unload(void)
{
	Validate();
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
		case UNIT_APC:
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

						if (passenger) {
							DirType toface = DIR_S + PrimaryFacing;
							bool placed = false;

							for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
								DirType newface = toface + face;
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
						if (IsDeploying) {
							Status = 2;
						} else {
							/*
							** Functionality from Red Alert for AI. ST - 7/25/2019 3:09PM
							*/			  
#ifndef USE_RA_AI
							Assign_Mission(MISSION_GUARD);
#else //USE_RA_AI
							if (!House->IsHuman && GameToPlay != GAME_NORMAL) {
								Assign_Mission(MISSION_HUNT);
							} else {
								Assign_Mission(MISSION_GUARD);
							}
#endif //USE_RA_AI
						}
					}
					break;

				case 2:
#ifdef USE_RA_AI
					/*
					** Functionality from Red Alert for AI. ST - 7/25/2019 3:09PM
					*/			  
					if (GameToPlay != GAME_NORMAL) {
						if (!IsDeploying) {
							Assign_Mission(MISSION_GUARD);
						}
					}
#endif // USE_RA_AI
					break;
			}
			return(1);

		case UNIT_HOVER:
			switch (Status) {
				case 0:
					if (Unload_Hovercraft_Process()) {
						Status = 1;
					}
					break;

				/*
				**	Hovercraft always leave the map when they finish
				**	unloading.
				*/
				case 1:
					if (Team) {
						Team->Remove(this);
					}
					Exit_Map();
					break;
			}
			break;
	}
	return(TICKS_PER_SECOND);
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
 *=============================================================================================*/
int UnitClass::Mission_Harvest(void)
{
	Validate();
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

	switch (Status) {

		/*
		**	Go and find a Tiberium field to harvest.
		*/
		case LOOKING:
			IsHarvesting = false;
			if (Goto_Tiberium()) {
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
						return(TICKS_PER_SECOND*15);
					}
				}
			}
			break;

		/*
		**	Harvest at current location until full or Tiberium exhausted.
		*/
		case HARVESTING:
			if (!Harvesting()) {
				IsHarvesting = false;
				if (Tiberium_Load() == 0x0100) {
					Status = FINDHOME;
				  	ArchiveTarget = ::As_Target(Coord_Cell(Coord));
				} else {
					if (!Goto_Tiberium() && !Target_Legal(NavCom))	{
					  	ArchiveTarget = TARGET_NONE;
						Status = FINDHOME;
					} else {
						Status = HARVESTING;
						IsHarvesting = true;
					}
				}
				return(1);
			}
			break;

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
				if (nearest && Transmit_Message(RADIO_HELLO, nearest) == RADIO_ROGER) {
					Status = HEADINGHOME;
				} else {
					ScenarioInit++;
					nearest = Find_Docking_Bay(STRUCT_REFINERY, false);
					ScenarioInit--;
					if (nearest) {
						Assign_Destination(::As_Target(nearest->Nearby_Location(this)));
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

		case GOINGTOIDLE:
			Assign_Mission(MISSION_GUARD);
			break;

	}
	return(TICKS_PER_SECOND);
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
	Validate();
	if (*this == UNIT_MCV) {
		switch (Status) {

			/*
			**	This stage handles locating a convenient spot, rotating to face the correct
			**	direction and then commencing the deployment operation.
			*/
			case 0:
				if (Goto_Clear_Spot()) {
					if (Try_To_Deploy()) {
						Status = 1;
					}
				}
				break;

			/*
			**	This stage watchdogs the deployment operation and if for some reason, the deployment
			**	is aborted (the IsDeploying flag becomes false), then it reverts back to hunting for
			**	a convenient spot to deploy.
			*/
			case 1:
				if (!IsDeploying) {
					Status = 0;
				}
				break;
		}
	} else {

		if (*this == UNIT_GUNBOAT) {
			if (!Target_Legal(NavCom)) {
				if (PrimaryFacing == DIR_W) {
					Assign_Destination( ::As_Target(XY_Cell(Map.MapCellX-1, Cell_Y(Coord_Cell(Coord)))) );
				} else {
					Assign_Destination( ::As_Target(XY_Cell(Map.MapCellX+Map.MapCellWidth, Cell_Y(Coord_Cell(Coord)))) );
				}
				Set_Speed(255);
			}
			if (!Speed) {
				Set_Speed(255);
			}
			if (!Target_Legal(TarCom) || !In_Range(TarCom, 0)) {
				Target_Something_Nearby(THREAT_AREA);
			}
		} else {
			return(TarComClass::Mission_Hunt());
		}
	}
	return(TICKS_PER_SECOND);
}


/***********************************************************************************************
 * UnitClass::Look -- Perform map revelation from a unit's position.                           *
 *                                                                                             *
 *    Reveal the map around the specified unit with the sighting range                         *
 *    associated with the specified unit.                                                      *
 *                                                                                             *
 * INPUT:   incremental -- If looking is to process only the cells in the                      *
 *                         outer ring of the unit's search radius, then                        *
 *                         set this parameter to true. This method is                          *
 *                         quite a bit faster than processing all cells,                       *
 *                         but must be used with caution.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/08/1992 JLB : Created.                                                                 *
 *   03/08/1994 JLB : Added incremental option.                                                *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
void UnitClass::Look(bool incremental)
{
	Validate();
	//if (!IsInLimbo && IsOwnedByPlayer) {				// Changed for mapping of multiple players
	if (!IsInLimbo && House && House->IsHuman) {
		int sight = Class->SightRange;

		if (sight) {
			Map.Sight_From(House, Coord_Cell(Coord), sight, (*this == UNIT_GUNBOAT) ? false : incremental);		// Passed our house into Map.Sight_From since it now needs to know who it is performing the action on behalf of. ST - 3/28/2019 2:55PM
		}
	}
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
short const * UnitClass::Overlap_List(void) const
{
	Validate();
	static short const _gunboat[] = {-3, -2, 2, 3, REFRESH_EOL};
	int	size;

	/*
	**	The gunboat is a special case.
	*/
	if (*this == UNIT_GUNBOAT) {
		return(&_gunboat[0]);
	}

	size = ICON_PIXEL_W;
	if (Is_Selected_By_Player() || IsFiring) {
		size += 24;
	}
	if (Is_Selected_By_Player() || Class->IsGigundo || IsAnimAttached) {
		size = ICON_PIXEL_W*2;
	}
	return(Coord_Spillage_List(Coord, size)+1);
}


#ifdef NEVER
/********************************************************************************************* *
 * UnitClass::Blocking_Object -- Determines how a object blocks a unit                         *
 *                                                                                             *
 *    This routine is used by the Can_Enter_Cell logic when an object is in the desired cell   *
 *    and it needs to know if that causes blockage. If blocked, this routine will return why.  *
 *                                                                                             *
 * INPUT:      TechnoClass * pointer to object that is blocking unit                           *
 *                                                                                             *
 *               CELL           the cell the unit is being blocked in                          *
 *                                                                                             *
 * OUTPUT:      MoveBitType the way that the object is blocking the unit                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/08/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
MoveBitType UnitClass::Blocking_Object(TechnoClass const *techno, CELL cell) const
{
	Validate();
	/*
	** There are some extra checks we need to make if the techno is a unit
	*/
	bool unit = (techno->What_Am_I() == RTTI_INFANTRY || techno->What_Am_I() == RTTI_UNIT);
	CellClass const * cellptr = &Map[cell];

	if (House->Is_Ally(techno)) {

		if (techno == Contact_With_Whom() && IsTethered) {
			return(MOVE_BIT_OK);
		}

		if (unit) {
			/*
			** If the unit in question has a destination than we should
			** be prepared to wait for the unit to get out of our way.
			*/
			if (((FootClass *)techno)->NavCom != TARGET_NONE) {
				int face 		= Dir_Facing(PrimaryFacing);
				int techface	= Dir_Facing(((FootClass const *)techno)->PrimaryFacing) ^4;
				if (face != techface && Distance((AbstractClass const *)techno) > 0x1FF) {
					return(MOVE_BIT_MOVING_BLOCK);
				} else {
//					Mono_Printf("Move No!\r");
					return(MOVE_BIT_NO);
				}
			}

			return(MOVE_BIT_TEMP);
		}
	} else {

		/*
		** If its an enemy unit, things are dealt with a little differently
		*/
		if (unit) {

#ifdef NEVER
			/*
			** If this is an enemy unit and we are not doing a find path then
			** we need to tell the unit to uncloak just in case it is a
			** stealth tank.
			*/
			if (!IsFindPath) {
				techno->Do_Uncloak();
			}
#endif

			/*
			** Can we just run it over?
			*/
			if (techno->Class_Of().IsCrushable && (cellptr->Flag.Composite & 0xE0) == 0 && Class->IsCrusher) {

				/*
				** Now lets run it over.
				*/
				return(MOVE_BIT_OK);
			}

			/*
			**	If the object is cloaked, then consider it passable for findpath purposes,
			**	but not so for all other cases.
			*/
			if (techno->Cloak == CLOAKED) {
				if (House == techno->House) return(MOVE_BIT_NO);
				if (IsFindPath) return(MOVE_BIT_OK);
				return(MOVE_BIT_CLOAK);
			}

			/*
			**	If our vehicle is weapon equipped, then report that the cell occupier
			**	needs only to be destroyed in order to make the cell passable.
			*/
			if (Class->Primary != WEAPON_NONE) {
				return(MOVE_BIT_DESTROYABLE);
			}
		}
	}
	return(MOVE_BIT_NO);
}
#endif


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
	Validate();
	CellClass const * cellptr = &Map[cell];

	if ((unsigned)cell >= MAP_CELL_TOTAL) return(MOVE_NO);

	/*
	**	The gunboat can always move. This prevents it from trying to move around possible hover
	**	craft blockage.
	*/
	if (*this == UNIT_GUNBOAT) return(MOVE_OK);

	/*
	**	Moving off the edge of the map is not allowed unless
	**	this is a loaner vehicle.
	*/
	if (IsLocked && !IsALoaner && !ScenarioInit && !Map.In_Radar(cell)) {
		return(MOVE_NO);
	}

	MoveType retval = MOVE_OK;

	/*
	**	Certain vehicles can drive over walls. Check for this case and
	**	and return the appropriate flag. Other units treat walls as impassable.
	*/
	if (cellptr->Overlay != OVERLAY_NONE) {
		OverlayTypeClass const * optr = &OverlayTypeClass::As_Reference(cellptr->Overlay);

		if (optr->IsCrate && !House->IsHuman) {
			return(MOVE_NO);
		}

		if (optr->IsWall) {
			if (Class->Primary != WEAPON_NONE) {
				WarheadTypeClass const * whead = &Warheads[BulletTypeClass::As_Reference(Weapons[Class->Primary].Fires).Warhead];

				if (whead->IsWallDestroyer || (whead->IsWoodDestroyer && optr->IsWooden)) {
					if (!House->IsHuman && !House->Is_Ally(cellptr->Owner)) {
						if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE;
					} else {
						return(MOVE_NO);
					}
				} else {
					return(MOVE_NO);
				}
			} else {
				return(MOVE_NO);
			}
		}
	}

	/*
	**	If the cell is out and out impassable because of underlying terrain, then
	**	return this immutable fact.
	*/
#ifdef ADVANCED
	if (retval != MOVE_DESTROYABLE && !Ground[cellptr->Land_Type()].Cost[Class->Speed]) {
#else
	if (!Ground[cellptr->Land_Type()].Cost[Class->Speed]) {
#endif
		return(MOVE_NO);
	}

	/*
	** Loop through all of the objects in the square setting a bit
	** for how they affect movement.
	*/
	bool crushable = false;
	ObjectClass *obj = cellptr->Cell_Occupier();
	while (obj) {

		if (obj != this) {

			/*
			**	Always allow entry if trying to move on a cell with
			**	authorization from the occupier.
			*/
			if (obj == Contact_With_Whom() && (IsTethered || (obj->What_Am_I() ==  RTTI_BUILDING && *((BuildingClass *)obj) == STRUCT_REPAIR))) {
				return(MOVE_OK);
			}

			bool is_moving = (obj->What_Am_I() == RTTI_INFANTRY || obj->What_Am_I() == RTTI_UNIT) && Target_Legal(((FootClass *)obj)->NavCom);

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
					if (!Class->IsCrusher || !obj->Class_Of().IsCrushable) {

						/*
						**	Any non-allied blockage is considered impassible if the unit
						**	is not equipped with a weapon.
						*/
						if (Class->Primary == WEAPON_NONE) return(MOVE_NO);

						/*
						**	Some kinds of terrain are considered destroyable if the unit is equipped
						**	with the weapon that can destroy it. Otherwise, the terrain is considered
						**	impassable.
						*/
						switch (obj->What_Am_I()) {
							case RTTI_TERRAIN:
								if (((TerrainClass *)obj)->Class->IsFlammable &&
										BulletTypeClass::As_Reference(Weapons[Class->Primary].Fires).Warhead == WARHEAD_FIRE) {

									if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE;
								} else {
									return(MOVE_NO);
								}
								break;

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
	**	If some allied object has reserved the cell, then consider the cell
	**	as blocked by a moving object.
	*/
	if (retval == MOVE_OK && !crushable && cellptr->Flag.Composite) {

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
					if (Class->Primary != WEAPON_NONE) {
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

#ifdef ADVANCED
		/*
		**	However, if the cell is occupied by a crushable vehicle, then we can
		**	never be sure if some other friendly vehicle is also trying to crush
		**	the cell at the same time. In the case of a crushable vehicle in the
		**	cell, then allow entry.
		*/
		if (!cellptr->Cell_Unit() || !cellptr->Cell_Unit()->Class->IsCrushable) {
			return(MOVE_MOVING_BLOCK);
		}
#else
		return(MOVE_MOVING_BLOCK);
#endif
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
	UnitClass * ptr;

	Units.Free_All();

	ptr = new UnitClass();
	VTable = ((void **)(((char *)ptr) + sizeof(AbstractClass) - 4))[0];
	delete ptr;
}


/***********************************************************************************************
 * UnitClass::Target_Coord -- The coordinate to use when targeting this unit.                  *
 *                                                                                             *
 *    Sometimes the coordinate to use when targeting an object is not the same as its center   *
 *    coordinate. This is especially true for boats since leading their movement is needed     *
 *    in order have any chance of hitting.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate to fire upon when attacking the unit.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE UnitClass::Target_Coord(void) const
{
	Validate();
//	if (*this == UNIT_GUNBOAT) {
//		return(Coord_Move(Coord, PrimaryFacing.Current(), 0x0080));
//	}
	return(TarComClass::Center_Coord());
}


/***********************************************************************************************
 * UnitClass::Scatter -- Causes the unit to travel to a nearby safe cell.                      *
 *                                                                                             *
 *    This routine is called when the unit discovers that it should get out of the "hot seat"  *
 *    and move to an adjacent cell. Since the safety of the adjacent cell is not determined    *
 *    before the move begins, it will appear that the unit is just scattering (which it        *
 *    should).                                                                                 *
 *                                                                                             *
 * INPUT:   threat   -- The coordinate of the source of the threat. The unit will try to move  *
 *                      roughly away from the threat.                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/25/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Scatter(COORDINATE threat, bool forced, bool nokidding)
{
	Validate();
	if (*this != UNIT_GUNBOAT && *this != UNIT_HOVER) {
		if (!PrimaryFacing.Is_Rotating() && ((!Target_Legal(TarCom) && !Target_Legal(NavCom)) || forced || nokidding || Random_Pick(1, 4) == 1)) {
			FacingType	toface;
			FacingType	newface;
			CELL			newcell;

			if (threat) {
				toface = Dir_Facing(Direction8(threat, Coord));
				toface = toface + (Random_Pick(0, 2)-1);
			} else {
				toface = Dir_Facing(PrimaryFacing.Current());
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
 * UnitClass::Stop_Driver -- Handles removing occupation bits when driving stops.              *
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
bool UnitClass::Stop_Driver(void)
{
	Validate();

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
		** If the vehicle is down, pick it up so it doesnt interfere with
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
	return(TarComClass::Stop_Driver());
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
	Validate();
	if (TarComClass::Start_Driver(headto)) {
		Mark_Track(headto, MARK_DOWN);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * UnitClass::Limbo -- Prepares vehicle and then limbos it.                                    *
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
bool UnitClass::Limbo(void)
{
	Validate();
	if (!IsInLimbo) {
		Stop_Driver();
	}
	return(TarComClass::Limbo());
}


/***********************************************************************************************
 * UnitClass::Response_Select -- Voice feedback when selecting the unit.                       *
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
void UnitClass::Response_Select(void)
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

	if (*this == UNIT_TRIC || *this == UNIT_TREX || *this == UNIT_RAPT || *this == UNIT_STEG) {
		response = VOC_DINOYES;
	}

	if (AllowVoice) {
		Sound_Effect(response, 0, -(Units.ID(this)+1));
	}
}


/***********************************************************************************************
 * UnitClass::Response_Move -- Voice feedback when ordering the unit to move.                  *
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
void UnitClass::Response_Move(void)
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

	if (*this == UNIT_TRIC || *this == UNIT_TREX || *this == UNIT_RAPT || *this == UNIT_STEG) {
		response = VOC_DINOMOUT;
	}

	if (AllowVoice) {
		Sound_Effect(response, 0, -(Units.ID(this)+1));
	}
}


/***********************************************************************************************
 * UnitClass::Response_Attack -- Voice feedback when ordering the unit to attack a target.     *
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
void UnitClass::Response_Attack(void)
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

	if (*this == UNIT_TRIC || *this == UNIT_TREX || *this == UNIT_RAPT || *this == UNIT_STEG) {
		response = VOC_DINOMOUT;
	}

	if (AllowVoice) {
		Sound_Effect(response, 0, -(Units.ID(this)+1));
	}
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
ActionType UnitClass::What_Action(ObjectClass * object) const
{
	Validate();
	ActionType action = TarComClass::What_Action(object);

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
		if (*this != UNIT_MCV) {
			if (!Class->IsTransporter || !How_Many()) {
				action = ACTION_NONE;
			}
		} else {
			((ObjectClass &)(*this)).Mark(MARK_UP);
			if (!BuildingTypeClass::As_Reference(STRUCT_CONST).Legal_Placement(Coord_Cell(Adjacent_Cell(Center_Coord(), FACING_NW)))) {
				action = ACTION_NO_DEPLOY;
			}
			((ObjectClass &)(*this)).Mark(MARK_DOWN);
		}
	}

	/*
	**	Special return to friendly refinery action.
	*/
	if (IsOwnedByPlayer && object->Is_Techno() && ((TechnoClass const *)object)->House->Is_Ally(this)) {
		if (object->What_Am_I() == RTTI_BUILDING && ((UnitClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (TechnoClass*)object) == RADIO_ROGER) {
			action = ACTION_ENTER;
		}
	}

	/*
	**	Special return to friendly repair factory action.
	*/
	if (IsOwnedByPlayer && action == ACTION_SELECT && object->What_Am_I() == RTTI_BUILDING) {
		BuildingClass * building = (BuildingClass *)object;
		if (building->Class->Type == STRUCT_REPAIR && !building->In_Radio_Contact() && !building->Is_Something_Attached()) {
			action = ACTION_MOVE;
		}
	}

	return(action);
}


ActionType UnitClass::What_Action(CELL cell) const
{
	Validate();
	ActionType action = TarComClass::What_Action(cell);
	if (action == ACTION_MOVE && Map[cell].Land_Type() == LAND_TIBERIUM && Class->IsToHarvest) {
		return(ACTION_HARVEST);
	}
	return(action);
}


/***********************************************************************************************
 * UnitClass::Can_Player_Move -- Determines if the player is legally allowed to move it.       *
 *                                                                                             *
 *    Use this routine to see if the player can move this object. If the player can move the   *
 *    object, even only in theory, then this function returns true. In all other cases, such   *
 *    as for enemy units, gunboats, or hovercraft, it returns false.                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can the player give this object a movement order?                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Can_Player_Move(void) const
{
	Validate();
	return(TarComClass::Can_Player_Move() && *this != UNIT_GUNBOAT && *this != UNIT_HOVER);
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
void UnitClass::Read_INI(char *buffer)
{
	UnitClass	*unit;			// Working unit pointer.
	char			*tbuffer;		// Accumulation buffer of unit IDs.
	HousesType	inhouse;			// Unit house.
	UnitType		classid;			// Unit class.
	int			len;				// Length of data in buffer.
	char			buf[128];

	len = strlen(buffer) + 2;
	tbuffer = buffer + len;

	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-len, buffer);
	while (*tbuffer != '\0') {

		WWGetPrivateProfileString(INI_Name(), tbuffer, NULL, buf, sizeof(buf)-1, buffer);
		inhouse = HouseTypeClass::From_Name(strtok(buf, ","));
		if (inhouse != HOUSE_NONE) {
			classid = UnitTypeClass::From_Name(strtok(NULL, ","));

			if (classid != UNIT_NONE) {

				if (HouseClass::As_Pointer(inhouse) != NULL) {
					unit = new UnitClass(classid, inhouse);
					if (unit) {

						/*
						**	Read the raw data.
						*/
						int strength = atoi(strtok(NULL, ",\r\n"));
						COORDINATE coord = Cell_Coord((CELL)atoi(strtok(NULL, ",\r\n")));
						DirType dir = (DirType)atoi(strtok(NULL, ",\r\n"));
						MissionType mission = MissionClass::Mission_From_Name(strtok(NULL, ",\n\r"));
						unit->Trigger = TriggerClass::As_Pointer(strtok(NULL,",\r\n"));
						if (unit->Trigger) {
							unit->Trigger->AttachCount++;
						}

						if (unit->Unlimbo(coord, dir)) {
							unit->Strength = Fixed_To_Cardinal(unit->Class->MaxStrength, strength);
							if (GameToPlay == GAME_NORMAL || unit->House->IsHuman) {
								unit->Assign_Mission(mission);
								unit->Commence();
							} else {
								unit->Enter_Idle_Mode();
							}

							/*
							**	The gunboat is a special case: It must "drive" off the edge of the map.
							**	Just pick the map edge that it is facing and set that as the destination
							**	of the drive.
							*/
							if (*unit == UNIT_GUNBOAT) {
								unit->PrimaryFacing.Set_Desired(DIR_W);
								unit->PrimaryFacing.Set_Current(DIR_W);
								unit->Assign_Mission(MISSION_HUNT);
								unit->Commence();
								unit->Assign_Destination( ::As_Target(XY_Cell(Map.MapCellX-1, Cell_Y(Coord_Cell(unit->Coord)))));
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
		tbuffer += strlen(tbuffer)+1;
	}
}


/***********************************************************************************************
 * UnitClass::Write_INI -- Writes all the units out to an INI file.                            *
 *                                                                                             *
 *    This routine writes all of the units in the game out to an INI file. This is used        *
 *    in the scenario editor when the game needs to be saved.                                  *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Housename, Typename, Strength, Coord, Facingnum, Missionname, Triggername              *
 *                                                                                             *
 * INPUT:   buffer   -- A pointer to the loaded INI file staging area.                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Write_INI(char *buffer)
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
	for (index = 0; index < Units.Count(); index++) {
		UnitClass * unit;

		unit = Units.Ptr(index);
		if (!unit->IsInLimbo && unit->IsActive) {

			sprintf(uname, "%03d", index);
			sprintf(buf, "%s,%s,%d,%u,%d,%s,%s",
				unit->House->Class->IniName,
				unit->Class->IniName,
				unit->Health_Ratio(),
				Coord_Cell(unit->Coord),
				unit->PrimaryFacing.Current(),
				MissionClass::Mission_Name(unit->Mission),
				unit->Trigger ? unit->Trigger->Get_Name() : "None"
				);
			WWWritePrivateProfileString(INI_Name(), uname, buf, buffer);
		}
	}
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
#define XYCELL(x,y)	(y*MAP_CELL_W+x)
void UnitClass::Exit_Repair(void)
{
	Validate();
	int	i;
	CELL	cell;
	bool	found = false;
	static short const ExitRepair[] = {
		XYCELL(0,-2),
		XYCELL(1,-1),
		XYCELL(2, 0),
		XYCELL(1, 1),
		XYCELL(0, 2),
		XYCELL(-1,1),
		XYCELL(-2,0),
		XYCELL(-1,-1)
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
		DirType	dir = Direction(cell);

		Assign_Mission(MISSION_MOVE);
		Assign_Destination(::As_Target(cell));
	}
}


/***********************************************************************************************
 * UnitClass::Mission_Guard -- Special guard mission override processor.                       *
 *                                                                                             *
 *    This routine will intercept the guard mission and if it is for a hovercraft, assign      *
 *    it the unload mission instead. This prevents the hovercraft from being stuck in the      *
 *    water if something unexpected causes it to drop into guard mode.                         *
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
	Validate();
	if (*this == UNIT_HOVER) {
		if (Is_Something_Attached()) {
			Assign_Mission(MISSION_UNLOAD);
			Find_LZ();
		} else {
			Exit_Map();
		}
		return(TICKS_PER_SECOND);
	}

	if (*this == UNIT_GUNBOAT) {
		Assign_Mission(MISSION_HUNT);
		return(TICKS_PER_SECOND);
	}

	if (*this == UNIT_HARVESTER && !House->IsHuman) {
		Assign_Mission(MISSION_HARVEST);
		return(TICKS_PER_SECOND);
	}

#ifdef USE_RA_AI
	/*
	** Copied functionality from RA for AI. ST - 7/25/2019 3:11PM
	*/
	if (GameToPlay != GAME_NORMAL) {
		if (*this == UNIT_MCV && House->IsBaseBuilding && House->IsHuman == false) {
			Assign_Mission(MISSION_UNLOAD);
			return(TICKS_PER_SECOND + Random_Pick(0, 2));
		}
	}
#endif // USE_RA_AI

	return(TarComClass::Mission_Guard());
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
 *=============================================================================================*/
int UnitClass::Mission_Move(void)
{
	Validate();
	IsHarvesting = false;

	/*
	**	Always make sure that that transport door is closed if the vehcile is moving.
	*/
	if (!Is_Door_Closed()) {
		APC_Close_Door();
	}

	/*
	**	Gunboats must always have the hunt mission.
	*/
	if (*this == UNIT_GUNBOAT) {
		Assign_Mission(MISSION_HUNT);
		return(TICKS_PER_SECOND);
	}
	return(TarComClass::Mission_Move());
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
	Validate();
	/*
	**	Determine the ideal facing that provides the least resistance. This would be the direction
	**	of the potential passenger or the current transport facing if it is going to unload.
	*/
	DirType faceto;
	if (passenger) {
		faceto = Direction(passenger);
	} else {
		faceto = PrimaryFacing.Current() + DIR_S;
	}

	/*
	**	Sweep through the adjacent cells in order to find the best candidate.
	*/
	FacingType bestdir;
	int bestval = -1;
	for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
		int value = 0;
		CELL cellnum = Adjacent_Cell(Coord_Cell(Coord), face);

		/*
		**	Base the initial value of the potential cell according to whether the passenger is
		**	allowed to enter the cell. If it can't, then give such a negative value to the
		**	cell so that it is prevented from ever choosing that cell for load/unload.
		*/
		if (passenger) {
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
		value -= (int)ABS(Dir_Diff(Facing_Dir(face), faceto));
		if (face == FACING_S) {
			value -= 100;
		}
		if (face == FACING_SW || face == FACING_SE) value += 64;

		/*
		**	If the value for the potiential cell is greater than the last recorded potential
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
 * UnitClass::Mission_Attack -- Handles the mission attack logic.                              *
 *                                                                                             *
 *    This routine intercepts the normal mission attack logic. If a gunboat is assigned the    *
 *    attack mission then it must be converted back to a hunt mission.                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the time before calling this routine again.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Mission_Attack(void)
{
	Validate();
	if (*this == UNIT_GUNBOAT) {
		Assign_Mission(MISSION_HUNT);
		return(TICKS_PER_SECOND);
	}
	return(TarComClass::Mission_Attack());
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
	Validate();
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
	Validate();
	if (Flagged != HOUSE_NONE) {
		Flagged = HOUSE_NONE;
		Mark(MARK_CHANGE);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * UnitClass::Stun -- Stuns the unit in preparation for unit removal.                          *
 *                                                                                             *
 *    This routine intercepts the stun operation for the unit and if there is a house flag     *
 *    attached, it will drop it to the ground.                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Stun(void)
{
	Validate();
	if (Flagged != HOUSE_NONE) {
		HouseClass::As_Pointer(Flagged)->Flag_Attach(Coord_Cell(Coord));
	}
	TarComClass::Stun();
}


/***********************************************************************************************
 * UnitClass::Pip_Count -- Fetchs the number of pips to display on unit.                       *
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
	Validate();
	if (Class->IsTransporter) {
		return(How_Many());
	}
	if (Class->IsToHarvest) {
		return(Fixed_To_Cardinal(UnitTypeClass::FULL_LOAD_CREDITS/100, Tiberium_Load()));
	}
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
	Validate();
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
	Validate();
	if (!IsDriving && !IsRotating) {
		if (PrimaryFacing == DIR_NW || PrimaryFacing == DIR_NE) {
			Open_Door(10, 2);
		} else {
			Open_Door(1, 2);
		}
	}
}


/***********************************************************************************************
 * UnitClass::Remap_Table -- Fetches the remap table to use for this object.                   *
 *                                                                                             *
 *    Use this routine to determine the rendering remap table to use for this object. The      *
 *    remap table is normally the unit remap table, except for the MCV and the Harvestor.      *
 *    These units use the building remap table since these units become part of the building   *
 *    animation.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the remap table to use for this unit.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void const * UnitClass::Remap_Table(void)
{
	Validate();
	if (*this == UNIT_MCV || *this == UNIT_HARVESTER) {
		return(House->Remap_Table(IsBlushing, false));
	}
	return(TarComClass::Remap_Table());
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
	Validate();
	if (Class->Primary == WEAPON_NONE) {
		if (Random_Pick(0, 1) == 0) {
			return(INFANTRY_C1);
		} else {
			return(INFANTRY_C7);
		}
	}
	return(TarComClass::Crew_Type());
}


/***********************************************************************************************
 * UnitClass::What_Am_I -- Returns with the RTTI type this object is.                          *
 *                                                                                             *
 *    This will return that this is a normal vehicle unit type. Each object class overrides    *
 *    this function in order to provide run time type identification support.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the RTTI type that this object is (i.e., RTTI_UNIT).                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
RTTIType UnitClass::What_Am_I(void) const
{
	Validate();
	return(RTTI_UNIT);
}