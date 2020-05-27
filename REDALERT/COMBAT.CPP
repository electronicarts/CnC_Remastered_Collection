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

/* $Header: /CounterStrike/COMBAT.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : COMBAT.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 19, 1994                                           *
 *                                                                                             *
 *                  Last Update : July 26, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Combat_Anim -- Determines explosion animation to play.                                    *
 *   Explosion_Damage -- Inflict an explosion damage affect.                                   *
 *   Modify_Damage -- Adjusts damage to reflect the nature of the target.                      *
 *   Wide_Area_Damage -- Apply wide area damage to the map.                                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"


/***********************************************************************************************
 * Modify_Damage -- Adjusts damage to reflect the nature of the target.                        *
 *                                                                                             *
 *    This routine is the core of combat tactics. It implements the                            *
 *    affect various armor types have against various weapon types. By                         *
 *    careful exploitation of this table, tactical advantage can be                            *
 *    obtained.                                                                                *
 *                                                                                             *
 * INPUT:   damage   -- The damage points to process.                                          *
 *                                                                                             *
 *          warhead  -- The source of the damage points.                                       *
 *                                                                                             *
 *          armor    -- The type of armor defending against the damage.                        *
 *                                                                                             *
 *          distance -- The distance (in leptons) from the source of the damage.               *
 *                                                                                             *
 * OUTPUT:  Returns with the adjusted damage points to inflict upon the                        *
 *          target.                                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/16/1994 JLB : Created.                                                                 *
 *   04/17/1994 JLB : Always does a minimum of damage.                                         *
 *   01/01/1995 JLB : Takes into account distance from damage source.                          *
 *   04/11/1996 JLB : Changed damage fall-off formula for less damage fall-off.                *
 *=============================================================================================*/
int Modify_Damage(int damage, WarheadType warhead, ArmorType armor, int distance)
{
	if (!damage) return(damage);

	/*
	**	If there is no raw damage value to start with, then
	**	there can be no modified damage either.
	*/
	if (Special.IsInert || !damage || warhead == WARHEAD_NONE) return(0);

	/*
	**	Negative damage (i.e., heal) is always applied full strength, but only if the heal
	**	effect is close enough.
	*/
	if (damage < 0) {
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
		if (distance < 0x008) {
			if(warhead != WARHEAD_MECHANICAL && armor == ARMOR_NONE) return(damage);
			if(warhead == WARHEAD_MECHANICAL && armor != ARMOR_NONE) return(damage);
		}
#else
		if (distance < 0x008 && armor == ARMOR_NONE) return(damage);
#endif
		return(0);
	}

	WarheadTypeClass const * whead = WarheadTypeClass::As_Pointer(warhead);
//	WarheadTypeClass const * whead = &Warheads[warhead];

	damage = damage * whead->Modifier[armor];

	/*
	**	Reduce damage according to the distance from the impact point.
	*/
	if (damage) {
		if (!whead->SpreadFactor) {
			distance /= PIXEL_LEPTON_W/4;
		} else {
			distance /= whead->SpreadFactor * (PIXEL_LEPTON_W/2);
		}
		distance = Bound(distance, 0, 16);
		if (distance) {
			damage = damage / distance;
		}

		/*
		**	Allow damage to drop to zero only if the distance would have
		**	reduced damage to less than 1/4 full damage. Otherwise, ensure
		**	that at least one damage point is done.
		*/
		if (distance < 4) {
			damage = max(damage, Rule.MinDamage);
		}
	}

	damage = min(damage, Rule.MaxDamage);
	return(damage);
}


/***********************************************************************************************
 * Explosion_Damage -- Inflict an explosion damage affect.                                     *
 *                                                                                             *
 *    Processes the collateral damage affects typically caused by an                           *
 *    explosion.                                                                               *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate of ground zero.                                         *
 *                                                                                             *
 *          strength -- Raw damage points at ground zero.                                      *
 *                                                                                             *
 *          source   -- Source of the explosion (who is responsible).                          *
 *                                                                                             *
 *          warhead  -- The kind of explosion to process.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine can consume some time and will affect the AI                       *
 *             of nearby enemy units (possibly).                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/16/1991 JLB : Created.                                                                 *
 *   11/30/1991 JLB : Uses coordinate system.                                                  *
 *   12/27/1991 JLB : Radius of explosion damage effect.                                       *
 *   04/13/1994 JLB : Streamlined.                                                             *
 *   04/16/1994 JLB : Warhead damage type modifier.                                            *
 *   04/17/1994 JLB : Cleaned up.                                                              *
 *   06/20/1994 JLB : Uses object pointers to distribute damage.                               *
 *   06/20/1994 JLB : Source is a pointer.                                                     *
 *   06/18/1996 JLB : Strength could be negative for healing effects.                          *
 *=============================================================================================*/
