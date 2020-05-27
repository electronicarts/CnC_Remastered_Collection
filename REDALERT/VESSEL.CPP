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

/* $Header: /CounterStrike/VESSEL.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : VESSEL.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 03/13/96                                                     *
 *                                                                                             *
 *                  Last Update : July 31, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   VesselClass::AI -- Handles the AI processing for vessel objects.                          *
 *   VesselClass::Assign_Destination -- Assign a destination for this vessel.                  *
 *   VesselClass::Can_Enter_Cell -- Determines if the vessel can enter the cell specified.     *
 *   VesselClass::Can_Fire -- Determines if this vessel can fire its weapon.                   *
 *   VesselClass::Is_Allowed_To_Recloak -- Can the vessel recloak now?                         *
 *   VesselClass::Class_Of -- Fetches a reference to the vessel's class data.                  *
 *   VesselClass::Combat_AI -- Handles firing and target selection for the vessel.             *
 *   VesselClass::Debug_Dump -- Dumps the vessel status information to the mono monitor.       *
 *   VesselClass::Draw_It -- Draws the vessel.                                                 *
 *   VesselClass::Edge_Of_World_AI -- Determine if vessel is off the edge of the world.        *
 *   VesselClass::Enter_Idle_Mode -- Causes the vessel to enter its default idle mode.         *
 *   VesselClass::Fire_Coord -- Fetches the coordinate the firing originates from.             *
 *   VesselClass::Greatest_Threat -- Determines the greatest threat (best target) for the vesse*
 *   VesselClass::Init -- Initialize the vessel heap system.                                   *
 *   VesselClass::Mission_Retreat -- Perform the retreat mission.                              *
 *   VesselClass::Overlap_List -- Fetches the overlap list for this vessel object.             *
 *   VesselClass::Per_Cell_Process -- Performs once-per-cell action.                           *
 *   VesselClass::Read_INI -- Read the vessel data from the INI database.                      *
 *   VesselClass::Repair_AI -- Process any self-repair required of this vessel.                *
 *   VesselClass::Rotation_AI -- Handles turret and body rotation for this vessel.             *
 *   VesselClass::Shape_Number -- Calculates the shape number for the ship body.               *
 *   VesselClass::Start_Driver -- Starts the vessel by reserving the location it is moving to. *
 *   VesselClass::Take_Damage -- Assign damage to the vessel.                                  *
 *   VesselClass::VesselClass -- Constructor for vessel class objects.                         *
 *   VesselClass::What_Action -- Determines action to perform on specified cell.               *
 *   VesselClass::Write_INI -- Write all vessel scenario data to the INI database.             *
 *   VesselClass::~VesselClass -- Destructor for vessel objects.                               *
 *   operator delete -- Deletes a vessel's memory block.                                       *
 *   operator new -- Allocates a vessel object memory block.                                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "function.h"


/***********************************************************************************************
 * VesselClass::VesselClass -- Constructor for vessel class objects.                           *
 *                                                                                             *
 *    This is the normal constructor for vessel class objects. It will set up a vessel that    *
 *    is valid excepting that it won't be placed on the map.                                   *
 *                                                                                             *
 * INPUT:   classid  -- The type of vessel this will be.                                       *
 *                                                                                             *
 *          house    -- The owner of this vessel.                                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
VesselClass::VesselClass(VesselType classid, HousesType house) :
	DriveClass(RTTI_VESSEL, Vessels.ID(this), house),
	Class(VesselTypes.Ptr((int)classid)),
	IsToSelfRepair(false),
	IsSelfRepairing(false),
	DoorShutCountDown(0),
	PulseCountDown(0),
	SecondaryFacing(PrimaryFacing)
{
	House->Tracking_Add(this);

	/*
	**	The ammo member is actually part of the techno class, but must be initialized
	**	manually here because this is where we first have access to the class pointer.
	*/
	Ammo = Class->MaxAmmo;

	/*
	**	For two shooters, clear out the second shot flag -- it will be set the first time
	**	the object fires. For non two shooters, set the flag since it will never be cleared
	**	and the second shot flag tells the system that normal rearm times apply -- this is
	**	what is desired for non two shooters.
	*/
	IsSecondShot = !Class->Is_Two_Shooter();
	Strength = Class->MaxStrength;

	/*
	**	The techno class cloakabilty flag is set according to the type
	**	class cloakability flag.
	*/
	IsCloakable = Class->IsCloakable;

	/*
	** Keep count of the number of units created.
	*/
//	if (Session.Type == GAME_INTERNET) {
//		House->UnitTotals->Increment_Unit_Total((int)classid);
//	}
}


/***********************************************************************************************
 * VesselClass::~VesselClass -- Destructor for vessel objects.                                 *
 *                                                                                             *
 *    The destructor will destroy the vessel and ensure that it is properly removed from the   *
 *    game engine.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
VesselClass::~VesselClass(void)
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
 * operator new -- Allocates a vessel object memory block.                                     *
 *                                                                                             *
 *    This routine is used to allocate a block of memory from the vessel heap. If there is     *
 *    no more space in the heap, then this routine will return NULL.                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the pointer to the allocated block of memory.                         *
 *                                                                                             *
 * WARNINGS:   This routine could return NULL.                                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * VesselClass::operator new(size_t)
{
	void * ptr = Vessels.Alloc();
	if (ptr != NULL) {
		((VesselClass *)ptr)->Set_Active();
	}
	return(ptr);
}


/***********************************************************************************************
 * operator delete -- Deletes a vessel's memory block.                                         *
 *                                                                                             *
 *    This overloaded delete operator will return the vessel's memory back to the pool of      *
 *    memory used for vessel allocation.                                                       *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the vessel's memory block.                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::operator delete(void * ptr)
{
	if (ptr != NULL) {
		assert(((VesselClass *)ptr)->IsActive);
		((VesselClass *)ptr)->IsActive = false;
	}
	Vessels.Free((VesselClass *)ptr);
}


/***********************************************************************************************
 * VesselClass::Class_Of -- Fetches a reference to the vessel's class data.                    *
 *                                                                                             *
 *    This routine will return with a reference to the static class data for this vessel.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the class data structure associated with this vessel.  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectTypeClass const & VesselClass::Class_Of(void) const
{
	assert(IsActive);

	return(*Class);
}


/***********************************************************************************************
 * VesselClass::Can_Enter_Cell -- Determines if the vessel can enter the cell specified.       *
 *                                                                                             *
 *    This routine is used by find path and other movement logic to determine if this          *
 *    vessel can enter the cell specified.                                                     *
 *                                                                                             *
 * INPUT:   cell  -- The cell to check this vessel against.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the movement restriction associated with movement into this object.   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
MoveType VesselClass::Can_Enter_Cell(CELL cell, FacingType ) const
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	if ((unsigned)cell >= MAP_CELL_TOTAL) return(MOVE_NO);

	CellClass const * cellptr = &Map[cell];

	/*
	**	Moving off the edge of the map is not allowed unless
	**	this is a loaner vehicle.
	*/
	if (!ScenarioInit && !Map.In_Radar(cell) && !Is_Allowed_To_Leave_Map()) {
		return(MOVE_NO);
	}

	MoveType retval = MOVE_OK;

	/*
	**	If there is blocking terrain (such as ice), then the vessel
	**	can't move there.
	*/
	if (cellptr->Cell_Terrain() != NULL) {
		return(MOVE_NO);
	}

	/*
	**	If the cell is out and out impassable because of underlying terrain, then
	**	return this immutable fact.
	*/
	if (Ground[cellptr->Land_Type()].Cost[Class->Speed] == 0) {
		return(MOVE_NO);
	}

	/*
	**	If some allied object has reserved the cell, then consider the cell
	**	as blocked by a moving object.
	*/
	if (cellptr->Flag.Composite) {

		if (cellptr->Flag.Occupy.Building) {
			return(MOVE_NO);
		}

		TechnoClass * techno = cellptr->Cell_Techno();
		if (techno != NULL && techno->Cloak == CLOAKED && !House->Is_Ally(techno)) {
			return(MOVE_CLOAK);
		}

		/*
		**	If reserved by a vehicle, then consider this blocked terrain.
		*/
		if (cellptr->Flag.Occupy.Vehicle) {
			retval = MOVE_MOVING_BLOCK;
		}
	}

	/*
	**	Return with the most severe reason why this cell would be impassable.
	*/
	return(retval);
}


