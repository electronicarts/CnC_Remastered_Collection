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

/* $Header:   F:\projects\c&c\vcs\code\house.cpv   2.13   02 Aug 1995 17:03:50   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : HOUSE.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 21, 1994                                                 *
 *                                                                                             *
 *                  Last Update : August 12, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   HouseClass::AI -- Process house logic.                                                    *
 *   HouseClass::Abandon_Production -- Abandons production of item type specified.             *
 *   HouseClass::Add_Nuke_Piece -- Add a nuclear piece to the collection.                      *
 *   HouseClass::Adjust_Capacity -- Adjusts the house Tiberium storage capacity.               *
 *   HouseClass::Adjust_Threat -- Adjust threat for the region specified.                      *
 *   HouseClass::As_Pointer -- Converts a house number into a house object pointer.            *
 *   HouseClass::Assign_Handicap -- Assigns the specified handicap rating to the house.        *
 *   HouseClass::Attacked -- Lets player know if base is under attack.                         *
 *   HouseClass::Available_Money -- Fetches the total credit worth of the house.               *
 *   HouseClass::Begin_Production -- Starts production of the specified object type.           *
 *   HouseClass::Blowup_All -- blows up everything                                             *
 *   HouseClass::Can_Build -- Determines if the aircraft type can be built.                    *
 *   HouseClass::Can_Build -- Determines if the building type can be built.                    *
 *   HouseClass::Can_Build -- Determines if the infantry unit can be built by this house.      *
 *   HouseClass::Can_Build -- Determines if the unit can be built by this house.               *
 *   HouseClass::Can_Build -- General purpose build legality checker.                          *
 *   HouseClass::Clobber_All -- removes house & all its objects                                *
 *   HouseClass::Debug_Dump -- Dumps the house status data to the mono screen.                 *
 *   HouseClass::Detach -- Removes specified object from house tracking systems.               *
 *   HouseClass::Does_Enemy_Building_Exist -- Checks for enemy building of specified type.     *
 *   HouseClass::Flag_Attach -- Attach flag to specified cell (or thereabouts).                *
 *   HouseClass::Flag_Attach -- Attaches the house flag the specified unit.                    *
 *   HouseClass::Flag_Remove -- Removes the flag from the specified target.                    *
 *   HouseClass::Flag_To_Die -- Flags the house to blow up soon.                               *
 *   HouseClass::Flag_To_Lose -- Flags the house to die soon.                                  *
 *   HouseClass::Flag_To_Win -- Flags the house to win soon.                                   *
 *   HouseClass::Harvested -- Adds Tiberium to the harvest storage.                            *
 *   HouseClass::Has_Nuke_Device -- Deteremines if the house has a nuclear device.             *
 *   HouseClass::HouseClass -- Constructor for a house object.                                 *
 *   HouseClass::Init -- init's in preparation for new scenario                                *
 *   HouseClass::Init_Air_Strike -- Add (or reset) the air strike sidebar button.              *
 *   HouseClass::Init_Data -- Initializes the multiplayer color data.                          *
 *   HouseClass::Init_Ion_Cannon -- Initialize the ion cannon countdown.                       *
 *   HouseClass::Init_Nuke_Bomb -- Adds (if necessary) the atom bomb to the sidebar.           *
 *   HouseClass::Is_Ally -- Checks to see if the object is an ally.                            *
 *   HouseClass::Is_Ally -- Determines if the specified house is an ally.                      *
 *   HouseClass::Is_Ally -- Determines if the specified house is an ally.                      *
 *   HouseClass::MPlayer_Defeated -- multiplayer; house is defeated                            *
 *   HouseClass::Make_Air_Strike_Available -- Make the airstrike available.                    *
 *   HouseClass::Make_Ally -- Make the specified house an ally.                                *
 *   HouseClass::Make_Enemy -- Make an enemy of the house specified.                           *
 *   HouseClass::Manual_Place -- Inform display system of building placement mode.             *
 *   HouseClass::One_Time -- Handles one time initialization of the house array.               *
 *   HouseClass::Place_Object -- Places the object (building) at location specified.           *
 *   HouseClass::Place_Special_Blast -- Place a special blast effect at location specified.    *
 *   HouseClass::Power_Fraction -- Fetches the current power output rating.                    *
 *   HouseClass::Read_INI -- Reads house specific data from INI.                               *
 *   HouseClass::Refund_Money -- Refunds money to back to the house.                           *
 *   HouseClass::Remap_Table -- Fetches the remap table for this house object.                 *
 *   HouseClass::Remove_Air_Strike -- Removes the air strike button from the sidebar.          *
 *   HouseClass::Remove_Ion_Cannon -- Disables the ion cannon.                                 *
 *   HouseClass::Remove_Nuke_Bomb -- Removes the nuclear bomb from the sidebar.                *
 *   HouseClass::Sell_Wall -- Tries to sell the wall at the specified location.                *
 *   HouseClass::Silo_Redraw_Check -- Flags silos to be redrawn if necessary.                  *
 *   HouseClass::Special_Weapon_AI -- Fires special weapon.                                    *
 *   HouseClass::Spend_Money -- Removes money from the house.                                  *
 *   HouseClass::Suggest_New_Object -- Determine what would the next buildable object be.      *
 *   HouseClass::Suggested_New_Team -- Determine what team should be created.                  *
 *   HouseClass::Suspend_Production -- Temporarily puts production on hold.                    *
 *   HouseClass::Validate -- validates house pointer														  *
 *   HouseClass::Write_INI -- Writes house specific data into INI file.                        *
 *   HouseClass::delete -- Deallocator function for a house object.                            *
 *   HouseClass::new -- Allocator for a house class.                                           *
 *   HouseClass::operator HousesType -- Conversion to HousesType operator.                     *
 *   HouseClass::~HouseClass -- Default destructor for a house object.                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

/*
** New sidebar for GlyphX multiplayer. ST - 3/26/2019 12:24PM
*/
#include "SidebarGlyphx.h"
#include "defines.h"


/***********************************************************************************************
 * HouseClass::Validate -- validates house pointer															  *
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
int HouseClass::Validate(void) const
{
	int num;

	num = Houses.ID(this);
	if (num < 0 || num >= HOUSE_MAX) {
		Validate_Error("HOUSE");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
#endif


/***********************************************************************************************
 * HouseClass::operator HousesType -- Conversion to HousesType operator.                       *
 *                                                                                             *
 *    This operator will automatically convert from a houses class object into the HousesType  *
 *    enumerated value.                                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the object's HousesType value.                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
HouseClass::operator HousesType(void) const
{
	Validate();
	return(Class->House);
}


/***********************************************************************************************
 * HouseClass::As_Pointer -- Converts a house number into a house object pointer.              *
 *                                                                                             *
 *    Use this routine to convert a house number into the house pointer that it represents.    *
 *    A simple index into the Houses template array is not sufficient, since the array order   *
 *    is arbitrary. An actual scan through the house object is required in order to find the   *
 *    house object desired.                                                                    *
 *                                                                                             *
 * INPUT:   house -- The house type number to look up.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the house object that the house number represents.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
HouseClass * HouseClass::As_Pointer(HousesType house)
{
	for (int index = 0; index < Houses.Count(); index++) {
		if (Houses.Ptr(index)->Class->House == house) {
			return(Houses.Ptr(index));
		}
	}
	return(0);
}


/***********************************************************************************************
 * HouseClass::One_Time -- Handles one time initialization of the house array.                 *
 *                                                                                             *
 *    This basically calls the constructor for each of the houses in the game. All other       *
 *    data specific to the house is initialized when the scenario is loaded.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this ONCE at the beginning of the game.                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::One_Time(void)
{
//	for (HousesType index = HOUSE_FIRST; index < HOUSE_COUNT; index++) {
//		new(index) HouseClass;
//	}
	
#ifdef USE_RA_AI	
	/*
	** Required for Red Alert AI. ST - 7/23/2019 3:21PM
	*/
	BuildChoice.Set_Heap(STRUCT_COUNT);
#endif
}


/***********************************************************************************************
 * HouseClass::Assign_Handicap -- Assigns the specified handicap rating to the house.          *
 *                                                                                             *
 *    The handicap rating will affect combat, movement, and production for the house. It can   *
 *    either make it more or less difficult for the house (controlled by the handicap value).  *
 *                                                                                             *
 * INPUT:   handicap -- The handicap value to assign to this house. The default value for      *
 *                      a house is DIFF_NORMAL.                                                *
 *                                                                                             *
 * OUTPUT:  Returns with the old handicap value.                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *   10/22/1996 JLB : Uses act like value for multiplay only.                                  *
 *=============================================================================================*/
DiffType HouseClass::Assign_Handicap(DiffType handicap)
{
	DiffType old = Difficulty;
	Difficulty = handicap;

	if (GameToPlay != GAME_NORMAL) {
		HouseTypeClass const * hptr = &HouseTypeClass::As_Reference(ActLike);
		FirepowerBias = hptr->FirepowerBias * Rule.Diff[handicap].FirepowerBias;
		GroundspeedBias = hptr->GroundspeedBias * Rule.Diff[handicap].GroundspeedBias;
		AirspeedBias = hptr->AirspeedBias * Rule.Diff[handicap].AirspeedBias;
		ArmorBias = hptr->ArmorBias * Rule.Diff[handicap].ArmorBias;
		ROFBias = hptr->ROFBias * Rule.Diff[handicap].ROFBias;
		CostBias = hptr->CostBias * Rule.Diff[handicap].CostBias;
		RepairDelay = Rule.Diff[handicap].RepairDelay;
		BuildDelay = Rule.Diff[handicap].BuildDelay;
		BuildSpeedBias = hptr->BuildSpeedBias * Rule.Diff[handicap].BuildSpeedBias;
	} else {
		FirepowerBias = Rule.Diff[handicap].FirepowerBias;
		GroundspeedBias = Rule.Diff[handicap].GroundspeedBias;
		AirspeedBias = Rule.Diff[handicap].AirspeedBias;
		ArmorBias = Rule.Diff[handicap].ArmorBias;
		ROFBias = Rule.Diff[handicap].ROFBias;
		CostBias = Rule.Diff[handicap].CostBias;
		RepairDelay = Rule.Diff[handicap].RepairDelay;
		BuildDelay = Rule.Diff[handicap].BuildDelay;
		BuildSpeedBias = Rule.Diff[handicap].BuildSpeedBias;
	}

	return(old);
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * HouseClass::Debug_Dump -- Dumps the house status data to the mono screen.                   *
 *                                                                                             *
 *    This utility function will output the current status of the house class to the mono      *
 *    screen. Through this information bugs may be fixed or detected.                          *
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
void HouseClass::Debug_Dump(MonoClass *) const
{
	Validate();
}
#endif


/***********************************************************************************************
 * HouseClass::new -- Allocator for a house class.                                             *
 *                                                                                             *
 *    This is the allocator for a house class. Since there can be only                         *
 *    one of each type of house, this is allocator has restricted                              *
 *    functionality. Any attempt to allocate a house structure for a                           *
 *    house that already exists, just returns a pointer to the previously                      *
 *    allocated house.                                                                         *
 *                                                                                             *
 * INPUT:   house -- The house to allocate a class object for.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the allocated class object.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void * HouseClass::operator new(size_t)
{
	void * ptr = Houses.Allocate();
	if (ptr) {
		((HouseClass *)ptr)->IsActive = true;
	}
	return(ptr);
}


/***********************************************************************************************
 * HouseClass::delete -- Deallocator function for a house object.                              *
 *                                                                                             *
 *    This function marks the house object as "deallocated". Such a                            *
 *    house object is available for reallocation later.                                        *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the house object to deallocate.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::operator delete(void *ptr)
{
	if (ptr) {
		((HouseClass *)ptr)->IsActive = false;
	}
	Houses.Free((HouseClass *)ptr);
}


/***********************************************************************************************
 * HouseClass::HouseClass -- Constructor for a house object.                                   *
 *                                                                                             *
 *    This function is the constructor and it marks the house object                           *
 *    as being allocated.                                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
HouseClass::HouseClass(HousesType house) :
	Class(&HouseTypeClass::As_Reference(house)),
	IonCannon(ION_CANNON_GONE_TIME, VOX_ION_READY, VOX_ION_CHARGING, VOX_ION_CHARGING, VOX_NO_POWER),
	AirStrike(AIR_CANNON_GONE_TIME, VOX_AIRSTRIKE_READY, VOX_NONE, VOX_NOT_READY, VOX_NOT_READY),
	NukeStrike(NUKE_GONE_TIME, VOX_NUKE_AVAILABLE, VOX_NONE, VOX_NOT_READY, VOX_NO_POWER)
{

	for (HousesType i = HOUSE_FIRST; i < HOUSE_COUNT; i++) {
		UnitsKilled[i] = 0;
		BuildingsKilled[i] = 0;
	}
	WhoLastHurtMe = house;			// init this to myself

	IsVisionary = false;
	IsFreeHarvester = false;
	Blockage = 0;
	UnitsLost = 0;
	BuildingsLost = 0;

	NewActiveBScan = 0;
	ActiveBScan = 0;
	NewActiveUScan = 0;
	ActiveUScan = 0;
	NewActiveIScan = 0;
	ActiveIScan = 0;
	NewActiveAScan = 0;
	ActiveAScan = 0;

	strcpy((char *)Name, "Computer");	// Default computer name.
	JustBuilt = STRUCT_NONE;
	AlertTime = 0;
	IsAlerted = false;
	IsAirstrikePending = false;
	AircraftFactory = -1;
	AircraftFactories = 0;
	ActLike = Class->House;
	Allies = 0;
	AScan = 0;
	NukeDest = 0;
	BlitzTime.Clear();
	ScreenShakeTime.Clear();
	BScan = 0;
	BuildingFactories = 0;
	BuildingFactory = -1;
	Capacity = 0;
	Credits = 0;
	CreditsSpent = 0;
	CurBuildings = 0;
	CurUnits = 0;
	DamageTime = DAMAGE_DELAY;
	Drain = 0;
	Edge = SOURCE_NORTH;
	FlagHome = 0;
	FlagLocation = TARGET_NONE;
	HarvestedCredits = 0;
	HouseTriggers[house].Clear();
	IGaveUp = false;
	InfantryFactories = 0;
	InfantryFactory = -1;
	InitialCredits = 0;
	InitialCredits = 0;
	IScan = 0;
	IsRecalcNeeded = true;
	IsCivEvacuated = false;
	IsDefeated = false;
	IsDiscovered = false;
	IsHuman = false;
	WasHuman = false;
	IsMaxedOut = false;
	IsStarted = false;
	IsToDie = false;
	IsToLose = false;
	IsToWin = false;
	Make_Ally(house);
	MaxBuilding = 0;
	MaxUnit = 0;
	NewAScan = 0;
	NewBScan = 0;
	NewIScan = 0;
	NewUScan = 0;
	NukePieces = 0x07;
	Power = 0;
	Radar = RADAR_NONE;
	RemapTable = Class->RemapTable;
	RemapColor = Class->RemapColor;
	Resigned = false;
	SpeakAttackDelay = 1;
	SpeakMaxedDelay = 1;

	SpeakMoneyDelay = 1;
	SpeakMoneyDelay.Set(Options.Normalize_Delay(SPEAK_DELAY));		// 2 minutes

	SpeakPowerDelay = 1;
	SpecialFactories = 0;
	SpecialFactory = -1;
	TeamTime = TEAM_DELAY;
	Tiberium = 0;
	TriggerTime = 0;
	UnitFactories = 0;
	UnitFactory = -1;
	UScan = 0;
	memset((void *)&Regions[0], 0x00, sizeof(Regions));

	Init_Unit_Trackers();

	DebugUnlockBuildables = false;

	StartLocationOverride = -1;

	/*
	** New AI variables from RA. Need to add to save/load?
	*/
#ifdef USE_RA_AI	
	
	IsBaseBuilding = true;
	Center = 0;
	Radius = 0;
	for (ZoneType zone = ZONE_FIRST; zone < ZONE_COUNT; zone++) {
		ZoneInfo[zone].AirDefense = 0;
		ZoneInfo[zone].ArmorDefense = 0;
		ZoneInfo[zone].InfantryDefense = 0;
	}

		
	LATime = 0;
	LAType = RTTI_NONE;
	LAZone = ZONE_NONE;
	LAEnemy = HOUSE_NONE;

	ToCapture = TARGET_NONE;

	RadarSpied = 0;
	PointTotal = 0;

	memset(BQuantity, '\0', sizeof(BQuantity));
	memset(UQuantity, '\0', sizeof(UQuantity));
	memset(IQuantity, '\0', sizeof(IQuantity));
	memset(AQuantity, '\0', sizeof(AQuantity));
		
	Attack = 0;

	Enemy = HOUSE_NONE;
			
	AITimer = 0;

	BuildStructure = STRUCT_NONE;
	BuildUnit = UNIT_NONE;
	BuildInfantry = INFANTRY_NONE;
	BuildAircraft = AIRCRAFT_NONE;

	State = STATE_BUILDUP;

	IsTiberiumShort = false;

	IQ = Rule.MaxIQ;

	IsParanoid = false;

	OldBScan = 0;

	Assign_Handicap(DIFF_NORMAL);

#endif

}


HouseClass::~HouseClass (void)
{
	Free_Unit_Trackers();
}

/***********************************************************************************************
 * HouseClass::Can_Build -- General purpose build legality checker.                            *
 *                                                                                             *
 *    This routine is called when it needs to be determined if the specified object type can   *
 *    be built by this house. Production and sidebar maintenance use this routine heavily.     *
 *                                                                                             *
 * INPUT:   type  -- Pointer to the type of object that legality is to be checked for.         *
 *                                                                                             *
 *          house -- This is the house to check for legality against. Note that this might     *
 *                   not be 'this' house since the check could be from a captured factory.     *
 *                   Captured factories build what the original owner of them could build.     *
 *                                                                                             *
 * OUTPUT:  Can the specified object be built?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *   08/12/1995 JLB : Updated for GDI building sandbag walls in #9.                            *
 *=============================================================================================*/
bool HouseClass::Can_Build(TechnoTypeClass const * type, HousesType house) const
{
	Validate();
	if (!type || !type->IsBuildable || !((1L << house) & type->Ownable)) return(false);

	/*
	**	The computer can always build everthing.
	*/
#ifdef USE_RA_AI
	if (!IsHuman && GameToPlay == GAME_NORMAL) return(true);		// Added game type check for AI from RA. ST - 7/25/2019 3:25PM
#else
	if (!IsHuman) return(true);
#endif

	/*
	**	Perform some equivalency fixups for the building existance flags.
	*/
	long flags = ActiveBScan;

#ifdef USE_RA_AI
	// OldBScan Copied from RA for AI. ST - 7/25/2019 3:27PM
	/*
	**	The computer records prerequisite buildings because it can't relay on the
	**	sidebar to keep track of this information.
	*/
	if (!IsHuman) {
		flags = OldBScan;
	}
#endif

	int pre = type->Pre;
	if (flags & STRUCTF_ADVANCED_POWER) flags |= STRUCTF_POWER;
	if (flags & STRUCTF_HAND) flags |= STRUCTF_BARRACKS;
	if (flags & STRUCTF_OBELISK) flags |= STRUCTF_ATOWER;
	if (flags & STRUCTF_TEMPLE) flags |= STRUCTF_EYE;
	if (flags & STRUCTF_AIRSTRIP) flags |= STRUCTF_WEAP;
	if (flags & STRUCTF_SAM) flags |= STRUCTF_HELIPAD;

	/*
	**	Multiplayer game uses a different legality check for building.
	*/
	if (GameToPlay != GAME_NORMAL || (Special.IsJurassic && AreThingiesEnabled)) {
		if (DebugUnlockBuildables) {
			return true;
		}
		return((pre & flags) == pre && type->Level <= BuildLevel);
	}

#ifdef NEWMENU
	int level = BuildLevel;
#else
	int level = Scenario;
#endif

	/*
	**	Special check to make the mission objective buildings the prerequisite
	**	for the stealth tank in mission #11 only.
	*/
	if (house == HOUSE_BAD &&
		type->What_Am_I() == RTTI_UNITTYPE &&
		((UnitTypeClass const *)type)->Type == UNIT_STANK &&
		level == 11) {

		pre = STRUCTF_MISSION;
		level = type->Scenario;
	}

	/*
	**	Special case check to ensure that GDI doesn't get the bazooka guy
	**	until mission #8.
	*/
	if (house == HOUSE_GOOD &&
		type->What_Am_I() == RTTI_INFANTRYTYPE &&
		((InfantryTypeClass const *)type)->Type == INFANTRY_E3 &&
		level < 7) {

		return(false);
	}

	/*
	**	Special check to allow GDI to build the MSAM by mission #9
	**	and no sooner.
	*/
	if (house == HOUSE_GOOD &&
		type->What_Am_I() == RTTI_UNITTYPE &&
		((UnitTypeClass const *)type)->Type == UNIT_MLRS &&
		level < 9) {

		return(false);
	}

	/*
	**	Special case to disable the APC from the Nod player.
	*/
	if (house == HOUSE_BAD &&
		type->What_Am_I() == RTTI_UNITTYPE &&
		((UnitTypeClass const *)type)->Type == UNIT_APC) {

		return(false);
	}

	/*
	**	Ensure that the Temple of Nod cannot be built by GDI even
	**	if GDI has captured the Nod construction yard.
	*/
	if (type->What_Am_I() == RTTI_BUILDINGTYPE &&
		(((BuildingTypeClass const *)type)->Type == STRUCT_TEMPLE || ((BuildingTypeClass const *)type)->Type == STRUCT_OBELISK) &&
		Class->House == HOUSE_GOOD) {

		return(false);
	}

	/*
	**	Ensure that the rocket launcher tank cannot be built by Nod.
	*/
	if (type->What_Am_I() == RTTI_UNITTYPE &&
		((UnitTypeClass const *)type)->Type == UNIT_MLRS &&
		Class->House == HOUSE_BAD) {

		return(false);
	}

	/*
	**	Ensure that the ion cannon cannot be built if
	**	Nod has captured the GDI construction yard.
	*/
	if (type->What_Am_I() == RTTI_BUILDINGTYPE &&
		(((BuildingTypeClass const *)type)->Type == STRUCT_EYE) &&
		Class->House == HOUSE_BAD) {

		return(false);
	}

	/*
	**	Nod can build the advanced power plant at scenario #12.
	*/
	if (house == HOUSE_BAD &&
		level >= 12 &&
		type->What_Am_I() == RTTI_BUILDINGTYPE &&
		((BuildingTypeClass const *)type)->Type == STRUCT_ADVANCED_POWER) {

		level = type->Scenario;
	}

	/*
	**	Nod cannot build a helipad in the normal game.
	*/
	if (house == HOUSE_BAD &&
		type->What_Am_I() == RTTI_BUILDINGTYPE &&
		((BuildingTypeClass const *)type)->Type == STRUCT_HELIPAD) {

		return(false);
	}

	/*
	**	GDI can build the sandbag wall only from scenario #9 onwards.
	*/
	if (house == HOUSE_GOOD &&
		level < 8 &&
		type->What_Am_I() == RTTI_BUILDINGTYPE &&
		((BuildingTypeClass const *)type)->Type == STRUCT_SANDBAG_WALL) {

		return(false);
	}

	/*
	**	GDI has a special second training mission. Adjust the scenario level so that
	**	scenario two will still feel like scenario #1.
	*/
	if (house == HOUSE_GOOD && level == 2) {
		level = 1;
	}

	// ST - 8/23/2019 4:53PM
	if (DebugUnlockBuildables) {
		level = 98;
		pre = 0;
	}			

	if (Debug_Cheat) level = 98;
	return((pre & flags) == pre && type->Scenario <= level);
}


/***********************************************************************************************
 * HouseClass::Can_Build -- Determines if the building type can be built.                      *
 *                                                                                             *
 *    This routine is used by the construction preparation code to building a list of building *
 *    types that can be built. It determines if a building can be built by checking if the     *
 *    prerequisite buildings have been built (and still exist) as well as checking to see if   *
 *    the house can build the specified structure.                                             *
 *                                                                                             *
 * INPUT:   s     -- The structure type number that is being checked.                          *
 *                                                                                             *
 *          house -- The house number to use when determining if the object can be built.      *
 *                   This is necessary because the current owner of the factory doesn't        *
 *                   control what the factory can produce. Rather, the original builder of     *
 *                   the factory controls this.                                                *
 *                                                                                             *
 * OUTPUT:  bool; Can this structure type be built at this time?                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/08/1994 JLB : Created.                                                                 *
 *   05/31/1995 JLB : Handles specified ownership override.                                    *
 *=============================================================================================*/
bool HouseClass::Can_Build(StructType s, HousesType house) const
{
	Validate();
	return(Can_Build(&BuildingTypeClass::As_Reference(s), house));
}


/***********************************************************************************************
 * HouseClass::Can_Build -- Determines if the infantry unit can be built by this house.        *
 *                                                                                             *
 *    Use this routine to determine if the infantry type specified can be built by this        *
 *    house. It determines this by checking the ownership allowed bits in the infantry         *
 *    type class.                                                                              *
 *                                                                                             *
 * INPUT:   infantry -- The infantry type to check against this house.                         *
 *                                                                                             *
 *          house -- The house number to use when determining if the object can be built.      *
 *                   This is necessary because the current owner of the factory doesn't        *
 *                   control what the factory can produce. Rather, the original builder of     *
 *                   the factory controls this.                                                *
 *                                                                                             *
 * OUTPUT:  bool; Can the infantry be produced by this house?                                  *
 *                                                                                             *
 * WARNINGS:   It does not check to see if there is a functional barracks available, but       *
 *             merely checks to see if it is legal for this house to build that infantry       *
 *             type.                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *   05/31/1995 JLB : Handles specified ownership override.                                    *
 *=============================================================================================*/
bool HouseClass::Can_Build(InfantryType infantry, HousesType house) const
{
	Validate();
	return(Can_Build(&InfantryTypeClass::As_Reference(infantry), house));
}


/***********************************************************************************************
 * HouseClass::Can_Build -- Determines if the unit can be built by this house.                 *
 *                                                                                             *
 *    This routine is used to determine if the unit type specified can in fact be built by     *
 *    this house. It checks the ownable bits in the unit's type to determine this.             *
 *                                                                                             *
 * INPUT:   unit  -- The unit type to check against this house.                                *
 *                                                                                             *
 *          house -- The house number to use when determining if the object can be built.      *
 *                   This is necessary because the current owner of the factory doesn't        *
 *                   control what the factory can produce. Rather, the original builder of     *
 *                   the factory controls this.                                                *
 *                                                                                             *
 * OUTPUT:  bool; Can the unit be built by this house?                                         *
 *                                                                                             *
 * WARNINGS:   This doesn't check to see if there is a functional factory that can build       *
 *             this unit, but merely if the unit can be built according to ownership rules.    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *   05/31/1995 JLB : Handles specified ownership override.                                    *
 *=============================================================================================*/
bool HouseClass::Can_Build(UnitType unit, HousesType house) const
{
	Validate();
	return(Can_Build(&UnitTypeClass::As_Reference(unit), house));
}


/***********************************************************************************************
 * HouseClass::Can_Build -- Determines if the aircraft type can be built.                      *
 *                                                                                             *
 *    Use this routine to determine if the specified aircraft type can be built. This routine  *
 *    is used by the sidebar and factory to determine what can be built.                       *
 *                                                                                             *
 * INPUT:   aircraft -- The aircraft type to check for build legality.                         *
 *                                                                                             *
 *          house    -- The house that is performing the check. This is typically the house    *
 *                      of the original building of the factory rather than the current        *
 *                      owner.                                                                 *
 *                                                                                             *
 * OUTPUT:  Can this aircraft type be built by the house specified?                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Can_Build(AircraftType aircraft, HousesType house) const
{
	Validate();
	return(Can_Build(&AircraftTypeClass::As_Reference(aircraft), house));
}


/***************************************************************************
 * HouseClass::Init -- init's in preparation for new scenario              *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/07/1994 BR : Created.                                              *
 *   12/17/1994 JLB : Resets tracker bits.                                 *
 *=========================================================================*/
void HouseClass::Init(void)
{
	Houses.Free_All();

	for (HousesType index = HOUSE_FIRST; index < HOUSE_COUNT; index++) {
		HouseTriggers[index].Clear();
	}
}


// Object selection list is switched with player context for GlyphX. ST - 4/17/2019 9:42AM
extern void Logic_Switch_Player_Context(HouseClass *house);

/***********************************************************************************************
 * HouseClass::AI -- Process house logic.                                                      *
 *                                                                                             *
 *    This handles the AI for the house object. It should be called once per house per game    *
 *    tick. It processes all house global tasks such as low power damage accumulation and      *
 *    house specific trigger events.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *   07/17/1995 JLB : Limits EVA speaking unless the player can do something.                  *
 *=============================================================================================*/
extern void Recalculate_Placement_Distances();
extern void On_Message(const char* message, float timeout_seconds, long long message_id);

