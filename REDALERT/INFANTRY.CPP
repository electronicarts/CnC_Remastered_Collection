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

/* $Header: /CounterStrike/INFANTRY.CPP 2     3/03/97 10:35p Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : INFANTRY.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : August 15, 1994                                              *
 *                                                                                             *
 *                  Last Update : October 28, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   InfantryClass::AI -- Handles the infantry non-graphic related AI processing.              *
 *   InfantryClass::Active_Click_With -- Handles action when clicking with infantry soldier.   *
 *   InfantryClass::Assign_Destination -- Gives the infantry a movement destination.           *
 *   InfantryClass::Assign_Target -- Gives the infantry a combat target.                       *
 *   InfantryClass::Can_Enter_Cell -- Determines if the infantry can enter the cell specified. *
 *   InfantryClass::Can_Fire -- Can the infantry fire its weapon?                              *
 *   InfantryClass::Class_Of -- Returns the class reference for this object.                   *
 *   InfantryClass::Clear_Occupy_Bit -- Clears occupy bit and given cell                       *
 *   InfantryClass::Debug_Dump -- Displays debug information about infantry unit.              *
 *   InfantryClass::Detach -- Removes the specified target from targeting computer.            *
 *   InfantryClass::Do_Action -- Launches the infantry into an animation sequence.             *
 *   InfantryClass::Doing_AI -- Handles the animation AI processing.                           *
 *   InfantryClass::Draw_It -- Draws a unit object.                                            *
 *   InfantryClass::Edge_Of_World_AI -- Detects when infantry has left the map.                *
 *   InfantryClass::Enter_Idle_Mode -- The infantry unit enters idle mode by this routine.     *
 *   InfantryClass::Fear_AI -- Process any fear related affects on this infantry.              *
 *   InfantryClass::Fire_At -- Fires projectile from infantry unit.                            *
 *   InfantryClass::Firing_AI -- Handles firing and combat AI for the infantry.                *
 *   InfantryClass::Full_Name -- Fetches the full name of the infantry unit.                   *
 *   InfantryClass::Get_Image_Data -- Fetches the image data for this infantry unit.           *
 *   InfantryClass::Greatest_Threat -- Determines greatest threat (target) for infantry unit.  *
 *   InfantryClass::InfantryClass -- The constructor for infantry objects.                     *
 *   InfantryClass::Init -- Initialize the infantry object system.                             *
 *   InfantryClass::Is_Ready_To_Random_Anima -- Checks to see if it is ready to perform an idle*
 *   InfantryClass::Limbo -- Performs cleanup operations needed when limboing.                 *
 *   InfantryClass::Mission_Attack -- Intercept attack mission for special handling.           *
 *   InfantryClass::Movement_AI -- This routine handles all infantry movement logic.           *
 *   InfantryClass::Overlap_List -- The list of cells that the infantry overlaps, but doesn't o*
 *   InfantryClass::Paradrop -- Handles paradropping infantry.                                 *
 *   InfantryClass::Per_Cell_Process -- Handles special operations that occur once per cell.   *
 *   InfantryClass::Random_Animate -- Randomly animate the infantry (maybe)                    *
 *   InfantryClass::Read_INI -- Reads units from scenario INI file.                            *
 *   InfantryClass::Response_Attack -- Plays infantry audio response to attack order.          *
 *   InfantryClass::Response_Move -- Plays infantry response to movement order.                *
 *   InfantryClass::Response_Select -- Plays infantry audio response due to being selected.    *
 *   InfantryClass::Scatter -- Causes the infantry to scatter to nearby cell.                  *
 *   InfantryClass::Set_Occupy_Bit -- Sets the occupy bit cell and bit pos	                    *
 *   InfantryClass::Set_Primary_Facing -- Change infantry primary facing -- always and instantl*
 *   InfantryClass::Shape_Number -- Fetch the shape number for this infantry.                  *
 *   InfantryClass::Start_Driver -- Handles giving immediate destination and move orders.      *
 *   InfantryClass::Stop_Driver -- Stops the infantry from moving any further.                 *
 *   InfantryClass::Take_Damage -- Applies damage to the infantry unit.                        *
 *   InfantryClass::Unlimbo -- Unlimbo infantry unit in legal sub-location.                    *
 *   InfantryClass::What_Action -- Determines what action to perform for the cell specified.   *
 *   InfantryClass::What_Action -- Infantry units might be able to capture -- check.           *
 *   InfantryClass::Write_INI -- Store the infantry to the INI database.                       *
 *   InfantryClass::operator delete -- Returns the infantry object back to the free pool       *
 *   InfantryClass::operator new -- Allocates an infantry object from the free pool.           *
 *   InfantryClass::~InfantryClass -- Default destructor for infantry units.                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

/*
** New sidebar for GlyphX multiplayer. ST - 8/7/2019 10:10AM
*/
#include "SidebarGlyphx.h"


int const InfantryClass::HumanShape[32] = {0,0,7,7,7,7,6,6,6,6,5,5,5,5,5,4,4,4,3,3,3,3,2,2,2,2,1,1,1,1,1,0};


/***************************************************************************
** This is the array of constant data associated with infantry maneuvers. It
**	specifies the frame rate as well as if the animation can be aborted.
*/
// interruptible, mobile, randomstart, rate
DoStruct const InfantryClass::MasterDoControls[DO_COUNT] = {
	{true,	false,	false,	0},	// DO_STAND_READY
	{true,	false,	false,	0},	// DO_STAND_GUARD
	{true,	false,	false,	0},	// DO_PRONE
	{true,	true,		true,		2},	// DO_WALK
	{true,	false,	false,	1},	// DO_FIRE_WEAPON
	{false,	true,		false,	2},	// DO_LIE_DOWN
	{true,	true,		true,		2},	// DO_CRAWL
	{false,	false,	false,	3},	// DO_GET_UP
	{true,	false,	false,	1},	// DO_FIRE_PRONE
	{true,	false,	false,	2},	// DO_IDLE1
	{true,	false,	false,	2},	// DO_IDLE2
	{false,	false,	false,	2},	// DO_GUN_DEATH
	{false,	false,	false,	2},	// DO_EXPLOSION_DEATH
	{false,	false,	false,	2},	// DO_EXPLOSION2_DEATH
	{false,	false,	false,	2},	// DO_GRENADE_DEATH
	{false,	false,	false,	2},	// DO_FIRE_DEATH
	{false,	false,	false,	2},	// DO_GESTURE1
	{false,	false,	false,	2},	// DO_SALUTE1
	{false,	false,	false,	2},	// DO_GESTURE2
	{false,	false,	false,	2},	// DO_SALUTE2
	{false,	false,	false,	2},	// DO_DOG_MAUL
};


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * InfantryClass::Debug_Dump -- Displays debug information about infantry unit.                *
 *                                                                                             *
 *    This routine is used by the debug version to display pertinent information about the     *
 *    infantry unit.                                                                           *
 *                                                                                             *
 * INPUT:   mono  -- The monochrome screen to display the debug information to.                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Debug_Dump(MonoClass * mono) const
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	mono->Set_Cursor(0, 0);

	mono->Print(Text_String(TXT_DEBUG_INFANTRY));
	mono->Set_Cursor(1, 11);mono->Printf("%3d", Doing);
	mono->Set_Cursor(8, 11);mono->Printf("%3d", Fear);

	mono->Fill_Attrib(66, 13, 12, 1, IsTechnician ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 14, 12, 1, IsStoked ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 15, 12, 1, IsProne ? MonoClass::INVERSE : MonoClass::NORMAL);

	FootClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * InfantryClass::InfantryClass -- The constructor for infantry objects.                       *
 *                                                                                             *
 *    This is the constructor used when creating an infantry unit. All values are required     *
 *    except for facing and position. If these are absent, then the infantry is created in     *
 *    a state of limbo -- not placed upon the map.                                             *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryClass::InfantryClass(InfantryType classid, HousesType house) :
	FootClass(RTTI_INFANTRY, Infantry.ID(this), house),
	Class(InfantryTypes.Ptr((int)classid)),
	Doing(DO_NOTHING),
	Comment(0),
	IsTechnician(false),
	IsStoked(false),
	IsProne(false),
	IsZoneCheat(false),
	WasSelected(false),
	Fear(FEAR_NONE),
	StopDriverFrame(-1),
	LookCell(0)
{
	House->Tracking_Add(this);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
	IsCloakable = Class->IsCloakable;
#endif
	/*
	**	For two shooters, clear out the second shot flag -- it will be set the first time
	**	the object fires. For non two shooters, set the flag since it will never be cleared
	**	and the second shot flag tells the system that normal rearm times apply -- this is
	**	what is desired for non two shooters.
	*/
	if (Class->Is_Two_Shooter()) {
		IsSecondShot = false;
	} else {
		IsSecondShot = true;
	}
	Strength = Class->MaxStrength;

	/*
	**	Civilians carry much less ammo than soldiers do.
	*/
	Ammo = Class->MaxAmmo;
}


/***********************************************************************************************
 * InfantryClass::~InfantryClass -- Default destructor for infantry units.                     *
 *                                                                                             *
 *    This is the default destructor for infantry type units. It will put the infantry into    *
 *    a limbo state if it isn't already in that state and the game is still active.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryClass::~InfantryClass(void)
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
		Limbo();
	}
	ID = -1;
}


/***********************************************************************************************
 * InfantryClass::operator new -- Allocates an infantry object from the free pool.             *
 *                                                                                             *
 *    This will allocate an infantry object from the infantry object free pool. If there is    *
 *    no available slot, then NULL is returned.                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the allocated infantry object or NULL if none could be   *
 *          allocated.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void * InfantryClass::operator new(size_t)
{
	void * ptr = Infantry.Allocate();
	if (ptr != NULL) {
		((InfantryClass *)ptr)->Set_Active();
	}
	return(ptr);
}


/***********************************************************************************************
 * InfantryClass::operator delete -- Returns the infantry object back to the free pool         *
 *                                                                                             *
 *    This routine is used return an infantry object back to the system.                       *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the infantry object to delete.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::operator delete(void * ptr)
{
	if (ptr != NULL) {
		((InfantryClass *)ptr)->IsActive = false;
	}
	Infantry.Free((InfantryClass *)ptr);
}


/***********************************************************************************************
 * InfantryClass::Take_Damage -- Applies damage to the infantry unit.                          *
 *                                                                                             *
 *    This routine applies the damage specified to the infantry object. It is possible that    *
 *    this routine will DESTROY the infantry unit in the process.                              *
 *                                                                                             *
 * INPUT:   damage   -- The damage points to inflict.                                          *
 *                                                                                             *
 *          distance -- The distance from the damage center point to the object's center point.*
 *                                                                                             *
 *          warhead  -- The warhead type that is inflicting the damage.                        *
 *                                                                                             *
 *          source   -- Who is responsible for inflicting the damage.                          *
 *                                                                                             *
 * OUTPUT:  bool; Was the infantry unit destroyed by this damage?                              *
 *                                                                                             *
 * WARNINGS:   Since the infantry unit could be destroyed by this routine, be sure to check    *
 *             for this in the code that follows the call to Take_Damage().                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *   11/22/1994 JLB : Shares base damage handler for techno objects.                           *
 *   03/31/1995 JLB : Revenge factor.                                                          *
 *=============================================================================================*/
ResultType InfantryClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source, bool forced)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	ResultType res = RESULT_NONE;

	/*
	**	Prone infantry take only half damage, but never below one damage point.
	*/
	if (IsProne && damage > 0) {
		damage = damage * Rule.ProneDamageBias;
	}

	/*
	** If we're taking damage from a dog, we have to decide if we're the
	** target of the dog.  Dogs don't spill collateral damage onto anyone
	** else, so if we're the target of a valid dog, take full damage, but if
	** we're not the target, or the dog doesn't exist, then take no damage.
	*/
	if (source != NULL && source->What_Am_I() == RTTI_INFANTRY && ((InfantryClass *)source)->Class->IsDog) {
		if (source->TarCom == As_Target()) {
			damage = Strength;
		} else {
			damage = 0;
		}
	}
	res = FootClass::Take_Damage(damage, distance, warhead, source, forced);

	/*
	** hack for dog: if you're hit by a dog, and you're the target, your
	** damage gets upped to max.
	*/

	if (res == RESULT_NONE) return(res);

	if (res == RESULT_DESTROYED) {
		if (*this == INFANTRY_TANYA) {
			IsTanyaDead = true;
		}
		Death_Announcement(source);
		Stop_Driver();
		Stun();
		Mission = MISSION_NONE;
		Assign_Mission(MISSION_GUARD);
		Commence();

		VocType sound;
		VocType altsound;
		sound = Sim_Random_Pick(VOC_SCREAM1, VOC_SCREAM11);
		altsound = VOC_YELL1;
		if (*this == INFANTRY_TANYA) {
			sound = altsound = VOC_TANYA_DIE;
		}
		if (Class->IsDog) {
			sound = altsound = VOC_DOG_HURT;
		}

		/*
		**	The type of warhead determines the animation the infantry
		**	will perform when killed.
		*/
		bool delthis = false;
		TARGET us = As_Target();
		switch (WarheadTypeClass::As_Pointer(warhead)->InfantryDeath) {
			default:
			case 0:
				delthis = true;
				break;

			case 1:
				Sound_Effect(sound, Coord);
				Do_Action(DO_GUN_DEATH, true);
				break;

			case 2:
				Sound_Effect(sound, Coord);
				Do_Action(DO_EXPLOSION_DEATH, true);
				break;

			case 3:
				Sound_Effect(sound, Coord);
				Do_Action(DO_GRENADE_DEATH, true);
				break;

			case 4:
				Sound_Effect(altsound, Coord);
				Do_Action(DO_FIRE_DEATH, true);
				break;

			case 5:
				Sound_Effect(sound, Coord);
				AnimType anim = ANIM_ELECT_DIE;
				if (Class->IsDog) anim = ANIM_DOG_ELECT_DIE;
				new AnimClass(anim, Coord);
				delthis = true;
				break;
		}

		if (delthis) {
			delete this;
		}
		return(res);
	}

	/*
	**	When infantry gets hit, it gets scared.
	*/
	if (res != RESULT_DESTROYED) {
		COORDINATE source_coord = (source) ? source->Coord : NULL;

		/*
		**	If an engineer is damaged and it is just sitting there, then tell it
		**	to go do something since it will definitely die if it doesn't.
		*/
		if (!House->IsHuman && *this == INFANTRY_RENOVATOR && (Mission == MISSION_GUARD || Mission == MISSION_GUARD_AREA)) {
			Assign_Mission(MISSION_HUNT);
		}

		if (source != NULL) {
			Scatter(source_coord);
		}

		if (source != NULL && Fear < FEAR_SCARED) {
			if (Class->IsFraidyCat) {
				Fear = FEAR_PANIC;
			} else {
				Fear = FEAR_SCARED;
			}
		} else {

			/*
			**	Increase the fear of the infantry by a bit. The fear increases more
			**	quickly if the infantry is damaged.
			*/
			int morefear = FEAR_ANXIOUS;
			if (Health_Ratio() > Rule.ConditionRed) morefear /= 2;
			if (Health_Ratio() > Rule.ConditionYellow) morefear /= 2;
			Fear = FearType(min((int)Fear + morefear, FEAR_MAXIMUM));
		}
	}
	return(res);
}


/***********************************************************************************************
 * InfantryClass::Shape_Number -- Fetch the shape number for this infantry.                    *
 *                                                                                             *
 *    This will determine the shape number to use for this infantry soldier. The shape number  *
 *    is relative to the shape file associated with this infantry unit.                        *
 *                                                                                             *
 * INPUT:   Window we will be drawing into                                                     *
 *                                                                                             *
 * OUTPUT:  Returns with the shape number for this infantry object to be used when drawing.    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *   9/4/2019 1:45PM ST : Added window parameter                                               *
 *=============================================================================================*/
