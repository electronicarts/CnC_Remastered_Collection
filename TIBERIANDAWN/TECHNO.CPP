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

/* $Header:   F:\projects\c&c\vcs\code\techno.cpv   2.13   02 Aug 1995 17:01:08   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BASE.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 8, 1994                                                  *
 *                                                                                             *
 *                  Last Update : August 23, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TechnoClass::AI -- Handles AI processing for techno object.                               *
 *   TechnoClass::Assign_Destination -- Assigns movement destination to the object.            *
 *   TechnoClass::Assign_Target -- Assigns the targeting computer with specified target.       *
 *   TechnoClass::Base_Is_Attacked -- Handle panic response to base being attacked.            *
 *   TechnoClass::Can_Fire -- Determines if this techno object can fire.                       *
 *   TechnoClass::Can_Player_Fire -- Determines if the player can give this object a fire order*
 *   TechnoClass::Can_Player_Move -- Determines if the object can move be moved by player.     *
 *   TechnoClass::Can_Repair -- Determines if the object can and should be repaired.           *
 *   TechnoClass::Captured -- Handles capturing this object.                                   *
 *   TechnoClass::Clicked_As_Target -- Sets the flash count for this techno object.            *
 *   TechnoClass::Crew_Type -- Fetches the kind of crew this object contains.                  *
 *   TechnoClass::Debug_Dump -- Displays the base class data to the monochrome screen.         *
 *   TechnoClass::Desired_Load_Dir -- Fetches loading parameters for this object.              *
 *   TechnoClass::Detach -- Handles removal of target from tracking system.                    *
 *   TechnoClass::Do_Cloak -- Start the object into cloaking stage.                            *
 *   TechnoClass::Do_Shimmer -- Causes this object to shimmer if it is cloaked.                *
 *   TechnoClass::Do_Uncloak -- Cause the stealth tank to uncloak.                             *
 *   TechnoClass::Draw_It -- Draws the health bar (if necessary).                              *
 *   TechnoClass::Draw_Pips -- Draws the transport pips and other techno graphics.             *
 *   TechnoClass::Enter_Idle_Mode -- Object enters its default idle condition.                 *
 *   TechnoClass::Evaluate_Cell -- Determine the value and object of specified cell.           *
 *   TechnoClass::Evaluate_Object -- Determines score value of specified object.               *
 *   TechnoClass::Exit_Object -- Causes specified object to leave this object.                 *
 *   TechnoClass::Find_Docking_Bay -- Searches for a close docking bay.                        *
 *   TechnoClass::Find_Exit_Cell -- Finds an appropriate exit cell for this object.            *
 *   TechnoClass::Fire_At -- Fires projectile at target specified.                             *
 *   TechnoClass::Fire_Direction -- Fetches the direction projectile fire will take.           *
 *   TechnoClass::Get_Ownable -- Fetches the ownable bits for this object.                     *
 *   TechnoClass::Greatest_Threat -- Determines best target given search criteria.             *
 *   TechnoClass::Hidden -- Returns the object back into the hidden state.                     *
 *   TechnoClass::In_Range -- Determines if specified target is within weapon range.           *
 *   TechnoClass::In_Range -- Determines if specified target is within weapon range.           *
 *   TechnoClass::In_Range -- Determines if the specified coordinate is within range.          *
 *   TechnoClass::Is_Techno -- Confirms that this is a TechnoClass object.                     *
 *   TechnoClass::Is_Weapon_Equipped -- Determines if this object has a combat weapon.         *
 *   TechnoClass::Kill_Cargo -- Destroys any cargo attached to this object.                    *
 *   TechnoClass::Mark -- Handles marking of techno objects.                                   *
 *   TechnoClass::Nearby_Location -- Radiates outward looking for clear cell nearby.           *
 *   TechnoClass::Owner -- Who is the owner of this object?                                    *
 *   TechnoClass::Per_Cell_Process -- Handles once-per-cell operations for techno type objects.*
 *   TechnoClass::Pip_Count -- Fetches the number of pips to display on this object.           *
 *   TechnoClass::Player_Assign_Mission -- Assigns a mission as result of player input.        *
 *   TechnoClass::Random_Animate -- Performs some idle animation for the object.               *
 *   TechnoClass::Rearm_Delay -- Calculates the delay before firing can occur.                 *
 *   TechnoClass::Receive_Message -- Handles inbound message as appropriate.                   *
 *   TechnoClass::Record_The_Kill -- Records the death of this object.                         *
 *   TechnoClass::Remap_Table -- Fetches the appropriate remap table to use.                   *
 *   TechnoClass::Response_Attack -- Handles the voice response when given attack order.       *
 *   TechnoClass::Response_Move -- Handles the voice repsonse to a movement request.           *
 *   TechnoClass::Response_Select -- Handles the voice response when selected.                 *
 *   TechnoClass::Revealed -- Handles revealing an object to the house specified.              *
 *   TechnoClass::Risk -- Fetches the risk associated with this object.                        *
 *   TechnoClass::Scatter -- Causes the object to scatter to an adjacent cell.                 *
 *   TechnoClass::Select -- Selects object and checks to see if can be selected.               *
 *   TechnoClass::Set_Mission -- Forced mission set (used by editor).                          *
 *   TechnoClass::Stun -- Prepares the object for removal from the game.                       *
 *   TechnoClass::Take_Damage -- Records damage assessed to this object.                       *
 *   TechnoClass::Target_Something_Nearby -- Handles finding and assigning a nearby target.    *
 *   TechnoClass::TechnoClass -- Constructor for techno type objects.                          *
 *   TechnoClass::TechnoClass -- Default constructor for techno objects.                       *
 *   TechnoClass::Techno_Draw_Object -- General purpose draw object routine.                   *
 *   TechnoClass::Threat_Range -- Returns the range to scan based on threat control.           *
 *   TechnoClass::Tiberium_Load -- Fetches the current tiberium load percentage.               *
 *   TechnoClass::Unlimbo -- Performs unlimbo process for all techno type objects.             *
 *   TechnoClass::Value -- Fetches the target value for this object.                           *
 *   TechnoClass::Visual_Character -- Determine the visual character of the object.            *
 *   TechnoClass::Weapon_Range -- Determines the maximum range for the weapon.                 *
 *   TechnoClass::What_Action -- Determines action to perform if cell is clicked on.           *
 *   TechnoClass::What_Action -- Determines what action to perform if object is selected.      *
 *   TechnoTypeClass::Cost_Of -- Fetches the cost of this object type.                         *
 *   TechnoTypeClass::Get_Cameo_Data -- Fetches the cameo image for this object type.          *
 *   TechnoTypeClass::Get_Ownable -- Fetches the ownable bits for this object type.            *
 *   TechnoTypeClass::Max_Passengers -- Fetches the maximum passengers allowed.                *
 *   TechnoTypeClass::Repair_Cost -- Fetches the cost to repair one step.                      *
 *   TechnoTypeClass::Repair_Step -- Fetches the health to repair one step.                    *
 *   TechnoTypeClass::TechnoTypeClass -- Constructor for techno type objects.                  *
 *   TechnoTypeClass::Time_To_Build -- Fetches the time to build this object.                  *
 *   TechnoTypeClass::Raw_Cost -- Fetches the raw (base) cost of the object.                   *
 *   TechnoClass::Refund_Amount -- Returns with the money to refund if this object is sold.    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***************************************************************************
**	Cloaking control values.
*/
#define	MAX_UNCLOAK_STAGE		38
#define	UNCLOAK_VIS_TIME		(1*TICKS_PER_SECOND)

//Added for getting the input for special character keys from the client 
// - 6/26/2019 JAS 
extern bool DLL_Export_Get_Input_Key_State(KeyNumType key);


/***************************************************************************
**	Which shape to use depending on which facing is controlled by these arrays.
*/
int const TechnoClass::BodyShape[32] = {0,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};


/***********************************************************************************************
 * TechnoTypeClass::TechnoTypeClass -- Constructor for techno type objects.                    *
 *                                                                                             *
 *    This is the normal constructor for techno type objects. It is called in the process of   *
 *    constructing all the object type (constant) data for the various techno type objects.    *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TechnoTypeClass::TechnoTypeClass(
		int name,
		char const *ininame,
		unsigned char level,
		long pre,
		bool is_leader,
		bool is_scanner,
		bool is_nominal,
		bool is_transporter,
		bool is_flammable,
		bool is_crushable,
		bool is_stealthy,
		bool is_selectable,
		bool is_legal_target,
		bool is_insignificant,
		bool is_immune,
		bool is_theater,
		bool is_twoshooter,
		bool is_turret_equipped,
		bool is_repairable,
		bool is_buildable,
		bool is_crew,
		int ammo,
		unsigned short strength,
		MPHType maxspeed,
		int sightrange,
		int cost,
		int scenario,
		int risk,
		int reward,
		int ownable,
		WeaponType primary,
		WeaponType secondary,
		ArmorType armor) :
	ObjectTypeClass(	true,
							is_flammable,
							is_crushable,
							is_stealthy,
							is_selectable,
							is_legal_target,
							is_insignificant,
							is_immune,
							name,
							ininame,
							armor,
							strength)
{
	Level = level;
	Pre = pre;
	MaxAmmo = ammo;
	MaxSpeed = maxspeed;
	CameoData = NULL;
	Primary = primary,
	Secondary = secondary,
	Cost = cost;
	IsLeader = is_leader;
	IsScanner = is_scanner;
	IsTransporter = is_transporter;
	IsTwoShooter = is_twoshooter;
	IsBuildable = is_buildable;
	IsCrew = is_crew;
	IsTheater = is_theater;
	IsRepairable = is_repairable;
	IsTurretEquipped= is_turret_equipped;
	IsNominal = is_nominal;
	Ownable = ownable;
	Reward = reward;
	Scenario = scenario;
	SightRange = sightrange;

	/*
	** Units risk value is based on the type of weapon he has and the
	** rate of fire it shoots at.
	*/
	risk = risk;
	Risk = 0;
	if (primary != WEAPON_NONE) {
		Risk = (Weapons[primary].Attack * (Weapons[primary].Range >> 4)) / Weapons[primary].ROF;
	}
}


/***********************************************************************************************
 * TechnoTypeClass::Raw_Cost -- Fetches the raw (base) cost of the object.                     *
 *                                                                                             *
 *    This routine is used to find the underlying cost for this object. The underlying cost    *
 *    does not include any free items that normally come with the object when purchased        *
 *    directly. Example: The raw cost of a refinery is the normal cost minus the cost of a     *
 *    harvester.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the credit cost of the base object type.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoTypeClass::Raw_Cost(void) const
{
	return(Cost);
}


/***********************************************************************************************
 * TechnoTypeClass::Get_Ownable -- Fetches the ownable bits for this object type.              *
 *                                                                                             *
 *    This routine will return the ownable bits for this object type. The ownable bits are     *
 *    a bitflag composite of the houses that can own (build) this object type.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the ownable bits for this object type.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
unsigned short TechnoTypeClass::Get_Ownable(void) const
{
	return(Ownable);
}


/***********************************************************************************************
 * TechnoTypeClass::Time_To_Build -- Fetches the time to build this object.                    *
 *                                                                                             *
 *    This routine will return the time it takes to construct this object. Usually the time    *
 *    to produce is directly related to cost.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the time to produce this object type. The time is expressed in the    *
 *          form of game ticks.                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoTypeClass::Time_To_Build(HousesType house) const
{
	int cost = Raw_Cost();

	/*
	**	For computer controlled buildings, slow down production on
	**	cheaper buildings.
	*/
	if (PlayerPtr->Difficulty != DIFF_HARD &&
		GameToPlay == GAME_NORMAL &&
		What_Am_I() == RTTI_BUILDINGTYPE &&
		PlayerPtr->Class->House != house) {

		cost = (cost + (PlayerPtr->Difficulty == DIFF_EASY ? 4000 : 2000)) / 2;
	}

	/*
	**	Fudge factor, so that Nod builds a bit faster if the object must be delivered to
	**	an airfield.
	*/
	if (What_Am_I() == RTTI_UNITTYPE && !(Ownable & HOUSEF_GOOD)) {
		return (cost - (cost/4));
	}

	return(cost);
}


/***********************************************************************************************
 * TechnoTypeClass::Cost_Of -- Fetches the cost of this object type.                           *
 *                                                                                             *
 *    This routine will return the cost to produce an object of this type.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the cost to produce one object of this type.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoTypeClass::Cost_Of(void) const
{
	return(Cost);
}


/***********************************************************************************************
 * TechnoTypeClass::Get_Cameo_Data -- Fetches the cameo image for this object type.            *
 *                                                                                             *
 *    This routine will fetch the cameo (sidebar small image) shape of this object type.       *
 *    If there is no cameo data available (typical for non-producable units), then NULL will   *
 *    be returned.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the cameo data for this object type if present.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void const * TechnoTypeClass::Get_Cameo_Data(void) const
{
	return(CameoData);
}


/***********************************************************************************************
 * TechnoTypeClass::Repair_Cost -- Fetches the cost to repair one step.                        *
 *                                                                                             *
 *    This routine will return the cost to repair one step. At the TechnoTypeClass level,      *
 *    this merely serves as a placeholder function. The derived classes will provide a         *
 *    functional version of this routine.                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the cost to repair one step.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoTypeClass::Repair_Cost(void) const
{
	return(0);
}


/***********************************************************************************************
 * TechnoTypeClass::Repair_Step -- Fetches the health to repair one step.                      *
 *                                                                                             *
 *    This routine merely serves as placeholder virtual function. The various type classes     *
 *    will override this routine to return the number of health points to repair in one        *
 *    "step".                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of health points to repair in one step.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoTypeClass::Repair_Step(void) const
{
	return(0);
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * TechnoClass::Debug_Dump -- Displays the base class data to the monochrome screen.           *
 *                                                                                             *
 *    This routine is used to dump the status of the object class to the monochrome screen.    *
 *    This display can be used to track down or prevent bugs.                                  *
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
void TechnoClass::Debug_Dump(MonoClass *mono) const
{
	mono->Set_Cursor(0,0);mono->Printf("(%04X)p=%d,d=%d", House->Power_Fraction(), House->Power, House->Drain);
//	mono->Set_Cursor(0,0);mono->Printf("(%d)", House->Blockage);
	mono->Text_Print("X", 16 + (IsALoaner?2:0), 11);
	mono->Text_Print("X", 16 + (IsLocked?2:0), 9);

	mono->Text_Print("X", 16 + (IsInRecoilState?2:0), 17);
	mono->Text_Print("X", 16 + (IsTethered?2:0), 8);
	mono->Text_Print("X", 16 + (IsOwnedByPlayer?2:0), 5);
	mono->Text_Print("X", 16 + (IsDiscoveredByPlayer?2:0), 6);
//	mono->Text_Print("X", 16 + (IsALemon?2:0), 9);
	mono->Set_Cursor(28, 7);mono->Printf("%2d", Arm);
	mono->Set_Cursor(34, 1);mono->Printf("%04X", TarCom);
	mono->Set_Cursor(29, 3);mono->Printf("%02X", PrimaryFacing.Current());

	FlasherClass::Debug_Dump(mono);
	StageClass::Debug_Dump(mono);
	RadioClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * TechnoClass::TechnoClass -- Default constructor for techno objects.                         *
 *                                                                                             *
 *    This default constructor for techno objects is used to reset all internal variables to   *
 *    their default state.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TechnoClass::TechnoClass(void) :
	TarCom(TARGET_NONE),
	House(0)
{
	ArchiveTarget = TARGET_NONE;
	Arm = 0;
	Ammo = -1;
	LineCount = 0;
	LineFrame = 0;
	LineMaxFrames = 0;
	PurchasePrice = 0;
	IsTickedOff = false;
	Cloak = UNCLOAKED;
	CloakingDevice.Set_Stage(1);
	CloakingDevice.Set_Rate(0);
	IsCloakable = false;
	IsALemon = false;
	IsALoaner = false;
	IsDiscoveredByPlayer = false;
	IsDiscoveredByComputer = false;
	IsInRecoilState = false;
	IsLeader = false;
	IsLocked = false;
	IsOwnedByPlayer = false;
	IsSecondShot = true;
	IsTethered = false;
	SuspendedTarCom = TARGET_NONE;
	PrimaryFacing.Set(DIR_N);

	// Added for multiplayer changes. ST - 3/6/2019 11:34AM
	IsDiscoveredByPlayerMask = 0;
}



/***********************************************************************************************
 * TechnoClass::Revealed -- Handles revealing an object to the house specified.                *
 *                                                                                             *
 *    When a unit moves out from under the shroud or when it gets delivered into already       *
 *    explored terrain, then it must be "revealed". Objects that are revealed may be           *
 *    announced to the player. The discovered bit updated accordingly.                         *
 *                                                                                             *
 * INPUT:   house -- The house that this object is revealed to.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *   12/27/1994 JLB : Discovered trigger event processing.                                     *
 *   03/06/2019  ST : Per-player discovery                                                     *
 *=============================================================================================*/
