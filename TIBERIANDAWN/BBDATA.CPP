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

/* $Header:   F:\projects\c&c\vcs\code\bbdata.cpv   2.17   16 Oct 1995 16:49:46   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BBDATA.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 23, 1994                                                 *
 *                                                                                             *
 *                  Last Update : October 17, 1994   [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BulletTypeClass::BulletTypeClass -- Constructor for bullet type objects.                  *
 *   BulletTypeClass::Load_Shapes -- Load shape data for bullet types.                         *
 *   BulletTypeClass::One_Time -- Performs the one time processing for bullets.                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

/***************************************************************************
**	Detailed information about each class of bullet (projectile) in the game.
*/
static BulletTypeClass const ClassSniper(
	BULLET_BULLET,
	"50cal",						// NAME:			Text name of this unit type.
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_LIGHT_SPEED,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HOLLOW_POINT,	// WARHEAD:		If fires weapon, warhead type
	ANIM_PIFF					// Explosion to use upon impact.
);

static BulletTypeClass const ClassBullet(
	BULLET_BULLET,
	"50cal",						// NAME:			Text name of this unit type.
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_LIGHT_SPEED,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_SA,					// WARHEAD:		If fires weapon, warhead type
	ANIM_PIFF					// Explosion to use upon impact.
);

static BulletTypeClass const ClassSpreadfire(
	BULLET_SPREADFIRE,
	"50cal",						// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_LIGHT_SPEED,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_PIFFPIFF				// Explosion to use upon impact.
);

static BulletTypeClass const ClassAPDS(
	BULLET_APDS,
	"120mm",						// NAME:			Text name of this unit type.
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_VERY_FAST,				// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_AP,					// WARHEAD:		If fires weapon, warhead type
	ANIM_VEH_HIT3				// Explosion to use upon impact.
);

static BulletTypeClass const Class120mm(
	BULLET_HE,
	"120mm",						// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		true,						// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		true,						// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_MEDIUM_FAST,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_ART_EXP1				// Explosion to use upon impact.
);

static BulletTypeClass const ClassMissile(
	BULLET_SSM,
	"DRAGON",					// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		true,						// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		true,						// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		true,						// Is projectile inherently inaccurate?
		true,						// Translucent colors are used?
		true,						// Good against aircraft?
	7,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_ROCKET,					// SPEED:		Miles per hour.
	5,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_FRAG1					// Explosion to use upon impact.
);

static BulletTypeClass const ClassMissile2(
	BULLET_SSM2,
	"DRAGON",					// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		true,						// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		true,						// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		true,						// Is projectile inherently inaccurate?
		true,						// Translucent colors are used?
		true,						// Good against aircraft?
	9,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_ROCKET,					// SPEED:		Miles per hour.
	7,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_FRAG1					// Explosion to use upon impact.
);

static BulletTypeClass const ClassPatriot(
	BULLET_SAM,
	"MISSILE",					// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		true,						// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		true,						// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		true,						// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_VERY_FAST,				// SPEED:		Miles per hour.
	10,							// ROT:			Rate of turn (degrees per tick).
	WARHEAD_AP,					// WARHEAD:		If fires weapon, warhead type
	ANIM_VEH_HIT1				// Explosion to use upon impact.
);

static BulletTypeClass const ClassDragon(
	BULLET_TOW,
	"DRAGON",					// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		true,						// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		true,						// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		true,						// Translucent colors are used?
		true,						// Good against aircraft?
	3,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_ROCKET,					// SPEED:		Miles per hour.
	5,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_AP,					// WARHEAD:		If fires weapon, warhead type
	ANIM_VEH_HIT2				// Explosion to use upon impact.
);

static BulletTypeClass const ClassFlame(
	BULLET_FLAME,
	"FLAME",						// NAME:			Text name of this unit type.
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	12,							// ARMING:		Time to arm projectile after launch.
	12,							// RANGE:		Inherent override range factor.
	MPH_FAST,					// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_FIRE, 				// WARHEAD:		If fires weapon, warhead type
	ANIM_NONE					// Explosion to use upon impact.
);