/***********************************************************************************************
 * VesselClass::Shape_Number -- Calculates the shape number for the ship body.                 *
 *                                                                                             *
 *    This routine will return with the shape number to use for the ship's body.               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the shape number to use for the ship's body when drawing.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/31/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int VesselClass::Shape_Number(void) const
{
	/*
	**	For eight facing units, adjust the facing number accordingly.
	*/
	FacingType facing = Dir_Facing(PrimaryFacing.Current());

	int shapenum = UnitClass::BodyShape[Dir_To_16(PrimaryFacing)*2]>>1;

	/*
	**	Special case code for transport. The north/south facing is in frame
	**	0. The east/west facing is in frame 3.
	*/
	if (*this == VESSEL_TRANSPORT) {
		shapenum = 0;
	}
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	if (*this == VESSEL_CARRIER) {
		shapenum = 0;
	}
#endif
	/*
	**	Door opening and closing animation stage check.
	*/
	if (!Is_Door_Closed()) {
		shapenum = Door_Stage();
	}

	return(shapenum);
}


/***********************************************************************************************
 * VesselClass::Draw_It -- Draws the vessel.                                                   *
 *                                                                                             *
 *    Draws the vessel on the tactical display. This routine is called by the map rendering    *
 *    process to display the vessel.                                                           *
 *                                                                                             *
 * INPUT:   x,y   -- The pixel coordinate to draw this vessel at.                              *
 *                                                                                             *
 *          window-- The window to base clipping and coordinates upon when drawing.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Draw_It(int x, int y, WindowNumberType window) const
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	/*
	**	Verify the legality of the unit class.
	*/
	void const * shapefile = Get_Image_Data();
	if (shapefile == NULL) return;

	/*
	** Need to know the shape name for the overlay now. ST - 8/19/2019 1:37PM
	*/
	const char *turret_shape_name = NULL;

	/*
	**	If drawing of this unit is not explicitly prohibited, then proceed
	**	with the render process.
	*/
	const bool is_hidden = (Visual_Character() == VISUAL_HIDDEN) && (window != WINDOW_VIRTUAL);
	if (!is_hidden) {
		int			facing = Dir_To_32(PrimaryFacing);
		int			tfacing = Dir_To_32(SecondaryFacing);
		DirType		rotation = DIR_N;
		int			scale = 0x0100;

		/*
		**	Actually perform the draw. Overlay an optional shimmer effect as necessary.
		*/
		Techno_Draw_Object(shapefile, Shape_Number(), x, y, window, rotation, scale);

		/*
		**	If there is a turret, then it must be rendered as well. This may include
		**	firing animation if required.
		*/
		if (Class->IsTurretEquipped) {
			int xx = x;
			int yy = y;

			/*
			**	Determine which turret shape to use. This depends on if there
			**	is any firing animation in progress.
			*/
			int shapenum = TechnoClass::BodyShape[tfacing]+32;
			DirType turdir = DirType(Dir_To_16(PrimaryFacing)*16);

			switch (Class->Type) {
				case VESSEL_CA:
					turret_shape_name = "TURR";
					shapefile = Class->TurretShapes;
					shapenum = TechnoClass::BodyShape[Dir_To_32(SecondaryFacing)];
					Class->Turret_Adjust(turdir, xx, yy);
					// Add shape file name forl new shape draw intercept. ST - 8/19/2019 1:42PM
					//Techno_Draw_Object(shapefile, shapenum, xx, yy, window);
					Techno_Draw_Object_Virtual(shapefile, shapenum, xx, yy, window, DIR_N, 0x0100, turret_shape_name);
					xx = x;
					yy = y;
					turdir = DirType(Dir_To_16(PrimaryFacing+DIR_S)*16);
					Class->Turret_Adjust(turdir, xx, yy);
					break;

				case VESSEL_DD:
					turret_shape_name = "SSAM";
					shapefile = Class->SamShapes;
					shapenum = TechnoClass::BodyShape[Dir_To_32(SecondaryFacing)];
					Class->Turret_Adjust(turdir, xx, yy);
					break;

				case VESSEL_PT:
					turret_shape_name = "MGUN";
					shapefile = Class->MGunShapes;
					shapenum = TechnoClass::BodyShape[Dir_To_32(SecondaryFacing)];
					Class->Turret_Adjust(turdir, xx, yy);
					break;

				default:
					shapenum = TechnoClass::BodyShape[Dir_To_32(SecondaryFacing)];
					Class->Turret_Adjust(turdir, xx, yy);
					break;
			}

			/*
			**	Actually perform the draw. Overlay an optional shimmer effect as necessary.
			*/
			// Add shape file name forl new shape draw intercept. ST - 8/19/2019 1:42PM
			if (turret_shape_name) {
				Techno_Draw_Object_Virtual(shapefile, shapenum, xx, yy, window, DIR_N, 0x0100, turret_shape_name);
			} else {
				Techno_Draw_Object(shapefile, shapenum, xx, yy, window);
			}
		}
	}

	DriveClass::Draw_It(x, y, window);

	/*
	** Patch so the transport will draw its passengers on top of itself.
	*/
	if (!Is_Door_Closed() && IsTethered && In_Radio_Contact() && !Contact_With_Whom()->IsInLimbo) {
		TechnoClass * contact = Contact_With_Whom();
		assert(contact->IsActive);

		int xxx = x + ((int)Lepton_To_Pixel((int)Coord_X(contact->Render_Coord())) - (int)Lepton_To_Pixel((int)Coord_X(Render_Coord())));
		int yyy = y + ((int)Lepton_To_Pixel((int)Coord_Y(contact->Render_Coord())) - (int)Lepton_To_Pixel((int)Coord_Y(Render_Coord())));
		contact->Draw_It(xxx, yyy, window);
		contact->IsToDisplay = false;
	}
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * VesselClass::Debug_Dump -- Dumps the vessel status information to the mono monitor.         *
 *                                                                                             *
 *    This routine will display the vessel's status information. The information is dumped to  *
 *    the monochrome monitor.                                                                  *
 *                                                                                             *
 * INPUT:   mono  -- Pointer to the monochrome screen that the information will be displayed   *
 *                   to.                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Debug_Dump(MonoClass * mono) const
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	mono->Set_Cursor(0, 0);
	mono->Print(Text_String(TXT_DEBUG_SHIP));
	mono->Set_Cursor(47, 5);mono->Printf("%02X:%02X", SecondaryFacing.Current(), SecondaryFacing.Desired());

	mono->Fill_Attrib(66, 13, 12, 1, IsSelfRepairing ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 14, 12, 1, IsToSelfRepair ? MonoClass::INVERSE : MonoClass::NORMAL);
	DriveClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * VesselClass::Overlap_List -- Fetches the overlap list for this vessel object.               *
 *                                                                                             *
 *    This routine will fetch the overlap list for this vessel type. It takes into             *
 *    consideration any movement the vessel may be doing.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the overlap list for this vessel.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * VesselClass::Overlap_List(bool redraw) const
{
	assert(Vessels.ID(this) == ID);
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

		if (IsSelected) {
			rect = Union(rect, Rect(-32, 32, 64, 64));
		}

		return(Coord_Spillage_List(Coord, rect, true));
	}
