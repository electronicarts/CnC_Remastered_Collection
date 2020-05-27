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

/* $Header:   F:\projects\c&c\vcs\code\aadata.cpv   2.18   16 Oct 1995 16:49:50   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                              name in                                                *
 *                    File Name : AADATA.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : July 22, 1994                                                *
 *                                                                                             *
 *                  Last Update : August 7, 1995 [JLB]                                         *
 *                               Determines                                                              *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   AircraftTypeClass::AircraftTypeClass -- Constructor for aircraft objects.                 *
 *   AircraftTypeClass::Create_And_Place -- Creates and places aircraft using normal game syste*
 *   AircraftTypeClass::Create_One_Of -- Creates an aircraft object of the appropriate type.   *
 *   AircraftTypeClass::Dimensions -- Fetches the graphic dimensions of the aircraft type.     *
 *   AircraftTypeClass::Display -- Displays a generic version of the aircraft type.            *
 *   AircraftTypeClass::From_Name -- Converts an ASCIIto an aircraft type number.      *
 *   AircraftTypeClass::Max_Pips -- Fetches the maximum number of pips allowed.                *
 *   AircraftTypeClass::Occupy_List -- Returns with occupation list for landed aircraft.       *
 *   AircraftTypeClass::One_Time -- Performs one time initialization of the aircraft type class.*
 *   AircraftTypeClass::Overlap_List --  the overlap list for a landed aircraft.     *
 *   AircraftTypeClass::Prep_For_Add -- Prepares the scenario editor for adding an aircraft objec*
 *   AircraftTypeClass::Repair_Cost -- Fetchs the cost per repair step.                        *
 *   AircraftTypeClass::Repair_Step -- Fetches the number of health points per repair.         *
 *   AircraftTypeClass::Who_Can_Build_Me -- Determines which object can build the aircraft obje*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


void const * AircraftTypeClass::LRotorData = NULL;
void const * AircraftTypeClass::RRotorData = NULL;

// A-10 attack plane
static AircraftTypeClass const AttackPlane(
	AIRCRAFT_A10,			// What kind of aircraft is this.
	TXT_A10,					// Translated text number for aircraft.
	"A10",					// INI name of aircraft.
	99,						// Build level.
	STRUCTF_NONE,			// Building prerequisite.
		false,				// Is a leader type?
		false,				// Does it fire a pair of shots in quick succession?
		false,				//	Is this a typical transport vehicle?
		true,					// Fixed wing aircraft?
		false,				// Equipped with a rotor?
		false,				// Custom rotor sets for each facing?
		false,				// Can this aircraft land on clear terrain?
		false,				// Can the aircraft be crushed by a tracked vehicle?
		true,					// Is it invisible on radar?
		false,				// Can the player select it so as to give it orders?
		true,					// Can it be assigned as a target for attack.
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it immune to normal combat damage?
		false,				// Theater specific graphic image?
		false,				// Can it be repaired in a repair facility?
		false,				// Can the player construct or order this unit?
		true,					// Is there a crew inside?
	3,							// Number of shots it has (default).
	60,						// The strength of this unit.
	0,							// The range that it reveals terrain around itself.
	800,						// Credit cost to construct.
	0,							// The scenario this becomes available.
	10,1,						// Risk, reward when calculating AI.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD,				// Who can own this aircraft type.
	WEAPON_NAPALM,WEAPON_NONE,
	ARMOR_ALUMINUM,		// Armor type of this aircraft.
	MPH_FAST,				// Maximum speed of aircraft.
	5,							// Rate of turn.
	MISSION_HUNT			// Default mission for aircraft.
);

// Transport helicopter.
static AircraftTypeClass const TransportHeli(
	AIRCRAFT_TRANSPORT,	// What kind of aircraft is this.
	TXT_TRANS,				// Translated text number for aircraft.
	"TRAN",					// INI name of aircraft.
	6,							// Build level.
	STRUCTF_HELIPAD,		// Building prerequisite.
		false,				// Is a leader type?
		false,				// Does it fire a pair of shots in quick succession?
		true,					//	Is this a typical transport vehicle?
		false,				// Fixed wing aircraft?
		true,					// Equipped with a rotor?
		true,					// Custom rotor sets for each facing?
		true,					// Can this aircraft land on clear terrain?
		false,				// Can the aircraft be crushed by a tracked vehicle?
		true,					// Is it invisible on radar?
		true,					// Can the player select it so as to give it orders?
		true,					// Can it be assigned as a target for attack.
		false,				// Is it insignificant (won't be announced)?
		false,				// Theater specific graphic image?
		false,				// Is it equipped with a combat turret?
		false,				// Can it be repaired in a repair facility?
		true,					// Can the player construct or order this unit?
		true,					// Is there a crew inside?
	0,							// Number of shots it has (default).
	90,						// The strength of this unit.
	0,							// The range that it reveals terrain around itself.
	1500,						// Credit cost to construct.
	98,						// The scenario this becomes available.
	10,80,					// Risk, reward when calculating AI.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_BAD|
	HOUSEF_GOOD,			// Who can own this aircraft type.
	WEAPON_NONE,WEAPON_NONE,
	ARMOR_ALUMINUM,		// Armor type of this aircraft.
	MPH_MEDIUM_FAST,		// Maximum speed of aircraft.
	5,							// Rate of turn.
	MISSION_HUNT			// Default mission for aircraft.
);

// Apache attach helicopter.
static AircraftTypeClass const AttackHeli(
	AIRCRAFT_HELICOPTER,	// What kind of aircraft is this.
	TXT_HELI,				// Translated text number for aircraft.
	"HELI",					// INI name of aircraft.
	6,							// Build level.
	STRUCTF_HELIPAD,		// Building prerequisite.
		true,					// Is a leader type?
		true,					// Does it fire a pair of shots in quick succession?
		false,				//	Is this a typical transport vehicle?
		false,				// Fixed wing aircraft?
		true,					// Equipped with a rotor?
		false,				// Custom rotor sets for each facing?
		false,				// Can this aircraft land on clear terrain?
		false,				// Can the aircraft be crushed by a tracked vehicle?
		true,					// Is it invisible on radar?
		true,					// Can the player select it so as to give it orders?
		true,					// Can it be assigned as a target for attack.
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it immune to normal combat damage?
		false,				// Theater specific graphic image?
		false,				// Can it be repaired in a repair facility?
		true,					// Can the player construct or order this unit?
		true,					// Is there a crew inside?
	15,						// Number of shots it has (default).
	125,						// The strength of this unit.
	0,							// The range that it reveals terrain around itself.
	1200,						// Credit cost to construct.
	10,						// The scenario this becomes available.
	10,80,					// Risk, reward when calculating AI.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_BAD,				// Who can own this aircraft type.
	WEAPON_CHAIN_GUN,WEAPON_NONE,
	ARMOR_STEEL,			// Armor type of this aircraft.
	MPH_FAST,				// Maximum speed of aircraft.
	4,							// Rate of turn.
	MISSION_HUNT			// Default mission for aircraft.
);


// Orca attack helicopter.
static AircraftTypeClass const OrcaHeli(
	AIRCRAFT_ORCA,			// What kind of aircraft is this.
	TXT_ORCA,				// Translated text number for aircraft.
	"ORCA",					// INI name of aircraft.
	6,							// Build level.
	STRUCTF_HELIPAD,		// Building prerequisite.
		true,					// Is a leader type?
		true,					// Does it fire a pair of shots in quick succession?
		false,				//	Is this a typical transport vehicle?
		false,				// Fixed wing aircraft?
		false,				// Equipped with a rotor?
		false,				// Custom rotor sets for each facing?
		false,				// Can this aircraft land on clear terrain?
		false,				// Can the aircraft be crushed by a tracked vehicle?
		true,					// Is it invisible on radar?
		true,					// Can the player select it so as to give it orders?
		true,					// Can it be assigned as a target for attack.
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it immune to normal combat damage?
		false,				// Theater specific graphic image?
		false,				// Can it be repaired in a repair facility?
		true,					// Can the player construct or order this unit?
		true,					// Is there a crew inside?
	6,							// Number of shots it has (default).
	125,						// The strength of this unit.
	0,							// The range that it reveals terrain around itself.
	1200,						// Credit cost to construct.
	10,						// The scenario this becomes available.
	10,80,					// Risk, reward when calculating AI.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD,			// Who can own this aircraft type.
	WEAPON_DRAGON,WEAPON_NONE,
	ARMOR_STEEL,			// Armor type of this aircraft.
	MPH_FAST,				// Maximum speed of aircraft.
	4,							// Rate of turn.
	MISSION_HUNT			// Default mission for aircraft.
);


// C-17 transport plane.
static AircraftTypeClass const CargoPlane(
	AIRCRAFT_CARGO,		// What kind of aircraft is this.
	TXT_C17,					// Translated text number for aircraft.
	"C17",					// INI name of aircraft.
	99,						// Build level.
	STRUCTF_NONE,			// Building prerequisite.
		false,				// Is a leader type?
		false,				// Does it fire a pair of shots in quick succession?
		true,					//	Is this a typical transport vehicle?
		true,					// Fixed wing aircraft?
		false,				// Equipped with a rotor?
		false,				// Custom rotor sets for each facing?
		false,				// Can this aircraft land on clear terrain?
		false,				// Can the aircraft be crushed by a tracked vehicle?
		true,					// Is it invisible on radar?
		false,				// Can the player select it so as to give it orders?
		false,				// Can it be assigned as a target for attack.
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it immune to normal combat damage?
		false,				// Theater specific graphic image?
		false,				// Can it be repaired in a repair facility?
		false,				// Can the player construct or order this unit?
		true,					// Is there a crew inside?
	0,							// Number of shots it has (default).
	25,						// The strength of this unit.
	0,							// The range that it reveals terrain around itself.
	800,						// Credit cost to construct.
	0,							// The scenario this becomes available.
	10,1,						// Risk, reward when calculating AI.
	HOUSEF_MULTI1|
	HOUSEF_MULTI2|
	HOUSEF_MULTI3|
	HOUSEF_MULTI4|
	HOUSEF_MULTI5|
	HOUSEF_MULTI6|
	HOUSEF_JP|
	HOUSEF_GOOD|
	HOUSEF_BAD,				// Who can own this aircraft type.
	WEAPON_NONE,WEAPON_NONE,
	ARMOR_ALUMINUM,		// Armor type of this aircraft.
	MPH_FAST,				// Maximum speed of aircraft.
	5,							// Rate of turn.
	MISSION_HUNT			// Default mission for aircraft.
);


AircraftTypeClass const * const AircraftTypeClass::Pointers[AIRCRAFT_COUNT] = {
	&TransportHeli,
	&AttackPlane,
	&AttackHeli,
	&CargoPlane,
	&OrcaHeli,
};


/***********************************************************************************************
 * AircraftTypeClass::AircraftTypeClass -- Constructor for aircraft objects.                   *
 *                                                                                             *
 *    This is the constructor for the aircraft object.                                         *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
AircraftTypeClass::AircraftTypeClass(
				AircraftType airtype,
				int name,
				char const *ininame,
				unsigned char level,
				long pre,
				bool is_leader,
				bool is_twoshooter,
				bool is_transporter,
				bool is_fixedwing,
				bool is_rotorequipped,
				bool is_rotorcustom,
				bool is_landable,
				bool is_crushable,
				bool is_stealthy,
				bool is_selectable,
				bool is_legal_target,
				bool is_insignificant,
				bool is_immune,
				bool is_theater,
				bool is_repairable,
				bool is_buildable,
				bool is_crew,
				int ammo,
				unsigned short strength,
				int sightrange,
				int cost,
				int scenario,
				int risk,
				int reward,
				int ownable,
				WeaponType primary,
				WeaponType secondary,
				ArmorType armor,
				MPHType maxspeed,
				int rot,
				MissionType deforder) :
					TechnoTypeClass(name,
										ininame,
										level,
										pre,
										is_leader,
										false,
										false,
										is_transporter,
										false,
										is_crushable,
										is_stealthy,
										is_selectable,
										is_legal_target,
										is_insignificant,
										is_immune,
										is_theater,
										is_twoshooter,
										false,
										is_repairable,
										is_buildable,
										is_crew,
										ammo,
										strength,
										maxspeed,
										sightrange,
										cost,
										scenario,
										risk,
										reward,
										ownable,
										primary,
										secondary,
										armor)
{
	IsRotorEquipped = is_rotorequipped;
	IsRotorCustom = is_rotorcustom;
	IsLandable = is_landable;
	IsFixedWing = is_fixedwing;
	Type = airtype;
	ROT = rot;
	Mission = deforder;
}


/***********************************************************************************************
 * AircraftTypeClass::From_Name -- Converts an ASCII name into an aircraft type number.        *
 *                                                                                             *
 *    This routine is used to convert an ASCII representation of an aircraft into the          *
 *    matching aircraft type number. This is used by the scenario INI reader code.             *
 *                                                                                             *
 * INPUT:   name  -- Pointer to ASCII name to translate.                                       *
 *                                                                                             *
 * OUTPUT:  Returns the aircraft type number that matches the ASCII name provided. If no       *
 *          match could be found, then AIRCRAFT_NONE is returned.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
AircraftType AircraftTypeClass::From_Name(char const *name)
{
	if (name) {
		for (AircraftType classid = AIRCRAFT_FIRST; classid < AIRCRAFT_COUNT; classid++) {
			if (stricmp(Pointers[classid]->IniName, name) == 0) {
				return(classid);
			}
		}
	}
	return(AIRCRAFT_NONE);
}


/***********************************************************************************************
 * AircraftTypeClass::One_Time -- Performs one time initialization of the aircraft type class. *
 *                                                                                             *
 *    This routine is used to perform the onetime initialization of the aircraft type. This    *
 *    includes primarily the shape and other graphic data loading.                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This goes to disk and also must only be called ONCE.                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftTypeClass::One_Time(void)
{
	AircraftType index;

	for (index = AIRCRAFT_FIRST; index < AIRCRAFT_COUNT; index++) {
		char fullname[_MAX_FNAME+_MAX_EXT];
		AircraftTypeClass	const & uclass = As_Reference(index);

		/*
		**	Fetch the supporting data files for the unit.
		*/
		char buffer[_MAX_FNAME];
		if ( Get_Resolution_Factor() ) {
			sprintf(buffer, "%sICNH", uclass.IniName);
		} else {
			sprintf(buffer, "%sICON", uclass.IniName);
		}
		_makepath(fullname, NULL, NULL, buffer, ".SHP");
		((void const *&)uclass.CameoData) = MixFileClass::Retrieve(fullname);

		/*
		**	Generic shape for all houses load method.
		*/
		_makepath(fullname, NULL, NULL, uclass.IniName, ".SHP");
		((void const *&)uclass.ImageData) = MixFileClass::Retrieve(fullname);
	}

	LRotorData = MixFileClass::Retrieve("LROTOR.SHP");
	RRotorData = MixFileClass::Retrieve("RROTOR.SHP");
}