bool TechnoClass::Revealed(HouseClass * house)
{
	//if (house == PlayerPtr && IsDiscoveredByPlayer) return(false);
	//if (house != PlayerPtr && IsDiscoveredByComputer) return(false);
	if (house == NULL) {
		return false;
	}

	if (Is_Discovered_By_Player(house)) {
		return false;
	}

	if (house->IsHuman == false) {
		if (IsDiscoveredByComputer) {
			return false;
		}
	}

	if (RadioClass::Revealed(house)) {

		/*
		**	An enemy object that is discovered will go into hunt mode if
		**	its current mission is to ambush.
		*/
		if (!house->IsHuman && Mission == MISSION_AMBUSH) {
			Assign_Mission(MISSION_HUNT);
		}

		if (GameToPlay == GAME_NORMAL) {
			if (house == PlayerPtr) {
				if (!IsOwnedByPlayer) {
					
					/*
					**	If there is a trigger event associated with this object, then process
					**	it for discovery purposes.
					*/
					if (Trigger) {
						Trigger->Spring(EVENT_DISCOVERED, this);
					}

					/*
					**	Alert the enemy house to presence of the friendly side.
					*/
					House->IsDiscovered = true;
				
					if (house->IsHuman) {
						Set_Discovered_By_Player(house);
					} else {
						IsDiscoveredByComputer = true;
					}

				} else {
					
					if (house->IsHuman) {
						Set_Discovered_By_Player(house);
					} else {
						IsDiscoveredByComputer = true;
					}
					Look();
					  
				}	
			}
		} else {
			
			if (house->IsHuman) {
				Set_Discovered_By_Player(house);
			} else {
				IsDiscoveredByComputer = true;
			}
			
			Look();
		}

		return(true);
	}
	return(false);
}

#if (0)	                               // ST - 3/6/2019 11:35AM
/***********************************************************************************************
 * TechnoClass::Revealed -- Handles revealing an object to the house specified.                *
 *                                                                                             *
 *    When a unit moves out from under the shroud or when it gets delivered into already       *
 *    explored terrain, then it must be "revealed". Objects that are revealed may be           *
 *    announced to the player. The discovered bit updated accordingly.                         *
 *                                                                                             *
 * INPUT:   house -- The house that this object is revealed to.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *   12/27/1994 JLB : Discovered trigger event processing.                                     *
 *=============================================================================================*/
bool TechnoClass::Revealed(HouseClass * house)
{
	if (house == PlayerPtr && IsDiscoveredByPlayer) return(false);
	if (house != PlayerPtr && IsDiscoveredByComputer) return(false);

	if (RadioClass::Revealed(house)) {

		/*
		**	An enemy object that is discovered will go into hunt mode if
		**	its current mission is to ambush.
		*/
		if (!house->IsHuman && Mission == MISSION_AMBUSH) {
			Assign_Mission(MISSION_HUNT);
		}

		if (house == PlayerPtr) {

			if (!IsOwnedByPlayer) {

				/*
				**	If there is a trigger event associated with this object, then process
				**	it for discovery purposes.
				*/
				if (Trigger) {
					Trigger->Spring(EVENT_DISCOVERED, this);
				}

				/*
				**	Alert the enemy house to presence of the friendly side.
				*/
				House->IsDiscovered = true;

			} else {

				/*
				**	A newly revealed object will always perform a look operation.
				*/
				if (house == PlayerPtr) IsDiscoveredByPlayer = true;
				if (house != PlayerPtr) IsDiscoveredByComputer = true;
				Look();
			}
		}

		if (house == PlayerPtr) IsDiscoveredByPlayer = true;
		if (house != PlayerPtr) IsDiscoveredByComputer = true;

		return(true);
	}
	return(false);
}
#endif

/***********************************************************************************************
 * TechnoClass::Hidden -- Returns the object back into the hidden state.                       *
 *                                                                                             *
 *    This routine is called for every object that returns to the darkness shroud or when      *
 *    it gets destroyed. This also occurs when an object enters another (such as infantry      *
 *    entering a transport helicopter).                                                        *
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
void TechnoClass::Hidden(void)
{
	// ST - 3/13/2019 4:56PM
	if (!Is_Discovered_By_Player()) {
		return;
	}
	//if (!IsDiscoveredByPlayer) return;
	if (!House->IsHuman) {
		Clear_Discovered_By_Players();
	}
}


/***********************************************************************************************
 * TechnoClass::Mark -- Handles marking of techno objects.                                     *
 *                                                                                             *
 *    On the Techno-level, marking handles transmission of the redraw command to any object    *
 *    that it is 'connected' with. This only occurs during loading and unloading.              *
 *                                                                                             *
 * INPUT:   mark  -- The marking method. This routine just passes this on to base classes.     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Mark(MarkType mark)
{
	if (RadioClass::Mark(mark)) {
		/*
		**	When redrawing an object, if there is another object teathered to this one,
		**	redraw it as well.
		*/
		if (IsTethered) {
			Transmit_Message(RADIO_REDRAW);
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TechnoClass::Receive_Message -- Handles inbound message as appropriate.                     *
 *                                                                                             *
 *    This routine is used to handle inbound radio messages. It only handles those messages    *
 *    that deal with techno objects. Typically, these include loading and unloading.           *
 *                                                                                             *
 * INPUT:   from     -- Pointer to the originator of the radio message.                        *
 *                                                                                             *
 *          message  -- The inbound radio message.                                             *
 *                                                                                             *
 *          param    -- Reference to optional parameter that might be used to transfer         *
 *                      more information than is possible with the simple radio message        *
 *                      type.                                                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the radio response.                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   06/17/1995 JLB : Handles tether contact messages.                                         *
 *=============================================================================================*/
RadioMessageType TechnoClass::Receive_Message(RadioClass * from, RadioMessageType message, long & param)
{
	switch (message) {

		/*
		**	Just received instructions to attack the specified target.
		*/
		case RADIO_ATTACK_THIS:
			if (Techno_Type_Class()->Primary != WEAPON_NONE) {
				Assign_Target(param);
				Assign_Mission(MISSION_ATTACK);
				return(RADIO_ROGER);
			}
			break;

		/*
		**	Establish a tethered connection to the object in radio contact.
		*/
		case RADIO_TETHER:
			if (!IsTethered) {
				IsTethered = true;
				Transmit_Message(RADIO_TETHER, from);
				return(RADIO_ROGER);
			}
			break;

		/*
		**	Break the tethered connection to the object in radio contact.
		*/
		case RADIO_UNTETHER:
			if (IsTethered) {
				IsTethered = false;
				Transmit_Message(RADIO_UNTETHER, from);
				return(RADIO_ROGER);
			}
			break;

		/*
		**	A teathered unit has reached it's destination. All is
		**	clear, so radio contact can be broken.
		*/
		case RADIO_UNLOADED:
			Transmit_Message(RADIO_UNTETHER, from);
			return(Transmit_Message(RADIO_OVER_OUT, from));

		/*
		**	When this message is received, it means that the other object
		**	has already turned its radio off. Turn this radio off as well.
		*/
		case RADIO_OVER_OUT:
			Transmit_Message(RADIO_UNTETHER, from);
			RadioClass::Receive_Message(from, message, param);
			return(RADIO_ROGER);

		/*
		**	Request to be informed when unloaded. This message is transmitted
		**	by the transport unit to the transported unit as it is about to be
		**	unloaded. It is saying, "All is clear. Drive off now."
		*/
		case RADIO_UNLOAD:
		case RADIO_BACKUP_NOW:
		case RADIO_HOLD_STILL:
			Transmit_Message(RADIO_TETHER, from);
			RadioClass::Receive_Message(from, message, param);
			return(RADIO_ROGER);

		/*
		**	Handle reloading one ammo point for this unit.
		*/
		case RADIO_RELOAD:
			if (Ammo == Techno_Type_Class()->MaxAmmo) return(RADIO_NEGATIVE);
			Ammo++;
			return(RADIO_ROGER);

		/*
		**	Handle repair of this unit.
		*/
		case RADIO_REPAIR:
			if (/*param > 0 &&*/ Health_Ratio() < 0x0100) {
				int cost = Techno_Type_Class()->Repair_Cost();
//				int cost = Fixed_To_Cardinal(Techno_Type_Class()->Repair_Cost(), param);
				cost = MAX(cost, 1);
				int step = Techno_Type_Class()->Repair_Step();
//				int step = Fixed_To_Cardinal(Techno_Type_Class()->Repair_Step(), param);
				step = MAX(step, 1);
				if (House->Available_Money() >= cost) {
#ifdef OBSOLETE
					if (Health_Ratio() >= 0x0100) {
						Strength = Class_Of().MaxStrength;
						from->Scatter(true);
						return(RADIO_NEGATIVE);
					}
#endif
					House->Spend_Money(cost);
					Strength += step;
					return(RADIO_ROGER);
				}
			}
			break;

		default:
			break;
	}
	return(RadioClass::Receive_Message(from, message, param));
}


/***********************************************************************************************
 * TechnoClass::TechnoClass -- Constructor for techno type objects.                            *
 *                                                                                             *
 *    This constructor sets the owner of this object and its strength. Any object not owned    *
 *    by the player is marked as a loaner. This is a special flag that indicates off-map       *
 *    movement is allowed. The flag is cleared when the object finally enters the map.         *
 *                                                                                             *
 * INPUT:   house    -- The house (owner) of this object.                                      *
 *                                                                                             *
 *          strength -- The strength to assign to this object.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TechnoClass::TechnoClass(HousesType house) :
	House(HouseClass::As_Pointer(house)),
	TarCom(TARGET_NONE)
{
	ArchiveTarget = TARGET_NONE;
	Arm = 0;
	Ammo = -1;
	LineCount = 0;
	LineFrame = 0;
	LineMaxFrames = 0;
	PurchasePrice = 0;
	IsTickedOff = false;
	Cloak = UNCLOAKED;
	CloakingDevice.Set_Stage(1);
	CloakingDevice.Set_Rate(0);
	IsCloakable = false;
	IsALemon = false;
	IsALoaner = false;
	IsDiscoveredByComputer = false;
	//IsOwnedByPlayer = (house == PlayerPtr->Class->House);  // ST - 4/24/2019 10:41AM		
	IsDiscoveredByPlayer = false;
	IsInRecoilState = false;
	IsLeader = false;
	IsLocked = false;
	IsSecondShot = false;
	IsTethered = false;

	SuspendedTarCom = TARGET_NONE;

	PrimaryFacing.Set(DIR_N);

	// Added for multiplayer changes. ST - 4/24/2019 10:40AM
	IsDiscoveredByPlayerMask = 0;
	if (GameToPlay == GAME_NORMAL) {
		IsOwnedByPlayer = (house == PlayerPtr->Class->House);
	} else {
		IsOwnedByPlayer = House->IsHuman;
	}

	/*
	**	There is a chance that a vehicle will be a "lemon".
	*/
	if (Random_Pick(0, 255) < (int)House->Class->Lemon) {
		IsALemon = true;
	}
}


/***********************************************************************************************
 * TechnoClass::Per_Cell_Process -- Handles once-per-cell operations for techno type objects.  *
 *                                                                                             *
 *    This routine handles marking a game object as not a loaner. It is set only if the unit   *
 *    is not player owned and is on the regular map. This is necessary so that enemy objects   *
 *    can exist off-map but as soon as they move onto the map, are flagged so that can never   *
 *    leave it again.                                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   10/26/94   JLB : Handles scanner units.                                                   *
 *   12/27/1994 JLB : Checks for an processes any trigger in cell.                             *
 *=============================================================================================*/
void TechnoClass::Per_Cell_Process(bool )
{
	CELL cell = Coord_Cell(Center_Coord());

	/*
	**	When enemy units enter the proper map area from off map, they are
	**	flagged so that they won't travel back off the map again.
	*/
	if (Map.In_Radar(cell)) {

		if (What_Am_I() == RTTI_UNIT) {
			UnitClass * u = (UnitClass *)this;

			if (*u != UNIT_HOVER && *u != UNIT_GUNBOAT) {
				IsLocked = true;
			}
		} else {
			IsLocked = true;
		}
	}

	/*
	**	If this object somehow moves into mapped terrain, but is not yet
	**	discovered, then flag it to be discovered.
	*/
	// ST - 3/13/2019 4:56PM
	if (!Is_Discovered_By_Player() && Map[cell].Is_Visible(House)) {
	//if (!IsDiscoveredByPlayer && Map[cell].IsVisible) {
		//Revealed(PlayerPtr);
		Revealed(House);		// ST - 8/7/2019 11:20AM
	}
}


/***********************************************************************************************
 * TechnoClass::Draw_It -- Draws the health bar (if necessary).                                *
 *                                                                                             *
 *    This routine will draw the common elements for techno type objects. This element is      *
 *    the health bar. The main game object has already been rendered by the time this          *
 *    routine is called.                                                                       *
 *                                                                                             *
 * INPUT:   x,y   -- The coordinate of the center of the unit.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   10/26/94   JLB : Knows about radar scanned cells.                                         *
 *   12/13/1994 JLB : Clips health bar against map edge.                                       *
 *   01/23/1995 JLB : Dynamic selected object rectangle.                                       *
 *=============================================================================================*/
void TechnoClass::Draw_It(int x, int y, WindowNumberType window)
{
	Clear_Redraw_Flag();

	const bool show_health_bar = (Strength > 0) && (Is_Selected_By_Player() ||
		((Special.HealthBarDisplayMode == SpecialClass::HB_DAMAGED) && (Strength < Techno_Type_Class()->MaxStrength)) ||
		(Special.HealthBarDisplayMode == SpecialClass::HB_ALWAYS));

	/*
	**	Fetch the dimensions of the object. These dimensions will be used to draw
	**	the selection box and the health bar.
	*/
	int width, height;
	Class_Of().Dimensions(width, height);
	int lx = width/2;
	int ly = height/2;
	int dx = width/5;
	int dy = height/5;
	int fudge = show_health_bar ? 4 : 0;

	/*
	** Draw lines
	*/
	if (LineFrame < LineMaxFrames) {
		// Only draw the last line for virtual window
		int start_line = (window == WINDOW_VIRTUAL) ? max(0, LineCount - 1) : 0;
		for (int i = start_line; i < LineCount; i++) {
			CC_Draw_Line(Lines[i][0], Lines[i][1], Lines[i][2], Lines[i][3], (unsigned char)Lines[i][4], LineFrame, window);
		}
		if ((window == WINDOW_VIRTUAL) && (++LineFrame >= LineMaxFrames)) {
			LineCount = 0;
			LineFrame = 0;
			LineMaxFrames = 0;
		}
	}

	if (Is_Selected_By_Player() || show_health_bar) {
		GraphicViewPortClass draw_window(	LogicPage->Get_Graphic_Buffer(),
														(WindowList[window][WINDOWX] << 3) + LogicPage->Get_XPos(),
														WindowList[window][WINDOWY] + LogicPage->Get_YPos(),
														WindowList[window][WINDOWWIDTH] << 3,
														WindowList[window][WINDOWHEIGHT]);


		/*
		**	The infantry select box should be a bit higher than normal.
		*/
		if (What_Am_I() == RTTI_INFANTRY) {
			y -= 6;
		}

		if (What_Am_I() == RTTI_BUILDING && ((BuildingTypeClass const &)Class_Of()).Type == STRUCT_BARRACKS) {
			y -= 5;
		}

		if (show_health_bar) {
			unsigned 	ratio = Health_Ratio();
			int			pwidth;		// Pixel width of bar interior.
			int			color;		// The color to give the interior of the bargraph.

			int xx = x-width/2;
			int yy = y-(height/2);

			/*
			**	Draw the outline of the bargraph.
			*/
			draw_window.Remap(xx+1, yy+1, width-1, 3-1, Map.FadingShade);
			draw_window.Draw_Rect(xx, yy, xx+width-1, yy+3, BLACK);

			/*
			**	Determine the width of the interior strength
			**	graph.
			*/
			pwidth = Fixed_To_Cardinal(width-2, ratio);

			pwidth = Bound(pwidth, 1, width-2);

			color = LTGREEN;
			if (ratio < 0x7F) {
				color = YELLOW;
			}
			if (ratio < 0x3F) {
				color = RED;
			}
			draw_window.Fill_Rect(xx+1, yy+1, xx+pwidth, yy+(3-1), color);
		}

		/*
		**	Draw the selected object graphic.
		*/
		if (Is_Selected_By_Player()) {
			// Upper left corner.
			draw_window.Draw_Line(x-lx, fudge+y-ly, x-lx+dx, fudge+y-ly, WHITE);
			draw_window.Draw_Line(x-lx, fudge+y-ly, x-lx, fudge+y-ly+dy, WHITE);

			// Upper right corner.
			draw_window.Draw_Line(x+lx, fudge+y-ly, x+lx-dx, fudge+y-ly, WHITE);
			draw_window.Draw_Line(x+lx, fudge+y-ly, x+lx, fudge+y-ly+dy, WHITE);

			// Lower right corner.
			draw_window.Draw_Line(x+lx, y+ly, x+lx-dx, y+ly, WHITE);
			draw_window.Draw_Line(x+lx, y+ly, x+lx, y+ly-dy, WHITE);

			// Lower left corner.
			draw_window.Draw_Line(x-lx, y+ly, x-lx+dx, y+ly, WHITE);
			draw_window.Draw_Line(x-lx, y+ly, x-lx, y+ly-dy, WHITE);
		}
	}

	// MBL 04.21.2020 
	bool selected = Is_Selected_By_Player() || Special.ResourceBarDisplayMode == SpecialClass::RB_ALWAYS;
	// if ((window == WINDOW_VIRTUAL) || (Is_Selected_By_Player() && House->Is_Ally(PlayerPtr))) 
	if ((window == WINDOW_VIRTUAL) || (selected && House->Is_Ally(PlayerPtr))) 
	{
		Draw_Pips((x-lx)+5, y+ly-3, window);
	}
}


