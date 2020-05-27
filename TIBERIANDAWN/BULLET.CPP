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

/* $Header:   F:\projects\c&c\vcs\code\bullet.cpv   2.18   16 Oct 1995 16:50:00   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : March 18, 1995 [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BulletClass::AI -- Logic processing for bullet.                                           *
 *   BulletClass::As_Target -- Converts the bullet into a target value.                        *
 *   BulletClass::BulletClass -- Bullet constructor.                                           *
 *   BulletClass::BulletClass -- Default constructor for bullet objects.                       *
 *   BulletClass::Detach -- Removes specified target from this bullet's targeting system.      *
 *   BulletClass::Draw_It -- Displays the bullet at location specified.                        *
 *   BulletClass::Init -- Clears the bullets array for scenario preparation.                   *
 *   BulletClass::Mark -- Performs related map refreshing under bullet.                        *
 *   BulletClass::Occupy_List -- Determines the bullet occupation list.                        *
 *   BulletClass::Unlimbo -- Transitions a bullet object into the game render/logic system.    *
 *   BulletClass::delete -- Bullet memory delete.                                              *
 *   BulletClass::new -- Allocates memory for bullet object.                                   *
 *   BulletClass::Validate -- validates bullet pointer													  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

#define	GRAVITY	3

/*
** This contains the value of the Virtual Function Table Pointer
*/
void * BulletClass::VTable;


