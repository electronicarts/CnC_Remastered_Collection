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

/* $Header:   F:\projects\c&c\vcs\code\defines.h_v   2.19   16 Oct 1995 16:44:54   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : DEFINES.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : September 10, 1993   [JLB]                                   *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef DEFINES_H
#define DEFINES_H


//#define PETROGLYPH_EXAMPLE_MOD

/**********************************************************************
**	If defined, then the advanced balancing features will be enabled
**	for this version.
*/
//#define ADVANCED
#define PATCH			// Super patch (1.17?)


/**********************************************************************
**	The demo version of C&C will be built if the following define
**	is active.
*/
//#define DEMO


/*
** Use AI pulled in from Red Alert. ST - 7/26/2019 11:42AM
*/
#define USE_RA_AI


/**********************************************************************
**  Define this to allow play of the bonus missions for the Gateway
**  bundle deal.
*/
#define BONUS_MISSIONS


/**********************************************************************
** Handle expansion scnearios as a set of single missions with all
**	necessary information self contained within the mission file.
*/
#ifndef DEMO
#define NEWMENU
#endif


/**********************************************************************
**	If the scenario editor to to be active in this build then uncomment
**	the following #define line.
*/
//#define SCENARIO_EDITOR


/**********************************************************************
**	This define enables the full set of cheat keys and special
**	command line options.
*/
#define CHEAT_KEYS


/**********************************************************************
**	If this is defined, the special Virgin limited cheat keys
**	are enabled. This allows the "cheat" parameter and then only
**	allows the ALT-W to win the mission.
*/
//#define VIRGIN_CHEAT_KEYS


/**********************************************************************
**	Optional parameter control for special options.
*/
//#define	PARM_6PLAYER		0x5D9F6F24		// "6"
#define	PARM_6PLAYER	0x9CAFC93B		// Alternate 6 player keyphrase.

/*
**	Enable the set of limited cheat key options.
*/
#ifdef VIRGIN_CHEAT_KEYS
#define	PARM_PLAYTEST		0xF7DDC227		// "PLAYTEST"
#endif

/*
**	Enable the full set of cheat key options.
*/
#ifdef CHEAT_KEYS
#ifndef PARM_PLAYTEST
#define	PARM_PLAYTEST		0xF7DDC227		// "PLAYTEST"
#endif
#define	PARM_CHEATDAVID	0xBE79088C		// Cheat keys for David Dettmer
#define	PARM_CHEATERIK		0x9F38A19D		// Cheat keys for Erik Yeo
#define	PARM_EDITORERIK	0xC2AA509B		//	Map editor for Erik Yeo
#define	PARM_CHEATPHIL		0x39D01821		// Cheat keys for Phil Gorrow
#define	PARM_CHEATJOE		0xABDD0362		// Cheat keys for Joe Bostic
#define	PARM_CHEATBILL		0xB5B63531		// Cheat keys for Bill Randolph
#define	PARM_CHEAT_STEVET	0x2E7FE493		// Cheat keys for Steve Tall
#define	PARM_EDITORBILL	0x7E7C4CCA		//	"-EDITOR"
#define	PARM_CHEATMIKE		0x00532693		// Mike Lightner
#define	PARM_CHEATADAM		0xDFABC23A		// Adam Isgreen
#endif

//#define	PARM_CHEAT		0x6F4BE7CA		// "CHEAT"
//#define	PARM_EDITOR		0x7E7C4CCA		//	"-EDITOR"

#define	PARM_EASY			0x59E975CE		// "EASY" Enables easy mode.
#define	PARM_HARD			0xACFE9D13		// "HARD" Enables hard mode.

#define	PARM_INSTALL		0xD95C68A2		//	"FROMINSTALL"
#define	PARM_TRUENAME		0xB1A34435		// Enables true object names.
#define	PARM_3POINT			0x03552894		// Enable three point turns.
#define	PARM_SCORE			0x7FDE2C33		// Enables alternate themes.
#define	PARM_COMBAT			0xDC57C4B2		// Gives combat advantage to attacker.
#define	PARM_TREETARGET	0x00AB6BEF		// Allows targeting of trees without <CTRL> key.
#define	PARM_BIB				0xF7867BF0		// Disables building bibs.
#define	PARM_MCV				0x104DF10F		// MCV undeploys rather than sells.
#define	PARM_HELIPAD		0x53EBECBC		// Helipad can be purchased separately from helicopter.
#define	PARM_IQ				0x9E3881B8		// Smart self defense logic enable.
#define	PARM_SQUISH			0x4EA2FBDF		// Squish images for infantry bodies.
#define	PARM_HUMAN			0xACB58F61		// Human generated sound effects.
#define	PARM_SCROLLING		0xC084AE82		// Restricts scrolling over the tabs.
//#define	PARM_SPECIAL		0xD18129F6		// Enables special mode.
//#define	PARM_SPECIAL	0x2E84E394	//	#1
//#define	PARM_SPECIAL	0x63CE7584	//	#2
//#define	PARM_SPECIAL	0x85F110A5	//	#3
///#define	PARM_SPECIAL	0x7F65F13C	//	#4
//#define	PARM_SPECIAL	0x431F5F61	//	#5
#define	PARM_SPECIAL	0x11CA05BB	//	#6 	funpark
//#define	PARM_SPECIAL	0xE0F651B9	//	#7
//#define	PARM_SPECIAL	0x10B9683D	//	#8
//#define	PARM_SPECIAL	0xEE1CD37D	//	#9


/**********************************************************************
**	Defines for verifying free disk space
*/
#define	INIT_FREE_DISK_SPACE		1024*4096	//8388608
#define	SAVE_GAME_DISK_SPACE		INIT_FREE_DISK_SPACE	// (INIT_FREE_DISK_SPACE - (1024*4096))
//#define	SAVE_GAME_DISK_SPACE		 100000


/**********************************************************************
**	This is the credit threshold that the computer's money must exceed
**	in order for structure repair to commence.
*/
#define	REPAIR_THRESHHOLD		1000


//#define	GERMAN	1
//#define	FRENCH	1
//#define	JAPANESE	1

#define	FOREIGN_VERSION_NUMBER	6

//
// typedef enums with -1 will show this warning, even when the type of the enum is signed. It's a compiler bug, apparently
// ST - 1/8/2019 9:23AM
//
#pragma warning (push)
#pragma warning (disable:4341)

/**********************************************************************
**	These enumerations are used to implement RTTI.
*/
typedef enum RTTIType : unsigned char {
	RTTI_NONE=0,
	RTTI_INFANTRY,
	RTTI_INFANTRYTYPE,
	RTTI_UNIT,
	RTTI_UNITTYPE,
	RTTI_AIRCRAFT,
	RTTI_AIRCRAFTTYPE,
	RTTI_BUILDING,
	RTTI_BUILDINGTYPE,

	RTTI_TERRAIN,
	RTTI_ABSTRACTTYPE,
	RTTI_ANIM,
	RTTI_ANIMTYPE,
	RTTI_BULLET,
	RTTI_BULLETTYPE,
	RTTI_OVERLAY,
	RTTI_OVERLAYTYPE,
	RTTI_SMUDGE,
	RTTI_SMUDGETYPE,
	RTTI_TEAM,
	RTTI_TEMPLATE,
	RTTI_TEMPLATETYPE,
	RTTI_TERRAINTYPE,
	RTTI_OBJECT,
	RTTI_SPECIAL
} RTTIType;


/**********************************************************************
**	These are the difficulty settings of the game.
*/
typedef enum DiffType : unsigned char {
	DIFF_EASY,
	DIFF_NORMAL,
	DIFF_HARD,

	DIFF_COUNT,
	DIFF_FIRST = 0
} DiffType;


/**********************************************************************
**	This is the size of the speech buffer. This value should be as large
**	as the largest speech sample, plus a few bytes for overhead
**	(16 bytes is sufficient).
*/
#define SPEECH_BUFFER_SIZE		50000L


/**********************************************************************
**	This is the size of the shape buffer. This buffer is used as a staging
**	buffer for the shape drawing technology. It MUST be as big as the
**	largest shape (uncompressed) that will be drawn. If this value is
**	changed, be sure to update the makefile and rebuild all of the shape
**	data files.
*/
#define SHAPE_BUFFER_SIZE		131072L

// Use this to allow keep track of versions as they affect saved games.
#define	VERSION_NUMBER		1
#define	RELEASE_NUMBER	01

#define FAME_FILE_NAME					"HALLFAME.DAT"


/**********************************************************************
**	Map controls. The map is composed of square elements called 'cells'.
**	All larger elements are build upon these.
*/

// Size of the map in cells. The width of the map must be a power
//	of two. This is accomplished by setting the width by the number of
//	bits it occupies. The number of meta-cells will be a subset of the
//	cell width.
#define	MAP_CELL_MAX_X_BITS		6
#define	MAP_CELL_MAX_Y_BITS		6
#define	MAP_CELL_X_MASK			(~(~0 << MAP_CELL_MAX_X_BITS))
//#define	MAP_CELL_Y_MASK			((~(~0 << MAP_CELL_MAX_Y_BITS)) << MAP_CELL_MAX_Y_BITS)

// Size of the map in cells.
#define	MAP_CELL_W				(1<<MAP_CELL_MAX_X_BITS)
#define	MAP_CELL_H				(1<<MAP_CELL_MAX_Y_BITS)
#define	MAP_CELL_TOTAL			(MAP_CELL_W*MAP_CELL_H)

#define	REFRESH_EOL				32767		// This number ends a refresh/occupy offset list.
#define	REFRESH_SIDEBAR		32766		// This number flags that sidebar needs refreshing.

/*
**	The map is broken down into regions of this specified dimensions.
*/
#define	REGION_WIDTH		4
#define	REGION_HEIGHT		4
#define	MAP_REGION_WIDTH	(((MAP_CELL_W + (REGION_WIDTH -1)) / REGION_WIDTH)+2)
#define	MAP_REGION_HEIGHT	(((MAP_CELL_H + (REGION_WIDTH -1)) / REGION_HEIGHT)+2)
#define  MAP_TOTAL_REGIONS	(MAP_REGION_WIDTH * MAP_REGION_HEIGHT)


/**********************************************************************
**	These are the various return conditions that production may
**	produce.
*/
typedef enum ProdFailType : unsigned char {
	PROD_OK,						//	Production request successful.
	PROD_LIMIT,					//	Failed with production capacity limit reached.
	PROD_ILLEGAL,				//	Failed because of illegal request.
	PROD_CANT,					//	Faile because unable to comply (busy or occupied).
} ProdFailType;


/**********************************************************************
**	These are the special weapons that can be used in the game. The common thread
**	with these weapons is that they are controlled through the sidebar
**	mechanism.
*/
typedef enum SpecialWeaponType : unsigned char {
	SPC_NONE,
	SPC_ION_CANNON,			//	Partical beam from satalite (Akira effect).
	SPC_NUCLEAR_BOMB,			//	Tactical nuclear weapon.
	SPC_AIR_STRIKE				//	Conventional air strike.
} SpecialWeaponType;


/**********************************************************************
**	These defines control the rate of ion cannon and airstrike recharging.
*/
#define NUKE_GONE_TIME			14*TICKS_PER_MINUTE
#define ION_CANNON_GONE_TIME	10*TICKS_PER_MINUTE
#define AIR_CANNON_GONE_TIME	8*TICKS_PER_MINUTE
#define OBELISK_ANIMATION_RATE	15


/**********************************************************************
**	These are the response values when checking to see if an object
**	can enter or exist at a specified location. By examining this
**	return value, appropriate action may be chosen.
**	NOTE: If this changes, update the static array in Find_Path module.
*/
typedef enum MoveType : unsigned char {
	MOVE_OK, 				// No blockage.
	MOVE_CLOAK,				// A cloaked blocking enemy object.
	MOVE_MOVING_BLOCK,	// Blocked, but only temporarily.
	MOVE_DESTROYABLE,		// Enemy unit or building is blocking.
	MOVE_TEMP,  			// Blocked by friendly unit.
	MOVE_NO,					// Strictly prohibited terrain.

	MOVE_COUNT
} MoveType;


/**********************************************************************
**	These are the themes that the game can play. They must be in exact
**	same order as specified in the CONQUER.TXT file as well as the filename
**	list located in the ThemeClass.
*/
typedef enum ThemeType : char {
	THEME_PICK_ANOTHER=-2,
	THEME_NONE=-1,
	THEME_AIRSTRIKE,
	THEME_80MX,
	THEME_CHRG,
	THEME_CREP,
	THEME_DRIL,
	THEME_DRON,
	THEME_FIST,
	THEME_RECON,
	THEME_VOICE,
	THEME_HEAVYG,
	THEME_J1,
	THEME_JDI_V2,
	THEME_RADIO,
	THEME_RAIN,
	THEME_AOI,							// Act On Instinct
	THEME_CCTHANG,						//	C&C Thang
	THEME_DIE,							//	Die!!
	THEME_FWP,							//	Fight, Win, Prevail
	THEME_IND,							//	Industrial
	THEME_IND2,							//	Industrial2
	THEME_JUSTDOIT,					//	Just Do It!
	THEME_LINEFIRE,					//	In The Line Of Fire
	THEME_MARCH,						//	March To Your Doom
	THEME_MECHMAN,						// Mechanical Man
	THEME_NOMERCY,						//	No Mercy
	THEME_OTP,							//	On The Prowl
	THEME_PRP,							//	Prepare For Battle
	THEME_ROUT,							//	Reaching Out
	THEME_HEART,						//
	THEME_STOPTHEM,					//	Stop Them
	THEME_TROUBLE,						//	Looks Like Trouble
	THEME_WARFARE,						//	Warfare
	THEME_BFEARED,						//	Enemies To Be Feared
	THEME_IAM,							// I Am
	THEME_WIN1,							//	Great Shot!
	THEME_MAP1,							// Map subliminal techno "theme".
	THEME_VALKYRIE,					// Ride of the valkyries.

	THEME_COUNT,
	THEME_LAST=THEME_BFEARED,
	THEME_FIRST=0
} ThemeType;

//inline ThemeType operator++(ThemeType &, int);


/**********************************************************************
**	This is the various threat scan methods that can be used when looking
**	for targets.
*/
typedef enum ThreatType : unsigned short {
	THREAT_NORMAL=0x0000,		// Any distance threat scan?
	THREAT_RANGE=0x0001,			// Limit scan to weapon range?
	THREAT_AREA=0x0002,			// Limit scan to general area (twice weapon range)?
	THREAT_AIR=0x0004,			// Scan for air units?
	THREAT_INFANTRY=0x0008,		// Scan for infantry units?
	THREAT_VEHICLES=0x0010,		// Scan for vehicles?
	THREAT_BUILDINGS=0x0020,	// Scan for buildings?
	THREAT_TIBERIUM=0x0040,		// Limit scan to Tiberium processing objects?
	THREAT_BOATS=0x0080,			// Scan for gunboats?
	THREAT_CIVILIANS=0x0100,	// Consider civilians to be primary target?
	THREAT_CAPTURE=0x0200,		// Consider capturable buildings only?
} ThreatType;
//inline ThreatType operator |(ThreatType, ThreatType);
//inline ThreatType operator &(ThreatType, ThreatType);
//inline ThreatType operator ~(ThreatType);