/***********************************************************************************************
 * TechnoClass::Unlimbo -- Performs unlimbo process for all techno type objects.               *
 *                                                                                             *
 *    This routine handles the common operation between techno objects when they are           *
 *    unlimboed. This includes revealing the map.                                              *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate to unlimbo object at.                                   *
 *                                                                                             *
 *          dir (optional) -- initial facing direction for this object                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the unlimbo successful?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Unlimbo(COORDINATE coord, DirType dir)
{
	if (RadioClass::Unlimbo(coord, dir)) {
		PrimaryFacing = dir;
		Enter_Idle_Mode(true);
		Commence();

		IsLocked = Map.In_Radar(Coord_Cell(coord));
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TechnoClass::In_Range -- Determines if specified target is within weapon range.             *
 *                                                                                             *
 *    This routine is used to compare the distance to the specified target with the range      *
 *    of the weapon. If the target is outside of weapon range, then false is returned.         *
 *                                                                                             *
 * INPUT:   target   -- The target to check if it is within weapon range.                      *
 *                                                                                             *
 *          which    -- Which weapon to use in determining range. 0=primary, 1=secondary.      *
 *                                                                                             *
 * OUTPUT:  bool; Is the specified target within weapon range?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::In_Range(TARGET target, int which, bool reciprocal_check) const
{
	if (IsLocked && Target_Legal(target)) {
		int range = Weapon_Range(which);
		BuildingClass const * building = As_Building(target);
		if (building) {
			range += ((building->Class->Width() + building->Class->Height()) * (ICON_LEPTON_W / 4));
		}
		if (::Distance(Fire_Coord(which), As_Coord(target)) <= range) {
			return(true);
		}
		
		/*
		** There's a longstanding bug where enemy units aligned north/south may have different perceptions of whether they
		** are in range of each other due to the turrets offset on the unit. This check will say that if either considers the 
		** the other to be in range, then they both do. ST - 3/18/2020 10:44AM
		*/
		if (reciprocal_check && !building) {
			ObjectClass *target_object = As_Object(target);
			if (target_object) {
				RTTIType my_type = What_Am_I();
				if (target_object->What_Am_I() == my_type) {
					if (range == target_object->Weapon_Range(which)) {
						TechnoClass *tech = As_Techno(target);
						if (tech->In_Range(As_Target(), which, false)) {
							return true;
						}
					}
				}
			}
		}
	}
	return(false);
}


/***********************************************************************************************
 * TechnoClass::In_Range -- Determines if specified target is within weapon range.             *
 *                                                                                             *
 *    This routine will determine if the pointer to the target object passed into this         *
 *    routine is within weapon range.                                                          *
 *                                                                                             *
 * INPUT:   target   -- Pointer to the target object to check if within weapon range.          *
 *                                                                                             *
 *          which    -- Which weapon to use in determining range. 0=primary, 1=secondary.      *
 *                                                                                             *
 * OUTPUT:  bool; Is the target within weapon range?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::In_Range(ObjectClass const * target, int which, bool reciprocal_check) const
{
	if (IsLocked && target) {
		int range = Weapon_Range(which);
		if (target->What_Am_I() == RTTI_BUILDING) {
			BuildingClass const * building = (BuildingClass const *)target;
			range += ((building->Class->Width() + building->Class->Height()) * (ICON_LEPTON_W / 4));
		}

		if (::Distance(Fire_Coord(which), target->Center_Coord()) <= range) {
			return(true);
		}
		
		/*
		** There's a longstanding bug where enemy units aligned north/south may have different perceptions of whether they
		** are in range of each other due to the turrets offset on the unit. This check will say that if either considers the 
		** the other to be in range, then they both do. ST - 3/18/2020 10:44AM
		*/
		if (reciprocal_check) {
			RTTIType my_type = What_Am_I();
			if (my_type != RTTI_BUILDING) {
				if (target->What_Am_I() == my_type) {
					if (range == target->Weapon_Range(which)) {
						if (((TechnoClass*)target)->In_Range(this, which, false)) {
							return true;
						}
					}
				}
			}
		}
	}
	return(false);
}


/***********************************************************************************************
 * TechnoClass::In_Range -- Determines if the specified coordinate is within range.            *
 *                                                                                             *
 *    Use this routine to determine if the specified coordinate is within weapon range.        *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate to examine against the object to determine range.       *
 *                                                                                             *
 *          which    -- The weapon to consider when determining range. 0=primary, 1=secondary. *
 *                                                                                             *
 * OUTPUT:  bool; Is the weapon within range?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::In_Range(COORDINATE coord, int which) const
{
	return(IsLocked && ::Distance(Fire_Coord(which), coord) <= Weapon_Range(which));
}


/***********************************************************************************************
 * TechnoClass::Evaluate_Object -- Determines score value of specified object.                 *
 *                                                                                             *
 *    This routine is used to determing the score value (value as a potential target) of the   *
 *    object specified. This routine will check the specified object for all the various       *
 *    legality checks that threat scanning requires. This is the main workhorse routine for    *
 *    target searching.                                                                        *
 *                                                                                             *
 * INPUT:   method   -- The threat method requested. This is a combined bitflag value that     *
 *                      not only specified the kind of targets to consider, but how far away   *
 *                      they are allowed to be.                                                *
 *                                                                                             *
 *          mask     -- This is an RTTI mask to use for quickly eliminating object types.      *
 *                      The mask is created outside of this routine because this routine is    *
 *                      usually called from within a loop and this value is constant in that   *
 *                      context.                                                               *
 *                                                                                             *
 *          range    -- The range at which potential target objects are rejected.              *
 *                      0  = must be within weapon range.                                      *
 *                      >0 = must be within this lepton distance.                              *
 *                      <0 = range doesn't matter.                                             *
 *                                                                                             *
 *          object   -- Pointer to the object itself.                                          *
 *                                                                                             *
 *          value    -- Reference to the value variable that this routine will fill in. The    *
 *                      higher the value the more likely this object will be selected as best. *
 *                                                                                             *
 * OUTPUT:  Did the target pass all legality checks? If this value is returned true, then the  *
 *          value parameter will be filled in correctly.                                       *
 *                                                                                             *
 * WARNINGS:   This routine is time consuming. Don't call unless necessary.                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/30/1995 JLB : Created.                                                                 *
 *   07/14/1995 JLB : Forces SAM site to not fire on landed aircraft.                          *
 *=============================================================================================*/
bool TechnoClass::Evaluate_Object(ThreatType method, int mask, int range, TechnoClass const * object, int & value) const
{
	/*
	**	An object in limbo can never be a valid target.
	*/
	if (object->IsInLimbo) return(false);

	/*
	**	Friendly units are never considered a good target. Bail if this
	**	object is a friend.
	*/
	if (House->Is_Ally(object)) return(false);

	/*
	**	If the object is farther away than allowed, bail.
	*/
	int dist = Distance(object);
	if (range > 0 && dist > range) return(false);
	if (range == 0 && !In_Range(object, 0) && !In_Range(object, 1)) return(false);

	/*
	**	If the object is not visible, then bail. Human controled units
	**	are always considered to be visible.
	*/
	if (!object->IsOwnedByPlayer && !object->IsDiscoveredByPlayer && GameToPlay == GAME_NORMAL && object->What_Am_I() != RTTI_AIRCRAFT) {
		return(false);
	}

	/*
	**	Quickly eliminate all unit types that are not allowed according to the mask
	**	value.
	*/
	RTTIType otype = object->What_Am_I();
	if (!((1 << otype) & mask)) return(false);		// Mask failure.

	/*
	**	If the object is cloaked, then it isn't a legal target.
	*/
	if (object->Cloak == CLOAKED) return(false);

	/*
	**	Determine if the target is theoretically allowed to be a target. If
	**	not, then bail.
	*/
	TechnoTypeClass const * tclass = object->Techno_Type_Class();
	if (!tclass->IsLegalTarget) return(false);		// Legality failure.

	/*
	**	Never consider agent Delphi a valid target.
	*/
	if (otype == RTTI_INFANTRY && ((InfantryTypeClass const *)tclass)->Type == INFANTRY_DELPHI) {
		return(false);
	}

	/*
	**	Special case so that SAM site doesn't fire on aircraft that are landed.
	*/
	if (otype == RTTI_AIRCRAFT && What_Am_I() == RTTI_BUILDING && *((BuildingClass *)this) == STRUCT_SAM) {
		if (((AircraftClass *)object)->Altitude == 0) return(false);
	}

	/*
	**	If only allowed to attack civilians, then eliminate all other types.
	*/
	if ((method & THREAT_CIVILIANS) && object->Owner() != HOUSE_NEUTRAL) {
		return(false);
	}

	/*
	**	If the scan is limited to capturable buildings only, then bail if the examined
	**	object isn't a capturable building.
	*/
	if ((method & THREAT_CAPTURE) && (otype != RTTI_BUILDING || !((BuildingTypeClass const *)tclass)->IsCaptureable)) {
		return(false);
	}

	/*
	**	If not allowed to attack boats, then eliminate them from consideration.
	*/
	if (!(method & THREAT_BOATS) &&
			otype == RTTI_UNIT &&
			(((UnitTypeClass const *)tclass)->Speed == SPEED_HOVER || ((UnitTypeClass const *)tclass)->Speed == SPEED_FLOAT)) {
		return(false);
	}

	/*
	**	SPECIAL CASE: Friendly units won't automatically fire on buildings
	**	if the building is not aggressive.
	*/
	if (House->IsHuman && otype == RTTI_BUILDING && tclass->Primary == WEAPON_NONE) return(false);

	/*
	**	If the search is restricted to Tiberium processing objects, then
	**	perform the special qualification check now.
	*/
	if (method & THREAT_TIBERIUM) {
		switch (otype) {
			case RTTI_UNIT:
				if (!((UnitTypeClass const *)tclass)->IsToHarvest) return(false);
				break;

			case RTTI_BUILDING:
				if (!((BuildingTypeClass const *)tclass)->Capacity) return(false);
				break;

			default:
				return(false);
		}
	}

	/*
	**	If this target value is better than the previously recorded best
	**	target value then record this target for possible return as the
	**	best target available.
	*/
	int rawval = object->Value();
	value = rawval + object->Kills;

#ifdef USE_RA_AI
	/*
	**	If the candidate object is owned by the designated enemy of this house, then
	**	give it a higher value. This will tend to gravitate attacks toward the main
	**	antagonist of this house.
	*/
	if (House->Enemy != HOUSE_NONE && House->Enemy == object->House->Class->House) {
		value += 500;
		value *= 3;
	}
#endif

#ifdef ADVANCED
	/*
	**	Lessen threat as a factor of distance.
	*/
	if (rawval) {

		value = (value * 32000) / (((dist/ICON_LEPTON_W)*(dist/ICON_LEPTON_W))+1);

		//value = Fixed_To_Cardinal(value, Cardinal_To_Fixed(MAP_CELL_W*2, (MAP_CELL_W*2) - (dist/ICON_LEPTON_W)));
		//value = MAX(value, 2);

		if (value < MAP_CELL_W*2) value = dist/ICON_LEPTON_W;
		value = MAX(value, 1);
		return(true);
	}
	value = 0;
	return(false);

#else

	/*
	**	Lessen threat as a factor of distance.
	*/
	if (range == -1 && value) {
		/*
		** Code from RA so that the value isn't always 2 on a mapwide scan with the range set to -1. ST - 3/2/2020 5:02PM
		*/
		value = (value * 32000) / ((dist/ICON_LEPTON_W)+1);
	} else {
		/*
		** Original TD code
		*/
		int modifier = dist;
		int crange = range / ICON_LEPTON_W;
		if (crange) modifier /= crange;
		if (modifier) value /= modifier;
	}
	if (rawval) {
		value = MAX(value, 2);
	}
	return(true);
#endif
}


/***********************************************************************************************
 * TechnoClass::Evaluate_Cell -- Determine the value and object of specified cell.             *
 *                                                                                             *
 *    This routine will examine the specified cell and return with the potential target        *
 *    object it contains and the value of it. Use this routine when searching for threats.     *
 *                                                                                             *
 * INPUT:   method   -- The scan method to use for target searching.                           *
 *                                                                                             *
 *          mask     -- Prebuilt mask of object RTTI types acceptable for scanning.            *
 *                                                                                             *
 *          range    -- Scan range limit to use for elimination purposes. This ensures that    *
 *                      objects in the "corner" of a square scan get properly discarded.       *
 *                                                                                             *
 *          object   -- Pointer to object pointer to be filled in with the object at this      *
 *                      cell as a valid target.                                                *
 *                                                                                             *
 *          value    -- Reference to the value of the object in this cell. It will be set      *
 *                      according to the object's value.                                       *
 *                                                                                             *
 * OUTPUT:  Was a valid potential target found in this cell?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Evaluate_Cell(ThreatType method, int mask, CELL cell, int range, TechnoClass const * * object, int & value) const
{
	*object = NULL;
	value = 0;

	/*
	**	If the cell is not on the legal map, then always ignore it.
	*/
	if (cell & 0xF000) return(false);
	if (!Map.In_Radar(cell)) return(false);

	/*
	**	Fetch the techno object from the cell. If there is no
	**	techno object there, then bail.
	*/
	CellClass * cellptr = &Map[cell];
	TechnoClass const * tentative = (TechnoClass const *)cellptr->Cell_Occupier();
	while (tentative) {
		if (tentative->Is_Techno() && !House->Is_Ally(tentative)) break;
		tentative = (TechnoClass const *)tentative->Next;
	}

	if (!tentative) return(false);
//	if (!tentative->Is_Techno()) return(false);
	*object = tentative;

	return(Evaluate_Object(method, mask, range, tentative, value));
}


/***********************************************************************************************
 * TechnoClass::Greatest_Threat -- Determines best target given search criteria.               *
 *                                                                                             *
 *    This routine will scan game objects looking for the best target. It is used by the       *
 *    general target searching processes. The type of target scan to perform is controlled     *
 *    by the method control parameter.                                                         *
 *                                                                                             *
 * INPUT:   method   -- The method control parameter is used to control the type of target     *
 *                      scan performed. It consists of a series of bit flags (see ThreatType)  *
 *                      that are combined to form the target scan desired.                     *
 *                                                                                             *
 * OUTPUT:  Returns the target value of a suitable target. If no target was found then the     *
 *          value TARGET_NONE is returned.                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1994 JLB : Created.                                                                 *
 *   06/20/1995 JLB : Greatly optimized scan method.                                           *
 *=============================================================================================*/