void HouseClass::AI(void)
{
	Validate();

	// Set PlayerPtr to be this house. Not really keen on this solution but it means I have to make fewer code changes to the original code. ST - 4/17/2019 4:32PM
	Logic_Switch_Player_Context(this);

	/*
	**	Reset the scan accumulation bits for the next logic pass.
	*/
	IScan = NewIScan;
	BScan = NewBScan;
	UScan = NewUScan;
	AScan = NewAScan;
	ActiveIScan = NewActiveIScan;
	ActiveBScan = NewActiveBScan;
	ActiveUScan = NewActiveUScan;
	ActiveAScan = NewActiveAScan;
	NewIScan = 0;
	NewBScan = 0;
	NewUScan = 0;
	NewAScan = 0;
	NewActiveIScan = 0;
	NewActiveBScan = 0;
	NewActiveUScan = 0;
	NewActiveAScan = 0;

#ifdef USE_RA_AI
	//
	// Copied from RA for AI. ST - 7/25/2019 3:58PM
	//
	/*
	**	If base building has been turned on by a trigger, then force the house to begin
	**	production and team creation as well. This is also true if the IQ is high enough to
	**	being base building.
	*/
	if (!IsHuman && GameToPlay != GAME_NORMAL && (IsBaseBuilding || IQ >= Rule.IQProduction)) {
		IsBaseBuilding = true;
		IsStarted = true;
		IsAlerted = true;
	}
#endif

	/*
	**	Check to see if the house wins.
	*/
	if (GameToPlay == GAME_NORMAL && IsToWin && BorrowedTime.Expired() && Blockage <= 0) {
		IsToWin = false;
		if (this == PlayerPtr) {
			PlayerWins = true;
		} else {
			PlayerLoses = true;
		}
	}

	/*
	**	Check to see if the house loses.
	*/
	if (GameToPlay == GAME_NORMAL && IsToLose && BorrowedTime.Expired()) {
		IsToLose = false;
		if (this == PlayerPtr) {
			PlayerLoses = true;
		} else {
			PlayerWins = true;
		}
	}

	/*
	**	Check to see if all objects of this house should be blown up.
	*/
	if (IsToDie && BorrowedTime.Expired()) {
		IsToDie = false;
		Blowup_All();
	}

	/*
	**	Double check power values to correct illegal conditions. It is possible to
	**	get a power output of negative (one usually) as a result of damage sustained
	**	and the fixed point fractional math involved with power adjustements. If the
	**	power rating drops below zero, then make it zero.
	*/
	if (GameToPlay == GAME_NORMAL) {
		Power = MAX(Power, 0);
		Drain = MAX(Drain, 0);
	}

	/*
	**	If the base has been alerted to the enemy and should be attacking, then
	**	see if the attack timer has expired. If it has, then create the attack
	**	teams.
	*/
	if (IsAlerted && AlertTime.Expired()) {

		/*
		**	Adjusted to reduce maximum number of teams created.
		*/
		int maxteams = Random_Pick(2, (int)(((BuildLevel-1)/3)+1));
		for (int index = 0; index < maxteams; index++) {
			TeamTypeClass const * ttype = Suggested_New_Team(true);
			if (ttype) {
				ScenarioInit++;
				ttype->Create_One_Of();
				ScenarioInit--;
			}
		}
		if (GameToPlay == GAME_NORMAL && PlayerPtr->Difficulty == DIFF_HARD) {
			AlertTime = (TICKS_PER_MINUTE * Random_Pick(4, 10));
		} else {
			if (GameToPlay == GAME_NORMAL && PlayerPtr->Difficulty == DIFF_EASY) {
				AlertTime = (TICKS_PER_MINUTE * Random_Pick(16, 40));
			} else {
				AlertTime = (TICKS_PER_MINUTE * Random_Pick(5, 20));
			}
		}
	}

	/*
	**	Create teams for this house if necessary.
	** (Use the same timer for some extra capture-the-flag logic.)
	*/
	if (TeamTime.Expired()) {
		TeamTypeClass const * ttype = Suggested_New_Team(false);
		if (ttype) {
			ttype->Create_One_Of();
		}

		/*
		** Also use this timer to detect if someone is sitting on my flag cell.
		*/
		if (Special.IsCaptureTheFlag && GameToPlay != GAME_NORMAL) {
			TechnoClass *techno;
			int moving;

			/*
			**	If this house's flag waypoint is a valid cell, see if there's
			**	someone sitting on it.  If so, make the scatter.
			*/
			if (FlagHome) {
				techno = Map[FlagHome].Cell_Techno();
				if (techno) {
					moving = false;
					if (techno->What_Am_I() == RTTI_INFANTRY ||
						techno->What_Am_I() == RTTI_UNIT) {
						if (Target_Legal(((FootClass *)techno)->NavCom)) {
							moving = true;
						}
					}

					if (!moving) {
						techno->Scatter(0,true,true);
					}
				}
			}
		}

		/*
		** Randomly create a Visceroid or other disastrous multiplayer object.
		** Create the object, and use Scan_Place_Object to place the object near
		** the center of the map.
		*/
		if (GameToPlay != GAME_NORMAL && Class->House==HOUSE_JP) {
			int rlimit;

			if (Special.IsJurassic && AreThingiesEnabled) {
				rlimit = 450;
			} else {
				rlimit = 1000;
			}

			//if (IRandom(0, rlimit) == 0) {
			if (IRandom(0, rlimit) <= 5) {		// More visceroids! ST - 3/3/2020 4:34PM
				UnitClass *obj = NULL;
				CELL cell;

				if (Special.IsJurassic && AreThingiesEnabled) {
					obj = new UnitClass(Random_Pick(UNIT_TRIC, UNIT_STEG), HOUSE_JP);
				} else if (Special.IsVisceroids) {
					if (BuildLevel >= 7) {
						if (!(UScan & UNITF_VICE)) {
							obj = new UnitClass(UNIT_VICE, HOUSE_JP);
						}
					}
				}

				if (obj) {
					cell = XY_Cell (Map.MapCellX + Random_Pick(0, Map.MapCellWidth - 1),
						Map.MapCellY + Random_Pick(0, Map.MapCellHeight - 1));
					if (!Scan_Place_Object(obj, cell)) {
						delete obj;
					}
				}
			}
		}

		TeamTime.Set(TEAM_DELAY);
	}

	/*
	**	If there is insufficient power, then all buildings that are above
	**	half strength take a little bit of damage.
	*/
	if (DamageTime.Expired()) {

/*
**	No free harvesters for computer or player. - 8/16/95
*/
#ifdef OBSOLETE
		/*
		**	Replace the last harvester if there is a refinery present.
		*/
		if (GameToPlay == GAME_NORMAL &&
			Frame > 5 &&
			(!IsHuman && BuildLevel <= 6) &&
			(ActiveBScan & STRUCTF_REFINERY) != 0 &&
			(UScan & UNITF_HARVESTER) == 0 &&
			!IsFreeHarvester) {

			IsFreeHarvester = true;
			FreeHarvester = TICKS_PER_MINUTE * 2;
		}
#endif

		/*
		**	If a free harvester is to be created and the time is right, then create
		**	the harvester and clear the free harvester pending flag.
		*/
		if (IsFreeHarvester && FreeHarvester.Expired()) {
			IsFreeHarvester = false;
			Create_Special_Reinforcement(this, (TechnoTypeClass *)&UnitTypeClass::As_Reference(UNIT_HARVESTER), NULL);
		}

		/*
		**	When the power is below required, then the buildings will take damage over
		**	time.
		*/
		if (Power_Fraction() < 0x100) {
			for (int index = 0; index < Buildings.Count(); index++) {
				BuildingClass & b = *Buildings.Ptr(index);

				if (b.House == this && b.Health_Ratio() > 0x080) {
					if (b.Class->Drain) {
						int damage = 1;
						b.Take_Damage(damage, 0, WARHEAD_AP, 0);
					}
				}
			}
		}
		DamageTime.Set(DAMAGE_DELAY);
	}

	/*
	**	If there are no more buildings to sell, then automatically cancel the
	**	sell mode.
	*/
	if (PlayerPtr == this && !BScan && Map.IsSellMode) {
		Map.Sell_Mode_Control(0);
	}

	/*
	**	Various base conditions may be announced to the player.
	*/
	if (PlayerPtr == this) {

		if (SpeakMoneyDelay.Expired() && Available_Money() < 100 && UnitFactories+BuildingFactories+InfantryFactories > 0) {

			// MBL 03.23.2020 - Change "Need more funds" to "Insufficient funds" per https://jaas.ea.com/browse/TDRA-5370
			// Speak(VOX_NEED_MO_MONEY);
			Speak(VOX_NO_CASH);

			//
			// !!! MBL 03.18.2020: 
			// !!! Changing "Insufficient Funds" speak delay for this case to 1 minute instead of 2. 
			// !!! Note that all other speak delays are 2 minutes in TD (SPEAK_DELAY) and RA (Rule.SpeakDelay)
			// !!! This is per https://jaas.ea.com/browse/TDRA-4659 (Ted and Jim)
			// !!! I Checked with Joe mostly okay with this change, but want to note that we are changing original behavior
			// !!! All other speak delays in TD and RA (max capacity and low power) remain at 2 minutes.
			// !!! Also, in Red Alert, this is still 2 minutes from Rules.ini (SpeakDelay variable)
			//
			// SpeakMoneyDelay.Set(Options.Normalize_Delay(SPEAK_DELAY));		// 2 minutes
			//
			// MBL 05.18.2020: Setting back to 2 minutes as requested per https://jaas.ea.com/browse/TDRA-5834
			//
			// SpeakMoneyDelay.Set(Options.Normalize_Delay(SPEAK_DELAY / 2));	// 1 minute (new)
			SpeakMoneyDelay.Set(Options.Normalize_Delay(SPEAK_DELAY));			// 2 minutes (original)

			int text_id = TXT_INSUFFICIENT_FUNDS;
			char const * text = Text_String(TXT_INSUFFICIENT_FUNDS);
			if (text != NULL) {
				On_Message(text, 35.0f, text_id);
			}
		}
		if (SpeakMaxedDelay.Expired() && IsMaxedOut) {
			IsMaxedOut = false;
			if ((Capacity - Tiberium) < 300 && Capacity > 500 && (BScan & (STRUCTF_REFINERY | STRUCTF_CONST))) {
 				Speak(VOX_NEED_MO_CAPACITY);
				SpeakMaxedDelay.Set(Options.Normalize_Delay(SPEAK_DELAY));
			}
		}
		if (SpeakPowerDelay.Expired() && Power_Fraction() < 0x0100) {
			if (BScan & STRUCTF_CONST) {
				Speak(VOX_LOW_POWER);
				SpeakPowerDelay.Set(Options.Normalize_Delay(SPEAK_DELAY));

				int text_id = TXT_LOW_POWER;
				char const * text = Text_String(TXT_LOW_POWER);
				if (text != NULL) {
					On_Message(text, 35.0f, text_id);
				}
			}
		}
	}

	/*
	**	If there is a flag associated with this house, then mark it to be
	**	redrawn.
	*/
	if (Target_Legal(FlagLocation)) {
		UnitClass * unit = As_Unit(FlagLocation);
		if (unit) {
			unit->Mark(MARK_CHANGE);
		} else {
			CELL cell = As_Cell(FlagLocation);
			Map[cell].Redraw_Objects();
		}
	}

	bool is_time = false;

	/*
	**	Triggers are only checked every so often. If the trigger timer has expired,
	**	then set the trigger processing flag.
	*/
	if (TriggerTime.Expired()) {
		is_time = true;
		TriggerTime = TICKS_PER_MINUTE/10;
	}

	/*
	**	Check to see if the ion cannon should be removed from the sidebar
	**	because of outside circumstances. The advanced communications facility
	**	being destroyed is a good example of this.
	*/
	if (IonCannon.Is_Present()) {
		if (!(ActiveBScan & STRUCTF_EYE) && !IonCannon.Is_One_Time()) {

			/*
			**	Remove the ion cannon when there is no advanced communication facility.
			**	Note that this will not remove the one time created ion cannon.
			*/
			if (IonCannon.Remove()) {
				if (this == PlayerPtr) Map.Column[1].Flag_To_Redraw();
				IsRecalcNeeded = true;
			}
		} else {

			/*
			**	Turn the ion cannon suspension on or off depending on the available
			**	power. Note that one time ion cannon will not be affected by this.
			*/
			IonCannon.Suspend(Power_Fraction() < 0x0100);

			/*
			**	Process the ion cannon AI and if something changed that would affect
			**	the sidebar, then flag the sidebar to be redrawn.
			*/
			if (IonCannon.AI(this == PlayerPtr)) {
				if (this == PlayerPtr) Map.Column[1].Flag_To_Redraw();
			}
		}

		/*
		**	The computer may decide to fire the ion cannon if it is ready.
		*/
		if (IonCannon.Is_Ready() && !IsHuman) {
			Special_Weapon_AI(SPC_ION_CANNON);
		}

	} else {

		/*
		**	If there is no ion cannon present, but there is an advanced communcation
		**	center available, then make the ion cannon available as well.
		*/
		if ((GameToPlay == GAME_NORMAL || Rule.AllowSuperWeapons) &&
			(ActiveBScan & STRUCTF_EYE) &&
			(ActLike == HOUSE_GOOD || GameToPlay != GAME_NORMAL) &&
			(IsHuman || GameToPlay != GAME_NORMAL)) {

			IonCannon.Enable(false, this == PlayerPtr, Power_Fraction() < 0x0100);

			/*
			**	Flag the sidebar to be redrawn if necessary.
			*/
			// Add to Glyphx multiplayer sidebar. ST - 3/22/2019 1:50PM
			if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
				if (IsHuman) {
					Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_ION_CANNON, this);
				}
			} else {
				if (this == PlayerPtr) {
					Map.Add(RTTI_SPECIAL, SPC_ION_CANNON);
					Map.Column[1].Flag_To_Redraw();
				}
			}
		}
	}

	/*
	**	Check to see if the nuke strike should be removed from the sidebar
	**	because of outside circumstances. The Temple of Nod
	**	being destroyed is a good example of this.
	*/
	if (NukeStrike.Is_Present()) {
		if (!(ActiveBScan & STRUCTF_TEMPLE) && (!NukeStrike.Is_One_Time() || GameToPlay == GAME_NORMAL)) {

			/*
			**	Remove the nuke strike when there is no Temple of Nod.
			**	Note that this will not remove the one time created nuke strike.
			*/
			if (NukeStrike.Remove(true)) {
				IsRecalcNeeded = true;
				if (this == PlayerPtr) Map.Column[1].Flag_To_Redraw();
			}
		} else {

			/*
			**	Turn the nuke strike suspension on or off depending on the available
			**	power. Note that one time nuke strike will not be affected by this.
			*/
			NukeStrike.Suspend(Power_Fraction() < 0x0100);

			/*
			**	Process the nuke strike AI and if something changed that would affect
			**	the sidebar, then flag the sidebar to be redrawn.
			*/
			if (NukeStrike.AI(this == PlayerPtr)) {
				if (this == PlayerPtr) Map.Column[1].Flag_To_Redraw();
			}
		}

		/*
		**	The computer may decide to fire the nuclear missile if it is ready.
		*/
		if (NukeStrike.Is_Ready() && !IsHuman) {
			Special_Weapon_AI(SPC_NUCLEAR_BOMB);
		}

	} else {

		/*
		**	If there is no nuke strike present, but there is a Temple of Nod
		**	available, then make the nuke strike strike available.
		*/
		if ((GameToPlay == GAME_NORMAL || Rule.AllowSuperWeapons) && (ActiveBScan & STRUCTF_TEMPLE) && Has_Nuke_Device() && IsHuman) {
			NukeStrike.Enable((GameToPlay == GAME_NORMAL), this == PlayerPtr);

			/*
			**	Flag the sidebar to be redrawn if necessary.
			*/
			// Add to Glyphx multiplayer sidebar. ST - 3/22/2019 1:50PM
			if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
				if (IsHuman) {
					Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_NUCLEAR_BOMB, this);
				}
			} else {
				if (this == PlayerPtr) {
					Map.Add(RTTI_SPECIAL, SPC_NUCLEAR_BOMB);
					Map.Column[1].Flag_To_Redraw();
				}
			}
		}
	}

	/*
	**	Process the airstrike AI and if something changed that would affect
	**	the sidebar, then flag the sidebar to be redrawn.
	*/
	if (AirStrike.Is_Present()) {
		if (AirStrike.AI(this == PlayerPtr)) {
			if (this == PlayerPtr) Map.Column[1].Flag_To_Redraw();
		}

		/*
		**	The computer may decide to call in the airstrike if it is ready.
		*/
		if (AirStrike.Is_Ready() && !IsHuman) {
			Special_Weapon_AI(SPC_AIR_STRIKE);
		}
	}

	/*
	**	Add the airstrike option if it is pending.
	*/
	if (IsAirstrikePending) {
		IsAirstrikePending = false;
		if (AirStrike.Enable(false, this == PlayerPtr)) {
			AirStrike.Forced_Charge(this == PlayerPtr);
			// Add to Glyphx multiplayer sidebar. ST - 3/22/2019 1:50PM
			if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
				if (IsHuman) {
					Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_AIR_STRIKE, this);
				}
			} else {
				if (this == PlayerPtr) {
					Map.Add(RTTI_SPECIAL, SPC_AIR_STRIKE);
					Map.Column[1].Flag_To_Redraw();
				}
			}
		}
	}

#ifdef NEVER
	/*
	** The following logic deals with the nuclear warhead state machine.  It
	** handles all the different stages of the temple firing and the rocket
	** travelling up and down.  The rocket explosion is handled by the anim
	** which is attached to the bullet.
	*/
	if (!IsHuman && NukePresent) {
		Special_Weapon_AI(SPC_NUCLEAR_BOMB);

	}
#endif

	if (GameToPlay != GAME_NORMAL) {
		Check_Pertinent_Structures();
	}

	/*
	** Special win/lose check for multiplayer games; by-passes the
	** trigger system.  We must wait for non-zero frame, because init
	** may not properly set IScan etc for each house; you have to go
	** through each object's AI before it will be properly set.
	*/
	if (GameToPlay != GAME_NORMAL && !IsDefeated &&
		!ActiveBScan && !ActiveAScan && !UScan && !ActiveIScan && Frame > 0) {
		MPlayer_Defeated();
	}

	for (int index = 0; index < HouseTriggers[Class->House].Count(); index++) {
		TriggerClass * t = HouseTriggers[Class->House][index];

		/*
		**	Check for just built the building trigger event.
		*/
		if (JustBuilt != STRUCT_NONE) {
			if (t->Spring(EVENT_BUILD, Class->House, JustBuilt)) {
				JustBuilt = STRUCT_NONE;
				continue;
			}
		}

		/*
		**	Check for civilian evacuation trigger event.
		*/
		if (IsCivEvacuated && t->Spring(EVENT_EVAC_CIVILIAN, Class->House)) {
			continue;
		}

		/*
		**	Number of buildings destroyed checker.
		*/
		if (t->Spring(EVENT_NBUILDINGS_DESTROYED, Class->House, BuildingsLost)) {
			continue;
		}

		/*
		**	Number of units destroyed checker.
		*/
		if (t->Spring(EVENT_NUNITS_DESTROYED, Class->House, UnitsLost)) {
			continue;
		}

		/*
		**	House has been revealed trigger event.
		*/
		if (IsDiscovered && t->Spring(EVENT_HOUSE_DISCOVERED, Class->House)) {
			IsDiscovered = false;
			continue;
		}

		/*
		**	The "all destroyed" triggers are only processed after a certain
		**	amount of safety time has expired.
		*/
		if (!EndCountDown) {

			/*
			**	All buildings destroyed checker.
			*/
			if (!ActiveBScan) {
				if (t->Spring(EVENT_BUILDINGS_DESTROYED, Class->House)) {
					continue;
				}
			}

			/*
			**	All units destroyed checker.
			*/
			if (!((ActiveUScan & ~(UNITF_GUNBOAT)) | IScan | (ActiveAScan & ~(AIRCRAFTF_TRANSPORT|AIRCRAFTF_CARGO|AIRCRAFTF_A10)))) {
				if (t->Spring(EVENT_UNITS_DESTROYED, Class->House)) {
					continue;
				}
			}

			/*
			**	All buildings AND units destroyed checker.
			*/
			if (!(ActiveBScan | (ActiveUScan & ~(UNITF_GUNBOAT)) | IScan | (ActiveAScan & ~(AIRCRAFTF_TRANSPORT|AIRCRAFTF_CARGO|AIRCRAFTF_A10)))) {
				if (t->Spring(EVENT_ALL_DESTROYED, Class->House)) {
					continue;
				}
			}
		}

		/*
		**	Credit check.
		*/
		if (t->Spring(EVENT_CREDITS, Class->House, Credits)) {
			continue;
		}

		/*
		**	Timeout check.
		*/
		if (is_time && t->Spring(EVENT_TIME, Class->House)) {
			continue;
		}

		/*
		**	All factories destroyed check.
		*/
		if (!(BScan & (STRUCTF_AIRSTRIP|STRUCTF_HAND|STRUCTF_WEAP|STRUCTF_BARRACKS)) && t->Spring(EVENT_NOFACTORIES, Class->House)) {
			continue;
		}
	}

	/*
	**	If a radar facility is not present, but the radar is active, then turn the radar off.
	**	The radar also is turned off when the power gets below 100% capacity.
	*/
	if (PlayerPtr == this) {
		if (Map.Is_Radar_Active()) {
			if (BScan & (STRUCTF_RADAR|STRUCTF_EYE)) {
				if (Power_Fraction() < 0x0100) {
					Map.Radar_Activate(0);
				}
			} else {
				Map.Radar_Activate(0);
			}
		} else {
			if (BScan & (STRUCTF_RADAR|STRUCTF_EYE)) {
				if (Power_Fraction() >= 0x0100) {
					Map.Radar_Activate(1);
				}
			} else {
				if (Map.Is_Radar_Existing()) {
					Map.Radar_Activate(4);
				}
			}
		}
		if (!(BScan & (STRUCTF_RADAR|STRUCTF_EYE))) {
			Radar = RADAR_NONE;
		} else {
			Radar = (Map.Is_Radar_Active() || Map.Is_Radar_Activating()) ? RADAR_ON : RADAR_OFF;
		}
	}

	
	VisibleCredits.AI(false, this, true);


	/*
	** Copied from Red Alert for multiplayer AI. ST - 7/23/2019 3:02PM
	** 
	** 
	**	
	*/
#ifdef USE_RA_AI	
	if (GameToPlay == GAME_GLYPHX_MULTIPLAYER && IsHuman == false) {
		
		/*
		** Perform any expert system AI processing.
		*/
		if (IsBaseBuilding && AITimer == 0) {
			AITimer = Expert_AI();
		}

		if (!IsBaseBuilding && State == STATE_ENDGAME) {
			Fire_Sale();
			Do_All_To_Hunt();
		}

		AI_Building();
		AI_Unit();
		AI_Vessel();
		AI_Infantry();
		AI_Aircraft();
	}
#endif

	/*
	**	If the production possibilities need to be recalculated, then do so now. This must
	**	occur after the scan bits have been properly updated.
	*/
	if (PlayerPtr == this && IsRecalcNeeded) {
		IsRecalcNeeded = false;
		Map.Recalc();

#ifdef NEVER
		/*
		**	Remove the ion cannon if necessary.
		*/
		if (IonCannon.Is_Present() && !(BScan & STRUCTF_EYE)) {
			IonCannon.Remove();
		}

		/*
		**	Remove the nuclear bomb if necessary.
		*/
		if (NukeStrike.Is_Present() && !(BScan & STRUCTF_TEMPLE)) {
			NukeStrike.Remove();
		}
#endif

		/*
		**	This placement might affect any prerequisite requirements for construction
		**	lists. Update the buildable options accordingly.
		*/
		for (int index = 0; index < Buildings.Count(); index++) {
			BuildingClass * building = Buildings.Ptr(index);
			if (building && building->Owner() == Class->House) {

				building->Update_Specials();
				if (PlayerPtr == building->House) {
					building->Update_Buildables();
				}
			}
		}

		Recalculate_Placement_Distances();
	}
}


/***********************************************************************************************
 * HouseClass::Attacked -- Lets player know if base is under attack.                           *
 *                                                                                             *
 *    Call this function whenever a building is attacked (with malice). This function will     *
 *    then announce to the player that his base is under attack. It checks to make sure that   *
 *    this is referring to the player's house rather than the enemy's.                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Attacked(BuildingClass* source)
{
	Validate();
	if (SpeakAttackDelay.Expired() && PlayerPtr->Class->House == Class->House) {
		Speak(VOX_BASE_UNDER_ATTACK, NULL, source ? source->Center_Coord() : 0);
		SpeakAttackDelay.Set(Options.Normalize_Delay(SPEAK_DELAY));

		/*
		**	If there is a trigger event associated with being attacked, process it
		**	now.
		*/
		for (int index = 0; index < HouseTriggers[Class->House].Count(); index++) {
			HouseTriggers[Class->House][index]->Spring(EVENT_ATTACKED, Class->House);
		}
	}
}


/***********************************************************************************************
 * HouseClass::Harvested -- Adds Tiberium to the harvest storage.                              *
 *                                                                                             *
 *    Use this routine whenever Tiberium is harvested. The Tiberium is stored equally between  *
 *    all storage capable buildings for the house. Harvested Tiberium adds to the credit       *
 *    value of the house, but only up to the maximum storage capacity that the house can       *
 *    currently maintain.                                                                      *
 *                                                                                             *
 * INPUT:   tiberium -- The number of Tiberium credits to add to the House's total.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Harvested(unsigned tiberium)
{
	Validate();
	long oldtib = Tiberium;

	Tiberium += tiberium;
	if (Tiberium > Capacity) {
		Tiberium = Capacity;
		IsMaxedOut = true;
	}
	HarvestedCredits += tiberium;
	Silo_Redraw_Check(oldtib, Capacity);
}


/***********************************************************************************************
 * HouseClass::Available_Money -- Fetches the total credit worth of the house.                 *
 *                                                                                             *
 *    Use this routine to determine the total credit value of the house. This is the sum of   *
 *    the harvested Tiberium in storage and the initial unspent cash reserves.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the total credit value of the house.                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
long HouseClass::Available_Money(void) const
{
	Validate();
	return(Tiberium + Credits);
}


/***********************************************************************************************
 * HouseClass::Spend_Money -- Removes money from the house.                                    *
 *                                                                                             *
 *    Use this routine to extract money from the house. Typically, this is a result of         *
 *    production spending. The money is extracted from available cash reserves first. When     *
 *    cash reserves are exhausted, then Tiberium is consumed.                                  *
 *                                                                                             *
 * INPUT:   money -- The amount of money to spend.                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/25/1995 JLB : Created.                                                                 *
 *   06/20/1995 JLB : Spends Tiberium before spending cash.                                    *
 *=============================================================================================*/
void HouseClass::Spend_Money(unsigned money)
{
	Validate();
	long oldtib = Tiberium;
	if ((int)money > Tiberium) {
		money -= (unsigned)Tiberium;
		Tiberium = 0;
		Credits -= money;
	} else {
		Tiberium -= money;
	}
	Silo_Redraw_Check(oldtib, Capacity);
	CreditsSpent += money;
}


/***********************************************************************************************
 * HouseClass::Refund_Money -- Refunds money to back to the house.                             *
 *                                                                                             *
 *    Use this routine when money needs to be refunded back to the house. This can occur when  *
 *    construction is aborted. At this point, the exact breakdown of Tiberium or initial       *
 *    credits used for the orignal purchase is lost. Presume as much of the money is in the    *
 *    form of Tiberium as storage capacity will allow.                                         *
 *                                                                                             *
 * INPUT:   money -- The number of credits to refund back to the house.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/25/1995 JLB : Created.                                                                 *
 *   06/01/1995 JLB : Refunded money is never lost                                             *
 *=============================================================================================*/
void HouseClass::Refund_Money(unsigned money)
{
	Validate();
	Credits += money;
}


/***********************************************************************************************
 * HouseClass::Adjust_Capacity -- Adjusts the house Tiberium storage capacity.                 *
 *                                                                                             *
 *    Use this routine to adjust the maximum storage capacity for the house. This storage      *
 *    capacity will limit the number of Tiberium credits that can be stored at any one time.   *
 *                                                                                             *
 * INPUT:   adjust   -- The adjustment to the Tiberium storage capacity.                       *
 *                                                                                             *
 *          inanger  -- Is this a forced adjustment to capacity due to some hostile event?     *
 *                                                                                             *
 * OUTPUT:  Returns with the number of Tiberium credits lost.                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::Adjust_Capacity(int adjust, bool inanger)
{
	Validate();
	long oldcap = Capacity;
	int retval = 0;

	Capacity += adjust;
	Capacity = MAX(Capacity, 0L);
	if (Tiberium > Capacity) {
		retval = Tiberium - Capacity;
		Tiberium = Capacity;
		if (!inanger) {
			Refund_Money(retval);
			retval = 0;
		} else {
			IsMaxedOut = true;
		}
	}
	Silo_Redraw_Check(Tiberium, oldcap);
	return(retval);
}


/***********************************************************************************************
 * HouseClass::Silo_Redraw_Check -- Flags silos to be redrawn if necessary.                    *
 *                                                                                             *
 *    Call this routine when either the capacity or tiberium levels change for a house. This   *
 *    routine will determine if the aggregate tiberium storage level will result in the        *
 *    silos changing their imagery. If this is detected, then all the silos for this house     *
 *    are flagged to be redrawn.                                                               *
 *                                                                                             *
 * INPUT:   oldtib   -- Pre-change tiberium level.                                             *
 *                                                                                             *
 *          oldcap   -- Pre-change tiberium storage capacity.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Silo_Redraw_Check(long oldtib, long oldcap)
{
	Validate();
	int oldratio = 0;
	if (oldcap) oldratio = (oldtib * 5) / oldcap;
	int newratio = 0;
	if (Capacity) newratio = (Tiberium * 5) / Capacity;

	if (oldratio != newratio) {
		for (int index = 0; index < Buildings.Count(); index++) {
			BuildingClass * b = Buildings.Ptr(index);
			if (b && !b->IsInLimbo && b->House == this && *b == STRUCT_STORAGE) {
				b->Mark(MARK_CHANGE);
			}
		}
	}
}


/***********************************************************************************************
 * HouseClass::Read_INI -- Reads house specific data from INI.                                 *
 *                                                                                             *
 *    This routine reads the house specific data for a particular                              *
 *    scenario from the scenario INI file. Typical data includes starting                      *
 *    credits, maximum unit count, etc.                                                        *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to loaded scenario INI file.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1994 JLB : Created.                                                                 *
 *   05/18/1995 JLB : Creates all houses.                                                      *
 *=============================================================================================*/
void HouseClass::Read_INI(char *buffer)
{
	HouseClass 	*p;				// Pointer to current player data.
	char const	*hname;			//	Pointer to house name.
	char			buf[128];

	for (HousesType index = HOUSE_FIRST; index < HOUSE_COUNT; index++) {
		hname = HouseTypeClass::As_Reference(index).IniName;
		int maxunit = WWGetPrivateProfileInt(hname, "MaxUnit", EACH_UNIT_MAX, buffer);

		maxunit = MAX(maxunit, 150);

		int maxbuilding = WWGetPrivateProfileInt(hname, "MaxBuilding", EACH_BUILDING_MAX, buffer);

		maxbuilding = MAX(maxbuilding, 150);

		int credits = WWGetPrivateProfileInt(hname, "Credits", 0, buffer);

		p = new HouseClass(index);

		p->MaxBuilding = maxbuilding;
		p->MaxUnit = maxunit;
		p->Credits = (long)credits * 100;
		p->InitialCredits = p->Credits;
		WWGetPrivateProfileString(hname, "Edge", "", buf, sizeof(buf)-1, buffer);
		p->Edge = Source_From_Name(buf);
		if (p->Edge == SOURCE_NONE) {
			p->Edge = SOURCE_NORTH;
		}

		if (GameToPlay == GAME_NORMAL) {
			WWGetPrivateProfileString(hname, "Allies", "", buf, sizeof(buf)-1, buffer);
			if (strlen(buf)) {
				char * tok = strtok(buf, ", \t");
				while (tok) {
					HousesType h = HouseTypeClass::From_Name(tok);
					p->Make_Ally(h);
					tok = strtok(NULL, ", \t");
				}

			} else {

				/*
				**	Special case for when no allies are specified in the INI file.
				**	The GDI side defaults to considering the neutral side to be
				**	friendly.
				*/
				if (p->Class->House == HOUSE_GOOD) {
					p->Make_Ally(HOUSE_NEUTRAL);
				}
			}
		}
	}
}


/***********************************************************************************************
 * HouseClass::Write_INI -- Writes house specific data into INI file.                          *
 *                                                                                             *
 *    Use this routine to write the house specific data (for all houses) into the INI file.    *
 *    It is used by the scenario editor when saving the scenario.                              *
 *                                                                                             *
 * INPUT:   buffer   -- INI file staging buffer.                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Write_INI(char *buffer)
{
	for (HousesType i = HOUSE_FIRST; i < HOUSE_COUNT; i++) {
		HouseClass * p = As_Pointer(i);

		if (p) {
			WWWritePrivateProfileInt(p->Class->IniName, "Credits", (int)(p->Credits / 100), buffer);
			WWWritePrivateProfileString(p->Class->IniName, "Edge", Name_From_Source(p->Edge), buffer);
			WWWritePrivateProfileInt(p->Class->IniName, "MaxUnit", p->MaxUnit, buffer);
			WWWritePrivateProfileInt(p->Class->IniName, "MaxBuilding", p->MaxBuilding, buffer);

			bool first = true;
			char sbuffer[100] = "";
			for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
				if (p->Is_Ally(house)) {
					if (!first) strcat(sbuffer, ",");
					strcat(sbuffer, As_Pointer(house)->Class->IniName);
					first = false;
				}
			}
			WWWritePrivateProfileString(p->Class->IniName, "Allies", sbuffer, buffer);
		}
	}
}


/***********************************************************************************************
 * HouseClass::Is_Ally -- Determines if the specified house is an ally.                        *
 *                                                                                             *
 *    This routine will determine if the house number specified is a ally to this house.       *
 *                                                                                             *
 * INPUT:   house -- The house number to check to see if it is an ally.                        *
 *                                                                                             *
 * OUTPUT:  Is the house an ally?                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Is_Ally(HousesType house) const
{
	Validate();
	if (house != HOUSE_NONE) {
		return(((1<<house) & Allies) != 0);
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::Is_Ally -- Determines if the specified house is an ally.                        *
 *                                                                                             *
 *    This routine will examine the specified house and determine if it is an ally.            *
 *                                                                                             *
 * INPUT:   house -- Pointer to the house object to check for ally relationship.               *
 *                                                                                             *
 * OUTPUT:  Is the specified house an ally?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Is_Ally(HouseClass const * house) const
{
	Validate();
	if (house) {
		return(Is_Ally(house->Class->House));
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::Is_Ally -- Checks to see if the object is an ally.                              *
 *                                                                                             *
 *    This routine will examine the specified object and return whether it is an ally or not.  *
 *                                                                                             *
 * INPUT:   object   -- The object to examine to see if it is an ally.                         *
 *                                                                                             *
 * OUTPUT:  Is the specified object an ally?                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Is_Ally(ObjectClass const * object) const
{
	Validate();
	if (object) {
		return(Is_Ally(object->Owner()));
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::Make_Ally -- Make the specified house an ally.                                  *
 *                                                                                             *
 *    This routine will make the specified house an ally to this house. An allied house will   *
 *    not be considered a threat or potential target.                                          *
 *                                                                                             *
 * INPUT:   house -- The house to make an ally of this house.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *   08/08/1995 JLB : Breaks off combat when ally commences.                                   *
 *=============================================================================================*/