#else
	redraw = redraw;
#endif

	return(Coord_Spillage_List(Coord, 56)+1);
}


/***********************************************************************************************
 * VesselClass::AI -- Handles the AI processing for vessel objects.                            *
 *                                                                                             *
 *    This routine is called once for each vessel object during each main game loop. All       *
 *    normal AI processing is handled here. This includes dispatching and maintaining any      *
 *    processing that is specific to vessel objects.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1996 JLB : Created.                                                                 *
 *   07/16/1996 JLB : Prefers anti-sub weapons if firing on subs.                              *
 *=============================================================================================*/
void VesselClass::AI(void)
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	if (Mission == MISSION_NONE && MissionQueue == MISSION_NONE) {
		Enter_Idle_Mode();
	}

	/*
	**	HACK ALERT:
	**	If the ship finds itself in a hunt order but it has no weapons, then tell it
	**	to sail off the map instead.
	*/
	if (Mission == MISSION_HUNT && !Is_Weapon_Equipped()) {
		Assign_Mission(MISSION_RETREAT);
	}

	/*
	**	Act on new orders if the unit is at a good position to do so.
	*/
	if (!IsDriving && Is_Door_Closed() /*Mission != MISSION_UNLOAD*/) {
		Commence();
	}

#ifndef CLIPDRAW
	if (Map.In_View(Coord_Cell(Center_Coord()))) {
		Mark(MARK_CHANGE);
	}
#endif

#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
// Re-stock the ammo of any on-board helicopters on an aircraft carrier.
	if (*this == VESSEL_CARRIER && How_Many()) {
		if (!MoebiusCountDown) {
			MoebiusCountDown = Rule.ReloadRate * TICKS_PER_MINUTE;
			ObjectClass *obj = Attached_Object();
			while (obj) {
				long bogus;
				((AircraftClass *)obj)->Receive_Message(this,RADIO_RELOAD,bogus);
				obj = (obj->Next);
			}
		}
	}
#endif
	/*
	**	Process base class AI routine. If as a result of this, the vessel gets
	**	destroyed, then detect this fact and bail early.
	*/
	DriveClass::AI();
	if (!IsActive) {
		return;
	}

	/*
	**	Handle body and turret rotation.
	*/
	Rotation_AI();

	/*
	**	Handle any combat processing required.
	*/
	Combat_AI();

	/*
	**	Delete this unit if it finds itself off the edge of the map and it is in
	**	guard or other static mission mode.
	*/
	if (Edge_Of_World_AI()) {
		return;
	}

	if (Class->Max_Passengers() > 0) {

		/*
		**	Double check that there is a passenger that is trying to load or unload.
		**	If not, then close the door.
		*/
		if (!Is_Door_Closed() && Mission != MISSION_UNLOAD && Transmit_Message(RADIO_TRYING_TO_LOAD) != RADIO_ROGER && !(long)DoorShutCountDown) {
			LST_Close_Door();
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
	** Do a step of repair here, if appropriate.
	*/
	Repair_AI();
}


/***********************************************************************************************
 * VesselClass::Per_Cell_Process -- Performs once-per-cell action.                             *
 *                                                                                             *
 *    This routine is called when the vessel travels one cell. It handles any processes that   *
 *    must occur on a per-cell basis.                                                          *
 *                                                                                             *
 * INPUT:   why   -- Specifies the circumstances under which this routine was called.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Per_Cell_Process(PCPType why)
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	BStart(BENCH_PCP);

	if (why == PCP_END) {
		CELL cell = Coord_Cell(Coord);

		/*
		**	The unit performs looking around at this time. If the
		**	unit moved further than one square during the last track
		**	move, don't do an incremental look. Do a full look around
		**	instead.
		*/
		Look(!IsPlanningToLook);
		IsPlanningToLook = false;

		if (IsToSelfRepair) {
			for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
				CELL cell = Coord_Cell(Adjacent_Cell(Center_Coord(), face));
				SmartPtr<BuildingClass> whom;
				whom = Map[cell].Cell_Building();
				if (whom != NULL && ((*whom == STRUCT_SHIP_YARD) || (*whom == STRUCT_SUB_PEN)) ) {

					// MBL 04.27.2020: Make only audible to the correct player
					// if (IsOwnedByPlayer) Speak(VOX_REPAIRING);
					if (IsOwnedByPlayer) Speak(VOX_REPAIRING, House);

					IsSelfRepairing = true;
					IsToSelfRepair = false;
					break;
				}
			}
		}

		/*
		**	If this is a loaner unit and is is off the edge of the
		**	map, then it gets eliminated.
		*/
		if (Edge_Of_World_AI()) {
			BEnd(BENCH_PCP);
			return;
		}
	}

	if (IsActive) {
		DriveClass::Per_Cell_Process(why);
	}
	BEnd(BENCH_PCP);
}


/***********************************************************************************************
 * VesselClass::What_Action -- Determines what action would occur if clicked on object.        *
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
 *   04/16/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
ActionType VesselClass::What_Action(ObjectClass const * object) const
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	ActionType action = DriveClass::What_Action(object);

	if (action == ACTION_SELF) {
		if (Class->Max_Passengers() == 0 || !How_Many() ) {
			action = ACTION_NONE;
		} else {
// check to see if the transporter can unload.
			bool found = 0;
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
		if (*this != VESSEL_CARRIER)
#endif
			for (FacingType face = FACING_N; face < FACING_COUNT && !found; face++) {
				CELL cellnum = Adjacent_Cell(Coord_Cell(Coord), face);
				CellClass * cell = &Map[cellnum];
				if (Map.In_Radar(cellnum)) {
					if (Ground[cell->Land_Type()].Cost[SPEED_FOOT] == 0 || cell->Flag.Occupy.Building || cell->Flag.Occupy.Vehicle || cell->Flag.Occupy.Monolith || (cell->Flag.Composite & 0x01F) == 0x01F) {
						continue;
					} else {
						found = true;
					}
				}
			}
			if (!found) {
				action = ACTION_NONE;
			}
		}
	}

	/*
	**	Special return to friendly repair factory action.
	*/
	if (House->IsPlayerControl && action == ACTION_SELECT && object->What_Am_I() == RTTI_BUILDING) {
		BuildingClass * building = (BuildingClass *)object;

		if (building->Class->ToBuild == RTTI_VESSELTYPE && building->House->Is_Ally(this)) {
			action = ACTION_ENTER;
		}
	}
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	if (action == ACTION_ATTACK && object->What_Am_I() == RTTI_VESSEL &&
		 (*this == VESSEL_MISSILESUB || *this == VESSEL_CA) ) {
		 	action = ACTION_NOMOVE;
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
 * VesselClass::Active_Click_With -- Intercepts the active click to see if deployment is possib*
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
 *   04/16/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Active_Click_With(ActionType action, ObjectClass * object)
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

//	if (action != What_Action(object)) {
		action = What_Action(object);
		switch (action) {
			case ACTION_ENTER:
				action = ACTION_MOVE;
				// BRR 10/18/96 IsToSelfRepair = true;
				break;

			default:
//				action = ACTION_NONE;
				break;
		}
//	}
//	if (action == ACTION_ENTER) {
		// BRR 10/18/96 IsToSelfRepair = true;
//		action = ACTION_MOVE;
//	} else {
//		if (action != ACTION_NONE) {
			// BRR 10/18/96 IsSelfRepairing = IsToSelfRepair = false;
//		}
//	}

	DriveClass::Active_Click_With(action, object);
}


