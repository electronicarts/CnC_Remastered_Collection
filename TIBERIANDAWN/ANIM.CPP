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

/* $Header:   F:\projects\c&c\vcs\code\anim.cpv   2.18   16 Oct 1995 16:48:48   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Dune                                                         *
 *                                                                                             *
 *                    File Name : ANIM.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : June 3, 1991                                                 *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   AnimClass::AI -- This is the low level anim processor.                                    *
 *   AnimClass::Adjust_Coord -- Adjusts anim coordinates                                       *
 *   AnimClass::AnimClass -- The constructor for animation objects.                            *
 *   AnimClass::As_Target -- Converts the animation into a target value.                       *
 *   AnimClass::Attach_To -- Attaches animation to object specified.                           *
 *   AnimClass::Sort_Above -- Sorts the animation above the target specified.                  *
 *   AnimClass::Center_Coord -- Determine center of animation.                                 *
 *   AnimClass::Detach -- Remove animation if attached to target.                              *
 *   AnimClass::Draw_It -- Draws the animation at the location specified.                      *
 *   AnimClass::In_Which_Layer -- Determines what render layer the anim should be in.          *
 *   AnimClass::Init -- Performs pre-scenario initialization.                                  *
 *   AnimClass::Mark -- Signals to map that redrawing is necessary.                            *
 *   AnimClass::Middle -- Processes any middle events.                                         *
 *   AnimClass::Occupy_List -- Determines the occupy list for the animation.                   *
 *   AnimClass::Overlap_List -- Determines the overlap list for the animation.                 *
 *   AnimClass::Render -- Draws an animation object.                                           *
 *   AnimClass::Sort_Y -- Returns with the sorting coordinate for the animation.               *
 *   AnimClass::Start -- Processes initial animation side effects.                             *
 *   AnimClass::delete -- Returns an anim object back to the free pool.                        *
 *   AnimClass::new -- Allocates an anim object from the pool.                                 *
 *   AnimClass::~AnimClass -- Destructor for anim objects.                                     *
 *   AnimClass::Validate -- validates anim pointer															  *
 *   Shorten_Attached_Anims -- Reduces attached animation durations.                           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/*
** This contains the value of the Virtual Function Table Pointer
*/
void * AnimClass::VTable;


/***********************************************************************************************
 * AnimClass::Validate -- validates anim pointer															  *
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
int AnimClass::Validate(void) const
{
	int num;

	num = Anims.ID(this);
	if (num < 0 || num >= ANIM_MAX) {
		Validate_Error("ANIM");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
#endif


/***********************************************************************************************
 * Shorten_Attached_Anims -- Reduces attached animation durations.                             *
 *                                                                                             *
 *    This routine is used to reduce the amount of time any attached animations will process.  *
 *    Typical use of this is when an object is on fire and the object should now be destroyed  *
 *    but the attached animations are to run until completion before destruction can follow.   *
 *    This routine will make the animation appear to run its course, but in as short of time   *
 *    as possible. The shortening effect is achieved by reducing the number of times the       *
 *    animation will loop.                                                                     *
 *                                                                                             *
 * INPUT:   obj   -- Pointer to the object that all attached animations will be processed.     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/11/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void Shorten_Attached_Anims(ObjectClass * obj)
{
	if (obj) {
		for (int index = 0; index < Anims.Count(); index++) {
			AnimClass & anim = *Anims.Ptr(index);

			if (anim.Object == obj) {
				anim.Loops = 0;
			}
		}
	}
}


/***********************************************************************************************
 * AnimClass::Sort_Y -- Returns with the sorting coordinate for the animation.                 *
 *                                                                                             *
 *    This routine is used by the sorting system. Animations that are located in the ground    *
 *    layer will be sorted by this the value returned from this function.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the sort coordinate to use for this animation.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   12/15/1994 JLB : Handles flat anims (infantry decay anims).                               *
 *=============================================================================================*/
COORDINATE AnimClass::Sort_Y(void) const
{
	Validate();
	if (Object) {
		return(Coord_Add(Object->Sort_Y(), 0x00010000L));
	}
	if (Target_Legal(SortTarget)) {
		ObjectClass * obj = As_Object(SortTarget);
		if (obj && obj->IsActive) {
			return(Coord_Add(obj->Sort_Y(), 0x00010000L));
		}
	}
	if (*this == ANIM_MOVE_FLASH) {
		return(Coord_Add(Center_Coord(), XYP_COORD(0, -24)));
	}
	if (*this == ANIM_LZ_SMOKE) {
		return(Coord_Add(Center_Coord(), XYP_COORD(0, 14)));
	}
	return(Coord);
}