void HouseClass::Make_Ally(HousesType house)
{
	Validate();
	if (house != HOUSE_NONE && !Is_Ally(house)) {

		/*
		**	If in normal game play but the house is defeated, then don't allow the ally
		**	key to work.
		*/
		if (!ScenarioInit && (IsDefeated || house == HOUSE_JP)) return;

		Allies |= (1 << house);

#ifdef CHEAT_KEYS
		if (Debug_Flag) {
			HouseClass * enemy = HouseClass::As_Pointer(house);
			if (enemy && !enemy->Is_Ally(this)) {
				enemy->Make_Ally(Class->House);
			}
		}
#endif

		if ((Debug_Flag || GameToPlay != GAME_NORMAL) && !ScenarioInit) {
			char buffer[80];

			/*
			**	Sweep through all techno objects and perform a cheeseball tarcom clear to ensure
			**	that fighting will most likely stop when the cease fire begins.
			*/
			for (int index = 0; index < Logic.Count(); index++) {
				ObjectClass * object = Logic[index];

				if (object && !object->IsInLimbo && object->Owner() == Class->House) {
					TARGET target = ((TechnoClass *)object)->TarCom;
					if (Target_Legal(target) && As_Techno(target)) {
						if (Is_Ally(As_Techno(target))) {
							((TechnoClass *)object)->TarCom = TARGET_NONE;
						}
					}
				}
			}

			sprintf(buffer, Text_String(TXT_HAS_ALLIED), Name, HouseClass::As_Pointer(house)->Name);
			Messages.Add_Message(buffer, MPlayerTColors[RemapColor], TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, 1200, 0, 0);
			Map.Flag_To_Redraw(false);
		}
	}
}


/***********************************************************************************************
 * HouseClass::Make_Enemy -- Make an enemy of the house specified.                             *
 *                                                                                             *
 *    This routine will flag the house specified so that it will be an enemy to this house.    *
 *    Enemy houses are legal targets for attack.                                               *
 *                                                                                             *
 * INPUT:   house -- The house to make an enemy of this house.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *   07/27/1995 JLB : Making war is a bilateral aaction.                                       *
 *=============================================================================================*/
void HouseClass::Make_Enemy(HousesType house)
{
	Validate();
	if (house != HOUSE_NONE && Is_Ally(house)) {
		HouseClass * enemy = HouseClass::As_Pointer(house);
		Allies &= ~(1 << house);
		if (enemy && enemy->Is_Ally(this)) {
			enemy->Allies &= ~(1 << Class->House);
		}

		if ((Debug_Flag || GameToPlay != GAME_NORMAL) && !ScenarioInit) {
			char buffer[80];

			sprintf(buffer, Text_String(TXT_AT_WAR), Name, enemy->Name);
			Messages.Add_Message(buffer, MPlayerTColors[RemapColor], TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, 600, 0, 0);
			Map.Flag_To_Redraw(false);
		}
	}
}


/***********************************************************************************************
 * HouseClass::Remap_Table -- Fetches the remap table for this house object.                   *
 *                                                                                             *
 *    This routine will return with the remap table to use when displaying an object owned     *
 *    by this house. If the object is blushing (flashing), then the lightening remap table is  *
 *    always used. The "unit" parameter allows proper remap selection for those houses that    *
 *    have a different remap table for buildings or units.                                     *
 *                                                                                             *
 * INPUT:   blushing -- Is the object blushing (flashing)?                                     *
 *                                                                                             *
 *          unit     -- Is the object a vehicle or infantry?                                   *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the remap table to use when drawing this object.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
unsigned char const * HouseClass::Remap_Table(bool blushing, bool unit) const
{
	Validate();
	if (blushing) return(&Map.FadingLight[0]);

	/*
	** For normal game play, return the TypeClass's remap table for this
	** house type
	*/
	if (GameToPlay == GAME_NORMAL) {
		/*
		**	Special case exception for Nod and single player only. Remap
		**	buildings to red as opposed to the default color of bluegrey.
		*/
		if (!unit && Class->House == HOUSE_BAD) {
			return(RemapRed);
		}

		/*
		**	Special case Jurassic missions to use the bluegrey remapping
		*/
		if (Special.IsJurassic && Class->House == HOUSE_MULTI4) {
			return(RemapLtBlue);
		}

		return(Class->RemapTable);
	} else {

		/*
		** For multiplayer games, return the remap table for this exact house instance.
		*/
		return(RemapTable);
	}
}


/***********************************************************************************************
 * HouseClass::Suggested_New_Team -- Determine what team should be created.                    *
 *                                                                                             *
 *    This routine examines the house condition and returns with the team that it thinks       *
 *    should be created. The units that are not currently a member of a team are examined      *
 *    to determine the team needed.                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the team type that should be created. If no team should  *
 *          be created, then NULL is returned.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TeamTypeClass const * HouseClass::Suggested_New_Team(bool alertcheck)
{
	Validate();
	return(TeamTypeClass::Suggested_New_Team(this, UScan, IScan, IsAlerted && alertcheck));
}


/***********************************************************************************************
 * HouseClass::Adjust_Threat -- Adjust threat for the region specified.                        *
 *                                                                                             *
 *    This routine is called when the threat rating for a region needs to change. The region   *
 *    and threat adjustment are provided.                                                      *
 *                                                                                             *
 * INPUT:   region   -- The region that adjustment is to occur on.                             *
 *                                                                                             *
 *          threat   -- The threat adjustment to perform.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Adjust_Threat(int region, int threat)
{
	Validate();
	static int _val[] = {
		-MAP_REGION_WIDTH - 1,	-MAP_REGION_WIDTH, -MAP_REGION_WIDTH + 1,
		-1,							0,						 1,
		MAP_REGION_WIDTH -1,		MAP_REGION_WIDTH,	 MAP_REGION_WIDTH +1
	};
	static int _thr[] = {
		2, 1, 2,
		1, 0, 1,
		2, 1,	2
	};
	int neg;
	int *val = &_val[0];
	int *thr = &_thr[0];

	if (threat < 0) {
		threat = -threat;
		neg = true;
	} else {
		neg = false;
	}

	for (int lp = 0; lp < 9; lp ++) {
		Regions[region + *val].Adjust_Threat(threat >> *thr, neg);
		val++;
		thr++;
	}
}


/***********************************************************************************************
 * HouseClass::Begin_Production -- Starts production of the specified object type.             *
 *                                                                                             *
 *    This routine is called from the event system. It will start production for the object    *
 *    type specified. This will be reflected in the sidebar as well as the house factory       *
 *    tracking variables.                                                                      *
 *                                                                                             *
 * INPUT:   type  -- The type of object to begin production on.                                *
 *                                                                                             *
 *          id    -- The subtype of object.                                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the reason why, or why not, production was started.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ProdFailType HouseClass::Begin_Production(RTTIType type, int id)
{
	Validate();
	int * factory = 0;
	int result = true;
	bool initial_start = false;
	FactoryClass * fptr;
	TechnoTypeClass const * tech = Fetch_Techno_Type(type, id);

	switch (type) {
		case RTTI_AIRCRAFT:
		case RTTI_AIRCRAFTTYPE:
			factory = &AircraftFactory;
			break;

		case RTTI_UNIT:
		case RTTI_UNITTYPE:
			factory = &UnitFactory;
			break;

		case RTTI_BUILDING:
		case RTTI_BUILDINGTYPE:
			factory = &BuildingFactory;
			break;

		case RTTI_INFANTRY:
		case RTTI_INFANTRYTYPE:
			factory = &InfantryFactory;
			break;

		case RTTI_SPECIAL:
			factory = &SpecialFactory;
			break;
	}

	/*
	**	Check for legality of the production object type suggested.
	*/
	if (!factory) return(PROD_ILLEGAL);

	/*
	**	If the house is already busy producing the requested object, then
	**	return with this failure code, unless we are restarting production.
	*/
	if (*factory != -1) {
		fptr = Factories.Raw_Ptr(*factory);
		if (fptr->Is_Building())
			return(PROD_CANT);
	} else {
		fptr = new FactoryClass();
		if (!fptr) return(PROD_CANT);
		*factory = Factories.ID(fptr);
		result = (tech) ? fptr->Set(*tech, *this) : fptr->Set(id, *this);
		initial_start = true;

		/*
		** If set failed, we probably reached the production cap. Don't let the factory linger, preventing further production attempts.
		** ST - 3/17/2020 2:03PM
		*/
		if (!result) {
			delete fptr;
			fptr = NULL;
			*factory = -1;
		}
	}

	if (result) {
		fptr->Start();

		/*
		**	Link this factory to the sidebar so that proper graphic feedback
		**	can take place.
		*/
		// Handle Glyphx multiplayer sidebar. ST - 3/26/2019 1:27PM
		if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
			if (IsHuman) {
				Sidebar_Glyphx_Factory_Link(*factory, type, id, this);
			}
		} else {		 
			if (PlayerPtr == this) {
				Map.Factory_Link(*factory, type, id);
			}
		}

		return(PROD_OK);
	}
	return(PROD_CANT);
}


/***********************************************************************************************
 * HouseClass::Suspend_Production -- Temporarily puts production on hold.                      *
 *                                                                                             *
 *    This routine is called from the event system whenever the production of the specified    *
 *    type needs to be suspended. The suspended production will be reflected in the sidebar    *
 *    as well as in the house control structure.                                               *
 *                                                                                             *
 * INPUT:   type  -- The type of object that production is being suspended for.                *
 *                                                                                             *
 * OUTPUT:  Returns why, or why not, production was suspended.                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ProdFailType HouseClass::Suspend_Production(RTTIType type)
{
	Validate();
	int * factory = 0;

	switch (type) {
		case RTTI_AIRCRAFT:
		case RTTI_AIRCRAFTTYPE:
			factory = &AircraftFactory;
			break;

		case RTTI_UNIT:
		case RTTI_UNITTYPE:
			factory = &UnitFactory;
			break;

		case RTTI_BUILDING:
		case RTTI_BUILDINGTYPE:
			factory = &BuildingFactory;
			break;

		case RTTI_INFANTRY:
		case RTTI_INFANTRYTYPE:
			factory = &InfantryFactory;
			break;

		case RTTI_SPECIAL:
			factory = &SpecialFactory;
			break;
	}

	/*
	**	Check for legality of the production object type suggested.
	*/
	if (!factory) return(PROD_ILLEGAL);

	/*
	**	If the house is already busy producing the requested object, then
	**	return with this failure code.
	*/
	if (*factory == -1) return(PROD_CANT);

	/*
	**	Create the factory pointer object.
	**	If the factory could not be created, then report this error condition.
	*/
	FactoryClass * fptr = Factories.Raw_Ptr(*factory);
	if (!fptr) return(PROD_CANT);

	/*
	**	Actually suspend the production.
	*/
	fptr->Suspend();

	/*
	**	Tell the sidebar that it needs to be redrawn because of this.
	*/
	if (PlayerPtr == this) {
		Map.SidebarClass::IsToRedraw = true;
		Map.SidebarClass::Column[0].IsToRedraw = true;
		Map.SidebarClass::Column[1].IsToRedraw = true;
		Map.Flag_To_Redraw(false);
	}

	return(PROD_OK);
}


/***********************************************************************************************
 * HouseClass::Abandon_Production -- Abandons production of item type specified.               *
 *                                                                                             *
 *    This routine is called from the event system whenever production must be abandoned for   *
 *    the type specified. This will remove the factory and pending object from the sidebar as  *
 *    well as from the house factory record.                                                   *
 *                                                                                             *
 * INPUT:   type  -- The object type that production is being suspended for.                   *
 *                                                                                             *
 * OUTPUT:  Returns the reason why or why not, production was suspended.                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ProdFailType HouseClass::Abandon_Production(RTTIType type)
{
	Validate();
	int * factory = 0;

	switch (type) {
		case RTTI_AIRCRAFT:
		case RTTI_AIRCRAFTTYPE:
			factory = &AircraftFactory;
			break;

		case RTTI_UNIT:
		case RTTI_UNITTYPE:
			factory = &UnitFactory;
			break;

		case RTTI_BUILDING:
		case RTTI_BUILDINGTYPE:
			factory = &BuildingFactory;
			break;

		case RTTI_INFANTRY:
		case RTTI_INFANTRYTYPE:
			factory = &InfantryFactory;
			break;

		case RTTI_SPECIAL:
			factory = &SpecialFactory;
			break;
	}

	/*
	**	Check for legality of the production object type suggested.
	*/
	if (!factory) return(PROD_ILLEGAL);

	/*
	**	If there is no factory to abandon, then return with a failure code.
	*/
	if (*factory == -1) return(PROD_CANT);

	/*
	**	Fetch the factory pointer object.
	*/
	FactoryClass * fptr = Factories.Raw_Ptr(*factory);
	if (!fptr) return(PROD_CANT);

	/*
	**	Tell the sidebar that it needs to be redrawn because of this.
	*/
	// Handle Glyphx multiplayer sidebar. ST - 3/22/2019 2:01PM
	if (GameToPlay == GAME_GLYPHX_MULTIPLAYER) {
		if (IsHuman) {
			Sidebar_Glyphx_Abandon_Production(type, *factory, this);
			// Need to clear pending object here?
		}
	} else {

		if (PlayerPtr == this) {
			Map.Abandon_Production(type, *factory);

			if (type == RTTI_BUILDINGTYPE || type == RTTI_BUILDING) {
				Map.PendingObjectPtr = 0;
				Map.PendingObject = 0;
				Map.PendingHouse = HOUSE_NONE;
				Map.Set_Cursor_Shape(0);
			}
		}
	}

	/*
	**	Abandon production of the object.
	*/
	fptr->Abandon();
	delete fptr;
	*factory = -1;

	return(PROD_OK);
}


/***********************************************************************************************
 * HouseClass::Special_Weapon_AI -- Fires special weapon.                                      *
 *                                                                                             *
 *    This routine will pick a good target to fire the special weapon specified.               *
 *                                                                                             *
 * INPUT:   id -- The special weapon id to fire.                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Special_Weapon_AI(SpecialWeaponType id)
{
	Validate();
	/*
	** Loop through all of the building objects on the map
	** and see which ones are available.
 	*/
	BuildingClass * bestptr = NULL;
	int best = -1;

	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass * b = Buildings.Ptr(index);

		/*
		** If the building is valid, not in limbo, not in the process of
		** being destroyed and not our ally, then we can consider it.
		*/
		if (b && !b->IsInLimbo && b->Strength && !Is_Ally(b)) {
			if (b->Value() > best || best == -1) {
				best = b->Value();
				bestptr = b;
			}
		}
	}

	if (bestptr) {
		CELL cell = Coord_Cell(bestptr->Center_Coord());
		Place_Special_Blast(id, cell);
	}
}


/***********************************************************************************************
 * HouseClass::Place_Special_Blast -- Place a special blast effect at location specified.      *
 *                                                                                             *
 *    This routine will create a blast effect at the cell specified. This is the result of     *
 *    the special weapons.                                                                     *
 *                                                                                             *
 * INPUT:   id    -- The special weapon id number.                                             *
 *                                                                                             *
 *          cell  -- The location where the special weapon attack is to occur.                 *
 *                                                                                             *
 * OUTPUT:  Was the special weapon successfully fired at the location specified?               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : commented.                                                               *
 *   07/25/1995 JLB : Added scatter effect for nuclear bomb.                                   *
 *   07/28/1995 JLB : Revamped to use super weapon class controller.                           *
 *=============================================================================================*/
bool HouseClass::Place_Special_Blast(SpecialWeaponType id, CELL cell)
{
	Validate();
	BuildingClass * launchsite = 0;
	AnimClass * anim = 0;
	
	// Added. ST - 12/2/2019 11:26AM
	bool fired = false;
	const char *what = NULL;
	
	int index;
	switch (id) {

		case SPC_ION_CANNON:
			if (IonCannon.Is_Ready()) {
				anim = new AnimClass(ANIM_ION_CANNON, Cell_Coord(cell));
				if (anim) anim->Owner = Class->House;
				if (this == PlayerPtr) {
					Map.IsTargettingMode = false;
				}
				IonCannon.Discharged(PlayerPtr == this);
				IsRecalcNeeded = true;
				fired = true;
				what = "ION";
			}
			break;

		case SPC_NUCLEAR_BOMB:
			if (NukeStrike.Is_Ready()) {


#ifdef NEVER
				/*
				**	Scatter the nuclear bomb impact point into an adjacent cell.
				*/
				for (;;) {
					CELL newcell = Adjacent_Cell(cell, Random_Pick(FACING_N, FACING_COUNT));
					if (Map.In_Radar(newcell)) {
						cell = newcell;
						break;
					}
				}
#endif

				/*
				**	Search for a suitable launch site for this missile.
				*/
				for (index = 0; index < Buildings.Count(); index++) {
					BuildingClass * b = Buildings.Ptr(index);
					if (b && !b->IsInLimbo && b->House == this && *b == STRUCT_TEMPLE) {
						launchsite = b;
						break;
					}
				}

				/*
				**	If a launch site was found, then proceed with the normal launch
				**	sequence.
				*/
				if (launchsite) {
					launchsite->Assign_Mission(MISSION_MISSILE);
					launchsite->Commence();
					NukeDest = cell;
					NukePieces = 0;

				} else {

					/*
					**	Only in the multiplayer version can the nuclear bomb be
					**	sent from some off screen source.
					*/
					if (GameToPlay == GAME_NORMAL) return(false);

					/*
					**	Since no launch site was found, just bring the missile in
					**	directly from the North map edge.
					*/
				 	BulletClass *bullet = new BulletClass(BULLET_NUKE_DOWN);
					if (bullet) {
						COORDINATE start = Cell_Coord(XY_Cell(Cell_X(cell), 0));
						bullet->Assign_Target(::As_Target(cell));
						bullet->Payback = NULL;
						bullet->Strength = 1;
						if (!bullet->Unlimbo(start, DIR_S)) {
							delete bullet;
						} else {
							bullet->PrimaryFacing.Set_Current(DIR_S);
						}
						Speak(VOX_INCOMING_NUKE); // "Nuclear Warhead Approaching" - "NUKE1"
						Sound_Effect(VOC_NUKE_FIRE, start);
					}
				}
				if (this == PlayerPtr) {
					Map.IsTargettingMode = false;
				}
				NukeStrike.Discharged(this == PlayerPtr);
				IsRecalcNeeded = true;
				fired = true;
				what = "NUKE";
			}
			break;

		case SPC_AIR_STRIKE:
			if (AirStrike.Is_Ready()) {
				int strike = 1;
				if (GameToPlay == GAME_NORMAL) {
					strike = Bound(BuildLevel/3, 1, 3);
				} else {
					strike = Bound(MPlayerUnitCount/5, 1, 3);
				}
				Create_Air_Reinforcement(this, AIRCRAFT_A10, strike, MISSION_HUNT, ::As_Target(cell), TARGET_NONE);
				if (this == PlayerPtr) {
					Map.IsTargettingMode = false;
				}
				AirStrike.Discharged(this == PlayerPtr);
				IsRecalcNeeded = true;
				fired = true;
				what = "AIR";
			}
			break;
	}

	/*
	** Maybe trigger an achivement. ST - 12/2/2019 11:25AM
	*/
	if (IsHuman && fired && what) {
		On_Achievement_Event(this, "SUPERWEAPON_FIRED", what);
	}

	return(true);
}


/***********************************************************************************************
 * HouseClass::Place_Object -- Places the object (building) at location specified.             *
 *                                                                                             *
 *    This routine is called when a building has been produced and now must be placed on       *
 *    the map. When the player clicks on the map, this routine is ultimately called when the   *
 *    event passes through the event queue system.                                             *
 *                                                                                             *
 * INPUT:   type  -- The object type to place. The actual object is lifted from the sidebar.   *
 *                                                                                             *
 *                                                                                             *
 *          cell  -- The location to place the object on the map.                              *
 *                                                                                             *
 * OUTPUT:  Was the placement successful?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
extern void On_Ping(const HouseClass* player_ptr, COORDINATE coord);

bool HouseClass::Place_Object(RTTIType type, CELL cell)
{
	Validate();
	TechnoClass * tech = 0;
	FactoryClass * factory = 0;

	switch (type) {
		case RTTI_AIRCRAFT:
		case RTTI_AIRCRAFTTYPE:
			if (AircraftFactory != -1) {
				factory = Factories.Raw_Ptr(AircraftFactory);
			}
			break;

		case RTTI_INFANTRY:
		case RTTI_INFANTRYTYPE:
			if (InfantryFactory != -1) {
				factory = Factories.Raw_Ptr(InfantryFactory);
			}
			break;

		case RTTI_UNIT:
		case RTTI_UNITTYPE:
			if (UnitFactory != -1) {
				factory = Factories.Raw_Ptr(UnitFactory);
			}
			break;

		case RTTI_BUILDING:
		case RTTI_BUILDINGTYPE:
			if (BuildingFactory != -1) {
				factory = Factories.Raw_Ptr(BuildingFactory);
			}
			break;
	}

	/*
	**	Only if there is a factory active for this type, can it be "placed".
	**	In the case of a missing factory, then this request is completely bogus --
	**	ignore it. This might occur if, between two events to exit the same
	**	object, the mouse was clicked on the sidebar to start building again.
	**	The second placement event should NOT try to place the object that is
	**	just starting construction.
	*/
	if (factory && factory->Has_Completed()) {
		tech = factory->Get_Object();

		if (cell == -1) {
			TechnoClass * pending = factory->Get_Object();
			if (pending) {
				TechnoClass * builder = pending->Who_Can_Build_Me(false, false);
				TechnoTypeClass const *object_type = pending->Techno_Type_Class();

				if (builder && builder->Exit_Object(pending)) {

					/*
					**	Since the object has left the factory under its own power, delete
					**	the production manager tied to this slot in the sidebar. Its job
					**	has been completed.
					*/
					factory->Set_Is_Blocked(false);
					factory->Completed();
					Abandon_Production(type);

					/*
					** Could be tied to an achievement. ST - 11/11/2019 11:56AM
					*/
					if (IsHuman) {
						if (object_type) {
							On_Achievement_Event(this, "UNIT_CONSTRUCTED", object_type->IniName);
						}
						if (pending->IsActive) {
							On_Ping(this, pending->Center_Coord());
						}
					}

				} else {

					/*
					**	The object could not leave under it's own power. Just wait
					**	until the player tries to place the object again.
					*/
					
					/*
					** Flag that it's blocked so we can re-try the exit later.
					** This would have been a bad idea under the old peer-peer code since it would have pumped events into
					** the queue too often. ST - 2/25/2020 11:56AM
					*/
					factory->Set_Is_Blocked(true);
					return(false);
				}
			}

		} else {

			if (tech) {
				TechnoClass * builder = tech->Who_Can_Build_Me(false, false);
				if (builder) {

					/*
					**	Ensures that the proximity check is performed even when the building is
					**	placed by way of a remote event.
					*/
					if (tech->What_Am_I() != RTTI_BUILDING || ((BuildingClass *)tech)->Passes_Proximity_Check(cell)) {
						builder->Transmit_Message(RADIO_HELLO, tech);
						if (tech->Unlimbo(Cell_Coord(cell))) {
							factory->Completed();
							Abandon_Production(type);

							if (PlayerPtr == this) {
								Sound_Effect(VOC_SLAM);
								Map.Set_Cursor_Shape(0);
								Map.PendingObjectPtr = 0;
								Map.PendingObject = 0;
								Map.PendingHouse = HOUSE_NONE;
							}
							return(true);
						} else {
							if (this == PlayerPtr) {
								Speak(VOX_DEPLOY);
							}
						}
						builder->Transmit_Message(RADIO_OVER_OUT);
					}
				}
				return(false);

			} else {

				// Play a bad sound here?
				return(false);
			}
		}
	}

	return(true);
}


/***********************************************************************************************
 * HouseClass::Manual_Place -- Inform display system of building placement mode.               *
 *                                                                                             *
 *    This routine will inform the display system that building placement mode has begun.      *
 *    The cursor will be created that matches the layout of the building shape.                *
 *                                                                                             *
 * INPUT:   builder  -- The factory that is building this object.                              *
 *                                                                                             *
 *          object   -- The building that is going to be placed down on the map.               *
 *                                                                                             *
 * OUTPUT:  Was the building placement mode successfully initiated?                            *
 *                                                                                             *
 * WARNINGS:   This merely adjusts the cursor shape. Nothing that affects networked games      *
 *             is affected.                                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/04/1995 JLB : Created.                                                                 *
 *   05/30/1995 JLB : Uses the Bib_And_Offset() function to determine bib size.                *
 *=============================================================================================*/
bool HouseClass::Manual_Place(BuildingClass * builder,  BuildingClass * object)
{
	Validate();
	if (this == PlayerPtr && !Map.PendingObject && builder && object) {

		/*
		**	Ensures that object selection doesn't remain when
		**	building placement takes place.
		*/
		Unselect_All();

		Map.Repair_Mode_Control(0);
		Map.Sell_Mode_Control(0);

		Map.PendingObject = object->Class;
		Map.PendingObjectPtr = object;
		Map.PendingHouse = Class->House;

		Map.Set_Cursor_Shape(object->Occupy_List(true));
		Map.Set_Cursor_Pos(Coord_Cell(builder->Coord));
		builder->Mark(MARK_CHANGE);
		return(true);
	}
	return(false);
}


#ifdef OBSOLETE
/***********************************************************************************************
 * HouseClass::Init_Ion_Cannon -- Initialize the ion cannon countdown.                         *
 *                                                                                             *
 *    This routine will initiate the ion cannon charging countdown. It will add the ion        *
 *    cannon to the sidebar if it isn't there and it is specified to be added.                 *
 *                                                                                             *
 * INPUT:   first_time  -- Set to true if the ion cannon must be added to the sidebar.         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : commented                                                                *
 *=============================================================================================*/
void HouseClass::Init_Ion_Cannon(SpecialControlType control)
{
	Validate();
	switch (control) {
		case CONTROL_RESET:
			if (IonCannonPresent) {
				IonOldStage = -1;
				IonControl.Set(ION_CANNON_GONE_TIME);
				if (PlayerPtr == this) {
					Map.Add(RTTI_SPECIAL, SPC_ION_CANNON);
					if (!ScenarioInit) {
						Speak(VOX_ION_CHARGING);
					}
				}
			}
			break;

		/*
		**	Adds the special no-prerequisite ion cannon option.
		*/
		case CONTROL_ONE_TIME:
			if (!IonCannonPresent) {
				Init_Ion_Cannon(CONTROL_ADD);
				IonOneTimeFlag = true;
			}
			break;

		/*
		**	Adds the normal legitimate ion cannon option. If there was
		**	already a one-time ion cannon available, the charging state
		**	is NOT interrupted.
		*/
		case CONTROL_ADD:
			IonOneTimeFlag = false;
			if (!IconCannonPresent) {
				IonCannonPresent = true;
				IonReady = false;
				Init_Ion_Cannon(CONTROL_RESET);
			}
			break;

		case CONTROL_REMOVE:
			break;
	}


	if (!(first_time && IonCannonPresent)) {

		if (IonCannonPresent && IonOneTimeFlag) {
			IonOneTimeFlag = false;
			if (this == PlayerPtr) Map.Recalc();
			return;
		}

		if (first_time && this == PlayerPtr) {
			Map.Add(RTTI_SPECIAL, SPC_ION_CANNON);
		}

		if (!ScenarioInit) {
			if (this == PlayerPtr) {
				Speak(VOX_ION_CHARGING);
			}
		}

		IonControl.Set(ION_CANNON_GONE_TIME);
		IonCannonPresent = true;
		IonReady = false;
		IonOldStage = -1;
		IonOneTimeFlag = one_time_effect;
	} else {
	 	if (first_time && IonCannonPresent && !one_time_effect && IonOneTimeFlag) {
			IonOneTimeFlag	= false;
		}
	}
}
#ifdef NEVER
void HouseClass::Init_Ion_Cannon(bool first_time, bool one_time_effect)
{
	Validate();
	if (!(first_time && IonCannonPresent)) {

		if (IonCannonPresent && IonOneTimeFlag) {
			IonOneTimeFlag = false;
			if (this == PlayerPtr) Map.Recalc();
			return;
		}

		if (first_time && this == PlayerPtr) {
			Map.Add(RTTI_SPECIAL, SPC_ION_CANNON);
		}

		if (!ScenarioInit) {
			if (this == PlayerPtr) {
				Speak(VOX_ION_CHARGING);
			}
		}

		IonControl.Set(ION_CANNON_GONE_TIME);
		IonCannonPresent = true;
		IonReady = false;
		IonOldStage = -1;
		IonOneTimeFlag = one_time_effect;
	} else {
	 	if (first_time && IonCannonPresent && !one_time_effect && IonOneTimeFlag) {
			IonOneTimeFlag	= false;
		}
	}
}
#endif


/***********************************************************************************************
 * HouseClass::Remove_Ion_Cannon -- Disables the ion cannon.                                   *
 *                                                                                             *
 *    This routine will disable the ion cannon. It is called when the ion cannon cannot        *
 *    establish a command link to the ground (usually when there is insufficient power).       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : commented                                                                *
 *=============================================================================================*/
void HouseClass::Remove_Ion_Cannon(void)
{
	Validate();
	if (IonCannonPresent) {
		IonCannonPresent = false;
		IonOneTimeFlag = false;
		IonReady = false;
		IonControl.Clear();
		IonOldStage = -1;
	}
}
#endif


/***************************************************************************
 * HouseClass::Clobber_All -- removes house & all its objects              *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/16/1995 BRR : Created.                                             *
 *   06/09/1995 JLB : Handles aircraft.                                    *
 *=========================================================================*/
void HouseClass::Clobber_All(void)
{
	Validate();
	int i;

	for (i = 0; i < ::Aircraft.Count(); i++) {
		if (::Aircraft.Ptr(i)->House == this) {
			delete ::Aircraft.Ptr(i);
			i--;
		}
	}
	for (i = 0; i < ::Units.Count(); i++) {
		if (::Units.Ptr(i)->House == this) {
			delete ::Units.Ptr(i);
			i--;
		}
	}
	for (i = 0; i < Infantry.Count(); i++) {
		if (Infantry.Ptr(i)->House == this) {
			delete Infantry.Ptr(i);
			i--;
		}
	}
	for (i = 0; i < Buildings.Count(); i++) {
		if (Buildings.Ptr(i)->House == this) {
			delete Buildings.Ptr(i);
			i--;
		}
	}
	for (i = 0; i < TeamTypes.Count(); i++) {
		if (TeamTypes.Ptr(i)->House == Class->House) {
			delete TeamTypes.Ptr(i);
			i--;
		}
	}
	for (i = 0; i < Triggers.Count(); i++) {
		if (Triggers.Ptr(i)->House == Class->House) {
			delete Triggers.Ptr(i);
			i--;
		}
	}

	delete this;
}


#ifdef NEVER
/***********************************************************************************************
 * HouseClass::Init_Nuke_Bomb -- Adds (if necessary) the atom bomb to the sidebar.             *
 *                                                                                             *
 *    Use this routine whenever a piece of atom bomb has been discovered (also at scenario     *
 *    start). It will add the nuclear bomb button to the sidebar if necessary.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : commented                                                                *
 *=============================================================================================*/
void HouseClass::Init_Nuke_Bomb(bool first_time, bool one_time_effect)
{
	Validate();
	if (!first_time || !NukePresent) {

		if (NukePresent && NukeOneTimeFlag) {
			NukeOneTimeFlag = false;
			if (this == PlayerPtr) Map.Recalc();
			return;
		}

		if (first_time && this == PlayerPtr) {
			Map.Add(RTTI_SPECIAL, SPC_NUCLEAR_BOMB);
		}

		NukeControl.Set(NUKE_GONE_TIME);
		NukePresent = true;
		NukeReady = false;
		NukeOldStage = -1;
		NukeOneTimeFlag = one_time_effect;

	} else {
		if (!one_time_effect && NukeOneTimeFlag) {
			NukeOneTimeFlag = false;
		}
	}
}


/***********************************************************************************************
 * HouseClass::Remove_Nuke_Bomb -- Removes the nuclear bomb from the sidebar.                  *
 *                                                                                             *
 *    This routine will remove the nuclear bomb from the sidebar. It should be called when     *
 *    the nuclear strike has been launched.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : commented                                                                *
 *   07/25/1995 JLB : Handles recharge reset logic.                                            *
 *=============================================================================================*/
void HouseClass::Remove_Nuke_Bomb(void)
{
	Validate();
	if (NukePresent && !NukeOneTimeFlag) {
		NukePresent = false;
		NukeControl.Clear();
		NukeOldStage = -1;
		NukeReady = false;
	}
}


/***********************************************************************************************
 * HouseClass::Init_Air_Strike -- Add (or reset) the air strike sidebar button.                *
 *                                                                                             *
 *    This routine will activate (add if so indicated) the air strike button to the sidebar.   *
 *    Call this routine when events indicate that a special air strike is available.           *
 *                                                                                             *
 * INPUT:   first_time  -- If the air strike button is to be added, then this will be true.    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : commented                                                                *
 *=============================================================================================*/