void Explosion_Damage(COORDINATE coord, int strength, TechnoClass * source, WarheadType warhead)
{
	CELL				cell;			// Cell number under explosion.
	ObjectClass *	object;			// Working object pointer.
	ObjectClass *	objects[32];	// Maximum number of objects that can be damaged.
	int				distance;	// Distance to unit.
	int				range;		// Damage effect radius.
	int				count;		// Number of vehicle IDs in list.

	if (!strength || Special.IsInert || warhead == WARHEAD_NONE) return;

	WarheadTypeClass const * whead = WarheadTypeClass::As_Pointer(warhead);
//	WarheadTypeClass const * whead = &Warheads[warhead];
//	range = ICON_LEPTON_W*2;
	range = ICON_LEPTON_W + (ICON_LEPTON_W >> 1);
	cell = Coord_Cell(coord);
	if ((unsigned)cell >= MAP_CELL_TOTAL) return;

	CellClass * cellptr = &Map[cell];
	ObjectClass * impacto = cellptr->Cell_Occupier();

	/*
	**	Fill the list of unit IDs that will have damage
	**	assessed upon them. The units can be lifted from
	**	the cell data directly.
	*/
	count = 0;
	for (FacingType i = FACING_NONE; i < FACING_COUNT; i++) {
		/*
		**	Fetch a pointer to the cell to examine. This is either
		**	an adjacent cell or the center cell. Damage never spills
		**	further than one cell away.
		*/
		if (i != FACING_NONE) {
			cellptr = &Map[cell].Adjacent_Cell(i);
		}

		/*
		**	Add all objects in this cell to the list of objects to possibly apply
		** damage to. The list stops building when the object pointer list becomes
		** full.  Do not include overlapping objects; selection state can affect
		** the overlappers, and this causes multiplayer games to go out of sync.
		*/
		object = cellptr->Cell_Occupier();
		while (object) {
			if (!object->IsToDamage && object != source) {
				object->IsToDamage = true;
				objects[count++] = object;
				if (count >= ARRAY_SIZE(objects)) break;
			}
			object = object->Next;
		}
 		if (count >= ARRAY_SIZE(objects)) break;
	}

	/*
	**	Sweep through the units to be damaged and damage them. When damaging
	**	buildings, consider a hit on any cell the building occupies as if it
	**	were a direct hit on the building's center.
	*/
	for (int index = 0; index < count; index++) {
		object = objects[index];

		object->IsToDamage = false;
		if (object->IsActive) {
			if (object->What_Am_I() == RTTI_BUILDING && impacto == object) {
				distance = 0;
			} else {
				distance = Distance(coord, object->Center_Coord());
			}
			if (object->IsDown && !object->IsInLimbo && distance < range) {
				int damage = strength;
				object->Take_Damage(damage, distance, warhead, source);
			}
		}
	}

	/*
	**	If there is a wall present at this location, it may be destroyed. Check to
	**	make sure that the warhead is of the kind that can destroy walls.
	*/
	cellptr = &Map[cell];
	if (cellptr->Overlay != OVERLAY_NONE) {
		OverlayTypeClass const * optr = &OverlayTypeClass::As_Reference(cellptr->Overlay);

		if (optr->IsTiberium && whead->IsTiberiumDestroyer) {
			cellptr->Reduce_Tiberium(strength / 10);
		}
		if (optr->IsWall) {
			if (whead->IsWallDestroyer || (whead->IsWoodDestroyer && optr->IsWooden)) {
				Map[cell].Reduce_Wall(strength);
			}
		}
	}

	/*
	**	If there is a bridge at this location, then it may be destroyed by the
	**	combat damage.
	*/
	if (cellptr->TType == TEMPLATE_BRIDGE1 || cellptr->TType == TEMPLATE_BRIDGE2 ||
		 cellptr->TType == TEMPLATE_BRIDGE1H || cellptr->TType == TEMPLATE_BRIDGE2H ||
		 cellptr->TType == TEMPLATE_BRIDGE_1A || cellptr->TType == TEMPLATE_BRIDGE_1B ||
		 cellptr->TType == TEMPLATE_BRIDGE_2A || cellptr->TType == TEMPLATE_BRIDGE_2B ||
		 cellptr->TType == TEMPLATE_BRIDGE_3A || cellptr->TType == TEMPLATE_BRIDGE_3B ) {

		if (((warhead == WARHEAD_AP || warhead == WARHEAD_HE) && Random_Pick(1, Rule.BridgeStrength) < strength)) {
			Map.Destroy_Bridge_At(cell);
		}
	}
}


