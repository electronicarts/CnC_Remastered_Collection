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

/* $Header: /CounterStrike/TECHNO.CPP 5     3/17/97 1:28a Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
 *                  Last Update : November 1, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TechnoClass::AI -- Handles AI processing for techno object.                               *
 *   TechnoClass::Anti_Air -- Determines the anti-aircraft strength of the object.             *
 *   TechnoClass::Anti_Armor -- Determines the anti-armor strength of the object.              *
 *   TechnoClass::Anti_Infantry -- Calculates the anti-infantry strength of this object.       *
 *   TechnoClass::Area_Modify -- Determine the area scan modifier for the cell.                *
 *   TechnoClass::Assign_Destination -- Assigns movement destination to the object.            *
 *   TechnoClass::Assign_Target -- Assigns the targeting computer with specified target.       *
 *   TechnoClass::Base_Is_Attacked -- Handle panic response to base being attacked.            *
 *   TechnoClass::Can_Fire -- Determines if this techno object can fire.                       *
 *   TechnoClass::Can_Player_Fire -- Determines if the player can give this object a fire order*
 *   TechnoClass::Can_Player_Move -- Determines if the object can move be moved by player.     *
 *   TechnoClass::Can_Repair -- Determines if the object can and should be repaired.           *
 *   TechnoClass::Can_Teleport_Here -- Checks cell to see if a valid teleport destination.     *
 *   TechnoClass::Captured -- Handles capturing this object.                                   *
 *   TechnoClass::Clicked_As_Target -- Sets the flash count for this techno object.            *
 *   TechnoClass::Cloaking_AI -- Perform the AI maintenance for a cloaking device.             *
 *   TechnoClass::Combat_Damage -- Fetch the amount of damage infliced by the specified weapon.*
 *   TechnoClass::Crew_Type -- Fetches the kind of crew this object contains.                  *
 *   TechnoClass::Debug_Dump -- Displays the base class data to the monochrome screen.         *
 *   TechnoClass::Desired_Load_Dir -- Fetches loading parameters for this object.              *
 *   TechnoClass::Detach -- Handles removal of target from tracking system.                    *
 *   TechnoClass::Do_Cloak -- Start the object into cloaking stage.                            *
 *   TechnoClass::Do_Shimmer -- Causes this object to shimmer if it is cloaked.                *
 *   TechnoClass::Do_Uncloak -- Cause the stealth tank to uncloak.                             *
 *   TechnoClass::Draw_It -- Draws the health bar (if necessary).                              *
 *   TechnoClass::Draw_Pips -- Draws the transport pips and other techno graphics.             *
 *   TechnoClass::Electric_Zap -- Fires electric zap at the target specified.                  *
 *   TechnoClass::Enter_Idle_Mode -- Object enters its default idle condition.                 *
 *   TechnoClass::Evaluate_Cell -- Determine the value and object of specified cell.           *
 *   TechnoClass::Evaluate_Just_Cell -- Evaluate a cell as a target by itself.                 *
 *   TechnoClass::Evaluate_Object -- Determines score value of specified object.               *
 *   TechnoClass::Exit_Object -- Causes specified object to leave this object.                 *
 *   TechnoClass::Find_Docking_Bay -- Searches for a close docking bay.                        *
 *   TechnoClass::Find_Exit_Cell -- Finds an appropriate exit cell for this object.            *
 *   TechnoClass::Fire_At -- Fires projectile at target specified.                             *
 *   TechnoClass::Fire_Coord -- Determine the coordinate where bullets appear.                 *
 *   TechnoClass::Fire_Direction -- Fetches the direction projectile fire will take.           *
 *   TechnoClass::Get_Ownable -- Fetches the ownable bits for this object.                     *
 *   TechnoClass::Greatest_Threat -- Determines best target given search criteria.             *
 *   TechnoClass::Hidden -- Returns the object back into the hidden state.                     *
 *   TechnoClass::How_Many_Survivors -- Determine the number of survivors to escape.           *
 *   TechnoClass::In_Range -- Determines if specified target is within weapon range.           *
 *   TechnoClass::In_Range -- Determines if specified target is within weapon range.           *
 *   TechnoClass::In_Range -- Determines if the specified coordinate is within range.          *
 *   TechnoClass::Is_Allowed_To_Recloak -- Can this object recloak?                            *
 *   TechnoClass::Is_Allowed_To_Retaliate -- Checks object to see if it can retaliate.         *
 *   TechnoClass::Is_In_Same_Zone -- Determine if specified cell is in same zone as object.    *
 *   TechnoClass::Is_Players_Army -- Determines if this object is part of the player's army.   *
 *   TechnoClass::Is_Ready_To_Cloak -- Determines if this object is ready to begin cloaking.   *
 *   TechnoClass::Is_Ready_To_Random_Animate -- Determines if the object should random animate.*
 *   TechnoClass::Is_Visible_On_Radar -- Is this object visible on player's radar screen?      *
 *   TechnoClass::Is_Weapon_Equipped -- Determines if this object has a combat weapon.         *
 *   TechnoClass::Kill_Cargo -- Destroys any cargo attached to this object.                    *
 *   TechnoClass::Look -- Performs a look around (map reveal) action.                          *
 *   TechnoClass::Mark -- Handles marking of techno objects.                                   *
 *   TechnoClass::Nearby_Location -- Radiates outward looking for clear cell nearby.           *
 *   TechnoClass::Owner -- Who is the owner of this object?                                    *
 *   TechnoClass::Per_Cell_Process -- Handles once-per-cell operations for techno type objects.*
 *   TechnoClass::Pip_Count -- Fetches the number of pips to display on this object.           *
 *   TechnoClass::Player_Assign_Mission -- Assigns a mission as result of player input.        *
 *   TechnoClass::Rearm_Delay -- Calculates the delay before firing can occur.                 *
 *   TechnoClass::Receive_Message -- Handles inbound message as appropriate.                   *
 *   TechnoClass::Record_The_Kill -- Records the death of this object.                         *
 *   TechnoClass::Refund_Amount -- Returns with the money to refund if this object is sold.    *
 *   TechnoClass::Remap_Table -- Fetches the appropriate remap table to use.                   *
 *   TechnoClass::Renovate -- Heal a building to maximum                                       *
 *   TechnoClass::Response_Attack -- Handles the voice response when given attack order.       *
 *   TechnoClass::Response_Move -- Handles the voice response to a movement request.           *
 *   TechnoClass::Response_Select -- Handles the voice response when selected.                 *
 *   TechnoClass::Revealed -- Handles revealing an object to the house specified.              *
 *   TechnoClass::Risk -- Fetches the risk associated with this object.                        *
 *   TechnoClass::Select -- Selects object and checks to see if can be selected.               *
 *   TechnoClass::Set_Mission -- Forced mission set (used by editor).                          *
 *   TechnoClass::Stun -- Prepares the object for removal from the game.                       *
 *   TechnoClass::Take_Damage -- Records damage assessed to this object.                       *
 *   TechnoClass::Target_Something_Nearby -- Handles finding and assigning a nearby target.    *
 *   TechnoClass::TechnoClass -- Constructor for techno type objects.                          *
 *   TechnoClass::Techno_Draw_Object -- General purpose draw object routine.                   *
 *   TechnoClass::Threat_Range -- Returns the range to scan based on threat control.           *
 *   TechnoClass::Tiberium_Load -- Fetches the current tiberium load percentage.               *
 *   TechnoClass::Time_To_Build -- Determines the time it would take to build this.            *
 *   TechnoClass::Unlimbo -- Performs unlimbo process for all techno type objects.             *
 *   TechnoClass::Value -- Fetches the target value for this object.                           *
 *   TechnoClass::Visual_Character -- Determine the visual character of the object.            *
 *   TechnoClass::Weapon_Range -- Determines the maximum range for the weapon.                 *
 *   TechnoClass::What_Action -- Determines action to perform if cell is clicked on.           *
 *   TechnoClass::What_Action -- Determines what action to perform if object is selected.      *
 *   TechnoClass::What_Weapon_Should_I_Use -- Determines what is the best weapon to use.       *
 *   TechnoTypeClass::Cost_Of -- Fetches the cost of this object type.                         *
 *   TechnoTypeClass::Get_Cameo_Data -- Fetches the cameo image for this object type.          *
 *   TechnoTypeClass::Get_Ownable -- Fetches the ownable bits for this object type.            *
 *   TechnoTypeClass::Is_Two_Shooter -- Determines if this object is a double shooter.         *
 *   TechnoTypeClass::Raw_Cost -- Fetches the raw (base) cost of the object.                   *
 *   TechnoTypeClass::Read_INI -- Reads the techno type data from the INI database.            *
 *   TechnoTypeClass::Repair_Cost -- Fetches the cost to repair one step.                      *
 *   TechnoTypeClass::Repair_Step -- Fetches the health to repair one step.                    *
 *   TechnoTypeClass::TechnoTypeClass -- Constructor for techno type objects.                  *
 *   TechnoTypeClass::Time_To_Build -- Fetches the time to build this object.                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***************************************************************************
**	Cloaking control values.
*/
#define	MAX_UNCLOAK_STAGE		38

//Added for getting the input for special character keys from the client 
// - 6/26/2019 JAS 
extern bool DLL_Export_Get_Input_Key_State(KeyNumType key);


/***************************************************************************
**	These are the pointers to the special shape data that the units may need.
*/
void const * TechnoTypeClass::WakeShapes = 0;
void const * TechnoTypeClass::TurretShapes = 0;
void const * TechnoTypeClass::SamShapes = 0;
void const * TechnoTypeClass::MGunShapes = 0;

//Xlat Tables for French and German
// For name overriding
//#define NEWNAMES  22
#ifdef GERMAN
const char* NewName[] = {
	"Scout Ant",	  			"Kundschafter-Ameise",
	"Warrior Ant",    		"Krieger-Ameise",
	"Fire Ant",	  				"Feuer-Ameise",
	"Queen Ant",	  			"Ameisenk”nigin",
	"ATS",						"Angriffs-U-Boot",
	"Tesla Tank",	  			"Telsapanzer",
	"Volkov",  					"Modell Volkov",
	"Chitzkoi", 				"Roboterhund",
	"Stavros",					"Stavros",
	"F-A Longbow",				"Jagdhubschrauber Longbow",
	"Civilian Specialist",	"Wissenschaftler",
	"Alloy Facility",			"Legierungswerk",
	NULL,
	};

#endif
#ifdef FRENCH
const char* NewName[] = {
	"Scout Ant",	  			"Fourmi de Reconnaissance",
	"Warrior Ant",    		"Fourmi GuerriŠre",
	"Fire Ant",	  				"Fourmi Lance-Flammes",
	"Queen Ant",	  			"Reine des Fourmis",
	"ATS", 						"SMTA",
	"Tesla Tank",	  			"Tank Tesla",
	"Volkov",  					"Volkov",
	"Chitzkoi", 				"Cyber-Chien",
	"Stavros",					"Stavros",
	"F-A Longbow",				"HAA (H‚licoptŠre d'Assaut Avanc‚)",
	"Civilian Specialist",	"Sp‚cialiste Civil",
	"Alloy Facility",			"Usine M‚tallurgique",
	NULL,
	};

#endif


/***************************************************************************
**	Which shape to use depending on which facing is controlled by these arrays.
*/
int const TechnoClass::BodyShape[32] = {0,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};


/***********************************************************************************************
 * TechnoClass::Is_Players_Army -- Determines if this object is part of the player's army.     *
 *                                                                                             *
 *    The player's army is considered to be all those mobile units that can be selected        *
 *    and controlled by the player (they may or may not have weapons in the traditional        *
 *    sense).                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this object part of the player's selectable controllable army?            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Players_Army(void) const
{
	/*
	**	An object that is dead (or about to be) is not considered part of
	**	the player's army.
	*/
	if (Strength <= 0) {
		return(false);
	}

	/*
	**	If the object is not yet on the map or is otherwise indisposed, then
	**	don't consider it.
	*/
	if (IsInLimbo || !IsLocked) {
		return(false);
	}

	/*
	**	Buildings, although sometimes serving a combat purpose, are not part
	**	of the player's army.
	*/
	if (What_Am_I() == RTTI_BUILDING) {
		return(false);
	}

	/*
	**	If not discoverd by the player, then don't consider it part of the
	**	player's army (yet).
	*/
	if (!Is_Discovered_By_Player()) {
		return(false);
	}

	/*
	**	If not selectable, then not really part of the player's active army.
	*/
	if (!Techno_Type_Class()->IsSelectable) {
		return(false);
	}

	/*
	**	If not under player control, then it isn't part of the player's army.
	*/
	if (!House->IsPlayerControl) {
		return(false);
	}

	return(true);
}


/***********************************************************************************************
 * TechnoClass::Can_Teleport_Here -- Checks cell to see if a valid teleport destination.       *
 *                                                                                             *
 *    Use this routine to examine the cell specified and if the cell could be a valid          *
 *    destination for a teleport, then return true.                                            *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine as a possible legal teleport destination.             *
 *                                                                                             *
 * OUTPUT:  bool; Is the specified cell a legal teleport destination?                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Can_Teleport_Here(CELL cell) const
{
	/*
	**	Infantry are never allowed to teleport.
	*/
	if (What_Am_I() == RTTI_INFANTRY) {
		return(false);
	}

	/*
	**	The destination cell must be on the playfield.
	*/
	if (!Map.In_Radar(cell)) {
		return(false);
	}

	/*
	**	Teleporting directly onto the flag spot is not allowed. This is due to the
	**	requirement that entering that location must proceed under normal channels.
	*/
	if (Map[cell].Overlay == OVERLAY_FLAG_SPOT) {
		return(false);
	}

	/*
	**	Determine if the object could enter the cell by normal means. If the
	**	cell is impassable, then it can't be teleported there.
	*/
	TechnoTypeClass const * ttype = Techno_Type_Class();
	if (!Map[cell].Is_Clear_To_Move(ttype->Speed, true, true, -1, ttype->Speed == SPEED_FLOAT ? MZONE_WATER : MZONE_NORMAL)) {
		return(false);
	}

	return(true);
}


/***********************************************************************************************
 * TechnoClass::What_Weapon_Should_I_Use -- Determines what is the best weapon to use.         *
 *                                                                                             *
 *    This routine will compare the weapons this object is equipped with verses the            *
 *    candidate target object. The best weapon to use against the target will be returned.     *
 *    Special emphasis is given to weapons that can fire on the target without requiring       *
 *    this object to move within range.                                                        *
 *                                                                                             *
 * INPUT:   target   -- The candidate target to determine which weapon is best against.        *
 *                                                                                             *
 * OUTPUT:  Returns with an identifier the specifies what weapon to use against the target.    *
 *          The return value will be "0" for the primary weapon and "1" for the secondary.     *
 *                                                                                             *
 * WARNINGS:   This routine is called very frequently. It should be as efficient as possible.  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::What_Weapon_Should_I_Use(TARGET target) const
{
	if (!Target_Legal(target)) return(0);

	/*
	**	Fetch the armor of the candidate target object. Presume that if the target
	**	is not an object, then its armor is equivalent to wood. Who knows why?
	*/
	ArmorType armor = ARMOR_WOOD;
	if (Is_Target_Object(target)) {
		armor = As_Object(target)->Class_Of().Armor;
	}

	TechnoTypeClass const * ttype = Techno_Type_Class();

	/*
	**	Get the value of the primary weapon verses the candidate target. Increase the
	**	value of the weapon if it happens to be in range.
	*/
	int w1 = 0;
	WeaponTypeClass const * wptr = ttype->PrimaryWeapon;
	if (wptr != NULL && wptr->WarheadPtr != NULL) w1 = wptr->WarheadPtr->Modifier[armor] * 1000;
	if (In_Range(target, 0)) w1 *= 2;
	FireErrorType ok = Can_Fire(target, 0);
	if (ok == FIRE_CANT || ok == FIRE_ILLEGAL) w1 = 0;

	/*
	**	Calculate a similar value for the secondary weapon.
	*/
	int w2 = 0;
	wptr = ttype->SecondaryWeapon;
	if (wptr != NULL && wptr->WarheadPtr != NULL) w2 = wptr->WarheadPtr->Modifier[armor] * 1000;
	if (In_Range(target, 1)) w2 *= 2;
	ok = Can_Fire(target, 1);
	if (ok == FIRE_CANT || ok == FIRE_ILLEGAL) w2 = 0;

	/*
	**	Return with the weapon identifier that should be used to fire upon the
	**	candidate target.
	*/
	if (w2 > w1) return(1);
	return(0);
}