/***********************************************************************************************
 * VesselClass::Active_Click_With -- Performs specified action on specified cell.              *
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
 *   04/16/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Active_Click_With(ActionType action, CELL cell)
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	// BRR 10/18/96 IsToSelfRepair = false;
//	if (action != ACTION_NONE) {
		// BRR 10/18/96 IsSelfRepairing = false;
//	}
	DriveClass::Active_Click_With(action, cell);
}


/***********************************************************************************************
 * VesselClass::Take_Damage -- Assign damage to the vessel.                                    *
 *                                                                                             *
 *    This routine is called to apply damage to this vessel. The amount and type of damage     *
 *    to apply is passed as parameters. This routine could end up destroying the vessel.       *
 *                                                                                             *
 * INPUT:   damage   -- Reference to the amount of damage to apply to this vessel. The damage  *
 *                      value will be adjusted so that the actual damage applied will be       *
 *                      stored into this variable for possible subsequent examination.         *
 *                                                                                             *
 *          distance -- The distance from the center of the damage to the vessel itself.       *
 *                                                                                             *
 *          warhead  -- The warhead type of damage to apply.                                   *
 *                                                                                             *
 *          source   -- The perpetrator of this damage. Knowing who was responsible allows     *
 *                      retaliation logic.                                                     *
 *                                                                                             *
 *          forced   -- Is this damage forced upon the vessel by some supernatural means?      *
 *                                                                                             *
 * OUTPUT:  Returns with the result of the damage applied. This enumeration indicates the      *
 *          general effect of the damage. Examine this return value to see if the vessel       *
 *          has been destroyed.                                                                *
 *                                                                                             *
 * WARNINGS:   The vessel could be destroyed by the call to this routine!                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/13/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
ResultType VesselClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source, bool forced)
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	ResultType res = RESULT_NONE;

	/*
	**	In order for a this to be damaged, it must either be a unit
	**	with a crew or a sandworm.
	*/
	res = FootClass::Take_Damage(damage, distance, warhead, source, forced);

	if (res == RESULT_DESTROYED) {
		Death_Announcement(source);
		if (Class->Explosion != ANIM_NONE) {
			AnimType anim = Class->Explosion;

			new AnimClass(anim, Coord);

			/*
			**	Very strong units that have an explosion will also rock the
			**	screen when they are destroyed.
			*/
			if (Class->MaxStrength > 400) {
				int shakes = Class->MaxStrength / 150;
				Shake_The_Screen(shakes, Owner());
				if (source && Owner() != source->Owner()) {
					Shake_The_Screen(shakes, source->Owner());
				}
			}
		}

		/*
		**	Possibly have the crew member run away.
		*/
		Mark(MARK_UP);
		while (Is_Something_Attached()) {
			FootClass * object = Detach_Object();

			/*
			**	Only infantry can run from a destroyed vehicle. Even then, it is not a sure
			**	thing.
			*/
			object->Record_The_Kill(source);
			delete object;
		}

		/*
		**	Finally, delete the vehicle.
		*/
		delete this;

	} else {

		/*
		**	When damaged and below half strength, start smoking if
		**	it isn't already smoking (and it's not a submarine).
		*/
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		if (Health_Ratio() <= Rule.ConditionYellow && !IsAnimAttached && (*this != VESSEL_SS && *this != VESSEL_MISSILESUB) ) {
#else
		if (Health_Ratio() <= Rule.ConditionYellow && !IsAnimAttached && (*this != VESSEL_SS) ) {
#endif
			AnimClass * anim = new AnimClass(ANIM_SMOKE_M, Coord_Add(Coord, XYP_Coord(0, -8)));
			if (anim != NULL) anim->Attach_To(this);
		}
	}
	return(res);
}


/***********************************************************************************************
 * VesselClass::Can_Fire -- Determines if this vessel can fire its weapon.                     *
 *                                                                                             *
 *    This routine is used to determine if this vessel can fire its weapon at the target       *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   target   -- The target candidate to determine if firing upon is valid.             *
 *                                                                                             *
 *          which    -- Which weapon to use when considering the candidate as a potential      *
 *                      target.                                                                *
 *                                                                                             *
 * OUTPUT:  Returns with the fire error type. This enum indicates if the vessel and fire. If   *
 *          it can't fire, then the enum indicates why.                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/13/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
FireErrorType VesselClass::Can_Fire(TARGET target, int which) const
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	DirType			dir;					// The facing to impart upon the projectile.
	int				diff;

#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	if (*this == VESSEL_CARRIER) {
		if(!How_Many() || Arm) {
			return(FIRE_REARM);
		} else {
			return(FIRE_OK);
		}
	}
#endif
	FireErrorType	fire = DriveClass::Can_Fire(target, which);
if(*this==VESSEL_DD) {
Mono_Set_Cursor(0,0);
}
	if (fire == FIRE_OK || fire == FIRE_CLOAKED) {
		WeaponTypeClass const * weapon = (which == 0) ? Class->PrimaryWeapon : Class->SecondaryWeapon;

		/*
		**	Ensure that a torpedo will never be fired upon a non naval target.
		** Unless that non-naval target is a naval building (sub pen/ship yard)
		*/
		bool isseatarget = Is_Target_Vessel(target);
		bool isbridgetarget = false;
		if (weapon->Bullet->IsSubSurface) {
			isbridgetarget = Is_Target_Cell(target);	// enable shooting at bridges
			isseatarget |= isbridgetarget;
		}
		BuildingClass * bldg = ::As_Building(target);
		if (bldg != NULL && bldg->Class->Speed == SPEED_FLOAT) {
			isseatarget = true;
		}

		dir = Direction(target);

		if (weapon->Bullet->IsSubSurface) {
			if (!isseatarget && Is_Target_Object(target)) {
				return(FIRE_CANT);
			}

			/*
			** If it's a torpedo, let's check line-of-sight to make sure that
			** there's only water squares between us and the target.
			*/
			ObjectClass * obj = As_Object(target);
			COORDINATE coord = Center_Coord();
			if (obj != NULL) {
				int totaldist = ::Distance(coord, obj->Center_Coord());
				while (totaldist > CELL_LEPTON_W) {
					coord = Coord_Move(coord, dir, CELL_LEPTON_W);
					if (Map[coord].Land_Type() != LAND_WATER) {
						if (!isbridgetarget) {
							return(FIRE_RANGE);
						}
					}

					/*
					** Check for friendly boats in the way.
					*/
					TechnoClass * tech = Map[coord].Cell_Techno();
					if (tech != NULL && tech != this && House->Is_Ally(tech)) {
						return(FIRE_RANGE);
					}
					totaldist -= CELL_LEPTON_W;
				}
			}
		}

		/*
		**	Depth charges are only good against submarines.
		*/
		if (weapon->Bullet->IsAntiSub) {
			if (!isseatarget) {
				return(FIRE_CANT);
			} else {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				if (Is_Target_Vessel(target) && (*As_Vessel(target) != VESSEL_SS && *As_Vessel(target) != VESSEL_MISSILESUB) ) {
#else
				if (Is_Target_Vessel(target) && *As_Vessel(target) != VESSEL_SS) {
#endif
					if (!Is_Target_Vessel(target) || !weapon->Bullet->IsSubSurface) {
						return(FIRE_CANT);
					}
				}
			}
		} else {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			if (Is_Target_Vessel(target) && (*As_Vessel(target) == VESSEL_SS || *As_Vessel(target) == VESSEL_MISSILESUB)) {
#else
			if (Is_Target_Vessel(target) && *As_Vessel(target) == VESSEL_SS) {
#endif
				return(FIRE_CANT);
			}
		}

		/*
		**	If this unit cannot fire while moving, then bail.
		*/
		if (!Class->IsTurretEquipped && Target_Legal(NavCom)) {
			return(FIRE_MOVING);
		}

		/*
		**	If the turret is rotating and the projectile isn't a homing type, then
		**	firing must be delayed until the rotation stops.
		*/
		if (!IsFiring && IsRotating && weapon->Bullet->ROT == 0) {
			return(FIRE_ROTATING);
		}

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
		if (diff > 8) {
			return(FIRE_FACING);
		}
	}
	return(fire);
}