/***********************************************************************************************
 * Combat_Anim -- Determines explosion animation to play.                                      *
 *                                                                                             *
 *    This routine is called when a projectile impacts. This routine will determine what       *
 *    animation should be played.                                                              *
 *                                                                                             *
 * INPUT:   damage   -- The amount of damage this warhead possess (warhead size).              *
 *                                                                                             *
 *          warhead  -- The type of warhead.                                                   *
 *                                                                                             *
 *          land     -- The land type that this explosion is over. Sometimes, this makes       *
 *                      a difference (especially over water).                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the animation to play. If no animation is to be played, then          *
 *          ANIM_NONE is returned.                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
AnimType Combat_Anim(int damage, WarheadType warhead, LandType land)
{
	/*
	**	For cases of no damage or invalid warhead, don't have any
	**	animation effect at all.
	*/
	if (damage == 0 || warhead == WARHEAD_NONE) {
		return(ANIM_NONE);
	}

	static AnimType _aplist[] = {
		ANIM_VEH_HIT3,					// Small fragment throwing explosion -- burn/exp mix.
		ANIM_VEH_HIT2,					//	Small fragment throwing explosion -- pop & sparkles.
		ANIM_FRAG1,						// Medium fragment throwing explosion -- short decay.
		ANIM_FBALL1,					// Large fireball explosion (bulges rightward).
	};

	static AnimType _helist[] = {
		ANIM_VEH_HIT1,					//	Small fireball explosion (bulges rightward).
		ANIM_VEH_HIT2,					//	Small fragment throwing explosion -- pop & sparkles.
		ANIM_ART_EXP1,					// Large fragment throwing explosion -- many sparkles.
		ANIM_FBALL1,					// Large fireball explosion (bulges rightward).
	};

	static AnimType _firelist[] = {
		ANIM_NAPALM1,					// Small napalm burn.
		ANIM_NAPALM2,					// Medium napalm burn.
		ANIM_NAPALM3,					// Large napalm burn.
	};

	static AnimType _waterlist[] = {
		ANIM_WATER_EXP3,
		ANIM_WATER_EXP2,
		ANIM_WATER_EXP1,
	};

	WarheadTypeClass const * wptr = WarheadTypeClass::As_Pointer(warhead);
//	WarheadTypeClass const * wptr = &Warheads[warhead];
	switch (wptr->ExplosionSet) {
		case 6:
			return(ANIM_ATOM_BLAST);

		case 2:
			if (damage > 15) {
				return(ANIM_PIFFPIFF);
			}
			return(ANIM_PIFF);

		case 4:
			if (land == LAND_NONE) return(ANIM_FLAK);
//	Fixed math error
			if (land == LAND_WATER) return(_waterlist[(ARRAY_SIZE(_waterlist)-1) * fixed(min(damage, 90), 90)]);
			return(_aplist[(ARRAY_SIZE(_aplist)-1) * fixed(min(damage, 90), 90)]);

		case 5:
			if (land == LAND_NONE) return(ANIM_FLAK);
			if (land == LAND_WATER) return(_waterlist[(ARRAY_SIZE(_waterlist)-1) * fixed(min(damage, 130), 130)]);
			return(_helist[(ARRAY_SIZE(_helist)-1) * fixed(min(damage, 130), 130)]);

		case 3:
			if (land == LAND_NONE) return(ANIM_FLAK);
			if (land == LAND_WATER) return(_waterlist[(ARRAY_SIZE(_waterlist)-1) * fixed(min(damage, 150), 150)]);
			return(_firelist[(ARRAY_SIZE(_firelist)-1) * fixed(min(damage, 150), 150)]);

		case 1:
			return(ANIM_PIFF);

		default:
			break;
	}
	return(ANIM_NONE);
}


/***********************************************************************************************
 * Wide_Area_Damage -- Apply wide area damage to the map.                                      *
 *                                                                                             *
 *    This routine will apply damage to a very wide area on the map. The damage will be        *
 *    spread out from the coordinate specified by the radius specified. The amount of damage   *
 *    will attenuate according to the distance from center.                                    *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate that the explosion damage will center about.            *
 *                                                                                             *
 *          radius   -- The radius of the explosion.                                           *
 *                                                                                             *
 *          damage   -- The amount of damage to apply at the center location.                  *
 *                                                                                             *
 *          source   -- Pointer to the purpetrator of the damage (if any).                     *
 *                                                                                             *
 *          warhead  -- The type of warhead that is causing the damage.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void Wide_Area_Damage(COORDINATE coord, LEPTON radius, int rawdamage, TechnoClass * source, WarheadType warhead)
{
	int cell_radius = (radius + CELL_LEPTON_W-1) / CELL_LEPTON_W;
	CELL cell = Coord_Cell(coord);

	for (int x = -cell_radius; x <= cell_radius; x++) {
		for (int y = -cell_radius; y <= cell_radius; y++) {
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

			int dist_from_center = Distance(XY_Coord(x+cell_radius, y+cell_radius), XY_Coord(cell_radius, cell_radius));
			int damage = rawdamage * Inverse(fixed(cell_radius, dist_from_center));
			Explosion_Damage(Cell_Coord(tcell), damage, source, warhead);
			if (warhead == WARHEAD_FIRE && damage > 100) {
				new SmudgeClass(Random_Pick(SMUDGE_SCORCH1, SMUDGE_SCORCH6), Cell_Coord(tcell));
			}
		}
	}
}