int InfantryClass::Shape_Number(WindowNumberType window) const
{
	/*
	**	Fetch the shape pointer to use for the infantry. This is controlled by what
	**	choreograph sequence the infantry is performing, it's facing, and whether it
	**	is prone.
	*/
	DoType doit = Doing;
	if (doit == DO_NOTHING) doit = DO_STAND_READY;

	/*
	** Hold the walk pose for a couple of frames after we come to a stop to try and avoid the problem where a moving infantry
	** goes into the stand pose for a single frame when pausing in the assigned cell destination. ST - 9/4/2019 1:39PM
	*/
	if (doit == DO_STAND_READY) {
		if (window == WINDOW_VIRTUAL) {
			if (StopDriverFrame != -1) {
				if (Frame - StopDriverFrame <= 2) {
					if (Path[0] != FACING_NONE) {
						doit = DO_WALK;
					}
				}
			}
		}					  
	}

	/*
	** The animation frame numbers may be different when rendering in legacy mode vs. exporting for render in GlyphX. ST - 9/5/2019 12:34PM
	*/
	const DoInfoStruct *do_controls = (window == WINDOW_VIRTUAL) ? Class->DoControlsVirtual : Class->DoControls;
			
	/*
	**	The infantry shape is always modulo the number of animation frames
	**	of the action stage that the infantry is doing.
	*/
	int shapenum = Fetch_Stage() % max(do_controls[doit].Count, 1);

	/*
	**	If facing makes a difference, then the shape number will be incremented
	**	by the facing accordingly.
	*/
	if (do_controls[doit].Jump) {
		shapenum += HumanShape[Dir_To_32(PrimaryFacing.Current())] * do_controls[doit].Jump;
	}
	
	/*
	**	Finally, the shape number is biased according to the starting frame number for
	**	that action in the infantry shape file.
	*/
	shapenum += do_controls[doit].Frame;

	/*
	**	Return with the final infantry shape number.
	*/
	return(shapenum);
}


/***********************************************************************************************
 * InfantryClass::Draw_It -- Draws a unit object.                                              *
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
 *   08/15/1994 JLB : Converted to infantry support.                                           *
 *   08/14/1996 JLB : Simplified.                                                              *
 *=============================================================================================*/
void InfantryClass::Draw_It(int x, int y, WindowNumberType window) const
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	/*
	**	Verify the legality of the unit class by seeing if there is shape imagery for it. If
	**	there is no shape image, then it certainly can't be drawn -- bail.
	*/
	void const * shapefile = Get_Image_Data();

	if (shapefile == NULL) return;

	y += 4;
	x -= 2;

	/*
	**	Actually draw the root body of the unit.
	*/
	Techno_Draw_Object(shapefile, Shape_Number(window), x, y, window);

	FootClass::Draw_It(x, y, window);
}

extern bool MPSuperWeaponDisable;

/***********************************************************************************************
 * InfantryClass::Per_Cell_Process -- Handles special operations that occur once per cell.     *
 *                                                                                             *
 *    This routine will handle any special operations that need to be performed once each      *
 *    cell travelled. This includes radioing a transport that it is now clear and the          *
 *    transport is free to leave.                                                              *
 *                                                                                             *
 * INPUT:   why   -- Specifies the circumstances under which this routine was called.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *   03/01/1995 JLB : Capture building options.                                                *
 *   05/31/1995 JLB : Capture is always successful now.                                        *
 *=============================================================================================*/
void InfantryClass::Per_Cell_Process(PCPType why)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	BStart(BENCH_PCP);
	CellClass * cellptr = &Map[Coord];

	if (why == PCP_END) {

		/*
		**	If the infantry unit is entering a cell that contains the building it is trying to
		**	capture, then capture it.
		*/
		if (Mission == MISSION_CAPTURE) {
			TechnoClass * tech = cellptr->Cell_Building();

			if (tech == NULL) tech = cellptr->Cell_Techno();
			if (tech != NULL && (tech->As_Target() == NavCom || tech->As_Target() == TarCom)) {
				if (*this == INFANTRY_RENOVATOR) {

					/*
					**	An engineer will either mega-repair a friendly or allied
					**	building or it will damage/capture an enemy building. Whether
					**	it damages or captures depends on how badly damaged the
					**	enemy building is.
					*/
#ifdef FIXIT_ENGINEER_CAPTURE
					if (House->Is_Ally(tech)) {
#else
					if (tech->House->Is_Ally(House)) {
#endif
						tech->Renovate();
					} else {
						bool iscapturable = false;
						if (tech->What_Am_I() == RTTI_BUILDING) {
							iscapturable = ((BuildingClass *)tech)->Class->IsCaptureable;
						}
#ifdef FIXIT_ENGINEER	//	checked - ajw 9/28/98
						if (tech->Health_Ratio() <= EngineerCaptureLevel && iscapturable) {
#else
						if (tech->Health_Ratio() <= Rule.ConditionRed && iscapturable) {
#endif
							if (tech->Trigger.Is_Valid()) {
								tech->Trigger->Spring(TEVENT_PLAYER_ENTERED, this);
							}
							tech->House->IsThieved = true;
							tech->Captured(House);
						} else {
#ifdef FIXIT_ENGINEER	//	checked - ajw 9/28/98
							int damage = min( (short)((int)(tech->Techno_Type_Class()->MaxStrength) * EngineerDamage), tech->Strength-1);
#else
							int damage = min( (tech->Techno_Type_Class()->MaxStrength) / 3, tech->Strength-1);
#endif
							tech->Take_Damage(damage, 0, WARHEAD_HE, this, true);
						}
						BEnd(BENCH_PCP);
						delete this;
						return;
					}

				} else {
					if (*this != INFANTRY_SPY && tech->Trigger.Is_Valid()) {
						tech->Trigger->Spring(TEVENT_PLAYER_ENTERED, this);
					}

					if (*this == INFANTRY_SPY) {
						int housespy = (1 << (House->Class->House));
//						tech->House->IsSpied = true;

						if (tech->Trigger.Is_Valid()) {
							tech->Trigger->Spring(TEVENT_SPIED, this);
						}

						if (IsOwnedByPlayer) Speak(VOX_BUILDING_INFILTRATED);

						tech->Mark(MARK_OVERLAP_UP);
						tech->SpiedBy |= housespy;
						tech->Mark(MARK_OVERLAP_DOWN);
						if (tech->What_Am_I() == RTTI_BUILDING) {
							StructType build = *(BuildingClass *)tech;
							if (build == STRUCT_RADAR /* || build == STRUCT_EYE */ ) {
								tech->House->RadarSpied |= housespy;
							}
							
							if (Session.Type == GAME_NORMAL || !MPSuperWeaponDisable) {
							
								// If they're spying on a sub pen, give 'em a sonar pulse
								if (build == STRUCT_SUB_PEN) {
									House->SuperWeapon[SPC_SONAR_PULSE].Enable(true, true, false);
									// Add to Glyphx multiplayer sidebar. ST - 8/7/2019 10:13AM
									if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
										if (House->IsHuman) {
											Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_SONAR_PULSE, House);
										}
									} else {
										if (IsOwnedByPlayer) {
											Map.Add(RTTI_SPECIAL, SPC_SONAR_PULSE);
											Map.Column[1].Flag_To_Redraw();
										}
									}
								}
								// If they're spying on an airfield, they get Parabombs
								if (build == STRUCT_AIRSTRIP) {
									House->SuperWeapon[SPC_PARA_BOMB].Enable(true, true, false);
									// Add to Glyphx multiplayer sidebar. ST - 8/7/2019 10:13AM
									if (Session.Type == GAME_GLYPHX_MULTIPLAYER) {
										if (House->IsHuman) {
											Sidebar_Glyphx_Add(RTTI_SPECIAL, SPC_PARA_BOMB, House);
										}
									}
									else {
										if (IsOwnedByPlayer) {
											Map.Add(RTTI_SPECIAL, SPC_PARA_BOMB);
											Map.Column[1].Flag_To_Redraw();
										}
									}
								}
							}
						}

					} else {

						if (*this == INFANTRY_THIEF) {	// Thief just raided a storage facility
							tech->House->IsThieved = true;

							if (tech->What_Am_I() == RTTI_BUILDING) {
								BuildingClass * bldg = (BuildingClass *)tech;
								if (bldg->Class->Capacity) {

									/*
									** If we just raided a storage facility (refinery or silo)
									** then give the thief up to half the capacity of the
									** storage facility.
									*/
									if (IsOwnedByPlayer || bldg->IsOwnedByPlayer) Speak(VOX_MONEY_STOLEN);
#ifdef OBSOLETE
									long capacity = bldg->Class->Capacity * 256;
									capacity /= (bldg->House->Tiberium+1);
									int bldgcap = bldg->Class->Capacity;

									long cash = (bldgcap * 256) / (capacity+1);
									if (cash > (bldgcap / 2)) cash = bldgcap / 2;
#else
									long cash = bldg->House->Available_Money() / 2;
#endif
									bldg->House->Spend_Money(cash);
									House->Refund_Money(cash);
								}
							}
						}
					}
				}
				BEnd(BENCH_PCP);
				delete this;
				return;

			} else {

	#ifdef OBSOLETE
				// are we trying to repair a bridge?
				if (Is_Target_Cell(TarCom) ) {
					CELL cell = Coord_Cell(Coord);
					if (cell == ::As_Cell(NavCom)) {
						TemplateType tt = cellptr->TType;
						int icon = cellptr->TIcon;
						int w = TemplateTypeClass::As_Reference(cellptr->TType).Width;
						int h = TemplateTypeClass::As_Reference(cellptr->TType).Height;

						cell -= icon % w;
						cell -= MAP_CELL_W * (icon / w);
						if (tt == TEMPLATE_BRIDGE1D || tt == TEMPLATE_BRIDGE2D) {
							new TemplateClass(TemplateType(cellptr->TType-1), cell);
							Map.Zone_Reset(MZONEF_ALL);
							delete this;
							return;
						} else {

							// Trying to repair multi-segment bridge.  Look for the
							// start tile, then fix it, and determine the direction to
							// go in and repair it all that way.
							TemplateType newtt = TEMPLATE_BRIDGE_1A;
							int xmov = -1;	// coords to move to for next template
							int ymov = 2;
							bool valid = false;
							switch (tt) {
								case TEMPLATE_BRIDGE_1B:
								case TEMPLATE_BRIDGE_1C:
									valid = true;
									break;
								case TEMPLATE_BRIDGE_2B:
								case TEMPLATE_BRIDGE_2C:
									newtt = TEMPLATE_BRIDGE_2A;
									xmov = 2;
									ymov = -1;
									valid = true;
									break;
								case TEMPLATE_BRIDGE_3C:
								case TEMPLATE_BRIDGE_3D:
									newtt = TEMPLATE_BRIDGE_3A;
									valid = true;
									break;
								case TEMPLATE_BRIDGE_3E:
									newtt = TEMPLATE_BRIDGE_3A;
									xmov = 2;
									ymov = -1;
									valid = true;
									break;
							}

							// Did we find a valid repairable bridge piece?
							if (valid) {
								bool doing = true;
								while (doing) {
									new TemplateClass(TemplateType(newtt), cell);
									cell += (MAP_CELL_W * ymov) + xmov;
									if (xmov < 0) {
										xmov = -1;
										ymov = 1;
									} else {
										xmov = 1;
										ymov = -1;
									}
									cellptr = &Map[cell];
									tt = cellptr->TType;
									if ((tt >= TEMPLATE_BRIDGE_3B && tt <= TEMPLATE_BRIDGE_3F) ||
										  tt == TEMPLATE_BRIDGE_1B || tt == TEMPLATE_BRIDGE_1C ||
										  tt == TEMPLATE_BRIDGE_2B || tt == TEMPLATE_BRIDGE_2C ) {

										if (tt >= TEMPLATE_BRIDGE_3B) {
											newtt = TEMPLATE_BRIDGE_3A;
										} else {
											if (tt < TEMPLATE_BRIDGE_2A) {
												newtt = TEMPLATE_BRIDGE_1A;
											} else {
												newtt = TEMPLATE_BRIDGE_2A;
											}
										}
										icon = cellptr->TIcon;
										w = TemplateTypeClass::As_Reference(cellptr->TType).Width;
										h = TemplateTypeClass::As_Reference(cellptr->TType).Height;

										cell -= icon % w;
										cell -= MAP_CELL_W * (icon / w);
									} else {
										doing = false;
									}
								}
								Map.Zone_Reset(MZONEF_ALL);
								delete this;
								return;
							}
						}
					}
				} else {
	#endif
					if (!Target_Legal(NavCom)) {
						Enter_Idle_Mode();
						if (Map[Coord].Cell_Building()) {
							Scatter(0, true);
						}
					}
	#ifdef OBSOLETE
				}
	#endif
			}
		}

		/*
		**	Infantry entering a transport vehicle will break radio contact
		**	at attach itself to the transporter.
		*/
		TechnoClass * techno = Contact_With_Whom();
		if (Mission == MISSION_ENTER && techno != NULL && Coord_Cell(Coord) == Coord_Cell(techno->Coord) && techno == As_Techno(NavCom)) {
			if (Transmit_Message(RADIO_IM_IN) == RADIO_ATTACH) {
				Limbo();
				techno->Attach(this);
			}
			BEnd(BENCH_PCP);
			return;
		}

		/*
		**	If the infantry unit is entering a cell that contains the building it is trying to
		**	sabotage, then sabotage it.
		*/
		if (Mission == MISSION_SABOTAGE) {
			BuildingClass * building = cellptr->Cell_Building();
			if (building != NULL && building->As_Target() == NavCom) {
				if (!building->IronCurtainCountDown && building->Mission != MISSION_DECONSTRUCTION) {
					building->IsGoingToBlow = true;
					building->Clicked_As_Target(PlayerPtr->Class->House, (Rule.C4Delay * TICKS_PER_MINUTE) / 2); // 2019/09/20 JAS - Added record of who clicked on the object
					building->Clicked_As_Target(building->Owner(), (Rule.C4Delay * TICKS_PER_MINUTE) / 2);
					building->CountDown = Rule.C4Delay * TICKS_PER_MINUTE;
					building->WhomToRepay = As_Target();
				}
				NavCom = TARGET_NONE;
				Do_Uncloak();
				Arm = Rearm_Delay(true);
				Scatter(building->Center_Coord(), true, true);	// RUN AWAY!
				BEnd(BENCH_PCP);
				return;
			} else {
				if (::As_Target(Coord_Cell(Center_Coord())) == NavCom) {
					Explosion_Damage(Coord, Rule.BridgeStrength, this, WARHEAD_HE);

					Stop_Driver();
					Scatter(Adjacent_Cell(Coord, PrimaryFacing), true, true);
					Assign_Mission(MISSION_MOVE);

					CELL cell = Coord_Cell(Center_Coord());
					CellClass * cellptr = &Map[cell];
					if (!Target_Legal(NavCom) || Map[As_Cell(NavCom)].Land_Type() == LAND_WATER) {
						Mark(MARK_DOWN);		// Needed only so that Tanya will get destroyed by the explosion.
					}
					Explosion_Damage(Coord, Rule.BridgeStrength, NULL, WARHEAD_HE);
					Explosion_Damage(Coord, Rule.BridgeStrength, NULL, WARHEAD_HE);
					if (!IsActive) {
						BEnd(BENCH_PCP);
						return;
					}

					Mark(MARK_DOWN);
				}
			}
		}

		/*
		**	If this unit is on a teather, then cut it at this time so that
		**	the "parent" unit is free to proceed. Note that the parent
		**	unit might actually be a building.
		*/
		if (IsTethered) {
			Transmit_Message(RADIO_UNLOADED);
			if (House->ActLike == HOUSE_USSR || House->ActLike == HOUSE_UKRAINE) {
				Do_Action(DO_GESTURE1);
			} else  {
				Do_Action(DO_GESTURE2);
			}

			/*
			**	Special voice play.
			*/
			if (*this ==  INFANTRY_TANYA) {
				Sound_Effect(VOC_TANYA_LAUGH, Coord);
			}

			/*
			**	If the cell is now full of infantry, tell them all to scatter
			**	in order to make room for more.
			*/
			if ((cellptr->Flag.Composite & 0x01F) == 0x01F) {
				cellptr->Incoming(0, true, true);
//				cellptr->Incoming(0, true);
			}
		}

		/*
		**	When the infantry reaches the center of the cell, it may begin a new mission.
		*/
		if (MissionQueue == MISSION_NONE && !Target_Legal(NavCom) && !Target_Legal(TarCom) && !In_Radio_Contact()) {
			Enter_Idle_Mode();
		}
		Commence();

		/*
		** If entering a cell with a land mine in it, blow up the mine.
		*/
		BuildingClass * bldng = cellptr->Cell_Building();
		if (bldng != NULL && *bldng == STRUCT_APMINE) {
			/*
			** Show the animation and get rid of the land mine
			*/
			COORDINATE blcoord = bldng->Center_Coord();
			new AnimClass(Combat_Anim(Rule.APMineDamage, WARHEAD_HE, cellptr->Land_Type()), blcoord);
			delete bldng;
			int damage;
			for (int index = 0; index < Infantry.Count(); index++) {
				InfantryClass * obj = Infantry.Ptr(index);
				if (obj != NULL && !obj->IsInLimbo) {
					int dist = ::Distance(obj->Coord, blcoord);
					if (dist <= 0xC0) {
						damage = Rule.APMineDamage;
						obj->Take_Damage(damage, 0, WARHEAD_HE);
					}
				}
			}
			if (!IsActive) {
				BEnd(BENCH_PCP);
				return;
			}
		}

		/*
		**	If the last cell we looked from isn't adjacent to our current cell,
		**	perform a full look.
		*/
		CELL cell = Coord_Cell(Coord);
		if (::Distance(Cell_X(cell), Cell_Y(cell), Cell_X(LookCell), Cell_Y(LookCell)) > 1) {
			Look(false);
		} else {
			Look(true);
		}

#if 1
/*
**	If after all is said and done, the unit finishes its move on an impassable cell, then
**	it must presume that it is in the case of a unit driving onto a bridge that blows up
**	before the unit completes it's move. In such a case the unit should have been destroyed
**	anyway, so blow it up now.
*/
LandType land = Map[Coord].Land_Type();
if (!IsDriving && !Class->IsBomber && (land == LAND_ROCK || land == LAND_WATER || land == LAND_RIVER)) {
	int damage = Strength;
	Take_Damage(damage, 0, WARHEAD_AP, NULL, true);
	return;
}
#endif

	}

	if (IsActive) {
		FootClass::Per_Cell_Process(why);
	}
	BEnd(BENCH_PCP);
}