/***********************************************************************************************
 * AircraftTypeClass::Create_One_Of -- Creates an aircraft object of the appropriate type.     *
 *                                                                                             *
 *    This routine is used to create an aircraft object that matches the aircraft type. It     *
 *    serves as a shortcut to creating an object using the "new" operator and "if" checks.     *
 *                                                                                             *
 * INPUT:   house -- The house owner of the aircraft that is to be created.                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the aircraft created. If the aircraft could not be       *
 *          created, then a NULL is returned.                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * AircraftTypeClass::Create_One_Of(HouseClass * house) const
{
	return(new AircraftClass(Type, house->Class->House));
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * AircraftTypeClass::Prep_For_Add -- Prepares the scenario editor for adding an aircraft objec*
 *                                                                                             *
 *    This routine is used by the scenario editor to prepare for the adding operation. It      *
 *    builds a list of pointers to object types that can be added.                             *
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
void AircraftTypeClass::Prep_For_Add(void)
{
	for (AircraftType index = AIRCRAFT_FIRST; index < AIRCRAFT_COUNT; index++) {
		if (As_Reference(index).Get_Image_Data()) {
			Map.Add_To_List(&As_Reference(index));
		}
	}
}


/***********************************************************************************************
 * AircraftTypeClass::Display -- Displays a generic version of the aircraft type.              *
 *                                                                                             *
 *    This routine is used by the scenario editor to display a generic version of the object   *
 *    type. This is displayed in the object selection dialog box.                              *
 *                                                                                             *
 * INPUT:   x,y      -- The coordinates to draw the aircraft at (centered).                    *
 *                                                                                             *
 *          window   -- The window to base the coordinates upon.                               *
 *                                                                                             *
 *          house    -- The owner of this generic aircraft.                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftTypeClass::Display(int x, int y, WindowNumberType window, HousesType house) const
{
	int shape = 0;
	void const * ptr = Get_Cameo_Data();
	if (!ptr) {
		ptr = Get_Image_Data();
		shape = 5;
	}
	CC_Draw_Shape(ptr, shape, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING, HouseClass::As_Pointer(house)->Remap_Table(false, true));
}
#endif


/***********************************************************************************************
 * AircraftTypeClass::Occupy_List -- Returns with occupation list for landed aircraft.         *
 *                                                                                             *
 *    This determines the occupation list for the aircraft (if it was landed).                 *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a cell offset occupation list for the aircraft.          *
 *                                                                                             *
 * WARNINGS:   This occupation list is only valid if the aircraft is landed.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * AircraftTypeClass::Occupy_List(bool) const
{
	static short const _list[] = {0, REFRESH_EOL};
	return(_list);
}


/***********************************************************************************************
 * AircraftTypeClass::Overlap_List -- Determines the overlap list for a landed aircraft.       *
 *                                                                                             *
 *    This routine figures out the overlap list for the aircraft as if it were landed.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the cell offset overlap list for the aircraft.                        *
 *                                                                                             *
 * WARNINGS:   This overlap list is only valid when the aircraft is landed.                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * AircraftTypeClass::Overlap_List(void) const
{
	static short const _list[] = {-(MAP_CELL_W-1), -MAP_CELL_W, -(MAP_CELL_W+1), -1, 1, (MAP_CELL_W-1), MAP_CELL_W, (MAP_CELL_W+1), REFRESH_EOL};
	return(_list);
}


/***********************************************************************************************
 * AircraftTypeClass::Who_Can_Build_Me -- Determines which object can build the aircraft objec *
 *                                                                                             *
 *    Use this routine to determine which object (factory) can build the aircraft. It          *
 *    determines this by scanning through the available factories, looking for one that is     *
 *    of the proper ownership and is available.                                                *
 *                                                                                             *
 * INPUT:   intheory -- When true, it doesn't consider if the factory is currently busy. It    *
 *                      only considers that it is the right type.                              *
 *                                                                                             *
 *          legal    -- Should building prerequisite legality checks be performed as well?     *
 *                      For building placements, this is usually false. For sidebar button     *
 *                      adding, this is usually true.                                          *
 *                                                                                             *
 *          house    -- The house of the desired aircraft to be built.                         *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the object that can build the aircraft.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/30/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingClass * AircraftTypeClass::Who_Can_Build_Me(bool , bool legal, HousesType house) const
{
	BuildingClass * anybuilding = NULL;
	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass * building = Buildings.Ptr(index);

		if (building &&
				!building->IsInLimbo &&
				building->House->Class->House == house &&
				building->Mission != MISSION_DECONSTRUCTION &&
				((1L << building->ActLike) & Ownable) &&
				(!legal || building->House->Can_Build(Type, building->ActLike)) &&
				building->Class->ToBuild == RTTI_AIRCRAFTTYPE) {

			if (building->IsLeader) return(building);
			anybuilding = building;
		}
	}
	return(anybuilding);
}


/***********************************************************************************************
 * AircraftTypeClass::Repair_Cost -- Fetchs the cost per repair step.                          *
 *                                                                                             *
 *    This routine will return the cost for every repair step.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the credit expense for every repair step.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/26/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftTypeClass::Repair_Cost(void) const
{
	return(Fixed_To_Cardinal(Cost/(MaxStrength/REPAIR_STEP), REPAIR_PERCENT));
}


/***********************************************************************************************
 * AircraftTypeClass::Repair_Step -- Fetches the number of health points per repair.           *
 *                                                                                             *
 *    For every repair event, the returned number of health points is acquired.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of health points to recover each repair step.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/26/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftTypeClass::Repair_Step(void) const
{
	return(REPAIR_STEP);
}


/***********************************************************************************************
 * AircraftTypeClass::Max_Pips -- Fetches the maximum number of pips allowed.                  *
 *                                                                                             *
 *    Use this routine to retrieve the maximum pip count allowed for this aircraft. This is    *
 *    the maximum number of passengers.                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the maximum number of pips for this aircraft.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/26/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftTypeClass::Max_Pips(void) const
{
	if (IsTransporter) {
		return(Max_Passengers());
	} else {
		if (Primary != WEAPON_NONE) {
			return(5);
		}
	}
	return(0);
}


/***********************************************************************************************
 * AircraftTypeClass::Create_And_Place -- Creates and places aircraft using normal game system *
 *                                                                                             *
 *    This routine is used to create and place an aircraft through the normal game system.     *
 *    Since creation of aircraft in this fashion is prohibited, this routine does nothing.     *
 *                                                                                             *
 * INPUT:   na                                                                                 *
 *                                                                                             *
 * OUTPUT:  Always returns a failure code (false).                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/07/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftTypeClass::Create_And_Place(CELL, HousesType) const
{
	return(false);
}





/***********************************************************************************************
 * ATC::Init -- load up terrain set dependant sidebar icons                                    *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    theater type                                                                      *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    4/25/96 0:33AM ST : Created                                                              *
 *=============================================================================================*/