/***********************************************************************************************
 * VesselClass::Fire_Coord -- Fetches the coordinate the firing originates from.               *
 *                                                                                             *
 *    This routine is called to determine the coordinate that a fired projectile will          *
 *    originate from.                                                                          *
 *                                                                                             *
 * INPUT:   which -- Which weapon is this query directed at?                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate where a projectile would appear if it were fired.      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/13/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE VesselClass::Fire_Coord(int which) const
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	COORDINATE coord = Center_Coord();

	if (*this == VESSEL_CA) {
		if (IsSecondShot) {
			coord = Coord_Move(coord, PrimaryFacing + DIR_S, 0x0100);
		} else {
			coord = Coord_Move(coord, PrimaryFacing, 0x0100);
		}
		coord = Coord_Move(coord, DIR_N, 0x0030);
		coord = Coord_Move(coord, Turret_Facing(), 0x0040);
		return(coord);
	}

	if (*this == VESSEL_PT) {
		coord = Coord_Move(coord, PrimaryFacing, 0x0080);
		coord = Coord_Move(coord, DIR_N, 0x0020);
		coord = Coord_Move(coord, Turret_Facing(), 0x0010);
		return(coord);
	}

	return(DriveClass::Fire_Coord(which));
}


/***********************************************************************************************
 * VesselClass::Init -- Initialize the vessel heap system.                                     *
 *                                                                                             *
 *    This routine is used to clear out the vessel heap. It is called whenever a scenario is   *
 *    being initialized prior to scenario or saved game loading.                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   All vessel objects are invalid after this routine is called.                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/13/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Init(void)
{
	Vessels.Free_All();
}


/***********************************************************************************************
 * VesselClass::Greatest_Threat -- Determines the greatest threat (best target) for the vessel *
 *                                                                                             *
 *    This routine is used by ships to determine what target they should go after.             *
 *                                                                                             *
 * INPUT:   threat   -- The threat type that this ship should go after (as determined by the   *
 *                      team mission or general self defense principles).                      *
 *                                                                                             *
 * OUTPUT:  Returns with the target that this ship should attack.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/13/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET VesselClass::Greatest_Threat(ThreatType threat) const
{
	if (*this == VESSEL_SS) {
		threat = threat & ThreatType(THREAT_RANGE|THREAT_AREA);
		threat = threat | THREAT_BOATS;

		//BG: get subs to attack buildings also.
		threat = threat | THREAT_BUILDINGS;
		threat = threat | THREAT_FACTORIES;
	} else {
		if ((threat & (THREAT_GROUND|THREAT_POWER|THREAT_FACTORIES|THREAT_TIBERIUM|THREAT_BASE_DEFENSE|THREAT_BOATS)) == 0) {
			if (Class->PrimaryWeapon != NULL) {
				threat = threat | Class->PrimaryWeapon->Allowed_Threats();
			}

			if (Class->SecondaryWeapon != NULL) {
				threat = threat | Class->SecondaryWeapon->Allowed_Threats();
			}

//			threat = threat | THREAT_GROUND | THREAT_BOATS;
		}

		// Cruisers can never hit infantry anyway, so take 'em out of the list
		// of possible targets.
		if (*this == VESSEL_CA) {
			threat = (ThreatType) (threat & (~THREAT_INFANTRY));
		}
	}
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	if (*this == VESSEL_CARRIER) {
		return(TARGET_NONE);
	}
#endif
	return(FootClass::Greatest_Threat(threat));
}


/***********************************************************************************************
 * VesselClass::Enter_Idle_Mode -- Causes the vessel to enter its default idle mode.           *
 *                                                                                             *
 *    This routine is called when the vessel is finished with what it is doing, but the next   *
 *    action is not known. This routine will determine what is the appropriate course of       *
 *    action for this vessel and then start it doing that.                                     *
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
void VesselClass::Enter_Idle_Mode(bool )
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	MissionType	order = MISSION_GUARD;

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

		if (Class->PrimaryWeapon == NULL) {
			if (IsALoaner && Class->Max_Passengers() > 0 && Is_Something_Attached() && !Team) {
				order = MISSION_UNLOAD;
			} else {
				order = MISSION_GUARD;
				Assign_Target(TARGET_NONE);
				Assign_Destination(TARGET_NONE);
			}

		} else {

			if (Mission == MISSION_GUARD || Mission == MISSION_GUARD_AREA || MissionControl[Mission].IsParalyzed || MissionControl[Mission].IsZombie) {
				return;
			}

			if (House->IsHuman || Team.Is_Valid()) {
				order = MISSION_GUARD;
			} else {
				if (House->IQ < Rule.IQGuardArea) {
					order = MISSION_GUARD;
				} else {
					order = MISSION_GUARD_AREA;
				}
			}
		}
	}
	Assign_Mission(order);
}


/***********************************************************************************************
 * VesselClass::Receive_Message -- Handles receiving a radio message.                          *
 *                                                                                             *
 *    This is the handler function for when a vessel receives a radio                          *
 *    message. Typical use of this is when a unit unloads from a lst                           *
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
 *   05/31/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
RadioMessageType VesselClass::Receive_Message(RadioClass * from, RadioMessageType message, long & param)
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	switch (message) {

		/*
		**	Asks if the passenger can load on this transport.
		*/
		case RADIO_CAN_LOAD:
			if (Class->Max_Passengers() == 0 || from == NULL || !House->Is_Ally(from->Owner())) return(RADIO_STATIC);
			if (How_Many() < Class->Max_Passengers()) {
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	if(*this == VESSEL_CARRIER && from->What_Am_I() == RTTI_AIRCRAFT) {
		return(RADIO_ROGER);
	}
#endif
			/*
			** Before saying "Sure, come on board", make sure we're adjacent to
			** the shore.
			*/
				CELL cell;
				Desired_Load_Dir(from, cell);
				if(cell) {
					return(RADIO_ROGER);
				}
			}
			return(RADIO_NEGATIVE);

		/*
		**	This message is sent by the passenger when it determines that it has
		**	entered the transport.
		*/
		case RADIO_IM_IN:
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	if(*this != VESSEL_CARRIER) {
#endif
			if (How_Many() == Class->Max_Passengers()) {
				LST_Close_Door();
			} else {
				DoorShutCountDown = TICKS_PER_SECOND;
			}
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
	}