TARGET TechnoClass::Greatest_Threat(ThreatType method) const
{
	ObjectClass const * bestobject = NULL;
	int bestval = -1;

	/*
	**	Build a quick elimination mask. If the RTTI of the object doesn't
	**	qualify with this mask, then we KNOW that it shouldn't be considered.
	*/
	int mask = 0;
	if (method & THREAT_CIVILIANS) mask |= ((1 << RTTI_BUILDING) | (1 << RTTI_INFANTRY) | (1 << RTTI_UNIT));
	if (method & THREAT_AIR) mask |= (1 << RTTI_AIRCRAFT);
	if (method & THREAT_CAPTURE) mask |= (1 << RTTI_BUILDING);
	if (method & THREAT_BUILDINGS) mask |= (1 << RTTI_BUILDING);
	if (method & THREAT_INFANTRY) mask |= (1 << RTTI_INFANTRY);
	if (method & THREAT_VEHICLES) mask |= (1 << RTTI_UNIT);

	/*
	** Let's pick from the available contenders on a map-wide scan. ST - 3/2/2020 3:49PM
	*/
	static const int _max_best_objects = 128;
	ObjectClass const *best_objects [_max_best_objects];
	int best_object_count = 0;

	/*
	**	Limit area target scans use a method where the actual map cells are
	**	examined for occupants. The occupant is then examined in turn. The
	**	best target within the area is returned as a target.
	*/
	if (method & (THREAT_AREA|THREAT_RANGE)) {
		int range = Threat_Range((method & THREAT_RANGE) ? 0 : 1);

//		int range = MAX(Weapon_Range(0), Weapon_Range(1));
//		if (!(method & THREAT_RANGE)) range *= 2;
//		range = Bound(range, 0x0100, 0x1400);			// Limit maximum scan distance.
		int crange = range / ICON_LEPTON_W;
		if (range == 0) {
			crange = MAX(Weapon_Range(0), Weapon_Range(1)) / ICON_LEPTON_W;
			crange++;
		}
		CELL cell = Coord_Cell(Fire_Coord(0));
//		CELL cell = Coord_Cell(Center_Coord());

		/*
		**	If aircraft are a legal target, then scan through all of them at this time.
		**	Scanning by cell is not possible for aircraft since they are not recorded
		**	at the cell level.
		*/
		if (method & THREAT_AIR) {
			for (int index = 0; index < Aircraft.Count(); index++) {
				TechnoClass * object = Aircraft.Ptr(index);

				int value = 0;
				if (Evaluate_Object(method, mask, range, object, value)) {
					if (value > bestval) {
						bestobject = object;
						bestval = value;
					}
				}
			}
		}

		/*
		**	When scanning the ground, always consider landed aircraft as a valid
		**	potential target. This is only true if vehicles are considered a
		**	valid target. A landed aircraft is considered a vehicle.
		*/
		if (method & THREAT_VEHICLES) {
			mask |= (1 << RTTI_AIRCRAFT);
		}

		/*
		**	Radiate outward from the object's location, looking for the best
		**	target.
		*/
		TechnoClass const * object;
		int value;
		for (int radius = 1; radius < crange; radius++) {

			/*
			**	Scan the top and bottom rows of the "box".
			*/
			for (int x = -radius; x <= radius; x++) {
				CELL newcell;

				if ((Cell_X(cell) + x) < Map.MapCellX) continue;
				if ((Cell_X(cell) + x) >= (Map.MapCellX+Map.MapCellWidth)) continue;

				if ((Cell_Y(cell) - radius) >= Map.MapCellY) {
					newcell = XY_Cell(Cell_X(cell) + x, Cell_Y(cell)-radius);
					if (Evaluate_Cell(method, mask, newcell, range, &object, value)) {
						if (bestval < value) {
							bestobject = object;
						}
					}
				}

				if ((Cell_Y(cell) + radius) < (Map.MapCellY+Map.MapCellHeight)) {
					newcell = XY_Cell(Cell_X(cell)+x, Cell_Y(cell)+radius);
					if (Evaluate_Cell(method, mask, newcell, range, &object, value)) {
						if (bestval < value) {
							bestobject = object;
						}
					}
				}
			}

			/*
			**	Scan the left and right columns of the "box".
			*/
			for (int y = -(radius-1); y < radius; y++) {
				CELL newcell;

				if ((Cell_Y(cell) + y) < Map.MapCellY) continue;
				if ((Cell_Y(cell) + y) >= (Map.MapCellY+Map.MapCellHeight)) continue;

				if ((Cell_X(cell) - radius) >= Map.MapCellX) {
					newcell = XY_Cell(Cell_X(cell)-radius, Cell_Y(cell)+y);
					if (Evaluate_Cell(method, mask, newcell, range, &object, value)) {
						if (bestval < value) {
							bestobject = object;
						}
					}
				}

				if ((Cell_X(cell) + radius) < (Map.MapCellX+Map.MapCellWidth)) {
					newcell = XY_Cell(Cell_X(cell)+radius, Cell_Y(cell)+y);
					if (Evaluate_Cell(method, mask, newcell, range, &object, value)) {
						if (bestval < value) {
							bestobject = object;
						}
					}
				}
			}

			/*
			**	Bail early if a target has already been found and the range is at
			**	one of the breaking points (i.e., normal range or range * 2).
			*/
			if (bestobject) {
				if (radius == crange/4) {
					return(bestobject->As_Target());
				}
				if (radius == crange/2) {
					return(bestobject->As_Target());
				}
			}
		}

	} else {

		/*
		**	A full map scan was requested. First scan through aircraft. The top map layer
		**	is NOT scanned since that layer will probably contain more bullets and animations
		**	than aircraft.
		*/
		int index;
		for (index = 0; index < Aircraft.Count(); index++) {
			TechnoClass * object = Aircraft.Ptr(index);

			int value = 0;
			if (Evaluate_Object(method, mask, -1, object, value)) {
				if (value > bestval) {
					bestobject = object;
					bestval = value;
					best_object_count = 0;
					best_objects[best_object_count++] = object;
				} else {
					if (value == bestval) {
						if (best_object_count < _max_best_objects) {
							best_objects[best_object_count++] = object;
						}
					}
				}
			}
		}

		/*
		**	Now scan through the entire ground layer. This is painful, but what other
		**	choice is there?
		*/
		for (index = 0; index < Map.Layer[LAYER_GROUND].Count(); index++) {
			ObjectClass const * object = Map.Layer[LAYER_GROUND][index];

			int value = 0;
			if (object->Is_Techno() && Evaluate_Object(method, mask, -1, (TechnoClass const *)object, value)) {
				if (value > bestval) {
					bestobject = object;
					bestval = value;
					best_object_count = 0;
					best_objects[best_object_count++] = object;
				} else {
					if (value == bestval) {
						if (best_object_count < _max_best_objects) {
							best_objects[best_object_count++] = object;
						}
					}
				}
			}
		}
	}

	/*
	**	If a good target object was found, then return with the target value
	**	of it.
	*/
	if (bestobject) {
		
		/*
		** If there's only one, return that. Otherwise pick from the equal contenders
		*/
		if (best_object_count > 1) {

			/*
			** Pick one at random from our contenders
			*/
			int index = Random_Pick(0, best_object_count - 1);
			bestobject = As_Object(best_objects[index]->As_Target());
		}

		return(bestobject->As_Target());
	}
	return(TARGET_NONE);
}


/***********************************************************************************************
 * TechnoClass::Owner -- Who is the owner of this object?                                      *
 *                                                                                             *
 *    Use this routine to examine this object and return who the owner is.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the house number of the owner of this object.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
HousesType TechnoClass::Owner(void) const
{
	return(House->Class->House);
}


/***********************************************************************************************
 * TechnoClass::Clicked_As_Target -- Sets the flash count for this techno object.              *
 *                                                                                             *
 *    Use this routine to set the flash count for the object. This flash count is the number   *
 *    of times the object will "flash". Typically it is called as a result of the player       *
 *    clicking on this object in order to make it the target of a move or attack.              *
 *                                                                                             *
 * INPUT:   count -- The number of times the object should flash.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Clicked_As_Target(HousesType house, int count) // 2019/09/20 JAS - Added record of who clicked on the object
{
	FlashCount = count;

	// 2019/09/20 JAS - Flashing info needs to exist per player
	if (house < HOUSE_COUNT)
	{
		FlashCountPerPlayer[house] = count;
	}
	else
	{
		//receiving HOUSE_COUNT means do it for every player
		for (int i = 0; i < HOUSE_COUNT; ++i)
		{
			FlashCountPerPlayer[i] = count;
		}
	}
}


/***********************************************************************************************
 * TechnoClass::AI -- Handles AI processing for techno object.                                 *
 *                                                                                             *
 *    This routine handles AI processing for techno objects. Typically, this merely dispatches *
 *    to the appropriate AI routines for the base classes.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Make sure that this routine is only called ONCE per game tick.                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::AI(void)
{
	CargoClass::AI();
	RadioClass::AI();
	DoorClass::AI();

	/*
	** Handle decision to re-cloak here. Process the cloaking/decloaking operation.
	*/
	if (IsCloakable) {

		/*
		**	If this object is uncloaked, but it can be cloaked and it thinks that it
		**	is a good time do so, then begin cloaking.
		*/
		if (Cloak == UNCLOAKED) {
			// Changed for multiplayer so we can visually see the different players in the original renderer. ST - 3/13/2019 5:40PM
			if (Is_Owned_By_Player()) Mark(MARK_CHANGE);
			//if (IsOwnedByPlayer) Mark(MARK_CHANGE);
			CloakingDevice.Graphic_Logic();
			if (!Arm && CloakingDevice.Fetch_Stage()) {
				if (Health_Ratio() > 0x0040) {
					Do_Cloak();
				} else {
					if (Random_Pick(0, 25) == 1) {
						Do_Cloak();
					}
				}
			}
		} else {

			VisualType pre = Visual_Character(true);
			CloakingDevice.Graphic_Logic();
			switch (Cloak) {

				/*
				**	Handle the uncloaking process. Always mark to redraw
				**	the object and when cloaking is complete, stabilize into
				**	the normal uncloaked state.
				*/
				case UNCLOAKING:
					Mark(MARK_CHANGE);
					if (Visual_Character(true) == VISUAL_NORMAL) {
						CloakingDevice.Set_Rate(UNCLOAK_VIS_TIME);
						CloakingDevice.Set_Stage(0);	// re-start the stage counter
						Cloak = UNCLOAKED;
					}
					break;

				/*
				**	Handle the cloaking process. Always mark to redraw the object
				**	and when the cloaking process is complete, stabilize into the
				**	normal cloaked state.
				*/
				case CLOAKING:
					Mark(MARK_CHANGE);
					switch (Visual_Character(true)) {

						/*
						**	If badly damaged, then it can never fully cloak.
						*/
						case VISUAL_DARKEN:
							if (Health_Ratio() < 0x0040 && Random_Pick(1, 3) == 1) {
								Cloak = UNCLOAKING;
							}
							break;

#ifdef NEVER
						case VISUAL_SHADOWY:
							if (pre != Visual_Character(true)) {
								Detach_All(false);
							}
							break;
#endif

						case VISUAL_HIDDEN:
							Cloak = CLOAKED;
							CloakingDevice.Set_Rate(0);

							/*
							**	Special check to ensure that if the unit is carring a captured
							**	flag, it will never fully cloak.
							*/
							if (What_Am_I() == RTTI_UNIT && ((UnitClass *)this)->Flagged != HOUSE_NONE) {
								Do_Shimmer();
							} else {
								Detach_All(false);
							}

							/*
							**	A computer controlled unit will try to scatter if possible so
							**	that it will be much harder to locate.
							*/
							if (What_Am_I() == RTTI_UNIT && !House->IsHuman) {
								Scatter(0, true);
							}
							break;
					}
					break;

				/*
				**	A cloaked object will always be redrawn if it is owned by the
				**	player. This ensures that the shimmering effect will animate.
				*/
				case CLOAKED:
					// Changed for multiplayer so we can visually see the different players in the original renderer. ST - 3/13/2019 5:40PM
					if (Is_Owned_By_Player()) {
					//if (IsOwnedByPlayer) {
						Mark(MARK_CHANGE);
					}
					break;
			}
		}
	}

	/*
	**	Arming delay always counts down to zero.
	*/
	if (Arm) Arm--;

	/*
	**	Handle line delay logic.
	*/
	if ((LineMaxFrames > 0) && (LineCount > 0)) {
		Map.Flag_To_Redraw(true);
	}

	/*
	**	Update the animation timer system. If the animation stage
	**	changes, then flag the object to be redrawn as well as  determine
	**	if the current animation process needs to change.
	*/
	if (What_Am_I() != RTTI_BUILDING) {
		if (StageClass::Graphic_Logic() || Time_To_Redraw()) {
			Mark(MARK_CHANGE);
		}
	}

	/*
	**	If the object is flashing and a change of flash state has occured, then mark the
	**	object to be redrawn.
	*/
	if (FlasherClass::Process()) {
		Mark(MARK_CHANGE);
	}
}


/***********************************************************************************************
 * TechnoClass::Select -- Selects object and checks to see if can be selected.                 *
 *                                                                                             *
 *    This function checks to see if this techno object can be selected. If it can, then it    *
 *    is selected.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/11/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Select(bool allow_mixed)
{
	// ST - 3/13/2019 4:56PM
	if (!Is_Discovered_By_Player() && !Is_Owned_By_Player() && !Debug_Unshroud) {
	//if (!IsDiscoveredByPlayer && !IsOwnedByPlayer && !Debug_Unshroud) {
		return(false);
	}

	if (RadioClass::Select(allow_mixed)) {

		/*
		**	Speak a confirmation of selection.
		*/
		// ST - 3/13/2019 4:59PM
		if (Is_Owned_By_Player() && AllowVoice) {
		//if (IsOwnedByPlayer && AllowVoice) {
			Response_Select();
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TechnoClass::Can_Fire -- Determines if this techno object can fire.                         *
 *                                                                                             *
 *    This performs a simple check to make sure that this techno object can fire. At this      *
 *    level, the only thing checked for is the rearming delay.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the fire legality control code.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
FireErrorType TechnoClass::Can_Fire(TARGET target, int which) const
{
	/*
	**	Don't allow firing if the target is illegal.
	*/
	if (!Target_Legal(target)) {
		return(FIRE_ILLEGAL);
	}

	ObjectClass * object = As_Object(target);

	/*
	**	If the object is completely cloaked, then you can't fire on it.
	*/
//Mono_Printf("Units[0]=%p.\n", Units.Raw_Ptr(0));
//Mono_Printf("Infantry[0]=%p.\n", Infantry.Raw_Ptr(0));
//Mono_Printf("Buildings[0]=%p.\n", Buildings.Raw_Ptr(0));
//Mono_Printf("Aircraft[0]=%p.\n", Aircraft.Raw_Ptr(0));
//Mono_Printf("object=%p, Strength=%d, IsActive=%d, IsInLimbo=%d.\n", object, (long)object->Strength, object->IsActive, object->IsInLimbo);Get_Key();
	if (object && /*(object->IsActive || GameToPlay != GAME_NORMAL) &&*/ object->Is_Techno() && ((TechnoClass *)object)->Cloak == CLOAKED) {
		return(FIRE_CANT);
	}

	/*
	**	If there is no weapon, then firing is not allowed.
	*/
	WeaponType weap = (which == 0) ? Techno_Type_Class()->Primary : Techno_Type_Class()->Secondary;
	if (weap == WEAPON_NONE) {
		return(FIRE_CANT);
	}

	/*
	**	Can only fire anti-aircraft weapons against aircraft unless the aircraft is
	**	sitting on the ground.
	*/
	if (object && object->What_Am_I() == RTTI_AIRCRAFT &&
			!BulletTypeClass::As_Reference(Weapons[weap].Fires).IsAntiAircraft &&
			((AircraftClass *)object)->Altitude > 0) {

		return(FIRE_CANT);
	}

	/*
	**	Don't allow firing if still rearming.
	*/
	if (Arm) return(FIRE_REARM);

	/*
	**	The target must be within range in order to allow firing.
	*/
	if (!In_Range(target, which)) {
		return(FIRE_RANGE);
	}

	/*
	**	If there is no ammo left, then it can't fire.
	*/
	if (!Ammo) {
		return(FIRE_AMMO);
	}

	/*
	**	If cloaked, then firing is disabled.
	*/
	if (Cloak != UNCLOAKED) {
		return(FIRE_CLOAKED);
	}

	return(FIRE_OK);
}


/***********************************************************************************************
 * TechnoClass::Stun -- Prepares the object for removal from the game.                         *
 *                                                                                             *
 *    This routine handles cleaning up this techno object from the game system so that when    *
 *    it is subsequently removed, it doesn't leave any loose ends.                             *
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
void TechnoClass::Stun(void)
{
	Assign_Target(TARGET_NONE);
	Assign_Destination(TARGET_NONE);
	Transmit_Message(RADIO_OVER_OUT);
	Detach_All();
	//Unselect();
	//When an object is stunned it needs to be deselected from all players, not just the current PlayerPtr.
	// - 8/18/2019 JAS 
	Unselect_All_Players();
}


/***********************************************************************************************
 * TechnoClass::Assign_Target -- Assigns the targeting computer with specified target.         *
 *                                                                                             *
 *    Use this routine to set the targeting computer for this object. It checks to make sure   *
 *    that targeting of itself is prohibited.                                                  *
 *                                                                                             *
 * INPUT:   target   -- The target for this object to attack.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Assign_Target(TARGET target)
{
	if (target == TarCom) return;

	if (!Target_Legal(target)) {
		target = TARGET_NONE;
	} else {

		/*
		**	Prevent targeting of self.
		*/
		if (target == As_Target()) {
			target = ::As_Target(Coord_Cell(Coord));
		} else {

			/*
			**	Make sure that the target is not already dead.
			*/
			ObjectClass * object = As_Object(target);
			if (object && (object->IsActive == false || object->Strength == 0)) {
				target = TARGET_NONE;
			}
		}
	}

	/*
	**	Set the unit's targeting computer.
	*/
	TarCom = target;
}


/***********************************************************************************************
 * TechnoClass::Rearm_Delay -- Calculates the delay before firing can occur.                   *
 *                                                                                             *
 *    This function calculates the delay between shots. It determines this from the standard   *
 *    rate of fire (ROF) of the base class and modifies it according to game speed and         *
 *    whether this is the first or second shot. All single shot attackers consider their       *
 *    shots to be "second" since the second shot is the one handled normally. The first shot   *
 *    usually gets assigned a much shorter delay time before the next shot can fire.           *
 *                                                                                             *
 * INPUT:   second   -- bool; Is this the second of a two shot salvo?                          *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before the next shot may fire.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Rearm_Delay(bool second) const
{
	if (second) {
		return(((int)Weapons[Techno_Type_Class()->Primary].ROF * House->ROFBias) + 3);
	}
	return(9);
}


/***********************************************************************************************
 * TechnoClass::Fire_At -- Fires projectile at target specified.                               *
 *                                                                                             *
 *    This is the main projectile firing code. Buildings, units, and infantry route fire       *
 *    requests through this function.                                                          *
 *                                                                                             *
 * INPUT:   target   -- The target that the projectile is to be fired at.                      *
 *                                                                                             *
 *          which    -- Which weapon to fire.                                                  *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the projectile object that was fired. If no projectile   *
 *          could be created or there was some other illegality detected, the return value     *
 *          will be NULL.                                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/26/1994 JLB : Created.                                                                 *
 *   07/03/1995 JLB : Moving platforms fire inaccurate projectiles.                            *
 *=============================================================================================*/