inline ThreatType operator|(ThreatType a, ThreatType b)
{return static_cast<ThreatType>(static_cast<int>(a) | static_cast<int>(b));}

inline ThreatType operator&(ThreatType a, ThreatType b)
{return static_cast<ThreatType>(static_cast<int>(a) & static_cast<int>(b));}

inline ThreatType operator~(ThreatType a)
{return static_cast<ThreatType>(~static_cast<int>(a));}


#define THREAT_GROUND	(THREAT_VEHICLES|THREAT_BUILDINGS|THREAT_INFANTRY)


/**********************************************************************
**	These return values are used when determine if firing is legal.
**	By examining this value it can be determined what should be done
**	to fix the reason why firing wasn't allowed.
*/
typedef enum FireErrorType : unsigned char {
	FIRE_OK,				// Weapon is allowed to fire.
	FIRE_AMMO,			// No ammo available to fire?
	FIRE_FACING,		// Not correctly facing target?
	FIRE_REARM,			// It is busy rearming?
	FIRE_ROTATING,		// Is it in process of rotating?
	FIRE_ILLEGAL,		// Is it targeting something illegal?
	FIRE_CANT,			// Is this unit one that cannot fire anything?
	FIRE_MOVING,		// Is it moving and not allowed to fire while moving?
	FIRE_RANGE,			// Is the target out of range?
	FIRE_CLOAKED,		// Is the shooter currently cloaked?
	FIRE_BUSY			// Is shooter currently doing something else?
} FireErrorType;


/**********************************************************************
**	If an object can cloak, then it will be in one of these states.
**	For objects that cannot cloak, they will always be in the
**	UNCLOAKED state. This state controls how the obect transitions between
**	cloaked and uncloaked conditions.
*/
typedef enum CloakType : unsigned char {
	UNCLOAKED,			// Completely visible (normal state).
	CLOAKING,			// In process of claoking.
	CLOAKED,				// Completely cloaked (invisible).
	UNCLOAKING			// In process of uncloaking.
} CloakType;


/**********************************************************************
**	For units that are cloaking, these value specify the visual character
**	of the object.
*/
typedef enum VisualType : unsigned char {
	VISUAL_NORMAL,				// Completely visible -- normal.
	VISUAL_INDISTINCT,		// The edges shimmer and become indistinct.
	VISUAL_DARKEN,				// Color and texture is muted along with shimmering.
	VISUAL_SHADOWY,			// Body is translucent in addition to shimmering.
	VISUAL_RIPPLE,				// Just a ripple (true predator effect).
	VISUAL_HIDDEN,				// Nothing at all is visible.
} VisualType;


/**********************************************************************
**	These missions enumerate the various state machines that can apply to
**	a game object. Only one of these state machines is active at any one
**	time.
*/
typedef enum MissionType : char {
	MISSION_NONE=-1,

	MISSION_SLEEP,				// Do nothing whatsoever.
	MISSION_ATTACK,			// Attack nearest enemy.
	MISSION_MOVE,				// Guard location or unit.
	MISSION_RETREAT,			// Return home for R & R.
	MISSION_GUARD,				// Stay still.
	MISSION_STICKY,			// Stay still -- never recruit.
	MISSION_ENTER,				// Move into object cooperatively.
	MISSION_CAPTURE,			//	Move into in order to capture.
	MISSION_HARVEST,			// Hunt for and collect nearby Tiberium.
	MISSION_GUARD_AREA,		// Active guard of area.
	MISSION_RETURN,			// Head back to refinery.
	MISSION_STOP,				// Sit still.
	MISSION_AMBUSH,			// Wait until discovered.
	MISSION_HUNT,				// Active search and destroy.
	MISSION_TIMED_HUNT,		// Wait a while, then go into HUNT (multiplayer AI)
	MISSION_UNLOAD,			// Search for and deliver cargo.
	MISSION_SABOTAGE,			//	Move into in order to destroy.
	MISSION_CONSTRUCTION,	// Building buildup operation.
	MISSION_DECONSTRUCTION,	// Building builddown operation.
	MISSION_REPAIR,			// Repair process mission.
	MISSION_RESCUE,
	MISSION_MISSILE,

	MISSION_COUNT,
	MISSION_FIRST=0
} MissionType;


/**********************************************************************
**	These are the enumerated animation sequences that a building may
**	be processing. These serve to control the way that a building
**	appears.
*/
typedef enum BStateType : char {
	BSTATE_NONE=-1,
	BSTATE_CONSTRUCTION,		// Construction animation.
	BSTATE_IDLE,				// Idle animation.
	BSTATE_ACTIVE,				// Animation when building is "doing its thing".
	BSTATE_FULL,				// Special alternate active state.
	BSTATE_AUX1,				// Auxiliary animation.
	BSTATE_AUX2,				// Auxiliary animation.

	BSTATE_COUNT
} BStateType;


/**********************************************************************
**	Whenever a unit is selected and a click occurs over another object
**	or terrain element, there is some action to initiate. This specifies
**	the different types of actions possible. This also controls how the
**	mouse cursor looks when "hovering" over the spot that clicking would
**	occur at.
*/
typedef enum ActionType : unsigned char {
	ACTION_NONE,			// Either undefined action or "do nothing".
	ACTION_MOVE,			// Can move there or at least try to.
	ACTION_NOMOVE,			// Special case for movable object, but illegal mouse position.
	ACTION_ENTER,			// Special case for infantry->APC or vehicle->Repair facility.
	ACTION_SELF,			// Self select special case.
	ACTION_ATTACK,			// Can attack or fire upon it in some fashion.
	ACTION_HARVEST,		// Special harvest mode.
	ACTION_SELECT,			// Would change selection to specified object.
	ACTION_TOGGLE_SELECT,// Toggles select state of the object.
	ACTION_CAPTURE,		// The unit will try to capture the object.
	ACTION_REPAIR,			// The target object should be repaired.
	ACTION_SELL,			// The target building should be sold back.
	ACTION_SELL_UNIT,		// The target unit should be sold back.
	ACTION_NO_SELL,  		// No sell or no repair.
	ACTION_NO_REPAIR,		// No sell or no repair.
	ACTION_SABOTAGE,		// The unit will try to sabotage/destroy the object.
	ACTION_ION,				// That target object should be blasted.
	ACTION_NUKE_BOMB,		// That target object should be blasted.
	ACTION_AIR_STRIKE,	// That target object should be blasted.
	ACTION_GUARD_AREA,	// Guard the area/object clicked on.
	ACTION_TOGGLE_PRIMARY,	// Toggle the primary status of the factory.
	ACTION_NO_DEPLOY,

	ACTION_COUNT
} ActionType;


/**********************************************************************
**	When a unit gets damaged, the result of the damage is returned as
**	this type. It can range from no damage taken to complete destruction.
*/
typedef enum ResultType : unsigned char {
	RESULT_NONE,				// No damage was taken by the target.
	RESULT_LIGHT,				// Some damage was taken, but no state change occurred.
	RESULT_HALF,				// Damaged to below half strength (only returned on transition).
	RESULT_MAJOR,				// Damaged down to 1 hit point.
	RESULT_DESTROYED,			// Damaged to complete destruction.
} ResultType;


/**********************************************************************
**	These are the special concrete control defines. They enumerate the
**	sequence order of the concrete icons in the concrete art file.
*/
// DEBUG === convert this to be zero based so that a nulled cell is the
//			 	 default cell.
enum ConcreteEnum : char {
	C_NONE=-1,
	C_LEFT=0,
	C_RIGHT=1,
	C_RIGHT_UPDOWN=2,
	C_LEFT_UPDOWN=3,
	C_UP_RIGHT=4,
	C_UP_LEFT=5,
	C_DOWN_RIGHT=6,
	C_DOWN_LEFT=7,
	C_RIGHT_DOWN=8,
	C_LEFT_DOWN=9,
	C_RIGHT_UP=10,
	C_LEFT_UP=11,
	C_UPDOWN_RIGHT=12,
	C_UPDOWN_LEFT=13
};


/**********************************************************************
**	Units that move can move at different speeds. These enumerate the
**	different speeds that a unit can move.
*/
typedef enum MPHType: unsigned char {
	MPH_IMMOBILE=0,
	MPH_VERY_SLOW=5,
	MPH_KINDA_SLOW=6,
	MPH_SLOW=8,
	MPH_SLOW_ISH=10,
	MPH_MEDIUM_SLOW=12,
	MPH_MEDIUM=18,
	MPH_MEDIUM_FAST=30,
	MPH_MEDIUM_FASTER=35,
	MPH_FAST=40,
	MPH_ROCKET=60,
	MPH_VERY_FAST=100,
	MPH_LIGHT_SPEED=255
} MPHType;


/**********************************************************************
**	General audio volume is enumerated by these identifiers. Since small
**	volume variations are usually unnoticable when specifying the volume
**	to play a sample, this enumeration list creates more readable code.
*/
typedef enum VolType : unsigned char 
{
	VOL_OFF=0,
	VOL_0=VOL_OFF,
	VOL_1=0x19,
	VOL_2=0x32,
	VOL_3=0x4C,
	VOL_4=0x66,
	VOL_5=0x80,
	VOL_6=0x9A,
	VOL_7=0xB4,
	VOL_8=0xCC,
	VOL_9=0xE6,
	VOL_10=0xFF,
	VOL_FULL=VOL_10
} VolType;


/**********************************************************************
**	The houses that can be played are listed here. Each has their own
**	personality and strengths.
*/
typedef enum HousesType : char {
	HOUSE_NONE=-1,
	HOUSE_GOOD,					// Global Defense Initiative
	HOUSE_BAD,					// Brotherhood of Nod
	HOUSE_NEUTRAL,				// Civilians
	HOUSE_JP,					// Disaster Containment Team
	HOUSE_MULTI1,				// Multi-Player house #1
	HOUSE_MULTI2,				// Multi-Player house #2
	HOUSE_MULTI3,				// Multi-Player house #3
	HOUSE_MULTI4,				// Multi-Player house #4
	HOUSE_MULTI5,				// Multi-Player house #5
	HOUSE_MULTI6,				// Multi-Player house #6

	HOUSE_COUNT,
	HOUSE_FIRST=HOUSE_GOOD
} HousesType;

//inline HousesType operator++(HousesType &, int);

#define	HOUSEF_GOOD		(1<<HOUSE_GOOD)
#define	HOUSEF_BAD		(1<<HOUSE_BAD)
#define	HOUSEF_NEUTRAL	(1<<HOUSE_NEUTRAL)
#define	HOUSEF_JP		(1<<HOUSE_JP)
#define	HOUSEF_MULTI1	(1<<HOUSE_MULTI1)
#define	HOUSEF_MULTI2	(1<<HOUSE_MULTI2)
#define	HOUSEF_MULTI3	(1<<HOUSE_MULTI3)
#define	HOUSEF_MULTI4	(1<<HOUSE_MULTI4)
#define	HOUSEF_MULTI5	(1<<HOUSE_MULTI5)
#define	HOUSEF_MULTI6	(1<<HOUSE_MULTI6)

typedef enum PlayerColorType : char {
	REMAP_NONE = -1,
	REMAP_GOLD,
	REMAP_FIRST=REMAP_GOLD,
	REMAP_LTBLUE,
	REMAP_RED,
	REMAP_GREEN,
	REMAP_ORANGE,
	REMAP_BLUE,
	REMAP_LAST=REMAP_BLUE,

	REMAP_COUNT
} PlayerColorType;


/**********************************************************************
** These are the types of games that can be played.  GDI & NOD are the
** usual human-vs-computer games; 2-Player games are network or modem,
** with 2 players; multi-player games are network with > 2 players.
*/
typedef enum ScenarioPlayerEnum : char 
{
	SCEN_PLAYER_NONE = -1,
	SCEN_PLAYER_GDI,
	SCEN_PLAYER_NOD,
	SCEN_PLAYER_JP,
	SCEN_PLAYER_2PLAYER,
	SCEN_PLAYER_MPLAYER,
	SCEN_PLAYER_COUNT,
	SCEN_PLAYER_FIRST = 0,
} ScenarioPlayerType;

//inline ScenarioPlayerType operator++(ScenarioPlayerType &, int);


/**********************************************************************
** These are the directional parameters for a scenario.
*/
typedef enum ScenarioDirEnum : char
{
	SCEN_DIR_NONE = -1,
	SCEN_DIR_EAST,
	SCEN_DIR_WEST,
	SCEN_DIR_COUNT,
	SCEN_DIR_FIRST = 0,
} ScenarioDirType;

//inline ScenarioDirType operator++(ScenarioDirType &, int);


/**********************************************************************
** These are the random variations of a scenario.
*/
typedef enum ScenarioVarEnum : char
{
	SCEN_VAR_NONE = -1,
	SCEN_VAR_A,
	SCEN_VAR_B,
	SCEN_VAR_C,
	SCEN_VAR_D,
	SCEN_VAR_COUNT,		// comes before the Lose value!
	SCEN_VAR_LOSE,
	SCEN_VAR_FIRST = 0,
} ScenarioVarType;

//inline ScenarioVarType operator++(ScenarioVarType &, int);


/**********************************************************************
**	The objects to be drawn on the map are grouped into layers. These
**	enumerated values specify those layers. The ground layer is sorted
**	from back to front.
*/
typedef enum LayerType : char {
	LAYER_NONE=-1,
	LAYER_GROUND,		// Touching the ground type object (units & buildings).
	LAYER_AIR,			// Flying above the ground (explosions & flames).
	LAYER_TOP,			// Topmost layer (aircraft & bullets).

	LAYER_COUNT,
	LAYER_FIRST=0
} LayerType;

//inline LayerType operator++(LayerType &, int);


/**********************************************************************
**	This enumerates the various bullet types. These types specify bullet's
**	visual and explosive characteristics.
*/
typedef enum BulletType : char {
	BULLET_NONE=-1,
	BULLET_SNIPER,			// Sniper bullet.
	BULLET_BULLET,			// Small arms
	BULLET_APDS,			// Armor piercing projectile.
	BULLET_HE,			 	// High explosive shell.
	BULLET_SSM,				// Surface to surface small missile type.
	BULLET_SSM2,			// MLRS missile.
	BULLET_SAM,				// Fast homing anti-aircraft missile.
	BULLET_TOW,				// TOW anti-vehicle short range missile.
	BULLET_FLAME,			// Flame thrower flame.
	BULLET_CHEMSPRAY,		// Chemical weapon spray.
	BULLET_NAPALM,			// Napalm bomblet.
	BULLET_GRENADE,		// Hand tossed grenade.
	BULLET_LASER,			// Laser beam from obelisk
	BULLET_NUKE_UP,  		// Nuclear Missile on its way down
	BULLET_NUKE_DOWN,		// Nuclear Missile on its way up
	BULLET_HONEST_JOHN,	// SSM with napalm warhead.
	BULLET_SPREADFIRE,	// Chain gun bullets.
	BULLET_HEADBUTT,		// Stegosaurus, Triceratops head butt
	BULLET_TREXBITE,		// Tyrannosaurus Rex's bite - especially bad for infantry

#ifdef PETROGLYPH_EXAMPLE_MOD
	BULLET_NUKE_LOB,		// Nuke projectile
#endif //PETROGLYPH_EXAMPLE_MOD

	BULLET_COUNT,
	BULLET_FIRST=0
} BulletType;