/***********************************************************************************************
 * InfantryClass::Detach -- Removes the specified target from targeting computer.              *
 *                                                                                             *
 *    This is a support routine that removes the target specified from any targeting or        *
 *    navigation computers. When a target is destroyed or removed from the game system,        *
 *    the target must be removed from any tracking systems of the other units. This routine    *
 *    handles removal for infantry units.                                                      *
 *                                                                                             *
 * INPUT:   target   -- The target to remove from the infantry unit's tracking systems.        *
 *                                                                                             *
 *          all      -- Is the target going away for good as opposed to just cloaking/hiding?  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Detach(TARGET target, bool all)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	if (TarCom == target) {
		Mark(MARK_OVERLAP_UP);
		IsFiring = false;
		Mark(MARK_OVERLAP_DOWN);
	}
	FootClass::Detach(target, all);
}


/***********************************************************************************************
 * InfantryClass::Init -- Initialize the infantry object system.                               *
 *                                                                                             *
 *    This routine will force the infantry object system into its empty initial state. It      *
 *    is called when the scenario needs to be cleared in preparation for a scenario load.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Init(void)
{
	Infantry.Free_All();
}


/***********************************************************************************************
 * InfantryClass::Assign_Destination -- Gives the infantry a movement destination.             *
 *                                                                                             *
 *    This routine updates the infantry's navigation computer so that the infantry will        *
 *    travel to the destination target specified.                                              *
 *                                                                                             *
 * INPUT:   target   -- The target to have the infantry unit move to.                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Assign_Destination(TARGET target)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	/*
	**	Special flag so that infantry will start heading in the right direction immediately.
	*/
	if (IsDriving && !IsFormationMove && Target_Legal(target) && Map[Center_Coord()].Is_Clear_To_Move(Class->Speed, true, false)) {
		Stop_Driver();
	}

	/*
	**	When telling an infantry soldier to move to a location twice, then this
	**	means that movement is more important than safety. Get up and run!
	*/
	if (House->IsHuman && Target_Legal(target) && NavCom == target && IsProne && !Class->IsFraidyCat && !Class->IsDog) {
		Do_Action(DO_GET_UP);
	}

	/*
	** If telling a dog to attack a human, start the dog running
	*/
	TechnoClass * tech = As_Techno(target);

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
		} else {
			Path[0] = FACING_NONE;
		}
	} else {
		Path[0] = FACING_NONE;
	}
	FootClass::Assign_Destination(target);
}


/***********************************************************************************************
 * InfantryClass::Assign_Target -- Gives the infantry a combat target.                         *
 *                                                                                             *
 *    This routine will update the infantry's targeting computer so that it will try to        *
 *    attack the target specified. This might result in it moving to be within range and thus  *
 *    also cause adjustment of the navigation computer.                                        *
 *                                                                                             *
 * INPUT:   target   -- The target that this infantry should attack.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *   06/30/1995 JLB : Tries to capture target if possible.                                     *
 *=============================================================================================*/
void InfantryClass::Assign_Target(TARGET target)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	Path[0] = FACING_NONE;
	if (Class->IsDog) {
		if (::As_Object(target) && ::As_Object(target)->What_Am_I() != RTTI_INFANTRY) {
			target = TARGET_NONE;
		}
	}
	FootClass::Assign_Target(target);

	/*
	**	If this is an infantry that can only capture, then also assign its destination to the
	**	target specified.
	*/
	if (!Target_Legal(NavCom) && Class->IsCapture && !Is_Weapon_Equipped()) {
		BuildingClass const * building = As_Building(target);
		if (building != NULL && building->Class->IsCaptureable) {
			Assign_Destination(target);
		}
	}
}


/***********************************************************************************************
 * InfantryClass::AI -- Handles the infantry non-graphic related AI processing.                *
 *                                                                                             *
 *    This routine is used to handle the non-graphic AI processing the infantry requires.      *
 *    Call this routine ONCE per game frame.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *   08/14/1996 JLB : Simplified.                                                              *
 *=============================================================================================*/
void InfantryClass::AI(void)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	FootClass::AI();

	if (!IsActive) {
		return;
	}

	if (IsUnloading) Mark(MARK_CHANGE_REDRAW);

	/*
	**	Infantry that are not on the ground should always be redrawn. Such is
	**	the case when they are parachuting to the ground.
	*/
	if (In_Which_Layer() != LAYER_GROUND) {
		Mark(MARK_CHANGE);
	}

	/*
	**	Special hack to make sure that if this infantry is in firing animation, but the
	**	stage class isn't set, then abort the firing flag.
	*/
	if (IsFiring && Fetch_Rate() == 0) {
		Mark(MARK_OVERLAP_UP);
		IsFiring = false;
		Do_Action(DO_STAND_READY);
		Mark(MARK_OVERLAP_DOWN);
	}

	/*
	**	Delete this unit if it finds itself off the edge of the map and it is in
	**	guard or other static mission mode.
	*/
	if (Edge_Of_World_AI()) {
		return;
	}

	/*
	**	Act on new orders if the unit is at a good position to do so.
	*/
	if (!IsFiring && !IsFalling && !IsDriving && (Doing == DO_NOTHING || MasterDoControls[Doing].Interrupt)) {
		if (Mission == MISSION_NONE && MissionQueue == MISSION_NONE) Enter_Idle_Mode();
		Commence();
	}

	/*
	**	Special hack to make sure the dog never attacks a cell.
	*/
	if (Class->IsDog && Target_Legal(TarCom) && Is_Target_Cell(TarCom)) {
		Assign_Target(TARGET_NONE);
	}

	/*
	**	Handle any infantry fear logic or related actions.
	*/
	Fear_AI();

	/*
	**	Special victory dance action.
	*/
	if (!Target_Legal(NavCom) && !IsProne && IsStoked && Comment == 0) {
		IsStoked = false;
		Do_Action(Percent_Chance(50) ? DO_GESTURE1 : DO_GESTURE2);
	}

	/*
	**	Determine if this infantry unit should fire off an
	**	attack or not.
	*/
	Firing_AI();

	/*
	**	Handle the completion of the animation sequence.
	*/
	Doing_AI();

	/*
	**	Perform movement operations at this time.
	*/
	Movement_AI();
}


/***********************************************************************************************
 * InfantryClass::Can_Enter_Cell -- Determines if the infantry can enter the cell specified.   *
 *                                                                                             *
 *    This routine is used to examine the cell specified and determine if the infantry is      *
 *    allowed to enter it. It is used by the path finding algorithm.                           *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine.                                                      *
 *                                                                                             *
 * OUTPUT:  Returns the type of blockage in the cell.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
MoveType InfantryClass::Can_Enter_Cell(CELL cell, FacingType ) const
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	/*
	** If we are moving into an illegal cell, then we can't do that.
	*/
	if ((unsigned)cell >= MAP_CELL_TOTAL) return(MOVE_NO);

	/*
	**	If moving off the edge of the map, then consider that an illegal move.
	*/
	if (!ScenarioInit && !Map.In_Radar(cell) && !Is_Allowed_To_Leave_Map()) {
		return(MOVE_NO);
	}

	CellClass * cellptr = &Map[cell];

	/*
	**	Walls are considered impassable for infantry UNLESS the wall has a hole
	**	in it.
	*/
	if (cellptr->Overlay != OVERLAY_NONE) {
		OverlayTypeClass const & otype = OverlayTypeClass::As_Reference(cellptr->Overlay);

		if (otype.IsCrate && !((Session.Type == GAME_NORMAL) ? House->IsPlayerControl : House->IsHuman) && Session.Type == GAME_NORMAL) {
			return(MOVE_NO);
		}

		if (otype.IsWall) {
			if ((cellptr->OverlayData / 16) != otype.DamageLevels) {

				/*
				**	If the wall can be destroyed, then return this fact instead of
				**	a complete failure to enter.
				*/
				if (Is_Weapon_Equipped() && Class->PrimaryWeapon->Is_Wall_Destroyer()) {
					return(MOVE_DESTROYABLE);
				}
				return(MOVE_NO);
			}
		}
	}

	/*
	** Loop through all of the objects in the square setting a bit
	** for how they affect movement.
	*/
	MoveType retval = MOVE_OK;
	ObjectClass * obj = cellptr->Cell_Occupier();
	while (obj != NULL) {

		if (obj != this) {

			/*
			**	Always allow movement if the cell is the object to be captured or sabotaged.
			*/
			if (((Mission == MISSION_ENTER && In_Radio_Contact()) || Mission == MISSION_CAPTURE || Mission == MISSION_SABOTAGE) &&
				(obj->As_Target() == NavCom || obj->As_Target() == TarCom)) {

				return(MOVE_OK);
			}

			/*
			**	Guard area should not allow the guarding unit to enter the cell with the
			**	guarded unit.
			*/
			if (Mission == MISSION_GUARD_AREA && ArchiveTarget == obj->As_Target() && Is_Target_Unit(ArchiveTarget)) {
				return(MOVE_NO);
			}

			/*
			** If object is a land mine, allow movement
			*/
			if (obj->What_Am_I() == RTTI_BUILDING) {
				if ((*(BuildingClass *)obj) == STRUCT_AVMINE) {
					obj = obj->Next;
					continue;
				} else {
					if (!Rule.IsMineAware || !((BuildingClass *)obj)->House->Is_Ally(House)) {
						if ((*(BuildingClass *)obj) == STRUCT_APMINE) {
							obj = obj->Next;
							continue;
						}
					}
				}
			}

			/*
			**	Special case check so that a landed aircraft that is in radio contact, will not block
			**	a capture attempt. It is presumed that this case happens when a helicopter is landed
			**	at a helipad.
			*/
//			if ((Mission != MISSION_CAPTURE && Mission != MISSION_SABOTAGE) || obj->What_Am_I() != RTTI_AIRCRAFT || !((AircraftClass *)obj)->In_Radio_Contact()) {

				/*
				**	Special check to always allow entry into the building that this infantry
				**	is trying to capture.
				*/
//				if (obj->What_Am_I() == RTTI_BUILDING || obj->What_Am_I() == RTTI_AIRCRAFT || obj->What_Am_I() == RTTI_UNIT) {
//					if ((Mission == MISSION_CAPTURE || Mission == MISSION_SABOTAGE) && (obj->As_Target() == NavCom || obj->As_Target() == TarCom)) {
//						return(MOVE_OK);
//					}
//				}

				/*
				**	Special check to always allow entry into the building that this infantry
				**	is trying to capture.
				*/
				if (Mission == MISSION_ENTER && obj->As_Target() == NavCom && IsTethered) {
					return(MOVE_OK);
				}

				/*
				**	Allied objects block movement using different rules than for enemy
				**	objects.
				*/
				if (House->Is_Ally(obj) || ScenarioInit) {
					switch (obj->What_Am_I()) {

						/*
						**	A unit blocks as either a moving blockage or a stationary temp blockage.
						**	This depends on whether the unit is currently moving or not.
						*/
						case RTTI_UNIT:
							if (((UnitClass *)obj)->IsDriving || Target_Legal(((UnitClass *)obj)->NavCom)) {
								if (retval < MOVE_MOVING_BLOCK) retval = MOVE_MOVING_BLOCK;
							} else {
								if (retval < MOVE_TEMP) retval = MOVE_TEMP;
							}
							break;

						/*
						**	Aircraft and buildings always block movement. If for some reason there is an
						**	allied terrain object, that blocks movement as well.
						*/
						case RTTI_TERRAIN:
						case RTTI_AIRCRAFT:
						case RTTI_BUILDING:
							return(MOVE_NO);

						default:
							break;
					}

				} else {

					/*
					**	Cloaked enemy objects are not considered if this is a Find_Path()
					**	call.
					*/
					if (!obj->Is_Techno() || ((TechnoClass *)obj)->Cloak != CLOAKED) {

						/*
						**	Any non-allied blockage is considered impassible if the infantry
						**	is not equipped with a weapon.
						*/
						if (Combat_Damage() <= 0) return(MOVE_NO);

						/*
						**	Some kinds of terrain are considered destroyable if the infantry is equipped
						**	with the weapon that can destroy it. Otherwise, the terrain is considered
						**	impassable.
						*/
						switch (obj->What_Am_I()) {
							case RTTI_TERRAIN:
#ifdef OBSOLETE
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
							case RTTI_INFANTRY:
								if ( *(InfantryClass *)obj == INFANTRY_SPY && !Class->IsDog) {
									retval = MOVE_TEMP;
									break;
								}
								// otherwise, fall thru.
							default:
								if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE;
								break;
						}
					} else {
						if (retval < MOVE_CLOAK) retval = MOVE_CLOAK;
					}
				}
//			}
		}

		/*
		**	Move to next object in chain.
		*/
		obj = obj->Next;
	}

	/*
	**	If foot soldiers cannot travel on the cell -- consider it impassable.
	*/
	if (retval == MOVE_OK && !IsTethered && Ground[cellptr->Land_Type()].Cost[SPEED_FOOT] == 0) {

#ifdef OBSOLETE
		/*
		** Special case - if it's an engineer, and the cell under consideration
		** is his NavCom, and his mission is mission_capture, then he's most
		** likely moving to his final destination to repair a bridge, so we
		** should let him.
		*/
		if (*this == INFANTRY_RENOVATOR && Is_Target_Cell(TarCom) && (cell == ::As_Cell(NavCom)) && (cellptr->TType == TEMPLATE_BRIDGE1D || cellptr->TType == TEMPLATE_BRIDGE2D || (cellptr->TType >= TEMPLATE_BRIDGE_1C && cellptr->TType <= TEMPLATE_BRIDGE_3E) ) ) {
			return(MOVE_OK);
		}
#endif
		return(MOVE_NO);
	}

	/*
	** if a unit has the cell reserved then we just can't go in there.
	*/
	if (retval == MOVE_OK && cellptr->Flag.Occupy.Vehicle) {
		return(MOVE_NO);
	}

	/*
	** if a block of infantry has the cell reserved then there are two
	** possibilities...
	*/
	if (cellptr->InfType != HOUSE_NONE) {
		if (House->Is_Ally(cellptr->InfType)) {
			if ((cellptr->Flag.Composite & 0x1F) == 0x1f) {
				if (retval < MOVE_MOVING_BLOCK) retval = MOVE_MOVING_BLOCK;
			}
		} else {
			if (Combat_Damage() > 0) {
				if (retval < MOVE_DESTROYABLE) {
					retval = MOVE_DESTROYABLE;
				}
			} else {
				return(MOVE_NO);
			}
		}
	}

	/*
	**	If it is still ok to move the infantry, then perform the last check
	**	to see if the cell is already full of infantry.
	*/
	if (retval == MOVE_OK && (cellptr->Flag.Composite & 0x1F) == 0x1F) {
		return(MOVE_NO);
	}

	/*
	**	Return with the most severe reason why this cell would be impassable.
	*/
	return(retval);
}


