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

/* $Header:   F:\projects\c&c\vcs\code\infantry.cpv   2.19   16 Oct 1995 16:50:30   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : August 15, 1995   [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   InfantryClass::AI -- Handles the infantry non-graphic related AI processing.              *
 *   InfantryClass::Active_Click_With -- Handles action when clicking with infantry soldier.   *
 *   InfantryClass::As_Target -- Converts the infantry unit into a target value.               *
 *   InfantryClass::Assign_Destination -- Gives the infantry a movement destination.           *
 *   InfantryClass::Assign_Mission -- Make sure he's out of boxing mode first                  *
 *   InfantryClass::Assign_Target -- Gives the infantry a combat target.                       *
 *   InfantryClass::Can_Enter_Cell -- Determines if the infantry can enter the cell specified. *
 *   InfantryClass::Can_Fire -- Can the infantry fire its weapon?                              *
 *   InfantryClass::Clear_Occupy_Bit -- Clears occupy bit and given cell                       *
 *   InfantryClass::Debug_Dump -- Displays debug information about infantry unit.              *
 *   InfantryClass::Detach -- Removes the specified target from targeting computer.            *
 *   InfantryClass::Do_Action -- Launches the infantry into an animation sequence.             *
 *   InfantryClass::Draw_It -- Draws a unit object.                                            *
 *   InfantryClass::Enter_Idle_Mode -- The infantry unit enters idle mode by this routine.     *
 *   InfantryClass::Fire_At -- Fires projectile from infantry unit.                            *
 *   InfantryClass::Fire_Coord -- Calculates the origin point for projectiles fired.           *
 *   InfantryClass::Greatest_Threat -- Determines greatest threat (target) for infantry unit.  *
 *   InfantryClass::InfantryClass -- The constructor for infantry objects.                     *
 *   InfantryClass::Init -- Initialize the infantry object system.                             *
 *   InfantryClass::Limbo -- Performs cleanup operations needed when limboing.                 *
 *   InfantryClass::Look -- The infantry performs a look operation.                            *
 *   InfantryClass::Made_A_Kill -- Marks a kill caused by this infantry soldier.               *
 *   InfantryClass::Overlap_List -- The list of cells that the infantry overlaps, but doesn't occ*
 *   InfantryClass::Per_Cell_Process -- Handles special operations that occur once per cell.   *
 *   InfantryClass::Random_Animate -- Randomly animate the infantry (maybe)                    *
 *   InfantryClass::Read_INI -- Reads units from scenario INI file.                            *
 *   InfantryClass::Rearm_Delay -- Return Arming delay for infantry if boxing                  *
 *   InfantryClass::Receive_Message -- Process radio messages                                  *
 *   InfantryClass::Response_Attack -- Plays infantry audio response to attack order.          *
 *   InfantryClass::Response_Move -- Plays infantry response to movement order.                *
 *   InfantryClass::Response_Select -- Plays infantry audio response due to being selected.    *
 *   InfantryClass::Scatter -- Causes the infantry to scatter to nearby cell.                  *
 *   InfantryClass::Set_Occupy_Bit -- Sets the occupy bit cell and bit pos	                    *
 *   InfantryClass::Set_Primary_Facing -- Change infantry primary facing -- always and instantl*
 *   InfantryClass::Start_Driver -- Handles giving immediate destination and move orders.      *
 *   InfantryClass::Stop_Driver -- Stops the infantry from moving any further.                 *
 *   InfantryClass::Take_Damage -- Applies damage to the infantry unit.                        *
 *   InfantryClass::Unlimbo -- Unlimbo infantry unit in legal sub-location.                    *
 *   InfantryClass::What_Action -- Infantry units might be able to capture -- check.           *
 *   InfantryClass::Write_INI -- Writes all the infantry out to an INI file.                   *
 *   InfantryClass::operator delete -- Returns the infantry object back to the free pool       *
 *   InfantryClass::operator new -- Allocates an infantry object from the free pool.           *
 *   InfantryClass::~InfantryClass -- Default destructor for infantry units.                   *
 *   InfantryClass::Full_Name -- Fetches the full name of the infantry unit.                   *
 *   InfantryClass::Mission_Attack -- Intercept attack mission for special handling.           *
 *   InfantryClass::Validate -- validates infantry pointer.												  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


int const InfantryClass::HumanShape[32] = {0,0,7,7,7,7,6,6,6,6,5,5,5,5,5,4,4,4,3,3,3,3,2,2,2,2,1,1,1,1,1,0};

int Infantry_Kick_Damage[]  = {10,15};
int Infantry_Punch_Damage[] = { 4, 7};

/*
** This contains the value of the Virtual Function Table Pointer
*/
void * InfantryClass::VTable;


/***************************************************************************
** This is the array of constant data associated with infantry maneuvers. It
**	specifies the frame rate as well as if the animation can be aborted.
*/
// interruptable, mobile, randomstart, rate
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
	{false,	false,	false,	2},	// DO_ON_GUARD
	{true,	false,	false,	2},	// DO_FIGHT_READY
	{false,	false,	false,	2},	// DO_PUNCH
	{false,	false,	false,	2},	// DO_KICK
	{false,	false,	false,	2},	// DO_PUNCH_HIT1
	{false,	false,	false,	2},	// DO_PUNCH_HIT2
	{false,	false,	false,	1},	// DO_PUNCH_DEATH
	{false,	false,	false,	2},	// DO_KICK_HIT1
	{false,	false,	false,	2},	// DO_KICK_HIT2
	{false,	false,	false,	1},	// DO_KICK_DEATH
	{false,	false,	false,	2},	// DO_READY_WEAPON
	{false,	false,	false,	2},	// DO_GUN_DEATH
	{false,	false,	false,	2},	// DO_EXPLOSION_DEATH
	{false,	false,	false,	2},	// DO_EXPLOSION2_DEATH
	{false,	false,	false,	2},	// DO_GRENADE_DEATH
	{false,	false,	false,	2},	// DO_FIRE_DEATH
	{false,	false,	false,	2},	// DO_GESTURE1
	{false,	false,	false,	2},	// DO_SALUTE1
	{false,	false,	false,	2},	// DO_GESTURE2
	{false,	false,	false,	2},	// DO_SALUTE2
	{true,	false,	false,	2},	// DO_PULL_GUN
	{true,	false,	false,	2},	// DO_PLEAD
	{true,	false,	false,	2},	// DO_PLEAD_DEATH
};


/***********************************************************************************************
 * InfantryClass::Validate -- validates infantry pointer.												  *
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
int InfantryClass::Validate(void) const
{
	int num;

	num = Infantry.ID(this);
	if (num < 0 || num >= INFANTRY_MAX) {
		Validate_Error("INFANTRY");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
#endif


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
void InfantryClass::Debug_Dump(MonoClass *mono) const
{
	Validate();
	mono->Set_Cursor(0, 0);mono->Print(
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
		"³Is Prone......³ ³ ³                                                           \n"
		"³Is A Loner....³ ³ ³                                                           \n"
		"³Deploying.....³ ³ ³                                                           \n"
		"³Rotating......³ ³ ³                                                           \n"
		"³Firing........³ ³ ³                                                           \n"
		"³Driving.......³ ³ ³                                                           \n"
		"³To Look.......³ ³ ³                                                           \n"
		"³Recoiling.....³ ³ ³                                                           \n"
		"³To Display....³ ³ ³                                                           \n"
		"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÁÄÙ                                                           \n");
	mono->Set_Cursor(1, 1);mono->Printf("%s:%s", House->Class->IniName, Class->IniName);
	mono->Text_Print("X", 16 + (IsProne?2:0), 10);
	mono->Set_Cursor(33, 7);mono->Printf("%2d", Fear);
	mono->Set_Cursor(41, 7);mono->Printf("%2d", Doing);
	FootClass::Debug_Dump(mono);
}
#endif

InfantryClass::InfantryClass(void) : Class(0) {};		// Default constructor does nothing.


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
	Class(&InfantryTypeClass::As_Reference(classid)),
	FootClass(house)
{

	/*
	**	For two shooters, clear out the second shot flag -- it will be set the first time
	**	the object fires. For non two shooters, set the flag since it will never be cleared
	**	and the second shot flag tells the system that normal rearm times apply -- this is
	**	what is desired for non two shooters.
	*/
	if (Class->IsTwoShooter) {
		IsSecondShot = false;
	} else {
		IsSecondShot = true;
	}
	Doing = DO_NOTHING;
	Fear = 0;			// Starts completely brave.
	IsProne = false;
	IsStoked = false;
	IsBoxing = false;
	IsTechnician = false;
	Strength = Class->MaxStrength;

	/*
	**	Civilians carry much less ammo than soldiers do.
	*/
	Ammo = Class->MaxAmmo;

	/*
	** Keep count of the number of units created. Dont track civilians.
	*/
	if (!Class->IsCivilian && GameToPlay == GAME_INTERNET){
		House->InfantryTotals->Increment_Unit_Total((int)classid);
	}