static BulletTypeClass const ClassChem(
	BULLET_CHEMSPRAY,
	"FLAME",						// NAME:			Text name of this unit type.
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	12,							// ARMING:		Time to arm projectile after launch.
	12,							// RANGE:		Inherent override range factor.
	MPH_FAST,					// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE, 				// WARHEAD:		If fires weapon, warhead type
	ANIM_NONE					// Explosion to use upon impact.
);

static BulletTypeClass const ClassNapalm(
	BULLET_NAPALM,
	"BOMBLET",					// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		true,						// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		true,						// Translucent colors are used?
		false,					// Good against aircraft?
	24,							// ARMING:		Time to arm projectile after launch.
	24,							// RANGE:		Inherent override range factor.
	MPH_MEDIUM_SLOW,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_FIRE,				// WARHEAD:		If fires weapon, warhead type
	ANIM_NAPALM2				// Explosion to use upon impact.
);

static BulletTypeClass const ClassGrenade(
	BULLET_GRENADE,
	"BOMB",						// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		true,						// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		true,						// Is projectile inherently inaccurate?
		true,						// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_MEDIUM_SLOW,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_VEH_HIT2				// Explosion to use upon impact.
);

static BulletTypeClass const ClassLaser(
	BULLET_LASER,
	"Laser",						// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_LIGHT_SPEED,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_LASER,				// WARHEAD:		If fires weapon, warhead type
	ANIM_NONE					// Explosion to use upon impact.
);

static BulletTypeClass const ClassNukeUp(
	BULLET_NUKE_UP,
	"ATOMICUP",					// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_VERY_FAST,				// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_FRAG1					// Explosion to use upon impact.
);

static BulletTypeClass const ClassNukeDown(
	BULLET_NUKE_DOWN,
	"ATOMICDN",					// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_VERY_FAST,				// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_ATOM_BLAST			// Explosion to use upon impact.
);

static BulletTypeClass const ClassHonestJohn(
	BULLET_HONEST_JOHN,
	"MISSILE",					// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		true,						// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	10,							// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_FAST,					// SPEED:		Miles per hour.
	10,							// ROT:			Rate of turn (degrees per tick).
	WARHEAD_FIRE,				// WARHEAD:		If fires weapon, warhead type
	ANIM_NAPALM3				// Explosion to use upon impact.
);

static BulletTypeClass const ClassHeadButt(
	BULLET_HEADBUTT,
	"GORE",						// NAME:			Text name of this unit type.
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_LIGHT_SPEED,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HEADBUTT,			// WARHEAD:		If fires weapon, warhead type
	ANIM_NONE					// Explosion to use upon impact.
);

static BulletTypeClass const ClassTRexBite(
	BULLET_TREXBITE,
	"CHEW",						// NAME:			Text name of this unit type.
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_LIGHT_SPEED,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_FEEDME,			// WARHEAD:		If fires weapon, warhead type
	ANIM_NONE					// Explosion to use upon impact.
);


#ifdef PETROGLYPH_EXAMPLE_MOD

static BulletTypeClass const NukeLob(
	BULLET_NUKE_LOB,
	"BOMB",						// NAME:			Text name of this unit type.
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		true,						// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		true,						// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_MEDIUM_FAST,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_ATOM_BLAST			// Explosion to use upon impact.
);