/***********************************************************************************************
 * InfantryClass::Overlap_List -- The list of cells that the infantry overlaps, but doesn't occ*
 *                                                                                             *
 *    This is a rendering support routine that will return a pointer to a list of cell offsets *
 *    that specify the cells the infantry unit is currently overlapping (graphic wise) but     *
 *    is not considered to occupy. This list is used to update the map display.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to an offset list for cells that the unit overlaps but doesn't   *
 *          occupy.                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef PARTIAL
short const * InfantryClass::Overlap_List(bool redraw) const
#else
short const * InfantryClass::Overlap_List(bool ) const
#endif
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	if (Class->IsDog) {
		return(Coord_Spillage_List(Coord, 24 + (Doing == DO_DOG_MAUL ? 40 : 0) + (Doing >= DO_GUN_DEATH && Doing <= DO_FIRE_DEATH ? 40 : 0) ));
	} else {

		/*
		**	The default infantry rectangle will be as large as the largest shape the infantry
		**	can be.
		*/

#ifdef PARTIAL
		Rect rect(-16, -24, 32, 36);

		/*
		**	If this is for a visual change redraw, then the overlap list will be based
		**	on the actual dimensions of the shape data. If the dimensions have already
		**	been calculated then use them, otherwise, use the default large rectangle
		**	previously created.
		*/
		if (Height == 0 && !Is_Selected_By_Player() && redraw && Class->DimensionData != NULL) {
			int shapenum = Shape_Number();
			if (!Class->DimensionData[shapenum].Is_Valid()) {
				Class->DimensionData[shapenum] = Shape_Dimensions(Get_Image_Data(), shapenum);
			}
			rect = Class->DimensionData[shapenum];
			rect.Y += 4;
			rect.X -= 2;
		}
		return(Coord_Spillage_List(Coord, rect, true));
#else

		static Rect rect(-16, -24, 32, 36);
		return(Coord_Spillage_List(Coord, rect, true));
#endif

//		return(Coord_Spillage_List(Coord, 24 /*+ ((Doing > DO_WALK || IsSelected)?12:0)*/ ));
	}
}


/***********************************************************************************************
 * InfantryClass::Can_Fire -- Can the infantry fire its weapon?                                *
 *                                                                                             *
 *    Determines if the infantry unit can fire on the target. If it can't fire, then the       *
 *    reason why is returned.                                                                  *
 *                                                                                             *
 * INPUT:   target   -- The target to determine if the infantry can fire upon.                 *
 *                                                                                             *
 * OUTPUT:  Returns the fire error type that indicates if the infantry can fire and if it      *
 *          can't, why not.                                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *   06/27/1995 JLB : Flame thrower can fire while prone now.                                  *
 *=============================================================================================*/
FireErrorType InfantryClass::Can_Fire(TARGET target, int which) const
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	/*
	**	Don't allow firing if the infantry is still firing on previous target.
	*/
//	if (IsFiring) return(FIRE_REARM);

	/*
	** If a medic is shooting at a healed target, let's declare the target
	** illegal so he won't be constantly healing healed infantrymen.
	*/
	if (Combat_Damage() < 0) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		TechnoClass * targ = As_Techno(target);
#else
		InfantryClass * targ = As_Infantry(target);
#endif
		if (targ == NULL || targ->Health_Ratio() >= Rule.ConditionGreen) {
			return(FIRE_ILLEGAL);
		}
	}

	/*
	**	If this unit cannot fire while moving, then bail.
	*/
	if (IsDriving || (Target_Legal(NavCom) && Doing != DO_NOTHING && !MasterDoControls[Doing].Interrupt)) {
		return(FIRE_MOVING);
	}

	/*
	** Only one dog can fire on an infantry at a time
	*/
	if (Class->IsDog) {
		for (int index = 0; index < Infantry.Count(); index++) {
			InfantryClass *dog = Infantry.Ptr(index);
			if (dog != this && dog->Class->IsDog && (dog->IsFiring || dog->IsInLimbo) && dog->TarCom == target) {
				return(FIRE_ILLEGAL);
			}
		}
	}

	return(FootClass::Can_Fire(target, which));
}


/***********************************************************************************************
 * TechnoClass::Fire_Coord -- Determine the coordinate where bullets appear.                   *
 *                                                                                             *
 *    This routine will determine the coordinate to use when this infantry fires. The          *
 *    coordinate is the location where bullets appear (or fire effects appear) when the        *
 *    object fires its weapon.                                                                 *
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
 *   11/12/2019 SKY : Created.                                                                 *
 *=============================================================================================*/
COORDINATE InfantryClass::Fire_Coord(int which) const
{
	COORDINATE coord = FootClass::Fire_Coord(which);

	/*
	**  Since electric weapons draw a zap between start and end points, prone infantry that fire one
	**  need to adjust their fire coordinate so the start point looks correct.
	*/
	if (IsProne) {
		TechnoTypeClass const & tclass = *Techno_Type_Class();
		WeaponTypeClass const * weapon = (which == 0) ? tclass.PrimaryWeapon : tclass.SecondaryWeapon;
		if (weapon && weapon->IsElectric) {
			coord = Coord_Add(coord, XY_Coord(0, 48));
		}
	}

	return coord;
}


/***********************************************************************************************
 * InfantryClass::Enter_Idle_Mode -- The infantry unit enters idle mode by this routine.       *
 *                                                                                             *
 *    Use this routine when the infantry unit as accomplished its task and needs to find       *
 *    something to do. The default behavior is to enter some idle state such as guarding.      *
 *                                                                                             *
 * INPUT:   initial  -- Is this called when the unit just leaves a factory or is initially     *
 *                      or is initially placed on the map?                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Enter_Idle_Mode(bool )
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	MissionType order = MISSION_GUARD;

	if (Target_Legal(TarCom)) {
		order = MISSION_ATTACK;
		if (Mission == MISSION_SABOTAGE) {
			order = MISSION_SABOTAGE;
		}
		if (Mission == MISSION_CAPTURE) {
			order = MISSION_CAPTURE;
		}
	} else {

		Handle_Navigation_List();

		if (Target_Legal(NavCom)) {
			order = MISSION_MOVE;
			if (Mission == MISSION_CAPTURE) {
				order = MISSION_CAPTURE;
			}
			if (Mission == MISSION_SABOTAGE) {
				order = MISSION_SABOTAGE;
			}
		} else {

			if (Mission == MISSION_GUARD || Mission == MISSION_GUARD_AREA || MissionControl[Mission].IsZombie || MissionControl[Mission].IsParalyzed) {
				return;
			}

			if (Class->IsDog) {
				if (House->IsHuman || Team.Is_Valid()) {
					order = MISSION_GUARD;
				} else {
					order = MISSION_GUARD_AREA;
					ArchiveTarget = ::As_Target(Coord_Cell(Center_Coord()));
				}
			} else {
				if (House->IsHuman || Team.Is_Valid()) {
					order = MISSION_GUARD;
				} else {
					if (House->IQ < Rule.IQGuardArea) {
						order = MISSION_GUARD;
					} else {
						if (Is_Weapon_Equipped()) {
							order = MISSION_GUARD_AREA;
						} else {
							order = MISSION_GUARD;
						}
					}
				}
			}
		}
	}
	Assign_Mission(order);
}


/***********************************************************************************************
 * InfantryClass::Random_Animate -- Randomly animate the infantry (maybe)                      *
 *                                                                                             *
 *    This routine is the random animator initiator for infantry units. This routine should    *
 *    be called regularly. On occasion, it will cause the infantry to go into an idle          *
 *    animation.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *   12/13/1994 JLB : Does random facing change.                                               *
 *   07/02/1995 JLB : Nikoomba special effects.                                                *
 *=============================================================================================*/
bool InfantryClass::Random_Animate(void)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	if (Is_Ready_To_Random_Animate()) {
		IdleTimer = Random_Pick(Rule.RandomAnimateTime * (TICKS_PER_MINUTE/2), Rule.RandomAnimateTime * (TICKS_PER_MINUTE*2));

		/*
		**	Scared infantry will always follow the golden rule of civilians;
		**		"When in darkness or in doubt, run in circles, scream, and shout!"
		*/
		if (Class->IsFraidyCat && !House->IsHuman && Fear > FEAR_ANXIOUS) {
			Scatter(NULL, true);
			return(true);
		}

		switch (Random_Pick(0, 10)) {
			case 0:
				if (Class->IsDog) {
					Do_Action(DO_IDLE1);
				}
				break;

			case 1:
				Do_Action(DO_SALUTE1);
				break;

			case 2:
				Do_Action(DO_SALUTE2);
				break;

			case 3:
				Do_Action(DO_GESTURE1);
				break;

			case 4:
				Do_Action(DO_GESTURE2);
				break;

			case 5:
				Do_Action(DO_IDLE1);
				break;

			case 6:
				Mark(MARK_CHANGE_REDRAW);
				PrimaryFacing.Set(Facing_Dir(Random_Pick(FACING_N, FACING_NW)));
				Mark(MARK_CHANGE_REDRAW);
				break;

			case 7:
				Do_Action(DO_IDLE2);
				Mark(MARK_CHANGE_REDRAW);
				PrimaryFacing.Set(Facing_Dir(Random_Pick(FACING_N, FACING_NW)));
				Mark(MARK_CHANGE_REDRAW);
				if (!Is_Selected_By_Player() && IsOwnedByPlayer && *this == INFANTRY_TANYA && Sim_Random_Pick(0, 2) == 0) {
					Sound_Effect(VOC_TANYA_SHAKE, Coord);
				}
				break;

			/*
			**	On occasion, civilian types will wander about.
			*/
			case 8:
				Mark(MARK_CHANGE_REDRAW);
				PrimaryFacing.Set(Facing_Dir(Random_Pick(FACING_N, FACING_NW)));
				Mark(MARK_CHANGE_REDRAW);
				if (!House->IsHuman && Class->IsFraidyCat) {
					Scatter(NULL, true);
				}
				break;

			case 9:
			case 10:
				Mark(MARK_CHANGE_REDRAW);
				PrimaryFacing.Set(Facing_Dir(Random_Pick(FACING_N, FACING_NW)));
				Mark(MARK_CHANGE_REDRAW);

		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * InfantryClass::Scatter -- Causes the infantry to scatter to nearby cell.                    *
 *                                                                                             *
 *    This routine is used when the infantry should scatter to a nearby cell. Scattering       *
 *    occurs as an occasional consequence of being fired upon. It is one of the features       *
 *    that makes infantry so "charming".                                                       *
 *                                                                                             *
 * INPUT:   threat   -- The coordinate source of the threat that is causing the infantry to    *
 *                      scatter. If the threat isn't from a particular direction, then this    *
 *                      parameter will be NULL.                                                *
 *                                                                                             *
 *          forced   -- The threat is real and a serious effort to scatter should be made.     *
 *                                                                                             *
 *          nokidding-- The scatter should affect the player's infantry even if it otherwise   *
 *                      wouldn't have.                                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *   12/12/1994 JLB : Flame thrower infantry always scatter.                                   *
 *   08/02/1996 JLB : Added the nokidding parameter                                            *
 *=============================================================================================*/
void InfantryClass::Scatter(COORDINATE threat, bool forced, bool nokidding)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	/*
	**	A unit that is in the process of going somewhere will never scatter.
	*/
	if (IsDriving) forced = false;

	/*
	**	Certain missions prevent scattering regardless of whether it would be
	**	a good idea or not.
	*/
	if (!MissionControl[Mission].IsScatter && !forced) return;

	/*
	**	If the infantry is currently engaged in legitimate combat, then don't
	**	scatter unless forced to.
	*/
	if (!Class->IsFraidyCat && Target_Legal(TarCom) && !forced) return;

	/*
	**	Don't scatter if performing an action that can't be interrupted.
	*/
	if (Doing != DO_NOTHING && !MasterDoControls[Doing].Interrupt) return;

	/*
	**	For human players, don't scatter the infantry, if the special
	**	flag has not been enabled that allows infantry scatter.
	*/
	if (!Rule.IsScatter && !nokidding && House->IsHuman && !forced && !Team.Is_Valid()) return;

	if (forced || Class->IsFraidyCat /*|| !(Random_Pick(1, 4) == 1)*/) {
		FacingType	toface;

		if (threat) {
			toface = Dir_Facing(Direction8(threat, Coord));
			toface = toface + FacingType(Random_Pick(0, 4)-2);
		} else {
			COORDINATE coord = Coord_Fraction(Center_Coord());

			if (coord != 0x00800080L) {
				toface = Dir_Facing((DirType)Desired_Facing8(0x0080, 0x0080, Coord_X(coord), Coord_Y(coord)));
			} else {
				toface = Dir_Facing(PrimaryFacing.Current());
			}
			toface = toface + FacingType(Random_Pick(0, 4)-2);
		}

		CELL newcell = 0;
		CELL altcell = 0;
		FacingType face;
		for (face = FACING_N; face < FACING_COUNT; face++) {
			FacingType newface = toface + face;
			newcell = Adjacent_Cell(Coord_Cell(Coord), newface);

			if (Map.In_Radar(newcell) && Can_Enter_Cell(newcell) == MOVE_OK) {
				if (altcell == 0) altcell = newcell;
				if (!Map[newcell].Is_Bridge_Here()) break;
//				Assign_Mission(MISSION_MOVE);
//				Assign_Destination(::As_Target(newcell));
			}
		}
		if (face == FACING_COUNT) {
			newcell = 0;
		}

		if (newcell == 0) {
			newcell = altcell;
		}

		if (newcell != 0) {
			Assign_Mission(MISSION_MOVE);
			Assign_Destination(::As_Target(newcell));
		}
	}
}


/***********************************************************************************************
 * InfantryClass::Look -- Performs a look around (map reveal) action.                          *
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
void InfantryClass::Look(bool incremental)
{
	LookCell = Coord_Cell(Coord);
	FootClass::Look(incremental);
}


/***********************************************************************************************
 * InfantryClass::Do_Action -- Launches the infantry into an animation sequence.               *
 *                                                                                             *
 *    This starts the infantry into a choreographed animation sequence. These sequences can    *
 *    be as simple as standing up or lying down, but can also be complex, such as dying or     *
 *    performing some idle animation.                                                          *
 *                                                                                             *
 * INPUT:   todo     -- The choreographed sequence to start.                                   *
 *                                                                                             *
 *          force    -- Force starting this animation even if the current animation is flagged *
 *                      as uninterruptible. This is necessary for death animations.            *
 *                                                                                             *
 * OUTPUT:  bool; Was the animation started?                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Do_Action(DoType todo, bool force)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	if (todo == DO_NOTHING || Class->DoControls[todo].Count == 0) {
		return(false);
	}

	if (*this == INFANTRY_SPY && todo >= DO_GESTURE1) {
		todo = (DoType)(DO_IDLE1 + Random_Pick(0,1));
	}

	if (todo != Doing && (Doing == DO_NOTHING || force || MasterDoControls[Doing].Interrupt)) {
		Mark(MARK_OVERLAP_UP);
		Doing = todo;
		Mark(MARK_OVERLAP_DOWN);
		if (todo == DO_IDLE1 || todo == DO_IDLE2) {
			Set_Rate(Options.Normalize_Delay(MasterDoControls[Doing].Rate));
		} else {
			Set_Rate(MasterDoControls[Doing].Rate);
		}
		Set_Stage(0);

		/*
		**	Kludge to make sure that if infantry is in the dying animation, it isn't still
		**	moving as well.
		*/
		if (Strength == 0) {
			Stop_Driver();
		}

		/*
		**	Make sure dogs don't try to go somewhere while they're mauling
		*/
		if (todo == DO_DOG_MAUL) {
			Stop_Driver();
			Assign_Destination(TARGET_NONE);
		}

		/*
		**	Since the animation sequence might be interrupted. Set any flags
		**	necessary so that if interrupted, the affect on the infantry is
		**	still accomplished.
		*/
		switch (todo) {
			case DO_LIE_DOWN:
				IsProne = true;
				break;

			case DO_GET_UP:
				IsProne = false;
				break;

			default:
				break;
		}

		return(true);
	}

	return(false);
}


