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

/* $Header:   F:\projects\c&c\vcs\code\terrain.cpv   2.16   16 Oct 1995 16:51:44   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TERRAIN.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 29, 1994                                               *
 *                                                                                             *
 *                  Last Update : May 8, 1995 [JLB]                                            *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TerrainClass::AI -- Process the terrain object AI.                                        *
 *   TerrainClass::As_Target -- Converts the terrain object into a target number.              *
 *   TerrainClass::Can_Enter_Cell -- Determines if the terrain object can exist in the cell.   *
 *   TerrainClass::Catch_Fire -- Catches the terrain object on fire.                           *
 *   TerrainClass::Center_Coord -- Fetches the center point coordinate for terrain object.     *
 *   TerrainClass::Debug_Dump -- Displays the status of the terrain object.                    *
 *   TerrainClass::Draw_It -- Renders the terrain object at the location specified.            *
 *   TerrainClass::Fire_Out -- Handles when fire has gone out.                                 *
 *   TerrainClass::Heath_Ratio -- Determines the health ratio for the terrain object.          *
 *   TerrainClass::Init -- Initialize the terrain object tracking system.                      *
 *   TerrainClass::Limbo -- Handles terrain specific limbo action.                             *
 *   TerrainClass::Mark -- Marks the terrain object on the map.                                *
 *   TerrainClass::Radar_Icon -- Fetches pointer to radar icon to use.                         *
 *   TerrainClass::Read_INI -- Reads terrain objects from INI file.                            *
 *   TerrainClass::Start_To_Crumble -- Initiates crumbling of terrain (tree) object.           *
 *   TerrainClass::Take_Damage -- Damages the terrain object as specified.                     *
 *   TerrainClass::TerrainClass -- Constructor for a terrain class object.                     *
 *   TerrainClass::TerrainClass -- This is the constructor for a terrain object.               *
 *   TerrainClass::Unlimbo -- Unlimbo terrain object onto the map.                             *
 *   TerrainClass::Write_INI -- Writes all terrain objects to the INI file.                    *
 *   TerrainClass::delete -- Deletes a terrain object.                                         *
 *   TerrainClass::new -- Creates a new terrain object.                                        *
 *   TerrainClass::~TerrainClass -- Default destructor for terrain class objects.              *
 *   TerrainClass::Validate -- validates terrain pointer													  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"terrain.h"

#define	BARNACLE_STAGE			22
#define	FIRST_SPORE_STAGE		30
#define	FIRST_SPORABLE_LEVEL	7

/*
** This contains the value of the Virtual Function Table Pointer
*/
void * TerrainClass::VTable;


/***********************************************************************************************
 * TerrainClass::Validate -- validates terrain pointer													  *
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
int TerrainClass::Validate(void) const
{
	int num;

	num = Terrains.ID(this);
	if (num < 0 || num >= TERRAIN_MAX) {
		Validate_Error("TERRAIN");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
#endif


/***********************************************************************************************
 * TerrainClass::~TerrainClass -- Default destructor for terrain class objects.                *
 *                                                                                             *
 *    This is the default destructor for terrain objects. It will remove the object from the   *
 *    map and tracking systems, but only if the game is running. Otherwise, it does nothing.   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TerrainClass::~TerrainClass(void)
{
	if (GameActive && Class) {
		TerrainClass::Limbo();
	}
}


/***********************************************************************************************
 * TerrainClass::Take_Damage -- Damages the terrain object as specified.                       *
 *                                                                                             *
 *    This routine is called when damage is to be inflicted upon the terrain object. It is     *
 *    through this routine that terrain objects are attacked and thereby destroyed. Not all    *
 *    terrain objects can be damaged by this routine however.                                  *
 *                                                                                             *
 * INPUT:   damage      -- The damage points to inflict (raw).                                 *
 *                                                                                             *
 *          warhead     -- The warhead type the indicates the kind of damage. This is used to  *
 *                         determine if the terrain object is damaged and if so, by how much.  *
 *                                                                                             *
 * OUTPUT:  bool; Was the terrain object destroyed by this damage?                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *   11/22/1994 JLB : Shares base damage handler for techno objects.                           *
 *   12/11/1994 JLB : Shortens attached burning animations.                                    *
 *=============================================================================================*/