BulletClass * TechnoClass::Fire_At(TARGET target, int which)
{
	BulletClass		*bullet;				// Projectile.
	DirType			dir;					// The facing to impart upon the projectile.
	COORDINATE				target_coord;		// Coordinate of the target.
	COORDINATE				fire_coord;			// Coordinate of firing position.
	TechnoTypeClass const & tclass = *Techno_Type_Class();
	ObjectClass		*object;
	WeaponTypeClass const *weapon = (which == 0) ? &Weapons[tclass.Primary] : &Weapons[tclass.Secondary];
	BulletTypeClass const &btype = BulletTypeClass::As_Reference(weapon->Fires);

	/*
	**	Perform a quick legality check to see if firing can occur.
	*/
	if (Debug_Map || weapon->Fires == BULLET_NONE || !Target_Legal(target)) {
		return(NULL);
	}

	/*
	**	Fetch the target coordinate for the target specified.
	*/
	object = As_Object(target);
	if (object) {
		target_coord = object->Target_Coord();
	} else {
		target_coord = As_Coord(target);
	}

	/*
	**	Get the location where the projectile should appear.
	*/
	fire_coord = Fire_Coord(which);

	/*
	**	If the projectile is a homing type (such as a missile), then it will
	**	launch in the direction the turret is facing, NOT necessarily the same
	**	direction as the target.
	*/
	if (btype.IsHoming || btype.IsDropping) {
		dir = Fire_Direction();
		if (btype.IsDropping) {
			fire_coord = Center_Coord();
		}
	} else {
		dir = ::Direction(fire_coord, target_coord);
	}

	/*
	**	Create the projectile. Then process any special operations that
	**	need to be performed according to the style of projectile
	**	created.
	*/
	int firepower = (weapon->Attack > 0) ? ((int)weapon->Attack * House->FirepowerBias) : 0;
 	bullet = new BulletClass(weapon->Fires);
	if (bullet) {
		bullet->Assign_Target(target);
		bullet->Payback = this;
		bullet->Strength = (short)firepower;

		/*
		**	If this is firing from a moving platform, then the projectile is inaccurate.
		*/
		if (Special.IsDefenderAdvantage && What_Am_I() != RTTI_BUILDING && ((FootClass const *)this)->IsDriving) {
			bullet->IsInaccurate = true;
		}

		if (bullet->Unlimbo(fire_coord, dir)) {
//Mono_Printf("Units[0]=%p.\n", Units.Raw_Ptr(0));
//Mono_Printf("Infantry[0]=%p.\n", Infantry.Raw_Ptr(0));
//Mono_Printf("Buildings[0]=%p.\n", Buildings.Raw_Ptr(0));
//Mono_Printf("Aircraft[0]=%p.\n", Aircraft.Raw_Ptr(0));
//Mono_Printf("object=%p, Strength=%d, IsActive=%d, IsInLimbo=%d.\n", object, (long)object->Strength, object->IsActive, object->IsInLimbo);Get_Key();
			bullet->Payback = this;
			bullet->Strength = (short)firepower;
		} else {
			delete bullet;
		}
		if (!bullet->Class->IsFueled) {
			IsInRecoilState = true;
		}
		Arm = Rearm_Delay(IsSecondShot);
		if (tclass.IsTwoShooter) {
			IsSecondShot = (IsSecondShot == false);
		}

		/*
		**	Perform any animation effect for this weapon.
		*/
		AnimType a = weapon->Anim;
		switch (a) {
			case ANIM_GUN_N:
			case ANIM_CHEM_N:
			case ANIM_FLAME_N:
				a = (AnimType)(a + Dir_Facing(Fire_Direction()));
				break;
		}

		/*
		**	If there is a special firing animation, then create and attach it
		**	now.
		*/
		if (a != ANIM_NONE) {
			AnimClass * anim = new AnimClass(a, Fire_Coord(which));
			if (anim) {
				anim->Attach_To(this);
			}
		}

		/*
		**	Reduce ammunition for this object.
		*/
		if (Ammo > 0) {
			Ammo--;
		}

		/*
		**	Firing will in all likelihood, require the unit to be redraw. Flag it to be
		**	redrawn here.
		*/
		Mark(MARK_CHANGE);

		/*
		**	If a projectile was fired from a unit that is hidden in the darkness,
		**	reveal that unit and a little area around it.
		** For multiplayer games, only reveal the unit if the target is the
		** local player.
		*/
#if (0)
		if ((!IsOwnedByPlayer && !IsDiscoveredByPlayer) || !Map[Coord_Cell(Center_Coord())].IsMapped) {
			if (GameToPlay == GAME_NORMAL) {
				Map.Sight_From(Coord_Cell(Center_Coord()), 1, false);
			} else {
				ObjectClass *obj = As_Object(target);
				if (obj) {
					HousesType tgt_owner = obj->Owner();

					if (PlayerPtr->Class->House == tgt_owner) {
						Map.Sight_From(Coord_Cell(Center_Coord()), 1, false);
					}
				}
			}
		}
#else
		/*
		** Now need to reveal for any human player that is the target. ST - 3/13/2019 5:43PM
		*/

		ObjectClass *obj = As_Object(target);
		if (obj) {
			HousesType tgt_owner = obj->Owner();

			HouseClass *player = HouseClass::As_Pointer(tgt_owner);
			if (player != nullptr && player->IsHuman) {
				if ((!Is_Owned_By_Player(player) && !Is_Discovered_By_Player(player)) || !Map[Coord_Cell(Center_Coord())].Is_Mapped(House)) {
					Map.Sight_From(player, Coord_Cell(Center_Coord()), 1, false);
				}
			}
		}

#endif
	}

	if (weapon->Fires == BULLET_LASER) {
		int x, y, x1, y1;
		COORDINATE source, dest;
		source = Fire_Coord(which);
		dest = As_Coord(target);
		if (SpecialDialog == SDLG_NONE) {
			Map.Coord_To_Pixel(source, x, y);
			Map.Coord_To_Pixel(dest, x1, y1);
			x += Map.TacPixelX;
			x1 += Map.TacPixelX;
			y += Map.TacPixelY;
			y1 += Map.TacPixelY;
			Lines[0][0] = x + 1;
			Lines[0][1] = y;
			Lines[0][2] = x1;
			Lines[0][3] = y1;
			Lines[0][4] = 0x7D;
			Lines[1][0] = x - 1;
			Lines[1][1] = y;
			Lines[1][2] = x1;
			Lines[1][3] = y1;
			Lines[1][4] = 0x7D;
			Lines[2][0] = x;
			Lines[2][1] = y;
			Lines[2][2] = x1;
			Lines[2][3] = y1;
			Lines[2][4] = 0x7F;
			LineCount = 3;
			LineFrame = 0;
			LineMaxFrames = 5;
			Map.Flag_To_Redraw(true);
		}
		new SmudgeClass(Random_Pick(SMUDGE_SCORCH1, SMUDGE_SCORCH6), As_Coord(target));
	}

	return(bullet);
}


/***********************************************************************************************
 * TechnoClass::Player_Assign_Mission -- Assigns a mission as result of player input.          *
 *                                                                                             *
 *    This routine is called when the mission for an object needs to change as a result of     *
 *    player input. The basic operation would be to queue the event and let the action         *
 *    occur at the frame dictated by the queing system. However, if a voice response is        *
 *    indicated, then perform it at this time. This will give a greater illusion of            *
 *    immediate response.                                                                      *
 *                                                                                             *
 * INPUT:   order       -- The mission order to assign to this object.                         *
 *                                                                                             *
 *          target      -- The target of this object. This will be used for combat and attack. *
 *                                                                                             *
 *          destination -- The movement destination for this object.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Player_Assign_Mission(MissionType mission, TARGET target, TARGET destination)
{
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
 * TechnoClass::What_Action -- Determines what action to perform if object is selected.        *
 *                                                                                             *
 *    This routine will examine the object specified and return with the action that will      *
 *    be performed if the mouse button were clicked over the object.                           *
 *                                                                                             *
 * INPUT:   object   -- The object that the mouse button might be clicked on.                  *
 *                                                                                             *
 * OUTPUT:  Returns with the action that will be performed if the object was clicked on.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *   03/21/1995 JLB : Special target control for trees.                                        *
 *=============================================================================================*/
ActionType TechnoClass::What_Action(ObjectClass * object) const
{
	if (object) {

		/*
		**	Return the ACTION_SELF flag if clicking on itself. However, if this
		**	object cannot do anything special with itself, then just return with
		**	the no action flag.
		*/
		if (object == this && CurrentObject.Count() == 1 && House == PlayerPtr) {
			return(ACTION_SELF);
		}

		//bool altdown = (Keyboard::Down(KN_LALT) || Keyboard::Down(KN_RALT));
		//bool ctrldown = (Keyboard::Down(KN_LCTRL) || Keyboard::Down(KN_RCTRL));
		//bool shiftdown = (Keyboard::Down(KN_LSHIFT) || Keyboard::Down(KN_RSHIFT));
		//Added for getting the input for special character keys from the client 
		// - 6/26/2019 JAS 
		bool altdown = DLL_Export_Get_Input_Key_State(KN_LALT);
		bool ctrldown = DLL_Export_Get_Input_Key_State(KN_LCTRL);
		bool shiftdown = DLL_Export_Get_Input_Key_State(KN_LSHIFT);

		/*
		**	Special guard area mission is possible if both the control and the
		**	alt keys are held down.
		*/
		// Changed for multiplayer. ST - 3/13/2019 5:52PM
		if (Is_Owned_By_Player() && ctrldown && altdown && Can_Player_Move() && Can_Player_Fire()) {
		//if (IsOwnedByPlayer && ctrldown && altdown && Can_Player_Move() && Can_Player_Fire()) {
			return(ACTION_GUARD_AREA);
		}

		/*
		**	Special override to force a move regardless of what is occupying the location.
		*/
		if (altdown) {
			// Changed for multiplayer. ST - 3/13/2019 5:52PM
			if (Is_Owned_By_Player() && Can_Player_Move()) {
			//if (IsOwnedByPlayer && Can_Player_Move()) {
				return(ACTION_MOVE);
			}
		}

		/*
		**	Override so that toggled select state can be performed while the <SHIFT> key
		**	is held down.
		*/
		bool is_a_loaner = object->Is_Techno() && ((TechnoClass*)object)->IsALoaner;
		if (shiftdown) {
			// Changed for multiplayer. ST - 3/13/2019 5:52PM
			if (!is_a_loaner) {
			//if (IsOwnedByPlayer && !IsALoaner) {
				return(ACTION_TOGGLE_SELECT);
			}
		}

		/*
		**	If firing is possible and legal, then return this action potential.
		*/
		bool control = Keyboard::Down(KN_LCTRL) || Keyboard::Down(KN_RCTRL);
		// Changed for multiplayer. ST - 3/13/2019 5:52PM
		if (Is_Owned_By_Player() && (ctrldown || !House->Is_Ally(object)) && (ctrldown || object->Class_Of().IsLegalTarget || (Special.IsTreeTarget && object->What_Am_I() == RTTI_TERRAIN))) {
		//if (IsOwnedByPlayer && (ctrldown || !House->Is_Ally(object)) && (ctrldown || object->Class_Of().IsLegalTarget || (Special.IsTreeTarget && object->What_Am_I() == RTTI_TERRAIN))) {
			if (Can_Player_Move() || In_Range(object, 0)) {
				// Check for anti-air capability
				if ((object->What_Am_I() != RTTI_AIRCRAFT) ||
					((Techno_Type_Class()->Primary != WEAPON_NONE) && BulletTypeClass::As_Reference(Weapons[Techno_Type_Class()->Primary].Fires).IsAntiAircraft) ||
					((Techno_Type_Class()->Secondary != WEAPON_NONE) && BulletTypeClass::As_Reference(Weapons[Techno_Type_Class()->Secondary].Fires).IsAntiAircraft) ||
					(((AircraftClass *)object)->Altitude == 0)) {
					return(ACTION_ATTACK);
				}
			}
		}

		/*
		**	Possibly try to select the specified object, if that is warranted.
		*/
		// Changed for multiplayer. ST - 3/13/2019 5:52PM
		if (!Is_Weapon_Equipped() || !Is_Owned_By_Player() || object->Owner() == Owner()) {
			if ((!is_a_loaner || !Is_Owned_By_Player()) && object->Class_Of().IsSelectable && (!object->Is_Selected_By_Player() || CurrentObject.Count())) {
		//if (!Is_Weapon_Equipped() || !IsOwnedByPlayer || object->Owner() == Owner()) {
			//if ((!IsALoaner || !IsOwnedByPlayer) && object->Class_Of().IsSelectable && !object->IsSelected) {
				return(ACTION_SELECT);
			}
			return(ACTION_NONE);
		}
	}
	return(ACTION_NONE);
}


/***********************************************************************************************
 * TechnoClass::What_Action -- Determines action to perform if cell is clicked on.             *
 *                                                                                             *
 *    Use this routine to determine what action will be performed if the specified cell        *
 *    is clicked on. Usually this action is either a ACTION_MOVE or ACTION_NOMOVE. The action  *
 *    nomove is used to perform special case checking for nearby cells if in fact the mouse    *
 *    is clicked over the cell.                                                                *
 *                                                                                             *
 * INPUT:   cell  -- The cell to check for being clicked over.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the action that will occur if the cell is clicked on.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *   07/10/1995 JLB : Force fire for buildings is explicitely disabled.                        *
 *=============================================================================================*/
ActionType TechnoClass::What_Action(CELL cell) const
{
	CellClass const * cellptr = &Map[cell];
	OverlayTypeClass const * optr = NULL;

	//bool ctrldown = Keyboard::Down(KN_LCTRL) || Keyboard::Down(KN_RCTRL);
	//bool shiftdown = Keyboard::Down(KN_LSHIFT) || Keyboard::Down(KN_RSHIFT);
	//bool altdown = (Keyboard::Down(KN_LALT) || Keyboard::Down(KN_RALT));
	//Added for getting the input for special character keys from the client 
	// - 6/26/2019 JAS 
	bool altdown = DLL_Export_Get_Input_Key_State(KN_LALT);
	bool ctrldown = DLL_Export_Get_Input_Key_State(KN_LCTRL);
	bool shiftdown = DLL_Export_Get_Input_Key_State(KN_LSHIFT);

	/*
	**	Disable recognizing the <CTRL> key forced fire option when dealing with buildings.
	*/
	if (What_Am_I() == RTTI_BUILDING) ctrldown = false;

	if (cellptr->Overlay != OVERLAY_NONE) {
		optr = &OverlayTypeClass::As_Reference(cellptr->Overlay);
	}

	/*
	**	Special guard area mission is possible if both the control and the
	**	alt keys are held down.
	*/
	// Changed for multiplayer. ST - 3/13/2019 5:52PM
	if (Is_Owned_By_Player() && ctrldown && altdown && Can_Player_Move() && Can_Player_Fire()) {
	//if (IsOwnedByPlayer && ctrldown && altdown && Can_Player_Move() && Can_Player_Fire()) {
		return(ACTION_GUARD_AREA);
	}

	// Changed for multiplayer. ST - 3/13/2019 5:52PM
	if (Is_Owned_By_Player() && Techno_Type_Class()->Primary != WEAPON_NONE && (ctrldown || (optr && optr->IsLegalTarget))) {
	//if (IsOwnedByPlayer && Techno_Type_Class()->Primary != WEAPON_NONE && (ctrldown || (optr && optr->IsLegalTarget))) {
		WarheadTypeClass const * whead = &Warheads[BulletTypeClass::As_Reference(Weapons[Techno_Type_Class()->Primary].Fires).Warhead];
		if (!optr || (optr->IsWall && (whead->IsWallDestroyer || (whead->IsWoodDestroyer && optr->IsWooden)))) {
			if (Can_Player_Move() || In_Range(::As_Target(cell), 0)) {
				return(ACTION_ATTACK);
			}
		}
	}

	// Changed for multiplayer. ST - 3/13/2019 5:52PM
	if (Is_Owned_By_Player() && Can_Player_Move()) {
	//if (IsOwnedByPlayer && Can_Player_Move()) {

		/*
		**	Special override to force a move regardless of what is occupying the location.
		*/
		if (shiftdown) {
			return(ACTION_MOVE);
		}

		/*
		**	If the object can enter the cell specified, then allow
		**	movement to it.
		*/
		if (Can_Enter_Cell(cell) <= MOVE_CLOAK) {
			return(ACTION_MOVE);
		}
		return(ACTION_NOMOVE);
	}
	return(ACTION_NONE);
}


/***********************************************************************************************
 * TechnoClass::Can_Player_Move -- Determines if the object can move be moved by player.       *
 *                                                                                             *
 *    Use this routine to determine whether a movement order can be given to this object.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can this object be given a movement order by the player?                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Can_Player_Move(void) const
{
	return(PlayerPtr == House);
}


/***********************************************************************************************
 * TechnoClass::Can_Player_Fire -- Determines if the player can give this object a fire order. *
 *                                                                                             *
 *    Call this routine to determine if this object can be given a fire order by the player.   *
 *    Such objects will affect the mouse cursor accordingly -- usually causes the targeting    *
 *    cursor to appear.                                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can this object be given firing orders by the player?                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Can_Player_Fire(void) const
{
	if (House->IsHuman && Is_Techno() && Techno_Type_Class()->Primary != WEAPON_NONE) {
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TechnoClass::Is_Weapon_Equipped -- Determines if this object has a combat weapon.          *
 *                                                                                             *
 *    Use this routine to determine if this object is equipped with a combat weapon. Such      *
 *    determination is used by the AI system to gauge the threat potential of the object.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this object equipped with a combat weapon?                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Weapon_Equipped(void) const
{
	return(Techno_Type_Class()->Primary != WEAPON_NONE);
}


/***********************************************************************************************
 * TechnoClass::Can_Repair -- Determines if the object can and should be repaired.             *
 *                                                                                             *
 *    Use this routine to determine if the specified object is a candidate for repair. In      *
 *    order to qualify, the object must be allowed to be repaired (in theory) and it must      *
 *    be below full strength. If these conditions are met, then it can be repaired.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; May this unit be repaired? A return value of false may mean that the object  *
 *                is not allowed to be repaired, or it might be full strength already.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Can_Repair(void) const
{
	/*
	**	Temporary hack to disable repair cursor over non-buildings.
	*/
	if (What_Am_I() == RTTI_UNIT || What_Am_I() == RTTI_INFANTRY || What_Am_I() == RTTI_AIRCRAFT) {
		return(false);
	}
	return(Techno_Type_Class()->IsRepairable && Strength != Class_Of().MaxStrength);
}


