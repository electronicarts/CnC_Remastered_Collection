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

/* $Header: /CounterStrike/VDATA.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : VDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 03/13/96                                                     *
 *                                                                                             *
 *                  Last Update : July 9, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   VesselTypeClass::As_Reference -- Converts a vessel type into a VesselTypeClass reference. *
 *   VesselTypeClass::Create_And_Place -- Creates a vessel and places it at location.          *
 *   VesselTypeClass::Create_One_Of -- Creates a vessel object that matches this vessel type.  *
 *   VesselTypeClass::Dimensions -- Fetches the pixel width and height of this vessel type.    *
 *   VesselTypeClass::Display -- Displays a generic representation of this vessel type.        *
 *   VesselTypeClass::From_Name -- Converts a name into a vessel type.                         *
 *   VesselTypeClass::Init_Heap -- Initialize the vessel heap.                                 *
 *   VesselTypeClass::One_Time -- Performs one time initialization for vessel types.           *
 *   VesselTypeClass::Overlap_List -- Figures the overlap list for the vessel type.            *
 *   VesselTypeClass::Prep_For_Add -- Adds vessel types to the scenario editor object list.    *
 *   VesselTypeClass::Turret_Adjust -- Adjust turret offset according to facing specified.     *
 *   VesselTypeClass::VesselTypeClass -- Constructor for naval vessel types.                   *
 *   VesselTypeClass::Who_Can_Build_Me -- Fetches pointer to available factory for this vessel.*
 *   VesselTypeClass::operator delete -- Returns a vessel type object back to the memory pool. *
 *   VesselTypeClass::operator new -- Allocate a vessel type object from the special memory poo*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "function.h"


// Submarine
static VesselTypeClass const VesselSubmarine(
	VESSEL_SS,
	TXT_SS,					// NAME:			Text name of this unit type.
	"SS",						// NAME:			Text name of this unit type.
	ANIM_FBALL1,			// EXPLOSION:	Type of explosion when destroyed.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Only has eight facings?
		true,					// Always use the given name for the vehicle?
		false,				// Is it equipped with a combat turret?
	8,							// Rotation stages.
	14							// Turret center offset along body centerline.
);

// Destroyer
static VesselTypeClass const VesselDestroyer(
	VESSEL_DD,
	TXT_DD,					// NAME:			Text name of this unit type.
	"DD",						// NAME:			Text name of this unit type.
	ANIM_FBALL1,			// EXPLOSION:	Type of explosion when destroyed.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Only has eight facings?
		true,					// Always use the given name for the vehicle?
		true,					// Is it equipped with a combat turret?
	8,							// Rotation stages.
	14							// Turret center offset along body centerline.
);

// Cruiser
static VesselTypeClass const VesselCruiser(
	VESSEL_CA,
	TXT_CA,					// NAME:			Text name of this unit type.
	"CA",						// NAME:			Text name of this unit type.
	ANIM_FBALL1,			// EXPLOSION:	Type of explosion when destroyed.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Only has eight facings?
		true,					// Always use the given name for the vehicle?
		true,					// Is it equipped with a combat turret?
	8,							// Rotation stages.
	14							// Turret center offset along body centerline.
);

// Transport
static VesselTypeClass const VesselTransport(
	VESSEL_TRANSPORT,
	TXT_TRANSPORT,			// NAME:			Text name of this unit type.
	"LST",					// NAME:			Text name of this unit type.
	ANIM_FBALL1,			// EXPLOSION:	Type of explosion when destroyed.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Only has eight facings?
		true,					// Always use the given name for the vehicle?
		false,				// Is it equipped with a combat turret?
	0,							// Rotation stages.
	0							// Turret center offset along body centerline.
);

// Gun Boat
static VesselTypeClass const VesselPTBoat(
	VESSEL_PT,
	TXT_PT,					// NAME:			Text name of this unit type.
	"PT",						// NAME:			Text name of this unit type.
	ANIM_FBALL1,			// EXPLOSION:	Type of explosion when destroyed.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Only has eight facings?
		true,					// Always use the given name for the vehicle?
		true,					// Is it equipped with a combat turret?
	8,							// Rotation stages.
	14							// Turret center offset along body centerline.
);


#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
// Missile Submarine
static VesselTypeClass const VesselMissileSubmarine(
	VESSEL_MISSILESUB,
	TXT_MISSILESUB,		// NAME:			Text name of this unit type.
	"MSUB",					// NAME:			Text name of this unit type.
	ANIM_FBALL1,			// EXPLOSION:	Type of explosion when destroyed.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Only has eight facings?
		true,					// Always use the given name for the vehicle?
		false,				// Is it equipped with a combat turret?
	8,							// Rotation stages.
	14							// Turret center offset along body centerline.
);
#endif

#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
// Transport
static VesselTypeClass const VesselCarrier(
	VESSEL_CARRIER,
	TXT_CARRIER,			// NAME:			Text name of this unit type.
	"CARR",					// NAME:			Text name of this unit type.
	ANIM_FBALL1,			// EXPLOSION:	Type of explosion when destroyed.
	0x0000,					//	Vertical offset.
	0x0000,					// Primary weapon offset along turret centerline.
	0x0000,					// Primary weapon lateral offset along turret centerline.
	0x0000,					// Secondary weapon offset along turret centerline.
	0x0000,					// Secondary weapon lateral offset along turret centerling.
		false,				// Only has eight facings?
		true,					// Always use the given name for the vehicle?
		false,				// Is it equipped with a combat turret?
	0,							// Rotation stages.
	0							// Turret center offset along body centerline.
);
#endif

/***********************************************************************************************
 * VesselTypeClass::VesselTypeClass -- Constructor for unit types.                             *
 *                                                                                             *
 *    This is the constructor for the vessel static data. Each vessels is assign a specific    *
 *    variation. This class elaborates what the variation actually is.                         *
 *                                                                                             *
 * INPUT:   bla bla bla... see below                                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created                                                                  *
 *=============================================================================================*/
