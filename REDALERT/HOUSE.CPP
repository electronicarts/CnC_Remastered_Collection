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

/* $Header: /counterstrike/HOUSE.CPP 4     3/13/97 7:11p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
 *                  Last Update : November 4, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   HouseClass::AI -- Process house logic.                                                    *
 *   HouseClass::AI_Aircraft -- Determines what aircraft to build next.                        *
 *   HouseClass::AI_Attack -- Handles offensive attack logic.                                  *
 *   HouseClass::AI_Base_Defense -- Handles maintaining a strong base defense.                 *
 *   HouseClass::AI_Building -- Determines what building to build.                             *
 *   HouseClass::AI_Fire_Sale -- Check for and perform a fire sale.                            *
 *   HouseClass::AI_Infantry -- Determines the infantry unit to build.                         *
 *   HouseClass::AI_Money_Check -- Handles money production logic.                             *
 *   HouseClass::AI_Power_Check -- Handle the power situation.                                 *
 *   HouseClass::AI_Unit -- Determines what unit to build next.                                *
 *   HouseClass::Abandon_Production -- Abandons production of item type specified.             *
 *   HouseClass::Active_Add -- Add an object to active duty for this house.                    *
 *   HouseClass::Active_Remove -- Remove this object from active duty for this house.          *
 *   HouseClass::Adjust_Capacity -- Adjusts the house Tiberium storage capacity.               *
 *   HouseClass::Adjust_Drain -- Adjust the power drain value of the house.                    *
 *   HouseClass::Adjust_Power -- Adjust the power value of the house.                          *
 *   HouseClass::Adjust_Threat -- Adjust threat for the region specified.                      *
 *   HouseClass::As_Pointer -- Converts a house number into a house object pointer.            *
 *   HouseClass::Assign_Handicap -- Assigns the specified handicap rating to the house.        *
 *   HouseClass::Attacked -- Lets player know if base is under attack.                         *
 *   HouseClass::Available_Money -- Fetches the total credit worth of the house.               *
 *   HouseClass::Begin_Production -- Starts production of the specified object type.           *
 *   HouseClass::Blowup_All -- blows up everything                                             *
 *   HouseClass::Can_Build -- General purpose build legality checker.                          *
 *   HouseClass::Clobber_All -- removes all objects for this house                             *
 *   HouseClass::Computer_Paranoid -- Cause the computer players to becom paranoid.            *
 *   HouseClass::Debug_Dump -- Dumps the house status data to the mono screen.                 *
 *   HouseClass::Detach -- Removes specified object from house tracking systems.               *
 *   HouseClass::Do_All_To_Hunt -- Send all units to hunt.                                     *
 *   HouseClass::Does_Enemy_Building_Exist -- Checks for enemy building of specified type.     *
 *   HouseClass::Expert_AI -- Handles expert AI processing.                                    *
 *   HouseClass::Factory_Count -- Fetches the number of factories for specified type.          *
 *   HouseClass::Factory_Counter -- Fetches a pointer to the factory counter value.            *
 *   HouseClass::Fetch_Factory -- Finds the factory associated with the object type specified. *
 *   HouseClass::Find_Build_Location -- Finds a suitable building location.                    *
 *   HouseClass::Find_Building -- Finds a building of specified type.                          *
 *   HouseClass::Find_Cell_In_Zone -- Finds a legal placement cell within the zone.            *
 *   HouseClass::Find_Juicy_Target -- Finds a suitable field target.                           *
 *   HouseClass::Fire_Sale -- Cause all buildings to be sold.                                  *
 *   HouseClass::Flag_Attach -- Attach flag to specified cell (or thereabouts).                *
 *   HouseClass::Flag_Attach -- Attaches the house flag the specified unit.                    *
 *   HouseClass::Flag_Remove -- Removes the flag from the specified target.                    *
 *   HouseClass::Flag_To_Die -- Flags the house to blow up soon.                               *
 *   HouseClass::Flag_To_Lose -- Flags the house to die soon.                                  *
 *   HouseClass::Flag_To_Win -- Flags the house to win soon.                                   *
 *   HouseClass::Get_Quantity -- Fetches the total number of aircraft of the specified type.   *
 *   HouseClass::Get_Quantity -- Gets the quantity of the building type specified.             *
 *   HouseClass::Harvested -- Adds Tiberium to the harvest storage.                            *
 *   HouseClass::HouseClass -- Constructor for a house object.                                 *
 *   HouseClass::Init -- init's in preparation for new scenario                                *
 *   HouseClass::Init_Data -- Initializes the multiplayer color data.                          *
 *   HouseClass::Is_Allowed_To_Ally -- Determines if this house is allied to make allies.      *
 *   HouseClass::Is_Ally -- Checks to see if the object is an ally.                            *
 *   HouseClass::Is_Ally -- Determines if the specified house is an ally.                      *
 *   HouseClass::Is_Hack_Prevented -- Is production of the specified type and id prohibted?    *
 *   HouseClass::Is_No_YakMig -- Determines if no more yaks or migs should be allowed.         *
 *   HouseClass::MPlayer_Defeated -- multiplayer; house is defeated                            *
 *   HouseClass::Make_Ally -- Make the specified house an ally.                                *
 *   HouseClass::Make_Enemy -- Make an enemy of the house specified.                           *
 *   HouseClass::Manual_Place -- Inform display system of building placement mode.             *
 *   HouseClass::One_Time -- Handles one time initialization of the house array.               *
 *   HouseClass::Place_Object -- Places the object (building) at location specified.           *
 *   HouseClass::Place_Special_Blast -- Place a special blast effect at location specified.    *
 *   HouseClass::Power_Fraction -- Fetches the current power output rating.                    *
 *   HouseClass::Production_Begun -- Records that production has begun.                        *
 *   HouseClass::Read_INI -- Reads house specific data from INI.                               *
 *   HouseClass::Recalc_Attributes -- Recalcs all houses existence bits.                       *
 *   HouseClass::Recalc_Center -- Recalculates the center point of the base.                   *
 *   HouseClass::Refund_Money -- Refunds money to back to the house.                           *
 *   HouseClass::Remap_Table -- Fetches the remap table for this house object.                 *
 *   HouseClass::Sell_Wall -- Tries to sell the wall at the specified location.                *
 *   HouseClass::Set_Factory -- Assign specified factory to house tracking.                    *
 *   HouseClass::Silo_Redraw_Check -- Flags silos to be redrawn if necessary.                  *
 *   HouseClass::Special_Weapon_AI -- Fires special weapon.                                    *
 *   HouseClass::Spend_Money -- Removes money from the house.                                  *
 *   HouseClass::Suggest_New_Building -- Examines the situation and suggests a building.       *
 *   HouseClass::Suggest_New_Object -- Determine what would the next buildable object be.      *
 *   HouseClass::Suggested_New_Team -- Determine what team should be created.                  *
 *   HouseClass::Super_Weapon_Handler -- Handles the super weapon charge and discharge logic.  *
 *   HouseClass::Suspend_Production -- Temporarily puts production on hold.                    *
 *   HouseClass::Tally_Score -- Fills in the score system for this round                       *
 *   HouseClass::Tiberium_Fraction -- Calculates the tiberium fraction of capacity.            *
 *   HouseClass::Tracking_Add -- Informs house of new inventory item.                          *
 *   HouseClass::Tracking_Remove -- Remove object from house tracking system.                  *
 *   HouseClass::Where_To_Go -- Determines where the object should go and wait.                *
 *   HouseClass::Which_Zone -- Determines what zone a coordinate lies in.                      *
 *   HouseClass::Which_Zone -- Determines which base zone the specified cell lies in.          *
 *   HouseClass::Which_Zone -- Determines which base zone the specified object lies in.        *
 *   HouseClass::Write_INI -- Writes the house data to the INI database.                       *
 *   HouseClass::Zone_Cell -- Finds the cell closest to the center of the zone.                *
 *   HouseClass::delete -- Deallocator function for a house object.                            *
 *   HouseClass::new -- Allocator for a house class.                                           *
 *   HouseClass::operator HousesType -- Conversion to HousesType operator.                     *
 *   HouseClass::~HouseClass -- Default destructor for a house object.                         *
 *   HouseStaticClass::HouseStaticClass -- Default constructor for house static class.         *
 *   HouseClass::AI_Raise_Power -- Try to raise power levels by selling off buildings.         *
 *   HouseClass::AI_Raise_Money -- Raise emergency cash by selling buildings.                  *
 *   HouseClass::Random_Cell_In_Zone -- Find a (technically) legal cell in the zone specified. *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"vortex.h"

//#include "WolDebug.h"

/*
** New sidebar for GlyphX multiplayer. ST - 8/7/2019 10:10AM
*/
#include "SidebarGlyphx.h"

TFixedIHeapClass<HouseClass::BuildChoiceClass> HouseClass::BuildChoice;

int TFixedIHeapClass<HouseClass::BuildChoiceClass>::Save(Pipe &) const
{
	return(true);
}

int TFixedIHeapClass<HouseClass::BuildChoiceClass>::Load(Straw &)
{
	return(0);
}

void TFixedIHeapClass<HouseClass::BuildChoiceClass>::Code_Pointers(void)
{
}

void TFixedIHeapClass<HouseClass::BuildChoiceClass>::Decode_Pointers(void)
{
}

extern bool RedrawOptionsMenu;

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
	assert(Houses.ID(this) == ID);

	return(Class->House);
}


