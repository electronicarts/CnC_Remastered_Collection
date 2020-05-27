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

/* $Header: /CounterStrike/BULLET.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BULLET.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 23, 1994                                               *
 *                                                                                             *
 *                  Last Update : October 10, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BulletClass::AI -- Logic processing for bullet.                                           *
 *   BulletClass::BulletClass -- Bullet constructor.                                           *
 *   BulletClass::Bullet_Explodes -- Performs bullet explosion logic.                          *
 *   BulletClass::Detach -- Removes specified target from this bullet's targeting system.      *
 *   BulletClass::Draw_It -- Displays the bullet at location specified.                        *
 *   BulletClass::In_Which_Layer -- Fetches the layer that the bullet resides in.              *
 *   BulletClass::Init -- Clears the bullets array for scenario preparation.                   *
 *   BulletClass::Is_Forced_To_Explode -- Checks if bullet should explode NOW.                 *
 *   BulletClass::Mark -- Performs related map refreshing under bullet.                        *
 *   BulletClass::Occupy_List -- Determines the bullet occupation list.                        *
 *   BulletClass::Shape_Number -- Fetches the shape number for the bullet object.              *
 *   BulletClass::Sort_Y -- Sort coordinate for bullet rendering.                              *
 *   BulletClass::Target_Coord -- Fetches coordinate to use when firing on this object.        *
 *   BulletClass::Unlimbo -- Transitions a bullet object into the game render/logic system.    *
 *   BulletClass::delete -- Bullet memory delete.                                              *
 *   BulletClass::new -- Allocates memory for bullet object.                                   *
 *   BulletClass::~BulletClass -- Destructor for bullet objects.                               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***********************************************************************************************
 * BulletClass::BulletClass -- Bullet constructor.                                             *
 *                                                                                             *
 *    This is the constructor for the bullet class. It handles all                             *
 *    initialization of the bullet and starting it in motion toward its                        *
 *    target.                                                                                  *
 *                                                                                             *
 * INPUT:   id       -- The type of bullet this is (could be missile).                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/02/1994 JLB : Created.                                                                 *
 *   06/20/1994 JLB : Firer is a base class pointer.                                           *
 *   12/10/1994 JLB : Auto calculate range optional.                                           *
 *   12/12/1994 JLB : Handles small arms as an instantaneous effect.                           *
 *   12/23/1994 JLB : Fixed scatter algorithm for non-homing projectiles.                      *
 *   12/31/1994 JLB : Removed range parameter (not needed).                                    *
 *=============================================================================================*/
BulletClass::BulletClass(BulletType id, TARGET target, TechnoClass * payback, int strength, WarheadType warhead, int speed) :
	ObjectClass(RTTI_BULLET, Bullets.ID(this)),
	Class(BulletTypes.Ptr((int)id)),
	Payback(payback),
	PrimaryFacing(DIR_N),
	IsInaccurate(false),
	IsToAnimate(false),
	IsLocked(true),
	TarCom(target),
	MaxSpeed(speed),
	Warhead(warhead)
{
	Strength = strength;
	Height = FLIGHT_LEVEL;
}


/***********************************************************************************************
 * BulletClass::~BulletClass -- Destructor for bullet objects.                                 *
 *                                                                                             *
 *    The bullet destructor must detect if a dog has been attached to this bullet. If so,      *
 *    then the attached dog must be unlimboed back onto the map. This operation is necessary   *
 *    because, unlike other objects, the dog flies with the bullet it fires.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
BulletClass::~BulletClass(void)
{
	if (GameActive) {

		/*
		**	SPECIAL CASE:
		**	The dog is attached to the dog bullet in a limbo state. When the bullet is
		**	destroyed, the dog must come back out of limbo at the closest location possible to
		**	the bullet.
		*/
		if (Payback != NULL && Payback->What_Am_I() == RTTI_INFANTRY && ((InfantryClass *)Payback)->Class->IsDog) {

			InfantryClass * dog = (InfantryClass *)Payback;
			if (dog) {
				bool unlimbo = false;
				DirType dogface = dog->PrimaryFacing;
				COORDINATE newcoord = Coord;

				/*
				**	Ensure that the coordinate, that the dog is to appear at, is legal. If not,
				**	then find a nearby legal location.
				*/
				if (Can_Enter_Cell(newcoord) != MOVE_OK) {
					newcoord = Map.Nearby_Location(Coord_Cell(newcoord), dog->Class->Speed);
				}

				/*
				** Try to put the dog down where the target impacted.  If we can't
				** put it in that cell, then scan through the adjacent cells,
				** starting with our current heading, until we find a place where
				** we can put him down.  If all 8 adjacent cell checks fail, then
				** just delete the dog.
				*/
				for (int i = -1; i < 8; i++) {
					if (i != -1) {
						newcoord = Adjacent_Cell(Coord, FacingType(i));
					}
					ScenarioInit++;
					if (dog->Unlimbo(newcoord, dog->PrimaryFacing)) {
						dog->Mark(MARK_DOWN);
						dog->Do_Action(DO_DOG_MAUL, true);
						if (dog->WasSelected) {
							dog->Select();
						}
						ScenarioInit--;

						unlimbo = true;
						break;
					}
					ScenarioInit--;
				}

				Payback = 0;

				if (!unlimbo) {
					delete dog;
				}
			}
		}
		BulletClass::Limbo();
	}

	Class=0;
	Payback=0;
}