/***********************************************************************************************
 * TechnoClass::Weapon_Range -- Determines the maximum range for the weapon.                   *
 *                                                                                             *
 *    Use this routine to determine the maximum range for the weapon indicated.                *
 *                                                                                             *
 * INPUT:   which -- Which weapon to use when determining the range. 0=primary, 1=secondary.   *
 *                                                                                             *
 * OUTPUT:  Returns with the range of the weapon (in leptons).                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Weapon_Range(int which) const
{
	WeaponType weapon = WEAPON_NONE;
	TechnoTypeClass const & ttype = *Techno_Type_Class();

	switch (which) {
		case 0:
			weapon = ttype.Primary;
			break;

		case 1:
			weapon = ttype.Secondary;
			break;
	}
	if (weapon != WEAPON_NONE) {
		if (weapon == WEAPON_NIKE && GameToPlay == GAME_NORMAL) {
			return(Weapons[weapon].Range*2);
		}
		return(Weapons[weapon].Range);
	}
	return(0);
}


/***************************************************************************
 * TechnoClass::Override_Mission -- temporarily overides a units mission   *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:      MissionType mission - the mission we want to overide        *
 *               TARGET      tarcom  - the new target we want to overide   *
 *               TARGET      navcom  - the new navigation point to overide *
 *                                                                         *
 * OUTPUT:      none                                                       *
 *                                                                         *
 * WARNINGS:   If a mission is already overidden, the current mission is   *
 *               just re-assigned.                                         *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/28/1995 PWG : Created.                                             *
 *=========================================================================*/
void TechnoClass::Override_Mission(MissionType mission, TARGET tarcom, TARGET navcom)
{
	SuspendedTarCom = TarCom;
	RadioClass::Override_Mission(mission, tarcom, navcom);
	Assign_Target(tarcom);
}


/***************************************************************************
 * TechnoClass::Restore_Mission -- Restores an overidden mission           *
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
bool TechnoClass::Restore_Mission(void)
{
	if (RadioClass::Restore_Mission()) {
		Assign_Target(SuspendedTarCom);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TechnoClass::Captured -- Handles capturing this object.                                     *
 *                                                                                             *
 *    This routine is called when this object gets captured by the house specified. It handles *
 *    removing this object from any targeting computers and then changes the ownership of      *
 *    the object to the new house.                                                             *
 *                                                                                             *
 * INPUT:   newowner -- Pointer to the house that is now the new owner.                        *
 *                                                                                             *
 * OUTPUT:  Was the object captured? Failure would mean that it is already under control of    *
 *          the house specified.                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Captured(HouseClass * newowner)
{
	if (newowner != House) {

		/*
		**	Capture attempt springs any "entered" trigger. The entered trigger
		**	occurs first since there may be a special trigger attached to this
		**	object that flags a capture as a win and a destroy as a loss. This
		**	order is necessary because the object is recorded as a kill as well.
		*/
		if (Trigger /*&& Trigger->House == newowner->Class->House*/) {
			Trigger->Spring(EVENT_PLAYER_ENTERED, this);
		}

		/*
		**	Record this as a kill.
		*/
		Record_The_Kill(NULL);

#ifdef USE_RA_AI
		//
		// Added for RA AI in TD. ST - 7/26/2019 9:12AM
		//
		House->Tracking_Remove(this);
		newowner->Tracking_Add(this);
#endif // USE_RA_AI

		/*
		**	Special kill record logic for capture process.
		*/
		switch (What_Am_I()) {
			case RTTI_BUILDING:
				if (newowner) newowner->BuildingsKilled[Owner()]++;
				break;

			case RTTI_AIRCRAFT:
			case RTTI_INFANTRY:
			case RTTI_UNIT:
				if (newowner) newowner->UnitsKilled[Owner()]++;
				break;

			default:
				break;
		}
		House->WhoLastHurtMe = newowner->Class->House;

		/*
		**	Remove from targeting computers.
		*/
		Detach_All(false);

#ifdef NEVER
		/*
		**	Break off any radio contact.
		*/
		Transmit_Message(RADIO_OVER_OUT);
#endif

		/*
		**	Change ownership now.
		*/
		House = newowner;
		IsOwnedByPlayer = (House == PlayerPtr);

		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TechnoClass::Take_Damage -- Records damage assessed to this object.                         *
 *                                                                                             *
 *    This routine is called when this object has taken damage. It handles recording whether   *
 *    this object has been destroyed. If it has, then mark the appropriate kill records as     *
 *    necessary.                                                                               *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ResultType TechnoClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source)
{
	/*
	**	Adjust damage according to house override armor value.
	*/
	if (damage > 0) {
		damage = damage * House->ArmorBias;
	}

	ResultType result = ObjectClass::Take_Damage(damage, distance, warhead, source);

	switch (result) {
		case RESULT_DESTROYED:
			Transmit_Message(RADIO_OVER_OUT);
			Stun();

			/*
			** May trigger an achievement. ST - 11/14/2019 1:56PM
			*/
			if (GameToPlay == GAME_NORMAL && !House->IsHuman && source && source->House && source->House->IsHuman) {
				TechnoTypeClass const *object_type = Techno_Type_Class();
				if (object_type) {
					RTTIType what = What_Am_I();
					if (what == RTTI_AIRCRAFT || what == RTTI_INFANTRY || what == RTTI_UNIT) {
						On_Achievement_Event(source->House, "UNIT_DESTROYED", object_type->IniName);
					}
				}			
			}
			break;

		/*
		**	If some damage was received and this object is cloaked, shimmer
		**	the cloak a bit.
		*/
		default:
			if (source && !House->Is_Ally(source)) {
				IsTickedOff = true;
			}
			Do_Shimmer();
			break;

		case RESULT_NONE:
			break;
	}
	return(result);
}


/***********************************************************************************************
 * TechnoTypeClass::Max_Passengers -- Fetches the maximum passengers allowed.                  *
 *                                                                                             *
 *    This routine will return with the maximum number of passengers allowed in this           *
 *    transport. This typically applies to APCs and possibley transport helicopters.           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of passengers this transport can carry.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoTypeClass::Max_Passengers(void) const
{
	if (IsTransporter) {
		return(5);
	}
	return(0);
}


/***********************************************************************************************
 * TechnoClass::Record_The_Kill -- Records the death of this object.                           *
 *                                                                                             *
 *    This routine is used to record the death of this object. It will handle updating the     *
 *    owner house with the kill record as well as springing any trigger events associated with *
 *    this object's death.                                                                     *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the source of this object's death (if there is a source).   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *   08/23/1995 JLB : Building loss is only counted if it received damage.                     *
 *=============================================================================================*/
void TechnoClass::Record_The_Kill(TechnoClass * source)
{
	/*
	**	Handle any trigger event associated with this object.
	*/
	if (Trigger && source) Trigger->Spring(EVENT_ATTACKED, this);

	if (Trigger && source) Trigger->Spring(EVENT_DISCOVERED, this);

	if (Trigger) Trigger->Spring(EVENT_DESTROYED, this);

	if (source) {
		/*
		**	Call the explicity cast versions of the Made_A_Kill function. This
		**	is necessary because we don't want to add a virtual function to the
		**	CrewClass. Doing so would complicate the save/load process.
		*/
		switch (source->What_Am_I()) {
			case RTTI_INFANTRY:
				((InfantryClass *)source)->Made_A_Kill();
				break;

			case RTTI_UNIT:
				((UnitClass *)source)->Made_A_Kill();
				break;

			case RTTI_BUILDING:
				((BuildingClass *)source)->Made_A_Kill();
				break;

			case RTTI_AIRCRAFT:
				((AircraftClass *)source)->Made_A_Kill();
				break;
		}

		House->WhoLastHurtMe = source->Owner();
	}

	switch (What_Am_I()) {
		case RTTI_BUILDING:
			if ( ((BuildingClass *)this)->WhoLastHurtMe != HOUSE_NONE) {
				House->BuildingsLost++;
			}
			if (source){
				if (GameToPlay == GAME_INTERNET){
					source->House->DestroyedBuildings->Increment_Unit_Total( ((BuildingClass*)this)->Class->Type );
				}
				source->House->BuildingsKilled[Owner()]++;
			}

			/*
			** If the map is displaying the multiplayer player names & their
			** # of kills, tell it to redraw.
			*/
			if (Map.Is_Player_Names()) {
				Map.Player_Names(true);
			}
			break;


		case RTTI_AIRCRAFT:
			House->UnitsLost++;
			if (source){
				if (GameToPlay == GAME_INTERNET){
					source->House->DestroyedAircraft->Increment_Unit_Total( ((AircraftClass*)this)->Class->Type );
				}
				source->House->UnitsKilled[Owner()]++;
			}
			/*
			** If the map is displaying the multiplayer player names & their
			** # of kills, tell it to redraw.
			*/
			if (Map.Is_Player_Names()) {
				Map.Player_Names(true);
			}
			break;


		case RTTI_INFANTRY:
			House->UnitsLost++;
			if (source){
				if (GameToPlay == GAME_INTERNET){
					source->House->DestroyedInfantry->Increment_Unit_Total( ((InfantryClass*)this)->Class->Type );
				}
				source->House->UnitsKilled[Owner()]++;
			}
			/*
			** If the map is displaying the multiplayer player names & their
			** # of kills, tell it to redraw.
			*/
			if (Map.Is_Player_Names()) {
				Map.Player_Names(true);
			}
			break;


		case RTTI_UNIT:
			House->UnitsLost++;
			if (source){
				if (GameToPlay == GAME_INTERNET){
					source->House->DestroyedUnits->Increment_Unit_Total( ((UnitClass*)this)->Class->Type );
				}
				source->House->UnitsKilled[Owner()]++;
			}

			/*
			** If the map is displaying the multiplayer player names & their
			** # of kills, tell it to redraw.
			*/
			if (Map.Is_Player_Names()) {
				Map.Player_Names(true);
			}
			break;

		default:
			break;
	}
}


/***********************************************************************************************
 * TechnoClass::Nearby_Location -- Radiates outward looking for clear cell nearby.             *
 *                                                                                             *
 *    This routine is used to find a nearby location from center of this object. It can lean   *
 *    toward finding a location closest to an optional object.                                 *
 *                                                                                             *
 * INPUT:   object   -- Optional object that the finding algorithm will try to find a close    *
 *                      spot to.                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that is closest to this object.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL TechnoClass::Nearby_Location(TechnoClass const * ) const
{
	/*
	**	Radiate outward from the object's location, looking for the best
	**	target.
	*/
	CELL best = 0;
	CELL cell = Coord_Cell(Center_Coord());
	for (int radius = 0; radius < MAP_CELL_W/2; radius++) {

		/*
		**	Scan the top and bottom rows of the "box".
		*/
		for (int x = -radius; x <= radius; x++) {
			CELL newcell = cell + XY_Cell(x, -radius);
			if (Map.In_Radar(newcell) && Map[newcell].Is_Generally_Clear()) {
				best = newcell;
			}

			newcell = cell + XY_Cell(x, radius);
			if (Map.In_Radar(newcell) && Map[newcell].Is_Generally_Clear()) {
				best = newcell;
			}
		}

		/*
		**	Scan the left and right columns of the "box".
		*/
		for (int y = -(radius-1); y < radius; y++) {
			CELL newcell = cell + XY_Cell(-radius, y);
			if (Map.In_Radar(newcell) && Map[newcell].Is_Generally_Clear()) {
				best = newcell;
			}

			newcell = cell + XY_Cell(radius, y);
			if (Map.In_Radar(newcell) && Map[newcell].Is_Generally_Clear()) {
				best = newcell;
			}
		}

		if (best) break;
	}

	return(best);
}


/***********************************************************************************************
 * TechnoClass::Do_Uncloak -- Cause the stealth tank to uncloak.                               *
 *                                                                                             *
 *    This routine will start the stealth tank to uncloak.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Do_Uncloak(void)
{
	if (IsCloakable && (Cloak == CLOAKED || Cloak == CLOAKING)) {
		Sound_Effect(VOC_CLOAK, Coord);
		Cloak = UNCLOAKING;
		CloakingDevice.Set_Stage(0);
		CloakingDevice.Set_Rate(1);
	}
}


/***********************************************************************************************
 * TechnoClass::Do_Cloak -- Start the object into cloaking stage.                              *
 *                                                                                             *
 *    This routine will start the object into its cloaking state.                              *
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
void TechnoClass::Do_Cloak(void)
{
	if (IsCloakable && (Cloak == UNCLOAKED || Cloak == UNCLOAKING)) {
		Sound_Effect(VOC_CLOAK, Coord);
		Detach_All(false);
		Cloak = CLOAKING;
		CloakingDevice.Set_Stage(0);
		CloakingDevice.Set_Rate(1);
	}
}


/***********************************************************************************************
 * TechnoClass::Do_Shimmer -- Causes this object to shimmer if it is cloaked.                  *
 *                                                                                             *
 *    This routine is called when this object should shimmer. If the object is cloaked, then   *
 *    a shimmering effect (partial decloak) occurs. For objects that are not cloaked, no       *
 *    affect occurs.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Do_Shimmer(void)
{
	if (IsCloakable && Cloak == CLOAKED) {
		Cloak = CLOAKING;
		CloakingDevice.Set_Stage(MAX_UNCLOAK_STAGE/2);
		CloakingDevice.Set_Rate(1);
	}
}


/***********************************************************************************************
 * TechnoClass::Visual_Character -- Determine the visual character of the object.              *
 *                                                                                             *
 *    This routine will determine how this object should be drawn. Typically, this is the      *
 *    unmodified visible state, but cloaked objects have a different character.                *
 *                                                                                             *
 * INPUT:   raw   -- Should the check be based on the unmodified cloak condition of the        *
 *                   object? If false, then an object owned by the player will never become    *
 *                   completely invisible.                                                     *
 *                                                                                             *
 * OUTPUT:  Returns with the visual character to use when displaying this object.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/07/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
VisualType TechnoClass::Visual_Character(bool raw)
{
	/*
	**	When uncloaked or in map editor mode, always draw the object normally.
	*/
	if (Cloak == UNCLOAKED || Debug_Map) return(VISUAL_NORMAL);

	/*
	**	A cloaked unit will not be visible at all unless it is owned
	**	by the player.
	*/
	if (Cloak == CLOAKED) {
		// Changed for multiplayer. Not needed except to test in the old renderer. ST - 3/13/2019 5:56PM
		if (!raw && Is_Owned_By_Player()) return(VISUAL_SHADOWY);
		//if (!raw && IsOwnedByPlayer) return(VISUAL_SHADOWY);
		return(VISUAL_HIDDEN);
	}

	int stage = CloakingDevice.Fetch_Stage();
	if (Cloak == UNCLOAKING) stage = MAX_UNCLOAK_STAGE - stage;
	if (stage <= 0) {
		return(VISUAL_NORMAL);
	}

	stage = Cardinal_To_Fixed(MAX_UNCLOAK_STAGE, stage);

	if (stage < 0x0040) return(VISUAL_INDISTINCT);
	if (stage < 0x0080) return(VISUAL_DARKEN);
	if (stage < 0x00C0) return(VISUAL_SHADOWY);
	if (!raw && Is_Owned_By_Player()) return(VISUAL_SHADOWY);		// Changed for multiplayer. Not needed except to test in the old renderer. ST - 3/13/2019 5:56PM
	//if (!raw && IsOwnedByPlayer) return(VISUAL_SHADOWY);
	if (stage < 0x00FF) return(VISUAL_RIPPLE);
	return(VISUAL_HIDDEN);
}


/***********************************************************************************************
 * TechnoClass::Techno_Draw_Object -- General purpose draw object routine.                     *
 *                                                                                             *
 *    This routine is used to draw the object. It will handle any remapping or cloaking        *
 *    effects required. This logic is isolated here since all techno object share the same     *
 *    render logic when it comes to remapping and cloaking.                                    *
 *                                                                                             *
 * INPUT:   shapefile   -- Pointer to the shape file that the shape will be drawn from.        *
 *                                                                                             *
 *          shapenum    -- The shape number of the object in the file to use.                  *
 *                                                                                             *
 *          x,y         -- Center pixel coordinate to use for rendering this object.           *
 *                                                                                             *
 *          window      -- The clipping window to use when rendering.                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Techno_Draw_Object(void const * shapefile, int shapenum, int x, int y, WindowNumberType window)
{
	if (shapefile) {
		VisualType visual = Visual_Character();
		void const * remap = Remap_Table();

		// Server still needs to "render" hidden objects to the virtual window, so objects get created properly - SKY
		if ((visual == VISUAL_HIDDEN) && (window == WINDOW_VIRTUAL)) {
			visual = VISUAL_SHADOWY;
		}

		if (visual != VISUAL_HIDDEN && visual != VISUAL_RIPPLE) {
			if (visual == VISUAL_SHADOWY) {
				CC_Draw_Shape(this, shapefile, shapenum, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING|SHAPE_PREDATOR, NULL, Map.FadingShade);
			} else {
				CC_Draw_Shape(this, shapefile, shapenum, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING|SHAPE_GHOST, remap, Map.UnitShadow);
			}
			if (visual == VISUAL_DARKEN) {
				CC_Draw_Shape(this, shapefile, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING, remap, Map.FadingShade);
			}
		}
		if (visual != VISUAL_NORMAL && visual != VISUAL_HIDDEN) {
			CC_Draw_Shape(this, shapefile, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL);
		}
	}
}



/***********************************************************************************************
 * TechnoClass::Techno_Draw_Object_Virtual -- Draw object with virtual window support          *
 *                                                                                             *
 * INPUT:   shapefile   -- Pointer to the shape file that the shape will be drawn from.        *
 *                                                                                             *
 *          shapenum    -- The shape number of the object in the file to use.                  *
 *                                                                                             *
 *          x,y         -- Center pixel coordinate to use for rendering this object.           *
 *                                                                                             *
 *          window      -- The clipping window to use when rendering.                          *
 *                                                                                             *
 *          shape_name  -- The name of the shapefile                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   6/20/2019 1:31PM - ST : Created.                                                          *
 *=============================================================================================*/