ResultType TerrainClass::Take_Damage(int & damage, int distance, WarheadType warhead, TechnoClass * source)
{
	Validate();
	ResultType res = RESULT_NONE;

	/*
	**	Small arms can never destroy a terrain element.
	*/
	if ((!IsOnFire || warhead == WARHEAD_FIRE) && warhead != WARHEAD_SA && !Class->IsImmune) {

		res = ObjectClass::Take_Damage(damage, distance, warhead, source);

		if (damage && warhead == WARHEAD_FIRE) {
			Catch_Fire();
		}

		/*
		**	If the terrain object is destroyed by this damage, then only remove it if it
		**	currently isn't on fire and isn't in the process of crumbling.
		*/
		if (res == RESULT_DESTROYED) {

			/*
			**	Remove this terrain object from the targeting computers of all other
			**	game objects. No use beating a dead horse.
			*/
			Detach_All();

			if (IsOnFire) {

				/*
				**	Attached flame animation should be shortened as much as possible so that
				**	crumbling can begin soon.
				*/
				Shorten_Attached_Anims(this);
			} else {
				Start_To_Crumble();
			}
		}
	}
	return(res);
}


/***********************************************************************************************
 * TerrainClass::As_Target -- Converts the terrain object into a target number.                *
 *                                                                                             *
 *    This routine will take the terrain object and generate a unique targeting number. This   *
 *    number is used for the NavCom and TarCom of other units.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the target number of this terrain object.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET TerrainClass::As_Target(void) const
{
	Validate();
	return(Build_Target(KIND_TERRAIN, Terrains.ID(this)));
}


/***********************************************************************************************
 * TerrainClass::new -- Creates a new terrain object.                                          *
 *                                                                                             *
 *    This routine creates a new terrain object by grabbing a free slot                        *
 *    out of the terrain object pool.                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the terrain object allocated.                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void * TerrainClass::operator new(size_t)
{
	void * ptr = Terrains.Allocate();
	if (ptr) {
		((TerrainClass *)ptr)->Set_Active();
	}
	return(ptr);
}


/***********************************************************************************************
 * TerrainClass::delete -- Deletes a terrain object.                                           *
 *                                                                                             *
 *    This routine deletes a terrain object by returning it to the                             *
 *    terrain object pool.                                                                     *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the terrain object to delete.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainClass::operator delete(void *ptr)
{
	if (ptr) {
		((TerrainClass *)ptr)->IsActive = false;
	}
	Terrains.Free((TerrainClass *)ptr);
}


/***********************************************************************************************
 * TerrainClass::TerrainClass -- This is the constructor for a terrain object                  *
 *                                                                                             *
 *    This constructor for a terrain object will initialize the terrain                        *
 *    object with it's proper type and insert it into the access                               *
 *    tracking system.                                                                         *
 *                                                                                             *
 * INPUT:   type  -- The terrain object type.                                                  *
 *                                                                                             *
 *          cell  -- The location of the terrain object.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TerrainClass::TerrainClass(TerrainType type, CELL cell) :
	Class(&TerrainTypeClass::As_Reference(type))
{
	IsBlossoming = false;
	IsBarnacled = false;
	IsSporing = false;
	IsCrumbling = false;
	IsOnFire = false;
	Strength = Class->MaxStrength;
	if (cell != -1) {
		if (!Unlimbo(Cell_Coord(cell))) {
			delete this;
		}
	}
	Set_Rate(0);	// turn off animation
}


/***********************************************************************************************
 * TerrainClass::Mark -- Marks the terrain object on the map.                                  *
 *                                                                                             *
 *    This routine will mark or remove the terrain object from the map                         *
 *    tracking system. This is typically called when the terrain object                        *
 *    is first created, when it is destroyed, and whenever it needs to be                      *
 *    redrawn.                                                                                 *
 *                                                                                             *
 * INPUT:   mark  -- The marking operation to perform.                                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the terrain object successfully marked?                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/02/1994 JLB : Created.                                                                 *
 *   12/23/1994 JLB : Performs low level legality check before proceeding.                     *
 *=============================================================================================*/
