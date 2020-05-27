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

/* $Header: /CounterStrike/RULES.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RULES.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/12/96                                                     *
 *                                                                                             *
 *                  Last Update : May 12, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef RULES_H
#define RULES_H

#include	"ccini.h"

class DifficultyClass
{
	public:
		fixed FirepowerBias;
		fixed GroundspeedBias;
		fixed AirspeedBias;
		fixed ArmorBias;
		fixed ROFBias;
		fixed CostBias;
		fixed BuildSpeedBias;

		fixed RepairDelay;
		fixed BuildDelay;

		unsigned IsBuildSlowdown:1;
		unsigned IsWallDestroyer:1;
		unsigned IsContentScan:1;
};

class RulesClass {
	public:

		RulesClass(void);

		bool Process(CCINIClass & file);
		bool General(CCINIClass & ini);
		bool MPlayer(CCINIClass & ini);
		bool Recharge(CCINIClass & ini);
		bool Heap_Maximums(CCINIClass & ini);
		bool AI(CCINIClass & ini);
		bool Powerups(CCINIClass & ini);
		bool Land_Types(CCINIClass & ini);
		bool Themes(CCINIClass & ini);
		bool IQ(CCINIClass & ini);
		bool Objects(CCINIClass & ini);
		bool Difficulty(CCINIClass & ini);

		/*
		**	This specifies the turbo boost speed for missiles when they are fired upon
		**	aircraft and the weapon is specified as having a turbo boost bonus.
		*/
		fixed TurboBoost;

		/*
		**	This specifies the average number of minutes between each computer attack.
		*/
		fixed AttackInterval;

		/*
		**	This specifies the average minutes delay before the computer will begin
		**	its first attack upon the player. The duration is also modified by the
		**	difficulty level.
		*/
		fixed AttackDelay;

		/*
		**	If the power ratio falls below this percentage, then a power emergency is
		**	in effect. At such times, the computer might decide to sell off some
		**	power hungry buildings in order to alleviate the situation.
		*/
		fixed PowerEmergencyFraction;

		/*
		**	The number of badgers that arrive when the parabomb option is used.
		*/
		int BadgerBombCount;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of airstrips.
		*/
		fixed AirstripRatio;

		/*
		**	Limit the number of airstrips to this amount.
		*/
		int AirstripLimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of helipads.
		*/
		fixed HelipadRatio;

		/*
		**	Limit the number of helipads to this amount.
		*/
		int HelipadLimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of Tesla Coils.
		*/
		fixed TeslaRatio;

		/*
		**	Limit tesla coil production to this maximum.
		*/
		int TeslaLimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of anti-aircraft defense.
		*/
		fixed AARatio;

		/*
		**	Limit anti-aircraft building quantity to this amount.
		*/
		int AALimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of defensive structures.
		*/
		fixed DefenseRatio;

		/*
		**	This is the limit to the number of defensive building that can be built.
		*/
		int DefenseLimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of war factories.
		*/
		fixed WarRatio;

		/*
		**	War factories are limited to this quantity for the computer controlled player.
		*/
		int WarLimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of infantry producing structures.
		*/
		fixed BarracksRatio;

		/*
		**	No more than this many barracks can be built.
		*/
		int BarracksLimit;

		/*
		**	Refinery building is limited to this many refineries.
		*/
		int RefineryLimit;

		/*
		**	This specifies the percentage of the base (by building quantity) that should
		**	be composed of refineries.
		*/
		fixed RefineryRatio;

		/*
		**	The computer is limited in the size of the base it can build. It is limited to the
		**	size of the largest human opponent base plus this surplus count.
		*/
		int BaseSizeAdd;

		/*
		**	If the power surplus is less than this amount, then the computer will
		**	build power plants.
		*/
		int PowerSurplus;

		/*
		**	The computer will build infantry if their cash reserve is greater than this amount.
		*/
		int InfantryReserve;

		/*
		**	This factor is multiplied by the number of buildings in the computer's base and infantry
		**	are always built until it matches that number.
		*/
		int InfantryBaseMult;

		/*
		**	This specifies the duration that a unit will remain chronoshifted before it
		**	will be returned to its starting location.
		*/
		fixed ChronoDuration;

		/*
		**	Percent chance that a water crate will be generated instead of a land
		**	crate when crates are on and in a multiplay game.
		*/
		fixed WaterCrateChance;

		/*
		**	Solo play has money crate amount fixed according to this rule value.
		*/
		int SoloCrateMoney;

		/*
		**	GPS tech level control.
		*/
		int GPSTechLevel;

		/*
		**	If a unit type is specified here, then the unit crate will generate
		**	a unit of this type (always).
		*/
		UnitType UnitCrateType;

		/*
		**	This is the time to delay between patrol-to-waypoint target scanning.
		*/
		fixed PatrolTime;

		/*
		**	This is the time interval that checking to create teams will span. The
		**	smaller this number, the more often checking for team creation will occur.
		*/
		fixed TeamDelay;

		/*
		**	This is the arbitrary delay to make all cloaking objects remain uncloaked
		**	before having it recloak.
		*/
		fixed CloakDelay;

		/*
		**	This is an overall game apparent speed bias to use for object
		**	movement purposes.
		*/
		fixed GameSpeedBias;

		/*
		**	If a potential target is close to the base then increase
		**	the likelyhood of attacking it by this bias factor.
		*/
		fixed NervousBias;

		/*
		**	Controls the Chronal vortex characteristics.
		*/
		LEPTON VortexRange;
		MPHType VortexSpeed;
		int VortexDamage;
		fixed VortexChance;

		/*
		**	When an explosive object explodes, the damage will spread out
		**	by this factor. The value represents the number of cells radius
		**	that the damage will spread for every 100 points of raw damage at
		**	the explosion center point.
		*/
		fixed ExplosionSpread;

		/*
		**	For weapons specially marked to check for nearby friendly buildings
		**	when scanning for good targets, this indicates the scan radius. Such
		**	weapons will supress firing on enemies if they are in close proximity
		**	to allied buildings.
		*/
		LEPTON SupressRadius;

		/*
		**	This is the tech level that para infantry are granted free to the owner
		**	of an airstrip.
		*/
		int ParaInfantryTechLevel;

		/*
		**	This is the tech level that spy planes are granted free to the owner of
		**	an airstrip.
		*/
		int SpyPlaneTechLevel;

		/*
		**	This is the tech level that the parabombs are granted free to the owner
		**	of an airstrip.
		*/
		int ParaBombTechLevel;

		/*
		**	This is the maximum number of IQ settings available. The human player is
		**	presumed to be at IQ level zero.
		*/
		int MaxIQ;

		/*
		**	The IQ level at which super weapons will be automatically fired by the computer.
		*/
		int IQSuperWeapons;

		/*
		**	The IQ level at which production is automatically controlled by the computer.
		*/
		int IQProduction;

		/*
		**	The IQ level at which newly produced units start out in guard area mode instead
		**	of normal guard mode.
		*/
		int IQGuardArea;

		/*
		**	The IQ level at which the computer will be able to decide what gets repaired
		**	or sold.
		*/
		int IQRepairSell;

		/*
		**	At this IQ level or higher, a unit is allowed to automatically try to crush
		**	an atagonist if possible.
		*/
		int IQCrush;

		/*
		**	The unit/infantry will try to scatter if an incoming threat
		**	is detected.
		*/
		int IQScatter;

		/*
		**	Tech level at which the computer will scan the contents of a transport
		**	in order to pick the best target to fire upon.
		*/
		int IQContentScan;

		/*
		**	Aircraft replacement production occurs at this IQ level or higher.
		*/
		int IQAircraft;

		/*
		**	Checks for and replaces lost harvesters.
		*/
		int IQHarvester;

		/*
		**	Is allowed to sell a structure being damaged.
		*/
		int IQSellBack;

		/*
		**	The silver and wood crates in solo play will have these powerups.
		*/
		CrateType SilverCrate;
		CrateType WoodCrate;
		CrateType WaterCrate;

		/*
		**	This specifies the minimum number of crates to place on the map in spite
		**	of the number of actual human players.
		*/
		int CrateMinimum;

		/*
		**	This specifies the crate maximum quantity to use.
		*/
		int CrateMaximum;

		/*
		**	Landing zone maximum alternate zone scan radius.
		*/
		LEPTON LZScanRadius;

		/*
		**	Multiplayer default settings.
		*/
		int MPDefaultMoney;
		int MPMaxMoney;
		unsigned IsMPShadowGrow:1;
		unsigned IsMPBasesOn:1;
		unsigned IsMPTiberiumGrow:1;
		unsigned IsMPCrates:1;
		unsigned IsMPAIPlayers:1;
		unsigned IsMPCaptureTheFlag:1;

		/*
		**	Drop zone reveal radius.
		*/
		LEPTON DropZoneRadius;

		/*
		**	This is the delay that multiplayer messages will remain on the screen.
		*/
		fixed MessageDelay;

		/*
		**	Savour delay between when scenario detects end and the actual
		**	end of the play.
		*/
		fixed SavourDelay;

		/*
		**	This specifies the damage to inflict for two differnt styles of
		**	land mine.
		*/
		int AVMineDamage;
		int APMineDamage;

		/*
		**	This is the maximum number of multiplayers allowed.
		*/
		int MaxPlayers;

		/*
		**	This is the delay between 'panic attacks' when the computer's base is under
		**	attack. This delay gives the previously assigned units a chance to affect the
		**	attacker before the computer sends more.
		*/
		fixed BaseDefenseDelay;

		/*
		**	These values control the team suspension logic for dealing with immedate base threats.
		**	When the base is attacked, all teams with less than the specified priority will be
		**	temporarily put on hold for the number of minutes specified.
		*/
		int SuspendPriority;
		fixed SuspendDelay;

		/*
		**	This serves as the fraction of a building's original cost that is converted
		**	into survivors (of some fashion). There are rounding and other marginal
		**	fudge effects, but this value is the greatest control over the survivor rate.
		*/
		fixed SurvivorFraction;

		/*
		**	This is the aircraft reload rate expressed in minutes per ammo load.
		*/
		fixed ReloadRate;

		/*
		**	The average time (in minutes) between the computer autocreating a team
		**	from the team's autocreate list.
		*/
		fixed AutocreateTime;

		/*
		**	Build up time for buildings (minutes).
		*/
		fixed BuildupTime;

		/*
		**	Ore truck speed for dumping.
		*/
		int OreDumpRate;

		/*
		**	This is the amount of damage done by the atom bomb in solo missions. The
		**	damage done during multiplay will be 1/5th this value.
		*/
		int AtomDamage;

		/*
		**	This array controls the difficulty affects on the game. There is one
		**	difficulty class object for each difficulty level.
		*/
		DifficultyClass Diff[DIFF_COUNT];

		/*
		**	Is the computer paranoid? If so, then it will band together with other computer
		**	paranoid players when the situation looks rough.
		*/
		bool IsComputerParanoid:1;

		/*
		**	Should helicopters shuffle their position between firing on their
		**	target?
		*/
		bool IsCurleyShuffle:1;

		/*
		**	Flash the power bar when the power goes below 100%.
		*/
		bool IsFlashLowPower:1;

		/*
		**	If the computer players will go to easy mode if there is more
		**	than one human player, this flag will be true.
		*/
		bool IsCompEasyBonus:1;

		/*
		**	If fine control of difficulty settings is desired, then set this value to true.
		**	Fine control allows 5 settings. The coarse control only allows three settings.
		*/
		bool IsFineDifficulty:1;

		/*
		**	If the harvester is to explode more violently than normal
		**	if it is carrying cargo, then this flag will be true.
		*/
		unsigned IsExplosiveHarvester:1;

		/*
		**	If this flag is true, then the construction yard can undeploy back into an MCV.
		*/
		unsigned IsMCVDeploy:1;

		/*
		**	If the base is to be revealed to a new ally, then this
		**	flag will be true.
		*/
		unsigned IsAllyReveal:1;

		/*
		**	Can the helipad (and airfield) be purchased separately from the associated
		**	aircraft.
		*/
		unsigned IsSeparate:1;

		/*
		**	Give target cursor for trees? Doing this will make targetting of trees easier.
		*/
		unsigned IsTreeTarget:1;

		/*
		**	Are friendly units automatically aware of mines so that they can avoid them?
		*/
		unsigned IsMineAware:1;

		/*
		**	If Tiberium is allowed to grow, then this flag will be true.
		*/
		unsigned IsTGrowth:1;

		/*
		**	If Tiberium is allowed to spread, then this flag will be true.
		*/
		unsigned IsTSpread:1;

		/*
		**	Should civilan buildings and civilians display their true name rather than
		**	the generic "Civilian Building" and "Civilain"?
		*/
		unsigned IsNamed:1;

		/*
		**	Should player controlled vehicles automatically try to crush nearby infantry
		**	instead of required the player to manually direct them to crush.
		*/
		unsigned IsAutoCrush:1;

		/*
		**	Should the player controlled buildings and units automatically return fire when
		**	fired upon?
		*/
		unsigned IsSmartDefense:1;

		/*
		**	Should player controlled units try to scatter more easily in order to
		**	avoid damage or threats?
		*/
		unsigned IsScatter:1;

		/*
		**	If the chronoshift effect should kill all cargo, then this flag will
		**	be set to true.
		*/
		unsigned IsChronoKill:1;

		/*
		**	When infantry are prone or when civilians are running around like crazy,
		**	they are less prone to damage. This specifies the multiplier to the damage
		**	(as a fixed point number).
		*/
		fixed ProneDamageBias;

		/*
		**	The time quake will do this percentage of damage to all units and buildings
		**	in the game. The number is expressed as a fixed point percentage.
		*/
		fixed QuakeDamagePercent;

		/*
		**	Percentage chance that a time quake will occur with each chronoshift use.
		*/
		fixed QuakeChance;

		/*
		**	Ore (Tiberium) growth rate. The value is the number of minutes between
		**	growth steps.
		*/
		fixed GrowthRate;

		/*
		**	This specifies the number of minutes between each shroud regrowth process.
		*/
		fixed ShroudRate;

		/*
		**	This is the average minutes between each generation of a random crate
		**	to be placed on the map if generating of random crates is indicated.
		*/
		fixed CrateTime;

		/*
		**	This specifies the number of minutes remaining before that if the mission timer
		**	gets to this level or below, it will be displayed in red.
		*/
		fixed TimerWarning;

		/*
		**	This specifies the minutes of delay between recharges for these
		**	special weapon types.
		*/
		fixed SonarTime;
		fixed ChronoTime;
		fixed ParaBombTime;
		fixed ParaInfantryTime;
		fixed ParaSaboteurTime;
		fixed SpyTime;
		fixed IronCurtainTime;
		fixed GPSTime;
		fixed NukeTime;

		/*
		**	Other miscellaneous delay times.
		*/
		fixed SpeakDelay;
		fixed DamageDelay;

		/*
		**	This is the gravity constant used to control the arcing and descent of ballistic
		**	object such as grenades and artillery.
		*/
		int Gravity;

		/*
		**	Gap generators have a shroud radius of this many cells.
		*/
		int GapShroudRadius;

		/*
		**	This is the minute interval between the gap generators refreshing
		**	their zones of gapping.
		*/
		fixed GapRegenInterval;

		/*
		**	Mobile radar jammer radius of effect.
		*/
		LEPTON RadarJamRadius;

		/*
		**	The speed at which a projectile that travels at or slower will cause
		**	objects in the target location to scatter. This simulates the ability
		**	of targets to run for cover if the projectile gives them enough time
		**	to react.
		*/
		MPHType Incoming;

		/*
		**	Minimum and maximum damage allowed per shot.
		*/
		int MinDamage;
		int MaxDamage;

		/*
		**	This is the rate of repair for units and buildings. The rate is the
		**	number of strength points repaired per repair clock tick. The cost of
		**	repair is the (fixed point) fractional cost to repair the object based
		**	on the full price of the object. Example; a value of 50% means that to
		**	repair the object from 1 damage point to full strength would cost 50% of
		**	the cost to build it from scratch.
		*/
		int RepairStep;
		fixed RepairPercent;
		int URepairStep;
		fixed URepairPercent;

		/*
		**	This is the rate that objects with self healing will heal. They will repair a bit
		**	every 'this' number of minutes.
		*/
		fixed RepairRate;

		/*
		**	These fixed point values are used to determine the status (health bar
		**	color) of the game objects. Objects in the 'yellow' are in a cautionary
		**	state. Object in the 'red' are in a danger state.
		*/
		fixed ConditionGreen;
		fixed ConditionYellow;
		fixed ConditionRed;

		/*
		**	Average number of minutes between infantry random idle animations.
		*/
		fixed RandomAnimateTime;

		/*
		**	These control the capacity and value of the ore types that a harvester
		**	may carry. The harvester carries a maximum discrete number of 'bails'.
		**	The value of each bail depends on the ore it is composed of.
		*/
		int BailCount;		// was STEP_COUNT
		int GoldValue;		// was GOLD_WORTH
		int GemValue;		// was GEM_WORTH

		/*
		**	This specifies the heap maximum for the various game objects.
		*/
		int AircraftMax;
		int AnimMax;
		int BuildingMax;
		int BulletMax;
		int FactoryMax;
		int InfantryMax;
		int OverlayMax;
		int SmudgeMax;
		int TeamMax;
		int TeamTypeMax;
		int TemplateMax;
		int TerrainMax;
		int TriggerMax;
		int UnitMax;
		int VesselMax;
		int ProjectileMax;
		int WeaponMax;
		int WarheadMax;
		int TrigTypeMax;

		/*
		**	Close enough distance that is used to determine if the object should
		**	stop movement when blocked. If the distance to the desired destination
		**	is equal to this distance or less, but the path is blocked, then consider
		**	the object to have gotten "close enough" to the destination to stop.
		*/
		LEPTON CloseEnoughDistance;

		/*
		**	Stray distance to group team members within. The larger the distance,
		**	the looser the teams will move.
		*/
		LEPTON StrayDistance;

		/*
		**	If a vehicle is closer than this range to a target that it can crush
		**	by driving over it, then it will try to drive over it instead of firing
		**	upon it. The larger the value, the greater the 'bigfoot crush syndrome' is
		**	has.
		*/
		LEPTON CrushDistance;

		/*
		**	For area effect crate bonus items will affect all objects within this radius.
		*/
		LEPTON CrateRadius;

		/*
		**	Maximum scatter distances for homing and non-homing projectiles.
		*/
		LEPTON HomingScatter;
		LEPTON BallisticScatter;

		/*
		**	This is the refund percentage when selling off buildings and units
		**	on the repair pad (service depot).
		*/
		fixed RefundPercent;

		/*
		**	The Iron Curtain invulnerability effect lasts for this many minutes.
		*/
		fixed IronCurtainDuration;

		/*
		**	The strength of bridges is held here. By corollary, the strength of the
		**	demolition charge carried by Tanya is equal to this value as well.
		*/
		int BridgeStrength;

		/*
		**	This is the overall build speed bias. Multiply this value by the normal build
		**	delay to get the effective build delay.
		*/
		fixed BuildSpeedBias;

		/*
		**	Weapon type array pointer should go here. Dynamic type.
		*/


		/*
		**	Warhead type class array pointer should go here. Dynamic type.
		*/


		/*
		**	Ground type and speed affect data should go here.
		*/


		/*
		**	This is the delay between the time a C4 bomb is planted and the time it will
		**	explode. The longer the delay, the greater safety margin for a demolitioner
		**	type. The short the delay, the less time the victim has to sell the building
		**	off.
		*/
		fixed C4Delay;

		/*
		**	The computer will only repair a structure if it has spare money greater than this
		**	amount. The thinking is that this will prevent the computer from frittering away
		**	all it's cash on repairing and thus leaving nothing for production of defenses.
		*/
		int RepairThreshhold;

		/*
		**	This is the delay (in minutes) between retries of a failed path. The longer the
		**	delay the faster the system, but the longer the units take to react to a blocked
		**	terrain event.
		*/
		fixed PathDelay;

		/*
		**	This is the special (debug version only) movie recorder timeout value. Each second
		**	results in about 2-3 megabytes.
		*/
		fixed MovieTime;

		/*
		** This is the level at or above which the chronosphere facility can
		** actually produce the chronosphere effect.  Below this tech level,
		** the facility is merely a showpiece and has no effect.
		*/
		int ChronoTechLevel;

		/*
		**	These are the Tiberium scan distances. The short range scan is used to determine if the
		**	current field has been exhausted. The long range scan is used when finding a Tiberium
		**	field to harvest. Keep these ranges as small as possible.
		*/
		LEPTON TiberiumShortScan;
		LEPTON TiberiumLongScan;

		/*
		**	Health bar display mode
		*/
		enum eHealthBarDisplayMode
		{
			HB_DAMAGED = 0,
			HB_ALWAYS,
			HB_SELECTED
		} HealthBarDisplayMode;

		/*
		**	Resource bar display mode
		*/
		enum eResourceBarDisplayMode
		{
			RB_SELECTED = 0,
			RB_ALWAYS,
		} ResourceBarDisplayMode;
};


#endif