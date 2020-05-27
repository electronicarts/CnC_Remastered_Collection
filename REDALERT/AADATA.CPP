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

/* $Header: /CounterStrike/AADATA.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : AADATA.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : July 22, 1994                                                *
 *                                                                                             *
 *                  Last Update : July 9, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   AircraftTypeClass::AircraftTypeClass -- Constructor for aircraft objects.                 *
 *   AircraftTypeClass::As_Reference -- Given an aircraft type, find the matching type object. *
 *   AircraftTypeClass::Create_And_Place -- Creates and places aircraft using normal game syste*
 *   AircraftTypeClass::Create_One_Of -- Creates an aircraft object of the appropriate type.   *
 *   AircraftTypeClass::Dimensions -- Fetches the graphic dimensions of the aircraft type.     *
 *   AircraftTypeClass::Display -- Displays a generic version of the aircraft type.            *
 *   AircraftTypeClass::From_Name -- Converts an ASCII name into an aircraft type number.      *
 *   AircraftTypeClass::Init_Heap -- Initialize the aircraft type class heap.                  *
 *   AircraftTypeClass::Max_Pips -- Fetches the maximum number of pips allowed.                *
 *   AircraftTypeClass::Occupy_List -- Returns with occupation list for landed aircraft.       *
 *   AircraftTypeClass::One_Time -- Performs one time initialization of the aircraft type class*
 *   AircraftTypeClass::Overlap_List -- Determines the overlap list for a landed aircraft.     *
 *   AircraftTypeClass::Prep_For_Add -- Prepares the scenario editor for adding an aircraft obj*
 *   AircraftTypeClass::operator delete -- Returns aircraft type to special memory pool.       *
 *   AircraftTypeClass::operator new -- Allocates an aircraft type object from special pool.   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


void const * AircraftTypeClass::LRotorData = NULL;
void const * AircraftTypeClass::RRotorData = NULL;

// Badger bomber
static AircraftTypeClass const BadgerPlane(
	AIRCRAFT_BADGER,		// What kind of aircraft is this.
	TXT_BADGER,				// Translated text number for aircraft.
	"BADR",					// INI name of aircraft.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
		true,					// Fixed wing aircraft?
		false,				// Equipped with a rotor?
		false,				// Custom rotor sets for each facing?
		false,				// Can this aircraft land on clear terrain?
		true,					// Is it invisible on radar?
		false,				// Can the player select it so as to give it orders?
		true,					// Can it be assigned as a target for attack.
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it immune to normal combat damage?
	STRUCT_NONE,			// Preferred landing building.
	0xFF,						// Landing speed
	16,						// Number of rotation stages.
	MISSION_HUNT			// Default mission for aircraft.
);

// Photo recon plane.
static AircraftTypeClass const U2Plane(
	AIRCRAFT_U2,			// What kind of aircraft is this.
	TXT_U2,					// Translated text number for aircraft.
	"U2",						// INI name of aircraft.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
		true,					// Fixed wing aircraft?
		false,				// Equipped with a rotor?
		false,				// Custom rotor sets for each facing?
		false,				// Can this aircraft land on clear terrain?
		true,					// Is it invisible on radar?
		false,				// Can the player select it so as to give it orders?
		true,					// Can it be assigned as a target for attack.
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it immune to normal combat damage?
	STRUCT_NONE,			// Preferred landing building.
	0xFF,						// Landing speed
	16,						// Number of rotation stages.
	MISSION_HUNT			// Default mission for aircraft.
);

// Mig attack aircraft.
static AircraftTypeClass const MigPlane(
	AIRCRAFT_MIG,			// What kind of aircraft is this.
	TXT_MIG,					// Translated text number for aircraft.
	"MIG",					// INI name of aircraft.
	0x0000,					//	Vertical offset.
	0x0020,					// Primary weapon offset along turret centerline.
	0x0020,					// Primary weapon lateral offset along turret centerline.
		true,					// Fixed wing aircraft?
		false,				// Equipped with a rotor?
		false,				// Custom rotor sets for each facing?
		false,				// Can this aircraft land on clear terrain?
		true,					// Is it invisible on radar?
		true,					// Can the player select it so as to give it orders?
		true,					// Can it be assigned as a target for attack.
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it immune to normal combat damage?
	STRUCT_AIRSTRIP,		// Preferred landing building.
	0xC0,						// Landing speed
	16,						// Number of rotation stages.
	MISSION_HUNT			// Default mission for aircraft.
);

// Yak attack aircraft.
static AircraftTypeClass const YakPlane(
	AIRCRAFT_YAK,			// What kind of aircraft is this.
	TXT_YAK,					// Translated text number for aircraft.
	"YAK",					// INI name of aircraft.
	0x0000,					//	Vertical offset.
	0x0020,					// Primary weapon offset along turret centerline.
	0x0020,					// Primary weapon lateral offset along turret centerline.
		true,					// Fixed wing aircraft?
		false,				// Equipped with a rotor?
		false,				// Custom rotor sets for each facing?
		false,				// Can this aircraft land on clear terrain?
		true,					// Is it invisible on radar?
		true,					// Can the player select it so as to give it orders?
		true,					// Can it be assigned as a target for attack.
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it immune to normal combat damage?
	STRUCT_AIRSTRIP,		// Preferred landing building.
	0xFF,						// Landing speed
	16,						// Number of rotation stages.
	MISSION_HUNT			// Default mission for aircraft.
);

// Transport helicopter.
static AircraftTypeClass const TransportHeli(
	AIRCRAFT_TRANSPORT,	// What kind of aircraft is this.
	TXT_TRANS,				// Translated text number for aircraft.
	"TRAN",					// INI name of aircraft.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
		false,				// Fixed wing aircraft?
		true,					// Equipped with a rotor?
		true,					// Custom rotor sets for each facing?
		true,					// Can this aircraft land on clear terrain?
		true,					// Is it invisible on radar?
		true,					// Can the player select it so as to give it orders?
		true,					// Can it be assigned as a target for attack.
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it immune to normal combat damage?
	STRUCT_NONE,			// Preferred landing building.
	0xFF,						// Landing speed
	32,						// Number of rotation stages.
	MISSION_HUNT			// Default mission for aircraft.
);

// Longbow attack helicopter
static AircraftTypeClass const AttackHeli(
	AIRCRAFT_LONGBOW,		// What kind of aircraft is this.
	TXT_HELI,				// Translated text number for aircraft.
	"HELI",					// INI name of aircraft.
	0x0000,					//	Vertical offset.
	0x0040,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
		false,				// Fixed wing aircraft?
		true,					// Equipped with a rotor?
		false,				// Custom rotor sets for each facing?
		false,				// Can this aircraft land on clear terrain?
		true,					// Is it invisible on radar?
		true,					// Can the player select it so as to give it orders?
		true,					// Can it be assigned as a target for attack.
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it immune to normal combat damage?
	STRUCT_HELIPAD,		// Preferred landing building.
	0xFF,						// Landing speed
	32,						// Number of rotation stages.
	MISSION_HUNT			// Default mission for aircraft.
);


// Hind
static AircraftTypeClass const OrcaHeli(
	AIRCRAFT_HIND,			// What kind of aircraft is this.
	TXT_ORCA,				// Translated text number for aircraft.
	"HIND",					// INI name of aircraft.
	0x0000,					//	Vertical offset.
	0x0040,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
		false,				// Fixed wing aircraft?
		true,					// Equipped with a rotor?
		false,				// Custom rotor sets for each facing?
		false,				// Can this aircraft land on clear terrain?
		true,					// Is it invisible on radar?
		true,					// Can the player select it so as to give it orders?
		true,					// Can it be assigned as a target for attack.
		false,				// Is it insignificant (won't be announced)?
		false,				// Is it immune to normal combat damage?
	STRUCT_HELIPAD,		// Preferred landing building.
	0xFF,						// Landing speed
	32,						// Number of rotation stages.
	MISSION_HUNT			// Default mission for aircraft.
);


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
				char const * ininame,
				int verticaloffset,
				int primaryoffset,
				int primarylateral,
				bool is_fixedwing,
				bool is_rotorequipped,
				bool is_rotorcustom,
				bool is_landable,
				bool is_stealthy,
				bool is_selectable,
				bool is_legal_target,
				bool is_insignificant,
				bool is_immune,
				StructType building,
				int landingspeed,
				int rotation,
				MissionType deforder
				) :
					TechnoTypeClass(RTTI_AIRCRAFTTYPE,
										int(airtype),
										name,
										ininame,
										REMAP_NORMAL,
										verticaloffset,
										primaryoffset,
										primarylateral,
										primaryoffset,
										primarylateral,
										false,
										is_stealthy,
										is_selectable,
										is_legal_target,
										is_insignificant,
										is_immune,
										false,
										false,
										true,
										true,
										rotation,
										SPEED_WINGED),
	IsFixedWing(is_fixedwing),
	IsLandable(is_landable),
	IsRotorEquipped(is_rotorequipped),
	IsRotorCustom(is_rotorcustom),
	Type(airtype),
	Mission(deforder),
	Building(building),
	LandingSpeed(landingspeed)
{
	/*
	**	Forced aircraft overrides from the default.
	*/
	Speed = SPEED_WINGED;
}