/***********************************************************************************************
 * HouseClass::Tiberium_Fraction -- Calculates the tiberium fraction of capacity.              *
 *                                                                                             *
 *    This will calculate the current tiberium (gold) load as a ratio of the maximum storage   *
 *    capacity.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the current tiberium storage situation as a ratio of load over capacity.   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/31/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
fixed HouseClass::Tiberium_Fraction(void) const
{
	if (Tiberium == 0) {
		return(0);
	}
	return(fixed(Tiberium, Capacity));
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
	if (house != HOUSE_NONE) {
		for (int index = 0; index < Houses.Count(); index++) {
			if (Houses.Ptr(index)->Class->House == house) {
				return(Houses.Ptr(index));
			}
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
	BuildChoice.Set_Heap(STRUCT_COUNT);
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

	if (Session.Type != GAME_NORMAL) {
		HouseTypeClass const * hptr = &HouseTypeClass::As_Reference(ActLike);
		FirepowerBias = hptr->FirepowerBias * Rule.Diff[handicap].FirepowerBias;
		GroundspeedBias = hptr->GroundspeedBias * Rule.Diff[handicap].GroundspeedBias * Rule.GameSpeedBias;
		AirspeedBias = hptr->AirspeedBias * Rule.Diff[handicap].AirspeedBias * Rule.GameSpeedBias;
		ArmorBias = hptr->ArmorBias * Rule.Diff[handicap].ArmorBias;
		ROFBias = hptr->ROFBias * Rule.Diff[handicap].ROFBias;
		CostBias = hptr->CostBias * Rule.Diff[handicap].CostBias;
		RepairDelay = Rule.Diff[handicap].RepairDelay;
		BuildDelay = Rule.Diff[handicap].BuildDelay;
		BuildSpeedBias = hptr->BuildSpeedBias * Rule.Diff[handicap].BuildSpeedBias * Rule.GameSpeedBias;
	} else {
		FirepowerBias = Rule.Diff[handicap].FirepowerBias;
		GroundspeedBias = Rule.Diff[handicap].GroundspeedBias * Rule.GameSpeedBias;
		AirspeedBias = Rule.Diff[handicap].AirspeedBias * Rule.GameSpeedBias;
		ArmorBias = Rule.Diff[handicap].ArmorBias;
		ROFBias = Rule.Diff[handicap].ROFBias;
		CostBias = Rule.Diff[handicap].CostBias;
		RepairDelay = Rule.Diff[handicap].RepairDelay;
		BuildDelay = Rule.Diff[handicap].BuildDelay;
		BuildSpeedBias = Rule.Diff[handicap].BuildSpeedBias * Rule.GameSpeedBias;
	}

	return(old);
}



#ifdef CHEAT_KEYS

void HouseClass::Print_Zone_Stats(int x, int y, ZoneType zone, MonoClass * mono) const
{
	mono->Set_Cursor(x, y);
	mono->Printf("A:%-5d I:%-5d V:%-5d", ZoneInfo[zone].AirDefense, ZoneInfo[zone].InfantryDefense, ZoneInfo[zone].ArmorDefense);
}


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
void HouseClass::Debug_Dump(MonoClass * mono) const
{
	mono->Set_Cursor(0, 0);
	mono->Print(Text_String(TXT_DEBUG_HOUSE));

	mono->Set_Cursor(1, 1);mono->Printf("[%d]%14.14s", Class->House, Name());
	mono->Set_Cursor(20, 1);mono->Printf("[%d]%13.13s", ActLike, HouseTypeClass::As_Reference(ActLike).Name());
	mono->Set_Cursor(39, 1);mono->Printf("%2d", Control.TechLevel);
	mono->Set_Cursor(45, 1);mono->Printf("%2d", Difficulty);
	mono->Set_Cursor(52, 1);mono->Printf("%2d", State);
	mono->Set_Cursor(58, 1);mono->Printf("%2d", Blockage);
	mono->Set_Cursor(65, 1);mono->Printf("%2d", IQ);
	mono->Set_Cursor(72, 1);mono->Printf("%5d", (long)RepairTimer);

	mono->Set_Cursor(1, 3);mono->Printf("%08X", AScan);
	mono->Set_Cursor(10, 3);mono->Printf("%8.8s", (BuildAircraft == AIRCRAFT_NONE) ? " " : AircraftTypeClass::As_Reference(BuildAircraft).Graphic_Name());
	mono->Set_Cursor(21, 3);mono->Printf("%3d", CurAircraft);
	mono->Set_Cursor(27, 3);mono->Printf("%8d", Credits);
	mono->Set_Cursor(37, 3);mono->Printf("%5d", Power);
	mono->Set_Cursor(45, 3);mono->Printf("%04X", RadarSpied);
	mono->Set_Cursor(52, 3);mono->Printf("%5d", PointTotal);
	mono->Set_Cursor(62, 3);mono->Printf("%5d", (long)TeamTime);
	mono->Set_Cursor(71, 3);mono->Printf("%5d", (long)AlertTime);

	mono->Set_Cursor(1, 5);mono->Printf("%08X", BScan);
	mono->Set_Cursor(10, 5);mono->Printf("%8.8s", (BuildStructure == STRUCT_NONE) ? " " : BuildingTypeClass::As_Reference(BuildStructure).Graphic_Name());
	mono->Set_Cursor(21, 5);mono->Printf("%3d", CurBuildings);
	mono->Set_Cursor(27, 5);mono->Printf("%8d", Tiberium);
	mono->Set_Cursor(37, 5);mono->Printf("%5d", Drain);
	mono->Set_Cursor(44, 5);mono->Printf("%16.16s", QuarryName[PreferredTarget]);
	mono->Set_Cursor(62, 5);mono->Printf("%5d", (long)TriggerTime);
	mono->Set_Cursor(71, 5);mono->Printf("%5d", (long)BorrowedTime);

	mono->Set_Cursor(1, 7);mono->Printf("%08X", UScan);
	mono->Set_Cursor(10, 7);mono->Printf("%8.8s", (BuildUnit == UNIT_NONE) ? " " : UnitTypeClass::As_Reference(BuildUnit).Graphic_Name());
	mono->Set_Cursor(21, 7);mono->Printf("%3d", CurUnits);
	mono->Set_Cursor(27, 7);mono->Printf("%8d", Control.InitialCredits);
	mono->Set_Cursor(38, 7);mono->Printf("%5d", UnitsLost);
	mono->Set_Cursor(44, 7);mono->Printf("%08X", Allies);
	mono->Set_Cursor(71, 7);mono->Printf("%5d", (long)Attack);

	mono->Set_Cursor(1, 9);mono->Printf("%08X", IScan);
	mono->Set_Cursor(10, 9);mono->Printf("%8.8s", (BuildInfantry == INFANTRY_NONE) ? " " : InfantryTypeClass::As_Reference(BuildInfantry).Graphic_Name());
	mono->Set_Cursor(21, 9);mono->Printf("%3d", CurInfantry);
	mono->Set_Cursor(27, 9);mono->Printf("%8d", Capacity);
	mono->Set_Cursor(38, 9);mono->Printf("%5d", BuildingsLost);
	mono->Set_Cursor(45, 9);mono->Printf("%4d", Radius / CELL_LEPTON_W);
	mono->Set_Cursor(71, 9);mono->Printf("%5d", (long)AITimer);

	mono->Set_Cursor(1, 11);mono->Printf("%08X", VScan);
	mono->Set_Cursor(10, 11);mono->Printf("%8.8s", (BuildVessel == VESSEL_NONE) ? " " : VesselTypeClass::As_Reference(BuildVessel).Graphic_Name());
	mono->Set_Cursor(21, 11);mono->Printf("%3d", CurVessels);
	mono->Set_Cursor(54, 11);mono->Printf("%04X", Coord_Cell(Center));
	mono->Set_Cursor(71, 11);mono->Printf("%5d", (long)DamageTime);


	for (int index = 0; index < ARRAY_SIZE(Scen.GlobalFlags); index++) {
		mono->Set_Cursor(1+index, 15);
		if (Scen.GlobalFlags[index] != 0) {
			mono->Print("1");
		} else {
			mono->Print("0");
		}
		if (index >= 24) break;
	}
	if (Enemy != HOUSE_NONE) {
		char const * name = "";
		name = HouseClass::As_Pointer(Enemy)->Name();
		mono->Set_Cursor(53, 15);mono->Printf("[%d]%21.21s", Enemy, HouseTypeClass::As_Reference(Enemy).Name());
	}

	Print_Zone_Stats(27, 11, ZONE_NORTH, mono);
	Print_Zone_Stats(27, 13, ZONE_CORE, mono);
	Print_Zone_Stats(27, 15, ZONE_SOUTH, mono);
	Print_Zone_Stats(1, 13, ZONE_WEST, mono);
	Print_Zone_Stats(53, 13, ZONE_EAST, mono);

	mono->Fill_Attrib(1, 17, 12, 1, IsActive ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 18, 12, 1, IsHuman ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 19, 12, 1, IsPlayerControl ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 20, 12, 1, IsAlerted ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 21, 12, 1, IsDiscovered ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 22, 12, 1, IsMaxedOut ? MonoClass::INVERSE : MonoClass::NORMAL);

	mono->Fill_Attrib(14, 17, 12, 1, IsDefeated ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 18, 12, 1, IsToDie ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 19, 12, 1, IsToWin ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 20, 12, 1, IsToLose ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 21, 12, 1, IsCivEvacuated ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 22, 12, 1, IsRecalcNeeded ? MonoClass::INVERSE : MonoClass::NORMAL);

	mono->Fill_Attrib(27, 17, 12, 1, IsVisionary ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 18, 12, 1, IsTiberiumShort ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 19, 12, 1, IsSpied ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 20, 12, 1, IsThieved ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 21, 12, 1, IsGPSActive ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 22, 12, 1, IsStarted ? MonoClass::INVERSE : MonoClass::NORMAL);

	mono->Fill_Attrib(40, 17, 12, 1, IsResigner ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 18, 12, 1, IsGiverUpper ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 19, 12, 1, IsBuiltSomething ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 20, 12, 1, IsBaseBuilding ? MonoClass::INVERSE : MonoClass::NORMAL);
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
void HouseClass::operator delete(void * ptr)
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
#define 	VOX_NOT_READY	VOX_NONE
HouseClass::HouseClass(HousesType house) :
	RTTI(RTTI_HOUSE),
	ID(Houses.ID(this)),
	Class(HouseTypes.Ptr(house)),
	Difficulty(Scen.CDifficulty),
	FirepowerBias(1),
	GroundspeedBias(1),
	AirspeedBias(1),
	ArmorBias(1),
	ROFBias(1),
	CostBias(1),
	BuildSpeedBias(1),
	RepairDelay(0),
	BuildDelay(0),
	ActLike(Class->House),
	IsHuman(false),
	WasHuman(false),
	IsPlayerControl(false),
	IsStarted(false),
	IsAlerted(false),
	IsBaseBuilding(false),
	IsDiscovered(false),
	IsMaxedOut(false),
	IsDefeated(false),
	IsToDie(false),
	IsToLose(false),
	IsToWin(false),
	IsCivEvacuated(false),
	IsRecalcNeeded(true),
	IsVisionary(false),
	IsTiberiumShort(false),
	IsSpied(false),
	IsThieved(false),
	IsGPSActive(false),
	IsBuiltSomething(false),
	IsResigner(false),
	IsGiverUpper(false),
	IsParanoid(false),
	IsToLook(true),
	IsQueuedMovementToggle(false),
	DidRepair(false),
	IQ(Control.IQ),
	State(STATE_BUILDUP),
	JustBuiltStructure(STRUCT_NONE),
	JustBuiltInfantry(INFANTRY_NONE),
	JustBuiltUnit(UNIT_NONE),
	JustBuiltAircraft(AIRCRAFT_NONE),
	JustBuiltVessel(VESSEL_NONE),
	Blockage(0),
	RepairTimer(0),
	AlertTime(0),
	BorrowedTime(0),
	BScan(0),
	ActiveBScan(0),
	OldBScan(0),
	UScan(0),
	ActiveUScan(0),
	OldUScan(0),
	IScan(0),
	ActiveIScan(0),
	OldIScan(0),
	AScan(0),
	ActiveAScan(0),
	OldAScan(0),
	VScan(0),
	ActiveVScan(0),
	OldVScan(0),
	CreditsSpent(0),
	HarvestedCredits(0),
	StolenBuildingsCredits(0),
	CurUnits(0),
	CurBuildings(0),
	CurInfantry(0),
	CurVessels(0),
	CurAircraft(0),
	Tiberium(0),
	Credits(0),
	Capacity(0),
	AircraftTotals(NULL),
	InfantryTotals(NULL),
	UnitTotals(NULL),
	BuildingTotals(NULL),
	VesselTotals(NULL),
	DestroyedAircraft(NULL),
	DestroyedInfantry(NULL),
	DestroyedUnits(NULL),
	DestroyedBuildings(NULL),
	DestroyedVessels(NULL),
	CapturedBuildings(NULL),
	TotalCrates(NULL),
	AircraftFactories(0),
	InfantryFactories(0),
	UnitFactories(0),
	BuildingFactories(0),
	VesselFactories(0),
	Power(0),
	Drain(0),
	AircraftFactory(-1),
	InfantryFactory(-1),
	UnitFactory(-1),
	BuildingFactory(-1),
	VesselFactory(-1),
	Radar(RADAR_NONE),
	FlagLocation(TARGET_NONE),
	FlagHome(0),
	UnitsLost(0),
	BuildingsLost(0),
	WhoLastHurtMe(house),
	StartLocationOverride(-1),
	Center(0),
	Radius(0),
	LATime(0),
	LAType(RTTI_NONE),
	LAZone(ZONE_NONE),
	LAEnemy(HOUSE_NONE),
	ToCapture(TARGET_NONE),
	RadarSpied(0),
	PointTotal(0),
	PreferredTarget(QUARRY_ANYTHING),
	ScreenShakeTime(0),
	Attack(0),
	Enemy(HOUSE_NONE),
	AITimer(0),
	UnitToTeleport(0),
	BuildStructure(STRUCT_NONE),
	BuildUnit(UNIT_NONE),
	BuildInfantry(INFANTRY_NONE),
	BuildAircraft(AIRCRAFT_NONE),
	BuildVessel(VESSEL_NONE),
	NukeDest(0),
	Allies(0),
	DamageTime(TICKS_PER_MINUTE * Rule.DamageDelay),
	TeamTime(TICKS_PER_MINUTE * Rule.TeamDelay),
	TriggerTime(0),
	SpeakAttackDelay(1),
	SpeakPowerDelay(1),
	SpeakMoneyDelay(1),
	SpeakMaxedDelay(1),
	RemapColor(Class->RemapColor),
	DebugUnlockBuildables(false)
{
	/*
	**	Explicit in-place construction of the super weapons is
	**	required here because the default constructor for super
	**	weapons must serve as a no-initialization constructor (save/load reasons).
	*/
	new (&SuperWeapon[SPC_NUCLEAR_BOMB]) SuperClass(TICKS_PER_MINUTE * Rule.NukeTime, true, VOX_ABOMB_PREPPING, VOX_ABOMB_READY, VOX_NOT_READY, VOX_INSUFFICIENT_POWER);
	new (&SuperWeapon[SPC_SONAR_PULSE]) SuperClass(TICKS_PER_MINUTE * Rule.SonarTime, false, VOX_NONE, VOX_SONAR_AVAILABLE, VOX_NOT_READY, VOX_NOT_READY);
	new (&SuperWeapon[SPC_CHRONOSPHERE]) SuperClass(TICKS_PER_MINUTE * Rule.ChronoTime, true, VOX_CHRONO_CHARGING, VOX_CHRONO_READY, VOX_NOT_READY, VOX_INSUFFICIENT_POWER);
	new (&SuperWeapon[SPC_PARA_BOMB]) SuperClass(TICKS_PER_MINUTE * Rule.ParaBombTime, false, VOX_NONE, VOX_NONE, VOX_NOT_READY, VOX_NOT_READY);
	new (&SuperWeapon[SPC_PARA_INFANTRY]) SuperClass(TICKS_PER_MINUTE * Rule.ParaInfantryTime, false, VOX_NONE, VOX_NONE, VOX_NOT_READY, VOX_NOT_READY);
	new (&SuperWeapon[SPC_SPY_MISSION]) SuperClass(TICKS_PER_MINUTE * Rule.SpyTime, false, VOX_NONE, VOX_SPY_PLANE, VOX_NOT_READY, VOX_NOT_READY);
	new (&SuperWeapon[SPC_IRON_CURTAIN]) SuperClass(TICKS_PER_MINUTE * Rule.IronCurtainTime, true, VOX_IRON_CHARGING, VOX_IRON_READY, VOX_NOT_READY, VOX_INSUFFICIENT_POWER);
	new (&SuperWeapon[SPC_GPS]) SuperClass(TICKS_PER_MINUTE * Rule.GPSTime, true, VOX_NONE, VOX_NONE, VOX_NOT_READY, VOX_INSUFFICIENT_POWER);

	memset(UnitsKilled, '\0', sizeof(UnitsKilled));
	memset(BuildingsKilled, '\0', sizeof(BuildingsKilled));
	memset(BQuantity, '\0', sizeof(BQuantity));
	memset(UQuantity, '\0', sizeof(UQuantity));
	memset(IQuantity, '\0', sizeof(IQuantity));
	memset(AQuantity, '\0', sizeof(AQuantity));
	memset(VQuantity, '\0', sizeof(VQuantity));
	strcpy(IniName, Text_String(TXT_COMPUTER));	// Default computer name.
	HouseTriggers[house].Clear();
	memset((void *)&Regions[0], 0x00, sizeof(Regions));
	Make_Ally(house);
	Assign_Handicap(Scen.CDifficulty);

	/*
	**	Set the time of the first AI attack.
	*/
	Attack = Rule.AttackDelay * Random_Pick(TICKS_PER_MINUTE/2, TICKS_PER_MINUTE*2);

	Init_Unit_Trackers();
}


/***********************************************************************************************
 * HouseClass::~HouseClass -- House class destructor                                           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/6/96 4:48PM ST : Created                                                               *
 *=============================================================================================*/
HouseClass::~HouseClass (void)
{
	Class = 0;

	Free_Unit_Trackers();
}


/***********************************************************************************************
 * HouseStaticClass::HouseStaticClass -- Default constructor for house static class.           *
 *                                                                                             *
 *    This is the default constructor that initializes all the values to their default         *
 *    settings.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/31/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
HouseStaticClass::HouseStaticClass(void) :
	IQ(0),
	TechLevel(1),
	Allies(0),
	MaxUnit(Rule.UnitMax/6),
	MaxBuilding(Rule.BuildingMax/6),
	MaxInfantry(Rule.InfantryMax/6),
	MaxVessel(Rule.VesselMax/6),
	MaxAircraft(Rule.UnitMax/6),
	InitialCredits(0),
	Edge(SOURCE_NORTH)
{
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
 *   10/23/1996 JLB : Hack to allow Tanya to both sides in multiplay.                          *
 *   11/04/1996 JLB : Computer uses prerequisite record.                                       *
 *=============================================================================================*/