/***********************************************************************************************
 * BulletClass::Validate -- validates bullet pointer														  *
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
int BulletClass::Validate(void) const
{
	int num;

	num = Bullets.ID(this);
	if (num < 0 || num >= BULLET_MAX) {
		Validate_Error("BULLET");
		return (0);
	}
	else
		return (1);
}
#else
#define	Validate()
#endif


/***********************************************************************************************
 * BulletClass::BulletClass -- Default constructor for bullet objects.                         *
 *                                                                                             *
 *    This is the default constructor for bullet objects. A bullet constructed by this routine *
 *    is not in a usable state for game purposes. It must be constructed by the full           *
 *    (parameterized) constructor -- usually called as part of the overloaded "new" operator.  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Do not use bullets that were constructed solely by this routine.                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BulletClass::BulletClass(void) :
	Class(0)
{
	Payback = NULL;
	IsToAnimate = false;
	Altitude = 0;
	Riser = 0;
	TarCom = TARGET_NONE;
	Strength = 0;
	IsLocked = true;
	IsInaccurate = false;
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
void BulletClass::operator delete(void *ptr)
{
	if (ptr) {
		((BulletClass *)ptr)->IsActive = false;
	}
	Bullets.Free((BulletClass *)ptr);
}


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
BulletClass::BulletClass(BulletType id) :
	Class(&BulletTypeClass::As_Reference(id))
{
	Altitude = 0;
	IsInaccurate = false;
	IsLocked = true;
//	IsLocked = false;
	IsToAnimate = false;
	Payback = 0;
	Riser = 0;
	Strength = Class->MaxStrength;
	TarCom = TARGET_NONE;
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
short const *BulletClass::Occupy_List(void) const
{
	Validate();
	switch (*this) {
		case BULLET_FLAME:
			return(Coord_Spillage_List(Coord, 25));

		case BULLET_NUKE_UP:
		case BULLET_NUKE_DOWN:
			return(Coord_Spillage_List(Coord, 48));

		default:
			if (Altitude) {
				static CELL _list[10];
				const short * ptr = Coord_Spillage_List(Coord, 5);
				int index = 0;
				CELL cell1 = Coord_Cell(Coord);

				while (ptr[index] != REFRESH_EOL) {
					_list[index] = ptr[index];
					index++;
				}

				COORDINATE coord = XY_Coord(0, Altitude);
				coord = Coord_Sub(Coord, coord);
				CELL cell2 = Coord_Cell(coord);
				ptr = Coord_Spillage_List(coord, 5);
				while (*ptr != REFRESH_EOL) {
					_list[index++] = (*ptr++) + (cell2 - cell1);
				}
				_list[index] = REFRESH_EOL;
				return(_list);
			}
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
	Validate();
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
	Validate();
	COORDINATE	coord;

	ObjectClass::AI();

	/*
	**	Balistic objects are handled here.
	*/
	bool forced = false;			// Forced explosion.
	if (Class->IsArcing) {
		Altitude += Riser;

		if (Altitude <= 0) {
			forced = true;
		}
		if (Riser > -100) {
			Riser -= GRAVITY;
		}
	}
	if (Class->IsDropping) {
		Altitude += Riser;

		if (Altitude <= 0) {
			forced = true;
		}
		if (Riser > -100) {
			Riser -= 1;
		}
	}

	/*
	**	Homing projectiles constantly change facing to face toward the target but
	**	they only do so every other game frame (improves game speed and makes
	**	missiles not so deadly).
	*/
	if ((Frame & 0x01) && Class->IsHoming && Target_Legal(TarCom)) {
		PrimaryFacing.Set_Desired(Direction256(Coord, ::As_Coord(TarCom)));
	}

	/*
	**	Move the projectile forward according to its speed
	**	and direction.
	*/
	coord = Coord;
	if (Class->IsFlameEquipped) {
		if (IsToAnimate) {
			new AnimClass(ANIM_SMOKE_PUFF, coord, 1);
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
//			if (IsLocked) {
				Mark();
				Delete_This();
//			}
			break;

		default:
		case IMPACT_NONE:

		/*
		**	The projectile has moved. Check its fuse. If detonation
		**	is signaled, then do so. Otherwise, just move.
		*/
		case IMPACT_NORMAL:
			Mark();
//			IsLocked = true;
			if (!Class->IsHigh) {
				CellClass * cellptr = &Map[Coord_Cell(coord)];
				if (cellptr->Overlay != OVERLAY_NONE && OverlayTypeClass::As_Reference(cellptr->Overlay).IsHigh) {
					forced = true;
					Coord = coord = Cell_Coord(Coord_Cell(coord));
				}
			}

			/*
			**	Bullets are generaly more effective when they are fired at aircraft.
			*/
			if (Class->IsAntiAircraft && As_Aircraft(TarCom) && Distance(TarCom) < 0x0080) {
				forced = true;

				if (*this == BULLET_TOW) {
					Strength += Strength/3;
				} else {
					Strength += Strength/2;
				}
			}

			if (!forced && (Class->IsDropping || !Fuse_Checkup(coord))) {
				Coord = coord;
				Mark();

				/*
				**	Certain projectiles loose strength when they travel.
				*/
				if (*this == BULLET_BULLET) {
					if (Strength > 5) Strength--;
				}

			} else {

				/*
				**	When the target is reached, explode and do the damage
				**	required of it. For homing objects, don't force the explosion to
				**	match the target position. Non-homing projectiles adjust position so
				**	that they hit the target. This compensates for the error in line of
				**	flight logic.
				*/
				Mark();
				if (!forced && !Class->IsArcing && !Class->IsHoming && Fuse_Target()) {
					Coord = Fuse_Target();
				}

				/*
				**	Non-aircraft targets apply damage to the ground.
				*/
				if (!Is_Target_Aircraft(TarCom) || As_Aircraft(TarCom)->In_Which_Layer() == LAYER_GROUND) {
					Explosion_Damage(Coord, Strength, Payback, Class->Warhead);
				} else {

					/*
					**	Special damage apply for SAM missiles. This is the only way that missile
					**	damage affects the aircraft target.
					*/
					if (Distance(TarCom) < 0x0080) {
						AircraftClass * object = As_Aircraft(TarCom);

						int str = Strength;
						if (object) object->Take_Damage(str, 0, Class->Warhead, Payback);
					}
				}

				/*
				**	For projectiles that are invisible while travelling toward the target,
				**	allow scatter effect for the impact animation.
				*/
				if (Class->IsInvisible) {
					Coord = Coord_Scatter(Coord, 0x0020);
				}
				if (Class->Explosion != ANIM_NONE) {
					AnimClass * newanim = new AnimClass(Class->Explosion, Coord);
					if (newanim) {
						newanim->Sort_Above(TarCom);
					}

					// MBL 05.20.2020
					// Fix for Nuke or Atom Bomb killing structures and units during animation sequence and not getting kills tracked
					// Per https://jaas.ea.com/browse/TDRA-6610
					//
					if (newanim && Class->Explosion == ANIM_ATOM_BLAST && newanim->Owner == HOUSE_NONE) {
						if (Payback && Payback->House && Payback->House->Class) {
							newanim->Owner = Payback->House->Class->House;
						}
					}
				}
				Delete_This();
				return;
			}
			break;
	}
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
void BulletClass::Draw_It(int x, int y, WindowNumberType window)
{
	Validate();
	int	facing = Facing_To_32(PrimaryFacing);

	/*
	**	Certain projectiles aren't visible. This includes small bullets (which are actually
	**	invisible) and flame thrower flames (which are rendered as an animation instead of a projectile).
	*/
	if (Class->IsInvisible) return;

	/*
	**	If there is no shape loaded for this object, then
	**	it obviously can't be rendered -- just bail.
	*/
	void const * shapeptr = Class->Get_Image_Data();
	if (!shapeptr) return;

	/*
	**	Get the basic shape number for this projectile.
	*/
	int shapenum = 0;
	if (!Class->IsFaceless) {
		shapenum = UnitClass::BodyShape[facing];
	}

	/*
	**	For tumbling projectiles, fetch offset stage.
	*/
	if (*this == BULLET_NAPALM) {
		shapenum += Frame % 6;
	}

	if (*this == BULLET_GRENADE) {
		shapenum += Frame % 8;
//		Timer++;
	}

	/*
	**	For flying projectiles, draw the shadow and adjust the actual projectile body
	**	render position.
	*/
	if (Altitude) {
		CC_Draw_Shape(shapeptr, shapenum, x, y, window, SHAPE_PREDATOR|SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_FADING, NULL, Map.FadingShade);
		y -= Lepton_To_Pixel(Altitude);
	}

	/*
	**	Draw the main body of the projectile.
	*/
	ShapeFlags_Type flags = SHAPE_NORMAL;
	if (Class->IsTranslucent) {
		flags = SHAPE_GHOST;
	}
	CC_Draw_Shape(this, shapeptr, shapenum, x, y, window, flags|SHAPE_CENTER|SHAPE_WIN_REL, NULL, Map.UnitShadow);
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
	BulletClass *ptr;

	Bullets.Free_All();

	ptr = new BulletClass();
	VTable = ((void **)(((char *)ptr) + sizeof(AbstractClass) - 4))[0];
	delete ptr;
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
	Validate();
	ObjectClass * obj = As_Object(target);

	if (obj == Payback) {
		Payback = 0;
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
 *    performed. In effect, it comes into existance.                                           *
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
	Validate();
	/*
	**	Try to unlimbo the bullet as far as the base class is concerned. Use the already
	**	set direction and strength if the "punt" values were passed in. This allows a bullet
	**	to be setup prior to being launched.
	*/
	if (ObjectClass::Unlimbo(coord)) {
		COORDINATE tcoord = As_Coord(TarCom);

		/*
		**	Homing projectiles (missiles) do NOT override facing. They just fire in the
		**	direction specified and let the chips fall where they may.
		*/
		if (!Class->IsHoming && !Class->IsDropping) {
			dir = Direction(tcoord);
		}

		/*
		**	Possibly adjust the target if this projectile is inaccurate. This occurs whenever
		**	certain weapons are trained upon targets they were never designed to attack. Example: when
		**	turrets or anti-tank missiles are fired at infantry. Indirect
		**	fire is inherently inaccurate.
		*/
		if (IsInaccurate || Class->IsInaccurate ||
			((Is_Target_Cell(TarCom) || Is_Target_Infantry(TarCom)) && (Class->Warhead == WARHEAD_AP || Class->IsFueled))) {

			/*
			**	Inaccuracy for low velocity or homing projectiles manifests itself as a standard
			**	Cicular Error of Probability (CEP) algorithm. High speed projectiles usually
			**	just overshoot the target by extending the straight line flight.
			*/
			if (Class->IsHoming || Class->IsArcing) {
				int scatterdist = ::Distance(coord, tcoord)/3;
				scatterdist = MIN(scatterdist, 0x0200);

				if (*this == BULLET_GRENADE) {
					scatterdist = ::Distance(coord, tcoord)/4;
					scatterdist = MIN(scatterdist, 0x0080);
				}

				dir = (DirType)((dir + (Random_Pick(0, 10)-5)) & 0x00FF);
				tcoord = Coord_Scatter(tcoord, Random_Pick(0, scatterdist));
			} else {
				tcoord = Coord_Move(tcoord, dir, Random_Pick(0, 0x0100));
			}

			/*
			**	Limit scatter to the weapon range of the firer.
			*/
			if (Payback) {
				if (!Payback->In_Range(tcoord, 0) && !Payback->In_Range(tcoord, 1)) {
					tcoord = Coord_Move(tcoord, ::Direction(tcoord, Coord), Distance(tcoord) - MAX(Payback->Weapon_Range(0), Payback->Weapon_Range(1)));
				}
			}
		}

		/*
		**	For very fast and invisible projectiles, just make the projectile exist at the target
		**	location and dispense with the actual flight.
		*/
		if (Class->MaxSpeed == MPH_LIGHT_SPEED && Class->IsInvisible) {
			Coord = tcoord;
		}

		/*
		**	Set the range equal to either the class defined range or the calculated
		**	number of game frames it would take for the projectile to reach the target.
		*/
		int range = 0xFF;
		if (!Class->Range) {
			if (!Class->IsDropping) {
				range = (::Distance(tcoord, Coord) / Class->MaxSpeed) + 4;
			}
		} else {
			range = Class->Range;
		}

		/*
		**	Projectile speed is usually the default value for that projectile, but
		**	certian projectiles alter speed according to the distance to the
		**	target.
		*/
		int speed = Class->MaxSpeed;
		if (speed == MPH_LIGHT_SPEED) speed = MPH_IMMOBILE;
		if (Class->IsArcing) {
			speed = Class->MaxSpeed + (Distance(tcoord)>>5);

			/*
			**	Set minimum speed (i.e., distance) for arcing projectiles.
			*/
			speed = MAX(speed, 25);
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
		**	the vertical and horizontal components. This solution would require a
		**	of square root and an arcsine lookup table. OUCH!
		*/
		Altitude = 0;
		Riser = 0;
		if (Class->IsArcing) {
			Altitude = 1;
			Riser = ((Distance(tcoord)/2) / (speed+1))*GRAVITY;
			Riser = MAX(Riser, (signed char)10);
		}
		if (Class->IsDropping) {
			Altitude = Pixel_To_Lepton(24);
			Riser = 0;
		}

		PrimaryFacing = dir;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * BulletClass::As_Target -- Converts the bullet into a target value.                          *
 *                                                                                             *
 *    This support routine is used to convert the bullet (as a pointer) into a target          *
 *    value (which is a number).                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the bullet as a target value.                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/08/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TARGET BulletClass::As_Target(void) const
{
	Validate();
	return(Build_Target(KIND_BULLET, Bullets.ID(this)));
}