void HouseClass::Init_Air_Strike(bool first_time, bool one_time_effect)
{
	Validate();
	if (!(first_time && AirPresent)) {

		if (AirPresent && AirOneTimeFlag) {
			AirOneTimeFlag = false;
			AirPresent		= false;
			Map.Recalc();
			return;
		}

		if (first_time) {
			if (PlayerPtr == this) {
				Map.Add(RTTI_SPECIAL, SPC_AIR_STRIKE);
			}
			AirControl.Set(0);
		} else {
			AirControl.Set(AIR_CANNON_GONE_TIME);
		}

		AirReady 			= first_time;
		AirPresent 			= true;
		AirOldStage 		= -1;
		AirOneTimeFlag		= one_time_effect;

		if (AirReady && !IsHuman) {
			Special_Weapon_AI(SPC_AIR_STRIKE);
		}
	} else {
	 	if (first_time && AirPresent && !one_time_effect && AirOneTimeFlag) {
			AirOneTimeFlag	= false;
		}
	}
}


/***********************************************************************************************
 * HouseClass::Remove_Air_Strike -- Removes the air strike button from the sidebar.            *
 *                                                                                             *
 *    This routine will remove the air strike button from the sidebar. Call this routine when  *
 *    the air strike has been launched.                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : commented                                                                *
 *=============================================================================================*/
void HouseClass::Remove_Air_Strike(void)
{
	Validate();
	AirPresent = false;
	AirReady = false;
	AirControl.Clear();
	AirOldStage = -1;
}


/***********************************************************************************************
 * HouseClass::Make_Air_Strike_Available -- Make the airstrike available.                      *
 *                                                                                             *
 *    This routine will make the airstrike available. Typically, this results in a new icon    *
 *    added to the sidebar.                                                                    *
 *                                                                                             *
 * INPUT:   present  -- The the airstrike being added?                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Make_Air_Strike_Available(bool present, bool one_time_effect)
{
	Validate();
	Init_Air_Strike(true, one_time_effect);
	AirPresent = present;
}
#endif


/***********************************************************************************************
 * HouseClass::Add_Nuke_Piece -- Add a nuclear piece to the collection.                        *
 *                                                                                             *
 *    This routine will a the specified nuclear piece to the house collection of parts. When   *
 *    all the pieces have been added, a nuclear strike ability is made available.              *
 *                                                                                             *
 * INPUT:   piece -- The nuclear piece to add. If equal to "-1", then the next possible piece  *
 *                   is added.                                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Add_Nuke_Piece(int piece)
{
	Validate();
	if (piece == -1) {
		piece = 1;
		if (!(NukePieces & 0x01)) {
			piece = 1;
		}
		if (!(NukePieces & 0x02)) {
			piece = 2;
		}
		if (!(NukePieces & 0x04)) {
			piece = 3;
		}
	}
	NukePieces |= 1 << (piece - 1);
//	Init_Nuke_Bomb(false);
}


/***********************************************************************************************
 * HouseClass::Detach -- Removes specified object from house tracking systems.                 *
 *                                                                                             *
 *    This routine is called when an object is to be removed from the game system. If the      *
 *    specified object is part of the house tracking system, then it will be removed.          *
 *                                                                                             *
 * INPUT:   target   -- The target value of the object that is to be removed from the game.    *
 *                                                                                             *
 *          all      -- Is the target going away for good as opposed to just cloaking/hiding?  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : commented                                                                *
 *=============================================================================================*/
void HouseClass::Detach(TARGET, bool )
{
	Validate();
//	if (LaunchSite == target) {
//		LaunchSite = TARGET_NONE;
//	}
}


/***********************************************************************************************
 * HouseClass::Does_Enemy_Building_Exist -- Checks for enemy building of specified type.       *
 *                                                                                             *
 *    This routine will examine the enemy houses and if there is a building owned by one       *
 *    of those house, true will be returned.                                                   *
 *                                                                                             *
 * INPUT:   btype -- The building type to check for.                                           *
 *                                                                                             *
 * OUTPUT:  Does a building of the specified type exist for one of the enemy houses?           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Does_Enemy_Building_Exist(StructType btype) const
{
	Validate();
	int bflag = 1L << btype;
	for (HousesType index = HOUSE_FIRST; index < HOUSE_COUNT; index++) {
		HouseClass * house = HouseClass::As_Pointer(index);

		if (house && !Is_Ally(house) && (house->BScan & bflag) != 0) {
			return(true);
		}
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::Suggest_New_Object -- Determine what would the next buildable object be.        *
 *                                                                                             *
 *    This routine will examine the house status and return with a techno type pointer to      *
 *    the object type that it thinks should be created. The type is restricted to match the    *
 *    type specified. Typical use of this routine is by computer controlled factories.         *
 *                                                                                             *
 * INPUT:   objecttype  -- The type of object to restrict the scan for.                        *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a techno type for the object type that should be         *
 *          created. If no object should be created, then NULL is returned.                    *
 *                                                                                             *
 * WARNINGS:   This is a time consuming routine. Only call when necessary.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TechnoTypeClass const * HouseClass::Suggest_New_Object(RTTIType objecttype) const
{
	Validate();
	TechnoTypeClass const * techno = NULL;

#ifdef USE_RA_AI
	//
	// Copied from RA for AI. ST - 7/25/2019 3:58PM
	//
	if (!IsHuman && GameToPlay != GAME_NORMAL) {
		switch (objecttype) {
			case RTTI_AIRCRAFT:
			case RTTI_AIRCRAFTTYPE:
				if (BuildAircraft != AIRCRAFT_NONE) {
					return(&AircraftTypeClass::As_Reference(BuildAircraft));
				}
				return(NULL);

			/*
			**	Unit construction is based on the rule that up to twice the number required
			**	to fill all teams will be created.
			*/
			case RTTI_UNIT:
			case RTTI_UNITTYPE:
				if (BuildUnit != UNIT_NONE) {
					return(&UnitTypeClass::As_Reference(BuildUnit));
				}
				return(NULL);

			/*
			**	Infantry construction is based on the rule that up to twice the number required
			**	to fill all teams will be created.
			*/
			case RTTI_INFANTRY:
			case RTTI_INFANTRYTYPE:
				if (BuildInfantry != INFANTRY_NONE) {
					return(&InfantryTypeClass::As_Reference(BuildInfantry));
				}
				return(NULL);

			/*
			**	Building construction is based upon the preconstruction list.
			*/
			case RTTI_BUILDING:
			case RTTI_BUILDINGTYPE:
				if (BuildStructure != STRUCT_NONE) {
					return(&BuildingTypeClass::As_Reference(BuildStructure));
				}
				return(NULL);
		}

		return NULL;
	}

#endif //USE_RA_AI



	switch (objecttype) {

		/*
		**	Unit construction is based on the rule that up to twice the number required
		**	to fill all teams will be created.
		*/
		case RTTI_UNIT:
		case RTTI_UNITTYPE:
			if (CurUnits < MaxUnit) {

				/*
				**	A computer controlled house will try to build a replacement
				**	harvester if possible. Never replace harvesters if the game
				**	is in easy mode.
				*/
				if (!(GameToPlay == GAME_NORMAL && PlayerPtr->Difficulty == DIFF_EASY) && !IsHuman && (ActiveBScan & STRUCTF_REFINERY) && !(UScan & UNITF_HARVESTER)) {
					techno = &UnitTypeClass::As_Reference(UNIT_HARVESTER);
					if (techno->Scenario <= BuildLevel) break;
					techno = 0;
				}

				int counter[UNIT_COUNT];
				if (GameToPlay == GAME_NORMAL) {
					memset(counter, 0x00, sizeof(counter));
				} else {
					for (UnitType index = UNIT_FIRST; index < UNIT_COUNT; index++) {
						if (Can_Build(index, Class->House) && UnitTypeClass::As_Reference(index).Level <= BuildLevel) {
							counter[index] = 16;
						} else {
							counter[index] = 0;
						}
					}
				}

				/*
				**	Build a list of the maximum of each type we wish to produce. This will be
				**	twice the number required to fill all teams.
				*/
				int index;
				for (index = 0; index < Teams.Count(); index++) {
					TeamClass * tptr = Teams.Ptr(index);
					if (tptr) {
						TeamTypeClass const * team = tptr->Class;

						if ((/*team->IsReinforcable || */!tptr->IsFullStrength) && team->House == Class->House) {
							for (int subindex = 0; subindex < team->ClassCount; subindex++) {
								if (team->Class[subindex]->What_Am_I() == RTTI_UNITTYPE) {
									counter[((UnitTypeClass const *)(team->Class[subindex]))->Type] = 1;
//									counter[((UnitTypeClass const *)(team->Class[subindex]))->Type] += team->DesiredNum[subindex]*2;
								}
							}
						}
					}
				}

				/*
				**	Team types that are flagged as prebuilt, will always try to produce enough
				**	to fill one team of this type regardless of whether there is a team active
				**	of that type.
				*/
				for (index = 0; index < TeamTypes.Count(); index++) {
					TeamTypeClass const * team = TeamTypes.Ptr(index);
					if (team) {
						if (team->House == Class->House && team->IsPrebuilt && (!team->IsAutocreate || IsAlerted)) {
							for (int subindex = 0; subindex < team->ClassCount; subindex++) {
								if (team->Class[subindex]->What_Am_I() == RTTI_UNITTYPE) {
									int subtype = ((UnitTypeClass const *)(team->Class[subindex]))->Type;
									counter[subtype] = MAX(counter[subtype], (int)team->DesiredNum[subindex]);
								}
							}
						}
					}
				}

				/*
				**	Reduce the theoretical maximum by the actual number of objects currently
				**	in play.
				*/
				for (int uindex = 0; uindex < Units.Count(); uindex++) {
					UnitClass * unit = Units.Ptr(uindex);
					if (unit && !unit->Team && unit->House == this && (unit->Mission != MISSION_GUARD_AREA && unit->Mission != MISSION_HUNT && unit->Mission != MISSION_STICKY && unit->Mission != MISSION_SLEEP)) {
						counter[unit->Class->Type]--;
					}
				}

				/*
				**	Pick to build the most needed object but don't consider those object that
				**	can't be built because of scenario restrictions or insufficient cash.
				*/
				int bestval = -1;
				int bestcount = 0;
				UnitType bestlist[UNIT_COUNT];
				for (UnitType utype = UNIT_FIRST; utype < UNIT_COUNT; utype++) {
					if (counter[utype] > 0 && Can_Build(utype, Class->House) && UnitTypeClass::As_Reference(utype).Cost_Of() <= Available_Money()) {
						if (bestval == -1 || bestval < counter[utype]) {
							bestval = counter[utype];
							bestcount = 0;
						}
						bestlist[bestcount++] = utype;
					}
				}

				/*
				**	The unit type to build is now known. Fetch a pointer to the techno type class.
				*/
				if (bestcount) {
					techno = &UnitTypeClass::As_Reference(bestlist[Random_Pick(0, bestcount-1)]);
				}
			}
			break;

		/*
		**	Infantry construction is based on the rule that up to twice the number required
		**	to fill all teams will be created.
		*/
		case RTTI_INFANTRY:
		case RTTI_INFANTRYTYPE:
			if (CurUnits < MaxUnit) {
				int counter[INFANTRY_COUNT];
				if (GameToPlay == GAME_NORMAL) {
					memset(counter, 0x00, sizeof(counter));
				} else {
					for (InfantryType index = INFANTRY_FIRST; index < INFANTRY_COUNT; index++) {
						if (Can_Build(index, Class->House) && InfantryTypeClass::As_Reference(index).Level <= BuildLevel) {
							counter[index] = 16;
						} else {
							counter[index] = 0;
						}
					}
				}

				/*
				**	Build a list of the maximum of each type we wish to produce. This will be
				**	twice the number required to fill all teams.
				*/
				int index;
				for (index = 0; index < Teams.Count(); index++) {
					TeamClass * tptr = Teams.Ptr(index);
					if (tptr) {
						TeamTypeClass const * team = tptr->Class;

						if ((team->IsReinforcable || !tptr->IsFullStrength) && team->House == Class->House) {
							for (int subindex = 0; subindex < team->ClassCount; subindex++) {
								if (team->Class[subindex]->What_Am_I() == RTTI_INFANTRYTYPE) {
									counter[((InfantryTypeClass const *)(team->Class[subindex]))->Type] += team->DesiredNum[subindex]+1;
								}
							}
						}
					}
				}

				/*
				**	Team types that are flagged as prebuilt, will always try to produce enough
				**	to fill one team of this type regardless of whether there is a team active
				**	of that type.
				*/
				for (index = 0; index < TeamTypes.Count(); index++) {
					TeamTypeClass const * team = TeamTypes.Ptr(index);
					if (team) {
						if (team->House == Class->House && team->IsPrebuilt && (!team->IsAutocreate || IsAlerted)) {
							for (int subindex = 0; subindex < team->ClassCount; subindex++) {
								if (team->Class[subindex]->What_Am_I() == RTTI_INFANTRYTYPE) {
									int subtype = ((InfantryTypeClass const *)(team->Class[subindex]))->Type;
//									counter[subtype] = 1;
									counter[subtype] = MAX(counter[subtype], (int)team->DesiredNum[subindex]);
									counter[subtype] = MIN(counter[subtype], 5);
								}
							}
						}
					}
				}

				/*
				**	Reduce the theoretical maximum by the actual number of objects currently
				**	in play.
				*/
				for (int uindex = 0; uindex < Infantry.Count(); uindex++) {
					InfantryClass * infantry = Infantry.Ptr(uindex);
					if (infantry && !infantry->Team && infantry->House == this && (infantry->Mission != MISSION_GUARD_AREA && infantry->Mission != MISSION_HUNT && infantry->Mission != MISSION_STICKY && infantry->Mission != MISSION_SLEEP)) {
						counter[infantry->Class->Type]--;
					}
				}

				/*
				**	Pick to build the most needed object but don't consider those object that
				**	can't be built because of scenario restrictions or insufficient cash.
				*/
				int bestval = -1;
				int bestcount = 0;
				InfantryType bestlist[INFANTRY_COUNT];
				for (InfantryType utype = INFANTRY_FIRST; utype < INFANTRY_COUNT; utype++) {
					if (counter[utype] > 0 && Can_Build(utype, Class->House) && InfantryTypeClass::As_Reference(utype).Cost_Of() <= Available_Money()) {
						if (bestval == -1 || bestval < counter[utype]) {
							bestval = counter[utype];
							bestcount = 0;
						}
						bestlist[bestcount++] = utype;
					}
				}

				/*
				**	The infantry type to build is now known. Fetch a pointer to the techno type class.
				*/
				if (bestcount) {
					techno = &InfantryTypeClass::As_Reference(bestlist[Random_Pick(0, bestcount-1)]);
				}
			}
			break;

		/*
		**	Building construction is based upon the preconstruction list.
		*/
		case RTTI_BUILDING:
		case RTTI_BUILDINGTYPE:
			if (CurBuildings < MaxBuilding) {
				BaseNodeClass * node = Base.Next_Buildable();
				if (node) {
					techno = &BuildingTypeClass::As_Reference(node->Type);
				}
			}
			break;
	}
	return(techno);
}


/***********************************************************************************************
 * HouseClass::Flag_Remove -- Removes the flag from the specified target.                      *
 *                                                                                             *
 *    This routine will remove the flag attached to the specified target object or cell.       *
 *    Call this routine before placing the object down. This is called inherently by the       *
 *    the Flag_Attach() functions.                                                             *
 *                                                                                             *
 * INPUT:   target   -- The target that the flag was attached to but will be removed from.     *
 *                                                                                             *
 *          set_home -- if true, clears the flag's waypoint designation                        *
 *                                                                                             *
 * OUTPUT:  Was the flag successfully removed from the specified target?                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Flag_Remove(TARGET target, bool set_home)
{
	Validate();
	bool rc = false;

	if (Target_Legal(target)) {

		/*
		**	Remove the flag from a unit
		*/
		UnitClass * object = As_Unit(target);
		if (object) {
			rc = object->Flag_Remove();
			if (rc && FlagLocation == target) {
				FlagLocation = TARGET_NONE;
			}

		} else {

			/*
			**	Remove the flag from a cell
			*/
			CELL cell = As_Cell(target);
			if (Map.In_Radar(cell)) {
				rc = Map[cell].Flag_Remove();
				if (rc && FlagLocation == target) {
					FlagLocation = TARGET_NONE;
				}
			}
		}

		/*
		**	Handle the flag home cell:
		**	If 'set_home' is set, clear the home value & the cell's overlay
		*/
		if (set_home) {
			if (FlagHome) {
				Map[FlagHome].Overlay = OVERLAY_NONE;
				Map.Flag_Cell(FlagHome);
				FlagHome = 0;
			}
		}
	}
	return(rc);
}


/***********************************************************************************************
 * HouseClass::Flag_Attach -- Attach flag to specified cell (or thereabouts).                  *
 *                                                                                             *
 *    This routine will attach the house flag to the location specified. If the location       *
 *    cannot contain the flag, then a suitable nearby location will be selected.               *
 *                                                                                             *
 * INPUT:   cell  -- The desired cell location to place the flag.                              *
 *                                                                                             *
 *          set_home -- if true, resets the flag's waypoint designation                        *
 *                                                                                             *
 * OUTPUT:  Was the flag successfully placed?                                                  *
 *                                                                                             *
 * WARNINGS:   The cell picked for the flag might very likely not be the cell requested.       *
 *             Check the FlagLocation value to determine the final cell resting spot.          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Flag_Attach(CELL cell, bool set_home)
{
	Validate();
	bool rc;
	bool clockwise;
	FacingType rot;
	FacingType fcounter;

	/*
	**	Randomly decide if we're going to search cells clockwise or counter-
	**	clockwise
	*/
	clockwise = IRandom(0,1);

	/*
	**	Only continue if this cell is a legal placement cell.
	*/
	if (Map.In_Radar(cell)) {

		/*
		**	If the flag already exists, then it must be removed from the object
		**	it is attached to.
		*/
		Flag_Remove(FlagLocation, set_home);

		/*
		**	Attach the flag to the cell specified. If it can't be placed, then pick
		**	a nearby cell where it can be placed.
		*/
		CELL newcell = cell;
		rc = Map[newcell].Flag_Place(Class->House);
		if (!rc) {

			/*
			**	Loop for increasing distance from the desired cell.
			**	For each distance, randomly pick a starting direction.  Between
			**	this and the clockwise/counterclockwise random value, the flag
			**	should appear to be placed fairly randomly.
			*/
			for (int dist = 1; dist < 32; dist++) {

				/*
				**	Clockwise search.
				*/
				if (clockwise) {
					rot = (FacingType)IRandom(FACING_N, FACING_NW);
					for (fcounter = FACING_N; fcounter <= FACING_NW; fcounter++) {
						newcell = Coord_Cell(Coord_Move(Cell_Coord(cell), Facing_Dir(rot), dist*256));
						if (Map.In_Radar(newcell) && Map[newcell].Flag_Place(Class->House)) {
							dist = 32;
							rc = true;
							break;
						}
						rot++;
						if (rot > FACING_NW) rot = FACING_N;
					}
				} else {

					/*
					**	Counter-clockwise search
					*/
					rot = (FacingType)IRandom (FACING_N, FACING_NW);
					for (fcounter = FACING_NW; fcounter >= FACING_N; fcounter--) {
						newcell = Coord_Cell(Coord_Move(Cell_Coord(cell), Facing_Dir(rot), dist*256));
						if (Map.In_Radar(newcell) && Map[newcell].Flag_Place(Class->House)) {
							dist = 32;
							rc = true;
							break;
						}
						rot--;
						if (rot < FACING_N)
							rot = FACING_NW;
					}
				}
			}
		}

		/*
		**	If we've found a spot for the flag, place the flag at the new cell.
		**	if 'set_home' is set, OR this house has no current flag home cell,
		**	mark that cell as this house's flag home cell. Otherwise fall back
		**	on returning the flag to its home.
		*/
		if (rc) {
			FlagLocation = As_Target(newcell);

			if (set_home || FlagHome == 0) {
				Map[newcell].Overlay = OVERLAY_FLAG_SPOT;
				FlagHome = newcell;
			}
		}
		else if (FlagHome != 0) {
			rc = Map[FlagHome].Flag_Place(Class->House);
		}

		return(rc);
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::Flag_Attach -- Attaches the house flag the specified unit.                      *
 *                                                                                             *
 *    This routine will attach the house flag to the specified unit. This routine is called    *
 *    when a unit drives over a cell containing a flag.                                        *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that the house flag is to be attached to.        *
 *                                                                                             *
 *          set_home -- if true, clears the flag's waypoint designation                        *
 *                                                                                             *
 * OUTPUT:  Was the flag attached successfully?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Flag_Attach(UnitClass * object, bool set_home)
{
	Validate();
	if (object && !object->IsInLimbo) {
		Flag_Remove(FlagLocation, set_home);

		/*
		**	Attach the flag to the object.
		*/
		object->Flag_Attach(Class->House);
		FlagLocation = object->As_Target();
		return(true);
	}
	return(false);
}

extern void On_Defeated_Message(const char* message, float timeout_seconds);

/***************************************************************************
 * HouseClass::MPlayer_Defeated -- multiplayer; house is defeated          *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/25/1995 BRR : Created.                                             *
 *=========================================================================*/
void HouseClass::MPlayer_Defeated(void)
{
	Validate();
	char txt[80];
	int i,j,k;
	unsigned char id;
	HousesType house;
	HouseClass *hptr;
	HouseClass *hptr2;
	int num_alive;
	int num_humans;
	int all_allies;
	int max_index;
	int max_count;
	int count;
	int score_index[MAX_PLAYERS];	// array of each multi-player's index into
											// the score array

	/*------------------------------------------------------------------------
	Set the defeat flag for this house
	------------------------------------------------------------------------*/
	IsDefeated = true;

#ifdef USE_RA_AI
	/*
	** Moved in from RA for AI. ST - 7/24/2019 4:02PM
	*/
	/*
	**	If this is a computer controlled house, then all computer controlled
	**	houses become paranoid.
	*/
	if (IQ == Rule.MaxIQ && !IsHuman && Rule.IsComputerParanoid) {
		Computer_Paranoid();
	}
#endif // USE_RA_AI

	/*------------------------------------------------------------------------
	Remove this house's flag & flag home cell
	------------------------------------------------------------------------*/
	if (Special.IsCaptureTheFlag) {
		if (FlagLocation) {
			Flag_Remove(FlagLocation,true);
		} else {
			if (FlagHome) {
				Flag_Remove(FlagHome,true);
			}
		}
	}

	/*------------------------------------------------------------------------
	If this is me:
	- Set MPlayerObiWan, so I can only send messages to all players, and
	  not just one (so I can't be obnoxiously omnipotent)
	- Reveal the map
	- Add my defeat message
	------------------------------------------------------------------------*/
	if (PlayerPtr == this) {
		MPlayerObiWan = 1;
		Debug_Unshroud = true;
		HiddenPage.Clear();
		Map.Flag_To_Redraw(true);

		/*.....................................................................
		Pop up a message showing that I was defeated
		.....................................................................*/
		sprintf(txt,Text_String(TXT_PLAYER_DEFEATED), MPlayerName);
		//Messages.Add_Message(txt, MPlayerTColors[MPlayerColorIdx], TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, 600, 0, 0);
		Map.Flag_To_Redraw(false);

		int timeout = 600;
		On_Defeated_Message(txt, timeout * 60.0f / TICKS_PER_MINUTE);
		//Sound_Effect(VOC_INCOMING_MESSAGE);

	} else {

		/*------------------------------------------------------------------------
		If it wasn't me, find out who was defeated
		------------------------------------------------------------------------*/
		if (IsHuman) {
			sprintf(txt, Text_String(TXT_PLAYER_DEFEATED), Text_String(TXT_UNKNOWN));
			id = 0;
			for (i = 0; i < MPlayerCount; i++) {
				house = MPlayerHouses[i];
				if (HouseClass::As_Pointer(house) == this) {
					sprintf (txt,Text_String(TXT_PLAYER_DEFEATED), MPlayerNames[i]);
					id = MPlayerID[i];
				}
			}

			Messages.Add_Message(txt, MPlayerTColors[MPlayerID_To_ColorIndex(id)],
				TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, 600, 0, 0);
			Map.Flag_To_Redraw(false);
		}
	}

	/*------------------------------------------------------------------------
	Find out how many players are left alive.
	------------------------------------------------------------------------*/
	num_alive = 0;
	num_humans = 0;
	for (i = 0; i < MPlayerMax; i++) {
		hptr = HouseClass::As_Pointer((HousesType)(HOUSE_MULTI1 + i));
		if (hptr && hptr->IsDefeated==0) {
			if (hptr->IsHuman)
				num_humans++;
			num_alive++;
		}
	}

	/*------------------------------------------------------------------------
	If all the houses left alive are allied with each other, then in reality
	there's only one player left:
	------------------------------------------------------------------------*/
	all_allies = 1;
	for (i = 0; i < MPlayerMax; i++) {
		/*.....................................................................
		Get a pointer to this house
		.....................................................................*/
		hptr = HouseClass::As_Pointer((HousesType)(HOUSE_MULTI1 + i));
		if (!hptr || hptr->IsDefeated)
			continue;

		/*.....................................................................
		Loop through all houses; if there's one left alive that this house
		isn't allied with, then all_allies will be false
		.....................................................................*/
		for (j = 0; j < MPlayerMax; j++) {
			hptr2 = HouseClass::As_Pointer((HousesType)(HOUSE_MULTI1 + j));
			if (!hptr2)
				continue;
			if (!hptr2->IsDefeated && !hptr->Is_Ally(hptr2)) {
				all_allies = 0;
				break;
			}
		}
		if (!all_allies)
			break;
	}
	/*........................................................................
	If all houses left are allies, set 'num_alive' to 1; game over.
	........................................................................*/
	if (all_allies)
		num_alive = 1;

	/*------------------------------------------------------------------------
	If there's only one human player left or no humans left, the game is over:
	- Determine whether this player wins or loses, based on the state of the
	  MPlayerObiWan flag
	- Find all players' indices in the MPlayerScore array
	- Tally up scores for this game
	------------------------------------------------------------------------*/
	if (num_alive == 1 || num_humans == 0) {
		if (PlayerPtr->IsDefeated) {
			PlayerLoses = true;
		} else {
			PlayerWins = true;
		}

		/*---------------------------------------------------------------------
		Find each player's score index
		---------------------------------------------------------------------*/
		for (i = 0; i < MPlayerCount; i++) {
			score_index[i] = -1;

			/*..................................................................
			Search for this player's name in the MPlayerScore array
			..................................................................*/
			for (j = 0; j < MPlayerNumScores; j++) {
				if (!stricmp(MPlayerNames[i],MPlayerScore[j].Name)) {
					score_index[i] = j;
					break;
				}
			}

			/*..................................................................
			If the index is still -1, the name wasn't found; add a new entry.
			..................................................................*/
			if (score_index[i] == -1) {
				if (MPlayerNumScores < MAX_MULTI_NAMES) {
					score_index[i] = MPlayerNumScores;
					MPlayerNumScores++;
				} else {

					/*...............................................................
					For each player in the scores array, count the # of '-1' entries
					from this game backwards; the one with the most is the one that
					hasn't played the longest; replace him with this new guy.
					...............................................................*/
					max_index = 0;
					max_count = 0;
					for (j = 0; j < MPlayerNumScores; j++) {
						count = 0;
						for (k = MPlayerNumScores - 1; k >= 0; k--) {
							if (MPlayerScore[j].Kills[k]==-1) {
								count++;
							} else {
								break;
							}
						}
						if (count > max_count) {
							max_count = count;
							max_index = j;
						}
					}
					score_index[i] = max_index;
				}

				/*...............................................................
				Initialize this score entry
				...............................................................*/
				MPlayerScore[score_index[i]].Wins = 0;
				strcpy (MPlayerScore[score_index[i]].Name,MPlayerNames[i]);
				for (j = 0; j < MAX_MULTI_GAMES; j++)
					MPlayerScore[score_index[i]].Kills[j] = -1;
			}

			/*..................................................................
			Init this player's Kills to 0 (-1 means he didn't play this round;
			0 means he played but got no kills).
			..................................................................*/
			MPlayerScore[score_index[i]].Kills[MPlayerCurGame] = 0;

			/*..................................................................
			Init this player's color to his last-used color index
			..................................................................*/
			MPlayerScore[score_index[i]].Color = MPlayerID_To_ColorIndex(MPlayerID[i]);
		}

#if 0	// (This is the old method of tallying scores:
		/*---------------------------------------------------------------------
		Tally up the scores for this game:
		- For each house:
		  - If this house is human & wasn't defeated, its the winner
		  - If this house was defeated, find out who did it & increment their
		    Kills value.
		---------------------------------------------------------------------*/
		for (house = HOUSE_MULTI1; house < (HOUSE_MULTI1 + MPlayerMax); house++) {
			hptr = HouseClass::As_Pointer(house);
			if (!hptr) continue;

			if (!hptr->IsDefeated) {

				/*...............................................................
				If this is the winning house, find which player it was & increment
				their 'Wins' value
				...............................................................*/
				if (hptr->IsHuman) {
					for (i = 0; i < MPlayerCount; i++) {
						if (house == MPlayerHouses[i]) {
							MPlayerScore[score_index[i]].Wins++;
							MPlayerWinner = score_index[i];
						}
					}
				}
			} else {

				/*..................................................................
				This house was defeated; find which player who defeated him & increment
				his 'Kills' value for this game
				..................................................................*/
				for (i = 0; i < MPlayerCount; i++) {
					if (hptr->WhoLastHurtMe == MPlayerHouses[i]) {
						MPlayerScore[score_index[i]].Kills[MPlayerCurGame]++;
					}
				}
			}
		}

#else	// This is the new method:

		/*---------------------------------------------------------------------
		Tally up the scores for this game:
		- For each player:
		  - If this player is undefeated this round, he's the winner
		  - Each player's Kills value is the sum of the unit's they killed
		---------------------------------------------------------------------*/
		for (i = 0; i < MPlayerCount; i++) {
			hptr = HouseClass::As_Pointer(MPlayerHouses[i]);

			/*..................................................................
			If this house was undefeated, it must have been the winner.  (If
			no human houses are undefeated, the computer won.)
			..................................................................*/
			if (!hptr->IsDefeated) {
				MPlayerScore[score_index[i]].Wins++;
				MPlayerWinner = score_index[i];
			}

			/*..................................................................
			Tally up all kills for this player
			..................................................................*/
			for (house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {

				MPlayerScore[score_index[i]].Kills[MPlayerCurGame] +=
					hptr->UnitsKilled[house];

				MPlayerScore[score_index[i]].Kills[MPlayerCurGame] +=
					hptr->BuildingsKilled[house];
			}
		}
#endif

		/*---------------------------------------------------------------------
		Destroy all the IPX connections, since we have to go through the rest
		of the Main_Loop() before we detect that the game is over, and we'll
		end up waiting for frame sync packets from the other machines.
		---------------------------------------------------------------------*/
		if (GameToPlay==GAME_IPX || GameToPlay == GAME_INTERNET) {
			i = 0;
			while (Ipx.Num_Connections() && (i++ < 1000) ) {
				id = Ipx.Connection_ID(0);
				Ipx.Delete_Connection(id);
			}
			MPlayerCount = 0;
		}
	}

	/*------------------------------------------------------------------------
	Be sure our messages get displayed, even if we're about to exit.
	------------------------------------------------------------------------*/
	Map.Render();
}


/***************************************************************************
 * HouseClass::Blowup_All -- blows up everything                           *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/16/1995 BRR : Created.                                             *
 *   06/09/1995 JLB : Handles aircraft.                                    *
 *=========================================================================*/
void HouseClass::Blowup_All(void)
{
	Validate();
	int i;
	int damage;
	UnitClass *uptr;
	InfantryClass *iptr;
	BuildingClass *bptr;
	int count;
	WarheadType warhead;

	/*
	**	Find everything owned by this house & blast it with a huge amount of damage
	**	at zero range.  Do units before infantry, so the units' drivers are killed
	**	too.  Using Explosion_Damage is like dropping a big bomb right on the
	**	object; it will also damage anything around it.
	*/
	for (i = 0; i < ::Units.Count(); i++) {
		if (::Units.Ptr(i)->House == this && !::Units.Ptr(i)->IsInLimbo) {
			uptr = ::Units.Ptr(i);

			/*
			**	Some units can't be killed with one shot, so keep damaging them until
			**	they're gone.  The unit will destroy itself, and put an infantry in
			**	its place.  When the unit destroys itself, decrement 'i' since
			**	its pointer will be removed from the active pointer list.
			*/
			count = 0;
			while (::Units.Ptr(i)==uptr && uptr->Strength) {
				damage = 0x7fff;
				Explosion_Damage(uptr->Center_Coord(), damage, NULL, WARHEAD_HE);
				count++;
				if (count > 5) {
					delete uptr;
					break;
				}
			}
			i--;
		}
	}

	/*
	**	Destroy all aircraft owned by this house.
	*/
	for (i = 0; i < ::Aircraft.Count(); i++) {
		if (::Aircraft.Ptr(i)->House == this && !::Aircraft.Ptr(i)->IsInLimbo) {
			AircraftClass * aptr = ::Aircraft.Ptr(i);

			damage = 0x7fff;
			aptr->Take_Damage(damage, 0, WARHEAD_HE, NULL);
			if (!aptr->IsActive) {
				i--;
			}
		}
	}

	/*
	**	Buildings don't delete themselves when they die; they shake the screen
	**	and begin a countdown, so don't decrement 'i' when it's destroyed.
	*/
	for (i = 0; i < Buildings.Count(); i++) {
		if (Buildings.Ptr(i)->House == this && !Buildings.Ptr(i)->IsInLimbo) {
			bptr = Buildings.Ptr(i);

			count = 0;
			bptr->IsSurvivorless = true;
			while (Buildings.Ptr(i)==bptr && bptr->Strength) {
				damage = 0x7fff;
				Explosion_Damage(bptr->Center_Coord(), damage, NULL, WARHEAD_HE);
				count++;
				if (count > 5) {
					delete bptr;
					break;
				}
			}
		}
	}

	/*
	**	Infantry don't delete themselves when they die; they go into a death-
	**	animation sequence, so there's no need to decrement 'i' when they die.
	**	Infantry should die by different types of warheads, so their death
	**	anims aren't all synchronized.
	*/
	for (i = 0; i < Infantry.Count(); i++) {
		if (Infantry.Ptr(i)->House == this && !Infantry.Ptr(i)->IsInLimbo) {
			iptr = Infantry.Ptr(i);

			count = 0;
			while (Infantry.Ptr(i)==iptr && iptr->Strength) {
				damage = 0x7fff;
				warhead = (WarheadType)IRandom (WARHEAD_SA, WARHEAD_FIRE);
				Explosion_Damage(iptr->Center_Coord(), damage, NULL, warhead);
				if (iptr->IsActive) {
					damage = 0x7fff;
					iptr->Take_Damage(damage, 0, warhead);
				}

				count++;
				if (count > 5) {
					delete iptr;
					break;
				}
			}
		}
	}

#ifdef NEVER
	/*
	**	Just delete the teams & triggers for this house.
	*/
	for (i = 0; i < TeamTypes.Count(); i++) {
		if (TeamTypes.Ptr(i)->House == Class->House) {
			delete TeamTypes.Ptr(i);
			i--;
		}
	}
	for (i = 0; i < Triggers.Count(); i++) {
		if (Triggers.Ptr(i)->House == Class->House) {
			delete Triggers.Ptr(i);
			i--;
		}
	}
#endif
}


/***********************************************************************************************
 * HouseClass::Flag_To_Die -- Flags the house to blow up soon.                                 *
 *                                                                                             *
 *    When this routine is called, the house will blow up after a period of time. Typically    *
 *    this is called when the flag is captured or the HQ destroyed.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was the house flagged to blow up?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Flag_To_Die(void)
{
	Validate();
	if (!IsToWin && !IsToDie && !IsToLose) {
		IsToDie = true;
		if (IsV107) {
			BorrowedTime = TICKS_PER_SECOND * 3;
		} else {
			BorrowedTime = TICKS_PER_SECOND * 1;
		}
	}
	return(IsToDie);
}


/***********************************************************************************************
 * HouseClass::Flag_To_Win -- Flags the house to win soon.                                     *
 *                                                                                             *
 *    When this routine is called, the house will be declared the winner after a period of     *
 *    time.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was the house flagged to win?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Flag_To_Win(void)
{
	Validate();
	if (!IsToWin && !IsToDie && !IsToLose) {
		IsToWin = true;
		if (IsV107) {
			BorrowedTime = TICKS_PER_SECOND * 3;
		} else {
			BorrowedTime = TICKS_PER_SECOND * 1;
		}
	}
	return(IsToWin);
}


/***********************************************************************************************
 * HouseClass::Flag_To_Lose -- Flags the house to die soon.                                    *
 *                                                                                             *
 *    When this routine is called, it will spell the doom of this house. In a short while      *
 *    all of the object owned by this house will explode. Typical use of this routine is when  *
 *    the flag has been captured or the command vehicle has been destroyed.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Has the doom been initiated?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Flag_To_Lose(void)
{
	Validate();
	IsToWin = false;
	if (!IsToDie && !IsToLose) {
		IsToLose = true;
		if (IsV107) {
			BorrowedTime = TICKS_PER_SECOND * 3;
		} else {
			BorrowedTime = TICKS_PER_SECOND * 1;
		}
	}
	return(IsToLose);
}


/***********************************************************************************************
 * HouseClass::Init_Data -- Initializes the multiplayer color data.                            *
 *                                                                                             *
 *    This routine is called when initializing the color and remap data for this house. The    *
 *    primary user of this routine is the multiplayer version of the game.                     *
 *                                                                                             *
 * INPUT:   color    -- The color of this house.                                               *
 *                                                                                             *
 *          house    -- The house that this should act like.                                   *
 *                                                                                             *
 *          credits  -- The initial credits to assign to this house.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Init_Data(PlayerColorType color, HousesType house, int credits)
{
	Validate();
	Credits = InitialCredits = credits;
	VisibleCredits.Current = Credits;

	ActLike = house;
	RemapColor = color;
	switch (color) {
		case REMAP_GOLD:
			RemapTable = RemapGold;
			((unsigned char &)Class->Color) = 157;
			((unsigned char &)Class->BrightColor) = 5;
			break;

		case REMAP_RED:
			RemapTable = RemapRed;
			((unsigned char &)Class->Color) = 123;
			((unsigned char &)Class->BrightColor) = 127;
			break;

		case REMAP_LTBLUE:
			RemapTable = RemapLtBlue;
			((unsigned char &)Class->Color) = 135;
			((unsigned char &)Class->BrightColor) = 2;
			break;

		case REMAP_ORANGE:
			RemapTable = RemapOrange;
			((unsigned char &)Class->Color) = 26;
			((unsigned char &)Class->BrightColor) = 24;
			break;

		case REMAP_GREEN:
			RemapTable = RemapGreen;
			((unsigned char &)Class->Color) = 167;
			((unsigned char &)Class->BrightColor) = 159;
			break;

		case REMAP_BLUE:
			RemapTable = RemapBlue;
			((unsigned char &)Class->Color) = 203;
			((unsigned char &)Class->BrightColor) = 201;
			break;
	}
}


/***********************************************************************************************
 * HouseClass::Power_Fraction -- Fetches the current power output rating.                      *
 *                                                                                             *
 *    Use this routine to fetch the current power output as a fixed point fraction. The        *
 *    value 0x0100 is 100% power.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with power rating as a fixed pointer number.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::Power_Fraction(void) const
{
	Validate();
	if (Power) {
		if (Drain) {
			return(Cardinal_To_Fixed(Drain, Power));
		} else {
			return(0x0100);
		}
	}
	return(0);
}


/***********************************************************************************************
 * HouseClass::Has_Nuke_Device -- Deteremines if the house has a nuclear device.               *
 *                                                                                             *
 *    This routine checks to see if the house has a nuclear device to launch. A nuclear        *
 *    device is available when the necessary parts have been retrieved in earlier scenarios    *
 *    or if this is the multiplayer version.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Does the house have a nuclear device?                                              *
 *                                                                                             *
 * WARNINGS:   This does not check to see if there is a suitable launch facility (i.e., the    *
 *             Temple of Nod), only that there is a nuclear device potential.                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Has_Nuke_Device(void)
{
	Validate();
	if (GameToPlay != GAME_NORMAL || !IsHuman) return(true);
	return((NukePieces & 0x07) == 0x07);
}


/***********************************************************************************************
 * HouseClass::Sell_Wall -- Tries to sell the wall at the specified location.                  *
 *                                                                                             *
 *    This routine will try to sell the wall at the specified location. If there is a wall     *
 *    present and it is owned by this house, then it can be sold.                              *
 *                                                                                             *
 * INPUT:   cell  -- The cell that wall selling is desired.                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Sell_Wall(CELL cell)
{
	Validate();
	if ((unsigned)cell > 0) {
		OverlayType overlay = Map[cell].Overlay;

		if (overlay != OVERLAY_NONE && Map[cell].Owner == Class->House) {
			OverlayTypeClass const & optr = OverlayTypeClass::As_Reference(overlay);

			if (optr.IsWall) {
				BuildingTypeClass const * btype = NULL;
				switch (overlay) {
					case OVERLAY_SANDBAG_WALL:
						btype = &BuildingTypeClass::As_Reference(STRUCT_SANDBAG_WALL);
						break;

					case OVERLAY_CYCLONE_WALL:
						btype = &BuildingTypeClass::As_Reference(STRUCT_CYCLONE_WALL);
						break;

					case OVERLAY_BRICK_WALL:
						btype = &BuildingTypeClass::As_Reference(STRUCT_BRICK_WALL);
						break;

					case OVERLAY_BARBWIRE_WALL:
						btype = &BuildingTypeClass::As_Reference(STRUCT_BARBWIRE_WALL);
						break;

					case OVERLAY_WOOD_WALL:
						btype = &BuildingTypeClass::As_Reference(STRUCT_WOOD_WALL);
						break;

					default:
						break;
				}
				if (btype != NULL && !btype->IsUnsellable) {

					if (PlayerPtr == this) {
						Sound_Effect(VOC_CASHTURN);
					}

					Refund_Money(btype->Cost_Of()/2);
					Map[cell].Overlay = OVERLAY_NONE;
					Map[cell].OverlayData = 0;
					Map[cell].Owner = HOUSE_NONE;
					Map[cell].Wall_Update();
					Map[cell].Adjacent_Cell(FACING_N).Wall_Update();
					Map[cell].Adjacent_Cell(FACING_W).Wall_Update();
					Map[cell].Adjacent_Cell(FACING_S).Wall_Update();
					Map[cell].Adjacent_Cell(FACING_E).Wall_Update();
					Map[cell].Recalc_Attributes();
					Map[cell].Redraw_Objects();
					ObjectClass::Detach_This_From_All(::As_Target(cell), true);
				}
			}
		}
	}
}



/***********************************************************************************************
 * HouseClass::Check_Pertinent_Structures -- See if any useful structures remain               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   1/31/2020 3:34PM ST : Created.                                                            *
 *=============================================================================================*/
void HouseClass::Check_Pertinent_Structures(void)
{
	/*
	** New default win mode to avoid griefing. ST - 1/31/2020 3:33PM
	**
	** Game is over when no pertinent structures remain
	*/

	if (!Special.IsEarlyWin) {
		return;
	}
		  
	if (IsToDie || IsToWin || IsToLose) {
		return;
	}

	bool any_good_buildings = false;
	
	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass *b = Buildings.Ptr(index);

		if (b && b->IsActive && b->House == this) {
			if (!b->Class->IsWall) {
				if (!b->IsInLimbo && b->Strength > 0) {
					any_good_buildings = true;
					break;
				}
			}
		}
	}
	
	if (!any_good_buildings) {
		for (int index = 0; index < Units.Count(); index++) {
			UnitClass * unit = Units.Ptr(index);

			if (unit && unit->IsActive && *unit == UNIT_MCV && unit->House == this) {
				if (!unit->IsInLimbo && unit->Strength > 0) {
					any_good_buildings = true;
					break;
				}
			}
		}
	}

	if (!any_good_buildings) {
		Flag_To_Die();
	}
}




/***********************************************************************************************
 * HouseClass::Init_Unit_Trackers -- Allocate the unit trackers for the house                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   4/23/2020 11:06PM ST : Created.                                                           *
 *=============================================================================================*/
void HouseClass::Init_Unit_Trackers(void)
{		
	AircraftTotals = new UnitTrackerClass( (int) AIRCRAFT_COUNT);
	InfantryTotals = new UnitTrackerClass( (int) INFANTRY_COUNT);
	UnitTotals = new UnitTrackerClass ( (int) UNIT_COUNT);
	BuildingTotals = new UnitTrackerClass ( (int) STRUCT_COUNT);

	DestroyedAircraft = new UnitTrackerClass ( (int) AIRCRAFT_COUNT);
	DestroyedInfantry = new UnitTrackerClass( (int) INFANTRY_COUNT);
	DestroyedUnits = new UnitTrackerClass ( (int) UNIT_COUNT);
	DestroyedBuildings = new UnitTrackerClass ( (int) STRUCT_COUNT);

	CapturedBuildings = new UnitTrackerClass ( (int) STRUCT_COUNT);
	TotalCrates = new UnitTrackerClass ( TOTAL_CRATE_TYPES );	//15 crate types
}



/***********************************************************************************************
 * HouseClass::Free_Unit_Trackers -- Free the unit trackers for the house                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   4/23/2020 11:06PM ST : Created.                                                           *
 *=============================================================================================*/
void HouseClass::Free_Unit_Trackers(void)
{		
	if (AircraftTotals) {
		delete AircraftTotals;
		AircraftTotals = NULL;
	}

	if (InfantryTotals) {
		delete InfantryTotals;
		InfantryTotals = NULL;
	}

	if (UnitTotals) {
		delete UnitTotals;
		UnitTotals = NULL;
	}

	if (BuildingTotals) {
		delete BuildingTotals;
		BuildingTotals = NULL;
	}

	if (DestroyedAircraft) {
		delete DestroyedAircraft;
		DestroyedAircraft = NULL;
	}

	if (DestroyedInfantry) {
		delete DestroyedInfantry;
		DestroyedInfantry = NULL;
	}

	if (DestroyedUnits) {
		delete DestroyedUnits;
		DestroyedUnits = NULL;
	}

	if (DestroyedBuildings) {
		delete DestroyedBuildings;
		DestroyedBuildings = NULL;
	}

	if (CapturedBuildings) {
		delete CapturedBuildings;
		CapturedBuildings = NULL;
	}

	if (TotalCrates) {
		delete TotalCrates;
		TotalCrates = NULL;
	}
}




































#ifdef USE_RA_AI	



/***********************************************************************************************

 Below AI code imported from RA

***********************************************************************************************/

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)	(sizeof(a)/sizeof(a[0]))
#endif