/***********************************************************************************************
 * AnimClass::Center_Coord -- Determine center of animation.                                   *
 *                                                                                             *
 *    This support function will return the "center" of the animation. The actual coordinate   *
 *    of the animation may be dependant on if the the animation is attached to an object.      *
 *    In such a case, it must factor in the object's location.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the coordinate of the center of the animation. The coordinate is in real   *
 *          game coordinates -- taking into consideration if the animation is attached.        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE AnimClass::Center_Coord(void) const
{
	Validate();
	if (Object) {
		return(Coord_Add(Coord, Object->Center_Coord()));
	}
	return(Coord);
}


/***********************************************************************************************
 * AnimClass::Render -- Draws an animation object.                                             *
 *                                                                                             *
 *    This is the working routine that renders the animation shape. It gets called once        *
 *    per animation per frame. It needs to be fast.                                            *
 *                                                                                             *
 * INPUT:   bool; Should the animation be rendered in spite of render flag?                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the animation rendered?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AnimClass::Render(bool forced)
{
	Validate();
	if (Delay) return(false);
	IsToDisplay = true;
	return(ObjectClass::Render(forced));
}


/***********************************************************************************************
 * AnimClass::Draw_It -- Draws the animation at the location specified.                        *
 *                                                                                             *
 *    This routine is used to render the animation object at the location specified. This is   *
 *    how the map imagery gets updated.                                                        *
 *                                                                                             *
 * INPUT:   x,y      -- The pixel coordinates to draw the animation at.                        *
 *                                                                                             *
 *          window   -- The to base the draw coordinates upon.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *   05/19/1995 JLB : Added white translucent effect.                                          *
 *=============================================================================================*/
//#pragma off (unreferenced)
void AnimClass::Draw_It(int x, int y, WindowNumberType window)
{
	Validate();

	bool render_legacy = !IsInvisible && (Class->VirtualAnim == ANIM_NONE || window != WINDOW_VIRTUAL);
	bool render_virtual = VirtualAnim != NULL && window == WINDOW_VIRTUAL;
	if (render_legacy) {
		void const * shapefile = Class->Get_Image_Data();
		if (shapefile) {
			void const * transtable = NULL;
			int shapenum = Class->Start + Fetch_Stage();
			void const * remap = NULL;
			ShapeFlags_Type flags = SHAPE_CENTER|SHAPE_WIN_REL;

			/*
			**	Some animations require special fixups.
			*/
			switch (Class->Type) {
				case ANIM_ION_CANNON:
					if (window != WINDOW_VIRTUAL) {
						y -= Get_Build_Frame_Height(shapefile) >> 1;
					} else {
						flags = flags | SHAPE_BOTTOM;
					}
					y += 12;
					break;

				case ANIM_RAPT_DIE:
				case ANIM_STEG_DIE:
				case ANIM_TREX_DIE:
				case ANIM_TRIC_DIE:
				case ANIM_ATOM_BLAST:
					transtable = Map.UnitShadow;
					break;
			}

			/*
			**	If the translucent table hasn't been determined yet, then check to see if it
			**	should use the white or normal translucent tables.
			*/
			if (!transtable && Class->IsWhiteTrans) transtable = Map.WhiteTranslucentTable;
			if (!transtable && Class->IsTranslucent) transtable = Map.TranslucentTable;

			/*
			**	Set the shape flags to properly take into account any fading or ghosting
			**	table necessary.
			*/
			if (IsAlternate) {
				flags = flags | SHAPE_FADING;
				remap = Map.RemapTables[HOUSE_GOOD][0];
			}
			if (transtable) flags = flags | SHAPE_GHOST;

			/*
			**	Draw the animation shape, but ignore legacy if beyond normal stage count.
			*/
			if ((window == WINDOW_VIRTUAL) || (Fetch_Stage() < Class->Stages)) {
				CC_Draw_Shape(this, shapefile, shapenum, x, y, window, flags, remap, transtable, Class->VirtualScale);
			}
		}
	}
	if (render_virtual) {
		VirtualAnim->Make_Visible();
		VirtualAnim->Draw_It(x, y, window);
		VirtualAnim->Make_Invisible();
	}
}