#endif //PETROGLYPH_EXAMPLE_MOD

			
/*
**	This is the array of pointers to the static data associated with
**	each bullet (projectile) type.
*/
BulletTypeClass const * const BulletTypeClass::Pointers[BULLET_COUNT] = {
	&ClassSniper,		//	BULLET_SNIPER
	&ClassBullet,		//	BULLET_BULLET
	&ClassAPDS,			//	BULLET_APDS
	&Class120mm,		//	BULLET_HE
	&ClassMissile,		//	BULLET_SSM
	&ClassMissile2,	//	BULLET_SSM2
	&ClassPatriot,		//	BULLET_SAM
	&ClassDragon,		//	BULLET_TOW
	&ClassFlame,		// BULLET_FLAME
	&ClassChem,			// BULLET_CHEMSPRAY
	&ClassNapalm,		// BULLET_NAPALM
	&ClassGrenade,		// BULLET_GRENADE
	&ClassLaser,		// BULLET_LASER
	&ClassNukeUp,		// BULLET_NUKE_UP
	&ClassNukeDown,	// BULLET_NUKE_DOWN
	&ClassHonestJohn,	// BULLET_HONEST_JOHN
	&ClassSpreadfire,	// BULLET_SPREADFIRE
	&ClassHeadButt,	// BULLET_HEADBUTT
	&ClassTRexBite,	// BULLET_TREXBITE
#ifdef PETROGLYPH_EXAMPLE_MOD
	&NukeLob,			// BULLET_NUKE_LOB
#endif //PETROGLYPH_EXAMPLE_MOD

};


/***********************************************************************************************
 * BulletTypeClass::BulletTypeClass -- Constructor for bullet type objects.                    *
 *                                                                                             *
 *    This is basically a constructor for static type objects used by bullets. All bullets     *
 *    are of a type constructed by this routine at game initialization time.                   *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BulletTypeClass::BulletTypeClass(
	BulletType type, 
	char const *ininame, 
	bool is_high,
	bool is_homing, 
	bool is_arcing, 
	bool is_dropping,
	bool is_invisible, 
	bool is_proximity_armed, 
	bool is_flame_equipped,
	bool is_fueled, 
	bool is_faceless, 
	bool is_inaccurate, 
	bool is_translucent, 
	bool is_antiaircraft,
	int arming, int range, MPHType maxspeed, unsigned rot,
	WarheadType warhead, AnimType explosion) : 
		ObjectTypeClass(true, false, false, true, false, false, true, true, TXT_NONE, ininame, ARMOR_NONE, 0) 
{
	Explosion = explosion;
	IsHigh = is_high;
	IsAntiAircraft = is_antiaircraft;
	IsTranslucent = is_translucent;
	IsArcing = is_arcing;
	IsHoming = is_homing;
	IsDropping = is_dropping;
	IsInvisible = is_invisible;
	IsProximityArmed = is_proximity_armed;
	IsFlameEquipped = is_flame_equipped;
	IsFueled = is_fueled;
	IsFaceless = is_faceless;
	IsInaccurate = is_inaccurate;
	Type = type;
	Warhead = warhead;
	MaxSpeed = maxspeed;
	ROT = rot;
	Arming = arming;
	Range = range;
}


/***********************************************************************************************
 * BulletTypeClass::One_Time -- Performs the one time processing for bullets.                  *
 *                                                                                             *
 *    This routine is used to perform any one time processing for the bullet type class. It    *
 *    handles loading of the shape files.                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine must be called before any rendering of bullets occurs and should   *
 *             only be called once.                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BulletTypeClass::One_Time(void)
{
	BulletType index;

	/*
	**	Load the bullet shapes.
	*/
	for (index = BULLET_FIRST; index < BULLET_COUNT; index++) {
		BulletTypeClass const & bullet = As_Reference(index);
		char	fullname[_MAX_FNAME+_MAX_EXT];

		if (!bullet.IsInvisible) {
			_makepath(fullname, NULL, NULL, bullet.IniName, ".SHP");

			RawFileClass file(fullname);

			if (file.Is_Available()) {
				((void const *&)bullet.ImageData) = Load_Alloc_Data(file);
			} else {
				((void const *&)bullet.ImageData) =	MixFileClass::Retrieve(fullname);
			}
		}
	}
}

