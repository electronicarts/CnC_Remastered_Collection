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

/* $Header: /CounterStrike/ANIM.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
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
 *   AnimClass::AnimClass -- The constructor for animation objects.                            *
 *   AnimClass::Attach_To -- Attaches animation to object specified.                           *
 *   AnimClass::Sort_Above -- Sorts the animation above the target specified.                  *
 *   AnimClass::Center_Coord -- Determine center of animation.                                 *
 *   AnimClass::Detach -- Remove animation if attached to target.                              *
 *   AnimClass::Do_Atom_Damage -- Do atom bomb damage centered around the cell specified.      *
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
 *   Anim_From_Name -- Given a name, this finds the corresponding anim type.                   *
 *   Shorten_Attached_Anims -- Reduces attached animation durations.                           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#define   VIC   1

/***********************************************************************************************
 * Anim_From_Name -- Given a name, this finds the corresponding anim type.                     *
 *                                                                                             *
 *    This routine will convert the supplied ASCII name into the animation type that it        *
 *    represents.                                                                              *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII name to convert.                                     *
 *                                                                                             *
 * OUTPUT:  Returns with the animation type that matches the name specified. If no match could *
 *          be found, then ANIM_NONE is returned.                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
AnimType Anim_From_Name(char const * name)
{
  #ifdef VIC
	if (name == NULL) return(ANIM_NONE);

	for (int anim = ANIM_FIRST; anim < ANIM_COUNT; anim++) {
		if (stricmp(AnimTypeClass::As_Reference((AnimType)anim).IniName, name) == 0) {
			return((AnimType)anim);
		}
	}
#endif
	return(ANIM_NONE);
}


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
	if (obj != NULL) {
		for (int index = 0; index < Anims.Count(); index++) {
			AnimClass & anim = * Anims.Ptr(index);

			if (As_Object(anim.xObject) == obj) {
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
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);

	if (xObject != TARGET_NONE) {
		return(Coord_Add(As_Object(xObject)->Sort_Y(), 0x00010000L));
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
	if (Class->IsGroundLayer || *this == ANIM_LZ_SMOKE) {
		return(Coord_Add(Center_Coord(), XYP_COORD(0, 14)));
	}
#endif
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
 *   02/02/1996 JLB : Coordinate based on visual center of object.                             *
 *=============================================================================================*/
COORDINATE AnimClass::Center_Coord(void) const
{
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);

	if (xObject != TARGET_NONE) {
		return(Coord_Add(Coord, As_Object(xObject)->Target_Coord()));
	}
#endif
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
bool AnimClass::Render(bool forced) const
{
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);

	if (Delay) return(false);
	if (Map[Center_Coord()].IsVisible) {
		const_cast<AnimClass*>(this)->IsToDisplay = true;	// const_cast. ST - 5/8/2019
	}
#endif
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
void AnimClass::Draw_It(int x, int y, WindowNumberType window) const
{
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);

	bool render_legacy = !IsInvisible && (Class->VirtualAnim == ANIM_NONE || window != WINDOW_VIRTUAL);
	bool render_virtual = Target_Legal(VirtualAnimTarget) && window == WINDOW_VIRTUAL;

	if (render_legacy) {
		BStart(BENCH_ANIMS);

		IsTheaterShape = Class->IsTheater;

		void const * shapefile = Get_Image_Data();
		if (shapefile != NULL) {
			void const * transtable = NULL;
			int shapenum = Class->Start + Fetch_Stage();
			void const * remap = NULL;

			/*
			**	If the translucent table hasn't been determined yet, then check to see if it
			**	should use the white or normal translucent tables.
			*/
			if (transtable == NULL && Class->IsWhiteTrans) transtable = DisplayClass::WhiteTranslucentTable;
			if (transtable == NULL && Class->IsTranslucent) transtable = DisplayClass::TranslucentTable;
			if (Class->Type == ANIM_ATOM_BLAST) transtable = Map.UnitShadow;

			/*
			**	Set the shape flags to properly take into account any fading or ghosting
			**	table necessary.
			*/
			ShapeFlags_Type flags = SHAPE_CENTER|SHAPE_WIN_REL;
			if (transtable != NULL) flags = flags | SHAPE_GHOST;

			/*
			**	Draw the animation shape, but ignore legacy if beyond normal stage count.
			*/
			if ((window == WINDOW_VIRTUAL) || (Fetch_Stage() < Class->Stages)) {
				// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
				CC_Draw_Shape(this, shapefile, shapenum, x, y, window, flags, remap, transtable, DIR_N, Class->VirtualScale);
			}
		}
		IsTheaterShape = false;
		BEnd(BENCH_ANIMS);
	}
	if (render_virtual) {
		AnimClass* virtual_anim = As_Animation(VirtualAnimTarget);
		virtual_anim->Make_Visible();
		virtual_anim->Draw_It(x, y, window);
		virtual_anim->Make_Invisible();
	}