#endif
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
					param = TARGET_NONE;
					Transmit_Message(RADIO_HELLO, from);
					Transmit_Message(RADIO_MOVE_HERE, param);
					return(RADIO_ROGER);
				} else {

					/*
					**	This causes the potential passenger to think that all is ok and to
					**	hold on for a bit.
					*/
					return(RADIO_ROGER);
				}
			}

			/*
			**
			*/
			if (Class->Max_Passengers() > 0 && *this == VESSEL_TRANSPORT && How_Many() < Class->Max_Passengers()) {
				DriveClass::Receive_Message(from, message, param);

				if (!IsDriving && !IsRotating) {
//				if (!IsDriving && !IsRotating && !IsTethered) {

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

							/*
							**	If it is now facing the correct direction, then open the
							**	transport doors. Close the doors if the transport is full or needs
							**	to rotate.
							*/
							if (!Is_Door_Open()) {
								LST_Open_Door();
							}

							/*
							**	Tell the potential passenger where it should go. If the passenger is
							**	already at the staging location, then tell it to move onto the transport
							**	directly.
							*/
							if (Transmit_Message(RADIO_MOVE_HERE, param, from) == RADIO_YEA_NOW_WHAT) {
								if (Is_Door_Open()) {
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
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
			if (Class->Max_Passengers() > 0 && *this == VESSEL_CARRIER && How_Many() < Class->Max_Passengers()) {
				TechnoClass::Receive_Message(from, message, param);
				/*
				**	Establish contact with the object if this building isn't already in contact
				**	with another.
				*/
				if (!In_Radio_Contact()) {
					Transmit_Message(RADIO_HELLO, from);
				}
				if (Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER) {
					param = As_Target();
					if (Transmit_Message(RADIO_MOVE_HERE, param) == RADIO_YEA_NOW_WHAT) {
						Transmit_Message(RADIO_TETHER);
					}
				}
				return(RADIO_ROGER);
			}
#endif
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
 * VesselClass::Desired_Load_Dir -- Determines the best cell and facing for loading.           *
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
 *   06/01/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
DirType VesselClass::Desired_Load_Dir(ObjectClass * passenger, CELL & moveto) const
{
	assert(Vessels.ID(this) == ID);
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

#if(0)
		/*
		**	Give more weight to the cells that require the least rotation of the transport or the
		**	least roundabout movement for the potential passenger.
		*/
		value -= (int)ABS((int)(signed char)Facing_Dir(face) - (int)(signed char)faceto);
		if (face == FACING_S) {
			value -= 100;
		}
		if (face == FACING_SW || face == FACING_SE) value += 64;
#endif
		/*
		**	If the value for the potential cell is greater than the last recorded potential
		**	value, then record this cell as the best candidate.
		*/
		if (bestval == -1 || value > bestval) {
			bestval = value;
			bestdir = face;
//		} else {
//			ObjectClass * obj = Map[cellnum].Cell_Occupier();
//			if (obj) obj->Scatter(Coord, true);
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
	return(DIR_N);
}


/***********************************************************************************************
 * VesselClass::LST_Open_Door -- Opens a LST door.                                             *
 *                                                                                             *
 *    This routine will initiate opening of the doors on the LST.                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/01/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::LST_Open_Door(void)
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	if (!IsDriving && !IsRotating) {
		Open_Door(5, 6);
	}
}


/***********************************************************************************************
 * VesselClass::LST_Close_Door -- Closes a LST door.                                           *
 *                                                                                             *
 *    This routine will initiate closing of the LST door.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/01/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::LST_Close_Door(void)
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	Close_Door(5, 6);
}


/***********************************************************************************************
 * VesselClass::Mission_Unload -- Handles unloading cargo.                                     *
 *                                                                                             *
 *    This is the AI control sequence for when a transport desires to unload its cargo.	     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before calling this routine again.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/01/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
int VesselClass::Mission_Unload(void)
{
	assert(Vessels.ID(this) == ID);
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
		case VESSEL_TRANSPORT:
			switch (Status) {
				case INITIAL_CHECK:
					dir = Desired_Load_Dir(NULL, cell);
					if (How_Many() > 0 && cell != 0) {
						Do_Turn(dir);
						Status = MANEUVERING;
						return(1);
					} else {
						if (!How_Many()) {	// don't break out if still carrying passengers
							Assign_Mission(MISSION_GUARD);
						}
					}
					break;

				case MANEUVERING:
					if (!IsRotating) {
						LST_Open_Door();
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

						/*
						**	Don't do anything if still in radio contact.
						*/
						if (In_Radio_Contact()) return(TICKS_PER_SECOND);

						FootClass * passenger = Detach_Object();

						if (passenger != NULL) {
							DirType toface = DIR_S + PrimaryFacing;
							bool placed = false;

							for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
								DirType newface = toface + Facing_Dir(face);
								CELL newcell = Adjacent_Cell(Coord_Cell(Coord), newface);

								if (passenger->Can_Enter_Cell(newcell) == MOVE_OK) {
									ScenarioInit++;
									passenger->Unlimbo(Coord_Move(Coord, newface, CELL_LEPTON_W/2), newface);
									ScenarioInit--;
									passenger->Assign_Mission(MISSION_MOVE);
									passenger->Assign_Destination(::As_Target(newcell));
									passenger->Commence();
									Transmit_Message(RADIO_HELLO, passenger);
									Transmit_Message(RADIO_TETHER, passenger);
									if (passenger->What_Am_I() == RTTI_UNIT) {
										((UnitClass *)passenger)->IsToScatter = true;
									}
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

								/*
								**	Tell everyone around the transport to scatter.
								*/
								for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
									CellClass * cellptr = &Map[Coord].Adjacent_Cell(face);
									if (cellptr->Is_Clear_To_Move(SPEED_TRACK, true, true)) {
										cellptr->Incoming(0, true);
									}
								}

//								Status = CLOSING_DOOR;
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
				**	Close LST door in preparation for normal operation.
				*/
				case CLOSING_DOOR:
					if (Is_Door_Open()) {
						LST_Close_Door();
					}
					if (Is_Door_Closed()) {
						if (IsALoaner) {
							Assign_Mission(MISSION_RETREAT);
						} else {
							Assign_Mission(MISSION_GUARD);
						}
					}
					break;
			}
			break;

		default:
			break;
	}
	return(MissionControl[Mission].Normal_Delay());
}


/***********************************************************************************************
 * VesselClass::Assign_Destination -- Assign a destination for this vessel.                    *
 *                                                                                             *
 *    This routine is called when a destination is to be assigned to this vessel.              *
 *                                                                                             *
 * INPUT:   target   -- The destination to assign to this vessel.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Assign_Destination(TARGET target)
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
	if (In_Radio_Contact() && Class->Max_Passengers() > 0 && (Contact_With_Whom()->Is_Infantry() || Contact_With_Whom()->What_Am_I() == RTTI_UNIT)) {
		long param = TARGET_NONE;
		Transmit_Message(RADIO_MOVE_HERE, param);		// should stop objects heading toward this transport.
		Transmit_Message(RADIO_OVER_OUT);
		if (!Is_Door_Closed()) {
			LST_Close_Door();
		}
	}

	if (!Is_Door_Closed()) {
		LST_Close_Door();
	}

	DriveClass::Assign_Destination(target);
}


/***********************************************************************************************
 * VesselClass::Pip_Count -- Fetches the number of pips to display on vessel.                  *
 *                                                                                             *
 *    This routine is used to fetch the number of "fullness" pips to display on the vessel.    *
 *    This will be the number of passengers on a transport.                                    *
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
int VesselClass::Pip_Count(void) const
{
	if (Techno_Type_Class()->Max_Passengers() > 0) {
		int passengers = 0;
		ObjectClass const * object = Attached_Object();
		for (int index = 0; index < Class_Of().Max_Pips(); index++) {
			if (object != NULL) {
				passengers++;
				object = object->Next;
			}
		}
		return passengers;
	}

	return 0;
}


/***********************************************************************************************
 * VesselClass::Mission_Retreat -- Perform the retreat mission.                                *
 *                                                                                             *
 *    This will cause the vessel to run away from the battlefield. It searches for an escape   *
 *    map edge according to the reinforcement edge specified in the house.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before this routine is called      *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int VesselClass::Mission_Retreat(void)
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	enum {
		PICK_RETREAT_POINT,
		TRAVEL
	};
	switch (Status) {
		case PICK_RETREAT_POINT:
			IsALoaner = true;
			if (!Target_Legal(NavCom)) {
//				CELL cell = Map.Calculated_Cell(House->Control.Edge, (Team.Is_Valid()) ? Team->Class->Origin : -1, -1, Class->Speed);
				CELL cell = Map.Calculated_Cell(House->Control.Edge, (Team.Is_Valid()) ? Team->Class->Origin : -1, Coord_Cell(Center_Coord()), Class->Speed);
				if (Team.Is_Valid()) {
					Team->Remove(this);
				}
				Assign_Destination(::As_Target(cell));
			}
			Status = TRAVEL;
			return(1);

		case TRAVEL:
			if (!Target_Legal(NavCom)) {
				Status = PICK_RETREAT_POINT;
			}
			break;

		default:
			break;
	}
	return(MissionControl[Mission].Normal_Delay());
}


/***********************************************************************************************
 * VesselClass::Is_Allowed_To_Recloak -- Can the vessel recloak now?                           *
 *                                                                                             *
 *    Asking this question is part of the recloak process. If the answer is no, then           *
 *    recloaking is postponed. This facilitates keeping submarines visible for longer than     *
 *    they otherwise would be.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can this vessel recloak now?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
bool VesselClass::Is_Allowed_To_Recloak(void) const
{
	return(PulseCountDown == 0);
}


/***********************************************************************************************
 * VesselClass::Read_INI -- Read the vessel data from the INI database.                        *
 *                                                                                             *
 *    This will read and create all vessels specified in the INI database. This routine is     *
 *    called when the scenario starts.                                                         *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to read the vessel data from.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Vessels will be created and placed on the map by this function.                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Read_INI(CCINIClass & ini)
{
	VesselClass	* vessel;			// Working vessel pointer.
	HousesType	inhouse;			// Vessel house.
	VesselType	classid;			// Vessel class.
	char			buf[128];

	int len = ini.Entry_Count(INI_Name());
	for (int index = 0; index < len; index++) {
		char const * entry = ini.Get_Entry(INI_Name(), index);

		ini.Get_String(INI_Name(), entry, NULL, buf, sizeof(buf));
		inhouse = HouseTypeClass::From_Name(strtok(buf, ","));
		if (inhouse != HOUSE_NONE) {
			classid = VesselTypeClass::From_Name(strtok(NULL, ","));

			if (classid != VESSEL_NONE) {

				if (HouseClass::As_Pointer(inhouse) != NULL) {
					vessel = new VesselClass(classid, inhouse);
					if (vessel != NULL) {

						/*
						**	Read the raw data.
						*/
						int strength = atoi(strtok(NULL, ",\r\n"));

						CELL cell = atoi(strtok(NULL, ",\r\n"));

						COORDINATE coord = Cell_Coord(cell);

						DirType dir = (DirType)atoi(strtok(NULL, ",\r\n"));
						MissionType mission = MissionClass::Mission_From_Name(strtok(NULL, ",\n\r"));

						vessel->Trigger = NULL;
						TriggerTypeClass * tp = TriggerTypeClass::From_Name(strtok(NULL, ",\r\n"));
						if (tp != NULL) {
							TriggerClass * tt = Find_Or_Make(tp);
							if (tt != NULL) {
								tt->AttachCount++;
								vessel->Trigger = tt;
							}
						}

						if (vessel->Unlimbo(coord, dir)) {
							vessel->Strength = (int)vessel->Class->MaxStrength * fixed(strength, 256);
							if (vessel->Strength > vessel->Class->MaxStrength-3) vessel->Strength = vessel->Class->MaxStrength;
	//						vessel->Strength = Fixed_To_Cardinal(vessel->Class->MaxStrength, strength);
							if (Session.Type == GAME_NORMAL || vessel->House->IsHuman) {
								vessel->Assign_Mission(mission);
								vessel->Commence();
							} else {
								vessel->Enter_Idle_Mode();
							}

						} else {

							/*
							**	If the vessel could not be unlimboed, then this is a catastrophic error
							**	condition. Delete the vessel.
							*/
							delete vessel;
						}
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * VesselClass::Write_INI -- Write all vessel scenario data to the INI database.               *
 *                                                                                             *
 *    This routine is used to add the vessel data (needed for scenario start) to the INI       *
 *    database specified. If there was any preexisting vessel data in the database, it will    *
 *    be cleared                                                                               *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the ini database to store the vessel data into.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Write_INI(CCINIClass & ini)
{
	/*
	**	First, clear out all existing vessel data from the ini file.
	*/
	ini.Clear(INI_Name());

	/*
	**	Write the vessel data out.
	*/
	for (int index = 0; index < Vessels.Count(); index++) {
		VesselClass * vessel = Vessels.Ptr(index);
		if (vessel != NULL && !vessel->IsInLimbo && vessel->IsActive) {
			char	uname[10];
			char	buf[128];

			sprintf(uname, "%d", index);
			sprintf(buf, "%s,%s,%d,%u,%d,%s,%s",
				vessel->House->Class->IniName,
				vessel->Class->IniName,
				vessel->Health_Ratio()*256,
				Coord_Cell(vessel->Coord),
				vessel->PrimaryFacing.Current(),
				MissionClass::Mission_Name(vessel->Mission),
				vessel->Trigger.Is_Valid() ? vessel->Trigger->Class->IniName : "None"
				);
			ini.Put_String(INI_Name(), uname, buf);
		}
	}
}


/***********************************************************************************************
 * VesselClass::Start_Driver -- Starts the vessel by reserving the location it is moving to.   *
 *                                                                                             *
 *    This routine is called when the vessel starts moving. It will reserve the destination    *
 *    cell so that it won't be occupied by another vessel as this one is travelling.           *
 *                                                                                             *
 * INPUT:   headto   -- The coordinate that will be headed to.                                 *
 *                                                                                             *
 * OUTPUT:  bool; Was the destination location successfully marked?                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool VesselClass::Start_Driver(COORDINATE & headto)
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	if (DriveClass::Start_Driver(headto) && IsActive) { //BG IsActive can be cleared by Start_Driver
		Mark_Track(headto, MARK_DOWN);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * VesselClass::What_Action -- Determines action to perform on specified cell.                 *
 *                                                                                             *
 *    This routine will determine what action to perform if the mouse were clicked over the    *
 *    cell specified.																								  *
 *                                                                                             *
 * INPUT:   cell  -- The cell that the mouse might be clicked on.                              *
 *                                                                                             *
 * OUTPUT:  Returns with the action type that this unit will perform if the mouse were         *
 *          clicked of the cell specified.                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
ActionType VesselClass::What_Action(CELL cell) const
{
	assert(Vessels.ID(this) == ID);
	assert(IsActive);

	ActionType action = DriveClass::What_Action(cell);
	if (action == ACTION_NOMOVE && Map[cell].Land_Type() == LAND_BEACH) {
		return(ACTION_MOVE);
	}

	if (action == ACTION_NOMOVE && Class->PrimaryWeapon != NULL && Class->PrimaryWeapon->Bullet->IsSubSurface && Map[cell].Is_Bridge_Here()) {
		return(ACTION_ATTACK);
	}
	return(action);
}


/***********************************************************************************************
 * VesselClass::Rotation_AI -- Handles turret and body rotation for this vessel.               *
 *                                                                                             *
 *    Any turret or body rotation for this vessel will be handled by this routine.             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per vessel per game logic loop.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Rotation_AI(void)
{
	if (Target_Legal(TarCom) && !IsRotating) {
		DirType dir = Direction(TarCom);

		if (Class->IsTurretEquipped) {
			SecondaryFacing.Set_Desired(dir);
		}
	}

	IsRotating = false;
	if (Class->IsTurretEquipped) {

		if (SecondaryFacing.Is_Rotating()) {
			Mark(MARK_CHANGE_REDRAW);
			if (SecondaryFacing.Rotation_Adjust((Class->ROT * House->GroundspeedBias)+1)) {
				Mark(MARK_CHANGE_REDRAW);
			}

			/*
			**	If no further rotation is necessary, flag that the rotation
			**	has stopped.
			*/
			IsRotating = SecondaryFacing.Is_Rotating();
		}
	}
}