/***********************************************************************************************
 * BulletClass::new -- Allocates memory for bullet object.                                     *
 *                                                                                             *
 *    This function will "allocate" a block of memory for a bullet object.                     *
 *    This memory block is merely lifted from a fixed pool of blocks.                          *
 *                                                                                             *
 * INPUT:   size  -- The size of the memory block needed.                                      *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to an available bullet object block.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void * BulletClass::operator new(size_t )
{
	void * ptr = Bullets.Allocate();
	if (ptr) {
		((BulletClass *)ptr)->Set_Active();
	}
	return(ptr);
}


/***********************************************************************************************
 * BulletClass::delete -- Bullet memory delete.                                                *
 *                                                                                             *
 *    Since bullets memory is merely "allocated" out of a pool, it never                       *
 *    actually gets deleted.                                                                   *
 *                                                                                             *
 * INPUT:   ptr   -- Generic pointer to bullet object.                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BulletClass::operator delete(void * ptr)
{
	if (ptr) {
		((BulletClass *)ptr)->IsActive = false;
	}
	Bullets.Free((BulletClass *)ptr);
}


/***********************************************************************************************
 * BulletClass::Occupy_List -- Determines the bullet occupation list.                          *
 *                                                                                             *
 *    This function will determine the cell occupation list and return a pointer to it. Most   *
 *    bullets are small and the list is usually short, but on occasion, it can be a list that  *
 *    rivals the size of regular vehicles.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the cell offset list that covers all the cells a bullet  *
 *          is over.                                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1994 JLB : Created.                                                                 *
 *   01/05/1995 JLB : Handles projectiles with altitude.                                       *
 *=============================================================================================*/
short const * BulletClass::Occupy_List(bool) const
{
	assert(Bullets.ID(this) == ID);
	assert(IsActive);

	/*
	**	Super-gigundo units use the >= 64 coord spillage list logic.
	*/
	if (Class->IsGigundo) {
		static short _list[] = {
			-1, 0, 1,
			MAP_CELL_W*1-1, MAP_CELL_W*1, MAP_CELL_W*1+1,
			-MAP_CELL_W*1-1, -MAP_CELL_W*1, -MAP_CELL_W*1+1,
			MAP_CELL_W*2-1, MAP_CELL_W*2, MAP_CELL_W*2+1,
			-MAP_CELL_W*2-1, -MAP_CELL_W*2, -MAP_CELL_W*2+1,
			-MAP_CELL_W*3-1, -MAP_CELL_W*3, -MAP_CELL_W*3+1,
			REFRESH_EOL
		};
		return(_list);
//		return(Coord_Spillage_List(Coord, 64));
	}

	/*
	**	Flying units need a special adjustment to the spillage list to take into account
	**	that the bullet imagery and the shadow are widely separated.
	*/
	if (Height > 0) {
		static short _list[25];
		const short * ptr = Coord_Spillage_List(Coord, 5);
		int index = 0;
		CELL cell1 = Coord_Cell(Coord);

		while (ptr[index] != REFRESH_EOL) {
			_list[index] = ptr[index];
			index++;
		}

		COORDINATE coord = Coord_Move(Coord, DIR_N, Height);
		CELL cell2 = Coord_Cell(coord);
		ptr = Coord_Spillage_List(coord, 5);
		while (*ptr != REFRESH_EOL) {
			_list[index++] = *ptr + (cell2 - cell1);
			ptr++;
		}
		_list[index] = REFRESH_EOL;
		return(_list);
	}

	return(Coord_Spillage_List(Coord, 10));
}