VesselTypeClass::VesselTypeClass(
				VesselType type,
				int name,
				char const * ininame,
				AnimType exp,
				int verticaloffset,
				int primaryoffset,
				int primarylateral,
				int secondaryoffset,
				int secondarylateral,
				bool is_eight,
				bool is_nominal,
				bool is_turret_equipped,
				int rotation,
				int toffset
				) :
					TechnoTypeClass(
										RTTI_VESSELTYPE,
										int(type),
										name,
										ininame,
										REMAP_NORMAL,
										verticaloffset,
										primaryoffset,
										primarylateral,
										secondaryoffset,
										secondarylateral,
										is_nominal,
										false,
										true,
										true,
										false,
										false,
										false,
										is_turret_equipped,
										true,
										true,
										rotation,
										SPEED_FLOAT
										),
	IsPieceOfEight(is_eight),
	Type(type),
	TurretOffset(toffset),
	Mission(MISSION_GUARD),
	Explosion(exp),
	MaxSize(0)
{
	/*
	**	Forced vessel overrides from the default.
	*/
	IsCrew = false;
	Speed = SPEED_FLOAT;
	IsScanner = true;
}


/***********************************************************************************************
 * VesselTypeClass::operator new -- Allocate a vessel type object from the special memory pool *
 *                                                                                             *
 *    This will allocate a vessel type class object from the memory pool.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the allocated vessel type class object. If memory in the *
 *          special heap has been exhaused, then NULL will be returned.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * VesselTypeClass::operator new(size_t)
{
	return(VesselTypes.Alloc());
}


/***********************************************************************************************
 * VesselTypeClass::operator delete -- Returns a vessel type object back to the memory pool.   *
 *                                                                                             *
 *    This will return a previously allocated vessel object back to the special pool from      *
 *    whence it was originally allocated.                                                      *
 *                                                                                             *
 * INPUT:   pointer  -- Pointer to the vessel type object to return to the pool.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselTypeClass::operator delete(void * pointer)
{
	VesselTypes.Free((VesselTypeClass *)pointer);
}


/***********************************************************************************************
 * VesselTypeClass::Init_Heap -- Initialize the vessel heap.                                   *
 *                                                                                             *
 *    This will pre-allocate all the vessel types required.                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once and do so before processing the rules.ini file.     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselTypeClass::Init_Heap(void)
{
	/*
	**	These vessel type class objects must be allocated in the exact order that they
	**	are specified in the VesselType enumeration. This is necessary because the heap
	**	allocation block index serves double duty as the type number index.
	*/
	new VesselTypeClass(VesselSubmarine);				//	VESSEL_SS
	new VesselTypeClass(VesselDestroyer);				//	VESSEL_DD
	new VesselTypeClass(VesselCruiser);					// VESSEL_CA
	new VesselTypeClass(VesselTransport);				// VESSEL_TRANSPORT
	new VesselTypeClass(VesselPTBoat);					// VESSEL_PT
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	new VesselTypeClass(VesselMissileSubmarine);		// VESSEL_MISSILESUB
#endif
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	new VesselTypeClass(VesselCarrier);					// VESSEL_CARRIER
#endif
}


