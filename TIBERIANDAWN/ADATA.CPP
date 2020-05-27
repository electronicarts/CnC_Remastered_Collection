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

/* $Header:   F:\projects\c&c\vcs\code\adata.cpv   2.18   16 Oct 1995 16:49:32   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : ADATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 30, 1994                                                 *
 *                                                                                             *
 *                  Last Update : August 23, 1994   [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   AnimTypeClass::One_Time -- Performs one time action for animation types.                  *
 *   AnimTypeClass::AnimTypeClass -- Constructor for animation types.                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

// Dinosaur death animations
static AnimTypeClass const TricDie(
	ANIM_TRIC_DIE,							// Animation number.
	"TRIC",									// Data name of animation.
	32,										// Maximum dimension of animation.
	4,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	true,										// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	176,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	20,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const TRexDie(
	ANIM_TREX_DIE,							// Animation number.
	"TREX",									// Data name of animation.
	48,										// Maximum dimension of animation.
	4,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	true,										// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	144,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	40,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const StegDie(
	ANIM_STEG_DIE,							// Animation number.
	"STEG",									// Data name of animation.
	33,										// Maximum dimension of animation.
	4,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	true,										// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	176,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	22,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const RaptDie(
	ANIM_RAPT_DIE,							// Animation number.
	"RAPT",									// Data name of animation.
	24,										// Maximum dimension of animation.
	4,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	true,										// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	144,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	40,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const SAMN(
	ANIM_SAM_N,								// Animation number.
	"SAMFIRE",								// Data name of animation.
	55,										// Maximum dimension of animation.
	4,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	18*0,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	18,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const SAMNW(
	ANIM_SAM_NW,							// Animation number.
	"SAMFIRE",								// Data name of animation.
	55,										// Maximum dimension of animation.
	22,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	18*1,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	18,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const SAMW(
	ANIM_SAM_W,								// Animation number.
	"SAMFIRE",								// Data name of animation.
	55,										// Maximum dimension of animation.
	40,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	18*2,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	18,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const SAMSW(
	ANIM_SAM_SW,							// Animation number.
	"SAMFIRE",								// Data name of animation.
	55,										// Maximum dimension of animation.
	58,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	18*3,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	18,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const SAMS(
	ANIM_SAM_S,								// Animation number.
	"SAMFIRE",								// Data name of animation.
	55,										// Maximum dimension of animation.
	76,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	18*4,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	18,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const SAMSE(
	ANIM_SAM_SE,							// Animation number.
	"SAMFIRE",								// Data name of animation.
	55,										// Maximum dimension of animation.
	94,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	18*5,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	18,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const SAME(
	ANIM_SAM_E,								// Animation number.
	"SAMFIRE",								// Data name of animation.
	55,										// Maximum dimension of animation.
	112,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	18*6,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	18,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const SAMNE(
	ANIM_SAM_NE,							// Animation number.
	"SAMFIRE",								// Data name of animation.
	55,										// Maximum dimension of animation.
	130,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	18*7,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	18,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const LZSmoke(
	ANIM_LZ_SMOKE,						// Animation number.
	"SMOKLAND",								// Data name of animation.
	32,										// Maximum dimension of animation.
	72,										// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	72,										// Loop start frame number.
	91,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	255,										// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

/*
**	Flammable object burning animations. Primarily used on trees and buildings.
*/
static AnimTypeClass const BurnSmall(
	ANIM_BURN_SMALL,						// Animation number.
	"BURN-S",								// Data name of animation.
	11,										// Maximum dimension of animation.
	13,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0008,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	30,										// Loop start frame number.
	62,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	4,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const BurnMed(
	ANIM_BURN_MED,							// Animation number.
	"BURN-M",								// Data name of animation.
	14,										// Maximum dimension of animation.
	13,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0010,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	30,										// Loop start frame number.
	62,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	4,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const BurnBig(
	ANIM_BURN_BIG,							// Animation number.
	"BURN-L",								// Data name of animation.
	23,										// Maximum dimension of animation.
	13,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	true,										// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0018,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	30,										// Loop start frame number.
	62,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	4,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

/*
**	Flammable object burning animations that trail into smoke. Used for
**	buildings and the gunboat.
*/
static AnimTypeClass const OnFireSmall(
	ANIM_ON_FIRE_SMALL,					// Animation number.
	"BURN-S",								// Data name of animation.
	11,										// Maximum dimension of animation.
	13,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0008,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	30,										// Loop start frame number.
	62,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	4,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_SMOKE_M
);
static AnimTypeClass const OnFireMed(
	ANIM_ON_FIRE_MED,						// Animation number.
	"BURN-M",								// Data name of animation.
	14,										// Maximum dimension of animation.
	13,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0010,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	30,										// Loop start frame number.
	62,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	4,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_ON_FIRE_SMALL
);
static AnimTypeClass const OnFireBig(
	ANIM_ON_FIRE_BIG,						// Animation number.
	"BURN-L",								// Data name of animation.
	23,										// Maximum dimension of animation.
	13,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	true,										// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0018,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	30,										// Loop start frame number.
	62,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	4,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_ON_FIRE_MED
);

/*
**	Flame thrower animations. These are direction specific.
*/
static AnimTypeClass const FlameN(
	ANIM_FLAME_N,							// Animation number.
	"FLAME-N",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	true,										// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const FlameNW(
	ANIM_FLAME_NW,							// Animation number.
	"FLAME-NW",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	true,										// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const FlameW(
	ANIM_FLAME_W,							// Animation number.
	"FLAME-W",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	true,										// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const FlameSW(
	ANIM_FLAME_SW,							// Animation number.
	"FLAME-SW",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	true,										// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const FlameS(
	ANIM_FLAME_S,							// Animation number.
	"FLAME-S",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	true,										// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const FlameSE(
	ANIM_FLAME_SE,							// Animation number.
	"FLAME-SE",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	true,										// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const FlameE(
	ANIM_FLAME_E,							// Animation number.
	"FLAME-E",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	true,										// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const FlameNE(
	ANIM_FLAME_NE,							// Animation number.
	"FLAME-NE",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	true,										// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

/*
**	Chem sprayer animations. These are direction specific.
*/
static AnimTypeClass const ChemN(
	ANIM_CHEM_N,							// Animation number.
	"CHEM-N",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const ChemNW(
	ANIM_CHEM_NW,							// Animation number.
	"CHEM-NW",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const ChemW(
	ANIM_CHEM_W,							// Animation number.
	"CHEM-W",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const ChemSW(
	ANIM_CHEM_SW,							// Animation number.
	"CHEM-SW",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const ChemS(
	ANIM_CHEM_S,							// Animation number.
	"CHEM-S",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const ChemSE(
	ANIM_CHEM_SE,							// Animation number.
	"CHEM-SE",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const ChemE(
	ANIM_CHEM_E,							// Animation number.
	"CHEM-E",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const ChemNE(
	ANIM_CHEM_NE,							// Animation number.
	"CHEM-NE",								// Data name of animation.
	0,											// Maximum dimension of animation.
	9,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const Grenade(
	ANIM_GRENADE,							// Animation number.
	"VEH-HIT2",								// Data name of animation.
	21,										// Maximum dimension of animation.
	1,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	true,										// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_GUN20,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const FBall1(
	ANIM_FBALL1,							// Animation number.
	"FBALL1",								// Data name of animation.
	67,										// Maximum dimension of animation.
	6,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	true,										// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_XPLOS,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const Frag1(
	ANIM_FRAG1,								// Animation number.
	"FRAG1",									// Data name of animation.
	45,										// Maximum dimension of animation.
	3,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	true,										// Forms a crater?
	true,										// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_XPLOBIG4,							// Sound effect to play.
	ANIM_NONE,
	29										// Virtual stages
);

static AnimTypeClass const Frag3(
	ANIM_FRAG2,								// Animation number.
	"FRAG3",									// Data name of animation.
	41,										// Maximum dimension of animation.
	3,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	true,										// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_XPLOBIG6,							// Sound effect to play.
	ANIM_NONE,
	29										// Virtual stages
);

static AnimTypeClass const VehHit1(
	ANIM_VEH_HIT1,							// Animation number.
	"VEH-HIT1",								// Data name of animation.
	30,										// Maximum dimension of animation.
	4,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	true,										// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_XPLOS,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const VehHit2(
	ANIM_VEH_HIT2,							// Animation number.
	"VEH-HIT2",								// Data name of animation.
	21,										// Maximum dimension of animation.
	1,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	true,										// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_XPLOS,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const VehHit3(
	ANIM_VEH_HIT3,							// Animation number.
	"VEH-HIT3",								// Data name of animation.
	19,										// Maximum dimension of animation.
	3,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	true,										// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_XPLOS,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const ArtExp1(
	ANIM_ART_EXP1,							// Animation number.
	"ART-EXP1",								// Data name of animation.
	41,										// Maximum dimension of animation.
	1,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	true,										// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_XPLOSML2,							// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const Napalm1(
	ANIM_NAPALM1,							// Animation number.
	"NAPALM1",								// Data name of animation.
	21,										// Maximum dimension of animation.
	5,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	true,										// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_FLAMER1,							// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const Napalm2(
	ANIM_NAPALM2,							// Animation number.
	"NAPALM2",								// Data name of animation.
	41,										// Maximum dimension of animation.
	5,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	true,										// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_FLAMER1,							// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const Napalm3(
	ANIM_NAPALM3,							// Animation number.
	"NAPALM3",								// Data name of animation.
	78,										// Maximum dimension of animation.
	5,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	true,										// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_FLAMER1,							// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const SmokePuff(
	ANIM_SMOKE_PUFF,						// Animation number.
	"SMOKEY",								// Data name of animation.
	24,										// Maximum dimension of animation.
	2,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	true,										// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const Piff(
	ANIM_PIFF,								// Animation number.
	"PIFF",									// Data name of animation.
	13,										// Maximum dimension of animation.
	1,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const PiffPiff(
	ANIM_PIFFPIFF,							// Animation number.
	"PIFFPIFF",								// Data name of animation.
	20,										// Maximum dimension of animation.
	2,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const Fire3(
	ANIM_FIRE_SMALL,						// Animation number.
	"FIRE3",									// Data name of animation.
	23,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0008,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	2,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE,
	-1,										// Virtual stages
	0x100,									// Virtual scale
	ANIM_FIRE_SMALL_VIRTUAL					// Virtual anim
);

static AnimTypeClass const Fire3Virtual(
	ANIM_FIRE_SMALL_VIRTUAL,				// Animation number.
	"FIRE3",									// Data name of animation.
	23,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	10,											// Loop start frame number.
	21,										// Ending frame of loop back.
	29,										// Number of animation stages.
	2,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const Fire1(
	ANIM_FIRE_MED2,		 				// Animation number.
	"FIRE1",									// Data name of animation.
	23,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	true,										// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0010,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	3,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE,
	-1,										// Virtual stages
	0x100,									// Virtual scale
	ANIM_FIRE_MED2_VIRTUAL					// Virtual anim
);

static AnimTypeClass const Fire1Virtual(
	ANIM_FIRE_MED2_VIRTUAL,				// Animation number.
	"FIRE1",									// Data name of animation.
	23,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	true,										// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	10,											// Loop start frame number.
	21,										// Ending frame of loop back.
	29,										// Number of animation stages.
	3,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const Fire4(
	ANIM_FIRE_TINY,		 				// Animation number.
	"FIRE4",									// Data name of animation.
	7,											// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0008,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	3,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE,
	-1,										// Virtual stages
	0x100,									// Virtual scale
	ANIM_FIRE_TINY_VIRTUAL					// Virtual anim
);

static AnimTypeClass const Fire4Virtual(
	ANIM_FIRE_TINY_VIRTUAL,		 			// Animation number.
	"FIRE4",									// Data name of animation.
	7,											// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	10,											// Loop start frame number.
	21,										// Ending frame of loop back.
	29,										// Number of animation stages.
	3,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE,
	30										// Virtual stages
);

static AnimTypeClass const Fire2(
	ANIM_FIRE_MED,							// Animation number.
	"FIRE2",									// Data name of animation.
	23,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	true,										// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0010,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	3,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE,
	-1,										// Virtual stages
	0x100,									// Virtual scale
	ANIM_FIRE_MED_VIRTUAL					// Virtual anim
);

static AnimTypeClass const Fire2Virtual(
	ANIM_FIRE_MED_VIRTUAL,							// Animation number.
	"FIRE2",									// Data name of animation.
	23,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	true,										// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	10,											// Loop start frame number.
	21,										// Ending frame of loop back.
	29,										// Number of animation stages.
	3,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const OilFieldBurn(
	ANIM_OILFIELD_BURN,					// Animation number.
	"FLMSPT",								// Data name of animation.
	42,										// Maximum dimension of animation.
	58,										// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	33,										// Loop start frame number.
	99,										// Ending frame of loop back.
	66,										// Number of animation stages.
	65535,									// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const Gunfire(
	ANIM_MUZZLE_FLASH,					// Animation number.
	"GUNFIRE",								// Data name of animation.
	16,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	true,										// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Number of times the animation loops.
//	2,											// Number of times the animation loops.
	1,											// Number of animation stages.
//	2,											// Number of animation stages.
	1,											// Ending frame of loop back.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE,
	10										// Virtual stages
);

#ifdef NEVER
static AnimTypeClass const E1RotFire(
	ANIM_E1_ROT_FIRE,						// Animation number.
	"E1ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	28,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const E1RotGrenade(
	ANIM_E1_ROT_GRENADE,					// Animation number.
	"E1ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	24,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const E1RotGun(
	ANIM_E1_ROT_GUN,						// Animation number.
	"E1ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	16,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const E1RotExp(
	ANIM_E1_ROT_EXP,						// Animation number.
	"E1ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	20,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const E2RotFire(
	ANIM_E2_ROT_FIRE,						// Animation number.
	"E2ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	28,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const E2RotGrenade(
	ANIM_E2_ROT_GRENADE,					// Animation number.
	"E2ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	24,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const E2RotGun(
	ANIM_E2_ROT_GUN,						// Animation number.
	"E2ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	16,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const E2RotExp(
	ANIM_E2_ROT_EXP,						// Animation number.
	"E2ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	20,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const E3RotFire(
	ANIM_E3_ROT_FIRE,						// Animation number.
	"E3ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	28,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const E3RotGrenade(
	ANIM_E3_ROT_GRENADE,					// Animation number.
	"E3ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	24,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const E3RotGun(
	ANIM_E3_ROT_GUN,						// Animation number.
	"E3ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	16,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const E3RotExp(
	ANIM_E3_ROT_EXP,						// Animation number.
	"E3ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	20,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const E4RotFire(
	ANIM_E4_ROT_FIRE,						// Animation number.
	"E4ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	28,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const E4RotGrenade(
	ANIM_E4_ROT_GRENADE,					// Animation number.
	"E4ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	24,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const E4RotGun(
	ANIM_E4_ROT_GUN,						// Animation number.
	"E4ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	16,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const E4RotExp(
	ANIM_E4_ROT_EXP,						// Animation number.
	"E4ROT",									// Data name of animation.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Is a flat on the ground animation?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	30,										// Delay between frames.
	20,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Loopback frame number.
	4,											// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
#endif

static AnimTypeClass const SmokeM(
	ANIM_SMOKE_M,							// Animation number.
	"SMOKE_M",								// Data name of animation.
	28,										// Maximum dimension of animation.
	30,										// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	67,										// Loop start frame number.
	-1,										// Loopback frame number.
	-1,										// Number of animation stages.
	6,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE,
	105										// Virtual stages
);

/*
**	Mini-gun fire effect -- used by guard towers.
*/
static AnimTypeClass const GUNN(
	ANIM_GUN_N,								// Animation number.
	"MINIGUN",								// Data name of animation.
	18,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Number of times the animation loops.
	6,											// Number of animation stages.
	0,											// Ending frame of loop back.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const GUNNW(
	ANIM_GUN_NW,							// Animation number.
	"MINIGUN",								// Data name of animation.
	18,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	6,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Number of times the animation loops.
	6,											// Number of animation stages.
	0,											// Ending frame of loop back.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const GUNW(
	ANIM_GUN_W,								// Animation number.
	"MINIGUN",								// Data name of animation.
	18,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	12,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Number of times the animation loops.
	6,											// Number of animation stages.
	0,											// Ending frame of loop back.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const GUNSW(
	ANIM_GUN_SW,							// Animation number.
	"MINIGUN",								// Data name of animation.
	18,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	18,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Number of times the animation loops.
	6,											// Number of animation stages.
	0,											// Ending frame of loop back.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const GUNS(
	ANIM_GUN_S,								// Animation number.
	"MINIGUN",								// Data name of animation.
	18,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	24,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Number of times the animation loops.
	6,											// Number of animation stages.
	0,											// Ending frame of loop back.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const GUNSE(
	ANIM_GUN_SE,							// Animation number.
	"MINIGUN",								// Data name of animation.
	18,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	30,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Number of times the animation loops.
	6,											// Number of animation stages.
	0,											// Ending frame of loop back.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const GUNE(
	ANIM_GUN_E,								// Animation number.
	"MINIGUN",								// Data name of animation.
	18,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	36,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Number of times the animation loops.
	6,											// Number of animation stages.
	0,											// Ending frame of loop back.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const GUNNE(
	ANIM_GUN_NE,							// Animation number.
	"MINIGUN",								// Data name of animation.
	18,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	42,										// Starting frame number.
	0,											// Loop start frame number.
	0,											// Number of times the animation loops.
	6,											// Number of animation stages.
	0,											// Ending frame of loop back.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);
static AnimTypeClass const IonCannon(
	ANIM_ION_CANNON,						// Animation number.
	"IONSFX",								// Data name of animation.
	48,										// Maximum dimension of animation.
	11,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	true,										// Scorches the ground?
	true,										// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	15,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_ION_CANNON,						// Sound effect to play.
	ANIM_ART_EXP1,
	32,										// Virtual stages
	0x200									// Virtual scale
);

static AnimTypeClass const AtomBomb(
	ANIM_ATOM_BLAST,						// Animation number.
	"ATOMSFX",								// Data name of animation.
	72,										// Maximum dimension of animation.
	19,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	true,										// Scorches the ground?
	true,										// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	-1,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NUKE_EXPLODE,						// Sound effect to play.
	ANIM_NONE,
	75,										// Virtual stages
	0x300									// Virtual scale
);
static AnimTypeClass const AtomDoor(
	ANIM_ATOM_DOOR,						// Animation number.
	"ATOMDOOR",								// Data name of animation.
	48,										// Maximum dimension of animation.
	19,										// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	true,										// Scorches the ground?
	true,										// Forms a crater?
	true,										// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	-1,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE
);

static AnimTypeClass const CDeviator(
	ANIM_CRATE_DEVIATOR,					// Animation number.
	"DEVIATOR",								// Data name of animation.
	48,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	-1,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE								// Follow up animation.
);
static AnimTypeClass const CDollar(
	ANIM_CRATE_DOLLAR,					// Animation number.
	"DOLLAR",								// Data name of animation.
	48,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	-1,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE								// Follow up animation.
);
static AnimTypeClass const CEarth(
	ANIM_CRATE_EARTH,						// Animation number.
	"EARTH",									// Data name of animation.
	48,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	-1,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE								// Follow up animation.
);
static AnimTypeClass const CEmpulse(
	ANIM_CRATE_EMPULSE,					// Animation number.
	"EMPULSE",								// Data name of animation.
	48,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	-1,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE								// Follow up animation.
);
static AnimTypeClass const CInvun(
	ANIM_CRATE_INVUN,						// Animation number.
	"INVUN",									// Data name of animation.
	48,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	-1,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE								// Follow up animation.
);
static AnimTypeClass const CMine(
	ANIM_CRATE_MINE,						// Animation number.
	"MINE",									// Data name of animation.
	48,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	-1,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE								// Follow up animation.
);
static AnimTypeClass const CRapid(
	ANIM_CRATE_RAPID,						// Animation number.
	"RAPID",									// Data name of animation.
	48,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	-1,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE								// Follow up animation.
);
static AnimTypeClass const CStealth(
	ANIM_CRATE_STEALTH,					// Animation number.
	"STEALTH2",								// Data name of animation.
	48,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	-1,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE								// Follow up animation.
);
static AnimTypeClass const CMissile(
	ANIM_CRATE_MISSILE,					// Animation number.
	"MISSILE2",								// Data name of animation.
	48,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	true,										// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	2,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	-1,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE								// Follow up animation.
);

static AnimTypeClass const MoveFlash(
	ANIM_MOVE_FLASH,							// Animation number.
	"MOVEFLSH",								// Data name of animation.
	24,										// Maximum dimension of animation.
	0,											// Biggest animation stage.
	true,										// Normalized animation rate?
	true,										// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	true,										// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	0,											// Ending frame of loop back.
	-1,										// Number of animation stages.
	0,											// Number of times the animation loops.
	VOC_NONE,								// Sound effect to play.
	ANIM_NONE								// Follow up animation.
);

static AnimTypeClass const ChemBall(
	ANIM_CHEM_BALL,						// Animation number.
	"CHEMBALL",								// Data name of animation.
	21,										// Maximum dimension of animation.
	5,											// Biggest animation stage.
	false,									// Normalized animation rate?
	false,									// Uses white translucent table?
	false,									// Scorches the ground?
	false,									// Forms a crater?
	false,									// Sticks to unit in square?
	false,									// Ground level animation?
	false,									// Translucent colors in this animation?
	false,									// Is this a flame thrower animation?
	0x0000,									// Damage to apply per tick (fixed point).
	1,											// Delay between frames.
	0,											// Starting frame number.
	0,											// Loop start frame number.
	-1,										// Ending frame of loop back.
	-1,										// Number of animation stages.
	1,											// Number of times the animation loops.
	VOC_FLAMER1,							// Sound effect to play.
	ANIM_NONE
);



AnimTypeClass const * const AnimTypeClass::Pointers[ANIM_COUNT] = {
	&FBall1,
	&Grenade,
	&Frag1,
	&Frag3,
	&VehHit1,
	&VehHit2,
	&VehHit3,
	&ArtExp1,
	&Napalm1,
	&Napalm2,
	&Napalm3,
	&SmokePuff,
	&Piff,
	&PiffPiff,
	&FlameN,
	&FlameNE,
	&FlameE,
	&FlameSE,
	&FlameS,
	&FlameSW,
	&FlameW,
	&FlameNW,
	&ChemN,
	&ChemNE,
	&ChemE,
	&ChemSE,
	&ChemS,
	&ChemSW,
	&ChemW,
	&ChemNW,
	&Fire3,
	&Fire2,
	&Fire1,
	&Fire4,
	&Gunfire,
#ifdef NEVER
	&E1RotFire,
	&E1RotGrenade,
	&E1RotGun,
	&E1RotExp,
	&E2RotFire,
	&E2RotGrenade,
	&E2RotGun,
	&E2RotExp,
	&E3RotFire,
	&E3RotGrenade,
	&E3RotGun,
	&E3RotExp,
	&E4RotFire,
	&E4RotGrenade,
	&E4RotGun,
	&E4RotExp,
#endif
	&SmokeM,
	&BurnSmall,
	&BurnMed,
	&BurnBig,
	&OnFireSmall,
	&OnFireMed,
	&OnFireBig,
	&SAMN,
	&SAMNE,
	&SAME,
	&SAMSE,
	&SAMS,
	&SAMSW,
	&SAMW,
	&SAMNW,
	&GUNN,
	&GUNNE,
	&GUNE,
	&GUNSE,
	&GUNS,
	&GUNSW,
	&GUNW,
	&GUNNW,
	&LZSmoke,
	&IonCannon,
	&AtomBomb,
	&CDeviator,
	&CDollar,
	&CEarth,
	&CEmpulse,
	&CInvun,
	&CMine,
	&CRapid,
	&CStealth,
	&CMissile,
	&AtomDoor,
	&MoveFlash,
	&OilFieldBurn,
	&TricDie,
	&TRexDie,
	&StegDie,
	&RaptDie,
	&ChemBall,
	&Fire3Virtual,
	&Fire2Virtual,
	&Fire1Virtual,
	&Fire4Virtual
};


/***********************************************************************************************
 * AnimTypeClass::AnimTypeClass -- Constructor for animation types.                            *
 *                                                                                             *
 *    This is the constructor for static objects that elaborate the various animation types    *
 *    allowed in the game. Each animation in the game is of one of these types.                *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
AnimTypeClass::AnimTypeClass(AnimType anim, char const *name, int size, int biggest,
		bool isnormal, bool iswhitetrans, bool isscorcher, bool iscrater, bool issticky, bool ground,
		bool istrans, bool isflame, unsigned int damage,
		int delaytime, int start, int loopstart, int loopend, int stages, int loops, 
		VocType sound, AnimType chainto, int virtualstages, int virtualscale, AnimType virtualanim) :
	ObjectTypeClass(true, false, false, true, false, false, true, true, TXT_NONE, name, ARMOR_NONE, 0)
{
	Biggest = biggest;
	ChainTo = chainto;
	Damage = damage;
	Delay = (unsigned char)delaytime;
	IsCraterForming = iscrater;
	IsFlameThrower = isflame;
	IsGroundLayer = ground;
	IsNormalized = isnormal;
	IsScorcher = isscorcher;
	IsSticky = issticky;
	IsTranslucent = istrans;
	IsWhiteTrans = iswhitetrans;
	LoopEnd = loopend;
	LoopStart = loopstart;
	Loops = (unsigned char)loops;
	Size = size;
	Sound = sound;
	Stages = stages;
	Start = start;
	Type = anim;
	VirtualStages = virtualstages;
	VirtualScale = virtualscale;
	VirtualAnim = virtualanim;
}


/***********************************************************************************************
 * AnimTypeClass::One_Time -- Performs one time action for animation types.                    *
 *                                                                                             *
 *    This will load the animation shape data. It is called by the game initialization         *
 *    process.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine should be called ONLY once.                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AnimTypeClass::One_Time(void)
{
	AnimType index;

	for (index = ANIM_FIRST; index < ANIM_COUNT; index++) {
		char fullname[_MAX_FNAME+_MAX_EXT];

		_makepath(fullname, NULL, NULL, As_Reference(index).IniName, ".SHP");

		RawFileClass file(fullname);
		if (file.Is_Available()) {
			((void const *&)As_Reference(index).ImageData) = Load_Alloc_Data(file);
		} else {
			((void const *&)As_Reference(index).ImageData) = MixFileClass::Retrieve(fullname);
		}
	}
}