bool HouseClass::Can_Build(ObjectTypeClass const * type, HousesType house) const
{
	assert(Houses.ID(this) == ID);
	assert(type != NULL);

	/*
	**	An object with a prohibited tech level availability will never be allowed, regardless
	**	of who requests it.
	*/
	if (((TechnoTypeClass const *)type)->Level == -1) return(false);

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	/*
	** If this is a CounterStrike II-only unit, and we're playing a multiplayer
	** game in 'downshifted' mode against CounterStrike or Red Alert, then
	** don't allow building this unit.
	*/
	if (!NewUnitsEnabled) {
		switch(type->What_Am_I()) {
			case RTTI_INFANTRYTYPE:
				if ( ((InfantryTypeClass *)type)->ID >= INFANTRY_RA_COUNT)
					return(false);
				break;
			case RTTI_UNITTYPE:
				if ( ((UnitTypeClass *)type)->ID >= UNIT_RA_COUNT)
					return(false);
				break;
			case RTTI_VESSELTYPE:
				if ( ((VesselTypeClass *)type)->ID >= VESSEL_RA_COUNT)
					return(false);
				break;
			default:
				break;
		}
	}
#endif

	/*
	**	The computer can always build everything.
	*/
	if (!IsHuman && Session.Type == GAME_NORMAL) return(true);

	/*
	**	Special hack to get certain objects to exist for both sides in the game.
	*/
	int own = type->Get_Ownable();

	/*
	**	Check to see if this owner can build the object type specified.
	*/
	if (((1L << house) & own) == 0) {
		return(false);
	}

	/*
	**	Perform some equivalency fixups for the building existence flags.
	*/
	long flags = ActiveBScan;

	/*
	**	The computer records prerequisite buildings because it can't relay on the
	**	sidebar to keep track of this information.
	*/
	if (!IsHuman) {
		flags = OldBScan;
	}

	int pre = ((TechnoTypeClass const *)type)->Prerequisite;

	/*
	**	Advanced power also serves as a prerequisite for normal power.
	*/
	if (flags & STRUCTF_ADVANCED_POWER) flags |= STRUCTF_POWER;

	/*
	**	Either tech center counts as a prerequisite.
	*/
	if (Session.Type != GAME_NORMAL) {
		if ((flags & (STRUCTF_SOVIET_TECH|STRUCTF_ADVANCED_TECH)) != 0) flags |= STRUCTF_SOVIET_TECH|STRUCTF_ADVANCED_TECH;
	}

	int level = Control.TechLevel;
#ifdef CHEAT_KEYS
	if (Debug_Cheat) {
		level = 98;
		pre = 0;
	}
#endif

	// ST - 8/23/2019 4:53PM
	if (DebugUnlockBuildables) {
		level = 98;
		pre = 0;
	}			

	/*
	**	See if the prerequisite requirements have been met.
	*/
	return((pre & flags) == pre && ((TechnoTypeClass const *)type)->Level <= (unsigned)level);
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

// Object selection list is switched with player context for GlyphX. ST - 8/7/2019 10:11AM
extern void Logic_Switch_Player_Context(HouseClass *house);
extern bool MPSuperWeaponDisable;

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

void HouseClass::AI(void)
{
	assert(Houses.ID(this) == ID);

	// Set PlayerPtr to be this house. ST - 8/7/2019 10:12AM
	Logic_Switch_Player_Context(this);

	/*
	**	If base building has been turned on by a trigger, then force the house to begin
	**	production and team creation as well. This is also true if the IQ is high enough to
	**	being base building.
	*/
	if (IsBaseBuilding || IQ >= Rule.IQProduction) {
		IsBaseBuilding = true;
		IsStarted = true;
		IsAlerted = true;
	}

	/*
	**	Check to see if the house wins.
	*/
	if (Session.Type == GAME_NORMAL && IsToWin && BorrowedTime == 0 && Blockage <= 0) {
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
	if (Session.Type == GAME_NORMAL && IsToLose && BorrowedTime == 0) {
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
	if (IsToDie && BorrowedTime == 0) {
		IsToDie = false;
		Blowup_All();
	}

	/*
	**	Double check power values to correct illegal conditions. It is possible to
	**	get a power output of negative (one usually) as a result of damage sustained
	**	and the fixed point fractional math involved with power adjustments. If the
	**	power rating drops below zero, then make it zero.
	*/
	Power = max(Power, 0);
	Drain = max(Drain, 0);

	/*
	**	If the base has been alerted to the enemy and should be attacking, then
	**	see if the attack timer has expired. If it has, then create the attack
	**	teams.
	*/
	if (IsAlerted && AlertTime == 0) {

		/*
		**	Adjusted to reduce maximum number of teams created.
		*/
		int maxteams = Random_Pick(2, (int)(((Control.TechLevel-1)/3)+1));
		for (int index = 0; index < maxteams; index++) {
			TeamTypeClass const * ttype = Suggested_New_Team(true);
			if (ttype != NULL) {
				ScenarioInit++;
				ttype->Create_One_Of();
				ScenarioInit--;
			}
		}
		AlertTime = Rule.AutocreateTime * Random_Pick(TICKS_PER_MINUTE/2, TICKS_PER_MINUTE*2);
//		int mintime = Rule.AutocreateTime * (TICKS_PER_MINUTE/2);
//		int maxtime = Rule.AutocreateTime * (TICKS_PER_MINUTE*2);
//		AlertTime = Random_Pick(mintime, maxtime);
	}

	/*
	**	If this house's flag waypoint is a valid cell, see if there's
	**	someone sitting on it.  If so, make the scatter.
	*/
	if (FlagHome != 0 && (Frame % TICKS_PER_SECOND) == 0) {

		TechnoClass * techno = Map[FlagHome].Cell_Techno();
		if (techno != NULL) {
			bool moving = false;
			if (techno->Is_Foot()) {
				if (Target_Legal(((FootClass *)techno)->NavCom)) {
					moving = true;
				}
			}

			if (!moving) {
				techno->Scatter(0, true, true);
			}
		}
	}

	/*
	**	Create teams for this house if necessary.
	** (Use the same timer for some extra capture-the-flag logic.)
	*/
	if (!IsAlerted && !TeamTime) {

		TeamTypeClass const * ttype = Suggested_New_Team(false);
		if (ttype) {
			ttype->Create_One_Of();
		}

		TeamTime = Rule.TeamDelay * TICKS_PER_MINUTE;
	}

	/*
	**	If there is insufficient power, then all buildings that are above
	**	half strength take a little bit of damage.
	*/
	if (DamageTime == 0) {

		/*
		**	When the power is below required, then the buildings will take damage over
		**	time.
		*/
		if (Power_Fraction() < 1) {
			for (int index = 0; index < Buildings.Count(); index++) {
				BuildingClass & b = *Buildings.Ptr(index);

				if (b.House == this && b.Health_Ratio() > Rule.ConditionYellow) {
					// BG: Only damage buildings that require power, to keep the
					//     land mines from blowing up under low-power conditions
					if (b.Class->Drain) {
						int damage = 1;
						b.Take_Damage(damage, 0, WARHEAD_AP, 0);
					}
				}
			}
		}
		DamageTime = TICKS_PER_MINUTE * Rule.DamageDelay;
	}

	/*
	**	If there are no more buildings to sell, then automatically cancel the
	**	sell mode.
	*/
	if (PlayerPtr == this && !ActiveBScan && Map.IsSellMode) {
		Map.Sell_Mode_Control(0);
	}

	/*
	**	Various base conditions may be announced to the player. Typically, this would be
	**	low tiberium capacity or low power.
	*/
	if (PlayerPtr == this) {

		if (SpeakMaxedDelay == 0 && Available_Money() < 100 && UnitFactories+BuildingFactories+InfantryFactories > 0) {
			Speak(VOX_NEED_MO_MONEY);
			Map.Flash_Money();
			SpeakMaxedDelay = Options.Normalize_Delay(TICKS_PER_MINUTE * Rule.SpeakDelay);

			int text_id = TXT_INSUFFICIENT_FUNDS;
			char const * text = Text_String(TXT_INSUFFICIENT_FUNDS);
			if (text != NULL) {
				Session.Messages.Add_Message(NULL, text_id, text, PCOLOR_GREEN, TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE);
			}
		}

		if (SpeakMaxedDelay == 0 && IsMaxedOut) {
			IsMaxedOut = false;
			if ((Capacity - Tiberium) < 300 && Capacity > 500 && (ActiveBScan & (STRUCTF_REFINERY | STRUCTF_CONST))) {
 				Speak(VOX_NEED_MO_CAPACITY);
				SpeakMaxedDelay = Options.Normalize_Delay(TICKS_PER_MINUTE * Rule.SpeakDelay);
			}
		}
		if (SpeakPowerDelay == 0 && Power_Fraction() < 1) {
			if (ActiveBScan & STRUCTF_CONST) {
				Speak(VOX_LOW_POWER);
				SpeakPowerDelay = Options.Normalize_Delay(TICKS_PER_MINUTE * Rule.SpeakDelay);
				Map.Flash_Power();

				int text_id = -1;
				char const * text = NULL;
				if (BQuantity[STRUCT_AAGUN] > 0) {
					text = Text_String(TXT_POWER_AAGUN);
					text_id = TXT_POWER_AAGUN;
				}
				if (BQuantity[STRUCT_TESLA] > 0) {
					text = Text_String(TXT_POWER_TESLA);
					text_id = TXT_POWER_TESLA;
				}
				if (text == NULL) {
					text = Text_String(TXT_LOW_POWER);
					text_id = TXT_LOW_POWER;
				}
				if (text != NULL) {
					Session.Messages.Add_Message(NULL, text_id, text, PCOLOR_GREEN, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE);
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
	if (TriggerTime == 0 || IsBuiltSomething) {
		is_time = true;
		TriggerTime = TICKS_PER_MINUTE/10;
		IsBuiltSomething = false;
	}

	/*
	**	Process any super weapon logic required.
	*/

	if (Session.Type != GAME_GLYPHX_MULTIPLAYER || !MPSuperWeaponDisable) {
		Super_Weapon_Handler();
	}

#ifdef FIXIT_VERSION_3			//	For endgame auto-sonar pulse.
	if( Scen.AutoSonarTimer == 0 )
	{
		//	If house has nothing but subs left, do an automatic sonar pulse to reveal them.
		if( VQuantity[ VESSEL_SS ] > 0 )		//	Includes count of VESSEL_MISSILESUBs. ajw
		{
			int iCount = 0;
			int i;
			for( i = 0; i != STRUCT_COUNT-3; ++i )
			{
				iCount += BQuantity[ i ];
			}
			if( !iCount )
			{
				for( i = 0; i != UNIT_RA_COUNT-3; ++i )
				{
					iCount += UQuantity[ i ];
				}
				if( !iCount )
				{
					//	ajw - Found bug - house's civilians are not removed from IQuantity when they die.
					//	Workaround...
					for( i = 0; i <= INFANTRY_DOG; ++i )
					{
						iCount += IQuantity[ i ];
					}
					if( !iCount )
					{
						for( i = 0; i != AIRCRAFT_COUNT; ++i )
						{
							iCount += AQuantity[ i ];
						}
						if( !iCount )
						{
							for( i = 0; i != VESSEL_RA_COUNT; ++i )
							{
								if( i != VESSEL_SS )
									iCount += VQuantity[ i ];
							}
							if( !iCount )
							{
								//	Do the ping.
								for (int index = 0; index < Vessels.Count(); index++) {
									VesselClass * sub = Vessels.Ptr(index);
									if (*sub == VESSEL_SS || *sub == VESSEL_MISSILESUB) {
										sub->PulseCountDown = 15 * TICKS_PER_SECOND;
										sub->Do_Uncloak();
									}
								}
								bAutoSonarPulse = true;
							}
						}
					}
				}
			}
		}
	}
#endif

	if (Session.Type != GAME_NORMAL) {
		Check_Pertinent_Structures();
	}

	/*
	** Special win/lose check for multiplayer games; by-passes the
	** trigger system.  We must wait for non-zero frame, because init
	** may not properly set IScan etc for each house; you have to go
	** through each object's AI before it will be properly set.
	*/
	if (Session.Type != GAME_NORMAL && !IsDefeated &&
		!ActiveBScan && !ActiveAScan && !UScan && !ActiveIScan && !ActiveVScan && Frame > 0) {
		MPlayer_Defeated();
	}

	/*
	**	Try to spring all events attached to this house. The triggers will check
	**	for themselves if they actually need to be sprung or not.
	*/
	for (int index = 0; index < HouseTriggers[Class->House].Count(); index++) {
		if (HouseTriggers[Class->House][index]->Spring() && index > 0) {
			index--;
			continue;
		}
	}

	/*
	**	If a radar facility is not present, but the radar is active, then turn the radar off.
	**	The radar also is turned off when the power gets below 100% capacity.
	*/
	if (PlayerPtr == this) {
		bool jammed = true;

		/*
		** Find if there are any radar facilities, and if they're jammed or not
		*/

		if (IsGPSActive) {
			jammed = false;
		} else {
			for (int index = 0; index < Buildings.Count(); index++) {
				BuildingClass * building = Buildings.Ptr(index);
#ifdef FIXIT_RADAR_JAMMED
				if (building != NULL && !building->IsInLimbo && building->House == PlayerPtr) {
#else
				if (building && building->House == PlayerPtr) {
#endif
					if (*building == STRUCT_RADAR /* || *building == STRUCT_EYE */) {
						if (!building->IsJammed) {
							jammed = false;
							break;
						}
					}
				}
			}
		}

		Map.Set_Jammed(this, jammed);
// Need to add in here where we activate it when only GPS is active.
		if (Map.Is_Radar_Active()) {
			if (ActiveBScan & STRUCTF_RADAR) {
				if (Power_Fraction() < 1 && !IsGPSActive) {
					Map.Radar_Activate(0);
				}
			} else {
				if (!IsGPSActive) {
					Map.Radar_Activate(0);
				}
			}

		} else {
			if (IsGPSActive || (ActiveBScan & STRUCTF_RADAR)) {
				if (Power_Fraction() >= 1 || IsGPSActive) {
					Map.Radar_Activate(1);
				}
			} else {
				if (Map.Is_Radar_Existing()) {
					Map.Radar_Activate(4);
				}
			}
		}
		if (!IsGPSActive && !(ActiveBScan & STRUCTF_RADAR)) {
			Radar = RADAR_NONE;
		} else {
			Radar = (Map.Is_Radar_Active() || Map.Is_Radar_Activating()) ? RADAR_ON : RADAR_OFF;
		}
	}

	VisibleCredits.AI(false, this, true);

	/*
	**	Perform any expert system AI processing.
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


	/*
	**	If the production possibilities need to be recalculated, then do so now. This must
	**	occur after the scan bits have been properly updated.
	*/
	if (PlayerPtr == this && IsRecalcNeeded) {
		IsRecalcNeeded = false;
		Map.Recalc();

		/*
		**	This placement might affect any prerequisite requirements for construction
		**	lists. Update the buildable options accordingly.
		*/
		for (int index = 0; index < Buildings.Count(); index++) {
			BuildingClass * building = Buildings.Ptr(index);
			if (building && building->Strength > 0 && building->Owner() == Class->House && building->Mission != MISSION_DECONSTRUCTION && building->MissionQueue != MISSION_DECONSTRUCTION) {

				if (PlayerPtr == building->House) {
					building->Update_Buildables();
				}
			}
		}

		Recalculate_Placement_Distances();
		Check_Pertinent_Structures();
	}

	/*
	** See if it's time to re-set the can-repair flag
	*/
	if (DidRepair && RepairTimer == 0) {
		DidRepair = false;
	}

	if (this == PlayerPtr && IsToLook) {
		IsToLook = false;
		Map.All_To_Look(PlayerPtr);
	}
}

/***********************************************************************************************
 * HouseClass::Super_Weapon_Handler -- Handles the super weapon charge and discharge logic.    *
 *                                                                                             *
 *    This handles any super weapons assigned to this house. It also performs any necessary    *
 *    maintenance that the super weapons require.                                              *
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
void HouseClass::Super_Weapon_Handler(void)
{
	/*
	**	Perform all super weapon AI processing. This just checks to see if
	**	the graphic needs changing for the special weapon and updates the
	**	sidebar as necessary.
	*/
	for (SpecialWeaponType special = SPC_FIRST; special < SPC_COUNT; special++) {
		SuperClass * super = &SuperWeapon[special];

		if (super->Is_Present()) {

			/*
			**	Perform any charge-up logic for the super weapon. If the super
			**	weapon is owned by the player and a graphic change is detected, then
			**	flag the sidebar to be redrawn so the player will see the change.
			*/
			if (super->AI(this == PlayerPtr)) {
				if (this == PlayerPtr) Map.Column[1].Flag_To_Redraw();
			}

			/*
			**	Repeating super weapons that require power will be suspended if there
			**	is insufficient power available.
			*/
			if (!super->Is_Ready() && super->Is_Powered() && !super->Is_One_Time()) {
				super->Suspend(Power_Fraction() < 1);
			}
		}
	}

	/*
	** Check to see if they have launched the GPS, but subsequently lost their
	** tech center.  If so, remove the GPS, and shroud the map.
	*/
	if (IsGPSActive && !(ActiveBScan & STRUCTF_ADVANCED_TECH) ) {
		IsGPSActive = false;

		/*
		** Updated for client/server multiplayer. ST  - 8/12/2019 11:32AM
		*/
		if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {
			if (IsPlayerControl) {
				Map.Shroud_The_Map(PlayerPtr);
			}
		
		} else {
			
			if (IsHuman) {
				Map.Shroud_The_Map(this);
			}
		}	
	}

	/*
	**	Check to see if the GPS Satellite should be removed from the sidebar
	**	because of outside circumstances. The advanced technology facility
	**	being destroyed is a good example of this.  Having fired the satellite
	** is another good example, because it's a one-shot item.
	*/
	if (SuperWeapon[SPC_GPS].Is_Present()) {
		if (!(ActiveBScan & STRUCTF_ADVANCED_TECH) || IsGPSActive || IsDefeated) {
			/*
			**	Remove the missile capability when there is no advanced tech facility.
			*/
			if (SuperWeapon[SPC_GPS].Remove()) {
				if (this == PlayerPtr) Map.Column[1].Flag_To_Redraw();
				IsRecalcNeeded = true;
			}
		} else {
			/*
			** Auto-fire the GPS satellite if it's charged up.
			*/
			if (SuperWeapon[SPC_GPS].Is_Ready()) {
				SuperWeapon[SPC_GPS].Discharged(this == PlayerPtr);
				if (SuperWeapon[SPC_GPS].Remove()) {
					if (this == PlayerPtr) Map.Column[1].Flag_To_Redraw();
				}
				IsRecalcNeeded = true;
				for (int index = 0; index < Buildings.Count(); index++) {
					BuildingClass * bldg = Buildings.Ptr(index);
					if (*bldg == STRUCT_ADVANCED_TECH && bldg->House == this) {
						bldg->HasFired = true;
						bldg->Assign_Mission(MISSION_MISSILE);
						break;
					}
				}
			}
		}
	} else {
		/*
		**	If there is no GPS satellite present, but there is a GPS satellite
		**	facility available, then make the GPS satellite available as well.
		*/
		if ((ActiveBScan & STRUCTF_ADVANCED_TECH) != 0 &&
			!IsGPSActive &&
			Control.TechLevel >= Rule.GPSTechLevel &&
			(IsHuman || IQ >= Rule.IQSuperWeapons)) {

			bool canfire = false;
			for (int index = 0; index < Buildings.Count(); index++) {
				BuildingClass * bldg = Buildings.Ptr(index);
				if (*bldg == STRUCT_ADVANCED_TECH && bldg->House == this && !bldg->IsInLimbo) {
					if (!bldg->HasFired) {
						canfire = true;
						break;
					}
				}
			}

			if (canfire) {
				SuperWeapon[SPC_GPS].Enable(false, this == PlayerPtr, Power_Fraction() < 1);

				/*
				**	Flag the sidebar to be redrawn if necessary.
				*/
				// Add to Glyphx multiplayer sidebar. ST - 8/7/2019 10:13AM
				if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
					if (IsHuman) {
						Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_GPS, this);
					}
				} else {
					if (this == PlayerPtr) {
						Map.Add(RTTI_SPECIAL, SPC_GPS);
						Map.Column[1].Flag_To_Redraw();
					}
				}
			}
		}
	}

	/*
	**	Check to see if the chronosphere should be removed from the sidebar
	**	because of outside circumstances. The chronosphere facility
	**	being destroyed is a good example of this.
	*/
	if (SuperWeapon[SPC_CHRONOSPHERE].Is_Present()) {
		if ( (!(ActiveBScan & STRUCTF_CHRONOSPHERE) && !SuperWeapon[SPC_CHRONOSPHERE].Is_One_Time()) || IsDefeated) {

			/*
			**	Remove the chronosphere when there is no chronosphere facility.
			**	Note that this will not remove the one time created chronosphere.
			*/
			if (SuperWeapon[SPC_CHRONOSPHERE].Remove()) {
				if (this == PlayerPtr) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
					if (Map.IsTargettingMode == SPC_CHRONOSPHERE || Map.IsTargettingMode == SPC_CHRONO2) {
						if (Map.IsTargettingMode == SPC_CHRONO2) {
							TechnoClass * tech = (TechnoClass *)::As_Object(UnitToTeleport);
							if (tech && tech->IsActive && tech->What_Am_I() == RTTI_UNIT && *(UnitClass *)tech == UNIT_CHRONOTANK) {
							} else {
								 Map.IsTargettingMode = SPC_NONE;
							}
						} else {
							Map.IsTargettingMode = SPC_NONE;
						}
					}
#else
					if (Map.IsTargettingMode == SPC_CHRONOSPHERE ||
						 Map.IsTargettingMode == SPC_CHRONO2) {
						 Map.IsTargettingMode = SPC_NONE;
					}
#endif
					Map.Column[1].Flag_To_Redraw();
				}
				IsRecalcNeeded = true;
			}
		}
	} else {
		/*
		**	If there is no chronosphere present, but there is a chronosphere
		**	facility available, then make the chronosphere available as well.
		*/
		if ((ActiveBScan & STRUCTF_CHRONOSPHERE) &&
//			(ActLike == HOUSE_GOOD || Session.Type != GAME_NORMAL) &&
			(unsigned)Control.TechLevel >= BuildingTypeClass::As_Reference(STRUCT_CHRONOSPHERE).Level &&
//			Control.TechLevel >= Rule.ChronoTechLevel &&
			(IsHuman || IQ >= Rule.IQSuperWeapons)) {

			SuperWeapon[SPC_CHRONOSPHERE].Enable(false, this == PlayerPtr, Power_Fraction() < 1);

			/*
			**	Flag the sidebar to be redrawn if necessary.
			*/
			// Add to Glyphx multiplayer sidebar. ST - 8/7/2019 10:13AM
			if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
				if (IsHuman) {
					Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_CHRONOSPHERE, this);
				}
			} else {
				if (this == PlayerPtr) {
					Map.Add(RTTI_SPECIAL, SPC_CHRONOSPHERE);
					Map.Column[1].Flag_To_Redraw();
				}
			}
		}
	}

	/*
	**	Check to see if the iron curtain should be removed from the sidebar
	**	because of outside circumstances. The iron curtain facility
	**	being destroyed is a good example of this.
	*/
	if (SuperWeapon[SPC_IRON_CURTAIN].Is_Present()) {
		if ( (!(ActiveBScan & STRUCTF_IRON_CURTAIN) && !SuperWeapon[SPC_IRON_CURTAIN].Is_One_Time())  || IsDefeated) {

			/*
			**	Remove the iron curtain when there is no iron curtain facility.
			**	Note that this will not remove the one time created iron curtain.
			*/
			if (SuperWeapon[SPC_IRON_CURTAIN].Remove()) {
				if (this == PlayerPtr) {
					if (Map.IsTargettingMode == SPC_IRON_CURTAIN) {
						 Map.IsTargettingMode = SPC_NONE;
					}
					Map.Column[1].Flag_To_Redraw();
				}
				IsRecalcNeeded = true;
			}
		}
	} else {
		/*
		**	If there is no iron curtain present, but there is an iron curtain
		**	facility available, then make the iron curtain available as well.
		*/
		if ((ActiveBScan & STRUCTF_IRON_CURTAIN) &&
			(ActLike == HOUSE_USSR || ActLike == HOUSE_UKRAINE || Session.Type != GAME_NORMAL) &&
			(IsHuman || IQ >= Rule.IQSuperWeapons)) {

			SuperWeapon[SPC_IRON_CURTAIN].Enable(false, this == PlayerPtr, Power_Fraction() < 1);

			/*
			**	Flag the sidebar to be redrawn if necessary.
			*/
			// Add to Glyphx multiplayer sidebar. ST - 8/7/2019 10:13AM
			if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
				if (IsHuman) {
					Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_IRON_CURTAIN, this);
				}
			} else {
				if (this == PlayerPtr) {
					Map.Add(RTTI_SPECIAL, SPC_IRON_CURTAIN);
					Map.Column[1].Flag_To_Redraw();
				}
			}
		}
	}

	/*
	**	Check to see if the sonar pulse should be removed from the sidebar
	**	because of outside circumstances. The spied-upon enemy sub pen
	**	being destroyed is a good example of this.
	*/
	if (SuperWeapon[SPC_SONAR_PULSE].Is_Present()) {
		int usspy = 1 << (Class->House);
		bool present = false;
		bool powered = false;
		for (int q = 0; q < Buildings.Count() && !powered; q++) {
			BuildingClass * bldg = Buildings.Ptr(q);
			if ((*bldg == STRUCT_SUB_PEN) && (bldg->House->Class->House != Class->House) && (bldg->Spied_By() & usspy) ) {
				present = true;
				powered = !(bldg->House->Power_Fraction() < 1);
			}
		}
		if ( (!present && !SuperWeapon[SPC_SONAR_PULSE].Is_One_Time())  || IsDefeated) {

			/*
			**	Remove the sonar pulse when there is no spied-upon enemy sub pen.
			**	Note that this will not remove the one time created sonar pulse.
			*/
			if (SuperWeapon[SPC_SONAR_PULSE].Remove()) {
				if (this == PlayerPtr) Map.Column[1].Flag_To_Redraw();
				IsRecalcNeeded = true;
			}
		}
	}

	/*
	**	Check to see if the nuclear weapon should be removed from the sidebar
	**	because of outside circumstances. The missile silos
	**	being destroyed is a good example of this.
	*/
	if (SuperWeapon[SPC_NUCLEAR_BOMB].Is_Present()) {
		if ( (!(ActiveBScan & STRUCTF_MSLO) && !SuperWeapon[SPC_NUCLEAR_BOMB].Is_One_Time()) || IsDefeated) {

			/*
			**	Remove the nuke when there is no missile silo.
			**	Note that this will not remove the one time created nuke.
			*/
			if (SuperWeapon[SPC_NUCLEAR_BOMB].Remove()) {
				if (this == PlayerPtr) {
					if (Map.IsTargettingMode == SPC_NUCLEAR_BOMB) {
						 Map.IsTargettingMode = SPC_NONE;
					}
					Map.Column[1].Flag_To_Redraw();
				}
				IsRecalcNeeded = true;
			}
		} else {
			/*
			**	Allow the computer to fire the nuclear weapon when the weapon is
			**	ready and the owner is the computer.
			*/
			if (SuperWeapon[SPC_NUCLEAR_BOMB].Is_Ready() && !IsHuman) {
				Special_Weapon_AI(SPC_NUCLEAR_BOMB);
			}
		}

	} else {
		/*
		**	If there is no nuclear missile present, but there is a missile
		**	silo available, then make the missile available as well.
		*/
		if ((ActiveBScan & STRUCTF_MSLO) &&
			((ActLike != HOUSE_USSR && ActLike != HOUSE_UKRAINE) || Session.Type != GAME_NORMAL) &&
			(IsHuman || IQ >= Rule.IQSuperWeapons)) {

			SuperWeapon[SPC_NUCLEAR_BOMB].Enable(false, this == PlayerPtr, Power_Fraction() < 1);

			/*
			**	Flag the sidebar to be redrawn if necessary.
			*/
			// Add to Glyphx multiplayer sidebar. ST - 8/7/2019 10:13AM
			if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
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


	if (SuperWeapon[SPC_SPY_MISSION].Is_Present()) {
		if ((ActiveBScan & STRUCTF_AIRSTRIP) == 0) {
			if (SuperWeapon[SPC_SPY_MISSION].Remove()) {
				if (this == PlayerPtr) Map.Column[1].Flag_To_Redraw();
				IsRecalcNeeded = true;
			}
		} else {
			if (this == PlayerPtr && !SuperWeapon[SPC_SPY_MISSION].Is_Ready()) {
				Map.Column[1].Flag_To_Redraw();
			}
			if (SuperWeapon[SPC_SPY_MISSION].Is_Ready() && !IsHuman) {
				Special_Weapon_AI(SPC_SPY_MISSION);
			}
		}
	} else {
		if ((ActiveBScan & STRUCTF_AIRSTRIP) != 0 && !Scen.IsNoSpyPlane && Control.TechLevel >= Rule.SpyPlaneTechLevel) {
			SuperWeapon[SPC_SPY_MISSION].Enable(false, this == PlayerPtr, false);
			// Add to Glyphx multiplayer sidebar. ST - 8/7/2019 10:13AM
			if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
				if (IsHuman) {
					Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_SPY_MISSION, this);
				}
			} else {
				if (this == PlayerPtr) {
					Map.Add(RTTI_SPECIAL, SPC_SPY_MISSION);
					Map.Column[1].Flag_To_Redraw();
				}
			}
		}
	}

	if (SuperWeapon[SPC_PARA_BOMB].Is_Present()) {
		if ((ActiveBScan & STRUCTF_AIRSTRIP) == 0) {
			if (SuperWeapon[SPC_PARA_BOMB].Remove()) {
				if (this == PlayerPtr) Map.Column[1].Flag_To_Redraw();
				IsRecalcNeeded = true;
			}
		} else {
			if (SuperWeapon[SPC_PARA_BOMB].Is_Ready() && !IsHuman) {
				Special_Weapon_AI(SPC_PARA_BOMB);
			}
		}
	} else {
		if ((ActiveBScan & STRUCTF_AIRSTRIP) != 0 && Control.TechLevel >= Rule.ParaBombTechLevel && Session.Type == GAME_NORMAL) {
			SuperWeapon[SPC_PARA_BOMB].Enable(false, this == PlayerPtr, false);
			// Add to Glyphx multiplayer sidebar. ST - 8/7/2019 10:13AM
			if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
				if (IsHuman) {
					Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_PARA_BOMB, this);
				}
			} else {
				if (this == PlayerPtr) {
					Map.Add(RTTI_SPECIAL, SPC_PARA_BOMB);
					Map.Column[1].Flag_To_Redraw();
				}
			}
		}
	}

	if (SuperWeapon[SPC_PARA_INFANTRY].Is_Present()) {
		if ((ActiveBScan & STRUCTF_AIRSTRIP) == 0) {
			if (SuperWeapon[SPC_PARA_INFANTRY].Remove()) {
				if (this == PlayerPtr) Map.Column[1].Flag_To_Redraw();
				IsRecalcNeeded = true;
			}
		} else {
			if (SuperWeapon[SPC_PARA_INFANTRY].Is_Ready() && !IsHuman) {
				Special_Weapon_AI(SPC_PARA_INFANTRY);
			}
		}
	} else {
		if ((ActiveBScan & STRUCTF_AIRSTRIP) != 0 && Control.TechLevel >= Rule.ParaInfantryTechLevel) {
			SuperWeapon[SPC_PARA_INFANTRY].Enable(false, this == PlayerPtr, false);
			// Add to Glyphx multiplayer sidebar. ST - 8/7/2019 10:13AM
			if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
				if (IsHuman) {
					Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_PARA_INFANTRY, this);
				}
			} else {
				if (this == PlayerPtr) {
					Map.Add(RTTI_SPECIAL, SPC_PARA_INFANTRY);
					Map.Column[1].Flag_To_Redraw();
				}
			}
		}
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
	assert(Houses.ID(this) == ID);