void TechnoClass::Techno_Draw_Object_Virtual(void const * shapefile, int shapenum, int x, int y, WindowNumberType window, const char *shape_name)
{
	if (shape_name == NULL || *shape_name == 0) {
		/*
		** If there's no override shape name, then call the regular draw
		*/
		Techno_Draw_Object(shapefile, shapenum, x, y, window);
		return;
	}
	
	if (shapefile) {
		VisualType visual = Visual_Character();
		void const * remap = Remap_Table();

		// Server still needs to "render" hidden objects to the virtual window, so objects get created properly - SKY
		if ((visual == VISUAL_HIDDEN) && (window == WINDOW_VIRTUAL)) {
			visual = VISUAL_SHADOWY;
		}

		if (visual != VISUAL_HIDDEN && visual != VISUAL_RIPPLE) {
			if (visual == VISUAL_SHADOWY) {
				CC_Draw_Shape(this, shape_name, shapefile, shapenum, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING|SHAPE_PREDATOR, NULL, Map.FadingShade);
			} else {
				CC_Draw_Shape(this, shape_name, shapefile, shapenum, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING|SHAPE_GHOST, remap, Map.UnitShadow);
			}
			if (visual == VISUAL_DARKEN) {
				CC_Draw_Shape(this, shape_name, shapefile, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING, remap, Map.FadingShade);
			}
		}
		if (visual != VISUAL_NORMAL && visual != VISUAL_HIDDEN) {
			CC_Draw_Shape(this, shape_name, shapefile, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL);
		}
	}
}



/***********************************************************************************************
 * TechnoClass::Remap_Table -- Fetches the appropriate remap table to use.                     *
 *                                                                                             *
 *    This routine is used to fetch the appropriate remap table to use for this object.        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the remap table to use for this object.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void const * TechnoClass::Remap_Table(void)
{
	return(House->Remap_Table(IsBlushing, true));
}


/***********************************************************************************************
 * TechnoClass::Detach -- Handles removal of target from tracking system.                      *
 *                                                                                             *
 *    This routine is called when the specified object is about to be removed from the game    *
 *    system. The target object is removed from any tracking computers that this object may    *
 *    have.                                                                                    *
 *                                                                                             *
 * INPUT:   target   -- The target object (as a target value) that is being removed from the   *
 *                      game.                                                                  *
 *                                                                                             *
 *          all      -- Is the target about to die? A false value might indicate that the      *
 *                      object is merely cloaking. In such a case, radio contact will not      *
 *                      be affected.                                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Detach(TARGET target, bool all)
{
	RadioClass::Detach(target, all);

	if (SuspendedMission != MISSION_NONE && SuspendedTarCom == target) {
		SuspendedMission = MISSION_NONE;
		SuspendedTarCom = TARGET_NONE;
	}

	/*
	**	If the targeting computer is assigned to the target, then the targeting
	**	computer must be cleared.
	*/
	if (TarCom == target) {
		Assign_Target(TARGET_NONE);
		Restore_Mission();
	}

	/*
	**	If it is in radio contact with another object, then that radio contact
	**	must be broken.
	*/
	if (all && In_Radio_Contact() && Contact_With_Whom()->As_Target() == target) {
		Transmit_Message(RADIO_OVER_OUT);
	}
}


/***********************************************************************************************
 * TechnoClass::Kill_Cargo -- Destroys any cargo attached to this object.                      *
 *                                                                                             *
 *    This routine handles the destruction of any cargo this object may contain. Typical of    *
 *    this would be when a transport helicopter gets destroyed.                                *
 *                                                                                             *
 * INPUT:   source   -- The source of the destruction of the cargo.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Kill_Cargo(TechnoClass * source)
{
	while (Is_Something_Attached()) {
		FootClass * foot = Detach_Object();
		if (foot) {
			foot->Record_The_Kill(source);
			delete foot;
		}
	}
}


/***********************************************************************************************
 * TechnoClass::Crew_Type -- Fetches the kind of crew this object contains.                    *
 *                                                                                             *
 *    This routine is called when generating survivors to this object. This routine returns    *
 *    the type of survivor to generate.                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the infantry type of a survivor.                                           *
 *                                                                                             *
 * WARNINGS:   This routine is designed to be called repeatedly. Once for each survivor to     *
 *             generate.                                                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryType TechnoClass::Crew_Type(void) const
{
	InfantryType infantry = INFANTRY_E1;
	if (House->ActLike == HOUSE_NEUTRAL) {
		infantry = Random_Pick(INFANTRY_C1, INFANTRY_C9);
	} else {
		if (Techno_Type_Class()->Primary == WEAPON_NONE && Random_Pick(0, 6) == 1) {
			if (Random_Pick(0, 1) == 0) {
				infantry = INFANTRY_C1;
			} else {
				infantry = INFANTRY_C7;
			}
		}
	}
	return(infantry);
}


/***********************************************************************************************
 * TechnoClass::Value -- Fetches the target value for this object.                             *
 *                                                                                             *
 *    This routine is used to fetch the target value for this object. The greater the value    *
 *    returned, the better this object is as a target.                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the target value for this object.                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *   08/16/1995 JLB : Adjusted for early mission lame-out.                                     *
 *=============================================================================================*/
int TechnoClass::Value(void) const
{
	int value = 0;

	/*
	**	In early missions, contents of transports are not figured
	**	into the total value. - 8/16/95
	*/
	if (BuildLevel > 8 || GameToPlay != GAME_NORMAL) {
		if (Is_Something_Attached()) {
			FootClass * object = Attached_Object();

			while (object) {
				value += object->Value();
				object = (FootClass *)object->Next;
			}
		}
	}
	return Risk() + Techno_Type_Class()->Reward + value;
}


/***********************************************************************************************
 * TechnoClass::Threat_Range -- Returns the range to scan based on threat control.             *
 *                                                                                             *
 *    This routine will return the range to scan based on the control value specified. The     *
 *    value returned by this routine is typically used when scanning for enemies.              *
 *                                                                                             *
 * INPUT:   control  -- The range control parameter.                                           *
 *                      0  = Use weapon range (zero is returned in this special case).         *
 *                      -1 = Scan without range restrictions (-1 is returned in this case).    *
 *                      1  = Scan up to twice weapon range.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a range (or special value) that can be used in the threat scan        *
 *          process. If zero is returned, then always check threat against In_Range(). If      *
 *          -1 is returned, then no range limitation restriction exists.                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Threat_Range(int control) const
{
	if (control == -1) return(-1);
	if (control == 0) return(0);

	int range = MAX(Weapon_Range(0), Weapon_Range(1));
	range *= 2;
	range = Bound(range, 0x0000, 0x0A00);
	return(range);
}


/***********************************************************************************************
 * TechnoClass::Base_Is_Attacked -- Handle panic response to base being attacked.              *
 *                                                                                             *
 *    This routine is called when the base is being attacked. It will pull units off of the    *
 *    field and send them back to defend the base. This routine will make taking an enemy      *
 *    base much more difficult.                                                                *
 *                                                                                             *
 * INPUT:   enemy -- Pointer to the enemy object that did the damage on the base.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine can drastically affect the game play. The computer will probably   *
 *             call off its attacks as a result.                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Commented.                                                               *
 *=============================================================================================*/
void TechnoClass::Base_Is_Attacked(TechnoClass const *enemy)
{
	FootClass *defender[6];
	int value[6];
	int count = 0;
	int weakest = 0;
	int desired = enemy->Risk() * 2;
	int risktotal = 0;

	/*
	** Humans have to deal with their own base is attacked problems.
	*/
	if (!enemy || House->Is_Ally(enemy) || House->IsHuman) {
		return;
	}

	/*
	**	Don't overreact if this building can defend itself.
	*/
	if (Techno_Type_Class()->Primary != WEAPON_NONE) return;

	/*
	** If the enemy is not an infantry or a unit there is not much we can
	** do about it.
	*/
	if (enemy->What_Am_I() != RTTI_INFANTRY && enemy->What_Am_I() != RTTI_UNIT ) {
		return;
	}

	/*
	**	If the enemy is a gunboat, then don't do anything.
	*/
	// This should allow helicopters to retaliate however. Hmmm.
	if (enemy->What_Am_I() == RTTI_UNIT && (*((UnitClass const *)enemy) == UNIT_GUNBOAT || *((UnitClass const *)enemy) == UNIT_HOVER)) {
		return;
	}

	/*
	** If the threat has already been dealt with then we don't need to do
	** any work.  Check for that here.
	*/
	if (!((FootClass *)enemy)->BaseAttackTimer.Expired()) {
		return;
	}

	/*
	** We will need units to defend our base.  We need to suspend teams until
	** the situation has been dealt with.
	*/
	TeamClass::Suspend_Teams(20);

	/*
	** Loop through the infantry looking for those who are capable of going
	** on a rescue mission.
	*/
	int index;
	for (index = 0; index < Infantry.Count() && desired > 0; index++) {
	 	InfantryClass * infantry = Infantry.Ptr(index);
		if (infantry && infantry->Owner() == Owner()) {

			/*
			**	Never recruite sticky guard units to defend a base.
			*/
			if (infantry->Mission == MISSION_STICKY || infantry->Mission == MISSION_SLEEP) continue;

			/*
			** Find the amount of threat that this unit can apply to the
			** enemy.
			*/
			int threat = infantry->Rescue_Mission(enemy->As_Target());

			/*
			** If it can't apply any threat then do just skip it and do not
			** add it to the list.
			*/
			if (!threat) {
				continue;
			}

			/*
			** If the value returned is negative then this unit is already
			** assigned to fighting the enemy, so subtract its value from
			** the enemys desired value.
			*/
			if (threat < 0) {
				desired += threat;
				continue;
			}

			if (count < 6) {
				defender[count] = (FootClass *)infantry;
				value[count] = threat;
				count++;
				continue;
			}

			if (threat > weakest) {
				int newweakest = threat;

				for (int lp = 0; lp < count; lp ++) {
					if (value[lp] == weakest) {
						value[lp] = threat;
						defender[lp] = (FootClass *) infantry;
						continue;
					}
					if (value[count] < newweakest) {
						newweakest = value[lp];
					}
				}
				weakest = newweakest;
			}
		}
	}

	/*
	** Loop through the units looking for those who are capable of going
	** on a rescue mission.
	*/
	for (index = 0; index < Units.Count() && desired > 0; index++) {
	 	UnitClass * unit = Units.Ptr(index);
		if (unit && unit->Owner() == Owner()) {

			/*
			**	Never recruite sticky guard units to defend a base.
			*/
			if (unit->Mission == MISSION_STICKY || unit->Mission == MISSION_SLEEP) continue;

			/*
			** Find the amount of threat that this unit can apply to the
			** enemy.
			*/
			int threat = unit->Rescue_Mission(enemy->As_Target());

			/*
			** If it can't apply any threat then do just skip it and do not
			** add it to the list.
			*/
			if (!threat) {
				continue;
			}

			/*
			** If the value returned is negative then this unit is already
			** assigned to fighting the enemy, so subtract its value from
			** the enemys desired value.
			*/
			if (threat < 0) {
				desired += threat;
				continue;
			}

			if (count < 6) {
				defender[count] = (FootClass *)unit;
				value[count] = threat;
				count++;
				continue;
			}
			if (threat > weakest) {
				int newweakest = threat;

				for (int lp = 0; lp < count; lp ++) {
					if (value[lp] == weakest) {
						value[lp] = threat;
						defender[lp] = (FootClass *) unit;
						continue;
					}
					if (value[count] < newweakest) {
						newweakest = value[lp];
					}
				}
				weakest = newweakest;
			}
		}
	}

	if (desired > 0) {

		/*
		** Sort the defenders by value, this doesn't take very long and will
		** help the closest defenders to respond.
		*/
		int lp;
		for (lp = 0; lp < count - 1; lp ++) {
			for (int lp2 = lp + 1; lp2 < count; lp2++) {
				if (value[lp] < value[lp2]) {

					value[lp] 	^= value[lp2];
					value[lp2]	^= value[lp];
					value[lp]	^= value[lp2];

					FootClass *temp;
					temp				= defender[lp];
					defender[lp]	= defender[lp2];
					defender[lp2]  = temp;
				}
			}
		}

		for (lp = 0; lp < count; lp ++) {
			defender[lp]->Assign_Mission(MISSION_RESCUE);
			defender[lp]->Assign_Target(enemy->As_Target());
			risktotal += defender[lp]->Risk();
			if (risktotal > desired) {
				break;
			}
		}
	}

	if (risktotal > desired) {
		((FootClass *)enemy)->BaseAttackTimer.Set(15 * 15);
	}
}


/***********************************************************************************************
 * TechnoClass::Get_Ownable -- Fetches the ownable bits for this object.                       *
 *                                                                                             *
 *    This routine will return the ownable bits for this object. The ownable bits represent    *
 *    the houses that are allowed to own this object.                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the ownable bits for this object.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
unsigned char TechnoClass::Get_Ownable(void) const
{
	return ((TechnoTypeClass const &)Class_Of()).Ownable;
}


/***********************************************************************************************
 * TechnoClass::Is_Techno -- Confirms that this is a TechnoClass object.                       *
 *                                                                                             *
 *    This routine is called to confirm if this object is derived from the TechnoClass         *
 *    object. At this level, the return value will always be true.                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Is this object a TechnoClass or derived from it?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Techno(void) const
{
	return(true);
}


/***********************************************************************************************
 * TechnoClass::Risk -- Fetches the risk associated with this object.                          *
 *                                                                                             *
 *    This routine is called when the risk value for this object needs to be determined.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the risk value for this object.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Risk(void) const
{
	return(Techno_Type_Class()->Risk);
}


/***********************************************************************************************
 * TechnoClass::Tiberium_Load -- Fetches the current tiberium load percentage.                 *
 *                                                                                             *
 *    This routine will return the current Tiberium load (expressed as a fixed point fraction) *
 *    that this object currently contains. Typical implementor of this function would be       *
 *    the harvester. Any object that can return a non-zero value should derive from this       *
 *    function in order to return the appropriate value.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current Tiberium load expressed as a fixed point number.          *
 *          0x0000   = empty                                                                   *
 *          0x0080   = half full                                                               *
 *          0x0100   = full                                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Tiberium_Load(void) const
{
	return(0x0000);
}


/***********************************************************************************************
 * TechnoClass::Desired_Load_Dir -- Fetches loading parameters for this object.                *
 *                                                                                             *
 *    This routine is called when an object desires to load up on this object. The object      *
 *    desiring to load is specified. The cell that the loading object should move to is        *
 *    determined. The direction that this object should face is also calculated. This routine  *
 *    will be overridden by those objects that can actually load up passengers.                *
 *                                                                                             *
 * INPUT:   object   -- The object that is desiring to load up.                                *
 *                                                                                             *
 *          moveto   -- Reference to the cell that the loading object should move to before    *
 *                      the final load process occurs (this value will be filled in).          *
 *                                                                                             *
 * OUTPUT:  Returns with the direction that the transport object should face.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType TechnoClass::Desired_Load_Dir(ObjectClass * , CELL & moveto) const
{
	moveto = 0;
	return(DIR_N);
}


/***********************************************************************************************
 * TechnoClass::Pip_Count -- Fetches the number of pips to display on this object.             *
 *                                                                                             *
 *    This routine will return the number of pips to display on this object when the object    *
 *    is selected. The default condition is to return no pips at all. This routine is          *
 *    derived for those objects that can have pips.                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of pips to display on this object when selected.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Pip_Count(void) const
{
	return(0);
}


/***********************************************************************************************
 * TechnoClass::Fire_Direction -- Fetches the direction projectile fire will take.             *
 *                                                                                             *
 *    This routine will fetch the direction that a fired projectile will take. This is         *
 *    usually the facing of the object's weapon. This routine will be derived for the objects  *
 *    that have their weapon barrel facing a different direction than the body.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the direction a fired projectile will take.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType TechnoClass::Fire_Direction(void) const
{
	return(PrimaryFacing.Current());
}


/***********************************************************************************************
 * TechnoClass::Response_Select -- Handles the voice response when selected.                   *
 *                                                                                             *
 *    This routine is called when a voice reponse to a select action is desired. This routine  *
 *    should be overridden for any object that actually has a voice response.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine can generate an audio response.                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Response_Select(void)
{
}


/***********************************************************************************************
 * TechnoClass::Response_Move -- Handles the voice repsonse to a movement request.             *
 *                                                                                             *
 *    This routine is called when a voice response to a movement order is desired. This        *
 *    routine should be overridden for any object that actually has a voice response.          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This can generate an audio repsonse.                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Response_Move(void)
{
}


/***********************************************************************************************
 * TechnoClass::Response_Attack -- Handles the voice response when given attack order.         *
 *                                                                                             *
 *    This routine is called when a voice response to an attack order is desired. This routine *
 *    should be overridden for any object that actually have a voice response.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This can generate an audio response.                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Response_Attack(void)
{
}


/***********************************************************************************************
 * TechnoClass::Target_Something_Nearby -- Handles finding and assigning a nearby target.      *
 *                                                                                             *
 *    This routine will search for a nearby target and assign it to this object's TarCom.      *
 *    The method to use when scanning for a target is controlled by the parameter passed.      *
 *                                                                                             *
 * INPUT:   threat   -- The threat control parameter used to control the range searched. The   *
 *                      only values recognized are THREAT_RANGE and THREAT_AREA.               *
 *                                                                                             *
 * OUTPUT:  Was a suitable target aquired and assigned to the TarCom?                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Target_Something_Nearby(ThreatType threat)
{
	threat = threat & (THREAT_RANGE|THREAT_AREA);

	/*
	**	Determine that if there is an existing target it is still legal
	**	and within range.
	*/
	if (Target_Legal(TarCom)) {
		if ((threat & THREAT_RANGE) && !In_Range(TarCom)) {
			Assign_Target(TARGET_NONE);
		}
	}

	/*
	**	If there is no target, then try to find one and assign it as
	**	the target for this unit.
	*/
	if (!Target_Legal(TarCom)) {
		Assign_Target(Greatest_Threat(threat));
	}

	/*
	**	Return with answer to question: Does this unit have a target?
	*/
	return(Target_Legal(TarCom));
}