/***********************************************************************************************
 * AnimClass::Mark -- Signals to map that redrawing is necessary.                              *
 *                                                                                             *
 *    This routine is used by the animation logic system to inform the map that the cells      *
 *    under the animation must be rerendered.                                                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AnimClass::Mark(MarkType mark)
{
	Validate();
	if (ObjectClass::Mark(mark)) {
		Map.Refresh_Cells(Coord_Cell(Center_Coord()), Overlap_List());
//		ObjectClass::Mark(mark);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * AnimClass::Overlap_List -- Determines the overlap list for the animation.                   *
 *                                                                                             *
 *    Use this routine to fetch the overlap list for the animation. This overlap list is the   *
 *    cells that this animation spills over.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to the overlap list for this particular instance of the          *
 *          animation.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * AnimClass::Overlap_List(void) const
{
	Validate();
	static short const OverlapN[] = {0, -MAP_CELL_W, -(MAP_CELL_W+1), -(MAP_CELL_W-1), -(2*MAP_CELL_W), -(2*MAP_CELL_W-1), -(2*MAP_CELL_W+1), REFRESH_EOL};
	static short const OverlapNW[] = {0, -1, -MAP_CELL_W, -(MAP_CELL_W+1), -(MAP_CELL_W+2), -(MAP_CELL_W*2+2), -(MAP_CELL_W*2+1), REFRESH_EOL};
	static short const OverlapW[] = {0, -1, -2, -(MAP_CELL_W+1), -(MAP_CELL_W+2), REFRESH_EOL};
	static short const OverlapSW[] = {0, -1, MAP_CELL_W, (MAP_CELL_W-1), (MAP_CELL_W-2), (MAP_CELL_W*2-2), (MAP_CELL_W*2-1), REFRESH_EOL};
	static short const OverlapS[] = {0, MAP_CELL_W-1, MAP_CELL_W, MAP_CELL_W+1, 2*MAP_CELL_W+1, 2*MAP_CELL_W, 2*MAP_CELL_W-1, REFRESH_EOL};
	static short const OverlapSE[] = {0, 1, MAP_CELL_W, (MAP_CELL_W+1), (MAP_CELL_W+2), (MAP_CELL_W*2+2), (MAP_CELL_W*2+1), REFRESH_EOL};
	static short const OverlapE[] = {0, 1, 2, -(MAP_CELL_W-1), -(MAP_CELL_W-2), REFRESH_EOL};
	static short const OverlapNE[] = {0, 1, -MAP_CELL_W, -(MAP_CELL_W-1), -(MAP_CELL_W-2), -(MAP_CELL_W*2-2), -(MAP_CELL_W*2-1), REFRESH_EOL};
	static short const OverlapIon[] = {
		(-MAP_CELL_W * 7) - 1, (-MAP_CELL_W * 7), (-MAP_CELL_W * 7) + 1,
		(-MAP_CELL_W * 6) - 1, (-MAP_CELL_W * 6), (-MAP_CELL_W * 6) + 1,
		(-MAP_CELL_W * 5) - 1, (-MAP_CELL_W * 5), (-MAP_CELL_W * 5) + 1,
		(-MAP_CELL_W * 4) - 1, (-MAP_CELL_W * 4), (-MAP_CELL_W * 4) + 1,
		(-MAP_CELL_W * 3) - 1, (-MAP_CELL_W * 3), (-MAP_CELL_W * 3) + 1,
		(-MAP_CELL_W * 2) - 1, (-MAP_CELL_W * 2), (-MAP_CELL_W * 2) + 1,
		(-MAP_CELL_W * 1) - 1, (-MAP_CELL_W * 1), (-MAP_CELL_W * 1) + 1,
		(-MAP_CELL_W * 0) - 1, (-MAP_CELL_W * 0), (-MAP_CELL_W * 0) + 1,
 		REFRESH_EOL
	};

	static short const OverlapAtom[] = {
		(-MAP_CELL_W * 2) - 1, (-MAP_CELL_W * 2), (-MAP_CELL_W * 2) + 1,
		(-MAP_CELL_W * 1) - 1, (-MAP_CELL_W * 1), (-MAP_CELL_W * 1) + 1,
		(-MAP_CELL_W * 0) - 1, (-MAP_CELL_W * 0), (-MAP_CELL_W * 0) + 1,
		( MAP_CELL_W * 1) - 1, ( MAP_CELL_W * 1), ( MAP_CELL_W * 1) + 1,
		( MAP_CELL_W * 2) - 1, ( MAP_CELL_W * 2), ( MAP_CELL_W * 2) + 1,
 		REFRESH_EOL
	};

	switch (Class->Type) {
		case ANIM_CHEM_N:
		case ANIM_FLAME_N:
			return(OverlapN);

		case ANIM_CHEM_NW:
		case ANIM_FLAME_NW:
			return(OverlapNW);

		case ANIM_CHEM_W:
		case ANIM_FLAME_W:
			return(OverlapW);

		case ANIM_CHEM_SW:
		case ANIM_FLAME_SW:
			return(OverlapSW);

		case ANIM_CHEM_S:
		case ANIM_FLAME_S:
			return(OverlapS);

		case ANIM_CHEM_SE:
		case ANIM_FLAME_SE:
			return(OverlapSE);

		case ANIM_CHEM_E:
		case ANIM_FLAME_E:
			return(OverlapE);

		case ANIM_CHEM_NE:
		case ANIM_FLAME_NE:
			return(OverlapNE);

		case ANIM_ION_CANNON:
			return(OverlapIon);

		case ANIM_ATOM_BLAST:
			return(OverlapAtom);

		default:
			break;
	}
	return(Coord_Spillage_List(Center_Coord(), Class->Size));
}


/***********************************************************************************************
 * AnimClass::Occupy_List -- Determines the occupy list for the animation.                     *
 *                                                                                             *
 *    Animations always occupy only the cell that their center is located over. As such, this  *
 *    routine always returns a simple (center cell) occupation list.                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the occupation list for the animation.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * AnimClass::Occupy_List(void) const
{
	Validate();
	static short _simple[] = {REFRESH_EOL};

	return(_simple);
}


/***********************************************************************************************
 * AnimClass::Init -- Performs pre-scenario initialization.                                    *
 *                                                                                             *
 *    This routine is used to initialize the animation system prior to a scenario being loaded *
 *    or reloaded. It effectively removes all animations from the system.                      *
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
void AnimClass::Init(void)
{
	AnimClass *ptr;

	Anims.Free_All();

	ptr = new AnimClass();
	VTable = ((void **)(((char *)ptr) + sizeof(AbstractClass) - 4))[0];
	delete ptr;
}


/***********************************************************************************************
 * AnimClass::new -- Allocates an anim object from the pool.                                   *
 *                                                                                             *
 *    This routine is used to allocate a free anim class object from the preallocated pool     *
 *    in the near heap. If there are no free animation objects, then null is returned.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a free anim object.                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void *AnimClass::operator new(size_t)
{
	void * ptr = Anims.Allocate();
	if (ptr) {
		((AnimClass *)ptr)->Set_Active();
	}
	return(ptr);
}


/***********************************************************************************************
 * AnimClass::delete -- Returns an anim object back to the free pool.                          *
 *                                                                                             *
 *    This routine is used to return an anim object back to the pool of free anim objects.     *
 *    Anim objects so returned are available to be reallocated for the next animation.         *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the anim object to return to the pool.                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AnimClass::operator delete(void *ptr)
{
	if (ptr) {
		((AnimClass *)ptr)->IsActive = false;
	}
	Anims.Free((AnimClass *)ptr);

	//Map.Validate();
}


/***********************************************************************************************
 * AnimClass::AnimClass -- The constructor for animation objects.                              *
 *                                                                                             *
 *    This routine is used as the constructor of animation objects. It initializes and adds    *
 *    the animation object to the display and logic systems.                                   *
 *                                                                                             *
 * INPUT:   animnum  -- The animation number to start.                                         *
 *                                                                                             *
 *          coord    -- The location of the animation.                                         *
 *                                                                                             *
 *          timedelay-- The delay before the animation starts.                                 *
 *                                                                                             *
 *          loop     -- The number of times to loop this animation.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   08/03/1994 JLB : Added a delayed affect parameter.                                        *
 *=============================================================================================*/