//inline BulletType operator++(BulletType &, int);


/**********************************************************************
**	All game buildings (structures) are enumerated here. This includes
**	civilian structures as well.
*/
typedef enum StructType : char {
	STRUCT_NONE=-1,
	STRUCT_WEAP,
	STRUCT_GTOWER,
	STRUCT_ATOWER,
	STRUCT_OBELISK,
	STRUCT_RADAR,
	STRUCT_TURRET,
	STRUCT_CONST,
	STRUCT_REFINERY,
	STRUCT_STORAGE,
	STRUCT_HELIPAD,
	STRUCT_SAM,
	STRUCT_AIRSTRIP,
	STRUCT_POWER,
	STRUCT_ADVANCED_POWER,
	STRUCT_HOSPITAL,
	STRUCT_BARRACKS,
	STRUCT_TANKER,
	STRUCT_REPAIR,
	STRUCT_BIO_LAB,
	STRUCT_HAND,
	STRUCT_TEMPLE,
	STRUCT_EYE,
	STRUCT_MISSION,

	/*
	**	All buildings that are never used as a prerequisite
	**	for construction, follow this point. Typically, this is
	**	limited to civilian structures.
	*/
	STRUCT_V01,
	STRUCT_V02,
	STRUCT_V03,
	STRUCT_V04,
	STRUCT_V05,
	STRUCT_V06,
	STRUCT_V07,
	STRUCT_V08,
	STRUCT_V09,
	STRUCT_V10,
	STRUCT_V11,
	STRUCT_V12,
	STRUCT_V13,
	STRUCT_V14,
	STRUCT_V15,
	STRUCT_V16,
	STRUCT_V17,
	STRUCT_V18,
	STRUCT_PUMP,
	STRUCT_V20,
	STRUCT_V21,
	STRUCT_V22,
	STRUCT_V23,
	STRUCT_V24,
	STRUCT_V25,
	STRUCT_V26,
	STRUCT_V27,
	STRUCT_V28,
	STRUCT_V29,
	STRUCT_V30,
	STRUCT_V31,
	STRUCT_V32,
	STRUCT_V33,
	STRUCT_V34,
	STRUCT_V35,
	STRUCT_V36,
	STRUCT_V37,
#ifdef OBSOLETE
	STRUCT_ROAD,
#endif
	STRUCT_SANDBAG_WALL,
	STRUCT_CYCLONE_WALL,
	STRUCT_BRICK_WALL,
	STRUCT_BARBWIRE_WALL,
	STRUCT_WOOD_WALL,

	STRUCT_COUNT,
	STRUCT_FIRST=0
} StructType;

//inline StructType operator++(StructType &, int);

#define	STRUCTF_NONE				0L
#define	STRUCTF_ADVANCED_POWER	(1L << STRUCT_ADVANCED_POWER)
#define	STRUCTF_REPAIR				(1L << STRUCT_REPAIR)
#define	STRUCTF_EYE					(1L << STRUCT_EYE)
#define	STRUCTF_TEMPLE				(1L << STRUCT_TEMPLE)
#define	STRUCTF_HAND				(1L << STRUCT_HAND)
#define	STRUCTF_BIO_LAB			(1L << STRUCT_BIO_LAB)
#define	STRUCTF_OBELISK			(1L << STRUCT_OBELISK)
#define	STRUCTF_ATOWER				(1L << STRUCT_ATOWER)
#define	STRUCTF_WEAP				(1L << STRUCT_WEAP)
#define	STRUCTF_GTOWER				(1L << STRUCT_GTOWER)
#define	STRUCTF_RADAR				(1L << STRUCT_RADAR)
#define	STRUCTF_TURRET				(1L << STRUCT_TURRET)
#define	STRUCTF_CIV1				(1L << STRUCT_CIV1)
#define	STRUCTF_CIV2				(1L << STRUCT_CIV2)
#define	STRUCTF_CIV3				(1L << STRUCT_CIV3)
#define	STRUCTF_CONST				(1L << STRUCT_CONST)
#define	STRUCTF_REFINERY			(1L << STRUCT_REFINERY)
#define	STRUCTF_STORAGE			(1L << STRUCT_STORAGE)
#define	STRUCTF_HELIPAD			(1L << STRUCT_HELIPAD)
#define	STRUCTF_SAM					(1L << STRUCT_SAM)
#define	STRUCTF_AIRSTRIP			(1L << STRUCT_AIRSTRIP)
#define	STRUCTF_POWER				(1L << STRUCT_POWER)
#define	STRUCTF_HOSPITAL			(1L << STRUCT_HOSPITAL)
#define	STRUCTF_BARRACKS			(1L << STRUCT_BARRACKS)
#define	STRUCTF_TANKER				(1L << STRUCT_TANKER)
#define	STRUCTF_MISSION			(1L << STRUCT_MISSION)


/**********************************************************************
**	The overlays are enumerated here. An overlay functions similarly to
**	a transparent icon. It is placed over the terrain but usually falls
**	"under" buildings, trees, and units.
*/
typedef enum OverlayType : char {
	OVERLAY_NONE=-1,
	OVERLAY_CONCRETE,			// Concrete.
	OVERLAY_SANDBAG_WALL,	// Piled sandbags.
	OVERLAY_CYCLONE_WALL,	// Chain-link fence.
	OVERLAY_BRICK_WALL,		// Solid concrete wall.
	OVERLAY_BARBWIRE_WALL,	// Barbed-wire wall.
	OVERLAY_WOOD_WALL,		// Wooden fence.
	OVERLAY_TIBERIUM1,		// Tiberium patch.
	OVERLAY_TIBERIUM2,		// Tiberium patch.
	OVERLAY_TIBERIUM3,		// Tiberium patch.
	OVERLAY_TIBERIUM4,		// Tiberium patch.
	OVERLAY_TIBERIUM5,		// Tiberium patch.
	OVERLAY_TIBERIUM6,		// Tiberium patch.
	OVERLAY_TIBERIUM7,		// Tiberium patch.
	OVERLAY_TIBERIUM8,		// Tiberium patch.
	OVERLAY_TIBERIUM9,		// Tiberium patch.
	OVERLAY_TIBERIUM10,		// Tiberium patch.
	OVERLAY_TIBERIUM11,		// Tiberium patch.
	OVERLAY_TIBERIUM12,		// Tiberium patch.
	OVERLAY_ROAD,				// Road/concrete piece.
	OVERLAY_SQUISH,			// Squish mark for overran infantry.
	OVERLAY_V12,				// Haystacks
	OVERLAY_V13,				// Haystack
	OVERLAY_V14,				// Wheat field
	OVERLAY_V15,				// Fallow field
	OVERLAY_V16,				//	Corn field
	OVERLAY_V17,				// Celery field
	OVERLAY_V18,				// Potato field
	OVERLAY_FLAG_SPOT,		// Flag start location.
	OVERLAY_WOOD_CRATE,		// Wooden goodie crate.
	OVERLAY_STEEL_CRATE,		//	Steel goodie crate.

	OVERLAY_COUNT,
	OVERLAY_FIRST=0
} OverlayType;

//inline OverlayType operator++(OverlayType &, int);


/**********************************************************************
**	This specifies the infantry in the game. The "E" designation is
**	similar to the army classification of enlisted soldiers.
*/
typedef enum InfantryType : char {
	INFANTRY_NONE=-1,
	INFANTRY_E1,				// Mini-gun armed.
	INFANTRY_E2,				// Grenade thrower.
	INFANTRY_E3,				// Rocket launcher.
	INFANTRY_E4,				// Flame thrower equipped.
	INFANTRY_E5,				// Chemical thrower equipped.
	INFANTRY_E7,				// Engineer.
	INFANTRY_RAMBO,			// Commando.

	INFANTRY_C1,				// Civilian
	INFANTRY_C2,				// Civilian
	INFANTRY_C3,				// Civilian
	INFANTRY_C4,				// Civilian
	INFANTRY_C5,				// Civilian
	INFANTRY_C6,				// Civilian
	INFANTRY_C7,				// Civilian
	INFANTRY_C8,				// Civilian
	INFANTRY_C9,				// Civilian
	INFANTRY_C10,				// Nikumba
	INFANTRY_MOEBIUS,			// Dr. Moebius
	INFANTRY_DELPHI,			// Agent "Delphi"
	INFANTRY_CHAN,				// Dr. Chan

	INFANTRY_COUNT,
	INFANTRY_FIRST=0
} InfantryType;

//inline InfantryType operator++(InfantryType &, int);


/**********************************************************************
**	The game units are enumerated here. These include not only traditional
**	vehicles, but also hovercraft and gunboats.
*/
typedef enum UnitType : char{
	UNIT_NONE=-1,
	UNIT_HTANK,					// Heavy tank (Mammoth).
	UNIT_MTANK,					// Medium tank (M1).
	UNIT_LTANK,					// Light tank ('Bradly').
	UNIT_STANK,					// Stealth tank (Romulan).
	UNIT_FTANK,					// Flame thrower tank.
	UNIT_VICE,					// Visceroid
	UNIT_APC,					// APC.
	UNIT_MLRS,					// MLRS rocket launcher.
	UNIT_JEEP,					// 4x4 jeep replacement.
	UNIT_BUGGY,					// Rat patrol dune buggy type.
	UNIT_HARVESTER,			// Resource gathering vehicle.
	UNIT_ARTY,					// Artillery unit.
	UNIT_MSAM,					// Anti-Aircraft vehicle.
	UNIT_HOVER,					// Hovercraft.
	UNIT_MHQ,					// Mobile Head Quarters.
	UNIT_GUNBOAT,				// Gunboat
	UNIT_MCV,					// Mobile construction vehicle.
	UNIT_BIKE,					// Nod recon motor-bike.
	UNIT_TRIC,					// Triceratops
	UNIT_TREX,					//	Tyranosaurus Rex
	UNIT_RAPT,					//	Velociraptor
	UNIT_STEG,					//	Stegasaurus

#ifdef PETROGLYPH_EXAMPLE_MOD
	UNIT_NUKE_TANK,			// Mammoth with a nuke
#endif			

	UNIT_COUNT,
	UNIT_FIRST=0
} UnitType;

//inline UnitType operator++(UnitType &, int);

#define	UNITF_HTANK			(1L<<UNIT_HTANK)
#define	UNITF_MTANK			(1L<<UNIT_MTANK)
#define	UNITF_LTANK			(1L<<UNIT_LTANK)
#define	UNITF_STANK			(1L<<UNIT_STANK)
#define	UNITF_FTANK			(1L<<UNIT_FTANK)
#define	UNITF_APC			(1L<<UNIT_APC)
#define	UNITF_MLRS			(1L<<UNIT_MLRS)
#define	UNITF_JEEP			(1L<<UNIT_JEEP)
#define	UNITF_BUGGY			(1L<<UNIT_BUGGY)
#define	UNITF_HARVESTER	(1L<<UNIT_HARVESTER)
#define	UNITF_ARTY			(1L<<UNIT_ARTY)
#define	UNITF_MSAM			(1L<<UNIT_MSAM)
#define	UNITF_HOVER			(1L<<UNIT_HOVER)
#define	UNITF_MHQ			(1L<<UNIT_MHQ)
#define	UNITF_GUNBOAT		(1L<<UNIT_GUNBOAT)
#define	UNITF_MCV			(1L<<UNIT_MCV)
#define	UNITF_BIKE			(1L<<UNIT_BIKE)
#define	UNITF_VICE			(1L<<UNIT_VICE)
#define	UNITF_TRIC			(1L<<UNIT_TRIC)
#define	UNITF_TREX			(1L<<UNIT_TREX)
#define	UNITF_RAPT			(1L<<UNIT_RAPT)
#define	UNITF_STEG			(1L<<UNIT_STEG)


/**********************************************************************
**	The variuos aircraft types are enumerated here. These include helicopters
**	as well as traditional aircraft.
*/
typedef enum AircraftType : char {
	AIRCRAFT_TRANSPORT,			// Transport helicopter.
	AIRCRAFT_A10,					// Ground attack plane.
	AIRCRAFT_HELICOPTER,			// Apache gunship.
	AIRCRAFT_CARGO,				// Cargo plane.
	AIRCRAFT_ORCA,					// Nod attack helicopter.

	AIRCRAFT_COUNT,
	AIRCRAFT_NONE=-1,
	AIRCRAFT_FIRST=0
} AircraftType;

#define	AIRCRAFTF_TRANSPORT	(1L << AIRCRAFT_TRANSPORT)
#define	AIRCRAFTF_A10			(1L << AIRCRAFT_A10)
#define	AIRCRAFTF_HELICOPTER	(1L << AIRCRAFT_HELICOPTER)
#define	AIRCRAFTF_CARGO		(1L << AIRCRAFT_CARGO)
#define	AIRCRAFTF_ORCA			(1L << AIRCRAFT_ORCA)

//inline AircraftType operator++(AircraftType &, int);