/***********************************************************************************************
 * VesselTypeClass::As_Reference -- Converts a vessel type into a VesselTypeClass reference.   *
 *                                                                                             *
 *    This routine will fetch a reference to the vessel type that corresponds to the vessel    *
 *    type specified.                                                                          *
 *                                                                                             *
 * INPUT:   type  -- The vessel type number to convert.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the vessel type class that corresponds to the vessel   *
 *          type specified.                                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
VesselTypeClass & VesselTypeClass::As_Reference(VesselType type)
{
	return(*VesselTypes.Ptr(type));
}


#ifdef NEVER
/***********************************************************************************************
 * VesselTypeClass::Who_Can_Build_Me -- Fetches pointer to available factory for this vessel.  *
 *                                                                                             *
 *    Use this routine to fetch a pointer to the vessel factory that can build this vessel     *
 *    type.                                                                                    *
 *                                                                                             *
 * INPUT:   intheory -- If true, then this indicates that if the factory is currently          *
 *                      busy doing other things, this won't make in ineligible for searching. *
 *                      Typical use of this is by the sidebar logic which needs only to know   *
 *                      if theoretical production is allowed.                                  *
 *                                                                                             *
 *          legal    -- If true, then the buildings are checked for specific legality when     *
 *                      being scanned. For building placement, this is usually false, for      *
 *                      sidebar button adding, this is usually true.                           *
 *                                                                                             *
 *          house    -- The owner of the unit to be produced. This has an effect of legality.  *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the factory (building) that can produce this vessel type.*
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingClass * VesselTypeClass::Who_Can_Build_Me(bool intheory, bool legal, HousesType house) const
{
	BuildingClass * anybuilding = NULL;

	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass * building = Buildings.Ptr(index);
		assert(building != NULL);

		if (	!building->IsInLimbo &&
				building->House->Class->House == house &&
				building->Class->ToBuild == RTTI_VESSELTYPE &&
				building->Mission != MISSION_DECONSTRUCTION &&
				((1L << building->ActLike) & Ownable) &&
				(!legal || building->House->Can_Build(Type, building->ActLike)) &&
				(intheory || !building->In_Radio_Contact())) {

			if (building->IsLeader) return(building);
			anybuilding = building;
		}
	}
	return(anybuilding);
}
#endif


/***********************************************************************************************
 * VesselTypeClass::Display -- Displays a generic representation of this vessel type.          *
 *                                                                                             *
 *    This routine is used by the scenario editor to display a representation of this          *
 *    vessel type in the object placement dialog.                                              *
 *                                                                                             *
 * INPUT:   x,y      -- Pixel coordinate to render the center of this vessel type to.          *
 *                                                                                             *
 *          window   -- The window to clip the shape to. The pixel coordinates are relative    *
 *                      to this window.                                                        *
 *                                                                                             *
 *          house    -- The owner of the vessel. This is used to give the vessel its color.    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef SCENARIO_EDITOR
void VesselTypeClass::Display(int x, int y, WindowNumberType window, HousesType ) const
{
	int shape = 0;
	void const * ptr = Get_Cameo_Data();
	if (ptr == NULL) {
		ptr = Get_Image_Data();
		shape = Rotation/6;
	}
	CC_Draw_Shape(ptr, shape, x, y, window, SHAPE_CENTER|SHAPE_WIN_REL);
}


/***********************************************************************************************
 * VesselTypeClass::Prep_For_Add -- Adds vessel types to the scenario editor object list.      *
 *                                                                                             *
 *    This routine is called when the scenario editor needs to obtain a list of the            *
 *    vessel object that can be placed down. It will submit all the vessel types that can      *
 *    be placed down.                                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselTypeClass::Prep_For_Add(void)
{
	for (VesselType index = VESSEL_FIRST; index < VESSEL_COUNT; index++) {
		if (As_Reference(index).Get_Image_Data() != NULL) {
			Map.Add_To_List(&As_Reference(index));
		}
	}
}
#endif	//SCENARIO_EDITOR


/***********************************************************************************************
 * VesselTypeClass::Create_One_Of -- Creates a vessel object that matches this vessel type.    *
 *                                                                                             *
 *    This routine is called when the type of vessel is known (by way of a VesselTypeClass)    *
 *    and a corresponding vessel object needs to be created.                                   *
 *                                                                                             *
 * INPUT:   house -- Pointer to the owner that this vessel will be assigned to.                *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the vessel object created. If no vessel could be         *
 *          created, then NULL is returned.                                                    *
 *                                                                                             *
 * WARNINGS:   The vessel is created in a limbo state. It must first be placed down upon       *
 *             the map before it starts to function.                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * VesselTypeClass::Create_One_Of(HouseClass * house) const
{
	return(new VesselClass(Type, house->Class->House));
}


/***********************************************************************************************
 * VesselTypeClass::Create_And_Place -- Creates a vessel and places it at location.            *
 *                                                                                             *
 *    This routine is used to create a vessel and then place it down upon the                  *
 *    map.                                                                                     *
 *                                                                                             *
 * INPUT:   cell  -- The location to place this vessel down upon.                              *
 *                                                                                             *
 *          house -- The house to assign this vessel's ownership to.                           *
 *                                                                                             *
 * OUTPUT:  bool; Was the vessel successfully created and placed down upon the map?            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool VesselTypeClass::Create_And_Place(CELL cell, HousesType house) const
{
	VesselClass * unit = new VesselClass(Type, house);
	if (unit != NULL) {
		return(unit->Unlimbo(Cell_Coord(cell), Random_Pick(DIR_N, DIR_MAX)));
	}
	delete unit;
	return(false);
}


/***********************************************************************************************
 * VesselTypeClass::Dimensions -- Fetches the pixel width and height of this vessel type.      *
 *                                                                                             *
 *    This routine is used to fetch the width and height of this vessel type. These dimensions *
 *    are not specific to any particular facing. Rather, they are only for the generic vessel  *
 *    size.                                                                                    *
 *                                                                                             *
 * INPUT:   width, height  -- Reference to the integers that are to be initialized with the    *
 *                            pixel width and height of this vessel type.                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselTypeClass::Dimensions(int & width, int & height) const
{
	width = 48;
	height = 48;
}


/***********************************************************************************************
 * VesselTypeClass::One_Time -- Performs one time initialization for vessel types.             *
 *                                                                                             *
 *    This routine will load in the vessel shape data. It should be called only once at the    *
 *    beginning of the game.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this once.                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselTypeClass::One_Time(void)
{
	for (VesselType index = VESSEL_FIRST; index < VESSEL_COUNT; index++) {
		char	fullname[_MAX_FNAME+_MAX_EXT];
		char	buffer[_MAX_FNAME];
		VesselTypeClass const & uclass = As_Reference(index);
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
		if (uclass.Level != -1 || index==VESSEL_CARRIER) {
#else
		if (uclass.Level != -1) {
#endif
//		if (uclass.IsBuildable) {

			/*
			**	Fetch the supporting data files for the unit.
			*/
			sprintf(buffer, "%sICON", uclass.Graphic_Name());
			_makepath(fullname, NULL, NULL, buffer, ".SHP");
			((void const *&)uclass.CameoData) = MFCD::Retrieve(fullname);
		}

		/*
		**	Fetch a pointer to the unit's shape data.
		*/
		_makepath(fullname, NULL, NULL, uclass.Graphic_Name(), ".SHP");
		((void const *&)uclass.ImageData) = MFCD::Retrieve(fullname);

		((int &)uclass.MaxSize) = 26;
	}
}