/***********************************************************************************************
 * TechnoClass::Exit_Object -- Causes specified object to leave this object.                   *
 *                                                                                             *
 *    This routine is called when there is an attached object that should detach and leave     *
 *    this object. Typical of this would be the refinery and APC.                              *
 *                                                                                             *
 * INPUT:   object   -- The object that is trying to leave this object.                        *
 *                                                                                             *
 * OUTPUT:  Was the object successfully launched from this object? Failure might indicate that *
 *          there is insufficient room to detach the specified object.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Exit_Object(TechnoClass *)
{
	return(0);
}


/***********************************************************************************************
 * TechnoClass::Random_Animate -- Performs some idle animation for the object.                 *
 *                                                                                             *
 *    This is a maintenance routine that is called when the object might want to check to see  *
 *    if it should go into some idle animation. Infantry are a good example of objects that    *
 *    perform idle animations. This routine must be overridden by the derived object types     *
 *    in order to give it some functionality.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Random_Animate(void)
{
}


/***********************************************************************************************
 * TechnoClass::Assign_Destination -- Assigns movement destination to the object.              *
 *                                                                                             *
 *    This routine is called when the object needs to have a new movement destination          *
 *    assigned. This routine must be overridden since at this level, movement is not allowed.  *
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
void TechnoClass::Assign_Destination(TARGET )
{
}


/***********************************************************************************************
 * TechnoClass::Scatter -- Causes the object to scatter to an adjacent cell.                   *
 *                                                                                             *
 *    This routine is called when the object needs to get out of the way. This might be as a   *
 *    result of combat or findpath reasons.                                                    *
 *                                                                                             *
 * INPUT:   coord    -- The source of the reason to scatter. The object should try to run      *
 *                      away from this coordinate.                                             *
 *                                                                                             *
 *          forced   -- Is the scatter a forced scatter? If false, then this is merely a       *
 *                      request that scattering might be a good idea.                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Scatter(COORDINATE , bool , bool )
{
}


/***********************************************************************************************
 * TechnoClass::Enter_Idle_Mode -- Object enters its default idle condition.                   *
 *                                                                                             *
 *    This routine is called when the object should intelligently revert to an idle state.     *
 *    Typically this routine is called after some mission has completed. This routine must     *
 *    be overridden by the various object types. It is located at this level merely to provide *
 *    a virtual function entry point.                                                          *
 *                                                                                             *
 * INPUT:   initial  -- Is this called when the unit just leaves a factory or is initially     *
 *                      or is initially placed on the map?                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Enter_Idle_Mode(bool )
{
}


/***********************************************************************************************
 * TechnoClass::Draw_Pips -- Draws the transport pips and other techno graphics.               *
 *                                                                                             *
 *    This routine is used to render the small transportation pip (occupant feedback graphic)  *
 *    used for transporter object. It will also display if the techno object is "primary"      *
 *    if necessary.                                                                            *
 *                                                                                             *
 * INPUT:   x,y   -- The pixel coordinate for the center of the first pip. Subsiquent pips     *
 *                   are drawn rightward.                                                      *
 *                                                                                             *
 *          window-- The window that pip clipping is relative to.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Draw_Pips(int x, int y, WindowNumberType window)
{
	/*
	**	Transporter type objects have a different graphic representation for the pips. The
	**	pip color represents the type of occupant.
	*/
	if (Techno_Type_Class()->IsTransporter) {
		ObjectClass const * object = Attached_Object();

		for (int index = 0; index < Class_Of().Max_Pips(); index++) {
			PipEnum pip = PIP_EMPTY;

			if (object) {
				pip = PIP_FULL;
				if (object->What_Am_I() == RTTI_INFANTRY) {
					if (*((InfantryClass *)object) == INFANTRY_RAMBO) {
						pip = PIP_COMMANDO;
					}
					if (*((InfantryClass *)object) == INFANTRY_E7) {
						pip = PIP_ENGINEER;
					}
					if (((InfantryClass *)object)->Class->IsCivilian) {
						pip = PIP_CIVILIAN;
					}
				}
				object = object->Next;
			}
			CC_Draw_Pip(this, Class_Of().PipShapes, pip, x+index*3, y, window, SHAPE_CENTER|SHAPE_WIN_REL);
		}

	} else {

		/*
		**	Display number of how many attached objects there are. This is also used
		**	to display the fullness rating for a harvester.
		*/
		int pips = Pip_Count();
		for (int index = 0; index < Class_Of().Max_Pips(); index++) {
			CC_Draw_Pip(this, Class_Of().PipShapes, (index < pips) ? PIP_FULL : PIP_EMPTY, x+index*3, y, window, SHAPE_CENTER|SHAPE_WIN_REL);
		}
	}

	/*
	**	Display whether this unit is a leader unit or not.
	*/
	if (IsLeader && (window != WINDOW_VIRTUAL)) {
		CC_Draw_Pip(this, Class_Of().PipShapes, PIP_PRIMARY, x-2, y-3, window, /*SHAPE_CENTER|*/SHAPE_WIN_REL);
	}
}


/***********************************************************************************************
 * TechnoClass::Find_Docking_Bay -- Searches for a close docking bay.                          *
 *                                                                                             *
 *    This routine will be used to find a building that can serve as a docking bay. The        *
 *    closest building that qualifies will be returned. If no building could be found then     *
 *    return with NULL.                                                                        *
 *                                                                                             *
 * INPUT:   b  -- The structure type to look for.                                              *
 *                                                                                             *
 *          friendly -- Allow searching for allied buildings as well.                          *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building that can serve as the best docking bay.     *
 *                                                                                             *
 * WARNINGS:   This routine might return NULL even if there are buildings of the specified     *
 *             type available. This is the case when the building(s) are currently busy and    *
 *             cannot serve as a docking bay at the moment.                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *   08/13/1995 JLB : Recognizes the "IsLeader" method of building preference.                 *
 *=============================================================================================*/
BuildingClass * TechnoClass::Find_Docking_Bay(StructType b, bool friendly) const
{
	BuildingClass * best = 0;

	/*
	**	First check to see if there are ANY buildings of the specified
	**	type in thi house's inventory. If not, then don't bother to scan
	**	for one.
	*/
	if (House->BScan & (1L << b)) {
		int bestval = -1;

		/*
		**	Loop through all the buildings and find the one that matches the specification
		**	and is willing to dock with this object.
		*/
		for (int index = 0; index < Buildings.Count(); index++) {
			BuildingClass * building = Buildings.Ptr(index);

			/*
			**	Check to see if the building qualifies (preliminary scan).
			*/
			if (building &&
				(friendly ? building->House->Is_Ally(this) : building->House == House) &&
				!building->IsInLimbo &&
				*building == b &&
				((TechnoClass *)this)->Transmit_Message(RADIO_CAN_LOAD, building) == RADIO_ROGER) {

				/*
				**	If the building qualifies and this building is better than the
				**	last qualifying building (as rated by distance), then record
				**	this building and keep scanning.
				*/
				if (bestval == -1 || Distance(building) < bestval || building->IsLeader) {
					best = building;
					bestval = Distance(building);
				}
			}
		}
	}
	return(best);
}


/***********************************************************************************************
 * TechnoClass::Find_Exit_Cell -- Finds an appropriate exit cell for this object.              *
 *                                                                                             *
 *    This routine is called when an object would like to exit from this (presumed) transport. *
 *    A suitable cell should be returned by this routine. The specified object will probably   *
 *    be unloaded at that cell.                                                                *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object that would like to unload. This is used to       *
 *                      determine suitability for placement.                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that is recommended for object exit.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL TechnoClass::Find_Exit_Cell(TechnoClass const * ) const
{
	return(Coord_Cell(Docking_Coord()));
}


/***********************************************************************************************
 * TechnoClass::Refund_Amount -- Returns with the money to refund if this object is sold.      *
 *                                                                                             *
 *    This routine is used by the selling back mechanism in order to credit the owning house   *
 *    with some refund credits. The value returned is the credits to refund to the owner.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the credits to refund to the owner.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Refund_Amount(void) const
{
	int cost = Techno_Type_Class()->Raw_Cost() * House->CostBias;

	/*
	**	If the object is carrying Tiberium directly (i.e., the harvester), then
	**	account for the credits of the load.
	*/
	cost += Fixed_To_Cardinal(UnitTypeClass::FULL_LOAD_CREDITS, Tiberium_Load())/2;

	if (House->IsHuman) {
		cost /= 2;
	}
	return(cost);
}








/*
**  Additions to TechnoClass to track discovery per-player. ST - 3/6/2019 11:18AM
** 
** 
** 
** 
** 
*/


/***********************************************************************************************
 * TechnoClass::Is_Discovered_By_Player -- Has this object been disovered by the given player  *
 *                                                                                             *
 * INPUT:   Player pointer                                                                     *
 *                                                                                             *
 * OUTPUT:  True if discovered by that player                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   3/6/2019 11:20AM - ST                                                                     *
 *=============================================================================================*/
bool TechnoClass::Is_Discovered_By_Player(HouseClass *player) const
{
	if (player && player->Class) {
		int shift = (int) player->Class->House;
		return (IsDiscoveredByPlayerMask & (1 << shift)) ? true : false;
	} else {
		int shift = (int) PlayerPtr->Class->House;
		return (IsDiscoveredByPlayerMask & (1 << shift)) ? true : false;
	}	
	return false;
}
	

/***********************************************************************************************
 * TechnoClass::Set_Discovered_By_Player -- Mark that this object been disovered by the player *
 *                                                                                             *
 * INPUT:   Player pointer                                                                     *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   3/6/2019 11:23AM - ST                                                                     *
 *=============================================================================================*/
void TechnoClass::Set_Discovered_By_Player(HouseClass *player)
{
	int shift = 0;
	if (player && player->Class) {
		shift = (int) player->Class->House;
	} else {
		shift = (int)PlayerPtr->Class->House;
	}	
	IsDiscoveredByPlayerMask |= (1 << shift);

	if (GameToPlay == GAME_NORMAL && player == PlayerPtr) {
		IsDiscoveredByPlayer = true;
	}
}



/***********************************************************************************************
 * TechnoClass::Clear_Discovered_By_Players -- Clear player discovery flags                    *
 *                                                                                             *
 * INPUT:   Player pointer                                                                     *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   4/27/2020 - SKY                                                                           *
 *=============================================================================================*/
void TechnoClass::Clear_Discovered_By_Players()
{
	IsDiscoveredByPlayerMask = 0;
	IsDiscoveredByPlayer = false;
}



/***********************************************************************************************
 * TechnoClass::Is_Owned_By_Player -- Is this object owned by the active human player          *
 *                                                                                             *
 * INPUT:   Player pointer                                                                     *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   3/13/2019 5:00PM - ST                                                                     *
 *=============================================================================================*/
bool TechnoClass::Is_Owned_By_Player(HouseClass *player) const
{
	if (player == NULL) {
		return (House == PlayerPtr) ? true : false;
	}
	return (House == player) ? true : false;
}			  











#ifdef USE_RA_AI

 /*
 ** Additions for AI, from Red Alert. ST - 7/16/2019 11:36AM
 */


/***********************************************************************************************
 * TechnoClass::Anti_Air -- Determines the anti-aircraft strength of the object.               *
 *                                                                                             *
 *    This routine will calculate and return the anti-aircraft strength of this object.        *
 *    Typical users of this strength value is the base defense expert system AI.               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the anti-aircraft defense value of this object. The value returned    *
 *          is an abstract number to be used for relative comparisons only.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Anti_Air(void) const
{
	//assert(IsActive);

	if (Is_Weapon_Equipped()) {
		WeaponType weapon_type = Techno_Type_Class()->Primary;
		WeaponTypeClass const * weapon = &Weapons[weapon_type];
		//BulletTypeClass const * bullet = weapon->Bullet;
		const BulletTypeClass &bullet = BulletTypeClass::As_Reference(Weapons[weapon_type].Fires);
		//WarheadTypeClass const * warhead = weapon->WarheadPtr;
		WarheadTypeClass const * warhead = &Warheads[bullet.Warhead];


		//if (bullet->IsAntiAircraft) {
		if (weapon_type != WEAPON_NONE && BulletTypeClass::As_Reference(Weapons[weapon_type].Fires).IsAntiAircraft) {
			int value = ((weapon->Attack * warhead->Modifier[ARMOR_ALUMINUM]) * weapon->Range) / weapon->ROF;

			//if (Techno_Type_Class()->Is_Two_Shooter()) {
			//	value *= 2;
			//}
			return(value/50);
		}
	}
	return(0);
}


/***********************************************************************************************
 * TechnoClass::Anti_Armor -- Determines the anti-armor strength of the object.                *
 *                                                                                             *
 *    This routine is used to examine and calculate the anti-armor strength of this object.    *
 *    Typical user user of this would be the expert system base defense AI.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the relative anti-armor combat value for this object. The value       *
 *          is abstract and is only to be used in relative comparisons.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Anti_Armor(void) const
{
	//assert(IsActive);

	if (Is_Weapon_Equipped()) {

		WeaponType weapon_type = Techno_Type_Class()->Primary;
		//if (!Techno_Type_Class()->PrimaryWeapon->Bullet->IsAntiGround) return(0);
		if (weapon_type != WEAPON_NONE && BulletTypeClass::As_Reference(Weapons[weapon_type].Fires).IsAntiAircraft) {
			return 0;
		}

		//WeaponTypeClass const * weapon = Techno_Type_Class()->PrimaryWeapon;
		//BulletTypeClass const * bullet = weapon->Bullet;
		//WarheadTypeClass const * warhead = weapon->WarheadPtr;
		WeaponTypeClass const * weapon = &Weapons[weapon_type];
		const BulletTypeClass &bullet = BulletTypeClass::As_Reference(Weapons[weapon_type].Fires);
		WarheadTypeClass const * warhead = &Warheads[bullet.Warhead];

		int mrange = min(weapon->Range, 0x0400);

		int value = ((weapon->Attack * warhead->Modifier[ARMOR_STEEL]) * mrange * warhead->SpreadFactor) / weapon->ROF;
		//if (Techno_Type_Class()->Is_Two_Shooter()) {
		//	value *= 2;
		//}
		if (bullet.IsInaccurate) {
			value /= 2;
		}
		return(value/50);
	}
	return(0);
}


/***********************************************************************************************
 * TechnoClass::Anti_Infantry -- Calculates the anti-infantry strength of this object.         *
 *                                                                                             *
 *    This routine is used to determine the anti-infantry strength of this object. The         *
 *    typical user of this routine is the expert system base defense AI.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the anti-infantry strength of this object. The value returned is      *
 *          abstract and should only be used for relative comparisons.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Anti_Infantry(void) const
{
	//assert(IsActive);

	if (Is_Weapon_Equipped()) {
		//if (!Techno_Type_Class()->PrimaryWeapon->Bullet->IsAntiGround) return(0);
		WeaponType weapon_type = Techno_Type_Class()->Primary;
		if (weapon_type != WEAPON_NONE && BulletTypeClass::As_Reference(Weapons[weapon_type].Fires).IsAntiAircraft) {
			return 0;
		}

		//WeaponTypeClass const * weapon = Techno_Type_Class()->PrimaryWeapon;
		//BulletTypeClass const * bullet = weapon->Bullet;
		//WarheadTypeClass const * warhead = weapon->WarheadPtr;
		WeaponTypeClass const * weapon = &Weapons[weapon_type];
		const BulletTypeClass &bullet = BulletTypeClass::As_Reference(Weapons[weapon_type].Fires);
		WarheadTypeClass const * warhead = &Warheads[bullet.Warhead];
		int mrange = min(weapon->Range, 0x0400);

		int value = ((weapon->Attack * warhead->Modifier[ARMOR_NONE]) * mrange * warhead->SpreadFactor) / weapon->ROF;
		//if (Techno_Type_Class()->Is_Two_Shooter()) {
		//	value *= 2;
		//}
		if (bullet.IsInaccurate) {
			value /= 2;
		}
		return(value/50);
	}
	return(0);
}



int TechnoTypeClass::Legal_Placement(CELL pos) const
{
	if (pos == -1) return(0);

	/*
	**	Normal buildings must check to see that every foundation square is free of
	**	obstacles. If this check passes for all foundation squares, only then does the
	**	routine return that it is legal to place.
	*/
	short const * offset = Occupy_List(true);
	bool build = (What_Am_I() == RTTI_BUILDINGTYPE);

	if (build) {
		return ((BuildingTypeClass*)this)->Legal_Placement(pos);
	}

	while (offset != NULL && *offset != REFRESH_EOL) {
		CELL cell = pos + *offset++;
		if (!Map.In_Radar(cell)) return(false);
		//if (build) {
		//	if (!Map[cell].Is_Clear_To_Build(Speed)) {
		//		return(0);
		//	}
		//} else {
			//if (!Map[cell].Is_Clear_To_Move(Speed, false, false)) {
			if (!Map[cell].Is_Clear_To_Move(false, false)) {
				return(0);
			}
		//}
	}
	return(1);
}
#endif //USE_RA_AI