#ifdef FIXIT_BASE_ANNOUNCE
	if (SpeakAttackDelay == 0 && ((Session.Type == GAME_NORMAL && IsPlayerControl) || PlayerPtr->Class->House == Class->House)) {
#else
	if (SpeakAttackDelay == 0 && PlayerPtr->Class->House == Class->House) {
#endif
		Speak(VOX_BASE_UNDER_ATTACK, NULL, source ? source->Center_Coord() : 0);
		SpeakAttackDelay = Options.Normalize_Delay(TICKS_PER_MINUTE * Rule.SpeakDelay);

		/*
		**	If there is a trigger event associated with being attacked, process it
		**	now.
		*/
		for (int index = 0; index < HouseTriggers[Class->House].Count(); index++) {
			HouseTriggers[Class->House][index]->Spring(TEVENT_ATTACKED);
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
	assert(Houses.ID(this) == ID);

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
 * HouseClass::Stole -- Accounts for the value of a captured building.								  *
 *                                                                                             *
 *    Use this routine whenever a building is captured.  It keeps track of the cost of the     *
 *    building for use in the scoring routine, because you get an 'economy' boost for the      *
 *    value of the stolen building (but you don't get the credit value for it.)                *
 *                                                                                             *
 * INPUT:   worth -- The worth of the building we captured (stole).            					  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/05/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Stole(unsigned worth)
{
	assert(Houses.ID(this) == ID);

	StolenBuildingsCredits += worth;
}


/***********************************************************************************************
 * HouseClass::Available_Money -- Fetches the total credit worth of the house.                 *
 *                                                                                             *
 *    Use this routine to determine the total credit value of the house. This is the sum of    *
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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

	long oldtib = Tiberium;
	if (money > (unsigned)Tiberium) {
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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

	long oldcap = Capacity;
	int retval = 0;

	Capacity += adjust;
	Capacity = max(Capacity, 0L);
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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
 *   10/17/1995 JLB : Added reveal base when allied.                                           *
 *=============================================================================================*/
void HouseClass::Make_Ally(HousesType house)
{
	assert(Houses.ID(this) == ID);

	if (Is_Allowed_To_Ally(house)) {

		Allies |= (1L << house);

		/*
		**	Don't consider the newfound ally to be an enemy -- of course.
		*/
		if (Enemy == house) {
			Enemy = HOUSE_NONE;
		}

		if (ScenarioInit) {
			Control.Allies |= (1L << house);
		}

		if (Session.Type != GAME_NORMAL && !ScenarioInit) {
			HouseClass * hptr = HouseClass::As_Pointer(house);

			/*
			**	An alliance with another human player will cause the computer
			**	players (if present) to become paranoid.
			*/
			if (hptr != NULL && IsHuman && Rule.IsComputerParanoid) {
//			if (hptr != NULL && hptr->IsHuman) {
//				if (!hptr->IsHuman) {
//					hptr->Make_Ally(Class->House);
//				}
				Computer_Paranoid();
			}

			char buffer[80];

			/*
			**	Sweep through all techno objects and perform a cheeseball tarcom clear to ensure
			**	that fighting will most likely stop when the cease fire begins.
			*/
			for (int index = 0; index < Logic.Count(); index++) {
				ObjectClass * object = Logic[index];

				if (object != NULL && object->Is_Techno() && !object->IsInLimbo && object->Owner() == Class->House) {
					TARGET target = ((TechnoClass *)object)->TarCom;
					if (Target_Legal(target) && As_Techno(target) != NULL) {
						if (Is_Ally(As_Techno(target))) {
							((TechnoClass *)object)->Assign_Target(TARGET_NONE);
						}
					}
				}
			}

			/*
			**	Cause all structures to be revealed to the house that has been
			**	allied with.
			*/
			if (Rule.IsAllyReveal && house == PlayerPtr->Class->House) {
				for (int index = 0; index < Buildings.Count(); index++) {
					BuildingClass const * b = Buildings.Ptr(index);

					if (b && !b->IsInLimbo && (HouseClass *)b->House == this) {
						Map.Sight_From(Coord_Cell(b->Center_Coord()), b->Class->SightRange, PlayerPtr, false);
					}
				}
			}

			if (IsHuman) {
				sprintf(buffer, Text_String(TXT_HAS_ALLIED), IniName, HouseClass::As_Pointer(house)->IniName);
//				sprintf(buffer, Text_String(TXT_HAS_ALLIED), Session.Players[Class->House - HOUSE_MULTI1]->Name, Session.Players[((HouseClass::As_Pointer(house))->Class->House) - HOUSE_MULTI1]->Name);
				Session.Messages.Add_Message(NULL, 0, buffer, RemapColor, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, TICKS_PER_MINUTE * Rule.MessageDelay);
			}

#if(TEN)
			//
			// Notify the TEN server of the new alliance
			//
			if (this == PlayerPtr && hptr != NULL && Session.Type == GAME_TEN) {
				Send_TEN_Alliance(hptr->IniName, 1);
			}
#endif	// TEN

#if(MPATH)
			//
			// Notify the MPATH server of the new alliance
			//
			//if (this == PlayerPtr && hptr != NULL && Session.Type == GAME_MPATH) {
				//Send_MPATH_Alliance(hptr->IniName, 1);
			//}
#endif	// MPATH

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
 *   07/27/1995 JLB : Making war is a bilateral action.                                        *
 *=============================================================================================*/
void HouseClass::Make_Enemy(HousesType house)
{
	assert(Houses.ID(this) == ID);

	if (house != HOUSE_NONE && Is_Ally(house)) {
		HouseClass * enemy = HouseClass::As_Pointer(house);
		Allies &= ~(1L << house);

		if (ScenarioInit) {
			Control.Allies &= !(1L << house);
		}

		/*
		**	Breaking an alliance is a bilateral event.
		*/
		if (enemy != NULL && enemy->Is_Ally(this)) {
			enemy->Allies &= ~(1L << Class->House);

			if (ScenarioInit) {
				Control.Allies &= ~(1L << Class->House);
			}
		}

		if ((Debug_Flag || Session.Type != GAME_NORMAL) && !ScenarioInit && IsHuman) {
			char buffer[80];

			sprintf(buffer, Text_String(TXT_AT_WAR), IniName, HouseClass::As_Pointer(house)->IniName);
//			sprintf(buffer, Text_String(TXT_AT_WAR), Session.Players[Class->House - HOUSE_MULTI1]->Name, Session.Players[enemy->Class->House - HOUSE_MULTI1]->Name);
			Session.Messages.Add_Message(NULL, 0, buffer, RemapColor, TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, TICKS_PER_MINUTE * Rule.MessageDelay);
			Map.Flag_To_Redraw(false);

#if(TEN)
			//
			// Notify the TEN server of the broken alliance
			//
			if (this == PlayerPtr && enemy != NULL && Session.Type == GAME_TEN) {
				Send_TEN_Alliance(enemy->IniName, 0);
			}
#endif	// TEN

#if(MPATH)
			//
			// Notify the MPATH server of the broken alliance
			//
			//if (this == PlayerPtr && enemy != NULL && Session.Type == GAME_MPATH) {
				//Send_MPATH_Alliance(enemy->IniName, 0);
			//}
#endif	// MPATH

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
 *          remap    -- The remap control value to use.                                        *
 *                      REMAP_NONE     No remap pointer returned at all.                       *
 *                      REMAP_NORMAL   Return the remap pointer for this house.                *
 *                      REMAP_ALTERNATE   (Nod solo play only -- forces red remap).            *
 *                                        Multiplay returns same as REMAP_NORMAL               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the remap table to use when drawing this object.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *   10/25/1995 JLB : Uses remap control value.                                                *
 *=============================================================================================*/
unsigned char const * HouseClass::Remap_Table(bool blushing, RemapType remap) const
{
	assert(Houses.ID(this) == ID);

	if (blushing) return(&Map.FadingLight[0]);

	if (remap == REMAP_NONE) return(0);

	return(ColorRemaps[RemapColor].RemapTable);
}


/***********************************************************************************************
 * HouseClass::Suggested_New_Team -- Determine what team should be created.                    *
 *                                                                                             *
 *    This routine examines the house condition and returns with the team that it thinks       *
 *    should be created. The units that are not currently a member of a team are examined      *
 *    to determine the team needed.                                                            *
 *                                                                                             *
 * INPUT:   alertcheck  -- Select from the auto-create team list.                              *
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
	assert(Houses.ID(this) == ID);

	return(TeamTypeClass::Suggested_New_Team(this, AScan, UScan, IScan, VScan, alertcheck));
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
	assert(Houses.ID(this) == ID);

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
	int * val = &_val[0];
	int * thr = &_thr[0];

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
 *   10/21/1996 JLB : Handles max object case.                                                 *
 *=============================================================================================*/
ProdFailType HouseClass::Begin_Production(RTTIType type, int id)
{
	assert(Houses.ID(this) == ID);
	int result = true;
	bool initial_start = false;
	FactoryClass * fptr;
	TechnoTypeClass const * tech = Fetch_Techno_Type(type, id);

	fptr = Fetch_Factory(type);

	/*
	**	If the house is already busy producing the requested object, then
	**	return with this failure code, unless we are restarting production.
	*/
	if (fptr != NULL) {
		if (fptr->Is_Building()) {
			return(PROD_CANT);
		}
	} else {
		fptr = new FactoryClass();
		if (!fptr) return(PROD_CANT);
		Set_Factory(type, fptr);
		result = fptr->Set(*tech, *this);
		initial_start = true;

		/*
		** If set failed, we probably reached the production cap. Don't let the factory linger, preventing further production attempts.
		** ST - 3/17/2020 2:03PM
		*/
		if (!result) {
			Set_Factory(type, NULL);
			delete fptr;
			fptr = NULL;
		}
	}

	if (result) {
		fptr->Start();

		/*
		**	Link this factory to the sidebar so that proper graphic feedback
		**	can take place.
		*/
		// Handle Glyphx multiplayer sidebar. ST - 8/14/2019 1:26PM
		if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
			if (IsHuman) {
				Sidebar_Glyphx_Factory_Link(fptr->ID, type, id, this);
			}
		} else {		 
			if (PlayerPtr == this) {
				Map.Factory_Link(fptr->ID, type, id);
			}
		}

		return(PROD_OK);
	}

	delete fptr;
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
	assert(Houses.ID(this) == ID);

	FactoryClass * fptr = Fetch_Factory(type);

	/*
	**	If the house is already busy producing the requested object, then
	**	return with this failure code.
	*/
	if (fptr == NULL) return(PROD_CANT);

	/*
	**	Actually suspend the production.
	*/
	fptr->Suspend();

	/*
	**	Tell the sidebar that it needs to be redrawn because of this.
	*/
	if (PlayerPtr == this) {
		Map.SidebarClass::IsToRedraw = true;
		if (!RunningAsDLL) {		// Don't force a redraw when running under GlyphX. PlayerPtr==this will always be true in this case, and we don't want to force a redraw even for AI players
			Map.Flag_To_Redraw(false);
		}
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
	assert(Houses.ID(this) == ID);

	FactoryClass * fptr = Fetch_Factory(type);

	/*
	**	If there is no factory to abandon, then return with a failure code.
	*/
	if (fptr == NULL) return(PROD_CANT);

	/*
	**	Tell the sidebar that it needs to be redrawn because of this.
	*/
	// Handle Glyphx multiplayer sidebar. ST - 8/7/2019 10:18AM
	if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
		if (IsHuman) {
			Sidebar_Glyphx_Abandon_Production(type, fptr->ID, this);
			
			// Need to clear pending object here if legacy renderer enabled

			if (type == RTTI_BUILDINGTYPE || type == RTTI_BUILDING && Map.PendingObjectPtr) {
				Map.PendingObjectPtr = 0;
				Map.PendingObject = 0;
				Map.PendingHouse = HOUSE_NONE;
				Map.Set_Cursor_Shape(0);
			}
		}
	} else {
		if (PlayerPtr == this) {
			Map.Abandon_Production(type, fptr->ID);

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
	Set_Factory(type, NULL);
	delete fptr;

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
	assert(Houses.ID(this) == ID);

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
		if (b != NULL && !b->IsInLimbo && b->Strength && !Is_Ally(b)) {
			if (Percent_Chance(90) && (b->Value() > best || best == -1)) {
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
extern void Logic_Switch_Player_Context(ObjectClass *object);
extern void Logic_Switch_Player_Context(HouseClass *object);
extern void On_Special_Weapon_Targetting(const HouseClass* player_ptr, SpecialWeaponType weapon_type);

bool HouseClass::Place_Special_Blast(SpecialWeaponType id, CELL cell)
{
	assert(Houses.ID(this) == ID);

	// Added. ST - 12/2/2019 11:26AM
	bool fired = false;
	const char *what = NULL;

	BuildingClass * launchsite = 0;
	AnimClass * anim = 0;
	switch (id) {
		case SPC_SONAR_PULSE:
			// Automatically discharge the sonar pulse and uncloak all subs.
			if (SuperWeapon[SPC_SONAR_PULSE].Is_Ready()) {
				SuperWeapon[SPC_SONAR_PULSE].Discharged(this == PlayerPtr);
				if (this == PlayerPtr) {
					Map.Column[1].Flag_To_Redraw();
					Map.Activate_Pulse();
				}
				Sound_Effect(VOC_SONAR);
				IsRecalcNeeded = true;
				fired = true;
				what = "SONAR";
				for (int index = 0; index < Vessels.Count(); index++) {
					VesselClass * sub = Vessels.Ptr(index);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
					if (*sub == VESSEL_SS || *sub == VESSEL_MISSILESUB) {
#else
					if (*sub == VESSEL_SS) {
#endif
						sub->PulseCountDown = 15 * TICKS_PER_SECOND;
						sub->Do_Uncloak();
					}
				}
			}
			break;

		case SPC_NUCLEAR_BOMB:
			if (SuperWeapon[SPC_NUCLEAR_BOMB].Is_Ready()) {
				if (SuperWeapon[SPC_NUCLEAR_BOMB].Is_One_Time()) {
					BulletClass * bullet = new BulletClass(BULLET_NUKE_DOWN, ::As_Target(cell), 0, 200, WARHEAD_NUKE, MPH_VERY_FAST);
					if (bullet) {
						int celly = Cell_Y(cell);
						celly -= 15;
						if (celly < 1) celly = 1;
						COORDINATE start = Cell_Coord(XY_Cell(Cell_X(cell), celly));
						if (!bullet->Unlimbo(start, DIR_S)) {
							delete bullet;
						}
						SuperWeapon[SPC_NUCLEAR_BOMB].Discharged(this == PlayerPtr);
						IsRecalcNeeded = true;
						fired = true;
						what = "NUKE";
						if (this == PlayerPtr) {
							Map.Column[1].Flag_To_Redraw();
							Map.IsTargettingMode = SPC_NONE;
						}
					}
				} else {

					/*
					**	Search for a suitable launch site for this missile.
					*/
					launchsite = Find_Building(STRUCT_MSLO);

					/*
					**	If a launch site was found, then proceed with the normal launch
					**	sequence.
					*/
					if (launchsite) {
						launchsite->Assign_Mission(MISSION_MISSILE);
						launchsite->Commence();
						NukeDest = cell;
					}
					if (this == PlayerPtr) {
						Map.IsTargettingMode = SPC_NONE;
					}
					SuperWeapon[SPC_NUCLEAR_BOMB].Discharged(this == PlayerPtr);
					IsRecalcNeeded = true;
					fired = true;
					what = "NUKE";
				}
			}
			break;

		case SPC_PARA_INFANTRY:
			if (SuperWeapon[SPC_PARA_INFANTRY].Is_Ready()) {

				TeamTypeClass * ttype = TeamTypeClass::As_Pointer("@PINF");
				if (ttype == NULL) {
					ttype = new TeamTypeClass;
					if (ttype != NULL) {
						strcpy(ttype->IniName, "@PINF");
						ttype->IsTransient = true;
						ttype->IsPrebuilt = false;
						ttype->IsReinforcable = false;
						ttype->Origin = WAYPT_SPECIAL;
						ttype->MissionCount = 1;
						ttype->MissionList[0].Mission = TMISSION_ATT_WAYPT;
						ttype->MissionList[0].Data.Value = WAYPT_SPECIAL;
						ttype->ClassCount = 2;
						ttype->Members[0].Quantity = AircraftTypeClass::As_Reference(AIRCRAFT_BADGER).Max_Passengers();
						ttype->Members[0].Class = &InfantryTypeClass::As_Reference(INFANTRY_E1);
						ttype->Members[1].Quantity = 1;
						ttype->Members[1].Class = &AircraftTypeClass::As_Reference(AIRCRAFT_BADGER);
					}
				}

				if (ttype != NULL) {
					ttype->House = Class->House;
					Scen.Waypoint[WAYPT_SPECIAL] = Map.Nearby_Location(cell, SPEED_FOOT);
					Do_Reinforcements(ttype);
				}

//				Create_Air_Reinforcement(this, AIRCRAFT_BADGER, 1, MISSION_HUNT, ::As_Target(cell), TARGET_NONE, INFANTRY_E1);
				if (this == PlayerPtr) {
					Map.IsTargettingMode = SPC_NONE;
				}
				SuperWeapon[SPC_PARA_INFANTRY].Discharged(this == PlayerPtr);
				IsRecalcNeeded = true;
				fired = true;
				what = "PARA";
			}
			break;

		case SPC_SPY_MISSION:
			if (SuperWeapon[SPC_SPY_MISSION].Is_Ready()) {
				Create_Air_Reinforcement(this, AIRCRAFT_U2, 1, MISSION_HUNT, ::As_Target(cell), ::As_Target(cell));
				if (this == PlayerPtr) {
					Map.IsTargettingMode = SPC_NONE;
				}
				SuperWeapon[SPC_SPY_MISSION].Discharged(this == PlayerPtr);
				IsRecalcNeeded = true;
				fired = true;
				what = "SPY";
			}
			break;

		case SPC_PARA_BOMB:
			if (SuperWeapon[SPC_PARA_BOMB].Is_Ready()) {
				Create_Air_Reinforcement(this, AIRCRAFT_BADGER, Rule.BadgerBombCount, MISSION_HUNT, ::As_Target(cell), TARGET_NONE);
				if (this == PlayerPtr) {
					Map.IsTargettingMode = SPC_NONE;
				}
				SuperWeapon[SPC_PARA_BOMB].Discharged(this == PlayerPtr);
				IsRecalcNeeded = true;
				fired = true;
				what = "PARABOMB";
			}
			break;

		case SPC_IRON_CURTAIN:
			if (SuperWeapon[SPC_IRON_CURTAIN].Is_Ready()) {
				int x = Keyboard->MouseQX - Map.TacPixelX;
				int y = Keyboard->MouseQY - Map.TacPixelY;
				TechnoClass * tech = Map[cell].Cell_Techno(x, y);
				if (tech) {
					switch (tech->What_Am_I()) {
						case RTTI_UNIT:
						case RTTI_BUILDING:
						case RTTI_VESSEL:
						case RTTI_AIRCRAFT:
							tech->IronCurtainCountDown = Rule.IronCurtainDuration * TICKS_PER_MINUTE;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
							if (tech->What_Am_I() == RTTI_UNIT && *(UnitClass *)tech == UNIT_DEMOTRUCK) {
								tech->IronCurtainCountDown = Rule.IronCurtainDuration * TICKS_PER_SECOND;
							}
#endif
							tech->Mark(MARK_CHANGE);
							Sound_Effect(VOC_IRON1, tech->Center_Coord());
							if (this == PlayerPtr) {
								Map.IsTargettingMode = SPC_NONE;
							}
							SuperWeapon[SPC_IRON_CURTAIN].Discharged(this == PlayerPtr);
							break;
						default:
							break;
					}
				}

				IsRecalcNeeded = true;
				fired = true;
				what = "IRON";
			}
			break;

		case SPC_CHRONOSPHERE:
			if (SuperWeapon[SPC_CHRONOSPHERE].Is_Ready()) {
				int x = Keyboard->MouseQX - Map.TacPixelX;
				int y = Keyboard->MouseQY - Map.TacPixelY;
				TechnoClass * tech = Map[cell].Cell_Techno(x, y);
				if (tech && Is_Ally(tech)) {
					if (tech->What_Am_I() == RTTI_UNIT ||
						tech->What_Am_I() == RTTI_INFANTRY ||
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
						(tech->What_Am_I() == RTTI_VESSEL && (*((VesselClass *)tech) != VESSEL_TRANSPORT && *((VesselClass *)tech) != VESSEL_CARRIER) )) {
#else
						(tech->What_Am_I() == RTTI_VESSEL && *((VesselClass *)tech) != VESSEL_TRANSPORT)) {
#endif

						if (tech->What_Am_I() != RTTI_UNIT || !((UnitClass *)tech)->IsDeploying) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	bool porthim = true;
	if(tech->What_Am_I() == RTTI_UNIT && ((UnitClass *)tech)->Class->Type == UNIT_CHRONOTANK) {
		porthim = false;
	}
	if (porthim) {
#endif
							HouseClass* old_player_ptr = PlayerPtr;
							Logic_Switch_Player_Context(this);
							Map.IsTargettingMode = SPC_CHRONO2;
							On_Special_Weapon_Targetting(PlayerPtr, Map.IsTargettingMode);
							Logic_Switch_Player_Context(old_player_ptr);
							UnitToTeleport = tech->As_Target();
							fired = true;
							what = "CHRONO";
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	}
#endif
						}
					}
				}
			}
			break;

		case SPC_CHRONO2:
			{
				TechnoClass * tech = (TechnoClass *)::As_Object(UnitToTeleport);
				CELL oldcell = cell;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				if (tech != NULL && tech->IsActive && tech->Is_Foot() && tech->What_Am_I() != RTTI_AIRCRAFT) {
#else
				if (tech != NULL && tech->Is_Foot() && tech->What_Am_I() != RTTI_AIRCRAFT) {
#endif
					/*
					** Destroy any infantryman that gets teleported
					*/
					if (tech->What_Am_I() == RTTI_INFANTRY) {
						InfantryClass * inf = (InfantryClass *)tech;
						inf->Mark(MARK_UP);
						inf->Coord = Cell_Coord(cell);
						inf->Mark(MARK_DOWN);
						int damage = inf->Strength;
						inf->Take_Damage(damage, 0, WARHEAD_FIRE, 0, true);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
					} else if(tech->What_Am_I() == RTTI_UNIT && *(UnitClass *)tech == UNIT_DEMOTRUCK) {
						tech->Assign_Target(tech->As_Target());
#endif
					} else {
						/*
						**	Warp the unit to the new location.
						*/
						DriveClass * drive = (DriveClass *)tech;
						drive->MoebiusCell = Coord_Cell(drive->Coord);
						oldcell = drive->MoebiusCell;
						drive->Teleport_To(cell);
						drive->IsMoebius = true;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if(tech->What_Am_I() == RTTI_UNIT && *(UnitClass *)tech == UNIT_CHRONOTANK) {
		drive->IsMoebius = false;
	}
	drive->MoebiusCountDown = Rule.ChronoDuration * TICKS_PER_MINUTE;
	if (tech->What_Am_I() == RTTI_UNIT && *(UnitClass *)tech == UNIT_CHRONOTANK) {
		drive->MoebiusCountDown = ChronoTankDuration * TICKS_PER_MINUTE;
	}
#else
						drive->MoebiusCountDown = Rule.ChronoDuration * TICKS_PER_MINUTE;
#endif
						Scen.Do_BW_Fade();
						Sound_Effect(VOC_CHRONO, drive->Coord);

						/*
						**	Set active animation on Chronospheres.
						*/
						for (int index = 0; index < Buildings.Count(); ++index) {
							BuildingClass* building = Buildings.Ptr(index);
							if (building != nullptr && building->IsActive && building->Owner() == Class->House && *building == STRUCT_CHRONOSPHERE) {
								building->Begin_Mode(BSTATE_ACTIVE);
							}
						}
					}
				}
				UnitToTeleport = TARGET_NONE;
				if (this == PlayerPtr) {
					Map.IsTargettingMode = SPC_NONE;
				}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if(tech && tech->IsActive && (tech->What_Am_I() != RTTI_UNIT || *(UnitClass *)tech != UNIT_CHRONOTANK)) {
#endif
				SuperWeapon[SPC_CHRONOSPHERE].Discharged(this == PlayerPtr);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	}
#endif
				IsRecalcNeeded = true;
				fired = true;
				what = "CHRONO2";

				/*
				** Now set a percentage chance that a time quake will occur.
				*/
				if (!TimeQuake) {
					TimeQuake = Percent_Chance(Rule.QuakeChance * 100);
				}

				/*
				** Now set a percentage chance that a chronal vortex will appear. It
				**	might appear where the object teleported to or it might appear
				**	where it teleported from -- random chance.
				*/
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
// Don't allow a vortex if the teleportation was due to a chrono tank.
	if(tech && tech->IsActive && (tech->What_Am_I() != RTTI_UNIT || *(UnitClass *)tech != UNIT_CHRONOTANK))
#endif
				if (!ChronalVortex.Is_Active() && Percent_Chance(Rule.VortexChance * 100)) {
					int x = Random_Pick(0, Map.MapCellWidth-1);
					int y = Random_Pick(0, Map.MapCellHeight-1);
					ChronalVortex.Appear(Cell_Coord(XY_Cell(Map.MapCellX + x, Map.MapCellY + y)));

//					if (Percent_Chance(50)) {
//						ChronalVortex.Appear(Cell_Coord(oldcell));
//					} else {
//						ChronalVortex.Appear(Cell_Coord(cell));
//					}
				}

				break;
			}
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
	assert(Houses.ID(this) == ID);

	TechnoClass * tech = 0;
	FactoryClass * factory = Fetch_Factory(type);

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
			if (pending != NULL) {

#ifdef FIXIT_HELI_LANDING
				/*
				**	Try to find a place for the object to appear from. For helicopters, it has the
				**	option of finding a nearby helipad if no helipads are free.
				*/
				TechnoClass * builder = pending->Who_Can_Build_Me(false, false);
				if (builder == NULL && pending->What_Am_I() == RTTI_AIRCRAFT && !((AircraftClass *)pending)->Class->IsFixedWing) {
					builder = pending->Who_Can_Build_Me(true, false);

				}
#else
				bool intheory = false;
				if (pending->What_Am_I() == RTTI_AIRCRAFT) {

					/*
					** BG hack - helicopters don't need a specific building to
					** emerge from, in fact, they'll land next to a building if
					** need be.
					*/
					if( !((AircraftClass *)pending)->Class->IsFixedWing) {
						intheory = true;
					}
				}
				TechnoClass * builder = pending->Who_Can_Build_Me(intheory, false);
#endif
				TechnoTypeClass const *object_type = pending->Techno_Type_Class();
				if (builder != NULL && builder->Exit_Object(pending)) {

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

					switch (pending->What_Am_I()) {
						case RTTI_UNIT:
							JustBuiltUnit = ((UnitClass*)pending)->Class->Type;
							IsBuiltSomething = true;
							break;

						case RTTI_VESSEL:
							JustBuiltVessel = ((VesselClass*)pending)->Class->Type;
							IsBuiltSomething = true;
							break;

						case RTTI_INFANTRY:
							JustBuiltInfantry = ((InfantryClass*)pending)->Class->Type;
							IsBuiltSomething = true;
							break;

						case RTTI_BUILDING:
							JustBuiltStructure = ((BuildingClass*)pending)->Class->Type;
							IsBuiltSomething = true;
							break;

						case RTTI_AIRCRAFT:
							JustBuiltAircraft = ((AircraftClass*)pending)->Class->Type;
							IsBuiltSomething = true;
							break;
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

					builder->Transmit_Message(RADIO_HELLO, tech);
					if (tech->Unlimbo(Cell_Coord(cell))) {
						factory->Completed();
						Abandon_Production(type);

						if (PlayerPtr == this) {
							Sound_Effect(VOC_PLACE_BUILDING_DOWN);
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
	assert(Houses.ID(this) == ID);

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


/***************************************************************************
 * HouseClass::Clobber_All -- removes all objects for this house				*
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      This routine removes the house itself, so the multiplayer code		*
 *		  must not rely on there being "empty" houses lying around.				*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/16/1995 BRR : Created.                                             *
 *   06/09/1995 JLB : Handles aircraft.                                    *
 *=========================================================================*/
void HouseClass::Clobber_All(void)
{
	assert(Houses.ID(this) == ID);

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
	for (i = 0; i < ::Vessels.Count(); i++) {
		if (::Vessels.Ptr(i)->House == this) {
			delete ::Vessels.Ptr(i);
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
		if (Triggers.Ptr(i)->Class->House == Class->House) {
			delete Triggers.Ptr(i);
			i--;
		}
	}
	for (i = 0; i < TriggerTypes.Count(); i++) {
		if (TriggerTypes.Ptr(i)->House == Class->House) {
			delete TriggerTypes.Ptr(i);
			i--;
		}
	}

	delete this;
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
void HouseClass::Detach(TARGET target, bool )
{
	assert(Houses.ID(this) == ID);

	if (ToCapture == target) {
		ToCapture = TARGET_NONE;
	}

	if (Is_Target_Trigger(target)) {
		HouseTriggers[ID].Delete(As_Trigger(target));
	}
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
	assert(Houses.ID(this) == ID);

	int bflag = 1L << btype;
	for (HousesType index = HOUSE_FIRST; index < HOUSE_COUNT; index++) {
		HouseClass * house = HouseClass::As_Pointer(index);

		if (house && !Is_Ally(house) && (house->ActiveBScan & bflag) != 0) {
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
 *          kennel      -- Is this from a kennel? There are special hacks to ensure that only  *
 *                         dogs can be produced from a kennel.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a techno type for the object type that should be         *
 *          created. If no object should be created, then NULL is returned.                    *
 *                                                                                             *
 * WARNINGS:   This is a time consuming routine. Only call when necessary.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TechnoTypeClass const * HouseClass::Suggest_New_Object(RTTIType objecttype, bool kennel) const
{
	assert(Houses.ID(this) == ID);

	TechnoTypeClass const * techno = NULL;

	switch (objecttype) {
		case RTTI_AIRCRAFT:
		case RTTI_AIRCRAFTTYPE:
			if (BuildAircraft != AIRCRAFT_NONE) {
				return(&AircraftTypeClass::As_Reference(BuildAircraft));
			}
			return(NULL);

		case RTTI_VESSEL:
		case RTTI_VESSELTYPE:
			if (BuildVessel != VESSEL_NONE) {
				return(&VesselTypeClass::As_Reference(BuildVessel));
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
				if (kennel && BuildInfantry != INFANTRY_DOG) return(NULL);
				if (!kennel && BuildInfantry == INFANTRY_DOG) return(NULL);
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
	assert(Houses.ID(this) == ID);

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
			if (FlagHome != 0) {
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
 *   10/08/1996 JLB : Uses map nearby cell scanning handler.                                   *
 *=============================================================================================*/
bool HouseClass::Flag_Attach(CELL cell, bool set_home)
{
	assert(Houses.ID(this) == ID);

	bool rc;
	bool clockwise;

	/*
	**	Randomly decide if we're going to search cells clockwise or counter-
	**	clockwise
	*/
	clockwise = Percent_Chance(50);

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
			newcell = Map.Nearby_Location(cell, SPEED_TRACK, -1, MZONE_NORMAL, true);
			if (newcell != 0) {
				rc = Map[newcell].Flag_Place(Class->House);
			}

#ifdef OBSOLETE
			/*
			**	Loop for increasing distance from the desired cell.
			**	For each distance, randomly pick a starting direction.  Between
			**	this and the clockwise/counterclockwise random value, the flag
			**	should appear to be placed fairly randomly.
			*/
			for (int dist = 1; dist < 32; dist++) {
				FacingType fcounter;
				FacingType rot;

				/*
				**	Clockwise search.
				*/
				if (clockwise) {
					rot = Random_Pick(FACING_N, FACING_NW);
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
					rot = Random_Pick(FACING_N, FACING_NW);
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
#endif
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
				Map[newcell].OverlayData = 0;
				Map[newcell].Recalc_Attributes();
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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

	char txt[80];
	int i,j;
	unsigned char id;
	HouseClass * hptr;
	HouseClass * hptr2;
	int num_alive;
	int num_humans;
	int all_allies;

	/*
	**	Set the defeat flag for this house
	*/
	IsDefeated = true;

	/*
	**	If this is a computer controlled house, then all computer controlled
	**	houses become paranoid.
	*/
	if (IQ == Rule.MaxIQ && !IsHuman && Rule.IsComputerParanoid) {
		Computer_Paranoid();
	}

	/*
	**	Remove this house's flag & flag home cell
	*/
	if (Special.IsCaptureTheFlag) {
		if (FlagLocation) {
			Flag_Remove(FlagLocation, true);
		} else {
			if (FlagHome != 0) {
				Flag_Remove(FlagHome, true);
			}
		}
	}

	/*
	**	If this is me:
	**	- Set MPlayerObiWan, so I can only send messages to all players, and
	**	  not just one (so I can't be obnoxiously omnipotent)
	**	- Reveal the map
	**	- Add my defeat message
	*/
	if (PlayerPtr == this) {
		Session.ObiWan = 1;
		Debug_Unshroud = true;
		HidPage.Clear();
		Map.Flag_To_Redraw(true);

		/*
		**	Pop up a message showing that I was defeated
		*/
		sprintf(txt, Text_String(TXT_PLAYER_DEFEATED), IniName);
		if (Session.Type == GAME_NORMAL) {
			Session.Messages.Add_Message(NULL, 0, txt, Session.ColorIdx,
				TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE);
		}
		Map.Flag_To_Redraw(false);
		if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
			int timeout = Rule.MessageDelay * TICKS_PER_MINUTE;
			On_Defeated_Message(txt, timeout * 60.0f / TICKS_PER_MINUTE);
			Sound_Effect(VOC_INCOMING_MESSAGE);
		}

	} else {

		/*
		**	If it wasn't me, find out who was defeated
		*/
		if (IsHuman) {
			sprintf (txt, Text_String(TXT_PLAYER_DEFEATED), IniName);

			//Session.Messages.Add_Message(NULL, 0, txt, RemapColor,
			//	TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE);
			Map.Flag_To_Redraw(false);
			RedrawOptionsMenu = true;

			int timeout = Rule.MessageDelay * TICKS_PER_MINUTE;
			On_Defeated_Message(txt, timeout * 60.0f / TICKS_PER_MINUTE);
			Sound_Effect(VOC_INCOMING_MESSAGE);
		}
	}

	/*
	**	Find out how many players are left alive.
	*/
	num_alive = 0;
	num_humans = 0;
	for (i = 0; i < Session.MaxPlayers; i++) {
		hptr = HouseClass::As_Pointer((HousesType)(HOUSE_MULTI1 + i));
		if (hptr && !hptr->IsDefeated) {
			if (hptr->IsHuman) {
				num_humans++;
			}
			num_alive++;
		}
	}

	/*
	**	If all the houses left alive are allied with each other, then in reality
	**	there's only one player left:
	*/
	all_allies = 1;
	for (i = 0; i < Session.MaxPlayers; i++) {

		/*
		**	Get a pointer to this house
		*/
		hptr = HouseClass::As_Pointer((HousesType)(HOUSE_MULTI1 + i));
		if (!hptr || hptr->IsDefeated)
			continue;

		/*
		**	Loop through all houses; if there's one left alive that this house
		**	isn't allied with, then all_allies will be false
		*/
		for (j = 0; j < Session.MaxPlayers; j++) {
			hptr2 = HouseClass::As_Pointer((HousesType)(HOUSE_MULTI1 + j));
			if (!hptr2) {
				continue;
			}

			if (!hptr2->IsDefeated && !hptr->Is_Ally(hptr2)) {
				all_allies = 0;
				break;
			}
		}
		if (!all_allies) {
			break;
		}
	}

	/*
	**	If all houses left are allies, set 'num_alive' to 1; game over.
	*/
	if (all_allies) {
		num_alive = 1;
	}

	/*
	**	If there's only one human player left or no humans left, the game is over:
	**	- Determine whether this player wins or loses, based on the state of the
	**	  player's IsDefeated flag
	**	- Find all players' indices in the Session.Score array
	**	- Tally up scores for this game
	*/
	if (num_alive == 1 || num_humans == 0) {
		if (PlayerPtr->IsDefeated) {
			PlayerLoses = true;
		} else {
			PlayerWins = true;
		}

		/*
		** Add up the scores
		*/
		Tally_Score();

		/*
		**	Destroy all the IPX connections, since we have to go through the rest
		**	of the Main_Loop() before we detect that the game is over, and we'll
		**	end up waiting for frame sync packets from the other machines.
		*/
		if (Session.Type==GAME_IPX || Session.Type == GAME_INTERNET) {
			i = 0;
			while (Ipx.Num_Connections() && (i++ < 1000) ) {
				id = Ipx.Connection_ID(0);
				Ipx.Delete_Connection(id);
			}
			Session.NumPlayers = 0;
		}

#if(TEN)
		//
		// Tell the TEN server who won
		//
		if (Session.Type == GAME_TEN) {
			Send_TEN_Win_Packet();
		}
#endif	// TEN

#if(MPATH)
		//
		// Tell the MPATH server who won
		//
		if (Session.Type == GAME_MPATH) {
			FILE *fp;

			fp = fopen("winner.txt","wt");
			if (fp) {
				for (i = 0; i < Session.Players.Count(); i++) {
					hptr = HouseClass::As_Pointer(Session.Players[i]->Player.ID);
					if (!hptr->IsDefeated) {
						fprintf(fp,"%s\n",hptr->IniName);
					}
				}
				fclose(fp);
			}
		}
#endif	// MPATH

	}

	/*
	**	Be sure our messages get displayed, even if we're about to exit.
	*/
	Map.Render();
}


/***************************************************************************
 * HouseClass::Tally_Score -- Fills in the score system for this round     *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		none.																						*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   11/29/1995 BRR : Created.                                             *
 *=========================================================================*/
void HouseClass::Tally_Score(void)
{
	HousesType house;
	HousesType house2;
	HouseClass * hptr;
	int score_index;
	int i,j,k;
	int max_index;
	int max_count;
	int count;

	/*
	** Loop through all houses, tallying up each player's score
	*/
	for (house = HOUSE_FIRST; house < HOUSE_COUNT; house++) {
		hptr = HouseClass::As_Pointer(house);
		/*
		** Skip this house if it's not human.
		*/
		if (!hptr || !hptr->IsHuman) {
			continue;
		}
		/*
		** Now find out where this player is in the score array
		*/
		score_index = -1;
		for (i = 0; i < Session.NumScores; i++) {
			if (!stricmp(hptr->IniName, Session.Score[i].Name)) {
				score_index = i;
				break;
			}
		}

		/*
		**	If the index is still -1, the name wasn't found; add a new entry.
		*/
		if (score_index == -1) {
			/*
			** Just add this player to the end of the array, if there's room
			*/
			if (Session.NumScores < MAX_MULTI_NAMES) {
				score_index = Session.NumScores;
				Session.NumScores++;
			}
			/*
			** If there's not room, we have to remove somebody.
			**	For each player in the scores array, count the # of '-1' entries
			**	from this game backwards; the one with the most is the one that
			**	hasn't played the longest; replace him with this new guy.
			*/
			else {
				max_index = 0;
				max_count = 0;
				for (j = 0; j < Session.NumScores; j++) {
					count = 0;
					for (k = Session.NumScores - 1; k >= 0; k--) {
						if (Session.Score[j].Kills[k]==-1) {
							count++;
						}
						else {
							break;
						}
					}
					if (count > max_count) {
						max_count = count;
						max_index = j;
					}
				}
				score_index = max_index;
			}

			/*
			**	Initialize this new score entry
			*/
			Session.Score[score_index].Wins = 0;
			strcpy (Session.Score[score_index].Name, hptr->IniName);
			for (j = 0; j < MAX_MULTI_GAMES; j++)
				Session.Score[score_index].Kills[j] = -1;
		}

		/*
		**	Init this player's Kills to 0 (-1 means he didn't play this round;
		**	0 means he played but got no kills).
		*/
		Session.Score[score_index].Kills[Session.CurGame] = 0;

		/*
		**	Init this player's color to his last-used color index
		*/
		Session.Score[score_index].Color = hptr->RemapColor;

		/*
		**	If this house was undefeated, it must have been the winner.
		** (If no human houses are undefeated, the computer won.)
		*/
		if (!hptr->IsDefeated) {
			Session.Score[score_index].Wins++;
			Session.Winner = score_index;
		}

		/*
		**	Tally up all kills for this player
		*/
		for (house2 = HOUSE_FIRST; house2 < HOUSE_COUNT; house2++) {
			Session.Score[score_index].Kills[Session.CurGame] += hptr->UnitsKilled[house2];
			Session.Score[score_index].Kills[Session.CurGame] += hptr->BuildingsKilled[house2];
		}
	}
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
 *   05/07/1996 JLB : Handles ships.                                       *
 *=========================================================================*/
void HouseClass::Blowup_All(void)
{
	assert(Houses.ID(this) == ID);

	int i;
	int damage;
	UnitClass * uptr;
	InfantryClass * iptr;
	BuildingClass * bptr;
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
				damage = uptr->Strength;
				uptr->Take_Damage(damage, 0, WARHEAD_HE, NULL, true);
				count++;
				if (count > 5 && uptr->IsActive) {
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

			damage = aptr->Strength;
			aptr->Take_Damage(damage, 0, WARHEAD_HE, NULL, true);
			if (!aptr->IsActive) {
				i--;
			}
		}
	}

	/*
	**	Destroy all vessels owned by this house.
	*/
	for (i = 0; i < ::Vessels.Count(); i++) {
		if (::Vessels.Ptr(i)->House == this && !::Vessels.Ptr(i)->IsInLimbo) {
			VesselClass * vptr = ::Vessels.Ptr(i);

			damage = vptr->Strength;
			vptr->Take_Damage(damage, 0, WARHEAD_HE, NULL, true);
			if (!vptr->IsActive) {
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
			while (Buildings.Ptr(i)==bptr && bptr->Strength) {
				damage = bptr->Strength;
				bptr->Take_Damage(damage, 0, WARHEAD_HE, NULL, true);
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
				damage = iptr->Strength;
				warhead = Random_Pick(WARHEAD_SA, WARHEAD_FIRE);
				iptr->Take_Damage(damage, 0, warhead, NULL, true);

				count++;
				if (count > 5) {
					delete iptr;
					break;
				}
			}
		}
	}
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
	assert(Houses.ID(this) == ID);

	if (!IsToWin && !IsToDie && !IsToLose) {
		IsToDie = true;
		BorrowedTime = TICKS_PER_MINUTE * Rule.SavourDelay;
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
	assert(Houses.ID(this) == ID);

	if (!IsToWin && !IsToDie && !IsToLose) {
		IsToWin = true;
		BorrowedTime = TICKS_PER_MINUTE * Rule.SavourDelay;
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
	assert(Houses.ID(this) == ID);

	IsToWin = false;
	if (!IsToDie && !IsToLose) {
		IsToLose = true;
		BorrowedTime = TICKS_PER_MINUTE * Rule.SavourDelay;
	}
	return(IsToLose);
}


/***********************************************************************************************
 * HouseClass::Init_Data -- Initializes the multiplayer color data.                            *
 *                                                                                             *
 *    This routine is called when initializing the color and remap data for this house. The    *
 *    primary user of this routine is the multiplayer version of the game, especially for		  *
 *    saving & loading multiplayer games.																		  *
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
extern bool NowSavingGame; // TEMP MBL: Need to discuss better solution with Steve
void HouseClass::Init_Data(PlayerColorType color, HousesType house, int credits)
{
	assert(Houses.ID(this) == ID);

	Credits = Control.InitialCredits = credits;
	VisibleCredits.Current = Credits;
	RemapColor = color;
	ActLike = house;

	// MBL 03.20.2020 
	// Attempt to fix Red Alert credit tick-up bug after saving a game that has had harvesting underway
	// Note that this code gets called with both game loads and saves
	// When this function is called, sometimes credits value has Tiberium (or HarvestedCredits?) variables applied, and sometimes now
	//
	if (NowSavingGame == true) 
	{
		// At this point VisibleCredits.Current (set above) does not have harvested ore/tiberium applied, but VisibleCredits.Credits does
		VisibleCredits.Current = VisibleCredits.Credits;
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
fixed HouseClass::Power_Fraction(void) const
{
	assert(Houses.ID(this) == ID);

	if (Power >= Drain || Drain == 0) return(1);

	if (Power) {
		return(fixed(Power, Drain));
	}
	return(0);
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
 *   11/02/1996 JLB : Checks unsellable bit for wall type.                                     *
 *=============================================================================================*/
void HouseClass::Sell_Wall(CELL cell)
{
	assert(Houses.ID(this) == ID);

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

					case OVERLAY_FENCE:
						btype = &BuildingTypeClass::As_Reference(STRUCT_FENCE);
						break;

					default:
						break;
				}
				if (btype != NULL && !btype->IsUnsellable) {

					if (PlayerPtr == this) {
						Sound_Effect(VOC_CASHTURN);
					}

					Refund_Money(btype->Raw_Cost() * Rule.RefundPercent);
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
					Map.Radar_Pixel(cell);
					Detach_This_From_All(::As_Target(cell), true);

					if (optr.IsCrushable) {
						Map.Zone_Reset(MZONEF_NORMAL);
					} else {
						Map.Zone_Reset(MZONEF_CRUSHER|MZONEF_NORMAL);
					}
				}
			}
		}
	}
}


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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
	int start = Random_Pick(0, ARRAY_SIZE(_zones)-1);
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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
				maxvessel += h->CurVessels;
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
		if (Control.MaxVessel < (unsigned)maxvessel + 10) {
			Control.MaxVessel = maxvessel + 10;
		}
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
	assert(Houses.ID(this) == ID);

	fixed frac = Power_Fraction();
	UrgencyType urgency = URGENCY_NONE;

	if (frac < 1 && Can_Make_Money()) {
		urgency = URGENCY_LOW;

		/*
		**	Very low power condition is considered a higher priority.
		*/
		if (frac < fixed::_3_4) urgency = URGENCY_MEDIUM;

		/*
		**	When under attack and there is a need for power in defense,
		**	then consider power building a higher priority.
		*/
		if (State == STATE_THREATENED || State == STATE_ATTACKED) {
			if (BScan | (STRUCTF_CHRONOSPHERE)) {
				urgency = URGENCY_HIGH;
			}
		}

	}
	return(urgency);
}


UrgencyType HouseClass::Check_Build_Defense(void) const
{
	assert(Houses.ID(this) == ID);

	/*
	**	This routine determines what urgency level that base defense
	**	should be given. The more vulnerable the base is, the higher
	**	the urgency this routine should return.
	*/
	return(URGENCY_NONE);
}


UrgencyType HouseClass::Check_Build_Offense(void) const
{
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

	/*
	**	If there are no more factories at all, then sell everything off because the game
	**	is basically over at this point.
	*/
	if (State != STATE_ATTACKED && CurBuildings && !(ActiveBScan & (STRUCTF_TENT|STRUCTF_BARRACKS|STRUCTF_CONST|STRUCTF_AIRSTRIP|STRUCTF_WEAP|STRUCTF_HELIPAD))) {
		return(URGENCY_CRITICAL);
	}
	return(URGENCY_NONE);
}


UrgencyType HouseClass::Check_Build_Engineer(void) const
{
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

	return(false);
}


/*
**	Given the specified urgency, build base defensive structures
**	according to need and according to existing base disposition.
*/
bool HouseClass::AI_Build_Defense(UrgencyType ) const
{
	assert(Houses.ID(this) == ID);

	return(false);
}

/*
**	Given the specified urgency, build offensive units according
**	to need and according to the opponents base defenses.
*/
bool HouseClass::AI_Build_Offense(UrgencyType ) const
{
	assert(Houses.ID(this) == ID);

	return(false);
}

/*
**	Given the specified urgency, build income producing
**	structures according to need.
*/
bool HouseClass::AI_Build_Income(UrgencyType ) const
{
	assert(Houses.ID(this) == ID);

	return(false);
}


bool HouseClass::AI_Fire_Sale(UrgencyType urgency)
{
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

	return(false);
}

/*
**	Given the specified urgency, sell of some power since
**	there appears to be excess.
*/
bool HouseClass::AI_Lower_Power(UrgencyType ) const
{
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

	/*
	**	Sell off structures in this order.
	*/
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
	assert(Houses.ID(this) == ID);

	/*
	**	Sell off structures in this order.
	*/
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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

	if (BuildStructure != STRUCT_NONE) return(TICKS_PER_SECOND);

	if (Session.Type == GAME_NORMAL && Base.House == Class->House) {
		BaseNodeClass * node = Base.Next_Buildable();
		if (node) {
			BuildStructure = node->Type;
		}
	}

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
		int level = Control.TechLevel;
		bool hasincome = (BQuantity[STRUCT_REFINERY] > 0 && !IsTiberiumShort && UQuantity[UNIT_HARVESTER] > 0);
		BuildingTypeClass const * b = NULL;
		HouseClass const * enemy = NULL;
		if (Enemy != HOUSE_NONE) {
			enemy = HouseClass::As_Pointer(Enemy);
		}

		level = Control.TechLevel;

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
		current = BQuantity[STRUCT_BARRACKS] + BQuantity[STRUCT_TENT];
		if (current < Round_Up(Rule.BarracksRatio*fixed(CurBuildings)) && current < (unsigned)Rule.BarracksLimit && (money > 300 || hasincome)) {
			b = &BuildingTypeClass::As_Reference(STRUCT_BARRACKS);
			if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					*choiceptr = BuildChoiceClass(current > 0 ? URGENCY_LOW : URGENCY_MEDIUM, b->Type);
				}
			} else {
				b = &BuildingTypeClass::As_Reference(STRUCT_TENT);
				if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
					choiceptr = BuildChoice.Alloc();
					if (choiceptr != NULL) {
						*choiceptr = BuildChoiceClass(current > 0 ? URGENCY_LOW : URGENCY_MEDIUM, b->Type);
					}
				}
			}
		}

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
		current = BQuantity[STRUCT_PILLBOX] + BQuantity[STRUCT_CAMOPILLBOX] + BQuantity[STRUCT_TURRET] + BQuantity[STRUCT_FLAME_TURRET];
		if (current < Round_Up(Rule.DefenseRatio*fixed(CurBuildings)) && current < (unsigned)Rule.DefenseLimit) {
			b = &BuildingTypeClass::As_Reference(STRUCT_FLAME_TURRET);
			if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr != NULL) {
					*choiceptr = BuildChoiceClass(URGENCY_MEDIUM, b->Type);
				}
			} else {
				if (Percent_Chance(50)) {
					b = &BuildingTypeClass::As_Reference(STRUCT_PILLBOX);
					if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
						choiceptr = BuildChoice.Alloc();
						if (choiceptr != NULL) {
							*choiceptr = BuildChoiceClass(URGENCY_MEDIUM, b->Type);
						}
					}
				} else {
					b = &BuildingTypeClass::As_Reference(STRUCT_TURRET);
					if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
						choiceptr = BuildChoice.Alloc();
						if (choiceptr != NULL) {
							*choiceptr = BuildChoiceClass(URGENCY_MEDIUM, b->Type);
						}
					}
				}
			}
		}

		/*
		**	Build some air defense.
		*/
		current = BQuantity[STRUCT_SAM] + BQuantity[STRUCT_AAGUN];
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
					b = &BuildingTypeClass::As_Reference(STRUCT_AAGUN);
					if (Can_Build(b, ActLike) && (b->Cost_Of() < money || hasincome)) {
						choiceptr = BuildChoice.Alloc();
						if (choiceptr != NULL) {
							*choiceptr = BuildChoiceClass((current < (unsigned)threat_quantity) ? URGENCY_HIGH : URGENCY_MEDIUM, b->Type);
						}
					}
				}
			}
		}

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

#ifdef OLD
		/*
		**	Build a repair bay if there isn't one already available.
		*/
		current = BQuantity[STRUCT_REPAIR];
		if (current == 0) {
			b = &BuildingTypeClass::As_Reference(STRUCT_REPAIR);
			if (Can_Build(b, ActLike) && b->Cost_Of() < money) {
				choiceptr = BuildChoice.Alloc();
				if (choiceptr) {
					*choiceptr = BuildChoiceClass(URGENCY_MEDIUM, b->Type);
				}
			}
		}
#endif

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
	assert(Houses.ID(this) == ID);

	if (BuildUnit != UNIT_NONE) return(TICKS_PER_SECOND);
	if (CurUnits >= Control.MaxUnit) return(TICKS_PER_SECOND);

	/*
	**	A computer controlled house will try to build a replacement
	**	harvester if possible.
	*/
	if (IQ >= Rule.IQHarvester && !IsTiberiumShort && !IsHuman && BQuantity[STRUCT_REFINERY] > UQuantity[UNIT_HARVESTER] && Difficulty != DIFF_HARD) {
		if (UnitTypeClass::As_Reference(UNIT_HARVESTER).Level <= (unsigned)Control.TechLevel) {
			BuildUnit = UNIT_HARVESTER;
			return(TICKS_PER_SECOND);
		}
	}

	if (Session.Type == GAME_NORMAL) {

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
				if (((team->IsReinforcable && !tptr->IsFullStrength) || (!tptr->IsForcedActive && !tptr->IsHasBeen && !tptr->JustAltered)) && team->House == Class->House) {
					for (int subindex = 0; subindex < team->ClassCount; subindex++) {
						TechnoTypeClass const * memtype = team->Members[subindex].Class;
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
					TechnoTypeClass const * memtype = team->Members[subindex].Class;

					if (memtype->What_Am_I() == RTTI_UNITTYPE) {
						int subtype = ((UnitTypeClass const *)memtype)->Type;
						counter[subtype] = max(counter[subtype], team->Members[subindex].Quantity);
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
			if (unit != NULL && unit->Is_Recruitable(this) && counter[unit->Class->Type] > 0) {
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
	}

	if (IsBaseBuilding) {

		int counter[UNIT_COUNT];
		int total = 0;
		UnitType index;
		for (index = UNIT_FIRST; index < UNIT_COUNT; index++) {
			UnitTypeClass const * utype = &UnitTypeClass::As_Reference(index);
			if (Can_Build(utype, ActLike) && utype->Type != UNIT_HARVESTER) {
				if (utype->PrimaryWeapon != NULL) {
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
	assert(Houses.ID(this) == ID);
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
				if (Can_Build(&VesselTypeClass::As_Reference(index), Class->House) && VesselTypeClass::As_Reference(index).Level <= (unsigned)Control.TechLevel) {
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

				if (((team->IsReinforcable && !tptr->IsFullStrength) || (!tptr->IsForcedActive && !tptr->IsHasBeen && !tptr->JustAltered)) && team->House == Class->House) {
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
							counter[subtype] = max(counter[subtype], team->Members[subindex].Quantity);
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
			if (unit != NULL && unit->Is_Recruitable(this) && counter[unit->Class->Type] > 0) {
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
	assert(Houses.ID(this) == ID);

	if (BuildInfantry != INFANTRY_NONE) return(TICKS_PER_SECOND);
	if (CurInfantry >= Control.MaxInfantry) return(TICKS_PER_SECOND);

	if (Session.Type == GAME_NORMAL) {
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

				if (((team->IsReinforcable && !tptr->IsFullStrength) || (!tptr->IsForcedActive && !tptr->IsHasBeen && !tptr->JustAltered)) && team->House == Class->House) {
					for (int subindex = 0; subindex < team->ClassCount; subindex++) {
						if (team->Members[subindex].Class->What_Am_I() == RTTI_INFANTRYTYPE) {
							counter[((InfantryTypeClass const *)(team->Members[subindex].Class))->Type] += team->Members[subindex].Quantity + (team->IsReinforcable ? 1 : 0);
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
							counter[subtype] = max(counter[subtype], team->Members[subindex].Quantity);
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
			if (infantry != NULL && infantry->Is_Recruitable(this) && counter[infantry->Class->Type] > 0) {
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
			if (Can_Build(&InfantryTypeClass::As_Reference(index), ActLike) && InfantryTypeClass::As_Reference(index).Level <= (unsigned)Control.TechLevel) {
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

						case INFANTRY_RENOVATOR:
							if (CurInfantry > 5) {
								typetrack[count].Value = 1 - max(IQuantity[index], 0);
							}
							break;

						case INFANTRY_TANYA:
							typetrack[count].Value = 1 - max(IQuantity[index], 0);
							break;

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
	assert(Houses.ID(this) == ID);

	if (!IsHuman && IQ >= Rule.IQAircraft) {
		if (BuildAircraft != AIRCRAFT_NONE) return(TICKS_PER_SECOND);
		if (CurAircraft >= Control.MaxAircraft) return(TICKS_PER_SECOND);

		if (Can_Build(&AircraftTypeClass::As_Reference(AIRCRAFT_LONGBOW), ActLike) &&
				 AircraftTypeClass::As_Reference(AIRCRAFT_LONGBOW).Level <= (unsigned)Control.TechLevel &&
				 BQuantity[STRUCT_HELIPAD] > AQuantity[AIRCRAFT_LONGBOW] + AQuantity[AIRCRAFT_HIND]) {
			BuildAircraft = AIRCRAFT_LONGBOW;
			return(TICKS_PER_SECOND);
		}

		if (Can_Build(&AircraftTypeClass::As_Reference(AIRCRAFT_HIND), ActLike) &&
				 AircraftTypeClass::As_Reference(AIRCRAFT_HIND).Level <= (unsigned)Control.TechLevel &&
				 BQuantity[STRUCT_HELIPAD] > AQuantity[AIRCRAFT_LONGBOW] + AQuantity[AIRCRAFT_HIND]) {
			BuildAircraft = AIRCRAFT_HIND;
			return(TICKS_PER_SECOND);
		}

		if (Can_Build(&AircraftTypeClass::As_Reference(AIRCRAFT_MIG), ActLike) &&
				 AircraftTypeClass::As_Reference(AIRCRAFT_MIG).Level <= (unsigned)Control.TechLevel &&
				 BQuantity[STRUCT_AIRSTRIP] > AQuantity[AIRCRAFT_MIG] + AQuantity[AIRCRAFT_YAK]) {
			BuildAircraft = AIRCRAFT_MIG;
			return(TICKS_PER_SECOND);
		}

		if (Can_Build(&AircraftTypeClass::As_Reference(AIRCRAFT_YAK), ActLike) &&
				 AircraftTypeClass::As_Reference(AIRCRAFT_YAK).Level <= (unsigned)Control.TechLevel &&
				 BQuantity[STRUCT_AIRSTRIP] > AQuantity[AIRCRAFT_MIG] + AQuantity[AIRCRAFT_YAK]) {
			BuildAircraft = AIRCRAFT_YAK;
			return(TICKS_PER_SECOND);
		}
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
	assert(Houses.ID(this) == ID);

	if (product != NULL) {
		switch (product->What_Am_I()) {
			case RTTI_UNIT:
				if (*((UnitClass*)product) == BuildUnit) {
					BuildUnit = UNIT_NONE;
				}
				break;

			case RTTI_VESSEL:
				if (*((VesselClass*)product) == BuildVessel) {
					BuildVessel = VESSEL_NONE;
				}
				break;

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
	assert(Houses.ID(this) == ID);

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

		case RTTI_VESSEL:
			CurVessels--;
			type = ((VesselTypeClass const &)techno->Class_Of()).Type;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			if (type >= VESSEL_RA_COUNT) type -= VESSEL_RA_COUNT;
#endif
			VQuantity[type]--;
			break;

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
	assert(Houses.ID(this) == ID);

	StructType building;
	AircraftType aircraft;
	InfantryType infantry;
	UnitType unit;
	VesselType vessel;
	int quant;

	switch (techno->What_Am_I()) {
		case RTTI_BUILDING:
			CurBuildings++;
			building = ((BuildingTypeClass const &)techno->Class_Of()).Type;
			BQuantity[building]++;
			BScan |= (1L << building);
			if (Session.Type == GAME_INTERNET) {
				BuildingTotals->Increment_Unit_Total(techno->Class_Of().ID);
			}
			break;

		case RTTI_AIRCRAFT:
			CurAircraft++;
			aircraft = ((AircraftTypeClass const &)techno->Class_Of()).Type;
			AQuantity[aircraft]++;
			AScan |= (1L << aircraft);
			if (Session.Type == GAME_INTERNET) {
				AircraftTotals->Increment_Unit_Total(techno->Class_Of().ID);
			}
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
				if (!((InfantryTypeClass const &)techno->Class_Of()).IsCivilian && Session.Type == GAME_INTERNET) {
					InfantryTotals->Increment_Unit_Total(techno->Class_Of().ID);
				}
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
			if (Session.Type == GAME_INTERNET) {
				UnitTotals->Increment_Unit_Total(techno->Class_Of().ID);
			}
			break;

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

		case RTTI_VESSELTYPE:
		case RTTI_VESSEL:
			return(&VesselFactories);

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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);

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
			house->VScan = 0;
			house->ActiveVScan = 0;
		}
	}

	/*
	**	A second pass through the sentient objects is required so that the appropriate scan
	**	bits will be set for the owner house.
	*/
	for (index = 0; index < Units.Count(); index++) {
		UnitClass const * unit = Units.Ptr(index);
		unit->House->UScan |= (1L << unit->Class->Type);
		if (unit->IsLocked && (Session.Type != GAME_NORMAL || !unit->House->IsHuman || unit->IsDiscoveredByPlayer)) {
			if (!unit->IsInLimbo) {
				unit->House->ActiveUScan |= (1L << unit->Class->Type);
			}
		}
	}
	for (index = 0; index < Infantry.Count(); index++) {
		InfantryClass const * infantry = Infantry.Ptr(index);
		infantry->House->IScan |= (1L << infantry->Class->Type);
		if (infantry->IsLocked && (Session.Type != GAME_NORMAL || !infantry->House->IsHuman || infantry->IsDiscoveredByPlayer)) {
			if (!infantry->IsInLimbo) {
				infantry->House->ActiveIScan |= (1L << infantry->Class->Type);
				infantry->House->OldIScan |= (1L << infantry->Class->Type);
			}
		}
	}
	for (index = 0; index < Aircraft.Count(); index++) {
		AircraftClass const * aircraft = Aircraft.Ptr(index);
		aircraft->House->AScan |= (1L << aircraft->Class->Type);
		if (aircraft->IsLocked && (Session.Type != GAME_NORMAL || !aircraft->House->IsHuman || aircraft->IsDiscoveredByPlayer)) {
			if (!aircraft->IsInLimbo) {
				aircraft->House->ActiveAScan |= (1L << aircraft->Class->Type);
				aircraft->House->OldAScan |= (1L << aircraft->Class->Type);
			}
		}
	}
	for (index = 0; index < Buildings.Count(); index++) {
		BuildingClass const * building = Buildings.Ptr(index);
		if (building->Class->Type < 32) {
			building->House->BScan |= (1L << building->Class->Type);
			if (building->IsLocked && (Session.Type != GAME_NORMAL || !building->House->IsHuman || building->IsDiscoveredByPlayer)) {
				if (!building->IsInLimbo) {
					building->House->ActiveBScan |= (1L << building->Class->Type);
					building->House->OldBScan |= (1L << building->Class->Type);
				}
			}
		}
	}
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
	assert(Houses.ID(this) == ID);

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
	assert(Houses.ID(this) == ID);
	assert(object != NULL);

	ZoneType zone;			// The zone that the object should go to.
	if (object->Anti_Air() + object->Anti_Armor() + object->Anti_Infantry() == 0) {
		zone = ZONE_CORE;
	} else {
		zone = Random_Pick(ZONE_NORTH, ZONE_WEST);
	}

	CELL cell = Random_Cell_In_Zone(zone);
	assert(cell != 0);

	return(Map.Nearby_Location(cell, SPEED_TRACK, Map[cell].Zones[MZONE_NORMAL], MZONE_NORMAL));
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
	assert(Houses.ID(this) == ID);

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

		case RTTI_VESSEL:
		case RTTI_VESSELTYPE:
			factory_index = VesselFactory;
			break;

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

	assert(rtti != RTTI_NONE);

	switch (rtti) {
		case RTTI_UNIT:
		case RTTI_UNITTYPE:
			factory_index = &UnitFactory;
			break;

		case RTTI_INFANTRY:
		case RTTI_INFANTRYTYPE:
			factory_index = &InfantryFactory;
			break;

		case RTTI_VESSEL:
		case RTTI_VESSELTYPE:
			factory_index = &VesselFactory;
			break;

		case RTTI_BUILDING:
		case RTTI_BUILDINGTYPE:
			factory_index = &BuildingFactory;
			break;

		case RTTI_AIRCRAFT:
		case RTTI_AIRCRAFTTYPE:
			factory_index = &AircraftFactory;
			break;
	}

	assert(factory_index != NULL);

	/*
	**	Assign the factory to the appropriate slot. For the case of clearing
	**	the factory out, then -1 is assigned.
	*/
	if (factory != NULL) {
		*factory_index = factory->ID;
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

	for (index = 0; index < Vessels.Count(); index++) {
		VesselClass * vessel = Vessels.Ptr(index);

		if (vessel->House == this && vessel->IsDown && !vessel->IsInLimbo) {
			if (vessel->Team) vessel->Team->Remove(vessel);
			vessel->Assign_Mission(MISSION_HUNT);
		}
	}

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
	if (Session.Type == GAME_NORMAL) {
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
	if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {		// Re-enable this for multiplayer if we support classic team/ally mode. ST - 10/29/2019
	
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

	Update_Spied_Power_Plants();
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
	Update_Spied_Power_Plants();
}


/***********************************************************************************************
 * HouseClass::Update_Spied_Power_Plants -- Redraw power graphs on spied-upon power plants.    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/11/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Update_Spied_Power_Plants(void)
{
	int count = CurrentObject.Count();
	if (count) {
		for (int index = 0; index < count; index++) {
			ObjectClass const * tech = CurrentObject[index];
			if (tech && tech->What_Am_I()==RTTI_BUILDING) {
				BuildingClass *bldg = (BuildingClass *)tech;
				if (!bldg->IsOwnedByPlayer && *bldg == STRUCT_POWER || *bldg == STRUCT_ADVANCED_POWER) {
					if ( bldg->Spied_By() & (1<<(PlayerPtr->Class->House)) ) {
						bldg->Mark(MARK_CHANGE);
					}
				}
			}
		}
	}
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
	if (Session.Type == GAME_INTERNET || Session.Type == GAME_GLYPHX_MULTIPLAYER) {
		AircraftTotals = new UnitTrackerClass( (int) AIRCRAFT_COUNT);
		InfantryTotals = new UnitTrackerClass( (int) INFANTRY_COUNT);
		UnitTotals = new UnitTrackerClass ( (int) UNIT_COUNT);
		BuildingTotals = new UnitTrackerClass ( (int) STRUCT_COUNT);
		VesselTotals = new UnitTrackerClass ( (int) VESSEL_COUNT);

		DestroyedAircraft = new UnitTrackerClass ( (int) AIRCRAFT_COUNT);
		DestroyedInfantry = new UnitTrackerClass( (int) INFANTRY_COUNT);
		DestroyedUnits = new UnitTrackerClass ( (int) UNIT_COUNT);
		DestroyedBuildings = new UnitTrackerClass ( (int) STRUCT_COUNT);
		DestroyedVessels = new UnitTrackerClass ( (int) VESSEL_COUNT);

		CapturedBuildings = new UnitTrackerClass ( (int) STRUCT_COUNT);
		TotalCrates = new UnitTrackerClass ( CRATE_COUNT );
	} else {
		
		AircraftTotals = NULL;
		InfantryTotals = NULL;
		UnitTotals = NULL;
		BuildingTotals = NULL;
		VesselTotals = NULL;

		DestroyedAircraft = NULL;
		DestroyedInfantry = NULL;
		DestroyedUnits = NULL;
		DestroyedBuildings = NULL;
		DestroyedVessels = NULL;

		CapturedBuildings = NULL;
		TotalCrates = NULL;
	}	
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

	if (VesselTotals) {
		delete VesselTotals;
		VesselTotals = NULL;
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

	if (DestroyedVessels) {
		delete DestroyedVessels;
		DestroyedVessels = NULL;
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