/**********************************************************************
**	The game templates are enumerated here. These are the underlying
**	terrain art. This includes everything from water to clifs. If the
**	terrain is broken up into icons, is not transparent, and is drawn
**	as the bottom most layer, then it is a template.
*/
typedef enum TemplateType : unsigned char {
	TEMPLATE_CLEAR1,
	TEMPLATE_WATER,			// This must be the first non-clear template.
	TEMPLATE_WATER2,
	TEMPLATE_SHORE1,
	TEMPLATE_SHORE2,
	TEMPLATE_SHORE3,
	TEMPLATE_SHORE4,
	TEMPLATE_SHORE5,
	TEMPLATE_SHORE11,
	TEMPLATE_SHORE12,
	TEMPLATE_SHORE13,
	TEMPLATE_SHORE14,
	TEMPLATE_SHORE15,
	TEMPLATE_SLOPE1,
	TEMPLATE_SLOPE2,
	TEMPLATE_SLOPE3,
	TEMPLATE_SLOPE4,
	TEMPLATE_SLOPE5,
	TEMPLATE_SLOPE6,
	TEMPLATE_SLOPE7,
	TEMPLATE_SLOPE8,
	TEMPLATE_SLOPE9,
	TEMPLATE_SLOPE10,
	TEMPLATE_SLOPE11,
	TEMPLATE_SLOPE12,
	TEMPLATE_SLOPE13,
	TEMPLATE_SLOPE14,
	TEMPLATE_SLOPE15,
	TEMPLATE_SLOPE16,
	TEMPLATE_SLOPE17,
	TEMPLATE_SLOPE18,
	TEMPLATE_SLOPE19,
	TEMPLATE_SLOPE20,
	TEMPLATE_SLOPE21,
	TEMPLATE_SLOPE22,
	TEMPLATE_SLOPE23,
	TEMPLATE_SLOPE24,
	TEMPLATE_SLOPE25,
	TEMPLATE_SLOPE26,
	TEMPLATE_SLOPE27,
	TEMPLATE_SLOPE28,
	TEMPLATE_SLOPE29,
	TEMPLATE_SLOPE30,
	TEMPLATE_SLOPE31,
	TEMPLATE_SLOPE32,
	TEMPLATE_SLOPE33,
	TEMPLATE_SLOPE34,
	TEMPLATE_SLOPE35,
	TEMPLATE_SLOPE36,
	TEMPLATE_SLOPE37,
	TEMPLATE_SLOPE38,
	TEMPLATE_SHORE32,
	TEMPLATE_SHORE33,
	TEMPLATE_SHORE20,
	TEMPLATE_SHORE21,
	TEMPLATE_SHORE22,
	TEMPLATE_SHORE23,
	TEMPLATE_BRUSH1,
	TEMPLATE_BRUSH2,
	TEMPLATE_BRUSH3,
	TEMPLATE_BRUSH4,
	TEMPLATE_BRUSH5,
	TEMPLATE_BRUSH6,
	TEMPLATE_BRUSH7,
	TEMPLATE_BRUSH8,
	TEMPLATE_BRUSH9,
	TEMPLATE_BRUSH10,
	TEMPLATE_PATCH1,
	TEMPLATE_PATCH2,
	TEMPLATE_PATCH3,
	TEMPLATE_PATCH4,
	TEMPLATE_PATCH5,
	TEMPLATE_PATCH6,
	TEMPLATE_PATCH7,
	TEMPLATE_PATCH8,
	TEMPLATE_SHORE16,
	TEMPLATE_SHORE17,
	TEMPLATE_SHORE18,
	TEMPLATE_SHORE19,
	TEMPLATE_PATCH13,
	TEMPLATE_PATCH14,
	TEMPLATE_PATCH15,
	TEMPLATE_BOULDER1,
	TEMPLATE_BOULDER2,
	TEMPLATE_BOULDER3,
	TEMPLATE_BOULDER4,
	TEMPLATE_BOULDER5,
	TEMPLATE_BOULDER6,
	TEMPLATE_SHORE6,
	TEMPLATE_SHORE7,
	TEMPLATE_SHORE8,
	TEMPLATE_SHORE9,
	TEMPLATE_SHORE10,

	TEMPLATE_ROAD1,
	TEMPLATE_ROAD2,
	TEMPLATE_ROAD3,
	TEMPLATE_ROAD4,
	TEMPLATE_ROAD5,
	TEMPLATE_ROAD6,
	TEMPLATE_ROAD7,
	TEMPLATE_ROAD8,
	TEMPLATE_ROAD9,
	TEMPLATE_ROAD10,
	TEMPLATE_ROAD11,
	TEMPLATE_ROAD12,
	TEMPLATE_ROAD13,
	TEMPLATE_ROAD14,
	TEMPLATE_ROAD15,
	TEMPLATE_ROAD16,
	TEMPLATE_ROAD17,
	TEMPLATE_ROAD18,
	TEMPLATE_ROAD19,
	TEMPLATE_ROAD20,
	TEMPLATE_ROAD21,
	TEMPLATE_ROAD22,
	TEMPLATE_ROAD23,
	TEMPLATE_ROAD24,
	TEMPLATE_ROAD25,
	TEMPLATE_ROAD26,
	TEMPLATE_ROAD27,
	TEMPLATE_ROAD28,
	TEMPLATE_ROAD29,
	TEMPLATE_ROAD30,
	TEMPLATE_ROAD31,
	TEMPLATE_ROAD32,
	TEMPLATE_ROAD33,
	TEMPLATE_ROAD34,
	TEMPLATE_ROAD35,
	TEMPLATE_ROAD36,
	TEMPLATE_ROAD37,
	TEMPLATE_ROAD38,
	TEMPLATE_ROAD39,
	TEMPLATE_ROAD40,
	TEMPLATE_ROAD41,
	TEMPLATE_ROAD42,
	TEMPLATE_ROAD43,

	TEMPLATE_RIVER1,
	TEMPLATE_RIVER2,
	TEMPLATE_RIVER3,
	TEMPLATE_RIVER4,
	TEMPLATE_RIVER5,
	TEMPLATE_RIVER6,
	TEMPLATE_RIVER7,
	TEMPLATE_RIVER8,
	TEMPLATE_RIVER9,
	TEMPLATE_RIVER10,
	TEMPLATE_RIVER11,
	TEMPLATE_RIVER12,
	TEMPLATE_RIVER13,

	TEMPLATE_RIVER14,
	TEMPLATE_RIVER15,
	TEMPLATE_RIVER16,
	TEMPLATE_RIVER17,
	TEMPLATE_RIVER18,
	TEMPLATE_RIVER19,
	TEMPLATE_RIVER20,
	TEMPLATE_RIVER21,
	TEMPLATE_RIVER22,
	TEMPLATE_RIVER23,
	TEMPLATE_RIVER24,
	TEMPLATE_RIVER25,
	TEMPLATE_FORD1,
	TEMPLATE_FORD2,
	TEMPLATE_FALLS1,
	TEMPLATE_FALLS2,

	TEMPLATE_BRIDGE1,
	TEMPLATE_BRIDGE1D,
	TEMPLATE_BRIDGE2,
	TEMPLATE_BRIDGE2D,
	TEMPLATE_BRIDGE3,
	TEMPLATE_BRIDGE3D,
	TEMPLATE_BRIDGE4,
	TEMPLATE_BRIDGE4D,

	TEMPLATE_SHORE24,
	TEMPLATE_SHORE25,
	TEMPLATE_SHORE26,
	TEMPLATE_SHORE27,
	TEMPLATE_SHORE28,
	TEMPLATE_SHORE29,
	TEMPLATE_SHORE30,
	TEMPLATE_SHORE31,

	TEMPLATE_PATCH16,
	TEMPLATE_PATCH17,
	TEMPLATE_PATCH18,
	TEMPLATE_PATCH19,
	TEMPLATE_PATCH20,

	TEMPLATE_SHORE34,
	TEMPLATE_SHORE35,
	TEMPLATE_SHORE36,
	TEMPLATE_SHORE37,
	TEMPLATE_SHORE38,
	TEMPLATE_SHORE39,
	TEMPLATE_SHORE40,
	TEMPLATE_SHORE41,
	TEMPLATE_SHORE42,
	TEMPLATE_SHORE43,
	TEMPLATE_SHORE44,
	TEMPLATE_SHORE45,

	TEMPLATE_SHORE46,
	TEMPLATE_SHORE47,
	TEMPLATE_SHORE48,
	TEMPLATE_SHORE49,
	TEMPLATE_SHORE50,
	TEMPLATE_SHORE51,
	TEMPLATE_SHORE52,
	TEMPLATE_SHORE53,
	TEMPLATE_SHORE54,
	TEMPLATE_SHORE55,
	TEMPLATE_SHORE56,
	TEMPLATE_SHORE57,
	TEMPLATE_SHORE58,
	TEMPLATE_SHORE59,
	TEMPLATE_SHORE60,
	TEMPLATE_SHORE61,

	TEMPLATE_SHORE62,
	TEMPLATE_SHORE63,

	TEMPLATE_COUNT,
	TEMPLATE_NONE=255,
	TEMPLATE_FIRST=0
} TemplateType;

//inline TemplateType operator++(TemplateType &, int);


/**********************************************************************
**	The three dimensional terrain objects are enumerated here. These
**	objects function similar to buildings in that they can be driven
**	behind and can take damage on an individual basis.
*/
typedef enum TerrainType : char {
	TERRAIN_NONE=-1,
	TERRAIN_TREE1,
	TERRAIN_TREE2,
	TERRAIN_TREE3,
	TERRAIN_TREE4,
	TERRAIN_TREE5,
	TERRAIN_TREE6,
	TERRAIN_TREE7,
	TERRAIN_TREE8,
	TERRAIN_TREE9,
	TERRAIN_TREE10,
	TERRAIN_TREE11,
	TERRAIN_TREE12,
	TERRAIN_TREE13,
	TERRAIN_TREE14,
	TERRAIN_TREE15,
	TERRAIN_TREE16,
	TERRAIN_TREE17,
	TERRAIN_TREE18,
	TERRAIN_BLOSSOMTREE1,
	TERRAIN_BLOSSOMTREE2,
	TERRAIN_CLUMP1,
	TERRAIN_CLUMP2,
	TERRAIN_CLUMP3,
	TERRAIN_CLUMP4,
	TERRAIN_CLUMP5,
	TERRAIN_ROCK1,
	TERRAIN_ROCK2,
	TERRAIN_ROCK3,
	TERRAIN_ROCK4,
	TERRAIN_ROCK5,
	TERRAIN_ROCK6,
	TERRAIN_ROCK7,

	TERRAIN_COUNT,
	TERRAIN_FIRST=0
} TerrainType;

//inline TerrainType operator++(TerrainType &, int);


/**********************************************************************
**	Smudges are enumerated here. Smudges are transparent icons that are
**	drawn over the underlying terrain in order to give the effect of
**	alterations to the terrin. Craters are a good example of this.
*/
typedef enum SmudgeType : char {
	SMUDGE_NONE=-1,
	SMUDGE_CRATER1,
	SMUDGE_CRATER2,
	SMUDGE_CRATER3,
	SMUDGE_CRATER4,
	SMUDGE_CRATER5,
	SMUDGE_CRATER6,
	SMUDGE_SCORCH1,
	SMUDGE_SCORCH2,
	SMUDGE_SCORCH3,
	SMUDGE_SCORCH4,
	SMUDGE_SCORCH5,
	SMUDGE_SCORCH6,
	SMUDGE_BIB1,
	SMUDGE_BIB2,
	SMUDGE_BIB3,

	SMUDGE_COUNT,
	SMUDGE_FIRST=0
} SmudgeType;

//inline SmudgeType operator++(SmudgeType &, int);


/**********************************************************************
**	Animations are enumerated here. Animations are the high speed and
**	short lived effects that occur with explosions and fire.
*/
typedef enum AnimType : char {
	ANIM_NONE=-1,
	ANIM_FBALL1=0,					// Large fireball explosion (bulges rightward).
	ANIM_GRENADE,					// Genade (dirt type) explosion.
	ANIM_FRAG1,						// Medium fragment throwing explosion -- short decay.
	ANIM_FRAG2,						// Medium fragment throwing explosion -- long decay.
	ANIM_VEH_HIT1,					//	Small fireball explosion (bulges rightward).
	ANIM_VEH_HIT2,					//	Small fragment throwing explosion -- pop & sparkles.
	ANIM_VEH_HIT3,					// Small fragment throwing explosion -- burn/exp mix.
	ANIM_ART_EXP1,					// Large fragment throwing explosion -- many sparkles.
	ANIM_NAPALM1,					// Small napalm burn.
	ANIM_NAPALM2,					// Medium napalm burn.
	ANIM_NAPALM3,					// Large napalm burn.
	ANIM_SMOKE_PUFF,				// Small rocket smoke trail puff.
	ANIM_PIFF,						// Machine gun impact piffs.
	ANIM_PIFFPIFF,					// Chaingun impact piffs.
	ANIM_FLAME_N,					// Flame thrower firing north.
	ANIM_FLAME_NE,					//	Flame thrower firing north east.
	ANIM_FLAME_E,					// Flame thrower firing east.
	ANIM_FLAME_SE,					// Flame thrower firing south east.
	ANIM_FLAME_S,					// Flame thrower firing south.
	ANIM_FLAME_SW,					// Flame thrower firing south west.
	ANIM_FLAME_W,					// Flame thrower firing west.
	ANIM_FLAME_NW,					// Flame thrower firing north west.
	ANIM_CHEM_N,					// Chem sprayer firing north.
	ANIM_CHEM_NE,					//	Chem sprayer firing north east.
	ANIM_CHEM_E,					// Chem sprayer firing east.
	ANIM_CHEM_SE,					// Chem sprayer firing south east.
	ANIM_CHEM_S,					// Chem sprayer firing south.
	ANIM_CHEM_SW,					// Chem sprayer firing south west.
	ANIM_CHEM_W,					// Chem sprayer firing west.
	ANIM_CHEM_NW,					// Chem sprayer firing north west.
	ANIM_FIRE_SMALL,				// Small flame animation.
	ANIM_FIRE_MED,					// Medium flame animation.
	ANIM_FIRE_MED2,				// Medium flame animation (oranger).
	ANIM_FIRE_TINY,				// Very tiny flames.
	ANIM_MUZZLE_FLASH,			// Big cannon flash (with translucency).
#ifdef NEVER
	ANIM_E1_ROT_FIRE,				// Infantry decay animations.
	ANIM_E1_ROT_GRENADE,
	ANIM_E1_ROT_GUN,
	ANIM_E1_ROT_EXP,
	ANIM_E2_ROT_FIRE,
	ANIM_E2_ROT_GRENADE,
	ANIM_E2_ROT_GUN,
	ANIM_E2_ROT_EXP,
	ANIM_E3_ROT_FIRE,
	ANIM_E3_ROT_GRENADE,
	ANIM_E3_ROT_GUN,
	ANIM_E3_ROT_EXP,
	ANIM_E4_ROT_FIRE,
	ANIM_E4_ROT_GRENADE,
	ANIM_E4_ROT_GUN,
	ANIM_E4_ROT_EXP,
#endif
	ANIM_SMOKE_M,					// Smoke rising from ground.
	ANIM_BURN_SMALL,				// Small combustable fire effect (with trail off).
	ANIM_BURN_MED,					// Medium combustable fire effect (with trail off).
	ANIM_BURN_BIG,					// Large combustable fire effect (with trail off).
	ANIM_ON_FIRE_SMALL,			// Burning effect for buildings.
	ANIM_ON_FIRE_MED,				// Burning effect for buildings.
	ANIM_ON_FIRE_BIG,				// Burning effect for buildings.
	ANIM_SAM_N,
	ANIM_SAM_NE,
	ANIM_SAM_E,
	ANIM_SAM_SE,
	ANIM_SAM_S,
	ANIM_SAM_SW,
	ANIM_SAM_W,
	ANIM_SAM_NW,
	ANIM_GUN_N,
	ANIM_GUN_NE,
	ANIM_GUN_E,
	ANIM_GUN_SE,
	ANIM_GUN_S,
	ANIM_GUN_SW,
	ANIM_GUN_W,
	ANIM_GUN_NW,
	ANIM_LZ_SMOKE,
	ANIM_ION_CANNON,
	ANIM_ATOM_BLAST,
	ANIM_CRATE_DEVIATOR,			// Red finned missile.
	ANIM_CRATE_DOLLAR,			// Dollar sign.
	ANIM_CRATE_EARTH,				// Cracked Earth.
	ANIM_CRATE_EMPULSE,			// Plasma ball.
	ANIM_CRATE_INVUN,				// Orange sphere with green rings.
	ANIM_CRATE_MINE,				// Spiked mine.
	ANIM_CRATE_RAPID,				// Red skull.
	ANIM_CRATE_STEALTH,			// Cloaking sphere.
	ANIM_CRATE_MISSILE,			// Green finned missile.
	ANIM_ATOM_DOOR,
	ANIM_MOVE_FLASH,
	ANIM_OILFIELD_BURN,
	ANIM_TRIC_DIE,
	ANIM_TREX_DIE,
	ANIM_STEG_DIE,
	ANIM_RAPT_DIE,
	ANIM_CHEM_BALL,				// Chemical warrior explosion.

	ANIM_FIRE_SMALL_VIRTUAL,	// Small flame animation (virtual).
	ANIM_FIRE_MED_VIRTUAL,		// Medium flame animation (virtual).
	ANIM_FIRE_MED2_VIRTUAL,		// Medium flame animation (oranger) (virtual).
	ANIM_FIRE_TINY_VIRTUAL,		// Very tiny flames (virtual).

	ANIM_COUNT,
	ANIM_FIRST=0
} AnimType;