/***********************************************************************************************
 * TechnoClass::How_Many_Survivors -- Determine the number of survivors to escape.             *
 *                                                                                             *
 *    This routine will determine the number of survivors that should run from this object     *
 *    when it is destroyed.                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the maximum number of survivors that should run from this object      *
 *          when the object gets destroyed.                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::How_Many_Survivors(void) const
{
	if (Techno_Type_Class()->IsCrew) {
		return(1);
	}
	return(0);
}


/***********************************************************************************************
 * TechnoClass::Combat_Damage -- Fetch the amount of damage infliced by the specified weapon.  *
 *                                                                                             *
 *    This routine will examine the specified weapon of this object and determine how much     *
 *    damage it could inflict -- best case.                                                    *
 *                                                                                             *
 * INPUT:   which -- Which weapon to consider. If -1 is specified, then the average of both    *
 *                   weapon types (if present) is returned.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the combat damage that could be inflicted by the specified weapon.    *
 *                                                                                             *
 * WARNINGS:   This routine could return a negative number if a medic is scanned.              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Combat_Damage(int which) const
{
	TechnoTypeClass const * ttype = Techno_Type_Class();

	int divisor = 0;
	int value = 0;

	if (which == 0 || which == -1) {
		if (ttype->PrimaryWeapon != NULL) {
			value += ttype->PrimaryWeapon->Attack;
			divisor += 1;
		}
	}

	if (which == 1 || which == -1) {
		if (ttype->SecondaryWeapon != NULL) {
			value += ttype->SecondaryWeapon->Attack;
			divisor += 1;
		}
	}

	if (divisor > 1) {
		return(value / divisor);
	}
	return(value);
}


/***********************************************************************************************
 * TechnoClass::Is_Allowed_To_Recloak -- Can this object recloak?                              *
 *                                                                                             *
 *    Determine is this object can recloak now and returns that info. Usually the answer is    *
 *    yes, but it can be overridden be derived classes.                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can this object recloak now? (presumes it has the ability to cloak)          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Allowed_To_Recloak(void) const
{
	return(true);
}


/***********************************************************************************************
 * TechnoClass::Fire_Coord -- Determine the coordinate where bullets appear.                   *
 *                                                                                             *
 *    This routine will determine the coordinate to use when this object fires. The coordinate *
 *    is the location where bullets appear (or fire effects appear) when the object fires      *
 *    its weapon.                                                                              *
 *                                                                                             *
 * INPUT:   which -- Which weapon is the coordinate to be calculated for? 0 means primary      *
 *                   weapon, 1 means secondary weapon.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate that any bullets fired from the specified weapon       *
 *          should appear.                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE TechnoClass::Fire_Coord(int which) const
{
	assert(IsActive);

	DirType dir = Turret_Facing();
	TechnoTypeClass const * tclass = Techno_Type_Class();

	int dist = 0;
	int lateral = 0;
	if (which == 0) {
		dist = tclass->PrimaryOffset;
		lateral = tclass->PrimaryLateral;
	} else {
		dist = tclass->SecondaryOffset;
		lateral = tclass->SecondaryLateral;
	}

	COORDINATE coord = Coord_Move(Center_Coord(), DIR_N, tclass->VerticalOffset + Height);
	coord = Coord_Move(coord, DIR_E, tclass->HorizontalOffset);
	if (IsSecondShot) {
		coord = Coord_Move(coord, (DirType)(dir+DIR_E), lateral);
	} else {
		coord = Coord_Move(coord, (DirType)(dir+DIR_W), lateral);
	}
	coord = Coord_Move(coord, dir, dist);

	return(coord);
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
void TechnoClass::Debug_Dump(MonoClass * mono) const
{
	assert(IsActive);
	mono->Set_Cursor(10, 7);mono->Printf("%2d", Fetch_Rate());
	mono->Set_Cursor(14, 7);mono->Printf("%2d", Fetch_Stage());
	mono->Set_Cursor(49, 1);mono->Printf("%3d", Ammo);
	mono->Set_Cursor(71, 1);mono->Printf("$%4d", PurchasePrice);
	mono->Set_Cursor(54, 1);mono->Printf("%3d", (long)Arm);
	if (Is_Something_Attached()) {
		mono->Set_Cursor(1, 5);mono->Printf("%08X", Attached_Object());
	}
	if (Target_Legal(TarCom)) {
		mono->Set_Cursor(29, 3);mono->Printf("%08X", TarCom);
	}
	if (Target_Legal(SuspendedTarCom)) {
		mono->Set_Cursor(38, 3);mono->Printf("%08X", SuspendedTarCom);
	}
	if (Target_Legal(ArchiveTarget)) {
		mono->Set_Cursor(69, 5);mono->Printf("%08X", ArchiveTarget);
	}
	mono->Set_Cursor(47, 3);mono->Printf("%02X:%02X", PrimaryFacing.Current(), PrimaryFacing.Desired());
	mono->Set_Cursor(64, 1);mono->Printf("%d(%d)", Cloak, CloakingDevice);

	mono->Fill_Attrib(14, 15, 12, 1, IsUseless ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 16, 12, 1, IsTickedOff ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 17, 12, 1, IsCloakable ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 13, 12, 1, IsLeader ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 14, 12, 1, IsALoaner ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 15, 12, 1, IsLocked ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 16, 12, 1, IsInRecoilState ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 17, 12, 1, IsTethered ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 13, 12, 1, IsOwnedByPlayer ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 14, 12, 1, IsDiscoveredByPlayer ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 15, 12, 1, IsDiscoveredByComputer ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 16, 12, 1, IsALemon ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Set_Cursor(47, 17);mono->Printf("%3d", (long)IronCurtainCountDown);
	mono->Fill_Attrib(40, 17, 12, 1, IronCurtainCountDown > 0 ? MonoClass::INVERSE : MonoClass::NORMAL);

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
TechnoClass::TechnoClass(RTTIType rtti, int id, HousesType house) :
	RadioClass(rtti, id),
	IsUseless(false),
	IsTickedOff(false),
	IsCloakable(false),
	IsLeader(false),
	IsALoaner(false),
	IsLocked(false),
	IsInRecoilState(false),
	IsTethered(false),
	IsOwnedByPlayer(false),
	IsDiscoveredByPlayer(false),
	IsDiscoveredByComputer(false),
	IsALemon(false),
	IsSecondShot(true),
	ArmorBias(1),
	FirepowerBias(1),
	IdleTimer(0),
	IronCurtainCountDown(0),
	SpiedBy(0),
	ArchiveTarget(TARGET_NONE),
	House(HouseClass::As_Pointer(house)),
	Cloak(UNCLOAKED),
	TarCom(TARGET_NONE),
	SuspendedTarCom(TARGET_NONE),
	PrimaryFacing(DIR_N),
	Arm(0),
	Ammo(-1),
	ElectricZapDelay(-1),
	ElectricZapTarget(0),
	ElectricZapWhich(0),
	PurchasePrice(0)
{
	//IsOwnedByPlayer = (PlayerPtr == House);
	// Added for multiplayer changes. ST - 4/24/2019 10:40AM
	IsDiscoveredByPlayerMask = 0;
	if (Session.Type == GAME_NORMAL) {
		IsOwnedByPlayer = (PlayerPtr == House);
	} else {
		IsOwnedByPlayer = House->IsHuman;
	}
}


/***********************************************************************************************
 * TechnoClass::Time_To_Build -- Determines the time it would take to build this.              *
 *                                                                                             *
 *    Use this routine to determine the amount of time it would take to build an object of     *
 *    this type.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the time it should take (unmodified by outside factors) to build      *
 *          this object. The time is expressed in game frames.                                 *
 *                                                                                             *
 * WARNINGS:   The time will usually be modified by power status and handicap rating for the   *
 *             owning house. The value returned is merely the raw unmodified time to build.    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *   09/27/1996 JLB : Takes into account the power availability.                               *
 *=============================================================================================*/
//#define UNIT_BUILD_BIAS fixed(5,4)
//#define UNIT_BUILD_BIAS fixed(6,4)
#define UNIT_BUILD_BIAS fixed(1,1)
//#define UNIT_BUILD_BIAS fixed(5,1)

extern int UnitBuildPenalty;

int TechnoClass::Time_To_Build(void) const
{
	int val = Class_Of().Time_To_Build();

#ifdef FIXIT_VERSION_3
	if (Session.Type == GAME_NORMAL ) {
#else
	if (Session.Type == GAME_NORMAL ||
					PlayingAgainstVersion == VERSION_RED_ALERT_104 ||
		 			PlayingAgainstVersion == VERSION_RED_ALERT_107){
#endif
		val *= House->BuildSpeedBias;
	}else{
		if (What_Am_I() == RTTI_BUILDING || What_Am_I() == RTTI_INFANTRY) {
			val *= House->BuildSpeedBias;
		} else {
			val *= House->BuildSpeedBias * fixed (UnitBuildPenalty, 100);	//UNIT_BUILD_BIAS;
		}
	}


	/*
	**	Adjust the time to build based on the power output of the owning house.
	*/
	fixed power = House->Power_Fraction();
	if (power > 1) power = 1;
	if (power < 1 && power > fixed::_3_4) power = fixed::_3_4;
	if (power < fixed::_1_2) power = fixed::_1_2;
	power.Inverse();
	val *= power;

	int divisor = House->Factory_Count(What_Am_I());
	if (divisor != 0) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
// Hack: allow the multiple-factory bonus, but only up to two factories if
//			this is an AM<->AM game.
		if(NewUnitsEnabled) {
			val /= min(divisor,2);
		} else {
			val /= divisor;
		}
#else
		val /= divisor;
#endif
	}
	return(val);
}


/***********************************************************************************************
 * TechnoClass::Is_Visible_On_Radar -- Is this object visible on player's radar screen?        *
 *                                                                                             *
 *    Use this routine to determine if this object should be visible on the player's radar     *
 *    screen. This routine doesn't take into consideration mapped terrain or whether the       *
 *    radar is active. It merely checks to see that if all else is functioning correctly,      *
 *    is this unit invisible or visible on the radar map. Typically, cloaked vehicles will     *
 *    not be visible.                                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this object nominally visible on the player's radar screen?               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Visible_On_Radar(void) const
{
	/*
	** Hack: MRJ is invisible to radar, unless it's allied with the player.
	*/
	if (What_Am_I() == RTTI_UNIT) {
		if(*((UnitClass *)this) == UNIT_MRJ) {
			if(!House->Is_Ally(PlayerPtr)) {
				return(false);
			}
		}
	}
	if (!Techno_Type_Class()->IsInvisible && (Cloak != CLOAKED || House->Is_Ally(PlayerPtr))) {
		return(true);
	}
	return(false);
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
 *=============================================================================================*/
bool TechnoClass::Revealed(HouseClass * house)
{
	assert(IsActive);

	/*   Replaced by client/server version from TD. ST - 8/7/2019 11:09AM
	if (house == PlayerPtr && IsDiscoveredByPlayer) return(false);
	if (house != PlayerPtr) {
		if (IsDiscoveredByComputer) return(false);
		IsDiscoveredByComputer = true;
	}
	*/
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
		IsDiscoveredByComputer = true;
	}

	if (RadioClass::Revealed(house)) {

		/*
		**	An enemy object that is discovered will go into hunt mode if
		**	its current mission is to ambush.
		*/
		if (!house->IsHuman && Mission == MISSION_AMBUSH) {
			Assign_Mission(MISSION_HUNT);
		}

		if (Session.Type == GAME_NORMAL) {
			if (house == PlayerPtr) {
				IsDiscoveredByPlayer = true;

				if (!IsOwnedByPlayer) {

					/*
					**	If there is a trigger event associated with this object, then process
					**	it for discovery purposes.
					*/
					if (!ScenarioInit && Trigger.Is_Valid()) {
						Trigger->Spring(TEVENT_DISCOVERED, this);
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

					/*
					**	A newly revealed object will always perform a look operation.
					*/
					Look();
				}
			} else {
				IsDiscoveredByComputer = true;
			}
		
		} else {
			
			if (house->IsHuman) {
				Set_Discovered_By_Player(house);
			}

			Look();
		}
			
		return(true);
	}
	return(false);
}


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
	assert(IsActive);

	//if (!IsDiscoveredByPlayer) return;
	if (!Is_Discovered_By_Player()) {	 // ST - 8/7/2019 11:17AM	
		return;
	}
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
	assert(IsActive);

	if (RadioClass::Mark(mark)) {

		/*
		**	When redrawing an object, if there is another object tethered to this one,
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
	assert(IsActive);

	switch (message) {

		/*
		**	Just received instructions to attack the specified target.
		*/
		case RADIO_ATTACK_THIS:
			if (Techno_Type_Class()->PrimaryWeapon != NULL) {
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
		**	A tethered unit has reached it's destination. All is
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
/*BG*/	Mark(MARK_CHANGE_REDRAW);
			Ammo++;
			return(RADIO_ROGER);

		/*
		**	Handle repair of this unit.
		*/
		case RADIO_REPAIR:
			/*
			** If it's a mine layer, re-arm him if he's empty. This always takes precedence
			**	over repair, since this operation is free.
			*/
			if (What_Am_I() == RTTI_UNIT && *((UnitClass *)this) == UNIT_MINELAYER && ((UnitClass *)this)->Ammo < ((UnitClass *)this)->Class->MaxAmmo) {
				((UnitClass *)this)->Ammo = ((UnitClass *)this)->Class->MaxAmmo;
				return(RADIO_NEGATIVE);
			}

			/*
			**	Determine if this unit can be repaired becaause it is under strength. If so, then
			**	proceed with the repair process.
			*/
			if (Health_Ratio() < Rule.ConditionGreen) {
				int cost = Techno_Type_Class()->Repair_Cost();
				cost = max(cost, 1);
				int step = Techno_Type_Class()->Repair_Step();
				step = max(step, 1);

				/*
				**	If there is sufficient money to repair the unit one step, then do so.
				**	Otherwise return with a "can't complete" radio response.
				*/
				if (House->Available_Money() >= cost) {
					House->Spend_Money(cost);
					Strength += step;

					/*
					**	Return with either an all ok or mission accomplished radio message. This
					**	lets the repairing object know if it should abort the repair control process
					**	or continue it.
					*/
					if (Health_Ratio() < Rule.ConditionGreen) {
						return(RADIO_ROGER);
					} else {
						Strength = Techno_Type_Class()->MaxStrength;
						return(RADIO_ALL_DONE);
					}
				} else {
					return(RADIO_CANT);
				}
			}
			return(RADIO_NEGATIVE);

		default:
			break;
	}
	return(RadioClass::Receive_Message(from, message, param));
}