/***********************************************************************************************
 * BulletClass::Mark -- Performs related map refreshing under bullet.                          *
 *                                                                                             *
 *    This routine marks the objects under the bullet so that they will                        *
 *    be redrawn. This is necessary as the bullet moves -- objects under                       *
 *    its path need to be restored.                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BulletClass::Mark(MarkType mark)
{
	assert(Bullets.ID(this) == ID);
	assert(IsActive);

	if (ObjectClass::Mark(mark)) {
		if (!Class->IsInvisible) {
			Map.Refresh_Cells(Coord_Cell(Coord), Occupy_List());
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * BulletClass::AI -- Logic processing for bullet.                                             *
 *                                                                                             *
 *    This routine will perform all logic (flight) logic on the bullet.                        *
 *    Primarily this is motion, fuse tracking, and detonation logic. Call                      *
 *    this routine no more than once per bullet per game tick.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BulletClass::AI(void)
{
	assert(Bullets.ID(this) == ID);
	assert(IsActive);

	COORDINATE	coord;

	ObjectClass::AI();

	if (!IsActive) return;

	/*
	**	Ballistic objects are handled here.
	*/
	bool forced = false;			// Forced explosion.
	if ((Class->IsArcing || Class->IsDropping) && !IsFalling) {
		forced = true;
	}

	/*
	**	Homing projectiles constantly change facing to face toward the target but
	**	they only do so every other game frame (improves game speed and makes
	**	missiles not so deadly).
	*/
	if ((Frame & 0x01) && Class->ROT != 0 && Target_Legal(TarCom)) {
		PrimaryFacing.Set_Desired(Direction256(Coord, ::As_Coord(TarCom)));
	}

	/*
	**	Move the projectile forward according to its speed
	**	and direction.
	*/
	coord = Coord;
	if (Class->IsFlameEquipped) {
		if (IsToAnimate) {
			if (stricmp(Class->GraphicName, "FB1") == 0) {
				new AnimClass(ANIM_FBALL_FADE, coord, 1);
			} else {
				new AnimClass(ANIM_SMOKE_PUFF, coord, 1);
			}
		}
		IsToAnimate = !IsToAnimate;
	}

	/*
	**	Handle any body rotation at this time. This process must
	**	occur every game fame in order to achieve smooth rotation.
	*/
	if (PrimaryFacing.Is_Rotating()) {
		PrimaryFacing.Rotation_Adjust(Class->ROT);
	}
	switch (Physics(coord, PrimaryFacing)) {
		/*
		**	When a projectile reaches the edge of the world, it
		**	vanishes from existence -- presumed to explode off
		**	map.
		*/
		case IMPACT_EDGE:
			Mark();
			if (Payback != NULL && Class->Type == BULLET_GPS_SATELLITE) {
				
				bool reveal = false;
				if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {
					if (Payback->House == PlayerPtr) {
						reveal = true;
					}
				} else {
					if (Payback->House->IsHuman) {
						reveal = true;
					}
				}
				if (reveal) {
					if (!Map.Is_Radar_Active()) {
						Map.Radar_Activate(1);
					}
					for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
						Map.Map_Cell(cell, Payback->House);
					}
					Map.RadarClass::Flag_To_Redraw(true);
				}
				Payback->House->IsGPSActive = true;
				Payback->House->IsVisionary = true;
			}
#ifdef OBSOLETE
			/*
			** Hack: If it's the artificial nukes, don't let the bullets come down (as
			** they're the only ones that blow up).  We know it's artificial if you're
			** at tech level 10 or below, because you can't build the nuclear silo until
			** tech level 15 or so.
			*/
			if (Payback != NULL && Class->Type == BULLET_NUKE_UP && Payback->House->Control.TechLevel <= 10) {
				BulletClass * bullet = new BulletClass(BULLET_NUKE_DOWN, ::As_Target(Payback->House->NukeDest), Payback, 200, WARHEAD_NUKE, MPH_VERY_FAST);
				if (bullet) {
					int celly = Cell_Y(Payback->House->NukeDest);
					celly -= 15;
					if (celly < 1) celly = 1;
					COORDINATE start = Cell_Coord(XY_Cell(Cell_X(Payback->House->NukeDest), celly));
					if (!bullet->Unlimbo(start, DIR_S)) {
						delete bullet;
					}
				}
			}