//inline AnimType operator++(AnimType &, int);


/****************************************************************************
**	Infantry can be performing various activities. These can range from simple
**	idle animations to physical hand to hand combat.
*/
typedef enum DoType : char {
	DO_NOTHING=-1,				// Not performing any choreographed sequence.
	DO_STAND_READY=0,
	DO_STAND_GUARD,
	DO_PRONE,
	DO_WALK,
	DO_FIRE_WEAPON,
	DO_LIE_DOWN,
	DO_CRAWL,
	DO_GET_UP,
	DO_FIRE_PRONE,
	DO_IDLE1,
	DO_IDLE2,
	DO_ON_GUARD,
	DO_FIGHT_READY,
	DO_PUNCH,
	DO_KICK,
	DO_PUNCH_HIT1,
	DO_PUNCH_HIT2,
	DO_PUNCH_DEATH,
	DO_KICK_HIT1,
	DO_KICK_HIT2,
	DO_KICK_DEATH,
	DO_READY_WEAPON,
	DO_GUN_DEATH,
	DO_EXPLOSION_DEATH,
	DO_EXPLOSION2_DEATH,
	DO_GRENADE_DEATH,
	DO_FIRE_DEATH,
	DO_GESTURE1,
	DO_SALUTE1,
	DO_GESTURE2,
	DO_SALUTE2,
// Civilian actions
	DO_PULL_GUN,
	DO_PLEAD,
	DO_PLEAD_DEATH,

	DO_COUNT,
	DO_FIRST=0
} DoType;

//inline DoType operator++(DoType &, int);

/*
**	This structure is associated with each maneuver type. It tells whether the
**	maneuver can be interrupted and the frame rate.
*/
typedef struct {
	unsigned Interrupt:1;	// Can it be interrupted?
	unsigned IsMobile:1;		// Can it move while doing this?
	unsigned RandomStart:1;	// Should animation be "randomized"?
	unsigned char Rate;		// Frame rate.
} DoStruct;

typedef struct {
	int				Frame;	// Starting frame of the animation.
	unsigned char	Count;	// Number of frames of animation.
	unsigned char	Jump;		// Frames to jump between facings.
} DoInfoStruct;


/****************************************************************************
**	These are the various radio message that can be transmitted between
**	units and buildings. Some of these require a response from the receiver
**	and some don't.
*/
typedef enum RadioMessageType : unsigned char {
	RADIO_STATIC,				// "hisssss" -- non-message
	RADIO_ROGER,				// "Roger."
	RADIO_HELLO,				// "Come in. I wish to talk."
	RADIO_OVER_OUT,			// "Something came up, bye."
	RADIO_PICK_UP,				// "Please pick me up."
	RADIO_ATTACH,				// "Attach to transport."
	RADIO_DELIVERY,			// "I've got a delivery for you."
	RADIO_HOLD_STILL,			// "I'm performing load/unload maneuver. Be careful."
	RADIO_UNLOADED,			// "I'm clear."
	RADIO_UNLOAD,				// "You are clear to unload. Please start driving off now."
	RADIO_NEGATIVE,			// "Am unable to comply."
	RADIO_BUILDING,			// "I'm starting construction now... act busy."
	RADIO_COMPLETE,			// "I've finished construction. You are free."
	RADIO_REDRAW,				// "Oops, sorry. I might have bumped you a little."
	RADIO_DOCKING,				// "I'm trying to load up now."
	RADIO_CAN_LOAD,			// "May I become a passenger?"
	RADIO_ARE_REFINERY,		// "Are you a refinery ready to take shipment?"
	RADIO_TRYING_TO_LOAD,	// "Are you trying to become a passenger?"
	RADIO_MOVE_HERE,			// "Move to location X."
	RADIO_NEED_TO_MOVE,		// "Do you need to move somewhere?"
	RADIO_YEA_NOW_WHAT,		// "All right already. Now what?"
	RADIO_IM_IN,				// "I'm a passenger now."
	RADIO_BACKUP_NOW,			// "Begin backup into refinery now."
	RADIO_RUN_AWAY,			// "Run away! Run away!"
	RADIO_TETHER,				// "Establish tether contact."
	RADIO_UNTETHER,			// "Break tether contact."
	RADIO_REPAIR,				// "Repair one step."
	RADIO_PREPARED,			// "Are you prepared to fight?"
	RADIO_ATTACK_THIS,		// "Attack this target please."
	RADIO_RELOAD,				// "Reload one step please."

	// Hand to hand combat messages (yelled rather than radioed).
	RADIO_KICK,				// "Take this kick, you.. you.."
	RADIO_PUNCH,			// "Take this punch, you.. you.."
	RADIO_PREPARE_TO_BOX,//	"Fancy a little fisticuffs, eh?"

	RADIO_COUNT
} RadioMessageType;


/****************************************************************************
**	These are custom C&C specific types. The CELL is used for map coordinate
**	with cell resolution. The COORD type is used for map coordinates that
**	have a lepton resolution.
*/
typedef unsigned long	COORDINATE;
typedef signed short		CELL;

typedef unsigned short	TARGET;
#define	TARGET_NONE 	((TARGET)0)


/****************************************************************************
**	Selected units have a special selected unit box around them. These are the
**	defines for the two types of selected unit boxes. One is for infantry and
**	the other is for regular units.
*/
typedef enum SelectEnum : char {
	SELECT_NONE=-1,
	SELECT_INFANTRY,					// Small infantry selection box.
	SELECT_UNIT,						// Big unit selection box.
	SELECT_BUILDING=SELECT_UNIT,	// Custom box for buildings.
	SELECT_TERRAIN=SELECT_UNIT,	// Custom box for terrain objects.
	SELECT_WRENCH,						// A building is repairing overlay graphic.

	SELECT_COUNT
} SelectEnum;


/****************************************************************************
**	The pip shapes and text shapes are enumerated according to the following
**	type. These special shapes are drawn over special objects or in other places
**	where shape technology is needed.
*/
typedef enum PipEnum : unsigned char
{
	PIP_EMPTY,			// Empty pip spot.
	PIP_FULL,			// Full pip spot.
	PIP_PRIMARY,		// "Primary" building marker.
	PIP_READY,			// "Ready" construction information tag.
	PIP_HOLDING,		// "Hold"ing construction information tag.
	PIP_ENGINEER,		// Full pip with engineer coloring.
	PIP_CIVILIAN,		// Full pip with civilian coloring.
	PIP_COMMANDO		// Full pip with commando coloring.
} PipEnum;


/****************************************************************************
**	The radar map can be in one of several states depending on the presense of
**	certain structures and power levels.
*/
typedef enum RadarEnum : unsigned char
{
	RADAR_NONE,	// Radar map doesn't exist at all
	RADAR_OFF,	// Radar map is present, but displayed as off and non-interactive
	RADAR_ON	// Radar map is present, visible, and fully interactive
} RadarEnum;


/****************************************************************************
**	The mouse cursor can be in different states. These states are listed
**	below. Some of these represent animating mouse cursors. The mouse
**	is controlled by passing one of these values to the appropriate
**	MouseClass member function.
*/
typedef enum MouseType : char
{
	MOUSE_NORMAL,
	MOUSE_N,
	MOUSE_NE,
	MOUSE_E,
	MOUSE_SE,
	MOUSE_S,
	MOUSE_SW,
	MOUSE_W,
	MOUSE_NW,
	MOUSE_NO_N,
	MOUSE_NO_NE,
	MOUSE_NO_E,
	MOUSE_NO_SE,
	MOUSE_NO_S,
	MOUSE_NO_SW,
	MOUSE_NO_W,
	MOUSE_NO_NW,
	MOUSE_NO_MOVE,
	MOUSE_CAN_MOVE,
	MOUSE_ENTER,
	MOUSE_DEPLOY,
	MOUSE_CAN_SELECT,
	MOUSE_CAN_ATTACK,
	MOUSE_SELL_BACK,
	MOUSE_SELL_UNIT,
	MOUSE_REPAIR,
	MOUSE_NO_REPAIR,
	MOUSE_NO_SELL_BACK,
	MOUSE_RADAR_CURSOR,
	MOUSE_ION_CANNON,
	MOUSE_NUCLEAR_BOMB,
	MOUSE_AIR_STRIKE,
	MOUSE_DEMOLITIONS,
	MOUSE_AREA_GUARD,
	MOUSE_COUNT
} MouseType;

/**********************************************************************
**	This structure is used to control the box relief style drawn by
**	the Draw_Box() function.
*/
typedef struct {
	int	Filler;		// Center box fill color.
	int	Shadow;		// Shadow color (darker).
	int	Highlight;	// Highlight color (lighter).
	int	Corner;		// Corner color (transition).
} BoxStyleType;

typedef enum BoxStyleEnum : unsigned char {
	BOXSTYLE_DOWN,					// Typical depressed edge border.
	BOXSTYLE_RAISED,				// Typical raised edge border.
	BOXSTYLE_BLUE_UP,				// Raised blue border.
	BOXSTYLE_DIS_DOWN,			// Disabled but depressed.
	BOXSTYLE_DIS_RAISED,			// Disabled but raised.
	BOXSTYLE_RAISED_ARROW,		// Typical raised edge w/arrow around caption
	BOXSTYLE_GREEN_DOWN,			// green depressed edge border.
	BOXSTYLE_GREEN_RAISED,		// green raised edge border.
	BOXSTYLE_GREEN_DIS_DOWN,	// disabled but depressed.
	BOXSTYLE_GREEN_DIS_RAISED,	// disabled but raised.
	BOXSTYLE_GREEN_BOX,			// list box.
	BOXSTYLE_GREEN_BORDER,		// main dialog box.

	BOXSTYLE_COUNT
} BoxStyleEnum;


/**********************************************************************
**	Damage, as inflicted by projectiles, has different characteristics.
**	These are the different "warhead" types that can be assigned to the
**	various projectiles in the game.
*/
typedef enum WarheadType : char{
	WARHEAD_NONE=-1,

	WARHEAD_SA,					// Small arms -- good against infantry.
	WARHEAD_HE,					//	High explosive -- good against buildings & infantry.
	WARHEAD_AP,					// Amor piercing -- good against armor.
	WARHEAD_FIRE,				// Incendiary -- Good against flammables.
	WARHEAD_LASER,				// Light Amplification of Stimulated Emission of Radiation.
	WARHEAD_PB,					// Particle beam (neutron beam).
	WARHEAD_FIST,				// punching in hand-to-hand combat
	WARHEAD_FOOT,				// kicking in hand-to-hand combat
	WARHEAD_HOLLOW_POINT,	// Sniper bullet type.
	WARHEAD_SPORE,				// Spores from blossom tree - affect infantry only
	WARHEAD_HEADBUTT,			// Other dinosaurs butt into people
	WARHEAD_FEEDME,			// T-Rex eats people, hurts vehicles/buildings

	WARHEAD_COUNT
} WarheadType;


/**********************************************************************
**	This enumerates the various weapon types. The weapon is characterized
**	by the projectile it launches, the damage it does, and the rate of
**	fire.
*/
typedef enum WeaponType : char {
	WEAPON_NONE=-1,

	WEAPON_RIFLE,
	WEAPON_CHAIN_GUN,
	WEAPON_PISTOL,
	WEAPON_M16,
	WEAPON_DRAGON,
	WEAPON_FLAMETHROWER,
	WEAPON_FLAME_TONGUE,
	WEAPON_CHEMSPRAY,
	WEAPON_GRENADE,
	WEAPON_75MM,
	WEAPON_105MM,
	WEAPON_120MM,
	WEAPON_TURRET_GUN,
	WEAPON_MAMMOTH_TUSK,
	WEAPON_MLRS,
	WEAPON_155MM,
	WEAPON_M60MG,
	WEAPON_TOMAHAWK,
	WEAPON_TOW_TWO,
	WEAPON_NAPALM,
	WEAPON_OBELISK_LASER,
	WEAPON_NIKE,
	WEAPON_HONEST_JOHN,
	WEAPON_STEG,
	WEAPON_TREX,

#ifdef PETROGLYPH_EXAMPLE_MOD
	WEAPON_NUKE_LOB,
#endif PETROGLYPH_EXAMPLE_MOD
			
	WEAPON_COUNT
} WeaponType;


/**********************************************************************
**	The various armor types are best suited to defend against a limited
**	kind of warheads. The game strategy revolves around proper
**	combination of armor and weaponry. Each vehicle or building has armor
**	rated according to one of the following types.
*/
typedef enum ArmorType : unsigned char {
	ARMOR_NONE,			// Vulnerable to SA and HE.
	ARMOR_WOOD,			// Vulnerable to HE and Fire.
	ARMOR_ALUMINUM,	// Vulnerable to AP and SA.
	ARMOR_STEEL,		// Vulnerable to AP.
	ARMOR_CONCRETE,	// Vulnerable to HE and AP.

	ARMOR_COUNT
} ArmorType;


/**********************************************************************
**	Working MCGA colors that give a pleasing effect for beveled edges and
**	other purposes.
*/
#define	MAGIC_COL_COUNT	12		// Translucent color count.
#define	SHADOW_COL_COUNT	4		// Terrain shroud translucent color count.
#define	USHADOW_COL_COUNT	1		// Unit shadow special ghost colors.


/**********************************************************************
**	Color cycling range that is used for water effects.
*/
#define	CYCLE_COLOR_START		32
#define	CYCLE_COLOR_COUNT		7


/**********************************************************************
**	Magic color fading pulsing effect limts -- color gun value.
*/
#define	MIN_CYCLE_COLOR		16
#define	MAX_CYCLE_COLOR		63


/**********************************************************************
**	The game's six and eight point fonts have these names.
*/
#define	FONT3		"3point.fnt"
#define	FONT6		"6point.fnt"
#define	FONT8		"8point.fnt"