#ifdef USE_RA_AI
	//
	// Added for RA AI in TD. ST - 7/26/2019 9:12AM
	//
	House->Tracking_Add(this);
#endif // USE_RA_AI

	StopDriverFrame = -1;
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
	if (GameActive && Class) {
#ifdef USE_RA_AI
		//
		// Added for RA AI in TD. ST - 7/26/2019 9:12AM
		//
		House->Tracking_Remove(this);
#endif //USE_RA_AI
		Limbo();
	}
	if (GameActive && Team) Team->Remove(this);
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
	if (ptr) {
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
void InfantryClass::operator delete(void *ptr)
{
	if (ptr) {
		((InfantryClass *)ptr)->IsActive = false;
	}
	Infantry.Free((InfantryClass *)ptr);

	//Map.Validate();
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
ResultType InfantryClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source)
{
	Validate();
	ResultType res = RESULT_NONE;

	IsFiring = false;

	/*
	**	Prone infantry take only half damage, but never below one damage point.
	*/
	if (IsProne && damage) {
		damage >>= 1;
//		damage = MAX(damage, 1);
	}


//Mono_Printf("Infantry Take_Damage(%d, %d, %d, %p)\r", damage, distance, warhead, source);
//Get_Key();

	res = FootClass::Take_Damage(damage, distance, warhead, source);

	/*
	**	Flame thrower guys take more damage because of the exposed pilot light
	**	on their flame gun.
	*/
	if (damage && res != RESULT_DESTROYED && *this == INFANTRY_E4) {
		damage = 5;
		ResultType newres = FootClass::Take_Damage(damage, distance, warhead, source);
		res = MAX(res, newres);
	}

	if (res == RESULT_NONE) return(res);

	if (res == RESULT_DESTROYED) {
		Death_Announcement(source);
		Stop_Driver();
		Stun();
		Mission = MISSION_NONE;
		Assign_Mission(MISSION_GUARD);
		Commence();

		/*
		**	Flame thrower infantry always go out with a bang.
		*/
		if (*this == INFANTRY_E4) {
			new AnimClass(ANIM_NAPALM1, Coord);
			Explosion_Damage(Coord, 80, 0, WARHEAD_FIRE);
		}

		if (*this == INFANTRY_E2) {
			new AnimClass(ANIM_ART_EXP1, Coord);
			Explosion_Damage(Coord, 30, 0, WARHEAD_HE);
		}

		if (*this == INFANTRY_E5) {
			new AnimClass(ANIM_CHEM_BALL, Coord);
			Explosion_Damage(Coord, 80, 0, WARHEAD_HE);
		}

		VocType sound;
		VocType altsound;
		if (*this == INFANTRY_RAMBO) {
//			if (Sim_Random_Pick(0, 3) != 1) {
				sound = VOC_RAMBO_YELL;
//			} else {
//				sound = VOC_RAMBO_OHSH;
//			}
			altsound = sound;
		} else {
			sound = Sim_Random_Pick(VOC_SCREAM1, VOC_SCREAM5);
			altsound = VOC_YELL1;
		}

		/*
		**	The type of warhead determines the animation the infantry
		**	will perform when killed.
		*/
		switch (warhead) {
			case WARHEAD_FEEDME:
				if (source) {
					source->Strength += 30;
					if (source->Strength > source->Class_Of().MaxStrength) {
						source->Strength = source->Class_Of().MaxStrength;
					}
				}
				// Fall thru to WARHEAD_SA:

			case WARHEAD_HEADBUTT:
			case WARHEAD_SPORE:
			case WARHEAD_HOLLOW_POINT:
			case WARHEAD_SA:
				Sound_Effect(sound, Coord);
				Do_Action(DO_GUN_DEATH, true);
				break;

			case WARHEAD_HE:
				Sound_Effect(sound, Coord);
				Do_Action(DO_EXPLOSION_DEATH, true);
				break;

			case WARHEAD_AP:
				Sound_Effect(sound, Coord);
				Do_Action(DO_GRENADE_DEATH, true);
				break;

			case WARHEAD_PB:
			case WARHEAD_LASER:
			case WARHEAD_FIRE:
				Sound_Effect(altsound, Coord);
				Do_Action(DO_FIRE_DEATH, true);
				break;

			case WARHEAD_FIST:
				Sound_Effect(sound, Coord);
				Do_Action(DO_PUNCH_DEATH,true);
				break;

			case WARHEAD_FOOT:
				Sound_Effect(sound, Coord);
				Do_Action(DO_KICK_DEATH,true);
				break;
		}

		return(res);
	}

	/*
	**	When infantry gets hit, it gets scared.
	*/
	if (res != RESULT_DESTROYED) {
		COORDINATE c4 = (source) ? source->Coord : NULL;
		if (source) {
			Scatter(c4);
		}

#ifdef BOXING
		if (IsBoxing) {
			int addval = 0;

			switch (warhead) {
				case WARHEAD_FIST:
					if (damage == Infantry_Punch_Damage[1]) addval++;
					Do_Action( (DoType) ( (int)DO_PUNCH_HIT1 + addval),true);
					break;

				case WARHEAD_FOOT:
					if (damage == Infantry_Kick_Damage[1]) addval++;
					Do_Action( (DoType) ( (int)DO_KICK_HIT1 + addval),true);
					break;
			}
		} else {
#endif
			if (source && Fear < FEAR_SCARED) {
				if (Class->IsFraidyCat) {
					Fear = FEAR_PANIC;
				} else {
					Fear = FEAR_SCARED;
				}
			} else {
				int morefear = FEAR_ANXIOUS;
				if (Health_Ratio() > 0x0080) morefear /= 4;
				Fear = MIN((int)Fear + morefear, FEAR_MAXIMUM);
			}
#ifdef BOXING
		}
#endif
	}
	return(res);
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
 *=============================================================================================*/
void InfantryClass::Draw_It(int x, int y, WindowNumberType window)
{
	Validate();
	void const * shapefile;		// Working shape file pointer.
	int facing = Facing_To_32(PrimaryFacing.Current());

	/*
	**	Verify the legality of the unit class.
	*/
	shapefile = Class->Get_Image_Data();
	if (!shapefile) return;

	y += 4;
	x -= 2;

	/*
	**	Fetch the basic body shape pointer. This requires taking into account
	**	the current animation stage.
	*/
	int	shapenum;
	int	facenum;

	shapenum = 0;
	facenum = HumanShape[facing];

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

	shapenum = Class->DoControls[doit].Count;
	shapenum = Fetch_Stage() % MAX(shapenum, 1);
	if (Class->DoControls[doit].Jump) {
		shapenum += facenum * Class->DoControls[doit].Jump;
	}
	shapenum += Class->DoControls[doit].Frame;

#ifdef BOXING
// BG hack to get him to face right when he's supposed to.
	if (IsBoxing && BodyFacing<128) shapenum += 47;
#endif

	/*
	**	Actually draw the root body of the unit.
	*/
	Techno_Draw_Object(shapefile, shapenum, x, y, window);
//	CC_Draw_Shape(shapefile, shapenum, x, y, window, SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_GHOST, House->Remap_Table(IsBlushing, true), Map.UnitShadow);

	FootClass::Draw_It(x, y, window);
}


/***********************************************************************************************
 * InfantryClass::Per_Cell_Process -- Handles special operations that occur once per cell.     *
 *                                                                                             *
 *    This routine will handle any special operations that need to be performed once each      *
 *    cell travelled. This includes radioing a transport that is is now clear and the          *
 *    transport is free to leave.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
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
void InfantryClass::Per_Cell_Process(bool center)
{
	Validate();
	CellClass *cellptr = &Map[Coord_Cell(Coord)];

	/*
	**	If the infantry unit is entering a cell that contains the building it is trying to
	**	capture, then capture it.
	*/
	if (center && Mission == MISSION_CAPTURE) {
		TechnoClass * tech = cellptr->Cell_Techno();
		if (tech && tech->As_Target() == NavCom) {
			tech->Captured(House);
			Delete_This();
			return;
		} else {
//#ifdef NEVER
			if (!Target_Legal(NavCom)) {
				Enter_Idle_Mode();
				if (Map[Coord_Cell(Coord)].Cell_Building()) {
					Scatter(0, true);
				}
			}
//#endif
		}
	}

	/*
	**	Infantry entering a transport vehicle will break radio contact
	**	at attach itself to the transporter.
	*/
	TechnoClass * techno = Contact_With_Whom();
	if (center && Mission == MISSION_ENTER && techno && Coord_Cell(Coord) == Coord_Cell(techno->Coord) && techno == As_Techno(NavCom)) {
		if (Transmit_Message(RADIO_IM_IN) == RADIO_ATTACH) {
			Limbo();
			techno->Attach(this);
		}
		return;
	}

	/*
	**	If the infantry unit is entering a cell that contains the building it is trying to
	**	sabotage, then sabotage it.
	*/
	if (center && Mission == MISSION_SABOTAGE) {
		BuildingClass *building = cellptr->Cell_Building();
		if (building && building->As_Target() == NavCom) {
			int temp = Special.IsScatter;

			building->IsGoingToBlow = true;
			building->Clicked_As_Target(PlayerPtr->Class->House, 20); // 2019/09/20 JAS - Added record of who clicked on the object
			building->Clicked_As_Target(building->Owner(), 20);
			building->CountDown.Set(20);
			building->WhomToRepay = As_Target();
			Special.IsScatter = true;
			NavCom = TARGET_NONE;
			Do_Uncloak();
			Arm = Rearm_Delay(true);
			Scatter(building->Center_Coord(), true);	// RUN AWAY!
			Special.IsScatter = temp;
			return;
		}
	}

	/*
	**	If this unit is on a teather, then cut it at this time so that
	**	the "parent" unit is free to proceed. Note that the parent
	**	unit might actually be a building.
	*/
	if (center && IsTethered) {
		Transmit_Message(RADIO_UNLOADED);
		if (House->Class->House == HOUSE_GOOD) {
			Do_Action(DO_GESTURE1);
		} else  {
			Do_Action(DO_GESTURE2);
		}

		/*
		**	Rambo types give a gung-ho comment when unloaded.
		*/
		if (*this == INFANTRY_RAMBO) {
			Sound_Effect(VOC_RAMBO_ROCK, Coord);
		}

		/*
		**	If the cell is now full of infantry, tell them all to scatter
		**	in order to make room for more.
		*/
		if ((cellptr->Flag.Composite & 0x01F) == 0x01F) {
			cellptr->Incoming(0, true);
		}
	}

	/*
	**	When the infantry reaches the center of the cell, it may begin a new mission.
	*/
	if (center) {
		Commence();
	}

	Look(true);
	FootClass::Per_Cell_Process(center);

	/*
	**	If over Tiberium, then this infantry unit will take damage.
	*/
	if (IsActive && !IsInLimbo && center && cellptr->Land_Type() == LAND_TIBERIUM && *this != INFANTRY_E5) {
		int damage = 2;
		Take_Damage(damage, 0, WARHEAD_FIRE);
	}
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
	Validate();
	if (TarCom == target) {
		IsFiring = false;
	}
	FootClass::Detach(target, all);
}


/***********************************************************************************************
 * InfantryClass::As_Target -- Converts the infantry unit into a target value.                 *
 *                                                                                             *
 *    This support routine is used to convert the infantry object (as a pointer) into a target *
 *    value (which is a number).                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the infantry unit as a target value.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET InfantryClass::As_Target(void) const
{
	Validate();
	return(Build_Target(KIND_INFANTRY, Infantry.ID(this)));
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
	InfantryClass *ptr;

	Infantry.Free_All();

	ptr = new InfantryClass();
	VTable = ((void **)(((char *)ptr) + sizeof(AbstractClass) - 4))[0];
	delete ptr;
}


/***********************************************************************************************
 * InfantryClass::Look -- The infantry performs a look operation.                              *
 *                                                                                             *
 *    This routine will cause the infantry unit to "look". For player owned infantry, this     *
 *    causes the dark shroud to be pushed back.                                                *
 *                                                                                             *
 * INPUT:   incremental -- If it is known that the infantry performed a look in the last cell  *
 *                         it was in AND it has only moved one cell, then setting this         *
 *                         parameter to true will perform a faster "incremental" look          *
 *                         operation.                                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This is a relatively slow routine. Call ONLY when necessary.                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Look(bool incremental)
{
	Validate();
	int	sight;		// Number of cells to sight.

	if (!IsInLimbo) {
		//if (IsOwnedByPlayer) {			// Changed for multiple player mapping. ST - 3/6/2019 1:27PM
		if (House->IsHuman) {
			sight = Class->SightRange;

			if (sight) {
				Map.Sight_From(House, Coord_Cell(Coord), sight, incremental);		// Passed our house into Map.Sight_From since it now needs to know who it is performing the action on behalf of. ST - 3/28/2019 2:55PM
			}
		}
	}
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
	Validate();
	/*
	**	Special flag so that infantry will start heading in the right direction immediately.
	*/
	if (Target_Legal(target)) {
		Stop_Driver();
	}

	/*
	**	When telling an infantry soldier to move to a location twice, then this
	**	means that movement is more important than safety. Get up and run!
	*/
	if (House->IsHuman && Target_Legal(target) && NavCom == target && IsProne && !Class->IsFraidyCat) {
		Do_Action(DO_GET_UP);
	}

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

			if (techno) {

				/*
				**	Determine if the transport is already in radio contact. If so, then just move
				**	toward the transport and try to establish contact at a later time.
				*/
				if (techno->In_Radio_Contact()) {

// TCTCTC -- call for an update from the transport to get a good rondezvous position.

					ArchiveTarget = target;
				} else {
					if (Transmit_Message(RADIO_HELLO, techno) == RADIO_ROGER) {
						if (Transmit_Message(RADIO_DOCKING) != RADIO_ROGER) {
							Transmit_Message(RADIO_OVER_OUT);
							Assign_Mission(MISSION_MOVE);
						}
					} else {
						Assign_Mission(MISSION_MOVE);
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
	Validate();
	Path[0] = FACING_NONE;
	FootClass::Assign_Target(target);

	/*
	**	If this is an infantry that can only capture, then also assign its destination to the
	**	target specified.
	*/
	if (!Target_Legal(NavCom) && Class->IsCapture && Class->Primary == WEAPON_NONE) {
		BuildingClass const * building = As_Building(target);
		if (building && building->Class->IsCaptureable && (GameToPlay != GAME_NORMAL || *building != STRUCT_EYE && Scenario < 13)) {
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
 *=============================================================================================*/
void InfantryClass::AI(void)
{
	Validate();
	FootClass::AI();

	if (IsUnloading) Mark(MARK_CHANGE);

	/*
	**	Special hack to make sure that if this infantry is in firing animation, but the
	**	stage class isn't set, then abort the firing flag.
	*/
	if (IsFiring && !Fetch_Rate()) {
		IsFiring = false;
	}

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
	**	Act on new orders if the unit is at a good position to do so.
	*/
	if (!IsDriving && (Doing == DO_NOTHING || MasterDoControls[Doing].Interrupt)) {
		Commence();
	}

	/*
	**	After a time, the infantry will gain courage.
	*/
	if (Fear) {

		/*
		**	Nikumba is really a coward at heart. He never becomes un-afraid.
		*/
		if (*this != INFANTRY_C10) {
			Fear--;

			/*
			**	When an armed civilian becomes unafraid, he will then reload
			**	another clip into his pistol.
			*/
			if (Fear == 0 && Ammo == 0 && Class->Primary != WEAPON_NONE) {
				Ammo = Class->MaxAmmo;
			}
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
			if (Fear >= FEAR_ANXIOUS && ((!Target_Legal(NavCom) && !IsDriving) || !Special.IsDefenderAdvantage)) {
				Do_Action(DO_LIE_DOWN);
			}
		}
	}

	/*
	**	When is darkness or in doubt,
	**		run in circles, scream, and shout.
	*/
	if (Class->IsFraidyCat && Fear > FEAR_ANXIOUS && !IsDriving && !Target_Legal(NavCom)) {
		Scatter(true);
	}

	/*
	**	Special victory dance action.
	*/
	if (!Target_Legal(NavCom) && !IsProne && IsStoked && Comment.Expired()) {
		IsStoked = false;
		Do_Action((Random_Pick(0, 1) == 0) ? DO_GESTURE1 : DO_GESTURE2);
		if (*this == INFANTRY_RAMBO) {
			VocType _response[] = {
				VOC_RAMBO_LEFTY,
				VOC_RAMBO_LAUGH,
				VOC_RAMBO_COMIN,
				VOC_RAMBO_TUFF
			};
			Sound_Effect(_response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)], Coord);
		}
	}

	/*
	**	Determine if this infantry unit should fire off an
	**	attack or not.
	*/
	switch (Can_Fire(TarCom, 0)) {
		case FIRE_CLOAKED:
			Do_Uncloak();
			break;

		case FIRE_OK:
#ifdef BOXING
			ObjectClass *object = As_Object(TarCom);

			if (object) {
				/* If we're engaged in hand-to-hand combat, keep boxing */
				if (IsBoxing) {
					IsFiring = true;
					if (((InfantryClass *)object)->Doing == DO_FIGHT_READY) {
						Do_Action((DoType) ((int)DO_PUNCH + (DoType)(Random_Pick(0, 1) == 1)),true);
					}
				} else {

					if (Is_Target_Infantry(TarCom) && (Distance(TarCom)<=0x80) && (Coord_Y(Coord) == Coord_Y(object->Coord))) {

						// Too close to shoot, so start hand-to-hand combat
						if (Establish_Contact((TechnoClass *)object)) {
							if (Transmit_Message(RADIO_PREPARE_TO_BOX) == RADIO_ROGER) {
								IsBoxing = true;
								Do_Action(DO_ON_GUARD,true);
							}
						}
					} else {
#endif

						/*
						**	Start firing animation.
						*/
						if (IsProne) {
							Do_Action(DO_FIRE_PRONE);
							IsFiring = true;
						} else {
							Do_Action(DO_FIRE_WEAPON);
							IsFiring = true;
						}
#ifdef BOXING
					}
#endif

					PrimaryFacing.Set(Direction8(Center_Coord(), As_Coord(TarCom)));

					/*
					**	If the target is in range, and the NavCom is the same, then just
					**	stop and keep firing.
					*/
					if (TarCom == NavCom) {
						NavCom = TARGET_NONE;
						Path[0] = FACING_NONE;
					}
#ifdef BOXING
				}
			}
#endif
			break;
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

#ifdef BOXING
	if (IsBoxing) {
		firestage = 1;
		if (Doing == DO_KICK) firestage = 2;
	}
#endif

	if (IsFiring && Fetch_Stage() == firestage) {
		Fire_At(TarCom, 0);

		if (Class->Primary == WEAPON_GRENADE) {
			Map[::As_Cell(TarCom)].Incoming(Coord, true);
		}
	}

	/*
	**	Handle the completion of the animation sequence.
	*/

	if (Doing == DO_NOTHING || Fetch_Stage() >= Class->DoControls[Doing].Count) {
		switch (Doing) {
			default:
				if (IsDriving) {
					if (IsProne) {
						Do_Action(DO_CRAWL, true);
					} else {
						Do_Action(DO_WALK, true);
					}
				} else {
					if (IsProne) {
						Do_Action(DO_PRONE, true);
					} else {
						Do_Action(DO_STAND_READY, true);
					}
				}
				break;

#ifdef BOXING
			case DO_FIGHT_READY:
			case DO_ON_GUARD:
			case DO_PUNCH:
			case DO_KICK:
			case DO_PUNCH_HIT1:
			case DO_PUNCH_HIT2:
			case DO_KICK_HIT1:
			case DO_KICK_HIT2:
				if (In_Radio_Contact()) {
					Do_Action(DO_FIGHT_READY, true);
				} else {
					IsBoxing = false;
					Do_Action(DO_READY_WEAPON);
				}
				break;
#endif

			/*
			**	When death is due to hand-to-hand combat, use the gunfire death
			**	decay animation since there is no custom animation available - yet.
			*/
			case DO_PUNCH_DEATH:
			case DO_KICK_DEATH:
				// Fall into next case.

			case DO_GUN_DEATH:
			case DO_EXPLOSION_DEATH:
			case DO_EXPLOSION2_DEATH:
			case DO_GRENADE_DEATH:
			case DO_FIRE_DEATH:
				Delete_This();
				return;
		}
	}

	/*
	**	Perform movement operations at this time.
	*/
	if (!IsFiring /*&& !IsBoxing*/) {
		if (!IsDriving) {

			/*
			**	When in guard mode, never allow a valid navcom.
			*/
			if (Mission == MISSION_GUARD && MissionQueue == MISSION_NONE && Target_Legal(NavCom)) {
				Assign_Destination(TARGET_NONE);
//				if (IsTethered) Scatter(0, true);
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
					if (!PathDelay.Expired()) {
						return;
					}
					if (!Basic_Path()) {
//Mono_Printf("Infantry Basic_Path is failing.\n");Get_Key();
						if (Distance(NavCom) < 0x0280 && !IsTethered) {
							Assign_Destination(TARGET_NONE);
						} else {
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

					if ((Mission == MISSION_MOVE || Mission == MISSION_ENTER) && !IsTethered && House->IsHuman && Distance(NavCom) < 0x0200) {
						Assign_Destination(TARGET_NONE);
					}

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

					Path[0] = FACING_NONE;
					Stop_Driver();
					if (IsNewNavCom) Sound_Effect(VOC_SCOLD);
					IsNewNavCom = false;

				} else {
					if (Start_Driver(acoord)) {
						PrimaryFacing.Set(Direction8(Center_Coord(), Head_To_Coord()));
						Set_Speed(0xFF);
						if (IsProne) {
							Do_Action(DO_CRAWL);
						} else {
							Do_Action(DO_WALK);
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
				Stop_Driver();
				Per_Cell_Process(true);

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
				if (IsProne) {
					if (Class->IsFraidyCat && !Class->IsCrawling) {
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
				MPHType maxspeed = MPHType(min((unsigned)(Class->MaxSpeed * House->GroundspeedBias), MPH_LIGHT_SPEED));
				Coord = Coord_Move(Coord, Direction(Head_To_Coord()), Fixed_To_Cardinal(maxspeed, movespeed));
			}
			Mark(MARK_DOWN);
		}
		IsNewNavCom = false;
	}
}


#ifdef NEVER
/***************************************************************************
 * InfantryClass::Blocking_Object -- Determines how a object blocks an inf *
 *                                                                         *
 * INPUT:		TechnoClass * pointer to object that is blocking unit       *
 *					CELL			  the cell the unit is being blocked in			*
 *                                                                         *
 * OUTPUT:		MoveBitType the way that the object is blocking the unit    *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/08/1995 PWG : Created.                                             *
 *=========================================================================*/
MoveBitType InfantryClass::Blocking_Object(TechnoClass const *techno, CELL cell) const
{
	Validate();
	bool inf  = (techno->What_Am_I() == RTTI_INFANTRY);
	bool unit = (techno->What_Am_I() == RTTI_UNIT) || inf;

	CellClass const * cellptr = &Map[cell];

	if (House->Is_Ally(techno)) {

		/*
		** Logic to handle a trasport type object.
		*/
		if (NavCom == techno->As_Target() && Contact_With_Whom() == techno) {
			return(MOVEF_OK);
		}

		/*
		** If the object is of type infantry, then the square is blocked only
		** if the cell is completely full of infantry.
		*/
		if (inf && ((cellptr->Flag.Composite & 0x1f) != 0x1f)) {
			return(MOVEF_OK);
		}

		if (unit) {

			/*
			** If the unit in question has a destination than we should
			** be prepared to wait for the unit to get out of our way.
			*/
			if (((FootClass *)techno)->NavCom != TARGET_NONE) {
				return(MOVEF_MOVING_BLOCK);
			}
			return(MOVEF_TEMP);
		}
	} else {

		/*
		** If its an enemy unit, things are dealt with a little differently
		*/
		if (unit) {


			/*
			**	If the object is cloaked, then consider it passable for findpath purposes,
			**	but not so for all other cases.
			*/
			if (techno->Cloak == CLOAKED) {
				if (IsFindPath) return(MOVEF_OK);
				return(MOVEF_CLOAK);
			}

			/*
			**	If our vehicle is weapon equipped, then report that the cell occupier
			**	needs only to be destroyed in order to make the cell passable.
			*/
			if (Class->Primary != WEAPON_NONE) {
				return(MOVEF_DESTROYABLE);
			}
		}
	}
	return(MOVEF_NO);
}
#endif



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
	Validate();
	/*
	** If we are moving into an illegal cell, then we can't do that.
	*/
	if ((unsigned)cell >= MAP_CELL_TOTAL) return(MOVE_NO);

	/*
	**	If moving off the edge of the map, then consider that an illegal move.
	*/
	if (IsLocked && !IsALoaner && !ScenarioInit && !Map.In_Radar(cell)) {
		return(MOVE_NO);
	}
	CellClass * cellptr = &Map[cell];

	/*
	**	Walls are considered impassable for infantry UNLESS the wall has a hole
	**	in it.
	*/
	if (cellptr->Overlay != OVERLAY_NONE) {
		OverlayTypeClass const & otype = OverlayTypeClass::As_Reference(cellptr->Overlay);

		if (otype.IsCrate && !House->IsHuman) {
			return(MOVE_NO);
		}

		if (otype.IsWall) {
			if ((cellptr->OverlayData / 16) != otype.DamageLevels) {
				return(MOVE_NO);
			}
		}
	}

	/*
	** Loop through all of the objects in the square setting a bit
	** for how they affect movement.
	*/
	MoveType retval = MOVE_OK;
	ObjectClass *obj = cellptr->Cell_Occupier();
	while (obj) {

		if (obj != this) {

			/*
			**	Special case check so that a landed aircraft that is in radio contact, will not block
			**	a capture attempt. It is presumed that this case happens when a helicopter is landed
			**	at a helipad.
			*/
			if ((Mission != MISSION_CAPTURE && Mission != MISSION_SABOTAGE) || obj->What_Am_I() != RTTI_AIRCRAFT || !((AircraftClass *)obj)->In_Radio_Contact()) {

				/*
				**	Special check to always allow entry into the building that this infantry
				**	is trying to capture.
				*/
				if (obj->What_Am_I() == RTTI_BUILDING || obj->What_Am_I() == RTTI_AIRCRAFT) {
					if ((Mission == MISSION_CAPTURE || Mission == MISSION_SABOTAGE) && (obj->As_Target() == NavCom || obj->As_Target() == TarCom)) {
						return(MOVE_OK);
					}
				}

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
				if (House->Is_Ally(obj)) {
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
						if (Class->Primary == WEAPON_NONE) return(MOVE_NO);

						/*
						**	Some kinds of terrain are considered destroyable if the infantry is equipped
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
						if (retval < MOVE_CLOAK) retval = MOVE_CLOAK;
					}
				}
			}
		}

		/*
		**	Move to next object in chain.
		*/
		obj = obj->Next;
	}

	/*
	**	If foot soldiers cannot travel on the cell -- consider it impassible.
	*/
	if (retval == MOVE_OK && !IsTethered && !Ground[cellptr->Land_Type()].Cost[SPEED_FOOT]) {
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
			if (Class->Primary != WEAPON_NONE) {
				if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE;
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
short const * InfantryClass::Overlap_List(void) const
{
	Validate();
	//return(Coord_Spillage_List(Coord, 24 + ((IsSelected || Doing > DO_WALK)?12:0)));
	return(Coord_Spillage_List(Coord, 24 + ((Doing > DO_WALK || Is_Selected_By_Player())?12:0)));
//	return(Coord_Spillage_List(Coord, (IsSelected ? 24 : 14))+1);
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
	Validate();
	WeaponTypeClass const * weapon = (which == 0) ? &Weapons[Class->Primary] : &Weapons[Class->Secondary];

#ifdef BOXING
	/*
	** If in hand-to-hand, and we're currently playing a got-hit animation,
	** then we can't punch back yet.
	*/
	if (IsBoxing) {
		if ( (Doing>=DO_PUNCH_HIT1 && Doing<=DO_KICK_DEATH) || (Doing == DO_ON_GUARD) ) return FIRE_BUSY;
		if (Arm) return(FIRE_BUSY);	// don't let fire if still re-arming
	}
#endif

	/*
	**	Don't allow firing if the turret is not ready.
	*/
	if (IsFiring) return(FIRE_REARM);

#ifdef OBSOLETE
	if (weapon->Fires == BULLET_FLAME && IsProne) return(FIRE_ILLEGAL);
#endif

	/*
	**	The target must still be legal.
	*/
	if (!Target_Legal(target)) {
		return(FIRE_ILLEGAL);
	}

	/*
	**	If this unit cannot fire while moving, then bail.
	*/
	if ((IsDriving && Special.IsDefenderAdvantage) || (Doing != DO_NOTHING && !MasterDoControls[Doing].Interrupt)) {
		return(FIRE_MOVING);
	}

	/*
	** If we're moving, but not facing the right direction, then exit.
	*/
	if (!Special.IsDefenderAdvantage && IsDriving) {
		int diff = PrimaryFacing.Difference(Direction(TarCom));
		if (ABS(diff) >= 32) {
			return(FIRE_MOVING);
		}
	}

	return(FootClass::Can_Fire(target, which));
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
	Validate();
	MissionType order;

	if (Target_Legal(TarCom)) {
		order = MISSION_ATTACK;
	} else {
		if (Target_Legal(NavCom)) {
			order = MISSION_MOVE;
		} else {
			if (GameToPlay == GAME_NORMAL || House->IsHuman) {
				order = MISSION_GUARD;
			} else {
#ifndef USE_RA_AI
				order = MISSION_HUNT;

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
void InfantryClass::Random_Animate(void)
{
	Validate();
	if (!IsDriving && !IsProne && (Doing == DO_STAND_GUARD || Doing == DO_STAND_READY) && !IsFiring) {

		/*
		**	Scared infantry will always follow the golden rule of civilians;
		**		"When in darkness or in doubt, run in circles, scream, and shout!"
		*/
		if (Class->IsFraidyCat && !House->IsHuman && Fear > FEAR_ANXIOUS) {
			Scatter(NULL, true);
			return;
		}

		/*
		**	If Nikoomba is not scared, then he will be doing his thing with random animations.
		*/
		if (*this == INFANTRY_C10) {
			switch (Random_Pick(0, 3)) {
				case 0:
					Do_Action(DO_IDLE2);
					break;

				default:
					break;
			}
		}

		switch (Random_Picky((int)0, (int)55, (char*)NULL, (int)0)) {
			case 10:
				Do_Action(DO_SALUTE1);
				break;

			case 11:
				Do_Action(DO_SALUTE2);
				break;

			case 12:
				Do_Action(DO_GESTURE1);
				break;

			case 13:
				Do_Action(DO_GESTURE2);
				break;

			case 4:
			case 3:
			case 0:
				PrimaryFacing.Set(Facing_Dir(Random_Pick(FACING_N, FACING_NW)));
				Mark(MARK_CHANGE);
				break;

			case 1:
				Do_Action(DO_IDLE1);
				PrimaryFacing.Set(Facing_Dir(Random_Pick(FACING_N, FACING_NW)));
				if (Sim_Random_Pick(1,20) == 1 && !Is_Selected_By_Player() && *this == INFANTRY_MOEBIUS && IsDiscoveredByPlayer) {
					static VocType _response[] = {
//						VOC_EXCELLENT1,
//						VOC_EXCELLENT2,
						VOC_EXCELLENT3,
//						VOC_EXCELLENT4,
//						VOC_EXCELLENT5,
						VOC_QUIP1,
//						VOC_QUIP2
					};
					Sound_Effect(_response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)], Coord);
				}
				break;

			case 2:
				Do_Action(DO_IDLE2);
				PrimaryFacing.Set(Facing_Dir(Random_Pick(FACING_N, FACING_NW)));
				if (!Is_Selected_By_Player() && IsOwnedByPlayer && *this == INFANTRY_RAMBO && Sim_Random_Pick(0, 2) == 0) {
					Sound_Effect(VOC_RAMBO_CMON, Coord);
				}
				break;

			/*
			**	On occasion, civilian types will wander about.
			*/
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				if (!House->IsHuman && Class->IsFraidyCat) {
					Scatter(NULL, true);
				}
				break;
		}
	}
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
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *   12/12/1994 JLB : Flame thrower infantry always scatter.                                   *
 *=============================================================================================*/
void InfantryClass::Scatter(COORDINATE threat, bool forced, bool nokidding)
{
	Validate();

	/*
	**	A unit that is in the process of going somewhere will never scatter.
	*/
	if (IsDriving || Target_Legal(NavCom)) forced = false;

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
	if (!Special.IsScatter && !nokidding && House->IsHuman && !forced && !Team) return;

	if (forced || Class->IsFraidyCat /*|| !(Random_Pick(1, 4) == 1)*/) {
		FacingType	toface;
		FacingType	newface;
		CELL			newcell;

		if (threat) {
			toface = Dir_Facing(Direction8(threat, Coord));
			toface = toface + (Random_Pick(0, 4)-2);
		} else {
			COORDINATE	coord = Coord & 0x00FF00FFL;

			if (coord != 0x00800080L) {
				toface = Dir_Facing((DirType)Desired_Facing8(0x0080, 0x0080, Coord_X(coord), Coord_Y(coord)));
			} else {
				toface = Dir_Facing(PrimaryFacing.Current());
			}
			toface = toface + (Random_Pick(0, 4)-2);
		}

		for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
			newface = toface + face;
			newcell = Adjacent_Cell(Coord_Cell(Coord), newface);

			if (Map.In_Radar(newcell) && Can_Enter_Cell(newcell) == MOVE_OK) {
				Assign_Mission(MISSION_MOVE);
				Assign_Destination(::As_Target(newcell));
			}
		}
	}
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
	Validate();
	if (todo != Doing && (Doing == DO_NOTHING || force || MasterDoControls[Doing].Interrupt)) {
		Mark(MARK_CHANGE);
		//Mark(MARK_OVERLAP_UP);
		Doing = todo;
		//Mark(MARK_OVERLAP_DOWN);
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
		if (!Strength) {
			Stop_Driver();
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

			case DO_READY_WEAPON:
				IsBoxing = false;
				break;

			case DO_ON_GUARD:
				IsBoxing = true;
				PrimaryFacing.Set(Direction8(Center_Coord(), As_Coord(TarCom)));
				Path[0] = FACING_NONE;
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
	Validate();
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

	if (IsProne) {
		Do_Action(DO_PRONE);
	} else {
		Do_Action(DO_STAND_READY);
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
	Validate();
	COORDINATE old = headto;

	/*
	**	Convert the head to coordinate to a legal sub-position location.
	*/
	headto = Map[Coord_Cell(headto)].Closest_Free_Spot(Coord_Move(headto, Direction(headto)+DIR_S, 0x007C));
	if (!headto && Can_Enter_Cell(Coord_Cell(old)) == MOVE_OK) {
		headto = Map[Coord_Cell(old)].Closest_Free_Spot(Coord_Move(old, Direction(headto)+DIR_S, 0x0080), true);
	}

	/*
	**	If the infantry started moving, then fixup the occupation bits.
	*/
	if (headto && FootClass::Start_Driver(headto)) {
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


#ifdef NEVER
/***********************************************************************************************
 * InfantryClass::Set_Primary_Facing -- Change infantry primary facing -- always and instantly *
 *                                                                                             *
 *    This routine ensures that when the infantry primary facing is changes, it is changed     *
 *    instantly and always. There is no provision for infantry facing changing slowly over     *
 *    time as the other vehicles usually do.                                                   *
 *                                                                                             *
 * INPUT:   facing   -- The desired facing for the infantry unit.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/11/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Set_Primary_Facing(DirType facing, bool )
{
	Validate();
	FootClass::Set_Primary_Facing(facing, true);
}
#endif


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
	Validate();
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
	Validate();
	BulletClass * bullet = NULL;
	WeaponTypeClass const * weapon = (which == 0) ? &Weapons[Class->Primary] : &Weapons[Class->Secondary];

	IsFiring = false;

#ifdef BOXING
	if (IsBoxing) {
		RadioMessageType	hitaction = (Doing == DO_KICK) ? RADIO_KICK : RADIO_PUNCH;

		/*
		**	When fighting, verify that the target is legal to proceed. If there is some
		**	error, then abort fightning mode. Otherwise, tell the target that it has
		**	just been hit.
		*/
		if (In_Radio_Contact() && Target_Legal(target) && Transmit_Message(hitaction) == RADIO_ROGER) {
			Arm = Rearm_Delay(true);
		} else {

			/*
			** Fighting done for some reason, so pick up gun
			*/
			IsBoxing = false;
			Do_Action(DO_READY_WEAPON,true);
		}
	} else {
#endif

		bullet = FootClass::Fire_At(target, which);
		if (bullet) {

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

			/*
			**	Create the projectile. Then process any special operations that
			**	need to be performed according to the style of projectile
			**	created.
			*/
			Sound_Effect(weapon->Sound, Coord);
		}

#ifdef BOXING
	}
#endif
	return(bullet);
}


/***********************************************************************************************
 * InfantryClass::Unlimbo -- Unlimbo infantry unit in legal sub-location.                      *
 *                                                                                             *
 *    This will attempt to unlimbo the infantry unit at the designated coordinate, but will    *
 *    ensure that the coordinate is a legal subposition.                                       *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate to unimbo the infantry at.                              *
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
	Validate();
	/*
	**	Make sure that the infantry start in a legal position on the map.
	*/
	coord = Map[Coord_Cell(coord)].Closest_Free_Spot(coord, ScenarioInit);
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
 *=============================================================================================*/
TARGET InfantryClass::Greatest_Threat(ThreatType threat) const
{
	Validate();
	/*
	**	Engineers consider only buildings that can be captures as being a threat. All others
	**	are ignored.
	*/
	if (Class->IsCapture && Class->Primary == WEAPON_NONE) {
		threat = threat | THREAT_CAPTURE;
	}

	switch (Class->Primary) {
		case WEAPON_NONE:
			if (*this != INFANTRY_E7) {
				return(TARGET_NONE);
			}
			// fall into next case.

			//
			// ^
			// | Original comment above.
			//
			// Crash here because INFANTRY_E7 has no primary weapon and WEAPON_NONE == -1, so we will reference a -ve index into the Weapons array.
			// Assume original intent was to fall out of the switch and call the base class Greatest_Threat, so adding a break here.
			// ST - 4/24/2019 11:01AM
			//
			break;		                   // Added. ST - 4/24/2019 11:02AM

		/*
		**	Dragon missile equiped soldiers are also assumed to carry a Stinger missile. As such,
		**	they will consider aircraft a legal target.
		*/
		default:
			if (BulletTypeClass::As_Reference(Weapons[Class->Primary].Fires).IsAntiAircraft) {
				threat = threat | THREAT_AIR;
			}
			break;

		/*
		**	The sniper rifle equipped soldier doesn't go hunting for targets
		**	unless specifically in hunt mode.
		*/
		case WEAPON_RIFLE:
			if (House->IsHuman && (Mission == MISSION_GUARD || Mission == MISSION_GUARD_AREA)) {
				return(TARGET_NONE);
			}
			return(TechnoClass::Greatest_Threat(threat | THREAT_INFANTRY|THREAT_BUILDINGS));
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
	Validate();
	VocType response;
	if (*this == INFANTRY_RAMBO) {
		static VocType _response[] = {
			VOC_RAMBO_YEA,
			VOC_RAMBO_YES,
			VOC_RAMBO_YO
		};
		response = _response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)];
	} else {
		if (Class->IsCivilian) {
			if (*this == INFANTRY_MOEBIUS) {
				static VocType _response[] = {
					VOC_YES,
					VOC_COMMANDER,
					VOC_HELLO,
					VOC_HMMM
				};
				response = _response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)];
			} else {
				if (Class->IsFemale) {
					response = VOC_GIRL_YEAH;
				} else {
					response = VOC_GUY_YEAH;
				}
			}
		} else {
			static VocType _response[] = {
				VOC_ACKNOWL,
				VOC_REPORT,
				VOC_REPORT,
				VOC_YESSIR,
				VOC_YESSIR,
				VOC_READY,
				VOC_AWAIT
			};
			response = _response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)];
		}
	}
	if (AllowVoice) {
		Sound_Effect(response, 0, Infantry.ID(this)+1);
	}
}


/***********************************************************************************************
 * InfantryClass::Response_Move -- Plays infantry response to movement order.                  *
 *                                                                                             *
 *    When the infantry is given the order to move, this routine handles the audio repsonse    *
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
	Validate();
	VocType response;
	if (*this == INFANTRY_RAMBO) {
		static VocType _response[] = {
			VOC_RAMBO_UGOTIT,
			VOC_RAMBO_ONIT,
			VOC_RAMBO_NOPROB
		};
		response = _response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)];
	} else {
		if (Class->IsCivilian) {
			if (*this == INFANTRY_MOEBIUS) {
				static VocType _response[] = {
					VOC_OF_COURSE,
					VOC_YESYES
				};
				response = _response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)];
			} else {
				if (Class->IsFemale) {
					response = VOC_GIRL_OKAY;
				} else {
					response = VOC_GUY_OKAY;
				}
			}
		} else {
			static VocType _response[] = {
				VOC_MOVEOUT,
				VOC_MOVEOUT,
				VOC_MOVEOUT,
				VOC_ROGER,
				VOC_RIGHT_AWAY,
				VOC_UGOTIT,
				VOC_AFFIRM,
				VOC_AFFIRM
			};
			response = _response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)];
		}
	}
	if (AllowVoice) {
		Sound_Effect(response, 0, Infantry.ID(this)+1);
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
	Validate();
	VocType response;
	if (*this == INFANTRY_RAMBO) {
		static VocType _response[] = {
			VOC_RAMBO_NOPROB,
			VOC_RAMBO_UGOTIT,
			VOC_RAMBO_NOPROB,
			VOC_RAMBO_ONIT
		};
		response = _response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)];
	} else {
		if (Class->IsCivilian) {
			if (Class->IsFemale) {
				response = VOC_GIRL_OKAY;
			} else {
				response = VOC_GUY_OKAY;
			}
		} else {
			static VocType _response[] = {
				VOC_RIGHT_AWAY,
				VOC_AFFIRM,
				VOC_AFFIRM,
				VOC_UGOTIT,
				VOC_NO_PROB,
				VOC_YESSIR,
				VOC_YESSIR,
				VOC_YESSIR
			};
			response = _response[Sim_Random_Pick(0, (int)(sizeof(_response) / sizeof(_response[0]))-1)];
		}
	}

	if (AllowVoice) {
		Sound_Effect(response, 0, Infantry.ID(this)+1);
	}
}


/***********************************************************************************************
 * InfantryClass::Fire_Coord -- Calculates the origin point for projectiles fired.             *
 *                                                                                             *
 *    This routine will return with the point of origin for any firing projectiles. Typically, *
 *    this only includes the rocket launcher and the grenade thrower. The other infantry       *
 *    have either invisible projectiles or special animations.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate where the projectile will appear as it is fired.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE InfantryClass::Fire_Coord(int) const
{
	Validate();
	if (Class->Type == INFANTRY_E4) {
		return(Coord);	// special case for flame thrower guy
	} else {
		return(Coord_Add(Coord, XYP_COORD(0, -5)));
	}
}


/***************************************************************************
 * InfantryClass::Receive_Message -- Process radio messages                *
 *                                                                         *
 *    If the infantry's boxing, it needs to return to a normal state when  *
 *    his opponent moves away.  Otherwise fall thru to FootClass processing*
 *                                                                         *
 * INPUT:   from    - Pointer to the originator of this message.           *
 *                                                                         *
 *          message - the message to process                               *
 *                                                                         *
 *          param    -- Reference to an optional parameter that can be     *
 *                      used to transfer more information than is          *
 *                      possible with the simple radio message values.     *
 *                                                                         *
 * OUTPUT:  an appropriate response message                                *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/19/1995 BWG : Created.                                             *
 *   05/14/1995 JLB : Handles loading maneuver messages.                   *
 *=========================================================================*/
RadioMessageType InfantryClass::Receive_Message(RadioClass * from, RadioMessageType message, long & param)
{
	Validate();
	int damage;

	switch (message) {

		case	RADIO_OVER_OUT:
#ifdef BOXING
			if (IsBoxing) Do_Action(DO_READY_WEAPON);
#endif
			break;

		/*
		**	Request a fisticuff fight. If this infantry is already involved in a fight,
		**	then refuse the offer.
		*/
		case	RADIO_PREPARE_TO_BOX:
#ifdef BOXING
			if (IsBoxing) break;
#endif
			if (Contact_With_Whom() == from) {
				Do_Action(DO_ON_GUARD, true);
				Assign_Target(Contact_With_Whom()->As_Target());
				return(RADIO_ROGER);
			}
			return(RADIO_NEGATIVE);

		/*
		**	Just received a kick! Take some damage.
		*/
		case	RADIO_KICK:
			damage = Infantry_Kick_Damage[Random_Pick(0, (int)(sizeof(Infantry_Kick_Damage) / sizeof(Infantry_Kick_Damage[0])))];
			if (Take_Damage(damage, 0, WARHEAD_FOOT, this) == RESULT_DESTROYED) return(RADIO_STATIC);
			return(RADIO_ROGER);

		/*
		**	Just recieved a punch! Take some damage.
		*/
		case	RADIO_PUNCH:
			damage = Infantry_Punch_Damage[Random_Pick(0, (int)(sizeof(Infantry_Punch_Damage) / sizeof(Infantry_Punch_Damage[0])))];
			if (Take_Damage(damage, 0, WARHEAD_FIST, this) == RESULT_DESTROYED) return(RADIO_STATIC);
			return(RADIO_ROGER);

	}
	return(FootClass::Receive_Message(from, message, param));
}


/***************************************************************************
 * InfantryClass::Rearm_Delay -- Return Arming delay for infantry if boxing*
 *                                                                         *
 *    If the infantry's in a boxing mode, return an appropriate re-arming  *
 *    delay.  Otherwise return the default return val.                     *
 *                                                                         *
 * INPUT:   second   -- bool; see TechnoClass...                           *
 *                                                                         *
 * OUTPUT:  Returns with the # of game frames to delay before shooting     *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/16/1995 BWG : Created.                                             *
 *=========================================================================*/
int InfantryClass::Rearm_Delay(bool second) const
{
	Validate();
#ifdef BOXING
	if (IsBoxing) {
		return(Random_Pick(5, 50));
	}
#endif
	return(FootClass::Rearm_Delay(second));
}


/***********************************************************************************************
 * InfantryClass::Assign_Mission -- Assign mission to infantry object.                         *
 *                                                                                             *
 *    When a new mission is assigned, make sure he gets out of boxing mode.                    *
 *                                                                                             *
 * INPUT:   order -- The new mission to assign to the unit.                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void InfantryClass::Assign_Mission(MissionType order)
{
	Validate();
	if (order == MISSION_SABOTAGE) {
		Sound_Effect(VOC_RAMBO_PRESENT, Coord);
	}

	IsBoxing = false;
	FootClass::Assign_Mission(order);
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
ActionType InfantryClass::What_Action(ObjectClass * object) const
{
	Validate();
	ActionType action = FootClass::What_Action(object);

	/*
	**	First see if it's a commando, and if he's attacking a building, have him return ACTION_SABOTAGE instead
	*/
	if (*this == INFANTRY_RAMBO && action == ACTION_ATTACK && object->What_Am_I() == RTTI_BUILDING) {
		return(ACTION_SABOTAGE);
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
		Is_Owned_By_Player() &&			// Changed for multiplayer. ST - 3/13/2019 5:37PM
		//IsOwnedByPlayer &&
		object->Is_Techno() &&
		//IsOwnedByPlayer &&
		((InfantryClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (TechnoClass*)object) == RADIO_ROGER) {
//	if (object->Owner() == Owner() && object->What_Am_I() == RTTI_UNIT && ((UnitClass *)object)->Class->IsTransporter && ((UnitClass *)object)->How_Many() < 5) {
		action = ACTION_ENTER;
	}

	if (Class->IsCapture && action == ACTION_ATTACK) {
		if (object->Owner() != Owner() &&
			((object->What_Am_I() == RTTI_AIRCRAFT && ((AircraftClass *)object)->Pip_Count() == 0 && *((AircraftClass *)object) == AIRCRAFT_TRANSPORT) ||
			(object->What_Am_I() == RTTI_BUILDING &&
				((BuildingClass *)object)->Class->IsCaptureable && (GameToPlay != GAME_NORMAL || *((BuildingClass *)object) != STRUCT_EYE || Scenario < 13) ))
			) {

			action = ACTION_CAPTURE;
		} else {
			if (Class->Primary == WEAPON_NONE) {
				action = ACTION_NONE;
			}
		}
	}
	return(action);
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
void InfantryClass::Read_INI(char *buffer)
{
	InfantryClass	*infantry;			// Working infantry pointer.
	char				*tbuffer;		// Accumulation buffer of infantry IDs.
	HousesType		inhouse;			// Infantry house.
	InfantryType	classid;			// Infantry class.
	int				len;				// Length of data in buffer.
	char				buf[128];

	len = strlen(buffer) + 2;
	tbuffer = buffer + len;

	/*------------------------------------------------------------------------
	Read the entire INFANTRY INI section into HIDBUF
	------------------------------------------------------------------------*/
	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-len, buffer);

	while (*tbuffer != '\0') {

		/*
		**	Get an infantry entry
		*/
		WWGetPrivateProfileString(INI_Name(), tbuffer, NULL, buf, sizeof(buf)-1, buffer);

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
					if (infantry) {

						/*
						**	3rd token: strength.
						*/
						int strength = atoi(strtok(NULL, ",\n\r"));

						/*
						**	4th token: cell #.
						*/
						COORDINATE coord = Cell_Coord((CELL)atoi(strtok(NULL, ",\n\r")));

						/*
						**	5th token: cell sub-location.
						*/
						coord = Coord_Add(coord & 0xFF00FF00L, StoppingCoordAbs[atoi(strtok(NULL, ","))]);

						/*
						**	Fetch the mission and facing.
						*/
						MissionType mission = MissionClass::Mission_From_Name(strtok(NULL, ",\n\r"));
						DirType dir = (DirType)atoi(strtok(NULL,",\n\r"));
						infantry->Trigger = TriggerClass::As_Pointer(strtok(NULL,",\n\r"));
						if (infantry->Trigger) {
							infantry->Trigger->AttachCount++;
						}

						if (infantry->Unlimbo(coord, dir)) {
							infantry->Strength = Fixed_To_Cardinal(infantry->Class_Of().MaxStrength, strength);
							if (GameToPlay == GAME_NORMAL || infantry->House->IsHuman) {
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
		tbuffer += strlen(tbuffer)+1;
	}
}


/***********************************************************************************************
 * InfantryClass::Write_INI -- Writes all the infantry out to an INI file.                     *
 *                                                                                             *
 *    This routine writes all of the infantry in the game out to an INI file. This is used     *
 *    in the scenario editor when the game needs to be saved.                                  *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Housename, Typename, Strength, Cellnum, CellSublocation, Missionname,                  *
 *         Facingnum, Triggername                                                              *
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
void InfantryClass::Write_INI(char *buffer)
{
	int			index;
	char			uname[10];
	char			buf[128];
	char			*tbuffer;		// Accumulation buffer of infantry IDs.

	/*
	**	First, clear out all existing infantry data from the ini file.
	*/
	tbuffer = buffer + strlen(buffer) + 2;
	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-strlen(buffer), buffer);
	while (*tbuffer != '\0') {
		WWWritePrivateProfileString(INI_Name(), tbuffer, NULL, buffer);
		tbuffer += strlen(tbuffer)+1;
	}

	/*
	**	Write the infantry data out.
	*/
	for (index = 0; index < Infantry.Count(); index++) {
		InfantryClass * infantry;

		infantry = Infantry.Ptr(index);
		if (!infantry->IsInLimbo) {

			sprintf(uname, "%03d", index);
			sprintf(buf, "%s,%s,%d,%u,%d,%s,%d,%s",
					infantry->House->Class->IniName,
					infantry->Class->IniName,
					infantry->Health_Ratio(),
					Coord_Cell(infantry->Coord),
					CellClass::Spot_Index(infantry->Coord),
					MissionClass::Mission_Name((infantry->Mission == MISSION_NONE) ?
						infantry->MissionQueue : infantry->Mission),
					infantry->PrimaryFacing.Current(),
					infantry->Trigger ? infantry->Trigger->Get_Name() : "None"
				);
			WWWritePrivateProfileString(INI_Name(), uname, buf, buffer);
		}
	}
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
	Validate();
	if (What_Action(object) != action) {
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

	FootClass::Active_Click_With(action, object);
}


/***********************************************************************************************
 * InfantryClass::Made_A_Kill -- Marks a kill caused by this infantry soldier.                 *
 *                                                                                             *
 *    When the infantry soldier is responsible for a kill, this routine is called. It checks   *
 *    to see if the soldier should make some comment or perform some action. The commando      *
 *    infantry is most likely to respond.                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of kills this infantry soldier has made.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int InfantryClass::Made_A_Kill(void)
{
	Validate();
	if (*this == INFANTRY_RAMBO || Random_Pick(0, 5) < Kills) {
		IsStoked = true;
		Comment = TICKS_PER_SECOND*2;
	}
	return(FootClass::Made_A_Kill());
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
	Validate();
	/*
	** Set the occupy postion for the spot that we passed in
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
	Validate();
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
 *=============================================================================================*/
int InfantryClass::Full_Name(void) const
{
	Validate();
	if (IsTechnician) {
		return(TXT_TECHNICIAN);
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
 *=============================================================================================*/
int InfantryClass::Mission_Attack(void)
{
	Validate();
	if (*this == INFANTRY_E7 && As_Building(TarCom)) {
		Assign_Destination(TarCom);
		Assign_Mission(MISSION_CAPTURE);
		return(1);
	}
	return(FootClass::Mission_Attack());
}


RTTIType InfantryClass::What_Am_I(void) const
{
	Validate();
	return(RTTI_INFANTRY);
}

ActionType InfantryClass::What_Action(CELL cell) const
{
	Validate();
	return FootClass::What_Action(cell);
}

ObjectTypeClass const & InfantryClass::Class_Of(void) const
{
	Validate();
	return(*Class);
}

bool InfantryClass::Is_Infantry(void) const
{
	Validate();
	return(true);
}