/*
** In RA, Control is a container for other variables. In TD, they are defined in the class
*/
#define Control (*this)

/*
** Percent_Chance - implementation similar to Red Alert
*/
inline bool Percent_Chance(int percent)
{
	return (Random_Pick(0, 99) < percent);
}


/*
** Engineer was renamed to RENOVATOR for RA
*/
#define INFANTRY_RENOVATOR INFANTRY_E7


TFixedIHeapClass<HouseClass::BuildChoiceClass> HouseClass::BuildChoice;

/*
** This is a replacement for the RA 'fixed' round up function. It takes the equivalent of a 'fixed' value, but returns just the integer part
** ST - 7/26/2019 11:13AM
*/
unsigned short Round_Up(unsigned short val)
{
	if ((val & 0xff) == 0) {
		return val;
	}
	val &= 0xff00;
	val += 0x0100;
	val >>= 8;
	return val;
}


unsigned short fixed(int val) {return (unsigned short)val;}



/***********************************************************************************************
 * HouseClass::Suggest_New_Building -- Examines the situation and suggests a building.         *
 *                                                                                             *
 *    This routine is called when a construction yard needs to know what to build next. It     *
 *    will either examine the prebuilt base list or try to figure out what to build next       *
 *    based on the current game situation.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building type class to build.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingTypeClass const * HouseClass::Suggest_New_Building(void) const
{
	//assert(Houses.ID(this) == ID);

	if (BuildStructure != STRUCT_NONE) {
		return(&BuildingTypeClass::As_Reference(BuildStructure));
	}
	return(NULL);
}


/***********************************************************************************************
 * HouseClass::Find_Building -- Finds a building of specified type.                            *
 *                                                                                             *
 *    This routine is used to find a building of the specified type. This is particularly      *
 *    useful for when some event requires a specific building instance. The nuclear missile    *
 *    launch is a good example.                                                                *
 *                                                                                             *
 * INPUT:   type  -- The building type to scan for.                                            *
 *                                                                                             *
 *          zone  -- The zone that the building must be located in. If no zone specific search *
 *                   is desired, then pass ZONE_NONE.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building type requested. If there is no building     *
 *          of the type requested, then NULL is returned.                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1995 JLB : Created.                                                                 *
 *   10/02/1995 JLB : Allows for zone specifics.                                               *
 *=============================================================================================*/
BuildingClass * HouseClass::Find_Building(StructType type, ZoneType zone) const
{
	//assert(Houses.ID(this) == ID);

	/*
	**	Only scan if we KNOW there is at least one building of the type
	**	requested.
	*/
	if (BQuantity[type] > 0) {

		/*
		**	Search for a suitable launch site for this missile.
		*/
		for (int index = 0; index < Buildings.Count(); index++) {
			BuildingClass * b = Buildings.Ptr(index);
			if (b && !b->IsInLimbo && b->House == this && *b == type) {
				if (zone == ZONE_NONE || Which_Zone(b) == zone) {
					return(b);
				}
			}
		}
	}
	return(NULL);
}


/***********************************************************************************************
 * HouseClass::Find_Build_Location -- Finds a suitable building location.                      *
 *                                                                                             *
 *    This routine is used to find a suitable building location for the building specified.    *
 *    The auto base building logic uses this when building the base for the computer.          *
 *                                                                                             *
 * INPUT:   building -- Pointer to the building that needs to be placed down.                  *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate to place the building at. If there are no suitable     *
 *          locations, then NULL is returned.                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE HouseClass::Find_Build_Location(BuildingClass * building) const
{
	//assert(Houses.ID(this) == ID);

	int zonerating[ZONE_COUNT];
	struct {
		int AntiAir;			// Average air defense for the base.
		int AntiArmor;			// Average armor defense for the base.
		int AntiInfantry;		// Average infantry defense for the base.
	} zoneinfo = {0,0,0};
	int antiair = building->Anti_Air();
	int antiarmor = building->Anti_Armor();
	int antiinfantry = building->Anti_Infantry();
	bool adj = true;

	/*
	**	Never place combat buildings adjacent to each other. This is partly
	**	because combat buildings don't have a bib and jamming will occur as well
	**	as because spacing defensive buildings out will yield a better
	**	defense.
	*/
	if (antiair || antiarmor || antiinfantry) {
		adj = false;
	}

	/*
	**	Determine the average zone strengths for the base. This value is
	**	used to determine what zones are considered under or over strength.
	*/
	ZoneType z;
	for (z = ZONE_NORTH; z < ZONE_COUNT; z++) {
		zoneinfo.AntiAir += ZoneInfo[z].AirDefense;
		zoneinfo.AntiArmor += ZoneInfo[z].ArmorDefense;
		zoneinfo.AntiInfantry += ZoneInfo[z].InfantryDefense;
	}
	zoneinfo.AntiAir /= ZONE_COUNT-ZONE_NORTH;
	zoneinfo.AntiArmor /= ZONE_COUNT-ZONE_NORTH;
	zoneinfo.AntiInfantry /= ZONE_COUNT-ZONE_NORTH;

	/*
	**	Give each zone a rating for value. The higher the value the more desirable
	**	to place the specified building in that zone. Factor the average value of
	**	zone defense such that more weight is given to zones that are very under
	**	defended.
	*/
	memset(&zonerating[0], '\0', sizeof(zonerating));
	for (z = ZONE_FIRST; z < ZONE_COUNT; z++) {
		int diff;

		diff = zoneinfo.AntiAir-ZoneInfo[z].AirDefense;
		if (z == ZONE_CORE) diff /= 2;
		if (diff > 0) {
			zonerating[z] += min(antiair, diff);
		}

		diff = zoneinfo.AntiArmor-ZoneInfo[z].ArmorDefense;
		if (z == ZONE_CORE) diff /= 2;
		if (diff > 0) {
			zonerating[z] += min(antiarmor, diff);
		}

		diff = zoneinfo.AntiInfantry-ZoneInfo[z].InfantryDefense;
		if (z == ZONE_CORE) diff /= 2;
		if (diff > 0) {
			zonerating[z] += min(antiinfantry, diff);
		}
	}

	/*
	**	Now that each zone has been given a desirability rating, find the zone
	**	with the greatest value and try to place the building in that zone.
	*/
	ZoneType zone = Random_Pick(ZONE_FIRST, ZONE_WEST);
	int largest = 0;
	for (z = ZONE_FIRST; z < ZONE_COUNT; z++) {
		if (zonerating[z] > largest) {
			zone = z;
			largest = zonerating[z];
		}
	}

	CELL zcell = Find_Cell_In_Zone(building, zone);
	if (zcell) {
		return(Cell_Coord(zcell));
	}

	/*
	**	Could not build in preferred zone, so try building in any zone.
	*/
	static ZoneType _zones[] = {ZONE_CORE, ZONE_NORTH, ZONE_SOUTH, ZONE_EAST, ZONE_WEST};
	int start = Random_Pick(0U, ARRAY_SIZE(_zones)-1);
	for (int zz = 0; zz < ARRAY_SIZE(_zones); zz++) {
		ZoneType tryzone = _zones[(zz + start) % ARRAY_SIZE(_zones)];
		zcell = Find_Cell_In_Zone(building, tryzone);
		if (zcell) return(zcell);
	}

	return(NULL);
}


/***********************************************************************************************
 * HouseClass::Recalc_Center -- Recalculates the center point of the base.                     *
 *                                                                                             *
 *    This routine will average the location of the base and record the center point. The      *
 *    recorded center point is used to determine such things as how far the base is spread     *
 *    out and where to protect the most. This routine should be called whenever a building     *
 *    is created or destroyed.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/28/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Recalc_Center(void)
{
	//assert(Houses.ID(this) == ID);

	/*
	**	First presume that there is no base. If there is a base, then these values will be
	**	properly filled in below.
	*/
	Center = 0;
	Radius = 0;
	for (ZoneType zone = ZONE_FIRST; zone < ZONE_COUNT; zone++) {
		ZoneInfo[zone].AirDefense = 0;
		ZoneInfo[zone].ArmorDefense = 0;
		ZoneInfo[zone].InfantryDefense = 0;
	}

	/*
	**	Only process the center base size/position calculation if there are buildings to
	**	consider. When no buildings for this house are present, then no processing need
	**	occur.
	*/
	if (CurBuildings > 0) {
		int x = 0;
		int y = 0;
		int count = 0;
		int index;

		for (index = 0; index < Buildings.Count(); index++) {
			BuildingClass const * b = Buildings.Ptr(index);

			if (b != NULL && !b->IsInLimbo && (HouseClass *)b->House == this && b->Strength > 0) {

				/*
				**	Give more "weight" to buildings that cost more. The presumption is that cheap
				**	buildings don't affect the base disposition as much as the more expensive
				**	buildings do.
				*/
				int weight = (b->Class->Cost_Of() / 1000)+1;
				for (int i = 0; i < weight; i++) {
					x += Coord_X(b->Center_Coord());
					y += Coord_Y(b->Center_Coord());
					count++;
				}
			}
		}

		/*
		**	This second check for quantity of buildings is necessary because the first
		**	check against CurBuildings doesn't take into account if the building is in
		**	limbo, but for base calculation, the limbo state disqualifies a building
		**	from being processed. Thus, CurBuildings may indicate a base, but count may
		**	not match.
		*/
		if (count > 0) {
			x /= count;
			y /= count;

#ifdef NEVER
			/*
			**	Bias the center of the base away from the edges of the map.
			*/
			LEPTON left = Cell_To_Lepton(Map.MapCellX + 10);
			LEPTON top = Cell_To_Lepton(Map.MapCellY + 10);
			LEPTON right = Cell_To_Lepton(Map.MapCellX + Map.MapCellWidth - 10);
			LEPTON bottom = Cell_To_Lepton(Map.MapCellY + Map.MapCellHeight - 10);
			if (x < left) x = left;
			if (x > right) x = right;
			if (y < top) y = top;
			if (y > bottom) y = bottom;
#endif

			Center = XY_Coord(x, y);
		}

		/*
		**	If there were any buildings discovered as legal to consider as part of the base,
		**	then figure out the general average radius of the building disposition as it
		**	relates to the center of the base.
		*/
		if (count > 1) {
			int radius = 0;
	
			for (index = 0; index < Buildings.Count(); index++) {
				BuildingClass const * b = Buildings.Ptr(index);

				if (b != NULL && !b->IsInLimbo && (HouseClass *)b->House == this && b->Strength > 0) {
					radius += Distance(Center, b->Center_Coord());
				}
			}
			Radius = max(radius / count, 2 * CELL_LEPTON_W);

			/*
			**	Determine the relative strength of each base defense zone.
			*/
			for (index = 0; index < Buildings.Count(); index++) {
				BuildingClass const * b = Buildings.Ptr(index);

				if (b != NULL && !b->IsInLimbo && (HouseClass *)b->House == this && b->Strength > 0) {
					ZoneType z = Which_Zone(b);

					if (z != ZONE_NONE) {
						ZoneInfo[z].ArmorDefense += b->Anti_Armor();
						ZoneInfo[z].AirDefense += b->Anti_Air();
						ZoneInfo[z].InfantryDefense += b->Anti_Infantry();
					}
				}
			}

		} else {
			Radius = 0x0200;
		}
	}
}


/***********************************************************************************************
 * HouseClass::Expert_AI -- Handles expert AI processing.                                      *
 *                                                                                             *
 *    This routine is called when the computer should perform expert AI processing. This       *
 *    method of AI is categorized as an "Expert System" process.                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of game frames to delay before calling this routine again.      *
 *                                                                                             *
 * WARNINGS:   This is relatively time consuming -- call periodically.                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::Expert_AI(void)
{
	//assert(Houses.ID(this) == ID);

	BuildingClass * b = 0;
	bool stop = false;
	int time = TICKS_PER_SECOND * 10;

	/*
	**	If the current enemy no longer has a base or is defeated, then don't consider
	**	that house a threat anymore. Clear out the enemy record and then try
	**	to find a new enemy.
	*/
	if (Enemy != HOUSE_NONE) {
		HouseClass * h = HouseClass::As_Pointer(Enemy);

		if (h == NULL || !h->IsActive || h->IsDefeated || Is_Ally(h) || h->BScan == 0) {
			Enemy = HOUSE_NONE;
		}
	}

	/*
	**	If there is no enemy assigned to this house, then assign one now. The
	**	enemy that is closest is picked. However, don't pick an enemy if the
	**	base has not been established yet.
	*/
	if (ActiveBScan && Center && Attack == 0) {
		int close = 0;
		HousesType enemy = HOUSE_NONE;
		int maxunit = 0;
		int maxinfantry = 0;
		int maxvessel = 0;
		int maxaircraft = 0;
		int maxbuilding = 0;
		int enemycount = 0;

		for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
			HouseClass * h = HouseClass::As_Pointer(house);
			if (h != NULL && h->IsActive && !h->IsDefeated && !Is_Ally(h)) {

				/*
				**	Perform a special restriction check to ensure that no enemy is chosen if
				**	there is even one enemy that has not established a base yet. This will
				**	ensure an accurate first pick for enemy since the distance to base
				**	value can be determined.
				*/
				if (!h->IsStarted) {
					enemy = HOUSE_NONE;
					break;
				}

				/*
				**	Keep track of the number of buildings and units owned by the
				**	enemy. This is used to bring up the maximum allowed to match.
				*/
				maxunit += h->CurUnits;
				maxbuilding += h->CurBuildings;
				maxinfantry += h->CurInfantry;
				//maxvessel += h->CurVessels;
				maxaircraft += h->CurAircraft;
				enemycount++;

				/*
				**	Determine a priority value based on distance to the center of the
				**	candidate base. The higher the value, the better the candidate house
				**	is to becoming the preferred enemy for this house.
				*/
				int value = ((MAP_CELL_W*2)-Distance(Center, h->Center));
				value *= 2;

				/*
				**	In addition to distance, record the number of kills directed
				**	against this house. The enemy that does more damage might be
				**	considered a greater threat.
				*/
				value += h->BuildingsKilled[Class->House]*5;
				value += h->UnitsKilled[Class->House];

				/*
				**	Factor in the relative sizes of the bases. An enemy that has a
				**	larger base will be considered a bigger threat. Conversely, a
				**	smaller base is considered a lesser threat.
				*/
				value += h->CurUnits - CurUnits;
				value += h->CurBuildings - CurBuildings;
				value += (h->CurInfantry - CurInfantry)/4;

				/*
				**	Whoever last attacked is given a little more priority as
				**	a potential designated enemy.
				*/
				if (house == LAEnemy) {
					value += 100;
				}

#ifdef OBSOLETE
				/*
				**	Human players are a given preference as the target.
				*/
				if (h->IsHuman) {
					value *= 2;
				}
#endif

				/*
				**	Compare the calculated value for this candidate house and if it is
				**	greater than the previously recorded maximum, record this house as
				**	the prime candidate for enemy.
				*/
				if (value > close) {
					enemy = house;
					close = value;
				}
			}
		}

		/*
		**	Record this closest enemy base as the first enemy to attack.
		*/
		Enemy = enemy;

		/*
		**	Up the maximum allowed units and buildings to match a rough average
		**	of what the enemies are allowed.
		*/
		if (enemycount) {
			maxunit /= enemycount;
			maxbuilding /= enemycount;
			maxinfantry /= enemycount;
			maxvessel /= enemycount;
			maxaircraft /= enemycount;
		}

		if (Control.MaxBuilding < (unsigned)maxbuilding + 10) {
			Control.MaxBuilding = maxbuilding + 10;
		}
		if (Control.MaxUnit < (unsigned)maxunit + 10) {
			Control.MaxUnit = maxunit + 10;
		}
		if (Control.MaxInfantry < (unsigned)maxinfantry + 10) {
			Control.MaxInfantry = maxinfantry + 10;
		}
		//if (Control.MaxVessel < (unsigned)maxvessel + 10) {
		//	Control.MaxVessel = maxvessel + 10;
		//}
		if (Control.MaxAircraft < (unsigned)maxaircraft + 10) {
			Control.MaxAircraft = maxaircraft + 10;
		}
	}

	/*
	**	House state transition check occurs here. Transitions that occur here are ones
	**	that relate to general base condition rather than specific combat events.
	**	Typically, this is limited to transitions between normal buildup mode and
	**	broke mode.
	*/
	if (State == STATE_ENDGAME) {
		Fire_Sale();
		Do_All_To_Hunt();
	} else {
		if (State == STATE_BUILDUP) {
			if (Available_Money() < 25) {
				State = STATE_BROKE;
			}
		}
		if (State == STATE_BROKE) {
			if (Available_Money() >= 25) {
				State = STATE_BUILDUP;
			}
		}
		if (State == STATE_ATTACKED && LATime + TICKS_PER_MINUTE < Frame) {
			State = STATE_BUILDUP;
		}
		if (State != STATE_ATTACKED && LATime + TICKS_PER_MINUTE > Frame) {
			State = STATE_ATTACKED;
		}
	}

	/*
	**	Records the urgency of all actions possible.
	*/
	UrgencyType urgency[STRATEGY_COUNT];
	StrategyType strat;
	for (strat = STRATEGY_FIRST; strat < STRATEGY_COUNT; strat++) {
		urgency[strat] = URGENCY_NONE;

		switch (strat) {
			case STRATEGY_BUILD_POWER:
				urgency[strat] = Check_Build_Power();
				break;

			case STRATEGY_BUILD_DEFENSE:
				urgency[strat] = Check_Build_Defense();
				break;

			case STRATEGY_BUILD_INCOME:
				urgency[strat] = Check_Build_Income();
				break;

			case STRATEGY_FIRE_SALE:
				urgency[strat] = Check_Fire_Sale();
				break;

			case STRATEGY_BUILD_ENGINEER:
				urgency[strat] = Check_Build_Engineer();
				break;

			case STRATEGY_BUILD_OFFENSE:
				urgency[strat] = Check_Build_Offense();
				break;

			case STRATEGY_RAISE_MONEY:
				urgency[strat] = Check_Raise_Money();
				break;

			case STRATEGY_RAISE_POWER:
				urgency[strat] = Check_Raise_Power();
				break;

			case STRATEGY_LOWER_POWER:
				urgency[strat] = Check_Lower_Power();
				break;

			case STRATEGY_ATTACK:
				urgency[strat] = Check_Attack();
				break;

			default:
				urgency[strat] = URGENCY_NONE;
				break;
		}
	}

	/*
	**	Performs the action required for each of the strategies that share
	**	the most urgent category. Stop processing if any strategy at the
	**	highest urgency performed any action. This is because higher urgency
	**	actions tend to greatly affect the lower urgency actions.
	*/
	for (UrgencyType u = URGENCY_CRITICAL; u >= URGENCY_LOW; u--) {
		bool acted = false;

		for (strat = STRATEGY_FIRST; strat < STRATEGY_COUNT; strat++) {
			if (urgency[strat] == u) {
				switch (strat) {
					case STRATEGY_BUILD_POWER:
						acted |= AI_Build_Power(u);
						break;

					case STRATEGY_BUILD_DEFENSE:
						acted |= AI_Build_Defense(u);
						break;

					case STRATEGY_BUILD_INCOME:
						acted |= AI_Build_Income(u);
						break;

					case STRATEGY_FIRE_SALE:
						acted |= AI_Fire_Sale(u);
						break;

					case STRATEGY_BUILD_ENGINEER:
						acted |= AI_Build_Engineer(u);
						break;

					case STRATEGY_BUILD_OFFENSE:
						acted |= AI_Build_Offense(u);
						break;

					case STRATEGY_RAISE_MONEY:
						acted |= AI_Raise_Money(u);
						break;

					case STRATEGY_RAISE_POWER:
						acted |= AI_Raise_Power(u);
						break;

					case STRATEGY_LOWER_POWER:
						acted |= AI_Lower_Power(u);
						break;

					case STRATEGY_ATTACK:
						acted |= AI_Attack(u);
						break;

					default:
						break;
				}
			}
		}
	}

	return(TICKS_PER_SECOND*5 + Random_Pick(1, TICKS_PER_SECOND/2));
}