/**********************************************************************
**	These are the control flags for Fancy_Text_Print function.
*/
typedef enum TextPrintType : unsigned short {
	TPF_LASTPOINT		=0x0000,		// Use previous font point value.
	TPF_6POINT			=0x0001,		// Use 6 point font.
	TPF_8POINT			=0x0002,		// Use 8 point font.
	TPF_3POINT			=0x0003,		// Use 3 point font.
	TPF_LED				=0x0004,		// Use LED font.
	TPF_VCR				=0x0005,		// Use VCR font.
	TPF_6PT_GRAD		=0x0006,		// Use 6 point gradient font.
	TPF_MAP				=0x0007,		// Use 6 point gradient font.
	TPF_GREEN12			=0x0008,		// Use green tab font
	TPF_GREEN12_GRAD	=0x0009,		// Use graduated green tab font
	TPF_LASTSHADOW		=0x0000,		// Use previous font palette.
	TPF_NOSHADOW		=0x0010,		// Don't have any shadow.
	TPF_DROPSHADOW		=0x0020,		//	Use a simple drop shadow.
	TPF_FULLSHADOW		=0x0040,		// Use a full outline shadow.
	TPF_LIGHTSHADOW	=0x0080,		// Use engraved drop 'shadow' color.
	TPF_CENTER			=0x0100,		// Center about the X axis.
	TPF_RIGHT			=0x0200,		// Right justify text.
	TPF_MEDIUM_COLOR	=0x1000,		// Use medium color for all text gradient
	TPF_BRIGHT_COLOR	=0x2000,		// Use bright color for all text gradient
	TPF_USE_GRAD_PAL	=0x4000		// Use a gradient palette based on fore color
} TextPrintType;

//inline TextPrintType operator |(TextPrintType, TextPrintType);
//inline TextPrintType operator &(TextPrintType, TextPrintType);
//inline TextPrintType operator ~(TextPrintType);

inline TextPrintType operator|(TextPrintType a, TextPrintType b)
{return static_cast<TextPrintType>(static_cast<int>(a) | static_cast<int>(b));}

inline TextPrintType operator&(TextPrintType a, TextPrintType b)
{return static_cast<TextPrintType>(static_cast<int>(a) & static_cast<int>(b));}

inline TextPrintType operator~(TextPrintType a)
{return static_cast<TextPrintType>(~static_cast<int>(a));}


/**********************************************************************
**	These control the maximum number of objects in the game. Make sure that these
**	maximums never exceed the maximum value for the "ID" element in the
**	object class.
*/
#define	AIRCRAFT_MAX			100			// Lasts for minutes.
#define	ANIM_MAX					200			// Lasts only a few seconds.
#define	BUILDING_MAX			500		// Lasts for hours.
#define	BULLET_MAX				50			// Lasts several seconds.
#define	FACTORY_MAX				32			// Lasts a few minutes.
#define	HOUSE_MAX				12			// Lasts entire scenario.
#define	INFANTRY_MAX			500		// Lasts for minutes.
#define	OVERLAY_MAX				1			// Very transitory.
#define	REINFORCEMENT_MAX		50			// Maximum number of reinforcements.
#define	SMUDGE_MAX				1			// Very transitory.
#define	TEAM_MAX					60			// Lasts forever.
#define	TEMPLATE_MAX			1			// Very transitory.
#define	TERRAIN_MAX				500		// Lasts for hours or eternity.
#define	TRIGGER_MAX				80			// Lasts forever.
#define	UNIT_MAX					500		// Lasts for minutes.
#define	TEAMTYPE_MAX			60			// Lasts forever.

// Save filename description.
#define	DESCRIP_MAX				44			// 40 chars + CR + LF + CTRL-Z + NULL

#define	MAX_ENTRY_SIZE 		15

#define  OBUTTON_HEIGHT       9			// Is defined in mapedit.h, need for buttons

#define	CONQUER_PATH_MAX		9			// Number of cells to look ahead for movement.

#define	EACH_UNIT_MAX			(UNIT_MAX/4)			// Default maximum any one player can have.
#define	EACH_BUILDING_MAX		(BUILDING_MAX/4)		// Default maximum any one player can build.


/**********************************************************************
**	Terrain can be of these different classes. At any point in the game
**	a particular piece of ground must fall under one of these classifications.
**	This is true, even if it is undergoing a temporary transition.
*/
typedef enum LandType : unsigned char {
	LAND_CLEAR,		// "Clear" terrain.
	LAND_ROAD,		// Road terrain.
	LAND_WATER,		// Water.
	LAND_ROCK,		// Impassable rock.
	LAND_WALL,		// Wall (blocks movement).
	LAND_TIBERIUM,	// Tiberium field.
	LAND_BEACH,		//	Beach terrain.

	LAND_COUNT
} LandType;


/**********************************************************************
**	The theaters of operation are as follows.
*/
typedef enum TheaterType : char {
	THEATER_NONE=-1,
	THEATER_DESERT,
	THEATER_JUNGLE,
	THEATER_TEMPERATE,
	THEATER_WINTER,

	THEATER_COUNT,
	THEATER_FIRST=0
} TheaterType;

//inline TheaterType operator++(TheaterType &, int);

#define	THEATERF_DESERT			(1<<THEATER_DESERT)
#define	THEATERF_JUNGLE			(1<<THEATER_JUNGLE)
#define	THEATERF_TEMPERATE		(1<<THEATER_TEMPERATE)
#define	THEATERF_WINTER			(1<<THEATER_WINTER)

typedef struct {
	char		Name[16];
	char		Root[10];
	char		Suffix[4];
} TheaterDataType;


/**********************************************************************
**	Each building has a predetermined size. These are the size numbers.
**	The trailing number is this define is the width and height (respectively)
**	of the building in cells.
*/
typedef enum BSizeType : char {
	BSIZE_NONE=-1,
	BSIZE_11=0,
	BSIZE_21,
	BSIZE_12,
	BSIZE_22,
	BSIZE_23,
	BSIZE_32,
	BSIZE_33,
	BSIZE_42,
	BSIZE_55,

	BSIZE_COUNT
} BSizeType;

//inline BSizeType operator++(BSizeType &, int);


/**********************************************************************
** When objects are manipulated on the map that are marked as being
**	removed (up), placed down (down), or just to be redrawn (change);
** or when an object's rendering (not logical) size changes, due to
** its being selected or having an animation attached (overlap up/down).
*/
typedef enum MarkType : unsigned char {
	MARK_UP,					//	Removed from the map.
	MARK_DOWN,				//	Placed on the map.
	MARK_CHANGE,			//	Altered in place on the map.
	MARK_OVERLAP_DOWN,	// Mark overlap cells on the map
	MARK_OVERLAP_UP,		// Clear overlap cells on the map
} MarkType;


/****************************************************************************
**	Window number definition list. Each window should be referred to by
**	the value given in this list.
*/
// Allow window number enums to be passed to library functions.
typedef enum WindowNumberType : unsigned char {
	WINDOW_MAIN,				// Full screen window.
	WINDOW_ERROR,				// Library error window.
	WINDOW_TACTICAL,			// Tactical map window.
	WINDOW_MENU,				// Main selection menu.
	WINDOW_SIDEBAR,			// Sidebar (buildable list) window.
	WINDOW_EDITOR,				// Scenario editor window.
	WINDOW_CUSTOM,				// Window that can be altered depending on circumstances
	WINDOW_VIRTUAL				// Window that is used when we want to capture rendering information without actually rendering. ST - 1/15/2019 3:03PM
} WindowNumberType;


/****************************************************************************
**	For every cell there are 8 adjacent cells. Use these direction numbers
**	when referring to adjacent cells. This comes into play when moving
**	between cells and in the Desired_Facing() algorithm.
*/
typedef enum FacingType : char {
	FACING_NONE=-1,
	FACING_N,			// North
	FACING_NE,			// North-East
	FACING_E,			// East
	FACING_SE,			// South-East
	FACING_S,			// South
	FACING_SW,			// South-West
	FACING_W,			// West
	FACING_NW,			// North-West

	FACING_COUNT,			// Total of 8 directions (0..7).
	FACING_FIRST=0
} FacingType;

//inline FacingType operator++(FacingType &, int);
inline FacingType operator + (FacingType f1, FacingType f2)
{
	return (FacingType)(((int)f1 + (int)f2) & 0x07);
}
inline FacingType operator + (FacingType f1, int f2)
{
	return (FacingType)(((int)f1 + (int)f2) & 0x07);
}

inline FacingType operator - (FacingType f1, FacingType f2)
{
	return (FacingType)(((int)f1 - (int)f2) & 0x07);
}
inline FacingType operator - (FacingType f1, int f2)
{
	return (FacingType)(((int)f1 - (int)f2) & 0x07);
}

inline FacingType operator += (FacingType & f1, FacingType f2)
{
	f1 = (FacingType)(((int)f1 + (int)f2) & 0x07);
	return(f1);
}
inline FacingType operator += (FacingType & f1, int f2)
{
	f1 = (FacingType)(((int)f1 + (int)f2) & 0x07);
	return(f1);
}


typedef enum DirType : unsigned char {
	DIR_MIN=0,
	DIR_N=0,
	DIR_NE=1<<5,
	DIR_E=2<<5,
	DIR_SE=3<<5,
	DIR_S=4<<5,
	DIR_SW=5<<5,
	DIR_SW_X1=(5<<5)-8,		// Direction of harvester while unloading.
	DIR_SW_X2=(5<<5)-16,		// Direction of harvester while unloading.
	DIR_W=6<<5,
	DIR_NW=7<<5,
	DIR_MAX=254
} DirType;
inline DirType operator + (DirType f1, DirType f2)
{
	return (DirType)(((int)f1 + (int)f2) & 0x00FF);
}
inline DirType operator + (DirType f1, int f2)
{
	return (DirType)(((int)f1 + (int)f2) & 0x00FF);
}


/****************************************************************************
**	Timer constants. These are used when setting the countdown timer.
**	Note that this is based upon a timer that ticks every 60th of a second.
*/
#define	TICKS_PER_SECOND		15
#define	TICKS_PER_MINUTE		(TICKS_PER_SECOND * 60)

#define	TIMER_SECOND			60
#define	TIMER_MINUTE			(TIMER_SECOND*60)

#define	FADE_PALETTE_FAST		(TIMER_SECOND/8)
#define	FADE_PALETTE_MEDIUM	(TIMER_SECOND/4)
#define	FADE_PALETTE_SLOW		(TIMER_SECOND/2)


/****************************************************************************
** Each vehicle is give a speed rating. This is a combination of not only
**	its physical speed, but the means by which it travels (wheels, tracks,
**	wings, etc). This is used to determine the movement table.
*/
typedef enum SpeedType : char {
	SPEED_NONE=-1,

	SPEED_FOOT=0,					// Bipedal.
	SPEED_TRACK,				// Tracked locomotion.
	SPEED_HARVESTER,			// Harvester speed rating.
	SPEED_WHEEL,				// Balloon tires.
	SPEED_WINGED,				// Lifter's, 'thopters, and rockets.
	SPEED_HOVER,				// Typical hovercraft logic.
	SPEED_FLOAT,				// Ships.

	SPEED_COUNT,
	SPEED_FIRST=SPEED_FOOT
} SpeedType;


/**********************************************************************
**	These are the sound effect digitized sample file names.
*/
typedef enum VocType : char{
	VOC_NONE=-1,

	VOC_RAMBO_PRESENT,//	"I've got a present for	ya"
	VOC_RAMBO_CMON,	//	"c'mon"
	VOC_RAMBO_UGOTIT,	//	"you got it"
	VOC_RAMBO_COMIN,	//	"keep 'em commin'"
	VOC_RAMBO_LAUGH,	//	"hahaha"
	VOC_RAMBO_LEFTY,	//	"that was left handed"
	VOC_RAMBO_NOPROB,	//	"no problem"
//	VOC_RAMBO_OHSH,	//	"oh shiiiiii...."
	VOC_RAMBO_ONIT,	//	"I'm on it"
	VOC_RAMBO_YELL,	//	"ahhhhhhh"
	VOC_RAMBO_ROCK,	//	"time to rock and roll"
	VOC_RAMBO_TUFF,	//	"real tuff guy"
	VOC_RAMBO_YEA,		//	"yea"
	VOC_RAMBO_YES,		//	"yes"
	VOC_RAMBO_YO,		//	"yo"

	VOC_GIRL_OKAY,		// "okay"
	VOC_GIRL_YEAH,		// "yeah?"
	VOC_GUY_OKAY,		//	"okay"
	VOC_GUY_YEAH,		// "yeah?"

	VOC_2DANGER,		//	"negative, too dangerous"
	VOC_ACKNOWL,		//	"acknowledged"
	VOC_AFFIRM,			//	"affirmative"
	VOC_AWAIT,			//	"awaiting orders"
//	VOC_BACKUP,			//	"send backup"
//	VOC_HELP,			//	"send help"
	VOC_MOVEOUT,		//	"movin' out"
	VOC_NEGATIVE,		//	"negative"
	VOC_NO_PROB,		//	"not a problem"
	VOC_READY,			//	"ready and waiting"
	VOC_REPORT,			//	"reporting"
	VOC_RIGHT_AWAY,	//	"right away sir"
	VOC_ROGER,			//	"roger"
//	VOC_SIR,				//	"sir?"
//	VOC_SQUAD,			//	"squad reporting"
//	VOC_PRACTICE,		//	"target practice"
	VOC_UGOTIT,			//	"you got it"
	VOC_UNIT,			//	"unit reporting"
	VOC_VEHIC,			//	"vehicle reporting"
	VOC_YESSIR,			//	"yes sir"

	VOC_BAZOOKA,		//	Gunfire
	VOC_BLEEP,			//	Clean metal bing
	VOC_BOMB1,			//	Crunchy parachute bomb type explosion
	VOC_BUTTON,			//	Dungeon Master button click
	VOC_RADAR_ON,		//	Elecronic static with beeps
	VOC_CONSTRUCTION,	//	construction sounds
	VOC_CRUMBLE,		//	muffled crumble sound
	VOC_FLAMER1,		//	flame thrower
	VOC_RIFLE,			//	rifle shot
	VOC_M60,				//	machine gun burst -- 6 rounds
	VOC_GUN20,			//	bat hitting heavy metal door
	VOC_M60A,			//	medium machine gun burst
	VOC_MINI,			//	mini gun burst
	VOC_RELOAD,			//	gun clip reload
	VOC_SLAM,			//	metal plates slamming together
	VOC_HVYGUN10,		//	loud sharp cannon
	VOC_ION_CANNON,	//	partical beam
	VOC_MGUN11,			//	alternate tripple burst
	VOC_MGUN2,			//	M-16 tripple burst
	VOC_NUKE_FIRE,		//	long missile sound
	VOC_NUKE_EXPLODE,	//	long but not loud explosion
	VOC_LASER,			//	humming laser beam
	VOC_LASER_POWER,	//	warming up sound of laser beam
	VOC_RADAR_OFF,		//	doom door slide
	VOC_SNIPER,			//	silenced rifle fire
	VOC_ROCKET1,		//	rocket launch variation #1
	VOC_ROCKET2,		//	rocket launch variation #2
	VOC_MOTOR,			//	dentists drill
	VOC_SCOLD,			//	cannot perform action feedback tone
	VOC_SIDEBAR_OPEN,	//	xylophone clink
	VOC_SIDEBAR_CLOSE,//	xylophone clink
	VOC_SQUISH2,		//	crushing infantry
	VOC_TANK1,			//	sharp tank fire with recoil
	VOC_TANK2,			//	sharp tank fire
	VOC_TANK3,			//	sharp tank fire
	VOC_TANK4,			//	big gun tank fire
	VOC_UP,				//	credits counting up
	VOC_DOWN,			//	credits counting down
	VOC_TARGET,			//	target sound
	VOC_SONAR,			//	sonar echo
	VOC_TOSS,			//	air swish
	VOC_CLOAK,			//	stealth tank
	VOC_BURN,			//	burning crackle
	VOC_TURRET,			//	muffled gunfire
	VOC_XPLOBIG4,		//	very long muffled explosion
	VOC_XPLOBIG6,		//	very long muffled explosion
	VOC_XPLOBIG7,		//	very long muffled explosion
	VOC_XPLODE,			//	long soft muffled explosion
	VOC_XPLOS,			//	short crunchy explosion
	VOC_XPLOSML2,		//	muffled mechanical explosion

	VOC_SCREAM1,		//	short infantry scream
	VOC_SCREAM3,		//	short infantry scream
	VOC_SCREAM4,		//	short infantry scream
	VOC_SCREAM5,		//	short infantry scream
	VOC_SCREAM6,		//	short infantry scream
	VOC_SCREAM7,		//	short infantry scream
	VOC_SCREAM10,		//	short infantry scream
	VOC_SCREAM11,		//	short infantry scream
	VOC_SCREAM12,		//	short infantry scream
	VOC_YELL1,			//	long infantry scream

	VOC_YES,				//	"Yes?"
	VOC_COMMANDER,		//	"Commander?"
	VOC_HELLO,			//	"Hello?"
	VOC_HMMM,			//	"Hmmm?"
//	VOC_PROCEED1,		//	"I will proceed, post haste."
//	VOC_PROCEED2,		//	"I will proceed, at once."
//	VOC_PROCEED3,		//	"I will proceed, immediately."
//	VOC_EXCELLENT1,	//	"That is an excellent plan."
//	VOC_EXCELLENT2,	//	"Yes, that is an excellent plan."
	VOC_EXCELLENT3,	//	"A wonderful plan."
//	VOC_EXCELLENT4,	//	"Asounding plan of action commander."
//	VOC_EXCELLENT5,	//	"Remarkable contrivance."
	VOC_OF_COURSE,		//	"Of course."
	VOC_YESYES,			//	"Yes yes yes."
	VOC_QUIP1,			//	"Mind the Tiberium."
//	VOC_QUIP2,			//	"A most remarkable  Metasequoia Glyptostroboides."
	VOC_THANKS,			//	"Thank you."

	VOC_CASHTURN,		// Sound of money being piled up.
	VOC_BLEEPY3,		// Clean computer bleep sound.

	VOC_DINOMOUT,		// Movin' out in dino-speak.
	VOC_DINOYES,		// Yes Sir in dino-speak.
	VOC_DINOATK1,		// Dino attack sound.
	VOC_DINODIE1,		// Dino die sound.

#ifdef PETROGLYPH_EXAMPLE_MOD
	VOC_NUKE_LOB,		// Modded unit firing sound
#endif
			
	VOC_COUNT,
	VOC_BUILD_SELECT=VOC_TARGET,
	VOC_FIRST=0
} VocType;