bool TerrainClass::Mark(MarkType mark)
{
	Validate();
	if (ObjectClass::Mark(mark)) {
		short const *overlap = Class->Overlap_List();
		short const *occupy = Class->Occupy_List();
		CELL cell = Coord_Cell(Coord);

		switch (mark) {
			case MARK_UP:
				Map.Pick_Up(cell, this);
				break;

			case MARK_DOWN:
				Map.Place_Down(cell, this);
				break;

			default:
				Map.Refresh_Cells(cell, overlap);
				Map.Refresh_Cells(cell, occupy);
				break;
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TerrainClass::Draw_It -- Renders the terrain object at the location specified.              *
 *                                                                                             *
 *    This routine is used to render the terrain object at the location specified and          *
 *    clipped to the window specified. This is the gruntwork drawing routine for the           *
 *    terrain objects as they are displayed on the map.                                        *
 *                                                                                             *
 * INPUT:   x,y      -- The coordinate to draw the terrain object at (centered).               *
 *                                                                                             *
 *          window   -- The clipping window to draw to.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/27/1994 JLB : Created.                                                                 *
 *   11/09/1994 JLB : Changed selected terrain highlight method.                               *
 *=============================================================================================*/
void TerrainClass::Draw_It(int x, int y, WindowNumberType window)
{
	Validate();
	void const * shapedata;

	shapedata = Class->Get_Image_Data();
	if (shapedata) {
		int	shapenum = 0;

		/*
		**	Determine the animation stage to render the terrain object. If it is crumbling, then
		**	it will display the crumbling animation.
		*/
		if (IsCrumbling || Class->IsTransformable) {
			shapenum = Fetch_Stage()+IsCrumbling;
		} else {
			if (Strength < 2) {
				shapenum++;
			}
		}

		ShapeFlags_Type flags = SHAPE_NORMAL;
		if (Is_Selected_By_Player() && Debug_Map) flags = flags | SHAPE_FADING;

		IsTheaterShape = true;
		CC_Draw_Shape(this, shapedata, shapenum, x, y, window, flags|SHAPE_WIN_REL|SHAPE_GHOST, Map.FadingLight, Map.UnitShadow);
		IsTheaterShape = false;
	}
}


/***********************************************************************************************
 * TerrainClass::Init -- Initialize the terrain object tracking system.                        *
 *                                                                                             *
 *    This routine will clear out the terrain object system so that no terrain objects will    *
 *    exists. It is called prior to loading or starting a scenario.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainClass::Init(void)
{
	TerrainClass *ptr;

	Terrains.Free_All();

	ptr = new TerrainClass();
	VTable = ((void **)(((char *)ptr) + sizeof(AbstractClass) - 4))[0];
	delete ptr;
}


/***********************************************************************************************
 * TerrainClass::Can_Enter_Cell -- Determines if the terrain object can exist in the cell.     *
 *                                                                                             *
 *    This routine will examine the cell specified and determine if the the terrain object     *
 *    can legally exist there.                                                                 *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine.                                                      *
 *                                                                                             *
 * OUTPUT:  If the terrain object can be placed in the cell specified, then a value less than  *
 *          256 will be returned.                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *   01/01/1995 JLB : Actually works now.                                                      *
 *=============================================================================================*/
MoveType TerrainClass::Can_Enter_Cell(CELL cell, FacingType) const
{
	Validate();
	short const	*offset;		// Pointer to cell offset list.

	if ((unsigned)cell >= MAP_CELL_TOTAL) return(MOVE_NO);

	offset = Occupy_List();
	while (*offset != REFRESH_EOL) {
		if (!Map[(CELL)(cell + *offset++)].Is_Generally_Clear()) {
			return(MOVE_NO);
		}
	}
	return(MOVE_OK);
}


/***********************************************************************************************
 * TerrainClass::Catch_Fire -- Catches the terrain object on fire.                             *
 *                                                                                             *
 *    This routine is called if the terrain object is supposed to catch on fire. The routine   *
 *    performs checking to make sure that only flammable terrain objects that aren't already   *
 *    on fire get caught on fire.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the terrain object caught on fire by this routine?                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1994 JLB : Created.                                                                 *
 *   12/11/1994 JLB : Don't catch fire if already on fire or crumbling.                        *
 *=============================================================================================*/
bool TerrainClass::Catch_Fire(void)
{
	Validate();
	if (!IsCrumbling && !IsOnFire && Class->IsFlammable) {
		AnimClass * anim = new AnimClass(ANIM_BURN_BIG, Coord_Add(Sort_Y(), 0xFFB00000L));
		if (anim) {
			anim->Attach_To(this);
		}
		anim = new AnimClass(ANIM_BURN_MED, Coord_Add(Sort_Y(), 0xFF200000L), 15);
		if (anim) {
			anim->Attach_To(this);
		}
		IsOnFire = true;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TerrainClass::Fire_Out -- Handles when fire has gone out.                                   *
 *                                                                                             *
 *    When the fire has gone out on a burning terrain object, this routine is called. The      *
 *    animation has already been terminated prior to calling this routine. All this routine    *
 *    needs to perform is any necessary local flag updating.                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainClass::Fire_Out(void)
{
	Validate();
	if (IsOnFire) {
		IsOnFire = false;
		if (!IsCrumbling && !Strength) {
			Detach_All();
			Mark();
			Start_To_Crumble();
			new AnimClass(ANIM_SMOKE_M, Coord_Add(Coord, Class->CenterBase));
		}
	}
}


/***********************************************************************************************
 * TerrainClass::AI -- Process the terrain object AI.                                          *
 *                                                                                             *
 *    This is used to handle any AI processing necessary for terrain objects. This might       *
 *    include animation effects.                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1994 JLB : Created.                                                                 *
 *   09/28/1994 JLB : Crumbling animation.                                                     *
 *=============================================================================================*/
void TerrainClass::AI(void)
{
	Validate();
	ObjectClass::AI();

	if (StageClass::Graphic_Logic()) {
		Mark();

		/*
		**	If the terrain object is in the process of crumbling, then when at the
		**	last stage of the crumbling animation, delete the terrain object.
		*/
		if (IsCrumbling && Fetch_Stage() == Get_Build_Frame_Count(Class->Get_Image_Data())-1) {
			delete this;
		}
	}

	/*
	** if this is a blossom tree, let's update it at this time
	*/
	if (Class->IsTransformable) {
		// If it's already blossomed, is it at barnacled stage?
		if (IsBlossoming) {
			// if it's not barnacled yet, check if we're at that stage
			if (!IsBarnacled) {
				if (Fetch_Stage() == BARNACLE_STAGE) {
					IsBarnacled = true;
					Set_Rate(0);	// turn off animation
				}
			} else {
				/*
				** If it's barnacled, see if it's pulsing and spore-ing
				*/
				if (IsSporing) {
					if (Fetch_Stage() >= Get_Build_Frame_Count(Class->Get_Image_Data())-1) {
						Explosion_Damage(Sort_Y(), 5, NULL, WARHEAD_SPORE);
						Set_Stage(FIRST_SPORE_STAGE);
						if (Random() & 1) {
							IsSporing = false;
							StageClass::Set_Rate(0);
						}
					}
				} else {
					if (Random() == 255) {	// is it time to start sporing?
						IsSporing = true;
						StageClass::Set_Rate(Options.Normalize_Delay(1));
					}
				}
			}
		} else {

			// If it hasn't tried to blossom yet, can it do so now?
			if (Random_Picky((int)1, (int)5000, (char*)NULL, (int)0) == 1) {
				IsBlossoming = true;
				StageClass::Set_Stage(1);
				StageClass::Set_Rate(Options.Normalize_Delay(1));
			}
		}
	}
}


#ifdef CHEAT_KEYS
/***********************************************************************************************
 * TerrainClass::Debug_Dump -- Displays the status of the terrain object.                      *
 *                                                                                             *
 *    This debugging support routine is used to display the status of the terrain object to    *
 *    the debug screen.                                                                        *
 *                                                                                             *
 * INPUT:   mono  -- The mono screen to display the status to.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainClass::Debug_Dump(MonoClass *mono) const
{
	Validate();
	ObjectClass::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * TerrainClass::Unlimbo -- Unlimbo terrain object onto the map.                               *
 *                                                                                             *
 *    This routine is used to unlimbo the terrain object onto a location on the map. Normal    *
 *    unlimbo procedures are sufficient except that the coordinate location of a terrain       *
 *    object is based on the upper left corner of a cell rather than the center. Mask the      *
 *    coordinate value so that it snaps to the upper left corner and then proceed with a       *
 *    normal unlimbo process.                                                                  *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate to mark as the terrain's location.                      *
 *                                                                                             *
 *          dir      -- unused                                                                 *
 *                                                                                             *
 * OUTPUT:  bool; Was the terrain object successful in the unlimbo process? Failure could be   *
 *                the result of illegal positioning.                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/02/1994 JLB : Created.                                                                 *
 *   11/16/1994 JLB : Checks for theater legality.                                             *
 *=============================================================================================*/
bool TerrainClass::Unlimbo(COORDINATE coord, DirType dir)
{
	Validate();
	if (Class->Theater & (1 << Map.Theater)) {
		return(ObjectClass::Unlimbo(coord, dir));
	}
	return(false);
}


/***********************************************************************************************
 * TerrainClass::Start_To_Crumble -- Initiates crumbling of terrain (tree) object.             *
 *                                                                                             *
 *    This routine is used to start the crumbling process for terrain object. This only        *
 *    applies to trees.                                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainClass::Start_To_Crumble(void)
{
	Validate();
	if (!IsCrumbling) {
		IsCrumbling = true;
		Set_Rate(2);
		Set_Stage(0);
	}
}


/***********************************************************************************************
 * TerrainClass::Limbo -- Handles terrain specific limbo action.                               *
 *                                                                                             *
 *    This routine (called as a part of the limbo process) will remove the terrain occupation  *
 *    flag in the cell it occupies.                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the terrain object unlimboed?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TerrainClass::Limbo(void)
{
	Validate();
	if (!IsInLimbo) {
		CELL cell = Coord_Cell(Coord);
		Map[cell].Flag.Occupy.Monolith = false;
	}
	return(ObjectClass::Limbo());
}


/***********************************************************************************************
 * TerrainClass::Center_Coord -- Fetches the center point coordinate for terrain object.       *
 *                                                                                             *
 *    Use this routine to fetch the center point terrain                                       *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE TerrainClass::Center_Coord(void) const
{
	Validate();
	return(Coord_Add(Coord, Class->CenterBase));
}


/***********************************************************************************************
 * TerrainClass::TerrainClass -- Constructor for a terrain class object.                       *
 *                                                                                             *
 *    This is the default constructor for a terrain class object. It basically initializes     *
 *    the object to a null -- do nothing -- state.                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TerrainClass::TerrainClass(void) :
	Class(0)
{
	IsOnFire = false;
	IsCrumbling = false;
	IsBlossoming = false;
	IsBarnacled = false;
	IsSporing = false;
	Strength = 0;
}


/***********************************************************************************************
 * TerrainClass::Radar_Icon -- Fetches pointer to radar icon to use.                           *
 *                                                                                             *
 *    This routine will return with a pointer to the radar icon to use for the cell number     *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   cell  -- The cell number to use when determine what icon pointer to return.        *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the 9 pixel values that make up the icon of this         *
 *          terrain object located at the cell specified.                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
unsigned char * TerrainClass::Radar_Icon(CELL cell)
{
	Validate();
	unsigned char *icon = (unsigned char *)Class->Get_Radar_Data();	// get a pointer to radar icons
	int  width = *icon++;							// extract the width from data
	int  height = *icon++;							// extract the width from data

	/*
	** Icon number that we need can be found by converting the cell and base
	** cell to and x and y offset from the upper left of the cell, and then
	** multiplying it by the width of the terrain in icons, which we
	** conveniantly stored out as the first byte of every icon we made.
	*/
	int basecell = Coord_Cell(Coord);				// find the base cell of terrain
	int ydiff = Cell_Y(cell) - Cell_Y(basecell);
	int xdiff = Cell_X(cell) - Cell_X(basecell);
	if (xdiff < width && ydiff < height) {
		int iconnum = (ydiff * width) + xdiff;
		return(icon + (iconnum * 9));
	}
	return(NULL);
}


/***********************************************************************************************
 * TerrainClass::Read_INI -- Reads terrain objects from INI file.                              *
 *                                                                                             *
 *    This routine reads a scenario control INI file and creates all                           *
 *    terrain objects specified therein. Objects so created are placed                         *
 *    upon the map.                                                                            *
 *                                                                                             *
 *      INI entry format:                                                                      *
 *      cellnum = TypeName, Triggername                                                        *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the loaded scenario INI file data.                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainClass::Read_INI(char *buffer)
{
	char	*tbuffer;	// Accumulation buffer of unit IDs.
	int	len;			// Size of data in buffer.
	char	buf[128];
	TerrainClass * tptr;

	len = strlen(buffer) + 2;
	tbuffer = buffer + len;

	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-len, buffer);
	while (*tbuffer != '\0') {
		TerrainType	terrain;		// Terrain type.
		CELL			cell;

		cell = atoi(tbuffer);
		WWGetPrivateProfileString(INI_Name(), tbuffer, NULL, buf, sizeof(buf)-1, buffer);
		terrain = TerrainTypeClass::From_Name(strtok(buf, ","));
		if (terrain != TERRAIN_NONE) {
			tptr = new TerrainClass(terrain, cell);
			tptr->Trigger = TriggerClass::As_Pointer(strtok(NULL,","));
			if (tptr->Trigger)
				tptr->Trigger->AttachCount++;
		}
		tbuffer += strlen(tbuffer)+1;
	}
}


/***********************************************************************************************
 * TerrainClass::Write_INI -- Writes all terrain objects to the INI file.                      *
 *                                                                                             *
 *    This routine is used to write all the terrain objects out to the INI file specified.     *
 *    It is used by the scenario editor to write out the map data.                             *
 *                                                                                             *
 *      INI entry format:                                                                      *
 *      cellnum = TypeName, Triggername                                                        *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the INI file staging area.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TerrainClass::Write_INI(char *buffer)
{
	int	index;
	char	uname[10];
	char	buf[127];
	char	*tbuffer;		// Accumulation buffer of unit IDs.

	/*
	**	First, clear out all existing terrain data from the ini file.
	*/
	tbuffer = buffer + strlen(buffer) + 2;
	WWGetPrivateProfileString(INI_Name(), NULL, NULL, tbuffer, ShapeBufferSize-strlen(buffer), buffer);
	while (*tbuffer != '\0') {
		WWWritePrivateProfileString(INI_Name(), tbuffer, NULL, buffer);
		tbuffer += strlen(tbuffer)+1;
	}

	/*
	**	Write the terrain data out.
	*/
	for (index = 0; index < Terrains.Count(); index++) {
		TerrainClass * terrain;

		terrain = Terrains.Ptr(index);
		if (!terrain->IsInLimbo && terrain->IsActive) {

			sprintf(uname, "%d", Coord_Cell(terrain->Coord));
			sprintf(buf, "%s,%s",
				terrain->Class->IniName,
				terrain->Trigger ? terrain->Trigger->Get_Name() : "None" );
			WWWritePrivateProfileString(INI_Name(), uname, buf, buffer);
		}
	}
}