/***********************************************************************************************
 * InfantryClass::Stop_Driver -- Stops the infantry from moving any further.                   *
 *                                                                                             *
 *    This is used to stop the infantry from animating in movement. This function will stop    *
 *    the infantry moving and revert it to either a prone or standing.                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the driving stopped?                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Stop_Driver(void)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	if (Head_To_Coord()) {

		/*
		**	Remove the "reservation" bit in the destination location.
		*/
		Clear_Occupy_Bit(Head_To_Coord());
	}

	/*
	**	Set the occupation bit at the current location.
	*/
	Set_Occupy_Bit(Coord);

	if (Doing != DO_STAND_READY) {
		StopDriverFrame = Frame;
	}

	if (Class->IsDog) {
		Do_Action(DO_STAND_READY);
	} else {
		if (IsProne) {
			Do_Action(DO_PRONE);
		} else {
			Do_Action(DO_STAND_READY);
		}
	}

	if (Can_Enter_Cell(Coord_Cell(Coord)) == MOVE_OK) {
		IsZoneCheat = false;
	} else {
		IsZoneCheat = true;
	}

	return(FootClass::Stop_Driver());
}


/***********************************************************************************************
 * InfantryClass::Start_Driver -- Handles giving immediate destination and move orders.        *
 *                                                                                             *
 *    Use this routine to being the infantry moving toward the destination specified. The      *
 *    destination is first checked to see if there is a free spot available. Then the infantry *
 *    reserves that spot and begins movement toward it.                                        *
 *                                                                                             *
 * INPUT:   headto   -- The coordinate location desired for the infantry to head to.           *
 *                                                                                             *
 * OUTPUT:  bool; Was the infantry successfully started on its journey? Failure may be because *
 *                the specified destination could not contain the infantry unit.               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/21/1994 JLB : Created.                                                                 *
 *   05/14/1995 JLB : Tries to move to closest spot possible.                                  *
 *   05/15/1995 JLB : Uses closest spot if moving onto transport.                              *
 *=============================================================================================*/
bool InfantryClass::Start_Driver(COORDINATE & headto)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	COORDINATE old = headto;

	/*
	**	Convert the head to coordinate to a legal sub-position location.
	*/
	headto = Map[headto].Closest_Free_Spot(Coord_Move(headto, Direction(headto)+DIR_S, 0x007C));
	if (!headto && Can_Enter_Cell(Coord_Cell(old)) == MOVE_OK) {
		headto = Map[old].Closest_Free_Spot(Coord_Move(old, Direction(headto)+DIR_S, 0x0080), true);
	}

	/*
	**	If the infantry started moving, then fixup the occupation bits.
	*/
	if (headto && FootClass::Start_Driver(headto)) {
		if (!IsActive) return(false);

		/*
		**	Remove the occupation bit from the infantry's current location.
		*/
  		Clear_Occupy_Bit(Coord);

		/*
		**	Set the occupation bit for the new headto location.
		*/
		Set_Occupy_Bit(headto);
		return(true);
	}

	return(false);
}


/***********************************************************************************************
 * InfantryClass::Limbo -- Performs cleanup operations needed when limboing.                   *
 *                                                                                             *
 *    This routine will clean up the infantry occupation bits (as necessary) as well as stop   *
 *    the infantry movement process when it gets limboed.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the infantry unit limboed?                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Limbo(void)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	if (!IsInLimbo) {
		Stop_Driver();

		Clear_Occupy_Bit(Coord);
	}
	return(FootClass::Limbo());
}


/***********************************************************************************************
 * InfantryClass::Fire_At -- Fires projectile from infantry unit.                              *
 *                                                                                             *
 *    Use this routine when the infantry unit wishes to fire a projectile. This routine        *
 *    will launch the projectile and perform any other necessary infantry specific operations. *
 *                                                                                             *
 * INPUT:   target   -- The target of the attack.                                              *
 *                                                                                             *
 *          which    -- Which weapon to use for firing. 0=primary, 1=secondary.                *
 *                                                                                             *
 * OUTPUT:  Returns with pointer to the projectile launched. If none could be launched, then   *
 *          NULL is returned. If there is already the maximum bullet objects in play, then     *
 *          this could happen.                                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BulletClass * InfantryClass::Fire_At(TARGET target, int which)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	Mark(MARK_OVERLAP_UP);
	IsFiring = false;
	Mark(MARK_OVERLAP_DOWN);

	BulletClass * bullet = FootClass::Fire_At(target, which);
	if (bullet != NULL && !IsInLimbo) {

		/*
		**	For fraidycat infantry that run out of ammo, always go into
		**	a maximum fear state at that time.
		*/
		if (Class->IsFraidyCat && !Ammo) {
			Fear = FEAR_MAXIMUM;
			if (Mission == MISSION_ATTACK || Mission == MISSION_HUNT) {
				Assign_Mission(MISSION_GUARD);
			}
		}
	}
	return(bullet);
}


/***********************************************************************************************
 * InfantryClass::Unlimbo -- Unlimbo infantry unit in legal sub-location.                      *
 *                                                                                             *
 *    This will attempt to unlimbo the infantry unit at the designated coordinate, but will    *
 *    ensure that the coordinate is a legal subposition.                                       *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate to unlimbo the infantry at.                             *
 *                                                                                             *
 *          facing   -- The desired initial facing for the infantry unit.                      *
 *                                                                                             *
 *          strength -- The desired initial strength for the infantry unit.                    *
 *                                                                                             *
 *          mission  -- The desired initial mission for the infantry unit.                     *
 *                                                                                             *
 * OUTPUT:  bool; Was the infantry unlimboed successfully?                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Unlimbo(COORDINATE coord, DirType facing)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	/*
	**	Make sure that the infantry start in a legal position on the map.
	*/
	coord = Map[coord].Closest_Free_Spot(coord, ScenarioInit);
	if (coord == NULL) {
		return(false);
	}

	if (FootClass::Unlimbo(coord, facing)) {

		/*
		**	Ensure that the owning house knows about the
		**	new object.
		*/
		House->IScan |= (1L << Class->Type);
		House->ActiveIScan |= (1L << Class->Type);

		/*
		**	If there is no sight range, then this object isn't discovered by the player unless
		**	it actually appears in a cell mapped by the player.
		*/
		if (Class->SightRange == 0) {
			IsDiscoveredByPlayer = false;
		}

		Set_Occupy_Bit(coord);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * InfantryClass::Greatest_Threat -- Determines greatest threat (target) for infantry unit.    *
 *                                                                                             *
 *    This routine intercepts the Greatest_Threat request and adds the appropriate target      *
 *    types to search for. For regular infantry, this consists of all the ground types. For    *
 *    rocket launching infantry, this also includes aircraft.                                  *
 *                                                                                             *
 * INPUT:   threat   -- The basic threat control value.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the best target for this infantry unit to attack. If no suitable      *
 *          target could be found, then TARGET_NONE is returned.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/01/1995 JLB : Created.                                                                 *
 *   09/28/1995 JLB : Engineers try to recapture buildings first.                              *
 *=============================================================================================*/
TARGET InfantryClass::Greatest_Threat(ThreatType threat) const
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	/*
	**	Engineers consider only buildings that can be captured as being a threat. All others
	**	are ignored. If there is a building that needs to be recaptured and it is nearby
	**	then automatically head toward it to recapture it.
	*/
	if (!House->IsHuman && Class->IsCapture && !Is_Weapon_Equipped()) {
		if (House->ToCapture != TARGET_NONE && Distance(House->ToCapture) < 0x0F00) {
			return(House->ToCapture);
		}
		threat = threat | THREAT_CAPTURE;
	}

	if (!Is_Weapon_Equipped()) {
		if (!Class->IsCapture && *this != INFANTRY_RENOVATOR && *this != INFANTRY_SPY && *this != INFANTRY_THIEF) {
			return(TARGET_NONE);
		}
	}

	/*
	**	Special hack to make Tanya not auto-fire if controlled by a
	**	human player.
	*/
	if (*this == INFANTRY_TANYA && House->IsHuman) {
		return(TARGET_NONE);
	}

	if (Class->PrimaryWeapon != NULL) {
		threat = threat | Class->PrimaryWeapon->Allowed_Threats();
	}
	if (Class->SecondaryWeapon != NULL) {
		threat = threat | Class->SecondaryWeapon->Allowed_Threats();
	}

	/*
	**	Organic weapon types don't consider anything but infantry to be a threat. Such
	**	weapon types would be the dog jaw and the medic first aid kit.
	*/
	if (Is_Weapon_Equipped() && Class->PrimaryWeapon->WarheadPtr->IsOrganic) {
		threat = threat & ~(THREAT_BUILDINGS|THREAT_VEHICLES|THREAT_BOATS|THREAT_AIR);
	}

	/*
	**	Human controlled infantry don't automatically fire upon buildings.
	*/
	if (Is_Weapon_Equipped() && House->IsHuman) {
		threat = threat & ~THREAT_BUILDINGS;
	}

	/*
	**	If this is a bomber type, then allow buildings to be considered a threat.
	*/
	if (Class->IsBomber && !House->IsHuman) {
		threat = threat | THREAT_BUILDINGS;
	}

	/*
	** Special hack: if it's a thief, then the only possible objects to
	** consider are tiberium-processing objects (silos & refineries).
	*/
	if (*this == INFANTRY_THIEF) {
		threat = threat | THREAT_CAPTURE | THREAT_TIBERIUM;
//		threat = (ThreatType)(THREAT_CAPTURE | THREAT_TIBERIUM);
	}
	return(FootClass::Greatest_Threat(threat));
}


/***********************************************************************************************
 * InfantryClass::Response_Select -- Plays infantry audio response due to being selected.      *
 *                                                                                             *
 *    This routine handles playing an audio response as a result of the player selecting the   *
 *    infantry unit. This occurs prior to giving it an order and may not be followed by any    *
 *    order at all.                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/01/1995 JLB : Created.                                                                 *
 *   05/05/1995 JLB : Rambo response types added.                                              *
 *=============================================================================================*/
void InfantryClass::Response_Select(void)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	if (!AllowVoice) return;

	if (Class->IsCivilian && *this != INFANTRY_EINSTEIN) {
		VocType response = VOC_NONE;
		if (Class->IsFemale) {
			response = VOC_GIRL_YEAH;
		} else {
			response = VOC_GUY_YEAH;
		}
		Sound_Effect(response, fixed(1), ID+1);

	} else {
		static VocType _eng_response[] = {VOC_ENG_YES,VOC_ENG_ENG};
		static VocType _ein_response[] = {VOC_E_AH};
		static VocType _dog_response[] = {VOC_DOG_YES};
		static VocType _spy_response[] = {VOC_SPY_COMMANDER,VOC_SPY_YESSIR};
		static VocType _medic_response[] = {VOC_MED_REPORTING,VOC_MED_YESSIR};
		static VocType _tanya_response[] = {VOC_TANYA_YEA,VOC_TANYA_YES,VOC_TANYA_WHATS};
		static VocType _thief_response[] = {VOC_THIEF_YEA,VOC_THIEF_WHAT};
		static VocType _default_response[] = {VOC_ACKNOWL,VOC_REPORT,VOC_REPORT,VOC_YESSIR,VOC_YESSIR,VOC_READY,VOC_AWAIT};
		static VocType _stavros[] = {VOC_STAVCMDR,VOC_STAVYES};
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		static VocType _mechanic_response[] = {VOC_MECHHOWDY1,VOC_MECHHUH1,VOC_MECHLAFF1};
		static VocType _shock_response[] = {VOC_STYES1,VOC_STJUMP1,VOC_STJUICE1};
#endif

		int size = 0;
		VocType * response = NULL;
		HousesType house = PlayerPtr->ActLike;
		switch (Class->Type) {
			case INFANTRY_GENERAL:
				if (house != HOUSE_USSR && house != HOUSE_BAD) {
					response = _stavros;
					size = ARRAY_SIZE(_stavros);
				} else {
					response = _default_response;
					size = ARRAY_SIZE(_default_response);
				}
				house = HOUSE_USSR;
				break;

			case INFANTRY_DOG:
				response = _dog_response;
				size = ARRAY_SIZE(_dog_response);
				break;

			case INFANTRY_EINSTEIN:
				response = _ein_response;
				size = ARRAY_SIZE(_ein_response);
				break;

			case INFANTRY_SPY:
				response = _spy_response;
				size = ARRAY_SIZE(_spy_response);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				if(house == HOUSE_USSR) {
					response = _default_response;
					size = ARRAY_SIZE(_default_response);
				}
#endif
				break;

			case INFANTRY_MEDIC:
				response = _medic_response;
				size = ARRAY_SIZE(_medic_response);
				break;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			case INFANTRY_MECHANIC:
				response = _mechanic_response;
				size = ARRAY_SIZE(_mechanic_response);
				break;
			case INFANTRY_SHOCK:
				response = _shock_response;
				size = ARRAY_SIZE(_shock_response);
				break;
#endif
			case INFANTRY_TANYA:
				response = _tanya_response;
				size = ARRAY_SIZE(_tanya_response);
				break;

			case INFANTRY_THIEF:
				response = _thief_response;
				size = ARRAY_SIZE(_thief_response);
				break;

			case INFANTRY_RENOVATOR:
				response = _eng_response;
				size = ARRAY_SIZE(_eng_response);
				break;

			default:
				response = _default_response;
				size = ARRAY_SIZE(_default_response);
				break;
		}
		if (response != NULL) {
			Sound_Effect(response[Sim_Random_Pick(0, size-1)], fixed(1), ID+1, 0, house);
		}
	}
}


/***********************************************************************************************
 * InfantryClass::Response_Move -- Plays infantry response to movement order.                  *
 *                                                                                             *
 *    When the infantry is given the order to move, this routine handles the audio response    *
 *    generated by the infantry unit.                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/01/1995 JLB : Created.                                                                 *
 *   05/05/1995 JLB : Rambo response types added.                                              *
 *=============================================================================================*/
void InfantryClass::Response_Move(void)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	if (!AllowVoice) return;

	if (Class->IsCivilian && *this != INFANTRY_EINSTEIN) {
		VocType response;
		if (Class->IsFemale) {
			response = VOC_GIRL_OKAY;
		} else {
			response = VOC_GUY_OKAY;
		}
		Sound_Effect(response, fixed(1), ID+1);

	} else {
		static VocType _eng_response[] = {VOC_ENG_AFFIRM,VOC_ENG_AFFIRM};
		static VocType _ein_response[] = {VOC_E_OK,VOC_E_YES};
		static VocType _dog_response[] = {VOC_DOG_BARK};
		static VocType _spy_response[] = {VOC_SPY_ONWAY,VOC_SPY_KING,VOC_SPY_INDEED};
		static VocType _medic_response[] = {VOC_MED_AFFIRM,VOC_MED_MOVEOUT};
#ifdef ENGLISH
		static VocType _tanya_response[] = {VOC_TANYA_THERE,VOC_TANYA_ROCK};
#else
		static VocType _tanya_response[] = {VOC_TANYA_THERE,VOC_TANYA_GIVE};
#endif
		static VocType _thief_response[] = {VOC_THIEF_MOVEOUT,VOC_THIEF_OKAY,VOC_THIEF_AFFIRM};
		static VocType _default_response[] = {VOC_ROGER,VOC_RIGHT_AWAY,VOC_UGOTIT,VOC_AFFIRM,VOC_AFFIRM};
		static VocType _stavros[] = {VOC_STAVMOV,VOC_STAVCRSE};
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		static VocType _mechanic[] = {VOC_MECHYES1,VOC_MECHRISE1,VOC_MECHHEAR1,VOC_MECHBOSS1};
		static VocType _shock[] = {VOC_STPOWER1,VOC_STDANCE1,VOC_STCHRGE1};
#endif

		int size = 0;
		VocType * response = NULL;
		HousesType house = PlayerPtr->ActLike;
		switch (Class->Type) {
			case INFANTRY_GENERAL:
				if (house != HOUSE_USSR && house != HOUSE_BAD) {
					response = _stavros;
					size = ARRAY_SIZE(_stavros);
				} else {
					response = _default_response;
					size = ARRAY_SIZE(_default_response);
				}
				house = HOUSE_USSR;
				break;

			case INFANTRY_DOG:
				response = _dog_response;
				size = ARRAY_SIZE(_dog_response);
				break;

			case INFANTRY_EINSTEIN:
				response = _ein_response;
				size = ARRAY_SIZE(_ein_response);
				break;

			case INFANTRY_RENOVATOR:
				response = _eng_response;
				size = ARRAY_SIZE(_eng_response);
				break;

			case INFANTRY_SPY:
				response = _spy_response;
				size = ARRAY_SIZE(_spy_response);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				if(house == HOUSE_USSR) {
					response = _default_response;
					size = ARRAY_SIZE(_default_response);
				}
#endif
				break;

			case INFANTRY_MEDIC:
				response = _medic_response;
				size = ARRAY_SIZE(_medic_response);
				break;

#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			case INFANTRY_MECHANIC:
				response = _mechanic;
				size = ARRAY_SIZE(_mechanic);
				break;

			case INFANTRY_SHOCK:
				response = _shock;
				size = ARRAY_SIZE(_shock);
				break;

#endif
			case INFANTRY_TANYA:
				response = _tanya_response;
				size = ARRAY_SIZE(_tanya_response);
				break;

			case INFANTRY_THIEF:
				response = _thief_response;
				size = ARRAY_SIZE(_thief_response);
				break;

			default:
				response = _default_response;
				size = ARRAY_SIZE(_default_response);
				break;
		}
		if (response != NULL) {
			Sound_Effect(response[Sim_Random_Pick(0, size-1)], fixed(1), ID+1, 0, house);
		}
	}
}