/***********************************************************************************************
 * VesselClass::Combat_AI -- Handles firing and target selection for the vessel.               *
 *                                                                                             *
 *    This routine will process firing logic for the vessel. It includes searching for targets *
 *    and performing any adjustments necessary to bring the target to bear.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per vessel per game logic loop.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Combat_AI(void)
{
	if (Target_Legal(TarCom) && Is_Weapon_Equipped()) {

		/*
		**	Determine which weapon can fire. First check for the primary weapon. If that weapon
		**	cannot fire, then check any secondary weapon. If neither weapon can fire, then the
		**	failure code returned is that from the primary weapon.
		*/
		int primary = What_Weapon_Should_I_Use(TarCom);
		FireErrorType ok = Can_Fire(TarCom, primary);

		switch (ok) {
			case FIRE_OK:
				Fire_At(TarCom, primary);
				break;

			case FIRE_FACING:
				if (Class->IsTurretEquipped) {
					SecondaryFacing.Set_Desired(Direction(TarCom));
				} else {
					if (!PrimaryFacing.Is_Rotating()) {
						PrimaryFacing.Set_Desired(Direction(TarCom));
					}
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
 * VesselClass::Edge_Of_World_AI -- Determine if vessel is off the edge of the world.          *
 *                                                                                             *
 *    In addition to detecting the edge of world case, this routine will delete the vessel     *
 *    if it occurs.                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the vessel deleted by this routine?                                      *
 *                                                                                             *
 * WARNINGS:   Be sure to examine the return value and if true, abort any further processing   *
 *             for this vessel since it has been deleted. This routine should be called once   *
 *             per vessel per game logic loop.                                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool VesselClass::Edge_Of_World_AI(void)
{
	if (!IsDriving && !Map.In_Radar(Coord_Cell(Coord)) && IsLocked) {
		if (Team.Is_Valid()) Team->IsLeaveMap = true;
		Stun();
		delete this;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * VesselClass::Repair_AI -- Process any self-repair required of this vessel.                  *
 *                                                                                             *
 *    When a vessel repairs, it does so 'by itself' and not under direct control of another    *
 *    object. This self repair logic is processed here. Upon repair completion of money        *
 *    exhuastion, the repair process will terminate.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Call this routine only once per vessel per game logic loop.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Repair_AI(void)
{
	if (IsSelfRepairing) {
		if ((Frame % (TICKS_PER_MINUTE * Rule.RepairRate)) == 0) {
			Mark(MARK_CHANGE);
			int cost = Class->Repair_Cost();
			int step = Class->Repair_Step();

			if (House->Available_Money() >= cost) {
				House->Spend_Money(cost);
				Strength += step;
				if (Strength >= Class->MaxStrength) {
					Strength = Class->MaxStrength;
					IsSelfRepairing = IsToSelfRepair = false;

					// MBL 04.27.2020: Make only audible to the correct player
					// if (IsOwnedByPlayer) Speak(VOX_UNIT_REPAIRED);
					if (IsOwnedByPlayer) Speak(VOX_UNIT_REPAIRED, House);
				}
			}
		}
	}
}

#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
/***********************************************************************************************
 * VesselClass::Fire_At -- Try to fire upon the target specified.                              *
 *                                                                                             *
 *    This routine is the auto-fire logic for the ship. It will check                          *
 *    to see if we're an aircraft carrier, and if so, launch one of our                        *
 *    aircraft.  If we're not an aircraft carrier, it lets the higher-level                    *
 *    Fire_At logic take over.                                                                 *
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
BulletClass * VesselClass::Fire_At(TARGET target, int which)
{
	//PG assert(Units.ID(this) == ID);
	assert(IsActive);

	if (*this == VESSEL_CARRIER) {
		Arm = CarrierLaunchDelay;
		FootClass * passenger = Detach_Object();
		if (passenger != NULL) {
			ScenarioInit++;
			passenger->Unlimbo(Center_Coord());
			ScenarioInit--;
			passenger->Assign_Mission(MISSION_ATTACK);
			passenger->Assign_Target(TarCom);
			passenger->Commence();
// If we've launched our last aircraft, discontinue attacking.
			if (!How_Many()) Assign_Target(TARGET_NONE);
		}
	} else {
		return DriveClass::Fire_At(target, which);
	}
	return (NULL);
}

#endif