/***********************************************************************************************
 * TechnoClass::Per_Cell_Process -- Handles once-per-cell operations for techno type objects.  *
 *                                                                                             *
 *    This routine handles marking a game object as not a loaner. It is set only if the unit   *
 *    is not player owned and is on the regular map. This is necessary so that enemy objects   *
 *    can exist off-map but as soon as they move onto the map, are flagged so that can never   *
 *    leave it again.                                                                          *
 *                                                                                             *
 * INPUT:   why   -- Specifies the circumstances under which this routine was called.          *
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
void TechnoClass::Per_Cell_Process(PCPType why)
{
	assert(IsActive);

	if (why == PCP_END) {
		CELL cell = Coord_Cell(Center_Coord());

		/*
		**	When enemy units enter the proper map area from off map, they are
		**	flagged so that they won't travel back off the map again.
		*/
		if (!IsLocked && Map.In_Radar(cell)) {
	  		IsLocked = true;
		}

		/*
		**	If this object somehow moves into mapped terrain, but is not yet
		**	discovered, then flag it to be discovered.
		*/
		// Change for client/server multiplayer ST - 8/7/2019 11:19AM
		//if (!IsDiscoveredByPlayer && Map[cell].IsVisible) {
			//Revealed(PlayerPtr);
		if (!Is_Discovered_By_Player() && Map[cell].Is_Visible(House)) {
			Revealed(House);
		}
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
void TechnoClass::Draw_It(int x, int y, WindowNumberType window) const
{
	assert(IsActive);

	/*
	**	Tells the door logic that it has been drawn.
	*/
	((TechnoClass *)this)->Clear_Redraw_Flag();

	/*
	** Draw electric zap
	*/
	if ((ElectricZapDelay >= 0) && ElectricZapTarget) {
		Electric_Zap(ElectricZapTarget, ElectricZapWhich, window);
	}

	int width, height;
	Class_Of().Dimensions(width, height);

	const bool show_health_bar = (Strength > 0) && (Is_Selected_By_Player() ||
		((Rule.HealthBarDisplayMode == RulesClass::HB_DAMAGED) && (Strength < Techno_Type_Class()->MaxStrength)) ||
		(Rule.HealthBarDisplayMode == RulesClass::HB_ALWAYS));

	/*
	**	Draw the selected object graphic.
	*/
	int lx = width / 2;
	int ly = height / 2;
	int dx = width / 5;
	int dy = height / 5;
	int fudge = show_health_bar ? 4 : 0;
	if (What_Am_I() == RTTI_VESSEL) {
		lx = width / 2;
	}

	//if (IsSelected) {
	if (Is_Selected_By_Player() || show_health_bar) {
		GraphicViewPortClass draw_window(	LogicPage->Get_Graphic_Buffer(),
														WindowList[window][WINDOWX] + LogicPage->Get_XPos(),
														WindowList[window][WINDOWY] + LogicPage->Get_YPos(),
														WindowList[window][WINDOWWIDTH],
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

		/*
		**	Fetch the dimensions of the object. These dimensions will be used to draw
		**	the selection box and the health bar.
		*/

		if (show_health_bar) {
			fixed	ratio = Health_Ratio();
			int	pwidth;		// Pixel width of bar interior.
			int	color;		// The color to give the interior of the bargraph.

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
			pwidth = (width-2) * ratio;

			pwidth = Bound(pwidth, 1, width-2);

			color = LTGREEN;
			if (ratio <= Rule.ConditionYellow) {
				color = YELLOW;
			}
			if (ratio <= Rule.ConditionRed) {
				color = RED;
			}
			draw_window.Fill_Rect(xx+1, yy+1, xx+pwidth, yy+(3-1), color);
		}

		/*
		**	Draw the selected object graphic.
		*/
		if (Is_Selected_By_Player()) {
			// Upper left corner.
			draw_window.Draw_Line(x - lx, fudge + y - ly, x - lx + dx, fudge + y - ly, WHITE);
			draw_window.Draw_Line(x - lx, fudge + y - ly, x - lx, fudge + y - ly + dy, WHITE);

			// Upper right corner.
			draw_window.Draw_Line(x + lx, fudge + y - ly, x + lx - dx, fudge + y - ly, WHITE);
			draw_window.Draw_Line(x + lx, fudge + y - ly, x + lx, fudge + y - ly + dy, WHITE);

			// Lower right corner.
			draw_window.Draw_Line(x + lx, y + ly, x + lx - dx, y + ly, WHITE);
			draw_window.Draw_Line(x + lx, y + ly, x + lx, y + ly - dy, WHITE);

			// Lower left corner.
			draw_window.Draw_Line(x - lx, y + ly, x - lx + dx, y + ly, WHITE);
			draw_window.Draw_Line(x - lx, y + ly, x - lx, y + ly - dy, WHITE);
		}
	}

	// MBL 04.21.2020 
	bool selected = Is_Selected_By_Player() || Rule.ResourceBarDisplayMode == RulesClass::RB_ALWAYS;
	// if ((window == WINDOW_VIRTUAL) || (Is_Selected_By_Player() && (House->Is_Ally(PlayerPtr) || (Spied_By() & (1 << (PlayerPtr->Class->House)))))) 
	if ((window == WINDOW_VIRTUAL) || (selected && (House->Is_Ally(PlayerPtr) || (Spied_By() & (1 << (PlayerPtr->Class->House)))))) 
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
	assert(IsActive);

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
	assert(IsActive);

	if (IsLocked && Target_Legal(target)) {
		int range = Weapon_Range(which);
		BuildingClass const * building = As_Building(target);
		if (building != NULL) {
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
	assert(IsActive);

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
	assert(IsActive);

	return(IsLocked && ::Distance(Fire_Coord(which), coord) <= Weapon_Range(which));
}


/***********************************************************************************************
 * TechnoClass::Area_Modify -- Determine the area scan modifier for the cell.                  *
 *                                                                                             *
 *    This routine scans around the cell specified and if there are any friendly buildings     *
 *    nearby, the multiplier return value will be reduced. If there are no friendly buildings  *
 *    nearby, then the return value will be 1. It checks to see if the primary weapon is       *
 *    supposed to perform this scan and if so, the scan will be performed. Otherwise the       *
 *    default value is quickly returned.                                                       *
 *                                                                                             *
 * INPUT:   cell  -- The cell where the potential target lies. An area around this cell will   *
 *                   be scanned for friendly buildings.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the multiplier to be multiplied by the potential target score value.  *
 *          For less opportune targets, the multiplier fraction will be less than one. For     *
 *          all other cases, it will return the default value of 1.                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
fixed TechnoClass::Area_Modify(CELL cell) const
{
//	assert(Techno_Type_Class()->PrimaryWeapon != NULL);
	if (Techno_Type_Class()->PrimaryWeapon == NULL || !Techno_Type_Class()->PrimaryWeapon->IsSupressed) return(1);

	int crange = Lepton_To_Cell(Rule.SupressRadius);
	fixed odds = 1;

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
				BuildingClass const * building = Map[newcell].Cell_Building();
				if (building != NULL && House->Is_Ally(building)) {
					odds /= 2;
				}
			}

			if ((Cell_Y(cell) + radius) < (Map.MapCellY+Map.MapCellHeight)) {
				newcell = XY_Cell(Cell_X(cell)+x, Cell_Y(cell)+radius);
				BuildingClass const * building = Map[newcell].Cell_Building();
				if (building != NULL && House->Is_Ally(building)) {
					odds /= 2;
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
				BuildingClass const * building = Map[newcell].Cell_Building();
				if (building != NULL && House->Is_Ally(building)) {
					odds /= 2;
				}
			}

			if ((Cell_X(cell) + radius) < (Map.MapCellX+Map.MapCellWidth)) {
				newcell = XY_Cell(Cell_X(cell)+radius, Cell_Y(cell)+y);
				BuildingClass const * building = Map[newcell].Cell_Building();
				if (building != NULL && House->Is_Ally(building)) {
					odds /= 2;
				}
			}
		}
	}
	return(odds);
}


/***********************************************************************************************
 * TechnoClass::Evaluate_Object -- Determines score value of specified object.                 *
 *                                                                                             *
 *    This routine is used to determine the score value (value as a potential target) of the   *
 *    object specified. This routine will check the specified object for all the various       *
 *    legality checks that threat scanning requires. This is the main workhorse routine for    *
 *    target searching.                                                                        *
 *                                                                                             *
 * INPUT:   method   -- The threat method requested. This is a combined bitflag value that     *
 *                      not only specifies the kind of targets to consider, but how far away   *
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
 *          zone     -- The zone restriction if any. A -1 means no zone check required.        *
 *                                                                                             *
 * OUTPUT:  Did the target pass all legality checks? If this value is returned true, then the  *
 *          value parameter will be filled in correctly.                                       *
 *                                                                                             *
 * WARNINGS:   This routine is time consuming. Don't call unless necessary.                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/30/1995 JLB : Created.                                                                 *
 *   07/14/1995 JLB : Forces SAM site to not fire on landed aircraft.                          *
 *   09/22/1995 JLB : Zone checking enabled.                                                   *
 *   10/05/1995 JLB : Gives greater weight to designated enemy house targets.                  *
 *   02/16/1996 JLB : Added additional threat checks.                                          *
 *=============================================================================================*/
bool TechnoClass::Evaluate_Object(ThreatType method, int mask, int range, TechnoClass const * object, int & value, int zone) const
{
	assert(IsActive);
	assert(object != NULL);

	BStart(BENCH_EVAL_OBJECT);

	/*
	**	An object in limbo can never be a valid target.
	*/
	if (object == NULL || object->IsInLimbo) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	If the object is cloaked, then it isn't a legal target.
	*/
	if (object->Cloak == CLOAKED) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	If the object is in a "harmless" state, then don't bother to consider it
	**	a threat.
	*/
	if (MissionControl[object->Mission].IsNoThreat) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	If the object is not within the desired zone, then ignore it, but only if
	**	zone checking is desired.
	*/
	COORDINATE objectcoord = object->Center_Coord();
	if (zone != -1 && Map[objectcoord].Zones[Techno_Type_Class()->MZone] != zone) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	Friendly units are never considered a good target. Bail if this
	**	object is a friend.  Unless we're a medic, of course.  But then,
	** only consider it a target if it's injured.
	*/
	if (House->Is_Ally(object)) {
		if (Combat_Damage() < 0) {
			if (object->Health_Ratio() == Rule.ConditionGreen) {
				BEnd(BENCH_EVAL_OBJECT);
				return(false);
			}
		} else {
			BEnd(BENCH_EVAL_OBJECT);
			return(false);
		}
	}

	/*
	**	If the object is further away than allowed, bail.
	*/
	int dist = Distance(object);
	if (range > 0 && dist > range) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	if (range == 0) {
		int primary = What_Weapon_Should_I_Use(object->As_Target());
		if (!In_Range(object, primary)) {
			BEnd(BENCH_EVAL_OBJECT);
			return(false);
		}
	}

	/*
	**	If the object is not visible, then bail. Human controlled units
	**	are always considered to be visible.
	*/
	if (!object->IsOwnedByPlayer && !object->IsDiscoveredByPlayer && Session.Type == GAME_NORMAL && object->What_Am_I() != RTTI_AIRCRAFT) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	Quickly eliminate all unit types that are not allowed according to the mask
	**	value.
	*/
	RTTIType otype = object->What_Am_I();
	if (!((1 << otype) & mask)) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);		// Mask failure.
	}

	/*
	**	Determine if the target is theoretically allowed to be a target. If
	**	not, then bail.
	*/
	TechnoTypeClass const * tclass = object->Techno_Type_Class();
	if (!tclass->IsLegalTarget) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);		// Legality failure.
	}

	/*
	**	Never consider a spy to be a valid target, unless you're a dog
	*/
	if (otype == RTTI_INFANTRY && ((InfantryTypeClass const *)tclass)->Type == INFANTRY_SPY) {
		if (What_Am_I() == RTTI_INFANTRY && ((InfantryClass *)this)->Class->IsDog) {
		// continue executing...
		} else {
			BEnd(BENCH_EVAL_OBJECT);
			return(false);
		}
	}

	/*
	**	Special case so that SAM site doesn't fire on aircraft that are landed.
	*/
	if (otype == RTTI_AIRCRAFT && What_Am_I() == RTTI_BUILDING && *((BuildingClass *)this) == STRUCT_SAM) {
		if (((AircraftClass *)object)->Height == 0) {
			BEnd(BENCH_EVAL_OBJECT);
			return(false);
		}
	}

	/*
	**	If only allowed to attack civilians, then eliminate all other types.
	*/
	if ((method & THREAT_CIVILIANS) && object->Owner() != HOUSE_NEUTRAL) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	If the scan is limited to capturable buildings only, then bail if the examined
	**	object isn't a capturable building.
	*/
	if ((method & THREAT_CAPTURE) && (otype != RTTI_BUILDING || !((BuildingTypeClass const *)tclass)->IsCaptureable)) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	** If we're a sub and the subject is a structure, bail if the structure
	** is other than a sub pen or shipyard.
	*/
	if (otype == RTTI_BUILDING && What_Am_I() == RTTI_VESSEL && *(VesselClass *)this == VESSEL_SS) {
		StructType ostruc = *(BuildingClass *)object;
		if (ostruc != STRUCT_SUB_PEN && ostruc != STRUCT_SHIP_YARD) {
			BEnd(BENCH_EVAL_OBJECT);
			return(false);
		}
	}

	/*
	**	SPECIAL CASE: Friendly units won't automatically fire on buildings
	**	if the building is not aggressive. That is, unless it is part of a team. A team
	**	is allowed to pick any target it so chooses.
	*/
	if ((!Is_Foot() || !((FootClass *)this)->Team.Is_Valid()) &&
			(House->IsHuman || (House->IsPlayerControl && Session.Type == GAME_NORMAL)) &&
			otype == RTTI_BUILDING && tclass->PrimaryWeapon == NULL) {
#ifdef OBSOLETE
	if ((!Is_Foot() || ((FootClass *)this)->Team.Is_Valid()) && House->IsHuman && otype == RTTI_BUILDING && tclass->PrimaryWeapon == NULL) {
#endif
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	** Player-controlled demo trucks never automatically target.
	*/
	if ((House->IsHuman || (House->IsPlayerControl && Session.Type == GAME_NORMAL)) && What_Am_I() == RTTI_UNIT && *(UnitClass *)this == UNIT_DEMOTRUCK) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	If the search is restricted to Tiberium processing objects, then
	**	perform the special qualification check now.
	*/
	if (method & THREAT_TIBERIUM) {
		switch (otype) {
			case RTTI_UNIT:
				if (!((UnitTypeClass const *)tclass)->IsToHarvest) {
					BEnd(BENCH_EVAL_OBJECT);
					return(false);
				}
				break;

			case RTTI_BUILDING:
				if (!((BuildingTypeClass const *)tclass)->Capacity && Session.Type != GAME_NORMAL) {
					BEnd(BENCH_EVAL_OBJECT);
					return(false);
				}
				break;

			default:
				BEnd(BENCH_EVAL_OBJECT);
				return(false);
		}
	}

	/*
	**	If this target value is better than the previously recorded best
	**	target value then record this target for possible return as the
	**	best.
	*/
	int rawval = object->Value();
	value = rawval + object->Crew.Kills;

	/*
	**	If the candidate object is owned by the designated enemy of this house, then
	**	give it a higher value. This will tend to gravitate attacks toward the main
	**	antagonist of this house.
	*/
	if (House->Enemy != HOUSE_NONE && House->Enemy == object->House->Class->House) {
		value += 500;
		value *= 3;
	}

	/*
	**	If the object is outside of the protective umbrella of the enemy base, then give it
	**	a target boost value.
	*/
	if (object->House->Which_Zone(object) == ZONE_NONE) {
		value *= 2;
	}

	/*
	**	If fake buildings are considered to be a greater target option, then boost
	**	the fake building's value.
	*/
	if ((method & THREAT_FAKES) && otype == RTTI_BUILDING) {
		//switch (!((BuildingTypeClass const *)tclass)->Type) {
		//The '!' in front of the expression means we are switching on a bool instead of the type. Going to remove it so it compiles, but it might change behavior depending on what Watcom original made of this.
		//Might be better to remove this switch altogether
		//ST - 5/9/2019
		switch (((BuildingTypeClass const *)tclass)->Type) {
			case STRUCT_FAKECONST:
			case STRUCT_FAKEWEAP:
			case STRUCT_FAKE_YARD:
			case STRUCT_FAKE_PEN:
			case STRUCT_FAKE_RADAR:
				break;

			/*
			**	Ignore all non-fake buildings.
			*/
			default:
				value = 0;
				break;
		}
	}

	/*
	**	If power plants are to be considered a greater threat, then increase
	**	their value here. Buildings that produce no power are not considered
	**	a threat.
	*/
	if ((method & THREAT_POWER) && otype == RTTI_BUILDING) {
		if (((BuildingTypeClass const *)tclass)->Power > 0) {
			value += ((BuildingTypeClass const *)tclass)->Power * 1000;
		} else {
			value = 0;
		}
	}

	/*
	**	If factories are to be considered a greater threat, then don't
	**	consider any non-factory building.
	*/
	if ((method & THREAT_FACTORIES) && otype == RTTI_BUILDING) {
		if (((BuildingTypeClass const *)tclass)->ToBuild == RTTI_NONE) {
			value = 0;
		}
	}

	/*
	**	If base defensive structures are to be considered a greater threat, then
	**	don't consider an unarmed building to be a threat.
	*/
	if ((method & THREAT_BASE_DEFENSE) /*&& otype == RTTI_BUILDING*/) {
		if (tclass->PrimaryWeapon == NULL) {
			value = 0;
		}
	}

	/*
	**	Possibly cause a reduction of the target's value if it is nearby friendly
	**	structures and the primary weapon of this object is flagged for
	**	friendly fire supression special check logic.
	*/
	fixed areamod = Area_Modify(Coord_Cell(object->Center_Coord()));
	if (areamod != 1) {
		value = areamod * value;
	}

	/*
	**	Adjust the target value upward if it is in the 'nervous zone' of the
	**	owning base. This will tend to protect the base more thoroughly than
	**	an unmodified scan would.
	*/
	if (House->Which_Zone(object) != ZONE_NONE) {
		value *= Rule.NervousBias;
	}

	/*
	**	Lessen threat as a factor of distance.
	*/
	if (value) {
//	if (rawval) {

		value = (value * 32000) / ((dist/ICON_LEPTON_W)+1);
//		value = (value * 32000) / (((dist/ICON_LEPTON_W)*(dist/ICON_LEPTON_W))+1);

//		if (value < MAP_CELL_W*2) value = dist/ICON_LEPTON_W;
		value = max(value, 1);
		BEnd(BENCH_EVAL_OBJECT);
		return(true);
	}
	value = 0;
	BEnd(BENCH_EVAL_OBJECT);
	return(false);
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
 *          zone     -- The zone restriction if any. A -1 means no zone check required.        *
 *                                                                                             *
 * OUTPUT:  Was a valid potential target found in this cell?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *   09/22/1995 JLB : Zone checking enabled.                                                   *
 *=============================================================================================*/
bool TechnoClass::Evaluate_Cell(ThreatType method, int mask, CELL cell, int range, TechnoClass const * * object, int & value, int zone) const
{
	assert(IsActive);

	BStart(BENCH_EVAL_CELL);

	*object = NULL;
	value = 0;

	/*
	**	If the cell is not on the legal map, then always ignore it.
	*/
	if ((unsigned)cell > MAP_CELL_TOTAL) {
		BEnd(BENCH_EVAL_CELL);
		return(false);
	}
	if (!Map.In_Radar(cell)) {
		BEnd(BENCH_EVAL_CELL);
		return(false);
	}

	/*
	**	Fetch the techno object from the cell. If there is no
	**	techno object there, then bail.
	*/
	CellClass * cellptr = &Map[cell];

	/*
	**	Don't consider for evaluation a cell that is not within the same zone. Only
	**	perform this check if zone checking is required.
	*/
	if (zone != -1 && cellptr->Zones[Techno_Type_Class()->MZone] != zone) {
		BEnd(BENCH_EVAL_CELL);
		return(false);
	}

	TechnoClass const * tentative = (TechnoClass const *)cellptr->Cell_Occupier();
	while (tentative != NULL) {
		if (tentative != this) {
			if (tentative->Is_Techno()) {
				if (Combat_Damage() < 0) {
					if (tentative->Health_Ratio() < Rule.ConditionGreen && House->Is_Ally(tentative)) break;
				} else {
					if (!House->Is_Ally(tentative)) break;
				}
			}
		}
		tentative = (TechnoClass const *)(ObjectClass *)tentative->Next;
	}

	if (tentative == NULL) {
		BEnd(BENCH_EVAL_CELL);
		return(false);
	}
	*object = tentative;

	bool result = Evaluate_Object(method, mask, range, tentative, value);

	BEnd(BENCH_EVAL_CELL);
	return(result);
}


/***********************************************************************************************
 * TechnoClass::Evaluate_Just_Cell -- Evaluate a cell as a target by itself.                   *
 *                                                                                             *
 *    This will examine the cell (as if it contained no sentient objects) and determine a      *
 *    target value to assign to it. Typically, this is only useful for wall destroyable        *
 *    weapons when dealing with enemy walls.                                                   *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine and evaluate.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the target value to assign to this cell.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Evaluate_Just_Cell(CELL cell) const
{
	BStart(BENCH_EVAL_WALL);

	/*
	**	Ships don't scan for walls.
	*/
	if (What_Am_I() == RTTI_VESSEL) {
		return(0);
	}

	/*
	**	First, only computer objects are allowed to automatically scan for walls.
	*/
	if (House->IsHuman) {
		BEnd(BENCH_EVAL_WALL);
		return(0);
	}

	/*
	**	Even then, if the difficulty indicates that it shouldn't search for wall
	**	targets, then don't allow it to do so.
	*/
	if (!Rule.Diff[House->Difficulty].IsWallDestroyer) {
		BEnd(BENCH_EVAL_WALL);
		return(0);
	}

	/*
	**	Determine if, in fact, a wall is located at this cell location.
	*/
	CellClass const * cellptr = &Map[cell];
	if (cellptr->Overlay == OVERLAY_NONE || !OverlayTypeClass::As_Reference(cellptr->Overlay).IsWall) {
		BEnd(BENCH_EVAL_WALL);
		return(0);
	}

	/*
	**	As a convenience to the target scanning logic, don't consider any wall to be
	**	a target if it isn't in range of the primary weapon.
	*/
	int primary = What_Weapon_Should_I_Use(::As_Target(cell));
	if (!In_Range(Cell_Coord(cell), primary)) {
		BEnd(BENCH_EVAL_WALL);
		return(0);
	}

	/*
	**	See if the object has a weapon that can damage walls.
	*/
	TechnoTypeClass const * ttype = (TechnoTypeClass const *)Techno_Type_Class();
	if (ttype->PrimaryWeapon == NULL || ttype->PrimaryWeapon->WarheadPtr == NULL) {
		BEnd(BENCH_EVAL_WALL);
		return(0);
	}

	/*
	**	If the weapon cannot deal with ground based targets, then don't consider
	**	this a valid cell target.
	*/
	if (ttype->PrimaryWeapon->Bullet != NULL && !ttype->PrimaryWeapon->Bullet->IsAntiGround) {
		BEnd(BENCH_EVAL_WALL);
		return(0);
	}

	/*
	**	If the primary weapon cannot destroy a wall, then don't give the cell any
	**	value as a target.
	*/
	if (!ttype->PrimaryWeapon->WarheadPtr->IsWallDestroyer) {
		BEnd(BENCH_EVAL_WALL);
		return(0);
	}

	/*
	**	If this is a friendly wall, then don't attack it.
	*/
	if (House->Is_Ally(cellptr->Owner)) {
		BEnd(BENCH_EVAL_WALL);
		return(0);
	}

	/*
	**	Since a wall was found, then return a value adjusted according to the range the wall
	**	is from the object. The greater the range, the lesser the value returned.
	*/
	BEnd(BENCH_EVAL_WALL);
	return(Weapon_Range(0) - Distance(Cell_Coord(cell)));
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
 *   09/22/1995 JLB : Takes into account the zone (if necessary).                              *
 *   05/30/1996 JLB : Tighter elimination mask checking.                                       *
 *=============================================================================================*/
TARGET TechnoClass::Greatest_Threat(ThreatType method) const
{
	assert(IsActive);

	BStart(BENCH_GREATEST_THREAT);

	ObjectClass const * bestobject = NULL;
	int bestval = -1;
	int zone = -1;

	TargetScan++;

	/*
	**	Determine the zone that the target must be in. For aircraft and gunboats, they
	**	ignore zones since they either can fly over any zone or are designed to fire into
	**	other zones. If scanning for targets that are within range, then zone checking need
	**	not be performed -- range checking is much more thorough and effective.
	*/
	if (!(method & THREAT_RANGE) &&
		What_Am_I() != RTTI_VESSEL &&
		What_Am_I() != RTTI_BUILDING &&
		What_Am_I() != RTTI_AIRCRAFT) {

		zone = Map[Center_Coord()].Zones[Techno_Type_Class()->MZone];
	}

	/*
	** Hack for dogs, 'cause they can only consider infantrymen to be a
	** threat.  Medics also.
	*/
	if (What_Am_I() == RTTI_INFANTRY) {
		if (((InfantryClass *)this)->Class->IsDog || Combat_Damage() < 0) {
			method = THREAT_INFANTRY | (method & (THREAT_RANGE | THREAT_AREA));
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			if(*(InfantryClass *)this == INFANTRY_MECHANIC) {
				method = (THREAT_VEHICLES | THREAT_AIR) | (method & (THREAT_RANGE | THREAT_AREA));
			}
#endif
		}
	}

	/*
	**	Build a quick elimination mask. If the RTTI of the object doesn't
	**	qualify with this mask, then we KNOW that it shouldn't be considered.
	*/
	int mask = 0;
	if (method & THREAT_CIVILIANS) mask |= ((1 << RTTI_BUILDING) | (1 << RTTI_INFANTRY) | (1 << RTTI_UNIT));
	if (method & THREAT_AIR) mask |= (1 << RTTI_AIRCRAFT);
	if (method & THREAT_CAPTURE) mask |= (1 << RTTI_BUILDING);
	if (method & (THREAT_CIVILIANS|THREAT_BUILDINGS|THREAT_FACTORIES|THREAT_POWER|THREAT_FAKES|THREAT_BASE_DEFENSE|THREAT_TIBERIUM)) mask |= (1 << RTTI_BUILDING);
	if (method & (THREAT_CIVILIANS|THREAT_INFANTRY|THREAT_BASE_DEFENSE)) mask |= (1 << RTTI_INFANTRY);
	if (method & THREAT_VEHICLES) mask |= (1 << RTTI_UNIT);
	if (method & THREAT_BASE_DEFENSE) mask |= (1 << RTTI_BUILDING);
	if (method & THREAT_BOATS) mask |= (1 << RTTI_VESSEL);

	/*
	**	Limit area target scans use a method where the actual map cells are
	**	examined for occupants. The occupant is then examined in turn. The
	**	best target within the area is returned as a target.
	*/
	if (method & (THREAT_AREA|THREAT_RANGE)) {
		int range = Threat_Range((method & THREAT_RANGE) ? 0 : 1);

		int crange = range / ICON_LEPTON_W;
		if (range == 0) {
			crange = max(Weapon_Range(0), Weapon_Range(1)) / ICON_LEPTON_W;
			crange++;
		}
		CELL cell = Coord_Cell(Fire_Coord(0));

		/*
		** BG: Miserable hack to get the stupid doctor to actually do area
		** 	 guarding.
		*/
		if (Combat_Damage() < 0) {
			/*if (method & THREAT_AREA)*/ crange++;
		}

		/*
		**	If aircraft are a legal target, then scan through all of them at this time.
		**	Scanning by cell is not possible for aircraft since they are not recorded
		**	at the cell level.
		*/
		if (method & THREAT_AIR) {
			for (int index = 0; index < Aircraft.Count(); index++) {
				TechnoClass * object = Aircraft.Ptr(index);

				int value = 0;
				if (object->In_Which_Layer() != LAYER_GROUND && Evaluate_Object(method, mask, range, object, value)) {
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
		CELL bestcell = -1;
		int bestcellvalue = 0;
		TechnoClass const * object;
		int value;
//		int rad = 1;

		// BG: Medics need to be able to look in their own cell too.
//		if (Combat_Damage() < 0 || (What_Am_I() == RTTI_INFANTRY && ((InfantryClass*)this)->Class->IsDog)) {
//			rad = 0;
//		}

		for (int radius = 0; radius < crange; radius++) {

			/*
			**	Scan the top and bottom rows of the "box".
			*/
			for (int x = -radius; x <= radius; x++) {
				CELL newcell;

				if ((Cell_X(cell) + x) < Map.MapCellX) continue;
				if ((Cell_X(cell) + x) >= (Map.MapCellX+Map.MapCellWidth)) continue;

				if ((Cell_Y(cell) - radius) >= Map.MapCellY) {
					newcell = XY_Cell(Cell_X(cell) + x, Cell_Y(cell)-radius);
					if (Evaluate_Cell(method, mask, newcell, range, &object, value, zone)) {
						if (bestval < value) {
							bestobject = object;
						}
					}
					if (bestobject == NULL) {
						value = Evaluate_Just_Cell(newcell);
						if (bestcellvalue < value) {
							bestcellvalue = value;
							bestcell = newcell;
						}
					}
				}

				if ((Cell_Y(cell) + radius) < (Map.MapCellY+Map.MapCellHeight)) {
					newcell = XY_Cell(Cell_X(cell)+x, Cell_Y(cell)+radius);
					if (Evaluate_Cell(method, mask, newcell, range, &object, value, zone)) {
						if (bestval < value) {
							bestobject = object;
						}
					}
					if (bestobject == NULL) {
						value = Evaluate_Just_Cell(newcell);
						if (bestcellvalue < value) {
							bestcellvalue = value;
							bestcell = newcell;
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
					if (Evaluate_Cell(method, mask, newcell, range, &object, value, zone)) {
						if (bestval < value) {
							bestobject = object;
						}
					}
					if (bestobject == NULL) {
						value = Evaluate_Just_Cell(newcell);
						if (bestcellvalue < value) {
							bestcellvalue = value;
							bestcell = newcell;
						}
					}
				}

				if ((Cell_X(cell) + radius) < (Map.MapCellX+Map.MapCellWidth)) {
					newcell = XY_Cell(Cell_X(cell)+radius, Cell_Y(cell)+y);
					if (Evaluate_Cell(method, mask, newcell, range, &object, value, zone)) {
						if (bestval < value) {
							bestobject = object;
						}
					}
					if (bestobject == NULL) {
						value = Evaluate_Just_Cell(newcell);
						if (bestcellvalue < value) {
							bestcellvalue = value;
							bestcell = newcell;
						}
					}
				}
			}

			/*
			**	Bail early if a target has already been found and the range is at
			**	one of the breaking points (i.e., normal range or range * 2).
			*/
			if (bestobject != NULL) {
				if (radius == crange/4) {
					return(bestobject->As_Target());
				}
				if (radius == crange/2) {
					return(bestobject->As_Target());
				}
			}
			if (bestcell != -1) {
				return(::As_Target(bestcell));
			}
		}

	} else {
		/*
		**	A full map scan was requested. First scan through aircraft. The top map layer
		**	is NOT scanned since that layer will probably contain more bullets and animations
		**	than aircraft.
		*/
		if (mask & (1L << RTTI_AIRCRAFT)) {
			for (int index = 0; index < Aircraft.Count(); index++) {
				TechnoClass * object = Aircraft.Ptr(index);

				int value = 0;
				if (Evaluate_Object(method, mask, -1, object, value)) {
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
		**	Now scan through the entire ground layer. This is painful, but what other
		**	choice is there?
		*/
		for (int index = 0; index < Map.Layer[LAYER_GROUND].Count(); index++) {
			ObjectClass const * object = Map.Layer[LAYER_GROUND][index];

			int value = 0;
			if (object->Is_Techno() && Evaluate_Object(method, mask, -1, (TechnoClass const *)object, value, zone)) {
				if (value > bestval) {
					bestobject = object;
					bestval = value;
				}
			}
		}
	}

	BEnd(BENCH_GREATEST_THREAT);

	/*
	**	If a good target object was found, then return with the target value
	**	of it.
	*/
	if (bestobject != NULL) {
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
	assert(IsActive);

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
	assert(IsActive);

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
	assert(IsActive);

	/*
	**	Handle recoil recovery here.
	*/
	if (IsInRecoilState) {
		IsInRecoilState = false;
		Mark(MARK_CHANGE_REDRAW);
	}

	/*
	** If this building is being spied on by the player, need to redraw if selected
	** since the money amount is rendering.
	*/
	if (Is_Selected_By_Player()) {
		if (What_Am_I() == RTTI_BUILDING) {
			int spiedby = Spied_By() & (1<<(PlayerPtr->Class->House));
			if (spiedby) {
				if (((BuildingClass *)this)->Class->Capacity) {
					Mark(MARK_CHANGE_REDRAW);
				}
			}
		}
	}

	CargoClass::AI();
	RadioClass::AI();

	if (!IsActive || (Height > 0 && What_Am_I() != RTTI_AIRCRAFT)) return;

	DoorClass::AI();

	/*
	**	If this is a vehicle that heals itself (e.g., Mammoth Tank), then it will perform
	**	the heal logic here.
	*/
	if (Techno_Type_Class()->IsSelfHealing && (Frame % (Rule.RepairRate * TICKS_PER_MINUTE)) == 0 && Health_Ratio() <= Rule.ConditionYellow) {
		Strength++;
		Mark(MARK_CHANGE);
	}

	/*
	**	Cloaking device processing.
	*/
	Cloaking_AI();

	/*
	**	If for some strange reason, the computer is firing upon itself, then
	**	tell it not to.
	*/
	if (!House->IsHuman && As_Techno(TarCom) && As_Techno(TarCom)->House->Is_Ally(this)) {
//#ifdef FIXIT_CSII	//	checked - ajw 9/28/98 (commented out)
//if(What_Am_I() == RTTI_INFANTRY && *(InfantryClass *)this==INFANTRY_GENERAL && Session.Type==GAME_NORMAL && House->Class->House==HOUSE_UKRAINE) {
//} else
//#endif
		Assign_Target(TARGET_NONE);
	}

	/*
	**	Perform a maintenance check to see that if somehow this object is trying to fire
	**	upon an object it can never hit (because it can't reach it), then abort the tarcom
	*/
	if (What_Am_I() != RTTI_AIRCRAFT && Target_Legal(TarCom) && (!Is_Foot() || !((FootClass *)this)->Team.Is_Valid()) && (!Is_Foot() || !Is_In_Same_Zone(As_Cell(TarCom)))) {
		int primary = What_Weapon_Should_I_Use(TarCom);
		if (!In_Range(TarCom, primary)) {
			Assign_Target(TARGET_NONE);
		}
	}

	/*
	**	Update the animation timer system. If the animation stage
	**	changes, then flag the object to be redrawn as well as determine
	**	if the current animation process needs to change.
	*/
	if (What_Am_I() != RTTI_BUILDING) {
		if (StageClass::About_To_Change()) {
			Mark(MARK_CHANGE_REDRAW);
		}
		if (StageClass::Graphic_Logic() || Time_To_Redraw()) {
			Mark(MARK_CHANGE_REDRAW);
		}
	}

	/*
	**	If the object is flashing and a change of flash state has occurred, then mark the
	**	object to be redrawn.
	*/
	if (FlasherClass::Process()) {
		Mark(MARK_CHANGE);
	}

	/*
	**	Handle electric zap delay logic.
	*/
	if (ElectricZapDelay >= 0) {
		Map.Flag_To_Redraw(true);
		if (--ElectricZapDelay < 0) {
			ElectricZapTarget = 0;
			ElectricZapWhich = 0;
		}
	}
}


/***********************************************************************************************
 * TechnoClass::Cloaking_AI -- Perform the AI maintenance for a cloaking device.               *
 *                                                                                             *
 *    This routine handles the cloaking device logic for this object. It will handle the       *
 *    transition effects as the object cloaks or decloaks. It will also try to start an        *
 *    object to cloak if possible.                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Cloaking_AI(void)
{
	/*
	** Handle decision to re-cloak here. Process the cloaking/decloaking operation.
	*/
	if (IsCloakable) {

		/*
		**	If this object is uncloaked, but it can be cloaked and it thinks that it
		**	is a good time do so, then begin cloaking.
		*/
		if (Cloak == UNCLOAKED) {
#ifdef PREDATOR
			// Changed for multiplayer so we can visually see the different players in the original renderer. ST - 3/13/2019 5:40PM
			//if (IsOwnedByPlayer) Mark(MARK_CHANGE);
			if (Is_Owned_By_Player()) Mark(MARK_CHANGE);
#endif
			CloakingDevice.Graphic_Logic();
			if (Is_Ready_To_Cloak()) {
				if (Health_Ratio() > Rule.ConditionRed) {
					Do_Cloak();
				} else {
					if (Percent_Chance(4)) {
						Do_Cloak();
					}
				}
			}
		} else {

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
						CloakingDevice.Set_Rate(0);
						CloakingDevice.Set_Stage(0);	// re-start the stage counter
						Cloak = UNCLOAKED;
						CloakDelay = Rule.CloakDelay * TICKS_PER_MINUTE;
						Mark(MARK_CHANGE);
					}
					break;

				/*
				**	Handle the cloaking process. Always mark to redraw the object
				**	and when the cloaking process is complete, stabilize into the
				**	normal cloaked state.
				*/
				case CLOAKING:
					Mark(MARK_CHANGE);
					if(!CloakingDevice.Fetch_Rate()) {
						CloakingDevice.Set_Rate(1);
					}
					switch (Visual_Character(true)) {

						/*
						**	If badly damaged, then it can never fully cloak.
						*/
						case VISUAL_DARKEN:
							if (Health_Ratio() <= Rule.ConditionRed && Percent_Chance(25)) {
								Cloak = UNCLOAKING;
							}
							break;

						case VISUAL_HIDDEN:
							Cloak = CLOAKED;
							CloakingDevice.Set_Rate(0);
							CloakingDevice.Set_Stage(0);
							Mark(MARK_CHANGE);

							Map[Center_Coord()].Redraw_Objects(true);
							Map.RadarClass::Flag_To_Redraw(true);

							/*
							**	Special check to ensure that if the unit is carrying a captured
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
#ifdef PREDATOR
					//if (IsOwnedByPlayer) {
					// Changed for multiplayer so we can visually see the different players in the original renderer. ST - 3/13/2019 5:40PM
					if (Is_Owned_By_Player()) {
						Mark(MARK_CHANGE);
					}
#endif
					break;
			}
		}
	}
}


/***********************************************************************************************
 * TechnoClass::Is_Ready_To_Cloak -- Determines if this object is ready to begin cloaking.     *
 *                                                                                             *
 *    This routine will examine this object and determine if it can and is ready and able      *
 *    to begin cloaking. It will also check to make sure it appears to be a good time to cloak *
 *    as well.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this unit ready and able to start cloaking?                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Ready_To_Cloak(void) const
{
	/*
	**	If it is already cloaked or in the process of cloaking, then it can't start cloaking.
	*/
	if (Cloak == CLOAKED || (Cloak == CLOAKING && CloakingDevice.Fetch_Rate())) {
		return(false);
	}

	/*
	**	If the object cannot recloak, then it certainly is not allowed to start.
	*/
	if (!IsCloakable || !Is_Allowed_To_Recloak()) {
		return(false);
	}

	/*
	**	If the object is currently rearming, then don't begin to recloak.
	*/
	if (Arm != 0) {
		return(false);
	}

	/*
	**	If it seems like this object is about to fire on a target, then don't begin
	**	cloaking either.
	*/
	if (Target_Legal(TarCom) && In_Range(TarCom)) {
		return(false);
	}

	/*
	**	Recloaking can only begin if the cloaking device is not already operating.
	*/
	if (CloakingDevice.Fetch_Stage() != 0) {
		return(false);
	}

	/*
	**	If the arbitrary cloak delay value is still counting down, then don't
	**	allow recloaking just yet.
	*/
	if (CloakDelay != 0) {
		return(false);
	}

	/*
	**	All tests passed, so this object is allowed to begin cloaking.
	*/
	return(true);
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
	assert(IsActive);

	//if (!IsDiscoveredByPlayer && !House->IsPlayerControl && !Debug_Unshroud) {  // ST - 8/7/2019 11:24AM	
	if (!Is_Discovered_By_Player() && !House->IsPlayerControl && !Debug_Unshroud) {
		return(false);
	}

	if (RadioClass::Select(allow_mixed)) {

		/*
		**	Speak a confirmation of selection.
		*/
		if (House->IsPlayerControl && AllowVoice) {
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
	assert(IsActive);

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
	if (object != NULL && object->Is_Techno() && ((TechnoClass *)object)->Cloak == CLOAKED) {
		return(FIRE_CANT);
	}

	/*
	**	A falling object is too busy falling to fire.
	*/
	if (IsFalling) {
		return(FIRE_CANT);
	}

	/*
	**	If there is no weapon, then firing is not allowed.
	*/
	WeaponTypeClass const * weapon = ((which == 0) ? Techno_Type_Class()->PrimaryWeapon : Techno_Type_Class()->SecondaryWeapon);
	if (weapon == NULL) {
		return(FIRE_CANT);
	}

	/*
	**	Can only fire anti-aircraft weapons against aircraft unless the aircraft is
	**	sitting on the ground.
	*/
	if (object != NULL && object->What_Am_I() == RTTI_AIRCRAFT &&
			!weapon->Bullet->IsAntiAircraft &&
			((AircraftClass *)object)->Height > 0) {

		return(FIRE_CANT);
	}

	/*
	**	If the object is on the ground, then don't allow firing if it can't fire upon ground objects.
	*/
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (object != NULL && object->Height == 0 && (object->What_Am_I() != RTTI_VESSEL || (*((VesselClass*)object) != VESSEL_SS && *((VesselClass*)object) != VESSEL_MISSILESUB )) &&
#else
	if (object != NULL && object->Height == 0 && (object->What_Am_I() != RTTI_VESSEL || *((VesselClass*)object) != VESSEL_SS) &&
#endif
		!weapon->Bullet->IsAntiGround) {

		return(FIRE_CANT);
	}
	if (Is_Target_Cell(target) && !weapon->Bullet->IsAntiGround) {
		return(FIRE_CANT);
	}

	/*
	**	Don't allow firing if still rearming.
	*/
	if (Arm != 0) return(FIRE_REARM);

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
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
// Special hack for John Archer's Hunt-The-Wumpus multiplayer mission... if
// the object firing is a cloaked civilian, don't require uncloaking before
// allowing firing.
		if (What_Am_I()==RTTI_INFANTRY && ((InfantryClass *)this)->Class->IsCivilian ) {
			return(FIRE_OK);
		}
#endif
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
	assert(IsActive);

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
	assert(IsActive);

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
			if (object != NULL && (object->IsActive == false || object->Strength == 0)) {
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
int TechnoClass::Rearm_Delay(bool second, int which) const
{
	assert(IsActive);

	if (What_Am_I() == RTTI_BUILDING && Ammo > 1) {
		return(1);
	}

	WeaponTypeClass const * weapon = (which == 0) ? Techno_Type_Class()->PrimaryWeapon : Techno_Type_Class()->SecondaryWeapon;
	if (second && weapon != NULL) {
		return(weapon->ROF * House->ROFBias);
	}
	return(3);
}


/***********************************************************************************************
 * TechnoClass::Electric_Zap -- Fires electric zap at the target specified.                    *
 *                                                                                             *
 *    This routine is used to fire an electric zap at the target specified.                    *
 *                                                                                             *
 * INPUT:   target   -- The target to fire the zap at.                                         *
 *                                                                                             *
 *          which    -- Which weapon is this zap associated with (0=primary, 1=secondary).     *
 *                                                                                             *
 *          window      -- The clipping window to use when rendering.                          *
 *                                                                                             *
 *          source_coord   -- The coordinate that the zap is to originate from. This is an     *
 *                            override value and if not specifide, the normal fire coordinate  *
 *                            is used.                                                         *
 *                                                                                             *
 *          remap    -- Pointer to the zap animation remap override table. If not specified    *
 *                      then the zap remains the normal blue white color.                      *
 *                                                                                             *
 * OUTPUT:  bool; Does this object need to redraw?                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 BWG : Created.                                                                 *
 *   09/30/1996 JLB : Uses standard facing conversion and distance routines.                   *
 *=============================================================================================*/
bool TechnoClass::Electric_Zap(COORDINATE target_coord, int which, WindowNumberType window, COORDINATE source_coord, unsigned char * remap) const
{
	//int x,y,x1,y1;
	//PG init variables
	int x = 0;
	int y = 0;
	int x1 = 0;
	int y1 = 0;
	COORDINATE source;

	if (source_coord != 0) {
		source = source_coord;
	} else {
		source = Fire_Coord(which);
	}
	if (What_Am_I() == RTTI_BUILDING) {
		((BuildingClass *)this)->IsCharging = false;
	}
	bool gonnadraw = false;

	if (SpecialDialog == SDLG_NONE) {
		Map.Coord_To_Pixel(source, x, y);
		Map.Coord_To_Pixel(target_coord, x1, y1);
		x += Map.TacPixelX;
		x1 += Map.TacPixelX;
		y += Map.TacPixelY;
		y1 += Map.TacPixelY;
		gonnadraw = true;
	}

	static int _shape[]={ 2, 3, 1, 0, 2, 3, 1, 0};
	static int  _xadd[8][8]={
		{ 0, 8, 8, 8, 0, 0, 0, 0},
		{ 0, 8, 8, 8, 0, 0, 0, 0},
		{ 0, 8, 8, 8, 0, 0, 0, 0},
		{ 0, 8, 8, 8, 0, 0, 0, 0},
		{ 0, 8, 8, 8, 0, 0, 0, 0},
		{-8, 0, 0, 0,-8,-8,-8,-8},
		{-8, 0, 0, 0,-8,-8,-8,-8},
		{-8, 0, 0, 0,-8,-8,-8,-8}
	};
	static int  _yadd[8][8]={
		{-8,-8,-8, 0, 0, 0,-8,-8},
		{-8,-8,-8, 0, 0, 0,-8,-8},
		{ 0, 0, 0, 8, 8, 8, 0, 0},
		{ 0, 0, 0, 8, 8, 8, 0, 0},
		{ 0, 0, 0, 8, 8, 8, 0, 0},
		{ 0, 0, 0, 8, 8, 8, 0, 0},
		{ 0, 0, 0, 8, 8, 8, 0, 0},
		{-8,-8,-8, 0, 0, 0,-8,-8}
	};

	int savex = x, savey = y;
	if (gonnadraw) {
		for (int shots = 0; shots < 3; shots++) {
			x = savex;
			y = savey;
			int lastfacing = 0;
			while (::Distance(x, y, x1, y1) > 8) {

				/*
				**	Determine true (0..7) facing from current position to
				**	destination (actually the source coordinate of the zap).
				*/
				int facing = Dir_Facing(Desired_Facing8(x, y, x1, y1));

				/*
				** If there's quite a bit of distance to go,
				** we may vary the desired facing to give the
				** bolt some randomness.
				*/
				if (::Distance(x, y, x1, y1) > 40) {
					switch (Sim_Random_Pick(1, 3 + ((shots==0) ? 3 : 0))) {
						case 1:
							facing++;
							break;

						case 2:
							facing--;
							break;

						default:
							break;
					}
					facing &= 7;
				}

				/*
				** Now that we have the direction of the bolt,
				** draw it and move the x & y coords in the right
				** direction for the next piece.
				*/
				// Electric zap coordinates are always tactical, so don't use the partial window if passed - SKY
				x += _xadd[facing][lastfacing];
				y += _yadd[facing][lastfacing];
				if (remap != NULL) {
					CC_Draw_Shape(this, "LITNING", LightningShapes, _shape[facing]+(shots ? 4 : 0), x, y, (window != WINDOW_PARTIAL) ? window : WINDOW_TACTICAL, SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL, remap);
				} else {
					CC_Draw_Shape(this, "LITNING", LightningShapes, _shape[facing]+(shots ? 4 : 0), x, y, (window != WINDOW_PARTIAL) ? window : WINDOW_TACTICAL, SHAPE_CENTER|SHAPE_WIN_REL);
				}
				lastfacing = facing;
			}
		}
	}

	return (gonnadraw);
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
 *   02/22/1996 JLB : Handles camera "weapon" case.                                            *
 *=============================================================================================*/
BulletClass * TechnoClass::Fire_At(TARGET target, int which)
{
	assert(IsActive);

	BulletClass * bullet;				// Projectile.
	DirType dir;					// The facing to impart upon the projectile.
	COORDINATE target_coord;		// Coordinate of the target.
	COORDINATE fire_coord;			// Coordinate of firing position.
	TechnoTypeClass const & tclass = *Techno_Type_Class();
	ObjectClass * object;
	WeaponTypeClass const * weapon = (which == 0) ? tclass.PrimaryWeapon : tclass.SecondaryWeapon;

	/*
	**	If this object doesn't have a weapon, then it is obvious that firing
	**	cannot ever succeed. Return with failure flag.
	*/
	if (weapon == NULL) return(NULL);

	BulletTypeClass const & btype = *weapon->Bullet;

	/*
	**	Perform a quick legality check to see if firing can occur.
	*/
	if (Debug_Map || !Target_Legal(target)) {
		return(NULL);
	}

	/*
	**	Fetch the target coordinate for the target specified.
	*/
	object = As_Object(target);
	if (object != NULL) {
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
	if (btype.ROT != 0 || btype.IsDropping) {
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
	int firepower = weapon->Attack;
	if (firepower > 0) {
		firepower = weapon->Attack * FirepowerBias * House->FirepowerBias;
	}


	/*
	**	Give the bullet a boost of speed if the weapon indicates that this is required. Only
	**	need to perform this check if the target is an aircraft.
	*/
	int firespeed = weapon->MaxSpeed;
	if (weapon->IsTurboBoosted && Is_Target_Aircraft(target)) {
		firespeed *= Rule.TurboBoost;
	}

 	bullet = new BulletClass(weapon->Bullet->Type, target, this, firepower, WarheadType(weapon->WarheadPtr->ID), firespeed);

	if (bullet != NULL) {

		/*
		**	If this is firing from a moving platform, then the projectile is inaccurate.
		*/
		if (Is_Foot() && ((FootClass const *)this)->IsDriving) {
			bullet->IsInaccurate = true;
		}

		if (bullet->Unlimbo(fire_coord, dir)) {
		} else {
			delete bullet;
		}
		if (tclass.IsTurretEquipped) {
			IsInRecoilState = true;
			Mark(MARK_CHANGE_REDRAW);
		}

		Arm = Rearm_Delay(IsSecondShot, which);
		if (tclass.Is_Two_Shooter()) {
			IsSecondShot = (IsSecondShot == false);
		}

		/*
		**	Perform any animation effect for this weapon.
		*/
		AnimType a = weapon->Anim;
		switch (a) {
			case ANIM_GUN_N:
				a = AnimType(a + Dir_Facing(Fire_Direction()));
				break;

			case ANIM_SAM_N:
				a = AnimType(ANIM_SAM_N + Dir_Facing(PrimaryFacing.Current()));
				break;
		}

  		/*
		**	Play any sound effect tied to this weapon type.
		*/
		Sound_Effect(weapon->Sound, Fire_Coord(which));

		/*
		**	If there is a special firing animation, then create and attach it
		**	now.
		*/
		if (a != ANIM_NONE) {
			AnimClass * anim = new AnimClass(a, Fire_Coord(which));
			if (anim != NULL) {
				anim->Attach_To(this);
			}
		}

		/*
		**	Electric zap animation.
		*/
		if (weapon->IsElectric) {
			ElectricZapDelay = 3;
			ElectricZapTarget = target_coord;
			ElectricZapWhich = which;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			if(What_Am_I() != RTTI_INFANTRY) {
				Set_Stage(0);
				Set_Rate(0);
			}
#else
			Set_Stage(0);
			Set_Rate(0);
#endif
			if (Ammo <= 1 && What_Am_I() == RTTI_BUILDING) {
				((BuildingClass *)this)->IsCharged = false;
			}

			TechnoClass * tech = As_Techno(target);
			if (tech != NULL)  {
				tech->Clicked_As_Target(PlayerPtr->Class->House, 4); // 2019/09/20 JAS - Added record of who clicked on the object
			}
		}

		/*
		**	Reduce ammunition for this object.
		*/
		if (Ammo > 0) {
			Ammo--;
		}

		/*
		**	Firing will in all likelihood, require the unit to be redrawn. Flag it to be
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
		if ((!IsOwnedByPlayer && !IsDiscoveredByPlayer) || (!Map[Center_Coord()].IsMapped && (What_Am_I()!=RTTI_AIRCRAFT || !IsOwnedByPlayer)) ) {
			if (Session.Type == GAME_NORMAL) {
				Map.Sight_From(Coord_Cell(Center_Coord()), 2, PlayerPtr, false);
			} else {
				ObjectClass * obj = As_Object(target);
				if (obj != NULL) {
					HousesType tgt_owner = obj->Owner();

					if (PlayerPtr->Class->House == tgt_owner) {
						Map.Sight_From(Coord_Cell(Center_Coord()), 2, PlayerPtr, false);
					}
				}
			}
		}

#else

		/*
		** For client/server multiplayer, we need to reveal for any human player that is the target. ST - 3/13/2019 5:43PM
		*/
		ObjectClass *obj = As_Object(target);
		if (obj) {
			HousesType tgt_owner = obj->Owner();

			HouseClass *player = HouseClass::As_Pointer(tgt_owner);
			if ((player != NULL) && player->IsHuman) {
				if ((!Is_Owned_By_Player(player) && !Is_Discovered_By_Player(player)) || (!Map[Coord_Cell(Center_Coord())].Is_Mapped(House) && (What_Am_I()!=RTTI_AIRCRAFT || !Is_Owned_By_Player(player))) ) {
					Map.Sight_From(Coord_Cell(Center_Coord()), 1, player, false);
				}
			}
		}

#endif
	}

	return(bullet);
}


/***********************************************************************************************
 * TechnoClass::Player_Assign_Mission -- Assigns a mission as result of player input.          *
 *                                                                                             *
 *    This routine is called when the mission for an object needs to change as a result of     *
 *    player input. The basic operation would be to queue the event and let the action         *
 *    occur at the frame dictated by the queuing system. However, if a voice response is       *
 *    indicated, then perform it at this time. This will give a greater illusion of            *
 *    immediate response.                                                                      *
 *                                                                                             *
 * INPUT:   mission     -- The mission order to assign to this object.                         *
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
	assert(IsActive);

	if (AllowVoice) {
		if (mission == MISSION_ATTACK) {
			Response_Attack();
		} else {
			Response_Move();
		}
	}

	if (FormMove) {
		SpeedType speed = FormSpeed;
		MPHType maxspeed = FormMaxSpeed;
		if (Is_Foot()) {
			const FootClass* foot = (const FootClass*)this;
			if (foot->Group < MAX_TEAMS) {
				speed = TeamSpeed[foot->Group];
				maxspeed = TeamMaxSpeed[foot->Group];
			}
		}
		Queue_Mission(TargetClass(this), mission, target, destination, speed, maxspeed);
	} else {

		/*
		**	Cooerce the movement mission into a queued movement mission if the ALT key was
		**	held down.
		*/
		//
		// MBL 04.14.2020 Original code KO, since is still active and can still hit
		//
		// if (mission == MISSION_MOVE && (Keyboard->Down(Options.KeyQueueMove1) || Keyboard->Down(Options.KeyQueueMove2))) {
		// 	mission = MISSION_QMOVE;
		// }

		//
		// MBL 04.14.2020 - Apply the same logic as above, using what is assigned as hotkeys
		//
		if (PlayerPtr && House) {
			if (PlayerPtr == House) {
				if (mission == MISSION_MOVE && PlayerPtr->IsQueuedMovementToggle) {
					mission = MISSION_QMOVE;
				}
			}
		}

		Queue_Mission(TargetClass(this), mission, target, destination);
	}
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
ActionType TechnoClass::What_Action(ObjectClass const * object) const
{
	assert(IsActive);

	if (object != NULL) {

		/*
		**	Return the ACTION_SELF flag if clicking on itself. However, if this
		**	object cannot do anything special with itself, then just return with
		**	the no action flag.
		*/
		if (object == this && CurrentObject.Count() == 1 && House->IsPlayerControl) {
			return(ACTION_SELF);
		}

		//bool altdown = (Keyboard->Down(Options.KeyForceMove1) || Keyboard->Down(Options.KeyForceMove2));
		//bool ctrldown = (Keyboard->Down(Options.KeyForceAttack1) || Keyboard->Down(Options.KeyForceAttack2));
		//bool shiftdown = (Keyboard->Down(Options.KeySelect1) || Keyboard->Down(Options.KeySelect2));
		//Added for getting the input for special character keys from the client 
		// - 6/26/2019 JAS 
		bool altdown = DLL_Export_Get_Input_Key_State(KN_LALT);
		bool ctrldown = DLL_Export_Get_Input_Key_State(KN_LCTRL);
		bool shiftdown = DLL_Export_Get_Input_Key_State(KN_LSHIFT);

		/*
		**	Special guard area mission is possible if both the control and the
		**	alt keys are held down.
		*/
		if (House->IsPlayerControl && ctrldown && altdown && Can_Player_Move() /*KO && Can_Player_Fire()*/) {
//		if (House->IsPlayerControl && ctrldown && altdown && Can_Player_Move() && Can_Player_Fire()) {
			return(ACTION_GUARD_AREA);
		}

		/*
		**	Special override to force a move regardless of what is occupying the location.
		*/
		if (altdown) {
			if (House->IsPlayerControl && Can_Player_Move()) {
				return(ACTION_MOVE);
			}
		}

		/*
		**	Override so that toggled select state can be performed while the <SHIFT> key
		**	is held down.
		*/
		bool is_a_loaner = object->Is_Techno() && ((TechnoClass*)object)->IsALoaner;
		if (shiftdown) {
			if (!is_a_loaner) {
				return(ACTION_TOGGLE_SELECT);
			}
		}

		/*
		**	If the weapon is blatantly disallowed from firing on the object specified, then
		**	don't allow the attach check logic to proceed.
		*/
		TechnoTypeClass const * ttype = Techno_Type_Class();
		if (Is_Weapon_Equipped() &&
				ttype->PrimaryWeapon->Bullet != NULL &&
				ttype->PrimaryWeapon->Bullet->IsSubSurface &&
				Map[object->Target_Coord()].Land_Type() != LAND_WATER) {

			// Do nothing.

		} else {

			/*
			**	If firing is possible and legal, then return this action potential.
			*/
			if (House->IsPlayerControl && (ctrldown || !House->Is_Ally(object)) && (ctrldown || object->Class_Of().IsLegalTarget || (Rule.IsTreeTarget && object->What_Am_I() == RTTI_TERRAIN))) {

				if (Is_Weapon_Equipped() ||
						(What_Am_I() == RTTI_INFANTRY &&
						(((InfantryTypeClass const *)ttype)->IsBomber ||
						((InfantryTypeClass const *)ttype)->IsCapture)
						)) {
					// Check for anti-air capability
					int primary = What_Weapon_Should_I_Use(object->As_Target());
					WeaponTypeClass const * weapon = (primary == 0) ? Techno_Type_Class()->PrimaryWeapon : Techno_Type_Class()->SecondaryWeapon;
					if ((object->What_Am_I() != RTTI_AIRCRAFT) ||
						((weapon != NULL) && weapon->Bullet->IsAntiAircraft) ||
						(object->Is_Techno() && (((TechnoClass *)object)->Height == 0))) {
						if (Can_Player_Move() || In_Range(object, primary)) {
							if (In_Range(object, primary) || (What_Am_I() == RTTI_INFANTRY && ((InfantryClass *)this)->Class->IsCapture && object->What_Am_I() == RTTI_BUILDING && ((BuildingClass *)object)->Class->IsCaptureable)) {
								return(ACTION_ATTACK);
							} else {
								if (!Can_Player_Move()) {
									return(ACTION_NONE);
								} else {
									return(ACTION_ATTACK);
								}
							}
						}
					}
				}
			}
		}

		/*
		**	Possibly try to select the specified object, if that is warranted.
		*/
		if (!Is_Weapon_Equipped() || !House->IsPlayerControl || object->Owner() == Owner()) {
			if ((!is_a_loaner || !Is_Owned_By_Player()) && object->Class_Of().IsSelectable && (!object->Is_Selected_By_Player() || CurrentObject.Count())) {
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
 *   07/10/1995 JLB : Force fire for buildings is explicitly disabled.                         *
 *=============================================================================================*/
ActionType TechnoClass::What_Action(CELL cell) const
{
	assert(IsActive);

	CellClass const * cellptr = &Map[cell];
	OverlayTypeClass const * optr = NULL;

	//bool altdown = (Keyboard->Down(Options.KeyForceMove1) || Keyboard->Down(Options.KeyForceMove2));
	//bool ctrldown = (Keyboard->Down(Options.KeyForceAttack1) || Keyboard->Down(Options.KeyForceAttack2));
	//bool shiftdown = (Keyboard->Down(Options.KeySelect1) || Keyboard->Down(Options.KeySelect2));
	//Added for getting the input for special character keys from the client 
	// - 6/26/2019 JAS 
	bool altdown = DLL_Export_Get_Input_Key_State(KN_LALT);
	bool ctrldown = DLL_Export_Get_Input_Key_State(KN_LCTRL);
	bool shiftdown = DLL_Export_Get_Input_Key_State(KN_LSHIFT);

	/*
	**	Disable recognizing the <CTRL> key forced fire option when dealing with buildings.
	*/
	if (What_Am_I() == RTTI_BUILDING) ctrldown = false;

	/*
	**	Disable recognizing the <CTRL> key forced fire option when dealing with submarines.
	*/
	if(What_Am_I() == RTTI_VESSEL) {
		WeaponTypeClass const * weapon = ((VesselClass *)this)->Class->PrimaryWeapon;
		if (weapon && weapon->Bullet->IsSubSurface) ctrldown = false;
	}


	if (cellptr->Overlay != OVERLAY_NONE) {
		optr = &OverlayTypeClass::As_Reference(cellptr->Overlay);
	}

	/*
	**	Special guard area mission is possible if both the control and the
	**	alt keys are held down.
	*/
	if (House->IsPlayerControl && ctrldown && altdown && Can_Player_Move() && Can_Player_Fire()) {
		return(ACTION_GUARD_AREA);
	}

	if (House->IsPlayerControl && Techno_Type_Class()->PrimaryWeapon != NULL && (ctrldown || (optr && optr->IsLegalTarget))) {
		WarheadTypeClass const * whead = Techno_Type_Class()->PrimaryWeapon->WarheadPtr;

// To be fixed for firing on ore by accounting for ore and ignoring the overlay in that case.

		if (optr == NULL || (optr->IsWall && (whead->IsWallDestroyer || (whead->IsWoodDestroyer && optr->IsWooden)))) {
			int primary = What_Weapon_Should_I_Use(::As_Target(cell));
			if (Can_Player_Move() || In_Range(::As_Target(cell), primary)) {
				return(ACTION_ATTACK);
			}
		}
	}

	if (House->IsPlayerControl && Can_Player_Move()) {

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
	assert(IsActive);

	return(House->IsPlayerControl);
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
	assert(IsActive);

	if (House->IsPlayerControl && Is_Techno() && Techno_Type_Class()->PrimaryWeapon != NULL) {
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TechnoClass::Is_Weapon_Equipped -- Determines if this object has a combat weapon.           *
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
	assert(IsActive);

	return(Techno_Type_Class()->PrimaryWeapon != NULL);
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
	assert(IsActive);

	/*
	**	Temporary hack to disable repair cursor over non-buildings.
	*/
	if (What_Am_I() != RTTI_BUILDING) {
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
	assert(IsActive);
	assert((unsigned)which < 2);

	WeaponTypeClass const * weapon = NULL;
	TechnoTypeClass const & ttype = *Techno_Type_Class();

	switch (which) {
		case 0:
			weapon = ttype.PrimaryWeapon;
			break;

		case 1:
			weapon = ttype.SecondaryWeapon;
			break;
	}
	if (weapon != NULL) {
		return(weapon->Range);
	}
	return(0);
}


/***************************************************************************
 * TechnoClass::Override_Mission -- temporarily overrides a units mission  *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:      MissionType mission - the mission we want to override       *
 *               TARGET      tarcom  - the new target we want to override  *
 *               TARGET      navcom  - the new navigation point to override*
 *                                                                         *
 * OUTPUT:      none                                                       *
 *                                                                         *
 * WARNINGS:   If a mission is already overridden, the current mission is  *
 *               just re-assigned.                                         *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/28/1995 PWG : Created.                                             *
 *=========================================================================*/
void TechnoClass::Override_Mission(MissionType mission, TARGET tarcom, TARGET navcom)
{
	assert(IsActive);

	SuspendedTarCom = TarCom;
	RadioClass::Override_Mission(mission, tarcom, navcom);
	Assign_Target(tarcom);
}


/***************************************************************************
 * TechnoClass::Restore_Mission -- Restores an overridden mission          *
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
	assert(IsActive);

	if (RadioClass::Restore_Mission()) {
		Assign_Target(SuspendedTarCom);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TechnoClass::Renovate -- Heal a building to maximum                                         *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/15/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Renovate(void)
{
	assert(IsActive);

	Mark(MARK_CHANGE);
	Strength = Techno_Type_Class()->MaxStrength;
	if (What_Am_I() == RTTI_BUILDING) {
		((BuildingClass *)this)->Repair(0);
	}
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
 *   09/29/1995 JLB : Keeps track of quantity records.                                         *
 *=============================================================================================*/
bool TechnoClass::Captured(HouseClass * newowner)
{
	assert(IsActive);

	if (newowner != House) {

		/*
		**	Capture attempt springs any "entered" trigger. The entered trigger
		**	occurs first since there may be a special trigger attached to this
		**	object that flags a capture as a win and a destroy as a loss. This
		**	order is necessary because the object is recorded as a kill as well.
		*/
		if (Trigger.Is_Valid()) {
			Trigger->Spring(TEVENT_PLAYER_ENTERED, this);
		}

		/*
		**	Record this as a kill.
		*/
		Record_The_Kill(NULL);

		/*
		**	Special kill record logic for capture process.
		*/
		House->Tracking_Remove(this);
		newowner->Tracking_Add(this);
		switch (What_Am_I()) {
			case RTTI_BUILDING:
				newowner->BuildingsKilled[Owner()]++;
				break;

			case RTTI_AIRCRAFT:
			case RTTI_INFANTRY:
			case RTTI_UNIT:
			case RTTI_VESSEL:
				newowner->UnitsKilled[Owner()]++;
				break;

			default:
				break;
		}
		House->WhoLastHurtMe = newowner->Class->House;

		/*
		**	Remove from targeting computers.
		*/
		Detach_All(false);

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
ResultType TechnoClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source, bool forced)
{
	assert(IsActive);

	ResultType result = RESULT_NONE;

	/*
	**	If not a forced damage condition, adjust damage according to house override armor
	**	value.
	*/
	if (!forced && damage > 0) {
		damage = damage * ArmorBias * House->ArmorBias;
	}

	if (IronCurtainCountDown == 0) {
		result = ObjectClass::Take_Damage(damage, distance, warhead, source, forced);
	}

	switch (result) {
		case RESULT_DESTROYED:
			Transmit_Message(RADIO_OVER_OUT);
			Stun();

			/*
			**	If this object explodes with violent damage, then perform the explosion
			**	now and use the warhead type and full strength as the explosion values.
			*/
			if (Techno_Type_Class()->IsExploding) {

				/*
				**	The warhead to use is based on the weapon this object is equipped with.
				*/
				WarheadType wh = WARHEAD_HE;
				if (Techno_Type_Class()->PrimaryWeapon != NULL) {
					wh = WarheadType(Techno_Type_Class()->PrimaryWeapon->WarheadPtr->ID);
				}

				int damage = Techno_Type_Class()->MaxStrength;
				new AnimClass(Combat_Anim(damage, wh, Map[Center_Coord()].Land_Type()), Center_Coord());
				int radius = damage * Rule.ExplosionSpread;
//				int radius = damage/2;
				Wide_Area_Damage(Center_Coord(), radius, damage, source, wh);
			}

			if (this == (TechnoClass *)::As_Object(House->UnitToTeleport)) {
				House->UnitToTeleport = 0;
				if (!Scen.IsFadingColor) {
					Scen.IsFadingBW = false;
					Scen.IsFadingColor = true;
					Scen.FadeTimer = GRAYFADETIME;
				}
				if (Map.IsTargettingMode == SPC_CHRONO2) {
					KeyNumType input = KN_RMOUSE;
					Map.AI(input, 0, 0);
				}
			}

			/*
			** May trigger an achievement. ST - 11/14/2019 1:56PM
			*/
			if (Session.Type == GAME_NORMAL && !House->IsHuman && source && source->House && source->House->IsHuman) {
				TechnoTypeClass const *object_type = Techno_Type_Class();
				if (object_type) {
					
					RTTIType what = What_Am_I();
					if (what == RTTI_AIRCRAFT || what == RTTI_INFANTRY || what == RTTI_UNIT || what == RTTI_VESSEL) {
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
			if (source != NULL && !House->Is_Ally(source)) {
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
	assert(IsActive);

	int total_recorded = 0;

	int points = Techno_Type_Class()->Points;

	/*
	**	Handle any trigger event associated with this object.
	*/
	if (Trigger.Is_Valid() && source) Trigger->Spring(TEVENT_ATTACKED, this);

	if (Trigger.Is_Valid() && source) Trigger->Spring(TEVENT_DISCOVERED, this);

	if (Trigger.Is_Valid()) Trigger->Spring(TEVENT_DESTROYED, this);

	if (source != NULL) {
		Crew.Made_A_Kill();

		House->WhoLastHurtMe = source->Owner();

		/*
		** Add up the score for killing this unit
		*/
		source->House->PointTotal += points;
	}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
// Hack check: if they were trying to teleport this unit when it died, take
//	the map mode out of teleportation mode.
	if(IsOwnedByPlayer && Map.IsTargettingMode == SPC_CHRONO2 && House->UnitToTeleport == As_Target()) {
		Map.IsTargettingMode = SPC_NONE;
	}
#endif
	switch (What_Am_I()) {
		case RTTI_BUILDING:
			{
				StructType bldg = *(BuildingClass *)this;
				if (bldg != STRUCT_BARREL && bldg != STRUCT_BARREL3 &&
					 bldg != STRUCT_APMINE && bldg != STRUCT_AVMINE) {
					if (((BuildingClass *)this)->WhoLastHurtMe != HOUSE_NONE) {
						House->BuildingsLost++;
					}

					if (source != NULL) {
						if (Session.Type == GAME_INTERNET) {
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
				}
			}
			break;

		case RTTI_AIRCRAFT:
			if (source != NULL && Session.Type == GAME_INTERNET) {
				source->House->DestroyedAircraft->Increment_Unit_Total( ((AircraftClass*)this)->Class->Type );
				total_recorded++;
			}
			//Fall through.....
		case RTTI_INFANTRY:
			if (source != NULL && !total_recorded && Session.Type == GAME_INTERNET) {
				source->House->DestroyedInfantry->Increment_Unit_Total( ((InfantryClass*)this)->Class->Type );
				total_recorded++;
			}
			//Fall through.....
		case RTTI_UNIT:
			if (source != NULL && !total_recorded && Session.Type == GAME_INTERNET) {
				source->House->DestroyedUnits->Increment_Unit_Total( ((UnitClass*)this)->Class->Type );
				total_recorded++;
			}
			//Fall through.....
		case RTTI_VESSEL:
			if (source != NULL && !total_recorded && Session.Type == GAME_INTERNET) {
				source->House->DestroyedUnits->Increment_Unit_Total( ((VesselClass*)this)->Class->Type );
			}

			House->UnitsLost++;
			if (source != NULL) source->House->UnitsKilled[Owner()]++;

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

	/*
	** Since we lost an object, we lose the associated points as well.
	*/
	House->PointTotal -= points;
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
 *   09/28/1995 JLB : Uses map scan function.                                                  *
 *=============================================================================================*/
CELL TechnoClass::Nearby_Location(TechnoClass const * techno) const
{
	assert(IsActive);

	SpeedType speed = Techno_Type_Class()->Speed;
	if (speed == SPEED_WINGED) {
		speed = SPEED_TRACK;
	}

	CELL cell = 0;
	if (techno != NULL) {
		cell = Coord_Cell(techno->Center_Coord());
	} else {
		cell = Coord_Cell(Center_Coord());
	}

	return(Map.Nearby_Location(cell, speed, Map[cell].Zones[Techno_Type_Class()->MZone], Techno_Type_Class()->MZone));
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
	assert(IsActive);

	if (IsCloakable && (Cloak == CLOAKED || Cloak == CLOAKING)) {
		if (Cloak == CLOAKED) {
			Map.RadarClass::Flag_To_Redraw(true);
		}
		Cloak = UNCLOAKING;
		CloakingDevice.Set_Stage(0);
		CloakingDevice.Set_Rate(1);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		if(What_Am_I() == RTTI_VESSEL) {
			Sound_Effect(VOC_SUBSHOW, Coord);
		} else {
			Sound_Effect(VOC_IRON1, Coord);
		}
#else
		Sound_Effect(VOC_SUBSHOW, Coord);
#endif
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
	assert(IsActive);

	if (IsCloakable && (Cloak == UNCLOAKED || Cloak == UNCLOAKING)) {
		Detach_All(false);

		if (Cloak == UNCLOAKED) {
			Map.RadarClass::Flag_To_Redraw(true);
		}

		Cloak = CLOAKING;
		CloakingDevice.Set_Stage(0);
		CloakingDevice.Set_Rate(1);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		if(What_Am_I() == RTTI_VESSEL) {
			Sound_Effect(VOC_SUBSHOW, Coord);
		} else {
			Sound_Effect(VOC_IRON1, Coord);
		}
#else
		Sound_Effect(VOC_SUBSHOW, Coord);
#endif
	}
}


/***********************************************************************************************
 * TechnoClass::Do_Shimmer -- Causes this object to shimmer if it is cloaked.                  *
 *                                                                                             *
 *    This routine is called when this object should shimmer. If the object is cloaked, then   *
 *    a shimmering effect (partial decloak) occurs. For objects that are not cloaked, no       *
 *    effect occurs.                                                                           *
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
	assert(IsActive);
#if(0)
	if (IsCloakable && Cloak == CLOAKED) {
		Cloak = CLOAKING;
		CloakingDevice.Set_Stage(MAX_UNCLOAK_STAGE/2);
		CloakingDevice.Set_Rate(1);
	}
#else
	Do_Uncloak();
#endif
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
 *   05/27/1996 JLB : Knows about invisible objects.                                           *
 *=============================================================================================*/
VisualType TechnoClass::Visual_Character(bool raw) const
{
	assert(IsActive);

	if (Techno_Type_Class()->IsInvisible) {
		if ((Session.Type != GAME_NORMAL) || Is_Owned_By_Player()) return(VISUAL_NORMAL);
		if (!Debug_Map) return(VISUAL_HIDDEN);
	}

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

	stage = fixed(stage, MAX_UNCLOAK_STAGE) * 256;

	if (stage < 0x0040) return(VISUAL_INDISTINCT);
	if (stage < 0x0080) return(VISUAL_DARKEN);
	if (stage < 0x00C0) return(VISUAL_SHADOWY);
	//if (!raw && IsOwnedByPlayer) return(VISUAL_SHADOWY);
	if (!raw && Is_Owned_By_Player()) return(VISUAL_SHADOWY);		// Changed for multiplayer. Not needed except to test in the old renderer. ST - 3/13/2019 5:56PM
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
 *          rotation    -- The rotation of the object.                                         *
 *                                                                                             *
 *          scale       -- The scaling factor to use (24.8 fixed point).                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *   01/11/1996 JLB : Added rotation and scaling.                                              *
 *=============================================================================================*/
void TechnoClass::Techno_Draw_Object(void const * shapefile, int shapenum, int x, int y, WindowNumberType window, DirType rotation, int scale) const
{
	assert(IsActive);

	if (rotation != DIR_N || scale != 0x0100) {
		Disable_Uncompressed_Shapes();
	}

	if (shapefile != NULL) {
		VisualType visual = Visual_Character();
		void const * remap = Remap_Table();
		void const * shadow = Map.UnitShadow;

#ifdef PARTIAL
		/*
		**	Create a minimum shape rectangle if one hasn't already been
		**	calculated and the shape file matches the one that the
		**	class thinks it should be using. This check is necessary because
		**	the dimension rectangle pointer is referenced from the type class
		**	object on the presumption that the shapefile pointer passed to this
		**	routine matches. If it doesn't match, then the wrong rectangle information
		**	will be stored into the type class object.
		*/
		TechnoTypeClass * ttype = Techno_Type_Class();
		if (shapefile == ttype->Get_Image_Data() && shapenum < Get_Build_Frame_Count(shapefile)-1) {
			if (ttype->DimensionData == NULL) {
				ttype->DimensionData = new Rect [Get_Build_Frame_Count(shapefile)];
			}
			if (ttype->DimensionData != NULL && !ttype->DimensionData[shapenum].Is_Valid()) {
				ttype->DimensionData[shapenum] = Shape_Dimensions(shapefile, shapenum);
			}
		}
#endif
		if (Height > 0) {
			shadow = Map.UnitShadowAir;
		}

		y -= Lepton_To_Pixel(Height);

		/*
		** If they're viewing a spy, and the spy belongs to some other house,
		** make it look like an infantryman from our house
		*/
		if (What_Am_I() == RTTI_INFANTRY) {
			if (!IsOwnedByPlayer) {
				if (*(InfantryClass *)this == INFANTRY_SPY) remap = PlayerPtr->Remap_Table();
			}
			if (((InfantryClass *)this)->Class->IsRemapOverride) {
				remap = ((InfantryClass *)this)->Class->OverrideRemap;
			}
		}

		/*
		** Check for the special visual effect for the iron curtain
		*/
		if (IronCurtainCountDown > 0) {
//			remap = RemapEmber;
			remap = DisplayClass::FadingRed;
		}

#ifdef PREDATOR
		if (visual != VISUAL_HIDDEN && visual != VISUAL_RIPPLE) {
			if (visual == VISUAL_SHADOWY) {
				CC_Draw_Shape(shapefile, shapenum, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING|SHAPE_PREDATOR, NULL, Map.FadingShade, rotation, scale);
			} else {
				CC_Draw_Shape(shapefile, shapenum, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING|SHAPE_GHOST, remap, shadow, rotation, scale);
			}
			if (visual == VISUAL_DARKEN) {
				CC_Draw_Shape(shapefile, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING, remap, Map.FadingShade, rotation, scale);
			}
		}
		if (visual != VISUAL_NORMAL && visual != VISUAL_HIDDEN) {
			CC_Draw_Shape(shapefile, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL, NULL, NULL, rotation, scale);
		}
#else
		switch (visual) {
			case VISUAL_NORMAL:
				// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
				CC_Draw_Shape(this, shapefile, shapenum, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING|SHAPE_GHOST, remap, shadow, rotation, scale);
				break;

			case VISUAL_INDISTINCT:
			case VISUAL_DARKEN:
				// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
				CC_Draw_Shape(this, shapefile, shapenum, x, y, window, SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL, remap, Map.FadingShade, rotation, scale);
				break;

			case VISUAL_SHADOWY:
			case VISUAL_RIPPLE:
				// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
				CC_Draw_Shape(this, shapefile, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL, NULL, Map.FadingShade, rotation, scale);
				break;

			case VISUAL_HIDDEN:
				// Server still needs to "render" hidden objects to the virtual window, so objects get created properly - SKY
				if (window == WINDOW_VIRTUAL) {
					CC_Draw_Shape(this, shapefile, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL, NULL, Map.FadingShade, rotation, scale);
				}
				break;
		}
#endif
	}

	Enable_Uncompressed_Shapes();
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
 *          rotation    -- The rotation of the object.                                         *
 *                                                                                             *
 *          scale       -- The scaling factor to use (24.8 fixed point).                       *
 *                                                                                             *
 *          shape_name  -- The name of the shapefile                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   8/1/2019 5:32PM - ST : Created.                                                           *
 *=============================================================================================*/
void TechnoClass::Techno_Draw_Object_Virtual(void const * shapefile, int shapenum, int x, int y, WindowNumberType window, DirType rotation, int scale, const char *shape_name) const
{
	assert(IsActive);

	if (shape_name == NULL || *shape_name == 0) {
		/*
		** If there's no override shape name, then call the regular draw
		*/
		Techno_Draw_Object(shapefile, shapenum, x, y, window, rotation, scale);
		return;
	}

	if (shapefile != NULL) {
		VisualType visual = Visual_Character();
		void const * remap = Remap_Table();
		void const * shadow = Map.UnitShadow;

#ifdef PARTIAL
		/*
		**	Create a minimum shape rectangle if one hasn't already been
		**	calculated and the shape file matches the one that the
		**	class thinks it should be using. This check is necessary because
		**	the dimension rectangle pointer is referenced from the type class
		**	object on the presumption that the shapefile pointer passed to this
		**	routine matches. If it doesn't match, then the wrong rectangle information
		**	will be stored into the type class object.
		*/
		TechnoTypeClass * ttype = Techno_Type_Class();
		if (shapefile == ttype->Get_Image_Data() && shapenum < Get_Build_Frame_Count(shapefile)-1) {
			if (ttype->DimensionData == NULL) {
				ttype->DimensionData = new Rect [Get_Build_Frame_Count(shapefile)];
			}
			if (ttype->DimensionData != NULL && !ttype->DimensionData[shapenum].Is_Valid()) {
				ttype->DimensionData[shapenum] = Shape_Dimensions(shapefile, shapenum);
			}
		}
#endif

		if (Height > 0) {
			shadow = Map.UnitShadowAir;
		}

		y -= Lepton_To_Pixel(Height);

		/*
		** If they're viewing a spy, and the spy belongs to some other house,
		** make it look like an infantryman from our house
		*/
		if (What_Am_I() == RTTI_INFANTRY) {
			if (!IsOwnedByPlayer) {
				if (*(InfantryClass *)this == INFANTRY_SPY) remap = PlayerPtr->Remap_Table();
			}
			if (((InfantryClass *)this)->Class->IsRemapOverride) {
				remap = ((InfantryClass *)this)->Class->OverrideRemap;
			}
		}

		/*
		** Check for the special visual effect for the iron curtain
		*/
		if (IronCurtainCountDown > 0) {
//			remap = RemapEmber;
			remap = DisplayClass::FadingRed;
		}

#ifdef PREDATOR
		if (visual != VISUAL_HIDDEN && visual != VISUAL_RIPPLE) {
			if (visual == VISUAL_SHADOWY) {
				CC_Draw_Shape(shapefile, shapenum, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING|SHAPE_PREDATOR, NULL, Map.FadingShade, rotation, scale);
			} else {
				CC_Draw_Shape(shapefile, shapenum, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING|SHAPE_GHOST, remap, shadow, rotation, scale);
			}
			if (visual == VISUAL_DARKEN) {
				CC_Draw_Shape(shapefile, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING, remap, Map.FadingShade, rotation, scale);
			}
		}
		if (visual != VISUAL_NORMAL && visual != VISUAL_HIDDEN) {
			CC_Draw_Shape(shapefile, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL, NULL, NULL, rotation, scale);
		}
#else
		switch (visual) {
			case VISUAL_NORMAL:
				// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
				CC_Draw_Shape(this, shape_name, shapefile, shapenum, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING|SHAPE_GHOST, remap, shadow, rotation, scale);
				break;

			case VISUAL_INDISTINCT:
			case VISUAL_DARKEN:
				// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
				CC_Draw_Shape(this, shape_name, shapefile, shapenum, x, y, window, SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL, remap, Map.FadingShade, rotation, scale);
				break;

			case VISUAL_SHADOWY:
			case VISUAL_RIPPLE:
				// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
				CC_Draw_Shape(this, shape_name, shapefile, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL, NULL, Map.FadingShade, rotation, scale);
				break;

			case VISUAL_HIDDEN:
				// Server still needs to "render" hidden objects to the virtual window, so objects get created properly - SKY
				if (window == WINDOW_VIRTUAL) {
					CC_Draw_Shape(this, shape_name, shapefile, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL, NULL, Map.FadingShade, rotation, scale);
				}
				break;
		}
#endif
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
void const * TechnoClass::Remap_Table(void) const
{
	assert(IsActive);

	if (Techno_Type_Class()->IsRemappable) {
		return(House->Remap_Table(IsBlushing, Techno_Type_Class()->Remap));
	}
	return(ColorRemaps[PCOLOR_GOLD].RemapTable);
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
	assert(IsActive);
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
	assert(IsActive);

	while (Is_Something_Attached()) {
		FootClass * foot = Detach_Object();
		if (foot != NULL) {
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
	assert(IsActive);

	/*
	**	If this object contains no crew, then there can be no
	**	crew inside, duh... return this news.
	*/
	if (!Techno_Type_Class()->IsCrew) {
		return(INFANTRY_NONE);
	}

	/*
	**	The normal infantry survivor is the standard issue
	**	minigunner. Certain buildings, especially neutral ones, tend to have
	**	civilians exit them instead.
	*/
	InfantryType infantry = INFANTRY_E1;
	if (House->ActLike == HOUSE_NEUTRAL) {
		infantry = Random_Pick(INFANTRY_C1, INFANTRY_C9);
	} else {
		if (Techno_Type_Class()->PrimaryWeapon == NULL && Percent_Chance(15)) {
			if (Percent_Chance(50)) {
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
	assert(IsActive);

	int value = 0;

	/*
	**	In early missions, contents of transports are not figured
	**	into the total value.
	*/
	if (Rule.Diff[House->Difficulty].IsContentScan || House->IQ >= Rule.IQContentScan) {
		if (Is_Something_Attached()) {
			FootClass * object = Attached_Object();

			while (object != NULL) {
				value += object->Value();
				object = (FootClass *)(ObjectClass *)object->Next;
			}
		}
	}

#ifdef TOFIX
	/*
	**	Increase the value of power producing object when there is power critical
	**	defensive structures.
	*/
	if (What_Am_I() == RTTI_BUILDING && ((BuildingClass *)this)->Class->Power) {
		if (House->BScan & (STRUCTF_ATOWER|STRUCTF_OBELISK)) {
			value += Techno_Type_Class()->Reward;
		}
	}
#endif

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
	assert(IsActive);

	/*
	**	Threat range means nothing if scanning the whole map. In such a case, just
	**	return with the same control flag specified.
	*/
	if (control == -1) return(-1);

	/*
	**	If simple guard range is requested, then return "0" since
	**	this is a special control value that is calculated as the object's
	**	weapon range.
	*/
	if (control == 0) {
		/*
		**	For normal guard mode or for area guard mode, use the override
		**	threat range value as specified by the object's type class.
		*/
		if (Techno_Type_Class()->ThreatRange != 0) {
			return(Techno_Type_Class()->ThreatRange);
		}
		return(0);
	}

	/*
	**	Area guard range is specified, so figure twice the weapon range of the
	**	longest range weapon this object is equipped with.
	*/
	int range = Techno_Type_Class()->ThreatRange;
	if (range == 0) {
		range = max(Weapon_Range(0), Weapon_Range(1));
	}

	range *= 2;
	range = Bound(range, 0x0000, 0x0A00);

	return(range);
}


/***********************************************************************************************
 * TechnoClass::Is_In_Same_Zone -- Determine if specified cell is in same zone as object.      *
 *                                                                                             *
 *    This will examine the specified cell to determine if it is in the same zone as this      *
 *    object's location.                                                                       *
 *                                                                                             *
 * INPUT:   cell  -- The cell that is to be checked against this object's current location.    *
 *                                                                                             *
 * OUTPUT:  bool; Is the specified cell in the same zone as this object is?                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_In_Same_Zone(CELL cell) const
{
	MZoneType zone = Techno_Type_Class()->MZone;
	return(Map[cell].Zones[zone] == Map[Center_Coord()].Zones[zone]);
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
 *   10/15/1996 JLB : Alternates between guard area and attack.                                *
 *   11/01/1996 JLB : Allow recruit of guard area units in multiplay.                          *
 *=============================================================================================*/
void TechnoClass::Base_Is_Attacked(TechnoClass const * enemy)
{
	assert(IsActive);

	FootClass * defender[6];
	memset(defender, '\0', sizeof(defender));

	int value[ARRAY_SIZE(defender)];
	memset(value, '\0', sizeof(value));

	int count = 0;
	int weakest = 0;
	int desired = enemy->Risk() * House->Control.TechLevel;
	int risktotal = 0;
	int zone = Map[Center_Coord()].Zones[Techno_Type_Class()->MZone];

	/*
	** Humans have to deal with their own base is attacked problems.
	*/
	if (enemy == NULL || House->Is_Ally(enemy) || House->IsHuman) {
		return;
	}

	/*
	**	Don't overreact if this building can defend itself.
	*/
	if (Session.Type == GAME_NORMAL && Techno_Type_Class()->PrimaryWeapon != NULL) return;

	/*
	** If the enemy is not an infantry or a unit there is not much we can
	** do about it.
	*/
	if (enemy->What_Am_I() != RTTI_INFANTRY && enemy->What_Am_I() != RTTI_UNIT) {
		return;
	}

	/*
	** If we are a certain type of building, such as a barrel or land mine,
	** ignore the attack.
	*/
	if (Techno_Type_Class()->IsInsignificant) {
		return;
	}

	/*
	** If the threat has already been dealt with then we don't need to do
	** any work. Check for that here.
	*/
	if (enemy->Is_Foot() && ((FootClass *)enemy)->BaseAttackTimer != 0) {
		return;
	}

	/*
	** We will need units to defend our base.  We need to suspend teams until
	** the situation has been dealt with.
	*/
	TeamClass::Suspend_Teams(Rule.SuspendPriority, House);

	/*
	** Loop through the infantry looking for those who are capable of going
	** on a rescue mission.
	*/
	for (int index = 0; index < Infantry.Count() && desired > 0; index++) {
	 	InfantryClass * infantry = Infantry.Ptr(index);
		if (infantry != NULL && infantry->Owner() == Owner()) {

			/*
			**	Never recruit sticky guard units to defend a base.
			*/
			if (!infantry->Is_Weapon_Equipped() ||
					(!MissionControl[infantry->Mission].IsRecruitable && Session.Type == GAME_NORMAL)) continue;
//					(Mission != MISSION_GUARD_AREA || Session.Type == GAME_NORMAL)) continue;

			/*
			**	Don't allow a response if it doesn't have a weapon that will affect the
			**	enemy object.
			*/
			if (infantry->Class->PrimaryWeapon->WarheadPtr->Modifier[enemy->Techno_Type_Class()->Armor] == 0) {
				continue;
			}

			/*
			**	Don't try to help if the building is on another planet.
			*/
			if (Map[infantry->Center_Coord()].Zones[infantry->Class->MZone] != Map[Center_Coord()].Zones[infantry->Class->MZone]) continue;

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
			**	Greatly increase the threat value if this unit is already assigned to protect
			**	the target.
			*/
			if (ArchiveTarget == As_Target()) {
				threat *= 100;
			}

			/*
			** If the value returned is negative then this unit is already
			** assigned to fighting the enemy, so subtract its value from
			** the enemy's desired value.
			*/
			if (threat < 0) {
				desired += threat;
				continue;
			}

			if (count < ARRAY_SIZE(defender)) {
				defender[count] = infantry;
				value[count] = threat;
				count++;
				continue;
			}

			if (threat > weakest) {
				int newweakest = threat;

				for (int lp = 0; lp < count; lp++) {
					if (value[lp] == weakest) {
						value[lp] = threat;
						defender[lp] = (FootClass *) infantry;
						continue;
					}
					if (value[lp] < newweakest) {
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
	for (int index = 0; index < Units.Count() && desired > 0; index++) {
	 	UnitClass * unit = Units.Ptr(index);
		if (unit != NULL && unit->Owner() == Owner()) {

			/*
			**	Never recruit sticky guard units to defend a base.
			*/
			if (!unit->Is_Weapon_Equipped() ||
				(!MissionControl[unit->Mission].IsRecruitable && Session.Type == GAME_NORMAL)) continue;

			/*
			**	Don't allow a response if it doesn't have a weapon that will affect the
			**	enemy object.
			*/
			if (unit->Class->PrimaryWeapon->WarheadPtr->Modifier[enemy->Techno_Type_Class()->Armor] == 0) {
				continue;
			}

			/*
			**	Don't try to help if the building is on another planet.
			*/
			if (Map[unit->Center_Coord()].Zones[unit->Class->MZone] != Map[Center_Coord()].Zones[unit->Class->MZone]) continue;

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
			**	Greatly increase the threat value if this unit is already assigned to protect
			**	the target.
			*/
			if (threat > 0 && ArchiveTarget == As_Target()) {
				threat *= 10;
			}

			/*
			** If the value returned is negative then this unit is already
			** assigned to fighting the enemy, so subtract its value from
			** the enemy's desired value.
			*/
			if (threat < 0) {
				desired += threat;
				continue;
			}

			if (count < ARRAY_SIZE(defender)) {
				defender[count] = unit;
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
					if (value[lp] < newweakest) {
//					if (value[count] < newweakest) {
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
		for (int lp = 0; lp < count - 1; lp ++) {
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

		for (int lp = 0; lp < count; lp ++) {
			if (Percent_Chance(50)) {
				defender[lp]->Assign_Mission(MISSION_RESCUE);
			} else {
				defender[lp]->Assign_Mission(MISSION_GUARD_AREA);
				defender[lp]->ArchiveTarget = As_Target();
			}
			defender[lp]->Assign_Target(enemy->As_Target());
			risktotal += defender[lp]->Risk();
			if (risktotal > desired) {
				break;
			}
		}
	}

	if (risktotal > desired && enemy->Is_Foot()) {
		((FootClass *)enemy)->BaseAttackTimer = TICKS_PER_MINUTE * Rule.BaseDefenseDelay;
	}
}


/***********************************************************************************************
 * TechnoClass::Is_Allowed_To_Retaliate -- Checks object to see if it can retaliate.           *
 *                                                                                             *
 *    This routine is called when this object has suffered some damage and it needs to know    *
 *    if it should fight back. The object that caused the damage is specifed as a parameter.   *
 *                                                                                             *
 * INPUT:   source   -- The points to the object that was the source of the damage applied     *
 *                      to this object.                                                        *
 *                                                                                             *
 * OUTPUT:  bool; Should retaliation occur?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Allowed_To_Retaliate(TechnoClass const * source) const
{
	/*
	**	If there is no source of the damage, then retaliation cannot occur.
	*/
	if (source == NULL) return(false);

	/*
	**	If the mission precludes retaliation, then don't retaliate.
	*/
	if (!MissionControl[Mission].IsRetaliate) return(false);

	/*
	**	Fixed wing aircraft are not responsive enough to retaliate to damage recieved.
	*/
	if (What_Am_I() == RTTI_AIRCRAFT && ((AircraftClass *)this)->Class->IsFixedWing) {
		return(false);
	}

	/*
	**	If the source of the damage is an ally, then retaliation shouldn't
	**	occur either.
	*/
	if (House->Is_Ally(source)) return(false);

	/*
	**	Only objects that have a damaging weapon are allowed to retaliate.
	*/
	if (Combat_Damage() <= 0 || !Is_Weapon_Equipped()) return(false);

	/*
	**	If this is not equipped with a weapon that can attack the molester, then
	**	don't allow retaliation.
	*/
	TechnoTypeClass const * ttype = Techno_Type_Class();
	if (ttype->PrimaryWeapon->WarheadPtr != NULL &&
		ttype->PrimaryWeapon->WarheadPtr->Modifier[source->Techno_Type_Class()->Armor] == 0) {
			return(false);
	}

	/*
	**	One can never retaliate against a dog because of their peculiar nature of attacking.
	**	Dogs must be attacked using normal target processing.
	*/
	if (source->What_Am_I() == RTTI_INFANTRY && ((InfantryClass *)source)->Class->IsDog) return(false);

	/*
	**	Don't allow retaliation if it isn't equipped with a weapon that can deal with the threat.
	*/
	if (source->What_Am_I() == RTTI_AIRCRAFT && !ttype->PrimaryWeapon->Bullet->IsAntiAircraft) return(false);

	/*
	**	Tanya is not allowed to retaliate against buildings in the normal sense while in guard mode. That
	**	is, unless it is owned by the computer. Normally, Tanya can't do anything substantial to a building
	**	except to blow it up.
	*/
	if ((House->IsHuman || (Session.Type == GAME_NORMAL && House->IsPlayerControl))
			&& source->What_Am_I() == RTTI_BUILDING && What_Am_I() == RTTI_INFANTRY && ((InfantryTypeClass const *)ttype)->IsBomber) {
		return(false);
	}

	/*
	**	If a human house is not allowed to retaliate automatically, then don't
	*/
	if (House->IsHuman && !Rule.IsSmartDefense && (What_Am_I() != RTTI_INFANTRY || *((InfantryClass*)this) != INFANTRY_TANYA || source->What_Am_I() != RTTI_INFANTRY)) return(false);

	/*
	**	If this object is part of a team that prevents retaliation then don't allow retaliation.
	*/
	if (Is_Foot() && ((FootClass *)this)->Team.Is_Valid() && ((FootClass *)this)->Team->Class->IsSuicide) {
		return(false);
	}

	/*
	**	Compare potential threat of the current target and the potential new target. Don't retaliate
	**	if it is currently attacking the greater threat.
	*/
	if (!House->IsHuman && Percent_Chance(50)) {
		fixed source_val = 0;
		int primary = What_Weapon_Should_I_Use(source->As_Target());
		WeaponTypeClass const * weapon = (primary == 0) ? source->Techno_Type_Class()->PrimaryWeapon : source->Techno_Type_Class()->SecondaryWeapon;
		if (weapon != NULL && weapon->WarheadPtr != NULL && In_Range(source, primary)) {
			source_val = weapon->WarheadPtr->Modifier[Techno_Type_Class()->Armor];
		}

		fixed current_val = 0;
		TechnoClass const * tech = As_Techno(TarCom);
		if (tech != NULL) {
			primary = What_Weapon_Should_I_Use(tech->As_Target());
			weapon = (primary == 0) ? source->Techno_Type_Class()->PrimaryWeapon : source->Techno_Type_Class()->SecondaryWeapon;
			if (weapon != NULL && weapon->WarheadPtr != NULL && In_Range(tech, primary)) {
				current_val = weapon->WarheadPtr->Modifier[Techno_Type_Class()->Armor];
			}
		}
		if (source_val <= current_val) return(false);
	}

	/*
	**	If it is already busy attacking another target, then don't retaliate.
	*/
//	if (In_Range(TarCom)) return(false);

	/*
	**	All checks passed, so return that retaliation is allowed.
	*/
	return(true);
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
int TechnoClass::Get_Ownable(void) const
{
	assert(IsActive);

	return ((TechnoTypeClass const &)Class_Of()).Get_Ownable();
//	return ((TechnoTypeClass const &)Class_Of()).Ownable;
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
	assert(IsActive);

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
fixed TechnoClass::Tiberium_Load(void) const
{
	assert(IsActive);

	return(0);
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
	assert(IsActive);

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
	assert(IsActive);

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
	assert(IsActive);

	return(Turret_Facing());
}


/***********************************************************************************************
 * TechnoClass::Response_Select -- Handles the voice response when selected.                   *
 *                                                                                             *
 *    This routine is called when a voice response to a select action is desired. This routine *
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
	assert(IsActive);
}


/***********************************************************************************************
 * TechnoClass::Response_Move -- Handles the voice response to a movement request.             *
 *                                                                                             *
 *    This routine is called when a voice response to a movement order is desired. This        *
 *    routine should be overridden for any object that actually has a voice response.          *
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
void TechnoClass::Response_Move(void)
{
	assert(IsActive);
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
	assert(IsActive);
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
 * OUTPUT:  Was a suitable target acquired and assigned to the TarCom?                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Target_Something_Nearby(ThreatType threat)
{
	assert(IsActive);
	threat = threat & (THREAT_RANGE|THREAT_AREA);

	/*
	**	Determine that if there is an existing target it is still legal
	**	and within range.
	*/
	if (Target_Legal(TarCom)) {
		if ((threat & THREAT_RANGE)) {
			int primary = What_Weapon_Should_I_Use(TarCom);
			if (!In_Range(TarCom, primary)) {
				Assign_Target(TARGET_NONE);
			}
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
	**	Return with answer to question: Does this unit now have a target?
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
	assert(IsActive);

	return(0);
}


/***********************************************************************************************
 * TechnoClass::Is_Ready_To_Random_Animate -- Determines if the object should random animate.  *
 *                                                                                             *
 *    This will examine this object to determine if it is time and ready to perform some       *
 *    kind of random animation.                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is it time to perform an random animation?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Ready_To_Random_Animate(void) const
{
	assert(IsActive);
	return(IdleTimer == 0);
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
	assert(IsActive);
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
	assert(IsActive);
}


/***********************************************************************************************
 * TechnoClass::Draw_Pips -- Draws the transport pips and other techno graphics.               *
 *                                                                                             *
 *    This routine is used to render the small transportation pip (occupant feedback graphic)  *
 *    used for transporter object. It will also display if the techno object is "primary"      *
 *    if necessary.                                                                            *
 *                                                                                             *
 * INPUT:   x,y   -- The pixel coordinate for the center of the first pip. Subsequent pips     *
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
 *   10/06/1995 JLB : Displays the team group number.                                          *
 *   09/10/1996 JLB : Medic hack for red pip.                                                  *
 *=============================================================================================*/
void TechnoClass::Draw_Pips(int x, int y, WindowNumberType window) const
{
	assert(IsActive);

	/*
	**	Transporter type objects have a different graphic representation for the pips. The
	**	pip color represents the type of occupant.
	*/
	bool carrying_passengers = (Techno_Type_Class()->Max_Passengers() > 0) &&
		((What_Am_I() != RTTI_AIRCRAFT) || (*(AircraftClass*)this != AIRCRAFT_BADGER) || (Mission != MISSION_HUNT));
	if (carrying_passengers) {
		ObjectClass const * object = Attached_Object();
		for (int index = 0; index < Class_Of().Max_Pips(); index++) {
			PipEnum pip = PIP_EMPTY;

			if (object != NULL) {
				pip = PIP_FULL;
				if (object->What_Am_I() == RTTI_INFANTRY) {
					pip = ((InfantryClass *)object)->Class->Pip;
				}
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
				if (What_Am_I() == RTTI_VESSEL && *(VesselClass *)this == VESSEL_CARRIER) {
					if (object->What_Am_I() == RTTI_AIRCRAFT) {
						AircraftClass *heli = (AircraftClass *)object;
						if (heli->Ammo != heli->Techno_Type_Class()->MaxAmmo) {
							pip = PIP_ENGINEER;
							if (!heli->Ammo) {
								pip = PIP_COMMANDO;
							}
						}
					}
				}
#endif
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

		/*
		** Check if it's a harvester, to show the right type of pips for the
		** various minerals it could have harvested.
		*/
		if (What_Am_I() == RTTI_UNIT && *(UnitClass *)this == UNIT_HARVESTER) {
			UnitClass * harv = (UnitClass *)this;

			int iron = harv->Gems;
			int nickel = harv->Gold;
			int graypips   = pips * fixed(iron, Rule.BailCount);
			int greenpips  = pips * fixed(nickel, Rule.BailCount);

			while (greenpips + graypips < pips) {
				int ironnickelmax = max(iron, nickel);
				if (iron > nickel) {
					graypips++;
				} else {
					greenpips++;
				}
			}

			for (int index = 0; index < Class_Of().Max_Pips(); index++) {
				int shape = PIP_EMPTY;
				if (index < pips) {
					if (greenpips) {
						shape = PIP_FULL;
						greenpips--;
					} else {
						shape = PIP_COMMANDO;
						graypips--;
					}
				}
				CC_Draw_Pip(this, Class_Of().PipShapes, shape, x+index*3, y, window, SHAPE_CENTER|SHAPE_WIN_REL);
			}
		}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		/*
		** Check if it's a Chrono tank, to show the recharge gauge.
		*/
		else if (What_Am_I() == RTTI_UNIT && *(UnitClass *)this == UNIT_CHRONOTANK) {
			for (int index = 0; index < 5; index++) {
				int shape = PIP_EMPTY;
				if (index < pips) {
					switch(index) {
						case 0:
						case 1:
							shape = PIP_COMMANDO;
							break;

						case 2:
						case 3:
							shape = PIP_ENGINEER;
							break;

						case 4:
							shape = PIP_FULL;
						default:
							break;
					}
				}
				CC_Draw_Pip(this, Class_Of().PipShapes, shape, x+index*3, y, window, SHAPE_CENTER|SHAPE_WIN_REL);
			}
#endif
		} else {
			bool building = false;
			int pip = PIP_FULL;	// green
			if(!IsOwnedByPlayer && What_Am_I() == RTTI_BUILDING) {
				if(*(BuildingClass *)this==STRUCT_POWER || *(BuildingClass *)this==STRUCT_ADVANCED_POWER) {
					building = true;
					if (House->Power_Fraction() < 1) {
						pip = PIP_ENGINEER;	// gold
						if (House->Drain > (House->Power * 2) ) {
							pip = PIP_COMMANDO;
						}
					}
				}
			}

			// Ally/spied power display is handled separately in the virtual window
			if (!building || (window != WINDOW_VIRTUAL)) {
				for (int index = 0; index < (building ? 5 : Class_Of().Max_Pips()); index++) {
					if (building) {
						CC_Draw_Pip(this, Class_Of().PipShapes, pip, x, y-index*3, window, SHAPE_CENTER|SHAPE_WIN_REL);
					} else {
						CC_Draw_Pip(this, Class_Of().PipShapes, (index < pips) ? PIP_FULL : PIP_EMPTY, x+index*3, y, window, SHAPE_CENTER|SHAPE_WIN_REL);
					}
				}

//BG			for (int index = 0; index < Class_Of().Max_Pips(); index++) {
//BG				CC_Draw_Shape(Class_Of().PipShapes, (index < pips) ? PIP_FULL : PIP_EMPTY, x+index*3, y, window, SHAPE_CENTER|SHAPE_WIN_REL);
//BG			}
			}
		}
	}

	/*
	**	Special hack to display a red pip on the medic.
	*/
	if (What_Am_I() == RTTI_INFANTRY && Combat_Damage() < 0) {
		CC_Draw_Shape(Class_Of().PipShapes, PIP_MEDIC, x+8, y, window, SHAPE_CENTER|SHAPE_WIN_REL);
	}

	/*
	**	Display whether this unit is a leader unit or not.
	*/
	if (IsLeader && (window != WINDOW_VIRTUAL)) {
		PipEnum prishape = PIP_PRIMARY;

		if(What_Am_I() == RTTI_BUILDING) {
			if(*((BuildingClass *)this) == STRUCT_KENNEL) {
				prishape = PIP_PRI;
			}
		}
		CC_Draw_Shape(Class_Of().PipShapes, prishape, x-2, y-3, window, /*SHAPE_CENTER|*/SHAPE_WIN_REL);
	}

	/*
	**	Display what group this unit belongs to. This corresponds to the team
	**	number assigned with the <CTRL> key.
	*/
	if (Is_Foot() && ((FootClass *)this)->Group != 0xFF && ((FootClass *)this)->Group < 10) {
		int yval = -1;
		int group = ((FootClass *)this)->Group;

		if (Class_Of().Max_Pips()) yval -= 4;
		if (group == 10) group = 0;

		CC_Draw_Shape(Class_Of().PipShapes, PIP_NUMBERS+group, x+2, y+yval, window, SHAPE_CENTER|SHAPE_WIN_REL);

		/*
		** If this unit is part of a formation, draw an 'F' after the group
		** number.
		*/
		if ( ((FootClass *)this)->XFormOffset != 0x80000000UL) {
			CC_Draw_Shape(Class_Of().PipShapes, PIP_LETTERF, x+8, y+yval, window, SHAPE_CENTER|SHAPE_WIN_REL);
		}
	}

	/*
	** If this building is being spied on by the player, draw the money or
	** factory-producing item or whatever.
	*/
	if (What_Am_I() == RTTI_BUILDING) {
		int spiedby = Spied_By() & (1<<(PlayerPtr->Class->House));

		/*
		** Print word "Decoy" above buildings that are spied upon or fake
		*/
		if (((BuildingClass *)this)->Class->IsFake) {
			if (spiedby || IsOwnedByPlayer) {
				CC_Draw_Shape(Class_Of().PipShapes, PIP_DECOY, x, y-16, window, SHAPE_WIN_REL);
			}
		}
		/*
		** See if we should print the credits for a spied refinery
		*/
		if (spiedby) {
			// If it's a refinery/silo, print the enemy's money
			if (((BuildingClass *)this)->Class->Capacity) {
				long money = House->Available_Money();

				/*
				**	Determine how many digits will be printed.
				*/
				int digits;
				int factor = 10;
				for (digits = 1; digits < 9; digits++) {
					if (money < factor) break;
					factor *= 10;
				}

				int startx = x + 6 * digits - 3;// + 6 * 8;
				while (money) {
					int xdigit = money % 10;
					money /= 10;
					CC_Draw_Shape(Class_Of().PipShapes, PIP_NUMBERS + xdigit, startx, y-6, window, SHAPE_CENTER|SHAPE_WIN_REL);
					startx -= 6;
				}
			}
		}
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
	assert(IsActive);

	BuildingClass * best = 0;

	/*
	**	First check to see if there are ANY buildings of the specified
	**	type in this house's inventory. If not, then don't bother to scan
	**	for one.
	*/
	if (House->Get_Quantity(b) != 0) {
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
			if (building != NULL &&
				(friendly ? building->House->Is_Ally(this) : building->House == House) &&
				!building->IsInLimbo &&
				*building == b &&
				(What_Am_I() == RTTI_AIRCRAFT || Map[building->Center_Coord()].Zones[Techno_Type_Class()->MZone] == Map[Center_Coord()].Zones[Techno_Type_Class()->MZone]) &&
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
	assert(IsActive);

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
	assert(IsActive);

	int cost = Techno_Type_Class()->Raw_Cost() * House->CostBias;

#ifdef TOFIX
	/*
	**	If the object is carrying Tiberium directly (i.e., the harvester), then
	**	account for the credits of the load.
	*/
//	cost += Fixed_To_Cardinal(UnitTypeClass::FULL_LOAD_CREDITS, Tiberium_Load())/2;
#endif

	if (House->IsHuman) {
		cost = cost * Rule.RefundPercent;
//		cost /= 2;
	}
	return(cost);
}


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
	assert(IsActive);

	if (Is_Weapon_Equipped()) {
		WeaponTypeClass const * weapon = Techno_Type_Class()->PrimaryWeapon;
		BulletTypeClass const * bullet = weapon->Bullet;
		WarheadTypeClass const * warhead = weapon->WarheadPtr;

		if (bullet->IsAntiAircraft) {
			int value = ((weapon->Attack * warhead->Modifier[ARMOR_ALUMINUM]) * weapon->Range) / weapon->ROF;

			if (Techno_Type_Class()->Is_Two_Shooter()) {
				value *= 2;
			}
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
	assert(IsActive);

	if (Is_Weapon_Equipped()) {
		if (!Techno_Type_Class()->PrimaryWeapon->Bullet->IsAntiGround) return(0);

		WeaponTypeClass const * weapon = Techno_Type_Class()->PrimaryWeapon;
		BulletTypeClass const * bullet = weapon->Bullet;
		WarheadTypeClass const * warhead = weapon->WarheadPtr;
		int mrange = min(weapon->Range, 0x0400);

		int value = ((weapon->Attack * warhead->Modifier[ARMOR_STEEL]) * mrange * warhead->SpreadFactor) / weapon->ROF;
		if (Techno_Type_Class()->Is_Two_Shooter()) {
			value *= 2;
		}
		if (bullet->IsInaccurate) {
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
	assert(IsActive);

	if (Is_Weapon_Equipped()) {
		if (!Techno_Type_Class()->PrimaryWeapon->Bullet->IsAntiGround) return(0);

		WeaponTypeClass const * weapon = Techno_Type_Class()->PrimaryWeapon;
		BulletTypeClass const * bullet = weapon->Bullet;
		WarheadTypeClass const * warhead = weapon->WarheadPtr;
		int mrange = min(weapon->Range, 0x0400);

		int value = ((weapon->Attack * warhead->Modifier[ARMOR_NONE]) * mrange * warhead->SpreadFactor) / weapon->ROF;
		if (Techno_Type_Class()->Is_Two_Shooter()) {
			value *= 2;
		}
		if (bullet->IsInaccurate) {
			value /= 2;
		}
		return(value/50);
	}
	return(0);
}


/***********************************************************************************************
 * TechnoClass::Look -- Performs a look around (map reveal) action.                            *
 *                                                                                             *
 *    This routine will reveal the map around this object.                                     *
 *                                                                                             *
 * INPUT:   incremental -- This parameter can enable a more efficient map reveal logic.        *
 *                         If it is absolutely known that the object has only moved one        *
 *                         cell from its previous location that it performed a Look() at,      *
 *                         then set this parameter to TRUE. It will only perform the look      *
 *                         check on the perimeter cells.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine is slow, try to call it only when necessary.                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Look(bool incremental)
{
	assert(IsActive);
	assert(!IsInLimbo);

	int sight_range = Techno_Type_Class()->SightRange;

	if (sight_range) {
		
			Map.Sight_From(Coord_Cell(Coord), sight_range, House, incremental);


#if (0)		                            // Leaving this here for posterity, in case we need it for revealing allies. ST - 10/17/2019 10:51AM
		/*
		** Changed this function to reveal for the appropriate players in GlyphX multiplayer. ST - 8/7/2019 11:34AM
		*/
		if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {

			Map.Sight_From(Coord_Cell(Coord), sight_range, House, incremental);
		
		} else {
		
			for (int i = 0; i < Session.Players.Count(); i++) {
				HousesType house_type = Session.Players[i]->Player.ID;
				HouseClass *house = HouseClass::As_Pointer(house_type);

				if (Is_Owned_By_Player(house) || Is_Discovered_By_Player(house)) {
					Map.Sight_From(Coord_Cell(Center_Coord()), sight_range, house, incremental);
				}
			}
		}	
#endif
	}
}


//**********************************************************************************************
// MODULE SEPARATION -- TechnoTypeClass member functions follow.
//**********************************************************************************************


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
 *   05/11/1996 JLB : Moderated risk calc so range doesn't dominate.                           *
 *=============================================================================================*/
TechnoTypeClass::TechnoTypeClass(
		RTTIType rtti,
		int id,
		int name,
		char const * ininame,
		RemapType remap,
		int verticaloffset,
		int primaryoffset,
		int primarylateral,
		int secondaryoffset,
		int secondarylateral,
		bool is_nominal,
		bool is_stealthy,
		bool is_selectable,
		bool is_legal_target,
		bool is_insignificant,
		bool is_immune,
		bool is_theater,
		bool is_turret_equipped,
		bool is_remappable,
		bool is_footprint,
		int rotation,
		SpeedType speed,
		int horizontaloffset) :
	ObjectTypeClass(	rtti,
							id,
							true,
							is_stealthy,
							is_selectable,
							is_legal_target,
							is_insignificant,
							is_immune,
							is_footprint,
							name,
							ininame),
	Remap(remap),
	IsDoubleOwned(false),
	IsInvisible(false),
	IsLeader(false),
	IsScanner(false),
	IsNominal(is_nominal),
	IsTheater(is_theater),
	IsTurretEquipped(is_turret_equipped),
	IsCrew(false),
	IsRepairable(true),
	IsRemappable(is_remappable),
	IsCloakable(false),
	IsSelfHealing(false),
	IsExploding(false),
	MZone(MZONE_NORMAL),
	ThreatRange(0),
	MaxPassengers(0),
	SightRange(0),
	Cost(0),
	Level(-1),
	Prerequisite(STRUCTF_NONE),
	Risk(0),Reward(0),
	MaxSpeed(MPH_IMMOBILE),
	Speed(speed),
	MaxAmmo(-1),
	Ownable(0),
	CameoData(NULL),
	Rotation(rotation),
	ROT(0),
	PrimaryWeapon(NULL),
	SecondaryWeapon(NULL),
	HorizontalOffset(horizontaloffset),
	VerticalOffset(verticaloffset),
	PrimaryOffset(primaryoffset),
	PrimaryLateral(primarylateral),
	SecondaryOffset(secondaryoffset),
	SecondaryLateral(secondarylateral),
	Points(0)
{
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
int TechnoTypeClass::Get_Ownable(void) const
{
	if (IsDoubleOwned && Session.Type != GAME_NORMAL) {
		return(Ownable | HOUSEF_SOVIET | HOUSEF_ALLIES);
	}
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
int TechnoTypeClass::Time_To_Build(void) const
{
	return(Cost * Rule.BuildSpeedBias * fixed(TICKS_PER_MINUTE, 1000));
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
 *    If there is no cameo data available (typical for non-produceable units), then NULL will  *
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
	// MBL 04.28.2020 "Fix" for RA Barracks structures coming back with a repair cost of 0, and repairing for free
	//
	// if (Is_Foot()) {
	// 	return((Raw_Cost()/(MaxStrength/Rule.URepairStep)) * Rule.URepairPercent);
	// }
	// return((Raw_Cost()/(MaxStrength/Rule.RepairStep)) * Rule.RepairPercent);
	//
	int repair_cost = 0;
	if (Is_Foot()) {
		repair_cost = ((Raw_Cost()/(MaxStrength/Rule.URepairStep)) * Rule.URepairPercent);
	}
	else
	{
		repair_cost = ((Raw_Cost()/(MaxStrength/Rule.RepairStep)) * Rule.RepairPercent);
	}
	repair_cost = MAX(repair_cost, 1);
	return repair_cost;
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
	if (Is_Foot()) {
		return(Rule.URepairStep);
	}
	return(Rule.RepairStep);
}


/***********************************************************************************************
 * TechnoTypeClass::Is_Two_Shooter -- Determines if this object is a double shooter.           *
 *                                                                                             *
 *    Some objects fire two shots in quick succession. If this is true for this object, then   *
 *    a 'true' value will be returned from this routine.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this object a two shooter?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoTypeClass::Is_Two_Shooter(void) const
{
	if (PrimaryWeapon != NULL && (PrimaryWeapon == SecondaryWeapon || PrimaryWeapon->Burst > 1)) {
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * _Scale_To_256 -- Scales a 1..100 number into a 1..255 number.                               *
 *                                                                                             *
 *    This is a helper routine that will take a decimal percentage number and convert it       *
 *    into a game based fixed point number.                                                    *
 *                                                                                             *
 * INPUT:   val   -- Decimal percent number to convert.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the decimal percent number converted to a game fixed point number.    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static inline int _Scale_To_256(int val)
{
	val = min(val, 100);
	val = max(val, 0);
	val = ((val * 256) / 100);
	val = min(val, 255);
	return(val);
}


/***********************************************************************************************
 * TechnoTypeClass::Read_INI -- Reads the techno type data from the INI database.              *
 *                                                                                             *
 *    Use this routine to fill in the data for this techno type class object from the          *
 *    database specified. Typical use of this is for the rules parsing.                        *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that the information will be lifted from.   *
 *                                                                                             *
 * OUTPUT:  bool; Was the database used to extract information? A failure (false) response     *
 *                would mean that the database didn't contain a section that applies to this   *
 *                techno class object.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoTypeClass::Read_INI(CCINIClass & ini)
{
	if (ini.Is_Present(Name())) {
#ifdef FIXIT_NAME_OVERRIDE
		char buffer[256];
		int id = ((RTTI+1) * 100) + ID;

		ini.Get_String(Name(), "Name", "", buffer, sizeof(buffer));
		if (strlen(buffer) > 0)  {

#if defined (GERMAN)||(FRENCH)

		for(int xx=0; NewName[xx] != NULL; xx++){
		    if(!strcmp(NewName[xx], buffer)){
			memcpy(buffer, "", sizeof(buffer));
			memcpy(buffer, NewName[xx+1], sizeof(buffer));
			break;
		   }
		}
#endif

			/*
			**	Insert the new name text into the buffer list.
			*/
			for (int index = 0; index < ARRAY_SIZE(NameOverride); index++) {
				if (NameIDOverride[index] == 0)  {
					NameOverride[index] = strdup(buffer);
					NameIDOverride[index] = id;
//					FullName = -(index+1);
					break;
				}
			}
		}
#endif

		IsDoubleOwned = ini.Get_Bool(Name(), "DoubleOwned", IsDoubleOwned);
		ThreatRange = ini.Get_Lepton(Name(), "GuardRange", ThreatRange);
		IsExploding = ini.Get_Bool(Name(), "Explodes", IsExploding);
		PrimaryWeapon = WeaponTypeClass::As_Pointer(ini.Get_WeaponType(Name(), "Primary", PrimaryWeapon != NULL ? (WeaponType)(PrimaryWeapon->ID) : WEAPON_NONE));
		SecondaryWeapon = WeaponTypeClass::As_Pointer(ini.Get_WeaponType(Name(), "Secondary", SecondaryWeapon != NULL ? (WeaponType)(SecondaryWeapon->ID) : WEAPON_NONE));
		IsCloakable = ini.Get_Bool(Name(), "Cloakable", IsCloakable);
		IsCrushable = ini.Get_Bool(Name(), "Crushable", IsCrushable);
		IsScanner = ini.Get_Bool(Name(), "Sensors", IsScanner);
		Armor = ini.Get_ArmorType(Name(), "Armor", Armor);
		Prerequisite = ini.Get_Buildings(Name(), "Prerequisite", Prerequisite);
		MaxStrength = ini.Get_Int(Name(), "Strength", MaxStrength);
		SightRange = ini.Get_Int(Name(), "Sight", SightRange);
		Level = ini.Get_Int(Name(), "TechLevel", Level);
		MaxSpeed = MPHType(_Scale_To_256(ini.Get_Int(Name(), "Speed", fixed(MaxSpeed, 256) * 100)));
		Cost = ini.Get_Int(Name(), "Cost", Cost);
		MaxAmmo = ini.Get_Int(Name(), "Ammo", MaxAmmo);
		Risk = Reward = Points = ini.Get_Int(Name(), "Points", Points);
		Ownable = ini.Get_Owners(Name(), "Owner", Ownable);
		IsCrew = ini.Get_Bool(Name(), "Crewed", IsCrew);
		IsRepairable = ini.Get_Bool(Name(), "Repairable", IsRepairable);
		IsInvisible = ini.Get_Bool(Name(), "Invisible", IsInvisible);
		IsSelfHealing = ini.Get_Bool(Name(), "SelfHealing", IsSelfHealing);
		ROT = ini.Get_Int(Name(), "ROT", ROT);
		MaxPassengers = ini.Get_Int(Name(), "Passengers", MaxPassengers);
//Mono_Printf("before image=: %s\n",GraphicName);
		ini.Get_String(Name(), "Image", GraphicName, GraphicName, sizeof(GraphicName));
//Mono_Printf("after image=: %s\n",GraphicName);if(Random_Pick(0,4)) Keyboard->Get();

		IsLeader = false;
		if (PrimaryWeapon != NULL && PrimaryWeapon->Attack > 0) {
			IsLeader = true;
		}

		/*
		**	Check to see what zone this object should recognize.
		*/
		if (PrimaryWeapon != NULL && PrimaryWeapon->WarheadPtr != NULL && PrimaryWeapon->WarheadPtr->IsWallDestroyer) {
			MZone = MZONE_DESTROYER;
		}
		if (Speed == SPEED_FLOAT) {
			MZone = MZONE_WATER;
		}

		return(true);
	}
	return(false);
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

	while (offset != NULL && *offset != REFRESH_EOL) {
		CELL cell = pos + *offset++;
		if (!Map.In_Radar(cell)) return(false);
		if (build) {
			if (!Map[cell].Is_Clear_To_Build(Speed)) {
				return(0);
			}
		} else {
			if (!Map[cell].Is_Clear_To_Move(Speed, false, false)) {
				return(0);
			}
		}
	}
	return(1);
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

	if (Session.Type == GAME_NORMAL && player == PlayerPtr) {
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
		return (PlayerPtr == House) ? true : false;
	}
	return (player == House) ? true : false;
}			  



/***********************************************************************************************
 * TechnoClass::Spied_By -- Spied by flags for this object                                     *
 *                                                                                             *
 * INPUT:   Player pointer                                                                     *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/31/2019 - SKY                                                                          *
 *=============================================================================================*/
unsigned TechnoClass::Spied_By() const
{
	return SpiedBy;
}