/***********************************************************************************************
 * InfantryClass::Response_Attack -- Plays infantry audio response to attack order.            *
 *                                                                                             *
 *    When the player gives an infantry unit the order to attack, this routine handles         *
 *    the audio response by that unit.                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/01/1995 JLB : Created.                                                                 *
 *   05/05/1995 JLB : Rambo response types added.                                              *
 *=============================================================================================*/
void InfantryClass::Response_Attack(void)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	if (!AllowVoice) return;

	if (Class->IsCivilian && *this != INFANTRY_EINSTEIN) {
		VocType response;
		if (Class->IsFemale) {
			response = VOC_GIRL_OKAY;
		} else {
			response = VOC_GUY_OKAY;
		}
		Sound_Effect(response, fixed(1), ID+1);

	} else {
		static VocType _eng_response[] = {VOC_ENG_AFFIRM,VOC_ENG_AFFIRM};
		static VocType _dog_response[] = {VOC_DOG_GROWL2};
		static VocType _ein_response[] = {VOC_E_OK,VOC_E_YES};
		static VocType _spy_response[] = {VOC_SPY_ONWAY,VOC_SPY_KING,VOC_SPY_INDEED};
		static VocType _medic_response[] = {VOC_MED_AFFIRM,VOC_MED_MOVEOUT};
#ifdef ENGLISH
		static VocType _tanya_response[] = {VOC_TANYA_CHEW,VOC_TANYA_CHING,VOC_TANYA_LAUGH};
#else
		static VocType _tanya_response[] = {VOC_TANYA_CHEW,VOC_TANYA_CHING,VOC_TANYA_LAUGH,VOC_TANYA_ROCK};
#endif
		static VocType _thief_response[] = {VOC_NONE};
		static VocType _default_response[] = {VOC_RIGHT_AWAY,VOC_AFFIRM,VOC_AFFIRM,VOC_UGOTIT,VOC_NO_PROB,VOC_YESSIR,VOC_YESSIR,VOC_YESSIR};
		static VocType _stavros[] = {VOC_STAVCRSE};
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		static VocType _mechanic[] = {VOC_MECHYEEHAW1,VOC_MECHHOTDIG1,VOC_MECHWRENCH1};
		static VocType _shock[] = {VOC_STLIGHT1,VOC_STBURN1,VOC_STCRISP1,VOC_STSHOCK1};
#endif

		int size = 0;
		VocType * response = NULL;
		HousesType house = PlayerPtr->ActLike;
		switch (Class->Type) {
			case INFANTRY_GENERAL:
				if (house != HOUSE_USSR && house != HOUSE_BAD) {
					response = _stavros;
					size = ARRAY_SIZE(_stavros);
				} else {
					response = _default_response;
					size = ARRAY_SIZE(_default_response);
				}
				house = HOUSE_USSR;
				break;

			case INFANTRY_DOG:
				response = _dog_response;
				size = ARRAY_SIZE(_dog_response);
				break;

			case INFANTRY_SPY:
				response = _spy_response;
				size = ARRAY_SIZE(_spy_response);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
				if(house == HOUSE_USSR) {
					response = _default_response;
					size = ARRAY_SIZE(_default_response);
				}
#endif
				break;

			case INFANTRY_EINSTEIN:
				response = _ein_response;
				size = ARRAY_SIZE(_ein_response);
				break;

			case INFANTRY_RENOVATOR:
				response = _eng_response;
				size = ARRAY_SIZE(_eng_response);
				break;

			case INFANTRY_MEDIC:
				response = _medic_response;
				size = ARRAY_SIZE(_medic_response);
				break;
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			case INFANTRY_MECHANIC:
				response = _mechanic;
				size = ARRAY_SIZE(_mechanic);
				break;

			case INFANTRY_SHOCK:
				response = _shock;
				size = ARRAY_SIZE(_shock);
				break;
#endif
			case INFANTRY_TANYA:
				response = _tanya_response;
				size = ARRAY_SIZE(_tanya_response);
				break;

			case INFANTRY_THIEF:
				response = _thief_response;
				size = ARRAY_SIZE(_thief_response);
				break;

			default:
				response = _default_response;
				size = ARRAY_SIZE(_default_response);
				break;
		}
		if (response != NULL) {
			Sound_Effect(response[Sim_Random_Pick(0, size-1)], fixed(1), ID+1, 0, house);
		}
	}
}


/***********************************************************************************************
 * InfantryClass::What_Action -- Infantry units might be able to capture -- check.             *
 *                                                                                             *
 *    This routine checks to see if the infantry unit can capture the specified object rather  *
 *    than merely attacking it. If this is the case, then ACTION_CAPTURE will be returned.     *
 *                                                                                             *
 * INPUT:   object   -- The object that the mouse is currently over.                           *
 *                                                                                             *
 * OUTPUT:  Returns the action that will be performed if the mouse were clicked over the       *
 *          object specified.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType InfantryClass::What_Action(ObjectClass const * object) const
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);
	assert(object != NULL);

	ActionType action = FootClass::What_Action(object);

	/*
	** If this is an engineer/renovator, we have to make some adjustments.
	** If the cursor is over an enemy building, return action-none.  If it's
	** over a friendly building, we have to return action-capture so he can
	** renovate it.
	** However, abort the whole thing if the building is a barrel or mine.
	*/
	if (*this == INFANTRY_RENOVATOR && object->What_Am_I() == RTTI_BUILDING && House->IsPlayerControl) {
		BuildingClass const * bldg = (BuildingClass *)object;
		if (bldg->Class->IsRepairable) {
			if (House->Is_Ally(bldg)) {
				if (bldg->Health_Ratio() == 1) {
					return(ACTION_NO_GREPAIR);
				}
				return(ACTION_GREPAIR);
			} else {

				if (bldg->Class->IsCaptureable) {
#ifdef FIXIT_ENGINEER	//	checked - ajw 9/28/98
					if (bldg->Health_Ratio() <= EngineerCaptureLevel) {
#else
					if (bldg->Health_Ratio() <= Rule.ConditionRed) {
#endif
						return(ACTION_CAPTURE);
					}
					return(ACTION_DAMAGE);
				}

//				if (bldg->Health_Ratio() <= Rule.ConditionRed && bldg->Class->IsCaptureable) {
			}
		}
	}

	/*
	** If this is a medic, and the cursor's over a friendly infantryman,
	** execute an action-attack.  In CSII, if this is a mechanic and the
	** cursor's over a friendly vehicle, execute an action-attack.
	*/
	if (Combat_Damage() < 0 && House->IsPlayerControl) {
		if (House->Is_Ally(object)) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
			if( (object->What_Am_I() == RTTI_INFANTRY && object != this && *this == INFANTRY_MEDIC) ||
			    (*this == INFANTRY_MECHANIC && (object->What_Am_I() == RTTI_UNIT || object->What_Am_I() == RTTI_AIRCRAFT) ) ) {

					if (object->Health_Ratio() < Rule.ConditionGreen) {
// If it's a mechanic force-moving into an APC, don't try to heal it.
						if(*this == INFANTRY_MECHANIC && object->What_Am_I() == RTTI_UNIT && *(UnitClass *)object == UNIT_APC && (Keyboard->Down(Options.KeyForceMove1) || Keyboard->Down(Options.KeyForceMove2)) ) {
						} else {
							return(ACTION_HEAL);
						}
					}
			}
#else
			if(object->What_Am_I() == RTTI_INFANTRY && object != this) {
				if (object->Health_Ratio() < Rule.ConditionGreen) {
					return(ACTION_HEAL);
				}
			}
#endif
			if(!object->Is_Techno() || !((TechnoClass *)object)->Techno_Type_Class()->Max_Passengers()) {
				if (action == ACTION_GUARD_AREA || action == ACTION_MOVE) {
					return(action);
				}
				return ((action == ACTION_TOGGLE_SELECT) ? ACTION_TOGGLE_SELECT : ACTION_SELECT);
			}
		} else {
			return(ACTION_NOMOVE);
		}
	}

#ifdef OBSOLETE
	/*
	** See if it's a thief attacking an enemy vehicle, let him CAPTURE it.
	*/
	if (*this == INFANTRY_THIEF && object->What_Am_I() == RTTI_UNIT) {
		if (((UnitClass *)object)->House != House) {
			return(ACTION_CAPTURE);
		}
	}
#endif

	/*
	** Dogs can only attack infantrymen
	*/
	if (Class->IsDog && action == ACTION_ATTACK && object->What_Am_I() != RTTI_INFANTRY) {
		action = ACTION_NONE;
	}

	/*
	**	See if it's a commando, and if he's attacking a building,
	** have him return ACTION_SABOTAGE instead
	*/
	if (Class->IsBomber && action == ACTION_ATTACK && object->What_Am_I() == RTTI_BUILDING) {
		BuildingClass const * obj = (BuildingClass *)object;
		/*
		** Hack: Tanya should shoot barrels, bomb other structures.
		*/
		if (obj->Class->IsRepairable) {
//		if (*obj != STRUCT_BARREL && *obj != STRUCT_BARREL3) {
			return(ACTION_SABOTAGE);
		} else {
			return(ACTION_ATTACK);
		}
	}

	/*
	** See if this infantry is trying to move onto where a land mine is.
	*/
	if (action == ACTION_NONE && object->What_Am_I() == RTTI_BUILDING && House->IsPlayerControl) {
		StructType blah = *((BuildingClass *)object);
		if (blah == STRUCT_AVMINE || blah == STRUCT_APMINE) return(ACTION_MOVE);
	}

	/*
	**	There is no self-select action available for infantry types.
	*/
	if (action == ACTION_SELF) {
		action = ACTION_NONE;
	}

	/*
	**	Check to see if it can enter a transporter.
	*/
	if (
		House->Is_Ally(object) &&
		House->IsPlayerControl && object->Is_Techno()) {
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
 if (object->What_Am_I() != RTTI_VESSEL || *(VesselClass *)object != VESSEL_CARRIER) {
#endif
		switch (((InfantryClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (TechnoClass*)object)) {
			case RADIO_ROGER:
				action = ACTION_ENTER;
				break;

			case RADIO_NEGATIVE:
				action = ACTION_NO_ENTER;
				break;

			default:
				break;
		}
#ifdef FIXIT_CARRIER	//	checked - ajw 9/28/98
 }
#endif
	}

	if (Class->IsCapture && action == ACTION_ATTACK) {
		if (!House->Is_Ally(object) && (
//Disable capturing of helicopters			 (object->What_Am_I() == RTTI_AIRCRAFT && ((AircraftClass *)object)->Pip_Count() == 0 && *((AircraftClass *)object) == AIRCRAFT_TRANSPORT) ||
			 (object->What_Am_I() == RTTI_BUILDING && ((BuildingClass *)object)->Class->IsCaptureable) )
			) {

				if (*this == INFANTRY_THIEF && (object->What_Am_I() == RTTI_BUILDING && ((BuildingClass *)object)->Class->Capacity == 0)) {
					action = ACTION_NONE;
				} else {

					/*
					** If we're trying to capture a building, make sure we can get
					** to it.  Find an adjacent cell that's the same zone as us.
					** The target circumstance is a naval yard that doesn't touch
					** the shore - a total island.  In that case, we can't capture
					** it, so we shouldn't show the action-capture cursor.
					*/
					action = ACTION_CAPTURE;
					if (object->What_Am_I() == RTTI_BUILDING) {
						CELL cell = ::As_Cell(object->As_Target());
						int targzone = Map[::As_Cell(As_Target())].Zones[Class->MZone];
						short const *list = ((BuildingClass *)object)->Class->Occupy_List(false);
						bool found = false;
						while (*list != REFRESH_EOL && !found) {
							CELL newcell = cell + *list++;
							for (FacingType i=FACING_N; i < FACING_COUNT; i++) {
								CELL adjcell = Adjacent_Cell(newcell, i);
								if ((unsigned)adjcell >= MAP_CELL_TOTAL) continue;
								if (Map[adjcell].Zones[Class->MZone] == targzone) {
									found = true;
									break;
								}
							}
						}
						if (!found) {
							action = ACTION_NONE;
						}
					}
				}
		} else {
			if (!Is_Weapon_Equipped()) {
				action = ACTION_NONE;
			}
		}
	}

	/*
	**	If it doesn't know what to do with the object, then just
	**	say it can't move there.
	*/
	if (action == ACTION_NONE) action = ACTION_NOMOVE;

	return(action);
}


/***********************************************************************************************
 * InfantryClass::Active_Click_With -- Handles action when clicking with infantry soldier.     *
 *                                                                                             *
 *    This routine is called when the player clicks over an object while this infantry soldier *
 *    is selected. Capture attempts are prohibited if the infantry cannot capture. The         *
 *    command might respond if told to sabotage something.                                     *
 *                                                                                             *
 * INPUT:   action   -- The action that is nominally to be performed.                          *
 *                                                                                             *
 *          object   -- The object over which the mouse was clicked.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Active_Click_With(ActionType action, ObjectClass * object)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	action = What_Action(object);

	switch (action) {
		case ACTION_GREPAIR:
		case ACTION_DAMAGE:
		case ACTION_CAPTURE:
			action = ACTION_CAPTURE;
			break;

		case ACTION_HEAL:
			action = ACTION_ATTACK;
			break;

//		case ACTION_ENTER:
//			action = ACTION_MOVE;
//			break;

		case ACTION_SABOTAGE:
		case ACTION_ATTACK:
		case ACTION_GUARD_AREA:
		case ACTION_MOVE:
			action = action;
			break;

		default:
//			action = ACTION_NONE;
			break;
	}

	FootClass::Active_Click_With(action, object);
}


/***********************************************************************************************
 * InfantryClass::Set_Occupy_Bit -- Sets the occupy bit cell and bit pos                       *
 *                                                                                             *
 * INPUT:      CELL      - the cell we are setting the bit in                                  *
 *                                                                                             *
 *               int      - the spot index we are setting the bit for                          *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/08/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Set_Occupy_Bit(CELL cell, int spot_index)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	/*
	** Set the occupy position for the spot that we passed in
	*/
	Map[cell].Flag.Composite |= (1 << spot_index);

	/*
	** Record the type of infantry that now owns the cell
	*/
	Map[cell].InfType = Owner();
}


/***************************************************************************
 * InfantryClass::Clear_Occupy_Bit -- Clears occupy bit and given cell		*
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/08/1995 PWG : Created.                                             *
 *=========================================================================*/
void InfantryClass::Clear_Occupy_Bit(CELL cell, int spot_index)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	/*
	** Clear the occupy bit for the infantry in that cell
	*/
	Map[cell].Flag.Composite &= ~(1 << spot_index);

	/*
	** If he was the last infantry recorded in the cell then
	** remove the infantry ownership flag.
	*/
	if (!(Map[cell].Flag.Composite & 0x1F)) {
		Map[cell].InfType = HOUSE_NONE;
	}
}