UrgencyType HouseClass::Check_Build_Power(void) const
{
	//assert(Houses.ID(this) == ID);

	//fixed frac = Power_Fraction();
	int frac = Power_Fraction();

	UrgencyType urgency = URGENCY_NONE;

	//if (frac < 1 && Can_Make_Money()) {
	if (frac < 0x0100 && Can_Make_Money()) {
		urgency = URGENCY_LOW;

		/*
		**	Very low power condition is considered a higher priority.
		*/
		//if (frac < fixed::_3_4) urgency = URGENCY_MEDIUM;
		if (frac < 0x00C0) urgency = URGENCY_MEDIUM;

		/*
		**	When under attack and there is a need for power in defense,
		**	then consider power building a higher priority.
		*/
		// No chronosphere in TD. ST - 7/19/2019 4:38PM
		//if (State == STATE_THREATENED || State == STATE_ATTACKED) {
		//	if (BScan | (STRUCTF_CHRONOSPHERE)) {
		//		urgency = URGENCY_HIGH;
		//	}
		//}

	}
	return(urgency);
}


UrgencyType HouseClass::Check_Build_Defense(void) const
{
	//assert(Houses.ID(this) == ID);

	/*
	**	This routine determines what urgency level that base defense
	**	should be given. The more vulnerable the base is, the higher
	**	the urgency this routine should return.
	*/
	return(URGENCY_NONE);
}


UrgencyType HouseClass::Check_Build_Offense(void) const
{
	//assert(Houses.ID(this) == ID);

	/*
	**	This routine determines what urgency level that offensive
	**	weaponry should be given. Surplus money or a very strong
	**	defense will cause the offensive urgency to increase.
	*/
	return(URGENCY_NONE);
}

/*
**	Determines what the attack state of the base is. The higher the state,
**	the greater the immediate threat to base defense is.
*/
UrgencyType HouseClass::Check_Attack(void) const
{
	//assert(Houses.ID(this) == ID);

	if (Frame > TICKS_PER_MINUTE && Attack == 0) {
		if (State == STATE_ATTACKED) {
			return(URGENCY_LOW);
		}
		return(URGENCY_CRITICAL);
	}
	return(URGENCY_NONE);
}


UrgencyType HouseClass::Check_Build_Income(void) const
{
	//assert(Houses.ID(this) == ID);

	/*
	**	This routine should determine if income processing buildings
	**	should be constructed and at what urgency. The lower the money,
	**	the lower the refineries, or recent harvester losses should
	**	cause a greater urgency to be returned.
	*/
	return(URGENCY_NONE);
}


UrgencyType HouseClass::Check_Fire_Sale(void) const
{
	//assert(Houses.ID(this) == ID);

	/*
	**	If there are no more factories at all, then sell everything off because the game
	**	is basically over at this point.
	*/
	//if (State != STATE_ATTACKED && CurBuildings && !(ActiveBScan & (STRUCTF_TENT|STRUCTF_BARRACKS|STRUCTF_CONST|STRUCTF_AIRSTRIP|STRUCTF_WEAP|STRUCTF_HELIPAD))) {
	if (State != STATE_ATTACKED && CurBuildings && !(ActiveBScan & (STRUCTF_BARRACKS|STRUCTF_CONST|STRUCTF_AIRSTRIP|STRUCTF_WEAP|STRUCTF_HELIPAD))) {
		return(URGENCY_CRITICAL);
	}
	return(URGENCY_NONE);
}


UrgencyType HouseClass::Check_Build_Engineer(void) const
{
	//assert(Houses.ID(this) == ID);

	/*
	**	This routine should check to see what urgency that the production of
	**	engineers should be. If a friendly building has been captured or the
	**	enemy has weak defenses, then building an engineer would be a priority.
	*/
	return(URGENCY_NONE);
}


/*
**	Checks to see if money is critically low and something must be done
**	to immediately raise cash.
*/
UrgencyType HouseClass::Check_Raise_Money(void) const
{
	//assert(Houses.ID(this) == ID);

	UrgencyType urgency = URGENCY_NONE;
	if (Available_Money() < 100) {
		urgency = URGENCY_LOW;
	}
	if (Available_Money() < 2000 && !Can_Make_Money()) {
		urgency++;
	}

	return(urgency);
}

/*
**	Checks to see if power is very low and if so, a greater urgency to
**	build more power is returned.
*/
UrgencyType HouseClass::Check_Lower_Power(void) const
{
	//assert(Houses.ID(this) == ID);

	if (Power > Drain+300) {
		return(URGENCY_LOW);
	}
	return(URGENCY_NONE);
}

/*
**	This routine determines if there is a power emergency. Such an
**	emergency might require selling of structures in order to free
**	up power. This might occur if the base is being attacked and there
**	are defenses that require power, but are just short of having
**	enough.
*/
UrgencyType HouseClass::Check_Raise_Power(void) const
{
	//assert(Houses.ID(this) == ID);

	UrgencyType urgency = URGENCY_NONE;

	if (Power_Fraction() < Rule.PowerEmergencyFraction && Power < Drain - 400) {
//	if (Power_Fraction() < Rule.PowerEmergencyFraction && (BQuantity[STRUCT_CONST] == 0 || Available_Money() < 200 || Power < Drain-400)) {
		urgency = URGENCY_MEDIUM;
		if (State == STATE_ATTACKED) {
			urgency++;
		}
	}
	return(urgency);
}


bool HouseClass::AI_Attack(UrgencyType )
{
	//assert(Houses.ID(this) == ID);

	bool shuffle = !((Frame > TICKS_PER_MINUTE && !CurBuildings) || Percent_Chance(33));
	bool forced = (CurBuildings == 0);
	int index;
	for (index = 0; index < Aircraft.Count(); index++) {
		AircraftClass * a = Aircraft.Ptr(index);

		if (a != NULL && !a->IsInLimbo && a->House == this && a->Strength > 0) {
			if (!shuffle && a->Is_Weapon_Equipped() && (forced || Percent_Chance(75))) {
				a->Assign_Mission(MISSION_HUNT);
			}
		}
	}
	for (index = 0; index < Units.Count(); index++) {
		UnitClass * u = Units.Ptr(index);

		if (u != NULL && !u->IsInLimbo && u->House == this && u->Strength > 0) {
			if (!shuffle && u->Is_Weapon_Equipped() && (forced || Percent_Chance(75))) {
				u->Assign_Mission(MISSION_HUNT);
			} else {

				/*
				**	If this unit is guarding the base, then cause it to shuffle
				**	location instead.
				*/
				if (Percent_Chance(20) && u->Mission == MISSION_GUARD_AREA && Which_Zone(u) != ZONE_NONE) {
					u->ArchiveTarget = ::As_Target(Where_To_Go(u));
				}
			}
		}
	}
	for (index = 0; index < Infantry.Count(); index++) {
		InfantryClass * i = Infantry.Ptr(index);

		if (i != NULL && !i->IsInLimbo && i->House == this && i->Strength > 0) {
			if (!shuffle && (i->Is_Weapon_Equipped() || *i == INFANTRY_RENOVATOR) && (forced || Percent_Chance(75))) {
				i->Assign_Mission(MISSION_HUNT);
			} else {

				/*
				**	If this soldier is guarding the base, then cause it to shuffle
				**	location instead.
				*/
				if (Percent_Chance(20) && i->Mission == MISSION_GUARD_AREA && Which_Zone(i) != ZONE_NONE) {
					i->ArchiveTarget = ::As_Target(Where_To_Go(i));
				}
			}
		}
	}
	Attack = Rule.AttackInterval * Random_Pick(TICKS_PER_MINUTE/2, TICKS_PER_MINUTE*2);
	return(true);
}


/*
**	Given the specified urgency, build a power structure to meet
**	this need.
*/
bool HouseClass::AI_Build_Power(UrgencyType ) const
{
	//assert(Houses.ID(this) == ID);

	return(false);
}


/*
**	Given the specified urgency, build base defensive structures
**	according to need and according to existing base disposition.
*/
bool HouseClass::AI_Build_Defense(UrgencyType ) const
{
	//assert(Houses.ID(this) == ID);

	return(false);
}

/*
**	Given the specified urgency, build offensive units according
**	to need and according to the opponents base defenses.
*/
bool HouseClass::AI_Build_Offense(UrgencyType ) const
{
	//assert(Houses.ID(this) == ID);

	return(false);
}

/*
**	Given the specified urgency, build income producing
**	structures according to need.
*/
bool HouseClass::AI_Build_Income(UrgencyType ) const
{
	//assert(Houses.ID(this) == ID);

	return(false);
}


bool HouseClass::AI_Fire_Sale(UrgencyType urgency)
{
	//assert(Houses.ID(this) == ID);

	if (CurBuildings && urgency == URGENCY_CRITICAL) {
		Fire_Sale();
		Do_All_To_Hunt();
		return(true);
	}
	return(false);
}

/*
**	Given the specified urgency, build an engineer.
*/
bool HouseClass::AI_Build_Engineer(UrgencyType ) const
{
	//assert(Houses.ID(this) == ID);

	return(false);
}

/*
**	Given the specified urgency, sell of some power since
**	there appears to be excess.
*/
bool HouseClass::AI_Lower_Power(UrgencyType ) const
{
	//assert(Houses.ID(this) == ID);

	BuildingClass * b = Find_Building(STRUCT_POWER);
	if (b != NULL) {
		b->Sell_Back(1);
		return(true);
	}

	b = Find_Building(STRUCT_ADVANCED_POWER);
	if (b != NULL) {
		b->Sell_Back(1);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::AI_Raise_Power -- Try to raise power levels by selling off buildings.           *
 *                                                                                             *
 *    This routine is called when the computer needs to raise power by selling off buildings.  *
 *    Usually this occurs because of some catastrophe that has lowered power levels to         *
 *    the danger zone.                                                                         *
 *                                                                                             *
 * INPUT:   urgency  -- The urgency that the power needs to be raised. This controls what      *
 *                      buildings will be sold.                                                *
 *                                                                                             *
 * OUTPUT:  bool; Was a building sold to raise power?                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::AI_Raise_Power(UrgencyType urgency) const
{
	//assert(Houses.ID(this) == ID);

	/*
	**	Sell off structures in this order.
	*/
#if (0)	
	static struct {
		StructType Structure;
		UrgencyType Urgency;
	} _types[] = {
		{STRUCT_CHRONOSPHERE, URGENCY_LOW},
		{STRUCT_SHIP_YARD, URGENCY_LOW},
		{STRUCT_SUB_PEN, URGENCY_LOW},
		{STRUCT_ADVANCED_TECH, URGENCY_LOW},
		{STRUCT_FORWARD_COM, URGENCY_LOW},
		{STRUCT_SOVIET_TECH, URGENCY_LOW},
		{STRUCT_IRON_CURTAIN, URGENCY_MEDIUM},
		{STRUCT_RADAR, URGENCY_MEDIUM},
		{STRUCT_REPAIR, URGENCY_MEDIUM},
		{STRUCT_TESLA, URGENCY_HIGH}
	};
#endif
	static struct {
		StructType Structure;
		UrgencyType Urgency;
	} _types[] = {
		{STRUCT_BIO_LAB, URGENCY_LOW},
		{STRUCT_EYE, URGENCY_MEDIUM},
		{STRUCT_RADAR, URGENCY_MEDIUM},
		{STRUCT_REPAIR, URGENCY_MEDIUM},
		{STRUCT_OBELISK, URGENCY_HIGH},
		{STRUCT_TURRET, URGENCY_HIGH},
		{STRUCT_ATOWER, URGENCY_HIGH},
		{STRUCT_GTOWER, URGENCY_HIGH}
	};


	/*
	**	Find a structure to sell and then sell it. Bail from further scanning until
	**	the next time.
	*/
	for (int i = 0; i < ARRAY_SIZE(_types); i++) {
		if (urgency >= _types[i].Urgency) {
			BuildingClass * b = Find_Building(_types[i].Structure);
			if (b != NULL) {
				b->Sell_Back(1);
				return(true);
			}
		}
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::AI_Raise_Money -- Raise emergency cash by selling buildings.                    *
 *                                                                                             *
 *    This routine handles the situation where the computer desperately needs cash but cannot  *
 *    wait for normal harvesting to raise it. Buildings must be sold.                          *
 *                                                                                             *
 * INPUT:   urgency  -- The urgency level that cash must be raised. The greater the urgency,   *
 *                      the more important the buildings that can be sold become.              *
 *                                                                                             *
 * OUTPUT:  bool; Was a building sold to raise cash?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::AI_Raise_Money(UrgencyType urgency) const
{
	//assert(Houses.ID(this) == ID);

	/*
	**	Sell off structures in this order.
	*/
#if (0)	
	static struct {
		StructType	Structure;
		UrgencyType	Urgency;
	} _types[] = {
		{STRUCT_CHRONOSPHERE, URGENCY_LOW},
		{STRUCT_SHIP_YARD, URGENCY_LOW},
		{STRUCT_SUB_PEN, URGENCY_LOW},
		{STRUCT_ADVANCED_TECH, URGENCY_LOW},
		{STRUCT_FORWARD_COM, URGENCY_LOW},
		{STRUCT_SOVIET_TECH, URGENCY_LOW},
		{STRUCT_STORAGE,URGENCY_LOW},
		{STRUCT_REPAIR,URGENCY_LOW},
		{STRUCT_TESLA,URGENCY_MEDIUM},
		{STRUCT_HELIPAD,URGENCY_MEDIUM},
		{STRUCT_POWER,URGENCY_HIGH},
		{STRUCT_AIRSTRIP,URGENCY_HIGH},
//		{STRUCT_WEAP,URGENCY_HIGH},
//		{STRUCT_BARRACKS,URGENCY_HIGH},
//		{STRUCT_TENT,URGENCY_HIGH},
		{STRUCT_CONST,URGENCY_CRITICAL}
	};
#endif

	static struct {
		StructType Structure;
		UrgencyType Urgency;
	} _types[] = {
		{STRUCT_BIO_LAB, URGENCY_LOW},
		{STRUCT_EYE, URGENCY_MEDIUM},
		{STRUCT_RADAR, URGENCY_MEDIUM},
		{STRUCT_STORAGE,URGENCY_LOW},
		{STRUCT_REPAIR, URGENCY_MEDIUM},
		{STRUCT_OBELISK, URGENCY_HIGH},
		{STRUCT_TURRET, URGENCY_HIGH},
		{STRUCT_ATOWER, URGENCY_HIGH},
		{STRUCT_GTOWER, URGENCY_HIGH},
		{STRUCT_HELIPAD,URGENCY_MEDIUM},
		{STRUCT_POWER,URGENCY_HIGH},
		{STRUCT_AIRSTRIP,URGENCY_HIGH},
		{STRUCT_CONST,URGENCY_CRITICAL}
	};



	BuildingClass * b = 0;

	/*
	**	Find a structure to sell and then sell it. Bail from further scanning until
	**	the next time.
	*/
	for (int i = 0; i < ARRAY_SIZE(_types); i++) {
		if (urgency >= _types[i].Urgency) {
			b = Find_Building(_types[i].Structure);
			if (b != NULL) {
				b->Sell_Back(1);
				return(true);
			}
		}
	}
	return(false);
}


#ifdef NEVER

/***********************************************************************************************
 * HouseClass::AI_Base_Defense -- Handles maintaining a strong base defense.                   *
 *                                                                                             *
 *    This logic is used to maintain a base defense.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine again. *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::AI_Base_Defense(void)
{
	//assert(Houses.ID(this) == ID);

	/*
	**	Check to find if any zone of the base is over defended. Such zones should have
	**	some of their defenses sold off to make better use of the money.
	*/

	/*
	**	Make sure that the core defense is only about 1/2 of the perimeter defense average.
	*/
	int average = 0;
	for (ZoneType z = ZONE_NORTH; z < ZONE_COUNT; z++) {
		average += ZoneInfo[z].AirDefense;
		average += ZoneInfo[z].ArmorDefense;
		average += ZoneInfo[z].InfantryDefense;
	}
	average /= (ZONE_COUNT-ZONE_NORTH);

	/*
	**	If the core value is greater than the average, then sell off some of the
	**	inner defensive structures.
	*/
	int core = ZoneInfo[ZONE_CORE].AirDefense + ZoneInfo[ZONE_CORE].ArmorDefense + ZoneInfo[ZONE_CORE].InfantryDefense;
	if (core >= average) {
		static StructType _stype[] = {
			STRUCT_GTOWER,
			STRUCT_TURRET,
			STRUCT_ATOWER,
			STRUCT_OBELISK,
			STRUCT_TESLA,
			STRUCT_SAM
		};
		BuildingClass * b;

		for (int index = 0; index < sizeof(_stype)/sizeof(_stype[0]); index++) {
			b = Find_Building(_stype[index], ZONE_CORE);
			if (b) {
				b->Sell_Back(1);
				break;
			}
		}
	}

	/*
	**	If the enemy doesn't have any offensive air capability, then sell off any
	**	SAM sites. Only do this when money is moderately low.
	*/
	if (Available_Money() < 1000 && (ActiveBScan & STRUCTF_SAM)) {

		/*
		**	Scan to find if ANY human opponents have aircraft or a helipad. If one
		** is found then consider that opponent to have a valid air threat potential.
		**	Don't sell off SAM sites in that case.
		*/
		bool nothreat = true;
		for (HousesType h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
			HouseClass * house = HouseClass::As_Pointer(h);

			if (house && house->IsActive && house->IsHuman && !Is_Ally(house)) {
				if ((house->ActiveAScan & (AIRCRAFTF_ORCA|AIRCRAFTF_TRANSPORT|AIRCRAFTF_HELICOPTER)) || (house->ActiveBScan & STRUCTF_HELIPAD)) {
					nothreat = false;
					break;
				}
			}
		}
	}

	return(TICKS_PER_SECOND*5);
}
#endif


/***********************************************************************************************
 * HouseClass::AI_Building -- Determines what building to build.                               *
 *                                                                                             *
 *    This routine handles the general case of determining what building to build next.        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine again. *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *   11/03/1996 JLB : Tries to match aircraft of enemy                                         *
 *=============================================================================================*/
int HouseClass::AI_Building(void)
{
	//assert(Houses.ID(this) == ID);

	if (BuildStructure != STRUCT_NONE) return(TICKS_PER_SECOND);

#if (0)	                               // Not used for GAME_NORMAL in C&C. ST - 7/23/2019 3:04PM
	if (Session.Type == GAME_NORMAL && Base.House == Class->House) {
		BaseNodeClass * node = Base.Next_Buildable();
		if (node) {
			BuildStructure = node->Type;
		}
	}
#endif

	if (IsBaseBuilding) {
		/*
		**	Don't suggest anything to build if the base is already big enough.
		*/
		unsigned int quant = 0;
		for (HousesType h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
			HouseClass const * hptr = HouseClass::As_Pointer(h);

			if (hptr != NULL && hptr->IsActive && hptr->IsHuman && quant < hptr->CurBuildings) {
				quant = hptr->CurBuildings;
			}
		}
		quant += Rule.BaseSizeAdd;

// TCTC -- Should multiply largest player base by some rational number.
//		if (CurBuildings >= quant) return(TICKS_PER_SECOND);

		BuildChoice.Free_All();
		BuildChoiceClass * choiceptr;
		StructType stype = STRUCT_NONE;
		int money = Available_Money();
		//int level = Control.TechLevel;
		bool hasincome = (BQuantity[STRUCT_REFINERY] > 0 && !IsTiberiumShort && UQuantity[UNIT_HARVESTER] > 0);
		BuildingTypeClass const * b = NULL;
		HouseClass const * enemy = NULL;
		if (Enemy != HOUSE_NONE) {
			enemy = HouseClass::As_Pointer(Enemy);
		}

		//level = Control.TechLevel;

		/*
		**	Try to build a power plant if there is insufficient power and there is enough
		**	money available.
		*/
		b = &BuildingTypeClass::As_Reference(STRUCT_ADVANCED_POWER);
		if (Can_Build(b, ActLike) && Power <= Drain+Rule.PowerSurplus && b->Cost_Of() < money) {
			choiceptr = BuildChoice.Alloc();
			if (choiceptr != NULL) {
				*choiceptr = BuildChoiceClass(BQuantity[STRUCT_REFINERY] == 0 ? URGENCY_LOW : URGENCY_MEDIUM, b->Type);
			}
		} else {
			b = &BuildingTypeClass::As_Reference(STRUCT_POWER);
			if (Can_Build(b, ActLike) && Power <= Drain+Rule.PowerSurplus && b->Cost_Of() < money) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					*choiceptr = BuildChoiceClass(BQuantity[STRUCT_REFINERY] == 0 ? URGENCY_LOW : URGENCY_MEDIUM, b->Type);
				}
			}
		}

		/*
		**	Build a refinery if there isn't one already available.
		*/
		unsigned int current = BQuantity[STRUCT_REFINERY];
		if (!IsTiberiumShort && current < Round_Up(Rule.RefineryRatio*fixed(CurBuildings)) && current < (unsigned)Rule.RefineryLimit) {
			b = &BuildingTypeClass::As_Reference(STRUCT_REFINERY);
			if (Can_Build(b, ActLike) && (money > b->Cost_Of() || hasincome)) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					*choiceptr = BuildChoiceClass(BQuantity[STRUCT_REFINERY] == 0 ? URGENCY_HIGH : URGENCY_MEDIUM, b->Type);
				}
			}
		}

		/*
		**	Always make sure there is a barracks available, but only if there
		**	will be sufficient money to train troopers.
		*/
		//current = BQuantity[STRUCT_BARRACKS] + BQuantity[STRUCT_TENT];
		current = BQuantity[STRUCT_BARRACKS] + BQuantity[STRUCT_HAND];
		if (current < Round_Up(Rule.BarracksRatio*fixed(CurBuildings)) && current < (unsigned)Rule.BarracksLimit && (money > 300 || hasincome)) {
			b = &BuildingTypeClass::As_Reference(STRUCT_BARRACKS);
			if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					*choiceptr = BuildChoiceClass(current > 0 ? URGENCY_LOW : URGENCY_MEDIUM, b->Type);
				}
			} else {
				//b = &BuildingTypeClass::As_Reference(STRUCT_TENT);
				b = &BuildingTypeClass::As_Reference(STRUCT_HAND);
				if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
					choiceptr = BuildChoice.Alloc();
					if (choiceptr != NULL) {
						*choiceptr = BuildChoiceClass(current > 0 ? URGENCY_LOW : URGENCY_MEDIUM, b->Type);
					}
				}
			}
		}
#if (0)
		/*
		**	Try to build one dog house.
		*/
		current = BQuantity[STRUCT_KENNEL];
		if (current < 1 && (money > 300 || hasincome)) {
			b = &BuildingTypeClass::As_Reference(STRUCT_KENNEL);
			if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					*choiceptr = BuildChoiceClass(URGENCY_MEDIUM, b->Type);
				}
			}
		}

		/*
		**	Try to build one gap generator.
		*/
		current = BQuantity[STRUCT_GAP];
		if (current < 1 && Power_Fraction() >= 1 && hasincome) {
			b = &BuildingTypeClass::As_Reference(STRUCT_GAP);
			if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					*choiceptr = BuildChoiceClass(URGENCY_MEDIUM, b->Type);
				}
			}
		}
#endif
		/*
		**	A source of combat vehicles is always needed, but only if there will
		**	be sufficient money to build vehicles.
		*/
		current = BQuantity[STRUCT_WEAP];
		if (current < Round_Up(Rule.WarRatio*fixed(CurBuildings)) && current < (unsigned)Rule.WarLimit && (money > 2000 || hasincome)) {
			b = &BuildingTypeClass::As_Reference(STRUCT_WEAP);
			if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					*choiceptr = BuildChoiceClass(current > 0 ? URGENCY_LOW : URGENCY_MEDIUM, b->Type);
				}
			}
		}

		/*
		**	Always build up some base defense.
		*/
		//current = BQuantity[STRUCT_PILLBOX] + BQuantity[STRUCT_CAMOPILLBOX] + BQuantity[STRUCT_TURRET] + BQuantity[STRUCT_FLAME_TURRET];
		current = BQuantity[STRUCT_TURRET] + BQuantity[STRUCT_OBELISK];
		if (current < Round_Up(Rule.DefenseRatio*fixed(CurBuildings)) && current < (unsigned)Rule.DefenseLimit) {
			//b = &BuildingTypeClass::As_Reference(STRUCT_FLAME_TURRET);
			b = &BuildingTypeClass::As_Reference(STRUCT_OBELISK);
			if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					*choiceptr = BuildChoiceClass(URGENCY_MEDIUM, b->Type);
				}
			} else {
				//if (Percent_Chance(50)) {
					//b = &BuildingTypeClass::As_Reference(STRUCT_PILLBOX);
					b = &BuildingTypeClass::As_Reference(STRUCT_TURRET);
					if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
						choiceptr = BuildChoice.Alloc();
						if (choiceptr != NULL) {
							*choiceptr = BuildChoiceClass(URGENCY_MEDIUM, b->Type);
						}
					}
#if (0)
				} else {
					b = &BuildingTypeClass::As_Reference(STRUCT_TURRET);
					if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
						choiceptr = BuildChoice.Alloc();
						if (choiceptr != NULL) {
							*choiceptr = BuildChoiceClass(URGENCY_MEDIUM, b->Type);
						}
					}
				}
#endif
			}
		}

		/*
		**	Build some air defense.
		*/
		//current = BQuantity[STRUCT_SAM] + BQuantity[STRUCT_AAGUN];
		current = BQuantity[STRUCT_SAM];
		if (current < Round_Up(Rule.AARatio*fixed(CurBuildings)) && current < (unsigned)Rule.AALimit) {

			/*
			**	Building air defense only makes sense if the opponent has aircraft
			**	of some kind.
			*/
			bool airthreat = false;
			int threat_quantity = 0;
			if (enemy != NULL && enemy->AScan != 0) {
				airthreat = true;
				threat_quantity = enemy->CurAircraft;
			}
			if (!airthreat) {
				for (HousesType house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
					HouseClass * h = HouseClass::As_Pointer(house);
					if (h != NULL && !Is_Ally(house) && h->AScan != 0) {
						airthreat = true;
						break;
					}
				}
			}

			if (airthreat) {

				if (BQuantity[STRUCT_RADAR] == 0) {
					b = &BuildingTypeClass::As_Reference(STRUCT_RADAR);
					if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
						choiceptr = BuildChoice.Alloc();
						if (choiceptr != NULL) {
							*choiceptr = BuildChoiceClass(URGENCY_HIGH, b->Type);
						}
					}
				}

				b = &BuildingTypeClass::As_Reference(STRUCT_SAM);
				if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
					choiceptr = BuildChoice.Alloc();
					if (choiceptr != NULL) {
						*choiceptr = BuildChoiceClass((current < (unsigned)threat_quantity) ? URGENCY_HIGH : URGENCY_MEDIUM, b->Type);
					}
				} else {
#if (0)					
					b = &BuildingTypeClass::As_Reference(STRUCT_AAGUN);
					if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
						choiceptr = BuildChoice.Alloc();
						if (choiceptr != NULL) {
							*choiceptr = BuildChoiceClass((current < (unsigned)threat_quantity) ? URGENCY_HIGH : URGENCY_MEDIUM, b->Type);
						}
					}
#endif
				}
			}
		}


#if (0)
		/*
		**	Advanced base defense would be good.
		*/
		current = BQuantity[STRUCT_TESLA];
		if (current < Round_Up(Rule.TeslaRatio*fixed(CurBuildings)) && current < (unsigned)Rule.TeslaLimit) {
			b = &BuildingTypeClass::As_Reference(STRUCT_TESLA);
			if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome) && Power_Fraction() >= 1) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					*choiceptr = BuildChoiceClass(URGENCY_MEDIUM, b->Type);
				}
			}
		}

		/*
		**	Build a tech center as soon as possible.
		*/
		current = BQuantity[STRUCT_ADVANCED_TECH] + BQuantity[STRUCT_SOVIET_TECH];
		if (current < 1) {
			b = &BuildingTypeClass::As_Reference(STRUCT_ADVANCED_TECH);
			if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome) && Power_Fraction() >= 1) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					*choiceptr = BuildChoiceClass(URGENCY_MEDIUM, b->Type);
				}
			} else {
				b = &BuildingTypeClass::As_Reference(STRUCT_SOVIET_TECH);
				if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome) && Power_Fraction() >= 1) {
					choiceptr = BuildChoice.Alloc();
					if (choiceptr != NULL) {
						*choiceptr = BuildChoiceClass(URGENCY_MEDIUM, b->Type);
					}
				}
			}
		}
#endif

		/*
		**	A helipad would be good.
		*/
		current = BQuantity[STRUCT_HELIPAD];
		if (current < Round_Up(Rule.HelipadRatio*fixed(CurBuildings)) && current < (unsigned)Rule.HelipadLimit) {
			b = &BuildingTypeClass::As_Reference(STRUCT_HELIPAD);
			if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					int threat_quantity = 0;
					if (enemy != NULL) {
						threat_quantity = enemy->CurAircraft;
					}

					*choiceptr = BuildChoiceClass((CurAircraft < (unsigned)threat_quantity) ? URGENCY_HIGH : URGENCY_MEDIUM, b->Type);
				}
			}
		}
		
		/*
		**	An airstrip would be good.
		*/
		current = BQuantity[STRUCT_AIRSTRIP];
		if (current < Round_Up(Rule.AirstripRatio*fixed(CurBuildings)) && current < (unsigned)Rule.AirstripLimit) {
			b = &BuildingTypeClass::As_Reference(STRUCT_AIRSTRIP);
			if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					int threat_quantity = 0;
					if (enemy != NULL) {
						threat_quantity = enemy->CurAircraft;
					}

					*choiceptr = BuildChoiceClass((CurAircraft < (unsigned)threat_quantity) ? URGENCY_HIGH : URGENCY_MEDIUM, b->Type);
				}
			}
		}

		/*
		**	Pick the choice that is the most urgent.
		*/
		UrgencyType best = URGENCY_NONE;
		int bestindex;
		for (int index = 0; index < BuildChoice.Count(); index++) {
			if (BuildChoice.Ptr(index)->Urgency > best) {
				bestindex = index;
				best = BuildChoice.Ptr(index)->Urgency;
			}
		}
		if (best != URGENCY_NONE) {
			BuildStructure = BuildChoice.Ptr(bestindex)->Structure;
		}
	}

	return(TICKS_PER_SECOND);
}