AnimClass::AnimClass(AnimType animnum, COORDINATE coord, unsigned char timedelay, unsigned char loop, bool alt) :
	Class(&AnimTypeClass::As_Reference(animnum))
{
	Object = 0;
	SortTarget = TARGET_NONE;
	Owner = HOUSE_NONE;

	if (Class->Stages == -1) {
		((int&)Class->Stages) = Get_Build_Frame_Count(Class->Get_Image_Data());
	}
	if (Class->LoopEnd == -1) {
		((int&)Class->LoopEnd) = Class->Stages;
	}
	if (Class->IsNormalized) {
		Set_Rate(Options.Normalize_Delay(Class->Delay));
	} else {
		Set_Rate(Class->Delay);
	}
	Set_Stage(0);

	Accum = 0;
	coord = Adjust_Coord(coord);
	Unlimbo(coord);

	VisibleFlags = 0xffff;

	/*
	**	Drop zone smoke always reveals the map around itself.
	*/
	if (*this == ANIM_LZ_SMOKE) {
		// Added PlayerPtr here as Sight_From now needs to know who to perform the action for. This should be OK as it's not used in MP. ST - 3/28/2019 2:43PM
		Map.Sight_From(PlayerPtr, Coord_Cell(coord), 4, false);
	}

	/*
	**	Determine the time before the first animation process. For time delayed
	**	animations, this is the value passed as a parameter.
	*/
	Delay = timedelay;

	Loops = (unsigned char)(MAX(loop, (unsigned char)1) * Class->Loops);
	Loops = (unsigned char)MAX(Loops, (unsigned char)1);

	IsToDelete = false;
	IsBrandNew = true;
	IsAlternate = alt;
	IsInvisible = false;

	/*
	**	If the animation starts immediately, then play the associated sound effect now.
	*/
	if (!Delay) {
		Start();
	}

	/*
	**	Check for a virtual animation
	*/
	if (Class->VirtualAnim != ANIM_NONE) {
		VirtualAnim = new AnimClass(Class->VirtualAnim, 0, timedelay, loop, alt);
		if (VirtualAnim != NULL) {
			VirtualAnim->Make_Invisible();
		}
	} else {
		VirtualAnim = NULL;
	}
}