/***********************************************************************************************
 * VesselTypeClass::Turret_Adjust -- Adjust turret offset according to facing specified.       *
 *                                                                                             *
 *    This routine will determine the pixel adjustment necessary for a turret. The direction   *
 *    specified is what the vessel body is facing.                                             *
 *                                                                                             *
 * INPUT:   dir   -- The presumed direction of the body facing for the vessel.                 *
 *                                                                                             *
 *          x,y   -- The center pixel position for the vessel. These values should be          *
 *                   adjusted (they are references) to match the adjusted offset for the       *
 *                   turret.                                                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselTypeClass::Turret_Adjust(DirType dir, int & x, int & y) const
{
	short xx = x;
	short yy = y;

	switch (Type) {
		case VESSEL_CA:
			Normal_Move_Point(xx, yy, dir, 22);
			x = xx;
			y = yy-4;
			break;

		case VESSEL_PT:
			Normal_Move_Point(xx, yy, dir, 14);
			x = xx;
			y = yy+1;
			break;

		case VESSEL_DD:
			Normal_Move_Point(xx, yy, dir+DIR_S, 8);
			x = xx;
			y = yy-4;
			break;
	}
}


/***********************************************************************************************
 * VesselTypeClass::Overlap_List -- Figures the overlap list for the vessel type.              *
 *                                                                                             *
 *    This routine will return the overlap list for a vessel that is sitting still in the      *
 *    center of a cell.                                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the overlap list that this vessel would use.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * VesselTypeClass::Overlap_List(void) const
{
	static short const _ship[] = {-3, -2, -1, 1, 2, 3,
		-MAP_CELL_W, -(MAP_CELL_W+1), -(MAP_CELL_W-1), -(MAP_CELL_W+2), -(MAP_CELL_W-2),
		+MAP_CELL_W, +(MAP_CELL_W+1), +(MAP_CELL_W-1), +(MAP_CELL_W+2), +(MAP_CELL_W-2),
		REFRESH_EOL};
//	static short const _ship[] = {-1, 1,
//		-MAP_CELL_W, -(MAP_CELL_W+1), -(MAP_CELL_W-1),
//		+MAP_CELL_W, +(MAP_CELL_W+1), +(MAP_CELL_W-1),
//		REFRESH_EOL};

	return(&_ship[0]);
}


/***********************************************************************************************
 * VesselTypeClass::From_Name -- Converts a name into a vessel type.                           *
 *                                                                                             *
 *    Use this routine to convert an ASCII version of a vessel type into the corresponding     *
 *    VesselType id value. Typical use of this would be to parse the INI file.                 *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII name to be converted into a vessel type.             *
 *                                                                                             *
 * OUTPUT:  Returns with the vessel type number that matches the string specified.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
VesselType VesselTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (VesselType classid = VESSEL_FIRST; classid < VESSEL_COUNT; classid++) {
			if (stricmp(As_Reference(classid).IniName, name) == 0) {
				return(classid);
			}
		}
	}
	return(VESSEL_NONE);
}


/***********************************************************************************************
 * VesselTypeClass::Max_Pips -- Fetches the maximum pips allowed for this vessel.              *
 *                                                                                             *
 *    This routine will determine the number of pips (maximum) allowed for this unit type.     *
 *    Typically, this is the number of passengers allowed.												  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the maximum number of pips allowed for this vessel type.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/01/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
int VesselTypeClass::Max_Pips(void) const
{
	return(Max_Passengers());
}