typedef enum VoxType : char{
	VOX_NONE=-1,
	VOX_ACCOMPLISHED,					//	mission accomplished
	VOX_FAIL,							//	your mission has failed
	VOX_NO_FACTORY,					//	unable to comply, building in progress
	VOX_CONSTRUCTION,					//	construction complete
	VOX_UNIT_READY,					// unit ready
	VOX_NEW_CONSTRUCT,				//	new construction options
	VOX_DEPLOY,							//	cannot deploy here
	VOX_DEAD_GDI,						//	GDI unit destroyed
	VOX_DEAD_NOD,						//	Nod unit destroyed
	VOX_DEAD_CIV,						//	civilian killed
//	VOX_AFFIRMATIVE,					//	affirmative
//	VOX_NEGATIVE,						//	negative
//	VOX_UPGRADE_UNIT,					//	upgrade complete, new unit available
//	VOX_UPGRADE_STRUCT,				//	upgrade complete, new structure available
	VOX_NO_CASH,						//	insufficient funds
	VOX_CONTROL_EXIT,					//	battle control terminated
	VOX_REINFORCEMENTS,				//	reinforcements have arrived
	VOX_CANCELED,						//	canceled
	VOX_BUILDING,						//	building
	VOX_LOW_POWER,						//	low power
	VOX_NO_POWER,						//	insufficient power
	VOX_NEED_MO_MONEY,				//	need more funds
	VOX_BASE_UNDER_ATTACK,			//	our base is under attack
	VOX_INCOMING_MISSILE,			//	incoming missile
	VOX_ENEMY_PLANES,					//	enemy planes approaching
	VOX_INCOMING_NUKE,				//	nuclear warhead approaching - "NUKE1"
//	VOX_RADIATION_OK,					//	radiation levels are acceptable
//	VOX_RADIATION_FATAL,				//	radiation levels are fatal
	VOX_UNABLE_TO_BUILD,				//	unable to build more
	VOX_PRIMARY_SELECTED,			//	primary building selected
//	VOX_REPAIRS_COMPLETE,			//	repairs completed
	VOX_NOD_CAPTURED,					//	Nod building captured
	VOX_GDI_CAPTURED,					//	GDI building captured
//	VOX_STRUCTURE_SOLD,				//	structure sold
	VOX_ION_CHARGING,					//	ion cannon charging
	VOX_ION_READY,						//	ion cannon ready
	VOX_NUKE_AVAILABLE,				//	nuclear weapon available
	VOX_NUKE_LAUNCHED,				//	nuclear weapon launched - "NUKLNCH1"
	VOX_UNIT_LOST,						//	unit lost
	VOX_STRUCTURE_LOST,				// structure lost
	VOX_NEED_HARVESTER,				// need harvester
	VOX_SELECT_TARGET,				// select target
	VOX_AIRSTRIKE_READY,				// airstrike ready
	VOX_NOT_READY,						//	not ready
	VOX_TRANSPORT_SIGHTED,			// Nod transport sighted
	VOX_TRANSPORT_LOADED,			// Nod transport loaded
	VOX_PREPARE,						//	enemy approaching
	VOX_NEED_MO_CAPACITY,			//	silos needed
	VOX_SUSPENDED,						//	on hold
	VOX_REPAIRING,						//	repairing
	VOX_ENEMY_STRUCTURE,				//	enemy structure destroyed
	VOX_GDI_STRUCTURE,				//	GDI structure destroyed
	VOX_NOD_STRUCTURE,				//	NOD structure destroyed
	VOX_ENEMY_UNIT,					// Enemy unit destroyed.
//	VOX_GOLD_UNIT,						//	gold unit destroyed
//	VOX_GOLD_STRUCTURE,				//	gold structure destroyed
//	VOX_GOLD_ONLINE,					//	gold player online
//	VOX_GOLD_OFFLINE,					//	gold player has departed
//	VOX_GOLD_LOST,						//	gold player destroyed
//	VOX_GOLD_WON,						//	gold player is victorious
//	VOX_RED_UNIT,						//	red unit destroyed
//	VOX_RED_STRUCTURE,				//	red structure destroyed
//	VOX_RED_ONLINE,					//	red player online
//	VOX_RED_OFFLINE,					//	red player has departed
//	VOX_RED_LOST,						//	red player destroyed
//	VOX_RED_WON,						//	red player is victorious
//	VOX_GREY_UNIT,						//	grey unit destroyed
//	VOX_GREY_STRUCTURE,				//	grey structure destroyed
//	VOX_GREY_ONLINE,					//	grey player online
//	VOX_GREY_OFFLINE,					//	grey player has departed
//	VOX_GREY_LOST,						//	grey player destroyed
//	VOX_GREY_WON,						//	grey player is victorious
//	VOX_ORANGE_UNIT,					//	orange unit destroyed
//	VOX_ORANGE_STRUCTURE,			//	orange structure destroyed
//	VOX_ORANGE_ONLINE,				//	orange player online
//	VOX_ORANGE_OFFLINE,				//	orange player has departed
//	VOX_ORANGE_LOST,					//	orange player destroyed
//	VOX_ORANGE_WON,					//	orange player is victorious
//	VOX_GREEN_UNIT,					//	green unit destroyed
//	VOX_GREEN_STRUCTURE,				//	green structure destroyed
//	VOX_GREEN_ONLINE,					//	green player online
//	VOX_GREEN_OFFLINE,				//	green player has departed
//	VOX_GREEN_LOST,					//	green player destroyed
//	VOX_GREEN_WON,						//	green player is victorious
//	VOX_BLUE_UNIT,						//	blue unit destroyed
//	VOX_BLUE_STRUCTURE,				//	blue structure destroyed
//	VOX_BLUE_ONLINE,					//	blue player online
//	VOX_BLUE_OFFLINE,					//	blue player has departed
//	VOX_BLUE_LOST,						//	blue player destroyed
//	VOX_BLUE_WON,						//	blue player is victorious

	VOX_COUNT,
//	VOX_MULTI_UNIT			= VOX_GOLD_UNIT,
//	VOX_MULTI_STRUCTURE	= VOX_GOLD_STRUCTURE,
//	VOX_MULTI_ONLINE		= VOX_GOLD_ONLINE,
//	VOX_MULTI_OFFLINE		= VOX_GOLD_OFFLINE,
//	VOX_MULTI_LOST			= VOX_GOLD_LOST,
//	VOX_MULTI_WON			= VOX_GOLD_WON,

	VOX_FIRST=0
} VoxType;

#define NUM_MULTI_VOICES	6
/****************************************************************************
**	Game reinforcements are each controlled by the following structure. The
**	data originates in the scenario INI file but is then carried throughout
**	any saved games.
*/
typedef enum SourceType : char {
	SOURCE_NONE=-1,				// No defined source (error condition).
	SOURCE_FIRST=0,

	SOURCE_NORTH=SOURCE_FIRST,	// From north edge.
	SOURCE_EAST,					// From east edge.
	SOURCE_SOUTH,					// From south edge.
	SOURCE_WEST,					// From west edge.
	SOURCE_SHIPPING,				// Shipping lane.
	SOURCE_BEACH,					// Selects beach to land upon.
	SOURCE_AIR,						// Dropped by air (someplace).
	SOURCE_VISIBLE,				// Dropped where player can see.
	SOURCE_ENEMYBASE,				// Dropped at enemy base.
	SOURCE_HOMEBASE,				// Dropped at friendly base.
	SOURCE_OCEAN,					// Enters from ocean map edge.

	SOURCE_COUNT
} SourceType;


/****************************************************************************
**	Each type of terrain has certain characteristics. These are indicated
**	by the structure below. For every element of terrain there is a
**	corresponding GroundType structure.
*/
typedef struct {
	int				Color;					// Radar map (map editor) id color.
	unsigned char	Cost[SPEED_COUNT];	// Terrain effect cost (normal).
	bool				Build;					// Can build on this terrain?
} GroundType;


/**************************************************************************
**	Find_Path returns with a pointer to this structure.
*/
typedef struct {
	CELL				Start;				// Starting cell number.
	int				Cost;					// Accumulated terrain cost.
	int				Length;				// Command string length.
	FacingType		*Command;			// Pointer to command string.
	unsigned long	*Overlap;			// Pointer to overlap list
	CELL				LastOverlap;		// stores position of last overlap
	CELL				LastFixup;			// stores position of last overlap
} PathType;


/**********************************************************************
** These are special indices into the Waypoint array; slots 0-25 are
** reserved for letter-designated Waypoints, the others are special.
*/
typedef enum WaypointEnum : unsigned char
{
	WAYPT_HOME = 26,				// Home-cell for this scenario
	WAYPT_REINF,					// cell where reinforcements arrive
	WAYPT_COUNT,
} WaypointType;


/****************************************************************************
**	Max # of players total, including the game's owner
*/
#define	MAX_PLAYERS			6


/****************************************************************************
**	These are the possible types of multiplayer games supported.
*/
typedef enum GameEnum : unsigned char {
	GAME_NORMAL,			// not multiplayer
	GAME_MODEM,				// modem game
	GAME_NULL_MODEM,		// NULL-modem
	GAME_IPX,				// IPX Network game
	GAME_INTERNET,			// WInsock game
	GAME_GLYPHX_MULTIPLAYER		// Multiplayer game controlled by the GLYPHX engine. ST - 3/12/2019 10:04AM
} GameType;


#define	MPLAYER_BUILD_LEVEL_MAX			7


/****************************************************************************
**	This is the max character length of a player's name
*/
#define	MPLAYER_NAME_MAX			12


/****************************************************************************
**	Max # of colors for multiplayer games
*/
#define	MAX_MPLAYER_COLORS		6


/****************************************************************************
** This defines the various possible communications protocols.
*/
typedef enum CommProtocolEnum : unsigned char {
	COMM_PROTOCOL_SINGLE_NO_COMP = 0,	// single frame with no compression
	COMM_PROTOCOL_SINGLE_E_COMP,			// single frame with event compression
	COMM_PROTOCOL_MULTI_E_COMP,			// multiple frame with event compression
	COMM_PROTOCOL_COUNT,
	DEFAULT_COMM_PROTOCOL = COMM_PROTOCOL_SINGLE_NO_COMP,
} CommProtocolType;


/****************************************************************************
** Min value for MaxAhead, for both net & modem; only applies for
** COMM_PROTOCOL_MULTI_E_COMP.
*/
#define MODEM_MIN_MAX_AHEAD			5
#define NETWORK_MIN_MAX_AHEAD			2


/****************************************************************************
**	Max length of inter-player message buffers.  Messages.Init() should specify
** a value <= this.  For editing messages, the "to" field length is added to
** this length to generate the entire editable message length.  For displayed
** messages, a "From" prefix length should be added to this value to generate
** the entire max displayable message length.
*/
#define	COMPAT_MESSAGE_LENGTH	28
#define	MAX_MESSAGE_SEGMENTS		3
#define	MAX_MESSAGE_LENGTH		COMPAT_MESSAGE_LENGTH*3

/*
** Defines for magic numbers to place in messages to allow concatenation.
*/
#define	MESSAGE_HEAD_MAGIC_NUMBER	0xFACE
#define	MESSAGE_TAIL_MAGIC_MUMBER	0xECAF

/*
** Max # of allowed messages at one time
*/
#define	MAX_NUM_MESSAGES			10