/***********************************************************************************************
 * AnimClass::~AnimClass -- Destructor for anim objects.                                       *
 *                                                                                             *
 *    This destructor handles removing the animation object from the system. It might require  *
 *    informing any object this animation is attached to that it is no longer attached.        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
AnimClass::~AnimClass(void)
{
	Validate();
	if (GameActive) {

		/*
		**	If this anim is attached to another object
		**	then check to see if this is the last anim attached to it. If this
		**	is the case, then inform the object that it is no longer attached to
		**	an animation.
		*/
		if (Object) {
			/*
			**	Remove the object from the appropriate display list.
			*/
			Map.Remove(this, In_Which_Layer());

			/*
			**	Scan for any other animations that are attached to the object that
			**	this animation is attached to. If there are no others, then inform the
			**	attached object of this fact.
			*/
			int index;
			for (index = 0; index < Anims.Count(); index++) {
				if (Anims.Ptr(index) != this && Anims.Ptr(index)->Object == Object) break;
			}

			/*
			**	Tell the object that it is no longer being damaged.
			*/
			if (index == Anims.Count()) {
				Object->Fire_Out();
				if (Object->In_Which_Layer() == LAYER_GROUND) Object->Mark(MARK_OVERLAP_UP);
				Object->IsAnimAttached = false;
				if (Object->In_Which_Layer() == LAYER_GROUND) Object->Mark(MARK_OVERLAP_DOWN);
			}
			Coord = Coord_Add(Object->Center_Coord(), Coord);
			Object = NULL;
		}

		Limbo();
	}
}