#endif
			delete this;
			break;

		default:
		case IMPACT_NONE:

		/*
		**	The projectile has moved. Check its fuse. If detonation
		**	is signaled, then do so. Otherwise, just move.
		*/
		case IMPACT_NORMAL:
			Mark();
//			if(Class->Type == BULLET_NUKE_DOWN) {
//				Render(true);
//			}
			if (Class->Type == BULLET_NUKE_UP) {
				if (Payback != NULL) {
					if (Distance(Payback->As_Target()) > 0x0C00) {
						delete this;
						return;
					}
				}
			}
			Coord = coord;

			/*
			**	See if the bullet should be forced to explode now in spite of what
			**	the fuse would otherwise indicate. Maybe the bullet hit a wall?
			*/
			if (!forced) {
				forced = Is_Forced_To_Explode(Coord);
			}

			/*
			**	If the bullet is not to explode, then perform normal flight
			**	maintenance (usually nothing). Otherwise, explode and then
			**	delete the bullet.
			*/
			if (!forced && (Class->IsDropping || !Fuse_Checkup(Coord))) {
				/*
				**	Certain projectiles lose strength when they travel.
				*/
				if (Class->IsDegenerate && Strength > 5) {
					Strength--;
				}

			} else {
				Bullet_Explodes(forced);
				delete this;
			}
			break;
	}

}


/***********************************************************************************************
 * BulletClass::Shape_Number -- Fetches the shape number for the bullet object.                *
 *                                                                                             *
 *    Use this routine to fetch a shape number to use for this bullet object.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the shape number to use when drawing this bullet.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int BulletClass::Shape_Number(void) const
{
	int shapenum = 0;

	if (!Class->IsFaceless) {
		shapenum = UnitClass::BodyShape[Dir_To_32(PrimaryFacing)];
	}

	/*
	**	For tumbling projectiles, fetch offset stage.
	*/
	if (Class->Tumble > 0) {
		shapenum += (long)Frame % Class->Tumble;
	}

	return(shapenum);
}


/***********************************************************************************************
 * BulletClass::Draw_It -- Displays the bullet at location specified.                          *
 *                                                                                             *
 *    This routine displays the bullet visual at the location specified.                       *
 *                                                                                             *
 * INPUT:   x,y   -- The center coordinate to render the bullet at.                            *
 *                                                                                             *
 *          window   -- The window to clip to.                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1994 JLB : Created.                                                                 *
 *   06/27/1994 JLB : Takes a window clipping parameter.                                       *
 *   01/08/1995 JLB : Handles translucent colors if necessary.                                 *
 *=============================================================================================*/
void BulletClass::Draw_It(int x, int y, WindowNumberType window) const
{
	assert(Bullets.ID(this) == ID);
	assert(IsActive);

	/*
	**	Certain projectiles aren't visible. This includes small bullets (which are actually
	**	invisible) and flame thrower flames (which are rendered as an animation instead of a projectile).
	*/
	if (Class->IsInvisible) return;

	/*
	**	If there is no shape loaded for this object, then
	**	it obviously can't be rendered -- just bail.
	*/
	void const * shapeptr = Get_Image_Data();
	if (shapeptr == NULL) return;

	/*
	**	Get the basic shape number for this projectile.
	*/
	int shapenum = Shape_Number();

	/*
	**	For flying projectiles, draw the shadow and adjust the actual projectile body
	**	render position.
	*/
	if (Height > 0 && Class->IsShadow) {

		if (Class->IsParachuted) {
			// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
			CC_Draw_Shape(this, AnimTypeClass::As_Reference(ANIM_PARA_BOMB).Get_Image_Data(), 1, x+Lepton_To_Pixel(Height/2), y+10, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING, NULL, DisplayClass::UnitShadow);
		} else {
			// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
			CC_Draw_Shape(this, shapeptr, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING, NULL, DisplayClass::UnitShadow);
		}
		y -= Lepton_To_Pixel(Height);
	}

	/*
	**	Draw the main body of the projectile.
	*/
	ShapeFlags_Type flags = SHAPE_NORMAL;
	if (Class->IsTranslucent) {
		flags = SHAPE_GHOST;
	}
	if (Class->IsSubSurface) 
	{
		// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
		CC_Draw_Shape(this, shapeptr, shapenum, x, y, window, flags|SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING, NULL, DisplayClass::FadingShade);
	} else {
		// Add 'this' parameter to call new shape draw intercept. ST - 5/22/2019
		CC_Draw_Shape(this, shapeptr, shapenum, x, y, window, flags|SHAPE_CENTER|SHAPE_WIN_REL, NULL, DisplayClass::UnitShadow);
	}
}