#endif
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
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);

	if (ObjectClass::Mark(mark)) {
		Map.Refresh_Cells(Coord_Cell(Center_Coord()), Overlap_List());
//		ObjectClass::Mark(mark);
		return(true);
	}
#endif
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
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);
	static short const OverlapAtom[] = {
		(-MAP_CELL_W * 2) - 1, (-MAP_CELL_W * 2), (-MAP_CELL_W * 2) + 1,
		(-MAP_CELL_W * 1) - 1, (-MAP_CELL_W * 1), (-MAP_CELL_W * 1) + 1,
		(-MAP_CELL_W * 0) - 1, (-MAP_CELL_W * 0), (-MAP_CELL_W * 0) + 1,
		( MAP_CELL_W * 1) - 1, ( MAP_CELL_W * 1), ( MAP_CELL_W * 1) + 1,
		( MAP_CELL_W * 2) - 1, ( MAP_CELL_W * 2), ( MAP_CELL_W * 2) + 1,
 		REFRESH_EOL
	};

	if (IsToDelete) {
		static short const _list[] = {REFRESH_EOL};
		return(_list);
	}

	if (Class->Type == ANIM_ATOM_BLAST) {
		return(OverlapAtom);
	}

#ifdef PARTIAL
IsTheaterShape = Class->IsTheater;
	if (Class->Get_Image_Data() != NULL) {
		int shapenum = Class->Start + Fetch_Stage();
		int count = Get_Build_Frame_Count(Class->Get_Image_Data());
		shapenum = min(shapenum, count-1);

		if (Class->DimensionData == NULL) {
			Class->DimensionData = new Rect [count];
		}
		if (Class->DimensionData != NULL && !Class->DimensionData[shapenum].Is_Valid()) {
			Class->DimensionData[shapenum] = Shape_Dimensions(Class->Get_Image_Data(), shapenum);
IsTheaterShape = false;
			return(Coord_Spillage_List(Center_Coord(), Class->DimensionData[shapenum]));
		}
	}
IsTheaterShape = false;
#endif
#endif
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
short const * AnimClass::Occupy_List(bool) const
{
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);

	static short _simple[] = {REFRESH_EOL};