/***********************************************************************************************
 * AnimClass::AI -- This is the low level anim processor.                                      *
 *                                                                                             *
 *    This routine is called once per frame per animation. It handles transition between       *
 *    animation frames and marks the map for redraw as necessary.                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Speed is of upmost importance.                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AnimClass::AI(void)
{
	Validate();
	/*
	**	For ground level based animations (ones that can run slowly as well as
	**	occur behind other ground objects) always cause the cell to be redrawn.
	*/
	if (!Delay && Class->IsGroundLayer) {
		Map.Refresh_Cells(Coord_Cell(Center_Coord()), Overlap_List());
	}

	/*
	**	Special case check to make sure that building on top of a smoke marker
	**	causes the smoke marker to vanish.
	*/
	if (Class->Type == ANIM_LZ_SMOKE && Map[Coord_Cell(Center_Coord())].Cell_Building()) {
		IsToDelete = true;
	}

	/*
	**	Delete this animation and bail early if the animation is flagged to be deleted
	**	immediately.
	*/
	if (IsToDelete) {
		Delete_This();
		return;
	}

	/*
	**	If this is a brand new animation, then don't process it the first logic pass
	**	since it might end up skipping the first animation frame before it has had a
	**	chance to draw it.
	*/
	if (IsBrandNew) {
		IsBrandNew = false;
		return;
	}

	if (Delay) {
		Delay--;
		if (!Delay) {
			Start();
		}
	} else {

		/*
		**	This is necessary because there is no recording of animations on the map
		**	and thus the animation cannot be intelligently flagged for redraw. Most
		**	animations move fast enough that they would need to be redrawn every
		**	game frame anyway so this isn't TOO bad.
		*/
		Mark(MARK_CHANGE);

		if (StageClass::Graphic_Logic()) {
			int stage = Fetch_Stage();

			/*
			**	If this animation is attached to another object and it is a
			**	damaging kind of animation, then do the damage to the other
			**	object.
			*/
			if (Object && Class->Damage && stage < Class->Stages) {
				unsigned int accum = Accum;

				accum += Class->Damage;

				if (accum > 255) {

					/*
					**	Administer the damage. If the object was destroyed by this anim,
					**	then the attached damaging anim is also destroyed.
					*/
					int damage = accum >> 8;
					if (Object->Take_Damage(damage, 0, WARHEAD_FIRE) == RESULT_DESTROYED) {
						//Object = 0;
						Delete_This();
						if (VirtualAnim != NULL) {
							VirtualAnim->Delete_This();
						}
						return;
					}
				}
				Accum = (unsigned char)(accum & 0x00FF);
			}

			/*
			**	During the biggest stage (covers the most ground), perform any ground altering
			**	action required. This masks craters and scorch marks, so that they appear
			**	naturally rather than "popping" into existance while in plain sight.
			*/
			if (Class->Start+stage == Class->Biggest) {
				Middle();
			}

			/*
			**	Check to see if the last frame has been displayed. If so, then the
			**	animation either ends or loops.
			*/
			if ((Loops <= 1 && stage >= Class->Stages) || (Loops > 1 && stage >= Class->LoopEnd-Class->Start)) {

				/*
				**	Determine if this animation should loop another time. If so, then start the loop
				**	but if not, then proceed into the animation termination handler.
				*/
				if (Loops) Loops--;
				if (Loops) {
					Set_Stage(Class->LoopStart);
				} else {
					if (Class->VirtualAnim != ANIM_NONE) {
						Make_Invisible();
						if (VirtualAnim == NULL) {
							if (Class->ChainTo != ANIM_NONE) {
								Chain();
							}
							else {
								Delete_This();
							}
						}
					}
					else {
						if ((Class->VirtualStages < 0) || (stage >= Class->VirtualStages)) {
							if (Class->ChainTo != ANIM_NONE) {
								Chain();
							}
							else {
								Delete_This();
							}
						}
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * AnimClass::Attach_To -- Attaches animation to object specified.                             *
 *                                                                                             *
 *    An animation can be "attached" to an object. In such cases, the animation is rendered    *
 *    as an offset from the center of the object it is attached to. This allows affects such   *
 *    as fire or smoke to be consistently placed on the vehicle it is associated with.         *
 *                                                                                             *
 * INPUT:   obj   -- Pointer to the object to attach the animation to.                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/19/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AnimClass::Attach_To(ObjectClass * obj)
{
	Validate();
	if (!obj) return;

	if (obj->In_Which_Layer() == LAYER_GROUND) obj->Mark(MARK_OVERLAP_UP);
	obj->IsAnimAttached = true;
	if (obj->In_Which_Layer() == LAYER_GROUND) obj->Mark(MARK_OVERLAP_DOWN);
	Map.Remove(this, In_Which_Layer());
	Object = obj;
	Map.Submit(this, In_Which_Layer());
	Coord = Coord_Sub(Coord, obj->Center_Coord());
}


/***********************************************************************************************
 * AnimClass::Sort_Above -- Sorts the animation right above the specified target.              *
 *                                                                                             *
 *    Allows an animation to always be sorted above a particular target. Typically used        *
 *    for explosions and other effects that look weird beneath those objects.                  *
 *                                                                                             *
 * INPUT:   target   -- Target to sort above.                                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/14/2019 SKY : Created.                                                                 *
 *=============================================================================================*/
void AnimClass::Sort_Above(TARGET target)
{
	SortTarget = target;
}


/***********************************************************************************************
 * AnimClass::In_Which_Layer -- Determines what render layer the anim should be in.            *
 *                                                                                             *
 *    Use this routine to find out which display layer (ground or air) that the animation      *
 *    should be in. This information is used to place the animation into the correct display   *
 *    list.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the layer that the animation should exist in.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/25/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
LayerType AnimClass::In_Which_Layer(void) const
{
	Validate();
	if (Object || Class->IsGroundLayer) {
		return(LAYER_GROUND);
	}
	return(LAYER_AIR);
}


/***********************************************************************************************
 * AnimClass::Start -- Processes initial animation side effects.                               *
 *                                                                                             *
 *    This routine is called when the animation first starts. Sometimes there are side effects *
 *    associated with this animation that must occur immediately. Typically, this is the       *
 *    sound effect assigned to this animation. If this animation is supposed to attach itself  *
 *    to any object at its location, then do so at this time as well.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/30/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AnimClass::Start(void)
{
	Validate();
	CELL cell = Coord_Cell(Coord);

	Mark();

	/*
	**	Play the sound effect for this animation.
	*/
	Sound_Effect(Class->Sound, Coord);

	/*
	**	If the stage where collateral effects occur is the first stage of the animation, then
	**	perform this action now. Subsiquent checks against this stage value starts with the
	**	second frame of the animation.
	*/
	if (!Class->Biggest) {
		Middle();
	}

	/*
	**	If this is the kind of animation that sticks to whatever object is in the same
	**	location, then attach the animation to the object. If the animation is already
	**	attached, then do nothing.
	*/
	if (!Object && Class->IsSticky && Map.In_Radar(cell)) {
		UnitClass * unit = Map[cell].Cell_Unit();

		if (unit && *unit == UNIT_GUNBOAT) {
			Attach_To(unit);
		}
	}
}


/***********************************************************************************************
 * AnimClass::Middle -- Processes any middle events.                                           *
 *                                                                                             *
 *    This routine is called when the animation as reached its largest stage. Typically, this  *
 *    routine is used to cause scorches or craters to appear at a cosmetically pleasing        *
 *    moment.                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/30/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AnimClass::Middle(void)
{
	Validate();
	CELL cell = Coord_Cell(Center_Coord());
	CellClass * cellptr = &Map[cell];

	if (Class->Type == ANIM_ATOM_BLAST) {

		/*
		**	Find someone to blame the explosion on. This is necessary in
		**	order to properly enact retribution and record the kill for
		**	score purposes.
		*/
		BuildingClass * building = NULL;
		TechnoClass * backup = NULL;
		if (Owner != HOUSE_NONE) {
			for (int index = 0; index < Logic.Count(); index++) {
				ObjectClass * obj = Logic[index];

				if (obj && obj->Is_Techno() && obj->Owner() == Owner) {
					backup = (TechnoClass *)obj;
					if (obj->What_Am_I() == RTTI_BUILDING && *((BuildingClass *)obj) == STRUCT_TEMPLE) {
						building = (BuildingClass *)obj;
						break;
					}
				}
			}

			if (!building) building = (BuildingClass *)backup;
		}

		int radius = 3;
		int rawdamage = 200;
		if (GameToPlay == GAME_NORMAL) {
			radius = 4;
			rawdamage = 1000;
			Fade_Palette_To(WhitePalette, 30, NULL);
		}
		for (int x = -radius; x <= radius; x++) {
			for (int y = -radius; y <= radius; y++) {
				int xpos = Cell_X(cell) + x;
				int ypos = Cell_Y(cell) + y;

				/*
				**	If the potential damage cell is outside of the map bounds,
				**	then don't process it. This unusual check method ensures that
				**	damage won't wrap from one side of the map to the other.
				*/
				if ((unsigned)xpos > MAP_CELL_W) {
					continue;
				}
				if ((unsigned)ypos > MAP_CELL_H) {
					continue;
				}
				CELL tcell = XY_Cell(xpos, ypos);
				if (!Map.In_Radar(tcell)) continue;

				int damage = rawdamage / ((ABS(radius)/2)+1);
				Explosion_Damage(Cell_Coord(tcell), damage, building, WARHEAD_FIRE);
				new SmudgeClass(Random_Pick(SMUDGE_SCORCH1, SMUDGE_SCORCH6), Cell_Coord(tcell));
			}
		}
		Shake_The_Screen(3);
		if (GameToPlay == GAME_NORMAL) {
			Fade_Palette_To(GamePalette, 15, NULL);
		}
	}

	/*
	**	If this animation leaves scorch marks (e.g., napalm), then do so at this time.
	*/
	if (Class->IsScorcher) {
		new SmudgeClass(Random_Pick(SMUDGE_SCORCH1, SMUDGE_SCORCH6), Center_Coord());
	}

	/*
	**	Some animations leave a crater when they occur. Artillery is a good example.
	**	Craters always remove the Tiberium where they occur.
	*/
	if (Class->IsCraterForming) {

		/*
		**	Craters reduce the level of Tiberium in the cell.
		*/
		cellptr->Reduce_Tiberium(6);

		/*
		**	If there already is a crater in the cell, then just expand the
		**	crater.
		*/
		new SmudgeClass(SMUDGE_CRATER1, Center_Coord());
	}

	/*
	**	Flame throwers leave scorch marks in unusual positions. In addition, they leave fire
	**	shards in unusual positions as well.
	*/
	if (Class->IsFlameThrower) {
		COORDINATE	c2 = Coord_Move(Center_Coord(), (DirType)((Class->Type - ANIM_FLAME_N)<<5), 0x00E0);
		CELL	cell = Coord_Cell(c2);
		COORDINATE	c3 = Map.Closest_Free_Spot(Coord_Move(Center_Coord(), (DirType)((Class->Type - ANIM_FLAME_N)<<5), 0x0140), true);

		c2 = Map.Closest_Free_Spot(c2, true);
		if (c3 && (Random_Pick(0, 1) == 1)) {
			if (!Map[Coord_Cell(c3)].Cell_Terrain()) {
				new AnimClass(ANIM_FIRE_SMALL, c3, 0, 2);
			}
		}
		if (c2 && (Random_Pick(0, 1) == 1)) {
			if (!Map[Coord_Cell(c2)].Cell_Terrain()) {
				new AnimClass(ANIM_FIRE_SMALL, c2, 0, 2);
			}
		}
		new SmudgeClass(SMUDGE_SCORCH1, c2);
		if (c3 && (Random_Pick(0, 1) == 1)) {
			if (!Map[Coord_Cell(c3)].Cell_Terrain()) {
				new AnimClass(ANIM_SMOKE_M, c3);
			}
		}
	}

	AnimClass * newanim;

	/*
	**	If this animation spawns side effects during its lifetime, then
	**	do so now.
	*/
	switch (Class->Type) {
		case ANIM_ION_CANNON: {
				BuildingClass * building = NULL;
				TechnoClass * backup = NULL;
				if (Owner != HOUSE_NONE) {
					for (int index = 0; index < Logic.Count(); index++) {
						ObjectClass * obj = Logic[index];

						if (obj && obj->Is_Techno() && obj->Owner() == Owner && !obj->IsInLimbo) {
							backup = (TechnoClass *)obj;
							if (obj->What_Am_I() == RTTI_BUILDING && *((BuildingClass *)obj) == STRUCT_EYE) {
								building = (BuildingClass *)obj;
								break;
							}
						}
					}

					if (!building) building = (BuildingClass *)backup;
				}
				Explosion_Damage(Center_Coord(), 600, building, WARHEAD_PB);
			}
			break;

		case ANIM_NAPALM1:
		case ANIM_NAPALM2:
		case ANIM_NAPALM3:
			new AnimClass(ANIM_FIRE_SMALL, Map.Closest_Free_Spot(Coord_Scatter(Center_Coord(), 0x0040), true), 0, ((Random_Pick(0, 1) == 1) ? 1 : 2));
			if (Random_Pick(0, 1) == 1) {
				new AnimClass(ANIM_FIRE_SMALL, Map.Closest_Free_Spot(Coord_Scatter(Center_Coord(), 0x00A0), true), 0, ((Random_Pick(0, 1) == 1) ? 1 : 2));
			}
			if (Random_Pick(0, 1) == 1) {
				new AnimClass(ANIM_FIRE_MED, Map.Closest_Free_Spot(Coord_Scatter(Center_Coord(), 0x0070), true), 0, ((Random_Pick(0, 1) == 1) ? 1 : 2));
			}
			break;

		case ANIM_FIRE_MED:
		case ANIM_FIRE_MED2:
			newanim = new AnimClass(ANIM_FIRE_SMALL, Center_Coord(), 0, ((Random_Pick(0, 1) == 1) ? 1 : 2));
			if (newanim && Object) {
				newanim->Attach_To(Object);
			}
			break;

		default:
			break;
	}
}


void AnimClass::Chain(void)
{
	/*
	**	The animation should end now, but first check to see if
	**	it needs to chain into another animation. If so, then the
	**	animation isn't technically over. It metamorphoses into the
	**	new form.
	*/
	if (Class->ChainTo != ANIM_NONE) {
		((AnimTypeClass const *&)Class) = &AnimTypeClass::As_Reference(Class->ChainTo);

		if (Class->IsNormalized) {
			Set_Rate(Options.Normalize_Delay(Class->Delay));
		} else {
			Set_Rate(Class->Delay);
		}
		Set_Stage(Class->Start);
	}
}


/***********************************************************************************************
 * AnimClass::As_Target -- Converts the animation into a target value.                         *
 *                                                                                             *
 *    This support routine is used to convert the animation (as a pointer) into a target       *
 *    value (which is a number).                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the animation as a target value.                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET AnimClass::As_Target(void) const
{
	Validate();
	return(Build_Target(KIND_ANIMATION, Anims.ID(this)));
}


/***************************************************************************
 * AnimClass::Adjust_Coord -- Adjusts anim coordinates                     *
 *                                                                         *
 * INPUT:      none                                                        *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/17/1995 PWG : Created.                                             *
 *=========================================================================*/
COORDINATE AnimClass::Adjust_Coord(COORDINATE coord)
{
	Validate();
	int x,y;

 	switch (Class->Type) {
		case ANIM_ATOM_DOOR:
			x = -1;
			y = -36;
			break;

		default:
			return(coord);
	}
	COORDINATE addval = XYPixel_Coord(x, y);
	coord = Coord_Add(coord, addval);
	return(coord);
}


/***********************************************************************************************
 * AnimClass::Detach -- Remove animation if attached to target.                                *
 *                                                                                             *
 *    This routine is called when the specified target is being removed from the game. If this *
 *    animation happens to be attached to this object, then the animation must be remove as    *
 *    well.                                                                                    *
 *                                                                                             *
 * INPUT:   target   -- The target that is about to be destroyed.                              *
 *                                                                                             *
 *          all      -- Is the target being destroyed RIGHT NOW? If not, then it will be       *
 *                      destroyed soon. In that case, the animation should continue to remain  *
 *                      attached for cosmetic reasons.                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/30/1995 JLB : Created.                                                                 *
 *   07/02/1995 JLB : Detach is a precursor to animation destruction.                          *
 *=============================================================================================*/
void AnimClass::Detach(TARGET target, bool all)
{
	Validate();
	if (all) {
		if (VirtualAnim && VirtualAnim->As_Target() == target) {
			VirtualAnim = NULL;
		}
		if (Object && Object->As_Target() == target) {
			Map.Remove(this, In_Which_Layer());
			Object = NULL;
			IsToDelete = true;
		}
	}
}