void AircraftTypeClass::Init(TheaterType theater)
{
	if (theater != LastTheater){
		if ( Get_Resolution_Factor() ) {

			AircraftType index;
			char buffer[_MAX_FNAME];
			char fullname[_MAX_FNAME+_MAX_EXT];
			void const * cameo_ptr;

			for (index = AIRCRAFT_FIRST; index < AIRCRAFT_COUNT; index++) {
				AircraftTypeClass	const & uclass = As_Reference(index);

				((void const *&)uclass.CameoData) = NULL;

				sprintf(buffer, "%.4sICNH", uclass.IniName);
				_makepath (fullname, NULL, NULL, buffer, Theaters[theater].Suffix);
				cameo_ptr = MixFileClass::Retrieve(fullname);
				if (cameo_ptr){
					((void const *&)uclass.CameoData) = cameo_ptr;
				}
			}
		}
	}
}





/***********************************************************************************************
 * AircraftTypeClass::Dimensions -- Fetches the graphic dimensions of the aircraft type.       *
 *                                                                                             *
 *    This routine will fetch the pixel dimensions of this aircraft type. These dimensions     *
 *    are used to control map refresh and select box rendering.                                *
 *                                                                                             *
 * INPUT:   width    -- Reference to variable that will be filled in with aircraft width.      *
 *                                                                                             *
 *          height   -- Reference to variable that will be filled in with aircraft height.     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/07/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftTypeClass::Dimensions(int &width, int &height) const
{
	width = 21;
	height = 20;
}


RTTIType AircraftTypeClass::What_Am_I(void) const {return RTTI_AIRCRAFTTYPE;};