/***********************************************************************************************
 * InfantryClass::Full_Name -- Fetches the full name of the infantry unit.                     *
 *                                                                                             *
 *    This routine will return with the full name (as a text number) for this infantry         *
 *    unit. Typically, this is the normal name, but in cases of civilian type survivors from   *
 *    a building explosion, it might be a technician instead. In such a case, the special      *
 *    technician name number is returned instead.                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the full name to use for this infantry unit.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/30/1995 JLB : Created.                                                                 *
 *   10/28/1996 JLB : Spy returns "enemy soldier" text name.                                   *
 *=============================================================================================*/
int InfantryClass::Full_Name(void) const
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	if (IsTechnician) {
		return(TXT_TECHNICIAN);
	}

	if (*this == INFANTRY_SPY && !House->IsPlayerControl) {
		return(TXT_E1);
	}

	return(Class->Full_Name());
}


/***********************************************************************************************
 * InfantryClass::Mission_Attack -- Intercept attack mission for special handling.             *
 *                                                                                             *
 *    This routine intercepts the normal attack mission and if an engineer is detected and the *
 *    target is a building, then the engineer will be automatically assigned the capture       *
 *    mission. In other cases, the normal attack logic will proceed.                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine again. *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/07/1995 JLB : Created.                                                                 *
 *   04/15/1996 BWG : Engineers can only attack their own house's buildings now.               *
 *   05/29/1996 JLB : Engineers can now damage/capture enemy buildings.                        *
 *=============================================================================================*/
int InfantryClass::Mission_Attack(void)
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	if (Class->IsBomber && As_Building(TarCom)) {
		Assign_Destination(TarCom);
		Assign_Mission(MISSION_SABOTAGE);
		return(1);
	}

	if (Class->IsCapture && As_Building(TarCom) != NULL && As_Building(TarCom)->Class->IsCaptureable) {
		Assign_Destination(TarCom);
		Assign_Mission(MISSION_CAPTURE);
		return(1);
	}

	return(FootClass::Mission_Attack());
}


/***********************************************************************************************
 * InfantryClass::What_Action -- Determines what action to perform for the cell specified.     *
 *                                                                                             *
 *    This routine will determine what action to perform if the mouse was clicked on the cell  *
 *    specified. This is just a courier function since the lower level classes actually        *
 *    perform the work. The need for this routine at this level is due to the existence of     *
 *    a similarly named function at this level as well.  C++ namespace rules require this      *
 *    function courier to be in place or an error will result.                                 *
 *                                                                                             *
 * INPUT:   cell  -- The cell that the mouse might be clicked upon.                            *
 *                                                                                             *
 * OUTPUT:  Returns with the action that would be given to this infantry unit if the mouse     *
 *          were clicked at the cell specified.                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType InfantryClass::What_Action(CELL cell) const
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	ActionType action = FootClass::What_Action(cell);

	/*
	** Dogs can only attack infantrymen
	*/
	if (Class->IsDog && action == ACTION_ATTACK) {
		action = ACTION_NONE;
	}

	/*
	** If this is a medic, and the cursor's over a friendly infantryman,
	** execute an action-attack.
	*/
	if (Combat_Damage() < 0 && House->IsPlayerControl) {
		if (action == ACTION_ATTACK) {
			action = ACTION_NOMOVE;
		}
	}

	/*
	**	Demolitioners may destroy a bridge
	*/
	if (Class->IsBomber && action == ACTION_MOVE && !Special.IsCaptureTheFlag) {
		switch (Map[cell].TType) {
			case TEMPLATE_BRIDGE1:
			case TEMPLATE_BRIDGE2:
			case TEMPLATE_BRIDGE1H:
			case TEMPLATE_BRIDGE2H:
			case TEMPLATE_BRIDGE_1A:
			case TEMPLATE_BRIDGE_1B:
			case TEMPLATE_BRIDGE_2A:
			case TEMPLATE_BRIDGE_2B:
//			case TEMPLATE_BRIDGE_3A:
//			case TEMPLATE_BRIDGE_3B:
				return(ACTION_SABOTAGE);
		}
	}

#ifdef OBSOLETE
	/*
	** Engineers may repair a destroyed bridge.
	*/
	if (*this == INFANTRY_RENOVATOR && action == ACTION_NOMOVE) {
		/*
		** If they're pointing on the wrong side of the bridge, ignore it
		** 'cause we can't get there.
		*/
		TemplateType tt = Map[cell].TType;
		if (tt == TEMPLATE_BRIDGE1D || tt == TEMPLATE_BRIDGE2D ||
			tt == TEMPLATE_BRIDGE_1C || tt == TEMPLATE_BRIDGE_2C ||
			(tt >= TEMPLATE_BRIDGE_3C && tt <= TEMPLATE_BRIDGE_3E) ) {
			/*
			** We know they're pointing at a destroyed bridge cell.  If the cell
			** they're pointing at is surrounded by impassables, return this
			** cell as impassable.  But, if any cell surrounding this cell is
			** passable, return that this is a capturable cell.
			*/
			if (Map[cell].Land_Type() == LAND_ROCK) {
				if (tt == TEMPLATE_BRIDGE_3C) return(ACTION_CAPTURE);

				if (tt == TEMPLATE_BRIDGE_3C) return(ACTION_CAPTURE);
				int y = Cell_Y(cell);
				if (y) {
					LandType above = Map[(CELL)(cell-(MAP_CELL_W-1))].Land_Type();
					if (above == LAND_CLEAR || above == LAND_ROAD) {
						if (Map[(CELL)(cell-(MAP_CELL_W-1))].Zone == Map[As_Cell(As_Target())].Zone) {
							return(ACTION_CAPTURE);
						}
						return(ACTION_NOMOVE);
					}
				}
				if (y < MAP_CELL_H) {
					LandType below = Map[(CELL)(cell + MAP_CELL_W-1)].Land_Type();
					if (below == LAND_CLEAR || below == LAND_ROAD) {
						if (Map[(CELL)(cell+MAP_CELL_W-1)].Zone == Map[As_Cell(As_Target())].Zone) {
							return(ACTION_CAPTURE);
						}
						return(ACTION_NOMOVE);
					}
				}
			}
			return(ACTION_NOMOVE);
		}
	}
#endif
	return(action);
}


/***********************************************************************************************
 * InfantryClass::Class_Of -- Returns the class reference for this object.                     *
 *                                                                                             *
 *    This routine will return a reference to the infantry type class object that describes    *
 *    this infantry's characteristics.                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the InfantryTypeClass object associated with this      *
 *          infantry object.                                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectTypeClass const & InfantryClass::Class_Of(void) const
{
	assert(Infantry.ID(this) == ID);
	assert(IsActive);

	return(*Class);
}


/***********************************************************************************************
 * InfantryClass::Read_INI -- Reads units from scenario INI file.                              *
 *                                                                                             *
 *    This routine is used to read all the starting units from the                             *
 *    scenario control INI file. The units are created and placed on the                       *
 *    map by this routine.                                                                     *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Housename, Typename, Strength, Cellnum, CellSublocation, Missionname,                  *
 *         Facingnum, Triggername                                                              *
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
void InfantryClass::Read_INI(CCINIClass & ini)
{
	InfantryClass	* infantry;			// Working infantry pointer.
	HousesType		inhouse;			// Infantry house.
	InfantryType	classid;			// Infantry class.
	char				buf[128];
	char				* validation;
	DirType 			dir;
	TriggerTypeClass	* tp;

	int len = ini.Entry_Count(INI_Name());
	for (int index = 0; index < len; index++) {
		char const * entry = ini.Get_Entry(INI_Name(), index);

		/*
		**	Get an infantry entry
		*/
		ini.Get_String(INI_Name(), entry, NULL, buf, sizeof(buf));

		/*
		**	1st token: house name.
		*/
		inhouse = HouseTypeClass::From_Name(strtok(buf, ",\n\r"));
		if (inhouse != HOUSE_NONE) {

			/*
			**	2nd token: infantry type name.
			*/
			classid = InfantryTypeClass::From_Name(strtok(NULL, ",\n\r"));

			if (classid != INFANTRY_NONE) {

				if (HouseClass::As_Pointer(inhouse) != NULL) {
					infantry = new InfantryClass(classid, inhouse);
					if (infantry != NULL) {

						/*
						**	3rd token: strength.
						*/
						int strength = atoi(strtok(NULL, ",\n\r"));

						/*
						**	4th token: cell #.
						*/
						CELL cell = atoi(strtok(NULL, ",\n\r"));
						COORDINATE coord = Cell_Coord(cell);

						/*
						**	5th token: cell sub-location.
						*/
						int sub = atoi(strtok(NULL, ","));
						coord = Coord_Add(Coord_Whole(coord), StoppingCoordAbs[ sub ]);

						/*
						**	Fetch the mission and facing.
						*/
						MissionType mission = MissionClass::Mission_From_Name(strtok(NULL, ",\n\r"));
						validation = strtok(NULL, ",\n\r");
						if (validation) {
							dir = (DirType)atoi(validation);
							validation = strtok(NULL, ",\n\r");
							if (validation) {
								tp = TriggerTypeClass::From_Name(validation);
							} else {
								tp = NULL;
							}
						} else {
							dir = (DirType)0;
							tp = NULL;
						}

						infantry->Trigger = NULL;
						if (tp != NULL) {
							TriggerClass * tt = Find_Or_Make(tp);
							if (tt != NULL) {
								tt->AttachCount++;
								infantry->Trigger = tt;
							}
						}

						if (infantry->Unlimbo(coord, dir)) {
							infantry->Strength = (int)infantry->Class_Of().MaxStrength * fixed(strength, 256);
							if (infantry->Strength > infantry->Class->MaxStrength-3) infantry->Strength = infantry->Class->MaxStrength;
	//						infantry->Strength = Fixed_To_Cardinal(infantry->Class_Of().MaxStrength, strength);
							if (Session.Type == GAME_NORMAL || infantry->House->IsHuman) {
								infantry->Assign_Mission(mission);
								infantry->Commence();
							} else {
								infantry->Enter_Idle_Mode();
							}
						} else {

							/*
							**	If the infantry could not be unlimboed, then this is a big error.
							**	Delete the infantry.
							*/
							delete infantry;
						}
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * InfantryClass::Write_INI -- Store the infantry to the INI database.                         *
 *                                                                                             *
 *    This will store all the infantry objects to the INI database specified.                  *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to store the infantry data to.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Write_INI(CCINIClass & ini)
{
	/*
	**	First, clear out all existing infantry data from the ini file.
	*/
	ini.Clear(INI_Name());

	/*
	**	Write the infantry data out.
	*/
	for (int index = 0; index < Infantry.Count(); index++) {
		InfantryClass * infantry = Infantry.Ptr(index);
		if (!infantry->IsInLimbo) {
			char	uname[10];
			char	buf[128];

			sprintf(uname, "%d", index);
			sprintf(buf, "%s,%s,%d,%u,%d,%s,%d,%s",
					infantry->House->Class->IniName,
					infantry->Class->IniName,
					infantry->Health_Ratio()*256,
					Coord_Cell(infantry->Coord),
					CellClass::Spot_Index(infantry->Coord),
					MissionClass::Mission_Name((infantry->Mission == MISSION_NONE) ?
						infantry->MissionQueue : infantry->Mission),
					infantry->PrimaryFacing.Current(),
					infantry->Trigger.Is_Valid() ? infantry->Trigger->Class->IniName : "None"
				);
			ini.Put_String(INI_Name(), uname, buf);
		}
	}
}


/***********************************************************************************************
 * InfantryClass::Fear_AI -- Process any fear related affects on this infantry.                *
 *                                                                                             *
 *    Use this routine to handle the fear logic for this infantry. It will slowly increase     *
 *    the bravery of the infantry as well as cause it to stand up or lie down as appropriate.  *
 *    It will even handle the special fraidy cat logic for civilian infantry.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this once per game logic loop per infantry unit.                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Fear_AI(void)
{
	/*
	**	After a time, the infantry will gain courage.
	*/
	if (Fear > 0) {

		Fear--;

		/*
		**	When an armed civilian becomes unafraid, he will then reload
		**	another clip into his pistol.
		*/
		if (Fear == 0 && Ammo == 0 && Is_Weapon_Equipped()) {
			Ammo = Class->MaxAmmo;
		}

		/*
		**	Stand up if brave and lie down if afraid.
		*/
		if (IsProne) {
			if (Fear < FEAR_ANXIOUS) {
				Do_Action(DO_GET_UP);
			}
		} else  {

			/*
			**	Drop to the ground if anxious. Don't drop to the ground while moving
			**	and the special elite flag is active.
			*/
			if (!Class->IsDog && Height == 0 && Fear >= FEAR_ANXIOUS && ((!Target_Legal(NavCom) && !IsDriving))) {
				Do_Action(DO_LIE_DOWN);
			}
		}
	}

	/*
	**	When in darkness or in doubt,
	**		run in circles, scream, and shout.
	*/
	if (Class->IsFraidyCat && Fear > FEAR_ANXIOUS && !IsFalling && !IsDriving && !Target_Legal(NavCom)) {
		Scatter(0, true);
	}
}


/***********************************************************************************************
 * InfantryClass::Edge_Of_World_AI -- Detects when infantry has left the map.                  *
 *                                                                                             *
 *    This routine will detect when the infantry has left the edge of the world and will       *
 *    delete it as necessary.                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the infantry unit deleted by this routine?                               *
 *                                                                                             *
 * WARNINGS:   Be sure the check the return value and if true, abort any further processing    *
 *             for this infantry unit.                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Edge_Of_World_AI(void)
{
	/*
	**	Delete this unit if it finds itself off the edge of the map and it is in
	**	guard or other static mission mode.
	*/
	if (Team.Is_Valid() && IsLocked) Team->IsLeaveMap = true;

	if (!Team.Is_Valid() && Mission == MISSION_GUARD && !Map.In_Radar(Coord_Cell(Coord))) {
		Stun();
		delete this;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * InfantryClass::Firing_AI -- Handles firing and combat AI for the infantry.                  *
 *                                                                                             *
 *    This will examine the infantry and determine what firing action is required. It will     *
 *    search for targets, starting firing animations, and launch bullets as necessary.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per infantry per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Firing_AI(void)
{
	if (Target_Legal(TarCom)) {
		int primary = What_Weapon_Should_I_Use(TarCom);

		if (!IsFiring) {
			switch (Can_Fire(TarCom, primary)) {
				case FIRE_ILLEGAL:
					if (Combat_Damage(primary) < 0) {
						ObjectClass * targ= As_Object(TarCom);
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
						if (targ) {
							if( (targ->What_Am_I() == RTTI_INFANTRY && *this == INFANTRY_MEDIC) ||
								 (*this == INFANTRY_MECHANIC && (targ->What_Am_I() == RTTI_AIRCRAFT || targ->What_Am_I() == RTTI_UNIT )) ) {

									if (targ->Health_Ratio() >= Rule.ConditionGreen) {
										Assign_Target(TARGET_NONE);
									}
							}
						} else {
							Assign_Target(TARGET_NONE);
						}
#else
						if (targ && targ->What_Am_I() == RTTI_INFANTRY) {
							if (targ->Health_Ratio() >= Rule.ConditionGreen) {
								Assign_Target(TARGET_NONE);
							}
						} else {
							Assign_Target(TARGET_NONE);
						}
#endif
					} else if (Class->IsDog) {
						Assign_Target(TARGET_NONE);
					}
					break;

				case FIRE_CLOAKED:
					Do_Uncloak();
					break;

				case FIRE_OK:
					/*
					**	Start firing animation.
					*/
					if (IsProne) {
						Do_Action(DO_FIRE_PRONE);
					} else {
						Do_Action(DO_FIRE_WEAPON);
					}

					Mark(MARK_OVERLAP_UP);
					IsFiring = true;
					Mark(MARK_OVERLAP_DOWN);

					PrimaryFacing.Set(Direction8(Center_Coord(), As_Coord(TarCom)));

					/*
					**	If the target is in range, and the NavCom is the same, then just
					**	stop and keep firing.
					*/
					if (TarCom == NavCom) {
						NavCom = TARGET_NONE;
						Path[0] = FACING_NONE;
					}
					break;
			}
		}

		/*
		**	If in the middle of firing animation, then only
		**	process that. Infantry cannot fire and move simultaneously.
		**	At some point in the firing animation process, a projectile
		**	will be launched. When the required animation frames have
		**	been completed, the firing animation stops.
		*/
		int firestage = Class->FireLaunch;
		if (IsProne) firestage = Class->ProneLaunch;

		if (IsFiring && Fetch_Stage() == firestage) {

			/*
			**	Target might have changed during the firing animation
			*/
			if (Can_Fire(TarCom, primary) == FIRE_OK) {
				Fire_At(TarCom, primary);

				/*
				**	Run away from slowly approaching projectiles.
				*/
				if (Class->PrimaryWeapon->MaxSpeed < Rule.Incoming) {
					Map[::As_Cell(TarCom)].Incoming(Coord, true);
				}

				/*
				** If it's a dog, get rid of him (he'll be re-created when he hits)
				*/
				if (Class->IsDog) {
					WasSelected = IsSelected;
					ScenarioInit++;
					Limbo();
					ScenarioInit--;
				}
			} else {
				Mark(MARK_OVERLAP_UP);
				IsFiring = false;
				Mark(MARK_OVERLAP_DOWN);
			}
		}
	} else {
		if (IsFiring) {
			Mark(MARK_OVERLAP_UP);
			IsFiring = false;
			Mark(MARK_OVERLAP_DOWN);
		}
	}
}


/***********************************************************************************************
 * InfantryClass::Doing_AI -- Handles the animation AI processing.                             *
 *                                                                                             *
 *    Infantry can be in one of many different animation sequences. At the conclusion of each  *
 *    sequence, the infantry will quite likely transition to a new animation state. This       *
 *    routine handles detecting when that trasition should occur and starting the infantry     *
 *    into its new state.                                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per infantry unit per game logic loop.              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Doing_AI(void)
{
	if (Doing == DO_NOTHING || Fetch_Stage() >= Class->DoControls[Doing].Count) {
		switch (Doing) {
			default:
				if (IsDriving) {
					if (Class->IsDog) {

						/*
						**	Dog crawl animation is actually the run animation.
						*/
						if (Target_Legal(TarCom)) {
							Do_Action(DO_CRAWL, true);
						} else {
							Do_Action(DO_WALK, true);
						}
					} else {
						if (IsProne) {
							Do_Action(DO_CRAWL, true);
						} else {
							Do_Action(DO_WALK, true);
						}
					}
				} else {
					if (Class->IsDog) {
						Do_Action(DO_STAND_READY, true);
					} else {
						if (IsProne) {
							Do_Action(DO_PRONE, true);
						} else {
							Do_Action(DO_STAND_READY, true);
						}
					}
				}
				break;

			case DO_DOG_MAUL:
				Do_Action(DO_STAND_READY, true);
				break;

			case DO_GUN_DEATH:
			case DO_EXPLOSION_DEATH:
			case DO_EXPLOSION2_DEATH:
			case DO_GRENADE_DEATH:
			case DO_FIRE_DEATH:
				if (Fetch_Stage() >= Class->DoControls[Doing].Count) {
					AnimClass* anim = NULL;
					LandType land = Map[Center_Coord()].Land_Type();
					if (land != LAND_ROCK && land != LAND_WATER && land != LAND_RIVER) {
						if (Doing == DO_GUN_DEATH && !Class->IsDog && Height==0) {
							anim = new AnimClass(ANIM_CORPSE1, Coord_Add(Center_Coord(), XYP_Coord(-2, 4)));
						}
						if (Doing == DO_GRENADE_DEATH && !Class->IsDog && Height==0) {
							anim = new AnimClass(ANIM_CORPSE1, Coord_Add(Center_Coord(), XYP_Coord(-10, 3)));
						}
						if (Doing == DO_EXPLOSION_DEATH && !Class->IsDog && Height==0) {
							anim = new AnimClass(ANIM_CORPSE3, Coord_Add(Center_Coord(), XYP_Coord(-2, 4)));
						}
						if (Doing == DO_EXPLOSION2_DEATH && !Class->IsDog && Height==0) {
							anim = new AnimClass(ANIM_CORPSE2, Center_Coord());
						}
					}
					if (anim != NULL) {
						anim->OwnerHouse = House->Class->House;
					}
					delete this;
					return;
				}
		}
	}
}