#endif
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
	Anims.Free_All();
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
void * AnimClass::operator new(size_t)
{
	void * ptr = Anims.Allocate();
	if (ptr != NULL) {
		((AnimClass *)ptr)->Set_Active();
	} else {
		GlyphX_Debug_Print("AnimClass::operator new FAILED");
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
void AnimClass::operator delete(void * ptr)
{
	if (ptr != NULL) {
		((AnimClass *)ptr)->IsActive = false;
	}
	Anims.Free((AnimClass *)ptr);
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
AnimClass::AnimClass(AnimType animnum, COORDINATE coord, unsigned char timedelay, unsigned char loop) :
	ObjectClass(RTTI_ANIM, Anims.ID(this)),
	Class(AnimTypes.Ptr((int)animnum)),
	xObject(TARGET_NONE),
	SortTarget(TARGET_NONE),
	OwnerHouse(HOUSE_NONE),
	Loops(1),
	IsToDelete(false),
	IsBrandNew(true),
	IsInvisible(false),
	Delay(timedelay),
	Accum(0),
	AttachLayer(LAYER_NONE)
{
#ifdef VIC
	if (Class->Stages == -1) {
IsTheaterShape = Class->IsTheater;
		((int&)Class->Stages) = Get_Build_Frame_Count(Class->Get_Image_Data());
IsTheaterShape = false;

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

	if (Class->IsGroundLayer) {
		Height = FLIGHT_LEVEL;
	}

	AnimClass::Unlimbo(coord);

	VisibleFlags = 0xffff;

	/*
	**	Drop zone smoke always reveals the map around itself.
	*/
	if (*this == ANIM_LZ_SMOKE) {
		// Added PlayerPtr here as Sight_From now needs to know who to perform the action for. This should be OK as long as it's not used in MP. ST - 8/2/2019 2:34PM
		Map.Sight_From(Coord_Cell(coord), Rule.DropZoneRadius / CELL_LEPTON_W, PlayerPtr, false);
	}

	Loops = (unsigned char)(max(loop, 1) * Class->Loops);
	Loops = (unsigned char)max(Loops, 1);

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
		AnimClass* virtual_anim = new AnimClass(Class->VirtualAnim, 0, timedelay, loop);
		if (virtual_anim != NULL) {
			virtual_anim->Make_Invisible();
			VirtualAnimTarget = virtual_anim->As_Target();
		} else {
			VirtualAnimTarget = TARGET_NONE;
		}
	} else {
		VirtualAnimTarget = TARGET_NONE;
	}
#endif
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
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);
	if (GameActive) {

		/*
		**	If this anim is attached to another object
		**	then check to see if this is the last anim attached to it. If this
		**	is the case, then inform the object that it is no longer attached to
		**	an animation.
		*/
		if (Target_Legal(xObject) && As_Object(xObject) != NULL) {
			ObjectClass * to = As_Object(xObject);

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
				if (Anims.Ptr(index) != this && Anims.Ptr(index)->xObject == xObject) break;
			}

			/*
			**	Tell the object that it is no longer being damaged.
			*/
			if (index == Anims.Count()) {
				to->Fire_Out();
				if (to->In_Which_Layer() == LAYER_GROUND) to->Mark(MARK_OVERLAP_UP);
				to->IsAnimAttached = false;
				if (to->In_Which_Layer() == LAYER_GROUND) to->Mark(MARK_OVERLAP_DOWN);
			}
			Coord = Coord_Add(to->Center_Coord(), Coord);
			xObject = TARGET_NONE;
		}

		Limbo();
	}

	xObject = TARGET_NONE;
	Class = 0;
	ID = -1;

#endif
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
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);

	/*
	**	For ground level based animations (ones that can run slowly as well as
	**	occur behind other ground objects) always cause the cell to be redrawn.
	*/
#ifdef PARTIAL
	if (!Delay && Class->IsGroundLayer) {
		Map.Refresh_Cells(Coord_Cell(Center_Coord()), Overlap_List());
	}
#else
	Map.Refresh_Cells(Coord_Cell(Center_Coord()), Overlap_List());
#endif

	/*
	**	Special case check to make sure that building on top of a smoke marker
	**	causes the smoke marker to vanish.
	*/
	if (Class->Type == ANIM_LZ_SMOKE && Map[Center_Coord()].Cell_Building()) {
		IsToDelete = true;
	}

	/*
	**	Don't allow corpses on rocks, rivers, and water (looks really weird).
	*/
	if (Class->Type >= ANIM_CORPSE1 && Class->Type <= ANIM_CORPSE3) {
		LandType land = Map[Center_Coord()].Land_Type();
		if (land == LAND_ROCK || land == LAND_WATER || land == LAND_RIVER) {
			IsToDelete = true;
		}
	}

	/*
	**	Delete this animation and bail early if the animation is flagged to be deleted
	**	immediately.
	*/
	if (IsToDelete) {
		delete this;
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

#ifdef FIXIT_MULTI_SAVE
	if (Class->Stages == -1) {
		IsTheaterShape = Class->IsTheater;
		((int&)Class->Stages) = Get_Build_Frame_Count(Class->Get_Image_Data());
		IsTheaterShape = false;
	}
	if (Class->LoopEnd == -1) {
		((int&)Class->LoopEnd) = Class->Stages;
	}
#endif

	if (Delay) {
		Delay--;
		if (!Delay) {
			Start();
		}
	} else {

#ifdef FIXIT_MULTI_SAVE
		if (Class->Stages == -1) {
			IsTheaterShape = Class->IsTheater;
			((int&)Class->Stages) = Get_Build_Frame_Count(Class->Get_Image_Data());
			IsTheaterShape = false;
		}
		if (Class->LoopEnd == -1) {
			((int&)Class->LoopEnd) = Class->Stages;
		}
#endif

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
			if (xObject != TARGET_NONE && Class->Damage > 0 && stage < Class->Stages) {
				Accum += Class->Damage;

				if (Accum >= 1) {

					/*
					**	Administer the damage. If the object was destroyed by this anim,
					**	then the attached damaging anim is also destroyed.
					*/
					int damage = Accum;
					Accum -= damage;
					if (As_Object(xObject)->Take_Damage(damage, 0, WARHEAD_FIRE) == RESULT_DESTROYED) {
						delete this;
						if (Target_Legal(VirtualAnimTarget)) {
							delete As_Animation(VirtualAnimTarget);
						}
						return;
					}
				}
			}

			/*
			**	During the biggest stage (covers the most ground), perform any ground altering
			**	action required. This masks craters and scorch marks, so that they appear
			**	naturally rather than "popping" into existence while in plain sight.
			*/
			if (Class->Biggest && Class->Start+stage == Class->Biggest) {
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
						if (!Target_Legal(VirtualAnimTarget)) {
							if (Class->ChainTo != ANIM_NONE) {
								Chain();
							}
							else {
								delete this;
							}
						}
					}
					else {
						if ((Class->VirtualStages < 0) || (stage >= Class->VirtualStages)) {
							if (Class->ChainTo != ANIM_NONE) {
								Chain();
							}
							else {
								delete this;
							}
						}
					}
				}
			}
		}
	}
