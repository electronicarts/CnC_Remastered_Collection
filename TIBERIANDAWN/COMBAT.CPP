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

/* $Header:   F:\projects\c&c\vcs\code\combat.cpv   2.17   16 Oct 1995 16:48:32   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
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
 *                  Last Update : January 1, 1995 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Explosion_Damage -- Inflict an explosion damage affect.                                   *
 *   Modify_Damage -- Adjusts damage to reflect the nature of the target.                      *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

int Modify_Damage(int damage, WarheadType warhead, ArmorType armor);
void Explosion_Damage(COORDINATE coord, unsigned strength, TechnoClass *source, WarheadType warhead);


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
 *=============================================================================================*/
int Modify_Damage(int damage, WarheadType warhead, ArmorType armor, int distance)
{
	/*
	**	If there is no raw damage value to start with, then
	**	there can be no modified damage either.
	*/
	if (Special.IsInert || !damage || warhead == WARHEAD_NONE) return(0);

	WarheadTypeClass const * whead = &Warheads[warhead];

	damage = Fixed_To_Cardinal(damage, whead->Modifier[armor]);

	/*
	**	Reduce damage according to the distance from the impact point.
	*/
	if (damage) {
//		if (distance < 0x0010) damage *= 2;			// Double damage for direct hits.
		distance >>= whead->SpreadFactor;
		distance = Bound(distance, 0, 16);
		damage >>= distance;
	}

	/*
	**	If damage was indicated, then it should never drop below one damage point regardless
	**	of modifiers. This allows a very weak attacker to eventually destroy anything it
	**	fires upon, given enough time.
	*/
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
 *=============================================================================================*/
void Explosion_Damage(COORDINATE coord, unsigned strength, TechnoClass * source, WarheadType warhead)
{
	CELL				cell;			// Cell number under explosion.
	ObjectClass *	object;			// Working object pointer.
	ObjectClass *	objects[32];	// Maximum number of objects that can be damaged.
	int				distance;	// Distance to unit.
	int				range;		// Damage effect radius.
	int				index;
	int				count;		// Number of vehicle IDs in list.

	if (!strength || Special.IsInert || warhead == WARHEAD_NONE) return;

	WarheadTypeClass const * whead = &Warheads[warhead];
	range = ICON_LEPTON_W + (ICON_LEPTON_W >> 1);
	cell = Coord_Cell(coord);
	if ((unsigned)cell >= MAP_CELL_TOTAL) return;
//	if (!Map.In_Radar(cell)) return;

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
				if (count >= (sizeof(objects)/sizeof(objects[0]))) break;
			}
			object = object->Next;
		}
 		if (count >= (sizeof(objects)/sizeof(objects[0]))) break;
	}

	/*
	**	Sweep through the units to be damaged and damage them. When damaging
	**	buildings, consider a hit on any cell the building occupies as if it
	**	were a direct hit on the building's center.
	*/
	for (index = 0; index < count; index++) {
		object = objects[index];

		object->IsToDamage = false;
		if (object->What_Am_I() == RTTI_BUILDING && impacto == object) {
			distance = 0;
		} else {
			distance = Distance(coord, object->Center_Coord());
		}
		if (object->IsDown && !object->IsInLimbo && distance < range) {
			int damage = strength;

			/*
			**	High explosive does double damage against aircraft.
			*/
			if (warhead == WARHEAD_HE && object->What_Am_I() == RTTI_AIRCRAFT) {
				damage *= 2;
			}

			/*
			**	Apply the damage to the object.
			*/
			if (damage) {
				object->Take_Damage(damage, distance, warhead, source);
			}
		}
	}

	/*
	**	If there is a wall present at this location, it may be destroyed. Check to
	**	make sure that the warhead is of the kind that can destroy walls.
	*/
	cellptr = &Map[cell];
	cellptr->Reduce_Tiberium(strength / 10);
	if (cellptr->Overlay != OVERLAY_NONE) {
		OverlayTypeClass const * optr = &OverlayTypeClass::As_Reference(cellptr->Overlay);

		if (optr->IsWall) {
			if (whead->IsWallDestroyer || (whead->IsWoodDestroyer && optr->IsWooden)) {
				Map[cell].Reduce_Wall(strength);
			}
		}
	}
}