/***********************************************************************************************
 * BulletClass::Init -- Clears the bullets array for scenario preparation.                     *
 *                                                                                             *
 *    This routine will zero out the bullet tracking list and object array in preparation for  *
 *    the start of a new scenario. All bullets cease to exists after this function is          *
 *    called.                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BulletClass::Init(void)
{
	Bullets.Free_All();
}


/***********************************************************************************************
 * BulletClass::Detach -- Removes specified target from this bullet's targeting system.        *
 *                                                                                             *
 *    When an object is removed from the game system, it must be removed all targeting and     *
 *    tracking systems as well. This routine is used to remove the specified object from the   *
 *    bullet. If the object isn't part of this bullet's tracking system, then no action is     *
 *    performed.                                                                               *
 *                                                                                             *
 * INPUT:   target   -- The target to remove from this tracking system.                        *
 *                                                                                             *
 *          all      -- Is the target going away for good as opposed to just cloaking/hiding?  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BulletClass::Detach(TARGET target, bool all)
{
	assert(Bullets.ID(this) == ID);
	assert(IsActive);

	ObjectClass * obj = As_Object(target);
	if (Payback != NULL && obj == Payback) {

		/*
		** If we're being called as a result of the dog that fired us being put
		** in limbo, then don't detach.  If for any other reason, detach.
		*/
		if (Payback->What_Am_I() != RTTI_INFANTRY || !((InfantryClass *)Payback)->Class->IsDog) {
			Payback = 0;
		}
	}

	if (all && target == TarCom) {
		TarCom = TARGET_NONE;
	}
}