/***********************************************************************************************
 * AircraftTypeClass::operator new -- Allocates an aircraft type object from special pool.     *
 *                                                                                             *
 *    This will allocate an aircraft type class object from the memory pool of that purpose.   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the newly allocated aircraft type class object. If there *
 *          was insufficient memory to fulfill the request, then NULL is returned.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * AircraftTypeClass::operator new(size_t)
{
	return(AircraftTypes.Alloc());
}


/***********************************************************************************************
 * AircraftTypeClass::operator delete -- Returns aircraft type to special memory pool.         *
 *                                                                                             *
 *    This will return the aircraft type class object back to the special memory pool that     *
 *    it was allocated from.                                                                   *
 *                                                                                             *
 * INPUT:   pointer  -- Pointer to the aircraft type class object to delete.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftTypeClass::operator delete(void * pointer)
{
	AircraftTypes.Free((AircraftTypeClass *)pointer);
}


/***********************************************************************************************
 * AircraftTypeClass::Init_Heap -- Initialize the aircraft type class heap.                    *
 *                                                                                             *
 *    This will initialize the aircraft type class heap by pre-allocating all known aircraft   *
 *    types. It should be called once and before the rules.ini file is processed.              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftTypeClass::Init_Heap(void)
{
	/*
	**	These aircraft type class objects must be allocated in the exact order that they
	**	are specified in the AircraftSmen enumeration. This is necessary because the heap
	**	allocation block index serves double duty as the type number index.
	*/
	new AircraftTypeClass(TransportHeli);
	new AircraftTypeClass(BadgerPlane);
	new AircraftTypeClass(U2Plane);
	new AircraftTypeClass(MigPlane);
	new AircraftTypeClass(YakPlane);
	new AircraftTypeClass(AttackHeli);
	new AircraftTypeClass(OrcaHeli);
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
AircraftType AircraftTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (int classid = AIRCRAFT_FIRST; classid < AIRCRAFT_COUNT; classid++) {
			if (stricmp(As_Reference((AircraftType)classid).IniName, name) == 0) {
				return(AircraftType)classid;
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
	for (int index = AIRCRAFT_FIRST; index < AIRCRAFT_COUNT; index++) {
		char fullname[_MAX_FNAME+_MAX_EXT];
		AircraftTypeClass	const & uclass = As_Reference((AircraftType)index);

		/*
		**	Fetch the supporting data files for the unit.
		*/
		char buffer[_MAX_FNAME];
		sprintf(buffer, "%sICON", uclass.Graphic_Name());
		_makepath(fullname, NULL, NULL, buffer, ".SHP");
		((void const *&)uclass.CameoData) = MFCD::Retrieve(fullname);

		/*
		**	Generic shape for all houses load method.
		*/
		_makepath(fullname, NULL, NULL, uclass.Graphic_Name(), ".SHP");
		((void const *&)uclass.ImageData) = MFCD::Retrieve(fullname);
	}

	LRotorData = MFCD::Retrieve("LROTOR.SHP");
	RRotorData = MFCD::Retrieve("RROTOR.SHP");
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
void AircraftTypeClass::Display(int x, int y, WindowNumberType window, HousesType ) const
{
	int shape = 0;
	void const * ptr = Get_Cameo_Data();
	if (ptr == NULL) {
		ptr = Get_Image_Data();
		shape = 5;
	}
	CC_Draw_Shape(ptr, shape, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL);
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
	if (PrimaryWeapon != NULL) {
		// Camera weapon (ex. on the Spy plane) doesn't display any pips
		if (!PrimaryWeapon->IsCamera) {
			return(5);
		}
	}
	return(Max_Passengers());
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
	if (Type == AIRCRAFT_BADGER) {
		width = 56;
		height = 56;
	} else {
		width = 21;
		height = 20;
	}
}


/***********************************************************************************************
 * AircraftTypeClass::As_Reference -- Given an aircraft type, find the matching type object.   *
 *                                                                                             *
 *    This routine is used to fetch a reference to the aircraft type class object that matches *
 *    the aircraft type specified.                                                             *
 *                                                                                             *
 * INPUT:   aircraft -- The aircraft type to fetch a reference to the type class object of.    *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the type class object of this aircraft type.           *
 *                                                                                             *
 * WARNINGS:   Be sure that the aircraft type specified is legal. Illegal values will result   *
 *             in undefined behavior.                                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
AircraftTypeClass & AircraftTypeClass::As_Reference(AircraftType aircraft)
{
	return(*AircraftTypes.Ptr(aircraft));
}