/***********************************************************************************************
 * HouseClass::AI_Unit -- Determines what unit to build next.                                  *
 *                                                                                             *
 *    This routine handles the general case of determining what units to build next.           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of games frames to delay before calling this routine again.*
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::AI_Unit(void)
{
	//assert(Houses.ID(this) == ID);

	if (BuildUnit != UNIT_NONE) return(TICKS_PER_SECOND);
	if (CurUnits >= Control.MaxUnit) return(TICKS_PER_SECOND);

	/*
	**	A computer controlled house will try to build a replacement
	**	harvester if possible.
	*/
	if (IQ >= Rule.IQHarvester && !IsTiberiumShort && !IsHuman && BQuantity[STRUCT_REFINERY] > UQuantity[UNIT_HARVESTER] && Difficulty != DIFF_HARD) {
		//if (UnitTypeClass::As_Reference(UNIT_HARVESTER).Level <= (unsigned)Control.TechLevel) {
		if (UnitTypeClass::As_Reference(UNIT_HARVESTER).Level <= (unsigned)BuildLevel) {
			BuildUnit = UNIT_HARVESTER;
			return(TICKS_PER_SECOND);
		}
	}

	//if (Session.Type == GAME_NORMAL) {  // Why? ST - 7/24/2019 2:38PM	

		int counter[UNIT_COUNT];
		memset(counter, 0x00, sizeof(counter));

		/*
		**	Build a list of the maximum of each type we wish to produce. This will be
		**	twice the number required to fill all teams.
		*/
		int index;
		for (index = 0; index < Teams.Count(); index++) {
			TeamClass * tptr = Teams.Ptr(index);
			if (tptr != NULL) {
				TeamTypeClass const * team = tptr->Class;
				//if (((team->IsReinforcable && !tptr->IsFullStrength) || (!tptr->IsForcedActive && !tptr->IsHasBeen && !tptr->JustAltered)) && team->House == Class->House) {
				if (((team->IsReinforcable && !tptr->IsFullStrength) || (!tptr->IsForcedActive && !tptr->IsHasBeen && !tptr->IsAltered)) && team->House == Class->House) {
					for (int subindex = 0; subindex < team->ClassCount; subindex++) {
						//TechnoTypeClass const * memtype = team->Members[subindex].Class;
						TechnoTypeClass const * memtype = team->Class[subindex];
						if (memtype->What_Am_I() == RTTI_UNITTYPE) {
							counter[((UnitTypeClass const *)memtype)->Type] = 1;
						}
					}
				}
			}
		}

		/*
		**	Team types that are flagged as prebuilt, will always try to produce enough
		**	to fill one team of this type regardless of whether there is a team active
		**	of that type.
		*/
		for (index = 0; index < TeamTypes.Count(); index++) {
			TeamTypeClass const * team = TeamTypes.Ptr(index);
			if (team != NULL && team->House == Class->House && team->IsPrebuilt && (!team->IsAutocreate || IsAlerted)) {
				for (int subindex = 0; subindex < team->ClassCount; subindex++) {
					//TechnoTypeClass const * memtype = team->Members[subindex].Class;
					TechnoTypeClass const * memtype = team->Class[subindex];

					if (memtype->What_Am_I() == RTTI_UNITTYPE) {
						int subtype = ((UnitTypeClass const *)memtype)->Type;
						//counter[subtype] = max(counter[subtype], team->Members[subindex].Quantity);
						counter[subtype] = max(counter[subtype], 1);
					}
				}
			}
		}

		/*
		**	Reduce the theoretical maximum by the actual number of objects currently
		**	in play.
		*/
		for (int uindex = 0; uindex < Units.Count(); uindex++) {
			UnitClass * unit = Units.Ptr(uindex);
			//if (unit != NULL && unit->Is_Recruitable(this) && counter[unit->Class->Type] > 0) {
			if (unit != NULL && counter[unit->Class->Type] > 0) {
				counter[unit->Class->Type]--;
			}
		}

		/*
		**	Pick to build the most needed object but don't consider those objects that
		**	can't be built because of scenario restrictions or insufficient cash.
		*/
		int bestval = -1;
		int bestcount = 0;
		UnitType bestlist[UNIT_COUNT];
		for (UnitType utype = UNIT_FIRST; utype < UNIT_COUNT; utype++) {
			if (counter[utype] > 0 && Can_Build(&UnitTypeClass::As_Reference(utype), Class->House) && UnitTypeClass::As_Reference(utype).Cost_Of() <= Available_Money()) {
				if (bestval == -1 || bestval < counter[utype]) {
					bestval = counter[utype];
					bestcount = 0;
				}
				bestlist[bestcount++] = utype;
			}
		}

		/*
		**	The unit type to build is now known. Fetch a pointer to the techno type class.
		*/
		if (bestcount) {
			BuildUnit = bestlist[Random_Pick(0, bestcount-1)];
		}
	//}

	if (IsBaseBuilding) {

		int counter[UNIT_COUNT];
		int total = 0;
		UnitType index;
		for (index = UNIT_FIRST; index < UNIT_COUNT; index++) {
			UnitTypeClass const * utype = &UnitTypeClass::As_Reference(index);
			if (Can_Build(utype, ActLike) && utype->Type != UNIT_HARVESTER) {
				//if (utype->PrimaryWeapon != NULL) {
				if (utype->Primary != WEAPON_NONE) {
					counter[index] = 20;
				} else {
					counter[index] = 1;
				}
			} else {
				counter[index] = 0;
			}
			total += counter[index];
		}

		if (total > 0) {
			int choice = Random_Pick(0, total-1);
			for (index = UNIT_FIRST; index < UNIT_COUNT; index++) {
				if (choice < counter[index]) {
					BuildUnit = index;
					break;
				}
				choice -= counter[index];
			}
		}
	}

	return(TICKS_PER_SECOND);
}


int HouseClass::AI_Vessel(void)
{
#if (0)
	//assert(Houses.ID(this) == ID);
	if (BuildVessel != VESSEL_NONE) return(TICKS_PER_SECOND);

	if (CurVessels >= Control.MaxVessel) {
		return(TICKS_PER_SECOND);
	}

	if (Session.Type == GAME_NORMAL) {

		int counter[VESSEL_COUNT];
		if (Session.Type == GAME_NORMAL) {
			memset(counter, 0x00, sizeof(counter));
		} else {
			for (VesselType index = VESSEL_FIRST; index < VESSEL_COUNT; index++) {
				//if (Can_Build(&VesselTypeClass::As_Reference(index), Class->House) && VesselTypeClass::As_Reference(index).Level <= (unsigned)Control.TechLevel) {
				if (Can_Build(&VesselTypeClass::As_Reference(index), Class->House) && VesselTypeClass::As_Reference(index).Level <= (unsigned)BuildLevel) {
					counter[index] = 16;
				} else {
					counter[index] = 0;
				}
			}
		}

		/*
		**	Build a list of the maximum of each type we wish to produce. This will be
		**	twice the number required to fill all teams.
		*/
		int index;
		for (index = 0; index < Teams.Count(); index++) {
			TeamClass * tptr = Teams.Ptr(index);
			if (tptr) {
				TeamTypeClass const * team = tptr->Class;

				//if (((team->IsReinforcable && !tptr->IsFullStrength) || (!tptr->IsForcedActive && !tptr->IsHasBeen && !tptr->JustAltered)) && team->House == Class->House) {
				if (((team->IsReinforcable && !tptr->IsFullStrength) || (!tptr->IsForcedActive && !tptr->IsHasBeen && !tptr->IsAltered)) && team->House == Class->House) {
					for (int subindex = 0; subindex < team->ClassCount; subindex++) {
						if (team->Members[subindex].Class->What_Am_I() == RTTI_VESSELTYPE) {
							counter[((VesselTypeClass const *)(team->Members[subindex].Class))->Type] = 1;
						}
					}
				}
			}
		}

		/*
		**	Team types that are flagged as prebuilt, will always try to produce enough
		**	to fill one team of this type regardless of whether there is a team active
		**	of that type.
		*/
		for (index = 0; index < TeamTypes.Count(); index++) {
			TeamTypeClass const * team = TeamTypes.Ptr(index);
			if (team) {
				if (team->House == Class->House && team->IsPrebuilt && (!team->IsAutocreate || IsAlerted)) {
					for (int subindex = 0; subindex < team->ClassCount; subindex++) {
						if (team->Members[subindex].Class->What_Am_I() == RTTI_VESSELTYPE) {
							int subtype = ((VesselTypeClass const *)(team->Members[subindex].Class))->Type;
							//counter[subtype] = max(counter[subtype], team->Members[subindex].Quantity);
							counter[subtype] = max(counter[subtype], 1);
						}
					}
				}
			}
		}

		/*
		**	Reduce the theoretical maximum by the actual number of objects currently
		**	in play.
		*/
		for (int vindex = 0; vindex < Vessels.Count(); vindex++) {
			VesselClass * unit = Vessels.Ptr(vindex);
			//if (unit != NULL && unit->Is_Recruitable(this) && counter[unit->Class->Type] > 0) {
			if (unit != NULL && counter[unit->Class->Type] > 0) {
				counter[unit->Class->Type]--;
			}
		}

		/*
		**	Pick to build the most needed object but don't consider those object that
		**	can't be built because of scenario restrictions or insufficient cash.
		*/
		int bestval = -1;
		int bestcount = 0;
		VesselType bestlist[VESSEL_COUNT];
		for (VesselType utype = VESSEL_FIRST; utype < VESSEL_COUNT; utype++) {
			if (counter[utype] > 0 && Can_Build(&VesselTypeClass::As_Reference(utype), Class->House) && VesselTypeClass::As_Reference(utype).Cost_Of() <= Available_Money()) {
				if (bestval == -1 || bestval < counter[utype]) {
					bestval = counter[utype];
					bestcount = 0;
				}
				bestlist[bestcount++] = utype;
			}
		}

		/*
		**	The unit type to build is now known. Fetch a pointer to the techno type class.
		*/
		if (bestcount) {
			BuildVessel = bestlist[Random_Pick(0, bestcount-1)];
		}
	}

	if (IsBaseBuilding) {
		BuildVessel = VESSEL_NONE;
	}
#endif
	return(TICKS_PER_SECOND);
}



/***********************************************************************************************
 * HouseClass::AI_Infantry -- Determines the infantry unit to build.                           *
 *                                                                                             *
 *    This routine handles the general case of determining what infantry unit to build         *
 *    next.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before being called again.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::AI_Infantry(void)
{
	//assert(Houses.ID(this) == ID);

	if (BuildInfantry != INFANTRY_NONE) return(TICKS_PER_SECOND);
	if (CurInfantry >= Control.MaxInfantry) return(TICKS_PER_SECOND);

	//if (Session.Type == GAME_NORMAL) {
	if (GameToPlay == GAME_NORMAL) {		// Not used for skirmish? ST - 7/24/2019 2:59PM
#if (0)		
		TechnoTypeClass const * techno = 0;
		int counter[INFANTRY_COUNT];
		memset(counter, 0x00, sizeof(counter));

		/*
		**	Build a list of the maximum of each type we wish to produce. This will be
		**	twice the number required to fill all teams.
		*/
		int index;
		for (index = 0; index < Teams.Count(); index++) {
			TeamClass * tptr = Teams.Ptr(index);
			if (tptr != NULL) {
				TeamTypeClass const * team = tptr->Class;

				//if (((team->IsReinforcable && !tptr->IsFullStrength) || (!tptr->IsForcedActive && !tptr->IsHasBeen && !tptr->JustAltered)) && team->House == Class->House) {
				if (((team->IsReinforcable && !tptr->IsFullStrength) || (!tptr->IsForcedActive && !tptr->IsHasBeen && !tptr->IsAltered)) && team->House == Class->House) {
					for (int subindex = 0; subindex < team->ClassCount; subindex++) {
						if (team->Members[subindex].Class->What_Am_I() == RTTI_INFANTRYTYPE) {
							//counter[((InfantryTypeClass const *)(team->Members[subindex].Class))->Type] += team->Members[subindex].Quantity + (team->IsReinforcable ? 1 : 0);
							counter[((InfantryTypeClass const *)(team->Members[subindex].Class))->Type] += 1 + (team->IsReinforcable ? 1 : 0);
						}
					}
				}
			}
		}

		/*
		**	Team types that are flagged as prebuilt, will always try to produce enough
		**	to fill one team of this type regardless of whether there is a team active
		**	of that type.
		*/
		for (index = 0; index < TeamTypes.Count(); index++) {
			TeamTypeClass const * team = TeamTypes.Ptr(index);
			if (team != NULL) {
				if (team->House == Class->House && team->IsPrebuilt && (!team->IsAutocreate || IsAlerted)) {
					for (int subindex = 0; subindex < team->ClassCount; subindex++) {
						if (team->Members[subindex].Class->What_Am_I() == RTTI_INFANTRYTYPE) {
							int subtype = ((InfantryTypeClass const *)(team->Members[subindex].Class))->Type;
//									counter[subtype] = 1;
							//counter[subtype] = max(counter[subtype], team->Members[subindex].Quantity);
							counter[subtype] = max(counter[subtype], 1);
							counter[subtype] = min(counter[subtype], 5);
						}
					}
				}
			}
		}

		/*
		**	Reduce the theoretical maximum by the actual number of objects currently
		**	in play.
		*/
		for (int uindex = 0; uindex < Infantry.Count(); uindex++) {
			InfantryClass * infantry = Infantry.Ptr(uindex);
			//if (infantry != NULL && infantry->Is_Recruitable(this) && counter[infantry->Class->Type] > 0) {
			if (infantry != NULL && counter[infantry->Class->Type] > 0) {
				counter[infantry->Class->Type]--;
			}
		}

		/*
		**	Pick to build the most needed object but don't consider those object that
		**	can't be built because of scenario restrictions or insufficient cash.
		*/
		int bestval = -1;
		int bestcount = 0;
		InfantryType bestlist[INFANTRY_COUNT];
		for (InfantryType utype = INFANTRY_FIRST; utype < INFANTRY_COUNT; utype++) {

			if (utype != INFANTRY_DOG || !(IScan & INFANTRYF_DOG)) {
				if (counter[utype] > 0 && Can_Build(&InfantryTypeClass::As_Reference(utype), Class->House) && InfantryTypeClass::As_Reference(utype).Cost_Of() <= Available_Money()) {
					if (bestval == -1 || bestval < counter[utype]) {
						bestval = counter[utype];
						bestcount = 0;
					}
					bestlist[bestcount++] = utype;
				}
			}
		}

		/*
		**	The infantry type to build is now known. Fetch a pointer to the techno type class.
		*/
		if (bestcount) {
			int pick = Random_Pick(0, bestcount-1);
			BuildInfantry = bestlist[pick];
		}
#endif
	}

	if (IsBaseBuilding) {
		HouseClass const * enemy = NULL;
		if (Enemy != HOUSE_NONE) {
			enemy = HouseClass::As_Pointer(Enemy);
		}

		/*
		**	This structure is used to keep track of the list of infantry types that should be
		**	built. The infantry type and the value assigned to it is recorded.
		*/
		struct {
			InfantryType	Type;		// Infantry type.
			int				Value;	// Relative value assigned.
		} typetrack[INFANTRY_COUNT];
		int count = 0;
		int total = 0;
		for (InfantryType index = INFANTRY_FIRST; index < INFANTRY_COUNT; index++) {
			//if (Can_Build(&InfantryTypeClass::As_Reference(index), ActLike) && InfantryTypeClass::As_Reference(index).Level <= (unsigned)Control.TechLevel) {
			if (Can_Build(&InfantryTypeClass::As_Reference(index), ActLike) && InfantryTypeClass::As_Reference(index).Level <= (unsigned)BuildLevel) {
				typetrack[count].Value = 0;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98 This looks like a potential bug. It is prob. for save game format compatibility.
				int clipindex = index;
				if (clipindex >= INFANTRY_RA_COUNT) clipindex -= INFANTRY_RA_COUNT;
				if ((enemy != NULL && enemy->IQuantity[clipindex] > IQuantity[clipindex]) || Available_Money() > Rule.InfantryReserve || CurInfantry < CurBuildings * Rule.InfantryBaseMult) {
#else
				if ((enemy != NULL && enemy->IQuantity[index] > IQuantity[index]) || Available_Money() > Rule.InfantryReserve || CurInfantry < CurBuildings * Rule.InfantryBaseMult) {
#endif

					switch (index) {
						case INFANTRY_E1:
							typetrack[count].Value = 3;
							break;

						case INFANTRY_E2:
							typetrack[count].Value = 5;
							break;

						case INFANTRY_E3:
							typetrack[count].Value = 2;
							break;

						case INFANTRY_E4:
							typetrack[count].Value = 5;
							break;

						//case INFANTRY_RENOVATOR:
						case INFANTRY_E7:
							if (CurInfantry > 5) {
								typetrack[count].Value = 1 - max(IQuantity[index], 0);
							}
							break;

						//case INFANTRY_TANYA:
						//	typetrack[count].Value = 1 - max(IQuantity[index], 0);
						//	break;

						default:
							typetrack[count].Value = 0;
							break;
					}
				}

				if (typetrack[count].Value > 0) {
					typetrack[count].Type = index;
					total += typetrack[count].Value;
					count++;
				}
			}
		}

		/*
		**	If there is at least one choice, then pick it. The object picked
		**	is influenced by the weight (value) assigned to it. This is accomplished
		**	by picking a number between 0 and the total weight value. The appropriate
		**	infantry object that matches the number picked is then selected to be built.
		*/
		if (count > 0) {
			int pick = Random_Pick(0, total-1);
			for (int index = 0; index < count; index++) {
				if (pick < typetrack[index].Value) {
					BuildInfantry = typetrack[index].Type;
					break;
				}
				pick -= typetrack[index].Value;
			}
		}
	}
	return(TICKS_PER_SECOND);
}


/***********************************************************************************************
 * HouseClass::AI_Aircraft -- Determines what aircraft to build next.                          *
 *                                                                                             *
 *    This routine is used to determine the general case of what aircraft to build next.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of frame to delay before calling this routine again.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::AI_Aircraft(void)
{
	//assert(Houses.ID(this) == ID);

	if (!IsHuman && IQ >= Rule.IQAircraft) {
		if (BuildAircraft != AIRCRAFT_NONE) return(TICKS_PER_SECOND);
		if (CurAircraft >= Control.MaxAircraft) return(TICKS_PER_SECOND);

		if (Can_Build(&AircraftTypeClass::As_Reference(AIRCRAFT_HELICOPTER), ActLike) &&
				 //AircraftTypeClass::As_Reference(AIRCRAFT_MIG).Level <= (unsigned)Control.TechLevel &&
				 AircraftTypeClass::As_Reference(AIRCRAFT_HELICOPTER).Level <= (unsigned)BuildLevel &&
				 BQuantity[STRUCT_AIRSTRIP] > AQuantity[AIRCRAFT_HELICOPTER] + AQuantity[AIRCRAFT_ORCA]) {
			BuildAircraft = AIRCRAFT_HELICOPTER;
			return(TICKS_PER_SECOND);
		}

		if (Can_Build(&AircraftTypeClass::As_Reference(AIRCRAFT_ORCA), ActLike) &&
				 //AircraftTypeClass::As_Reference(AIRCRAFT_YAK).Level <= (unsigned)Control.TechLevel &&
				 AircraftTypeClass::As_Reference(AIRCRAFT_ORCA).Level <= (unsigned)BuildLevel &&
				 BQuantity[STRUCT_AIRSTRIP] > AQuantity[AIRCRAFT_ORCA] + AQuantity[AIRCRAFT_HELICOPTER]) {
			BuildAircraft = AIRCRAFT_ORCA;
			return(TICKS_PER_SECOND);
		}

#if (0)
		if (Can_Build(&AircraftTypeClass::As_Reference(AIRCRAFT_LONGBOW), ActLike) &&
				 //AircraftTypeClass::As_Reference(AIRCRAFT_LONGBOW).Level <= (unsigned)Control.TechLevel &&
				 AircraftTypeClass::As_Reference(AIRCRAFT_LONGBOW).Level <= (unsigned)BuildLevel &&
				 BQuantity[STRUCT_HELIPAD] > AQuantity[AIRCRAFT_LONGBOW] + AQuantity[AIRCRAFT_HIND]) {
			BuildAircraft = AIRCRAFT_LONGBOW;
			return(TICKS_PER_SECOND);
		}

		if (Can_Build(&AircraftTypeClass::As_Reference(AIRCRAFT_HIND), ActLike) &&
				 //AircraftTypeClass::As_Reference(AIRCRAFT_HIND).Level <= (unsigned)Control.TechLevel &&
				 AircraftTypeClass::As_Reference(AIRCRAFT_HIND).Level <= (unsigned)BuildLevel &&
				 BQuantity[STRUCT_HELIPAD] > AQuantity[AIRCRAFT_LONGBOW] + AQuantity[AIRCRAFT_HIND]) {
			BuildAircraft = AIRCRAFT_HIND;
			return(TICKS_PER_SECOND);
		}

		if (Can_Build(&AircraftTypeClass::As_Reference(AIRCRAFT_MIG), ActLike) &&
				 //AircraftTypeClass::As_Reference(AIRCRAFT_MIG).Level <= (unsigned)Control.TechLevel &&
				 AircraftTypeClass::As_Reference(AIRCRAFT_MIG).Level <= (unsigned)BuildLevel &&
				 BQuantity[STRUCT_AIRSTRIP] > AQuantity[AIRCRAFT_MIG] + AQuantity[AIRCRAFT_YAK]) {
			BuildAircraft = AIRCRAFT_MIG;
			return(TICKS_PER_SECOND);
		}

		if (Can_Build(&AircraftTypeClass::As_Reference(AIRCRAFT_YAK), ActLike) &&
				 //AircraftTypeClass::As_Reference(AIRCRAFT_YAK).Level <= (unsigned)Control.TechLevel &&
				 AircraftTypeClass::As_Reference(AIRCRAFT_YAK).Level <= (unsigned)BuildLevel &&
				 BQuantity[STRUCT_AIRSTRIP] > AQuantity[AIRCRAFT_MIG] + AQuantity[AIRCRAFT_YAK]) {
			BuildAircraft = AIRCRAFT_YAK;
			return(TICKS_PER_SECOND);
		}
#endif
	}

	return(TICKS_PER_SECOND);
}


/***********************************************************************************************
 * HouseClass::Production_Begun -- Records that production has begun.                          *
 *                                                                                             *
 *    This routine is used to inform the Expert System that production of the specified object *
 *    has begun. This allows the AI to proceed with picking another object to begin production *
 *    on.                                                                                      *
 *                                                                                             *
 * INPUT:   product  -- Pointer to the object that production has just begun on.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Production_Begun(TechnoClass const * product)
{
	//assert(Houses.ID(this) == ID);

	if (product != NULL) {
		switch (product->What_Am_I()) {
			case RTTI_UNIT:
				if (*((UnitClass*)product) == BuildUnit) {
					BuildUnit = UNIT_NONE;
				}
				break;
#if (0)
			case RTTI_VESSEL:
				if (*((VesselClass*)product) == BuildVessel) {
					BuildVessel = VESSEL_NONE;
				}
				break;
#endif
			case RTTI_INFANTRY:
				if (*((InfantryClass*)product) == BuildInfantry) {
					BuildInfantry = INFANTRY_NONE;
				}
				break;

			case RTTI_BUILDING:
				if (*((BuildingClass*)product) == BuildStructure) {
					BuildStructure = STRUCT_NONE;
				}
				break;

			case RTTI_AIRCRAFT:
				if (*((AircraftClass*)product) == BuildAircraft) {
					BuildAircraft = AIRCRAFT_NONE;
				}
				break;

			default:
				break;
		}
	}
}


/***********************************************************************************************
 * HouseClass::Tracking_Remove -- Remove object from house tracking system.                    *
 *                                                                                             *
 *    This routine informs the Expert System that the specified object is no longer part of    *
 *    this house's inventory. This occurs when the object is destroyed or captured.            *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object to remove from the tracking systems of this      *
 *                      house.                                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Tracking_Remove(TechnoClass const * techno)
{
	//assert(Houses.ID(this) == ID);

	int type;

	switch (techno->What_Am_I()) {
		case RTTI_BUILDING:
			CurBuildings--;
			BQuantity[((BuildingTypeClass const &)techno->Class_Of()).Type]--;
			break;

		case RTTI_AIRCRAFT:
			CurAircraft--;
			AQuantity[((AircraftTypeClass const &)techno->Class_Of()).Type]--;
			break;

		case RTTI_INFANTRY:
			CurInfantry--;
			if (!((InfantryClass *)techno)->IsTechnician) {
				type = ((InfantryTypeClass const &)techno->Class_Of()).Type;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				if (type >= INFANTRY_RA_COUNT) type -= INFANTRY_RA_COUNT;
#endif
				IQuantity[type]--;
			}
			break;

		case RTTI_UNIT:
			CurUnits--;
			type = ((UnitTypeClass const &)techno->Class_Of()).Type;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			if (type >= UNIT_RA_COUNT) type -= UNIT_RA_COUNT;
#endif
			UQuantity[type]--;
			break;

#if (0)
		case RTTI_VESSEL:
			CurVessels--;
			type = ((VesselTypeClass const &)techno->Class_Of()).Type;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			if (type >= VESSEL_RA_COUNT) type -= VESSEL_RA_COUNT;
#endif
			VQuantity[type]--;
			break;
#endif

		default:
			break;
	}
}


/***********************************************************************************************
 * HouseClass::Tracking_Add -- Informs house of new inventory item.                            *
 *                                                                                             *
 *    This function is called when the specified object is now available as part of the house's*
 *    inventory. This occurs when the object is newly produced and also when it is captured    *
 *    by this house.                                                                           *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object that is now part of the house inventory.         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Tracking_Add(TechnoClass const * techno)
{
	//assert(Houses.ID(this) == ID);

	StructType building;
	AircraftType aircraft;
	InfantryType infantry;
	UnitType unit;
	//VesselType vessel;
	//int quant;

	switch (techno->What_Am_I()) {
		case RTTI_BUILDING:
			CurBuildings++;
			building = ((BuildingTypeClass const &)techno->Class_Of()).Type;
			BQuantity[building]++;
			BScan |= (1L << building);
#if (0)	                               // This is a stats thing. ST - 7/24/2019 3:08PM
			if (Session.Type == GAME_INTERNET) {
				BuildingTotals->Increment_Unit_Total(techno->Class_Of().ID);
			}
#endif
			break;

		case RTTI_AIRCRAFT:
			CurAircraft++;
			aircraft = ((AircraftTypeClass const &)techno->Class_Of()).Type;
			AQuantity[aircraft]++;
			AScan |= (1L << aircraft);
#if (0)	                               // This is a stats thing. ST - 7/24/2019 3:08PM
			if (Session.Type == GAME_INTERNET) {
				AircraftTotals->Increment_Unit_Total(techno->Class_Of().ID);
			}
#endif
			break;

		case RTTI_INFANTRY:
			CurInfantry++;
			infantry = ((InfantryTypeClass const &)techno->Class_Of()).Type;
			if (!((InfantryClass *)techno)->IsTechnician) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				quant = infantry;
				if (quant >= INFANTRY_RA_COUNT) quant -= INFANTRY_RA_COUNT;
				IQuantity[quant]++;
#else
				IQuantity[infantry]++;
#endif

#if (0)	                               // This is a stats thing. ST - 7/24/2019 3:08PM
				if (!((InfantryTypeClass const &)techno->Class_Of()).IsCivilian && Session.Type == GAME_INTERNET) {
					InfantryTotals->Increment_Unit_Total(techno->Class_Of().ID);
				}
#endif
				IScan |= (1L << infantry);
			}
			break;

		case RTTI_UNIT:
			CurUnits++;
			unit = ((UnitTypeClass const &)techno->Class_Of()).Type;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			quant = unit;
			if (quant >= UNIT_RA_COUNT) quant -= UNIT_RA_COUNT;
			UQuantity[quant]++;
#else
			UQuantity[unit]++;
#endif
			UScan |= (1L << unit);
#if (0)	                               // This is a stats thing. ST - 7/24/2019 3:08PM
			if (Session.Type == GAME_INTERNET) {
				UnitTotals->Increment_Unit_Total(techno->Class_Of().ID);
			}
#endif
			break;


#if (0)
		case RTTI_VESSEL:
			CurVessels++;
			vessel = ((VesselTypeClass const &)techno->Class_Of()).Type;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			quant = vessel;
			if (quant >= VESSEL_RA_COUNT) quant -= VESSEL_RA_COUNT;
			VQuantity[quant]++;
#else
			VQuantity[vessel]++;
#endif
			VScan |= (1L << vessel);
			if (Session.Type == GAME_INTERNET) {
				VesselTotals->Increment_Unit_Total(techno->Class_Of().ID);
			}
			break;
#endif

		default:
			break;
	}
}


/***********************************************************************************************
 * HouseClass::Factory_Counter -- Fetches a pointer to the factory counter value.              *
 *                                                                                             *
 *    Use this routine to fetch a pointer to the variable that holds the number of factories   *
 *    that can produce the specified object type. This is a helper routine used when           *
 *    examining the number of factories as well as adjusting their number.                     *
 *                                                                                             *
 * INPUT:   rtti  -- The RTTI of the object that could be produced.                            *
 *                                                                                             *
 * OUTPUT:  Returns with the number of factories owned by this house that could produce the    *
 *          object of the type specified.                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int * HouseClass::Factory_Counter(RTTIType rtti)
{
	switch (rtti) {
		case RTTI_UNITTYPE:
		case RTTI_UNIT:
			return(&UnitFactories);
#if (0)
		case RTTI_VESSELTYPE:
		case RTTI_VESSEL:
			return(&VesselFactories);
#endif
		case RTTI_AIRCRAFTTYPE:
		case RTTI_AIRCRAFT:
			return(&AircraftFactories);

		case RTTI_INFANTRYTYPE:
		case RTTI_INFANTRY:
			return(&InfantryFactories);

		case RTTI_BUILDINGTYPE:
		case RTTI_BUILDING:
			return(&BuildingFactories);

		default:
			break;
	}
	return(NULL);
}


/***********************************************************************************************
 * HouseClass::Active_Remove -- Remove this object from active duty for this house.            *
 *                                                                                             *
 *    This routine will recognize the specified object as having been removed from active      *
 *    duty.                                                                                    *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object to remove from active duty.                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Active_Remove(TechnoClass const * techno)
{
	if (techno == NULL) return;

	if (techno->What_Am_I() == RTTI_BUILDING) {
		int * fptr = Factory_Counter(((BuildingClass *)techno)->Class->ToBuild);
		if (fptr != NULL) {
			*fptr = *fptr - 1;
		}
	}
}


/***********************************************************************************************
 * HouseClass::Active_Add -- Add an object to active duty for this house.                      *
 *                                                                                             *
 *    This routine will recognize the specified object as having entered active duty. Any      *
 *    abilities granted to the house by that object are now available.                         *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object that is entering active duty.                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Active_Add(TechnoClass const * techno)
{
	if (techno == NULL) return;

	if (techno->What_Am_I() == RTTI_BUILDING) {
		int * fptr = Factory_Counter(((BuildingClass *)techno)->Class->ToBuild);
		if (fptr != NULL) {
			*fptr = *fptr + 1;
		}
	}
}


/***********************************************************************************************
 * HouseClass::Which_Zone -- Determines what zone a coordinate lies in.                        *
 *                                                                                             *
 *    This routine will determine what zone the specified coordinate lies in with respect to   *
 *    this house's base. A location that is too distant from the base, even though it might    *
 *    be a building, is not considered part of the base and returns ZONE_NONE.                 *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to examine.                                                *
 *                                                                                             *
 * OUTPUT:  Returns with the base zone that the specified coordinate lies in.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ZoneType HouseClass::Which_Zone(COORDINATE coord) const
{
	//assert(Houses.ID(this) == ID);

	if (coord == 0) return(ZONE_NONE);

	int distance = Distance(Center, coord);
	if (distance <= Radius) return(ZONE_CORE);
	if (distance > Radius*4) return(ZONE_NONE);

	DirType facing = Direction(Center, coord);
	if (facing < DIR_NE || facing > DIR_NW) return(ZONE_NORTH);
	if (facing >= DIR_NE && facing < DIR_SE) return(ZONE_EAST);
	if (facing >= DIR_SE && facing < DIR_SW) return(ZONE_SOUTH);
	return(ZONE_WEST);
}


/***********************************************************************************************
 * HouseClass::Which_Zone -- Determines which base zone the specified object lies in.          *
 *                                                                                             *
 *    Use this routine to determine what zone the specified object lies in.                    *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that will be checked for zone occupation.        *
 *                                                                                             *
 * OUTPUT:  Returns with the base zone that the object lies in. For objects that are too       *
 *          distant from the center of the base, ZONE_NONE is returned.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ZoneType HouseClass::Which_Zone(ObjectClass const * object) const
{
	//assert(Houses.ID(this) == ID);

	if (!object) return(ZONE_NONE);
	return(Which_Zone(object->Center_Coord()));
}


/***********************************************************************************************
 * HouseClass::Which_Zone -- Determines which base zone the specified cell lies in.            *
 *                                                                                             *
 *    This routine is used to determine what base zone the specified cell is in.               *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine.                                                      *
 *                                                                                             *
 * OUTPUT:  Returns the base zone that the cell lies in or ZONE_NONE if the cell is too far    *
 *          away.                                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ZoneType HouseClass::Which_Zone(CELL cell) const
{
	//assert(Houses.ID(this) == ID);

	return(Which_Zone(Cell_Coord(cell)));
}


/***********************************************************************************************
 * HouseClass::Recalc_Attributes -- Recalcs all houses existence bits.                         *
 *                                                                                             *
 *    This routine will go through all game objects and reset the existence bits for the       *
 *    owning house. This method ensures that if the object exists, then the corresponding      *
 *    existence bit is also set.                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Recalc_Attributes(void)
{
	/*
	**	Clear out all tracking values that will be filled in by this
	**	routine. This allows the filling in process to not worry about
	**	old existing values.
	*/
	int index;
	for (index = 0; index < Houses.Count(); index++) {
		HouseClass * house = Houses.Ptr(index);

		if (house != NULL) {
			house->BScan = 0;
			house->ActiveBScan = 0;
			house->IScan = 0;
			house->ActiveIScan = 0;
			house->UScan = 0;
			house->ActiveUScan = 0;
			house->AScan = 0;
			house->ActiveAScan = 0;
#if (0)
			house->VScan = 0;
			house->ActiveVScan = 0;
#endif
		}
	}

	/*
	**	A second pass through the sentient objects is required so that the appropriate scan
	**	bits will be set for the owner house.
	*/
	for (index = 0; index < Units.Count(); index++) {
		UnitClass const * unit = Units.Ptr(index);
		unit->House->UScan |= (1L << unit->Class->Type);
		//if (unit->IsLocked && (Session.Type != GAME_NORMAL || !unit->House->IsHuman || unit->IsDiscoveredByPlayer)) {
		if (unit->IsLocked) {
			if (!unit->IsInLimbo) {
				unit->House->ActiveUScan |= (1L << unit->Class->Type);
			}
		}
	}
	for (index = 0; index < Infantry.Count(); index++) {
		InfantryClass const * infantry = Infantry.Ptr(index);
		infantry->House->IScan |= (1L << infantry->Class->Type);
		//if (infantry->IsLocked && (Session.Type != GAME_NORMAL || !infantry->House->IsHuman || infantry->IsDiscoveredByPlayer)) {
		if (infantry->IsLocked) {
			if (!infantry->IsInLimbo) {
				infantry->House->ActiveIScan |= (1L << infantry->Class->Type);
				//infantry->House->OldIScan |= (1L << infantry->Class->Type);
			}
		}
	}
	for (index = 0; index < Aircraft.Count(); index++) {
		AircraftClass const * aircraft = Aircraft.Ptr(index);
		aircraft->House->AScan |= (1L << aircraft->Class->Type);
		//if (aircraft->IsLocked && (Session.Type != GAME_NORMAL || !aircraft->House->IsHuman || aircraft->IsDiscoveredByPlayer)) {
		if (aircraft->IsLocked) {
			if (!aircraft->IsInLimbo) {
				aircraft->House->ActiveAScan |= (1L << aircraft->Class->Type);
				//aircraft->House->OldAScan |= (1L << aircraft->Class->Type);
			}
		}
	}
	for (index = 0; index < Buildings.Count(); index++) {
		BuildingClass const * building = Buildings.Ptr(index);
		if (building->Class->Type < 32) {
			building->House->BScan |= (1L << building->Class->Type);
			//if (building->IsLocked && (Session.Type != GAME_NORMAL || !building->House->IsHuman || building->IsDiscoveredByPlayer)) {
			if (building->IsLocked) {
				if (!building->IsInLimbo) {
					building->House->ActiveBScan |= (1L << building->Class->Type);
					building->House->OldBScan |= (1L << building->Class->Type);
				}
			}
		}
	}