#endif
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
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);

	if (obj == NULL) return;
	assert(obj->IsActive);

	if (obj->In_Which_Layer() == LAYER_GROUND) obj->Mark(MARK_OVERLAP_UP);
	obj->IsAnimAttached = true;
	if (obj->In_Which_Layer() == LAYER_GROUND) obj->Mark(MARK_OVERLAP_DOWN);
	Limbo();
	xObject = obj->As_Target();
	Unlimbo(Coord);
	AttachLayer = In_Which_Layer();
	Height = (AttachLayer == LAYER_GROUND) ? FLIGHT_LEVEL : 0;
	Coord = Coord_Sub(Coord, obj->Target_Coord());
#endif
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
#ifdef VIC
	SortTarget = target;
#endif
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
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);

	if (AttachLayer != LAYER_NONE) {
		return AttachLayer;
	}

	if (Class->Type >= ANIM_CORPSE1 && Class->Type <= ANIM_CORPSE3) {
		return(LAYER_SURFACE);
	}

	if (Target_Legal(xObject)) {
		return As_Object(xObject)->In_Which_Layer();
	}

	if (Class->IsGroundLayer) {
		return(LAYER_GROUND);
	}
#endif
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
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);

	Mark();

	/*
	**	Play the sound effect for this animation.
	*/
	Sound_Effect(Class->Sound, Coord);

	/*
	**	If the stage where collateral effects occur is the first stage of the animation, then
	**	perform this action now. Subsequent checks against this stage value starts with the
	**	second frame of the animation.
	*/
	if (!Class->Biggest) {
		Middle();
	}
#endif
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
 *   10/17/1995 JLB : Ion camera added.                                                        *
 *=============================================================================================*/