/***********************************************************************************************
 * InfantryClass::Movement_AI -- This routine handles all infantry movement logic.             *
 *                                                                                             *
 *    It examines the infantry state and determines what movement action should be initiated   *
 *    or processed. It handles the actual movement of the infantry as well as any path finding *
 *    or infantry startup logic.                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per infantry unit per game logic loop.              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Movement_AI(void)
{
	/*
	**	Special hack check to ensure that infantry will never get stuck in a movement order if
	**	there is no place to go.
	*/
	if (Mission == MISSION_MOVE && !Target_Legal(NavCom)) {
		Enter_Idle_Mode();
	}

	if (!IsFiring && !IsFalling && Doing != DO_DOG_MAUL) {
		if (!IsDriving) {

			/*
			**	When in guard mode, never allow a valid navcom.
			*/
			if (Mission == MISSION_GUARD && MissionQueue == MISSION_NONE && Target_Legal(NavCom)) {
				Assign_Destination(TARGET_NONE);
//				if (IsTethered) Scatter(0, true);
			}

			/*
			**	Double check to make sure it doesn't have a movement destination into a zone
			**	that it can't travel to. In such a case, abort the movement process by clearing
			**	the navigation computer.
			*/
			if ((!IsZoneCheat || Can_Enter_Cell(Coord_Cell(Coord)) != MOVE_NO) && !IsDriving && !IsTethered && Target_Legal(NavCom) && IsLocked && Map[Coord].Zones[Class->MZone] != Map[As_Cell(NavCom)].Zones[Class->MZone]) {
// hack: if it's tanya, spy, or engineer, let 'em move there anyway.
				if (!Class->IsCapture && Mission != MISSION_ENTER) {
//				if (*this != INFANTRY_TANYA && *this != INFANTRY_SPY && *this != INFANTRY_RENOVATOR) {
					Assign_Destination(TARGET_NONE);
				}
			}

			/*
			**	A head to coordinate is needed. If there is no path
			**	available, then create one.
			*/
			if (Target_Legal(NavCom) && Strength && Mission != MISSION_GUARD) {

				/*
				**	Determine if the next cell in the list is available
				**	to be entered. If not, then abort the path and try
				**	again.
				*/
				if (Path[0] != FACING_NONE && Can_Enter_Cell(Adjacent_Cell(Coord_Cell(Center_Coord()), Path[0])) != MOVE_OK) {
					Path[0] = FACING_NONE;
				}

				/*
				**	Check to see if the target is closer than expected. This occurs
				**	when heading toward a moving object and that object is heading
				**	toward the unit. Shorten the precalculated path to be no longer
				**	than the distance to the target.
				*/
				int d = Lepton_To_Cell(Distance(NavCom));
				if (d < CONQUER_PATH_MAX) {
					Path[d] = FACING_NONE;
				}

				/*
				**	Find a path to follow if one isn't already calculated.
				*/
				if (Path[0] == FACING_NONE) {

					/*
					**	Calculate the path from the current location to the
					**	destination indicated by the navigation computer. If there
					**	was a fundamental error with finding a path, then this
					**	indicates that basic path & movement logic needs to be
					**	aborted.
					*/
					if (PathDelay != 0) {
						return;
					}
					if (!Basic_Path()) {

						/*
						**	Check to ensure that if a computer controlled unit is in
						**	hunt mode, but cannot reach the target it would like to,
						**	abort the target tracking and let the normal hunt logic
						**	assign a new one.
						*/
						if (!House->IsHuman && Mission == MISSION_HUNT) {
							Assign_Destination(TARGET_NONE);
							Assign_Target(TARGET_NONE);
						} else {

							/*
							**	If the infantry unit is close enough to the target, then
							**	tell it to stop.
							*/
							if (Distance(NavCom) < Rule.CloseEnoughDistance && !IsTethered) {
								Assign_Destination(TARGET_NONE);
							} else {

								/*
								**	Update the try try again counter so that this
								**	infantry unit will try again at a later time.
								*/
								if (TryTryAgain) {
									TryTryAgain--;
								} else {
									if (IsNewNavCom) Sound_Effect(VOC_SCOLD);
									IsNewNavCom = false;

									//If we're trying to enter a transport we need to fail so others can try to enter. - LLL 4/17/2020
									if (Mission == MISSION_ENTER) {
										Mission = MISSION_NONE;
										Assign_Mission(MISSION_GUARD);
										Commence();

										Transmit_Message(RADIO_OVER_OUT);
									}

									/*
									**	Abort the target and destination process since the path
									**	could not be found. In such a case, processing should stop
									**	or else the game will bog down with repeated path failures.
									**	Only perform the abort of the target is in a different zone.
									*/
									if ((!IsZoneCheat || Can_Enter_Cell(Coord_Cell(Coord)) != MOVE_NO) && IsLocked && Target_Legal(NavCom) && Map[As_Cell(NavCom)].Zones[Class->MZone] != Map[Coord].Zones[Class->MZone]) {
										Assign_Destination(TARGET_NONE);
									}
									if (IsLocked && Target_Legal(TarCom) && Map[As_Cell(TarCom)].Zones[Class->MZone] != Map[Coord].Zones[Class->MZone]) {
										Assign_Target(TARGET_NONE);
									}
								}
							}
						}
						Stop_Driver();
						return;
					}
					TryTryAgain = PATH_RETRY;
				}

				/*
				**	Determine the coordinate to head to based on the infantry's
				**	current location and the next location in the path.
				*/
				COORDINATE acoord = Adjacent_Cell(Coord, Path[0]);
				CELL acell = Coord_Cell(acoord);

				if (Can_Enter_Cell(acell) != MOVE_OK) {

					if ((Mission == MISSION_MOVE || Mission == MISSION_ENTER) && !IsTethered /*&& House->IsHuman*/ && Distance(NavCom) < Rule.CloseEnoughDistance) {
						Assign_Destination(TARGET_NONE);
					} else {

						/*
						** If blocked by a moving block then just exit start of move and
						** try again next tick.
						*/
						if (Can_Enter_Cell(acell) == MOVE_DESTROYABLE) {
							if (Map[acell].Cell_Object()) {
								if (!House->Is_Ally(Map[acell].Cell_Object())) {
									Override_Mission(MISSION_ATTACK, Map[acell].Cell_Object()->As_Target(), TARGET_NONE);
								}
							} else {
								if (Map[acell].Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(Map[acell].Overlay).IsWall) {
									Override_Mission(MISSION_ATTACK, ::As_Target(acell), TARGET_NONE);
								}
							}
						}
					}

					Path[0] = FACING_NONE;
					Stop_Driver();
					if (IsNewNavCom) Sound_Effect(VOC_SCOLD);
					IsNewNavCom = false;

				} else {
					if (Start_Driver(acoord)) {
						if (!IsActive) return;
						PrimaryFacing.Set(Direction8(Center_Coord(), Head_To_Coord()));
						if (IsFormationMove) {
							Set_Speed(Ground[Map[Coord].Land_Type()].Cost[FormationSpeed] * 255);
						} else {
							Set_Speed(0xFF);
						}

						if (Class->IsDog) {

							/*
							**	Dog crawl animation is actually the run animation.
							*/
							if (Target_Legal(TarCom)) {
								Do_Action(DO_CRAWL);
							} else {
								Do_Action(DO_WALK);
							}
						} else {
							if (IsProne) {
								Do_Action(DO_CRAWL);
							} else {
								Do_Action(DO_WALK);
							}
						}
					}
				}
			}

		} else {

			/*
			**	The infantry knows where it should be headed, so head there. Check
			**	to see if the infantry is "close enough" to the desired location that
			**	it should just consider itself to have arrived. In this case, force
			**	the infantry to the destination location and mark this path step
			**	as complete.
			*/
			Mark(MARK_UP);
			if (Distance(Head_To_Coord()) < 0x0010) {

				memcpy(&Path[0], &Path[1], sizeof(Path)-sizeof(Path[0]));
				Path[(sizeof(Path)/sizeof(Path[0]))-1] = FACING_NONE;
				Coord = Head_To_Coord();
				Per_Cell_Process(PCP_END);
				if (!IsActive || IsInLimbo) return;

				Stop_Driver();
				if (!IsActive || IsInLimbo) return;

				if (Coord_Cell(Coord) == As_Cell(NavCom)) {
					NavCom = TARGET_NONE;
					if (Mission == MISSION_MOVE) {
						Enter_Idle_Mode();
					}
					//Stop_Driver();
					Path[0] = FACING_NONE;
				}
			} else {
				int	movespeed = Speed;

				/*
				**	When prone, the infantry moves at half speed or double
				**	speed. This depends on whether the infantry actually has
				**	prone animation stages. Civilians don't, and so they
				**	run instead.
				*/
				if (Class->IsDog && Target_Legal(TarCom)) {
					movespeed *= 2;
				}

				if (IsProne && !Class->IsDog) {
					if ((Class->IsFraidyCat && !Class->IsCrawling) ) {
						movespeed = Speed*2;
					} else {
						movespeed = Speed/2;
					}
				}

				if (IsTethered) {
					Transmit_Message(RADIO_REDRAW);
				}

				/*
				**	Advance the infantry as far as it should go.
				*/
				MPHType maxspeed = MPHType(min(Class->MaxSpeed * SpeedBias * House->GroundspeedBias, MPH_LIGHT_SPEED));

				if (IsFormationMove) maxspeed = FormationMaxSpeed;

				Coord = Coord_Move(Coord, Direction(Head_To_Coord()), maxspeed * fixed(movespeed, 256));
			}
			Mark(MARK_DOWN);
		}
		IsNewNavCom = false;
	}
}


/***********************************************************************************************
 * InfantryClass::Get_Image_Data -- Fetches the image data for this infantry unit.             *
 *                                                                                             *
 *    The image data for the infantry differs from normal if this is a spy. A spy always       *
 *    appears like a minigunner to the non-owning players.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the image data to use for this infantry soldier.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void const * InfantryClass::Get_Image_Data(void) const
{
	if (!IsOwnedByPlayer && *this == INFANTRY_SPY) {
		return(MFCD::Retrieve("E1.SHP"));
	}
	return(TechnoClass::Get_Image_Data());
}


/***********************************************************************************************
 * InfantryClass::Is_Ready_To_Random_Anima -- Checks to see if it is ready to perform an idle  *
 *                                                                                             *
 *    This routine will examine this infantry and determine if it is allowed and ready to      *
 *    perform an idle animation. The conditions under which idle animations can be performed   *
 *    are restrictive. Hence this routine.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this infantry ready to do an idle animation?                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/01/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Is_Ready_To_Random_Animate(void) const
{
	/*
	**	See if the base classes (more rudimentary checking) determines that idle animations
	**	cannot occur. If they cannot, then return with the failure code.
	*/
	if (!FootClass::Is_Ready_To_Random_Animate()) {
		return(false);
	}

	/*
	**	While the infantry is in the air (such as when paradropping), it won't be allowed
	**	to idle animate.
	*/
	if (Height > 0) {
		return(false);
	}

	/*
	**	When the infantry is walking or otherwise engauged in travel, it won't idle animate.
	*/
	if (IsDriving) {
		return(false);
	}

	/*
	**	When prone, idle animations cannot occur. This is primarily because there are no prone
	**	idle animations.
	*/
	if (IsProne) {
		return(false);
	}

	/*
	**	When firing, the infantry should not perform any idle animations.
	*/
	if (IsFiring) {
		return(false);
	}

	/*
	**	Only if the infantry is in guard or ready stance is idle animations allowed. This is
	**	because the idle animations start and end with these frames.
	*/
	if (Doing != DO_STAND_GUARD && Doing != DO_STAND_READY) {
		return(false);
	}

	/*
	**	Since no reason was found to indicate it is not a good time to idle
	**	animate, then it must be a good time to do so.
	*/
	return(true);
}


/***********************************************************************************************
 * InfantryClass::Paradrop -- Handles paradropping infantry.                                   *
 *                                                                                             *
 *    This routine will paradrop this soldier at the location specified. It will cause the     *
 *    soldier to hunt if controlled by the computer and to guard if controlledy by the         *
 *    human.                                                                                   *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to paradrop the soldier to.                                *
 *                                                                                             *
 * OUTPUT:  bool; Was the paradrop successful?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool InfantryClass::Paradrop(COORDINATE coord)
{
	if (FootClass::Paradrop(coord)) {
		if (House->IsHuman) {
			Assign_Mission(MISSION_GUARD);
		} else {
			Assign_Mission(MISSION_HUNT);
		}
		return(true);
	}
	return(false);
}