#if (0)
	for (index = 0; index < Vessels.Count(); index++) {
		VesselClass const * vessel = Vessels.Ptr(index);
		vessel->House->VScan |= (1L << vessel->Class->Type);
		if (vessel->IsLocked && (Session.Type != GAME_NORMAL || !vessel->House->IsHuman || vessel->IsDiscoveredByPlayer)) {
			if (!vessel->IsInLimbo) {
				vessel->House->ActiveVScan |= (1L << vessel->Class->Type);
				vessel->House->OldVScan |= (1L << vessel->Class->Type);
			}
		}
	}
#endif
}


/***********************************************************************************************
 * HouseClass::Zone_Cell -- Finds the cell closest to the center of the zone.                  *
 *                                                                                             *
 *    This routine is used to find the cell that is closest to the center point of the         *
 *    zone specified. Typical use of this routine is for building and unit placement so that   *
 *    they can "cover" the specified zone.                                                     *
 *                                                                                             *
 * INPUT:   zone  -- The zone that the center point is to be returned.                         *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that is closest to the center point of the zone specified.   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL HouseClass::Zone_Cell(ZoneType zone) const
{
	//assert(Houses.ID(this) == ID);

	switch (zone) {
		case ZONE_CORE:
			return(Coord_Cell(Center));

		case ZONE_NORTH:
			return(Coord_Cell(Coord_Move(Center, DIR_N, Radius*3)));

		case ZONE_EAST:
			return(Coord_Cell(Coord_Move(Center, DIR_E, Radius*3)));

		case ZONE_WEST:
			return(Coord_Cell(Coord_Move(Center, DIR_W, Radius*3)));

		case ZONE_SOUTH:
			return(Coord_Cell(Coord_Move(Center, DIR_S, Radius*3)));

		default:
			break;
	}
	return(0);
}


/***********************************************************************************************
 * HouseClass::Where_To_Go -- Determines where the object should go and wait.                  *
 *                                                                                             *
 *    This function is called for every new unit produced or delivered in order to determine   *
 *    where the unit should "hang out" to await further orders. The best area for the          *
 *    unit to loiter is returned as a cell location.                                           *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that needs to know where to go.                  *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that the unit should move to.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *   11/04/1996 JLB : Simplified to use helper functions                                       *
 *=============================================================================================*/
CELL HouseClass::Where_To_Go(FootClass const * object) const
{
	//assert(Houses.ID(this) == ID);
	//assert(object != NULL);

	ZoneType zone;			// The zone that the object should go to.
	if (object->Anti_Air() + object->Anti_Armor() + object->Anti_Infantry() == 0) {
		zone = ZONE_CORE;
	} else {
		zone = Random_Pick(ZONE_NORTH, ZONE_WEST);
	}

	CELL cell = Random_Cell_In_Zone(zone);
	//assert(cell != 0);

	return(Map.Nearby_Location(cell));	//, SPEED_TRACK, Map[cell].Zones[MZONE_NORMAL], MZONE_NORMAL));
}


/***********************************************************************************************
 * HouseClass::Find_Juicy_Target -- Finds a suitable field target.                             *
 *                                                                                             *
 *    This routine is used to find targets out in the field and away from base defense.        *
 *    Typical of this would be the attack helicopters and the roving attack bands of           *
 *    hunter killers.                                                                          *
 *                                                                                             *
 * INPUT:   coord -- The coordinate of the attacker. Closer targets are given preference.      *
 *                                                                                             *
 * OUTPUT:  Returns with a suitable target to attack.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET HouseClass::Find_Juicy_Target(COORDINATE coord) const
{
	//assert(Houses.ID(this) == ID);

	UnitClass * best = 0;
	int value = 0;

	for (int index = 0; index < Units.Count(); index++) {
		UnitClass * unit = Units.Ptr(index);

		if (unit && !unit->IsInLimbo && !Is_Ally(unit) && unit->House->Which_Zone(unit) == ZONE_NONE) {
			int val = Distance(coord, unit->Center_Coord());

			if (unit->Anti_Air()) val *= 2;

			if (*unit == UNIT_HARVESTER) val /= 2;

			if (value == 0 || val < value) {
				value = val;
				best = unit;
			}
		}
	}
	if (best) {
		return(best->As_Target());
	}
	return(TARGET_NONE);
}


/***********************************************************************************************
 * HouseClass::Get_Quantity -- Fetches the total number of aircraft of the specified type.     *
 *                                                                                             *
 *    Call this routine to fetch the total quantity of aircraft of the type specified that is  *
 *    owned by this house.                                                                     *
 *                                                                                             *
 * INPUT:   aircraft -- The aircraft type to check the quantity of.                            *
 *                                                                                             *
 * OUTPUT:  Returns with the total quantity of all aircraft of that type that is owned by this *
 *          house.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int  HouseClass::Get_Quantity(AircraftType aircraft)
{
	return(AQuantity[aircraft]);
}


/***********************************************************************************************
 * HouseClass::Fetch_Factory -- Finds the factory associated with the object type specified.   *
 *                                                                                             *
 *    This is the counterpart to the Set_Factory function. It will return with a factory       *
 *    pointer that is associated with the object type specified.                               *
 *                                                                                             *
 * INPUT:   rtti  -- The RTTI of the object type to find the factory for.                      *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the factory (if present) that can manufacture the        *
 *          object type specified.                                                             *
 *                                                                                             *
 * WARNINGS:   If this returns a non-NULL pointer, then the factory is probably already busy   *
 *             producing another unit of that category.                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
FactoryClass * HouseClass::Fetch_Factory(RTTIType rtti) const
{
	int factory_index = -1;

	switch (rtti) {
		case RTTI_INFANTRY:
		case RTTI_INFANTRYTYPE:
			factory_index = InfantryFactory;
			break;

		case RTTI_UNIT:
		case RTTI_UNITTYPE:
			factory_index = UnitFactory;
			break;

		case RTTI_BUILDING:
		case RTTI_BUILDINGTYPE:
			factory_index = BuildingFactory;
			break;

		case RTTI_AIRCRAFT:
		case RTTI_AIRCRAFTTYPE:
			factory_index = AircraftFactory;
			break;
#if (0)
		case RTTI_VESSEL:
		case RTTI_VESSELTYPE:
			factory_index = VesselFactory;
			break;
#endif
		default:
			factory_index = -1;
			break;
	}

	/*
	**	Fetch the actual pointer to the factory object. If there is
	**	no object factory that matches the specified rtti type, then
	**	null is returned.
	*/
	if (factory_index != -1) {
		return(Factories.Raw_Ptr(factory_index));
	}
	return(NULL);
}


/***********************************************************************************************
 * HouseClass::Set_Factory -- Assign specified factory to house tracking.                      *
 *                                                                                             *
 *    Call this routine when a factory has been created and it now must be passed on to the    *
 *    house for tracking purposes. The house maintains several factory pointers and this       *
 *    routine will ensure that the factory pointer gets stored correctly.                      *
 *                                                                                             *
 * INPUT:   rtti  -- The RTTI of the object the factory it to manufacture.                     *
 *                                                                                             *
 *          factory  -- The factory object pointer.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Set_Factory(RTTIType rtti, FactoryClass * factory)
{
	int * factory_index = 0;

	//assert(rtti != RTTI_NONE);

	switch (rtti) {
		case RTTI_UNIT:
		case RTTI_UNITTYPE:
			factory_index = &UnitFactory;
			break;

		case RTTI_INFANTRY:
		case RTTI_INFANTRYTYPE:
			factory_index = &InfantryFactory;
			break;
#if (0)
		case RTTI_VESSEL:
		case RTTI_VESSELTYPE:
			factory_index = &VesselFactory;
			break;
#endif
		case RTTI_BUILDING:
		case RTTI_BUILDINGTYPE:
			factory_index = &BuildingFactory;
			break;

		case RTTI_AIRCRAFT:
		case RTTI_AIRCRAFTTYPE:
			factory_index = &AircraftFactory;
			break;
	}

	//assert(factory_index != NULL);

	/*
	**	Assign the factory to the appropriate slot. For the case of clearing
	**	the factory out, then -1 is assigned.
	*/
	if (factory != NULL) {
		//*factory_index = factory->ID;
		*factory_index = Factories.ID(factory);
	} else {
		*factory_index = -1;
	}
}


/***********************************************************************************************
 * HouseClass::Factory_Count -- Fetches the number of factories for specified type.            *
 *                                                                                             *
 *    This routine will count the number of factories owned by this house that can build       *
 *    objects of the specified type.                                                           *
 *                                                                                             *
 * INPUT:   rtti  -- The type of object (RTTI) that the factories are to be counted for.       *
 *                                                                                             *
 * OUTPUT:  Returns with the number of factories that can build the object type specified.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::Factory_Count(RTTIType rtti) const
{
	int const * ptr = ((HouseClass *)this)->Factory_Counter(rtti);
	if (ptr != NULL) {
		return(*ptr);
	}
	return(0);
}


/***********************************************************************************************
 * HouseClass::Get_Quantity -- Gets the quantity of the building type specified.               *
 *                                                                                             *
 *    This will return the total number of buildings of that type owned by this house.         *
 *                                                                                             *
 * INPUT:   building -- The building type to check.                                            *
 *                                                                                             *
 * OUTPUT:  Returns with the number of buildings of that type owned by this house.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int  HouseClass::Get_Quantity(StructType building)
{
	return(BQuantity[building]);
}


/***********************************************************************************************
 * HouseClass::Read_INI -- Reads house specific data from INI.                                 *
 *                                                                                             *
 *    This routine reads the house specific data for a particular                              *
 *    scenario from the scenario INI file. Typical data includes starting                      *
 *    credits, maximum unit count, etc.                                                        *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to loaded scenario INI file.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1994 JLB : Created.                                                                 *
 *   05/18/1995 JLB : Creates all houses.                                                      *
 *=============================================================================================*/
#if (0) // ST 7/17/2019
void HouseClass::Read_INI(CCINIClass & ini)
{
	HouseClass 	* p;				// Pointer to current player data.
	char const	* hname;			//	Pointer to house name.

	for (HousesType index = HOUSE_FIRST; index < HOUSE_COUNT; index++) {
		hname = HouseTypeClass::As_Reference(index).IniName;

		p = new HouseClass(index);
		p->Control.TechLevel = ini.Get_Int(hname, "TechLevel", Scen.Scenario);
		p->Control.MaxBuilding = ini.Get_Int(hname, "MaxBuilding", p->Control.MaxBuilding);
		p->Control.MaxUnit = ini.Get_Int(hname, "MaxUnit", p->Control.MaxUnit);
		p->Control.MaxInfantry = ini.Get_Int(hname, "MaxInfantry", p->Control.MaxInfantry);
		p->Control.MaxVessel = ini.Get_Int(hname, "MaxVessel", p->Control.MaxVessel);
		if (p->Control.MaxVessel == 0) p->Control.MaxVessel = p->Control.MaxUnit;
		p->Control.InitialCredits = ini.Get_Int(hname, "Credits", 0) * 100;
		p->Credits = p->Control.InitialCredits;

		int iq = ini.Get_Int(hname, "IQ", 0);
		if (iq > Rule.MaxIQ) iq = 1;
		p->IQ = p->Control.IQ = iq;

		p->Control.Edge = ini.Get_SourceType(hname, "Edge", SOURCE_NORTH);
		p->IsPlayerControl = ini.Get_Bool(hname, "PlayerControl", false);

		int owners = ini.Get_Owners(hname, "Allies", (1 << HOUSE_NEUTRAL));
		p->Make_Ally(index);
		p->Make_Ally(HOUSE_NEUTRAL);
		for (HousesType h = HOUSE_FIRST; h < HOUSE_COUNT; h++) {
			if ((owners & (1 << h)) != 0) {
				p->Make_Ally(h);
			}
		}
	}
}
#endif

/***********************************************************************************************
 * HouseClass::Write_INI -- Writes the house data to the INI database.                         *
 *                                                                                             *
 *    This routine will write out all data necessary to recreate it in anticipation of a       *
 *    new scenario. All houses (that are active) will have their scenario type data written    *
 *    out.                                                                                     *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to write the data to.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#if (0) // ST 7/17/2019
void HouseClass::Write_INI(CCINIClass & ini)
{
	/*
	**	The identity house control object. Only if the house value differs from the
	**	identity, will the data be written out.
	*/
	HouseStaticClass control;

	for (HousesType i = HOUSE_FIRST; i < HOUSE_COUNT; i++) {
		HouseClass * p = As_Pointer(i);

		if (p != NULL) {
			char const * name = p->Class->IniName;

			ini.Clear(name);
			if (i >= HOUSE_MULTI1) continue;

			if (p->Control.InitialCredits != control.InitialCredits) {
				ini.Put_Int(name, "Credits", (int)(p->Control.InitialCredits / 100));
			}

			if (p->Control.Edge != control.Edge) {
				ini.Put_SourceType(name, "Edge", p->Control.Edge);
			}

			if (p->Control.MaxUnit > 0 && p->Control.MaxUnit != control.MaxUnit) {
				ini.Put_Int(name, "MaxUnit", p->Control.MaxUnit);
			}

			if (p->Control.MaxInfantry > 0 && p->Control.MaxInfantry != control.MaxInfantry) {
				ini.Put_Int(name, "MaxInfantry", p->Control.MaxInfantry);
			}

			if (p->Control.MaxBuilding > 0 && p->Control.MaxBuilding != control.MaxBuilding) {
				ini.Put_Int(name, "MaxBuilding", p->Control.MaxBuilding);
			}

			if (p->Control.MaxVessel > 0 && p->Control.MaxVessel != control.MaxVessel) {
				ini.Put_Int(name, "MaxVessel", p->Control.MaxVessel);
			}

			if (p->Control.TechLevel != control.TechLevel) {
				ini.Put_Int(name, "TechLevel", p->Control.TechLevel);
			}

			if (p->Control.IQ != control.IQ) {
				ini.Put_Int(name, "IQ", p->Control.IQ);
			}

			if (p->IsPlayerControl != false && p != PlayerPtr) {
				ini.Put_Bool(name, "PlayerControl", p->IsPlayerControl);
			}

			ini.Put_Owners(name, "Allies", p->Control.Allies & ~((1 << p->Class->House) | (1 << HOUSE_NEUTRAL)));
		}
	}
}
#endif


#if (0)
/***********************************************************************************************
 * HouseClass::Is_No_YakMig -- Determines if no more yaks or migs should be allowed.           *
 *                                                                                             *
 *    This routine will examine the current yak and mig situation verses airfields. If there   *
 *    are equal aircraft to airfields, then this routine will return TRUE.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Are all airfields full and thus no more yaks or migs are allowed?            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Is_No_YakMig(void) const
{
	int quantity = AQuantity[AIRCRAFT_YAK] + AQuantity[AIRCRAFT_MIG];

	/*
	**	Adjust the quantity down one if there is an aircraft in production. This will
	**	allow production to resume after being held.
	*/
	FactoryClass const * factory = Fetch_Factory(RTTI_AIRCRAFT);
	if (factory != NULL && factory->Get_Object() != NULL) {
		AircraftClass const * air = (AircraftClass const *)factory->Get_Object();
		if (*air == AIRCRAFT_MIG || *air == AIRCRAFT_YAK) {
			quantity -= 1;
		}
	}

	if (quantity >= BQuantity[STRUCT_AIRSTRIP]) {
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::Is_Hack_Prevented -- Is production of the specified type and id prohibted?      *
 *                                                                                             *
 *    This is a special hack check routine to see if the object type and id specified is       *
 *    prevented from being produced. The Yak and the Mig are so prevented if there would be    *
 *    insufficient airfields for them to land upon.                                            *
 *                                                                                             *
 * INPUT:   rtti  -- The RTTI type of the value specified.                                     *
 *                                                                                             *
 *          value -- The type number (according to the RTTI type specified).                   *
 *                                                                                             *
 * OUTPUT:  bool; Is production of this object prohibited?                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Is_Hack_Prevented(RTTIType rtti, int value) const
{
	if (rtti == RTTI_AIRCRAFTTYPE && (value == AIRCRAFT_MIG || value == AIRCRAFT_YAK)) {
		return(Is_No_YakMig());
	}
	return(false);
}
#endif

/***********************************************************************************************
 * HouseClass::Fire_Sale -- Cause all buildings to be sold.                                    *
 *                                                                                             *
 *    This routine will sell back all buildings owned by this house.                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was a fire sale performed?                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Fire_Sale(void)
{
	if (CurBuildings > 0) {
		for (int index = 0; index < Buildings.Count(); index++) {
			BuildingClass * b = Buildings.Ptr(index);

			if (b != NULL && !b->IsInLimbo && b->House == this && b->Strength > 0) {
				b->Sell_Back(1);
			}
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::Do_All_To_Hunt -- Send all units to hunt.                                       *
 *                                                                                             *
 *    This routine will cause all combatants of this house to go into hunt mode. The effect of *
 *    this is to throw everything this house has to muster at the enemies of this house.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *   10/02/1996 JLB : Handles aircraft too.                                                    *
 *=============================================================================================*/
void HouseClass::Do_All_To_Hunt(void) const
{
	int index;

	for (index = 0; index < Units.Count(); index++) {
		UnitClass * unit = Units.Ptr(index);

		if (unit->House == this && unit->IsDown && !unit->IsInLimbo) {
			if (unit->Team) unit->Team->Remove(unit);
			unit->Assign_Mission(MISSION_HUNT);
		}
	}

	for (index = 0; index < Infantry.Count(); index++) {
		InfantryClass * infantry = Infantry.Ptr(index);

		if (infantry->House == this && infantry->IsDown && !infantry->IsInLimbo) {
			if (infantry->Team) infantry->Team->Remove(infantry);
			infantry->Assign_Mission(MISSION_HUNT);
		}
	}

#if (0)
	for (index = 0; index < Vessels.Count(); index++) {
		VesselClass * vessel = Vessels.Ptr(index);

		if (vessel->House == this && vessel->IsDown && !vessel->IsInLimbo) {
			if (vessel->Team) vessel->Team->Remove(vessel);
			vessel->Assign_Mission(MISSION_HUNT);
		}
	}
#endif

	for (index = 0; index < Aircraft.Count(); index++) {
		AircraftClass * aircraft = Aircraft.Ptr(index);

		if (aircraft->House == this && aircraft->IsDown && !aircraft->IsInLimbo) {
			if (aircraft->Team) aircraft->Team->Remove(aircraft);
			aircraft->Assign_Mission(MISSION_HUNT);
		}
	}
}


/***********************************************************************************************
 * HouseClass::Is_Allowed_To_Ally -- Determines if this house is allied to make allies.        *
 *                                                                                             *
 *    Use this routine to determine if this house is legally allowed to ally with the          *
 *    house specified. There are many reason why an alliance is not allowed. Typically, this   *
 *    is when there would be no more opponents left to fight or if this house has been         *
 *    defeated.                                                                                *
 *                                                                                             *
 * INPUT:   house -- The house that alliance with is desired.                                  *
 *                                                                                             *
 * OUTPUT:  bool; Is alliance with the house specified prohibited?                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Is_Allowed_To_Ally(HousesType house) const
{
	/*
	**	Is not allowed to ally with a house that is patently invalid, such
	**	as one that is illegally defined.
	*/
	if (house == HOUSE_NONE) {
		return(false);
	}

	/*
	**	One cannot ally twice with the same house.
	*/
	if (Is_Ally(house)) {
		return(false);
	}

	/*
	**	If the scenario is being set up, then alliances are always
	**	allowed. No further checking is required.
	*/
	if (ScenarioInit) {
		return(true);
	}

	/*
	**	Alliances (outside of scneario init time) are allowed only if
	**	this is a multiplayer game. Otherwise, they are prohibited.
	*/
	//if (Session.Type == GAME_NORMAL) {
	if (GameToPlay == GAME_NORMAL) {
		return(false);
	}

	/*
	**	When the house is defeated, it can no longer make alliances.
	*/
	if (IsDefeated) {
		return(false);
	}

#ifdef FIXIT_VERSION_3
	// Fix to prevent ally with computer.
	if ( !HouseClass::As_Pointer(house)->IsHuman) {
		return(false);
	}
#else	//	FIXIT_VERSION_3
#ifdef FIXIT_NO_COMP_ALLY
	// Fix to prevent ally with computer.
	if (PlayingAgainstVersion > VERSION_RED_ALERT_104 && !HouseClass::As_Pointer(house)->IsHuman) {
		return(false);
	}
#endif
#endif	//	FIXIT_VERSION_3

	/*
	**	Count the number of active houses in the game as well as the
	**	number of existing allies with this house.
	*/
	int housecount = 0;
	int allycount = 0;
	for (HousesType house2 = HOUSE_MULTI1; house2 < HOUSE_COUNT; house2++) {
		HouseClass * hptr = HouseClass::As_Pointer(house2);
		if (hptr != NULL && hptr->IsActive && !hptr->IsDefeated) {
			housecount++;
			if (Is_Ally(hptr)) {
				allycount++;
			}
		}
	}

	/*
	**	Alliance is not allowed if there wouldn't be any enemies left to
	**	fight.
	*/
	if (housecount == allycount+1) {
		return(false);
	}

	return(true);
}


/***********************************************************************************************
 * HouseClass::Computer_Paranoid -- Cause the computer players to becom paranoid.              *
 *                                                                                             *
 *    This routine will cause the computer players to become suspicious of the human           *
 *    players and thus the computer players will band together in order to defeat the          *
 *    human players.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Computer_Paranoid(void)
{
	if (GameToPlay != GAME_GLYPHX_MULTIPLAYER) {		// Re-enable this for multiplayer if we support classic team/ally mode. ST - 10/29/2019
	
		/*
		**	Loop through every computer controlled house and make allies with all other computer
		**	controlled houses and then make enemies with all other human controlled houses.
		*/
		for (HousesType house = HOUSE_MULTI1; house < HOUSE_COUNT; house++) {
			HouseClass * hptr = HouseClass::As_Pointer(house);
			if (hptr != NULL && hptr->IsActive && !hptr->IsDefeated && !hptr->IsHuman) {
				hptr->IsParanoid = true;

				/*
				**	Break alliance with every human it is allied with and make friends with
				**	any other computer players.
				*/
				for (HousesType house2 = HOUSE_MULTI1; house2 < HOUSE_COUNT; house2++) {
					HouseClass * hptr2 = HouseClass::As_Pointer(house2);
					if (hptr2 != NULL && hptr2->IsActive && !hptr2->IsDefeated) {
						if (hptr2->IsHuman) {
							hptr->Make_Enemy(house2);
						} else {
							hptr->Make_Ally(house2);
						}
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * HouseClass::Adjust_Power -- Adjust the power value of the house.                            *
 *                                                                                             *
 *    This routine will update the power output value of the house. It will cause any buildgins*
 *    that need to be redrawn to do so.                                                        *
 *                                                                                             *
 * INPUT:   adjust   -- The amount to adjust the power output value.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Adjust_Power(int adjust)
{
	Power += adjust;
}


/***********************************************************************************************
 * HouseClass::Adjust_Drain -- Adjust the power drain value of the house.                      *
 *                                                                                             *
 *    This routine will update the drain value of the house. It will cause any buildings that  *
 *    need to be redraw to do so.                                                              *
 *                                                                                             *
 * INPUT:   adjust   -- The amount to adjust the drain (positive means more drain).            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Adjust_Drain(int adjust)
{
	Drain += adjust;
}


/***********************************************************************************************
 * HouseClass::Find_Cell_In_Zone -- Finds a legal placement cell within the zone.              *
 *                                                                                             *
 *    Use this routine to determine where the specified object should go if it were to go      *
 *    some random (but legal) location within the zone specified.                              *
 *                                                                                             *
 * INPUT:   techno   -- The object that is desirous of going into the zone specified.          *
 *                                                                                             *
 *          zone     -- The zone to find a location within.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that the specified object could be placed in the zone. If    *
 *          no valid location could be found, then 0 is returned.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1996 JLB : Created.                                                                 *
 *   11/04/1996 JLB : Not so strict on zone requirement.                                       *
 *=============================================================================================*/
CELL HouseClass::Find_Cell_In_Zone(TechnoClass const * techno, ZoneType zone) const
{
	if (techno == NULL) return(0);

	int bestval = -1;
	int bestcell = 0;
	TechnoTypeClass const * ttype = techno->Techno_Type_Class();

	/*
	**	Pick a random location within the zone specified.
	*/
	CELL trycell = Random_Cell_In_Zone(zone);

	short const * list = NULL;
	if (techno->What_Am_I() == RTTI_BUILDING) {
		list = techno->Occupy_List(true);
	}

	/*
	**	Find a legal placement position as close as possible to the picked location while still
	**	remaining within the zone.
	*/
	for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
//		if (Map.In_Radar(cell)) {
		if (Map.In_Radar(cell) && Which_Zone(cell) != ZONE_NONE) {
			bool ok = ttype->Legal_Placement(cell);

			/*
			**	Another (adjacency) check is required for buildings.
			*/
			if (ok && list != NULL && !Map.Passes_Proximity_Check(ttype, techno->House->Class->House, list, cell)) {
				ok = false;
			}

			if (ok) {
				int dist = Distance(Cell_Coord(cell), Cell_Coord(trycell));
				if (bestval == -1 || dist < bestval) {
					bestval = dist;
					bestcell = cell;
				}
			}
		}
	}

	/*
	**	Return the best location to move to.
	*/
	return(bestcell);
}


/***********************************************************************************************
 * HouseClass::Random_Cell_In_Zone -- Find a (technically) legal cell in the zone specified.   *
 *                                                                                             *
 *    This routine will pick a random cell within the zone specified. The pick will be         *
 *    clipped to the map edge when necessary.                                                  *
 *                                                                                             *
 * INPUT:   zone  -- The zone to pick a cell from.                                             *
 *                                                                                             *
 * OUTPUT:  Returns with a picked cell within the zone. If the entire zone lies outside of the *
 *          map, then a cell in the core zone is returned instead.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
CELL HouseClass::Random_Cell_In_Zone(ZoneType zone) const
{
	COORDINATE coord = 0;
	int maxdist = 0;
	switch (zone) {
		case ZONE_CORE:
			coord = Coord_Scatter(Center, Random_Pick(0, Radius), true);
			break;

		case ZONE_NORTH:
			maxdist = min(Radius*3, (Coord_Y(Center) - Cell_To_Lepton(Map.MapCellY)) - CELL_LEPTON_H);
			if (maxdist < 0) break;
			coord = Coord_Move(Center, (DirType)(Random_Pick(DIR_N, DIR_E)-((DirType)32)), Random_Pick(min(Radius*2, maxdist), min(Radius*3, maxdist)));
			break;

		case ZONE_EAST:
			maxdist = min(Radius*3, (Cell_To_Lepton(Map.MapCellX + Map.MapCellWidth) - Coord_X(Center)) - CELL_LEPTON_W);
			if (maxdist < 0) break;
			coord = Coord_Move(Center, Random_Pick(DIR_NE, DIR_SE), Random_Pick(min(Radius*2, maxdist), min(Radius*3, maxdist)));
			break;

		case ZONE_SOUTH:
			maxdist = min(Radius*3, (Cell_To_Lepton(Map.MapCellY + Map.MapCellHeight) - Coord_Y(Center)) - CELL_LEPTON_H);
			if (maxdist < 0) break;
			coord = Coord_Move(Center, Random_Pick(DIR_SE, DIR_SW), Random_Pick(min(Radius*2, maxdist), min(Radius*3, maxdist)));
			break;

		case ZONE_WEST:
			maxdist = min(Radius*3, (Coord_X(Center) - Cell_To_Lepton(Map.MapCellX)) - CELL_LEPTON_W);
			if (maxdist < 0) break;
			coord = Coord_Move(Center, Random_Pick(DIR_SW, DIR_NW), Random_Pick(min(Radius*2, maxdist), min(Radius*3, maxdist)));
			break;
	}

	/*
	**	Double check that the location is valid and if so, convert it into a cell
	**	number.
	*/
	CELL cell;
	if (coord == 0 || !Map.In_Radar(Coord_Cell(coord))) {
		if (zone == ZONE_CORE) {

			/*
			**	Finding a cell within the core failed, so just pick the center
			**	cell. This cell is guaranteed to be valid.
			*/
			cell = Coord_Cell(Center);
		} else {

			/*
			**	If the edge fails, then try to find a cell within the core.
			*/
			cell = Random_Cell_In_Zone(ZONE_CORE);
		}
	} else {
		cell = Coord_Cell(coord);
	}

	/*
	**	If the randomly picked location is not in the legal map area, then clip it to
	**	the legal map area.
	*/
	if (!Map.In_Radar(cell)) {
		int x = Cell_X(cell);
		int y = Cell_Y(cell);

		if (x < Map.MapCellX) x = Map.MapCellX;
		if (y < Map.MapCellY) y = Map.MapCellY;
		if (x >= Map.MapCellX + Map.MapCellWidth) x = Map.MapCellX + Map.MapCellWidth -1;
		if (y >= Map.MapCellY + Map.MapCellHeight) y = Map.MapCellY + Map.MapCellHeight -1;
		cell = XY_Cell(x, y);
	}
	return(cell);
}

/***********************************************************************************************
 * HouseClass::Get_Ally_Flags --  Get the bit flags denoting the allies this house has.		  *
 *                                                                                             *
 * INPUT:   none																										  *
 *                                                                                             *
 * OUTPUT:  Returns the bit field storing which houses this house is allied with.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/12/2019 JAS : Created.                                                                 *
 *=============================================================================================*/
unsigned HouseClass::Get_Ally_Flags()
{
	return Allies;
}

#endif