/***********************************************************************************************
 * BulletClass::Unlimbo -- Transitions a bullet object into the game render/logic system.      *
 *                                                                                             *
 *    This routine is used to take a bullet object that is in limbo and transition it to the   *
 *    game system. A bullet object so transitioned, will be drawn and logic processing         *
 *    performed. In effect, it comes into existence.                                           *
 *                                                                                             *
 * INPUT:   coord -- The location where the bullet object is to appear.                        *
 *                                                                                             *
 *          dir   -- The initial facing for the bullet object.                                 *
 *                                                                                             *
 * OUTPUT:  bool; Was the unlimbo successful?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BulletClass::Unlimbo(COORDINATE coord, DirType dir)
{
	assert(Bullets.ID(this) == ID);
	assert(IsActive);

	/*
	**	Try to unlimbo the bullet as far as the base class is concerned. Use the already
	**	set direction and strength if the "punt" values were passed in. This allows a bullet
	**	to be setup prior to being launched.
	*/
	if (!Class->IsHigh) {
		Height = 0;
	}
	if (ObjectClass::Unlimbo(coord)) {
		Map.Remove(this, In_Which_Layer());

		COORDINATE tcoord = As_Coord(TarCom);

		/*
		**	Homing projectiles (missiles) do NOT override facing. They just fire in the
		**	direction specified and let the chips fall where they may.
		*/
		if (Class->ROT == 0 && !Class->IsDropping) {
			dir = Direction(tcoord);
		}

		/*
		**	Possibly adjust the target if this projectile is inaccurate. This occurs whenever
		**	certain weapons are trained upon targets they were never designed to attack. Example: when
		**	turrets or anti-tank missiles are fired at infantry. Indirect
		**	fire is inherently inaccurate.
		*/
		if (IsInaccurate || Class->IsInaccurate ||
			((Is_Target_Cell(TarCom) || Is_Target_Infantry(TarCom)) && (Warhead == WARHEAD_AP || Class->IsFueled))) {

			/*
			**	Inaccuracy for low velocity or homing projectiles manifests itself as a standard
			**	Circular Error of Probability (CEP) algorithm. High speed projectiles usually
			**	just overshoot the target by extending the straight line flight.
			*/
			if (/*Class->ROT != 0 ||*/ Class->IsArcing) {
				int scatterdist = (::Distance(coord, tcoord)/16)-0x0040;
				scatterdist = min(scatterdist, Rule.HomingScatter);
				scatterdist = max(scatterdist, 0);

				dir = (DirType)((dir + (Random_Pick(0, 10)-5)) & 0x00FF);
				tcoord = Coord_Scatter(tcoord, Random_Pick(0, scatterdist));
			} else {
				int scatterdist = (::Distance(coord, tcoord)/16)-0x0040;
				scatterdist = min(scatterdist, Rule.BallisticScatter);
				scatterdist = max(scatterdist, 0);
				tcoord = Coord_Move(tcoord, dir, Random_Pick(0, scatterdist));
			}
		}

		/*
		**	For very fast and invisible projectiles, just make the projectile exist at the target
		**	location and dispense with the actual flight.
		*/
		if (MaxSpeed == MPH_LIGHT_SPEED && Class->IsInvisible) {
			Coord = tcoord;
		}

		/*
		**	Set the range equal to either the class defined range or the calculated
		**	number of game frames it would take for the projectile to reach the target.
		*/
		int range = 0xFF;
		if (!Class->IsDropping) {
			range = (::Distance(tcoord, Coord) / MaxSpeed) + 4;
		}

		/*
		**	Projectile speed is usually the default value for that projectile, but
		**	certain projectiles alter speed according to the distance to the
		**	target.
		*/
		int speed = MaxSpeed;
		if (speed == MPH_LIGHT_SPEED) speed = MPH_IMMOBILE;
		if (Class->IsArcing) {
			speed = MaxSpeed + (Distance(tcoord) / 32);

			/*
			**	Set minimum speed (i.e., distance) for arcing projectiles.
			*/
			speed = max(speed, 25);
		}
		if (!Class->IsDropping) {
			Fly_Speed(255, (MPHType)speed);
		}

		/*
		**	Arm the fuse.
		*/
		Arm_Fuse(Coord, tcoord, range, ((As_Aircraft(TarCom)!=0) ? 0 : Class->Arming));

		/*
		**	Projectiles that make a ballistic flight to impact point must determine a
		**	vertical component for the projectile launch. This is crudely simulated
		**	by biasing ground speed according to target distance and then giving
		**	enough vertical velocity to keep the projectile airborne for the
		**	desired amount of time. The mathematically correct solution would be to
		**	calculate launch angle (given fixed projectile velocity) and then derive
		**	the vertical and horizontal components. That solution would require a
		**	square root and an arcsine lookup table. OUCH!
		*/
		Riser = 0;
		if (Class->IsArcing) {
			IsFalling = true;
			Height = 1;
			Riser = ((Distance(tcoord)/2) / (speed+1)) * Rule.Gravity;
			Riser = max(Riser, 10);
		}
		if (Class->IsDropping) {
			IsFalling = true;
			Height = FLIGHT_LEVEL;
//			Height = Pixel_To_Lepton(24);
			Riser = 0;
			if (Class->IsParachuted) {
				AnimClass * anim = new AnimClass(ANIM_PARA_BOMB, Target_Coord());
//				AnimClass * anim = new AnimClass(ANIM_PARACHUTE, Target_Coord());
				if (anim) {
					anim->Attach_To(this);
				}
			}
		}
		Map.Submit(this, In_Which_Layer());

		PrimaryFacing = dir;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * BulletClass::Target_Coord -- Fetches coordinate to use when firing on this object.          *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate that should be used when firing at the object.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE BulletClass::Target_Coord(void) const
{
	assert(Bullets.ID(this) == ID);
	assert(IsActive);

	return(Coord_Add(XY_Coord(0, -Height), Coord));
}


/***********************************************************************************************
 * BulletClass::Sort_Y -- Sort coordinate for bullet rendering.                                *
 *                                                                                             *
 *    This will return the coordinate to use when sorting this bullet in the display list.     *
 *    Typically, this only occurs for bullets in the ground layer. Since bullets are to be     *
 *    seen a bit more than the normal sorting order would otherwise imply, bias the sort       *
 *    value such that bullets will tend to be drawn on top of the objects.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate to use when sorting this bullet in the display list.   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
COORDINATE BulletClass::Sort_Y(void) const
{
	assert(this != 0);
	assert(IsActive);

	return(Coord_Move(Coord, DIR_S, CELL_LEPTON_H/2));
}


/***********************************************************************************************
 * BulletClass::In_Which_Layer -- Fetches the layer that the bullet resides in.                *
 *                                                                                             *
 *    This examines the bullet to determine what rendering layer it should be in. The          *
 *    normal logic applies unless this is a torpedo. A torpedo is always in the surface        *
 *    layer.                                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the render layer that this bullet should reside in.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
LayerType BulletClass::In_Which_Layer(void) const
{
	if (Class->IsSubSurface) {
		return(LAYER_SURFACE);
	}
	return(ObjectClass::In_Which_Layer());
}


/***********************************************************************************************
 * BulletClass::Is_Forced_To_Explode -- Checks if bullet should explode NOW.                   *
 *                                                                                             *
 *    This routine will examine the bullet and where it is travelling in order to determine    *
 *    if it should prematurely explode. Typical of this would be when a bullet hits a wall     *
 *    or a torpedo hits a ship -- regardless of where the projectile was originally aimed.     *
 *                                                                                             *
 * INPUT:   coord -- The new coordinate to place the bullet at presuming it is forced to       *
 *                   explode and a modification of the bullet's coordinate is needed.          *
 *                   Otherwise, the coordinate is not modified.                                *
 *                                                                                             *
 * OUTPUT:  bool; Should the bullet explode now?                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BulletClass::Is_Forced_To_Explode(COORDINATE & coord) const
{
	coord = Coord;
	CellClass const * cellptr = &Map[coord];

	/*
	**	Check for impact on a wall or other high obstacle.
	*/
	if (!Class->IsHigh && cellptr->Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(cellptr->Overlay).IsHigh) {
		coord = Cell_Coord(Coord_Cell(coord));
		return(true);
	}

	/*
	**	Check to make sure that underwater projectiles (torpedoes) will not
	**	travel in anything but water.
	*/
	if (Class->IsSubSurface) {
		int d = ::Distance(Coord_Fraction(coord), XY_Coord(CELL_LEPTON_W/2, CELL_LEPTON_W/2));
		if (cellptr->Land_Type() != LAND_WATER || (d < CELL_LEPTON_W/3 && cellptr->Cell_Techno() != NULL && cellptr->Cell_Techno() != Payback)) {

			/*
			**	Force explosion to be at center of techno object if one is present.
			*/
			if (cellptr->Cell_Techno() != NULL) {
				coord = cellptr->Cell_Techno()->Target_Coord();
			}

			/*
			**	However, if the torpedo was blocked by a bridge, then force the
			**	torpedo to explode on top of that bridge cell.
			*/
			if (cellptr->Is_Bridge_Here()) {
				coord = Coord_Snap(coord);
			}

			return(true);
		}
	}

	/*
	**	Bullets are generally more effective when they are fired at aircraft.
	*/
	if (Class->IsAntiAircraft && As_Aircraft(TarCom) && Distance(TarCom) < 0x0080) {
		return(true);
	}

	/*
	**	No reason for forced explosion was detected, so return 'false' to
	**	indicate that no forced explosion is required.
	*/
	return(false);
}