void AnimClass::Middle(void)
{
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);

	CELL cell = Coord_Cell(Center_Coord());
	CellClass * cellptr = &Map[cell];

	if (Class->Type == ANIM_ATOM_BLAST) {
		Do_Atom_Damage(OwnerHouse, cell);
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

	AnimClass * newanim;

	/*
	**	If this animation spawns side effects during its lifetime, then
	**	do so now. Usually, these side effects are in the form of other
	**	animations.
	*/
	switch (Class->Type) {
		case ANIM_NAPALM1:
		case ANIM_NAPALM2:
		case ANIM_NAPALM3:
			new AnimClass(ANIM_FIRE_SMALL, Map.Closest_Free_Spot(Coord_Scatter(Center_Coord(), 0x0040), true), 0, Random_Pick(1, 2));
			if (Percent_Chance(50)) {
				new AnimClass(ANIM_FIRE_SMALL, Map.Closest_Free_Spot(Coord_Scatter(Center_Coord(), 0x00A0), true), 0, Random_Pick(1, 2));
			}
			if (Percent_Chance(50)) {
				new AnimClass(ANIM_FIRE_MED, Map.Closest_Free_Spot(Coord_Scatter(Center_Coord(), 0x0070), true), 0, Random_Pick(1, 2));
			}
			break;

		case ANIM_FIRE_MED:
		case ANIM_FIRE_MED2:
			newanim = new AnimClass(ANIM_FIRE_SMALL, Center_Coord(), 0, Random_Pick(1, 2));
			if (newanim != NULL && xObject != TARGET_NONE) {
				newanim->Attach_To(As_Object(xObject));
			}
			break;

		default:
			break;
	}
#endif
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

		Class = (AnimTypeClass *)&AnimTypeClass::As_Reference(Class->ChainTo);

		if (Class->Stages == -1) {
			IsTheaterShape = Class->IsTheater;
			((int&)Class->Stages) = Get_Build_Frame_Count(Class->Get_Image_Data());
			IsTheaterShape = false;
		}
		if (Class->LoopEnd == -1) {
			((int&)Class->LoopEnd) = Class->Stages;
		}

		IsToDelete = false;
		Loops = Class->Loops;
		Accum = 0;
		if (Class->IsNormalized) {
			Set_Rate(Options.Normalize_Delay(Class->Delay));
		} else {
			Set_Rate(Class->Delay);
		}
		Set_Stage(Class->Start);
		Start();
	}
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
#ifdef VIC
	assert(Anims.ID(this) == ID);
	assert(IsActive);

	if (all) {
		if (VirtualAnimTarget && VirtualAnimTarget == target) {
			VirtualAnimTarget = TARGET_NONE;
		}
		if (xObject == target) {
			Map.Remove(this, In_Which_Layer());
			xObject = TARGET_NONE;
			AttachLayer = LAYER_NONE;
			IsToDelete = true;
			Mark(MARK_UP);
		}
	}
#endif
}


/***********************************************************************************************
 * AnimClass::Do_Atom_Damage -- Do atom bomb damage centered around the cell specified.        *
 *                                                                                             *
 *    This routine will apply damage around the ground-zero cell specified.                    *
 *                                                                                             *
 * INPUT:   ownerhouse  -- The owner of this atom bomb.                                        *
 *                                                                                             *
 *          cell        -- The ground zero location to apply the atom bomb damage.             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void AnimClass::Do_Atom_Damage(HousesType ownerhouse, CELL cell)
{
#ifdef VIC
	/*
	**	Find someone to blame the explosion on. This is necessary in
	**	order to properly enact retribution and record the kill for
	**	score purposes.
	*/
	BuildingClass * building = NULL;
	TechnoClass * backup = NULL;
	if (ownerhouse != HOUSE_NONE) {
		for (int index = 0; index < Logic.Count(); index++) {
			ObjectClass * obj = Logic[index];

			if (obj != NULL && obj->Is_Techno() && obj->Owner() == ownerhouse) {
				backup = (TechnoClass *)obj;
				if (obj->What_Am_I() == RTTI_BUILDING && *((BuildingClass *)obj) == STRUCT_MSLO) {
					building = (BuildingClass *)obj;
					break;
				}
			}
		}

		if (building == NULL) building = (BuildingClass *)backup;
	}

	int radius;
	int rawdamage;
	if (Session.Type == GAME_NORMAL) {
		radius = 4;
		rawdamage = Rule.AtomDamage;
		//WhitePalette.Set(FADE_PALETTE_SLOW, Call_Back);		//TO_FIX. ST 5/8/2019
	} else {
		radius = 3;
		rawdamage = Rule.AtomDamage/5;
	}

	Wide_Area_Damage(Cell_Coord(cell), radius * CELL_LEPTON_W, rawdamage, building, WARHEAD_FIRE);
	Shake_The_Screen(3);
	if (Session.Type == GAME_NORMAL) {
		//GamePalette.Set(FADE_PALETTE_SLOW, Call_Back);	//TO_FIX. ST 5/8/2019
	}
#endif
}