#define	PORTBUF_MAX			64						// Dialog field size
#define	IRQBUF_MAX			3
#define	BAUDBUF_MAX			7
#define	INITSTRBUF_MAX		41
#define	CWAITSTRBUF_MAX	16
#define	CREDITSBUF_MAX		5
#define	MODEM_NAME_MAX		PORTBUF_MAX - 1	// Max length of modem name in list box

typedef enum DetectPortType : unsigned char {
	PORT_VALID = 0,
	PORT_INVALID,
	PORT_IRQ_INUSE
} DetectPortType;

typedef enum DialStatusType : unsigned char {
	DIAL_CONNECTED			= 0,
	DIAL_NO_CARRIER,
	DIAL_BUSY,
	DIAL_ERROR,
	DIAL_NO_DIAL_TONE,
	DIAL_CANCELED
} DialStatusType;

typedef enum DialMethodType : unsigned char {
	DIAL_TOUCH_TONE = 0,
	DIAL_PULSE,

	DIAL_METHODS
} DialMethodType;

typedef enum CallWaitStringType : unsigned char {
	CALL_WAIT_TONE_1 = 0,
	CALL_WAIT_TONE_2,
	CALL_WAIT_PULSE,
	CALL_WAIT_CUSTOM,

	CALL_WAIT_STRINGS_NUM
} CallWaitStringType;


/****************************************************************************
**	This structure defines the settings for the serial port.
*/
typedef struct {
	int Port;
	int IRQ;
	int Baud;
	DialMethodType DialMethod;
	int InitStringIndex;
	int CallWaitStringIndex;
	char CallWaitString[ CWAITSTRBUF_MAX ];
	bool Init;
	bool Compression;
	bool ErrorCorrection;
	bool HardwareFlowControl;
	char ModemName [ MODEM_NAME_MAX ];
} SerialSettingsType;


/****************************************************************************
**	These are the various commands sent during startup of a Serial game.
*/
typedef enum SerialCommandType : unsigned short {
	SERIAL_CONNECT			= 100,	// Are you there?  Hello?  McFly?
	SERIAL_GAME_OPTIONS	= 101,	// Hey, dudes, here's some new game options
	SERIAL_SIGN_OFF		= 102,	// Bogus, dudes, my boss is coming; I'm outta here!
	SERIAL_GO				= 103,	// OK, dudes, jump into the game loop!
	SERIAL_MESSAGE			= 104,	// Here's a message
	SERIAL_TIMING			= 105,	// timimg packet
	SERIAL_SCORE_SCREEN	= 106,	// player at score screen
	SERIAL_READY_TO_GO	= 107,	// Host is ready to start the game
	SERIAL_LAST_COMMAND				// last command
} SerialCommandType;

//
// how much time (ticks) to go by before sending another packet
// to simulate traffic.
//
#define PACKET_TIMING_TIMEOUT		40


/****************************************************************************
**	These is the structure sent over the network Global Channel.
**	Also used for the Null-Modem and Modem.
*/
typedef struct {
	SerialCommandType Command;					// One of the enum's defined above
	char Name[MPLAYER_NAME_MAX];				// Player or Game Name
	int Version;									// game's version number
	HousesType House;								// player's House
	unsigned char Color;							// player's color or SIGNOFF ID
	unsigned char Scenario;						// Scenario #
	unsigned int Credits;						// player's credits
	unsigned int IsBases		: 1;				// 1 = bases are allowed
	unsigned int IsTiberium	: 1;				// 1 = tiberium is allowed
	unsigned int IsGoodies	: 1;				// 1 = goodies are allowed
	unsigned int IsGhosties	: 1;				// 1 = ghosts are allowed
	unsigned char BuildLevel;					// buildable level
	unsigned char UnitCount;					// max # units
	int Seed;										// random number seed
	SpecialClass Special;						// command-line options
	unsigned int GameSpeed;						// Game Speed
	unsigned long ResponseTime;				// packet response time
	char Message[COMPAT_MESSAGE_LENGTH];	// inter-player message
	unsigned char ID;								// ID of sender of message
} SerialPacketType;

typedef enum ModemGameType : unsigned char {
	MODEM_NULL_HOST = 0,
	MODEM_NULL_JOIN,
	MODEM_DIALER,
	MODEM_ANSWERER,
	INTERNET_HOST = MODEM_NULL_HOST,
	INTERNET_JOIN = MODEM_NULL_JOIN
} ModemGameType;


/****************************************************************************
**	This is the max number of events supported on one frame.
*/
#define	MAX_EVENTS			256


/****************************************************************************
**	These are the various commands sent over the network's Global Channel.
*/
typedef enum NetCommandType : unsigned char {
	NET_QUERY_GAME,			// Hey, what games are out there?
	NET_ANSWER_GAME,			// Yo, Here's my game's name!
	NET_QUERY_PLAYER,			// Hey, what players are in this game?
	NET_ANSWER_PLAYER,		// Yo, I'm in that game!
	NET_QUERY_JOIN,			// Hey guys, can I play too?
	NET_CONFIRM_JOIN,			// Well, OK, if you really want to.
	NET_REJECT_JOIN,			// No, you can't join; sorry, dude.
	NET_GAME_OPTIONS,			// Hey, dudes, here's some new game options
	NET_SIGN_OFF,				// Bogus, dudes, my boss is coming; I'm outta here!
	NET_GO,						// OK, dudes, jump into the game loop!
	NET_MESSAGE,				// Here's a message
	NET_PING,					// I'm pinging you to take a time measurement
} NetCommandType;


/****************************************************************************
**	These is the structure sent over the network Global Channel.
*/
typedef struct {
	NetCommandType Command;						// One of the enum's defined above
	char Name[MPLAYER_NAME_MAX];				// Player or Game Name
	union {
		struct {
			int Version;							// game's version number
			unsigned int IsOpen		: 1;		// 1 = game is open for joining
		} GameInfo;
		struct {
			HousesType House;						// player's House
			unsigned int Color;					// player's color
			unsigned long NameCRC;				// CRC of player's game's name
		} PlayerInfo;
		struct {
			unsigned char Scenario;				// Scenario #
			unsigned int Credits;				// player's credits
			unsigned int IsBases		: 1;		// 1 = bases are allowed
			unsigned int IsTiberium	: 1;		// 1 = tiberium is allowed
			unsigned int IsGoodies	: 1;		// 1 = goodies are allowed
			unsigned int IsGhosties	: 1;		// 1 = ghosts are allowed
			unsigned char BuildLevel;			// buildable level
			unsigned char UnitCount;			// max # units
			int Seed;								// random number seed
			SpecialClass Special;				// command-line options
			unsigned int GameSpeed;				// Game Speed
		} ScenarioInfo;
		struct {
			char Buf[COMPAT_MESSAGE_LENGTH];		// inter-user message
			unsigned char ID;						// ID of sender of message
			unsigned long NameCRC;				// CRC of sender's Game Name
		} Message;
		struct {
			int OneWay;								// one-way response time
		} ResponseTime;
	};
} GlobalPacketType;


/****************************************************************************
**	This structure is for keeping score in multiplayer games.
*/
#define	MAX_MULTI_NAMES	8		// max # names (rows) on the score screen
#define	MAX_MULTI_GAMES	4		// max # games (columns) on the score screen

typedef struct {
	char Name[MPLAYER_NAME_MAX];
	int Wins;
	int Kills[MAX_MULTI_GAMES];
	int Color;
} MPlayerScoreType;


typedef enum {
	KF_NUMBER   = 0x08,
	KF_UNCOMP	= 0x10,
	KF_DELTA    = 0x20,
	KF_KEYDELTA = 0x40,
	KF_KEYFRAME = 0x80,
	KF_MASK     = 0xF0
} KeyFrameType;


//--------------------------------------------------------------------
// New Config structure for .CFG files
//--------------------------------------------------------------------
typedef struct {
   unsigned   DigitCard;      	// SoundCardType.
   unsigned   Port;      		 	// SoundCardType.
   unsigned   IRQ;		       	// SoundCardType.
   unsigned   DMA;     		 		// SoundCardType.
   unsigned   BitsPerSample;     // bits per sound sample
   unsigned   Channels;          // stereo/mono sound card
   unsigned   Speed;		         // stereo/mono sound card
	bool       Reverse;				// Reverse left/right speakers
	char       Language[4];
} NewConfigType;


/****************************************************************************
**	These are the types of dialogs that can pop up outside of the main loop,
** an call the game in the background.
*/
typedef enum {
	SDLG_NONE,
	SDLG_OPTIONS,
	SDLG_SURRENDER,
	SDLG_SPECIAL,
} SpecialDialogType;

typedef enum : unsigned char {
	CC_GDI_COLOR		= YELLOW,
	CC_NOD_COLOR		= RED,
	CC_BLUE_GREEN		= CYAN,
	CC_BLUE_GREY		= LTBLUE,
	CC_ORANGE			= PURPLE,
	CC_GREEN				= GREEN,
	CC_TAN				= BROWN,

	CC_GREEN_SHADOW	= 140,
	CC_GREEN_BKGD		= 141,
	CC_GREEN_CORNERS	= CC_GREEN_BKGD,
	CC_LIGHT_GREEN		= 159,
	CC_GREEN_BOX		= CC_LIGHT_GREEN,
	CC_BRIGHT_GREEN	= 167,
	CC_UNDERLINE		= CC_BRIGHT_GREEN,
	CC_GREEN_BAR		= CC_BRIGHT_GREEN,
} CCPaletteType;


/****************************************************************************
**	These specify the shape numbers in the OPTIONS.SHP file. These shapes
**	are used to dress up the dialog boxes. Many of these shapes come in pairs.
**	For dialog box shapes, they are left image / right image paired. For buttons,
**	they are up / down paired.
*/
typedef enum OptionControlType : char {
	OPTION_NONE=-1,			// No fancy shmancy shape.
	OPTION_DIALOG=0,			// Small dialog boxes.
	OPTION_CONTROLS=2,		// Large dialog boxes, game controls.
	OPTION_DELETE=4,			// Delete,Load,Save game.
	OPTION_SERIAL=6,			// Serial dialog.
	OPTION_PHONE=8,			// Phone dialog.
	OPTION_VISUAL=10,			// Visual dialog.
	OPTION_NETWORK=12,		// Network dialog.
	OPTION_JOIN_NETWORK=14,	// Join network dialog.
	OPTION_SOUND=16,			// Sound controls.
	OPTION_SMALL_THUMB=18,	// Small slider thumb.
	OPTION_LARGE_THUMB=19,	//	Large slider thumb.
	OPTION_PLAY=20,			// Play track.
	OPTION_STOP=22,			// Stop track.
	OPTION_UP_ARROW=24,		// Slider up arrow.
	OPTION_DOWN_ARROW=26,	// Slider down arrow.
	OPTION_BUTTON_GDI=28,	// Huh?
	OPTION_BUTTON_NOD=30,	// Huh?

	OPTION_COUNT
} OptionControlType;


#define TOTAL_CRATE_TYPES 15

#define size_of(typ,id) sizeof(((typ*)0)->id)




/*
** 
**  Imports from Red Alert for AI. ST - 7/16/2019 11:44AM
** 
** 
** 
*/
#ifdef USE_RA_AI


/**********************************************************************
**	A base is broken up into several zones. This type enumerates the
**	various zones.
*/
typedef enum ZoneType : char {
	ZONE_CORE,			// Center of base.
	ZONE_NORTH,			// North section.
	ZONE_EAST,			// East section.
	ZONE_SOUTH,			// South section.
	ZONE_WEST,			// West section.

	ZONE_COUNT,
	ZONE_FIRST=0,
	ZONE_NONE=-1
} ZoneType;


/**********************************************************************
**	The map is prescanned to mark of movement zones according to certain
**	movement characteristics. This enum specifies those characteristics
**	and movement zones kept track of.
*/
typedef enum MZoneType : unsigned char {
	MZONE_NORMAL,				// Normal terrestrial objects (can't crush walls).
	MZONE_CRUSHER,				// Can crush crushable wall types.
	MZONE_DESTROYER,			// Can destroy walls.
	MZONE_WATER,				//	Water based objects.

	MZONE_COUNT,
	MZONE_FIRST=0
} MZoneType;

#define	MZONEF_NORMAL		(1<<MZONE_NORMAL)
#define	MZONEF_CRUSHER		(1<<MZONE_CRUSHER)
#define	MZONEF_DESTROYER	(1<<MZONE_DESTROYER)
#define	MZONEF_WATER		(1<<MZONE_WATER)
#define	MZONEF_ALL			(MZONEF_NORMAL|MZONEF_CRUSHER|MZONEF_DESTROYER|MZONEF_WATER)


/**********************************************************************
**	This records the current state of the computer controlled base. The
**	AI will respond according to this state in order to control
**	production and unit orders.
*/
typedef enum StateType : unsigned char {
	STATE_BUILDUP,			// Base is building up (defensive buildup stage).
	STATE_BROKE,			// Low on money, need cash or income source.
	STATE_THREATENED,		// Enemy units are designated to move close by.
	STATE_ATTACKED,		// Base is under direct attack.
	STATE_ENDGAME			//	Resistance is futile.
} StateType;


/**********************************************************************
**	Urgency rating used to determine what action to perform. The greater
**	the urgency the more likely the corresponding action will be chosen.
**	These values are assigned to each potential desired action the house
**	is to perform.
*/
typedef enum UrgencyType : unsigned char {
	URGENCY_NONE,				// No action on this matter is needed or desired.
	URGENCY_LOW,				// Minimal attention requested.
	URGENCY_MEDIUM,			//	Normal attention requested.
	URGENCY_HIGH,				// High attention requested.
	URGENCY_CRITICAL,			// This matter must be addressed immediately.

	URGENCY_COUNT,
	URGENCY_FIRST=URGENCY_CRITICAL
} UrgencyType;


/**********************************************************************
**	These are the various actions a house may perform. These actions refer
**	to global events that encompass selling and production. Low level house
**	specific actions of choosing targets is handled elsewhere.
*/
typedef enum StrategyType : unsigned char {
	STRATEGY_BUILD_POWER,			// Power is low, build more.
	STRATEGY_BUILD_DEFENSE,			// Defense needs boosting.
	STRATEGY_BUILD_INCOME,			// Income is low or in jeopardy, raise it.
	STRATEGY_FIRE_SALE,				// Situation hopeless, sell and attack.
	STRATEGY_BUILD_ENGINEER,		// An engineer is needed.
	STRATEGY_BUILD_OFFENSE,			// Offensive weapons are needed.
	STRATEGY_RAISE_MONEY,			// Money is low, emergency raise cash.
	STRATEGY_RAISE_POWER,			// Money is low, raise power by selling.
	STRATEGY_LOWER_POWER,			// Too much power, sell power plants.
	STRATEGY_ATTACK,					// General charge the enemy attack logic.

	STRATEGY_COUNT,
	STRATEGY_FIRST=STRATEGY_BUILD_POWER
} StrategyType;

#endif //USE_RA_AI



#pragma warning (pop)

#endif