/***********************************************************************************************
 * BulletClass::Bullet_Explodes -- Performs bullet explosion logic.                            *
 *                                                                                             *
 *    This handles the exploding bullet action. It will generate the animation and the         *
 *    damage as necessary.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The bullet should be deleted after this routine is called.                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BulletClass::Bullet_Explodes(bool forced)
{
	/*
	**	When the target is reached, explode and do the damage
	**	required of it. For homing objects, don't force the explosion to
	**	match the target position. Non-homing projectiles adjust position so
	**	that they hit the target. This compensates for the error in line of
	**	flight logic.
	*/
	if ( (Payback != NULL && Payback->What_Am_I() == RTTI_INFANTRY && ((InfantryClass *)Payback)->Class->IsDog) ||
		(!forced && !Class->IsArcing && Class->ROT == 0 && Fuse_Target())) {
		Coord = Fuse_Target();
	}

	/*
	**	Non-aircraft targets apply damage to the ground.
	*/
	if (!Is_Target_Aircraft(TarCom) || As_Aircraft(TarCom)->In_Which_Layer() == LAYER_GROUND) {
		Explosion_Damage(Coord, Strength, Payback, Warhead);
		if (!IsActive) return;

	} else {

		/*
		**	Special damage apply for SAM missiles. This is the only way that missile
		**	damage affects the aircraft target.
		*/
		if (Distance(TarCom) < 0x0080) {
			AircraftClass * object = As_Aircraft(TarCom);

			int str = Strength;
			if (object) object->Take_Damage(str, 0, Warhead, Payback);
		}
	}

	/*
	**	For projectiles that are invisible while travelling toward the target,
	**	allow scatter effect for the impact animation.
	*/
	if (Class->IsInvisible) {
		Coord = Coord_Scatter(Coord, 0x0020);
	}

	/*
	**	Fetch the land type that the explosion will be upon. Special case for
	**	flying aircraft targets, their land type will be LAND_NONE.
	*/
	CellClass const * cellptr = &Map[Coord];
	LandType land = cellptr->Land_Type();
	if (Is_Target_Aircraft(TarCom) && As_Aircraft(TarCom)->In_Which_Layer() == LAYER_TOP) {
		land = LAND_NONE;
	}

	AnimType anim = Combat_Anim(Strength, Warhead, land);

	/*
	** If it's a water explosion that's going to play, don't play it
	** if its cell is the same as the center cell of the target ship.
	*/
	if (anim >= ANIM_WATER_EXP1 && anim <= ANIM_WATER_EXP3 && Is_Target_Vessel(TarCom)) {
		if (Coord_Cell(Coord) == Coord_Cell(As_Vessel(TarCom)->Center_Coord())) {
			anim = (AnimType) (ANIM_VEH_HIT1 + (anim - ANIM_WATER_EXP1));
		}
	}

	if (anim != ANIM_NONE) {
		AnimClass * aptr = new AnimClass(anim, Coord);
		if (aptr) {
			aptr->Sort_Above(TarCom);
		}
		/*
		** Special case trap: if they're making the nuclear explosion,
		** and no anim is available, force the nuclear damage anyway
		** because nuke damage is done in the middle of the animation
		** and if there's no animation, there won't be any damage.
		*/
		if (!aptr && anim == ANIM_ATOM_BLAST) {
			GlyphX_Debug_Print("FAILED to create ANIM_ATOM_BLAST");
			HousesType house = HOUSE_NONE;
			if (Payback) {
				house = Payback->House->Class->House;
			}
			AnimClass::Do_Atom_Damage(house, Coord_Cell(Coord));
		}

		// MBL 05.20.2020
		// Fix for Nuke or Atom Bomb killing structures and units during animation sequence and not getting kills tracked
		// Per https://jaas.ea.com/browse/TDRA-6610
		//
		else if (aptr && anim == ANIM_ATOM_BLAST && aptr->OwnerHouse == HOUSE_NONE) {
			if (Payback && Payback->House && Payback->House->Class) {
				aptr->OwnerHouse = Payback->House->Class->House;
			}
		}
	}

//				if (Payback && Payback->House == PlayerPtr && stricmp(Class->Name(), "GPSSATELLITE") == 0) {
	if (Payback && Class->Type == BULLET_GPS_SATELLITE) {
		
		bool reveal = false;
		if (Session.Type != GAME_GLYPHX_MULTIPLAYER) {
			if (Payback->House == PlayerPtr) {
				reveal = true;
			}
		} else {
			if (Payback->House->IsHuman) {
				reveal = true;
			}
		}
		
		if (reveal) {
			if (!Map.Is_Radar_Active()) {
				Map.Radar_Activate(1);
			}
			for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
				Map.Map_Cell(cell, Payback->House);
			}
			Map.RadarClass::Flag_To_Redraw(true);
		}
//					Sound_Effect(VOC_SATTACT2);
		Payback->House->IsGPSActive = true;
		Payback->House->IsVisionary = true;
	}
}